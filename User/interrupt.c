#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Measure.h"
#include "Port.h"
#include "Data.h"
#include "General.h"
#include "Interrupt.h"
#include "ProfileGeneric.h"

#include "Display.h"
#include "cpucard.h"
#include "Initial.h"
#include "Mem.h"
#include "Time.h"
#include "DisconnectControl.h"
#include "Measure.h"
#include "GetMeterData.h"

//=====================================================================
//HT6023
//=====================================================================
 



/*
*********************************************************************************************************
*                                 			   HT60XX
*                            			      HT60XX����
*
*                       			Copyright 2013, Hi-Trend Tech, Corp.
*                             			 All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : ht60xx_it.c
* By           : SocTeam
* Version      : CurrentVersion
* Description  : 
*********************************************************************************************************
*/

#define  __HT60XX_IT_C

#include "ht6xxx_lib.h"


/*
*********************************************************************************************************
*                                            ���غ�/�ṹ��
*********************************************************************************************************
*/
#define RX_Head1    0xAA
#define RX_Head2    0x55
#define RX_Over     0x16

/*
*********************************************************************************************************
*                                            ���ر���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/

 void wait()
{
      __asm("BX lr");
}

void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
  wait();
  return;
}

void SysTick_Handler()			//1/64���ж�
{
  if(( HT_GPIOE->PTDAT & GPIOE_LVDIN0 ) == 0)
  {
    if( SM.PowerUpTime != 0 )
    {
      if( SM.PowerUpTime >= PWUPTIME ) 
        SM.PowerUpTime = PWUPTIME/2;
      else SM.PowerUpTime--;
    }	
  }	
  else 
  {
    if( SM.PowerUpTime < PWUPTIME ) SM.PowerUpTime++;
  }
  Clk.Sec_64++;
  if( Clk.Sec_64 >= 64 )
  {				// 1/64 Second
    Clk.Sec_64 = 0;
  }
  if((Flag.Power & F_PwrUp) != 0 )									//PWMD
  {																	//PWMD
    if(( Clk.Sec_64 == 0 )||( Clk.Sec_64 == 32 )) 					//PWMD
    {																//PWMD
      Flag.Clk |= F_HalfSec;
      //HT_GPIO_BitsToggle(HT_GPIOC,GPIO_Pin_8);		
    }																//PWMD	
    if(( Clk.Sec_64 == 0 )||( Clk.Sec_64 == 21 )||( Clk.Sec_64 == 42 )) 
    {
      Flag.Clk |= F_ThirdSec;
    }
  }
  return;	
}

/*
*********************************************************************************************************
*                                        PMU INTERRUPT HANDLER #16+0
*
*********************************************************************************************************
*/

void PMU_IRQHandler()
{
    if(SET == HT_PMU_ITFlagStatusGet(PMU_PMUIF_VCCIF))           /*!< Vcc����ж���λ            */
    {
     
        HT_PMU_ClearITPendingBit(PMU_PMUIF_VCCIF);               /*!< ����жϱ�־               */
    }
    
    if(SET == HT_PMU_ITFlagStatusGet(PMU_PMUIF_BORIF))           /*!< Bor����ж���λ            */
    {

        HT_PMU_ClearITPendingBit(PMU_PMUIF_BORIF);               /*!< ����жϱ�־               */
    }

    if(SET == HT_PMU_ITFlagStatusGet(PMU_PMUIF_LVD0IF))           /*!< Lvdin����ж���λ          */
    {

        HT_PMU_ClearITPendingBit(PMU_PMUIF_LVD0IF);               /*!< ����жϱ�־               */
    }    
}

/*
*********************************************************************************************************
*                                  3DES/AES INTERRUPT HANDLER  #16+1
*
*********************************************************************************************************
*/
#if defined HT6x1x
void TDES_IRQHandler()                      /*!< 3DES               */
{
    
}
#else
void AES_IRQHandler()                       /*!< AES                */
{
    
}
#endif

/*
*********************************************************************************************************
*                                   EXTI0 INTERRUPT HANDLER  #16+2
*
*********************************************************************************************************
*/

void EXTI0_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT0))         /*!< INT0�������ж�           */
    {
        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT0);             /*!< ����жϱ�־             */
        if( SM.PowerUpTime < PWUPTIME ) SM.PowerUpTime++;
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT0))         /*!< INT0�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT0);             /*!< ����жϱ�־             */
    }      
}

