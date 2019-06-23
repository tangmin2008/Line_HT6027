#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"

//#include "interrupt.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"

//#include "RN8213.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
//#include "Esamcpu.h"
//#include "AESGCM.H"
#include "aes_example.h"

void Creat_config_program_record( void )
{
	unsigned long Value;
	
	E2P_RData( (unsigned char*)&Value, No_of_config_program, 4 );	
	Value += 1;
	E2P_WData( No_of_config_program, (unsigned char*)&Value, 4 );	
	E2P_WData( Date_of_last_config_program, &Clk.YearH, 12 );			
}	

unsigned short CreatExceptionResponseAPDU( unsigned char* Dest )
{
	*Dest = APDU_EXCEPTION_response;
	*(Dest+1) = service_error;
	*(Dest+2) = service_not_supported;	
	return 3;
}	

short IsUIFrame( short COM_No )
{
  	unsigned char* ComBuf;
  	unsigned char ControlByte;
  	unsigned char ClientAddr;
	
  	ComBuf = CommPara[COM_No].Buf;
        
    ClientAddr = *(ComBuf+HDLC_RClientAddr);
  	
		ControlByte = *(ComBuf+HDLC_CtlPtr);
	
		if(ClientAddr==0x53) return 0x53; 				//GPRSģ����������ض��ͻ��˵�ַsky20160106..
			
		if(( ControlByte == HDLC_UI )||( ControlByte == HDLC_UI_N )) return 0;
		else return -1;
}	

unsigned short APPLProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short i;
	unsigned char Buff[10];
	unsigned char *Ptr;
	short Len = 0;
	
	Ptr = Buff;
	
	switch( *RecBuff )
	{
	    case APDU_AARQ:   //�ֵͼ����߼�����..
	    			crypt.aarqHLS[COM_No] = 0;
	    		  Len = AARQHProcess( COM_No, RecBuff, SendBuff );
//						Len = AARQProcess( COM_No, RecBuff, SendBuff );				
						if( Len == -1 )
						{
							Len = CreatExceptionResponseAPDU( SendBuff );				
						}	
						break;
	    case RLRQ_APDU:			
					Len = RLRQProcess( COM_No, RecBuff, SendBuff );				
					if( Len == -1 )
					{
						Len = CreatExceptionResponseAPDU( SendBuff );				
					}	
					break;			
			case APDU_GET_Request:					
				//	if(( IsUIFrame( COM_No ) != 0 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )))		//GET�����Ƿ�֧��	//UIֱ֡ͨ
//				if(( IsUIFrame( COM_No ) != 0x53 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )))		//GET�����Ƿ�֧��	//0x53ֱͨ..
//				if(( IsUIFrame( COM_No ) != 0x53 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||(((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )&&(HComm.ClientAddr[COM_No]!=0x10)&&(*CommPara[COM_No].APPLState == APPLON ))))		//GET�����Ƿ�֧��	//0x53ֱͨ..
			//	if(( IsUIFrame( COM_No ) != 0x53 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )))		//GET�����Ƿ�֧��	//0x53ֱͨ..
			if(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 ))
					{
						Len = CreatExceptionResponseAPDU( SendBuff );				
					}	
					else 
					{
						//����Ƿ������..
						/* Ptr = RecBuff+3;
						
						if(( *(RecBuff+1)== get_request_normal)&&((PIN_SETEN & P_SETEN ) != 0 )){ 
								if((HComm.ClientAddr[COM_No]==0x66)&&(GetRPR_clientNO( Ptr )==0xffff))
								{							  
										Len = CreatExceptionResponseAPDU( SendBuff );			
								}else
								{
									//����Ƿ������..
									//Ptr = RecBuff+3;
									if(HComm.ClientAddr[COM_No]==0x10){
											i = GetRPublic_clientNO( Ptr );										
									}
									if(i==0xffff){
										 Len = CreatExceptionResponseAPDU( SendBuff );
										 break;
									}else{
												if(HComm.ClientAddr[COM_No]==0x02)
												{
														i = GetRReading_clientNO( Ptr );										
												}
												if(i==0xffff)
													Len = CreatExceptionResponseAPDU( SendBuff );	
												else 	
													Len = GETProcess( COM_No, RecBuff, SendBuff );
											}
									}
								}
						else
							{ 
								 Len = GETProcess( COM_No, RecBuff, SendBuff );
							}
							*/
							 Len = GETProcess( COM_No, RecBuff, SendBuff );
					}	
					break;
				case APDU_SET_Request:					
		//			if( *CommPara[COM_No].APPLState == APPLIdle )
					//	if(HComm.ClientAddr[COM_No]==0x10)..
					if((HComm.ClientAddr[COM_No]==0x10)||(HComm.ClientAddr[COM_No]==0x02)||( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x08 ) == 0 ))		//SET�����Ƿ�֧��
					{
						Len = CreatExceptionResponseAPDU( SendBuff);				
					}	
					else 
					{
							if(( *(RecBuff+1)==set_request_normal))
						///	if(( *(RecBuff+1)==set_request_normal)&&((PIN_SETEN & P_SETEN ) != 0 ))
							{   
								//����Ƿ�����д..
							/*	Ptr = RecBuff+3;
								if(HComm.ClientAddr[COM_No]==0x66){
									i = GetWPR_clientNO( Ptr );										
								}
								if(i==0xffff){
									 Len = CreatExceptionResponseAPDU( SendBuff );
									 break;
								}
								*/
								Len = SETProcess( COM_No, RecBuff, SendBuff );
								if( *(SendBuff+3) == success ) Creat_config_program_record();
						}else{
								Len = SETProcess( COM_No, RecBuff, SendBuff );
								if( *(SendBuff+3) == success ) Creat_config_program_record();
						}	
						
					///	Len = SETProcess( COM_No, RecBuff, SendBuff );
					///			if( *(SendBuff+3) == success ) Creat_config_program_record();
		//				Len = CreatExceptionResponseAPDU( SendBuff );				
					}	
					break;
			 case APDU_EVENT_NOTIFICATION_Request:
				///	EVENT_NOTIFICATIONProcess( COM_No, RecBuff, SendBuff );					
					break;	
			 case APDU_ACTION_Request: 				
		//			if( *CommPara[COM_No].APPLState == APPLIdle )
					//15���ȡ�б������Ȩ�޾�֧��..
					if((*(RecBuff+1)==action_request_normal)&&(*(RecBuff+3)==0)&&(*(RecBuff+4)==0x0F))	
					{
							Len = ACTIONProcess( COM_No, RecBuff, SendBuff );	
							//return Len;
					} // sky20150309..
					else
					{
								if((HComm.ClientAddr[COM_No]==0x10)||(HComm.ClientAddr[COM_No]==0x02)||( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x01 ) == 0 ))		//ACTION�����Ƿ�֧��
								{
									Len = CreatExceptionResponseAPDU( SendBuff);				
								}	
								else 
								{			if(( *(RecBuff+1)==action_request_normal))
										///	if(( *(RecBuff+1)==action_request_normal)&&((PIN_SETEN & P_SETEN ) != 0 ))
												{ 	
													//����Ƿ�����Action..
												/*	Ptr = RecBuff+3;
													if(HComm.ClientAddr[COM_No]==0x66){
															i = GetAPR_clientNO( Ptr );										
													}
													if(i==0xffff){
														 Len = CreatExceptionResponseAPDU( SendBuff );
														 break;
													}		
													*/
													Len = ACTIONProcess( COM_No, RecBuff, SendBuff );	
										  }else Len = ACTIONProcess( COM_No, RecBuff, SendBuff );					
								}
					}	
					break;
				case GLO_GET_Request:

					break;
				case GLO_SET_Request:
				
					break;
				case GLO_EVENT_NOTIFICATION_Request:
				
					break;
				case GLO_ACTION_Request:
				
					break;
				case General_SIGNING:
				
					break;
					
		//		case APDU_AARE;							
		//		case APDU_GET_Response;   				
		//		case APDU_SET_Response;   				
		//		case APDU_ACTION_Response;
				default: 
					Len = CreatExceptionResponseAPDU( SendBuff );				
					break;								
	}	
	
	return Len;
}

