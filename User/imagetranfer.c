#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Measure.h"

#include "COSEMComm.h"
#include "ProfileGeneric.h"
#include "Imagetranfer.h"

//ȡimage_transfer������..
/*
	״̬��(Image_Transferred_Status)
	0 = transfer not initiated (not active; default state)
	1 = transfer initiated
	2 = verification initiated
	3 = verification successful
	4 = verification failed
	5 = activation initiated
	6 = activation successful
	7 = activation failed
	��־:(Image_Tranfer_Flag)
	�޸�״̬��־:
	Image_Tranfer_Flag=0x06; ----��Ҫִ��У�����������Ƿ�����..
	Image_Tranfer_Flag=0x70; ----��Ҫִ�м�����������..	
	
	//Image_Tranfer_Flag ��־λ
	#define IMAGE_FLS4K_NEED    		 (0x01)		//FLASH(4K)��Ҫ�����ȶ�����д�����,Ϊ����ACTION2ֱ��д��..
	#define ACTION1_INTI_FLAG        (0x02)		//ACTION1ִ�б�־..
	#define ACTION2_TRAN_FLAG        (0x04)		//ACTION2ִ�б�־..
	#define ACTION3_VERIFY_FLAG      (0x08)		//ACTION3ִ�б�־..
	#define ACTION4_ACTIVATE_FLAG    (0x10)		//ACTION1ִ�в���ͨ����־..
	#define ACTIVATE_TIME_FLAG       (0x20)		//ʱ�䵽�����־..
	#define FLAG_BIT6                (0x40)
	#define FLAG_BIT7                (0x80)
	//	== 0; //��־����..//��ʼ��..
	
*/
short GetImage_transferClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned short Len=2;
	unsigned char Attribute;
	unsigned long long_temp;///Long_Addr;
	unsigned short i,j;
	
	Ptr = Buff;
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
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
		if((Attribute!=0x11)&&(Attribute!=0x12)){
				if(( Attribute>7 )||(Attribute<=0)) return Len;
				*(Dest+1) = Image_transfer_ParaTab[Attribute-1].DataType;
	  }
	  
		switch( Attribute )
		{
			case 1:														//logical_name
				//*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				//*(Dest+1) = D_Unsigned32;								//image_block_size			D_Unsigned32
				long_temp = IMAGE_BLOCK_SIZE;							//����ɽ��ܵ����ݿ鳤�ȣ�����̶�..ImageBlockSize shall not exceed the 
																		//ServerMaxReceivePduSize negotiated.
				FillLongToBuff( Dest+2, long_temp );
				Len = 6;
				break;	
			case 3:
				//*(Dest+1) = Data_bit_string;									//image_transferred_block_status				Data_bit_string
				*(Dest+2) = 0x82;
//				*(Dest+3) = 0x04;				//����λ�����㳤��..
//				*(Dest+4) = 0x00;
//				for(i=0;i<128;i++)
//				{				
//					E2P_RData( Dest+5+i, IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, 1 );
//				}								
//				Len = 133;
				*(Dest+3) = 0x03;				//����λ�����㳤��.. //TEST SKY2014
				*(Dest+4) = 0x98;
				for(i=0;i<128;i++)
				{				
					E2P_RData( Dest+5+i, IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, 1 );
				}								
//				Len = 120;
			  Len = 133;
				break;	
			case 4:
				//*(Dest+1) = D_Unsigned32;								//image_firstnot_transferred_block_number			D_Unsigned32
				//ÿ����Ҫ�ڳ����м����д��EEPROM..
				//E2P_RData( Dest+2, IMAGE_FIRSTNOT_TRANSFERRED_BLOCK_NUMBER, 4 );

//				E2P_RData( Dest+5, IMAGE_TRANSFERRED_BLOCK_STATUS, 128 );
//				Ptr = Dest+5;
//				long_temp = 0xEEEE;				
//				for(i=0;i<128;i++)
//				{
//					for(j=0;j<8;j++)
//					{
//						if((*Ptr)&(0x80>>j)){
//							long_temp = i*8 + j;
//							break;
//						}						
//					}
//					if(long_temp != 0xEEEE) break;					
//					Ptr++;
//				}

				Ptr = Dest+5;
				long_temp = 0xEEEE;				
				for(i=0;i<128;i++)
				{
					E2P_RData( Dest+5+i, IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, 1  );
					Ptr = Dest+5+i;					
					for(j=0;j<8;j++)
					{
						if(!((*Ptr)&(0x80>>j))){
							long_temp = i*8 + j;
							break;
						}						
					}
					if(long_temp != 0xEEEE) break;					
					Ptr++;
				}

/*				if(long_temp == 0xEEEE){
						E2P_RData( Dest+125, IMAGE_TRANSFERRED_BLOCK_STATUS+120, 120 );														
						Ptr = Dest+125;
						long_temp = 0xEEEE;				
						for(i=120;i<240;i++)
						{
							for(j=0;j<8;j++)
							{
									if((*Ptr)&(0x80>>j)){
									long_temp = i*8 + j;
									break;
								}
							}
							if(long_temp != 0xEEEE) break;
							Ptr++;
						}
						if(long_temp == 0xEEEE){				
							E2P_RData( Dest+245, IMAGE_TRANSFERRED_BLOCK_STATUS+240, 79 );
							Ptr = Dest+245;
							long_temp = 0xEEEE;				
							for(i=240;i<319;i++)
							{
								for(j=0;j<8;j++)
								{
									if((*Ptr)&(0x80>>j)){
										long_temp = i*8 + j;
										break;
									}
								}
								if(long_temp != 0xEEEE) break;
								Ptr++;
							}											
						}
				}
*/				
//				if(long_temp != 0xEEEE)
//				{                                  
//				   long_temp++;								//��ʼ���ݿ��1��ʼ����..
//         }
//�޸����ݿ��0��ʼ����..
				FillLongToBuff( Dest+2, long_temp );
				Len = 6;
				break;	
			case 5:
				//*(Dest+1) = Data_boolean;								//image_transferred_enabled boolean
				E2P_RData( Dest+2, IMAGE_TRANSFERRED_ENABLED, 1 );
				if(*(Dest+2)!=0) *(Dest+2) = 1;
				Len = 3;
				break;	
			case 6:
				//*(Dest+1) = D_Unsigned16;								//image_transferred_block_status		enum
				//����״̬..
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );
				*(Dest+2) = Flag.Image_Transferred_Status;				
				Len = 3;
				break;
			case 7:
				//*(Dest+1) = Data_array;								//image_to_activate_info  array ---time	4+5			
				*(Dest+2) = 0x01;		//1���ṹ��					//1�����򼤻�..
				*(Dest+3) = 0x02;		//�ṹ��..
				*(Dest+4) = 0x03;		//3��Ԫ��..
				*(Dest+5) = D_Unsigned32;	
				
				// E2P_RData( (unsigned char*)&long_temp, IMAGE_TO_ACTIVATE_INFO, 4 );
				// long_temp = (unsigned long)252*1024;  //��ʱ����sky2013..
				
				E2P_RData( Dest+6, IMMAGE_TRANSFER_SIZE, 4 );				

				*(Dest+10) = D_OctetStr;
				//*(Dest+11) = 0x10;				
				// E2P_RData( Dest+2, IMAGE_TO_ACTIVATE_INFO, 9 );
				//�޸İ汾����ʾ..				
				// RAM_Write( Dest+12, (unsigned char *)firmware_version1, 16 );
				*(Dest+11) = 0x06;	
				E2P_RData( Dest+12, IMMAGE_TRANSFER_IDENTIFIER, 6 );
											
				*(Dest+18) = D_OctetStr;
				*(Dest+19) = 0x0B;				
				E2P_RData( Dest+20, IMAGE_TO_ACTIVATE_INFO+20, 11 );
				Len = 31;
				break;
		/*	
			//����ʹ��..
			case 0x11:		//��01---���µĳ���
				if(*(Source+10)!=D_Unsigned32){
					// *Dest = *(Source+9);
					*(Dest+1) = Get_data_access_result;					//�쳣��־;
					Len = 2;		
					return 2;
				}
				long_temp = GetBuffToLong(Source+11);
				if(long_temp==0){
					// *Dest = *(Source+9);
					*(Dest+1) = Get_data_access_result;					//�쳣��־;
					Len = 2;		
					return 2;					
				}
				*(Dest+1) = D_OctetStr;
				
//				if(IMAGE_BLOCK_SIZE<=127){
//					 	*(Dest+2) =  IMAGE_BLOCK_SIZE;
//					 		Len = 3;
//							Ptr = Dest+3;
//				}
//				else{
					 	*(Dest+2) = 0x82;
					 	FillIntegerToBuff( Dest+3, IMAGE_BLOCK_SIZE );					 	
					 	Len = 5;
						Ptr = Dest+5;					
//				}
				
				//��ȡ����..
//					if(long_temp==0){	//ǰ128���ֽڶ�������..
//							Long_Addr	= UPGRADE_NEW_START;			
//							//Long_Addr	= (unsigned long)896*(FLASH_4K);
//					}else{
//							Long_Addr	= UPGRADE_START_PROGRAM;					
//							//Long_Addr	= (unsigned long)896*(FLASH_4K)+FLASH_4K;
//							Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;
//					}

					
					Long_Addr	= UPGRADE_NEW_START;			
//					Long_Addr	+= long_temp*IMAGE_BLOCK_SIZE;		//Block_number���㿪ʼ..			
					Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;		//Block_number��1��ʼ..			
					
					Read_Flash(  Ptr,Long_Addr, IMAGE_BLOCK_SIZE );
					
					Len += IMAGE_BLOCK_SIZE;
				break;	
			//����ʹ��..
			case 0x12:		//02--CPU���еĳ���..
				if(*(Source+10)!=D_Unsigned32){
					// *Dest = *(Source+9);
					*(Dest+1) = Get_data_access_result;					//�쳣��־;
					return 2;
				}
				long_temp = GetBuffToLong(Source+11);
				*(Dest+1) = D_OctetStr;
				
//				if(IMAGE_BLOCK_SIZE<=127){
//					 	*(Dest+2) =  IMAGE_BLOCK_SIZE;
//					 		Len = 3;
//							Ptr = Dest+3;
//				}
//				else{
					 	*(Dest+2) = 0x82;
					 	FillIntegerToBuff( Dest+3, IMAGE_BLOCK_SIZE );					 	
					 	Len = 5;
						Ptr = Dest+5;					
//				}
//								if(long_temp==0){		//ǰ128���ֽڶ�������..
//										Long_Addr	= CURRENT_NEW_START;	
//								}else{
//										Long_Addr	= CURRENT_START_PROGRAM;
//										Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;
//								}
				 Long_Addr	= CURRENT_NEW_START;			
//				 Long_Addr	+= long_temp*IMAGE_BLOCK_SIZE;		//Block_number���㿪ʼ..																
				 Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;		//Block_number��1��ʼ..			
				 
				 Read_Flash(  Ptr,Long_Addr, IMAGE_BLOCK_SIZE );
							
				 Len += IMAGE_BLOCK_SIZE;		
				break;	
				*/
			default: break;			
		}
	}		
	return Len;
}	

