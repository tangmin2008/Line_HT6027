#include "TypeMeter.h"
#include "TypeRAM.h"
#include "TypeFRAM.h"
#include "TypeE2p.h"
#include "TypeMeter.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "Interrupt.h"
#include "General.h"
#include "Time.h"
#include "Mem.h"
#include "Display.h"
#include "Power.h"
#include "Initial.h"
#include "RemoteComm.h"
#include "Demand.h"
#include "ECProcess.h"


CMU_InitTypeDef  CMU_InitStructure;    
GPIO_InitTypeDef  GPIO_InitStructure;
//ʹ�õ�ع���
void Bat_on(unsigned char flag)
{
  if(flag)
  {
    HT_GPIO_BitsSet(HT_GPIOD,GPIO_Pin_8);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOD,GPIO_Pin_8); 
  }
}
//��ŵ�
void Battery_on(unsigned char flag)
{
  if(flag)
  {
    HT_GPIO_BitsSet(HT_GPIOD,GPIO_Pin_9);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOD,GPIO_Pin_9);
    
  }
}

void fnTarget_Init(void)
{
  CMU_InitTypeDef  CMU_InitStruct;
  __disable_irq();
  Flag.Power &= ~F_PwrUp;
  HT_CMU_Prefetch_Set( DISABLE ); 	//�ر�ָ��Ԥȡ���ܣ����͹���		//17.02.07
  CMU_InitStruct.SysClkSel = SysPLL;//SysHRCDiv1;//SysPLL;
  CMU_InitStruct.CPUDiv = CPUDiv1;
  HT_CMU_Init(&CMU_InitStruct);	
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  if(SysTick_Config(0x5400))		// 15.6ms	//T=ticks*(1/f)
  {
    while(1);
  }		
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//ʹ��SysTick�δ�ʱ��
  HT_GPIOE->PTUP |= (GPIOE_LVDIN0);
  HT_GPIOE->PTDIR &= ~(uint32_t)(GPIOE_LVDIN0);//���ϵ��ʼʱ�̣�PE7��Ϊ�������ż���Դ
#if 0
  /*!< GPIOC������Ϣ*/    		   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
  GPIO_InitStructure.GPIO_Pin = GPIOC_EE_SCL|GPIOC_EE_SDA|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
  HT_GPIO_Init(HT_GPIOC, &GPIO_InitStructure);
#endif  

  HT_RTC->RTCTMR1 = 0x00000000;		//(X+1)*1S
  HT_RTC->RTCCON |= RTC_RTCCON_RTC1EN;
  HT_RTC->RTCIE = RTC_RTCIE_RTC1IE + RTC_RTCIE_SECIE;
  HT_RTC->RTCIF = 0x00000000;
  NVIC_EnableIRQ(RTC_IRQn);		//ʹ���ж�
  __enable_irq();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
  HT_GPIO_Init(HT_GPIOB, &GPIO_InitStructure);
  HT_GPIOB->PTSET |=  GPIO_Pin_5;
}
void PwrOnInit(void)
{
  __disable_irq();
  HT_FreeDog();
  HT_CMU_Prefetch_Set( ENABLE );		//��ָ��Ԥȡ���ܣ���ǿ�������������		//17.02.07
 
    /***** ���´�����������CMUʱ�Ӽ���Ƶ *****/
  CMU_InitStructure.SysClkSel = SysPLL;//SysHRCDiv1;//SysPLL;
  CMU_InitStructure.CPUDiv = CPUDiv1;
  HT_CMU_Init(&CMU_InitStructure);//����ʱ��ΪPLL���22.020096Mhz

    /*!< GPIOA������Ϣ*/	

  // NET RX TX ָʾ��
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
   HT_GPIO_Init(HT_GPIOA, &GPIO_InitStructure);  
   HT_GPIO_BitsSet(HT_GPIOA,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9);
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_11;
   HT_GPIO_Init(HT_GPIOA, &GPIO_InitStructure);
   

#if 0		
    HT_INT->EXTIE |= (uint32_t)(INT_EXTIE_RIE_INT3);
    HT_INT->PINFLT |= (uint32_t)(INT_EXTIE_RIE_INT3);
    HT_INT->EXTIF  |= (uint32_t)0x00000000; 			 /*����жϱ�־*/	
    NVIC_EnableIRQ(EXTI3_IRQn);	
#endif
    
    HT_GPIOB->IOCFG |= GPIO_Pin_All;		/*����GPIOΪ���ù���Pin*/ 
    HT_GPIOB->AFCFG &=~ GPIO_Pin_All;		/*����Ϊ��һ���ù���*/
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOB, &GPIO_InitStructure);  
    HT_GPIO_BitsSet(HT_GPIOB,GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_10);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOB, &GPIO_InitStructure);
    HT_GPIO_BitsSet(HT_GPIOB,GPIO_Pin_5);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    HT_GPIO_Init(HT_GPIOB, &GPIO_InitStructure);

    /*!< GPIOC������Ϣ*/    		   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIOC_EE_SCL|GPIOC_EE_SDA|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
    HT_GPIO_Init(HT_GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
    GPIO_InitStructure.GPIO_Pin = GPIOC_232_TXD|GPIOC_232_RXD|GPIO_Pin_8;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOC, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|FSI|FSCLK;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
    HT_GPIO_Init(HT_GPIOC, &GPIO_InitStructure);
    HT_GPIO_BitsSet(HT_GPIOC,GPIO_Pin_10);
    //HT_GPIO_BitsReset(HT_GPIOC,GPIO_Pin_8);
	

    /*!< GPIOD������Ϣ*/     		   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOD, &GPIO_InitStructure);
    HT_GPIO_BitsSet(HT_GPIOD,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
    HT_GPIO_Init(HT_GPIOD, &GPIO_InitStructure);
	
    /*!< GPIOE������Ϣ*/        		   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
    HT_GPIO_Init(HT_GPIOE, &GPIO_InitStructure);
    HT_GPIO_BitsSet(HT_GPIOE,GPIO_Pin_0|GPIO_Pin_2);
#if 0
	HT_GPIOE->PTDIR |= GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6;
	HT_GPIOE->PTOD  |= GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6;
	HT_GPIOE->PTCLR |= GPIO_Pin_1;
	HT_GPIOE->PTSET |= GPIO_Pin_6|GPIO_Pin_2|GPIO_Pin_3;
#endif        
        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|FSO;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
    HT_GPIO_Init(HT_GPIOE, &GPIO_InitStructure);
    
    /*!< GPIOG������Ϣ*/        		   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOG, &GPIO_InitStructure);
    HT_GPIO_BitsSet(HT_GPIOG,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOG, &GPIO_InitStructure);
    
    /*!< GPIOH������Ϣ*/        		   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_OD;
    HT_GPIO_Init(HT_GPIOG, &GPIO_InitStructure);
    HT_GPIO_BitsSet(HT_GPIOG,GPIO_Pin_0|GPIO_Pin_3);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Floating;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    HT_GPIO_Init(HT_GPIOH, &GPIO_InitStructure);
        
        HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR0EN, ENABLE); 
	HT_TMR0->TMRDIV = 0x0004;                            /*!< ���ö�ʱ��Ԥ��Ƶ��     4��Ƶ */
        HT_TMR0->TMRPRD = 0x157C;                             /*!< ���ö�ʱ�����ڼĴ���  1000us */   
        HT_TMR0->TMRCON = 0x0307;                            /*!< ���ö�ʱ������ģʽ          */    
        HT_TMR0->TMRIE = 0x0001;               			     /*!< ʹ��TIMER�ж�               */
        NVIC_EnableIRQ(TIMER_0_IRQn);

	//------------���ü���ͨѶ��----------
	HT_GPIOE->IOCFG |= (GPIO_Pin_4|GPIO_Pin_5);		/*����GPIOΪ���ù���Pin*/ 
	HT_GPIOE->AFCFG &=~ (GPIO_Pin_5|GPIO_Pin_4);		/*����Ϊ��һ���ù���*/
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR2EN, ENABLE); 
	HT_TMR2->TMRDIV = 0x00c7;                            /*!< ���ö�ʱ��Ԥ��Ƶ��  200��Ƶ   */
        HT_TMR2->TMRPRD = 0x157C;                             /*!< ���ö�ʱ�����ڼĴ���  50ms */   
        HT_TMR2->TMRCON = 0x0307;                            /*!< ���ö�ʱ������ģʽ          */    
        HT_TMR2->TMRIE = 0x0001;               			     /*!< ʹ��TIMER�ж�               */
        NVIC_EnableIRQ(TIMER_2_IRQn);        

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR3EN, ENABLE); 
	HT_TMR3->TMRDIV = 0x0001;                            /*!< ���ö�ʱ��Ԥ��Ƶ��     2��Ƶ */
        HT_TMR3->TMRPRD = 0x157C;                             /*!< ���ö�ʱ�����ڼĴ���  500us */   
        HT_TMR3->TMRCON = 0x0307;                            /*!< ���ö�ʱ������ģʽ          */    
        HT_TMR3->TMRIE = 0x0001;               			     /*!< ʹ��TIMER�ж�               */
        NVIC_EnableIRQ(TIMER_3_IRQn);    
        
        HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR5EN, DISABLE); 