//ȡBIT STRING�������BITλ�Ƿ�Ϊ1
unsigned char GetSTRINGLowestBIT( unsigned char* Data, short Len )
{
	unsigned char BitCt;
	unsigned char LowestByte;
	
	if( Len == 0 ) return 0;
	
//	BitCt = *Data;
	BitCt = 7;						//Test
		
//	LowestByte = *(Data+Len-1);
	LowestByte = *(Data+Len+1);
	
	LowestByte = LowestByte >> BitCt;
	if( LowestByte & 0x01 ) return 1;
	else return 0;
	
}	

void FillApplicationContextName( unsigned char* Dest )
{
	*Dest = 0xA1;
	*(Dest+1) = 0x09;
	*(Dest+2) = 0x06;
	*(Dest+3) = 0x07;
	*(Dest+4) = 0x60;
	*(Dest+5) = 0x85;
	*(Dest+6) = 0x74;
	*(Dest+7) = 0x05;
	*(Dest+8) = 0x08;
	*(Dest+9) = 0x01;
	*(Dest+10) = 0x01;
}	

short CompareApplicationContextName( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	ICEC.use_HLSname = 0;
	FillApplicationContextName( Ptr );
	
	if( Data_Comp( Source, Ptr, 10 ) != 0 ) return 1;
	if(*(Source+10)==0x03)
		{
			ICEC.use_HLSname = 3;
			 //��������֤name..
			 return 0;;
			}
//	if( Data_Comp( Source, Ptr, 11 ) != 0 ) return 1;
//	else return -1;
	if(*(Source+10)==0x01)
		{
			ICEC.use_HLSname = 1;
			//�޼���..
			 return 0;;
			}
	else return 1;
	
}	

void FillMechanism_name( unsigned char* Dest )
{
	*Dest = 0x8B;
	*(Dest+1) = 0x07;
	*(Dest+2) = 0x60;
	*(Dest+3) = 0x85;
	*(Dest+4) = 0x74;
	*(Dest+5) = 0x05;
	*(Dest+6) = 0x08;
	*(Dest+7) = 0x02;
	*(Dest+8) = 0x01;
}	

short CompareMechanism_name( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	FillMechanism_name( Ptr );
	
	crypt.choice_algorithm = 0;
	
	if( Data_Comp( Source, Ptr, 8 ) != 0 ) return 1;
	
	ICEC.use_SHA256 = 0x0;
	crypt.choice_algorithm = *(Source+8);		
	//01--LLS; 02--HLS(ESAMӲ����);	05--HLS(�����)	
	if(*(Source+8)==0x01) return 0;
	if((*(Source+8)==0x02)||(*(Source+8)==0x05)) return 0x55;
	if(*(Source+8)==0x04)
	{
		 ICEC.use_SHA256 = 0x1;
		 return 0x55;	
	}		
	if(*(Source+8)!=0x05) return 1;
	// crypt.aarqHLS = 0x55;
	else return 0x55;
			
//	if( Data_Comp( Source, Ptr, 9 ) != 0 ) return 1;
//	else return 0;
}	

//��ʼ������ͳһ��Ϊ123456
short CompareAuthentication_value( short COM_No,unsigned char* Source )
{
	unsigned char Buff[10];
	unsigned char Buff2[10];
	unsigned char Buff3[10];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char* password;
	short Len;

	Ptr = Buff;
	Point = Buff2;
	password = Buff3;
	
	Len = *(Source+1);
	if( Len > 10 ) return 1;
	if( *(Source+2) != 0x80 ) return 1;
	if( *(Source+3) != ( Len - 2 ) ) return 1;
	Len -= 2;
	
	RAM_Fill( Point, 10 );
	RAM_Write( Point, Source+4, Len );
///	#if ( IranDemo == YesCheck )
///		RAM_Write( Ptr, Point, 8 );	
///	#else
	//	if((PIN_SETEN & P_SETEN ) == 0 )			//�������ߣ��ڲ�ʹ��..
	//	{
	//		RAM_Write( Ptr, Point, 8 );	
	//	}	
	//	else
	//	{
			//��ʱ���ӳ������룺88935860
			*password = '8';
			*(password+1) = '8';
			*(password+2) = '9';
			*(password+3) = '3';
			*(password+4) = '5';
			*(password+5) = '8';
			*(password+6) = '6';
			*(password+7) = '0';
			if( Data_Comp( Point, password, 8 ) == 0 ) return 0;		//�������룬��ʱ����..
			// E2P_RData( Ptr, ELLS_secret1, 8 );
	//		if((HComm.ClientAddr[COM_No]==0x10)||(HComm.ClientAddr[COM_No]==0x11)||(HComm.ClientAddr[COM_No]==0x13)||(HComm.ClientAddr[COM_No]==0x01))
			//if((HComm.ClientAddr[COM_No]==0x01)||((HComm.ClientAddr[COM_No]==0x11)&&(COM_No==0)))
			if((HComm.ClientAddr[COM_No]==0x01)||(HComm.ClientAddr[COM_No]==0x11))
			{
				E2P_RData( Ptr, ELLS_secret2, 8 );
			}else{
				if(HComm.ClientAddr[COM_No]==0x13)
				{
				
				E2P_RData( Ptr, ELLS_secret4, 8 );
					//	 *Ptr = '1';
					//	*(Ptr+1) = '2';
					//	*(Ptr+2) = '3';
					//	*(Ptr+3) = '4';
					//	*(Ptr+4) = '5';
					//	*(Ptr+5) = '6';
					//	*(Ptr+6) = 0;
					//	*(Ptr+7) = 0;
				}else{
						if(HComm.ClientAddr[COM_No]==0x12)
							{
							E2P_RData( Ptr, ELLS_secret3, 8 );
							//		 *Ptr = '1';
							//		*(Ptr+1) = '2';
							//		*(Ptr+2) = '3';
							//		*(Ptr+3) = '4';
							//		*(Ptr+4) = '5';
							//		*(Ptr+5) = '6';
							//		*(Ptr+6) = '7';
							//		*(Ptr+7) = '8';
							}else{
										if(HComm.ClientAddr[COM_No]==0x10)
										{
										return 0;
										/*
												*Ptr = '1';
												*(Ptr+1) = '2';
												*(Ptr+2) = '3';
												*(Ptr+3) = '4';
												*(Ptr+4) = '5';
												*(Ptr+5) = '6';
												*(Ptr+6) = 0;
												*(Ptr+7) = 0;
											*/	
											}
										
									}
							} 
			}		
	//	}	
///	#endif
	if( Data_Comp( Point, Ptr, 8 ) != 0 )
		{
		E2P_RData((unsigned char*)&ICComm.SHEZHIErrTime,Wrong_password_times_disable_Com,1);
		
			if((COM_No==1)||(COM_No==0))
			{
				ICComm.CommErrTime[COM_No]++;
					if(COM_No==1)
					{//����1 2016-8-17-fire �����
						E2P_WData( COMMERRTIMES1, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME1,(unsigned char*)&Clk.YearH,12 );
					}
					else
					{
						//����0 2016-8-17-fire 485��
						E2P_WData( COMMERRTIMES2, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME2, (unsigned char*)&Clk.YearH,12 );
					}
					if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)
						{
							E2P_RData((unsigned char*)&Para.Comm_LockTime,Disable_Com_Duration,2);
							if(Para.Comm_LockTime==0xFFFF)
								Para.Comm_LockTime = 0;
						}
			}
		 return 1;
		}
	else
		{
			if((COM_No==1)||(COM_No==0))
			{
				if(ICComm.CommErrTime[COM_No]!=0)
				{
					ICComm.CommErrTime[COM_No] = 0;
					if(COM_No==1)
					{	//����1 2016-8-17-fire �����
						E2P_WData( COMMERRTIMES1, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						//if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME1,(unsigned char*)&Clk.YearH,12 );
					}
					else
					{	//����0 2016-8-17-fire 485��
						E2P_WData( COMMERRTIMES2, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						//if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME2, (unsigned char*)&Clk.YearH,12 );
					}
				}
			}
		 return 0;
		}
	
}

//short GetComformanceState( short COM_No, unsigned char* Source )
short GetComformanceState( short COM_No, unsigned char* Source, unsigned char SecretState )

{
	unsigned char Buff[3];
	unsigned char* Ptr;
	unsigned char* Conformance;
	
	Ptr = Buff;
	
	if( SecretState == 0x00 )					//�������֤
	{
		Buff[0] = 0x00;
//		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11��BIT12
		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)];		//BIT11��BIT12
//		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ByteBit[7-(Conformance_action%8)];		//BIT19��20��21��23
		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_selective_access%8)];		//BIT19��20��21��23
	}
	else										//LLS��Ͱ�ȫ�����֤
	{
		if((HComm.ClientAddr[COM_No]==0x02)||(HComm.ClientAddr[COM_No]==0x10))
		{
				Buff[0] = 0x00;
				Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11
				Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)];		//BIT19��20��21
		}
	  else
	  {
				Buff[0] = 0x00;
				Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11
				Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ ByteBit[7-(Conformance_action%8)];		//BIT19��20��21��23
		}
	}	
	*Source &= *Ptr;
	*(Source+1) &= *(Ptr+1);
	*(Source+2) &= *(Ptr+2);

	Conformance = CommPara[COM_No].Conformance;
	*(Conformance+3) = SecretState;
	
