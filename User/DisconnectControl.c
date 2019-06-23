#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"

#include "Mem.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"
#include "RegisterActivation.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
///#include "Demand.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
//#include <RN8213.h>

//ȡDisconnect_control������
short GetDisconnect_controlClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
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
				RAM_Write( Dest+3, Source+2, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_BOOLEAN;	
				/*
				if( Para.RMeterRelayState == 0x1A  ) *(Dest+2) = 0;		//����բ	TRUE
				else *(Dest+2) = 1;										//δ��բ	FLASE
				Len = 3;
				*/
				E2P_RData(&Para.RMeterRelayState,MeterRelayState,1);
				if(Para.RMeterRelayState==1)
					*(Dest+2) = 1;//TRUE = (BOOLEAN 1) Connected
				else
					*(Dest+2) = 0;//FALSE = (BOOLEAN 0) Disconnected
					Len = 3;
				break;	
			case 3:
				*(Dest+1) = D_ENUM;	
//				if( Para.RMeterTripState == 0x1A  ) *(Dest+2) = 0;		//����բ	(0) Disconnected,
//				else if( Para.RMeterTripState == 0x55  ) *(Dest+2) = 2;	//��բ����  (2) Ready_for_reconnection
//				else *(Dest+2) = 1;										//�Ѻ�բ    (1) Connected,                   

	/*			if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState != 0x55 ))
				{
					if( ICComm.RelayONState != 0xAA ) *(Dest+2) = 0;	//����բ	(0) Disconnected,
					else *(Dest+2) = 2;									//��բ����  (2) Ready_for_reconnection
				}	
				else if( Para.RMeterRelayState != 0x1A )
				{
					*(Dest+2) = 1;										//�Ѻ�բ    (1) Connected,
				}	
*/
				E2P_RData(Dest+2,MeterRelayState,1);
				Len = 3;                                                                           
				break;	                                                            
			case 4:
				*(Dest+1) = D_ENUM;	
				E2P_RData(&Para.RMeterControlMode,MeterControlMode,1);
				*(Dest+2) = Para.RMeterControlMode;
				Len = 3;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char SetDisconnect_controlClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//�޶�Ӧ��OBIS��
	}	
	else if( *DataType != D_ENUM )
	{
		ErrorByte = type_unmatched; 									
	}	 
	else
	{
		switch( *Attribute )
		{
			case 4:
				if( *(WriteBufAds+1) > 6 )									
				{
					ErrorByte = other_reason;
					break;
				}
				Para.RMeterControlMode = *(WriteBufAds+1);
				E2P_WData( MeterControlMode, WriteBufAds+1, 1 );	
				break;	
			default: 
				ErrorByte = type_unmatched;			
				break;			
		}		
	}		
	return ErrorByte;
}		

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*
#if ( MagnKeepRelay	== YesCheck )
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	ICComm.RelayDrivePulseWidth = 5;
	
	if( DriveType == RELAY_ON ) 
	{
		PDIR_JDQA |= P_JDQA; 
		PDIR_JDQB |= P_JDQB; 
		POUT_JDQA &= ~P_JDQA;  
		Delay(30);							//11.05.06
		POUT_JDQB |= P_JDQB;
		POUT_JDQA &= ~P_JDQA;    
	}	
	if( DriveType == RELAY_OFF ) 
	{
		PDIR_JDQA |= P_JDQA; 
		PDIR_JDQB |= P_JDQB; 
		POUT_JDQB &= ~P_JDQB; 	//11.05.06
		Delay(30);							//11.05.06
		POUT_JDQA |= P_JDQA;  
		POUT_JDQB &= ~P_JDQB;  
	}			
}	
#else
*/
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	ICComm.RelayDrivePulseWidth = 5;
	
	if( DriveType == RELAY_ON ) 
	{
		HT_GPIOC->PTCLR |= P_RELAY1ON;
		HT_GPIOC->PTSET |= P_RELAY1OFF;
		
		HT_GPIOH->PTCLR |= P_RELAY2ON;
		HT_GPIOH->PTSET |= P_RELAY2OFF;
	}	
	if( DriveType == RELAY_OFF ) 
	{	
		HT_GPIOC->PTCLR |= P_RELAY1OFF;
		HT_GPIOC->PTSET |= P_RELAY1ON;
		
		HT_GPIOH->PTCLR |= P_RELAY2OFF;
		HT_GPIOH->PTSET |= P_RELAY2ON;
	}			
}