#if 0
	HT_TMR5->TMRDIV = 0x0013;                            /*!< ���ö�ʱ��Ԥ��Ƶ��     20��Ƶ */
        HT_TMR5->TMRPRD = 0x157C;                             /*!< ���ö�ʱ�����ڼĴ���  5ms */   
        HT_TMR5->TMRCON = 0x0307;                            /*!< ���ö�ʱ������ģʽ          */    
        HT_TMR5->TMRIE = 0x0001;               			     /*!< ʹ��TIMER�ж�               */
        NVIC_EnableIRQ(TIMER_5_IRQn);
#endif    
	//-----------����LCD-----------
	HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_LCDEN, DISABLE);
#if 0        
	HT_LCD->LCDCLK = 0x0088;
	HT_LCD->LCDCON = 0x0180;
#endif

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	
	if(SysTick_Config(0x54000))		// 15.6ms	//T=ticks*(1/f)
	{
		while(1);
	}	
	
	//ʹ��SysTick�δ�ʱ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	//����SysTick�δ�ʱ��
        //	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        HT_RTC_ToutSet(Tout1Hz);
        HT_TBSConfig(TBS_TBSCON_VBATEn,ENABLE);
        HT_TBS_PeriodSet(VBATPRD,TBS_TBSPRD_VBATPRD_2S);
        HT_TBS_ITConfig(TBS_TBSIE_VBATIE,ENABLE);
        //  NVIC_EnableIRQ(UART0_IRQn);                                  /*!< ʹ��UART�ж�*/
        //  NVIC_EnableIRQ(UART1_IRQn);                                  /*!< ʹ��UART�ж�*/
        NVIC_EnableIRQ(UART2_IRQn);                                  /*!< ʹ��UART�ж�*/
        // NVIC_EnableIRQ(UART3_IRQn);                                  /*!< ʹ��UART�ж�*/
        NVIC_EnableIRQ(TBS_IRQn);
        __enable_irq();
}