//	if( IsAllData( Ptr, 3, 0x00 ) == 0 )
	if( IsAllData( Source, 3, 0x00 ) == 0 )
	{
//		RAM_Write( Conformance, Ptr, 3 );
		RAM_Write( Conformance, Source, 3 );
		return 1;
	}
	else
	{	
		RAM_Write( Conformance, Source, 3 );
		return 0;	
	}	
}
#if 0
{
	unsigned char Buff[3];
	unsigned char* Ptr;
	unsigned char* Conformance;
	
	Ptr = Buff;

	if( SecretState == 0x00 )					//�������֤
	{
		Buff[0] = 0x00;
//		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11��BIT12
		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)];		//BIT11��BIT12
//		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ByteBit[7-(Conformance_action%8)];		//BIT19��20��21��23
		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_selective_access%8)];		//BIT19��20��21��23
	}
	else										//LLS��Ͱ�ȫ�����֤
	{
		Buff[0] = 0x00;
		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11
		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ByteBit[7-(Conformance_action%8)];		//BIT19��20��21��23
	}	
	
	*Source &= *Ptr;
	*(Source+1) &= *(Ptr+1);
	*(Source+2) &= *(Ptr+2);

	Conformance = CommPara[COM_No].Conformance;
	*(Conformance+3) = SecretState;
	
//	if( IsAllData( Ptr, 3, 0x00 ) == 0 )
	if( IsAllData( Source, 3, 0x00 ) == 0 )
	{
//		RAM_Write( Conformance, Ptr, 3 );
		RAM_Write( Conformance, Source, 3 );
		return 1;
	}
	else
	{	
		RAM_Write( Conformance, Source, 3 );
		return 0;	
	}	
}	 
#endif
//����User_Systemtile����
short	CreatUser_Systemtile( short COM_No, unsigned char* Dest, unsigned short ErrByte )	
{
	unsigned short Len=12;
	
//	Ptr = Dest;
	
	*Dest = 0xA4;					// user-information�ֶ����(component [30], Context-specific)��ǵı���
	*(Dest+1) = 0x0A;					// ��ǵ����ֵ�ֶεĳ��ȵı���
									//-- application-context-name���(OCTET STRING)�ı���
	*(Dest+2) = 0x04;				// user-information (OCTET STRING, Universal)ѡ��ı���
	*(Dest+3) = 0x08;				// OCTET STRINGֵ�ֶεĳ��ȵı���(4����λ�ֽ�)
			
	RAM_Write( Dest+4, ICComm.SystemTitle, 8 );
	return Len;
}
	
//����User_information����
short	CreatUser_information( short COM_No, unsigned char* Dest, unsigned short ErrByte )	
{
	unsigned short Len=8;
	unsigned char Buff[50];
	unsigned char *Ptr;
	unsigned short Len_cipher = 0;
	Ptr = Buff;
//	Ptr = Dest;
	
	*Dest = 0xBE;					// user-information�ֶ����(component [30], Context-specific)��ǵı���
	*(Dest+1) = 0x06;					// ��ǵ����ֵ�ֶεĳ��ȵı���
									//-- application-context-name���(OCTET STRING)�ı���
	*(Dest+2) = 0x04;				// user-information (OCTET STRING, Universal)ѡ��ı���
	*(Dest+3) = 0x04;				// OCTET STRINGֵ�ֶεĳ��ȵı���(4����λ�ֽ�)
	
	if( ErrByte == APPL_OPEN_9 )
	{
												//-- xDLMS-Initiate.response PDU��A-XDR����
		*(Dest+4) = 0x0E;						// DLMS PDUѡ�����confirmedServiceError�ı�ǣ���ʽ��ǣ��ı���
												//-- confirmedServiceError���
		*(Dest+5) = CSError_initiateError;		//confirmedServiceError�е�1   initiateError [1] ServiceError
		*(Dest+6) = SError_initiate;			//ServiceError �е� 6       inttiate [6]  ��ʼ��������� 
		*(Dest+7) = SError6_other;
	}
	else if( ErrByte == APPL_OPEN_12_S1 )
	{
		*(Dest+4) = 0x0E;
		*(Dest+5) = CSError_initiateError;
		*(Dest+6) = SError_initiate;
		*(Dest+7) = SError6_dlms_version_too_low;		
	}		
	else if( ErrByte == APPL_OPEN_14_S2 )
	{
		*(Dest+4) = 0x0E;
		*(Dest+5) = CSError_initiateError;
		*(Dest+6) = SError_initiate;
		*(Dest+7) = SError6_pdu_size_too_short;				
	}		
	else
	{
//			if(ICEC.useDK!=1)
//			{
					*(Dest+1) = 0x10;			// ��ǵ����ֵ�ֶεĳ��ȵı���
					*(Dest+3) = 0x0E;			// OCTET STRINGֵ�ֶεĳ��ȵı���(14����λ�ֽ�)	
										//-- xDLMS-Initiate.response PDU��A-XDR����
					*(Dest+4) = 0x08;			// DLMS PDUѡ�����InitiateResponse�ı�ǣ���ʽ��ǣ��ı���
										//-- negotiated-quality-of-service���(��ѡ��, ������)
					*(Dest+5) = 0x00;			// proposed-quality-of-service�����ʹ�ñ��(FALSE, ������)
										//-- negotiated-dlms-version-number����ı���(Unsigned8, value=6)
					*(Dest+6) = 0x06;			// ����ΪUnsigned8�����ݵ�A-XDR�����������ֵ
										//-- һ���Կ�[APPLICATION 31] IMPLICIT BITSTRING (SIZE(24))�ı���
					*(Dest+7) = 0x5F;
					*(Dest+8) = 0x1F;			// [APPLICATION 31] ��� (ASN.1 ��ʽ���)�ı���
					*(Dest+9) = 0x04;			// 'contents' �ֶγ��ȣ��ð�λ�ֽ�����ʾ���ı��루4����λ�ֽڣ�
					*(Dest+10) = 0x00;			// λ�������һ����λ�ֽ�û��ʹ�õı������ı���
					RAM_Write( Dest+11, CommPara[COM_No].Conformance, 3 );		// �̶�����λ��ֵ�ı���
					if(COM_No==2)//2016-8-17-fire
					{
							//-- server-max-receive-pdu-size����ı���(Unsigned16, value=128)
//							*(Dest+14) = MAX_INFO_SIZE / 256;	
//							*(Dest+15) = MAX_INFO_SIZE % 256;				// ����ΪUnsigned16�����ݵ�A-XDR�����������ֵ
							*(Dest+14) = GPRS_MAX_INFO_SIZE / 256;	
							*(Dest+15) = GPRS_MAX_INFO_SIZE % 256;				// ����ΪUnsigned16�����ݵ�A-XDR�����������ֵ
				  }
				  else
				  {
				  		//-- server-max-receive-pdu-size����ı���(Unsigned16, value=512)
							*(Dest+14) = MAX_PDU_SIZE / 256;	
							*(Dest+15) = MAX_PDU_SIZE % 256;				// ����ΪUnsigned16�����ݵ�A-XDR�����������ֵ
				  }
			//							//-- server-max-receive-pdu-size����ı���(Unsigned16, value=512)
			//		*(Dest+14) = MAX_PDU_SIZE / 256;	
			//		*(Dest+15) = MAX_PDU_SIZE % 256;	// ����ΪUnsigned16�����ݵ�A-XDR�����������ֵ
										//-- VAA-Name����ı��� (Unsigned16, ʹ��LN����ʱֵ=0x0007��ʹ��SN����ʱֵ= FA 00)
					*(Dest+16) = 0x00;
					*(Dest+17) = 0x07;			// ����ΪUnsigned16�����ݵ�A-XDR�����������ֵ
					Len = 18;
//			}
//			else
//			{
//		  }
	}
	/*�������ܣ����ܲ��֣���ע�͵�
	// cipher..
	if(ICEC.use_cipher_information == 0x21) //���ܷ���..
	{
//		 Len_cipher = Gcm128EnDecrypt( ESAM, Ptr,Dest+4, 0 ,crypt.glo_FC[0],1, 14 );
//		 *(Dest+1)  =  Len_cipher + 9;
//		 *(Dest+3)  =  Len_cipher + 7;
//		 *(Dest+4)  = 0x28;
//		 *(Dest+5)  = Len_cipher + 5;
//		 RAM_Write( Dest+6, crypt.glo_FC, 5 );
//		 RAM_Write( Dest+11, Ptr, Len_cipher );
//		 Len = Len_cipher+11;	
//
  unsigned char TEMP_DK;
  TEMP_DK = ICEC.use_DK;
  ICEC.use_DK = 0;
		 	if( Init_For_Process( ESAM ) != 0 ) 
			{ 
					     ICEC.EsamOK = 0;
					     //return 2;   
								//Flag.MeterState1 |= F_ESAMErr;				//�������״̬1
			} 
		 Len_cipher = Gcm128EnDecrypt( ESAM, Ptr,Dest+4, 0 ,crypt.glo_FC[0],1, 14 );
		 *(Dest+1)  =  Len_cipher + 9;
		 *(Dest+3)  =  Len_cipher + 7;
		 *(Dest+4)  = 0x28;
		 *(Dest+5)  = Len_cipher + 5;
		 RAM_Write( Dest+6, crypt.glo_FC, 5 );
		 RAM_Write( Dest+11, Ptr, Len_cipher );
		 Len = Len_cipher+11;	

			ICEC.use_DK = TEMP_DK; 
			
			if((ICEC.use_DK ==1)&&(ICComm.SaveDk[0]<=50))
			{
			 	if( Init_For_Process( ESAM ) != 0 ) 
					{ 
							     ICEC.EsamOK = 0;
							     //return 2;   
										//Flag.MeterState1 |= F_ESAMErr;				//�������״̬1
					}	

					Gcm128EnDecrypt( ESAM, &ICComm.SaveDk[1],&ICComm.SaveDk[1], 1 ,crypt.glo_FC[0],0x55, ICComm.SaveDk[0] );   //����0x55��ʾ����DKģʽ..			  
		  }
	//	 Gcm128EnDecrypt( ESAM, Ptr,Ptr+7, 1 ,crypt.glo_FC[0],0x55, ((*(Ptr+1))-5) );   //����0x55��ʾ����DKģʽ..
		 //����DK����ʱ����Կ..�´ν���DK����ʹ��..
	//	 ICComm.SaveDk[0] = ((*(Ptr+1))-5);
	//	 if(ICComm.SaveDk[0]<=50)  RAM_Write(&ICComm.SaveDk[1],Ptr+7,ICComm.SaveDk[0]);
			 		 
		 	 
	}
	*/
	return Len;
}	