/*
*********************************************************************************************************
*                                   EXTI1 INTERRUPT HANDLER  #16+3
*
*********************************************************************************************************
*/

void EXTI1_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT1))         /*!< INT1�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT1);             /*!< ����жϱ�־             */        
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT1))         /*!< INT1�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT1);             /*!< ����жϱ�־             */        
    }      
}

/*
*********************************************************************************************************
*                                   EXTI2 INTERRUPT HANDLER  #16+4
*
*********************************************************************************************************
*/

void EXTI2_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT2))         /*!< INT2�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT2);             /*!< ����жϱ�־             */         
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT2))         /*!< INT2�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT2);             /*!< ����жϱ�־             */       
    }      
}

/*
*********************************************************************************************************
*                                   EXTI3 INTERRUPT HANDLER  #16+5
*
*********************************************************************************************************
*/

void EXTI3_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT3))         /*!< INT3�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT3);             /*!< ����жϱ�־             */  		
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT3))         /*!< INT3�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT3);             /*!< ����жϱ�־             */         
    }      
}

/*
*********************************************************************************************************
*                                   EXTI4 INTERRUPT HANDLER  #16+6
*
*********************************************************************************************************
*/

void EXTI4_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT4))         /*!< INT4�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT4);             /*!< ����жϱ�־             */  
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT4))         /*!< INT4�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT4);             /*!< ����жϱ�־             */        	
    }      
}

/*
*********************************************************************************************************
*                                   EXTI5 INTERRUPT HANDLER  #16+7
*
*********************************************************************************************************
*/

void EXTI5_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT5))         /*!< INT5�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT5);             /*!< ����жϱ�־             */  
		
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT5))         /*!< INT5�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT5);             /*!< ����жϱ�־             */         	 
			
     }      
}

/*
*********************************************************************************************************
*                                   EXTI6 INTERRUPT HANDLER  #16+8
*
*********************************************************************************************************
*/

void EXTI6_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT6))         /*!< INT6�������ж�           */
    {
        
        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT6);             /*!< ����жϱ�־             */        
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT6))         /*!< INT6�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT6);             /*!< ����жϱ�־             */         
    }      
}

/*
*********************************************************************************************************
*                                   UART0 INTERRUPT HANDLER  #16+9
*
*********************************************************************************************************
*/

#if 0
void UART0_IRQHandler()
{
#if 1
  if(SET == HT_UART_ITFlagStatusGet(HT_UART0, UART_UARTSTA_TXIF))         /*!< UART0�����ж�         */
  {
      HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_TXIF);             /*!< ����жϱ�־          */

    	if( HT_UART0->UARTCON&UART_UARTCON_TXIE )							//17.05.02
    	{	
    		
	}            
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART0, UART_UARTSTA_RXIF))         /*!< UART5�����ж�         */
    {
		HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF);             /*!< ����жϱ�־          */ //UCA2IE &= ~UCRXIE; ;
		if( HT_UART0->UARTCON&UART_UARTCON_RXIE )			//17.03.15
		{	

                  HT_UART0->UARTCON &=~ (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
                }
    } 
#else
  CM_Gprs_Interrupt_Handle(HT_UART0);
#endif
}
#endif

/*
*********************************************************************************************************
*                                   UART1 INTERRUPT HANDLER  #16+10
*
*********************************************************************************************************
*/
//����
void UART1_IRQHandler()
{
  
  if(SET == HT_UART_ITFlagStatusGet(HT_UART1, UART_UARTSTA_TXIF)) 		/*!< UART1�����ж�		   */
  {
    HT_UART_ClearITPendingBit(HT_UART1, UART_UARTSTA_TXIF); 			/*!< ����жϱ�־		   */
  }
  
  if(SET == HT_UART_ITFlagStatusGet(HT_UART1, UART_UARTSTA_RXIF)) 		/*!< UART2�����ж�		   */
  {
    HT_UART_ClearITPendingBit(HT_UART1, UART_UARTSTA_RXIF); 			/*!< ����жϱ�־		   */
  } 
}
                                                                 