//���ã�ֻ������5�ǿ�����������ʹ��״̬��Ҫ���õģ����඼��ֻ��..
unsigned char SetImage_transferClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;	
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
//	unsigned char AccessMode;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//�޶�Ӧ��OBIS��
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 5:														//���������Ƿ���������ʹ��boolen..					
				if( *DataType != Data_boolean ) { ErrorByte = type_unmatched; break; }
				// DataLen -= 1;				
				if( DataLen != 2 ) { return type_unmatched; }					//���ȴ�	
				E2P_WData( IMAGE_TRANSFERRED_ENABLED, WriteBufAds+1, 1 );
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}	

#if 0
//Զ�������ķ����������,<��������ڼ䲻�ܶϵ磬���򲻽��������������߿��ܲ�������>.. ACTION..
//=====================================================================================================
unsigned char ActionImage_transferClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[320];
	unsigned char* Ptr;
	unsigned char* read_Ptr;
	unsigned char ErrorByte=0;	
	unsigned char* Logical_Name;
	unsigned char Method,status,BIT_status;
	unsigned short  BIT_status_byte;
  unsigned long Block_number,long_temp,Long_Addr;
  		      short WriteErrorByte=0;
	unsigned  int Lenth;  					
	unsigned long Crc32_Temp,Crc32_Data;
	unsigned  int i;
	unsigned char FW_BUFF[16];
	unsigned char *FW;
		
	Ptr = Buff;
  FW  = FW_BUFF;
  
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		return object_undefined; 															//�޶�Ӧ��OBIS��
	}
		
	//	�ж�״̬ strategyimage_transfer_status =(1) image transfer initiatedimage				
	E2P_RData( Ptr, IMAGE_TRANSFERRED_ENABLED, 1 );					//��������ɱ�־..
	if(*Ptr == 0) 	return temporary_failure;								//����δʹ�ܣ���ִ��ACTION..
	
	Ptr = Source+9;
	if(*Ptr==0x01)
        {
          Ptr = Source+10;
          DataLen--; 
        }
         
	switch( Method )
		{
			case 1:			// ���ܽ��ж�γ�ʼ��.. //2013.6.20�޸� ����һ�����ٽ��г�ʼ������..
				//ACTION 1:ACTION image_transfer_init(image_identifier, image_size)..
				//				data ::= structure
				//{
				//image_identifier: octet-string,
				//image_size: double-long-unsigned
				//}			
				//includes data in format
				//{ 
				//	image_identifier
				//	image_size
				//}
				if( DataLen != 15 ) { return type_unmatched; }					//���ȴ�	
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 0x06 )||( *(Ptr+10) != D_Unsigned32 )) return other_reason;
				//if((*(Ptr+4)!=0x61)||(*(Ptr+5)!=0x01))	return other_reason; 		//�ж�image_identifier 0110 0001 00000001				
        //if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason; 		//�ж�image_identifier 0110 0001 00000001
        
        //20130626��..
				/*
				FGI:Firmware (Image) Group Identification
				000 Not-used 
				001 PRIME parts 
				010 Communication Module (Non-PRIME) 
				011 Meter parts(single) 
				100 Data Concentrator 
				//101 �C 111
				 Reserved for Future Use 
				101 3P Direct connection meter 
				110 3P CT meter 
				111 3P CT/PT meter
				*/
				if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason;       //Single meter 
			///	#if( METERTYPE	 == U230_I10A__100A300 )
			///	    if((*(Ptr+8)&0xE0)!=0xA0)	 return other_reason; 		//�ж�image_identifier 101 3P Direct connection meter 
			///	#endif				
			///	#if( METERTYPE	 == U230_I2_5A__10A3200 )
			///	    if((*(Ptr+8)&0xE0)!=0xC0)	 return other_reason; 		//�ж�image_identifier 110 3P CT meter
			///	#endif				    
			///	#if( METERTYPE	 == U57_I2_5A__10A12000 )
			///	    if((*(Ptr+8)&0xE0)!=0xE0)	 return other_reason; 		//�ж�image_identifier 111 3P CT/PT meter
			///	#endif        

				long_temp = GetBuffToLong(Ptr+11);					
				if((long_temp>(FLASH_184K + 128))||(long_temp<0xa3ff)) 	return other_reason; 		//����̫�̻򳬳�..
				
				//����ͨ��,�ж��Ƿ��Ѿ����й���ʼ������..
				read_Ptr = Buff;
				E2P_RData( read_Ptr, IMMAGE_TRANSFER_IDENTIFIER, 6 );			
				E2P_RData( read_Ptr+6, IMMAGE_TRANSFER_SIZE, 4 );			
				if(( Data_Comp( read_Ptr, Ptr+4, 6 ) == 0 )&&(Data_Comp( read_Ptr+6, Ptr+11, 4 ) == 0 )){
					//��ʼ����ɱ�־..
		                         Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				        //*Ptr = TRANSFER_INITIATED;
				          E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
					 return 0;
				}
					
				//����ͨ��������ʼ��..
				Flag.Image_Tranfer_Flag = 0; 			//��־����..
        		Flag.Image_Transferred_Status = 0;  		//
        		//д���µİ汾�ż�Size
				E2P_WData( IMMAGE_TRANSFER_IDENTIFIER, Ptr+4, 6 );				
				E2P_WData( IMMAGE_TRANSFER_SIZE, Ptr+11, 4 );		//�ļ��ܵĴ����ֽ���..							
				

				//�߳����������ŵ�FLASH..
				for(long_temp=UPGRADE_NEW_START;long_temp<UPGRADE_END_PROGRAM;long_temp=long_temp+FLASH_4K)
				{
						WriteErrorByte = BlockErase( long_temp );	
						if(WriteErrorByte!=0) return  WriteErrorByte;
				}
				
