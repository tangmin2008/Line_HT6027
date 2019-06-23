#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "ht6xxx_lib.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"

#include "Mem.h"
#include "Time.h"
#include "Power.h"

#include "Initial.h"
#include "ECProcess.h"

#include "imagetranfer.h"
#include "RegisterActivation.h"
#include "ComSetup.h"
#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "AssociationLN.h"
//#include "GetData.h"
#include "ProfileGeneric.h"
#include "ScriptTable.h"
#include "ActCalendar.h"
#include "SingleAction.h"
#include "Limiter.h"
#include "RegisterMonitor.h"
#include "DisconnectControl.h"
#include "Demand.h"

/*

#include "RegisterMonitor.h"


#include "DisconnectControl.h"




*/
/*
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif
#if ( IDTHarmonic == YesCheck )
#include "harmonic.h"
#endif
//#include "Event.h"
//#include "Esamcpu.h"
	 */
short CreatGetRequestNormalError( unsigned char* Dest, unsigned char InvokeId, unsigned char ErrorByte )
{
	*Dest = APDU_GET_Response;	
	*(Dest+1) = get_response_normal;
	*(Dest+2) = InvokeId;
	*(Dest+3) = Get_data_access_result;
	*(Dest+4) = ErrorByte;
    
  return 5;
}	

short Judge_Selective_access_Para( unsigned char* Dest )
{
	
	if( *Dest == 0 ) return 0;							//OptionFlag=0;
	if( *(Dest+1) == 1 )										//range_descriptor	
	{
		return 0;		
	}
	else if( *(Dest+1) == 2 )							//entry_descriptor
	{
		return 0;						
	}		 
	else return -1;	
	
}	

short JudgeClassPara( short COM_No, unsigned char* Dest, unsigned char* Source )
{
	short i;
	unsigned short ClassId;
	unsigned char InvokeId;
	unsigned char AttributeNum;
	unsigned char OptionFlag;
	unsigned char Len;
	unsigned short* RecDataPtr;
    
	InvokeId = *(Source+2);
	
	HComm.Send_state_InvokeId = InvokeId;
	  
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	if( *RecDataPtr < (3+13))					//E6+E6+00+C0+01+81+Classid(2)+Logical_name(6)+Attribute(1)+OPTION(1) = 16
	{
		CreatGetRequestNormalError( Dest, InvokeId, type_unmatched );					//���ݲ������ȴ�
		return -1;
	}	

	Len = *RecDataPtr;
	Len -= 3;
	
	ClassId = *(Source+3);
	ClassId *= 256;
	ClassId += *(Source+4);
	
	for( i=0;i<CosemParaSum;i++ )
	{
		if( ClassId == Cosem_ParaTab[i].ClassId ) break;								
	}	
	if( i == CosemParaSum ) 
	{
		CreatGetRequestNormalError( Dest, InvokeId, object_class_inconsistent );		//�������ݲ�֧��
		return -1;		
	}		
	AttributeNum = *(Source+2+9);
	
	//���Ӳ��Զ�ȡFLASH����sky2013....
	if((ClassId==Class_id_Image_transfer)&&((AttributeNum==0x11)||(AttributeNum==0x12))){		//��������ʹ��..
		if(Len >25) return -1;	
	}else{ 				
				if(( AttributeNum > Cosem_ParaTab[i].AttributeNum )||( AttributeNum == 0 ))			//����Ϊ0��֧�ֻ����Գ�
				{
					CreatGetRequestNormalError( Dest, InvokeId, type_unmatched );
					return -1;		
				}		
				OptionFlag = *(Source+2+10);
			//	if((ClassId!=Class_id_Profile_generic)&&(ClassId!=Class_id_Utility_tables)&&(ClassId!=Class_id_Association_SN)&&(ClassId!=Class_id_Association_LN))
				if((ClassId==Class_id_Profile_generic)&&(AttributeNum==2))			//ֻ��Profile_generic������2���ܴ���ѡ���Է����⣬�������֧��ѡ���Է���
				{
			//		if(((OptionFlag==0)&&(Len>13))||( Len>=SUBS_PARA_LEN)||(Judge_Selective_access_Para(Source+2+10)!=0))	//������ʽ������Ƿ�
					if(((OptionFlag==0)&&(Len>13))||(Judge_Selective_access_Para(Source+2+10)!=0))	//������ʽ������Ƿ�
					{
						CreatGetRequestNormalError( Dest, InvokeId, type_unmatched );
						return -1;				
					}	
				}	
				else
				{
					if(( OptionFlag != 0 )||( Len > 13 ))											//�����������Ϳ��ܴ���ѡ���Է����⣬�������Ͷ�Ϊ0�����ȹ̶�Ϊ13�ֽڡ�
					{
						CreatGetRequestNormalError( Dest, InvokeId, type_unmatched );
						return -1;		
					}	
				} 
	}			
	RAM_Fill( CommPara[COM_No].SubsBlockPara, SUBS_PARA_LEN );
//	RAM_Write( CommPara[COM_No].SubsBlockPara, Source, Len );
	RAM_Write( CommPara[COM_No].SubsBlockPara, Source, 20 );
	*CommPara[COM_No].BlockNumber = 0;
  return 0;
}	

short CreatSetRequestNormalError( unsigned char* Dest, unsigned char InvokeId, unsigned char ErrorByte )
{
	*Dest = APDU_SET_Response;	
	*(Dest+1) = set_response_normal;
	*(Dest+2) = InvokeId;
	*(Dest+3) = ErrorByte;
    
    return 4;
}	

short JudgeSetClassPara( short COM_No, unsigned char* Dest, unsigned char* Source )
{
	short i;
	unsigned short ClassId;
	unsigned char InvokeId;
	unsigned char AttributeNum;
	unsigned char OptionFlag;
	unsigned char Len;
	unsigned short* RecDataPtr;
    
	InvokeId = *(Source+2);
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	if( *RecDataPtr < (3+13))					//E6+E6+00+C1+01+81+Classid(2)+Logical_name(6)+Attribute(1)+OPTION(1)+"Block_Number" = 16+4
	{
		CreatSetRequestNormalError( Dest, InvokeId, type_unmatched );					//���ݲ������ȴ�
		return -1;
	}	

//	if((*(CommPara[COM_No].Conformance+2) & 0x08 ) == 0 )		//SET�����Ƿ�֧��
//	{
//		CreatSetRequestNormalError( Dest, InvokeId, read_write_denied );				//����дȨ�޴�
//		return -1;
//	}	

	Len = *RecDataPtr;
	Len -= 3;
	
	ClassId = *(Source+3);
	ClassId *= 256;
	ClassId += *(Source+4);
	
	for( i=0;i<CosemParaSum;i++ )
	{
		if( ClassId == Cosem_ParaTab[i].ClassId ) break;								
	}	
	if( i == CosemParaSum ) 
	{
		CreatSetRequestNormalError( Dest, InvokeId, object_class_inconsistent );		//�������ݲ�֧��
		return -1;		
	}
	
	// ��ʱ�޸�sky20140730..
	if(((*(CommPara[COM_No].Conformance+2) & 0x08 ) == 0 )&&(ClassId!=0x08))		//SET�����Ƿ�֧��
	{
		CreatSetRequestNormalError( Dest, InvokeId, read_write_denied );				//����дȨ�޴�
		return -1;
	}	
	//end..
		
	AttributeNum = *(Source+2+9);
	if(( AttributeNum > Cosem_ParaTab[i].AttributeNum )||( AttributeNum == 0 ))			//����Ϊ0��֧�ֻ����Գ�
	{
		CreatSetRequestNormalError( Dest, InvokeId, type_unmatched );
		return -1;		
	}		
	OptionFlag = *(Source+2+10);
	if( OptionFlag != 0 )											//��֧��ѡ���Է���д���ݡ�
	{
		CreatSetRequestNormalError( Dest, InvokeId, type_unmatched );
		return -1;		
	}	
	RAM_Fill( CommPara[COM_No].SubsBlockPara, SUBS_PARA_LEN );
	RAM_Write( CommPara[COM_No].SubsBlockPara, Source, MAC_Set_First_Block_Data );
  return 0;
}	

unsigned char* JudgeSetBlockData( unsigned char* Data, unsigned short Len, unsigned short* DataLen, unsigned char* ErrorByte )
{
	unsigned char* Ptr;
		
	Ptr = Data;
	if(( *Data & 0x80 ) != 0 )										//�������ݳ����Ƿ�Ϊ����1�ֽ�
	{                                               				
		if( *Data == 0x81 )											//�������ݳ����Ƿ�Ϊ1�ֽ� )
		{
			*DataLen = *(Data+1);									//��Ч�������ݳ���
			Ptr = Data+2;											//��Ч����������ʼ��ַ			
		}			
		else if( *Data != 0x82 )									//�������ݳ����Ƿ�Ϊ2�ֽ�
		{                                           				
			*ErrorByte = other_reason;								
		}                                           				
		else                                        				
		{                                           				
			*DataLen = GetBuffToInteger( Data+1 );					//��Ч�������ݳ���
			Ptr = Data+3;											//��Ч����������ʼ��ַ
		}			                                				
	}                                               				
	else                                            				
	{	                                            				
		*DataLen = *Data;											//��Ч�������ݳ���    
		Ptr = Data+1;				                				//��Ч����������ʼ��ַ
	}				
	if( *DataLen != (Len -(Ptr-Data)) ) *ErrorByte = other_reason;	//���ݳ��ȳ���							

	return Ptr;	
}	

//Type=0: �յ���һ��
//Type!=0: ������
#if 0
short SetRequestWithDataBlockProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff, short Type )
{
	unsigned char* Ptr;
	unsigned short DataLen=0;
	unsigned char ErrorByte=success;
	unsigned long* BlockNumber;
	unsigned short Len;
	unsigned char* SetBlockBuff;
	unsigned short* SetBlockPtr;
	unsigned char* Data_Last_Block;										//�������������־
	unsigned char* Data_Block_Number;									//�������ݿ����
	unsigned char* Data_Block_Data;										//����������ʼ��ַ
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	SetBlockBuff = CommPara[COM_No].SetBlockBuff;
	SetBlockPtr = CommPara[COM_No].SetBlockPtr;
	
	*SendBuff = APDU_SET_Response;
	*(SendBuff+1) = set_response_datablock;
	*(SendBuff+2) = *(RecBuff+2);										//InvokeId
                                                    					
	if( Type == 0 )														//��һ��
	{
			*BlockNumber = 1;
			RAM_Write( SetBlockBuff, RecBuff+MAC_Get_Set_ClassId, 10 );     //�������ö����Է��뻺��            			
			*SetBlockPtr = 10;
			Data_Last_Block = RecBuff+MAC_Set_First_Last_Block;				//�������������־
			Data_Block_Number = RecBuff+MAC_Set_First_Block_Number;          //�������ݿ����    
			Data_Block_Data = RecBuff+MAC_Set_First_Block_Data;		        //����������ʼ��ַ  
			Len = MAC_Set_First_Block_Data + 3;								//�����ֽ�ǰ��21���ֽ�
	}
	else 										//������
	{
			*BlockNumber += 1;
			Data_Last_Block = RecBuff+MAC_Set_Last_Block;
			Data_Block_Number = RecBuff+MAC_Set_Block_Number;
			Data_Block_Data = RecBuff+MAC_Set_Block_Data;
			Len = MAC_Set_Block_Data + 3;									//�����ֽ�ǰ��11���ֽ�
	}	

	if( GetBuffToLong( Data_Block_Number ) != *BlockNumber ) ErrorByte = other_reason;	//���õĿ�Ų���		
	
	if( ErrorByte == success )
	{
			Ptr = JudgeSetBlockData( Data_Block_Data, (*CommPara[COM_No].RecDataPtr)-Len, &DataLen, (unsigned char*)&ErrorByte );
			if(( DataLen + (*SetBlockPtr) ) > MAX_BLOCK_SIZE ) ErrorByte = other_reason;	//�����ݳ��Ȳ��ܳ���
  }
	
	if( ErrorByte == success )
	{
			RAM_Write( SetBlockBuff+(*SetBlockPtr), Ptr, DataLen );			//������������������û�����
			*SetBlockPtr += DataLen;		                                            			
			if( *Data_Last_Block != 0 )										//�������һ�飬���Խ������ݴ���		
			{
				ErrorByte = SetClassData( COM_No, GetBuffToInteger( SetBlockBuff ), SetBlockBuff, *SetBlockPtr-10 );		//��������
			}	
	}	
	if(( ErrorByte != success )||( *Data_Last_Block != 0 ))
	{
		*(SendBuff+1) = set_response_last_datablock;									//���������̽���
		*(SendBuff+3) = ErrorByte;                                    //���ش����־
		RAM_Write( SendBuff+4, Data_Block_Number, 4 );             		//�������յ��Ŀ��	
		Len = 8;                                                                    			
	}                                                                               			
	else                                                                            			
	{                                                                               			
		RAM_Write( SendBuff+3, Data_Block_Number, 4 );             		//�������յ��Ŀ��	
		Len = 7;                                                                    			
	}		                                                                        			
	if(( ErrorByte != success )||( *Data_Last_Block != 0 ))				//�������һ����д��������		
	{
		*BlockNumber = 0;
		RAM_Fill( CommPara[COM_No].SubsBlockPara, SUBS_PARA_LEN );
		*SetBlockPtr = 0;
		RAM_Fill( SetBlockBuff, 10 );
	}	
    return Len;	
}	
#endif

short SetRequestWithDataBlockProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff, short Type )
{
	unsigned char* Ptr;
	unsigned short DataLen=0;
	unsigned char ErrorByte=success;
	unsigned long* BlockNumber;
	unsigned short Len;
	unsigned char* SetBlockBuff;
	unsigned short* SetBlockPtr;
	unsigned char* Data_Last_Block;										//�������������־
	unsigned char* Data_Block_Number;									//�������ݿ����
	unsigned char* Data_Block_Data;										//����������ʼ��ַ
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	SetBlockBuff = CommPara[COM_No].SetBlockBuff;
	SetBlockPtr = CommPara[COM_No].SetBlockPtr;
	*SendBuff = APDU_SET_Response;
	*(SendBuff+1) = set_response_datablock;
	*(SendBuff+2) = *(RecBuff+2);										//InvokeId
	if( Type == 0 )														//��һ��
	{
			*BlockNumber = 1;
			RAM_Write( SetBlockBuff, RecBuff+MAC_Get_Set_ClassId, 10 );     //�������ö����Է��뻺��    
		///	DataFlash_Write(Test_Sector, RecBuff+MAC_Get_Set_ClassId, Test_Sector,Test_end,10);//---------2016-5-28--------
			*SetBlockPtr = 10;
			Data_Last_Block = RecBuff+MAC_Set_First_Last_Block;				//�������������־
			Data_Block_Number = RecBuff+MAC_Set_First_Block_Number;          //�������ݿ����    
			Data_Block_Data = RecBuff+MAC_Set_First_Block_Data;		        //����������ʼ��ַ  
			Len = MAC_Set_First_Block_Data + 3;								//�����ֽ�ǰ��21���ֽ�
	}
	else 										//������
	{
			*BlockNumber += 1;
			Data_Last_Block = RecBuff+MAC_Set_Last_Block;
			Data_Block_Number = RecBuff+MAC_Set_Block_Number;
			Data_Block_Data = RecBuff+MAC_Set_Block_Data;
			Len = MAC_Set_Block_Data + 3;									//�����ֽ�ǰ��11���ֽ�
	}	

	if( GetBuffToLong( Data_Block_Number ) != *BlockNumber ) ErrorByte = other_reason;	//���õĿ�Ų���		
	if( ErrorByte == success )
	{
			Ptr = JudgeSetBlockData( Data_Block_Data, (*CommPara[COM_No].RecDataPtr)-Len, &DataLen, (unsigned char*)&ErrorByte );//ȡ���ݿ�����
			if(( DataLen + (*SetBlockPtr) ) > MAX_BLOCK_SIZE ) ErrorByte = other_reason;	//�����ݳ��Ȳ��ܳ���
  }
	
	if( ErrorByte == success )
	{
			RAM_Write( SetBlockBuff+(*SetBlockPtr), Ptr, DataLen );			//������������������û�����			
		*SetBlockPtr += DataLen;		                                            			
			if( *Data_Last_Block != 0 )										//�������һ�飬���Խ������ݴ���		
			{
				ErrorByte = SetClassData( COM_No, GetBuffToInteger( SetBlockBuff ), SetBlockBuff, *SetBlockPtr-10 );		//��������
			}
	}	
	if(( ErrorByte != success )||( *Data_Last_Block != 0 ))
	{
		*(SendBuff+1) = set_response_last_datablock;									//���������̽���
		*(SendBuff+3) = ErrorByte;                                    //���ش����־

		RAM_Write( SendBuff+4, Data_Block_Number, 4 );             		//�������յ��Ŀ��	
		Len = 8;                                                                    			
	}                                                                               			
	else                                                                            			
	{                                                                               			
		RAM_Write( SendBuff+3, Data_Block_Number, 4 );             		//�������յ��Ŀ��	
		Len = 7;                                                                    			
	}		                                                                        			
	if(( ErrorByte != success )||( *Data_Last_Block != 0 ))				//�������һ����д��������		
	{
		*BlockNumber = 0;
		RAM_Fill( CommPara[COM_No].SubsBlockPara, SUBS_PARA_LEN );
		*SetBlockPtr = 0;
	//	RAM_Fill( SetBlockBuff, 10 );	
	}	
    return Len;	
}	
unsigned char SetClassData( unsigned char COM_No, unsigned short ClassId, unsigned char* Data, unsigned short DataLen )
{
	unsigned char ErrorByte=object_class_inconsistent;

	switch( ClassId )
	{
		case Class_id_Data:								//1
			ErrorByte = SetDataClassData( COM_No,Data, DataLen ); 
			break;
		case Class_id_Register:							//3
			ErrorByte = SetRegisterClassData( Data, DataLen ); 
			break;
		case Class_id_Extended_register:				//4
//			ErrorByte = SetExtendedRegisterClassData( RecBuff+16, Logical_Name, Attribute ); 
			break;
		case Class_id_Demand_register:					//5
			ErrorByte = SetDemand_registerClassData( Data, DataLen ); 
			break;
		case Class_id_Register_activation:				//6
		//	ErrorByte = SetRegister_ActivationClassData( Data, DataLen ); 
			break;
		case Class_id_Profile_generic:					//7
			ErrorByte = SetProfile_genericClassData( COM_No, Data, DataLen ); 
			break;
//		case Class_id_Utility_tables:					//26
//			break;
//		case Class_id_Register_table:					//61
//			break;
//		case Class_id_Status_mapping:					//63
//			break;
//		case Class_id_Association_SN:					//12
//			break;
		case Class_id_Association_LN:					//15
			ErrorByte =  SetAssociation_LNClassData( COM_No, Data, DataLen ); 
			break;
		case Class_id_SAP_assignment:					//17
			ErrorByte = SetSAP_assignmentClassData( Data, DataLen ); 
			break;
		case Class_id_Clock:							//8
			ErrorByte = SetClockClassData( Data, DataLen ); 
			break;
		case Class_id_Script_table:						//9
			ErrorByte = SetScript_TableClassData( Data, DataLen ); 
			break;
//		case Class_id_Schedule:							//10
//			break;
		case Class_id_Special_days_table:				//11
			ErrorByte = SetSpecial_daysClassData( Data, DataLen ); 
			break;
		case Class_id_Activity_calendar:				//20
			ErrorByte = SetActivity_calendarClassData( Data, DataLen ); 
			break;
		case Class_id_Register_monitor:					//21
			ErrorByte = SetRegister_monitorClassData( Data, DataLen ); 
			break;
		case Class_id_Single_action_schedule:			//22
			ErrorByte = SetSingle_action_scheduleClassData( Data, DataLen ); 
			break;
		case Class_id_Disconnect_control:				//70
			ErrorByte = SetDisconnect_controlClassData( Data, DataLen ); 
			break;
		case Class_id_Limiter:							//71
			ErrorByte = SetLimiterClassData( Data, DataLen ); 
			break;
		case Class_id_Image_transfer:				//18
			ErrorByte = SetImage_transferClassData( Data, DataLen ); 
			break;			
		case Class_id_IEC_local_port_setup:				//19
			ErrorByte = SetIECLocalPortSetupClassData( Data, DataLen ); 
			break;
		case Class_id_IEC_HDLC_setup:					//23		
			ErrorByte = SetIECHDLCSetupClassData( Data, DataLen ); 
			break;
			
		case Class_id_Auto_answer_setup:					//28		
			ErrorByte = Set_Auto_answer_setupClassData( Data, DataLen ); 
			break;
		case Class_id_Auto_connect_setup:					//29		
			ErrorByte = Set_Auto_connect_setupClassData( Data, DataLen ); 
			break;
		case Class_id_Push_Setup:					//40		
			ErrorByte = Set_PushsetupClassData( Data, DataLen ); 
			break;
		
		case Class_id_TCP_UDP_setup:					//41		
			ErrorByte = SetTCP_UDP_setupClassData( Data, DataLen ); 
			break;
	  case Class_id_IPv4_setup:						//42		
			ErrorByte = SetIPv4_setupClassData( Data, DataLen );
			break;
	  case Class_id_MACaddr_GPRS:						//43		
			ErrorByte =  SetMACaddr_GPRSClassData( Data, DataLen );
			break;
		case Class_id_GPRS_modem_setup:					//45		
			ErrorByte = SetGPRS_modem_setupClassData( Data, DataLen ); 
			break;
		case Class_id_SMTP_setup:								//46		
			ErrorByte = SetSMTP_setupClassData( Data, DataLen ); 
			break;			
		case Class_id_Security_setup:								//64		
			ErrorByte = SetSecurity_setupClassData( Data, DataLen ); 
			break;	
		default: 
			break;			
	}	
	return ErrorByte;
}	


short SetRequestNormalProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short ClassId;
	unsigned char InvokeId;
	unsigned short DataLen;
	unsigned char ErrorByte=object_class_inconsistent;
	
	InvokeId = *(RecBuff+2);

	ClassId = *(RecBuff+3);
	ClassId *= 256;
	ClassId += *(RecBuff+4);

	*SendBuff = APDU_SET_Response;
	*(SendBuff+1) = set_response_normal;
	*(SendBuff+2) = InvokeId;

	DataLen = *CommPara[COM_No].RecDataPtr - 16;

	ErrorByte = SetClassData( COM_No, ClassId, RecBuff+3, DataLen );

	*(SendBuff+3) = ErrorByte;

    return 4;	
}	

unsigned char SetDataClassData(unsigned char COM_No, unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[12];
	unsigned char ErrorByte=0;
	unsigned short Addr;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
    unsigned char* Ptr;
	unsigned char* Point;
	short i,j;
	short Len;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	i = GetDataClassTabItemNo( Logical_Name );						//���Ҷ�Ӧ��OBIS��
	if( i == DLMSDataSum ) { return object_undefined; }
	//if(( *Attribute != 2 )||( DLMS_DATATab[i].ComInfo & WRITE ) == 0 ) { return read_write_denied; }
	
//	if(( *Attribute != 2 )||( DLMS_DATATab[i].State& Write_All ) == 0 ) { return read_write_denied; }
	if( *Attribute != 2 ) 
		 return read_write_denied; 
	if((HComm.ClientAddr[COM_No]==0x02)||(HComm.ClientAddr[COM_No]==0x09))
		{
		if((DLMS_DATATab[i].Acess&Write_A2)==0)
			return read_write_denied; 
		}
	else if((HComm.ClientAddr[COM_No]==0x03)||(HComm.ClientAddr[COM_No]==0x0a))
		{
			if((DLMS_DATATab[i].Acess&Write_A3)==0)
				return read_write_denied;
		}
	else if((HComm.ClientAddr[COM_No]==0x04)||(HComm.ClientAddr[COM_No]==0x0b)||(HComm.ClientAddr[COM_No]==0x01))//0x01�������ǵ����ÿ�
		{
			if((DLMS_DATATab[i].Acess&Write_A4)==0)
				return read_write_denied;
		}
	else if((HComm.ClientAddr[COM_No]==0x05)||(HComm.ClientAddr[COM_No]==0x0c))
		{
			if((DLMS_DATATab[i].Acess&Write_A5)==0)
				return read_write_denied;
		}
	else
		return read_write_denied;
	

	if( DLMS_DATATab[i].DataType != *DataType ) { return type_unmatched; }
	Len = DLMS_DATATab[i].ComInfo & RS_LEN;	
	Addr = DLMS_DATATab[i].Addr;
	if((*DataType == Data_array)||(*DataType ==Data_structure)){
		if(*DataType ==Data_structure){
			if(Len==2){
				 Ptr = WriteBufAds + 1;
				 if((*Ptr!=2)||((*(Ptr+1))!= D_ENUM)||((*(Ptr+3))!= Data_bit_string)) return other_reason;
				  // GPRS -1
				 	E2P_WData( DLMS_DATATab[i].Addr, Ptr+2, 1 );	
				 	E2P_WData( DLMS_DATATab[i].Addr+2, Ptr+5, 4 );	
			}
		}else{
		if((*(Logical_Name) ==1)&&(*(Logical_Name+1) ==65)&&(*(Logical_Name+2) ==0)&&((*(Logical_Name+3) ==134)||(*(Logical_Name+3) ==135))&&(*(Logical_Name+4) ==0)&&(*(Logical_Name+5) ==255))
			{//̨��Բ��������ʾ��
			Ptr = WriteBufAds + 1;
			E2P_WData( Addr, Ptr, 1 );
			Ptr+=1;
			Addr+=2;
			for(i=0; i<*(WriteBufAds + 1); i++)
				{
				Point = Buff;
				if((*Ptr!=Data_structure)&&(*(Ptr+1)!=4)&&(*(Ptr+2)!=D_Unsigned16)) return other_reason;
				RAM_Write(Point,Ptr+3,2);
				if((*(Ptr+5)!=D_OctetStr)&&(*(Ptr+6)!=6)) return other_reason;
				RAM_Write(Point+2,Ptr+7,6);
				if(*(Ptr+13)!=D_Integer8) return other_reason;
				RAM_Write(Point+8,Ptr+14,1);
				if(*(Ptr+15)!=D_Unsigned16) return other_reason;
				RAM_Write(Point+9,Ptr+16,2);				
				E2P_WData( Addr, Point, 11 );
				Addr+=12;
				Ptr+=18;
				}

			}
				/*	if( DataLen > Len*6 ) { return type_unmatched; }					//���ȳ���
						  Ptr = WriteBufAds + 1;
						  if((*(Logical_Name+1) ==1)||(*(Logical_Name+1) ==2)||(*(Logical_Name+1) ==4))
						  //if((*(Logical_Name+4) ==3)||(*(Logical_Name+4) ==5)||(*(Logical_Name+4) ==6)||(*(Logical_Name+4) ==7))
							{
									for( j=0;j<Len;j++ )
									{
										if(((*Ptr+1)!=Data_structure)||(*(Ptr+2)!= 1)||(*(Ptr+3)!= 0x09)||(*(Ptr+4)!= 2))					//
										{
											return other_reason;
										}else{
														E2P_WData( DLMS_DATATab[i].Addr+j*3, Ptr+4, 2 );
													}		
										Ptr += 6;	
									}
							}
							else
							{
									for( j=0;j<Len;j++ )								//��������--��ѹ�ϸ���..
									{
										if((*Ptr+1)!=D_Unsigned32)					//
										{
											return other_reason;
										}else{
														E2P_WData( DLMS_DATATab[i].Addr+j*5, Ptr+2, 4 );
													}		
										Ptr += 5;	
									}					
						  }
				*/
			}				
	}else{			
//				DataLen -= 2;
//				if( DataLen > Len ) { return type_unmatched; }					//���ȳ���
//				if(( *DataType == D_VisibleStr )||( *DataType == D_OctetStr ))
//				{						
//						if(( DataLen < Len )&&(( *DataType == D_VisibleStr )||( *DataType == D_OctetStr )))
//						{
//							RAM_Fill( WriteBufAds+2+DataLen, Len-DataLen );
//						}	
//						E2P_WData( DLMS_DATATab[i].Addr, WriteBufAds+2, Len );
//				}else{
//							if(DataLen != Len) ErrorByte = type_unmatched;
//							else E2P_WData( DLMS_DATATab[i].Addr, WriteBufAds+1, Len );
//						 }
					if(( *DataType == D_VisibleStr )||( *DataType == D_OctetStr )||(*DataType == Data_UTF8_string))
					{
						DataLen -= 2;
						
						//////У������---fire////////
						if(i<17)
							{
						/*	Com_HT7017.Comm_Enable = DISABLE;
							Point = Buff;
						//	NVIC_DisableIRQ(TIMER_2_IRQn); 
							Buff[0]=0x00;
							Buff[1]=0xA6;
							UART_Write(WPREG_Address,Point);	//ʹ��50-71д����//
							Delay(Measur_Delay);
						//	Delay(Measur_Delay);
							RAM_Write(Point,WriteBufAds+2,2);
							if(DLMS_DATATab[i].Addr==I2Gain)
								UART_Write(I2Gain_Address,Point);
							else if(DLMS_DATATab[i].Addr==HFCONST)
								UART_Write(HFConst_Address,Point);
							else if(DLMS_DATATab[i].Addr==AGPBase)
								{
								UART_Write(GP1,Point);
								Delay(Measur_Delay);
						//		Delay(Measur_Delay);
								UART_Write(GQ1,Point);
								Delay(Measur_Delay);
						//		Delay(Measur_Delay);
								UART_Write(GS1,Point);
								}
							else if(DLMS_DATATab[i].Addr==AGPhs)
								UART_Write(GPhs1,Point);
							else if(DLMS_DATATab[i].Addr==E2AP1OFFSET)
								{
								*Point = 0;
								*(Point+1) = *(WriteBufAds+2);
								UART_Write(P1OFFSETH_Address,Point);//���ֽ�
								Delay(Measur_Delay);
						//		Delay(Measur_Delay);
								*(Point+1) = *(WriteBufAds+3);
								UART_Write(P1OFFSET,Point);//���ֽ�
								}
							else if(DLMS_DATATab[i].Addr==AGPBase2)
								{
								UART_Write(GP2_Address,Point);
								Delay(Measur_Delay);
						//		Delay(Measur_Delay);
								UART_Write(GQ2_Address,Point);
								Delay(Measur_Delay);
						//		Delay(Measur_Delay);
								UART_Write(GS2_Address,Point);
								}
							else if(DLMS_DATATab[i].Addr==AGPhs2)
								{
								UART_Write(GPhs2,Point);
								}
					//		else							
					//		{
							SwapData(WriteBufAds+2,Len);
							E2P_WAdj( DLMS_DATATab[i].Addr, WriteBufAds+2, Len );	
					//		SOCInit();//2016-9-1-fire
					//		HT7017Init();
						//	}
					Delay(Measur_Delay);
				//	Delay(Measur_Delay);
							
							Buff[0]=0x00;
							Buff[1]=0x00;
							UART_Write(WPREG_Address,Point);	//�ر�ʹ��	
							
				//			Delay(Measur_Delay);
				//			Delay(Measur_Delay);
					//		NVIC_EnableIRQ(TIMER_2_IRQn); 
							InitPara7();
				*/
						}
						//��������ȷ�ϱ�־-�жϰ汾���Ƿ���ȷ..
						if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==13)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))
						{
								Ptr = WriteBufAds+1;
								// �Ժ����Ӱ汾�ж���Ϣ..
							//	if((*Ptr ==7 )&&(*(Ptr+7)==0x01))	 Flag.New_check_ok = 0x58; //����ȷ��..
						}
						//���1024�ֽڵ���Ϣ��������..
						if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==99)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))
						{
							Ptr = WriteBufAds+1;							
							if(( *Ptr & 0x80 ) != 0 )										//�������ݳ����Ƿ�Ϊ����1�ֽ�
							{                                               				
								if( *Ptr == 0x81 )											//�������ݳ����Ƿ�Ϊ1�ֽ� )
								{
									Len = *(Ptr+1);									//��Ч�������ݳ���
									Ptr = WriteBufAds+3;							//��Ч����������ʼ��ַ
									DataLen -= 1;			
								}			
								else if( *Ptr != 0x82 )									//�������ݳ����Ƿ�Ϊ2�ֽ�
								{                                           				
									ErrorByte = other_reason;								
								}                                           				
								else                                        				
								{                                           				
									Len = GetBuffToInteger( Ptr+1 );					//��Ч�������ݳ���
									Ptr = WriteBufAds+4;											//��Ч����������ʼ��ַ
									DataLen -= 2;
								}			                                				
							}                                               				
							else                                            				
							{	                                            				
									Len = *Ptr;														//��Ч�������ݳ���    
									Ptr = WriteBufAds+2;				                				//��Ч����������ʼ��ַ
							}	
																	
							if(( DataLen > 1024 )||(DataLen!=Len)) { return type_unmatched; }					//���ȳ���
								
							FillIntegerToBuff(Buff,DataLen);
							E2P_WData( DLMS_DATATab[i].Addr, Buff, 2 );	//��Ϣ�ܳ���..
							for(j=0;j<10;j++)
							{
								if(DataLen>128){
									 DataLen = DataLen -128;
									 E2P_WData( DLMS_DATATab[i].Addr+3+j*129, Ptr+j*128, 128 );	//��Ϣ�ܳ���..
								}else{
									E2P_WData( DLMS_DATATab[i].Addr+3+j*129, Ptr+j*128, DataLen );	//��Ϣ�ܳ���..
									break;
								}								
							}		
						}else{
									if( DataLen > Len ) { return type_unmatched; }					//���ȳ���	
									if( DataLen < Len )
									{
										RAM_Fill( WriteBufAds+2+DataLen, Len-DataLen );
									}	
									E2P_WData( DLMS_DATATab[i].Addr, WriteBufAds+2, Len );	
						 }
					}	
					else
					{
						DataLen -= 1;				
						if( DataLen != Len ) { return type_unmatched; }					//���ȴ�
						SwapData( WriteBufAds+1, Len );	
						E2P_WData( DLMS_DATATab[i].Addr, WriteBufAds+1, Len );			
					}	
		}	
	return ErrorByte;
}	

unsigned short GetECTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;				
	
	for( i=0;i<DLMS_ECSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_ECTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;
}

unsigned short GetCUMDTabItemNo( unsigned char* Logical_Name )
{

	unsigned short i;				
	
	for( i=0;i<DLMS_CUMDSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_CUMDTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	return i;
	
}

unsigned short GetREALTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;				
	
	for( i=0;i<DLMS_RealSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_REALTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;
}

unsigned short GetRegTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;				
	
	for( i=0;i<DLMS_RegSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_RegTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;
}


unsigned char SetRegisterClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	short i;
	short Len;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
/*
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
	ErrorByte = IDTComAdjWrite( WriteBufAds+1, Logical_Name, Attribute ); 
	if( ErrorByte != 0xFF ) return ErrorByte;			//IDTУ������
	else
	{
		ErrorByte = 0;
	}	
#endif
*/	
	i = GetECTabItemNo( Logical_Name );				//�������			//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_ECSum ) { return read_write_denied; }

	i = GetCUMDTabItemNo( Logical_Name );			//�ۻ��������		//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_CUMDSum ) { return read_write_denied; }

	i = GetREALTabItemNo( Logical_Name );			//ʵʱ�������		//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_RealSum ) { return read_write_denied; }
/*
#if ( IDTHarmonic == YesCheck )
	i = GetHRTabItemNo( Logical_Name );				//г���������		//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_HRSum ) { return read_write_denied; }
#endif
*/
	i = GetRegTabItemNo( Logical_Name );			//ʵʱ�������		//���Ҷ�Ӧ��OBIS��
	if( i == DLMS_RegSum ) { return object_undefined; }
	
	if(( *Attribute != 2 )||(( DLMS_RegTab[i].State & Write_All ) == 0 )) { return read_write_denied; }
	if( DLMS_RegTab[i].DataType != *DataType ) { return type_unmatched; }
	Len = DLMS_RegTab[i].ComInfo & RS_LEN;
	if(( *DataType == D_VisibleStr )||( *DataType == D_OctetStr ))
	{
		DataLen -= 2;				
		if( DataLen > Len ) { return type_unmatched; }					//���ȳ���	
		if( DataLen < Len )
		{
			RAM_Fill( WriteBufAds+2+DataLen, Len-DataLen );
		}	
		E2P_WData( DLMS_RegTab[i].Addr, WriteBufAds+2, Len );	
	}	
	else
	{
		DataLen -= 1;				
		if( DataLen != Len ) { return type_unmatched; }					//���ȴ�
		SwapData( WriteBufAds+1, Len );	
		E2P_WData( DLMS_RegTab[i].Addr, WriteBufAds+1, Len );			
	}	
	return ErrorByte;
}	


unsigned char SetSAP_assignmentClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	unsigned char AccessMode;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	AccessMode = SAP_assignmentAttribute_ParaTab[(*Attribute)-1].AccessMode;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//�޶�Ӧ��OBIS��
	}	
	else if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//��д��Ȩ��
	}	 
	else if( *DataType != SAP_assignmentAttribute_ParaTab[(*Attribute)-1].DataType )
	{
		ErrorByte = type_unmatched; 									//��д��Ȩ��
	}	 
	else
	{
		switch( *Attribute )
		{
			case 2:								//time
				Buff[0] = 0x01;					//����
				Buff[1] = Data_structure;
				Buff[2] = 0x02;					//structure ��2Ԫ��
				Buff[3] = D_Unsigned16;
				if(( Data_Comp( Buff, WriteBufAds+1, 4 ) != 0 )||( *(WriteBufAds+7) != D_OctetStr )||( *(WriteBufAds+8) != 0x10 )) 
				{
					ErrorByte = other_reason;
					break;	
				}	
				SwapData( WriteBufAds+5, 2 );
				E2P_WData( ESAP, WriteBufAds+5, 2 );
				E2P_WData( Logical_device_name, WriteBufAds+9, 16 );
				break;	
			default:
				ErrorByte = type_unmatched;			
				break;			
		}
	}
	return ErrorByte;
}	