/*                                                               
*********************************************************************************************************
*                                   UART2 INTERRUPT HANDLER  #16+11
*
*********************************************************************************************************
*/
//485
#if 0
void UART2_IRQHandler()
	{
	  unsigned short Length;
	  
		if(SET == HT_UART_ITFlagStatusGet(HT_UART2, UART_UARTSTA_TXIF)) 		/*!< UART2�����ж�		   */
		{
			HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_TXIF); 			/*!< ����жϱ�־		   *///UCA2IE &= ~UCTXIE; 
			if( HT_UART2->UARTCON&UART_UARTCON_TXIE )							//17.05.02
			{	
			/*
				if( Comm.TX_FESum2 != 0 )
				{
					Comm.TX_FESum2--;
					HT_UART0->SBUF = 0xFE; //UCA2TXBUF = 0xFE; 
					Comm.BTime2 = MS500;
					return; 					//17.03.09	
				}	
				*/
				Comm.BTime2 = MS500;
		 switch( Comm.IECState2 )
				 {
				 /*
					 case IECStartCodeOver: 	 // IECͨѶ��ʼ֡����
						 UART0->TXD = Comm.Buf0[Comm.Ptr0++];
						 if( Comm.Buf0[Comm.Ptr0-1] != LF  ) return;
						 Comm.IECState0 = IECBandRate;
						 break;  
					 case IECBandRateOver:		 // IECͨѶ�л�������֡����
						 break;  
					 case IECModeCRXOver:			 // IECͨѶģʽC֡����
						 if(( Comm.Buf0[0] == ACK ) || ( Comm.Buf0[0] == NAK )) break;
						 UART0->TXD = Comm.Buf0[Comm.Ptr0++];
						 if(( Comm.Buf0[Comm.Ptr0-2] != EOT ) && ( Comm.Buf0[Comm.Ptr0-2] != ETX )) return;
						 Comm.IECState0 = IECModeC;
						 break;  
				 */
					 case IECModeHDLCRXOver:	 // IECͨѶģʽE,HDLC֡����
					 Length = Comm.Buf2[1] & 0x07;
					 Length *= 256;			 
					 Length += Comm.Buf2[2];
							 if( Comm.Ptr2 < (Length+2) ) 
								{							
							//	 HT_GPIOA->PTDIR |= GPIOA_PLC_SET;	 
								 HT_UART2->SBUF = Comm.Buf2[Comm.Ptr2++];
								 return;
								} 
							 else 
								{
								Comm.IECState2 = IECModeHDLC;
							 }
						 break;  
					 default: break;
				 }
				Comm.Run2 |= F_ComOK;
				Comm.Buf2[0] = 0;
				Comm.Ptr2 = 0;
				HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF); 			/*!< ����жϱ�־		   */
				Comm.TX_Delay2 = GetTXDelay( (unsigned char*)&HT_UART2->SREL );//Comm.TX_Delay1 = GetTXDelay( (unsigned char*)&UCA1BR1 );
				
			}			 
		} 
	
		if(SET == HT_UART_ITFlagStatusGet(HT_UART2, UART_UARTSTA_RXIF)) 		/*!< UART2�����ж�		   */
		{
			HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF); 			/*!< ����жϱ�־		   */ //UCA2IE &= ~UCRXIE; 
    
			if( HT_UART2->UARTCON&UART_UARTCON_RXIE )			//17.03.15
			{	
                                Comm.BTime2 = 16;//25ms
				if( Comm.Ptr2 >= MAX_CARRY_INFO_SIZE )			//10.07.03
				{
					Comm.Ptr2 = 0;						//10.07.03
					return; 							//10.07.03
				}	
				
				Comm.Buf2[Comm.Ptr2++] = HT_UART2->SBUF;		//Comm.Buf2[Comm.Ptr2] = UCA2RXBUF; 	//10.07.03
			
			}
		}	   
	}
#endif

/*
*********************************************************************************************************
*                                   UART3 INTERRUPT HANDLER  #16+12
*
*********************************************************************************************************
*/
void UART3_IRQHandler()
{
  if(SET == HT_UART_ITFlagStatusGet(HT_UART3, UART_UARTSTA_TXIF))            /*!< UART3�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_TXIF);                /*!< ����жϱ�־          */  
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART3, UART_UARTSTA_RXIF))            /*!< UART4�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_RXIF);                /*!< ����жϱ�־          */   
    } 

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_1, UART_ISO7816STA_TXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_1, UART_ISO7816STA_TXIF);      /*!< ����жϱ�־          */         
    }  

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_1, UART_ISO7816STA_RXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_1, UART_ISO7816STA_RXIF);      /*!< ����жϱ�־          */         
    } 

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_1, UART_ISO7816STA_PRDIF)) /*!< 7816���������жϱ�־  */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_1, UART_ISO7816STA_PRDIF);     /*!< ����жϱ�־          */         
    }      
}