//				//128*8��״̬����1..
//				Ptr = Buff;
//				RAM_DataFill(Ptr,128,0xff);				
//				WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS, Ptr, 128 );
//				if(WriteErrorByte!=0) return  hardware_fault;
				
//				//128*8��״̬����1..
//				Ptr = Buff;
//				*Ptr = 0xff;
//				for(i=0;i<128;i++)
//				{				
//						WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, Ptr, 1 );
//						if(WriteErrorByte!=0) return  hardware_fault;				
//				}	
				//128*8��״̬����0..The image_transferred_blocks_status   attribute shall be reset
				Ptr = Buff;
				*Ptr = 0x00;
				for(i=0;i<128;i++)
				{				
						WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, Ptr, 1 );
						if(WriteErrorByte!=0) return  hardware_fault;				
				}								
													
				//��ʼ����ɱ�־..
        Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				//*Ptr = TRANSFER_INITIATED;
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
			
				break;
					
			case 2:		//image_block_transfer:ACTION 2 ---���ݴ������ݿ飬��Ҫ..
				//	data ::= structure
				//	{
				//		image_block_number: double-long-unsigned,
				//		image_block_value: octet-string
				//	}
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != D_Unsigned32 )||( *(Ptr+7) != Data_octet_string )) return other_reason;

				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//������״̬��־..	 					 
//			if((status ==0 )||(status >7))  return temporary_failure;				//δ��ʼ���򲻽����κβ���..
//			if((Flag.Image_Transferred_Status != TRANSFER_INITIATED )&&(Flag.Image_Transferred_Status != VERIFICATION_FAILED))  return temporary_failure;		//δ��ʼ�������У�飬У��ͨ���򲻽�������дFLASH����..
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >7))  return temporary_failure;				//δ��ʼ���򲻽����κβ���..
												
				//���ݿ鼰����..
				Block_number = GetBuffToLong(Ptr+3);
					
        long_temp = FLASH_184K + 128;
        long_temp = long_temp/IMAGE_BLOCK_SIZE + 1;                                       
                                        
				//if(Block_number>2017) 		return other_reason; 		//���ݿ鳬��(FLASH_252K + 128)/128=0..
//			if(Block_number>long_temp) 		return other_reason; 		//���ݿ鳬��(FLASH_252K + 128)/128=0..
				if(Block_number>long_temp) 		return other_reason; 		//���ݿ鳬��(FLASH_252K + 128)/128=0..

				//if(Block_number==0) 		return other_reason; 		//Block_number�޸Ĵ�1��ʼ����,��Ҫ����..���ݿ�Ϊ��,���˳�����..	
					
					Block_number ++;   //�޸İ��մ�0��ʼ����sky20160301..
										                                        
					if(( *(Ptr+8) & 0x80 ) != 0 )
					{
							if( *(Ptr+8) == 0x81 )										//�������ݳ����Ƿ�Ϊ1�ֽ� 
							{
								Lenth = *(Ptr+9);										//��Ч�������ݳ���
								//�����յ����ֽ��������ݿ��Ƿ�һ��..
								if( DataLen != Lenth + 10 ) { return type_unmatched; }					//���ȴ�
								Ptr = Ptr+10;											    //��Ч����������ʼ��ַ			
							}			
							else if( *(Ptr+8) != 0x82 ) return other_reason;
							else 
							{
								Lenth = GetBuffToInteger( Ptr+9 );
								//�����յ����ֽ��������ݿ��Ƿ�һ��..
								if( DataLen != Lenth + 11 ) { return type_unmatched; }					//���ȴ�								
								Ptr = Ptr+11;
							}	
					}	
					else
					{
								Lenth = *(Ptr+8);										//��Ч�������ݳ���
								//�����յ����ֽ��������ݿ��Ƿ�һ��..
								if( DataLen != Lenth + 9 ) { return type_unmatched; }					//���ȴ�								
								Ptr = Ptr+9;											    //��Ч����������ʼ��ַ			
					}	
					
					//sky20141218��������汾�ŵ��ж�..
					if(Block_number==1){
						FW = (unsigned char *)((unsigned char *)firmware_version0);
						if(Data_CompHL(FW,Ptr,10)!=0) return hardware_fault;
					}
						
					if(Block_number<=200){					//50K���ڿռ䣬�����ж�,�����������������һ֡�ж�..
						 if(Lenth!=IMAGE_BLOCK_SIZE)	return  temporary_failure;			//�ж�ÿ֡�����ݳ���..
						 Flag.Image_Tranfer_Flag = 0x02;
					}
					else
					{
	               	 if(Lenth!=IMAGE_BLOCK_SIZE)
					 	{
	               	 	  Flag.Image_Tranfer_Flag = 0x04;	 //�����һ֡���ж�..
	               	 	  Flag.Image_Transferred_Status = VERIFICATION_INITIATED;   //У���ʼ��..
	               	 	  E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
	               		}
   					}
					
//					//д����..						
//					if(Block_number==0){		//ǰ128���ֽڵ����ֿ�����..
//							Long_Addr	= UPGRADE_NEW_START;			
//							//Long_Addr	= (unsigned long)896*(FLASH_4K);
//					}else{
//							Long_Addr	= UPGRADE_START_PROGRAM;					
//							//Long_Addr	= (unsigned long)896*(FLASH_4K)+FLASH_4K;
//							Long_Addr	+= (Block_number-1)*IMAGE_BLOCK_SIZE;
//					}

