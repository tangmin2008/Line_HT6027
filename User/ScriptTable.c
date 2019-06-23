//#include <Includes.h>
#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"

#include "Mem.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"
#include "RegisterActivation.h"
#include "Imagetranfer.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
//#include "PushSetup.h"
//#include "Getdata.h"

//ȡScript_table������
//Scripts���ű���	�涨��ͬ�Ľű����������б�
//array              script
//script              structure
//{
//script_identifier: long-unsigned
//actions: array action_specification
//}
//�ű���ʶ��0��script_identifier 0��������
//���涨һ��ִ�з����ı�ʶ����script_identifier��Ϊ0��������һ���սű�(���κο�ִ�еĲ���)��
//action_specification  structure
//{
//service_id: enum
//class_id: long-unsigned
//logical_name: octet-string
//index: integer
//parameter: service specific
//}
//���У�
//service_id������Ӧ���������ö���Ĳ�����
//      1)  д����
//      2)  ִ���ض��ķ���
//index������(��service_idΪ1)��ѡ�������һ������Ҫ�ܵ�Ӱ�죬����(��service_idΪ2)��һ������Ҫ��ִ�С�
//��һ������(logical_name)��������Ϊ1����һ��������������ҲΪ1��
//ע�� ����˵����action_specification��ֻ���ڼ�������κ���Ӧ�ķ���(�ӷ��������ͻ���)��

short GetScript_tableClassData( unsigned char* Dest, unsigned char* Source ) 
{
//  unsigned short 	NumAddr;			//�ű���
// 	unsigned short 	ComInfo;			
//	unsigned short 	IdentAddr;			//�ű���ʶ��1��ַ
	unsigned short 	ActsNumAddr;		//�ű���ʶ��1����˵������ַ
	unsigned short 	ActsAddr;			//�ű���ʶ��1����˵����
// 	unsigned short 	Length;				//�ű���ʶ������˵��������
//	short i,j;
	short i,j,k;
//	unsigned char Buff[20];
//	unsigned char Logical_Name[6];
	unsigned char* Ptr;
//	unsigned char* Point;
//	unsigned short Addr;
	unsigned char Len=2;

	unsigned char Attribute,Num_array,Num_array1;
	
//	Ptr = Logical_Name;
////	Point = Buff;
//
//	RAM_Write( Ptr, Source+2, 6 );
	
	Ptr = Source+2;
	
	for( i=0;i<DLMS_ScriptParaSum;i++ )
	{
		if( Data_Comp( Ptr, (unsigned char*)DLMS_ScriptParaTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	if( i == DLMS_ScriptParaSum )
	{
		*Dest = Get_data_access_result;					//�쳣��־..
		*(Dest+1) = object_undefined; 					//�޶�Ӧ��OBIS��..
		Len = 2;		
	}
	else 
	{
		*Dest = Get_data;											//�������ݷ���..	
		Attribute = *(Source+2+6);						//���������..
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = Data_octet_string;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:										//����2..
			ActsNumAddr = DLMS_ScriptParaTab[i].ActsNumAddr;
			E2P_RData((unsigned char *)&Num_array,ActsNumAddr,1);
			if(Num_array>3) Num_array=1;
			*(Dest+1) = Data_array;
			*(Dest+2) = Num_array;
			Ptr = Dest+3;
			ActsAddr = DLMS_ScriptParaTab[i].ActsAddr;
			Len = 3;
			for(i=0;i<Num_array;i++)
				{
				*Ptr = Data_structure;
				*(Ptr+1) = 0x02;
				*(Ptr+2) = D_Unsigned16;
				E2P_RData(Ptr+3,ActsAddr,2);
				*(Ptr+5) = Data_array;
				*(Ptr+6) = 0x01;
				*(Ptr+7) = Data_structure;   
				*(Ptr+8) = 0x05;
				*(Ptr+9) = Data_enum;
				E2P_RData(Ptr+10,ActsAddr+3,1);
				*(Ptr+11) = D_Unsigned16;
				E2P_RData(Ptr+12,ActsAddr+5,2);
				*(Ptr+14) = Data_octet_string;
				*(Ptr+15) = 0x06;
				E2P_RData(Ptr+16,ActsAddr+8,6);
				*(Ptr+22) = D_Integer8;
				E2P_RData(Ptr+23,ActsAddr+15,1);
				*(Ptr+24) = Data_octet_string;
				*(Ptr+25) = 0x05;
				E2P_RData(Ptr+26,ActsAddr+17,5);
				Len += 31;
				ActsAddr+=23;
				Ptr+=31;
				}
					break;							
			default:
					break;	
				}	
			}
	return Len;
}
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
unsigned char ActionScript_tableClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method,i;
	unsigned char DataType;
	unsigned short Script_identifier;
	unsigned char buff[2]; 
	unsigned char* TEST;
	Ptr = Buff;
	TEST = buff;
	Logical_Name = Source+2;
	Method = *(Source+8);
		// ���Ӽ������ж�..
	if(*(Source+9)==0x01)
   {
      DataType = *(Source+10);
			Script_identifier = GetBuffToInteger( Source+11 );
   }
   else
   {
			DataType = *(Source+9);
			Script_identifier = GetBuffToInteger( Source+10 );
   }
	for( i=0;i<DLMS_ScriptParaSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_ScriptParaTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	if( i == DLMS_ScriptParaSum ) { return object_undefined; }
	
	switch( i )
	{
		case 0:														//ѡ���ĸ��ű�..			
			if(( Method == 1 )&&( DataType == D_Unsigned16 )&&( Script_identifier == 1 ))
			{
	   			///if( SM.PrgDly != 0 )
					if(( SM.PrgDly != 0 )||(( PIN_SETEN & P_SETEN ) == 0 ))		 //ȷ�ϱ�̰����򳬼�����..
					{
                     SM.MeterClrState = MeterClrStep1;
					SM.MeterClrStateChk = MeterClrStep1 + 0xA5A5;						
				}
				else
					ErrorByte = other_reason;	
			}
			else ErrorByte = other_reason;										
			break;	
		case 1:	
					E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//��������ɱ�־..
				if(Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )  
					return temporary_failure;				//δУ���򲻽��м������..				
			
					Flag.Image_Transferred_Status = ACTIVATION_INITIATED;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
					//�����..			  
				    *Ptr = 0xb5;
					*(Ptr+1) = 0xc9;				
					E2P_WData( IMMAGE_TRANSFER_ACTIVATE, Ptr, 2 );
					Flag.Image_Tranfer_Flag = 0x70;					
				 Disk.FullDisDelay = 0x06;			//��ʾ6sȫ��..
			break;
		case 2:	
			if(( Method == 1 )&&( DataType == D_Unsigned16 )&&( Script_identifier <=4  ))
			{
					//ִ�з����ж�..
					__NOP();		//�Ƿ���Ҫ��Ӧ���޿�ִ�ж�Ӧ�ű�������Ҫ����ִ���κβ���..
					GetDay_schedule( GetDay_id() );											//�ж�ȡ��ǰ����
					GetFeeNo();
			}
			else ErrorByte = other_reason;													
			break;
			
		case 3: 
			//Disconnector control
			if(( Method == 2 )&&( DataType == D_Unsigned16 ))
			{
				if( Script_identifier == 1 )	
					{

					}
			/*		__NOP();		//�Ƿ���Ҫ��Ӧ���޿�ִ�ж�Ӧ�ű�������Ҫ����ִ���κβ���..
					
					buff[0]=0;
					E2P_WData( DisDigitNum, TEST,1 );	//����С��λ��0
					E2P_RData( (unsigned char*)&Para.RDisDigitNum, DisDigitNum, 1 );	//����С��λ��
					*/
			}
			else  ErrorByte = other_reason;										
			break;
		case 4: 
			//Normal mode
				if(( Method == 1 )&&( DataType == D_Unsigned16 )&&( Script_identifier == 1 ))
			{
					
					__NOP();		//�Ƿ���Ҫ��Ӧ���޿�ִ�ж�Ӧ�ű�������Ҫ����ִ���κβ���..
					
					buff[0]=3;
					E2P_WData( DisDigitNum, TEST,1 );	//����С��λ��0
					E2P_RData( (unsigned char*)&Para.RDisDigitNum, DisDigitNum, 1 );	//����С��λ��
			}
			else  ErrorByte = other_reason;										
				break;
			
		default:
			break;	
	}						
		
	return ErrorByte;
}	
/*
//���״̬��־�¼���¼..
void Clr_local_control_para_monitor( void )
{
	// limiter.Limiter_RelayMode = 0; //..
	//limiter.Ram_Limiter_threshold_active = 0xFFFFFFFF;
	limiter.Threshold_MDEvent.byte = 0;
	limiter.flg_Threshold_MDEvent1.byte =0;
	limiter.flg_Threshold_MDEvent1BK.byte =0;
	
  limiter.min_under_threshold_duration_cnt = 0;
  limiter.Ram_Reconnection_timeout_emergency_cnt=0;
  
   limiter.Threshold_MDEvent.byte = 0;  
 	 limiter.over_threshold_duration_cnt = 0;
 	
 	limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;
 	limiter.Ram_Auto_Reclosing_relay_Num_cnt =0; 

}

void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	if(( Para.RMeterRunMode1 & 0x01 ) != 0 ) ICComm.RelayDrivePulseWidth = 192;		//����3S
	else ICComm.RelayDrivePulseWidth = 0;
	
	if( DriveType == RELAY_ON ) GPIO->PBSET |=	P_RELAY1ON;  			//11.03.29
	if( DriveType == RELAY_OFF ) GPIO->PBCLR |=	P_RELAY1ON;	 			//11.03.29
}

void OperateRelay( void )
{
	if( Para.RMeterTripState == 0x1A )	//Զ����բ..
	{			
		if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//׼����բ..																											
		ICComm.RelayONState = 0x0;
	}	

	if(( Para.RMeterRelayState == 0x1A )&&( Para.RMeterTripState == 0x1B ))
	{
		if( ICComm.RelayONState	== 0 )
		{
//			if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))
			if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x04 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))   //SKY20140218..
			{
				if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//�����բ(���ܸ��ϵ�)..
			}
			else if( Para.RMeterControlMode == 0x00 )   //��ģʽ4�Ļ��������ӵ�ǰ���ֶ�����������..//SKY20140218..
//			else if(( Para.RMeterControlMode == 0x00 )||( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x04 ))     
			{
				if( Para.RMeterRelayState == 0x1A ) 
				{
					ICComm.RelayONState = 0x55;	//������բ(���ܸ��ϵ�).. 			
				}	
			}
		}		
	}	

	if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//ִ�к�բ..		
	{
		limiter.LAST_OVER_Event.byte = 0;    //sky2013.12.24..
		ICComm.RelayDriveCnt = 5;	
		RelayDrive( RELAY_ON );	
		ICComm.RelayONState = 0x0;
		Para.RMeterRelayState = 0x55;			//�Ѿ���բ..
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
		if( SM.DisconnectorControlCode == Manual_connection ){
			 LoadRecord( Disconnector_Control_No );
			 	SM.Event_all_flag |= Disconnector_Control_BIT;
			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();			 	
		}
	}				

	if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//ִ����բ..		
	{
		ICComm.RelayDriveCnt = 5;	
		RelayDrive( RELAY_OFF );	
		ICComm.RelayOFFState = 0x0;
		Para.RMeterRelayState = 0x1A;			//�Ѿ���բ..
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
	}				
}		
*/
void Exec_Disconnect_control_Script( unsigned short script_selector )
{

	if( script_selector == 0x0001 )
	{ 
		//������բ..
		if( Para.RMeterControlMode > 0 )
		{
			Para.RMeterTripState = Route_G; 
			
			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();
		}
	}else if( script_selector == 0x0002 )
		{
		if( Para.RMeterControlMode > 4 )
			Para.RMeterTripState = Route_H;

			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();
		}
}

void Exec_Tariffication_Script( unsigned char* script_logical_name,  unsigned short  script_selector )
{
	unsigned char Buff[15];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	*Ptr = D_OctetStr;
	*(Ptr+1) = 6;	
	RAM_Write( Ptr+2, script_logical_name, 6 );	
	if( script_selector == 0x0001 )
	{ 
		*(Ptr+8) = 0x01;
		*(Ptr+11) = 0x01;
	}
	else if( script_selector == 0x0002 )
	{
		 *(Ptr+8) = 0x02;
		 *(Ptr+11) = 0x02;
	}
        else if( script_selector == 0x0003 )
	{
		 *(Ptr+8) = 0x03;
		 *(Ptr+11) = 0x03;		 
	}
        else if( script_selector == 0x0004 )
	{
		 *(Ptr+8) = 0x04;
		 *(Ptr+11) = 0x04;		 		 		 		 
	}else{
		 *(Ptr+8) = 0x0;
		 *(Ptr+11) = 0x0;		 		
	}
	*(Ptr+9) = D_Unsigned16;
	*(Ptr+10) = 0;		
	ActionScript_tableClassData( Ptr, 12 );
}


void Exec_Image_activation_Script( unsigned short script_selector )
{
	unsigned char* Ptr;
	unsigned char buff[2]; 
	Ptr = buff;

		E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//��������ɱ�־..
	if(Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )  
		return ;				//δУ���򲻽��м������..				
	
		Flag.Image_Transferred_Status = ACTIVATION_INITIATED;	
		E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
		//�����..			  
		*Ptr = 0xb5;
		*(Ptr+1) = 0xc9;				
		E2P_WData( IMMAGE_TRANSFER_ACTIVATE, Ptr, 2 );
		Flag.Image_Tranfer_Flag = 0x70; 				
	 Disk.FullDisDelay = 0x06;			//��ʾ6sȫ��..
}		


void Exec_ScriptTable( unsigned char* script_logical_name, unsigned short script_selector )
{
	short i;
	unsigned short Script_No;
	unsigned char buff[2]; 
	unsigned char* ptr;
	ptr = buff;
	for( i=0;i<DLMS_ScriptSum;i++ )
	{
		if(( Data_Comp( script_logical_name, (unsigned char*)DLMS_ScriptTab[i].Logical_Name, 6 ) == 0 )&&( DLMS_ScriptTab[i].Script_Ident == script_selector ))
		{
			Script_No = DLMS_ScriptTab[i].Script_No;				
			break;
		}	
	}	

	if( i == DLMS_ScriptSum ) return;	
	
	switch( Script_No )
	{
		case Global_meter_reset_ScriptNo:	
	   				//if(1){
	   			if( SM.PrgDly != 0 )
					{
					SM.MeterClrState = MeterClrStep1;
					SM.MeterClrStateChk = MeterClrStep1 + 0xA5A5;
					}						
			break;	
		case MDI_End_of_billing_period1_ScriptNo:
			Exec_MDI_reset_End_of_billing_period();//�¶���
			//����¼�
		///	clr_event_Ram();
			break;
		case MDI_End_of_billing_period2_ScriptNo:
			LoadRecord( Billing_period2_No );//�춳��
			break;
		case Tariffication_Script1No:			
		case Tariffication_Script2No:			
		case Tariffication_Script3No:			
		case Tariffication_Script4No:
			Exec_Tariffication_Script( script_logical_name, script_selector );
			break;	
		case Activate_Test_Script1No:
			buff[0]=0;
			E2P_WData( DisDigitNum, ptr,1 );	//����С��λ��0
			break;
		case Activate_Normal_Script1No:
			buff[0]=3;
			E2P_WData( DisDigitNum, ptr,1 );	//����С��λ��3
			break;
			/*
		case Disconnect_control_Script1No:
		case Disconnect_control_Script2No:
			Exec_Disconnect_control_Script( script_logical_name, script_selector );
			break;
		case Image_activation_ScriptNo:
			Exec_Image_activation_Script( script_logical_name, script_selector );
			break;	
			*/
		default:
			break;	
	}	
				
}	
void Exec_Push_Script( unsigned char* script_logical_name,  unsigned short  script_selector )
{
	switch(script_selector)
		{
		case Push_Interval_1:
		//	PushSetup_Interval_1(3,1);
			break;
		case Push_Interval_2:
		//	PushSetup_Interval_2(3,2);
			break;
		case Push_Interval_3:
		//	PushSetup_Interval_3(3,3);
			break;
		case Push_Alarm:
		//	Push_Alarms(3,4);
			break;
		case Push_On_Connectivity:
		//	PushSetup_On_Connectivity(3,0);
			break;
		case Push_On_Installation:
			break;
		case Push_On_Power_Down:
			break;
		case Push_Consumer_Installation:
			break;
		default:
			break;
		}
}
void Exec_Billing_Period_Script( unsigned short script_selector )
{
	switch(script_selector)
		{
		case MDI_End_of_billing_period1:
			LoadRecord( Billing_period2_No );//�ն���
				break;
		case MDI_End_of_billing_period2:
			Exec_MDI_reset_End_of_billing_period();//�¶���
			clr_event_Ram();
				break;
		default:
				break;
		}
}

//-----------2016-9-28--fire-----class 9 ���ű���ѡ���Ӧ����ִ��
void Class9_ScriptTable( unsigned char* script_logical_name, unsigned short script_selector )
{
	short i;
	unsigned short Script_No;
	
	for( i=0;i<DLMS_ScriptParaSum;i++ )
	{
		if( Data_Comp( script_logical_name, (unsigned char*)DLMS_ScriptParaTab[i].Logical_Name, 6 ) == 0 )
		{
			break;
		}	
	}	

	if( i == DLMS_ScriptParaSum ) return;
	//The script_identifier 0 is reserved. If specified with an execute method, it results
	//in a null script (no actions to perform).
	if( script_selector == 0 ) return;	
	Script_No = DLMS_ScriptParaTab[i].NumAddr;
	switch( Script_No )
	{
		case Global_meter_reset_Script:	
				///if( SM.PrgDly != 0 )
				if(( SM.PrgDly != 0 )||(( PIN_SETEN & P_SETEN ) == 0 ))		 //ȷ�ϱ�̰����򳬼�����..
					{
					SM.MeterClrState = MeterClrStep1;
					SM.MeterClrStateChk = MeterClrStep1 + 0xA5A5;
					}						
			break;
			case Image_activation_Script:
				Exec_Image_activation_Script( script_selector );
				break;
			case Tariffication_Script:
				Exec_Tariffication_Script( script_logical_name, script_selector );
				break;
			case Disconnect_control_Script:
				Exec_Disconnect_control_Script( script_selector );					
				break;
			case Push_script:
				Exec_Push_Script( script_logical_name, script_selector );	///ִ��40�����Ͳ���	
				break;
			case Passive_Tariffication_Script:
				//Passive Tariffication Script			
				break;
			case MBus_Disconnector_script:
				//GPRS_ConnectORNot(3,2); //�ر�
				//M-Bus Disconnector
				break;
			case Limiter_Script:
				//Limiter Script Table	
				break;
			case MDI_End_of_billing_period_Script:
				//MDI Reset (Predefined Script)/ End of Billing Period
				Exec_Billing_Period_Script( script_selector );
				break;
			default:
				break;	
	}	
}	

unsigned char SetScript_TableClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short ActsAddr;
	unsigned short	ActsNumAddr;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
    unsigned char Num_array;
    unsigned short i;

	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	// DataType = WriteBufAds;
	
	for( i=0;i<DLMS_ScriptParaSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_ScriptParaTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	if( i == DLMS_ScriptParaSum ) { return object_undefined; }
	
	if(( *Attribute == 1 )||(( DLMS_ScriptParaTab[i].ComInfo & WRITE ) == 0) ) { return read_write_denied; }

	if( *Attribute == 2 )
		{
		 E2P_WData(ActsNumAddr,WriteBufAds+1,1);
		 Num_array = *(WriteBufAds+1);
		 Ptr = WriteBufAds+2;
		 ActsAddr = DLMS_ScriptParaTab[i].ActsAddr;
		for(i=0;i<Num_array;i++)
			{
			E2P_WData(ActsAddr,Ptr+3,2);
			E2P_WData(ActsAddr+3,Ptr+10,1);
			E2P_WData(ActsAddr+5,Ptr+12,2);
			E2P_WData(ActsAddr+8,Ptr+16,6);
			E2P_WData(ActsAddr+15,Ptr+23,1);
			E2P_WData(ActsAddr+17,Ptr+25,5);
			
			Ptr += 32;
			ActsAddr+=23;
			}
		}
	return ErrorByte;
}	


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short Judge_Script_Table_objects( unsigned char* Data, unsigned char Tab_No )
{
	unsigned char Buff[12];
	
	unsigned short Items;
	short i,j,k;
	unsigned char* Ptr;
	unsigned short Temp;
			
	Ptr = Buff;
        
	if( *Data != Data_array ) return other_reason;	
			Items = *(Data+1);
			if((Tab_No==2)&&(Items>4)) return other_reason;	
			Ptr = Data +2;
				for( i=0;i<Items;i++ )
				{
					if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )) return other_reason;		
					if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+5) != Data_array )) return other_reason;
				
					Temp = *(Ptr+6);
					if((Tab_No==2)&&( Temp > 2))  return other_reason;
					Ptr += 7;
					for(j=0;j<Temp;j++)
					{
						if(( *Ptr != Data_structure )||( *(Ptr+1) != 5 )||( *(Ptr+2) != Data_enum )||( *(Ptr+4) != D_Unsigned16 )||( *(Ptr+7) != Data_octet_string )||( *(Ptr+8) != 0x06 )||( *(Ptr+15) != D_Integer8 )||( *(Ptr+17) != Data_octet_string )||( *(Ptr+18) != 0x05 )) return other_reason;		
							
					  if(Tab_No==2)
					  {
					  	if(( *(Ptr+5) != 0 )||( *(Ptr+6) != 0x06 )) return other_reason;		//�ж��Ƿ��ǵ�6��..
					  		
					  	for( k=0;k<DLMS_Reg_ActivationSum;k++ )
						{
							if( Data_Comp( Ptr+9, (unsigned char*)DLMS_Reg_ActivationTab[k].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
						}	
						if( k == DLMS_Reg_ActivationSum ) return other_reason;		//�ж��Ƿ��ǵ���/����..
							
					  	for(k=0;k<4;k++)
					  	{
					  		if( Data_Comp( Ptr+19, (unsigned char*)DLMS_ACTIVATION_FEE[k].FEE_OCTET_STRING, 5 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��					  						
					  	}
					  	if( k == 4 ) return other_reason;		//�ж��Ƿ��Ƿ����õ��Ƕ����4�����ʺ�..
					  }
						Ptr +=24;	
					}					
				}
	return 0;
}
short Judge_Test_Normal_Table_objects( unsigned char* Data, unsigned char Tab_No )
{
	unsigned char Buff[12];
	
	unsigned short Items;
	short i,j,k;
	unsigned char* Ptr;
	unsigned short Temp;
			
	Ptr = Buff;
        
	if( *Data != Data_array ) return other_reason;	
			Items = *(Data+1);
			if(((Tab_No==3)||(Tab_No==4))&&(Items>4)) return other_reason;	
			Ptr = Data +2;
				for( i=0;i<Items;i++ )
				{
					if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )) return other_reason;		
					if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+5) != Data_array )) return other_reason;
				
					Temp = *(Ptr+6);
					///if((Tab_No==2)&&( Temp > 2))  return other_reason;
					Ptr += 7;
					for(j=0;j<Temp;j++)
					{
						if(( *Ptr != Data_structure )||( *(Ptr+1) != 5 )||( *(Ptr+2) != Data_enum )||( *(Ptr+4) != D_Unsigned16 )||( *(Ptr+7) != Data_octet_string )||( *(Ptr+8) != 0x06 )||( *(Ptr+15) != D_Integer8 )||( *(Ptr+17) != Data_octet_string )||( *(Ptr+18) != 0x05 )) return other_reason;		
							
					  if((Tab_No==3)||(Tab_No==4))
					  {
					  	if(( *(Ptr+5) != 0 )||( *(Ptr+6) != 0x46 )) return other_reason;		//�ж��Ƿ��ǵ�70��..					  		
					  }
						Ptr +=24;	
					}					
				}
	return 0;
}