/*
{
	ICComm.RelayDriveType = DriveType;
	if(( Para.RMeterRunMode1 & 0x01 ) != 0 ) ICComm.RelayDrivePulseWidth = 192;		//����3S
	else ICComm.RelayDrivePulseWidth = 0;
	
//	if( DriveType == RELAY_ON ) POUT_JDQA &= ~P_JDQA;  			//11.03.29
//	if( DriveType == RELAY_OFF ) POUT_JDQA |= P_JDQA;  			//11.03.29
	if( DriveType == RELAY_ON ) 
		{
		POUT_RELAY1ON|=P_RELAY1ON;///POUT_RELBJ &= ~P_RELBJ;  			//11.03.29
		POUT_RELAY1OFF&=~P_RELAY1OFF;
		}
	if( DriveType == RELAY_OFF )
		{
		POUT_RELAY1OFF|=P_RELAY1OFF;/// POUT_RELBJ |= P_RELBJ;  			//11.03.29
		POUT_RELAY1ON&=~P_RELAY1ON;
		}
}
*/


///#endif

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
unsigned char ActionDisconnect_controlClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
		
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		return object_undefined; 									//�޶�Ӧ��OBIS��..
	}	
	
//	����..
//	if(*(Source+9)==0x01)
//   {
//     if((*(Source+10)!=0x0f)&&(*(Source+11)!=0))  return other_reason;	
//   }
		
	if( Method == 1 )		//��բ..
	{
		if( Para.RMeterControlMode > 0 )
		{
			Para.RMeterTripState=Route_B|Route_C;
			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();
		}else ErrorByte = other_reason;							
	}
	else if( Method == 2 )
	{
		if((Para.RMeterControlMode==2)||(Para.RMeterControlMode==4))
			Para.RMeterTripState=Route_A;
		else
			Para.RMeterTripState=Route_D;

			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();		
	}	
	else ErrorByte = other_reason;										

	return ErrorByte;
}	

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
////------------------------------------------------------------------------------
////** void  local_control_para_monitor( void )
////** DESCRIPTION: ʵʱ�������,ÿ���ӵ���һ��..
////** RETURNS: None..
////------------------------------------------------------------------------------
//void  local_control_para_monitor( void )
//{
//	BYTE_STRUCT   stateEvent1, stateEvent2;		//��ʱ���õ�1λ,Ԥ��..
//	unsigned char *Ptr_num_times;
//  unsigned char Threshod_num_times;
//	unsigned short *Ptr_time;
//	unsigned short Threshod_time;
//	unsigned char FLAG = 0;
//	 	 
//	//����Խ���ж�..
//	
//	Ave_Power();
//	if(limiter.Limiter_RelayMode == 0) return;		//��û��ģʽ(���ϵ�)..
//	if(limiter.Limiter_RelayMode == 0xB2)					//����..
//	{	
//			Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt;
//			Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_emergency;
//			Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
//			Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
//			//limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;
//	}else{
//	//				if((limiter.flg_Threshold_MDEvent1.byte&BIT0)&&(limiter.LastTime_Limiter_RelayMode == 0xB2))
//						
////									Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
////									Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
////				 }else{
//				 					Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_cnt;		//����..
//				 					Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_Normal;  //�����ж�ֵ
//				 					Ptr_time = &limiter.Ram_Reconnection_timeout_Normal_cnt;
//									Threshod_time = limiter.Ram_Reconnection_timeout_Normal;
//									// limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;	//�ϴ�ģʽ����.																		
////							}
//			}
//			
//					
//		//�����ж�..
//		if((*Ptr_num_times)<=Threshod_num_times)  //δ��������
//		{		
//				if(limiter.Ram_Limiter_monitored_value>limiter.Ram_Limiter_threshold_active)
//					{
//						  if( limiter.over_threshold_duration_cnt<limiter.Ram_Limiter_min_over_threshold_duration ){
//						      limiter.over_threshold_duration_cnt++;				      
//						  }else{
//						      	limiter.flg_Threshold_MDEvent1.byte |= BIT0;
//						       }
//						   (*Ptr_time) = 0;				 				  
//						  limiter.min_under_threshold_duration_cnt = 0;
//					}else{
//						 	  //�ж�Խ��..
//						 	   if( limiter.over_threshold_duration_cnt>0)
//						 	   	      limiter.over_threshold_duration_cnt--;				      
//						     else{
//						      		 limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//����..
//						      	}
//						      	
//						        	//���ж�һ��Խ��״̬����ʱ���������բ..
//										  if(*Ptr_time<Threshod_time){
//										  	  (*Ptr_time) += 1;
//										  	  // FLAG = 0;
//										  }
//						      	
//						 	   if( limiter.min_under_threshold_duration_cnt<limiter.Ram_Limiter_min_under_threshold_duration ){
//								      limiter.min_under_threshold_duration_cnt++;				      						      
//								  }else{
//								  			  limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//����..
//								  			 // *Ptr_num_times = 0;
//								  			  limiter.Ram_Auto_Reclosing_relay_Num_cnt =0;
//								  			  limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;		
//		 						       }
//		           // limiter.over_threshold_duration_cnt = 0;  
//					    }
//						//------------------------------------------------------
//						//�����������Խ��..
//					  stateEvent1.byte = limiter.flg_Threshold_MDEvent1.byte&BIT0;
//					  stateEvent2.byte = limiter.flg_Threshold_MDEvent1BK.byte&BIT0;
//						limiter.flg_Threshold_MDEvent1BK.byte = limiter.flg_Threshold_MDEvent1.byte;
//					//    record_eventFlg_MD( stateEvent1,stateEvent2,&quality_event.Threshold_MDEvent );	  
//					  if( stateEvent1.byte!=stateEvent2.byte )
//					  	{		  	
//								 if( stateEvent1.bit.b0!=stateEvent2.bit.b0 ){
//									  if( stateEvent1.bit.b0==1 ){				//��⵽A���¼�������ʼ..
//									      if( limiter.Threshold_MDEvent.bit.b4==1 ){
//									          limiter.Threshold_MDEvent.bit.b0 = 0;
//									          limiter.Threshold_MDEvent.bit.b4 = 0;
//									      }
//									      else
//									          limiter.Threshold_MDEvent.bit.b0 = 1;
//									  }
//									  else{//��⵽A���¼���������..
//									      if( limiter.Threshold_MDEvent.bit.b0==1 ){
//									          limiter.Threshold_MDEvent.bit.b0 = 0;
//									          limiter.Threshold_MDEvent.bit.b4 = 0;
//									      }
//									      else
//									          limiter.Threshold_MDEvent.bit.b4 = 1;
//									  }
//									}
//							}	
//	}else{
//					if(limiter.Limiter_RelayMode == 0xB2)		limiter.Threshold_MDEvent.bit.b0 = 1;			//����,��բ..
//					else 	limiter.Threshold_MDEvent.bit.b4 = 1;			//��բ..
//				}	
//}