/*
*********************************************************************************************************
*                                   UART4 INTERRUPT HANDLER  #16+13
*
*********************************************************************************************************
*/

void UART4_IRQHandler()
{

    if(SET == HT_UART_ITFlagStatusGet(HT_UART4, UART_UARTSTA_TXIF))            /*!< UART4�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART4, UART_UARTSTA_TXIF);                /*!< ����жϱ�־          */  
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART4, UART_UARTSTA_RXIF))            /*!< UART4�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART4, UART_UARTSTA_RXIF);                /*!< ����жϱ�־          */   
    } 

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_0, UART_ISO7816STA_TXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_0, UART_ISO7816STA_TXIF);      /*!< ����жϱ�־          */         
    }  

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_0, UART_ISO7816STA_RXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_0, UART_ISO7816STA_RXIF);      /*!< ����жϱ�־          */         
    } 

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_0, UART_ISO7816STA_PRDIF)) /*!< 7816���������жϱ�־  */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_0, UART_ISO7816STA_PRDIF);     /*!< ����жϱ�־          */         
    }      
}
void UART5_IRQHandler()
{ 
	 if(SET == HT_UART_ITFlagStatusGet(HT_UART5, UART_UARTSTA_TXIF))			/*!< UART5�����ж�		   */
		{
			HT_UART_ClearITPendingBit(HT_UART5, UART_UARTSTA_TXIF); 			   /*!< ����жϱ�־		  */		  
	
		} 
	
		if(SET == HT_UART_ITFlagStatusGet(HT_UART5, UART_UARTSTA_RXIF)) 		   /*!< UART3�����ж�		  */
		{		 
			HT_UART_ClearITPendingBit(HT_UART5, UART_UARTSTA_RXIF); 			   /*!< ����жϱ�־		  */			
		}	 
}

/*
*********************************************************************************************************
*                                   TIMER_0 INTERRUPT HANDLER  #16+15
*
*********************************************************************************************************
*/
extern void Iec101WatchTime(void);
void TIMER_0_IRQHandler()
{
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR0, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
    {
        
        HT_TMR_ClearITPendingBit(HT_TMR0, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */
    } 
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR0, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR0, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */        
    } 
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR0, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR0, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */         
    } 
    Iec101WatchTime();
   // SM.CalibCount++;
}

/*
*********************************************************************************************************
*                                   TIMER_1 INTERRUPT HANDLER  #16+16
*
*********************************************************************************************************
*/

void TIMER_1_IRQHandler()
{

    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR1, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR1, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */        
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR1, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR1, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */        
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR1, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR1, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */        
    } 
    
}

/*
*********************************************************************************************************
*                                   TIMER_2 INTERRUPT HANDLER  #16+17
*
*********************************************************************************************************
*/

void TIMER_2_IRQHandler()//20ms
{
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR2, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR2, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */   
    }   
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR2, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR2, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */         
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR2, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR2, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */         
    } 
    
}

/*
*********************************************************************************************************
*                                   TIMER_3 INTERRUPT HANDLER  #16+18
*
*********************************************************************************************************
*/