void PwrDnInit(void)
{
  __disable_irq();
  HT_FreeDog();
  Flag.Power &= ~F_PwrUp;
  HT_CMU_Prefetch_Set( DISABLE );		//�ر�ָ��Ԥȡ���ܣ����͹���		//17.02.07
#if 0  
  HT_CMU->WPREG = 0xA55A;		//WPREG д��0xA55A����ر�д��������
	
  HT_CMU->SYSCLKDIV = CMU_CPUCLKDIV_16;                            /*CPUʱ�ӷ�Ƶ���� Fsys/16*/	
  HT_CMU->CLKCTRL0 = CMU_CLKCTRL0_PLLEN; /*!< PLLʹ��*/	
  if((HT_CMU->SYSCLKCFG & CMU_SYSCLKCFG_CLKSEL)==3)	//22M��Ƶ
  {
    Delay(22000);
  }
  else if((HT_CMU->SYSCLKCFG & CMU_SYSCLKCFG_CLKSEL)<=1)	//(32768)M��Ƶ
  {
    __NOP();
    __NOP();
  }
  else		//(HRC/2)=5M������Ƶ
  {
    Delay(340);
  }		
  HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_PLL | CMU_SYSCLKCFG_WCLKEN);       /*����ϵͳΪPLLʱ�� */ 	
  HT_CMU->WPREG = 0x0000;		//WPREG д��0xA55A������д��������
#endif
  
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		
  if(SysTick_Config(0x54000))		// 15.6ms	//T=ticks*(1/f)
  {
    while(1);
  }		
  
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//ʹ��SysTick�δ�ʱ��

	/**�ض�ʱ��**/
  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR2EN, DISABLE); 
  HT_TMR2->TMRDIV = 0x0000;        /*���ö�ʱ��Ԥ��Ƶ��*/
  HT_TMR2->TMRPRD = 0x0000;        /*���ö�ʱ�����ڼĴ���*/   
  HT_TMR2->TMRCON = 0x0000;        /*���ö�ʱ������ģʽ*/
  HT_TMR2->TMRIE = 0x0000;         /*ʹ��TIMER�ж�*/
  NVIC_DisableIRQ(TIMER_2_IRQn);    

  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR3EN, DISABLE); 
  HT_TMR3->TMRDIV = 0x0000;        /*���ö�ʱ��Ԥ��Ƶ��*/
  HT_TMR3->TMRPRD = 0x0000;        /*���ö�ʱ�����ڼĴ���*/   
  HT_TMR3->TMRCON = 0x0000;        /*���ö�ʱ������ģʽ*/    
  HT_TMR3->TMRIE = 0x0000;         /*ʹ��TIMER�ж�*/
  NVIC_DisableIRQ(TIMER_3_IRQn);


  NVIC_DisableIRQ(UART0_IRQn);                           //��UART�ж�
  HT_UART0->UARTCON = 0x00000000;
  HT_UART0->SREL = 0x00000000;
  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, DISABLE);  //����UART0ģ��

  NVIC_DisableIRQ(UART1_IRQn);                           //��UART�ж�
  HT_UART1->UARTCON = 0x00000000;
  HT_UART1->SREL = 0x00000000;
  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART1EN, DISABLE);  //����UART1ģ��
	
  NVIC_DisableIRQ(UART2_IRQn);                           //��UART�ж�
  HT_UART2->UARTCON = 0x00000000;
  HT_UART2->SREL = 0x00000000;
  HT_UART2->IRCON = 0x00000000;	//����TX�������
  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, DISABLE);  //����UART2ģ��

  NVIC_DisableIRQ(UART3_IRQn);                           //��UART�ж�
  HT_UART3->UARTCON = 0x00000000;
  HT_UART3->SREL = 0x00000000;
  HT_UART3->IRCON = 0x00000000;	//����TX�������
  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, DISABLE);  //����UART2ģ��
	

  HT_CMU->WPREG = 0xA55A;		//WPREG д��0xA55A����ر�д��������  
  HT_GPIOC->PTDIR |= (uint32_t)(0x0770);
  HT_GPIOC->PTOD &= ~((uint32_t)(0x0770));									
  HT_GPIOC->PTCLR |= (uint32_t)(0x0770);		

  HT_CMU->WPREG = 0x0000;		//WPREG д��0xA55A������д��������

  NVIC_DisableIRQ(EXTI5_IRQn);
  NVIC_DisableIRQ(EXTI6_IRQn);	
  NVIC_DisableIRQ(EXTI3_IRQn);	
  
  HT_RTC->RTCTMR1 = 0x00000004;		//(X+1)*1S
  HT_RTC->RTCCON |= RTC_RTCCON_RTC1EN;
//HT_RTC->RTCIE = RTC_RTCIE_RTC1IE;
  HT_RTC->RTCIE = RTC_RTCIE_RTC1IE + RTC_RTCIE_SECIE;
//HT_RTC->RTCIE = 0;
  HT_RTC->RTCIF = 0x00000000;
  NVIC_EnableIRQ(RTC_IRQn); 		//ʹ���ж�
  __enable_irq();	
}