//����2��дѡ��..
unsigned char GetScript_tableAccessPurview( unsigned char* Logical_Name )
{
	short i;
	unsigned short Temp;
	unsigned char AccessPurview;
	
	for( i=0;i<DLMS_ScriptParaSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_ScriptParaTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	if( i == DLMS_ScriptParaSum ) { return object_undefined; }		

	Temp = ( DLMS_ScriptParaTab[i].ComInfo & ( WRITE + READ ));
	switch( Temp)
	{
		case READ: 
			AccessPurview = read_only; 	
			break;
		case WRITE: 
			AccessPurview = write_only; 	
			break;
		case (READ+WRITE): 
			AccessPurview = read_and_write; 	
			break;
		default:
		   	AccessPurview = no_access;
		   	break; 			
	}	 
	return AccessPurview;
}
		

//==��ʼ��Ĭ��..
void initialTariffication_Script_TablePara( void )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned short ActsAddr;
	unsigned char max_array=1;
	
//����ű�
	Ptr = Buff;			
	E2P_WData( DLMS_ScriptParaTab[0].ActsNumAddr, &max_array, 1 );
	ActsAddr = DLMS_ScriptParaTab[0].ActsAddr;
	*Ptr=0x00;
	*(Ptr+1)=0x01;
	E2P_WData(ActsAddr,Ptr,2);
	*(Ptr+2)=0x02;
	E2P_WData(ActsAddr+3,Ptr+2,1);
	*(Ptr+3)=0x00;
	*(Ptr+4)=0x09;
	E2P_WData(ActsAddr+5,Ptr+3,2);
	*(Ptr+5)=0x00;
	*(Ptr+6)=0x00;
	*(Ptr+7)=0x0A;
	*(Ptr+8)=0x00;
	*(Ptr+9)=0x00;
	*(Ptr+10)=0xff;
	E2P_WData(ActsAddr+8,Ptr+5,6);
	*(Ptr+11)=0x01;
	E2P_WData(ActsAddr+15,Ptr+11,1);
	*(Ptr+12)=0xff;
	*(Ptr+13)=0xff;
	*(Ptr+14)=0xff;
	*(Ptr+15)=0xff;
	*(Ptr+16)=0xff;
	E2P_WData(ActsAddr+17,Ptr+12,5);

	//�����ű�
	Ptr = Buff;
	E2P_WData( DLMS_ScriptParaTab[1].ActsNumAddr, &max_array, 1 );
	ActsAddr = DLMS_ScriptParaTab[1].ActsAddr;
	*Ptr=0x00;
	*(Ptr+1)=0x01;
	E2P_WData(ActsAddr,Ptr,2);
	*(Ptr+2)=0x02;
	E2P_WData(ActsAddr+3,Ptr+2,1);
	*(Ptr+3)=0x00;
	*(Ptr+4)=0x12;
	E2P_WData(ActsAddr+5,Ptr+3,2);
	*(Ptr+5)=0x00;
	*(Ptr+6)=0x00;
	*(Ptr+7)=0x2C;
	*(Ptr+8)=0x00;
	*(Ptr+9)=0x00;
	*(Ptr+10)=0xff;
	E2P_WData(ActsAddr+8,Ptr+5,6);
	*(Ptr+11)=0x04;
	E2P_WData(ActsAddr+15,Ptr+11,1);
	*(Ptr+12)=0xff;
	*(Ptr+13)=0xff;
	*(Ptr+14)=0xff;
	*(Ptr+15)=0xff;
	*(Ptr+16)=0xff;
	E2P_WData(ActsAddr+17,Ptr+12,5);
	
	//�̵������ƽű�
	Ptr = Buff;
	max_array=2;
	E2P_WData( DLMS_ScriptParaTab[3].ActsNumAddr, &max_array, 1 );
	ActsAddr = DLMS_ScriptParaTab[3].ActsAddr;
	*Ptr=0x00;
	*(Ptr+1)=0x01;
	E2P_WData(ActsAddr,Ptr,2);
	*(Ptr+2)=0x02;
	E2P_WData(ActsAddr+3,Ptr+2,1);
	*(Ptr+3)=0x00;
	*(Ptr+4)=0x46;
	E2P_WData(ActsAddr+5,Ptr+3,2);
	*(Ptr+5)=0x00;
	*(Ptr+6)=0x00;
	*(Ptr+7)=0x60;
	*(Ptr+8)=0x03;
	*(Ptr+9)=0x0A;
	*(Ptr+10)=0xff;
	E2P_WData(ActsAddr+8,Ptr+5,6);
	*(Ptr+11)=0x01;//��բ
	E2P_WData(ActsAddr+15,Ptr+11,1);
	*(Ptr+12)=0xff;
	*(Ptr+13)=0xff;
	*(Ptr+14)=0xff;
	*(Ptr+15)=0xff;
	*(Ptr+16)=0xff;
	E2P_WData(ActsAddr+17,Ptr+12,5);

	Ptr = Buff;
	ActsAddr +=23;
	*Ptr=0x00;
	*(Ptr+1)=0x02;
	E2P_WData(ActsAddr,Ptr,2);
	*(Ptr+2)=0x02;
	E2P_WData(ActsAddr+3,Ptr+2,1);
	*(Ptr+3)=0x00;
	*(Ptr+4)=0x46;
	E2P_WData(ActsAddr+5,Ptr+3,2);
	*(Ptr+5)=0x00;
	*(Ptr+6)=0x00;
	*(Ptr+7)=0x60;
	*(Ptr+8)=0x03;
	*(Ptr+9)=0x0A;
	*(Ptr+10)=0xff;
	E2P_WData(ActsAddr+8,Ptr+5,6);
	*(Ptr+11)=0x02;//��բ
	E2P_WData(ActsAddr+15,Ptr+11,1);
	*(Ptr+12)=0xff;
	*(Ptr+13)=0xff;
	*(Ptr+14)=0xff;
	*(Ptr+15)=0xff;
	*(Ptr+16)=0xff;
	E2P_WData(ActsAddr+17,Ptr+12,5);

	//�¶��� �ն�����ƽű�
	Ptr = Buff;
	max_array=2;
	E2P_WData( DLMS_ScriptParaTab[8].ActsNumAddr, &max_array, 1 );
	ActsAddr = DLMS_ScriptParaTab[8].ActsAddr;
	*Ptr=0x00;
	*(Ptr+1)=0x01;
	E2P_WData(ActsAddr,Ptr,2);
	*(Ptr+2)=0x02;
	E2P_WData(ActsAddr+3,Ptr+2,1);
	*(Ptr+3)=0x00;
	*(Ptr+4)=0x07;
	E2P_WData(ActsAddr+5,Ptr+3,2);
	*(Ptr+5)=0x00;
	*(Ptr+6)=0x00;
	*(Ptr+7)=0x62;
	*(Ptr+8)=0x02;
	*(Ptr+9)=0x00;
	*(Ptr+10)=0xff;
	E2P_WData(ActsAddr+8,Ptr+5,6);
	*(Ptr+11)=0x02;//�ն���
	E2P_WData(ActsAddr+15,Ptr+11,1);
	*(Ptr+12)=0xff;
	*(Ptr+13)=0xff;
	*(Ptr+14)=0xff;
	*(Ptr+15)=0xff;
	*(Ptr+16)=0xff;
	E2P_WData(ActsAddr+17,Ptr+12,5);

	Ptr = Buff;
	ActsAddr +=23;
	*Ptr=0x00;
	*(Ptr+1)=0x02;
	E2P_WData(ActsAddr,Ptr,2);
	*(Ptr+2)=0x02;
	E2P_WData(ActsAddr+3,Ptr+2,1);
	*(Ptr+3)=0x00;
	*(Ptr+4)=0x07;
	E2P_WData(ActsAddr+5,Ptr+3,2);
	*(Ptr+5)=0x00;
	*(Ptr+6)=0x00;
	*(Ptr+7)=0x62;
	*(Ptr+8)=0x01;
	*(Ptr+9)=0x00;
	*(Ptr+10)=0xff;
	E2P_WData(ActsAddr+8,Ptr+5,6);
	*(Ptr+11)=0x02;//�¶���
	E2P_WData(ActsAddr+15,Ptr+11,1);
	*(Ptr+12)=0xff;
	*(Ptr+13)=0xff;
	*(Ptr+14)=0xff;
	*(Ptr+15)=0xff;
	*(Ptr+16)=0xff;
	E2P_WData(ActsAddr+17,Ptr+12,5);
	
}	