void TIMER_3_IRQHandler()	//500us��ʱ
{
  if(SET == HT_TMR_ITFlagStatusGet(HT_TMR3, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
  {
    HT_TMR_ClearITPendingBit(HT_TMR3, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */  
  }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR3, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR3, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */         
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR3, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR3, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */        
    } 
    
}

/*
*********************************************************************************************************
*                                     TBS INTERRUPT HANDLER  #16+19
*
*********************************************************************************************************
*/

void GetTBS_Value(int16_t TBS_Value,short Type)
{
	unsigned long* Ptr;	
	unsigned long VADCINx;
	signed   long Temp;
	
	Ptr = &SM.ClockBatAD;
        if((TBS_Value&0x8000)&&(TBS_Value&0x7fff))
        {
          Temp = (2148+(TBS_Value&0x7fff));
        }
        else
          Temp = TBS_Value;
	VADCINx = (unsigned long)((Temp));//VADCIN0 = 0.0258*ADC0DAT+4.7559�����У� VADCIN0 Ϊʵ��ADC ������ѹ��mV��
		
	*(Ptr+Type) = VADCINx; 
}

void TBS_IRQHandler()
{

    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_TMPIF))                 /*!< �¶Ȳ����ж�          */
    {
        
        HT_TBS_ClearITPendingBit(TBS_TBSIF_TMPIF);                     /*!< ����жϱ�־          */
    }  
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_VBATIF))                /*!< ��ز����ж�          */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_VBATIF);                    /*!< ����жϱ�־          */  
        GetTBS_Value(HT_TBS->VBATDAT,0);
    }  
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_ADC0IF))                /*!< ADC0�����ж�          */
    {
        HT_TBS_ClearITPendingBit(TBS_TBSIF_ADC0IF);                    /*!< ����жϱ�־          */        
        HT_TBS->TBSIE &= ~TBS_TBSIE_ADC0IE;

    }   
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_ADC1IF))                /*!< ADC1�����ж�          */
    {
        HT_TBS_ClearITPendingBit(TBS_TBSIF_ADC1IF);                    /*!< ����жϱ�־          */        
        HT_TBS->TBSIE &= ~TBS_TBSIE_ADC1IE;
    }   
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_VBATCMPIF))             /*!< ��ص�ѹ�Ƚ��ж�      */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_VBATCMPIF);                 /*!< ����жϱ�־          */       
    }  
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_VCCIF))                 /*!< ��Դ�����ж�          */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_VCCIF);                     /*!< ����жϱ�־          */         
    }       
}

/*
*********************************************************************************************************
*                                    RTC INTERRUPT HANDLER  #16+20
*
*********************************************************************************************************
*/

void RTC_IRQHandler()
{
    uint8_t Flag_RTC = HT_RTC->RTCIF;
    HT_RTC->RTCIF=0;                                               /*!< ����жϱ�־          */
    
//    if(Flag & RTC_RTCIF_SECIF)                                     /*!< ���ж�                */
    if((Flag_RTC & RTC_RTCIF_SECIF)&&(HT_RTC->RTCIE & RTC_RTCIE_SECIE))                                    /*!< ���ж�                */
    {
      Flag.Clk |= F_Sec;
      _BCD1INC(Clk.Sec);
      if( Clk.Sec[0] >= 0x60 )
      {				// Second
        Clk.Sec[0] = 0;
	Flag.Clk |= F_Min;
	_BCD1INC(Clk.Min);
	if( Clk.Min[0] >= 0x60 )
	{			// Minute
          Clk.Min[0] = 0;
          Flag.Clk |= F_Hour;
          _BCD1INC(Clk.Hour);
          if( Clk.Hour[0] >= 0x24 )
          {		// Hour
            Clk.Hour[0] = 0;
            Flag.Clk |= F_Day;
            Clk.Week[0]++;
            if( Clk.Week[0] > 6 ) Clk.Week[0] = 0;				// Week
            DayInc( &Clk.Day[0] );
          }
        }
      }
      if(SM.BatUpTime)
      {
        SM.BatUpTime--;
      }
      if((Flag.Run & F_Hold) && ( HT_GPIOE->PTDAT & GPIOE_LVDIN0 ))
      {
        if(SM.PowerUpTime<48)
          SM.PowerUpTime = 48;
        SM.PowerUpTime++;
      }
    }       
    
    if(Flag_RTC & RTC_RTCIF_MINIF)                                     /*!< ���ж�                */
    {
       
    }  
    
    if(Flag_RTC & RTC_RTCIF_HRIF)                                      /*!< Сʱ�ж�              */
    {
      
    }

    if(Flag_RTC & RTC_RTCIF_DAYIF)                                     /*!< ���ж�                */
    {
        
    }  

    if(Flag_RTC & RTC_RTCIF_MTHIF)                                     /*!< ���ж�                */
    {
       
    }  

                                   /*!< RTC��ʱ��1�ж�        */
    if((Flag_RTC & RTC_RTCIF_RTC1IF)&&(HT_RTC->RTCIE&RTC_RTCIE_RTC1IE))                                    /*!< RTC��ʱ��1�ж�        */
    {
      if(((Flag.Power & F_PwrUp) == 0)&&((Flag.Run & F_Hold)!=0))
      {
        
	HT_WDT->WDTCLR = 0xAAFF;        //16s     //���Ź�ι��//					
      }	
    }

    if(Flag_RTC & RTC_RTCIF_RTC2IF)                                    /*!< RTC��ʱ��2�ж�        */
    {
       
    }  

    if(Flag_RTC & RTC_RTCIF_ALMIF)                                     /*!< �����ж�              */
    {
       
    }      
}

