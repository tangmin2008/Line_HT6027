#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
//#include "In430.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Port.h"
#include "TypeFRAM.h"
#include "RsComm.h"
#include "Data.h"
#include "interrupt.h"
#include "Mem.h"
#include "Measure.h"
#include "General.h" 
#include "Time.h"
#include "Power.h"
#include "Display.h"
#include "FreezeData.h"
//#include "Load.h"
#include "cpucard.h"
#include "RemoteComm.h"
#include "YearSave.h"									//�¹���	//13.11.29

#if ( LocalSmartMeter == YesCheck )
#else
void GetAndCompareESAMPara( void )
{
	unsigned char Buff[50];
	unsigned char Buff2[10];
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned long Value1;
	unsigned long Value2;
	unsigned short Len;
	unsigned short Length;
    
	Ptr = Buff;
	Point = Buff2;

	GetCurSecretKeyState();
	
//	if( ReadBinary( ESAM, Ptr, FILE_No2, 0x04, 38 ) == 0 ) 
	if( ReadBinary( ESAM, Ptr, FILE_No4, 0x04, 38 ) == 0 ) 
	{	
//		E2P_RData( Point, UserType, 1 );						//�û�����
//		if( *Ptr != *Point ) E2P_WData( UserType, Ptr, 1 );		
		
//		SwapData( Ptr+6, 5 );
//		E2P_RData( Point, FeeSwitchTime, 5 );					//��ʱ�����л�ʱ��
//		if( Data_Comp( Ptr+6, Point, 5 ) != 0 )
//		{
//			E2P_WData( FeeSwitchTime, Ptr+6, 5 );					
//		}	
		
//		SwapData( Ptr+20, 3 );
//		E2P_RData( Point, CurrentRate, 3 );						
//		if( Data_Comp( Ptr+20, Point, 3 ) != 0 )
//		{
//			E2P_WData( CurrentRate, Ptr+20, 3 );					
//		}	

		SwapData( Ptr+20, 3 );
		Value1 = BCD3_Long( Ptr+20 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, CurrentRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( CurrentRate, (unsigned char*)&Value1, 4 );					
		}	

//		SwapData( Ptr+23, 3 );
//		E2P_RData( Point, VoltRate, 3 );						
//		if( Data_Comp( Ptr+23, Point, 3 ) != 0 )
//		{
//			E2P_WData( VoltRate, Ptr+23, 3 );					
//		}	

		SwapData( Ptr+23, 3 );
		Value1 = BCD3_Long( Ptr+23 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, VoltRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( VoltRate, (unsigned char*)&Value1, 4 );					
		}	
	
//		SwapData( Ptr+26, 6 );
//		E2P_RData( Point, EUser_No, 6 );						
//		if( Data_Comp( Ptr+26, Point, 6 ) != 0 )
//		{
//			E2P_WData( EUser_No, Ptr+26, 6 );					
//		}	
	
//		SwapData( Ptr+32, 6 );
//		E2P_RData( Point, Client_No, 6 );						
//		if( Data_Comp( Ptr+32, Point, 6 ) != 0 )
//		{
//			E2P_WData( Client_No, Ptr+32, 6 );					
//		}	
	}
	
//	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x05, 8 ) == 0 ) 		
//	{
//		SwapData( Ptr, 8 );
//		E2P_RData( Point, EMeter_No, 8 );						
//		if( Data_Comp( Ptr, Point, 8 ) != 0 )
//		{
//			E2P_WData( EMeter_No, Ptr, 8 );					
//		}	
//	}	

	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x04, 9 ) == 0 ) 	//��ȡ1�ֽڳ����ֽ�
	{
		Len = *Ptr;
		if( Len < 8 ) Length = 8 - Len;
		else 
		{
			Length = 0;
			Len = 8;
		}	
		SwapData( Ptr+1, Len );
		RAM_Fill( Ptr+1+Len, Length );
		
		E2P_RData( Point, EMeter_No, 8 );						
		if( Data_Comp( Ptr+1, Point, 8 ) != 0 )
		{
			E2P_WData( EMeter_No, Ptr+1, 8 );					
		}	
	}	
	
	if( ReadRecord( ESAM, Ptr ) == 0 ) 					//����4�ֽ�ʣ���4�ֽڹ��������6�ֽڿͻ����	
	{
		SwapData( Ptr+8, 6 );
		E2P_RData( Point, Client_No, 6 );						
		if( Data_Comp( Ptr+8, Point, 6 ) != 0 )
		{
			E2P_WData( Client_No, Ptr+8, 6 );					
		}	
	}		
}

short Card_CoolRST( short CARD_TYPE )
{

	return 0;
}

void Card_Release( short CARD_TYPE )
{
	return;
}
#endif

#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else																		//�¹���	//13.11.06

#if ( MagnKeepRelay	== YesCheck )
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	ICComm.RelayDrivePulseWidth = 5;
	
	if( DriveType == RELAY_ON ) 
	{
//		POUT_JDQA &= ~P_JDQA;  
		HT_GPIOD->PTCLR |= GPIOD_RLYON;//POUT_JDQA &= ~P_JDQA;  			//11.03.29
		Delay(30);						//11.05.06
//		POUT_JDQB |= P_JDQB;  
		HT_GPIOD->PTSET |= GPIOD_RLYOFF;//POUT_JDQB |= P_JDQB;  
	}	
	if( DriveType == RELAY_OFF ) 
	{
//		POUT_JDQB &= ~P_JDQB;  			//11.05.06
		HT_GPIOD->PTCLR |= GPIOD_RLYOFF;//POUT_JDQB &= ~P_JDQB;  
		Delay(30);						//11.05.06
//		POUT_JDQA |= P_JDQA;  
		HT_GPIOD->PTSET |= GPIOD_RLYON;//POUT_JDQA |= P_JDQA;  			//11.03.29
//		POUT_JDQB &= ~P_JDQB;  
		HT_GPIOD->PTCLR |= GPIOD_RLYOFF;//POUT_JDQB &= ~P_JDQB;  
	}			
}	
#else
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	if(( Para.RMeterRunMode1 & 0x01 ) != 0 ) ICComm.RelayDrivePulseWidth = SM.RRelayPulseWide;		//����3S
	else ICComm.RelayDrivePulseWidth = 0;
	
	if( DriveType == RELAY_ON ) 
	{
#if ( NewHardScheme2 == YesCheck )		//10.12.25
//		POUT_JDQA &= ~P_JDQA;  			//11.03.29
		HT_GPIOD->PTCLR |= GPIOD_RLYON;//POUT_JDQA &= ~P_JDQA;  			//11.03.29
#else
//		PDIR_JDQA &= ~P_JDQA;  
		HT_GPIOD->PTDIR &= ~GPIOD_RLYON;//PDIR_JDQA &= ~P_JDQA;  
#endif
	}	
	if( DriveType == RELAY_OFF ) 
	{
#if ( NewHardScheme2 == YesCheck )		//10.12.25
//		POUT_JDQA |= P_JDQA;  			//11.03.29
		HT_GPIOD->PTSET |= GPIOD_RLYON;//POUT_JDQA |= P_JDQA;  			//11.03.29
#else
//		PDIR_JDQA |= P_JDQA;  
		HT_GPIOD->PTDIR |= GPIOD_RLYON;//PDIR_JDQA |= P_JDQA;  
#endif
	}			
}	
#endif

//ÿ����֮һ��ִ��һ��
void JudgeSTA_State( void )					//�¹���			
{
#if ( CarryCommWithOC == YesCheck )			//10.11.23
//�Ϸ���ʹ�ù�����ز���̬Ϊ�ͣ�����Ƭ���˳�̬Ϊ��																			//10.11.23
//	if(( PIN_STAZB & P_STAZB ) != 0 ) SM.CarrySTADelay = 0;		//����ִ��	
	if(( HT_GPIOA->PTDAT & GPIOA_PLC_STA ) != 0 ) SM.CarrySTADelay = 0;		//����ִ��
	else 
	{
		if( SM.CarrySTADelay < 7 )	SM.CarrySTADelay++;			//������ִ�����2��		//�¹���	//13.10.25
	}	 	
#else
//�·���ȡ��������������л���ƽ���߼���ƽ�������ز���̬Ϊ�ͣ���Ƭ���˳�̬ҲΪ��											//10.11.23
//	if(( PIN_STAZB & P_STAZB ) == 0 ) SM.CarrySTADelay = 0;		//����ִ��
	if(( HT_GPIOA->PTDAT & GPIOA_PLC_STA ) == 0 ) SM.CarrySTADelay = 0;		//����ִ��
	else 
	{
		if( SM.CarrySTADelay < 7 )	SM.CarrySTADelay++;			//������ִ�����2��		//�¹���	//13.10.25
	}	 	
#endif	
}

#if ( LocalSmartMeter == YesCheck )
#else
void OperateRelay( void )
{
	if( Para.RMeterSafeState == 0x3A )									//����
	{
#if ( RelayONImmediately == YesCheck )
		if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0x55;	//������բ
#else
		if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0xAA;	//�����բ
#endif
	}	
	if(( ICComm.TripOFFDelay == 0 )&&( Para.RMeterTripState == 0x1A ))	//Զ����բ
	{			
		if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//׼����բ																											
		ICComm.RelayONState = 0x0;
	}	

	if(( Para.RMeterRelayState == 0x1A )&&( Para.RMeterTripState == 0x1B ))
	{
#if ( RelayONImmediately == YesCheck )
		if( ICComm.RelayONState	== 0 ) ICComm.RelayONState = 0x55;		//������բ(���ܸ��ϵ�)
#else
		if( ICComm.RelayONState	== 0 ) ICComm.RelayONState = 0xAA;		//�����բ(���ܸ��ϵ�)
#endif		
	}	

#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//ִ�к�բ			//10.11.23	//�¹��� 	/13.10.25
#else
	if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//ִ�к�բ		
#endif
	{
			RecordStartEvent( TripON_No );
			ICComm.RelayDriveCnt = 5;	
			RelayDrive( RELAY_ON );	
			ICComm.RelayONState = 0x0;
			ICComm.RelayOFFState = 0x00;			//��բ��ȡ��������բ״̬	//�¹���	//13.10.25
			Para.RMeterRelayState = 0x55;			//�Ѿ���բ
			E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//			CreateTripRec( TripON_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			EVar.RelayFaultVar.Cunt = 1;														//�¹���	//14.05.03	
			EVar.RelayFaultVar.OldState = 0xff;													//�¹���	//14.05.03
			E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//�¹���	//14.05.12	
			Flag.RSwitchOldState &= 0x04;														//�¹���	//14.05.03	
			Flag.RSwitchOldState |= 0x40;			//�����¼�쳣�¼�							//�¹���	//14.05.12				
			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
#endif
			RecordEndEvent( TripON_No, 0 );
	}				

#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//ִ����բ		//10.11.23	//�¹���	//13.10.25
#else
	if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//ִ����բ		
#endif
	{
#if ( IOver30ALimitTrip	== YesCheck )		//�¹���	//14.04.25
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCount >= Para.RUp30AClose24HOUR ))			//�¹���	//14.06.19
#else
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCnt == UP30A24HOUR ))
#endif
		{
#endif
			RecordStartEvent( TripOFF_No );
			ICComm.RelayDriveCnt = 5;	
			RelayDrive( RELAY_OFF );	
			ICComm.RelayOFFState = 0x0;
			ICComm.RelayONState = 0x00;				//��բ��ȡ�������բ״̬	//�¹���	//13.10.25
			Para.RMeterRelayState = 0x1A;			//�Ѿ���բ
			E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//			CreateTripRec( TripOFF_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			EVar.RelayFaultVar.Cunt = 1;														//�¹���	//14.05.03
			EVar.RelayFaultVar.OldState = 0xff;													//�¹���	//14.05.03	
			E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//�¹���	//14.05.12	
			Flag.RSwitchOldState &= 0x04;														//�¹���	//14.05.03	
			Flag.RSwitchOldState |= 0x40;			//�����¼�쳣�¼�							//�¹���	//14.05.12							
			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
#endif
			RecordEndEvent( TripOFF_No, 0 );
#if ( IOver30ALimitTrip	== YesCheck )		//11.06.29
		}
#endif
	}				
}		
#endif
#endif


#if ( LocalSmartMeter == YesCheck )

void CYCDelay(unsigned short cycle) /*����cycleΪϣ������������1/6��*/ 
{
    unsigned short t;
    t = cycle;
    while(t > 1)
    {
        t--;
    }
}
/*
void CommInitIC( short CARD_TYPE )
{
	if( CARD_TYPE == PSAM )
	{
		ICComm.ICBuf1[0] = 0xff;
		ICComm.ICPtr1 = 0;
		ICComm.ICBTime1 = 0;					// �ַ��䶨ʱ
		ICComm.ICSendLength1 = 0;				// IC��1������շ����ֽ���
		ICComm.Run &= ~F_ICComOK1;
   		ICRxInitial1();
   	}
   	else
   	{
		SM.ESAMByteCnt = 0;						//10.11.13
		ICComm.ICBuf2[0] = 0xff;
		ICComm.ICPtr2 = 0;
		ICComm.ICBTime2 = 0;					// �ַ��䶨ʱ
		ICComm.ICSendLength2 = 0;	        // IC��2������շ����ֽ���
		ICComm.Run &= ~F_ICComOK2;
   		ICRxInitial2();	
   	}			
}
*/
ISO7816_InitTypeDef ISO7816_InitStruct;
#if ( ISO7816Config	== YesCheck )
void ICRxInitial1( void )
{
	/***** ���´�����������ISO7816ͨѶ*****/
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);    /*!< ʹ��UART37816ENģ��              */
			
	ISO7816_InitStruct.ISO7816_Parity = ISO7816Parity_EVEN;		//ISO7816��żУ��λѡ��   
//    ISO7816_InitStruct.ISO7816_Parity = ISO7816Parity_ODD;		//ISO7816��żУ��λѡ��   
	ISO7816_InitStruct.ISO7816_RepeatTime = Repeat_Three;       //ISO7816�ط����մ����趨 
	ISO7816_InitStruct.AutoRxEN = ENABLE;                 		//ISO7816�Զ�����ʹ�ܿ��� 
	ISO7816_InitStruct.AutoTxEN = ENABLE;                 		//ISO7816�Զ�����ʹ�ܿ��� 
	ISO7816_InitStruct.ISO7816_AckLen = ACKLen_1Bit;            //ISO7816��Ӧλ�����趨   
//    ISO7816_InitStruct.ISO7816_AckLen = ACKLen_1P5Bit;            //ISO7816��Ӧλ�����趨   
//    ISO7816_InitStruct.ISO7816_AckLen = ACKLen_2Bit;            //ISO7816��Ӧλ�����趨   
//	ISO7816_InitStruct.ISO7816_BaudRate = 11275;         		//ISO7816�������趨     10753(4.000M)��11275(4.096M)  
	ISO7816_InitStruct.ISO7816_BaudRate = 10753;         		//ISO7816�������趨     10753(4.000M)��11275(4.096M)  

	HT_ISO7816_Init( HT_ISO7816_1, &ISO7816_InitStruct );
	HT_ISO7816_ITConfig(HT_ISO7816_1, UART_ISO7816CON_RXIE, ENABLE );
}	

void ICTxInitial1( void )
{
	HT_ISO7816_1->ISO7816STA &= ~(UART_ISO7816STA_RXIF|UART_ISO7816STA_TXIF);
  	HT_ISO7816_1->ISO7816CON |= UART_ISO7816CON_TXIE;     		
    HT_ISO7816_1->SBUF = ICComm.ICBuf1[0];     		
}	
#else				//��UART��ʽͨѶ
void ICRxInitial1( void )
{
    UART_InitTypeDef UART_InitStructure; 
		    /***** ���´�����������UART3ͨѶ*****/
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);        /*!< ʹ��UART3ģ��              */
				
	/*!< UART1������Ϣ               */
	UART_InitStructure.UART_Logic = UartLogicPositive;          /*!< UART�߼�Ϊ��               */
//	UART_InitStructure.UART_StopBits = OneStopBits;             /*!< 1λֹͣλ                  */
	UART_InitStructure.UART_StopBits = TwoStopBits;             /*!< 1λֹͣλ                  */
	UART_InitStructure.UART_WordLength = WordLength_8Bits;      /*!< 8λ����λ                  */
	UART_InitStructure.UART_Parity = UartParity_EVEN;        	/*!< żУ��                     */
	UART_InitStructure.UART_BaudRate = 10753;          			/*!< ������2400                 */
	UART_InitStructure.ReceiveEN = ENABLE;                      /*!< ����ʹ��   ע�����ж�ʹ��  */
	UART_InitStructure.SendEN = DISABLE;                         /*!< ����ʹ��   ע�����ж�ʹ��  */		
	
	HT_UART_Init(HT_UART3, &UART_InitStructure);	
	HT_UART_ITConfig(HT_UART3, UART_UARTCON_RXIE, ENABLE);
}	

void ICTxInitial1( void )
{
	HT_UART_ITConfig(HT_UART3, UART_UARTCON_RXIE, DISABLE);
	HT_UART3->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);
	HT_UART3->UARTCON |= UART_UARTCON_TXIE + UART_UARTCON_TXEN;
    HT_UART3->SBUF = ICComm.ICBuf1[0];     		
}	

#endif

void CommInitIC( short CARD_TYPE )
{
	if( CARD_TYPE == PSAM )
	{
		ICComm.ICBuf1[0] = 0xff;
		ICComm.ICPtr1 = 0;
		ICComm.ICBTime1 = 0;					// �ַ��䶨ʱ
		ICComm.ICSendLength1 = 0;				// IC��1������շ����ֽ���
		ICComm.Run &= ~F_ICComOK1;
   		ICRxInitial1();
   	}
//   	else
//   	{
//		SM.ESAMByteCnt = 0;						//10.11.13
//		ICComm.ICBuf2[0] = 0xff;
//		ICComm.ICPtr2 = 0;
//		ICComm.ICBTime2 = 0;					// �ַ��䶨ʱ
//		ICComm.ICSendLength2 = 0;	        // IC��2������շ����ֽ���
//		ICComm.Run &= ~F_ICComOK2;
//   		ICRxInitial2();	
//   	}			
}

short RecDataDelay( short CARD_TYPE, unsigned long Cycle, unsigned short RecLength )
{
	unsigned long i;
	unsigned short ICComOK;
	unsigned short* ICBTime;				//V6
	unsigned short* RecLen;				//V6
	
	if( CARD_TYPE == PSAM ) 
	{
		ICComOK = F_ICComOK1;
		ICBTime = &ICComm.ICBTime1;		//V6
		RecLen = &ICComm.ICRecLength1;
	}	
//	else 
//	{
//		ICComOK = F_ICComOK2;
//		ICBTime = &ICComm.ICBTime2;		//V6
//		RecLen = &ICComm.ICRecLength2;
//	}	
	
	*RecLen = RecLength;
	
	i=Cycle;
	while( i>0)
	{
// 		WDTCTL = WDT_ARST_1000;
		HT_FreeDog();
		if(( ICComm.Run & ICComOK ) != 0 ) break;	
		i--;
	}

	*RecLen = 0;

	if( i == Cycle ) return -1;
	if( i == 0 ) *ICBTime = 0;			//V6
	return 0;	
}	