////short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length )	
//short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char SecretState )	
//{
//	unsigned char Len;
//	unsigned short ErrByte = 0;
//	unsigned char* Ptr;
//	unsigned short Temp;
//
//	Ptr = Source;
//	Ptr++;
//	Len = *Ptr;
//	*Length = Len + 2;
//	Ptr++;
//	if( *Ptr != 0x04 ) return APPL_OPEN_9;		//04  // user-information (OCTET STRING, Universal)ѡ��ı���
//	Len -= 2;
//	Ptr++;
//	if( *Ptr != Len ) return APPL_OPEN_9;		// OCTET STRINGֵ�ֶεĳ��ȵı���(n����λ�ֽ�)
//	Ptr++; Len--;
//	if( *Ptr != 0x01 ) return APPL_OPEN_9;		//01   // DLMS PDUѡ�����InitiateRequest�ı�ǣ���ʽ��ǣ��ı���
//	Ptr++; Len--;
////	if( *Ptr != 0 )	return APPL_OPEN_9;			//ӦΪ00    // dedicated-key�����ʹ�ñ�־(FALSE, Ӧ�ò�����)
////	Ptr++; Len--;
////2015-03-12 2:15:39����dedicated-key�����ʹ�ñ�־�ж�..
//	if( *Ptr == 0x01 )
//	{
//		Ptr++; Len--;
//		if(Len<28) return APPL_OPEN_9;	  //DKΪ�̶�16�ֽ�..
//		if((*Ptr == 0x09 )&&(*(Ptr+1) == 0x10 ))
//		{
//				RAM_Write( ICEC.d_key, Ptr+2, 0x10 );
//				Ptr += 18;
//				Len -= 18;
//				ICEC.use_DK = 1;
//		}else return APPL_OPEN_9;	 
//		
//	}
//	else
//	{
//			if( *Ptr != 0 )	return APPL_OPEN_9;			//��Ϊ01 ��ӦΪ00    // dedicated-key�����ʹ�ñ�־(FALSE, Ӧ�ò�����)
//	    Ptr++; Len--;
//	    ICEC.use_DK =0;
//	}												// response-allowed�����ʹ�ñ�־(TRUE)	
//	Ptr++; Len--;										
//	if( *Ptr != 0 ) { Ptr++; Len--; }			//01 proposed-quality-of-service�����ʹ�ñ�־(TRUE,����)
//	Ptr++; Len--;								//01 proposed-quality-of-service������ı��� (1)	
//	if( *Ptr < 6 ) return APPL_OPEN_12_S1;		//proposed-dlms-version-number����ı���(Unsigned8, value=6)
//	Ptr++; Len--;
//	if( *Ptr != 0x5F ) return APPL_OPEN_9;
//	if( Len == 8 ) 
//	{
//		Ptr++;
//		if( *Ptr != 0x1F ) return APPL_OPEN_9;	
//	}
//	Ptr++;										 //4 'contents' �ֶγ��ȣ��ð�λ�ֽ�����ʾ���ı��루4����λ�ֽڣ�
//	Ptr++;										 // λ�������һ����λ�ֽ�û��ʹ�õı������ı���
//	Ptr++;										 // λ��
////	if( GetComformanceState( COM_No, Ptr) != 0 ) return APPL_OPEN_13_S3;
//	if( GetComformanceState( COM_No, Ptr, SecretState ) != 0 ) return APPL_OPEN_13_S3;
//	Ptr += 3;
//	Temp = *Ptr;
//	Ptr++;
//	Temp *= 256;
//	Temp += *Ptr;
//	if( Temp < 12 ) return APPL_OPEN_14_S2;		//��������	//13.03.29	
//	return ErrByte;
//	
//}	