/*
*********************************************************************************************************
*                                    I2C INTERRUPT HANDLER  #16+21
*
*********************************************************************************************************
*/

void I2C_IRQHandler()
{
    
}

/*
*********************************************************************************************************
*                                    SPI0 INTERRUPT HANDLER  #16+22
*
*********************************************************************************************************
*/

void SPI0_IRQHandler()
{

    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI0,SPI_SPISTA_MODF))                 /*!< SPI0ģʽ���ϱ�־λ     */
    {

        HT_SPI_ClearITPendingBit(HT_SPI0, SPI_SPISTA_MODF);                    /*!< ����жϱ�־          */
    }    

    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI0, SPI_SPISTA_SPIF))                /*!< SPI0���ݴ�����ɱ�־λ*/
    {

        HT_SPI_ClearITPendingBit(HT_SPI0, SPI_SPISTA_SPIF);                    /*!< ����жϱ�־          */        
    }     

}

/*
*********************************************************************************************************
*                                  WDT/SPI1 INTERRUPT HANDLER  #16+23
*
*********************************************************************************************************
*/
#if defined  HT6x1x
void WDT_IRQHandler()
{
    
}
#elif defined HT6x2x
void SPI1_IRQHandler()
{
    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI1,SPI_SPISTA_MODF))                 /*!< SPI1ģʽ���ϱ�־λ     */
    {

        HT_SPI_ClearITPendingBit(HT_SPI1, SPI_SPISTA_MODF);                    /*!< ����жϱ�־          */
    }    

    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI1, SPI_SPISTA_SPIF))                /*!< SPI1���ݴ�����ɱ�־λ*/
    {

        HT_SPI_ClearITPendingBit(HT_SPI1, SPI_SPISTA_SPIF);                    /*!< ����жϱ�־          */        
    }     
}
#endif

/*
*********************************************************************************************************
*                                 FREQ TEST INTERRUPT HANDLER  #16+24
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void SelfTestF_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                               TIMER_4/EMU INTERRUPT HANDLER  #16+25
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void TIMER_4_IRQHandler(void)			//250us��ʱ
{
    HT_TMR_ClearITPendingBit(HT_TMR4, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */          
}
#elif defined HT501x
void EMU_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                               TIMER_5/DMA INTERRUPT HANDLER  #16+26
*
*********************************************************************************************************
*/