//					//����ɹ�״̬λ��1..
//					BIT_status_byte =(unsigned short )(Block_number/8);
//					if(BIT_status_byte>319)  return  hardware_fault;
//					BIT_status = (unsigned char )(Block_number - (Block_number/8)*8);
//					BIT_status =	(0x80>>BIT_status);					
//					E2P_RData( &status, IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte, 1 );
//					if(status&BIT_status) return other_reason;			//�Ѿ�д�ɹ������ظ�д..
//					
//					Long_Addr	= UPGRADE_NEW_START;			
//					Long_Addr	+= Block_number*IMAGE_BLOCK_SIZE;			//Block_number���㿪ʼ..			
//					
//					WriteErrorByte = DataFlash_Write_NOT_Erase( Long_Addr, Ptr, Lenth );
//					if(WriteErrorByte!=0) return  hardware_fault;		
//
//					status |=	BIT_status;
//					E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte, &status, 1 );	

					//����ɹ�״̬λ��0..
					Block_number = Block_number -1;			//Block_number�޸Ĵ�1��ʼ����,��Ҫ����..
					
					BIT_status_byte =(unsigned short )(Block_number/8);
//					if(BIT_status_byte>319)  return  hardware_fault;
					if(BIT_status_byte>128)  return  hardware_fault;//�������䷶Χ��
					BIT_status = (unsigned char )(Block_number - (Block_number/8)*8);
					
					//BIT_status =	~(0x80>>BIT_status);				

					BIT_status =	(0x80>>BIT_status);			//2013.6.20..������һ�δ����״̬=1

					E2P_RData( &status, IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, 1 );
//					if((status&BIT_status)==0) return other_reason;			//�Ѿ�д�ɹ������ظ�д..
//					if((status&BIT_status)==0) return   ErrorByte;			//�Ѿ�д�ɹ������ظ�д(������ȷӦ��)..
					if((status&BIT_status)==1) return   ErrorByte;			////20160229 ��״̬0--δ����,1--�Ѵ���..	..��///���˵���Ѵ�����ÿ�,ֱ�ӷ���
						
					//BIT_status =	~BIT_status;
					//���δ洢���յ����ݿ�����		
					Long_Addr	= UPGRADE_NEW_START;			
					Long_Addr	+= Block_number*IMAGE_BLOCK_SIZE;			//Block_number���㿪ʼ..			
					
				///	WriteErrorByte = DataFlash_Write_NOT_Erase( Long_Addr, Ptr, Lenth );
					WriteErrorByte = Write_Flash( Long_Addr, Ptr, Lenth );//�������ݴӻ�����д��flash��
					if(WriteErrorByte!=0) return  hardware_fault;		

					//status &=	BIT_status;
					status |=	BIT_status;    //20160229 ��״̬0--δ����,1--�Ѵ���..	
					E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, &status, 1 );	          
				break;	
			case 3:			
				//image_verify:ACTION 3  Verifies the integrity of the Image before activation. data::= integer (0)
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//���ȴ�					
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//������״̬��־..	 			
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >5))  return temporary_failure;				//δ��ʼ���򲻽���У��..
					
				E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
				long_temp = GetBuffToLong(Ptr+1);
				if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;

				//ͷ��128���ֽڼ���CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
				
				//��ȡ�ļ�CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Read_FlashD( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp!=Crc32_Data) return temporary_failure;	

				//������¼�����ļ�CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
				
				//��ȡ�ļ�CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
				Read_FlashD( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	

				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp==Crc32_Data){
  				//У�麯��..
				  //*Ptr = VERIFICATION_SUCCESSFUL;				
				  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
				  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
					
//				*Ptr = 0xa5;
////			*(Ptr+1) = 0xa5;				
//				E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
				  Flag.Image_Tranfer_Flag = 0x26;	
				  
				  //����У��ɹ���¼..
					SM.StandardEventCode = Firmware_ready_for_activation;
					LoadRecord( Standard_Event_No );
					SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					//-------------				  					
				}else{
								Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
								E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );	
							  //����У��ʧ�ܼ�¼..
								SM.StandardEventCode = Firmware_verification_failed;
								LoadRecord( Standard_Event_No );
								SM.Event_all_flag |= Standard_Event_BIT;
								SM.Error_all_flag |= Standard_Event_BIT;									
					 			return temporary_failure;		
							}
				break;	
			case 4:			
				//image_activate:ACTION 4 Activates the Image(s). data::= integer (0) --ͬAction 3
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//���ȴ�..
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				Ptr = Buff;
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//��������ɱ�־..
				// if(Flag.Image_Transferred_Status >= 5 ) 	return other_reason;
				if((Flag.Image_Transferred_Status < 2 )||(Flag.Image_Transferred_Status >= 5 )) 	return other_reason;
				else{							
				// if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL)&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED)){
					if(*Ptr ==0) return other_reason;
						E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
						long_temp = GetBuffToLong(Ptr+1);
						if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;
		
						//ͷ��128���ֽڼ���CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
						
						//��ȡ�ļ�CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );	
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+124 ,4 );
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp!=Crc32_Data) return temporary_failure;	
		
						//������¼�����ļ�CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
						
						//��ȡ�ļ�CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp==Crc32_Data){
		  				  //У�麯��..
						  //*Ptr = VERIFICATION_SUCCESSFUL;				
						  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
						  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
							E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
							
						  *Ptr = 0xa5;
		//				  *(Ptr+1) = 0xa5;				
						  E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
						  Flag.Image_Tranfer_Flag = 0x26;					  

						}else{
										Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
										E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
							 			return temporary_failure;		
									}

				}	 					 
				//if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED))  return temporary_failure;				//δУ���򲻽��м������..
			if(Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )  return temporary_failure;				//δУ���򲻽��м������..				

				Flag.Image_Transferred_Status = ACTIVATION_INITIATED;	
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
				//�����..			  
			  *Ptr = 0xb5;
				*(Ptr+1) = 0xc9;				
				E2P_WData( IMMAGE_TRANSFER_ACTIVATE, Ptr, 2 );
				Flag.Image_Tranfer_Flag = 0x70;
				
			///	Disk.FullDisDelay = 0x06;			//��ʾ6sȫ��..
				
//					//��������ɹ���¼..
//					SM.StandardEventCode = Firmware_activated;
//					LoadRecord( Standard_Event_No );
//					SM.Event_all_flag |= Standard_Event_BIT;
					//-------------				  									  																