//------------------------------------------------------------------------------
//** void Relay_localControl_Event( void )  //����ÿ�봦���������..
//** DESCRIPTION: �ж��¼��ķ�������¼����..
//** RETURNS: None..
//------------------------------------------------------------------------------

void Relay_localControl_Event( void )
{	
  	BYTE_STRUCT 	*eventFlg_Ptr;			  
		BYTE_STRUCT   stateEvent1, stateEvent2;		//��ʱ���õ�1λ,Ԥ��..
		unsigned char *Ptr_num_times;
  	unsigned char Threshod_num_times;
		unsigned short 	*Ptr_time;
		unsigned short 	Threshod_time;
		
	 
	  //ǰ10���ڲ����ж�..
	  // quality_event.Deal_EVENT_NO ++;  	//�˱�־�ڵ��������¼�������..
	  if(quality_event.Deal_EVENT_NO<10)  return;
	  	
		//ÿ��ִ��һ��	 	 
		//����Խ���ж�..
	
	Ave_Power();
	if(limiter.Limiter_RelayMode == 0) return;		//��û��ģʽ(���ϵ�)..
	
		if(limiter.LastTime_Limiter_RelayMode!=limiter.Limiter_RelayMode){	//ģʽ�л�
//				*Ptr_num_times = 0;
//				*Ptr_time = 0;
//				limiter.over_threshold_duration_cnt =0;
//				limiter.min_under_threshold_duration_cnt = 0;
//			//	if(limiter.LAST_OVER_Event.byte == 0x11) limiter.flg_Threshold_MDEvent1.byte |= BIT0;
//			//	else limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;
				//����ģʽ����ͨģʽ�����Ƚ��к�բ����..
				if((limiter.LastTime_Limiter_RelayMode ==0xb2)&&( limiter.Limiter_RelayMode!=0xb2)){
						if( Para.RMeterRelayState == 0x1A ){
								if(limiter.LAST_OVER_Event.byte == 0x11){
											limiter.LAST_OVER_Event.byte = 0;
											limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt = 0;
									 		localControl_EventEnd();
									}
						}
				} 
//					 	// ��������¼��жϱ�־..
						Clr_local_control_para_monitor();									
//				}				
		// ģʽ�仯..
		limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;
	}
	
	if(limiter.Limiter_RelayMode == 0xB2)					//����..
	{	
			Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt;
			Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_emergency;
			Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
			Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
			//limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;
	}else{
	//				if((limiter.flg_Threshold_MDEvent1.byte&BIT0)&&(limiter.LastTime_Limiter_RelayMode == 0xB2))
						
//									Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
//									Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
//				 }else{
				 					Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_cnt;		//����..
				 					Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_Normal;  //�����ж�ֵ
				 					Ptr_time = &limiter.Ram_Reconnection_timeout_Normal_cnt;
									Threshod_time = limiter.Ram_Reconnection_timeout_Normal;
									// limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;	//�ϴ�ģʽ����.																		
//							}
			}

					
//		//�����ж�..
//		if((*Ptr_num_times)<Threshod_num_times)  //δ��������
//		{		
				if(limiter.Ram_Limiter_monitored_value>limiter.Ram_Limiter_threshold_active)
					{
						  if( limiter.over_threshold_duration_cnt<limiter.Ram_Limiter_min_over_threshold_duration ){
						      limiter.over_threshold_duration_cnt++;				      
						  }else{
						      	limiter.flg_Threshold_MDEvent1.byte |= BIT0;
						       }
						   (*Ptr_time) = 0;				 				  
						  limiter.min_under_threshold_duration_cnt = 0;
					}else{
						 	  //�ж�Խ��..
						 	   if( limiter.over_threshold_duration_cnt>0)
						 	   	      limiter.over_threshold_duration_cnt--;				      
						     else{
						      		 limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//����..
						      	}
						      	
						   //���ж�һ��Խ��״̬����ʱ���������բ..
						   if(limiter.LAST_OVER_Event.byte == 0x11)
						    { 
									  if(*Ptr_time<Threshod_time){
									  	  (*Ptr_time) += 1;										 
									  }else *Ptr_time = Threshod_time;
						   	}else{ 
						   				//		//�����ж�..
											if((*Ptr_num_times)<Threshod_num_times)  //δ��������
											{								      		
												 	   if( limiter.min_under_threshold_duration_cnt<limiter.Ram_Limiter_min_under_threshold_duration ){
														      limiter.min_under_threshold_duration_cnt++;				      						      
														  }else{
														  			  limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//����..
														  			 // *Ptr_num_times = 0;
														  			  limiter.Ram_Auto_Reclosing_relay_Num_cnt =0;
														  			  limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;		
								 						       }
					 						  }
		 						   	}
		           // limiter.over_threshold_duration_cnt = 0;  
					    }
						//------------------------------------------------------
						//�����������Խ��..
					  stateEvent1.byte = limiter.flg_Threshold_MDEvent1.byte&BIT0;
					  stateEvent2.byte = limiter.flg_Threshold_MDEvent1BK.byte&BIT0;
						limiter.flg_Threshold_MDEvent1BK.byte = limiter.flg_Threshold_MDEvent1.byte;
					//    record_eventFlg_MD( stateEvent1,stateEvent2,&quality_event.Threshold_MDEvent );	  
					  if( stateEvent1.byte!=stateEvent2.byte )
					  	{		  	
								 if( stateEvent1.bit.b0!=stateEvent2.bit.b0 ){
									  if( stateEvent1.bit.b0==1 ){				//��⵽�¼�������ʼ..
									      if( limiter.Threshold_MDEvent.bit.b4==1 ){
									          limiter.Threshold_MDEvent.bit.b0 = 0;
									          limiter.Threshold_MDEvent.bit.b4 = 0;
									      }
									      else
									          limiter.Threshold_MDEvent.bit.b0 = 1;
									  }
									  else{//��⵽�¼���������..
									      if( limiter.Threshold_MDEvent.bit.b0==1 ){
									          limiter.Threshold_MDEvent.bit.b0 = 0;
									          limiter.Threshold_MDEvent.bit.b4 = 0;
									      }
									      else
									          limiter.Threshold_MDEvent.bit.b4 = 1;
									  }
									}
							}	
//	}else{
//					if(limiter.Limiter_RelayMode == 0xB2)		limiter.Threshold_MDEvent.bit.b0 = 1;			//����,��բ..
//					else 	limiter.Threshold_MDEvent.bit.b4 = 1;			//��բ..
//				}	
	   
	  eventFlg_Ptr = &limiter.Threshold_MDEvent;

	//----------------------------------------------------------------------------
	if( eventFlg_Ptr->bit.b4==1 ){     //��������¼�����(��բ)..
			//		//�����ж�..
		if((*Ptr_num_times)<Threshod_num_times)  //δ��������
			{			   	
			  	if(*Ptr_time==Threshod_time){
						 (*Ptr_time) = 0;										 
						 (*Ptr_num_times)++;		//����..				  	
		        localControl_EventEnd( );
		        eventFlg_Ptr->bit.b4 = 0;
		      }
		 }else{		//�����������..
		 					if(limiter.Limiter_RelayMode != 0xB2){
		 						 if(*Ptr_time==Threshod_time){
									 (*Ptr_time) = 0;										 
									 (*Ptr_num_times)++;		//����..				  	
					         localControl_EventEnd( );
					         eventFlg_Ptr->bit.b4 = 0;
					       }		 						
		 					}	
					}
	}
	else if( eventFlg_Ptr->bit.b0==1 ){//�����ʼ�¼�����(��բ)..
				//		//�����ж�..
		if((*Ptr_num_times)<Threshod_num_times)  //δ��������
			{	
					*Ptr_time = 0;   
		      localControl_EventStart();
		      eventFlg_Ptr->bit.b0 = 0;
      }else{		//������
      		 if(limiter.Limiter_RelayMode == 0xB2){  //����ģʽ--��բ..
      		 		*Ptr_time = 0;   
				      localControl_EventStart();
				      eventFlg_Ptr->bit.b0 = 0;      		 	
      		 }
      }
	}		
}