#if defined  HT6x2x || HT6x3x
void TIMER_5_IRQHandler(void)		//5ms
{
  int i;
  unsigned char flag;
  HT_TMR_ClearITPendingBit(HT_TMR5, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */     
  for(i=0;i<8;++i)
  {
        ECInt.YPulse_Mode[i] = (ECInt.YPulse_Mode[i] << 1);
        flag=0;
        switch(i)
        {
        case 0:
          if(HT_GPIOD->PTDAT&GPIO_Pin_1)
            flag=1;
          break;
        case 1:
          if(HT_GPIOD->PTDAT&GPIO_Pin_6)
            flag=1;
          break;
        case 2:
          if(HT_GPIOG->PTDAT&GPIO_Pin_12)
            flag=1;
          break;
        case 3:
          if(HT_GPIOG->PTDAT&GPIO_Pin_14)
            flag=1;
          break;
        case 4:
          if(HT_GPIOA->PTDAT&GPIO_Pin_1)
            flag=1;
          break;
        case 5:
          if(HT_GPIOA->PTDAT&GPIO_Pin_4)
            flag=1;
          break;
        case 6:
          if(HT_GPIOB->PTDAT&GPIO_Pin_2)
            flag=1;
          break;
        case 7:
          if(HT_GPIOH->PTDAT&GPIO_Pin_2)
            flag=1;
          break;
        default:
          break;
        }
        if(flag) 
          ECInt.YPulse_Mode[i] |= 0x01;
        else  
          ECInt.YPulse_Mode[i] &= ~0x01;
        if( ECInt.YPulse_Mode[i] == 0xf0 )
        {
#if( RunOverCheck == YesCheck )
          SM.SecSumPluse[i] += 1;			//V03
          if( SM.RunOverState == 0 ) ECInt.YPulse_Cum[i] += 1;	//V03
#else
          ECInt.YPulse_Cum[i] += 1;
#endif
          ECInt.YPulse_CumChk[8] = ECInt.YPulse_Cum[8] + 0xA5;
          SM.Delay30Min = 30;	
          SM.Delay60Sec = 55;	
          SM.SumPluseECP0[i] += 1;
        }
        
        ECInt.WPulse_Mode[i] = (ECInt.WPulse_Mode[i] << 1);
        flag=0;
        switch(i)
        {
        case 0:
          if(HT_GPIOD->PTDAT&GPIO_Pin_0)
            flag=1;
          break;
        case 1:
          if(HT_GPIOD->PTDAT&GPIO_Pin_5)
            flag=1;
          break;
        case 2:
          if(HT_GPIOG->PTDAT&GPIO_Pin_11)
            flag=1;
          break;
        case 3:
          if(HT_GPIOG->PTDAT&GPIO_Pin_13)
            flag=1;
          break;
        case 4:
          if(HT_GPIOA->PTDAT&GPIO_Pin_0)
            flag=1;
          break;
        case 5:
          if(HT_GPIOH->PTDAT&GPIO_Pin_4)
            flag=1;
          break;
        case 6:
          if(HT_GPIOB->PTDAT&GPIO_Pin_1)
            flag=1;
          break;
        case 7:
          if(HT_GPIOH->PTDAT&GPIO_Pin_1)
            flag=1;
          break;
        default:
          break;
        }
        if(flag) 
          ECInt.WPulse_Mode[i] |= 0x01;
        else  
          ECInt.WPulse_Mode[i] &= ~0x01;
        if( ECInt.WPulse_Mode[i] == 0xf0 ) 
        {
          ECInt.WPulse_Cum[i] += 1;
          ECInt.WPulse_CumChk[i] = ECInt.WPulse_Cum[i] + 0xA5;				
        }	

#if ( NewHardScheme2 == YesCheck )		
        if( SM.PulseWidthCnt[i] != 0 )
#else
          if((( SM.RPulseOutMode == 1 )||( SM.RPulseOutMode == 2 ))&& ( SM.PulseWidthCnt != 0 ))
#endif
          {
            SM.PulseWidthCnt[i]--;
            if( SM.PulseWidthCnt[i] == 0 )
            {
            }
          }
  }
}
#elif defined HT501x
void DMA_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                               UART6/KEY INTERRUPT HANDLER  #16+27
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void UART6_IRQHandler(void)
{
    
}
#elif defined HT501x
void KEY_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                                 EXTI7 INTERRUPT HANDLER  #16+28
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void EXTI7_IRQHandler(void)
{
    
}
#endif
/*
*********************************************************************************************************
*                                 EXTI8 INTERRUPT HANDLER  #16+29
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void EXTI8_IRQHandler(void)
{
	if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF2_RIF_INT8))		   /*!< INT4�������ж�			 */
	{

		HT_EXTIRise_ClearITPendingBit(INT_EXTIF2_RIF_INT8);			   /*!< ����жϱ�־			 */ 		
	}

	if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF2_FIF_INT8))		   /*!< INT4�½����ж�			 */
	{

		HT_EXTIFall_ClearITPendingBit(INT_EXTIF2_FIF_INT8);			   /*!< ����жϱ�־			 */ 	   	
	}	   
}

#endif

/*
*********************************************************************************************************
*                                 EXTI9 INTERRUPT HANDLER  #16+30
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void EXTI9_IRQHandler(void)
{
	if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF2_RIF_INT9)) 	   /*!< INT4�������ж�			 */
	{

		HT_EXTIRise_ClearITPendingBit(INT_EXTIF2_RIF_INT9); 		   /*!< ����жϱ�־			 */ 		
	}

	if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIE2_RIE_INT9)) 	   /*!< INT4�½����ж�			 */
	{

		HT_EXTIFall_ClearITPendingBit(INT_EXTIE2_RIE_INT9); 		   /*!< ����жϱ�־			 */ 	   	
	}	   
}

#endif
/*
*********************************************************************************************************
*                                 DMA INTERRUPT HANDLER  #16+31
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void DMA_IRQHandler(void)
{
    HT_DMA_ClearITPendingBit(DMA_DMAIF_TCIF1);
    DMA_Cmd(HT_DMA_Channel1, DISABLE);  
    DMA_Cmd(HT_DMA_Channel0, DISABLE);  
}
#endif