//				Activate_Image_Transfer(0);
//				
//				Delay(2000);
//				asm(" mov &0xFFFE, PC;");            									
				
				break;	
			default: 
				ErrorByte = other_reason;		
				break;
		}			
  return ErrorByte;
}
#endif
//Զ�������ķ����������,<��������ڼ䲻�ܶϵ磬���򲻽��������������߿��ܲ�������>.. ACTION..
//����ʹ�ã���ǰ128���ֽ���Ϣ�����ε���ֻ�����������������ļ������ε�CRCУ�顢��Ӳ���汾�ŵ���Ϣ-----2016-9-14
//=====================================================================================================
unsigned char ActionImage_transferClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[320];
	unsigned char* Ptr;
	unsigned char* read_Ptr;
	unsigned char ErrorByte=0;	
	unsigned char* Logical_Name;
	unsigned char Method,status,BIT_status;
	unsigned short  BIT_status_byte;
  unsigned long Block_number,long_temp,Long_Addr;
  		      short WriteErrorByte=0;
	unsigned short Lenth;  					
	unsigned long Crc32_Temp,Crc32_Data;
	unsigned short i;
	unsigned char FW_BUFF[16];
	unsigned char *FW;
		
	Ptr = Buff;
  FW  = FW_BUFF;
  
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		return object_undefined; 															//�޶�Ӧ��OBIS��
	}
		
	//	�ж�״̬ strategyimage_transfer_status =(1) image transfer initiatedimage				
	E2P_RData( Ptr, IMAGE_TRANSFERRED_ENABLED, 1 );					//��������ɱ�־..
	if(*Ptr == 0) 	return temporary_failure;								//����δʹ�ܣ���ִ��ACTION..
	
	Ptr = Source+9;
	if(*Ptr==0x01)
        {
          Ptr = Source+10;
          DataLen--; 
        }
         
	switch( Method )
		{
			case 1:			// ���ܽ��ж�γ�ʼ��.. //2013.6.20�޸� ����һ�����ٽ��г�ʼ������..
				//ACTION 1:ACTION image_transfer_init(image_identifier, image_size)..
				//				data ::= structure
				//{
				//image_identifier: octet-string,
				//image_size: double-long-unsigned
				//}			
				//includes data in format
				//{ 
				//	image_identifier
				//	image_size
				//}
				if( DataLen != 15 ) { return type_unmatched; }					//���ȴ�	
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 0x06 )||( *(Ptr+10) != D_Unsigned32 )) return other_reason;
				//if((*(Ptr+4)!=0x61)||(*(Ptr+5)!=0x01))	return other_reason; 		//�ж�image_identifier 0110 0001 00000001				
        //if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason; 		//�ж�image_identifier 0110 0001 00000001
        
        //20130626��..
				/*
				FGI:Firmware (Image) Group Identification
				000 Not-used 
				001 PRIME parts 
				010 Communication Module (Non-PRIME) 
				011 Meter parts(single) 
				100 Data Concentrator 
				//101 �C 111
				 Reserved for Future Use 
				101 3P Direct connection meter 
				110 3P CT meter 
				111 3P CT/PT meter
				*/
				#if( MeterPhase== SinglePhase)
					if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason;       //Single meter 
				#else
					#if( METERTYPE	 == U230_I10A__100A300 )
					    if((*(Ptr+8)&0xE0)!=0xA0)	 return other_reason; 		//�ж�image_identifier 101 3P Direct connection meter 
					#elif( METERTYPE	 == U230_I2_5A__10A3200 )				
					    if((*(Ptr+8)&0xE0)!=0xC0)	 return other_reason; 		//�ж�image_identifier 110 3P CT meter
					#elif( METERTYPE	 == U57_I2_5A__10A12000 )				    
					    if((*(Ptr+8)&0xE0)!=0xE0)	 return other_reason; 		//�ж�image_identifier 111 3P CT/PT meter
					#endif   
				#endif
				#if 0
				long_temp = GetBuffToLong(Ptr+11);					
				if((long_temp>(FLASH_184K + 128))||(long_temp<0xa3ff)) 	return other_reason; 		//����̫�̻򳬳�..
				
				//����ͨ��,�ж��Ƿ��Ѿ����й���ʼ������..
				read_Ptr = Buff;
				E2P_RData( read_Ptr, IMMAGE_TRANSFER_IDENTIFIER, 6 );			
				E2P_RData( read_Ptr+6, IMMAGE_TRANSFER_SIZE, 4 );			
				if(( Data_Comp( read_Ptr, Ptr+4, 6 ) == 0 )&&(Data_Comp( read_Ptr+6, Ptr+11, 4 ) == 0 )){
					//��ʼ����ɱ�־..
		                         Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				          E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
					 return 0;
				}
				#endif	
				//����ͨ��������ʼ��..
				Flag.Image_Tranfer_Flag = 0; 			//��־����..
        		Flag.Image_Transferred_Status = 0;  		//
				
				//д���µİ汾�ż�Size
				E2P_WData( IMMAGE_TRANSFER_IDENTIFIER, Ptr+4, 6 );				
				E2P_WData( IMMAGE_TRANSFER_SIZE, Ptr+11, 4 );		//�ļ��ܵĴ����ֽ���..							
				

				//�߳����������ŵ�FLASH..
				for(long_temp=UPGRADE_NEW_START;long_temp<UPGRADE_END_PROGRAM;long_temp=long_temp+FLASH_4K)
				{
						WriteErrorByte = BlockErase( long_temp );	
						if(WriteErrorByte!=0) return  WriteErrorByte;
				}
				//128*8��״̬����0..The image_transferred_blocks_status   attribute shall be reset
				Ptr = Buff;
				*Ptr = 0x00;
				for(i=0;i<128;i++)
				{				
						WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, Ptr, 1 );
						if(WriteErrorByte!=0) return  hardware_fault;				
				}								
													
				//��ʼ����ɱ�־..
        Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				//*Ptr = TRANSFER_INITIATED;
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
			
				break;
					
			case 2:		//image_block_transfer:ACTION 2 ---���ݴ������ݿ飬��Ҫ..
				//	data ::= structure
				//	{
				//		image_block_number: double-long-unsigned,
				//		image_block_value: octet-string
				//	}
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != D_Unsigned32 )||( *(Ptr+7) != Data_octet_string )) return other_reason;

				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//������״̬��־..	 					 
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >7))  return temporary_failure;				//δ��ʼ���򲻽����κβ���..
												
				//���ݿ鼰����..
				Block_number = GetBuffToLong(Ptr+3);
					
       			long_temp = FLASH_184K + 128;
        		///long_temp = long_temp/IMAGE_BLOCK_SIZE + 1;                                       
                long_temp = long_temp/IMAGE_BLOCK_SIZE;                          
				if(Block_number>long_temp) 		return other_reason; 		//���ݿ鳬��(FLASH_252K + 128)/128=0..
					
					Block_number ++;   //�޸İ��մ�0��ʼ����sky20160301..
										                                        
					if(( *(Ptr+8) & 0x80 ) != 0 )
					{
							if( *(Ptr+8) == 0x81 )										//�������ݳ����Ƿ�Ϊ1�ֽ� 
							{
								Lenth = *(Ptr+9);										//��Ч�������ݳ���
								//�����յ����ֽ��������ݿ��Ƿ�һ��..
								if( DataLen != Lenth + 10 ) { return type_unmatched; }					//���ȴ�
								Ptr = Ptr+10;											    //��Ч����������ʼ��ַ			
							}			
							else if( *(Ptr+8) != 0x82 ) return other_reason;
							else 
							{	//0x82
								Lenth = GetBuffToInteger( Ptr+9 );
								//�����յ����ֽ��������ݿ��Ƿ�һ��..
								if( DataLen != Lenth + 11 ) { return type_unmatched; }					//���ȴ�								
								Ptr = Ptr+11;
							}	
					}	
					else
					{
								Lenth = *(Ptr+8);										//��Ч�������ݳ���
								//�����յ����ֽ��������ݿ��Ƿ�һ��..
								if( DataLen != Lenth + 9 ) { return type_unmatched; }					//���ȴ�								
								Ptr = Ptr+9;											    //��Ч����������ʼ��ַ			
					}	
					
					//sky20141218��������汾�ŵ��ж�..
					if(Block_number==1){
						//E2P_RData(FW,Device_ID4,16);
						FW = (unsigned char *)((unsigned char *)firmware_version0);
						if(Data_CompHL(FW,Ptr,16)!=0) return hardware_fault;
					}
						
					if(Block_number<=200){					//50K���ڿռ䣬�����ж�,�����������������һ֡�ж�..
						 if(Lenth!=IMAGE_BLOCK_SIZE)	return  temporary_failure;			//�ж�ÿ֡�����ݳ���..
						 Flag.Image_Tranfer_Flag = 0x02;
					}
					else
					{
	               	 if(Lenth!=IMAGE_BLOCK_SIZE)
					 	{
	               	 	  Flag.Image_Tranfer_Flag = 0x04;	 //�����һ֡���ж�..
	               	 	  Flag.Image_Transferred_Status = VERIFICATION_INITIATED;   //У���ʼ��..
	               	 	  E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
	               		}
   					}
					
					//����ɹ�״̬λ��0..
					Block_number = Block_number -1;			//Block_number�޸Ĵ�1��ʼ����,��Ҫ����..
					
					BIT_status_byte =(unsigned short )(Block_number/8);
					if(BIT_status_byte>128)  return  hardware_fault;//�������䷶Χ��
					BIT_status = (unsigned char )(Block_number - (Block_number/8)*8);
					
					BIT_status =	(0x80>>BIT_status);			//2013.6.20..������һ�δ����״̬=1

					E2P_RData( &status, IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, 1 );
					if((status&BIT_status)==1) return   ErrorByte;			////20160229 ��״̬0--δ����,1--�Ѵ���..	..��///���˵���Ѵ�����ÿ�,ֱ�ӷ���
						
					//���δ洢���յ����ݿ�����		
					Long_Addr	= UPGRADE_NEW_START;	///	UPGRADE_START_PROGRAM;	
					Long_Addr	+= Block_number*IMAGE_BLOCK_SIZE;			//Block_number���㿪ʼ..			
					
					WriteErrorByte = Write_Flash( Long_Addr, Ptr, Lenth );//�������ݴӻ�����д��flash��
					if(WriteErrorByte!=0) return  hardware_fault;		

					status |=	BIT_status;    //20160229 ��״̬0--δ����,1--�Ѵ���..	
					E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, &status, 1 );	          
				break;	
			case 3:			
				//image_verify:ACTION 3  Verifies the integrity of the Image before activation. data::= integer (0)
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//���ȴ�					
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//������״̬��־..	 			
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >5))  return temporary_failure;				//δ��ʼ���򲻽���У��..
				E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
				long_temp = GetBuffToLong(Ptr+1);
				if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;

				//ͷ��128���ֽڼ���CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
				
				//��ȡ�ļ�CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp!=Crc32_Data) return temporary_failure;	

				//������¼�����ļ�CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
				
				//��ȡ�ļ�CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
				Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	

				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp==Crc32_Data){
  				//У�麯��..
				  //*Ptr = VERIFICATION_SUCCESSFUL;				
				  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
				  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
					
//				*Ptr = 0xa5;
////			*(Ptr+1) = 0xa5;				
//				E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
				  Flag.Image_Tranfer_Flag = 0x26;	
				  
				  //����У��ɹ���¼..
					SM.StandardEventCode = Firmware_ready_for_activation;
					LoadRecord( Standard_Event_No );
					SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					//-------------				  					
				}else{
								Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
								E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );	
							  //����У��ʧ�ܼ�¼..
								SM.StandardEventCode = Firmware_verification_failed;
								LoadRecord( Standard_Event_No );
								SM.Event_all_flag |= Standard_Event_BIT;
								SM.Error_all_flag |= Standard_Event_BIT;									
					 			return temporary_failure;		
							}

					//	  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
					//		E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
					//	  Flag.Image_Tranfer_Flag = 0x26;	
						  
						  //����У��ɹ���¼..
						//	SM.StandardEventCode = Firmware_ready_for_activation;
						//	LoadRecord( Standard_Event_No );
						//	SM.Event_all_flag |= Standard_Event_BIT;
						//	SM.Error_all_flag |= Standard_Event_BIT;
				break;	
			case 4:			
				//image_activate:ACTION 4 Activates the Image(s). data::= integer (0) --ͬAction 3
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//���ȴ�..
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				Ptr = Buff;
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//��������ɱ�־..
		#if 0
				// if(Flag.Image_Transferred_Status >= 5 ) 	return other_reason;
				if((Flag.Image_Transferred_Status < 2 )||(Flag.Image_Transferred_Status >= 5 )) 	return other_reason;
				else{							
				// if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL)&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED)){
					if(*Ptr ==0) return other_reason;
						E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
						long_temp = GetBuffToLong(Ptr+1);
						if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;
		
						//ͷ��128���ֽڼ���CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
						
						//��ȡ�ļ�CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );	
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+124 ,4 );
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp!=Crc32_Data) return temporary_failure;	
		
						//������¼�����ļ�CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
						
						//��ȡ�ļ�CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp==Crc32_Data){
		  				  //У�麯��..
						  //*Ptr = VERIFICATION_SUCCESSFUL;				
						  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
						  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
							E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
							
						  *Ptr = 0xa5;
		//				  *(Ptr+1) = 0xa5;				
						  E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
						  Flag.Image_Tranfer_Flag = 0x26;					  

						}else{
										Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
										E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
							 			return temporary_failure;		
									}

				}
		#endif
				//if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED))  return temporary_failure;				//δУ���򲻽��м������..
			if(Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )  return temporary_failure;				//δУ���򲻽��м������..				

				Flag.Image_Transferred_Status = ACTIVATION_INITIATED;	
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
				//�����..			  
			  *Ptr = 0xb5;
				*(Ptr+1) = 0xc9;				
				E2P_WData( IMMAGE_TRANSFER_ACTIVATE, Ptr, 2 );
				Flag.Image_Tranfer_Flag = 0x70;
				
				Disk.FullDisDelay = 0x06;			//��ʾ6sȫ��..
				break;	
			default: 
				ErrorByte = other_reason;		
				break;
		}			
  return ErrorByte;
}