short Judge_Clock_Time( unsigned char* Data )
{
	unsigned short Temp;
	
	if( *Data != D_OctetStr ) 
	{
		return type_unmatched;
	}			
	if( *(Data+1) != 12 ) 
	{
		return other_reason;
	}

	Temp = *(Data+2)*256;
	Temp +=	*(Data+3);
	if(( Temp == 0xFFFF )
		||( *(Data+4) == 0 )||( *(Data+4) > 12 )
		||( *(Data+5) == 0 )||( *(Data+5) > 31 )
//		||( *(Data+6) == 0 )||( *(Data+6) > 7 )
		||( *(Data+6) > 7 )
		||( *(Data+7) > 23 )
		||( *(Data+8) > 59 )
		||( *(Data+9) > 59 ))
//		||( *(Data+10) > 99 ))   //����Щ��255--δ�涨�ĵط�..
	{
		return other_reason;				
	}		
		
/*		if( *Data != Data_DateTime ) 
		{
			return type_unmatched;
		}			
	
		Temp = *(Data+1)*256;
		Temp += *(Data+2);
		if(( Temp >2100 )
			||( *(Data+3) == 0 )||( *(Data+3) > 12 )
			||( *(Data+4) == 0 )||( *(Data+4) > 31 )
	//		||( *(Data+6) == 0 )||( *(Data+6) > 7 )
			||( *(Data+5) > 7 )
			||( *(Data+6) > 23 )
			||( *(Data+7) > 59 )
			||( *(Data+8) > 59 ))
	//		||( *(Data+10) > 99 ))	 //����Щ��255--δ�涨�ĵط�..
		{
			return other_reason;				
		}		
*/
	return 0;	
}	

unsigned char SetClockClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	unsigned char AccessMode;
	unsigned short Clock_Year,Temp_clock_time_shift;
//	short Temp2;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	AccessMode = ClockClassAttribute_ParaTab[(*Attribute)-1].AccessMode;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x01;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//�޶�Ӧ��OBIS��
	}	
	else if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//��д��Ȩ��
	}	 
	else if( *DataType != ClockClassAttribute_ParaTab[(*Attribute)-1].DataType )
	{
		ErrorByte = type_unmatched; 									//��д��Ȩ��
	}	 
	else
	{
		switch( *Attribute )
		{
			case 2:									//time
				ErrorByte = Judge_Clock_Time( WriteBufAds );
				if( ErrorByte == 0 )
//				if( *(WriteBufAds+1) == 12 )
				{
				/*
//					Temp = *(WriteBufAds+2)*256;
//					Temp +=	*(WriteBufAds+3);
//					if( Temp != 0xFFFF )
//					{
//						Clk.YearH = *(WriteBufAds+2);
//						Clk.YearL = *(WriteBufAds+3);
//					}	
//					if(( *(WriteBufAds+4) >= 1 )&&( *(WriteBufAds+4) <= 12 ))
//					{
//						Clk.Month = *(WriteBufAds+4);
//					}	
//					if(( *(WriteBufAds+5) >= 1 )&&( *(WriteBufAds+4) <= 31 ))
//					{
//						Clk.DayH = *(WriteBufAds+5);
//					}
//					if(( *(WriteBufAds+6) >= 1 )&&( *(WriteBufAds+6) <= 7 ))
//					{
//						Clk.WeekH = *(WriteBufAds+6);
//					}
//					if( *(WriteBufAds+7) <= 23 )
//					{
//						Clk.HourH = *(WriteBufAds+7);
//					}
//					if( *(WriteBufAds+8) <= 59 )
//					{
//						Clk.MinH = *(WriteBufAds+8);
//					}
//					if( *(WriteBufAds+9) <= 59 )
//					{
//						Clk.SecH = *(WriteBufAds+9);
//					}
//					if( *(WriteBufAds+10) <= 99 )
//					{
//						Clk.SecPer = *(WriteBufAds+10);
//					}
//					Temp2 = *(WriteBufAds+11);
//					Temp2 *= 256;
//					Temp2 += *(WriteBufAds+12); 					
//					if(( Temp2 >= -720 )&&( Temp2 <= 720 ))
//					{
//						Clk.DeviatH = *(WriteBufAds+11);
//						Clk.DeviatL = *(WriteBufAds+12);
//						SwapData( WriteBufAds+11, 2 );
//						E2P_WData( Clock_time_zone, WriteBufAds+11, 2 );
//					}							
//					Clk.State = *(WriteBufAds+13);		
					//ʱ���޸�ǰ��¼..
					///SM.StandardEventCode = Clock_Adjust_old_time_date;
					///SM.StandardEventCode = Setting_RTC;///fire ֻ��ʱ���޸ĺ��¼
					///LoadRecord( Standard_Event_No );///2016-6-15 fire
					SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					//-------------
					E2P_WData( Date_of_last_clock_synchronisation, &Clk.YearH, 12 );
					
					//��¼ʱ������ƫ��ʱ��Clock_Time_Shift..
					{
						unsigned short temp_old,temp_new;
						if(Data_CompHL(WriteBufAds+2,&Clk.YearH, 3)==0)
						{
								if(Clk.DayH>*(WriteBufAds+5)) Temp = Clk.DayH - *(WriteBufAds+5);
								else Temp = *(WriteBufAds+5) -Clk.DayH;
								if(Temp>2) Temp_clock_time_shift =0xffff;			//ʱ�䳬��..
								else{	
											temp_old = (unsigned short)Clk.DayH*1440 +(unsigned short)Clk.HourH*60 +Clk.MinH;
											temp_new = (unsigned short)(*(WriteBufAds+5))*1440 +(unsigned short)(*(WriteBufAds+7))*60 +*(WriteBufAds+8);
											if(temp_new>temp_old) Temp = temp_new - temp_old;
											else Temp = temp_old - temp_new;
											if(Temp>1080) Temp_clock_time_shift =0xffff;
											else{
														if(Temp!=0){
															 	Temp = Temp*60;
															 	if(temp_old>temp_new) Temp_clock_time_shift = Temp + Clk.SecH - *(WriteBufAds+9);
															 	else  Temp_clock_time_shift = Temp + *(WriteBufAds+9) - Clk.SecH;	
														}else{
																		if(Clk.SecH>*(WriteBufAds+9)) Temp_clock_time_shift = Clk.SecH - *(WriteBufAds+9);
																		else 	Temp_clock_time_shift = *(WriteBufAds+9) - Clk.SecH;
																	} 
													}			
								}
						}else	 Temp_clock_time_shift = 0xffff;			//ʱ������޸�ʱ��..
							
						Temp = 0;
						E2P_RData( (unsigned char *)&Temp, Clock_Time_Shift_Limit, 1 );				//ʱ�ӵ���..	
						if(Temp_clock_time_shift>Temp)
							{
								SM.Profile1_status |= BIT1;				//20141230..
								SM.Profile2_status |= BIT1;
								//ʱ�䳬���޸�ʱ�䵥�����¼���¼..
//								if(SM.Profile1_status&BIT5)
//									{
										///SM.StandardEventCode = Time_Shift_Limit_No;
										LoadRecord( Standard_Event_No );//2016-6-15-fire
										SM.Event_all_flag |= Standard_Event_BIT;
										SM.Error_all_flag |= Standard_Event_BIT;
										//-------------
//									}
								Temp = 0xBBCC;
							}
//						else
//							{
//								SM.Profile1_status &= ~BIT2;
//								SM.Profile2_status &= ~BIT2;
//							}		
						E2P_WData( Clock_Time_Shift, (unsigned char*)&Temp_clock_time_shift, 2 );						
					}
					//---------------------------------------
					RAM_Write( &Clk.YearH, WriteBufAds+2, 12 );
					
					E2P_WData( Clock_time_zone, WriteBufAds+11, 2 );
					MoveCurrentTimeHex_BCD();					

					RAM_Write( Ptr, Clk.Sec, 3 );
					*(Ptr+3) = Clk.Week[0];
					RAM_Write( Ptr+4, Clk.Day, 3 );
			//		fnSysClock_Write(&Clk.Sec[0]);///fire-2016-7-4

///fire-8213û����ʱ��оƬ����ȡ�ڲ���RTCʱ������
#if( RTCCHIP == RX8025 )
					E2Pwrite( 0x00, Ptr, 7, RTCAds );		//дʱ��оƬ����
#elif( RTCCHIP == RX8025T )																	
					*(Ptr+3) = (unsigned char)ByteBit[*(Ptr+3)];					
					E2Pwrite( 0x00, Ptr, 7, RTCAds );		//дʱ��оƬ����	//8025T
#else
					if( *(Ptr+3) == 0 ) *(Ptr+3) = 0x07;
					E2Pwrite( 0x00, Ptr, 7, DS3231Ads );	//дʱ��оƬ����
#endif

					#if ( DAYLIGHTSAVE == YesCheck )		//����ʱ���⴦��,��¼��ǰ���õ�ʱ��..
							// Clk.State = 0;	//��д����ʱ�Ƿ��Ѿ��л�״̬�֣������ж����л�ʱ�̵�ʱ�����..
							// E2P_WData( Clock_daylight_savings_lasttime, &Clk.YearH, 11 );		//ʱ���л�������ʱ�����ж�..
						///	Clk.daylightsaving = judge_daylightsaving(0xa6);//2016-6-15-fire
							if(Clk.daylightsaving!=0xa6) E2P_WData( Clock_daylight_savings_lasttime, &Clk.YearH, 11 );		//ʱ���л�������ʱ�����ж�..				
					#endif	
					//ʱ���޸ĺ��¼..
					///SM.StandardEventCode = Clock_Adjust_new_time_date;
					SM.StandardEventCode = Setting_RTC;///fire
					LoadRecord( Standard_Event_No );//2016-6-15-fire
					SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					//-------------
	*/				
	//--------------------дHT6025ʱ�ӼĴ���-------------
				RAM_Write( &Clk.YearH, WriteBufAds+2, 12 );
					Clock_Year = Clk.YearH;
					Clock_Year *= 256;
					Clock_Year += Clk.YearL;
				
				*Ptr = Clk.WeekH;
				*(Ptr+1) = Clock_Year-2000;
				*(Ptr+2) = Clk.Month;
				*(Ptr+3) = Clk.DayH;
				*(Ptr+4) = Clk.HourH;
				*(Ptr+5) = Clk.MinH;
				*(Ptr+6) = Clk.SecH;
			//дRTCʱ��Ĵ�����������/��/��/��/ʱ/��/�빲7���Ĵ���
				HT_RTC_Write(Ptr);
					//��ʱ��������..
				///	if(Temp == 0xBBCC) SM.StandardEventCode = Time_Shift_Limit_No;
				}else ErrorByte = other_reason;
				break;	
				/*
			case 3:									//time_zone
//				SwapData( WriteBufAds+1, 2 );
				Clk.DeviatH = *(WriteBufAds+1);			//��GMTʱ��  ����
				Clk.DeviatL = *(WriteBufAds+2);	
				E2P_WData( Clock_time_zone, WriteBufAds+1, 2 );
				break;	
			case 5:									//daylight_savings_begin
			case 6:									//daylight_savings_end
				if( *(WriteBufAds+1) == 12 )
				{
					if( *Attribute == 5 ) Temp = Clock_daylight_savings_begin;
					else Temp = Clock_daylight_savings_end;
//					E2P_WData( Temp, WriteBufAds+1, 12 );							
					E2P_WData( Temp, WriteBufAds+2, 12 );
			}else ErrorByte = other_reason;
				break;	
			case 7:									//daylight_savings_deviation
				E2P_WData( Clock_daylight_savings_deviation, WriteBufAds+1, 1 );
				break;	
			case 8:									//daylight_savings_enabled
				E2P_WData( Clock_daylight_savings_enabled, WriteBufAds+1, 1 );
				break;	
				*/
			default:
				ErrorByte = type_unmatched;			
				break;			
		}
	}
	return ErrorByte;
}	

unsigned char SetIECLocalPortSetupClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	unsigned char AccessMode;
	unsigned char Len;
	unsigned char Temp_flag=0;	//���ֹ��ں�485��
		
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	AccessMode = IECLocalPortSetupClassAttribute_ParaTab[(*Attribute)-1].AccessMode;
	
	Buff[0] = 0x00;				//���� uart1
	Buff[1] = 0x00;
	Buff[2] = 0x14;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;

	Buff[6] = 0x00;				//485-1 �� uart0
	Buff[7] = 0x00;
	Buff[8] = 0x14;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) == 0 ) Temp_flag = 1;
	else if( Data_Comp( Ptr+6, Logical_Name, 6 ) == 0 ) Temp_flag = 2; 
	else Temp_flag = 0;
	if( Temp_flag == 0 )
	{
		ErrorByte = object_undefined; 									//�޶�Ӧ��OBIS��
	}	
	else if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//��д��Ȩ��
	}	 
	else if( *DataType != IECLocalPortSetupClassAttribute_ParaTab[(*Attribute)-1].DataType )
	{
		ErrorByte = type_unmatched; 									//��д��Ȩ��
	}	 
	else
	{
		switch( *Attribute )
		{
			case 2:
//				if( *(WriteBufAds+1) > 1 ) 						//version = 0				
				if( *(WriteBufAds+1) > 2 )						//version = 1			
				{
					ErrorByte = other_reason;
					break;
				}
				if( Temp_flag == 1 )
				{
			//		Para.RLocal_default_mode0 = *(WriteBufAds+1);
//					Para.RLocal_default_mode1 = *(WriteBufAds+1);			//12.11.29
	//				Para.RLocal_default_mode2 = *(WriteBufAds+1);			//12.11.29
					Para.RLocal_default_mode1= *(WriteBufAds+1);			//12.11.29
					E2P_WData( IEC_local_default_mode1, WriteBufAds+1, 1 );	
				}
				else if( Temp_flag == 2 )
					{
//					Para.RLocal_default_mode0 = *(WriteBufAds+1);
					Para.RLocal_default_mode0 = *(WriteBufAds+1);			//12.11.29
//					Para.RLocal_default_mode2 = *(WriteBufAds+1);			//12.11.29
//					Para.RLocal_default_mode3 = *(WriteBufAds+1);			//12.11.29
					E2P_WData( IEC_local_default_mode0, WriteBufAds+1, 1 );						
				}
				break;	
			case 3:
			case 4:
//				if( *(WriteBufAds+1) > 9 ) 
				if( *(WriteBufAds+1) > 6 ) 						//���֧��19200
				{
					ErrorByte = hardware_fault;
					break;
				}
				if( *Attribute == 3 ) 
				{
					if( Temp_flag == 1 )
					{
					//		Para.RLocal_default_baud0 = *(WriteBufAds+1);
							//Para.RLocal_default_baud1 = *(WriteBufAds+1);		//12.11.29
					//		Para.RLocal_default_baud2 = *(WriteBufAds+1);		//12.11.29
							Para.RLocal_default_baud1 = *(WriteBufAds+1);		//12.11.29
							Temp = IEC_local_default_baud1;
					}else if( Temp_flag == 2 )
					{
								Para.RLocal_default_baud0= *(WriteBufAds+1);		//12.11.29
								Temp = IEC_local_default_baud0;
					}						
				}	
				else{
							if( Temp_flag == 1 )
							{
							 	Temp = IEC_local_prop_baud1;
							}
							else if( Temp_flag == 2 )
							{
								Temp = IEC_local_prop_baud0;
							}		
						}
				E2P_WData( Temp, WriteBufAds+1, 1 );	
				break;	
			case 6:
			case 7:
			case 8:
			case 9:
				// ������ʱ��ͳһ����..
				Len = *(WriteBufAds+1);
				if( Len > 8 ) 
				{
					ErrorByte = other_reason;
					break;
				}
				else
				{
					RAM_Fill( WriteBufAds+2+Len, 8-Len );
					if( Temp_flag == 1 )
					{
							if( *Attribute == 0x07 ) Temp = IEC_local_pass_p1_1;
							else if( *Attribute == 0x08 ) Temp = IEC_local_pass_p2_1;
							else if( *Attribute == 0x09 ) Temp = IEC_local_pass_w5_1;
							else Temp = IEC_local_device_addr1;
					}
					else if( Temp_flag == 2 )
						{
							if( *Attribute == 0x07 ) Temp = IEC_local_pass_p1_0;
							else if( *Attribute == 0x08 ) Temp = IEC_local_pass_p2_0;
							else if( *Attribute == 0x09 ) Temp = IEC_local_pass_w5_0;
							else Temp = IEC_local_device_addr0;						
					}
					E2P_WData( Temp, WriteBufAds+2,  8 );
				}	
				break;	
			default: 
				ErrorByte = type_unmatched;			
				break;			
		}		
	}		
	return ErrorByte;
	
}		

unsigned char SetIECHDLCSetupClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	unsigned char AccessMode;
	unsigned char Temp_flag=0;	//���ֹ��ں�485��
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	AccessMode = IECHDLCSetupClassAttribute_ParaTab[(*Attribute)-1].AccessMode;
	
	Buff[0] = 0x00;				//����
	Buff[1] = 0x00;
	Buff[2] = 0x16;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Buff[6] = 0x00;				//485��
	Buff[7] = 0x01;
	Buff[8] = 0x16;
	Buff[9] = 0x00;
	Buff[10] = 0x00;
	Buff[11] = 0xff;	
		
	
	if( Data_Comp( Ptr, Logical_Name, 6 ) == 0 ) Temp_flag = 1;
	else if( Data_Comp( Ptr+6, Logical_Name, 6 ) == 0 ) Temp_flag = 2; 
	else Temp_flag = 0;
	if( Temp_flag == 0 )
	{		ErrorByte = object_undefined; 									//�޶�Ӧ��OBIS��
	}	
	else if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//��д��Ȩ��
	}	 
	else if( *DataType != IECHDLCSetupClassAttribute_ParaTab[(*Attribute)-1].DataType )
	{
		ErrorByte = type_unmatched; 									//��д��Ȩ��
	}	 
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//			case 3:														//HDLC���ʹ��ڳߴ�(2)		unsigned	1~7:1
//			case 4:														//HDLC���մ��ڳߴ�(2)		unsigned	1~7:1
//			case 5:														//HDLC��֡���ͳ���(3)		//unsigned16	32~2030:128(DLMS)	//unsigned8 32~128(IEC62056-62) 
//			case 6:														//HDLC��֡���ճ���(3)		//unsigned16	32~2030:128(DLMS)	//unsigned8 32~128(IEC62056-62) 
//				ErrorByte = read_write_denied; 							//��д��Ȩ��
//				break;	
			case 2:														//HDLC����(2)				enum		0~9:5(9600)		
				if( *(WriteBufAds+1) > 9 ) 