//short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length )	
short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char SecretState )	
#if 0
{
	unsigned char Len;
	unsigned short ErrByte = 0;
	unsigned char* Ptr;
	unsigned short Temp;

	ICEC.use_DK = 0;
       ICEC.use_cipher_information = 0;
	
	Ptr = Source;
	Ptr++;
	Len = *Ptr;
	*Length = Len + 2;
	Ptr++;
	if( *Ptr != 0x04 ) return APPL_OPEN_9;		//04  // user-information (OCTET STRING, Universal)ѡ��ı���
	Len -= 2;
	Ptr++;
	if( *Ptr != Len ) return APPL_OPEN_9;		// OCTET STRINGֵ�ֶεĳ��ȵı���(n����λ�ֽ�)
	Ptr++; Len--;
	//��DK��������0x21---33..
/*	��������
if( *Ptr == 0x21)
	{
		 ICEC.use_cipher_information = 0x21;  //������..
			// ����systemtitle�� AAD(AK)..
			RAM_Write( crypt.glo_FC,Ptr+2, 5 );
//			E2P_RData( ICComm.SystemTitle,E2P_server_system_title, 8 );   //��������systile..
			//����AAD--AK
			E2P_RData( ICComm.SysAAD,E2P_global_key_transfer_data, 16 );   //AK..
		  //����..
		 //====
		 //��ʼ��..
		 	if( Init_For_Process( ESAM ) != 0 ) 
			{ 
				ICEC.EsamOK = 0;
				if( Card_CoolRST( ESAM ) != 0 ) 
				{ 
					ICEC.EsamOK = 0;
					//return 1;
				}				
			}
		 	if( Init_For_Process( ESAM ) != 0 ) 
			{ 
					     ICEC.EsamOK = 0;
					     //return 2;   
								//Flag.MeterState1 |= F_ESAMErr;				//�������״̬1
			}
		 //����DK����ʱ����Կ..�´ν���DK����ʹ��..
		 ICComm.SaveDk[0] = ((*(Ptr+1))-5);
		 if(ICComm.SaveDk[0]<=50)  RAM_Write(&ICComm.SaveDk[1],Ptr+7,ICComm.SaveDk[0]);
		 	//����AARQ,AARE��Ҫ��ȫ����ԿҪ����..
			  
		 Gcm128EnDecrypt( ESAM, Ptr,Ptr+7, 1 ,crypt.glo_FC[0],0x55, ((*(Ptr+1))-5) );   //����0x55��ʾ����DKģʽ..
		 //Gcm128EnDecrypt( ESAM, Ptr,Ptr+7, 1 ,crypt.glo_FC[0],0x55, ((*(Ptr+1))-5) );   //����0x55��ʾ����DKģʽ..
	}
	*/
	if( *Ptr != 0x01 ) return APPL_OPEN_9;		//01   // DLMS PDUѡ�����InitiateRequest�ı�ǣ���ʽ��ǣ��ı���
	Ptr++; Len--;
//	if( *Ptr != 0 )	return APPL_OPEN_9;			//ӦΪ00    // dedicated-key�����ʹ�ñ�־(FALSE, Ӧ�ò�����)
//	Ptr++; Len--;
//2015-03-12 2:15:39����dedicated-key�����ʹ�ñ�־�ж�..
	if( *Ptr == 0x01 )
	{
		Ptr++; Len--;
		if(Len<27) return APPL_OPEN_9;	  //DKΪ�̶�16�ֽ�..
		//if((*Ptr == 0x09 )&&(*(Ptr+1) == 0x10 ))
		if(*Ptr == 0x10 )
		{
				RAM_Write( ICEC.d_key, Ptr+1, 0x10 );
				Ptr += 17;
				Len -= 17;
				ICEC.use_DK = 1;
		}else return APPL_OPEN_9;
	}
	else
	{
			if( *Ptr != 0 )	return APPL_OPEN_9;			//��Ϊ01 ��ӦΪ00    // dedicated-key�����ʹ�ñ�־(FALSE, Ӧ�ò�����)
	    Ptr++; Len--;
	    ICEC.use_DK =0;
	}												// response-allowed�����ʹ�ñ�־(TRUE)	
	Ptr++; Len--;										
	if( *Ptr != 0 ) { Ptr++; Len--; }			//01 proposed-quality-of-service�����ʹ�ñ�־(TRUE,����)
	Ptr++; Len--;								//01 proposed-quality-of-service������ı��� (1)	
	if( *Ptr < 6 ) return APPL_OPEN_12_S1;		//proposed-dlms-version-number����ı���(Unsigned8, value=6)
	Ptr++; Len--;
	if( *Ptr != 0x5F ) return APPL_OPEN_9;
        //if( Len == 8 )
	if( Len!= 7 ) 
	{
		Ptr++;
		if( *Ptr != 0x1F ) return APPL_OPEN_9;	
	}
	Ptr++;										 //4 'contents' �ֶγ��ȣ��ð�λ�ֽ�����ʾ���ı��루4����λ�ֽڣ�
	Ptr++;										 // λ�������һ����λ�ֽ�û��ʹ�õı������ı���
	Ptr++;										 // λ��
//	if( GetComformanceState( COM_No, Ptr) != 0 ) return APPL_OPEN_13_S3;
	if( GetComformanceState( COM_No, Ptr, SecretState ) != 0 ) return APPL_OPEN_13_S3;
	Ptr += 3;
	Temp = *Ptr;
	Ptr++;
	Temp *= 256;
	Temp += *Ptr;
	if( Temp < 12 ) return APPL_OPEN_14_S2;		//��������	//13.03.29	
	return ErrByte;
	
}
#endif

{
	unsigned char Len;
	unsigned short ErrByte = 0;
	unsigned char* Ptr;
	unsigned short Temp;

	Ptr = Source;
	Ptr++;
	Len = *Ptr;
	*Length = Len + 2;
	Ptr++;
	if( *Ptr != 0x04 ) return APPL_OPEN_9;		//04  // user-information (OCTET STRING, Universal)ѡ��ı���
	Len -= 2;
	Ptr++;
	if( *Ptr != Len ) return APPL_OPEN_9;		// OCTET STRINGֵ�ֶεĳ��ȵı���(n����λ�ֽ�)
	Ptr++; Len--;
	if( *Ptr != 0x01 ) return APPL_OPEN_9;		//01   // DLMS PDUѡ�����InitiateRequest�ı�ǣ���ʽ��ǣ��ı���
	Ptr++; Len--;
	if( *Ptr != 0 )	return APPL_OPEN_9;			//ӦΪ00    // dedicated-key�����ʹ�ñ�־(FALSE, Ӧ�ò�����)
	Ptr++; Len--;
												// response-allowed�����ʹ�ñ�־(TRUE)	
	Ptr++; Len--;										
	if( *Ptr != 0 ) { Ptr++; Len--; }			//01 proposed-quality-of-service�����ʹ�ñ�־(TRUE,����)
	Ptr++; Len--;								//01 proposed-quality-of-service������ı��� (1)	
	if( *Ptr < 6 ) return APPL_OPEN_12_S1;		//proposed-dlms-version-number����ı���(Unsigned8, value=6)
	Ptr++; Len--;
	if( *Ptr != 0x5F ) return APPL_OPEN_9;
	if( Len == 8 ) 
	{
		Ptr++;
		if( *Ptr != 0x1F ) return APPL_OPEN_9;	
	}
	Ptr++;										 //4 'contents' �ֶγ��ȣ��ð�λ�ֽ�����ʾ���ı��루4����λ�ֽڣ�
	Ptr++;										 // λ�������һ����λ�ֽ�û��ʹ�õı������ı���
	Ptr++;										 // λ��
//	if( GetComformanceState( COM_No, Ptr) != 0 ) return APPL_OPEN_13_S3;
	if( GetComformanceState( COM_No, Ptr, SecretState ) != 0 ) return APPL_OPEN_13_S3;
	Ptr += 3;
	Temp = *Ptr;
	Ptr++;
	Temp *= 256;
	Temp += *Ptr;
	if( Temp < 12 ) return APPL_OPEN_14_S2;		//��������	//13.03.29	
	
	return ErrByte;
	
}	