//����CRCУ��..
unsigned long Flash_GenerateCRC32(unsigned long Addr,unsigned long len )
{
	unsigned long lenth;
	unsigned char Buff[520];
	unsigned char *Ptr;
	unsigned short i;
	unsigned long Crc32_check=0;
	
	i = 0;
	while( len > 0)
	{
		Ptr = Buff;		
		lenth = len/512;
		if(lenth==0){
				lenth = len;
		}else{
				lenth = 512;
		}
	///	Read_Flash( Ptr,(unsigned long)Addr+(unsigned long)i*512, lenth );
		Read_Flash( Ptr,(unsigned long)Addr+(unsigned long)i*512, lenth );
		Crc32_check = GenerateCRC32(Crc32_check, Ptr,lenth); 
		i++;
		len -=lenth;		
	}
	return Crc32_check; 
}

// ����CRC32
// ��������CRC32����Ҫ�� oldcrc32 = 0 ��ֵ��������,���ȿ�ʼ��Ҫ����ʼֵ 
unsigned long GenerateCRC32(unsigned long oldcrc32, unsigned char * dataptr,unsigned short len) 
{ 
		// unsigned long oldcrc32; 
		unsigned long crc32; 
		unsigned long oldcrc; 
		// unsigned short charcnt; 
		unsigned char c,t; 
		// oldcrc32 = 0x00000000; //��ֵΪ0 
		while (len--) { 
				t= (oldcrc32 >> 24) & 0xFF; 		//Ҫ�Ƴ����ֽڵ�ֵ 
				oldcrc=crc_32_tab[t]; 					//�����Ƴ����ֽڵ�ֵ��� 		
				c = *dataptr; 									//���ƽ������ֽ�ֵ 
				oldcrc32= (oldcrc32 << 8) | c; 	//�����ƽ������ֽ�ֵ���ڼĴ���ĩ�ֽ��� 
				oldcrc32=oldcrc32^oldcrc; 			//���Ĵ���������ֵ����xor���� 
				dataptr++;
			} 
			crc32=oldcrc32; 
			return crc32; 
}