//				if( *(WriteBufAds+1) > 6 ) 								//���֧�ֵ�19200
				{
					ErrorByte = hardware_fault;
					break;
				}
				if( Temp_flag == 1 )
				{
						E2P_WData( HDLC_comm_speed, WriteBufAds+1, 1 );	
						Para.RHDLC_comm_speed = *(WriteBufAds+1);
				}else{
						E2P_WData( HDLC_comm_speed1, WriteBufAds+1, 1 );	
						Para.RHDLC_comm_speed1 = *(WriteBufAds+1);					
				}
				break;	
			case 7:														//HDLC�ֽڼ��ʱ��(3)		unsigned16	20~6000:1000
				Temp = GetBuffToInteger( WriteBufAds+1 );
				if(( Temp < 20 )||( Temp > 6000 ))						//ms	
				{
					ErrorByte = type_unmatched;
					break;
				}
				if( Temp_flag == 1 )
				{				
						Para.RHDLC_inter_octet_time_out = (Temp-1)*2;
						E2P_WData( HDLC_inter_octet_time_out, (unsigned char*)&Temp, 2 );
				}else{
						Para.RHDLC_inter_octet_time_out1 = (Temp-1)*2;
						E2P_WData( HDLC_inter_octet_time_out1, (unsigned char*)&Temp, 2 );					
				}						
				break;	
			case 8:														//HDLC��ֹ�˳�ʱ��(3)		unsigned16	0~120:120	
				Temp = GetBuffToInteger( WriteBufAds+1 );
				if( Temp > 300 )
				{
					ErrorByte = type_unmatched;
					break;
				}
				if( Temp_flag == 1 )
				{				
						Para.RHDLC_inactivity_time_out = Temp;
						E2P_WData( HDLC_inactivity_time_out, (unsigned char*)&Temp, 2 );
				}else{
						Para.RHDLC_inactivity_time_out1 = Temp;
						E2P_WData( HDLC_inactivity_time_out1, (unsigned char*)&Temp, 2 );
				}				
				break;	
			case 9:														//HDLC��ַ(3)				unsigned16	0x0010~0x3ffd:���ĩ4λ��1000������1234+1000=2234=0x08BA	
				Temp = GetBuffToInteger( WriteBufAds+1 );
				if(( Temp < 0x10 )||( Temp > 0x3ffd ))
				{
					ErrorByte = type_unmatched;
					break;
				}
//				if( Temp_flag == 1 )
//				{								
					E2P_WData( HDLC_device_address, (unsigned char*)&Temp, 2 );
//				}else{
//					E2P_WData( HDLC_device_address1, (unsigned char*)&Temp, 2 );
//				}					
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}	


short SETProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	short Len=4;
	
	switch( *(RecBuff+1) )
	{
		case set_request_normal:   							//1		// IMPLICIT Set-Request-Normal,			
			if( JudgeSetClassPara( COM_No, SendBuff, RecBuff ) == 0 )
			{
				Len = SetRequestNormalProcess( COM_No, RecBuff, SendBuff );
			}			
			break;
		case set_request_with_first_datablock:        		//2 		//IMPLICIT Set-Request-With-First-Datablock,
			if( JudgeSetClassPara( COM_No, SendBuff, RecBuff ) == 0 )
			{
				Len = SetRequestWithDataBlockProcess( COM_No, RecBuff, SendBuff, 0 );
			}			
			break;			
		case set_request_with_datablock:            		//3 		//IMPLICIT Set-Request-With-Datablock,
			Len = SetRequestWithDataBlockProcess( COM_No, RecBuff, SendBuff, 1 );
			break;
		case set_request_with_list:                	 		//4 		//IMPLICIT Set-Request-With-List,
		case set_request_with_list_and_first_datablock:  	//5 		//IMPLICIT Set-Request-With-List-And-First-Datablock
		default:
			Len = CreatSetRequestNormalError( SendBuff, *(RecBuff+2), type_unmatched );					//���ݲ������ȴ�
			break;					
	}	
	return Len;
}  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EVENT_NOTIFICATIONProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned char Logical_Name[6];
	unsigned char Temp;
	unsigned long Value;
	
	Logical_Name[0] = 0x00;				//�����źų�ǿ	
	Logical_Name[1] = 0x00;
	Logical_Name[2] = 0x60;
	Logical_Name[3] = 0x0C;
	Logical_Name[4] = 0x05;
	Logical_Name[5] = 0xFF;
#if 0
	if(( GetBuffToInteger( RecBuff+13 ) == 0x0003 )&&( *(RecBuff+21) == 0x02 )&&( *(RecBuff+22) == 0x11 ))
	{
		if( Data_Comp( RecBuff+15, Logical_Name, 6 ) == 0 )			//�����źų�ǿ
		{
			Para.NetIntensity = *(RecBuff+23);	
			Para.NetIntensity |= 0x80;								//�յ��������ʾ����
			SM.NetIntensityMinDly = 10;								//10������û�յ������źţ�����ʾ���߷���
			//add �����Ƿ�ɹ��ж�..
			crypt.GPRS_STAND = 0x0B;    //GPRSģ����ڱ�־..
			if( *(RecBuff+24) == 0x16 ) //skyblue20140925	
				SM.CommCheckFlag = *(RecBuff+25);				
				//01--�������ͳɹ���02---������; 00---��ʾδ���յ�������������.. 03 --������վ��04--δ������վ
				if(SM.CommCheckFlag==0x03) crypt.comm_online = 1;
				else if((SM.CommCheckFlag==0x02)&&(SM.CommCheckFlag==0x04)) crypt.comm_online = 0;
				
			return;
		}	
	}	
/*
	Logical_Name[0] = 0x07;
	Logical_Name[1] = 0x00;
	Logical_Name[2] = 0x00;
	Logical_Name[3] = 0x00;
	Logical_Name[4] = 0x00;
	Logical_Name[5] = 0xFF;

	if(( GetBuffToInteger( RecBuff+13 ) == 0x0003 )&&( IsAllData( RecBuff+21, 3, 0x02 ) == 0 )&&( *(RecBuff+24) == 0x09 )&&( *(RecBuff+25) == 0x04 )&&( *(RecBuff+30) == 0x06 ))
	{
		if( Data_Comp( RecBuff+15, Logical_Name, 6 ) == 0 )			//����
		{
			Temp = GetBuffToInteger( RecBuff+26 );
			E2P_WData( GasMeter_SAP, (unsigned char*)&Temp, 2 );
			Temp = GetBuffToInteger( RecBuff+28 );
			E2P_WData( GasMeter_HDLC_device_address, (unsigned char*)&Temp, 2 );
			Value = GetBuffToLong( RecBuff+31 );
			E2P_WData( GasMeter_ReadValue, (unsigned char*)&Value, 4 );						
			//Һ������ʾG
			outmeter.Gasmeter_time = 6;
		  // outmeter.Gas_meter = 0x7;
		  return;					
		}		
		else
		{
			Logical_Name[0] = 0x08;
			if( Data_Comp( RecBuff+15, Logical_Name, 6 ) == 0 )		//ˮ��
			{
				Temp = GetBuffToInteger( RecBuff+26 );
				E2P_WData( WaterMeter_SAP, (unsigned char*)&Temp, 2 );
				Temp = GetBuffToInteger( RecBuff+28 );
				E2P_WData( WaterMeter_HDLC_device_address, (unsigned char*)&Temp, 2 );
				Value = GetBuffToLong( RecBuff+31 );
				E2P_WData( WaterMeter_ReadValue, (unsigned char*)&Value, 4 );												
				//Һ������ʾW
				outmeter.Watermeter_time = 6;
			  // outmeter.Watermeter = 0x8;
			  return;
			}							
		}	
	}
	*/
	
	Logical_Name[0] = 0x00;				//GPRSģ����Ϣ	
	Logical_Name[1] = 0x00;
	Logical_Name[2] = 0x80;
	Logical_Name[3] = 0x80;
	Logical_Name[4] = 0x32;
	Logical_Name[5] = 0xFF;

//0x00,0x2B,  --����43����ҷ�����..
//0x00,0X00,0x80,0x80,0x32,0xFF,
//0x02,0x03,
//0x06,0x??,0x??,0x??, ,0x??,
//09 0f XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX (ģ�鴮��)
//09 0f XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX (SIM������)
	if(( GetBuffToInteger( RecBuff+13 ) == 0x002B )&&( *(RecBuff+21) == 0x02 )&&( *(RecBuff+22) == 0x03 )&&( *(RecBuff+23) == 0x06 ))
	{
		if( Data_Comp( RecBuff+15, Logical_Name, 6 ) == 0 )			//�����źų�ǿ
		{
			crypt.GPRS_STAND = 0x0B;    //GPRSģ����ڱ�־..
			//��̬IP��ַ..
			E2P_WData( IPv4_IP_address, RecBuff+24, 4 );
			if(( *(RecBuff+28) == 0x09 )&&( *(RecBuff+29) == 0x0f )) E2P_WData( MACad_GPRS, RecBuff+30, 15 );
			if(( *(RecBuff+45) == 0x09 )&&( *(RecBuff+46) == 0x0f )) E2P_WData( IMSI_simcard, RecBuff+47, 15 );
			return;
		}	
	}		

// д����汾��-PRIME\MBUS\gprs..
//GPRS�����ʽ�磺
//7E A0 28 FE FE FE FF 21 03 FB 53 E6 E6 00 C2 01 81 00 01 01 04 00 02 00 FF 02 00 01 01 02 01 09 06 01 02 03 04 05 06 EA BA 7E

	if(( GetBuffToInteger( RecBuff+3 ) == 0x0001 )&&( *(RecBuff+11) == 0x02 )&&( *(RecBuff+12) == 0x00 )&&( *(RecBuff+13) == 0x01 )&&( *(RecBuff+14) == 0x01 ))
	{
		Logical_Name[0] = 0x01;				//GPRS..	
		Logical_Name[1] = 0x02;
		Logical_Name[2] = 0x00;
		Logical_Name[3] = 0x02;
		Logical_Name[4] = 0x00;
		Logical_Name[5] = 0xFF;

		if( Data_Comp( RecBuff+5, Logical_Name, 6 ) == 0 )			////GPRSģ������汾��..
		{
			if(( *(RecBuff+15) == 0x02 )&&( *(RecBuff+16) == 0x01 )&&( *(RecBuff+17) == 0x09 )&&( *(RecBuff+18) == 0x06 ))
	    {
	    	//д����汾��..
	    	E2P_WData( Active_firmware_identification_com_non_PRIME, RecBuff+19, 6 );
			}
			return;		  
		}
		
	 // MBUS
		Logical_Name[0] = 0x01;				//MBUS..	
		Logical_Name[1] = 0x04;
		Logical_Name[2] = 0x00;
		Logical_Name[3] = 0x02;
		Logical_Name[4] = 0x00;
		Logical_Name[5] = 0xFF;

		if( Data_Comp( RecBuff+5, Logical_Name, 6 ) == 0 )			////MBUSģ������汾��..
		{
			if(( *(RecBuff+15) == 0x02 )&&( *(RecBuff+16) == 0x01 )&&( *(RecBuff+17) == 0x09 )&&( *(RecBuff+18) == 0x06 ))
	    {
	    	//д����汾��..
	    	E2P_WData( Active_firmware_identification_Mbus, RecBuff+19, 6 );
			}
			return;		  
		}
	
	 // PRIME
		Logical_Name[0] = 0x01;				//PRIME..	
		Logical_Name[1] = 0x01;
		Logical_Name[2] = 0x00;
		Logical_Name[3] = 0x02;
		Logical_Name[4] = 0x00;
		Logical_Name[5] = 0xFF;

		if( Data_Comp( RecBuff+5, Logical_Name, 6 ) == 0 )			//PRIMEģ������汾��..
		{
			if(( *(RecBuff+15) == 0x02 )&&( *(RecBuff+16) == 0x01 )&&( *(RecBuff+17) == 0x09 )&&( *(RecBuff+18) == 0x06 ))
	    {
	    	//д����汾��..
	    	E2P_WData( Active_firmware_identification_PRIME, RecBuff+19, 6 );
			}
			return;		  
		}				
	}	
	#endif	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
short CreatActionRequestNormalError( unsigned char* Dest, unsigned char InvokeId, unsigned char ErrorByte )
{
	*Dest = APDU_ACTION_Response;	
	*(Dest+1) = action_response_normal;
	*(Dest+2) = InvokeId;
	*(Dest+3) = ErrorByte;
	*(Dest+4) = 0x00;										//OPTIONAL 
    
    return 5;
}	

short JudgeActionClassPara( short COM_No, unsigned char* Dest, unsigned char* Source )
{
	short i;
	unsigned short ClassId;
	unsigned char InvokeId;
	unsigned char Method;
//	unsigned char OptionFlag;
	unsigned char Len;
	unsigned short* RecDataPtr;
    
	InvokeId = *(Source+2);
	  HComm.Send_state_InvokeId = InvokeId;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	if( *RecDataPtr < (3+13))					//E6+E6+00+C3+01+81+Classid(2)+Logical_name(6)+Method(1)+OPTION(1) = 16
	{
		CreatActionRequestNormalError( Dest, InvokeId, type_unmatched );				//���ݲ������ȴ�
		return -1;
	}	

	Len = *RecDataPtr;
	Len -= 3;
	
	ClassId = GetBuffToInteger( Source+3 );
	
	for( i=0;i<CosemParaSum;i++ )
	{
		if( ClassId == Cosem_ParaTab[i].ClassId ) break;								
	}	
	if( i == CosemParaSum ) 
	{
		CreatActionRequestNormalError( Dest, InvokeId, object_class_inconsistent );		//�������ݲ�֧��
		return -1;		
	}		
	Method = *(Source+2+9);
	if(( Method > Cosem_ParaTab[i].MethodNum )||( Method == 0 ))			//����Ϊ0��֧��
	{
		CreatActionRequestNormalError( Dest, InvokeId, type_unmatched );
		return -1;		
	}		
//	OptionFlag = *(Source+2+10);
//	if( OptionFlag != 0 )											//��֧��ѡ���Է���д���ݡ�
//	{
//		CreatActionRequestNormalError( Dest, InvokeId, type_unmatched );
//		return -1;		
//	}	
	RAM_Fill( CommPara[COM_No].SubsBlockPara, SUBS_PARA_LEN );
//	RAM_Write( CommPara[COM_No].SubsBlockPara, Source, MAC_Set_First_Block_Data );
    return 0;
}	

unsigned char ActionClassData( short COM_No, unsigned short ClassId, unsigned char* Data, unsigned short DataLen )
{
	unsigned char ErrorByte=object_class_inconsistent;

	switch( ClassId )
	{
		case Class_id_Register:	
			ActionDemand_registerClassData( Data, DataLen );
			break;
		case Class_id_Extended_register:				//4
			break;
		case Class_id_Demand_register:					//5
			ErrorByte = ActionDemand_registerClassData( Data, DataLen ); 
			break;
		case Class_id_Register_activation:				//6
			ErrorByte = ActionRegister_ActivationClassData( Data, DataLen ); 
			break;
		case Class_id_Profile_generic:					//7
			ErrorByte = ActionProfile_genericClassData( Data, DataLen ); 
			break;
//		case Class_id_Utility_tables:					//26
//			break;
//		case Class_id_Register_table:					//61
//			break;
//		case Class_id_Status_mapping:					//63
//			break;
//		case Class_id_Association_SN:					//12
//			break;

		case Class_id_Clock:							//8
			break;
		case Class_id_Script_table:						//9
			ErrorByte = ActionScript_tableClassData( Data, DataLen ); 
			break;
//		case Class_id_Schedule:							//10
//			break;
		case Class_id_Special_days_table:				//11
			ErrorByte = ActionSpecial_DaysClassData( Data, DataLen ); 
			break;
		case Class_id_Association_LN:					//15
			ErrorByte = ActionAssociation_LNClassData( COM_No,Data, DataLen ); 
			break;
		case Class_id_SAP_assignment:					//17
			break;
		case Class_id_Image_transfer:					//18		
			ErrorByte = ActionImage_transferClassData( Data, DataLen ); 
			break;						
		case Class_id_Activity_calendar:				//20
			ErrorByte = ActionActivity_calendarClassData( Data, DataLen ); 
			break;
		case Class_id_Disconnect_control:				//70
			ErrorByte = ActionDisconnect_controlClassData( Data, DataLen ); 
			break;
		case Class_id_Security_setup:					//64
			ErrorByte = ActionSecurity_setupClassData( COM_No, Data, DataLen ); 
			break;
//		case Class_id_Limiter:							//71
//			break;
		default: 
			break;			
	}	
	return ErrorByte;
}

//����ACTION�Ŀ鴫������..(��ʱ����SET���ݿ��ָ��--ͬʱֻ�ܲ���SET DATABLOCK ��ACTION DATABLOCK)	
//Type=0: �յ���һ��
//Type!=0: ������
short ActionRequestWithDataBlockProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff, short Type )
{
	unsigned char* Ptr;
	unsigned short DataLen=0;
	unsigned char ErrorByte=success;
	unsigned long* BlockNumber;
	unsigned short Len;
	unsigned char* ActionBlockBuff;
	unsigned short* ActionBlockPtr;
	unsigned char* Data_Last_Block;										//�������������־
	unsigned char* Data_Block_Number;									//�������ݿ����
	unsigned char* Data_Block_Data;										//����������ʼ��ַ
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	ActionBlockBuff = CommPara[COM_No].SetBlockBuff;
	ActionBlockPtr = CommPara[COM_No].SetBlockPtr;
	
	*SendBuff = APDU_ACTION_Response;
	if(*(RecBuff+3)==0) *(SendBuff+1) = action_response_next_pblock;
	else *(SendBuff+1) = action_response_normal;
	*(SendBuff+2) = *(RecBuff+2);										//InvokeId
                                                    					
	if( Type == 0 )														//��һ��
	{
		*BlockNumber = 1;
		RAM_Write( ActionBlockBuff, RecBuff + MAC_Get_Set_ClassId, 9 );     //�������ö����Է��뻺��            			
		*ActionBlockPtr = 9;
		Data_Last_Block = RecBuff+MAC_Action_First_Last_Block;				//ACTION���������־
		Data_Block_Number = RecBuff+MAC_Action_First_Block_Number;       //ACTION���ݿ����    
		Data_Block_Data = RecBuff+MAC_Action_First_Block_Data;		        //ACTION������ʼ��ַ  
		Len = MAC_Action_First_Block_Data + 3;								//ACIION�����ֽ�ǰ��20���ֽ�,SET��21���ֽ�..
	}
	else 																//������
	{
		*BlockNumber += 1;
		Data_Last_Block = RecBuff+MAC_Action_Last_Block;
		Data_Block_Number = RecBuff+MAC_Action_Block_Number;
		Data_Block_Data = RecBuff+MAC_Set_Block_Data;
		Len = MAC_Action_Block_Data + 3;									//�����ֽ�ǰ��11���ֽ�
	}	

	if( GetBuffToLong( Data_Block_Number ) != *BlockNumber ) ErrorByte = other_reason;	//���õĿ�Ų���		
	
	if( ErrorByte == success )
	{
		Ptr = JudgeSetBlockData( Data_Block_Data, (*CommPara[COM_No].RecDataPtr)-Len, &DataLen, (unsigned char*)&ErrorByte );
		if(( DataLen + (*ActionBlockPtr) ) > MAX_BLOCK_SIZE ) ErrorByte = other_reason;	//�����ݳ��Ȳ��ܳ���
  }
	
	if( ErrorByte == success )
	{
		RAM_Write( ActionBlockBuff+(*ActionBlockPtr), Ptr, DataLen );			//��action������������û�����
		*ActionBlockPtr += DataLen;		                                            			
		if( *Data_Last_Block != 0 )										//�������һ�飬���Խ������ݴ���		
		{
			ErrorByte = ActionClassData( COM_No, GetBuffToInteger( ActionBlockBuff ), ActionBlockBuff, *ActionBlockPtr-9 );		//ACTION����
		}	
	}	
	if(( ErrorByte != success )||( *Data_Last_Block != 0 ))
	{
		*(SendBuff+1) = action_request_normal;									//���������̽���
	}else{
				*(SendBuff+1) = action_request_with_first_pblock;			//���������̹�����..
				}
		*(SendBuff+3) = ErrorByte;                                    //���ش����־
		RAM_Write( SendBuff+4, Data_Block_Number, 4 );             		//�������յ��Ŀ��	
		Len = 8;                                                                    			
                                                           			
	if(( ErrorByte != success )||( *Data_Last_Block != 0 ))				//�������һ����д��������		
	{
		*BlockNumber = 0;
		RAM_Fill( CommPara[COM_No].SubsBlockPara, SUBS_PARA_LEN );
		*ActionBlockPtr = 0;
		RAM_Fill( ActionBlockBuff, 9 );
	}	
    return Len;	
}	

short ActionRequestNormalProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short ClassId;
	unsigned char InvokeId;
	unsigned short DataLen;
	unsigned char ErrorByte=object_class_inconsistent;
	
	InvokeId = *(RecBuff+2);
  
	ClassId = GetBuffToInteger( RecBuff+3 );

	*SendBuff = APDU_ACTION_Response;
	*(SendBuff+1) = action_response_normal;
	*(SendBuff+2) = InvokeId;

        HComm.Send_state_InvokeId = InvokeId;
  
	DataLen = *CommPara[COM_No].RecDataPtr - 15;

	ErrorByte = ActionClassData( COM_No, ClassId, RecBuff+3, DataLen );
  /*
  if(( ErrorByte==0x23	)||(ErrorByte==0x24))   //ESAM��Կ�������⴦��..
  {
  		//����ESAMоƬ..
			if( Init_For_Process( ESAM ) != 0 ) 
				{ 
						ErrorByte = 0x02;		//оƬ��������..	
						*(SendBuff+3) = ErrorByte;  //������..
						return 4;
				}
				*(SendBuff+3) = 0;  //��ȷ����..					
  	if(ErrorByte==0x23)   //���¼������еķǶԳ���Կ��..
  		{
  					*(SendBuff+4) = 0x09;   
		   	    *(SendBuff+5) = 0x81;   //��ʾ����1���ֽڱ�ʾ����..
		   	    *(SendBuff+6) = 0x80;    
 						ErrorByte = UpdateAsymKey( SendBuff+7,RecBuff+15, 0 );
//		   	    *(SendBuff+5) = 0x80;   //�����Ϲ���..
//		   	   	ErrorByte = UpdateAsymKey( SendBuff+6,RecBuff+15, 0 );
//						return 0x86;
						
					 	if(ErrorByte!=0)
						 {
						 	ErrorByte = 0x69;		//оƬ��������..	
						  *(SendBuff+3) = ErrorByte;  //������..
						  return 4;  		
					   }else return 0x87;
      }  		
      else
  		{     //ʹ��ECIES���¼������е�ȫ����Կ..  					
  			    *(SendBuff+4) = 0x09;   
		   	    *(SendBuff+5) = 0x10;  
 						ErrorByte = UpdateEciesKey( SendBuff+6,RecBuff+15, 0 );
					 	if(ErrorByte!=0)
						 {
						 	ErrorByte = 0x69;		//оƬ��������..	
						  *(SendBuff+3) = ErrorByte;  //������..
						  return 4;  		
					   }else return 0x16; 			
			}
  }
  
  //Action�������⴦����Ŀ:��֤���������⴦��15�෽��1..
  if(ErrorByte==0x22)  ////��֤ͨ����,��ȷ����..
  {  
  	if(ICEC.use_SHA256 != 0x01)  //����SHA-256�㷨..
  		{
		  	//00 09 11 10 01 23 45 67 fe 14 66 af b3 db cd 4f 93 89 e2 b7
		  	*(SendBuff+3) = 0;  //��ȷ����..
		  	*(SendBuff+4) = 0x09; 
		   	*(SendBuff+5) = 0x11; 
		   	*(SendBuff+6) = 0x10;
		   	RAM_Write( SendBuff+7,&crypt.glo_FC[1], 4 ); 
		   	//����TAG..
					if( Init_For_Process( ESAM ) != 0 ) 
					{ 
						ICEC.EsamOK = 0;
						*(SendBuff+3) = 0x01;  //������..
						//return 1;
						return 4;
					}
					else 
					{
							ICEC.EsamOK = 5;					
							Gcm128EnDecrypt( ESAM, SendBuff+11,crypt.C_Challenge, 0 ,0x10,0x08, 8 );   //0x08---�������������TAG;
					}			  
		     return 23;	 
    	}
    	else
    	{		//ICEC.use_SHA256 == 0x01��SHA-256�㷨..
    		//00 09 25 10 01 23 45 67 fe 14 66 af b3 db cd 4f 93 89 e2 b7
		  	*(SendBuff+3) = 0;  //��ȷ����..
		  	*(SendBuff+4) = 0x09; 
		   	*(SendBuff+5) = 0x24;
		   	*(SendBuff+6) = 0x10;
		   	RAM_Write( SendBuff+7,&crypt.glo_FC[1], 4 );
		   	
		   	RAM_Write( SendBuff+11,RecBuff+83, 32 ); // ������ʱ����..
		   	
		   	return 43;	 
    	}   	 
  }
//  else
//  {
//		*(SendBuff+3) = ErrorByte;
//		*(SendBuff+4) = 0x00;									//OPTIONAL
//		
//    return 5;	
//	}
*/
	  if(ErrorByte==0x22)  ////��֤ͨ����,��ȷ����..
	  {   //��ʱ����..
		//00 09 11 10 01 23 45 67 fe 14 66 af b3 db cd 4f 93 89 e2 b7
		*(SendBuff+3) = 0;	//��ȷ����..
		*(SendBuff+4) = 0x09; 
		*(SendBuff+5) = 0x11; 
		*(SendBuff+6) = 0x10; 
		*(SendBuff+7) = 0x01; 
		*(SendBuff+8) = 0x23; 
		*(SendBuff+9) = 0x45; 
		*(SendBuff+10 ) = 0x67;
		*(SendBuff+11 ) = 0xfe; 
		*(SendBuff+12 ) = 0x14; 
		*(SendBuff+13 ) = 0x66; 
		*(SendBuff+14 ) = 0xaf; 
		*(SendBuff+15 ) = 0xb3; 
		*(SendBuff+16 ) = 0xdb; 
		*(SendBuff+17 ) = 0xcd; 
		*(SendBuff+18 ) = 0x4f; 	
		*(SendBuff+19 ) = 0x93; 
		*(SendBuff+20 ) = 0x89; 	
		*(SendBuff+21 ) = 0xe2; 
		*(SendBuff+22 ) = 0xb7;
		 return 23; 		 
	  }
else
{
	*(SendBuff+3) = ErrorByte;
	*(SendBuff+4) = 0x00;									//OPTIONAL	
    return 5;	
	}
}	

short ACTIONProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	short Len=4;
	
	switch( *(RecBuff+1) )
	{
		case action_request_normal:							//1 		//IMPLICIT Action-Request-Normal,                                                    
			if( JudgeActionClassPara( COM_No, SendBuff, RecBuff ) == 0 )
			{
				Len = ActionRequestNormalProcess( COM_No, RecBuff, SendBuff );
			}			
			break;
		//ACITON�еĿ���ܴ���
		case action_request_next_pblock: 					//2 		//IMPLICIT Action-Request-Next-Pblock,                                          
		case action_request_with_list: 						//3 		//IMPLICIT Action-Request-With-List,                                              
       break;
		case action_request_with_first_pblock:        		//4 		//IMPLICIT Action-Request-With-First-Pblock,    
			if( JudgeActionClassPara( COM_No, SendBuff, RecBuff ) == 0 )
			{
				Len = ActionRequestWithDataBlockProcess( COM_No, RecBuff, SendBuff, 0 );
			}			
			break;
		case action_request_with_list_and_first_pblock:		//5 		//IMPLICIT Action-Request-With-List-And-First-Pblock,            
			break;			
		case action_request_with_pblock:            		//6 		//IMPLICIT Action-Request-With-Pblock
			Len = ActionRequestWithDataBlockProcess( COM_No, RecBuff, SendBuff, 1 );
			break;			
//		case action_request_next_pblock: 					//2 		//IMPLICIT Action-Request-Next-Pblock,                                          
//		case action_request_with_list: 						//3 		//IMPLICIT Action-Request-With-List,                                              
//		case action_request_with_first_pblock: 				//4 		//IMPLICIT Action-Request-With-First-Pblock,                              
//		case action_request_with_list_and_first_pblock:		//5 		//IMPLICIT Action-Request-With-List-And-First-Pblock,            
//		case action_request_with_pblock: 					//6 		//IMPLICIT Action-Request-With-Pblock
			  //sky2014..
		default:
			Len = CreatActionRequestNormalError( SendBuff, *(RecBuff+2), type_unmatched );					//���ݲ������ȴ�
			break;					
	}	
	return Len;
}  

//===============================================================================================================================================        

short GetRequestWithListProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	return 0;	
}	

short GETProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	short Len=5;
	unsigned char buff[5];
	RAM_Write(&buff[0],RecBuff,5);
	switch( *(RecBuff+1) )
	{
		case get_request_normal:   					//1		// IMPLICIT Get-Request-Normal,			
			if( JudgeClassPara( COM_No, SendBuff, RecBuff ) == 0 )
			{
				Len = GetRequestNormalProcess( COM_No, RecBuff, SendBuff );
			}			
			break;
		case get_request_next:      				//2 	//IMPLICIT Get-Request-Next,
			Len = GetRequestNextProcess( COM_No, RecBuff, SendBuff );
			break;
		case get_request_with_list:   				//3 	//IMPLICIT Get-Request-With-List
//			Len = GetRequestWithListProcess( COM_No, RecBuff, SendBuff );
//			break;
		default:
			Len = CreatGetRequestNormalError( SendBuff, *(RecBuff+2), type_unmatched );					//���ݲ������ȴ�
			break;					
	}	
	return Len;
}		

unsigned short GetRAMDataClassTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;
		
	for( i=0;i<DLMS_RAM_DataSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_RAM_DATATab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;	
}

unsigned short GetDataClassTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;
		
	for( i=0;i<DLMSDataSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_DATATab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;	
}	

//ȡData��RAMTab����
short GetRAMDataTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute ) 
{
	unsigned short i;
	unsigned char* Addr;
	short Len;
	
	Len = -1;
	
	i = GetRAMDataClassTabItemNo( Logical_Name );						//���Ҷ�Ӧ��OBIS��
	
		
	if( i != DLMS_RAM_DataSum )
	{
		*Dest = Get_data;								    //�������ݷ���	
		if( Attribute == 0x01 )							//����1��logical_name
		{
			*(Dest+1) = Data_octet_string;
			*(Dest+2) = 0x06;
			RAM_Write( Dest+3, Logical_Name, 6 );
			Len = 9;
		}
		else											//����2��value
		{	
			Addr = DLMS_RAM_DATATab[i].Addr;
			Len = DLMS_RAM_DATATab[i].ComInfo & RS_LEN;
	
			*(Dest+1) = DLMS_RAM_DATATab[i].DataType;
			if(( *(Dest+1) == D_OctetStr )||( *(Dest+1) == D_VisibleStr ))
			{
				*(Dest+2) = Len;
				RAM_Write( Dest+3, Addr, Len );	
				/*				
				//��ǰ���ڶ��������⴦����������..
				#if( IranRequire == YesCheck )
				if((*Logical_Name ==0x01)&&	(*(Logical_Name+1) ==0x0)&&	(*(Logical_Name+2) ==0x0)&&	(*(Logical_Name+3) ==0x09)&&	(*(Logical_Name+4) ==0x02)&&	(*(Logical_Name+5) ==0xff))			
				{
					GregorianHexToPersianHex( Dest+3, (unsigned char*)&Clk.YearH );
				}								
				#endif	
				*/
				// 0.0.96.14.0.255 ===��ǰ���������ʱ���� ,1.1.94.98.1.255--ȱʡ����..
//				if(((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==14)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))
//					||(((*Logical_Name ==1)&&	(*(Logical_Name+1) ==1)&&	(*(Logical_Name+2) ==94)&&	(*(Logical_Name+3) ==98)&&	(*(Logical_Name+4) ==1)&&	(*(Logical_Name+5) ==0xff))))			
				if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==14)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))
				{
					//if(*Addr == 0)
				//	RAM_Write( Dest+3, (unsigned char *)DLMS_ACTIVATION_FEE[(*Addr)-1].FEE_OCTET_STRING, Len );
	//				if(*Logical_Name ==1) *(Dest+7) = '4';
						
				}								
				//�¼�״̬�����⴦��,����������¼�״̬��..
				if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==97)&&	(*(Logical_Name+3) ==97)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))			
				{
					SM.Event_all_flag = 0;
						
					// Flag.New_check_ok = 0x58; //��ʱ���Դ��20140907..
				}
				else
				{
				/*
						//������:��ȡGPRS�豸ESAM��Կ�Ͷ�ȡESAM���кţ���Ϊ�ϵ�ȡ��20150906..
					//	if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==1)&&	((*(Logical_Name+4) ==144)||(*(Logical_Name+4) ==143))&&	(*(Logical_Name+5) ==0xff))			
						if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==1)&&	((*(Logical_Name+4) ==144)||(*(Logical_Name+4) ==143))&&	(*(Logical_Name+5) ==0xff))			
						{
							//0xff
							*(Dest+3) = 0;
							if( Init_For_Process( ESAM ) != 0 ) 
								{ 
									// *(Dest+3) = 250;
									return 4;
									// Flag.MeterState1 |= F_ESAMErr;				//�������״̬1
								}
//							if(*(Logical_Name+4) ==144)   // ��ȡGPRS�豸ESAM��Կ..
//								{
								if(GetESAMPubKey( Dest+3, 0 )!=0)   //û����..
									{
										 Len = 0;
										 *(Dest+2) = Len;
										 *(Dest+3) = 0;
									}	
//								}
						}	
						*/
				}
				Len += 3;
			}
			else
			{
				RAM_Write( Dest+2, Addr, Len );	
								
//				//������:��ȡESAM�汾��..
//				if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==1)&&	(*(Logical_Name+4) ==142)&&	(*(Logical_Name+5) ==0xff))			
//				{
//					//0xff
//					*(Dest+2) = 0xff;       //û����
//					//������ȡESAM�汾��..
//					if( Init_For_Process( ESAM ) != 0 ) 
//						{ 
//							ICEC.EsamOK = 0;
//							//Flag.MeterState1 |= F_ESAMErr;				//�������״̬1
//						}
//					if(GetESAMKEY( ESAM, &ICEC.EsamKEY, 1 )==0) *(Dest+2) = ICEC.EsamKEY; 
//					//===================					
//				}	
			/*
//				//��ȡESAM�汾��20160310..
				if((*Logical_Name ==0)&&(*(Logical_Name+1) ==64)&&(*(Logical_Name+2) ==96)&&(*(Logical_Name+3) ==98)&&(*(Logical_Name+5) ==0xff))			
				{
					*(Dest+2) = 0xff;       //û����
					*(Dest+3) = 0xff;       //û����
					if((*(Logical_Name+4) ==11)||(*(Logical_Name+4) ==12)||(*(Logical_Name+4) ==13)||(*(Logical_Name+4) ==14))
					{
						//������ȡESAM�汾��..
						if( Init_For_Process( ESAM ) != 0 ) 
							{ 
								ICEC.EsamOK = 0;
								//Flag.MeterState1 |= F_ESAMErr;				//�������״̬1
							}
						if(GetESAMKEY( ESAM, &ICEC.EsamKEY, *(Logical_Name+4) )==0)
							{
								 *(Dest+2) = ICEC.EsamKEY;
								 *(Dest+3) = 0;
							} 
						//===================	
																
					}
				}	
				*/
//				//�������¼�״̬�����⴦��,����������¼�״̬��..
//				if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==128)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))			
//				{
//					SM.Event_all_flag = 0;	
//					// Flag.New_check_ok = 0x58; //��ʱ���Դ��20140907..
//				}
		//	if((*(Dest+1)==D_Unsigned32)||(*(Dest+1)==D_Unsigned16))
		if(*(Dest+1)==D_Unsigned16)
			{
				SwapData( Dest+2, Len );
			}									
				Len += 2;
			}	
		}	
	}
	else
	//��ʱ���ӹ������״̬ʹ��..
	// if(i == DLMS_RAM_DataSum)
		{
		/*
				//�����ж�״̬�����⴦��..
				if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==9)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==99)&&	(*(Logical_Name+4) ==66)&&	(*(Logical_Name+5) ==0xff)&&( Attribute == 0x02 )	)			
				{
						*Dest = Get_data;								//�������ݷ���
						*(Dest+1) = Data_octet_string;
						*(Dest+2) = 0x04;
												
						*(Dest+3) = (unsigned char)(SM.ComModuleCnt&0xff);		//ͨѶģ����ֵ..
						*(Dest+4) = (unsigned char)(SM.MagneticCnt&0xff);			//�ų����ֵ..
						*(Dest+5) = (unsigned char)(SM.CoverCnt&0xff);				//���Ǽ��ֵ..
						*(Dest+6) = (unsigned char)(SM.TerminalCnt&0xff);			//���˼��ֵ..															
						Len = 7;	
				}			
				*/
		} //����״̬���ʹ�ð���end..		
	return Len;
}		