/*
short Card_CoolRST( short CARD_TYPE )
{
	unsigned short* ComPtr;
	unsigned char* ComBuf;
	short RST_Port;
	
	WDTCTL = WDT_ARST_1000;
	if( CARD_TYPE == PSAM )
	{
		PDIR_CARDPW |= P_CARDPW;
		RST_Port = P_CARDRST;
		ComPtr = &ICComm.ICPtr1;
		ComBuf = ICComm.ICBuf1;		
	}	
	else
	{
#if( ESAMPowerControl == YesCheck )			//10.12.02			
		POUT_ESAMPW &= ~P_ESAMPW;			//��ESAM��Դ
		CYCDelay(6500*4);					//��Դ�ȶ�ʱ��
#endif
		RST_Port = P_ESAMRST;
		ComPtr = &ICComm.ICPtr2;
		ComBuf = ICComm.ICBuf2;		
	}	

	PDIR_ESAMCLK |= P_ESAMCLK;
	PSEL_ESAMCLK |= P_ESAMCLK;
	CYCDelay(6500*4);						//>= 400/f	//Լ6.5ms
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	WDTCTL = WDT_ARST_1000;
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	WDTCTL = WDT_ARST_1000;
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	WDTCTL = WDT_ARST_1000;

#if ( CardIOTwoLine == YesCheck )		     
	if( CARD_TYPE == PSAM )					//PIO	
	{										//PIO	
		P4OUT &= ~RST_Port;					//PIO	
		P4DIR |= RST_Port;					//PIO	
		CYCDelay(650*4);					//PIO		//>= 400/f
		P4DIR &= ~RST_Port;					//PIO	
	}										//PIO	
	else									//PIO	
	{										//PIO	
		P1OUT &= ~RST_Port;
		P1DIR |= RST_Port;
		CYCDelay(650*4);								//>= 400/f
		P1DIR &= ~RST_Port;		
	}										//PIO	
#else
	P1OUT &= ~RST_Port;
	P1DIR |= RST_Port;
	CYCDelay(650*4);						//>= 400/f
	P1DIR &= ~RST_Port;
#endif

	CommInitIC( CARD_TYPE );
	RecDataDelay( CARD_TYPE, 65000*4, 13 );	
	if(( *ComPtr < 13 )||( *ComBuf != 0x3B)||( *(ComBuf+1) != 0x69 )) 
	{
		if( CARD_TYPE == ESAM )																//�¹���
		{																					//�¹���
			if(( Flag.MeterState1 & F_ESAMErr ) == 0 ) UpdateEventUpState( 1 );				//�¹���	//13.10.08
		}																					//�¹���		
		return -1; 
	}	
	if( CARD_TYPE == PSAM ) RAM_Write( Para.CardNo, ComBuf+5, 8 );
	else 
	{
		RAM_Write( Para.ECardNo, ComBuf+4, 9 );
		SwapData( Para.ECardNo, 9 );
		E2P_WData( ESAMCardNo, Para.ECardNo, 8 );		
	}	
	return 0;
}	
*/

short Card_CoolRST( short CARD_TYPE )
{
	unsigned short* ComPtr;
	unsigned char* ComBuf;
	
	HT_FreeDog();
	if( CARD_TYPE == PSAM )
	{
		VCARD_CLR;
		ComPtr = &ICComm.ICPtr1;
		ComBuf = ICComm.ICBuf1;		
	}	
	else
	{
#if( ESAMPowerControl == YesCheck )			//10.12.02			
		VESAM_CLR;							//��ESAM��Դ
		CYCDelay(6500*4);					//��Դ�ȶ�ʱ��
#endif
	}	

	CYCDelay(6500*4);						//>= 400/f	//Լ6.5ms
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	HT_FreeDog();
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	HT_FreeDog();
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	HT_FreeDog();

	if( CARD_TYPE == PSAM )					//PIO	
	{										//PIO	
		CARDRST_CLR;
		CYCDelay(650*4);					//PIO		//>= 400/f
		CARDRST_SET;
	}										//PIO	

	if( CARD_TYPE == PSAM )
	{
		CommInitIC( CARD_TYPE );
		RecDataDelay( CARD_TYPE, 65000*4, 13 );	
		if(( *ComPtr < 13 )||( *ComBuf != 0x3B)||( *(ComBuf+1) != 0x69 )) 
		{
			return -1; 
		}
		RAM_Write( Para.CardNo, ComBuf+5, 8 );			
	}
	else
	{
		if( GetESAMSaftInfo( Para.ECardNo, 0x02 ) != -1 )
		{
			SwapData( Para.ECardNo, 8 );
			E2P_WData( ESAMCardNo, Para.ECardNo, 8 );			
		}	
		else
		{
			if(( Flag.MeterState1 & F_ESAMErr ) == 0 ) UpdateEventUpState( 1 );				//�¹���	//13.10.08	
			return -1;
		}	
	}			
	return 0;
}	

/*	
void Card_Release( short CARD_TYPE )
{
	if( CARD_TYPE == ESAM )
	{	
		PDIR_ESAMRST |= P_ESAMRST;
		CYCDelay(6500);						//>= 400/f
		PSEL_ESAMCLK &= ~P_ESAMCLK;
		PDIR_ESAMCLK &= ~P_ESAMCLK;
#if( ESAMPowerControl == YesCheck )			//10.12.02			
		TA0CCTL2 = 0;
		PDIR_ESAMIO &= ~P_ESAMIO; 
		PSEL_ESAMIO &= ~P_ESAMIO; 

		POUT_ESAMIO &= ~P_ESAMIO; 
		PDIR_ESAMIO |= P_ESAMIO; 

		POUT_ESAMCLK &= ~P_ESAMCLK;
		PDIR_ESAMCLK |= P_ESAMCLK;
		CYCDelay(1000);						//>= 400/f
		POUT_ESAMPW |= P_ESAMPW;			//�ر�ESAM��Դ
#endif
	}
	else
	{
		PDIR_CARDRST |= P_CARDRST;					
		CYCDelay(6500);						//>= 400/f
		PDIR_CARDPW &= ~P_CARDPW;
	}			
}
*/

void Card_Release( short CARD_TYPE )
{
	if( CARD_TYPE == ESAM )
	{	
#if( ESAMPowerControl == YesCheck )			//10.12.02			
//		ESAMCS_SET;
//		CYCDelay(1000);						//>= 400/f
//		ESAMCLK_CLR;
//		CYCDelay(1000);						//>= 400/f
//		ESAMMOSI_CLR;
//		CYCDelay(1000);						//>= 400/f
//		VESAM_SET;
//		ESAMCS_CLR;
#endif
	}
	else
	{
		CARDRST_CLR;
		CYCDelay(6500);						//>= 400/f
//		PDIR_CARDPW &= ~P_CARDPW;
		VCARD_SET;
	}			
}

//����2�ֽڴ�����Ӧ״̬
void SaveErrorResponse( short CARD_TYPE )							//�¹���	//13.12.02
{
	if( CARD_TYPE == PSAM )
	{
		if( ICComm.ICPtr1 != 0 ) 
		{
			RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf1, 2 );			
			if(( ICComm.ErrorResponse[0] == 0x6B )&&( ICComm.ErrorResponse[1] == 0x00 )) ICComm.ErrorMessage = ICERROR_07;
			else ICComm.ErrorMessage = ICERROR_19;
		}else ICComm.ErrorMessage = ICERROR_19;					//�¹���	//14.01.06	
	}
	else
	{
//		if( ICComm.ICPtr2 != 0 ) RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf2, 2 );
		ICComm.ErrorMessage = ICERROR_02;						//����ESAM��			
	}			
}	 

//����5�ֽڲ�������ͷ
void SaveOperateCommHead( short CARD_TYPE )							//�¹���	//13.12.02
{
	RAM_Fill( ICComm.ErrorResponse, 2 ); 
	if( CARD_TYPE == PSAM )
	{
		RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf1, 5 );			
	}
//	else RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf2, 5 );			
}	 

//���5�ֽڲ�������ͷ
void ClearOperateCommHead_ErrorResponse( void )							//�¹���	//13.12.02
{
	RAM_Fill( ICComm.OperateCommHead, 5 );
	RAM_Fill( ICComm.ErrorResponse, 2 ); 
}

void ICDataSend( short CARD_TYPE, short Length, unsigned long Cycle, unsigned short RecLength )
{
	if( RecLength == 1 ) SaveOperateCommHead( CARD_TYPE );			//�ȴ��������ݳ���Ϊ1ʱ����Ϊ����֡		//�¹���	//13.12.02
	
	if( CARD_TYPE == PSAM )
	{
		ICComm.ICPtr1 = 0;
		ICComm.ICSendLength1= Length;
		ICComm.Run &= ~F_ICComOK1; 
		ICComm.ICRxTx_Data1 = ICComm.ICBuf1[0];
//		TA0CCR3 = TA0R;
		ICTxInitial1();
	}
//	else
//	{
//		ICComm.ICPtr2 = 0;
//		ICComm.ICSendLength2= Length;
//		ICComm.Run &= ~F_ICComOK2; 
//		ICComm.ICRxTx_Data2 = ICComm.ICBuf2[0];
//		TA0CCR2 = TA0R;
//		ICTxInitial2();
//	}		
	RecDataDelay( CARD_TYPE, Cycle, RecLength );	
}		

/*
//(����ȶԼ�����)
short CompareEncrypt( unsigned char* Dest, unsigned char* Source, unsigned short Length )		//�¹���
{
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;						//��д����
	
		ICComm.ICBuf2[0] = 0x80;						//����Կ��ɢ��֤	
		ICComm.ICBuf2[1] = 0xFA;
		ICComm.ICBuf2[2] = 0x01;
		ICComm.ICBuf2[3] = 0x00;
		ICComm.ICBuf2[4] = Length;
	
		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( ICComm.ICBuf2[0] != 0xFA ) { SaveErrorResponse( ESAM ); continue; }				//����2�ֽڴ�����Ӧ״̬ //�¹���	//13.12.02
	
		RAM_Write( ICComm.ICBuf2, Source, Length );

		ICDataSend( ESAM, Length-1, CYC60000, 2 );			//V6
		
		if(( ICComm.ICBuf2[0] == 0x61 )&&( ICComm.ICBuf2[1] == Length )) 
		{
			if( GetResponse( ESAM, ICComm.ICBuf2, Length ) != 0 ) return -1;
			RAM_Write( Dest, ICComm.ICBuf2, Length );	
			ICComm.ICAlarmFlg = 0x00;
			return 0;	
		}
		else SaveErrorResponse( ESAM );					//����2�ֽڴ�����Ӧ״̬ //�¹���	//13.12.02
	}	
	return -1;
		
}			
*/

/*
//����Կ��ɢ��֤(����)
//DisperseGene: ��ɢ����
short DisperseAuthenticationCardNo( short FileNo, unsigned char* DisperseGene )
{
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;						//��д����
	
		ICComm.ICBuf2[0] = 0x80;						//����Կ��ɢ��֤	
		ICComm.ICBuf2[1] = 0xFA;
		ICComm.ICBuf2[2] = 0x00;
		ICComm.ICBuf2[3] = FileNo;
		ICComm.ICBuf2[4] = 0x08;
	
		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( ICComm.ICBuf2[0] != 0xFA ) { SaveErrorResponse( ESAM ); continue; }				//����2�ֽڴ�����Ӧ״̬ //�¹���	//13.12.02
	
		RAM_Write( ICComm.ICBuf2, DisperseGene, 8 );

		ICDataSend( ESAM, 7, CYC60000, 2 );			//V6
		
		if((ICComm.ICBuf2[0] != 0x90)||(ICComm.ICBuf2[1] != 0x00)) { ICComm.ICAlarmFlg = 0x10; SaveErrorResponse( ESAM ); return -1;	}//���ŷ�ɢ��֤����		//����2�ֽڴ�����Ӧ״̬ //�¹���	//13.12.02
		ICComm.ICAlarmFlg = 0x0;						//��Կ��֤����
		return 0;	
	}	
	return -1;
		
}			

//����Կ��ɢ��֤(�����)
short DisperseAuthenticationRand( unsigned char* Dest, unsigned char* Source )
{
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;						//��д����
	
		ICComm.ICBuf2[0] = 0x80;						//����Կ��ɢ��֤	
		ICComm.ICBuf2[1] = 0xFA;
		ICComm.ICBuf2[2] = 0x00;
		ICComm.ICBuf2[3] = 0x00;
		ICComm.ICBuf2[4] = 0x08;
	
		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( ICComm.ICBuf2[0] != 0xFA ) { SaveErrorResponse( ESAM ); continue; }				//����2�ֽڴ�����Ӧ״̬ //�¹���	//13.12.02
	
		RAM_Write( ICComm.ICBuf2, Source, 8 );

		ICDataSend( ESAM, 7, CYC60000, 2 );			//V6
		
		if(( ICComm.ICBuf2[0] == 0x61 )&&( ICComm.ICBuf2[1] == 8 )) 
		{
			if( GetResponse( ESAM, ICComm.ICBuf2, 8 ) != 0 ) return -1;
			RAM_Write( Dest, ICComm.ICBuf2, 8 );	
			ICComm.ICAlarmFlg = 0x00;
			return 0;	
		}
		else SaveErrorResponse( ESAM );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}	
	return -1;
		
}			  
*/