//�����..
unsigned char Activate_Image_Transfer(unsigned char Enter_Flag )
{
	//unsigned long lenth;
	unsigned char Buff[20];
	unsigned char *Ptr;
	unsigned char Error=0;
//	unsigned short Addr_data16;
//	unsigned long Addr_data20;
	//unsigned long Crc32_check=0;
  unsigned long long_temp;//long_ADDR;
	
	//����ǰȷ��CRC32�Ƿ�ͨ��..
	
	//���ݵ�ǰ�ĳ������������Ƿ���Ҫ����???��ʱ������..��..	
	//���߳����浱ǰ�����FLASH��ַ..(���ٳ�һ��flash��ŵ�ǰ����)
	for(long_temp=CURRENT_NEW_START;long_temp<CURRENT_END_PROGRAM;long_temp=long_temp+FLASH_4K)
	{
		Error = BlockErase( long_temp );	
		if(Error!=0) return  Error;
	}
/*        
 //��������(0x5c00 --- 0xFFFF)��FLASH..
 //0xF000 ---0xFFFF��������������..
 Ptr = Buff;
 long_ADDR = CURRENT_NEW_START;        
 //for(Addr_data16=0x5c00;Addr_data16<=0xEfff;(Addr_data16+512))
 for(Addr_data16=0x5c00;Addr_data16<=0xEfff;(Addr_data16=Addr_data16+512))
  {
	  cpu_fls_int_Read_Flash( Ptr, Addr_data16, 512 );
		            
	///  Error = DataFlash_Write_NOT_Erase( long_ADDR, Ptr, 512 );
	  Error =  Write_Flash( long_ADDR, Ptr, 512 );
	  if(Error!=0) return  Error;   //sky20140714..
	  long_ADDR +=512;           
  }
  
  long_ADDR = CURRENT_NEW_START + 0x9400;				//(0xefff-0x5c00)+1;
  Ptr = Buff;
 
 ///Cortex-M0 Flash��MSP4305438��ͬ��û����Ϣ�οռ䣬������---fire
  //�洢data20����..(0x10000----0x45bff)
  for(Addr_data20=0x10000;Addr_data20<=0x45bff;(Addr_data20=Addr_data20+512))
  {
	  cpu_fls_data20_Read_Flash( Ptr, Addr_data20, 512 );
		            
	///  Error = DataFlash_Write_NOT_Erase( long_ADDR, Ptr, 512 );
	   Error =  Write_Flash( long_ADDR, Ptr, 512 );
	  if(Error!=0) return  Error;   //sky20140714..
	  long_ADDR +=512;           
  }  
  */
  
  //���㱸�ݵ�ǰ�����У���..
	//У�����Դ�..
	//����������ָʾ��β�����
	// 0xa1a5---����A--�³���
	// 0xb3b7--��������B---�ϳ���
	// 0xc1c3XX---����XX--��ʾʧ�ܴ���..
	//дCPU-INFO-FLASH C..
	
	//д��Ϣ������INFOC
///  cpu_Fls_int_SegClr( 0x55,(unsigned char *)CPU_INFOC  );//fire
///	*Ptr = 0xa1;
///	*(Ptr+1) = 0xa5;
///	*(Ptr+2) = 0xff;
	Ptr = Buff;
	E2P_RData( Ptr+3,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
	long_temp = GetBuffToLong(Ptr+3);
	if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;
//	E2P_RData( Ptr+7,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
	
//	if(Data_Comp(Ptr+3,Ptr+7,4)!=0) 	return other_reason;
		
//	*Ptr = 0xb3;
//	*(Ptr+1) = 0xb7;
//	*(Ptr+2) = 0;	
	
///	 cpu_Fls_int_Write(0x0C, CPU_INFOC, Ptr, 12 );//fire
	 
////��������ɹ���¼..
//	SM.StandardEventCode = Firmware_activated;
//	LoadRecord( Standard_Event_No );
//	SM.Event_all_flag |= Standard_Event_BIT;  
	
	 Flag.Image_Transferred_Status = 5;	
	 E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		

//�汾�Ž����л�..
 	Ptr = Buff;
 	E2P_RData( Ptr,IMMAGE_TRANSFER_IDENTIFIER, 6 );		//���������İ汾��..
 	
 	E2P_WData( Device_ID4, Ptr, 6 ); 	
 	*Ptr = 0;
 	*(Ptr+1) = 0;			
    *(Ptr+2) = 0;		
 	*(Ptr+3) = 0;	
 	*(Ptr+4) = 0;			
 	*(Ptr+5) = 0;	
 	*(Ptr+6) = 0;
 	*(Ptr+7) = 0;	
 	E2P_WData( ImageID7_1, Ptr, 7 );	
	Ptr = (unsigned char *)((unsigned char *)firmware_version0);	
 	E2P_WData( ImageID7_2, Ptr, 16 );
// 
//	//  test
//	cpu_fls_int_Read_Flash( Ptr+10, CPU_INFOC, 3 );		
	
///	cpu_Fls_int_SegClr( 0x55,(unsigned char *)0xee00  );//fire
	
	SM.Init_Flag = 0;			//��ʼ��..  
	return Error; 
}

//У�鲢�����¼�:
//�����..
unsigned char Verify_Image_Transfer( unsigned char Enter_Flag )
{
 	unsigned char  Buff[150];
	unsigned char* Ptr;
//	unsigned char* read_Ptr;
//	unsigned char ErrorByte=0;	
//	unsigned char* Logical_Name;
//	unsigned char Method,status,BIT_status;
//	unsigned short  BIT_status_byte;
//  unsigned long Block_number,long_temp,Long_Addr;
  unsigned long long_temp;        
//  		      short WriteErrorByte=0;
//	unsigned  int Lenth;  					
	unsigned long Crc32_Temp,Crc32_Data;
//	unsigned  int i;
//
	if(Flag.Image_Tranfer_Flag>= 0x04)
	{
		
	Ptr = Buff;
	E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//��������ɱ�־..
	if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL)&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED))
	{
			if(*Ptr ==0) return other_reason;
			E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//�ļ��ܵĴ����ֽ���..
			long_temp = GetBuffToLong(Ptr+1);
			if((long_temp<65536)||(long_temp>(FLASH_252K + 128)))		 	return other_reason;
				
			//ͷ��128���ֽڼ���CRC32..
			Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
								
			//��ȡ�ļ�CRC32..
		///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );	
			Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
			Crc32_Data = GetBuffToLong(Ptr+5);
			if(Crc32_Temp!=Crc32_Data){
					Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
					return temporary_failure;		
			}
				
			//������¼�����ļ�CRC32..
			Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
								
			//��ȡ�ļ�CRC32..
		///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
			Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );
			Crc32_Data = GetBuffToLong(Ptr+5);
			if(Crc32_Temp==Crc32_Data){
				//У�麯��..
				//*Ptr = VERIFICATION_SUCCESSFUL;				
				//E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
				Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
									
				*Ptr = 0xa5;
				//				  *(Ptr+1) = 0xa5;				
				E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
				Flag.Image_Tranfer_Flag = 0x26;					  
		
		  }else{
					Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
		 			return temporary_failure;		
				}
  	}
	}
	return 0x55;      
}
#if 0