//ȡData��Tab����
short GetDataTab(  short COM_No, unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute ) 
{
	unsigned short i,curr;
	unsigned short Addr;
	short Len;
	//����1024�ֽڵ���Ϣʹ��..
	unsigned short lenth,lenth_temp;
	unsigned short j;
	unsigned char Buff[12];
	unsigned char *Ptr;
	unsigned char *Point;
	unsigned char Buff1[4];
    unsigned long* BlockNumber;
	
	Len = -1;	
	i = GetDataClassTabItemNo( Logical_Name );			//���Ҷ�Ӧ��OBIS��
	if( i != DLMSDataSum )
	{
		*Dest = Get_data;								//�������ݷ���	
		if( Attribute == 0x01 )							//����1��logical_name
		{
			*(Dest+1) = Data_octet_string;
			*(Dest+2) = 0x06;
			RAM_Write( Dest+3, Logical_Name, 6 );
			Len = 9;
		}
		else											//����2��value
		{	
			Addr = DLMS_DATATab[i].Addr;
			Len = DLMS_DATATab[i].ComInfo & RS_LEN;
			*(Dest+1) = DLMS_DATATab[i].DataType;			
			//���1024�ֽڵ���Ϣ��������..
			if((*Logical_Name ==0)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==96)&&	(*(Logical_Name+3) ==99)&&	(*(Logical_Name+4) ==0)&&	(*(Logical_Name+5) ==0xff))
			{
				 BlockNumber = CommPara[COM_No].BlockNumber;				//����������Ϣ����Ҫʹ�����ݿ�ͨѶ..	
				 E2P_RData( Buff, Addr, 2 );
				 lenth = GetBuffToInteger(Buff);		//��Ϣ����
				 if(lenth>=1024) Len = 1024;
				 else Len = lenth;
				 if(Len<=127){
				 		*(Dest+2) = Len;
						E2P_RData( Dest+3, Addr+3, Len );
						Len += 3;
						*BlockNumber = 0;				 		
				 }else{
							 if(Len<=384){
				 					*(Dest+2) = 0x82;
				 					FillIntegerToBuff( Dest+3, Len );
				 					lenth = Len;
				 					Ptr = Dest+5;
				 					for(j=0;j<4;j++)
				 					{
				 						if(lenth>128) lenth_temp =128;
				 						else lenth_temp = lenth;	
										E2P_RData( Ptr, Addr+3+j*129, lenth_temp );
										if((lenth-lenth_temp)==0) break;
										lenth -= 128;
										Ptr +=128;
									}									
									Len += 5;
									*BlockNumber = 0;				 		
								}else{			//��Ҫ���ְ�����..
										*Dest = 0;										//last-block=0��ʾ�������һ֡���ݣ������������ݿ顣
										*BlockNumber = 1;
										RAM_Write( Buff, (unsigned char*)BlockNumber, 4 );
										SwapData( Buff, 4 );
										RAM_Write( Dest+1, Buff, 4 );					//block-number ������=1Ϊ����ĵ�һ�顣
										*(Dest+5) = 0x00;								//resultѡ��0��raw-data�����������ݲ���A-XDR�����ΪOCTETSTRING����Ϊ��ASN.1��û�й涨��OCTETSTRING�ĳ��ȣ��������µ�OCTETSTRING���ñ䳤��ʽ���루A-XDR����涨��
										*(Dest+6) = 0x82;								//���BITλ1��ʾ���ֽ�Ϊ��������룬02��ʾ��������ֽ�Ϊ����ֵ
										//��һ�����ݿ�ĳ���
										if(Len>1024) Len = 1024;
										lenth = 384;		//��һ���ȴ�512���ֽ�..
										FillIntegerToBuff( Dest+7, lenth );										
										*(Dest+9) = D_OctetStr;							//Data��������1,ָ���ΪSEQUENCE OF Data����
										*(Dest+10) = 0x82;								//���BITλ1��ʾ���ֽ�Ϊ��������룬02��ʾ��������ֽ�Ϊ����ֵ
										//���ݳ��� 
										FillIntegerToBuff( Dest+11, Len );
					 					
					 					lenth = 384;
					 					Ptr = Dest+13;
					 					for(j=0;j<4;j++)
					 					{
					 						if(lenth>128) lenth_temp =128;
					 						else lenth_temp = lenth;	
											E2P_RData( Ptr, Addr+3+j*129, lenth_temp );
											if((lenth-lenth_temp)==0) break;
											lenth -= 128;
											Ptr +=128;
										}																				
										//Len = 384 + 13;					 	
                    Len = 397;					 	
									}
				 			 }				
			}else{				
				//�������⴦�������ARRAY���ͺ�Data_structure���⴦��sky20140925..
				if((*(Dest+1) == Data_array)||(*(Dest+1) ==Data_structure)){
						if(*(Dest+1) ==Data_structure){
								 if(Len!=2) return 2;
								 *(Dest+2) = Len;					//array����..
								 *(Dest+3) = D_ENUM;
								 E2P_RData( Dest+4, Addr, 1 );  // 2�ֽ�..
								 *(Dest+5) = Data_bit_string;
								 *(Dest+6) = 32;
								 E2P_RData( Dest+7, Addr+2, 4 );  //4+1=5�ֽ�..
								 Len = 11;
						}else{
								   //if( *(Dest+1) == Data_array ){				
											*(Dest+2) = Len;					//array����..
											curr = 0;
											//���⴦��..						
										//	if(((*(Logical_Name+0) ==1))&&((*(Logical_Name+1) ==1)||(*(Logical_Name+1) ==2)||(*(Logical_Name+1) ==4))&&(*(Logical_Name+3) ==2))
											if((*(Logical_Name) ==1)&&(*(Logical_Name+1) ==65)&&(*(Logical_Name+2) ==0)&&((*(Logical_Name+3) ==134)||(*(Logical_Name+3) ==135))&&(*(Logical_Name+4) ==0)&&(*(Logical_Name+5) ==255))
											{
											E2P_RData(Dest+2,Addr,1);
											Addr+=2;
											for(i=0;i<*(Dest+2);i++)
												{
												Ptr = Buff;
												E2P_RData(Ptr,Addr,11);
												*(Dest+3+18*i) = Data_structure;
												*(Dest+4+18*i) = 4;
												*(Dest+5+18*i) = D_Unsigned16;
												RAM_Write(Dest+6+18*i,Ptr,2);//class_id
												*(Dest+8+18*i) = D_OctetStr;
												*(Dest+9+18*i) = 6;
												RAM_Write(Dest+10+18*i,Ptr+2,6);//logical_name
												*(Dest+16+18*i) = D_Integer8;
												RAM_Write(Dest+17+18*i,Ptr+8,1);//attribute_index
												*(Dest+18+18*i) = D_Unsigned16;
												RAM_Write(Dest+19+18*i,Ptr+9,2);//data_index
												Addr+=12;
												curr += 18;
												}
												
											/*
												 *(Dest+2) = 1;	
													for( i=0;i<Len;i++ )
													{
														*(Dest+3+curr) = Data_structure;
														*(Dest+4+curr) = 0x01;					//structure ��2Ԫ��				
														*(Dest+5+curr) = D_OctetStr;
														*(Dest+6+curr) = 0x06;
														 E2P_RData( Dest+7+curr, Addr+i*3, 6 );
														 curr += 10;					 
													}
											*/
													Len = curr + 3;
										  }else{
													for( i=0;i<Len;i++ )
													{
														*(Dest+3+curr) = D_Unsigned32;								
														 E2P_RData( Dest+4+curr, Addr+i*5, 4 );
														 curr +=5;					 
													}
													Len = curr + 3;					  	
										  }
                                                }
										
					}else{
							if(( *(Dest+1) == D_OctetStr )||( *(Dest+1) == D_VisibleStr ))
							{
							if(i<9)//17
								{	
							/*	Com_HT7017.Comm_Enable = DISABLE;
								
						//		NVIC_DisableIRQ(TIMER_2_IRQn); 
								if( DLMS_DATATab[i].Addr==IRMS1)
									{
									Point = Measurement.HT7017_Ia;
									UART_ReadInstan(Rms_I1);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									RAM_Write(Dest+3,Point,4);
									}
								else if( DLMS_DATATab[i].Addr==IRMS2){
									Point = Measurement.HT7017_Ib;
									UART_ReadInstan(Rms_I2);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									Delay(2000);
									RAM_Write(Dest+3,Point,4);
									}
								else if( DLMS_DATATab[i].Addr==I2Gain){
									Point = Measurement.HT7017_I2Gain;
									UART_ReadInstan(I2Gain_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									Delay(2000);
									RAM_Write(Dest+3,Point+2,2);	
									}
								else if( DLMS_DATATab[i].Addr==HFCONST){
									Point = Measurement.HT7017_HFConst;
									UART_ReadInstan(HFConst_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									Delay(2000);
									RAM_Write(Dest+3,Point+2,2);
									}
								else if( DLMS_DATATab[i].Addr==AGPBase){
									Point = Measurement.HT7017_P1Gain;
									UART_ReadInstan(GP1_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									RAM_Write(Dest+3,Point+2,2);//��һͨ���й����޹�����������
									}
								else if( DLMS_DATATab[i].Addr==AGPhs){
									Point = Measurement.HT7017_Gphs1;
									UART_ReadInstan(GPhs1_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									RAM_Write(Dest+3,Point+2,2);//��һͨ����λУ��
									}
								else if( DLMS_DATATab[i].Addr==E2AP1OFFSET)
									{
									Point = Buff1;
									RAM_Fill(Point,4);
									UART_ReadInstan(P1OFFSETH_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									MasureData_Get();
									UART_ReadInstan(P1OFFSETL_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									*(Point+0) = Measurement.HT7017_OffsetH[3];
									*(Point+1) = Measurement.HT7017_OffsetL[3];
									RAM_Write(Dest+3,Point,2);//PoffsetУ����С�ź��й�����У�������ֽ�
									}
								else if( DLMS_DATATab[i].Addr==AGPBase2){
									Point = Measurement.HT7017_P2Gain;
									UART_ReadInstan(GP2_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									RAM_Write(Dest+3,Point+2,2);//�ڶ�ͨ���й����޹�����������
									}
								else if( DLMS_DATATab[i].Addr==AGPhs2){
									Point = Measurement.HT7017_Gphs2;
									UART_ReadInstan(GPhs2_Address);
									Delay(Measur_Delay);
									Delay(Measur_Delay);
									RAM_Write(Dest+3,Point+2,2);//�ڶ�ͨ����λУ��
									}
								else
								E2P_RAdj( Dest+3, Addr, Len );
								*(Dest+2) = Len;
							//	E2P_RAdj( Dest+3, Addr, Len );
							//	SwapData(Dest+3,Len);
								
						//	NVIC_EnableIRQ(TIMER_2_IRQn); 
								Len += 3;
							*/
								}
							else
								{
								*(Dest+2) = Len;
								E2P_RData( Dest+3, Addr, Len );
								Len += 3;
								}
							}
							else
							{
								E2P_RData( Dest+2, Addr, Len );
								SwapData( Dest+2, Len );			//��λ��ǰ..
								Len += 2;
							}
					 }
			 }	 
		}	
	}		
	return Len;
}		


//ȡData������
short GetDataClassData( short COM_No,unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	short Len;
	unsigned char Attribute;
	
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );

	Attribute = *(Source+2+6);								//���������

	Len = GetRAMDataTab( Dest, Ptr, Attribute );			//��Data��RAMTab����
	if( Len != -1 ) return Len; 
	
	Len = GetDataTab( COM_No,Dest, Ptr, Attribute );				//ȡData��Tab����
	if( Len != -1 ) return Len; 
	else
	{
		*Dest = Get_data_access_result;						//�쳣��־
		*(Dest+1) = object_undefined; 						//�޶�Ӧ��OBIS��
		Len = 2;
	}
	return Len;		

}		

//������Register���ݣ�����ֵ��-1���޴��������-1���������ݳ���
short GetRegisterECTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute )
{
	unsigned short i;
	unsigned short Addr;
	short Len;
	unsigned char buff[6];
	unsigned char* ptr;
	unsigned long Hex_data;
	ptr=buff;
	
	Len = -1;
	for( i=0;i<DLMS_ECSum;i++ )						//�������
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_ECTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}
	if( i != DLMS_ECSum )
	{
	HT_FreeDog();
		*Dest = Get_data;								//�������ݷ���	
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Logical_Name, 6 );
				Len = 9;
				break;	
			case 2:									//value
			//	Addr = DLMS_ECTab[i].Addr;
				*(Dest+1) = D_Unsigned32;	
				/*
					///E2P_RData( ptr, Addr ,6);
					Read_CMonEC(ptr, Addr);
					Hex_data = BCD4_Long(ptr);//2016-6-23-fire ����϶��ĵ�λ��wh,����Kwh
					RAM_Write(Dest+2,(unsigned char*)&Hex_data,4);//2016-6-23-fire
					*/
			//	Read_CMonEC4_HEX_U( Dest+2, Addr );
				//	SwapData( Dest+2, 4 );
				if((Flag.Power & F_PwrUp) != 0)
					RAM_Write(Dest+2,DLMS_ECTab[i].ECRAds,4);
				else
					{
					E2P_RFM( Dest+2, DLMS_ECTab[i].ECFAds, ECDIGIT );
					if(E2PECIsOK(Dest+2)!=0)
						E2P_RData( Dest+2, DLMS_ECTab[i].Addr, ECDIGIT );

					}
				Len = 6;
				break;	
			case 3:									//scaler_unit
				*(Dest+1) = Data_structure;	
				*(Dest+2) = 02;	
				*(Dest+3) = D_Integer8;	
				*(Dest+4) = 0xFE;					//scaler:-2				
				*(Dest+5) = D_ENUM;	
				*(Dest+6) = DLMS_ECTab[i].Unit;		//unit: enum
				Len = 7;
				break;	
			default: break;			
		}	
	}
	return Len;	
}	

//���ۻ�����Register���ݣ�����ֵ��-1���޴��������-1���������ݳ���
short GetRegisterCUMDTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute )
{

	short i;
	unsigned short Addr;
	short Len;
	
	Len = -1;
	/*

	i = GetCUMDTabItemNo( Logical_Name );			//�ۻ��������		//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_CUMDSum )
	{
		*Dest = Get_data;								//�������ݷ���	
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Logical_Name, 6 );
				Len = 9;
				break;	
			case 2:									//value
				Addr = DLMS_CUMDTab[i].Addr;
				*(Dest+1) = D_Unsigned32;	
				E2P_RData( Dest+2, Addr, 4 );
				SwapData( Dest+2, 4 );
//				Len = 5;
				Len = 6;
				break;	
			case 3:									//scaler_unit
				*(Dest+1) = Data_structure;	
				*(Dest+2) = 02;	
				*(Dest+3) = D_Integer8;	
//				*(Dest+4) = 0xFE;					//scaler:-1				
				*(Dest+4) = 0xFF;					//scaler:-1				
				*(Dest+5) = D_ENUM;	
				*(Dest+6) = DLMS_CUMDTab[i].Unit;	//unit: enum
				Len = 7;
				break;	
			default: break;			
		}	
	}	*/
	return Len;	
	
}

//��ʵʱ����Register���ݣ�����ֵ��-1���޴��������-1���������ݳ���
short GetRegisterRealTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute )
{
	short i;
	unsigned char*  Addr;
	unsigned short Length;
	short Len;
//	unsigned short Temp;
	///long Value = 0;
	 long Value = 0;
	//unsigned long ULvalue;
	
	Len = -1;

	i = GetREALTabItemNo( Logical_Name );			//ʵʱ�������		//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_RealSum )
	{
		*Dest = Get_data;							//�������ݷ���	
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Logical_Name, 6 );
				Len = 9;
				break;	
			case 2:									//value
				*(Dest+1) = DLMS_REALTab[i].DataType;
				Addr = DLMS_REALTab[i].Addr;
				Length = DLMS_REALTab[i].ComInfo & RS_LEN;				
				RAM_Write( Dest+2, Addr, Length );
				RAM_Write( (unsigned char*)&Value, Addr, Length );
				//Value *= 100;//2λС��
				RAM_Write( Dest+2, (unsigned char*)&Value, Length );
														
				SwapData( Dest+2, Length );
				Len = Length+2;
				break;	
			case 3:									//scaler_unit
				*(Dest+1) = Data_structure;	
				*(Dest+2) = 02;	
				*(Dest+3) = D_Integer8;	
				*(Dest+4) = DLMS_REALTab[i].Scaler;	//scaler:-1				
				*(Dest+5) = D_ENUM;	
				*(Dest+6) = DLMS_REALTab[i].Unit;	//unit: enum
				Len = 7;
				break;	
			default: break;			
		}	
	}	
	return Len;	
}

//����ͨRegister���ݣ�����ֵ��-1���޴��������-1���������ݳ���
short GetRegisterRegTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute )
{
	short i;
	unsigned short Addr;
	unsigned short Length;
	short Len;
	
	Len = -1;

	i = GetRegTabItemNo( Logical_Name );			//ʵʱ�������		//���Ҷ�Ӧ��OBIS��
	if( i != DLMS_RegSum )
	{
		*Dest = Get_data;								//�������ݷ���	
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Logical_Name, 6 );
				Len = 9;
				break;	
			case 2:									//value
				*(Dest+1) = DLMS_RegTab[i].DataType;
				Addr = DLMS_RegTab[i].Addr;
				Length = DLMS_RegTab[i].ComInfo & RS_LEN;
				if(( *(Dest+1) == D_VisibleStr )||( *(Dest+1) == D_OctetStr ))								
				{
					#if(ACTIVE_FEE ==YesCheck)				//���Ʊ����׵�������EEPROM��..
					//0x01,0x00,0x00,0x02,0x07,0xff, // �������ֳ����ǹ̶���1--12����
						if((*Logical_Name ==1)&&	(*(Logical_Name+1) ==0)&&	(*(Logical_Name+2) ==0)&&	(*(Logical_Name+3) ==2)&&	(*(Logical_Name+4) ==7)&&	(*(Logical_Name+5) ==0xff))
						{
							//*(Dest+2) = Length;
							E2P_RData( Dest+2, Addr, 1 );
							if(*(Dest+2)>12) *(Dest+2) = 12;
							E2P_RData( Dest+3, Addr+2, *(Dest+2) );	
							//*(Dest+2) = Length;
							Len = *(Dest+2)+3;											
							return Len;	
						}
					#endif
					*(Dest+2) = Length;
					E2P_RData( Dest+3, Addr, Length );
					Len = Length+3;					
				}
				else 
				{
					E2P_RData( Dest+2, Addr, Length );
					SwapData( Dest+2, Length );
					Len = Length+2;
				}	
				break;	
			case 3:									//scaler_unit
				*(Dest+1) = Data_structure;	
				*(Dest+2) = 0x02;	
				*(Dest+3) = D_Integer8;	
				*(Dest+4) = DLMS_RegTab[i].Scaler;	//scaler:-1				
				*(Dest+5) = D_ENUM;	
				*(Dest+6) = DLMS_RegTab[i].Unit;	//unit: enum
				Len = 7;
				break;	
			default: break;			
		}	
	}	
	return Len;	
}

//ȡRegister������
short GetRegisterClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	short Len;
	unsigned char Attribute;
	
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );

	Attribute = *(Source+2+6);								//���������

#if ( MEASCHIP == IDT90E32 )							//12.08.29	
	Len = GetRegisterIDTAdjTab( Dest, Logical_Name, Attribute ); 
	if( Len != -1 ) return Len;