void CheckSetClose2( void )						//10.11.05
{
}

void CheckSetClose4( void )						//10.11.05
{
}

void CheckPassWd_ErrCnt2( void )				//10.11.05
{
}

void CheckPassWd_ErrCnt4( void )				//10.11.05
{
}

//�ϵ���FRAM�ж�ȡ��������RAM�ڵ���			
void GetFRamEC( void )							//10.06.22
{
	unsigned short Buff[10];
	unsigned char* Ptr;
	unsigned char* ECRAddress;
	unsigned short ECFAddress;
	unsigned short FeeMode;	
	short i,j;
    
	Ptr = (unsigned char*)Buff;
	
	for( i=0;i<ECUnitNum;i++ )
	{
		if((( i >= 8 )&&( i< 15 ))||(( i >= 23 )&&( i< 30 ))||(( i >= 38 )&&( i< 45 ))||(( i >= 53 )&&( i< 60 ))) continue;		//������г���ȵ���
		
		ECFAddress = ECRgTab[i].ECFAds;
		FeeMode = ECRgTab[i].ECRgMode;
		//ECRAddress = ECRgTab[i].ECRAds;
		for( j=0;j<5;j++ )
		{
			E2P_RFM( Ptr, ECFAddress, ECDIGIT );	 	//��ȡFRAM�ڵ���
			if( E2PECIsOK( Ptr ) == 0 )
			{
				RAM_Write( ECRAddress, Ptr, ECDIGIT );
				*(ECRAddress+ECDIGIT) = ChkNum( ECRAddress, ECDIGIT );
			}		
			if( FeeMode != 0x00 ) break;				//������
			else
			{
				ECFAddress += (ECDIGIT+1);
				ECRAddress += (ECDIGIT+1);
			}	
		}	
	}	
}	

unsigned short DoCrc16(unsigned short RegInit, unsigned char *message, unsigned short len) 
{
    unsigned short CrcReg = RegInit; 
    while (len--) 
       { CrcReg = (CrcReg >> 8) ^ Crc16tab[(CrcReg ^ *message++) & 0xff];}
    CrcReg=CrcReg^0xffff;    
    return CrcReg;
} 

void VarInit(void)
{
  int i,j;
  unsigned char tmp[88];
  SM.TestDisCnt = TESTDISCNT;
  for(i=0;i<MAX_CH_NUM;++i)
  {
    if(GetPt_Event_Record(i,0,tmp))
    {
      memcpy(&SM.PQNum[i][3],tmp+6,4);
    }
    for(j=0;j<3;++j)
    {
      if(GetPn_Event_Record(i,j,0,tmp))
      {
        memcpy(&SM.PQNum[i][j],tmp+6,4);
      }
    }
  }
#if 0  
  for(i=0;i<8;++i)
  {
    Real_Data[i].AFreq=50000+i;
  //  Real_Data[i].BFreq=50000+i;
  //  Real_Data[i].CFreq=50000+i;
  }
  
  for(i=0;i<8;++i)
  {
    Real_Data[i].Ua=2200+i;
    Real_Data[i].Ub=2210+i;
    Real_Data[i].Uc=2194+i;
  }
  
  for(i=0;i<8;++i)
  {
    Real_Data[i].Ia=200+i;
    Real_Data[i].Ib=210+i;
    Real_Data[i].Ic=194+i;
  }
  
  for(i=0;i<8;++i)
  {
    Real_Data[i].Pt=200+i;
    Real_Data[i].Pa=210+i;
    Real_Data[i].Pb=194+i;
    Real_Data[i].Pc=194+i;
  }
#endif  
  //���峣��100000
  MSpec.RMeterConst = 100000;
  MSpec.R7022E_HFConst = 4;
  //���峣��100000
  //1000
  /*
  MSpec.RMeterConst = 1000;
  MSpec.R7022E_HFConst = 400;
  */
   //20000
  //MSpec.RMeterConst = 20000;
  //MSpec.R7022E_HFConst = 20;
  //10000
  //MSpec.RMeterConst = 10000;
 // MSpec.R7022E_HFConst = 40;
  //6400
  //MSpec.RMeterConst = 6400;
  //MSpec.R7022E_HFConst = 63;
  MSpec.RBaseCurrent = 1000;
  MSpec.RPW00002Ib = 1000;
  MSpec.R7022E_PStartup = 5;
  MSpec.R7022E_IStart =  1;
}