void localControl_EventStart( void )
{
//	unsigned char Buff[15];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;

		limiter.LAST_OVER_Event.byte = 0x11;
		
		//limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;

		//������բ..
		if( Para.RMeterControlMode > 0 )
		{
		/*
			if(( Para.RMeterTripState == 0x1A )&&( Para.RMeterRelayState == 0x1A )) return; 	//Զ����բ������բ��������ɺ��ԡ�
			else if( Para.RMeterRelayState != 0x1A )
			{
				ICComm.RelayOFFState = 0x55;									//׼����բ
				Para.RMeterTripState = 0x1A;				
				// E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				
				SM.DisconnectorControlCode = Limiter_threshold_exceed;
				//else 									
				LoadRecord( Disconnector_Control_No );
				OperateRelay();
				SM.Event_all_flag |= Disconnector_Control_BIT;
				SM.Error_all_flag |= Disconnector_Control_BIT;
			
				ICComm.RelayONState = 0xAA;	
			
				E2P_RData( &Para.RMeterTripState,MeterTripState,  1 );		
				if(Para.RMeterTripState != 0x1B){
						Para.RMeterTripState = 0x1B;
						E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				 }				
			}		
			*/			
			Para.RMeterTripState = Route_G; 
		}
	
}

void localControl_EventEnd( void )
{
//	unsigned char Buff[15];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;
	
/*		//���Ӵ�������..
	if(limiter.Limiter_RelayMode == 0xB2)		//����ģʽ..
		{
				if( limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt<limiter.Ram_Auto_Reclosing_relay_Num_emergency ){
						//limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt++;				      
				 }else{
								//if(limiter.Ram_Auto_Reclosing_relay_Num_cnt)
								//��Ҫ������բ״̬����բ..
								return;
					 	  }
		}else{		//��ͨģʽ..
						if( limiter.Ram_Auto_Reclosing_relay_Num_cnt<limiter.Ram_Reconnection_timeout_Normal ){
									//limiter.Ram_Auto_Reclosing_relay_Num_cnt++;
									_NOP();				      
				 		}else{
								//if(limiter.Ram_Auto_Reclosing_relay_Num_cnt)
								//��������Ҫһֱ���ں�բ״̬���ȴ���վ����բ����..								
								// return;
					 	  }
					}
	*/
	
	limiter.LAST_OVER_Event.byte = 0x22;
	if( Para.RMeterControlMode > 4 )
		Para.RMeterTripState = Route_H;

/*				
//	if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))
	if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 ))
		{
			if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//�����բ
		}
		else if(( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x04 )||( Para.RMeterControlMode == 0x06 ))
		{
//			if(( Para.RMeterRelayState == 0x1A )&&(Para.RMeterTripState != 0x1A)) 
		if( Para.RMeterRelayState == 0x1A ) 
			{
				ICComm.RelayONState = 0x55;	//������բ 			
				SM.DisconnectorControlCode = Limiter_threshold_OK;
				LoadRecord( Disconnector_Control_No );
				OperateRelay();
				SM.Event_all_flag |= Disconnector_Control_BIT;
				SM.Error_all_flag |= Disconnector_Control_BIT;
			}	
		}	
		*/
//		Para.RMeterTripState = 0x1B;
//		E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
//			E2P_RData( &Para.RMeterTripState,MeterTripState,  1 );		
//			if(Para.RMeterTripState != 0x1B){
//					Para.RMeterTripState = 0x1B;
//					E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
//			}								
}
void Relay_Ctr(void)
{
		if(Para.RMeterControlMode==1)
			Para.RMeterTripState&=0x7e; //bcfgde
		else if(Para.RMeterControlMode==2)
			Para.RMeterTripState&=0xee; //bcfgae
		else if(Para.RMeterControlMode==3)
			Para.RMeterTripState&=0x7a; //bcgde
		else if(Para.RMeterControlMode==4)
			Para.RMeterTripState&=0xea; //bcgae
		else if(Para.RMeterControlMode==5)
			Para.RMeterTripState&=0x7f; //bcfgdeh
		else if(Para.RMeterControlMode==6)
			Para.RMeterTripState&=0x7b; //bcgdeh
		else{
				if(Para.RMeterRelayState!=1)
				{
				RelayDrive( RELAY_ON );//�̵�����բ
				Para.RMeterRelayState=1;
				E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
				 SM.DisconnectorControlCode = Local_reconnection;
				LoadRecord( Disconnector_Control_No );
				}
			}
	if(Para.RMeterControlMode==0) return;
	if(Para.RMeterRelayState==0)//Disconected
	{
		if(Para.RMeterTripState&Route_A)
		{
		RelayDrive( RELAY_ON );//�̵�����բ
		Para.RMeterRelayState=1;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Remote_connection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_D)
		{	
		Para.RMeterRelayState=2;
		SM.DisconnectorControlCode = Remote_connection;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		}
	}
	else if(Para.RMeterRelayState==1)//Connected
	{
		if(Para.RMeterTripState&Route_B)
		{
		RelayDrive( RELAY_OFF );//�̵�����բ
		Para.RMeterRelayState=0;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Remote_disconnection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_F)
		{
		RelayDrive( RELAY_OFF );//�̵�����բ
		Para.RMeterRelayState=2;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Manual_disconnection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_G)
		{
		RelayDrive( RELAY_OFF );//�̵�����բ
		Para.RMeterRelayState=2;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Local_disconnection;
		LoadRecord( Disconnector_Control_No );
		}
	}
	else if(Para.RMeterRelayState==2)//Ready for reconnection
	{
		if(Para.RMeterTripState&Route_C)
		{
		Para.RMeterRelayState=0;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		}
		else if(Para.RMeterTripState&Route_E)
		{
		RelayDrive( RELAY_ON );//�̵�����բ
		Para.RMeterRelayState=1;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Manual_connection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_H)
		{
		RelayDrive( RELAY_ON );//�̵�����բ
		Para.RMeterRelayState=1;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Local_reconnection;
		LoadRecord( Disconnector_Control_No );
		}
	}
	Para.RMeterTripState=0;

}