#endif

	Len = GetRegisterECTab( Dest, Ptr, Attribute );			//����������
	if( Len != -1 ) return Len; 
	Len = GetRegisterCUMDTab( Dest, Ptr, Attribute );		//���ۻ���������
	if( Len != -1 ) return Len; 
	Len = GetRegisterRealTab( Dest, Ptr, Attribute );		//��ʵʱ��������
	if( Len != -1 ) return Len; 
#if ( IDTHarmonic == YesCheck )
	Len = GetRegisterHRTab( Dest, Ptr, Attribute );			//��г����������
	if( Len != -1 ) return Len; 
#endif

	Len = GetRegisterRegTab( Dest, Ptr, Attribute );		//����ͨ����
	if( Len != -1 ) return Len; 
	else
	{
		*Dest = Get_data_access_result;						//�쳣��־
		*(Dest+1) = object_undefined; 						//�޶�Ӧ��OBIS��
		Len = 2;
	}
	return Len;		
}	

unsigned short GetMDTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;				
	
	for( i=0;i<DLMS_MDSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_MDTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;
}	

//ȡExtendedRegister������
short GetExtendedRegisterClassData( unsigned char* Dest, unsigned char* Source ) 
{
	short i;
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned short Addr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned char buff[6];
	unsigned char* ptr_data;
	unsigned long Hex_data;
	ptr_data=buff;
	
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );
	
	i = GetMDTabItemNo( Ptr );							//���Ҷ�Ӧ��OBIS��
	if( i == DLMS_MDSum )								
	{
		*Dest = Get_data_access_result;					//�쳣��־
		*(Dest+1) = object_undefined; 					//�޶�Ӧ��OBIS��
		Len = 2;		
	}
	else 
	{
		*Dest = Get_data;											//�������ݷ���	
		Attribute = *(Source+2+6);						//���������
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:									//value
				*(Dest+1) = D_Unsigned32;
				Addr = DLMS_MDTab[i].Addr;
			/*
				#if ( LastAveragePf ==	YesCheck )
							if(DLMS_MDTab[i].Unit==Unit_FF)	;	//E2P_RFM( Dest+2, Addr, 4 );	2016-6-15 fire
							else 		E2P_RData( Dest+2, Addr, 4 );
				#else
							E2P_RData( Dest+2, Addr, 4 );
				#endif		
				//E2P_RData( Dest+2, Addr, 4 );
				SwapData( Dest+2, 4 );
			*/
			//��BCD����ת��Hex���ݷ��ͻ�ȥ
				///Read_CMonEC( ptr_data, Addr );//2016-6-23-fire
				///E2P_RData( ptr_data, Addr , 3);
			//	RAM_Fill(ptr_data,6);
				E2P_RData( Dest+2, Addr , 4);
			//	Hex_data = BCD4_Long(ptr_data)*10;//2016-6-23-fire ��Ϊ�Ǵ�һ��С���㣬����ά��������Զ�����һ��С����ǰ��һλ��ʹ��������С10��
			//	RAM_Write(Dest+2,(unsigned char*)&Hex_data,4);//2016-6-23-fire
				SwapData( Dest+2, 4 );
				Len = 6;
				break;	
			case 3:									//scaler_unit
				*(Dest+1) = Data_structure;	
				*(Dest+2) = 0x02;	
				*(Dest+3) = D_Integer8;
				#if ( LastAveragePf ==	YesCheck )
							if(DLMS_MDTab[i].Unit==Unit_FF)	*(Dest+4) = 0xFD;	//scaler:-1	
							else 		*(Dest+4) = 0xFF;	//scaler:-1
				#else
							*(Dest+4) = 0xFF;	//scaler:0		
				#endif				
//				*(Dest+4) = DLMS_MDTab[i].Scaler;	//scaler:-1				
			//	*(Dest+4) = 0xFF;	//scaler:-1				
				*(Dest+5) = D_ENUM;	
				*(Dest+6) = DLMS_MDTab[i].Unit;	//unit: enum
				Len = 7;
				break;	
			case 4:									//status
				*(Dest+1) = D_Unsigned8;	
				*(Dest+2) = 0x00;	
				Len = 3;
				break;	
			case 5:									//capture_time
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x0C;
				Addr = DLMS_MDTab[i].MDTAddr;
				//E2P_RData( Dest+3, Addr, 12 );
			/*	
				#if ( LastAveragePf ==	YesCheck )
							if(DLMS_MDTab[i].Unit==Unit_FF)	;//E2P_RFM( Dest+3, Addr, 12 );   //2016-6-15 fire
							else 		E2P_RData( Dest+3, Addr, 12 );
				#else
							E2P_RData( Dest+3, Addr, 12 );
				#endif	
			*/	
				///E2P_RData( ptr_data, Addr, 5 );		//2016-6-24-fire ��ȡ�洢��BCDʱ��
				E2P_RData( ptr_data, Addr, 5 );
				MoveTimeBCD_Hex(Dest+3,ptr_data);	//2016-6-24-fire	BCDʱ��ת����Hexʱ�䷢��ȥ
				Len = 15;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

//ȡClock������
short GetClockClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned short Temp;
	unsigned char Len=2;
	unsigned char Attribute;
	
	Ptr = Buff;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x01;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//�쳣��־
		*(Dest+1) = object_undefined; 					//�޶�Ӧ��OBIS��
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//�������ݷ���	
		Attribute = *(Source+2+6);						//���������
		GetTime();
		MoveCurrentTimeBCD_Hex();
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:									//time
				// *(Dest+1) = Data_DateTime;
			//	RAM_Write( Dest+2, (unsigned char*)&Clk.YearH, 12 );
				*(Dest+1) = D_OctetStr;				
				 *(Dest+2) = 0x0C;            
				RAM_Write( Dest+3, (unsigned char*)&Clk.YearH, 12 );
				Len = 15;
				break;	
			case 3:									//time_zone
				*(Dest+1) = D_Integer16;	
				*(Dest+2) = Clk.DeviatH;			//��GMTʱ��  ����
				*(Dest+3) = Clk.DeviatL;	
				Len = 4;
				break;	
			case 4:									//status
				*(Dest+1) = D_Unsigned8;	
				*(Dest+2) = Clk.State;	
				Len = 3;
				break;	
			case 5:									//daylight_savings_begin
			case 6:									//daylight_savings_end
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x0C;
				*(Dest+3) = 0xFF;
				*(Dest+4) = 0xFF;
				*(Dest+5) = 0xFF;
				*(Dest+6) = 0xFF;
				*(Dest+7) = 0xFF;
				*(Dest+8) = 0xFF;
				*(Dest+9) = 0xFF;
				*(Dest+10) = 0xFF;
				*(Dest+11) = 0xFF;
				*(Dest+12) = 0x80;
				*(Dest+13) = 0x00;
				*(Dest+14) = 0x00;
				/*
//				RAM_DataFill( Dest+3, 12, 0xFF);
				if( Attribute == 5 ) Temp = Clock_daylight_savings_begin;
				else Temp = Clock_daylight_savings_end;
				E2P_RData( Dest+3, Temp, 12 );	
				*/
				Len = 15;
				break;	
			case 7:									//daylight_savings_deviation
				*(Dest+1) = D_Integer8;	
				*(Dest+2) = 0x00;	
//				E2P_RData( Dest+2, Clock_daylight_savings_deviation, 1 );
				Len = 3;
				break;	
			case 8:									//daylight_savings_enabled
				*(Dest+1) = D_BOOLEAN;	
				*(Dest+2) = 0x00;	
//				E2P_RData( Dest+2, Clock_daylight_savings_enabled, 1 );
				Len = 3;
				break;	
			case 9:									//clock_base
				*(Dest+1) = D_ENUM;	
			//	*(Dest+2) = 0x01;					//�ڲ�����
				*(Dest+2) = 0x00; //not defined
				Len = 3;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

//ȡSAP_assignment������
short GetSAP_assignmentClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	
	Ptr = Buff;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//�쳣��־
		*(Dest+1) = object_undefined; 					//�޶�Ӧ��OBIS��
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//�������ݷ���	
		Attribute = *(Source+2+6);						//���������
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:									//time
				*(Dest+1) = Data_array;
				*(Dest+2) = 0x01;					//����
				*(Dest+3) = Data_structure;
				*(Dest+4) = 0x02;					//structure ��2Ԫ��
				*(Dest+5) = D_Unsigned16;
//				*(Dest+6) = 0x00;					//SAP=0x0001
//				*(Dest+7) = 0x01;
				E2P_RData( Dest+6, ESAP, 2 );
				SwapData( Dest+6, 2 );
				*(Dest+8) = D_OctetStr;
				*(Dest+9) = 0x10;					//���� = 16
				
//				*(Dest+10) = 'S';
//				*(Dest+11) = 'T';
//				*(Dest+12) = 'C';
//				*(Dest+13) = '7';
//				*(Dest+14) = '9';
//				*(Dest+15) = '9';
//				*(Dest+16) = '2';
//				*(Dest+17) = '8';
//				*(Dest+18) = '7';
//				*(Dest+19) = '4';
//				*(Dest+20) = '3';
//				*(Dest+21) = 0;
//				*(Dest+22) = 0;
//				*(Dest+23) = 0;
//				*(Dest+24) = 0;
//				*(Dest+25) = 0;
//				E2P_WData( Logical_device_name, Dest+10, 16 );
//				E2P_WData( Electricity_ID1, Dest+13, 8 );

				E2P_RData( Dest+10, Logical_device_name, 16 );
				Len = 26;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

//ȡIECLocalPortSetup������ //�������ֹ��ں�485��
short GetIECLocalPortSetupClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned char Temp_FLAG=0;	
		
	Ptr = Buff;
	
	Buff[0] = 0x00;				//����
	Buff[1] = 0x00;
	Buff[2] = 0x14;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;

	Buff[6] = 0x00;				//485-1 ��
	Buff[7] = 0x00;
	Buff[8] = 0x14;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) == 0 ) Temp_FLAG = 1;
	else if( Data_Comp( Ptr+6, Source+2, 6 ) == 0 ) Temp_FLAG = 2; 
	else Temp_FLAG = 0;
	
	if( Temp_FLAG == 0 )
	{
		*Dest = Get_data_access_result;					//�쳣��־
		*(Dest+1) = object_undefined; 					//�޶�Ӧ��OBIS��
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//�������ݷ���	
		Attribute = *(Source+2+6);						//���������
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Source+2, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_ENUM;	
				if( Temp_FLAG == 1 ) 
				{
					E2P_RData( Dest+2, IEC_local_default_mode1, 1 );
//					if( *(Dest+2) > 1 ) *(Dest+2) = 0;					//0:IEC 62056-21   1:IEC 62056-46
					if( *(Dest+2) > 2 ) *(Dest+2) = 0;					//0:IEC 62056-21   1:IEC 62056-46 2:�޶��� version = 1
					E2P_WData( IEC_local_default_mode1, Dest+2, 1 );
				}	
				else if( Temp_FLAG == 2 )
				{
					E2P_RData( Dest+2, IEC_local_default_mode0, 1 );
//					if( *(Dest+2) > 1 ) *(Dest+2) = 0;					//0:IEC 62056-21   1:IEC 62056-46
					if( *(Dest+2) > 2 ) *(Dest+2) = 1;					//0:IEC 62056-21   1:IEC 62056-46 2:�޶��� version = 1
					E2P_WData( IEC_local_default_mode0, Dest+2, 1 );
				}	
				Len = 3;
				break;	
			case 3:
				*(Dest+1) = D_ENUM;	
				if( Temp_FLAG == 1 ) 
				{
					E2P_RData( Dest+2, IEC_local_default_baud1, 1 );
//					if( *(Dest+2) > 9 ) *(Dest+2) = 0;					//0~9: 0:300
					if( *(Dest+2) > 6 ) *(Dest+2) = 0;					//0~9: 0:300  //���֧�ֵ�19200
					E2P_WData( IEC_local_default_baud1, Dest+2, 1 );
				}	
				else if( Temp_FLAG == 2 ) 
				{
					E2P_RData( Dest+2, IEC_local_default_baud0, 1 );
//					if( *(Dest+2) > 9 ) *(Dest+2) = 0;					//0~9: 0:300
					if( *(Dest+2) > 6 ) *(Dest+2) = 5;					//0~9: 0:300  //���֧�ֵ�19200
					E2P_WData( IEC_local_default_baud0, Dest+2, 1 );
				}	
				Len = 3;
				break;	
			case 4:
				*(Dest+1) = D_ENUM;	
				if( Temp_FLAG == 1 )
				{
					E2P_RData( Dest+2, IEC_local_prop_baud1, 1 );
//					if( *(Dest+2) > 9 ) *(Dest+2) = 0;					//0~9: 0:300
					if( *(Dest+2) > 6 ) *(Dest+2) = 0;					//0~9: 0:300  //���֧�ֵ�19200
					E2P_WData( IEC_local_prop_baud1, Dest+2, 1 );
				}	
				else if( Temp_FLAG == 2 )
				{
					E2P_RData( Dest+2, IEC_local_prop_baud0, 1 );
//					if( *(Dest+2) > 9 ) *(Dest+2) = 0;					//0~9: 0:300
					if( *(Dest+2) > 6 ) *(Dest+2) = 5;					//0~9: 0:300  //���֧�ֵ�19200
					E2P_WData( IEC_local_prop_baud0, Dest+2, 1 );
				}	
				Len = 3;
				break;	
			case 5:
				*(Dest+1) = D_ENUM;	
				if( Temp_FLAG == 1 ) 
				{
					E2P_RData( Dest+2, IEC_local_response_time1, 1 );				
					if( *(Dest+2) != 0 ) *(Dest+2) = 0;					//0: 20ms  1:200ms
					E2P_WData( IEC_local_response_time1, Dest+2, 1 );
				}	
				else if( Temp_FLAG == 2 ) 
				{
					E2P_RData( Dest+2, IEC_local_response_time0, 1 );
					if( *(Dest+2) != 0 ) *(Dest+2) = 0;					//0: 20ms  1:200ms
					E2P_WData( IEC_local_response_time0, Dest+2, 1 );
				}	
				Len = 3;
				break;	
			case 6:
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x08;
				if( Temp_FLAG == 1 ) E2P_RData( Dest+3, IEC_local_device_addr1, 8 );
				else if( Temp_FLAG == 2 )E2P_RData( Dest+3, IEC_local_device_addr0, 8 );
				Len = 11;
				break;	
			case 7:
			case 8:
			case 9:
				*Dest = Get_data_access_result;					//�쳣��־
				*(Dest+1) = read_write_denied; 					
				Len = 2;		
				break;					
			default: break;			
		}
	}		
	return Len;
}	

//ȡIECHDLCSetup������
short GetIECHDLCSetupClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned short Temp;
	unsigned char Temp_FLAG=0;	
	
	Ptr = Buff;
	
	Buff[0] = 0x00;				//���趨ΪP1ʹ�ã�������ͨ��p1�������Ԫ�������ϵ�����clients
	Buff[1] = 0x00;
	Buff[2] = 0x16;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Buff[6] = 0x00;				//���趨ΪP5ʹ�ã�������ͨ��p5�������Ԫ�������ϵ�����clients
	Buff[7] = 0x01;
	Buff[8] = 0x16;
	Buff[9] = 0x00;
	Buff[10] = 0x00;
	Buff[11] = 0xff;
		
	if( Data_Comp( Ptr, Source+2, 6 ) == 0 ) 
		{
		Temp_FLAG = 1;
		E2P_RData((unsigned char*)&Temp,HDLC_inactivity_time_out,2);
		Para.RHDLC_inactivity_time_out = Temp;
		}
	else if( Data_Comp( Ptr+6, Source+2, 6 ) == 0 )
		{
		Temp_FLAG = 2; 
		
		E2P_RData((unsigned char*)&Temp,HDLC_inactivity_time_out1,2);
		Para.RHDLC_inactivity_time_out1= Temp;
		}
	
	if( Temp_FLAG == 0 )
	{
		*Dest = Get_data_access_result;					//�쳣��־
		*(Dest+1) = object_undefined; 					//�޶�Ӧ��OBIS��
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//�������ݷ���	
		Attribute = *(Source+2+6);						//���������
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Source+2, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_ENUM;										//HDLC����(2)				enum		0~9:5(9600)	
			//	*(Dest+2) = 6;//57600 baud

				if( Temp_FLAG == 1 ) 
				{
					E2P_RData( Dest+2, HDLC_comm_speed, 1 );
					if( *(Dest+2) > 9 ) 
	//				if( *(Dest+2) > 6 ) 									//���֧�ֵ�19200
					{
						*(Dest+2) = 5;
						E2P_WData( HDLC_comm_speed, Dest+2, 1 );
					}
				}else if( Temp_FLAG == 2 ) 
				{
					E2P_RData( Dest+2, HDLC_comm_speed1, 1 );
					if( *(Dest+2) > 9 ) 
	//				if( *(Dest+2) > 6 ) 									//���֧�ֵ�19200
					{
						*(Dest+2) = 5;
						E2P_WData( HDLC_comm_speed1, Dest+2, 1 );
					}
				}	
				
				Len = 3;
				break;	
			case 3:														//HDLC���ʹ��ڳߴ�(2)		unsigned	1~7:1
			case 4:														//HDLC���մ��ڳߴ�(2)		unsigned	1~7:1
				*(Dest+1) = D_Unsigned8;	
				*(Dest+2) = 0x01;	
				Len = 3;
				break;	
			case 5:														//HDLC��֡���ͳ���(3)		//unsigned16	32~2030:128(DLMS)	//unsigned8 32~128(IEC62056-62) 
			case 6:														//HDLC��֡���ճ���(3)		//unsigned16	32~2030:128(DLMS)	//unsigned8 32~128(IEC62056-62) 
				*(Dest+1) = D_Unsigned8;			//version = 0
				*(Dest+2) = MAX_INFO_SIZE;			//version = 0
				Len = 3;							//version = 0

			/*	*(Dest+1) = D_Unsigned16;			//version = 1
				*(Dest+2) = 0x03;					//768bytes
				*(Dest+3) = 0x00;

				if( Temp_FLAG == 1 ) 
				{		//���ӿ�--485-1	
					*(Dest+3) = MAX_INFO_SIZE;			//version = 1
				}else if( Temp_FLAG == 2 )
				{	//����	
					*(Dest+3) = MAX_485_SIZE;			//version = 1					
				}
				
				Len = 4;							//version = 1
				*/
				break;	
			case 7:
				*(Dest+1) = D_Unsigned16;								//HDLC�ֽڼ��ʱ��(3)		unsigned16	20~6000:1000
			//	*(Dest+2) = 0x00;//25ms
			//	*(Dest+3) = 0x19;

				
//				E2P_RData( Dest+2, HDLC_inter_octet_time_out, 2 );
				if( Temp_FLAG == 1 ) 
				{		//485���ӿ�
						E2P_RData( (unsigned char*)&Temp, HDLC_inter_octet_time_out, 2 );
						if(( Temp < 20 )||( Temp > 6000 )) 
						{
							Temp = 1000;
							E2P_WData( HDLC_inter_octet_time_out, (unsigned char*)&Temp, 2 );
						}	
				}else if( Temp_FLAG == 2 )
				{		//����	
						E2P_RData( (unsigned char*)&Temp, HDLC_inter_octet_time_out1, 2 );
						if(( Temp < 20 )||( Temp > 6000 )) 
						{
							Temp = 1000;
							E2P_WData( HDLC_inter_octet_time_out1, (unsigned char*)&Temp, 2 );
						}						
				}
				FillIntegerToBuff( Dest+2, Temp );	
			
				Len = 4;
				break;	
			case 8:
				*(Dest+1) = D_Unsigned16;	//HDLC��ֹ�˳�ʱ��(3)		unsigned16	0~120:120
			/*	if(Temp_FLAG == 2)
					{
					*(Dest+2) = 0x00;//0s������Ĺ��ܲ�ʹ��
					*(Dest+3) = 0x00;
					}
				else if(Temp_FLAG == 1)
					{
					*(Dest+2) = 0x00;//60s
					*(Dest+3) = 0x3c;
					}
				*/

//				E2P_RData( Dest+2, HDLC_inactivity_time_out, 2 );
				if( Temp_FLAG == 1 ) 
				{	//485���ӿ�		
						E2P_RData( (unsigned char*)&Temp, HDLC_inactivity_time_out, 2 );
						if( Temp > 120 ) 
						{
							Temp = 120;
							E2P_WData( HDLC_inactivity_time_out, (unsigned char*)&Temp, 2 );
						}	
				}else if( Temp_FLAG == 2 )
				{//����
						E2P_RData( (unsigned char*)&Temp, HDLC_inactivity_time_out1, 2 );
						if( Temp > 120 ) 
						{
							Temp = 120;
							E2P_WData( HDLC_inactivity_time_out1, (unsigned char*)&Temp, 2 );
						}					
				}
				FillIntegerToBuff( Dest+2, Temp );
				
				Len = 4;
				break;	
			case 9:
				*(Dest+1) = D_Unsigned16;								//HDLC��ַ(3)				unsigned16	0x0010~0x3ffd:���ĩ4λ��1000������1234+1000=2234=0x08BA
			//	*(Dest+2) = 0x00;
			//	*(Dest+3) = 0x10;
			
//				if( Temp_FLAG == 1 ) 
//				{	//485���ӿ�					
					E2P_RData( (unsigned char*)&Temp, HDLC_device_address, 2 );
//				}
//				else if( Temp_FLAG == 2 )
//				{//����	
//					E2P_RData( (unsigned char*)&Temp, HDLC_device_address1, 2 );	//�豸��ַӦ��һ��
//				}	
				if( Temp > 0x3ffd) Temp = 0x260f;
				FillIntegerToBuff( Dest+2, Temp );
				
				Len = 4;
				break;	
			default: break;			
		}
	}		
	return Len;
}	