void HW_ON( void )
{
  HT_FreeDog();
  NVIC_EnableIRQ(UART2_IRQn);                                  //ʹ��UART�ж�//	
}	

void HW_OFF( void )
{
  NVIC_DisableIRQ(UART2_IRQn);                                  //ʹ��UART�ж�//
  HT_UART2->UARTCON = 0x00000000;
  HT_UART2->SREL = 0x00000000;
  HT_UART2->IRCON = 0x00000000;	//����TX�������
  HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, DISABLE);        	//����UART2ģ��	
}

void HWTime( unsigned short ParaWord )
{
  unsigned char Buff[3];
  Buff[0] = 0;
  RAM_Write( (unsigned char*)&Buff[1], (unsigned char*)&ParaWord, 2 );	
  E2Pwrite( 0x0D, (unsigned char*)&Buff[0], 3, RTCAds );	//��������  								
}	

//#if( RTCCHIP == RX8025T )						//10.12.04
void RepeatHWTime( unsigned short Temp )
{
	Temp /= 256;	
	E2Pwrite( 0x0F, (unsigned char*)&Temp, 1, RTCAds );	//��������  						
	Temp = 0;
	E2Pwrite( 0x0D, (unsigned char*)&Temp, 1, RTCAds );	//��������  						
}	
//#endif

void EnterHold(void)
{
  // Bat_on(0);
  //HT_WDT->WDTCLR = 0xAAFF;        //16s                                /*!< ���Ź�ι��                   */
  HT_FreeDog();
#if 0 
  if(!SM.PowerUpTime)  //zzl add for �͹��Ľ�����������ģ��
  {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//����SysTick�δ�ʱ��
    HT_CMU->WPREG = 0xA55A;		//WPREG д��0xA55A����ر�д��������				
    HT_PMU->VDETCFG = 0x0000;
    HT_PMU->VDETPCFG = 0x0000;
    HT_PMU->PMUCON= 0x0000;
    HT_PMU->PMUIF = 0x0000;
    HT_PMU->PMUIE = 0x0000;	
    HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_LF|CMU_SYSCLKCFG_WCLKEN);
	 
    HT_CMU->SYSCLKDIV = 0x0000;
    HT_CMU->CLKCTRL0 = 0x0000;    
    HT_CMU->WPREG = 0x0000;		//WPREG д��0xA55A������д��������	
  }
#endif
  HT_EnterHold();		//2016.10.04clf
}

void ExitHold(void)
{
  HT_FreeDog();
#if 0  
  HT_CMU->WPREG = 0xA55A;		//WPREG д��0xA55A����ر�д��������                            //CPUʱ�ӷ�Ƶ���� Fsys/16//	
  HT_CMU->SYSCLKDIV = CMU_CPUCLKDIV_1;                            //CPUʱ�ӷ�Ƶ���� Fsys/8//	
  HT_CMU->CLKCTRL0 = CMU_CLKCTRL0_PLLEN; //PLLʹ��//
  __NOP();
  __NOP();
  __NOP();
  __NOP();	
  HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_PLL | CMU_SYSCLKCFG_WCLKEN);       //����ϵͳΪPLLʱ�� // 	
  HT_CMU->WPREG = 0x0000;		//WPREG д��0xA55A������д��������
#else
  CMU_InitStructure.SysClkSel = SysPLL;
  CMU_InitStructure.CPUDiv = CPUDiv1;
  HT_CMU_Init(&CMU_InitStructure);//����ʱ��ΪPLL���22.020096Mhz
#endif
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		
  if(SysTick_Config(0x54000))		// 15.6ms	//T=ticks*(1/f)
  {
    while(1);
  }		
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//ʹ��SysTick�δ�ʱ��
}