//short	ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length )	
short	ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char* SecretState )	
{
	unsigned char Len;
	unsigned short ErrByte = 0;
	//unsigned char *AAPtr;
		
	switch( *Source )
	{
		case AARQ_protocol_version:					//0x80	//IMPLICIT BIT STRING {version1 (0)} DEFAULT {version1}
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//���BITΪ0���쳣
			{
				ErrByte = APPL_OPEN_4_S2;				
			}
			*Length = *(Source+1) + 2; 
			break;
		case AARQ_application_context_name:			//0xA1
			if( CompareApplicationContextName( Source ) != 0 ) ErrByte = APPL_OPEN_5_S2;				
//			else
//			{
//				if(*(Source+10)==3) *(Dest+10) = *(Source+10);
//			}
			*Length = 11;
			break;
		case AARQ_called_AP_title:					//0xA2	//OPTIONAL
		case AARQ_called_AE_qualifier:				//0xA3	//OPTIONAL	
		case AARQ_called_AP_invocation_id:			//0xA4	//OPTIONAL
		case AARQ_called_AE_invocation_id:			//0xA5	//OPTIONAL
		case AARQ_calling_AE_qualifier:				//0xA7	//OPTIONAL	
		case AARQ_calling_AP_invocation_id:			//0xA8	//OPTIONAL
		case AARQ_calling_AE_invocation_id:			//0xA9	//OPTIONAL
			*Length = *(Source+1) + 2;
			break;
	 case AARQ_calling_AP_title:					//0xA6	//OPTIONAL
	 	  //AAPtr = ICComm.ClientSystemTitle;
	 		E2P_RData( ICComm.ClientSystemTitle, E2P_client_system_title, 8 );			//��ȡ��������SYSTEM TITLE ..
		   if( Data_Comp( ICComm.ClientSystemTitle, Source+4, 8 ) != 0 )
		   {
		   	   RAM_Write(ICComm.ClientSystemTitle,Source+4, 8 );
		   	   E2P_WData( E2P_client_system_title, ICComm.ClientSystemTitle,8 );
		   } 
	 		*Length = *(Source+1) + 2;
	 		// ��������Ҫ����..
	 		
			break;		
		//-- ���ֻʹ���ںˣ�������ֶβ����֡� 		
		case AARQ_sender_acse_requirements:			//0x8A	//IMPLICIT OPTIONAL
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//���BITΪ1��ʹ�ð�ȫ��֤
		///	if( GetSTRINGLowestBIT( Source, Len ) == 1 )	//���BITΪ1��ʹ�ð�ȫ��֤  2016-8-25-fire ��Ϊ1 APPL_OPEN_7 ��1��ͨ��
			{
				ErrByte = APPL_OPEN_7_S1;				
			}
			else 
				{
				*SecretState |= 0x01;
			///	ErrByte = APPL_OPEN_7_S3;//2016-8-25-fire
				}
//			*SecretState |= 0x80;
			*Length = *(Source+1) + 2;
			break;
		 //-- ���ѡ�������֤��Ԫ��������ֶ�Ӧ����
		case AARQ_mechanism_name:					//0x8B	//IMPLICIT OPTIONAL
			if(( *SecretState & 0x01 ) != 0 ) 
			{
				//if( CompareMechanism_name( Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;				
				// else *SecretState |= 0x02;
				ErrByte = CompareMechanism_name( Source );
				if(ErrByte == 0x55){
					 crypt.aarqHLS[COM_No] = 0x55;
					 ErrByte = 0;
				 }
				if( ErrByte != 0 )
				{
					 ErrByte = APPL_OPEN_7_S1;
					 crypt.choice_algorithm = 0;
				}				
				else *SecretState |= 0x02;
				// HLS..
							
			}	
			else ErrByte = APPL_OPEN_7_S1; 				
			*Length = *(Source+1) + 2;
			break;
		 //-- ���ѡ�������֤��Ԫ��������ֶ�Ӧ����
		case AARQ_calling_authentication_value:		//0xAC	//EXPLICIT OPTIONAL
			if(( *SecretState & 0x03 ) == 0x03 ) 
			{
				if(crypt.aarqHLS[COM_No] != 0x55){
						if( CompareAuthentication_value( COM_No,Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
						else *SecretState |= 0x04;
				}else{
						if( CompareAuthentication_HLS( Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
						else *SecretState |= 0x04;
				}					
			}
			else ErrByte = APPL_OPEN_7_S1;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_implementation_information:		//0xBD	//IMPLICIT OPTIONAL
			ErrByte = APPL_OPEN_8;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_user_information:					//0xBE	//IMPLICIT OPTIONAL
//			if(( *SecretState != 0x80 )&&( *SecretState != 0x0F )) ErrByte = APPL_OPEN_7_S1;				
//			if(( *SecretState != 0x00 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;				
			if((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 )&&( *SecretState != 0x07 )) 
				ErrByte = APPL_OPEN_7_S1;		//�ͻ�����ַΪ0x10,	MechanismName = NO_SECURITY	
		///	if((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x23 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//�ͻ�����ַΪ0x11,	MechanismName = NO_SECURITY	 2016-8-25-fire
//			if(((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x03 )||(*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 ))&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//�ͻ�����ַΪ0x10,	MechanismName = NO_SECURITY	
			else
			{
//				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length );
				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length, *SecretState );
			}	
			break;
    default:
    		ErrByte = APPL_OPEN_8;
    		break;	
	}	
	return ErrByte;
}	

/*
	short ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char* SecretState )	
	{
	unsigned char Len;
	unsigned short ErrByte = 0;
	
	switch( *Source )
	{
		case AARQ_protocol_version:					//0x80	//IMPLICIT BIT STRING {version1 (0)} DEFAULT {version1}
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//���BITΪ0���쳣
			{
				ErrByte = APPL_OPEN_4_S2;				
			}
			*Length = *(Source+1) + 2;
			break;
		case AARQ_application_context_name:			//0xA1
			if( CompareApplicationContextName( Source ) != 0 ) ErrByte = APPL_OPEN_5_S2;				
			*Length = 11;
			break;
		case AARQ_called_AP_title:					//0xA2	//OPTIONAL
		case AARQ_called_AE_qualifier:				//0xA3	//OPTIONAL	
		case AARQ_called_AP_invocation_id:			//0xA4	//OPTIONAL
		case AARQ_called_AE_invocation_id:			//0xA5	//OPTIONAL
		case AARQ_calling_AP_title:					//0xA6	//OPTIONAL
		case AARQ_calling_AE_qualifier:				//0xA7	//OPTIONAL	
		case AARQ_calling_AP_invocation_id:			//0xA8	//OPTIONAL
		case AARQ_calling_AE_invocation_id:			//0xA9	//OPTIONAL
			*Length = *(Source+1) + 2;
			break;
		//-- ���ֻʹ���ںˣ�������ֶβ����֡� 		
		case AARQ_sender_acse_requirements:			//0x8A	//IMPLICIT OPTIONAL
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//���BITΪ1��ʹ�ð�ȫ��֤
			{
				ErrByte = APPL_OPEN_7_S1;				
			}
			else *SecretState |= 0x01;
//			*SecretState |= 0x80;
			*Length = *(Source+1) + 2;
			break;
		 //-- ���ѡ�������֤��Ԫ��������ֶ�Ӧ����
		case AARQ_mechanism_name:					//0x8B	//IMPLICIT OPTIONAL
			if(( *SecretState & 0x01 ) != 0 ) 
			{
				if( CompareMechanism_name( Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
				else *SecretState |= 0x02;		
			}	
			else ErrByte = APPL_OPEN_7_S1; 				
			*Length = *(Source+1) + 2;
			break;
		 //-- ���ѡ�������֤��Ԫ��������ֶ�Ӧ����
		case AARQ_calling_authentication_value:		//0xAC	//EXPLICIT OPTIONAL
			if(( *SecretState & 0x03 ) == 0x03 ) 
			{
				if( CompareAuthentication_value( COM_No,Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
				else *SecretState |= 0x04;					
			}
			else ErrByte = APPL_OPEN_7_S1;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_implementation_information:		//0xBD	//IMPLICIT OPTIONAL
			ErrByte = APPL_OPEN_8;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_user_information:					//0xBE	//IMPLICIT OPTIONAL
//			if(( *SecretState != 0x80 )&&( *SecretState != 0x0F )) ErrByte = APPL_OPEN_7_S1;				
//			if(( *SecretState != 0x00 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;				
			if((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//�ͻ�����ַΪ0x10,	MechanismName = NO_SECURITY	
//			if(((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x03 )||(*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 ))&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//�ͻ�����ַΪ0x10,	MechanismName = NO_SECURITY	
			else
			{
//				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length );
				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length, *SecretState );
			}	
			break;
    default:
    		ErrByte = APPL_OPEN_8;
    		break;	
	}	
	return ErrByte;
}	
*/
short GetAAREResult_Diagnostic(  short COM_No, unsigned char* Dest, unsigned short RejectType )
{	
	unsigned char Buff[20];
	unsigned char *Ptr;
	short len;
	Ptr = Buff;
	
	*Dest = 0xA2;   	// result ��� (component [2], Context-specific)�ı�Ǻͳ��ȵı���
	*(Dest+1) = 0x03;   // ��ǵ����ֵ�ֶεĳ��ȵı���
						//-- result-component (INTEGER)�ı���
	*(Dest+2) = 0x02;   // result (INTEGER, Universal)ѡ��ı���
	*(Dest+3) = 0x01;  	// resultֵ�ֶεĳ��ȵı���(1����λ�ֽ�)
	*(Dest+4) = Result_accepted_0;  	// Result (accepted (0))ֵ�ı���
						//-- result-source-diagnostic (tagged component [3])�ı���
	*(Dest+5) = 0xA3;   // result-source-diagnostic���(component [3], Context-specific)��ǵı���
	*(Dest+6) = 0x05;   // ��ǵ����ֵ�ֶεĳ��ȵı���
	*(Dest+7) = Diagnostic_acse_service_user;  	// acse-service-user ѡ��(1)��ǵı��� 
	*(Dest+8) = 0x03;  	// ��ǵ����ֵ�ֶεĳ��ȵı���
						//-- result-source-diagnostics���(INTEGER)�ı���
	*(Dest+9) = 0x02;  	// result-source-diagnostics (INTEGER, Universal)ѡ��ı���
	*(Dest+10) = 0x01;  // ֵ�ֶγ��ȵı��� (1����λ�ֽ�)
	*(Dest+11) = Diagnostic_null_0;  // ֵ�ı��룺null (0)
	switch( RejectType )
	{
		case APPL_OPEN_4_S2:	
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))ֵ�ı���
			*(Dest+7) = Diagnostic_acse_service_provider;  			// acse-service-provider ѡ��(2)��ǵı��� 
			*(Dest+11) = Diagnostic_no_common_acse_version_2;  		// ֵ�ı��룺no-common-acse-version (2)
			break;
		case APPL_OPEN_5_S2:	
		case APPL_OPEN_5_S3:	
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))ֵ�ı���
			*(Dest+7) = Diagnostic_acse_service_user;  				// acse-service-user ѡ��(1)��ǵı��� 
			*(Dest+11) = Diagnostic_application_context_name_not_supported_2;  	// ֵ�ı��룺application-context-name-not-supported (2)
			break;
		case APPL_OPEN_7_S1:	
		case APPL_OPEN_7_S2:	
		case APPL_OPEN_7_S3:	
		case APPL_OPEN_7_S4:	
		case APPL_OPEN_7_S5:	
		case APPL_OPEN_7_S6:	
		case APPL_OPEN_7_S7:	
		case APPL_OPEN_7_S8:	
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))ֵ�ı���
			*(Dest+7) = Diagnostic_acse_service_user;  				// acse-service-user ѡ��(1)��ǵı��� 
			*(Dest+11) = Diagnostic_authentication_failure_13;  	// ֵ�ı��룺authentication-failure (13)
			break;
		case APPL_OPEN_8:				
		case APPL_OPEN_9:			
		case APPL_OPEN_12_S1:
		case APPL_OPEN_13_S3:
		case APPL_OPEN_14_S2:
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))ֵ�ı���
			*(Dest+7) = Diagnostic_acse_service_user;  				// acse-service-user ѡ��(1)��ǵı��� 
			*(Dest+11) = Diagnostic_no_reason_given_1;  			// ֵ�ı��룺no-reason-given (1)
			break;
        default: break;
	}
	len = 12;
	if((crypt.aarqHLS[COM_No]== 0x55)&&(RejectType==0)){
			*(Dest+11) = 0x0e;  
			 // 0e 88 02 07 80 89 07 60 85 74 05 08 02 05 aa 0a 80 08 50 36 77 52 4a 32 31 46
			 *(Dest+12) = 0x88; 
			 *(Dest+13) = 0x02; 
			 *(Dest+14) = 0x07; 
			 *(Dest+15) = 0x80; 
			 *(Dest+16) = 0x89; 
			 *(Dest+17) = 0x07; 
			 *(Dest+18) = 0x60; 
			 *(Dest+19) = 0x85; 
			 *(Dest+20) = 0x74; 
			 *(Dest+21) = 0x05; 
			 *(Dest+22) = 0x08; 			 
			 *(Dest+23) = 0x02; 
			 //*(Dest+24) = 0x05;
			 *(Dest+24) = crypt.choice_algorithm;
			  			 
			 *(Dest+25) = 0xaa; 
			 *(Dest+26) = 0x0a; 
			 *(Dest+27) = 0x80; 
			 *(Dest+28) = 0x08;
			 // �������ʱ�̶�..������8���ֽڵ������..
			 if(crypt.Challenge_Len==8)
			 {
			 /*
					//ȡESAM�����������..
					GetChallenge( ESAM, Ptr, 8 );
				  RAM_Write( Dest+29, Ptr,  8 );  
				  //��������������..
				  RAM_Write( crypt.S_Challenge, Ptr,  8 );
			 		///return 37;
			 		 	len = 37;		
			 	  return len;
			 		*/
		   }
		   else
		   {  //Challenge_Len==16
		   		*(Dest+26) = 0x12; 
		   		*(Dest+28) = 0x10; 
		   		RAM_Write( Dest+29, crypt.C_Challenge,  16 );  
				  //��������������..
				  RAM_Write( crypt.S_Challenge, Dest+29,  16 );
				  	len = 45;		
			 	  return len;
		   }  			 
			 /* 
			 *(Dest+29) = 0x50; 
			 *(Dest+30) = 0x36; 
			 *(Dest+31) = 0x77; 
			 *(Dest+32) = 0x52; 			 
			 *(Dest+33) = 0x4a; 
			 *(Dest+34) = 0x32; 
			 *(Dest+35) = 0x31; 
			 *(Dest+36) = 0x46;
			*/
	}
	else 
		len = 12;
			return len;
}	