void FillLongToBuff( unsigned char* Dest, unsigned long Data )
{
	RAM_Write( Dest, (unsigned char*)&Data, 4 );
	SwapData( Dest, 4 );
}	


void FillIntegerToBuff( unsigned char* Dest, unsigned short Data )
{
	*Dest = Data / 256;
	*(Dest+1) = Data % 256;
}	

unsigned short GetBuffToInteger( unsigned char* Data )
{
	unsigned short Temp;
	
	Temp = *Data;
	Temp *= 256;
	Temp += *(Data+1);

	return Temp;
}

unsigned long GetBuffToLong( unsigned char* Data )
{
	unsigned char Buff[4];
	unsigned long Value;
	unsigned char* Ptr;
	
	Ptr = Buff;
	RAM_Write( Ptr, Data, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value, Ptr, 4 );
	
	return Value;
}	


short GetRequestNormalProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short ClassId;
	unsigned char InvokeId;
	short Len=3;
	
	InvokeId = *(RecBuff+2);

	ClassId = *(RecBuff+3);
	ClassId *= 256;
	ClassId += *(RecBuff+4);

	*SendBuff = APDU_GET_Response;
	*(SendBuff+1) = get_response_normal;
	*(SendBuff+2) = InvokeId;

  HComm.Send_state_InvokeId = InvokeId;
  
	switch( ClassId )
	{
		case Class_id_Data:								//1
			Len += GetDataClassData( COM_No,SendBuff+3, RecBuff+3 ); 
			//if(( Len > 389 )&&( *CommPara[COM_No].BlockNumber != 0 ))
			if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 ))
				{
					 *(SendBuff+1) = get_response_with_datablock;	
					 HComm.COSEM_BlockState[COM_No] = FIRSTDATABLOCK;
					 HComm.BIGBlockNumber[COM_No]  = 1;
				}
				else
				{
					 HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;
					 HComm.BIGBlockNumber[COM_No]  = 0;
			  }		
			break;
		case Class_id_Register:							//3
			Len += GetRegisterClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Extended_register:				//4
			Len += GetExtendedRegisterClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Demand_register:					//5
			Len += GetDemand_registerClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Register_activation:				//6
       		Len += GetRegister_ActivationClassData( SendBuff+3, RecBuff+3 );                   
			break;
		case Class_id_Profile_generic:					//7
			Len += GetProfile_genericClassData( COM_No, SendBuff+3, RecBuff+3 ); 
			//if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 )) *(SendBuff+1) = get_response_with_datablock;
			if( *CommPara[COM_No].BlockNumber != 0 )
				{
					 *(SendBuff+1) = get_response_with_datablock;	
					 HComm.COSEM_BlockState[COM_No] = FIRSTDATABLOCK;
					 HComm.BIGBlockNumber[COM_No]  = 1;
				}		
			break;
//		case Class_id_Utility_tables:					//26
//			break;
//		case Class_id_Register_table:					//61
//			break;
//		case Class_id_Status_mapping:					//63
//			break;
//		case Class_id_Association_SN:					//12
//			break;
		case Class_id_Association_LN:					//15
			Len += GetAssociation_LNClassData( COM_No, SendBuff+3, RecBuff+3 ); 
//			if(( Len > 10 )&&(*(RecBuff+11) == 0x02 )) *(SendBuff+1) = get_response_with_datablock;
			if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 ))
				{
					 *(SendBuff+1) = get_response_with_datablock;	
					 HComm.COSEM_BlockState[COM_No] = FIRSTDATABLOCK;
					 HComm.BIGBlockNumber[COM_No]  = 1;
				}		
			break;
		case Class_id_SAP_assignment:					//17
			Len += GetSAP_assignmentClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Clock:							//8
			Len += GetClockClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Script_table:						//9
			Len += GetScript_tableClassData( SendBuff+3, RecBuff+3 ); 
			break;
//		case Class_id_Schedule:							//10
//			break;
		case Class_id_Special_days_table:				//11
			Len += GetSpecial_daysClassData( COM_No, SendBuff+3, RecBuff+3 ); 
			//if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 )) *(SendBuff+1) = get_response_with_datablock;
			if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 ))
			{
					 *(SendBuff+1) = get_response_with_datablock;	
					 HComm.COSEM_BlockState[COM_No] = FIRSTDATABLOCK;
					 HComm.BIGBlockNumber[COM_No]  = 1;
			}	
			break;
		case Class_id_Activity_calendar:				//20
			Len += GetActivity_calendarClassData( COM_No, SendBuff+3, RecBuff+3 ); 
		//	if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 )) *(SendBuff+1) = get_response_with_datablock;
			if(( Len > 10 )&&( *CommPara[COM_No].BlockNumber != 0 )) 
			{
					 *(SendBuff+1) = get_response_with_datablock;	
					 HComm.COSEM_BlockState[COM_No] = FIRSTDATABLOCK;
					 HComm.BIGBlockNumber[COM_No]  = 1;
			}
			break;
		case Class_id_Register_monitor:					//21
		Len += GetRegister_monitorClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Single_action_schedule:			//22
			Len += GetSingle_action_scheduleClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Disconnect_control:				//70
			Len += GetDisconnect_controlClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Limiter:									//71
			Len += GetLimiterClassData( SendBuff+3, RecBuff+3 ); 
			break;
    case Class_id_Image_transfer:						//18
			Len += GetImage_transferClassData( SendBuff+3, RecBuff+3  ); 
			break;						
		case Class_id_IEC_local_port_setup:			//19
			Len += GetIECLocalPortSetupClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_IEC_HDLC_setup:						//23		
			Len += GetIECHDLCSetupClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Auto_answer_setup:					//28		
			Len +=  Get_Auto_answer_setupClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Auto_connect_setup:					//29		
			Len +=  Get_Auto_connect_setupClassData( SendBuff+3, RecBuff+3  ); 
			break;
		case Class_id_Push_Setup:					//40		
		    Len +=  Get_PushsetupClassData(SendBuff+3, RecBuff+3  ); 
			break;			
		case Class_id_TCP_UDP_setup:						//41		
			Len += GetTCP_UDP_setupClassData( SendBuff+3, RecBuff+3 ); 
			break;
	  case Class_id_IPv4_setup:						//42		
			Len += GetIPv4_setupClassData( SendBuff+3, RecBuff+3 ); 
			break;
	  case Class_id_MACaddr_GPRS:						//43		
			Len += GetMACaddr_GPRSClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_GPRS_modem_setup:					//45		
			Len += GetGPRS_modem_setupClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_SMTP_setup:								//46		
			Len += GetSMTP_setupClassData( SendBuff+3, RecBuff+3 ); 
			break;			
		case Class_id_Security_setup:								//64		
			Len += GetSecurity_setupClassData( SendBuff+3, RecBuff+3 ); 
			break;
		case Class_id_Manufacture_setup:								//128 ---���쳧���Զ�����..		
			Len += GetManufacture_setupData( SendBuff+3, RecBuff+3 ); 
			break;									
		default: 
			break;			
	}
		//���־λ..
		if( *CommPara[COM_No].BlockNumber == 0 )  	 HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;
		
    return Len;	
}	

short Get_with_datablock_Abnormity( unsigned char* Dest, unsigned char AbnormityType )
{
	*Dest = 0x01;					//last-block 
	*(Dest+1) = 0x00;			
	*(Dest+2) = 0x00;			
	*(Dest+3) = 0x00;			
	*(Dest+4) = 0x01;					//block-number
	*(Dest+5) = 0x01;					//IMPLICIT Data-Access-Result			
	*(Dest+6) = AbnormityType;
	
	return 10;
}	

//===============================================================================================================================================        
short JudgeRequestNext( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long* BlockNumber;
	unsigned short Value;
	short Len = 0;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	
	RAM_Write( Ptr, RecBuff+3, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value, Ptr, 4 );

	if( *BlockNumber == 0 )
	{
		Len = Get_with_datablock_Abnormity( SendBuff+3, no_long_get_in_progress );
	}	

	if( *BlockNumber != Value )
	{
		*(SendBuff+3) = 0x01;					//last-block 
		RAM_Write( SendBuff+4, RecBuff+3, 4 );//block-number
		*(SendBuff+8) = 0x01;					//IMPLICIT Data-Access-Result			
		*(SendBuff+9) = long_get_aborted;
		Len = 10;
	}	
	
	return Len;
}

short GetRequestNextProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short ClassId;
	unsigned char InvokeId;
	unsigned char* SubsBlockPara;
	short Len=3;

	InvokeId = *(RecBuff+2);

	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	SubsBlockPara += 3;									//12.03.06	 
	ClassId = *SubsBlockPara;
	ClassId *= 256;
	ClassId += *(SubsBlockPara+1);

	*SendBuff = APDU_GET_Response;
	*(SendBuff+1) = get_response_with_datablock;
	*(SendBuff+2) = InvokeId;

  HComm.Send_state_InvokeId = InvokeId;
  
	switch( ClassId )
	{
		case Class_id_Data:
			Len = JudgeRequestNext( COM_No, RecBuff, SendBuff );
			if( Len == 0 )
			{
				Len = GetNextAssociation_messageClassData( COM_No, RecBuff+3, SendBuff+3 );	
				Len += 3;
			}				
			break;
		case Class_id_Association_LN:					//15
			Len = JudgeRequestNext( COM_No, RecBuff, SendBuff );
			if( Len == 0 )
			{
			/*
//				Len = GetNextAssociation_LNClassData( COM_No, SendBuff+3, RecBuff+3 );
				if(Flag.Asso == 1)
					Len = GetNextAssociation_LN1ClassData( COM_No, RecBuff+3, SendBuff+3 );	
				if(Flag.Asso == 2)
					Len = GetNextAssociation_LN2ClassData( COM_No, RecBuff+3, SendBuff+3 );	
				if(Flag.Asso == 3)
					*/
					Len = GetNextAssociation_LNClassData( COM_No, RecBuff+3, SendBuff+3 );	
			///	if(Flag.Asso == 4)					
			///		Len = GetNextAssociation_LNClassData( COM_No, RecBuff+3, SendBuff+3 );	

				Len += 3;
			}	
			break;
		case Class_id_Profile_generic:					//7
			Len = JudgeRequestNext( COM_No, RecBuff, SendBuff );
			if( Len == 0 )
			{
			Len = GetNextProfile_genericClassData( COM_No, RecBuff+3, SendBuff+3 );	
				Len += 3;
			}	
			break;
		case Class_id_Special_days_table:				//11
			Len = JudgeRequestNext( COM_No, RecBuff, SendBuff );
			if( Len == 0 )
			{
//				Len = GetNextSpecial_daysClassData( COM_No, SendBuff+3, RecBuff+3 ); 
			Len = GetNextSpecial_daysClassData( COM_No, RecBuff+3, SendBuff+3 ); 
				Len += 3;
            }    
			break;
		case Class_id_Activity_calendar:				//20
			Len = JudgeRequestNext( COM_No, RecBuff, SendBuff );
			if( Len == 0 )
			{
				Len = GetNextActivity_calendarClassData( COM_No, RecBuff+3, SendBuff+3 ); 
				Len += 3;
			}	
			break;
		default: 
			*CommPara[COM_No].BlockNumber = 0;
			HComm.BIGBlockNumber[COM_No] = 0;
			HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;    //skyblue20140927..
			Len = JudgeRequestNext( COM_No, RecBuff, SendBuff );
			break;
	}		
	return Len;	
}	

//�������ݿ�MESSAGE1024�Ĳ���..
short GetNextAssociation_messageClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned char Buff[6];	
	//unsigned long* BlockNumber;
  	unsigned char* Logical_name;
	unsigned char* SubsBlockPara;
    unsigned char* Ptr;
  	unsigned char Attribute;
	unsigned short Addr;
	unsigned short lenth,lenth_temp;
	unsigned short Len;
	unsigned short Temp,j;
	unsigned short Sendlen;
	
	
	//BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	Logical_name = SubsBlockPara+MAC_Get_Set_Logical_name;
	Attribute = *(SubsBlockPara+MAC_Get_Set_Attribute);

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;			//96
	Buff[3] = 0x0D;			//13
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Buff, Logical_name, 6 ) != 0 ) 
	{
		Len = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
		return Len;		
	}	
	switch( Attribute )
	{
		case 2:											//entries (static) array
			Addr = Consumer_Message_Text_num;
			E2P_RData( Buff, Addr, 2 );
			Len = GetBuffToInteger(Buff);		//��Ϣ����
			if(Len>1024) Len = 1024;			
			//Sendlen = 384*(*BlockNumber);
			//Addr += Sendlen + 3*(*BlockNumber); 	// 128*3 +3 =129*3
			Sendlen = 384*(HComm.BIGBlockNumber[COM_No]);
			Addr += Sendlen + 3*(HComm.BIGBlockNumber[COM_No]); 	// 128*3 +3 =129*3							 
			if( Len >= Sendlen ) Len -= Sendlen;		//ʣ������
			else 
			{
				Len = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
				break;
			}	
			if( Len > Sendlen )
			{
				Len = 384;
				Temp =1;							
			}else Temp =0;
			CreatSubObjectListHead( COM_No, SendBuff ); 
			FillIntegerToBuff( SendBuff+7, Len );
			lenth = Len;
			Ptr = SendBuff+9;
			for(j=0;j<4;j++)
			{
				if(lenth>128) lenth_temp =128;
				else lenth_temp = lenth;	
				E2P_RData( Ptr, Addr+3+j*129, lenth_temp );
				if((lenth-lenth_temp)==0) break;
				lenth -= 128;
				Ptr +=128;
			}																				
			Len += 9;
			//if( Temp == 0 ) *SendBuff = 0x01;								//last-block=1��ʾ�����һ֡���ݣ�����û�����ݿ顣
			if( Temp == 0 )
			{
				 *SendBuff = 0x01;								//last-block=1��ʾ�����һ֡���ݣ�����û�����ݿ顣
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
			break;
		default:
			Len = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
			break;		
	}	
	return Len;
}		

/*
SysT = "4641526570BC614E ";Ϊ�豸��systemtitle.�߼��豸����FAR1010712345678����Ӧ��systemtitleΪ4641526570BC614E��

=======================================================
�߼��豸����FAR 101 07 12345678��
��Ӧ��systemtitleΪ464152 65 7 0 BC614E��  (101 =0x65)   (12345678 = 0xBC614E);

*/
void 	LogToSytemtile( void )  //���߼��豸������õ�systemtitle..
{
	unsigned char Buff[16];
	unsigned char *Ptr;
	unsigned char *sysptr;
	unsigned long  numberHEX;
	
	Ptr = Buff;
	sysptr = ICComm.SystemTitle;
	
	E2P_RData( Ptr, Logical_device_name, 16 );			//��ȡ�߼��豸��..
	
	//0--2
	*sysptr = *Ptr;
	*(sysptr+1) = *(Ptr+1);
	*(sysptr+2) = *(Ptr+2);
	
	//����ȫ�������ֽ��д���..
	//3
	*(sysptr+3) =(unsigned char) ((*(Ptr+3)-0x30)*100+(*(Ptr+4)-0x30)*10+(*(Ptr+5)-0x30));
	//4
	*(sysptr+4) =(unsigned char) ((*(Ptr+6)-0x30)+((*(Ptr+7)-0x30)<<4));
	//5-6-7-8		
	numberHEX = ((unsigned long)(*(Ptr+8)-0x30))*10000000 + ((unsigned long)(*(Ptr+9)-0x30))*1000000 + ((unsigned long)(*(Ptr+10)-0x30))*100000 + ((unsigned long)(*(Ptr+11)-0x30))*10000 + (*(Ptr+12)-0x30)*1000 + (*(Ptr+13)-0x30)*100 +(*(Ptr+14)-0x30)*10 +(*(Ptr+15)-0x30);
	
	*(sysptr+5) =(numberHEX>>16)&0x00ff;
	*(sysptr+6) =(numberHEX>>8)&0x00ff;;
	*(sysptr+7) =(numberHEX)&0x00ff;;

	//�Աȷ�������SYSTEM TITLE 
		E2P_RData( Ptr, E2P_server_system_title, 8 );			//��ȡ��������SYSTEM TITLE ..
   if( Data_Comp( Ptr, sysptr, 8 ) != 0 )
   {
   	   E2P_WData( E2P_server_system_title, sysptr,8 );
   } 
}