//��ʼ����ȫʱ���..
void Image_tranfer_init(void)
{
	unsigned char INFO_buff[20];
	unsigned char *INFO_buff_Ptr;

	//����������ָʾ��β�����
	// 0xa1a5---����A--�³���
	// 0xb3b7--��������B---�ϳ���
	// XXXXXXXX --�ĸ��ֽ�������..
	// 0xc1c3XX---����XX--��ʾʧ�ܴ���..
	//дCPU-INFO-FLASH C..

	INFO_buff_Ptr = INFO_buff;
	cpu_fls_int_Read_Flash( INFO_buff_Ptr, CPU_INFOC, 12 );  
  if((*(INFO_buff_Ptr)==0xa1)&&(*(INFO_buff_Ptr+1)==0xa5)&&(*(INFO_buff_Ptr+2)==0x55)) 	Flag.New_Test_Flag = 0x57;
  else 	Flag.New_Test_Flag = 0;
	
	// �Ƿ���Ҫ�жϰ�ȫʱ��..
	Flag.New_Image_safetime = 99999999;
	Flag.New_check_ok = 0;	
	//safe time
	if(Flag.New_Test_Flag==0x57) 	E2P_RData( (unsigned char *)&Flag.New_Image_safetime, New_firmware_safety_time, 4 );
	if(Flag.New_Image_safetime ==0)  Flag.New_check_ok = 0x58;	   //ȷ�ϲ����³���..		
}

// �Ȱ�ȫʱ����˺��ж��Ƿ�ص��ϵĳ�������³���..
void Image_tranfer_NewOrOld(void)
{
	unsigned char INFO_buff[20];
	unsigned char *INFO_buff_Ptr;
	unsigned char ImageBuff[7];
	unsigned char *Ptr;	
	
	if(Flag.New_Test_Flag != 0x57) return;
  if(Flag.New_Image_safetime>0) Flag.New_Image_safetime--;

  //������ʱ�������˹���..(����Ҫ�л�ԭ������)..
   Flag.New_check_ok = 0x58;
   Flag.New_Image_safetime = 0xFFFFFFFF;
   // =============2016-01-14 13:22:12..
   
//  if(Flag.New_check_ok != 0x58)	   //ȷ���Ƿ�ִ���³���..
//  {
//  	//��ȡEEPROM��ID7_CONFIG���ж�..
//  }	  
  if(Flag.New_check_ok == 0x58)	   //ִ���³���..
  {
	  	INFO_buff_Ptr = INFO_buff;
			cpu_fls_int_Read_Flash( INFO_buff_Ptr, CPU_INFOC, 12 );
			*INFO_buff_Ptr=0x0;  
			*(INFO_buff_Ptr+1)=0x0; 
		  *(INFO_buff_Ptr+2)=0x0;
		  //д��Ϣ������INFOC
  		cpu_Fls_int_SegClr( 0x55,(unsigned char *)CPU_INFOC  );
  	 	cpu_Fls_int_Write(0x0C, CPU_INFOC, INFO_buff_Ptr, 12 );	
  	 	
  	 	//��������ɹ���¼..
  	 	Flag.Image_Transferred_Status = 6;	
			E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
					
			SM.StandardEventCode = Firmware_activated;
			LoadRecord( Standard_Event_No );
			SM.Event_all_flag |= Standard_Event_BIT;
			SM.Error_all_flag |= Standard_Event_BIT;
			
			Flag.New_check_ok = 0;			//����..
			Flag.New_Test_Flag = 0;  	
			
			Ptr = ImageBuff;			
 			E2P_RData( Ptr,IMMAGE_TRANSFER_IDENTIFIER, 6 );		//���������İ汾��..
			E2P_WData( Device_ID7, Ptr, 6 ); 	
		 	*(Ptr+6) = 0x01;
		 	E2P_WData( ImageID7_1, Ptr, 7 );						
  }else{
  			 if(Flag.New_Image_safetime==0)  //ִ���ϳ���..
  			 	{
						  //���㱸�ݵ�ǰ�����У���..
							//У�����Դ�..
							//����������ָʾ��β�����
							// 0xa1a5---����A--�³���
							// 0xb3b7--��������B---�ϳ���
							// 0xc1c3XX---����XX--��ʾʧ�ܴ���..
							//дCPU-INFO-FLASH C..
							
							//д��Ϣ������INFOC
							INFO_buff_Ptr = INFO_buff;
							cpu_fls_int_Read_Flash( INFO_buff_Ptr, CPU_INFOC, 12 ); 
						  cpu_Fls_int_SegClr( 0x55,(unsigned char *)CPU_INFOC  );
							*INFO_buff_Ptr = 0xb3;
							*(INFO_buff_Ptr+1) = 0xb7;
							*(INFO_buff_Ptr+2) = 0xff;
							
							//�ļ�������ֽ���#define FLASH_256K	 0x40000		//256K..
							*(INFO_buff_Ptr+3) = 0;
							*(INFO_buff_Ptr+4) = 0x03;
							*(INFO_buff_Ptr+5) = 0xf0;
							*(INFO_buff_Ptr+6) = 0;
							 
							*(INFO_buff_Ptr+7) = 0;
							*(INFO_buff_Ptr+8) = 0x03;
							*(INFO_buff_Ptr+9) = 0xf0;
							*(INFO_buff_Ptr+10) = 0;
	 					 
	 					  cpu_Fls_int_Write(0x0C, CPU_INFOC, INFO_buff_Ptr, 12 );
							 
							Flag.Image_Transferred_Status = 7;	
							E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
						
						//д�汾��..
							Ptr = ImageBuff;			
				 			E2P_RData( Ptr,Device_ID7, 6 );		//���������İ汾��..
						 	E2P_WData( ImageID7_0, Ptr, 6 ); 	
						 	E2P_WData( ImageID7_2, Ptr, 6 );
						 	*Ptr = 0;
						 	*(Ptr+1) = 0;			
						  *(Ptr+2) = 0;		
						 	*(Ptr+3) = 0;	
						 	*(Ptr+4) = 0;			
						 	*(Ptr+5) = 0;	
						 	*(Ptr+6) = 0;
						 	*(Ptr+7) = 0;	
						 	E2P_WData( ImageID7_1, Ptr, 7 );								 
						//	//  test
						//	cpu_fls_int_Read_Flash( Ptr+10, CPU_INFOC, 3 );		
							
							cpu_Fls_int_SegClr( 0x55,(unsigned char *)0xee00  );
							
							SM.Init_Flag = 0;			//��ʼ��.. 
							asm(" mov &0xFFFE, PC;");   	//ִ�������� 		 
  			 	}
  		}
}
#endif