//short AARQProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
//{
//	unsigned short* RecDataPtr;
//	short Length;
//	unsigned char* Source;
//	unsigned short SendLength=0;
//	unsigned char* Dest;
//	unsigned short ItemLen;
//	unsigned short ErrByte;
//	unsigned char* APPLState;
//	unsigned char SecretState;
//	
//	RecDataPtr = CommPara[COM_No].RecDataPtr;
//	APPLState = CommPara[COM_No].APPLState;
//	Length = *RecDataPtr - 3;
//	Length -= 2;
//	
//	if( *(RecBuff+1) != Length ) return -1;			//�յ��ĳ��Ⱥ���Ϣ֡�еĳ��Ȳ��ȣ��Ƿ�
//
//	*SendBuff = APDU_AARE;
//	*(SendBuff+1) = 0;
//	FillApplicationContextName( SendBuff+2 );
//	SendLength = 11;
//	
//	Dest = SendBuff+13;
//	
//	Source = RecBuff+2;
//	ItemLen = 0;
//	SecretState = 0;
//	while( Length > 0 )
//	{
////		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen );	
//		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen, &SecretState );	
//		if( ErrByte != 0 )
//		{
//			GetAAREResult_Diagnostic( Dest, ErrByte );
//			Dest += 12;
//			SendLength += 12;
//			break;
//		}	
//		Length -= ItemLen;
//		Source += ItemLen;
//	}
//	if( ErrByte == 0 )
//	{
//		GetAAREResult_Diagnostic( Dest, ErrByte );
//		Dest += 12;
//		SendLength += 12;
//	}		
//	SendLength += CreatUser_information( COM_No, Dest, ErrByte );
//	*(SendBuff+1) = SendLength;
//	
//	if( ErrByte != 0 ) *APPLState = APPLIdle;			//δ����Ӧ������
//	else *APPLState = APPLON; 							//����Ӧ������	
//
//	return SendLength+2;	
//}					

short RLRQProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short* RecDataPtr;
	short Length;
	//unsigned char* Source;
	unsigned short SendLength=0;
	//unsigned char* Dest;
	//unsigned short ItemLen;
	unsigned short ErrByte=0;
	unsigned char* APPLState;
	//unsigned char SecretState;
	
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	APPLState = CommPara[COM_No].APPLState;
	Length = *RecDataPtr - 3;
	Length -= 2;
	
	//if( *(RecBuff+1) != Length ) return -1;			//�յ��ĳ��Ⱥ���Ϣ֡�еĳ��Ȳ��ȣ��Ƿ�

///----------------------------------------------
	*SendBuff = RLRE_APDU;
		*(SendBuff+1) = 0x03;
		*(SendBuff+2) = 0x80;	
		*(SendBuff+3) = 0x01;		
		*(SendBuff+4) = 0x00;
    SendLength = 3;
    
	SendLength += CreatUser_information( COM_No, SendBuff+5, ErrByte );
	*(SendBuff+1) = SendLength;
	
	//SendLength = 0;
//	*(SendBuff+1) = 0;
//	FillApplicationContextName( SendBuff+2 );
//	SendLength = 11;
//	
//	Dest = SendBuff+13;
//	
//	Source = RecBuff+2;
//	ItemLen = 0;
//	SecretState = 0;
//	while( Length > 0 )
//	{
////		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen );	
//		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen, &SecretState );	
//		if( ErrByte != 0 )
//		{
//			GetAAREResult_Diagnostic( Dest, ErrByte );
//			Dest += 12;
//			SendLength += 12;
//			break;
//		}	
//		Length -= ItemLen;
//		Source += ItemLen;
//	}
//	if( ErrByte == 0 )
//	{
//		GetAAREResult_Diagnostic( Dest, ErrByte );
//		Dest += 12;
//		SendLength += 12;
//	}		
//	SendLength += CreatUser_information( COM_No, Dest, ErrByte );
//	*(SendBuff+1) = SendLength;
//	
//	if( ErrByte != 0 ) *APPLState = APPLIdle;			//δ����Ӧ������
//	else *APPLState = APPLON; 							//����Ӧ������	
//
//	return SendLength+2;

	ICEC.use_DK = 0;
	*APPLState = APPLIdle;

	return SendLength+2;	
}					
/*
//Ȩ�޹�����
//����Ȩ��0x10..
unsigned short GetRPublic_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<RPublic_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)RPublic_client[i].OBIS_Name, 9 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	if(i==RPublic_clientSum) i = 0xFFFF;	 //�������ܳ���0xffff..
	return i;	
}	
//
//ֻ��Ȩ��02..
unsigned short GetRReading_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<RReading_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)RReading_client[i].OBIS_Name, 9 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	if(i==RReading_clientSum) i = 0xFFFF;	 //�������ܳ���0xffff..
	return i;	
}	


unsigned short GetRPR_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<RPR_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)RPR_client[i].OBIS_Name, 9 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	if(i==RPR_clientSum) i = 0xFFFF;	 //�������ܳ���0xffff..
	return i;	
}
	
//ֻ����Ȩ��..
unsigned short GetWPR_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<WPR_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)WPR_client[i].OBIS_Name, 9 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	if(i==WPR_clientSum) i = 0xFFFF;	 //�������ܳ���0xffff..
	return i;	
}

//ֻ��ACTIONȨ��..
unsigned short GetAPR_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<APR_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)APR_client[i].OBIS_Name, 9 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	if(i==APR_clientSum) i = 0xFFFF;	 //�������ܳ���0xffff..
	return i;	
}	
*/

short AARQHProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short* RecDataPtr;
	short Length;
	unsigned char* Source;
	unsigned short SendLength=0;
	unsigned char* Dest;
	unsigned short ItemLen;
	unsigned short ErrByte=0;
	unsigned char* APPLState;
	unsigned char SecretState;
	
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	APPLState = CommPara[COM_No].APPLState;
	Length = *RecDataPtr - 3;
	Length -= 2;
	
	if( *(RecBuff+1) != Length ) return -1;			//�յ��ĳ��Ⱥ���Ϣ֡�еĳ��Ȳ��ȣ��Ƿ�

	*SendBuff = APDU_AARE;
	*(SendBuff+1) = 0;
	FillApplicationContextName( SendBuff+2 );
	SendLength = 11;
	
	Dest = SendBuff+13;
	
	//if(HComm.ClientAddr[COM_No]!=0x10)  // �ǹ���Ȩ�޶�ȡ��Ŀ..
//		{
				Source = RecBuff+2;
				ItemLen = 0;
				SecretState = 0;
				while( Length > 0 )
				{
			//		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen );	
					ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen, &SecretState );	
					if( ErrByte != 0 )
					{
						GetAAREResult_Diagnostic( COM_No,Dest, ErrByte );
						Dest += 12;
						SendLength += 12;
						break;
					}	
					Length -= ItemLen;
					Source += ItemLen;
				}		
				if( ErrByte == 0 )
				{
					GetAAREResult_Diagnostic( COM_No,Dest, ErrByte );
					if(crypt.aarqHLS[COM_No]== 0x55){
						 if(ICEC.use_SHA256 == 0x1)
						 	{
								Dest += 45;
								SendLength += 45;
						  }
						  else
							{
								Dest += 37;
								SendLength += 37;
							}
				 }else{
				 			Dest += 12;
							SendLength += 12;
					}
				}	
//	}

	//����systemtitle	
	if(ICEC.use_HLSname == 3)
	{
		  *(SendBuff+12) = 0x03;
		  SendLength += CreatUser_Systemtile( COM_No, Dest, ErrByte );   //03--����
      Dest += 12;       
  }
  else
  {
  	  // AARQ������һ��CLIENT_SYSTEMTILE��
  		E2P_RData( ICComm.ClientSystemTitle, E2P_client_system_title, 8 );			
  }
			
	SendLength += CreatUser_information( COM_No, Dest, ErrByte );
	*(SendBuff+1) = SendLength;
	
	if( ErrByte != 0 ) *APPLState = APPLIdle;			//δ����Ӧ������
	else *APPLState = APPLON; 							//����Ӧ������	
  
  //DK-FC AARQ֮��λ���0��ʼ..
  HComm.DK_FCRXCount = 0;
  HComm.DK_FCTXCount = 0;
  
	return SendLength+2;	
}	

short CompareAuthentication_HLS( unsigned char* Source )
{
	unsigned char Buff[20];
	unsigned char Buff2[10];
	//unsigned char Buff3[10];
	unsigned char* Ptr;
	unsigned char* Point;
	// unsigned char* password;
	short Len;

	Ptr = Buff;
	Point = Buff2;
	//password = Buff3;
	
	Len = *(Source+1);
	//if( Len != 10 ) return 1;
	if( Len > 18 ) return 1;
	
	if( *(Source+2) != 0x80 ) return 1;
	if( *(Source+3) != ( Len - 2 ) ) return 1;
	Len -= 2;
	
	crypt.Challenge_Len = Len;
	//Ŀǰֻ֧��8��16�ֽ�..
	if(Len == 8 )
	{
		//����ͻ��������..
		RAM_Write( crypt.C_Challenge, Source+4, 8 );   //���������,����TAG���бȶ�..
	}
	else
	{
		  if(Len != 16 ) return 1;		
			RAM_Write( crypt.C_Challenge, Source+4, 16 );   //���������,����TAG���бȶ�..
  }
	
	/*	//ȡESAM�����������..
		GetChallenge( ESAM, Ptr, 8 );
		
	  RAM_Write( Source+4, Ptr,  8 );  
	  //��������������..
	  RAM_Write( crypt.S_Challenge, Ptr,  8 );  
	*/
	
	/*
		RAM_Fill( Point, 10 );

		RAM_Write( Point, Source+4, Len );   //���������,����TAG���бȶ�..
		//����TAG..
	  //TEMP NULL
		//

		// ����8λ�����.. 50 36 77 52 4a 32 31 46 
	  *(Ptr) = 0x50;
	  *(Ptr+1) = 0x36;  
	  *(Ptr+2) = 0x77;
	  *(Ptr+3) = 0x52;
	  *(Ptr+4) = 0x4a;
	  *(Ptr+5) = 0x32;
	  *(Ptr+6) = 0x31;
	  *(Ptr+7) = 0x46;
	  RAM_Write( Source+4, Ptr,  8 );  
  */      
	return 0;
}

// ���ܽ��ܵ��ó���..
// ���ȵĵط����ܳ���0x80��������Ҫ����20160309�޸�������(��֡)..
int Decrypt_OR_Encrypt( unsigned short COM_No,unsigned char* Ciphertext,unsigned short *nCtextLen,unsigned char use_crypt )
{
//use_crypt:0���ܣ�1����
	return AES_GCM_Decrypt_OR_Encrypt_H(COM_No,Ciphertext,nCtextLen,use_crypt);

}