//����Կ��ɢ��֤(����)
//DisperseGene: ��ɢ����
short DisperseAuthenticationCardNoRand( short FileNo, unsigned char* Dest, unsigned char* DisperseGene, unsigned char* Rand )
{
	unsigned char* ESAMSendBuff;

	ESAMSendBuff = FBuff.Buff;
	ICComm.ICAlarmFlg = ICALARM_28;						//��д����

	RAM_Write( ESAMSendBuff+6, DisperseGene, 8 );
	RAM_Write( ESAMSendBuff+6+8, Rand, 8 );
	if( ReadWriteESAM( Dest, 0x80, 0x08, 0x0800+FileNo, 0x0010 ) == -1 ) { SaveErrorResponse( ESAM ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}			



//ȡ�����
/*
short GetChallenge( short CARD_TYPE, unsigned char* Dest, short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
	else ICBuf = ICComm.ICBuf2;
	*ICBuf = 0x00;
	*(ICBuf+1) = 0x84;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = 0x00;
	*(ICBuf+4) = Length;

	SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02

	ICDataSend( CARD_TYPE, 4, CYC60000, Length+1 );		//V6	
	if( *ICBuf != 0x84 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	RAM_Write( Dest, ICBuf+1, Length );
	ICComm.ICAlarmFlg = 0x00;
	return 0;
}	
*/

short GetChallenge( short CARD_TYPE, unsigned char* Dest, unsigned short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
	else
	{
		if( ReadWriteESAM( Dest, 0x80, 0x04, Length, 0x0000 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		ICComm.ICAlarmFlg = 0x00;
		return 0;
	}	 
	*ICBuf = 0x00;
	*(ICBuf+1) = 0x84;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = 0x00;
	*(ICBuf+4) = Length;

	SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02

	ICDataSend( CARD_TYPE, 4, CYC60000, Length+1 );		//V6	
	if( *ICBuf != 0x84 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	RAM_Write( Dest, ICBuf+1, Length );
	ICComm.ICAlarmFlg = 0x00;
	return 0;
}	


//ȡ��Ӧ����
short GetResponse( short CARD_TYPE, unsigned char* Dest, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) 
		{
			ICBuf = ICComm.ICBuf1;
		}	
		else 
		{
//			ICBuf = ICComm.ICBuf2;					//��ESAM����
		}	
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xC0;
		*(ICBuf+2) = 0x00;
		*(ICBuf+3) = 0x00;
		*(ICBuf+4) = Length;

		SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02

		ICDataSend( CARD_TYPE, 4, CYC120000, Length+3 );		//V6		
		if(( *ICBuf == 0xC0 )&&((( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))||( *(ICBuf+Length+1) == 0x61 )))
		{
			RAM_Write( Dest, ICBuf+1, Length );
			ICComm.ICAlarmFlg = 0x00;						//��д����
			return 0;
		}	  			//�յ���Ӧ���ݣ�
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;
}	

//�ⲿ��֤
short ExternalAuthentication( short CARD_TYPE, unsigned char* Source, short FileNo, short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_10;						//��Կ��֤����
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//	else ICBuf = ICComm.ICBuf2;							//��ESAM�ⲿ��֤
	*ICBuf = 0x00;
	*(ICBuf+1) = 0x82;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = FileNo;
	*(ICBuf+4) = Length;
	
	ICDataSend( CARD_TYPE, 4, CYC60000, 1 );		//V6
	if( *ICBuf != 0x82 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02

	RAM_Write( ICBuf, Source, Length );

	ICDataSend( CARD_TYPE, Length-1, CYC60000, 2 );		//V6		//12.12.30
	if((*ICBuf != 0x90)||(*(ICBuf+1) != 0x00)) { ICComm.ICAlarmFlg = 0x10; SaveErrorResponse( CARD_TYPE ); return -1;	}//�ⲿ��֤����		//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	ICComm.ICAlarmFlg = 0x0;						//��Կ��֤����
	return 0;	
}	


//��ָ�������ݽ��м��ܻ���ܲ���
//CARD_TYPE: ����������
//Dest : ����������
//Source : ����ǰ����
//OperateType���������� 0�����ܡ�1�����ܡ�2������MAC���� 
//FileNo : ���õĲ�����Կ�ļ���
//Length��������ܻ���ܵ����ݳ���
short EnDecrypt( short CARD_TYPE, unsigned char* Dest, unsigned char* Source, short OperateType ,short FileNo, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) 
		{
			ICBuf = ICComm.ICBuf1;
		}	
		else 
		{
//			ICBuf = ICComm.ICBuf2;					//�޶�ӦESAM����
		}	
		*ICBuf = 0x00;
		*(ICBuf+1) = 0x88;
		*(ICBuf+2) = OperateType;
		*(ICBuf+3) = FileNo;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC60000, 1 );		//V6
		if( *ICBuf != 0x88 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02

		RAM_Write( ICBuf, Source, Length );

		ICDataSend( CARD_TYPE, Length-1, CYC60000, 2 );		//V6		//12.12.30
		if( *ICBuf == 0x61 )
		{
			if( GetResponse( CARD_TYPE, Dest, Length ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}	
	return -1;	
}

/*
//�ڲ���֤	
short InternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char *Ptr;
	
	Ptr = Buff;
	
	if( GetChallenge( ESAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//��ESAMȡ�����	//�¹���	//14.01.06

	if( DisperseAuthenticationCardNo( FILE_No1, DisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//����Կ��ɢ��֤(����)   ESAMϵͳ�����֤��Կ���ļ���ʶ��Ϊ01	//�¹���	//14.01.06
	if( DisperseAuthenticationRand( ICComm.ICBuf2, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; } //����Կ��ɢ��֤(�����)		//�¹���	//14.01.06
	if( EnDecrypt( PSAM, ICComm.ICBuf1, Ptr, ENCRYPT , FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//PSAM DES����	//�¹���	//14.01.06
	if( Data_Comp( ICComm.ICBuf1, ICComm.ICBuf2, 8 ) == 0 ) return 0;	//����������ȣ��ڲ���֤�ɹ�
	else { ICComm.ErrorMessage = ICERROR_05; ClearOperateCommHead_ErrorResponse(); return -1; }		//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02

}
*/
//�ڲ���֤	
short InternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char Buff2[16];
	unsigned char *Ptr;
	unsigned char *Point;
	
	Ptr = Buff;
	Point = Buff2;
	
	if( GetChallenge( ESAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//��ESAMȡ�����	//�¹���	//14.01.06
	if( DisperseAuthenticationCardNoRand( FILE_No1, Point, DisperseGene, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//����Կ��ɢ��֤(����+�����)   ESAMϵͳ�����֤��Կ���ļ���ʶ��Ϊ01	
	if( EnDecrypt( PSAM, ICComm.ICBuf1, Ptr, ENCRYPT , FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//PSAM DES����	//�¹���	//14.01.06
	if( Data_Comp( ICComm.ICBuf1, Point, 8 ) == 0 ) return 0;	//����������ȣ��ڲ���֤�ɹ�
	else { ICComm.ErrorMessage = ICERROR_05; ClearOperateCommHead_ErrorResponse(); return -1; }		//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02

}

/*		//ȡ��
//ESAM�ⲿ��֤
short ESAMExternalAuthentication( short FileNo, short FrameType )
{
	unsigned char Buff[16];
	unsigned char *Ptr;
	short ESAMFileNo;
	
	Ptr = Buff;
	
	if( GetChallenge( ESAM, Ptr, 8 ) != 0 ) return -1;					//��ESAMȡ�����
	if( EnDecrypt( PSAM, Ptr, Ptr, ENCRYPT , FileNo, 8 ) != 0 ) return -1;	//PSAM DES����	
	RAM_Write( Ptr+8, Para.CardNo, 8 );
	if( FrameType == 0x02 ) ESAMFileNo = FILE_No8;					//����ʱ�ο�Ϊ�����ⲿ��֤��Կ
	else ESAMFileNo = FILE_No3;	
	return ExternalAuthentication( ESAM, Ptr, ESAMFileNo, 16 );						//�ⲿ��֤
}
*/

/*
//PSAM�ⲿ��֤(CPU��)
short PSAMExternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char *Ptr;
	
	Ptr = Buff;
	
	if( GetChallenge( PSAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//��PSAMȡ�����	//�¹���	//14.01.06
	if( DisperseAuthenticationCardNo( FILE_No2, DisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//����Կ��ɢ��֤(����)  //ESAM���û�����д��Կ�ļ���ʶ��Ϊ02	//�¹���	//14.01.06
	if( DisperseAuthenticationRand( Ptr, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//����Կ��ɢ��֤(�����)	//�¹���	//14.01.06
	if( ExternalAuthentication( PSAM, Ptr, FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_06; return -1; } 	//�ⲿ��֤	//�¹���	//14.01.06
	return 0;
}		
*/
//PSAM�ⲿ��֤(CPU��)
short PSAMExternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char *Ptr;
	
	Ptr = Buff;
	
	if( GetChallenge( PSAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//��PSAMȡ�����	//�¹���	//14.01.06
	if( DisperseAuthenticationCardNoRand( FILE_No2, Ptr, DisperseGene, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//����Կ��ɢ��֤(����+�����)   ESAM���û�����д��Կ�ļ���ʶ��Ϊ02	
	if( ExternalAuthentication( PSAM, Ptr, FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_06; return -1; } 	//�ⲿ��֤	//�¹���	//14.01.06
	return 0;
}		

short SelectDF( unsigned short DirFile )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		ICBuf = ICComm.ICBuf1;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xA4;
		*(ICBuf+2) = 0x00;
		*(ICBuf+3) = 0x00;
		*(ICBuf+4) = 0x02;

		ICDataSend( PSAM, 4, CYC5000, 1 );				//V6		
		if( *ICBuf != 0xA4 ) { SaveErrorResponse( PSAM ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, (unsigned char*)&DirFile, 2 );

		ICDataSend( PSAM, 1, CYC60000, 2 );			//V6
		if( *ICBuf == 0x61 ) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( PSAM );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}	
	return -1;	
}

/*
short SelectDF_ESAM( unsigned short DirFile )			//�¹���
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xA4;
		*(ICBuf+2) = 0x00;
		*(ICBuf+3) = 0x00;
		*(ICBuf+4) = 0x02;

		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( *ICBuf != 0xA4 ) { SaveErrorResponse( ESAM ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, (unsigned char*)&DirFile, 2 );

		ICDataSend( ESAM, 1, CYC60000, 2 );			//V6
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( ESAM );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}	
	return -1;	
}
*/

/*
//���������ļ�
//CARD_TYPE: ����������
//Dest : ����Ŀ�ĵ�ַ
//FileNo : �������ļ���
//StartAddr�� ��ȡ�������ļ��е���ʼ��ַ
//Length����ȡ���ݳ���
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	short RepeatCnt=4; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) { ICBuf = ICComm.ICBuf1; }//ICPtr = &ICComm.ICPtr1; }
		else { ICBuf = ICComm.ICBuf2; }//ICPtr = &ICComm.ICPtr2; }
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;

		SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02
	
		ICDataSend( CARD_TYPE, 4, CYC60000, Length+3 );			//V6
		if(( *ICBuf == 0xB0 )&&( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))
		{
			RAM_Write( Dest, ICBuf+1, Length );
			ICComm.ICAlarmFlg = 0x0;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;
}
*/

//���������ļ�
//CARD_TYPE: ����������
//Dest : ����Ŀ�ĵ�ַ
//FileNo : �������ļ���
//StartAddr�� ��ȡ�������ļ��е���ʼ��ַ
//Length����ȡ���ݳ���
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	short RepeatCnt=4; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) { ICBuf = ICComm.ICBuf1; }
//		else { ICBuf = ICComm.ICBuf2; }
		else
		{
			ICBuf = FBuff.Buff;
			*(ICBuf+6) = 0x00; 
			ShortToSwapChar( ICBuf+7, StartAddr );
			ShortToSwapChar( ICBuf+7+2, Length );
			if( ReadWriteESAM( Dest, 0x80, 0x2C, FileNo, 0x0005 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;

		SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02
	
		ICDataSend( CARD_TYPE, 4, CYC60000, Length+3 );			//V6
		if(( *ICBuf == 0xB0 )&&( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))
		{
			RAM_Write( Dest, ICBuf+1, Length );
			ICComm.ICAlarmFlg = 0x0;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;
}

/*
//��MAC���������ļ�
//CARD_TYPE: 	������
//SFileType:	Դ�ļ�����
//Dest:			��һ����ȡ�õ������/�������ݵ�Ŀ�ĵ�ַ
//D_FileNo: 	Ŀ���ļ���
//S_FileNo: 	Դ�ļ���
//D_StartAddr: 	Ŀ����ʼ��ַ
//S_StartAddr:  Դ��ʼ��ַ
//Length: 		д���ݳ���
//R_Type: 		�����ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//RLength:		�������ݳ��ȣ�����4�ֽ�MAC�������ȡ�����ݳ��ȿ��ܲ�ֹ�ó��ȣ�ֻȡ��Ҫȡ�ĳ��ȼ���
//WLength:		��������һ�ſ���ESAM����д���ݳ��ȣ�����4�ֽ�MAC�����ܺ���8�ֽڿ���
//R_Type: 		�����ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//DisperseGene: ��ɢ����
short ReadBinaryMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short S_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = S_FileNo+0x80;
		*(ICBuf+3) = S_StartAddr;
		if( R_Type == 0 ) *(ICBuf+4) = 0x09;		//��ʱ�����8�ֽڿ���
		else *(ICBuf+4) = 0x11;						//��8�ֽڿ���
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4�ֽ������
		
//		*(ICBuf+4) = 0x04;
		*(ICBuf+4) = 0x83;
//		*(ICBuf+5) = 0xD6;
		*(ICBuf+5) = 0x2A;
		*(ICBuf+6) = D_FileNo+0x80;
		*(ICBuf+7) = D_StartAddr;
		*(ICBuf+8) = WLength;

		if( R_Type == 1 )					//�������кŷ�ɢ 
		{
			RAM_Write( ICBuf+9, DisperseGene, 8 );

			ICDataSend( CARD_TYPE, 0x10, CYC60000, 2 );		
		}
		else ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, RLength ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;	
}		
*/

//��MAC���������ļ�
//CARD_TYPE: 	������
//SFileType:	Դ�ļ�����
//Dest:			��һ����ȡ�õ������/�������ݵ�Ŀ�ĵ�ַ
//D_FileNo: 	Ŀ���ļ���
//S_FileNo: 	Դ�ļ���
//D_StartAddr: 	Ŀ����ʼ��ַ
//S_StartAddr:  Դ��ʼ��ַ
//Length: 		д���ݳ���
//R_Type: 		�����ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//RLength:		�������ݳ��ȣ�����4�ֽ�MAC�������ȡ�����ݳ��ȿ��ܲ�ֹ�ó��ȣ�ֻȡ��Ҫȡ�ĳ��ȼ���
//WLength:		��������һ�ſ���ESAM����д���ݳ��ȣ�����4�ֽ�MAC�����ܺ���8�ֽڿ���
//R_Type: 		�����ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//DisperseGene: ��ɢ����
short ReadBinaryMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short S_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;
		else
		{
			ICBuf = FBuff.Buff;
//			if( W_Type == 1 )
//			{
				*(ICBuf+6) = 0x48; 
				RAM_Write( ICBuf+7, DisperseGene, 8 );
				RAM_Write( ICBuf+7+8, Dest, 4 );
				ShortToSwapChar( ICBuf+7+8+4, S_StartAddr );
				ShortToSwapChar( ICBuf+7+8+4+2, RLength );
				*(ICBuf+23) = 0x04;
				*(ICBuf+24) = 0xD6;
				*(ICBuf+25) = D_FileNo+0x80;
				*(ICBuf+26) = D_StartAddr;
				*(ICBuf+27) = WLength;
//			}	
//			else RAM_Write( ICBuf+6, Source, Length );	
			if( ReadWriteESAM( Dest, 0x83, 0x2C, S_FileNo, 22 ) == -1 ) { SaveErrorResponse( ESAM ); return -1; }				//����2�ֽڴ�����Ӧ״̬

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = S_FileNo+0x80;
		*(ICBuf+3) = S_StartAddr;
		if( R_Type == 0 ) *(ICBuf+4) = 0x09;		//��ʱ�����8�ֽڿ���
		else *(ICBuf+4) = 0x11;						//��8�ֽڿ���
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4�ֽ������
		
//		*(ICBuf+4) = 0x04;
		*(ICBuf+4) = 0x83;
//		*(ICBuf+5) = 0xD6;
		*(ICBuf+5) = 0x2A;
		*(ICBuf+6) = D_FileNo+0x80;
		*(ICBuf+7) = D_StartAddr;
		*(ICBuf+8) = WLength;

		if( R_Type == 1 )					//�������кŷ�ɢ 
		{
			RAM_Write( ICBuf+9, DisperseGene, 8 );

			ICDataSend( CARD_TYPE, 0x10, CYC60000, 2 );		
		}
		else ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, RLength ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;	
}		

/*
//��MAC����¼�ļ� д�������ļ�
//CARD_TYPE: 	������
//Dest:			��һ����ȡ�õ������/�������ݵ�Ŀ�ĵ�ַ
//D_FileNo: 	Ŀ���ļ���
//S_FileNo: 	Դ�ļ���(ָ����¼��)
//D_StartAddr: 	Ŀ����ʼ��ַ
//Length: 		д���ݳ���
//R_Type: 		�����ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//RLength:		�������ݳ��ȣ�����4�ֽ�MAC�������ȡ�����ݳ��ȿ��ܲ�ֹ�ó��ȣ�ֻȡ��Ҫȡ�ĳ��ȼ���
//WLength:		��������һ�ſ���ESAM����д���ݳ��ȣ�����4�ֽ�MAC�����ܺ���8�ֽڿ���
//R_Type: 		�����ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//DisperseGene: ��ɢ����
short ReadRecMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB2;
		*(ICBuf+2) = S_FileNo;						//ָ����¼��
		*(ICBuf+3) = 0x0C;							//��P1ָ���ļ�¼��
		if( R_Type == 0 ) *(ICBuf+4) = 0x09;		//��ʱ�����8�ֽڿ���
		else *(ICBuf+4) = 0x11;						//��8�ֽڿ���
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB2 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4�ֽ������
		
		*(ICBuf+4) = 0x04;
		*(ICBuf+5) = 0xD6;
		*(ICBuf+6) = D_FileNo+0x80;
		*(ICBuf+7) = D_StartAddr;
		*(ICBuf+8) = WLength;

		if( R_Type == 1 )					//�������кŷ�ɢ 
		{
			RAM_Write( ICBuf+9, DisperseGene, 8 );
			ICDataSend( CARD_TYPE, 0x10, CYC60000, 2 );		
		}
		else ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, RLength ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;	
}		
*/

//��MAC���������ļ�,����Ǯ���ļ�
//CARD_TYPE: 	������
//Dest:			��һ����ȡ�õ������/�������ݵ�Ŀ�ĵ�ַ
//S_FileNo: 	Դ�ļ���
//S_StartAddr:  Դ��ʼ��ַ
short ReadBinaryIncMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short S_StartAddr )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;					//��ESAM����
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = 0x80+S_FileNo;
		*(ICBuf+3) = S_StartAddr;
		*(ICBuf+4) = 0x09;		
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4�ֽ������
		
//		*(ICBuf+4) = 0x84;
//		*(ICBuf+5) = 0x32;
//		*(ICBuf+6) = 0x01;
//		*(ICBuf+7) = 0x0C;
//		*(ICBuf+8) = 0x0C;

		*(ICBuf+4) = 0x80;
		*(ICBuf+5) = 0x42;
		*(ICBuf+6) = 0x00;
		*(ICBuf+7) = 0x00;
		*(ICBuf+8) = 0x0C;

		ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, 12 ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;	
}		

/*									
//��MAC���������ļ�,д��¼�ļ�
//CARD_TYPE: 	������
//Dest:			��һ����ȡ�õ������/�������ݵ�Ŀ�ĵ�ַ
//S_FileNo: 	Դ�ļ���
//S_StartAddr:  Դ��ʼ��ַ
short ReadBinaryRecMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short D_FileNo, short S_StartAddr )		//�¹���	//13.11.23
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = 0x80+S_FileNo;
		*(ICBuf+3) = S_StartAddr;
		*(ICBuf+4) = 0x09;		
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4�ֽ������
		
		*(ICBuf+4) = 0x04;
		*(ICBuf+5) = 0xDC;
		*(ICBuf+6) = D_FileNo;
		*(ICBuf+7) = 0x0C;
		*(ICBuf+8) = 0x08;

		ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, 8 ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;	
}		
*/
//��MAC���������ļ�,д��¼�ļ�
//CARD_TYPE: 	������
//Dest:			��һ����ȡ�õ������/�������ݵ�Ŀ�ĵ�ַ
//S_FileNo: 	Դ�ļ���
//S_StartAddr:  Դ��ʼ��ַ
short ReadBinaryRecMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short D_FileNo, short S_StartAddr )		//�¹���	//13.11.23
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;					//��ESAM����
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = 0x80+S_FileNo;
		*(ICBuf+3) = S_StartAddr;
		*(ICBuf+4) = 0x09;		
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4�ֽ������
		
//		*(ICBuf+4) = 0x04;
//		*(ICBuf+5) = 0xDC;
//		*(ICBuf+6) = D_FileNo;
//		*(ICBuf+7) = 0x0C;
//		*(ICBuf+8) = 0x08;

		*(ICBuf+4) = 0x83;
		*(ICBuf+5) = 0x3E;
		*(ICBuf+6) = 0x00;
		*(ICBuf+7) = 0x00;
		*(ICBuf+8) = 0x08;

		ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, 8 ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;	
}		

/*
//д�������ļ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//FileNo : �������ļ���
//StartAddr�� д�������ļ��е���ʼ��ַ
//Length��д���ݳ���
short UpdateBinary( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xD6;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Source, Length );

		ICDataSend( CARD_TYPE, Length-1, CYC120000, 2 );	//V6		//12.12.30
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}	
	return -1;	
}
*/

//дESAM�������ļ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//FileNo : �������ļ���
//StartAddr�� д�������ļ��е���ʼ��ַ
//Length��д���ݳ���
short UpdateBinary( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length )
{
    unsigned char Buff[10];
  unsigned char* ICBuf;

	ICBuf = FBuff.Buff;
//	RAM_Write( ICBuf+6, Source, Length );	
//	if( ReadWriteESAM( Buff, 0x83, 0x2A, 0x8000+(FileNo<<8)+StartAddr, Length ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬
	*(ICBuf+6) = 0x00;
	ShortToSwapChar( ICBuf+7, StartAddr );
	RAM_Write( ICBuf+9, Source, Length );
	if( ReadWriteESAM( Buff, 0x80, 0x2A, FileNo, Length+3 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
short UpdateBinaryLA( short CARD_TYPE, unsigned char* Source, unsigned short StartAddr, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xD6;
		RAM_Write( ICBuf+3, (unsigned char*)&StartAddr, 2 );
		*(ICBuf+2) = *(ICBuf+4);
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		RAM_Write( ICBuf, Source, Length );

		ICDataSend( CARD_TYPE, Length-1, CYC120000, 2 );		//V6		//12.12.30
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}	
	return -1;	
}
*/

/*
//��MACд�������ļ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//FileNo : �������ļ���
//StartAddr�� д�������ļ��е���ʼ��ַ
//Length��д���ݳ���,��������ʱ�Ѿ����������кŵ�8�ֽ�
//W_Type: 		д���ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//DisperseGene: ��ɢ����
short UpdateBinaryMAC( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length, short W_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xD6;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		if( W_Type == 1 )
		{
			RAM_Write( ICBuf, DisperseGene, 8 );
			RAM_Write( ICBuf+8, Source, Length-8 );
		}
		else RAM_Write( ICBuf, Source, Length );	

		ICDataSend( CARD_TYPE, Length-1, CYC240000, 2 );	//V6
		
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		return -1; 
	}	
	return -1;	
}
*/

//��MACд�������ļ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//FileNo : �������ļ���
//StartAddr�� д�������ļ��е���ʼ��ַ
//Length��д���ݳ���,��������ʱ�Ѿ����������кŵ�8�ֽ�
//W_Type: 		д���ͣ�0�����������кŷ�ɢ��1���������кŷ�ɢ
//DisperseGene: ��ɢ����
short UpdateBinaryMAC( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length, short W_Type, unsigned char* DisperseGene )
{
	unsigned char Buff[10];
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;
		else 
		{
			ICBuf = FBuff.Buff;
			if( W_Type == 1 )
			{
				RAM_Write( ICBuf+6, DisperseGene, 8 );
				RAM_Write( ICBuf+6+8, Source, Length - 8 );
			}	
			else RAM_Write( ICBuf+6, Source, Length );	
			if( ReadWriteESAM( Buff, 0x83, 0x2A, 0x8000+(FileNo<<8)+StartAddr, Length ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xD6;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		if( W_Type == 1 )
		{
			RAM_Write( ICBuf, DisperseGene, 8 );
			RAM_Write( ICBuf+8, Source, Length-8 );
		}
		else RAM_Write( ICBuf, Source, Length );	

		ICDataSend( CARD_TYPE, Length-1, CYC240000, 2 );	//V6
		
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		return -1; 
	}	
	return -1;	
}

/*
//��MAC��Ǯ
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//DisperseGene: ��ɢ����
short UpdateBinaryIncMAC( short CARD_TYPE, unsigned char* Source, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x84;
		*(ICBuf+1) = 0x32;
		*(ICBuf+2) = 0x01;
		*(ICBuf+3) = 0x0C;
		*(ICBuf+4) = 12;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0x32 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02

		RAM_Write( ICBuf, Source, 12 );	
		ICDataSend( CARD_TYPE, 11, CYC60000, 2 );	//V6
		if(( *ICBuf == 0x61 )&&( *(ICBuf+1) == 0x08 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		return -1;	
	}	
	return -1;	
}
*/

//��MAC��Ǯ
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//DisperseGene: ��ɢ����
short UpdateBinaryIncMAC( short CARD_TYPE, unsigned char* Source, unsigned char* DisperseGene )
{
	unsigned char Buff[20];
	unsigned char* ESAMSendBuff;

	ESAMSendBuff = FBuff.Buff;
	ICComm.ICAlarmFlg = ICALARM_28;						//��д����

	RAM_Write( ESAMSendBuff+6, Source, 12 );
	if( ReadWriteESAM( Buff, 0x80, 0x42, 0x0000, 0x000C ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
//��MAC��Ǯ
//CARD_TYPE: ����������
//Source : ����Դ��ַ
short UpdateBinaryDecMAC( short CARD_TYPE, unsigned char* Source )				//�¹���	//13.11.27
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x84;
		*(ICBuf+1) = 0x30;
		*(ICBuf+2) = 0x02;
		*(ICBuf+3) = 0x0C;
		*(ICBuf+4) = 0x08;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0x30 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02

		RAM_Write( ICBuf, Source, 8 );	

		ICDataSend( CARD_TYPE, 7, CYC60000, 2 );	//V6
		if(( *ICBuf == 0x61 )&&( *(ICBuf+1) == 0x08 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		return -1;	
	}	
	return -1;	
}
*/

/*
//��MACд��¼�ļ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
short UpdateBinaryRecMAC( short CARD_TYPE, unsigned char* Source, short D_FileNo )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;

		*ICBuf = 0x04;
		*(ICBuf+1) = 0xDC;
		*(ICBuf+2) = D_FileNo;
		*(ICBuf+3) = 0x0C;
		*(ICBuf+4) = 0x08;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xDC ) { SaveErrorResponse( CARD_TYPE ); continue; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02

		RAM_Write( ICBuf, Source, 8 );	

		ICDataSend( CARD_TYPE, 7, CYC60000, 2 );	//V6
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
		return -1;	
	}	
	return -1;	
}
*/
//��MACд��¼�ļ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
short UpdateBinaryRecMAC( short CARD_TYPE, unsigned char* Source, short D_FileNo )
{
	unsigned char Buff[10];
	unsigned char* ICBuf;

	ICBuf = FBuff.Buff;
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����

	RAM_Write( ICBuf+6, Source, 8 );	
	if( ReadWriteESAM( Buff, 0x83, 0x3E, 0x0000, 0x0008 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
//д��Կ
//Source : ����Դ��ַ
//FileNo : ��Կ��ʶ��
short UpdateKey( unsigned char* Source, short FileNo )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_16;						//��д����
	ICBuf = ICComm.ICBuf2;
	*ICBuf = 0x84;
	*(ICBuf+1) = 0xD4;
	*(ICBuf+2) = 0x01;
	*(ICBuf+3) = FileNo;
	*(ICBuf+4) = 0x20;
	
	ICDataSend( ESAM, 4, CYC5000, 1 );			//V6
	if( *ICBuf != 0xD4 ) { SaveErrorResponse( ESAM ); return -1; }				//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	RAM_Write( ICBuf, Source, 0x20 );

	ICDataSend( ESAM, (0x20-1), CYC60000, 2 );		//V6			//12.12.30
	if(( *ICBuf != 0x90 )||( *(ICBuf+1) != 0x00 )) { SaveErrorResponse( ESAM );  return -1; }		//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	ICComm.ICAlarmFlg = 0x0;						//��д����
	return 0;	
}
*/

/*
//��Ǯ���ļ�
//CARD_TYPE: ����������
//Dest : ����Ŀ�ĵ�ַ
short ReadRecord( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
	else ICBuf = ICComm.ICBuf2;
	*ICBuf = 0x00;
	*(ICBuf+1) = 0xB2;
	*(ICBuf+2) = FileNo;
	*(ICBuf+3) = StartAddr;
	*(ICBuf+4) = Length;

	SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02
	
	ICDataSend( CARD_TYPE, 4, CYC60000, Length+1 );		//V6	
	if( *ICBuf != 0xB2 ) { SaveErrorResponse( CARD_TYPE ); return -1; }		//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	RAM_Write( Dest, ICBuf+1, Length );
	ICComm.ICAlarmFlg = 0x0;						//��д����
	return 0;	
}
*/
//��ESAMǮ���ļ�
//Dest : ����Ŀ�ĵ�ַ
short ReadRecord( short CARD_TYPE, unsigned char* Dest )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����
	ICBuf = FBuff.Buff;
	*(ICBuf+6) = 0x00;
	if( ReadWriteESAM( Dest, 0x80, 0x48, 0x0000, 0x0001 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
//���ۿ�
//CARD_TYPE: ����������
//Source : ����Դ��ַ
//Dest : �ȶԽ������Դ��ַ
//Ins : ����	0x32: �������   0x30: �ۿ�����
short InDecrease( short CARD_TYPE, unsigned char* Source, short StartAddr, unsigned char Ins )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��ۿ����
	if( CARD_TYPE == PSAM ) 
	{
		ICBuf = ICComm.ICBuf1;
	} 
	else 
	{
	  	ICBuf = ICComm.ICBuf2;
	}  
	*ICBuf = 0x80;
	*(ICBuf+1) = Ins;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = StartAddr;
	*(ICBuf+4) = 0x04;
	
	ICDataSend( CARD_TYPE, 4, CYC5000, 1 );			//V6
	if( *ICBuf != Ins ) { SaveErrorResponse( CARD_TYPE ); return -1; }		//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	RAM_Write( ICBuf, Source, 0x04 );

	ICDataSend( CARD_TYPE, (0x04-1), CYC60000, 2 );	//V6	//12.12.30
	if(( *ICBuf == 0x61 )&&( *(ICBuf+1) == 8 )) 
	{
		if( GetResponse( CARD_TYPE, ICBuf, 8 ) != 0 ) return -1;
		ICComm.ICAlarmFlg = 0x00;
		return 0;	
	}	
	else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	return -1;	
}
*/

//���Ŀۿ�
//Source : ����Դ��ַ
short InDecrease( short CARD_TYPE, unsigned char* Source )
{
	unsigned char Buff[20];
    unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����
	ICBuf = FBuff.Buff;
	RAM_Write( ICBuf+6, Source, 0x04 );
	if( ReadWriteESAM( Buff, 0x80, 0x46, 0x0000, 0x0004 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}	

unsigned long GetDecData( unsigned char* Source )
{
	unsigned long Value1,Value2,Value3;
		
	RAM_Write( (unsigned char*)&Value1, Source, 4 );		//ʣ����
	RAM_Write( (unsigned char*)&Value2, Source+4, 4 );		//͸֧���
	RAM_Write( (unsigned char*)&Value3, Source+8, 4 );		//ESAM��ʣ����

	if( Value2 == 0 ) 										
	{
		if( Value1 >= Value3 ) Value3 = 0;					//���ʣ����>=ESAM��ʣ����  
		else
		{
			Value3 -= Value1; 
			return Value3;
		}		
	}														//�Ѿ�͸֧���۹�	
	return Value3;
}	

short WriteESAMRunInfoReOver( unsigned char* ReDest, unsigned char* OverDest )
{
	unsigned char Buff1[5];
	unsigned char Buff2[5];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned long Value;
	
	Ptr = Buff1;
	Point = Buff2;
	
	RAM_Write( Ptr, ReDest, 4 );
	SwapData( Ptr, 4 );
	
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 23, 4 ) != 0 ) return 1;
	
	RAM_Write( (unsigned char*)&Value, OverDest, 4 );
	Long_BCD4( Point, Value );
	SwapData( Point, 4 );
	if( UpdateBinary( ESAM, Point, FILE_No7, 31, 4 ) != 0 ) return 1;
	RAM_Write( Ptr, Clk.Min, 5 );
	SwapData( Ptr, 5 );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 42, 5 ) != 0 ) return 1;
	
	return 0;
}	


short Write_ESAMBurse( void )
{
//	unsigned char Buff[12];
	unsigned char Buff[30];
	unsigned char Buff2[10];
	unsigned char* Ptr;
	unsigned char* Point;
//	unsigned char Ins;
	unsigned long Value;
	
	Ptr = Buff;
	Point = Buff2;
	
	E2P_RFM( Ptr, FConsumECSumBCD, 8 );											//11.07.14
	if(( Flag.BatState & F_E2PCheck ) == 0 )									//11.07.14
	{																			//11.07.14	
		E2P_WData( ConsumECSumBCD, Ptr, 8 );									//11.07.14
	}																			//11.07.14
	else																		//11.07.14
	{																			//11.07.14
		if( ChkNum( ICEC.RConsumECSumBCD, 8 ) == ICEC.RConsumECSumBCDChk )		//11.07.14
		{																		//11.07.14	
			E2P_RFM( Ptr, FConsumECSumBCD, 8 );									//11.07.14
			if(( Flag.BatState & F_E2PCheck ) == 0 )							//11.07.14
			{																	//11.07.14	
				E2P_WData( ConsumECSumBCD, Ptr, 8 );							//11.07.14	
			}																	//11.07.14
			else																//11.07.14
			{																	//11.07.14
				E2P_WData( ConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );			//11.07.14
			}																	//11.07.14
		}																		//11.07.14
	}																			//11.07.14

//	RAM_Fill( Ptr, 12);
	RAM_Fill( Ptr, 30);
	
	GetReOverEC( Ptr, Ptr+4 );
	RAM_Write( Point, Ptr, 8 );

//	if( ReadRecord( ESAM, Ptr+8, FILE_No1, 0x0C, 0x04 ) != 0 ) return -1;
	if( ReadRecord( ESAM, Ptr+8 ) != 0 ) return -1;					//����4�ֽ�ʣ���4�ֽڹ��������6�ֽڿͻ����	

	SwapData( Ptr+8, 4 );														//4�ֽ�ʣ����	
	
	Value = GetDecData( Ptr );
	if( Value == 0 ) return 0;								//����Ҫ��Ǯ	
	RAM_Write( Ptr, (unsigned char*)&Value, 4 );
	SwapData( Ptr, 4 );										//�ߵ��ֽڽ���
//	Ins = 0x30;												//��Ǯ		
//	if( InDecrease( ESAM, Ptr, 0x0C, Ins ) != 0 ) return -1;
	if( InDecrease( ESAM, Ptr ) != 0 ) return -1;
	if( WriteESAMRunInfoReOver( Point, Point+4 ) != 0 ) return -1;
	return 0; 
}	

void InitialAbnormalInsertCardMeassage( void )			//�¹���	//13.12.02
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value1,Value2;
	
	Ptr = Buff;
	
	RAM_Fill( (unsigned char*)&ICComm.ErrorMessage, 8 );
	RAM_DataFill( Para.CardNo, 8, 0xFF ); 									//�¹���	//14.01.22
	E2P_RData( (unsigned char*)&Value1, BuyECCnt, 4 );
	Long_BCD4( Ptr, Value1 );				
	RAM_Write( ICComm.InsertCardECCnt, Ptr, 2 );
	GetReOverEC( (unsigned char*)&Value1, (unsigned char*)&Value2 );		//ʣ�����
	Long_BCD4( ICComm.InsertCardRemainEC, Value1 );				
		
}	

//�жϲ忨����������
void CardProceIntegrality( void )			
{
	if( ICComm.ICOperateState == 0x01 ) ICComm.ICOperateState = 0x02;
}	

short JudgeMeterVolt80Un( void )								//�¹���	//13.12.03
{
	short i;
	unsigned short TempInt;
	unsigned short V80Un;						
	      
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
//	if(( PIN_SETEN & P_SETEN ) == 0 ) return 0;				//�¹���	//14.07.08
	if(( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 ) return 0;
#endif

	V80Un = MSpec.RRatedVolt;				
	V80Un *= 4;								
	V80Un /= 5;								
	
	TempInt = V80Un;
	for( i=0;i<3;i++ )
	{
#if ( LinkMode == Phase3Wire3 )
		if( i == 1 ) continue;
#endif			
		TempInt = BCD2_Word( Real.Ua+i*2 );
		if( TempInt >= V80Un ) break;			
	}		
	if( i == 3 ) return -1;			//��Դ�쳣
	else return 0;
}

void ProcessICAlarmFlg( void )		//�¹���	//13.12.03
{
	if( ICComm.ErrorMessage == 0 ) ICComm.ICDisAlarmFlg = 0; 			//�¹���	//13.12.20	//14.01.06
	else if(( ICComm.ErrorMessage >= ICERROR_01 )&&( ICComm.ErrorMessage <= ICERROR_03 )) { ICComm.ICAlarmFlg = ICALARM_31; ICComm.ICDisAlarmFlg = ICALARM_31; }
	else if(( ICComm.ErrorMessage >= ICERROR_04 )&&( ICComm.ErrorMessage <= ICERROR_10 )) { ICComm.ICAlarmFlg = ICALARM_32; ICComm.ICDisAlarmFlg = ICALARM_32; }
	else if(( ICComm.ErrorMessage >= ICERROR_11 )&&( ICComm.ErrorMessage <= ICERROR_13 )) { ICComm.ICAlarmFlg = ICALARM_33; ICComm.ICDisAlarmFlg = ICALARM_33; }
	else if(( ICComm.ErrorMessage >= ICERROR_14 )&&( ICComm.ErrorMessage <= ICERROR_18 )) { ICComm.ICAlarmFlg = ICALARM_34; ICComm.ICDisAlarmFlg = ICALARM_34; }
 	else if(( ICComm.ErrorMessage >= ICERROR_19 )&&( ICComm.ErrorMessage <= ICERROR_20 )) { ICComm.ICAlarmFlg = ICALARM_35; ICComm.ICDisAlarmFlg = ICALARM_35; }
	else if( ICComm.ErrorMessage == ICERROR_21 ) { ICComm.ICAlarmFlg = ICALARM_36; ICComm.ICDisAlarmFlg = ICALARM_36; }
}	

							//remote
void ICProce( void )
{
	unsigned char FrameType;
	unsigned char InterFileNo;
	
	InitialAbnormalInsertCardMeassage();			//�¹���	//13.12.02
	
	ICComm.CardType = 0;
	if( Card_CoolRST( PSAM ) != 0 )
	{
		if( Card_CoolRST( PSAM ) != 0 ) 
		{ 
			ICComm.ICAlarmFlg = ICALARM_23; 
			Recd_ADD1( ILInsert_Cunt, 3 ); 			//�¹���	//13.12.10
			RAM_DataFill( Para.CardNo, 8, 0xFF ); 	//�¹���	//13.12.10
			ICComm.ErrorMessage = ICERROR_04; 		//�¹���	//13.12.03
			return;  
		}		
	}
	ICEC.IdentityValidDelay = 0;					//�¹���	//13.12.11
	ICEC.IdentityValidDelayChk = 0xA5;				//�¹���	//13.12.11
	if( Card_CoolRST( ESAM ) != 0 ) 	//����ESAM���ڳ��ڹ���״̬
	{ 
		ICComm.ICAlarmFlg = ICALARM_11; 
		Flag.MeterState1 |= F_ESAMErr;	//�������״̬1
		ICComm.ErrorMessage = ICERROR_03; 		//�¹���	//13.12.02
		return;  		
	}
	else Flag.MeterState1 &= ~F_ESAMErr;
	
	if( SelectDF( 0x01DF) != 0 ) { CardProceIntegrality(); return; }
	if( ReadBinary( PSAM, ICComm.ICBuf1, 0x01, 0x0, 4 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_28; CardProceIntegrality(); return;  }
	if( ICComm.ICBuf1[0] != 0x68) { ICComm.ICAlarmFlg = ICALARM_28; ICComm.ErrorMessage = ICERROR_14; CardProceIntegrality(); return;  }	//�¹���	//13.12.03
	FrameType = ICComm.ICBuf1[1];
	InterFileNo = FILE_No1;						//ϵͳ�����֤��Կ�ļ���ʶ��Ϊ01
		if( InternalAuthentication(InterFileNo, Para.CardNo ) != 0 ) 
		{
			if(( FrameType == CARD_PREPARE )&&( ICComm.ErrorMessage == ICERROR_05 ))		//�¹���	//14.01.02	//14.01.06
			{
//				if( GetTrueBitSum_32Bit( Flag.SecretKeyState, Para.RSecretKeySum ) != 0  )		//������Կ��ȫ��Ϊ������Կ
				if( ICComm.RPrivateKeyFlag == 0x00 )		//˽Կ			
				{
					ICComm.ErrorMessage = ICERROR_09;
				}	
			}	
			
			ICComm.ICAlarmFlg = ICALARM_10;
			if( ICComm.ErrorMessage == ICERROR_05 ) Recd_ADD1( ILInsert_Cunt, 3 );		//�¹���	//14.01.06
			CardProceIntegrality(); 
			return;
		}	
	if( Card_Purview( FrameType ) != 0 ) { CardProceIntegrality(); return; }			//�¹���		//13.12.05	
	if( GetFullFrame( FrameType ) != 0 ) { CardProceIntegrality(); return; }
	ICEC.DecDelay = 15;
	if( Write_ESAMBurse() != 0 ) { ICComm.ErrorMessage = ICERROR_02; ICComm.ICAlarmFlg = ICALARM_28; CardProceIntegrality(); return; }		//��ESAM�ڵ�ʣ����
	ICComm_Ctrl( FrameType );
	CardProceIntegrality(); 				//�忨�������������ж�
}		

							//remote
short GetFullFrame( short FrameType )
{
	unsigned char Buff[100];			//�¹���		//13.12.12
	unsigned char *Ptr;					//�¹���		//13.12.12
	short FrameNo;
	short Len;
	short FileNo;								
	
	Ptr = Buff; 						//�¹���		//13.12.12
	
	FrameNo = FrameType;
	if(( FrameNo != 1 )&&( FrameNo != 6 )) { ICComm.ICAlarmFlg = ICALARM_26; ICComm.ErrorMessage = ICERROR_08; return -1;  }	//��Ч���������뷶ΧΪ1��9		//�¹���		//13.12.03
	if( FrameNo == 1 ) FrameNo = 0;	//�����		//�¹���		//13.09.06
	else FrameNo = 1;					//�����		//�¹���		//13.09.06
																				
	ICComm.CardType = FrameType;
	Len = FramePara[FrameNo].Len;

	if( ReadBinary( PSAM, ICComm.ICBuf1, FILE_No1, 0x00, Len ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//�¹���	//14.01.06

	SwapFrameData( ICComm.ICBuf1+0x04, FramePara[FrameNo].TabPtr, FramePara[FrameNo].ItemCount );	//�¹����ܹ����ſ�����Ҫ������		//�¹���		//13.09.06

	if(( ICComm.ICBuf1[0] != 0x68 )||( ICComm.ICBuf1[3]!= (Len-6 ))		//֡��ʽ�Ƿ����		//����ռ��2�ֽ�
		||( ICComm.ICBuf1[Len-1] != 0x16 )||( ICComm.ICBuf1[ Len-2] != ChkNumAdd( ICComm.ICBuf1+1,Len-3 ))) 
		{ 
			ICComm.ErrorMessage = ICERROR_14;							//�¹���		//13.12.03
			ICComm.ICAlarmFlg = ICALARM_25; 
			return -1;	
		}
	if( FrameType == CARD_BUYEC )		//�û�����Ҫ����ⲿ��֤		//�¹���		//13.12.05
	{														
		FileNo = FILE_No2;				//�û�����д��Կ�ļ���ʶΪ02	
		RAM_Write( Ptr, ICComm.ICBuf1, Len );							//�¹���		//13.12.12
		if( PSAMExternalAuthentication( FileNo, Para.CardNo ) != 0 ) { return -1; }		//��дǰ�Կ������ⲿ��֤	//V8	//�¹���		//13.12.03	//14.01.06
		RAM_Write( ICComm.ICBuf1, Ptr, Len );							//�¹���		//13.12.12
	}										
		
	return 0;	
}

short Card_Purview( short FrameType )						//�¹���		//13.12.03
{
//	unsigned char Sum; 
	
//	Sum = GetTrueBitSum_32Bit( Flag.SecretKeyState, Para.RSecretKeySum );	//ȡ˽Կ��
//	if( Sum != 0  )										//������Կ��ȫ��Ϊ������Կ
	if( ICComm.RPrivateKeyFlag == 0x00 )		//˽Կ			
	{
		if( FrameType == CARD_PREPARE )		
		{ 
			ICComm.ErrorMessage = ICERROR_09;
			return -1; 
		}
	}	
//	if( Sum != Para.RSecretKeySum  )					//������Կ����ȫ��Ϊ��ʽ��Կ��������幺�翨	
	if( ICComm.RPrivateKeyFlag == 0x55 )		//��Կ			
	{
		if( FrameType == CARD_BUYEC )		
		{ 
			ICComm.ErrorMessage = ICERROR_09;
			return -1; 
		}
	}	
	
	return 0;
}	
							//remote
void ICComm_Ctrl( short FrameType )
{
	switch( FrameType )
	{
		case CARD_BUYEC:	//�������Ϣ�ļ�
			IC_BuyEC();		//���翨
			break;
		case CARD_PREPARE:	//Ԥ�ÿ�ָ����Ϣ�ļ�
			IC_Prepare();	//Ԥ�ÿ�
			break;
		default: break;
	}	
}	

						//remote
void GetECState( void )
{
	unsigned char Buff[8];
	unsigned char* Ptr;	
	
	Ptr = Buff;
	
	GetReOverEC( Ptr, Ptr+4 );
	GetICECState( Ptr, Ptr+4 );
}

void GetConsumECSum( unsigned char* Dest )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	if( ChkNum( ICEC.RConsumECSumBCD, 8 ) == ICEC.RConsumECSumBCDChk )
	{
		RAM_Write( Dest, ICEC.RConsumECSumBCD, 8 );
	}	
	else
	{
		E2P_RFM( Ptr, FConsumECSumBCD, 8 );
		RAM_Write( Dest, Ptr, 8 );														//11.07.14
		if(( Flag.BatState & F_E2PCheck ) == 0 )
		{
			RAM_Write( Dest, Ptr, 8 );
			RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
			ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
		}
		else
		{	
			E2P_RData( Ptr, ConsumECSumBCD, 8 );
			if(( Flag.BatState & F_E2PCheck ) == 0 )
			{
				E2P_RFM( Dest, FConsumECSumBCD, 8 );									//11.07.14
				if(( Flag.BatState & F_E2PCheck ) == 0 )								//11.07.14	
				{																		//11.07.14	
					RAM_Write( ICEC.RConsumECSumBCD, Dest, 8 );							//11.07.14		
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );		//11.07.14	
				}																		//11.07.14		
				else																	//11.07.14	
				{																		//11.07.14
					RAM_Write( Dest, Ptr, 8 );
					RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
					E2P_WFM( FConsumECSumBCD, Ptr, 8 );
				}																		//11.07.14
			}
		}
	}		
}

//ȡʣ���͸֧����
//ReDest : ʣ�����Ŀ���ַ		HEX
//OverDest : ͸֧����Ŀ���ַ	HEX
void GetReOverEC( unsigned char* ReDest, unsigned char* OverDest )
{
	unsigned long Temp;
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long long LValue1,LValue2;
	
	Ptr = Buff;
	
	GetConsumECSum( Ptr+10 );
	RAM_Write( (unsigned char*)&LValue2, Ptr+10, 8 );	
	E2P_RData( Ptr, BuyECSumBCD, 8 );
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );	

	if( LValue1 >= LValue2 )
	{
		RAM_Fill( OverDest, 4 );
		LValue1 -= LValue2;	
		Temp = LValue1 / 10000;						
		RAM_Write( ReDest, (unsigned char*)&Temp, 4 );		//ʮ�����Ƶ�ʣ���� 	XXXXXX.XXԪ
		ICEC.RRemainEC = Temp;				//ʣ����
		ICEC.ROverdraftEC = 0;              //͸֧���
	}
	else
	{
		RAM_Fill( ReDest, 4 );
		LValue2 -= LValue1;	
		Temp = LValue2 / 10000;		
		RAM_Write( OverDest, (unsigned char*)&Temp, 4 );	//ʮ�����Ƶ�͸֧��� 	XXXXXX.XXԪ
		ICEC.RRemainEC = 0;					//ʣ����
		ICEC.ROverdraftEC = Temp;           //͸֧���
	}		
}	

void KeyRelayOn( void )									//11.06.27
{
	ICComm.ICInState |= ( F_BuyEC + F_BuyECOver );		//11.08.19		
	SaveBuyECState();									//11.08.19
	GetECState();										//11.08.19
	if( ICComm.RelayONState == 0xAA ) ICComm.RelayONState = 0x55;	//�������բ״̬�£��忨ֱ�Ӻ�բ

}

//�˳�Ԥ��բ״̬
void ExitPreTripState( void )													//11.08.19
{																				//11.08.19
	if( Para.RPreTripState == 0x55 ) 											//11.08.19	
	{																			//11.08.19
		Para.RPreTripState = 0x00;												//11.08.19
		E2P_WData( EPreTripState, (unsigned char*)&Para.RPreTripState,1 );		//11.08.19
	}																			//11.08.19	
}

//����Ԥ��բ״̬
void EnterPreTripState( void )													//11.08.19
{																				//11.08.19
	if( Para.RPreTripState != 0x55 ) 											//11.08.19	
	{																			//11.08.19
		Para.RPreTripState = 0x55;												//11.08.19
		E2P_WData( EPreTripState, (unsigned char*)&Para.RPreTripState,1 );		//11.08.19
	}																			//11.08.19	
}

//��λ���к�բ�������־
void SetJudgePermitFlag( void )										//�¹���	//13.12.10			
{
	if( Para.RMeterSafeState == 0x3A ) 				//����			//�¹���	//14.01.09		
	{
		ClearJudgePermitFlag();										//�¹���	//13.12.10			
		return;		
	}	
	if(( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed )||( ICComm.ICECState == F_ECEnd ))	//�¹���	//14.01.09
	{	
		if( ICComm.RJudgePermitFlag != 0x55 )
		{
			ICComm.RJudgePermitFlag = 0x55;
			E2P_WData( JudgePermitFlag, (unsigned char*)&ICComm.RJudgePermitFlag,1 );				
		}
	}			
}	

//������к�բ�������־
void ClearJudgePermitFlag( void )									//�¹���	//13.12.10			
{
	if( ICComm.RJudgePermitFlag != 0x00 )
	{
		ICComm.RJudgePermitFlag = 0x00;
		E2P_WData( JudgePermitFlag, (unsigned char*)&ICComm.RJudgePermitFlag,1 );				
	}		
}	

void SaveBuyECState( void )					//�������	//11.06.27
{
	unsigned char Temp;
	unsigned char Temp2;
	
	E2P_RData( (unsigned char*)&Temp, EICInState, 1 );
	
	Temp &= (F_BuyEC + F_BuyECOver);
	Temp2 = ICComm.ICInState;
	Temp2 &= (F_BuyEC + F_BuyECOver);
	
	if( Temp != Temp2 )
	{
		E2P_WData( EICInState, (unsigned char*)&ICComm.ICInState, 1 );
	}	
}


//ReDest: ʣ�����
//OverDest: ͸֧����	
//F_ECNormal	0x00	//ʣ��������ڵ��ڱ�������1��
//F_ECAlarm1	0x01	//ʣ��������ڵ��ڱ�������2��С�ڱ�������1��
//F_ECAlarm20	0x02	//ʣ���������0��С�ڱ�������2����û�в���û���(��բ����)��
//F_ECAlarm21	0x03	//ʣ���������0��С�ڱ�������2������û���(�ָ��õ�)��
//F_ECOver		0x04	//ʣ���������0��͸֧��������0���Ƿ���բ����͸֧�����޶������
//F_ECOverdraft	0x05	//͸֧��
//F_ECOverEnd	0x06	//͸֧�������ꣻ		//V7
//F_ECExceed	0x07	//����͸֧����޶	//�������	//11.06.27
//F_ECEnd		0x08	//ʣ���������0��͸֧��������0��͸֧�����޶����0��			//11.08.19
//F_NULL		0x55	//��(���ڼ̵�����)		//V7

							//remote
void GetICECState( unsigned char* ReDest, unsigned char* OverDest )
{
	unsigned long RemainEC,Alarm1,Alarm2;
	unsigned char Buff[8];
	unsigned char* Ptr;
	unsigned char RelayState=0;
	unsigned char OldICECState;							//�¹���	//14.01.07	
	Ptr = Buff;
	
	ICComm.Power &= ~F_FmqbjEn;					//V8				//������ʹ��
	OldICECState = ICComm.ICECState;					//�¹���	//14.01.07	
	RAM_Write( (unsigned char*)&RemainEC, ReDest, 4 );	//ʣ�����
	RAM_Write( (unsigned char*)&Alarm1, OverDest, 4 );	//͸֧����
	if( RemainEC == 0 )
	{
		if( Alarm1 == 0 ) ICComm.ICECState = F_ECOver;
		else ICComm.ICECState = F_ECOverdraft;
		E2P_RData( Ptr, OverdraftECLimt, 4 );
		Alarm2 = BCD4_Long( Ptr );		//͸֧�����޶�	
		if( Alarm2 > Alarm1 )
		{
			if(( ICComm.ICInState & F_BuyECOver ) == 0 ) 	//�¹���	//13.12.07
			{
				RelayState = 1;	//��բ			//V7	//͸֧���Ƿ����û���	//�������	//11.06.27
				if(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC <= ICEC.RPermitTripONLimt )) ;		//�¹���	//14.01.09
				else if( Para.RMeterSafeState == 0x3A )	;									//����			//�¹���	//14.01.14
				else if(( ICComm.TripOFFDelay == 0 )&&( Para.RMeterTripState == 0x1A ))	;	//Զ����բ		//�¹���	//14.01.14
				else																						//�¹���	//14.01.09
				{
					EnterPreTripState();						//13.12.07
					ICComm.RelayONState = 0xAA;			//13.12.07		
				}	
			}	
			else
			{												//V8
				RelayState = 0;	//����բ
			}												//V8	
		}	
		else 
		{
			if( ICComm.ICECState != F_ECOver ) ICComm.ICECState = F_ECOverEnd;		//V7
			else ICComm.ICECState = F_ECEnd;		//ʣ���������0��͸֧��������0��͸֧�����޶����0��			//11.08.19
			RelayState = 1;	//��բ			
			ICComm.ICInState &= ~( F_BuyECOver+ F_CheckECOver );		//�������	//11.06.27
			if( Alarm1 > Alarm2 ) ICComm.ICECState = F_ECExceed;		//����͸֧�޶��ʾERR-01		

			ICComm.RelayONState = 0x00;							//13.12.07
			ExitPreTripState();									//11.08.19
		}	
		
		if( OldICECState != 0xff )								//�ϵ�RAM�Ҳ��������ϱ�͸֧״̬		//�¹��� 	//14.01.07
		{
			if(( OldICECState != F_ECOverdraft )&&( OldICECState != F_ECOverEnd )&&( OldICECState != F_ECExceed ))
			{
				if(( ICComm.ICECState == F_ECOverdraft )||( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed ))
				{
					UpdateEventUpState( 9 );	
				}	
			}				
		}			
	}	
	else
	{
		ICComm.ICInState &= ~( F_BuyECOver+ F_CheckECOver );			//V7
		E2P_RData( Ptr, AlarmEC1, 4 );
		Alarm1 = BCD4_Long( Ptr );
		E2P_RData( Ptr, AlarmEC2, 4 );
		Alarm2 = BCD4_Long( Ptr );

		if( RemainEC > Alarm1 ) { ICComm.ICECState = F_ECNormal; ICComm.ICInState &= ~F_BuyEC; }	//�������	//11.06.27
		if(( RemainEC > Alarm2 )&&(RemainEC <= Alarm1)) ICComm.ICECState = F_ECAlarm1;
		if(( RemainEC > 0 )&&(RemainEC <= Alarm2))
		{
			if(( ICComm.ICInState & F_BuyEC ) == 0 ) { ICComm.ICECState = F_ECAlarm20; RelayState = 1; }	//��բ 
			else { ICComm.ICECState = F_ECAlarm21; RelayState = 0; ICComm.Power |= F_FmqbjEn; }	//����բ		//V8	
		}
		if( RemainEC > Alarm2) 
		{
			ICComm.ICInState &= ~(F_BuyEC+F_CheckEC);		//V7
		}	
	}	
	
	SaveBuyECState();				//�������	//11.06.27

	SetJudgePermitFlag();										//��λ���к�բ�������־	//�¹���	//13.12.10	//14.01.10
	
	if( ICComm.ICECState != F_ECAlarm21 ) ICComm.ICInState &= ~F_Alarm2RelayONPermit;		//11.04.23

	if( ICComm.RelayTestDelay == F_NULL )
	{
		if( Para.RMeterSafeState == 0x3A )									//����
		{
#if ( RelayONImmediately == YesCheck )
			if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0x55;	//������բ
#else
			if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0xAA;	//�����բ
#endif
			if( Para.RMeterRelayState != 0x1A ) ICComm.RelayONState = 0x00;//�¹���	//13.01.09
			ExitPreTripState();									//11.08.19
			return;															//����״̬��������ʣ�����������բ	
		}	
		if(( ICComm.TripOFFDelay == 0 )&&( Para.RMeterTripState == 0x1A ))	//Զ����բ
		{			
			if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//׼����բ																											
			ICComm.RelayONState = 0x00;
			ExitPreTripState();									//11.08.19
			return;															//Զ����բ״̬��������ʣ�����������բ
		}	
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		if(( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E ))	//Զ��Ԥ��բ״̬��
		{
			if( RelayState == 1 )											//Ҫ���뱾��Ԥ��բ����բ
			{
				ICComm.PreTripOFFKeepTime = 0;								//�˳�Զ��Ԥ��բ
				Para.RMeterTripState = 0x1B;			//�����բ			//�˳�Զ��Ԥ��բ
				E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				
				if( Para.RMeterRelayState == 0x1A )							//���Ѿ���Զ��Ԥ��բ������բ״̬��ά����բ״̬	
				{
					if(( ICComm.ICECState == F_ECEnd )||( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed )) 	
					{																											 				
						ExitPreTripState();									//ע��Ŀǰ�Ǳ���Ԥ��բ״̬														
					}																												
					else EnterPreTripState();								//ע��Ŀǰ���뱾��Ԥ��բ״̬�������ڱ��غ�բ				
				}
			}	
			else															//Զ��Ԥ��բ״̬�±���û��Ԥ��բ����բ����ִ��Զ��Ԥ��բ 
			{
				if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//׼����բ																											
				ICComm.RelayONState = 0x00;
				return;
			}	
		}	
#endif
		if(( RelayState == 1 )&&( Para.RMeterRelayState != 0x1A ))			//��ǰ��բ״̬��Ҫ��բ
		{
			if(( ICComm.ICECState == F_ECEnd )||( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed )) 	//11.08.19	
			{																											 	//11.08.19				
				ExitPreTripState();																							//11.08.19	
			}																												//11.08.19	
			else EnterPreTripState();																						//11.08.19	

			ICComm.RelayOFFState = 0x55;									//׼����բ
			ICComm.RelayONState = 0x00;
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			if( IsAllData( Comm.TripOFFOperateCode, 4, 0xFF ) != 0 )		//�¹���	//14.05.12
			{
				RAM_DataFill( Comm.TripOFFOperateCode, 4, 0xFF );			//�Զ���բ			
				E2P_WData( ETripOFFOperateCode, Comm.TripOFFOperateCode, 4 );//�¹���	//14.05.12
			}	
#else
			RAM_DataFill( Comm.TripOFFOperateCode, 4, 0xFF );				//�Զ���բ			
#endif
		}	
#if ( IOver30ALimitTrip	== YesCheck )		//�¹���	//14.04.25
		else if( RelayState == 0 )
		{
			ICComm.RelayOFFState = 0x0;				//13.02.28				//�����ֵ�������󻹼���ִ�е�������ʱ����բ����	//14.09.10			
			if( Para.RMeterRelayState == 0x1A )		//�̵���״̬��Ҫ���  	//V7
			{ 
#if ( RelayONImmediately == YesCheck )
				ICComm.RelayONState = 0x55;										//������բ
#else
				ICComm.RelayONState = 0xAA;										//�����բ
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
#else       	
				RAM_DataFill( Comm.TripONOperateCode, 4, 0xFF );				//�Զ���բ��������													
#endif
			}
		}	
#else
		else if(( RelayState == 0 )&&( Para.RMeterRelayState == 0x1A ))		//�̵���״̬��Ҫ���  //V7
		{
#if ( RelayONImmediately == YesCheck )
			ICComm.RelayONState = 0x55;										//������բ
#else
			ICComm.RelayONState = 0xAA;										//�����բ
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
#else
			RAM_DataFill( Comm.TripONOperateCode, 4, 0xFF );				//�Զ���բ��������									
#endif
		}	
#endif
	}	
}
	
void OperateRelay( void )
{
#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&((( Para.RPreTripState == 0x55 )||( ICComm.RJudgePermitFlag == 0x00 )||(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC > ICEC.RPermitTripONLimt ))||(Para.RMeterSafeState == 0x3A))&&( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 )))	//ִ�к�բ			//�¹���	//13.12.10
#else
	if((( Para.RPreTripState == 0x55 )||( ICComm.RJudgePermitFlag == 0x00 )||(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC > ICEC.RPermitTripONLimt ))||(Para.RMeterSafeState == 0x3A))&&( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//ִ�к�բ			//�¹���	//13.12.10
#endif
	{
		RecordStartEvent( TripON_No );
		ExitPreTripState();									//11.08.19
		ClearJudgePermitFlag();	//����к�բ�������־	//�¹���	//13.12.10

		ICComm.ICInState |= ( F_BuyEC + F_BuyECOver );		//11.08.19		
		SaveBuyECState();									//11.08.19

		ICComm.ICInState &= ~F_Alarm2RelayONPermit;		//11.04.23
		ICComm.RelayDriveCnt = 5;	
		RelayDrive( RELAY_ON );	
		ICComm.RelayONState = 0x0;
		ICComm.RelayOFFState = 0x00;			//��բ��ȡ��������բ״̬	//�¹���	//13.10.25
		Para.RMeterRelayState = 0x55;			//�Ѿ���բ
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//		CreateTripRec( TripON_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		EVar.RelayFaultVar.Cunt = 1;														//�¹���	//14.05.03	
		EVar.RelayFaultVar.OldState = 0xff;													//�¹���	//14.05.03
		E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//�¹���	//14.05.12	
		Flag.RSwitchOldState &= 0x04;														//�¹���	//14.05.03	
		Flag.RSwitchOldState |= 0x40;			//�����¼�쳣�¼�							//�¹���	//14.05.12				
		E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
//15.08.05
		if( IsAllData( Comm.TripONOperateCode, 4, 0xFF ) != 0 )			//�¹���	//14.05.12
		{
			RAM_DataFill( Comm.TripONOperateCode, 4, 0xFF );			//�Զ���բ			
			E2P_WData( ETripONOperateCode, Comm.TripONOperateCode, 4 );	//�¹���	//14.05.12
		}	
#endif
		RecordEndEvent( TripON_No, 0 );
	}				

#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 )))	//ִ����բ		//�¹���	//13.10.25
#else
	if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//ִ����բ		
#endif
	{
#if ( IOver30ALimitTrip	== YesCheck )		//�¹���	//14.04.25
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCount >= Para.RUp30AClose24HOUR ))			//�¹���	//14.06.19
#else
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCnt == UP30A24HOUR ))
#endif
		{
#endif
			RecordStartEvent( TripOFF_No );
			ICComm.RelayDriveCnt = 5;				//��Ϊ5����ٲ���һ��	
			RelayDrive( RELAY_OFF );	
			ICComm.RelayOFFState = 0x0;
			ICComm.RelayONState = 0x00;				//��բ��ȡ�������բ״̬	//�¹���	//13.10.25
			Para.RMeterRelayState = 0x1A;			//�Ѿ���բ
			E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//			CreateTripRec( TripOFF_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			EVar.RelayFaultVar.Cunt = 1;														//�¹���	//14.05.03
			EVar.RelayFaultVar.OldState = 0xff;													//�¹���	//14.05.03	
			E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//�¹���	//14.05.12	
			Flag.RSwitchOldState &= 0x04;														//�¹���	//14.05.03	
			Flag.RSwitchOldState |= 0x40;			//�����¼�쳣�¼�							//�¹���	//14.05.12							
			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
#endif
			RecordEndEvent( TripOFF_No, 0 );
#if ( IOver30ALimitTrip	== YesCheck )		//11.06.29
		}
#endif
	}				
}		
	
void CreatICPrgRec( unsigned char CardType )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	
	Ptr = Buff;	
	
	*Ptr = ICComm.ParaUpdateFlag; 
	*(Ptr+1) = CardType;		
	*(Ptr+2) = 0x98;		
	*(Ptr+3) = 0x99;			
//	CreatPrgRec( Ptr, PARAM, Para.CardNo, 2 );	
	EndPrgRecEvent( 0 );
	CreatPrgRecEvent( Ptr );

}

void Clr_AccountFlag( void )										//�¹���	//13.12.05	
{
	ICComm.ROpenAccountFlag = 0x03;									
	E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );			//�忪�����
}

void Clr_ReOverEC( void )
{
	Clr_RemainEC();
	Clr_E2PData( OverdraftEC, 4, 1 );
}	
	
void Clr_RemainEC( void )
{	
	RAM_Fill( ICEC.RConsumECSumBCD, 8 );
	ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	E2P_WFM( FConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );
	
	Clr_E2PData( BuyECSumBCD, 8, 2 );
	Clr_E2PData( CurrentBuyEC, 4, 1 );
}



void IC_Prepare( void )		//Ԥ�ÿ�					//�¹���		//13.09.10
{
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char RandomData[8];						//�¹���	//13.11.23				
	unsigned char Para1Data[20];						//�¹���	//13.11.23					
	unsigned char Para2Data[20];						//�¹���	//13.11.23				
	unsigned char CPrice1Data[80];						//�¹���	//13.11.23				
	unsigned char CPrice2Data[80];						//�¹���	//13.11.23					
	unsigned char BPrice1Data[80];						//�¹���	//13.11.23				
	unsigned char BPrice2Data[80];						//�¹���	//13.11.23							
	unsigned char CStepData[80];						//�¹���	//13.11.23				
	unsigned char BStepData[80];						//�¹���	//13.11.23					

	unsigned char Buff[60];								//�¹���	//13.11.23						
	unsigned char Buff2[10];
	unsigned char ParaUpdateFlag;
	short i;
    
	Point = Buff2;
	
	ParaUpdateFlag = ICComm.ICBuf1[5];
	ICComm.ParaUpdateFlag = ParaUpdateFlag;

//	if( ReadBinary( PSAM, Point, FILE_No5, 0x04, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; ICComm.ICAlarmFlg = ICALARM_28; return; }		//��PSAMǮ���ļ��ڵĹ������	//�¹���	//14.01.06
//	if( IsAllData( Point, 4, 0x00 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_34; ICComm.ErrorMessage = ICERROR_17; ClearOperateCommHead_ErrorResponse(); return; }		//�����ļ��ڵĹ������������0			//�¹���	//13.11.23	//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02

	ICComm.RelayTestDelay = F_NULL; 	

	Ptr = Buff;

	Clr_AccountFlag();
	Clr_ReOverEC();

	RAM_Fill( Ptr, 8);
	E2P_WData( BfERemainEC, Ptr, 4 );																//��ʼ��ǰʣ�����Ϊ0	//�¹���	//13.11.26

	if( GetChallenge( ESAM, RandomData, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//�¹���	//14.01.06
	RAM_Write( Ptr, RandomData, 4 );
//	if( ReadBinaryRecMAC( PSAM, Ptr, 0x05, 0x01, 0x00 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//��Ǯ���ļ����		//�¹���	//13.11.23	//14.01.06
	if( ReadBinaryRecMAC( PSAM, Ptr, 0x02, 0x01, 0x00 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//��Ǯ���ļ����		//�¹���	//13.11.23	//14.01.06
	if( UpdateBinaryRecMAC( ESAM, Ptr, 0x01 ) != 0 ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 );  return; }	//дǮ���ļ����		//�¹���	//13.12.10

	AddBuyECSumBCD( Ptr );						 //�ۼ��ۼƹ�����		

//Ԥ�ÿ��Ĺ�����������裿ȱʡΪ0��
//	RAM_Write( Ptr, RandomData, 4 );
//	if( ReadBinaryRecMAC( PSAM, Ptr, 0x05, 0x03, 0x04 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//���������			//�¹���	//13.11.23	//14.01.06
//	if( UpdateBinaryRecMAC( ESAM, Ptr, 0x03 ) != 0 ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return; }		//д�������			//�¹���	//13.12.10

//	SwapData( Ptr, 4 );																				
	RAM_Fill( Ptr, 4);							//Ԥ�ÿ��������Ϊ0
	E2P_WData( BuyECCnt, Ptr, 4 );																	
			
	RAM_Fill( Ptr, 8);
	E2P_WData( UserCardNo, Ptr, 8 ); 						//�忨���к�
	E2P_WData( ILInsert_Cunt, Ptr, 3 );												//�¹���	//13.11.26				
	E2P_WData( BuyECFlag, Ptr, 1 );								//����ɹ�			//�¹���	//13.11.26		
	ClearJudgePermitFlag();					//����к�բ�������־	//�¹���	//13.12.10

	Para.RMeterTripState = 0x1B;													//�¹���	//13.11.26		
	E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );			//�¹���	//13.11.26		

	Para.RMeterAlarmState = 0x2B;													//�¹���	//13.11.26				
	E2P_WData( MeterAlarmState, (unsigned char*)&Para.RMeterAlarmState, 1 );		//�¹���	//13.11.26		

	RAM_Fill( ICEC.LocalDisperseGene, 8);											//�¹���	//13.11.26		
//	E2P_RData( ICEC.LocalDisperseGene, EUser_No, 6 );								//�¹���	//13.11.26		
	E2P_RData( ICEC.LocalDisperseGene, EMeter_No, 8 );								//�¹���	//13.11.26		
	SwapData( ICEC.LocalDisperseGene, 8 );											//�¹���	//13.11.26		

	if(( ParaUpdateFlag & 0x80 ) != 0 )				//������������					//�¹���	//13.11.23				
	{
		RAM_Write( Para1Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No2, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					
		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No4, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					
	}	

	if(( ParaUpdateFlag & 0x01 ) != 0 )				//���µ�ǰ�׷���				//�¹���	//13.11.22
	{
		RAM_Write( CPrice1Data, RandomData, 4 );
		RAM_Write( CPrice2Data, RandomData, 4 );		
//		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No3, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					 
		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No5, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					 
//		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No3, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					  
		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No5, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					  
	}
	if(( ParaUpdateFlag & 0x02 ) != 0 )				//���±����׷���				//�¹���	//13.11.22
	{
		RAM_Write( BPrice1Data, RandomData, 4 );
		RAM_Write( BPrice2Data, RandomData, 4 );
		RAM_Write( Para2Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No4, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					   
		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No6, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					   
//		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No4, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No6, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
//		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No2, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06  		//���׷�ʱ�����л�ʱ��
		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No4, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06  		//���׷�ʱ�����л�ʱ��
	}

	if(( ParaUpdateFlag & 0x04 ) != 0 )				//���µ�ǰ�׽���				//�¹���	//13.11.22
	{
		RAM_Write( CStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, CStepData, FILE_No3, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
		if( ReadBinaryMAC( PSAM, CStepData, FILE_No5, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//���µ�ǰ�׽���				//�¹���	//13.11.22
	{
		RAM_Write( BStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BStepData, FILE_No4, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06  
		if( ReadBinaryMAC( PSAM, BStepData, FILE_No6, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06  
	}

	CardProceIntegrality(); 				//����������������������ο�				//�¹���	//14.01.04					

	if(( ParaUpdateFlag & 0x80 ) != 0 )				//������������
	{
//		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No2, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.03								
		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No4, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.03								

		SwapData( Para1Data, 4 );
		E2P_WData( AlarmEC1, Para1Data, 4 );
		SwapData( Para1Data+4, 4 );
		E2P_WData( AlarmEC2, Para1Data+4, 4 );
		SwapData( Para1Data+8, 3 );
//		E2P_WData( CurrentRate, Para1Data+8, 3 );
		WriteVoltCurrentRate( CurrentRate, Para1Data+8 );
		SwapData( Para1Data+11, 3 );
//		E2P_WData( VoltRate, Para1Data+11, 3 );
		WriteVoltCurrentRate( VoltRate, Para1Data+11 );
	}

	GetBackBuyECFile1( Ptr );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 0, 49 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }		//ȫ������д��ESAM��������Ϣ�ļ�	//�¹���	//14.01.06

	if(( ParaUpdateFlag & 0x01 ) != 0 )				//���µ�ǰ�׷���				//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No3, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No5, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No3, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No5, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+16*5+i*5, Point, 4 );								
		}	
		GetFeePriceNum();													//�¹���	//13.11.30	
	}

	if(( ParaUpdateFlag & 0x02 ) != 0 )				//���±����׷���				//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No4, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No6, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No4, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No6, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+16*5+i*5, Point, 4 );								
		}	

//		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No2, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No4, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		RAM_Write( Point, Para2Data, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( FeeSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//�����÷�ʱ�����л�ʱ��󣬲���ʱ����Ƿ���ȣ������ݵ�ǰʱ�������ж��Ƿ��л�						
//		E2P_WData( OldFeeSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( FeeSwitchTime, OldFeeSwitchTime, Point );
		FeeTabSwitch();

	}

	if(( ParaUpdateFlag & 0x04 ) != 0 )				//���µ�ǰ�׽���				//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No3, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No5, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, CStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			E2P_RData( Ptr+i*3, Case1YearSaveDay1+i*4, 3 );	
			RAM_Write( Point, CStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case1YearSaveDay1+i*4, Point, 3 );									
			RAM_Write( Ptr+12+i*3, Point, 3 );
		}
		YearMonSaveDaySwitchProcess( Ptr, Ptr+12 );			
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//���±����׽���				//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No4, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No6, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, BStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			RAM_Write( Point, BStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case2YearSaveDay1+i*4, Point, 3 );									
		}	

		RAM_Write( Point, BStepData+64, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( StepSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//�����÷�ʱ�����л�ʱ��󣬲���ʱ����Ƿ���ȣ������ݵ�ǰʱ�������ж��Ƿ��л�						
//		E2P_WData( OldStepSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( StepSwitchTime, OldStepSwitchTime, Point );
		StepTabSwitch();

	}

	GetECState();                                               		//11.07.22

	if(( ICComm.ICECState == F_ECAlarm20 )||( ICComm.ICECState == F_ECOver ))	//ȡ��Ԥ��բ	//�¹���	//13.12.23	
	{
		ExitPreTripState();
		ICComm.RelayOFFState = 0;
		ICComm.RelayONState = 0x00;
	}
	
	ClearJudgePermitFlag();										//�¹���	//13.12.10    					

	ICComm.ICDisState = ICDisRemainEC;					//����ʾʣ�����
	ICComm.ICInState &= ~F_RegECLimt;
}	

/*
void CreatBuyECRec( void )						//���������¼
{
	unsigned char Buff[25];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue;
	long LastPt;
	
	Ptr = Buff;	
	
	LastPt = PtAddrChk( BuyMoney_No, Next );				
	if( LastPt == -1 ) return;
	RAM_Write( Ptr, Clk.Min, 5 );
	E2P_RData( (unsigned char*)&Value, BuyECCnt, 4 );
	Long_BCD4( Ptr+5, Value );				
	E2P_RData( (unsigned char*)&Value, CurrentBuyEC, 4 );
	Long_BCD4( Ptr+7, Value );				
	E2P_RData( (unsigned char*)&Value, BfERemainEC, 4 );
	Long_BCD4( Ptr+11, Value );				
	GetReOverEC( Ptr+15, Ptr+19 );
	RAM_Write( (unsigned char*)&Value, Ptr+15, 4 );
	Long_BCD4( Ptr+15, Value );				
	E2P_RData( (unsigned char*)&LValue, BuyECSumBCD, 8 );
	Value = LValue / 10000;
	Long_BCD4( Ptr+19, Value );				
	E2P_WData( LastPt, Ptr, 23 );
}	
*/

unsigned long GetBuyECSum( void ) 							//ȡ�ۼƹ�����
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue;
	
	Ptr = Buff;
	
	E2P_RData( Ptr, BuyECSumBCD, 8 );						//�ۼƹ����� XXXXXXXXXX.XXXXXX Ԫ
	RAM_Write( (unsigned char*)&LValue, Ptr, 8 );
	LValue /= 10000;
	Value = (unsigned long)LValue;							//�����ۼƹ����� XXXXXX.XX Ԫ	

	return Value;	
}	


							//remote
void AddBuyECSumBCD( unsigned char* Source ) 		//�ۼ��ۼƹ�����
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue1;
	unsigned long long LValue2;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Source, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value, Ptr, 4 );
	E2P_WData( CurrentBuyEC, Ptr, 4 );						//���ι�����

	E2P_RData( Ptr, BuyECSumBCD, 8 );						//�ۼƹ����� XXXXXXXXXX.XXXXXX Ԫ
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );
	LValue2 = Value;
	LValue2 *= 10000;
	LValue1 += LValue2;
	E2P_WData( BuyECSumBCD, (unsigned char*)&LValue1, 8 );	//�ۼƹ����� XXXXXXXXXX.XXXXXX Ԫ
	
}	

void DecBuyECSumBCD( unsigned char* Source ) 		//�˷��ۼƹ�����		//�¹���	//13.11.27
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue1;
	unsigned long long LValue2;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Source, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value, Ptr, 4 );
//	Long_BCD4( Ptr, Value );
//	E2P_WData( EFeeBack, Ptr, 4 );							//�����˷ѽ��

	E2P_RData( Ptr, BuyECSumBCD, 8 );						//�ۼƹ����� XXXXXXXXXX.XXXXXX Ԫ
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );
	LValue2 = Value;
	LValue2 *= 10000;
	if( LValue1 >= LValue2 )
	{
		LValue1 -= LValue2;
		E2P_WData( BuyECSumBCD, (unsigned char*)&LValue1, 8 );	//�ۼƹ����� XXXXXXXXXX.XXXXXX Ԫ
	}		
}	

short WritePSAMBackInfoFile( unsigned char* Source )		//��д�û�����д��Ϣ�ļ�		//�¹���	//14.01.06
{
	GetBackBuyECFile1( Source );
	if( UpdateBinary( ESAM, Source, FILE_No7, 0, 49 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//ȫ������д��ESAM��������Ϣ�ļ�	//�¹���	//13.01.06

	if( GetChallenge( PSAM, Source, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }		//�¹���	//13.01.06											
//	if( ReadBinaryMAC( ESAM, Source, FILE_No5, FILE_No7, 0x00, 0x00, 49+4, 49+4, 1, Para.CardNo ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//�¹���	//13.01.06 
	if( ReadBinaryMAC( ESAM, Source, FILE_No5, FILE_No7, 0x00, 0x00, 49, 49+4, 1, Para.CardNo ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//�¹���	//13.01.06 
	if( UpdateBinaryMAC( PSAM, Source, FILE_No5, 0x00, 49+4, 0, Para.CardNo ) ) { ICComm.ErrorMessage = ICERROR_10; return -1; }																	

	return 0;
}	
							//remote
void IC_BuyEC( void )		//���翨					//�¹���	//13.11.23				
{
	unsigned char Buff[60];								//�¹���	//13.11.23	
	unsigned char Buff2[20];
	unsigned char RandomData[8];						//�¹���	//13.11.23				
	unsigned char Para1Data[20];						//�¹���	//13.11.23					
	unsigned char Para2Data[20];						//�¹���	//13.11.23				
	unsigned char CPrice1Data[80];						//�¹���	//13.11.23				
	unsigned char CPrice2Data[80];						//�¹���	//13.11.23					
	unsigned char BPrice1Data[80];						//�¹���	//13.11.23				
	unsigned char BPrice2Data[80];						//�¹���	//13.11.23							
	unsigned char CStepData[80];						//�¹���	//13.11.23				
	unsigned char BStepData[80];						//�¹���	//13.11.23					
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned long Temp1=0,Temp2;
	unsigned long Value,Value2,Value3;
	unsigned char BuyState=0x55;
	unsigned char ParaUpdateFlag;
	unsigned char CardType;
	short i;
    
	Point = Buff2;
	Ptr = Buff;
	
	ParaUpdateFlag = ICComm.ICBuf1[5];
	ICComm.ParaUpdateFlag = ParaUpdateFlag;    
	    
	RAM_Write( Ptr, ICComm.ICBuf1, 44 );	//ͨѶ�������ݰ����ڴ�
	CardType = *(Ptr+42);					//�û�������	

															//�¹���	//13.12.05							
//	E2P_RData( Point, EUser_No, 6 );						
	E2P_RData( Point, EMeter_No, 8 );						
	if( Data_Comp( Point, Ptr+30, 6 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_12; ICComm.ErrorMessage = ICERROR_11; ClearOperateCommHead_ErrorResponse(); return; }	//�˶Ա�Ŵ�	//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.10

	if(( *(Ptr+42) != 0x01)&&( *(Ptr+42) != 0x02 )&&( *(Ptr+42) != 0x03 ))		//�����ʹ�		
	{ 
		ICComm.ErrorMessage = ICERROR_08;					//�¹���		//13.12.03
		ICComm.ICAlarmFlg = ICALARM_26; 
		ClearOperateCommHead_ErrorResponse(); 				//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02
		return; 
	}

															//�¹���	//13.12.05									
	if( ReadBinary( PSAM, Point, FILE_No2, 0x04, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }		//��PSAMǮ���ļ��ڵĹ������	//�¹���	//14.01.06
	SwapData( Point, 4 );
	RAM_Write( (unsigned char*)&Temp2, Point, 4 );			//PSAM�Ĺ������
			
	E2P_RData( (unsigned char*)&ICComm.ROpenAccountFlag, OpenAccountFlag, 1 );
	ICComm.ROpenAccountFlag &= 0x03;											//ȡ���ء�Զ�̿���״̬		//�¹���		//13.12.05
	if(( ICComm.ROpenAccountFlag == 0x03 )&&( CardType != 0x01 ))				//δ������ֻ�Ͽ�����		//�¹���		//13.12.05
	{
		if( CardType == 0x02 )	ICComm.ErrorMessage = ICERROR_15;		//�¹���		//13.12.03
		else ICComm.ErrorMessage = ICERROR_16;							//�¹���		//13.12.03
		ICComm.ICAlarmFlg = ICALARM_22; 
		ClearOperateCommHead_ErrorResponse(); 				//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02
		return; 	
	}	
	
	if(( CardType == 0x01)&&( ICComm.ROpenAccountFlag == 0x03 )) ;				//�¿�������δ�������˶Կͻ����				//�¹���		//13.12.05
	else	
	{
		E2P_RData( Point, Client_No, 6 );
		if( Data_Comp( Point, Ptr+36, 6 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_12; ICComm.ErrorMessage = ICERROR_12; ClearOperateCommHead_ErrorResponse(); return; }				//�˶Կͻ���Ŵ�	//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02
		if( CardType == 0x02 )								//�ѿ����Ĺ��翨Ҫ�п����к�					//�¹���		//13.12.10	
		{
			E2P_RData( Point, UserCardNo, 8 );
			if( Data_Comp( Point, Para.CardNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_13; ClearOperateCommHead_ErrorResponse(); return; }				//�˶Կ����кŴ�	//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.10	
		}	
	}

	if(( CardType == 0x01 )&&( Temp2 > 1 ))					//���������������Ϊ0��1������				//�¹���		//13.12.05
	{
		ICComm.ErrorMessage = ICERROR_17; 						
		ClearOperateCommHead_ErrorResponse(); 
		return; 		
	}		
															//�¹���	//13.12.05									
//	if( ReadRecord( ESAM, Point, FILE_No3, 0x0C, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//��ESAMǮ���ļ��ڵĹ������	//�¹���	//14.01.06
	if( ReadRecord( ESAM, Point ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//��ESAMǮ���ļ��ڵĹ��������������ͻ����	//�¹���	//14.01.06
	RAM_Write( Point, Point+4, 4 );														//��ESAMǮ���ļ��ڵĹ������	//�¹���	//14.01.06				
	SwapData( Point, 4 );
	RAM_Write( (unsigned char*)&Temp1, Point, 4 );			//ESAM�Ĺ������
	
	if( Temp2 < Temp1 ) 									//���ڹ������С�ڱ��ڹ��������������			//�¹���	//13.12.05
	{ 
		if(( CardType == 0x01 )||( CardType == 0x03 ))		//�������򲹿����濨���к�	
		{
			E2P_WData( UserCardNo, Para.CardNo, 8 ); 		
		}
		if( CardType == 0x01 )
		{
			ICComm.ROpenAccountFlag &= ~0x01;									//�ñ��ؿ���״̬		
			E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );
		}
		
		if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//��д�û�����д��Ϣ�ļ�				//�¹���	//13.12.05	//14.01.06
		ICComm.ICDisState = ICDisRemainEC;			//�����忨��������ʾʣ�����  			//�¹���	//13.12.05
		ICComm.ParaUpdateFlag = 0;					//������û�����δ���²���ȴ���ӱ��	//�¹���	//14.01.06
		return; 
	}				

	if( Temp2 >= (Temp1+2) )	
	{
		if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//��д�û�����д��Ϣ�ļ�		//�¹���	//13.12.05	//14.01.06				
		ICComm.ErrorMessage = ICERROR_17; 
		ClearOperateCommHead_ErrorResponse();
		return;			
	}	

//	if( ReadBinary( PSAM, Point, FILE_No5, 0x00, 3 ) != 0 ) return;		//���������Ϣ�ļ��ж��Ƿ񿪻���
	if( ReadBinary( PSAM, Point, FILE_No5, 0x05, 3 ) != 0 ) return;		//���������Ϣ�ļ��ж��Ƿ񿪻���	//�������ֻ�������������δ�����ļ����	//Test
	if( IsAllData( Point, 3, 0x0 ) == 0 ) BuyState = 0; 				//��д�ļ�Ϊ��					//�¹���	//13.12.05		

//    BuyState = 0; 				//��д�ļ�Ϊ��(�������δ���)			//16.11.28      //Test
    
	if( Temp2 == (Temp1+1) )							//�¹���	//13.12.05									
	{
		if( BuyState != 0 )
		{
			if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//��д�û�����д��Ϣ�ļ�		//�¹���	//13.12.05	//14.01.06				
			ICComm.ErrorMessage = ICERROR_18; 
			ClearOperateCommHead_ErrorResponse();
			return;
		}	
	}
			
	if(( CardType == 0x01 )||( CardType == 0x03 ))												//�¹���	//13.12.05
	{
		if(( Temp2 == Temp1 )&&( BuyState != 0 ))
		{
			E2P_RData( Point, UserCardNo, 8 );														
			if( Data_Comp( Point, Para.CardNo, 8 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_12; ICComm.ErrorMessage = ICERROR_13; ClearOperateCommHead_ErrorResponse(); return; }	//�û������кŴ�	//��ͨѶԭ�������������ͷ��������Ӧ״̬		//�¹���	//13.12.02		
			else 
			{
				if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//��д�û�����д��Ϣ�ļ�		//�¹���	//13.12.05		//14.01.06			
				ICComm.ICDisState = ICDisRemainEC;			//�����忨��������ʾʣ�����  //�¹���	//13.12.05
				ICComm.ParaUpdateFlag = 0;					//����忪������������δ���²���ȴ���ӱ��	//�¹���	//14.01.06
				return;
			}	
		}						
	}	
	if( CardType == 0x02 )												//�¹���	//13.12.05
	{
		if(( Temp2 == Temp1 )&&( BuyState != 0 ))
		{
			if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//��д�û�����д��Ϣ�ļ�		//�¹���	//13.12.05	//14.01.06				
			ICComm.ICDisState = ICDisRemainEC;			//�����忨��������ʾʣ�����  	//�¹���	//13.12.05		
			ICComm.ParaUpdateFlag = 0;					//������û�����δ���²���ȴ���ӱ��	//�¹���	//14.01.06
			return;																				//�¹���	//14.01.06	
		}
	}
	


	ICComm.RelayTestDelay = F_NULL; 					//V7
	if( Temp2 == (Temp1+1) )
	{

		GetReOverEC( Point, Point+4 );								//ʣ�����

		if( ReadBinary( PSAM, Point+8, FILE_No2, 0x00, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }		//��PSAMǮ���ļ��ڵĹ�����	//�¹���	//14.01.06
		SwapData( Point+8, 4 );
		RAM_Write( (unsigned char*)&Value, Point, 4 );
		RAM_Write( (unsigned char*)&Value2, Point+8, 4 );
		Value += Value2;												//ʣ������ӱ��ι�����
		
		E2P_RData( Point+12, RegrateECLimt, 4 );
		Value2 = BCD4_Long( Point+12 );	
		if( Value2 == 0 ) Value2 = 99999999;						//�ڻ��޶�=0������999999.99Ԫ����	//�¹���	//13.12.10
		RAM_Write( (unsigned char*)&Value3, Point+4, 4 );
		Value2 += Value3;											//�ڻ��޶�+͸֧����	

		if( Value <= Value2 )
		{

			E2P_WData( BfERemainEC, Point, 4 );						//��������ǰʣ�����
			E2P_WData( BfOverdraftEC, Point+4, 4 );					//��������ǰ͸֧����
			
			ICComm.ICDisState = ICDisBfRemainEC;					//����ʾ����ǰ����
			ICComm.ICInState &= ~F_RegECLimt;

			if( GetChallenge( ESAM, Ptr, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//���������������д��ESAMǮ���ļ�	//�¹���	//14.01.06
			if( ReadBinaryIncMAC( PSAM, Ptr, FILE_No2, 0x00 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; } 	//�¹���	//14.01.06
			if( UpdateBinaryIncMAC( ESAM, Ptr, Para.CardNo ) != 0 ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 );  return ; }		//�¹���	//14.01.02

			RecordStartEvent( BuyMoney_No );
			AddBuyECSumBCD( Ptr );					 				//�ۼ��ۼƹ�����		
			
			Value = Temp1 + 1;
			E2P_WData( BuyECCnt, (unsigned char*)&Value, 4 );
			*Point = 0x0;
			E2P_WData( BuyECFlag, Point, 1 );							//����ɹ�
//			CreatBuyECRec();						//���������¼
			RecordEndEvent( BuyMoney_No, 0 );

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			ICComm.PreTripOFFKeepTime = 0; 																			//�¹���	//14.06.22	
			if(( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E )) Para.RMeterTripState = 0x1B;		//�¹���	//14.06.22	
#endif

		}
		else 
		{
			ICComm.ICInState |= F_RegECLimt;
			ICComm.ErrorMessage = ICERROR_21;					//�¹���	//13.12.05
			return;												//�¹���	//13.12.05
		}	
		ICComm.ICInState |= F_BuyECOK;				//V8	//���۹����Ƿ�ɹ�������ʾ��ǰ������	
	}	

	if( CardType != 0x01 ) 
	{
		ICComm.ParaUpdateFlag &= 0x0A;	//�ǿ�����ֻ֧���޸ı����׷��ʵ�ۺͱ����׽��ݲ���		//�¹���	//13.11.28				
		ParaUpdateFlag &= 0x0A;			//�ǿ�����ֻ֧���޸ı����׷��ʵ�ۺͱ����׽��ݲ���		//�¹���	//14.01.06				
	}	

	if( GetChallenge( ESAM, RandomData, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//�¹���	//14.01.06				
	if( CardType == 0x01 )							//���¿ͻ����					//�¹���	//13.12.05				
	{
		RAM_Write( Ptr, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, Ptr, FILE_No2, FILE_No1, 36, 36, 6+4, 6+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 	
		if( ReadBinaryMAC( PSAM, Ptr, FILE_No15, FILE_No1, 6, 36, 6+4, 6+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 	
//		if( UpdateBinaryMAC( ESAM, Ptr, FILE_No2, 36, 6+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Ptr, FILE_No15, 6, 6+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		
		SwapData( Ptr, 6 );
		E2P_WData( Client_No, Ptr, 6 );
	}	

	if((( ParaUpdateFlag & 0x80 ) != 0 )&&( CardType == 0x01 ))	//������������		//�¹���	//13.11.23				
	{
		RAM_Write( Para1Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No2, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					
		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No4, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 					
	}	

	if((( ParaUpdateFlag & 0x01 ) != 0 )&&( CardType == 0x01 ))	//���µ�ǰ�׷���	//�¹���	//13.11.22
	{
		RAM_Write( CPrice1Data, RandomData, 4 );
		RAM_Write( CPrice2Data, RandomData, 4 );		
//		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No3, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No5, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
//		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No3, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No5, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
	}
	if(( ParaUpdateFlag & 0x02 ) != 0 )				//���±����׷���				//�¹���	//13.11.22
	{
		RAM_Write( BPrice1Data, RandomData, 4 );
		RAM_Write( BPrice2Data, RandomData, 4 );
		RAM_Write( Para2Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No4, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No6, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
//		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No4, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No6, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 
//		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No2, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 		//���׷�ʱ�����л�ʱ��
		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No4, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06 		//���׷�ʱ�����л�ʱ��
	}

	if((( ParaUpdateFlag & 0x04 ) != 0 )&&( CardType == 0x01 ))	//���µ�ǰ�׽���	//�¹���	//13.11.22
	{
		RAM_Write( CStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, CStepData, FILE_No3, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06
		if( ReadBinaryMAC( PSAM, CStepData, FILE_No5, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//���±����׽���				//�¹���	//13.11.22
	{
		RAM_Write( BStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BStepData, FILE_No4, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06
		if( ReadBinaryMAC( PSAM, BStepData, FILE_No6, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//�¹���	//14.01.06
	}

	if((( ParaUpdateFlag & 0x80 ) != 0 )&&( CardType == 0x01 ))	//������������
	{
//		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No2, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No4, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		SwapData( Para1Data, 4 );
		E2P_WData( AlarmEC1, Para1Data, 4 );
		SwapData( Para1Data+4, 4 );
		E2P_WData( AlarmEC2, Para1Data+4, 4 );
		SwapData( Para1Data+8, 3 );
//		E2P_WData( CurrentRate, Para1Data+8, 3 );
		WriteVoltCurrentRate( CurrentRate, Para1Data+8 );
		SwapData( Para1Data+11, 3 );
//		E2P_WData( VoltRate, Para1Data+11, 3 );
		WriteVoltCurrentRate( VoltRate, Para1Data+11 );
	}

	if( WritePSAMBackInfoFile( Ptr ) != 0 ) return;		//��д�û�����д��Ϣ�ļ�				//�¹���	//14.01.04	//14.01.06				
	CardProceIntegrality(); 			//����������������������ο�			//�¹���	//14.01.04					

	if((( ParaUpdateFlag & 0x01 ) != 0 )&&( CardType == 0x01 ))					//���µ�ǰ�׷���	//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No3, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No5, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

//		CreateFeePricePrgRec( FeePricePrg_No );									//�¹���	//13.11.26
		RecordStartEvent( FeePricePrg_No );

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No3, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No5, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+16*5+i*5, Point, 4 );								
		}	
		GetFeePriceNum();													//�¹���	//13.11.30	
	}

	if(( ParaUpdateFlag & 0x02 ) != 0 )				//���±����׷���				//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No4, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No6, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

//		CreateFeePricePrgRec( FeePricePrg_No );									//�¹���	//13.11.26
		RecordStartEvent( FeePricePrg_No );
		
		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No4, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No6, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+16*5+i*5, Point, 4 );								
		}	

//		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No2, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No4, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		RAM_Write( Point, Para2Data, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( FeeSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//�����÷�ʱ�����л�ʱ��󣬲���ʱ����Ƿ���ȣ������ݵ�ǰʱ�������ж��Ƿ��л�						
//		E2P_WData( OldFeeSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( FeeSwitchTime, OldFeeSwitchTime, Point );
		FeeTabSwitch();

	}

	if((( ParaUpdateFlag & 0x04 ) != 0 )&&( CardType == 0x01 ))					//���µ�ǰ�׽���				//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No3, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No5, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

//		CreateStepParaPrgRec( StepParaPrg_No );									//�¹���	//13.11.26
		RecordStartEvent( StepParaPrg_No );
			
		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, CStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			E2P_RData( Ptr+i*3, Case1YearSaveDay1+i*4, 3 );	
			RAM_Write( Point, CStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case1YearSaveDay1+i*4, Point, 3 );									
			RAM_Write( Ptr+12+i*3, Point, 3 );
		}
		YearMonSaveDaySwitchProcess( Ptr, Ptr+12 );			
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//���±����׽���			//�¹���	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No4, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No6, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//�¹���	//13.12.10								

//		CreateStepParaPrgRec( StepParaPrg_No );									//�¹���	//13.11.26
		RecordStartEvent( StepParaPrg_No );
		
		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, BStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			RAM_Write( Point, BStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case2YearSaveDay1+i*4, Point, 3 );									
		}	

		RAM_Write( Point, BStepData+64, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( StepSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//�����÷�ʱ�����л�ʱ��󣬲���ʱ����Ƿ���ȣ������ݵ�ǰʱ�������ж��Ƿ��л�						
//		E2P_WData( OldStepSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( StepSwitchTime, OldStepSwitchTime, Point );
		StepTabSwitch();

	}

	if(( ICComm.ICInState & F_RegECLimt ) != 0 ) ICComm.ICAlarmFlg = ICALARM_14;

	if(( CardType == 0x01 )||( CardType == 0x03 ))		//�������򲹿����濨���к�		//�¹���	//13.12.05					
	{
		E2P_WData( UserCardNo, Para.CardNo, 8 ); 		
	}
	if( CardType == 0x01 )																//�¹���	//13.12.05						
	{
		ICComm.ROpenAccountFlag &= ~0x01;									//�ñ��ؿ���״̬		
		E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );
	}
	
	GetReOverEC( Point, Point+4 );								//ʣ�����
	GetICECState( Point, Point+4 );
	if( ICComm.RelayONState == 0xAA ) ICComm.RelayONState = 0x55;	//�������բ״̬�£��忨ֱ�Ӻ�բ

	ICComm.ICInState |= ( F_BuyEC + F_BuyECOver );												//V7
	SaveBuyECState();
	if( ICComm.ICDisState != ICDisBfRemainEC ) ICComm.ICDisState = ICDisRemainEC;		//����ʾʣ�����  //������� 11.06.27

}

//EEPROM�ڵ��ֳ��������ÿ��汾�ź������֤ʱЧ�Բ�������
//��ESAM�ڵĲ���Ϊ׼,4����
void GetAndCompareESAMPara( void )
{
	unsigned char Buff[50];
	unsigned char Buff2[10];
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned long Value1;
	unsigned long Value2;
	unsigned short Len;
	unsigned short Length;
    
	Ptr = Buff;
	Point = Buff2;

//	if( ReadBinary( ESAM, Ptr, FILE_No5, 00, 4 ) == 0 ) 		//�¹���
//	{	
//		SwapData( Ptr, 4 );
//		E2P_RData( Point, ESecretKeyState, 4 );						
//		if( Data_Comp( Ptr, Point, 4 ) != 0 )
//		{
//			E2P_WData( ESecretKeyState, Ptr, 4 );				//������Կ��Ϣ	
//			RAM_Write( Flag.SecretKeyState, Ptr, 4 );
//		}	
//	}
	
	GetCurSecretKeyState();
	
//	if( ReadBinary( ESAM, Ptr, FILE_No2, 0x04, 38 ) == 0 ) 
	if( ReadBinary( ESAM, Ptr, FILE_No4, 0x04, 38 ) == 0 ) 
	{	
		E2P_RData( Point, UserType, 1 );						//�û�����
		if( *Ptr != *Point ) E2P_WData( UserType, Ptr, 1 );		
		
//		SwapData( Ptr+6, 5 );
//		E2P_RData( Point, FeeSwitchTime, 5 );					//��ʱ�����л�ʱ��
//		if( Data_Comp( Ptr+6, Point, 5 ) != 0 )
//		{
//			E2P_WData( FeeSwitchTime, Ptr+6, 5 );					
//		}	

		SwapData( Ptr+6, 5 );
		E2P_RData( Point, FeeSwitchTime, 7 );					//��ʱ�����л�ʱ��
		ChangeTimeHexToBCD( Point, Point );
		if( Data_Comp( Ptr+6, Point+1, 5 ) != 0 )
		{
			RAM_Write( Point+1, Ptr+6, 5 );
			*Point = 0;
			if( IsAllData( Point+1, 5, 0x00 ) == 0 ) *(Point+6) = 0;
			else ChangeTimeBCDToHex( Point, Point );
			E2P_WData( FeeSwitchTime, Point, 7 );					
		}	
		
		SwapData( Ptr+12, 4 );
		E2P_RData( Point, AlarmEC1, 4 );						
		if( Data_Comp( Ptr+12, Point, 4 ) != 0 )
		{
			E2P_WData( AlarmEC1, Ptr+12, 4 );					
		}	
	
		SwapData( Ptr+16, 4 );
		E2P_RData( Point, AlarmEC2, 4 );						
		if( Data_Comp( Ptr+16, Point, 4 ) != 0 )
		{
			E2P_WData( AlarmEC2, Ptr+16, 4 );					
		}	
	
//		SwapData( Ptr+20, 3 );
//		E2P_RData( Point, CurrentRate, 3 );						
//		if( Data_Comp( Ptr+20, Point, 3 ) != 0 )
//		{
//			E2P_WData( CurrentRate, Ptr+20, 3 );					
//		}	

		SwapData( Ptr+20, 3 );
		Value1 = BCD3_Long( Ptr+20 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, CurrentRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( CurrentRate, (unsigned char*)&Value1, 4 );					
		}	

//		SwapData( Ptr+23, 3 );
//		E2P_RData( Point, VoltRate, 3 );						
//		if( Data_Comp( Ptr+23, Point, 3 ) != 0 )
//		{
//			E2P_WData( VoltRate, Ptr+23, 3 );					
//		}	

		SwapData( Ptr+23, 3 );
		Value1 = BCD3_Long( Ptr+23 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, VoltRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( VoltRate, (unsigned char*)&Value1, 4 );					
		}	
	
//		SwapData( Ptr+26, 6 );
//		E2P_RData( Point, EUser_No, 6 );						
//		if( Data_Comp( Ptr+26, Point, 6 ) != 0 )
//		{
//			E2P_WData( EUser_No, Ptr+26, 6 );					
//		}	
	
//		SwapData( Ptr+32, 6 );
//		E2P_RData( Point, Client_No, 6 );						
//		if( Data_Comp( Ptr+32, Point, 6 ) != 0 )
//		{
//			E2P_WData( Client_No, Ptr+32, 6 );					
//		}	
	}
	
//	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x05, 8 ) == 0 ) 		
//	{
//		SwapData( Ptr, 8 );
//		E2P_RData( Point, EMeter_No, 8 );						
//		if( Data_Comp( Ptr, Point, 8 ) != 0 )
//		{
//			E2P_WData( EMeter_No, Ptr, 8 );					
//		}	
//	}	

	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x04, 9 ) == 0 ) 	//��ȡ1�ֽڳ����ֽ�
	{
		Len = *Ptr;
		if( Len < 8 ) Length = 8 - Len;
		else 
		{
			Length = 0;
			Len = 8;
		}	
		SwapData( Ptr+1, Len );
		RAM_Fill( Ptr+1+Len, Length );
		
		E2P_RData( Point, EMeter_No, 8 );						
		if( Data_Comp( Ptr+1, Point, 8 ) != 0 )
		{
			E2P_WData( EMeter_No, Ptr+1, 8 );					
		}	
	}	
	
	if( ReadRecord( ESAM, Ptr ) == 0 ) 					//����4�ֽ�ʣ���4�ֽڹ��������6�ֽڿͻ����	
	{
		SwapData( Ptr+8, 6 );
		E2P_RData( Point, Client_No, 6 );						
		if( Data_Comp( Ptr+8, Point, 6 ) != 0 )
		{
			E2P_WData( Client_No, Ptr+8, 6 );					
		}	
	}	
}	


//�������õ���
//Num��0.01�ȵ�����
//PriceType��0:������ݵ��	1��������ʵ��
void IncConsumECSum( unsigned short Num, short PriceType )		//15.07.31
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue1,LValue2;
	
	Ptr = Buff;
	
	if( PriceType == 0 ) LValue2 = GetCurrentStepPrice();	//ȡ��ǰ���ݵ�ۣ�6λС����16����
	else LValue2 = GetCurrentFeePrice();					//ȡ��ǰ���ʵ�ۣ�6λС����16����
	if( LValue2 == 0 ) return;								//����Ϊ0�����ټ���

	GetFRAMConsumECSum( Ptr );
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );	
	
	LValue2 *= Num;
	RAM_Fill( Ptr, 8 );
//	E2P_RData( Ptr, VoltRate, 3 );
//	Value = BCD4_Long( Ptr );
	E2P_RData( (unsigned char*)&Value, VoltRate, 4 );
	LValue2 *= Value;									//��ѹ���������
	LValue2 /= 100;										//��ѹ��������ȴ�2λС��
//	E2P_RData( Ptr, CurrentRate, 3 );
//	Value = BCD4_Long( Ptr );
	E2P_RData( (unsigned char*)&Value, CurrentRate, 4 );
	LValue2 *= Value;									//�������������
	LValue2 /= 100;										//������������ȴ�2λС��
	LValue1 += LValue2;
	RAM_Write( ICEC.RConsumECSumBCD, (unsigned char*)&LValue1, 8 );
	ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	E2P_WFM( FConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );
}	

//ȡ��ǰ���ý��
//��һ���ȼ�ΪFRAM���ڶ����ȼ�ΪRAM���������ȼ�ΪEEPROM�����ΪFRAM��У����ֵ
void GetFRAMConsumECSum( unsigned char* Dest )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	
	Ptr = Buff;

	E2P_RFM( Ptr, FConsumECSumBCD, 8 );
	RAM_Write( Dest, Ptr, 8 );
	if(( Flag.BatState & F_E2PCheck ) == 0 )
	{
		RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
		ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	}
	else
	{	
		if( ChkNum( ICEC.RConsumECSumBCD, 8 ) == ICEC.RConsumECSumBCDChk )
		{
			E2P_RFM( Ptr, FConsumECSumBCD, 8 );											//11.07.14
			RAM_Write( Dest, Ptr, 8 );													//11.07.14
			if(( Flag.BatState & F_E2PCheck ) == 0 )									//11.07.14	
			{																			//11.07.14	
				RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );								//11.07.14		
				ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );			//11.07.14	
			}																			//11.07.14		
			else																		//11.07.14	
			{																			//11.07.14
				RAM_Write( Dest, ICEC.RConsumECSumBCD, 8 );                         	
				E2P_WFM( FConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );			    	
			}																			//11.07.14
		}	
		else
		{
			E2P_RData( Ptr, ConsumECSumBCD, 8 );
			if(( Flag.BatState & F_E2PCheck ) == 0 )
			{
				E2P_RFM( Dest, FConsumECSumBCD, 8 );									//11.07.14
				if(( Flag.BatState & F_E2PCheck ) == 0 )								//11.07.14	
				{																		//11.07.14	
					RAM_Write( ICEC.RConsumECSumBCD, Dest, 8 );							//11.07.14		
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );		//11.07.14	
				}																		//11.07.14		
				else																	//11.07.14	
				{																		//11.07.14
					RAM_Write( Dest, Ptr, 8 );
					RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
					E2P_WFM( FConsumECSumBCD, Ptr, 8 );
				}																		//11.07.14
			}
		}		
	}
}

unsigned long GetCurrentFeePrice( void )                             //�¹���    //13.12.12
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned short Addr;
	
	Ptr = Buff;
	
	Addr = Case1Fee1Price;											//�¹���	//13.11.28	
	if(( SM.FeeNo == 0 )||(  SM.FeeNo > 4 )) SM.FeeNo = 1;			//�¹���	//14.01.03	
	E2P_RData( Ptr, Addr+(SM.FeeNo-1)*5, 4 );
	Value = BCD4_Long( Ptr );

	return Value;	
}	

unsigned long GetCurrentECPrice( void )                             //�¹���    //13.11.30
{
	unsigned long Value;
	
	if(( ICComm.RFeePriceNum == 0 )&&( SM.StepNo != 0 ))			//12.12.22		//����������Ϊ0����������Ϊ0����ǰ���=��ǰ���ݵ��
	{																//12.12.22
		Value = GetCurrentStepPrice(); 								//12.12.22	
		return Value;												//12.12.22
	}																//12.12.22
	
	Value = GetCurrentFeePrice();
	Value += GetCurrentStepPrice(); 

	return Value;	
}	

//Type: 0: �ض�EEPROM�ڽ��ݵ�����1������RAM�ڽ��ݵ���
unsigned short GetCurrentStepNo( short Type )							//�¹���	//13.11.30
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned short i;
	
	Ptr = Buff;
	
	SM.StepNo = 0;
	if(( Type == 0 )||( ICEC.ECStepChk != ChkNum( (unsigned char*)ICEC.ECStep, 24 ) ))		
	{
		for( i=0;i<6;i++ )
		{
			E2P_RData( Ptr, Case1Step1EC+i*5, 4 ); 	
			ICEC.ECStep[i] = BCD4_Long( Ptr );
		}	
		ICEC.ECStepChk = ChkNum( (unsigned char*)ICEC.ECStep, 24 );
	}	
	
	for( i=0;i<6;i++ )
	{
		if( ICEC.ECStep[i] != 0 ) break;								//���н��ݵ���Ϊ���ʾ��ִ�н��ݵ��
	}	
	if( i == 6 ) return 0;
	
	if( ICComm.YearMonSaveMode == 0 ) GetCycleInsideECPp0( Ptr );		//�½���
	else GetYearCycleInsideECPp0( Ptr );									//�����	
	Value = BCD4_Long( Ptr );
	for( i=0;i<6;i++ )
	{
		if( Value <= ICEC.ECStep[i] ) break;
		if( i != 5 )													//14.01.07
		{																//14.01.07
			if( ICEC.ECStep[i] >= ICEC.ECStep[i+1] ) 
			{
				i += 1;													//14.01.07	//�����������
				break;													//14.01.07	//�����������
			}	
		}																//14.01.07
	}	
	SM.StepNo = i+1;
	return i+1;	
}	

unsigned long GetCurrentStepPrice( void )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned short StepNo;
	
	Ptr = Buff;
	
	StepNo = GetCurrentStepNo(1); 
	if( StepNo == 0 ) return 0;					//�ݶ���Ϊ��򳬷�Χ
	E2P_RData( Ptr, Case1Step1Price+(StepNo-1)*5, 4 );					//�¹���
	Value = BCD4_Long( Ptr );
	
	return Value;	
}	

void GetCycleInsideECPp0( unsigned char* Dest  )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value1;
	unsigned long Value2;
	unsigned long Value3;
	unsigned long Value4;
	
	Ptr = Buff;

	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	Value1 = BCD4_Long( Ptr );
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
	E2P_RData( Ptr+4, L1_SaveDay1EC_Pp0, 4 );
	if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
#else
	GetLSCycleEC( Ptr+4, CMon_EC_Pp0, 0 );
#endif
	Value2 = BCD4_Long( Ptr+4 );
	
	if(( Para.RActiveMode & 0x0C ) == 0x04 )
	{
		Read_CMonEC4( Ptr, CMon_EC_Pn0 );
		Value3 = BCD4_Long( Ptr );
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
		E2P_RData( Ptr+4, L1_SaveDay1EC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
#else
		GetLSCycleEC( Ptr+4, CMon_EC_Pn0, 0 );
#endif
		Value4 = BCD4_Long( Ptr+4 );
		
		Value1 += Value3;
		Value2 += Value4;
	}	

	if( Value1 < Value2 )
	{
		Value1 += 100000000;		//������ת
	}	
	Value1 -= Value2;
	Long_BCD4( Dest, Value1 );
}

void GetYearCycleInsideECPp0( unsigned char* Dest )			//�¹���	//13.11.28
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value1;
	unsigned long Value2;
	unsigned long Value3;
	unsigned long Value4;
	
	Ptr = Buff;

	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	Value1 = BCD4_Long( Ptr );
	E2P_RData( Ptr+4, L1_YearSaveDayEC_Pp0, 4 );
	if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
	Value2 = BCD4_Long( Ptr+4 );
	
	if(( Para.RActiveMode & 0x0C ) == 0x04 )
	{
		Read_CMonEC4( Ptr, CMon_EC_Pn0 );
		Value3 = BCD4_Long( Ptr );
		E2P_RData( Ptr+4, L1_YearSaveDayEC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
		Value4 = BCD4_Long( Ptr+4 );
		
		Value1 += Value3;
		Value2 += Value4;
	}	

	if( Value1 < Value2 )
	{
		Value1 += 100000000;		//������ת
	}	
	Value1 -= Value2;
	Long_BCD4( Dest, Value1 );
}


							//remote
void SwapFrameData( unsigned char* Dest, unsigned char* Source, short ItemCount )
{
	unsigned char Buff[14];
	unsigned char* Ptr;
	short i,j,Len;
	
	Ptr = Buff;
	
	if( ItemCount == 0 ) return;
	RAM_Fill( Ptr, 14);

	for( i=0;i<ItemCount;i++ )
	{
		Len = *(Source+i);
		if( Len > 14 ) Len = 14;		//������14 
		for( j=0;j<Len;j++ )
		{
			*(Ptr+j) = *(Dest+Len-1-j);	
		}		
		RAM_Write( Dest, Ptr, Len );
		Dest += Len;
	}	
}	

void GetBackBuyECFile1( unsigned char* Dest )
{
	unsigned long Value;
	    
	*Dest = 0x68;
	*(Dest+1) = 0x11;
	*(Dest+2) = 0x00;
	*(Dest+3) = 0x2B;
	*(Dest+4) = 0;													//�¹���		//13.09.06			
//	E2P_RData( Dest+5, CurrentRate, 3 );
	E2P_RData( (unsigned char*)&Value, CurrentRate, 4 );
	Value /= 100;													//��ȴ�2λС��
	Long_BCD3( Dest+5, Value );
//	E2P_RData( Dest+8, VoltRate, 3 );
	E2P_RData( (unsigned char*)&Value, VoltRate, 4 );
	Value /= 100;													//��ȴ�2λС��
	Long_BCD3( Dest+8, Value );
//	E2P_RData( Dest+11, EUser_No, 6 );
	E2P_RData( Dest+11, EMeter_No, 8 );
	E2P_RData( Dest+17, Client_No, 6 );
	GetReOverEC( Dest+23, Dest+31 );								//ʣ����
	RAM_Write( (unsigned char*)&Value, Dest+31, 4 );
	Long_BCD4( Dest+31, Value );
	E2P_RData( Dest+27, BuyECCnt, 4 );
	RAM_Fill( Dest+35, 4);											//�¹���		//13.09.06			
	E2P_RData( Dest+39, ILInsert_Cunt, 3 );
	RAM_Write( Dest+42, Clk.Min, 5 );
	SwapFrameData( Dest+4, (unsigned char*)Back_BuyECTab1, 14 );
	*(Dest+47) = ChkNumAdd( Dest+1, 46 );
	*(Dest+48) = 0x16;

}

//дESAM������Ϣ�ļ�
short WriteESAMRunInfoFile( void )
{
	unsigned char Buff[50];
	unsigned char* Ptr;

	Ptr = Buff;
	
	GetBackBuyECFile1( Ptr );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 0, 49 ) != 0 ) return 1;	//{ ICComm.ICAlarmFlg = ICALARM_60; return; }	//ȫ������д��ESAM��������Ϣ�ļ�	
	
	return 0;
}

//10.07.30
//дESAM������Ϣ�ļ�
short WriteESAMRunInfoFilePassWordState( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	CYCDelay(6500);						//6.5ms/4=1.26ms > 10ETU			//12.05.24

	RAM_Fill( Ptr, 12 );
	RAM_Write( Ptr, Source, 4 );
	RAM_Write( Ptr+7, Clk.Min, 5 );
	SwapData( Ptr+7, 5 );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 35, 12 ) != 0 ) return 1;		//��Կ��Ϣ���ݡ�����д��ESAM��������Ϣ�ļ�	
	return 0;
}


void IC_IOState( void )
{

	GetECState();												

}

#else

//����2�ֽڴ�����Ӧ״̬
void SaveErrorResponse( short CARD_TYPE )							//�¹���	//13.12.02
{
	if( CARD_TYPE == PSAM )
	{
		if( ICComm.ICPtr1 != 0 ) 
		{
			RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf1, 2 );			
			if(( ICComm.ErrorResponse[0] == 0x6B )&&( ICComm.ErrorResponse[1] == 0x00 )) ICComm.ErrorMessage = ICERROR_07;
			else ICComm.ErrorMessage = ICERROR_19;
		}else ICComm.ErrorMessage = ICERROR_19;					//�¹���	//14.01.06	
	}
	else
	{
//		if( ICComm.ICPtr2 != 0 ) RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf2, 2 );
		ICComm.ErrorMessage = ICERROR_02;						//����ESAM��			
	}			
}	 

//����5�ֽڲ�������ͷ
void SaveOperateCommHead( short CARD_TYPE )							//�¹���	//13.12.02
{
	RAM_Fill( ICComm.ErrorResponse, 2 ); 
	if( CARD_TYPE == PSAM )
	{
		RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf1, 5 );			
	}
//	else RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf2, 5 );			
}	 

//���������ļ�
//CARD_TYPE: ����������
//Dest : ����Ŀ�ĵ�ַ
//FileNo : �������ļ���
//StartAddr�� ��ȡ�������ļ��е���ʼ��ַ
//Length����ȡ���ݳ���
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	short RepeatCnt=4; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//��д����
		if( CARD_TYPE == PSAM ) { ICBuf = ICComm.ICBuf1; }
//		else { ICBuf = ICComm.ICBuf2; }
		else
		{
			ICBuf = FBuff.Buff;
			*(ICBuf+6) = 0x00; 
			ShortToSwapChar( ICBuf+7, StartAddr );
			ShortToSwapChar( ICBuf+7+2, Length );
			if( ReadWriteESAM( Dest, 0x80, 0x2C, FileNo, 0x0005 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
//		*ICBuf = 0x00;
//		*(ICBuf+1) = 0xB0;
//		*(ICBuf+2) = FileNo+0x80;
//		*(ICBuf+3) = StartAddr;
//		*(ICBuf+4) = Length;
//
//		SaveOperateCommHead( CARD_TYPE );			//�¹���	//13.12.02
//	
//		ICDataSend( CARD_TYPE, 4, CYC60000, Length+3 );			//V6
//		if(( *ICBuf == 0xB0 )&&( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))
//		{
//			RAM_Write( Dest, ICBuf+1, Length );
//			ICComm.ICAlarmFlg = 0x0;						
//			return 0;	
//		}	
//		else SaveErrorResponse( CARD_TYPE );							//����2�ֽڴ�����Ӧ״̬		//�¹���	//13.12.02
	}
	return -1;
}

//��ESAMǮ���ļ�
//Dest : ����Ŀ�ĵ�ַ
short ReadRecord( short CARD_TYPE, unsigned char* Dest )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//��д����
	ICBuf = FBuff.Buff;
	*(ICBuf+6) = 0x00;
	if( ReadWriteESAM( Dest, 0x80, 0x48, 0x0000, 0x0001 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//����2�ֽڴ�����Ӧ״̬

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

#endif					// LocalSmartMeter == YesCheck



