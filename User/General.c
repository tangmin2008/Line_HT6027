#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Measure.h"
#include "TypeMeter.h"
#include "TypeFRAM.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
//#include "RsComm.h"
#include "Initial.h"
#include "Time.h"
#include "Measure.h"
#include "Power.h"
//#include "FreezeData.h"
#include "math.h"					//SmallCurrent
//#include "cpucard.h"
//#include "YearSave.h"									//�¹���	//13.11.29
#if ( AdjustErrorCheck == YesCheck )	//15.09.05
#include "AdjustError.h"
#endif
#include "ECProcess.h"
#include "ActCalendar.h"
#include "Demand.h"
#include "ProfileGeneric.h"
#include "IECComm.h"
	
//**********************************************************************
// System Reset
//**********************************************************************
//void Reset( void )
//{
//  WDTCTL = 0x0000;
//}
//**********************************************************************
// Watchdog Reset
//**********************************************************************
void WDTReset( void )
{
//   WDTCTL = WDT_ARST_1000;
   HT_FreeDog();
}
//**********************************************************************
// Delay
//**********************************************************************
void Delay( unsigned short i )
{
  while( i > 0)
  {
    i--;
  }	
}
/*
void udelay(unsigned int us)
{
  unsigned int i,j;
  for(i=0;i<us;++i)
  {
    for(j=0;j<10;++j)
      ;
    HT_FreeDog();
  }
}*/
/**********************************************************************
	Calculate the checksum Of Data in RAM
**********************************************************************/
unsigned char ChkNum( unsigned char *Ptr, short Len )
{
	unsigned char Num = 0xA5;
   
   	while( Len > 0 )
   	{
    	Num += *Ptr++;
      	Len--;
   	}
   	return Num;
}
unsigned char ChkNumAdd( unsigned char *Ptr, short Len )
{
	unsigned char Num = 0x0;
   
   	while( Len > 0 )
   	{
    	Num += *Ptr++;
      	Len--;
   	}
   	return Num;
}
unsigned short ChkNumShort( unsigned short *Ptr, short Len )
{
	unsigned short Num = 0xA5A5;
   
   	while( Len > 0 )
   	{
    	Num += *Ptr++;
      	Len--;
   	}
   	return Num;
}
//**********************************************************************
// turn byte data to BCD data
//**********************************************************************
unsigned char Byte_BCD(unsigned char Data)
{
   if (Data>=99) return 0x99;
   return( Data/10*0x10 + Data%10 );
}

//**********************************************************************
// turn word data to BCD data
//**********************************************************************
void Word_BCD(unsigned char *Ptr, unsigned short Data)
{
   if (Data>=9999){
      *(Ptr+1) = 0x99;
      *Ptr = 0x99;
      return;
   }

   *(Ptr+1) = Byte_BCD( (unsigned char)(Data/100) );
   *Ptr = Byte_BCD( (unsigned char)(Data%100) );
}

//**********************************************************************
// turn long data to BCD3 data
//**********************************************************************

void Long_BCD3(unsigned char *Ptr, unsigned long Data)
{
	if (Data>=999999){
      	*(Ptr+2) = 0x99;
      	*(Ptr+1) = 0x99;
      	*Ptr = 0x99;
      	return;
   	}
// 	*(Ptr+2) = Data/100000*0x10 + Data/10000;
   	*(Ptr+2) = Byte_BCD(Data/10000);								//13.01.10
   	Word_BCD(Ptr, (unsigned short)(Data%10000));
}

//**********************************************************************
// turn long data to BCD4 data
//**********************************************************************
void Long_BCD4(unsigned char *Ptr, unsigned long Data)
{
	if (Data>=99999999){
      	*(Ptr+3) = 0x99;
      	*(Ptr+2) = 0x99;
      	*(Ptr+1) = 0x99;
      	*Ptr = 0x99;
      	return;
   	}
	Word_BCD(Ptr+2, Data/10000);
	Word_BCD(Ptr, Data%10000);
}
//**********************************************************************
// 
//**********************************************************************
short IsBCD(unsigned char Data)
{
	if(((( Data >> 4) & 0x0f) > 9 )||( ( Data & 0x0f ) > 9 )) return 1;
	return 0;
}

short AllIsBCD(unsigned char* Data, unsigned short Len )
{
	unsigned short i;
	
	for( i=0;i<Len;i++ )
	{		
		if(((( *(Data+i) >> 4) & 0x0f) > 9 )||( ( *(Data+i) & 0x0f ) > 9 )) return 1;	
	}	
	return 0;
}

//**********************************************************************
// turn BCD data to byte data
//**********************************************************************
unsigned char BCD_Byte(unsigned char Data)
{
   return(((Data>>4)&0x0f)*10+(Data&0x0f));
}

//**********************************************************************
// turn BCD2 data to word data
//**********************************************************************
unsigned short BCD2_Word(unsigned char *Ptr)
{
   return(  (short)BCD_Byte(*Ptr)+
            (short)BCD_Byte(*(Ptr+1))*100 );
}

//**********************************************************************
// turn BCD3 data to word data
//**********************************************************************
unsigned long BCD3_Long(unsigned char *Ptr)
{
   return(  (long)BCD_Byte(*Ptr)+
            (long)BCD_Byte(*(Ptr+1))*100+
            (long)BCD_Byte(*(Ptr+2))*10000 );
}

//**********************************************************************
// turn BCD4 data to word data
//**********************************************************************
unsigned long BCD4_Long(unsigned char *Ptr)
{
   return(  (long)BCD_Byte(*Ptr)+
            (long)BCD_Byte(*(Ptr+1))*100+
            (long)BCD_Byte(*(Ptr+2))*10000+
            (long)BCD_Byte(*(Ptr+3))*1000000 );
}

void _BCD1INC(unsigned char* Ptr )			
{
	unsigned char Data;
	
	Data = BCD_Byte(* Ptr);
	Data++;
	if( Data >= 100 ) Data %= 100;
	* Ptr = Byte_BCD(Data);
}	

void _BCD2INC(unsigned char* Ptr )			
{
	unsigned short Data;
	
	Data = BCD2_Word(Ptr);
	Data++;
	if( Data >= 10000 ) Data %= 10000;
	Word_BCD(Ptr, Data );
}		

void _BCD3INC(unsigned char* Ptr )			
{
	uint32_t Data;
	
	Data = BCD3_Long(Ptr);
	Data++;
	if( Data >= 1000000 ) Data %= 1000000;
	Long_BCD3(Ptr, Data);
}	

void _BCD4INC(unsigned char* Ptr )			
{
	uint32_t Data;
	
	Data = BCD4_Long(Ptr);
	Data++;
	if( Data >= 100000000 ) Data %= 100000000;
	Long_BCD4(Ptr, Data);
}	

void _BCD1ADD(unsigned char *Dest, unsigned char *Src)
{
	unsigned char Data1,Data2;

	Data1 = BCD_Byte(* Src);
	Data2 = BCD_Byte(* Dest);
	Data1 += Data2;
	if( Data1 >= 100 ) Data1 %= 100;
	* Dest = Byte_BCD(Data1);
}

void _BCD2ADD(unsigned char *Dest, unsigned char *Src)
{
	unsigned short Data1,Data2;

	Data1 = BCD2_Word(Src);
	Data2 = BCD2_Word(Dest);
	Data1 += Data2;
	if( Data1 >= 10000 ) Data1 %= 10000;
	Word_BCD( Dest, Data1 );
}

void _BCD3ADD(unsigned char *Dest, unsigned char *Src)
{
	uint32_t Data1,Data2;

	Data1 = BCD3_Long(Src);
	Data2 = BCD3_Long(Dest);
	Data1 += Data2;
	if( Data1 >= 1000000 ) Data1 %= 1000000;
	Long_BCD3( Dest, Data1 );

}

void _BCD4ADD(unsigned char *Dest, unsigned char *Src)
{
	uint32_t Data1,Data2;

	Data1 = BCD4_Long(Src);
	Data2 = BCD4_Long(Dest);
	Data1 += Data2;
	if( Data1 >= 100000000 ) Data1 %= 100000000;
	Long_BCD4( Dest, Data1 );
}

void _BCD5ADD(unsigned char *Dest, unsigned char *Src)
{
	uint64_t Data1,Data2;
	unsigned char Temp;

	Data1 = BCD_Byte(*(Src+4));
	Data1 *= 100000000;
	Data1 += BCD4_Long(Src);
	Data2 = BCD_Byte(*(Dest+4));
	Data2 *= 100000000;
	Data2 += BCD4_Long(Dest);
	Data1 += Data2;
	if( Data1 >= 10000000000 ) Data1 %= 10000000000;
	Temp = Data1 / 100000000;	
	*(Dest+4) = Byte_BCD( Temp );
	Long_BCD4( Dest, Data1 % 100000000 );
}

void _BCD6ADD(unsigned char *Dest, unsigned char *Src)
{
	uint64_t Data1,Data2;
	unsigned short Temp;
	
	Data1 = BCD2_Word(Src+4);
	Data1 *= 100000000;
	Data1 += BCD4_Long(Src);
	Data2 = BCD2_Word(Dest+4);
	Data2 *= 100000000;
	Data2 += BCD4_Long(Dest);
	Data1 += Data2;
	if( Data1 >= 1000000000000 ) Data1 %= 1000000000000;
	Temp = Data1 / 100000000;	
	Word_BCD(Dest+4, Temp);
	Long_BCD4( Dest, Data1 % 100000000 );
}

void _BCD1DEC(unsigned char *Dest)
{
	unsigned char Data;
	
	Data = BCD_Byte(* Dest);
	if( Data == 0 ) Data = 99;
	else Data--;
	*Dest = Byte_BCD(Data);
}

void _BCD2SUB(unsigned char *Dest, unsigned char *Src)
{
	unsigned short Data1,Data2;

	Data1 = BCD2_Word(Dest);
	Data2 = BCD2_Word(Src );
	Data1 -= Data2;
	if( Data1 >= 10000 ) Data1 %= 10000;
	Word_BCD( Dest, Data1 );

}	

void _BCD4SUB(unsigned char *Dest, unsigned char *Src)
{
	uint32_t Data1,Data2;

	Data1 = BCD4_Long(Dest);
	Data2 = BCD4_Long(Src);
	Data1 -= Data2;
	if( Data1 >= 100000000 ) Data1 %= 100000000;
	Long_BCD4( Dest, Data1 );

}

void _BCD5SUB(unsigned char *Dest, unsigned char *Src)
{
	uint64_t Data1,Data2;
	unsigned char Temp;

	Data1 = BCD_Byte(*(Dest+4));
	Data1 *= 100000000;
	Data1 += BCD4_Long(Dest);
	Data2 = BCD_Byte(*(Src+4));
	Data2 *= 100000000;
	Data2 += BCD4_Long(Src);
	Data1 -= Data2;
	if( Data1 >= 10000000000 ) Data1 %= 10000000000;
	Temp = Data1 / 100000000;	
	*(Dest+4) = Byte_BCD( Temp );
	Long_BCD4( Dest, Data1 % 100000000 );

}

void _BCD6SUB(unsigned char *Dest, unsigned char *Src)
{
	uint64_t Data1,Data2;
	unsigned short Temp;
	
	Data1 = BCD2_Word(Dest+4);
	Data1 *= 100000000;
	Data1 += BCD4_Long(Dest);
	Data2 = BCD2_Word(Src+4);
	Data2 *= 100000000;
	Data2 += BCD4_Long(Src);
	Data1 -= Data2;
	if( Data1 >= 1000000000000 ) Data1 %= 1000000000000;
	Temp = Data1 / 100000000;	
	Word_BCD(Dest+4, Temp);
	Long_BCD4( Dest, Data1 % 100000000 );
}

void InitPara4(void)
{

}
void InitPara5(void)
{
}

void InitPara6(void)			
{	
}
void InitPara7(void)
{
}
void InitPara(void)
{
  short i;
  unsigned char *p_buf;
  unsigned char buf[8];

  for(i=0;i<MAX_CH_NUM;++i)
  {  
    p_buf =(unsigned char*)&(m_ecpara[i].cmon_day);
    E2P_RData(p_buf,CMon_DAY0+i*25,24);
  }
  E2P_RData(buf,PW_ADDR,1);
  Para.PW = buf[0];
  if(Para.PW!=0x33)
    Para.PW = 0x34;
  if(Para.PW==0x34)
  {
    HT_GPIO_BitsSet(HT_GPIOC,GPIO_Pin_4);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOC,GPIO_Pin_4);
  }
  E2P_RData(buf,CONST_H,4);
  memcpy(&MSpec.RMeterConst,buf,4);
  if((MSpec.RMeterConst<2000) || (MSpec.RMeterConst>400000))
  {
    MSpec.RMeterConst = 20000;
  }
  MSpec.R7022E_HFConst = 1200000/MSpec.RMeterConst;
  //MSpec.R7022E_HFConst = 400000/MSpec.RMeterConst;
 // MSpec.R7022E_HFConst =   4452655/MSpec.RMeterConst;
  //MSpec.R7022E_HFConst =   1447513/MSpec.RMeterConst;
  E2P_RData(buf,BASE_CURRENT,4);
  memcpy(&MSpec.RBaseCurrent,buf,4);
  if((MSpec.RBaseCurrent<1000) || (MSpec.RBaseCurrent>10000))
  {
    MSpec.RBaseCurrent = 1000;
  }
  
  E2P_RData(buf,PW_IB,2);
  memcpy(&MSpec.RPW00002Ib,buf,2);
  if((MSpec.RPW00002Ib<50) || (MSpec.RPW00002Ib>10000))
  {
    MSpec.RPW00002Ib = 288;
  }
  
  E2P_RData(buf,U_SEL,1);
  
  if(buf[0]&0x1)
  {
    HT_GPIO_BitsSet(HT_GPIOG,GPIO_Pin_3);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOG,GPIO_Pin_3);
  }
  
  if(buf[0]&0x2)
  {
    HT_GPIO_BitsSet(HT_GPIOG,GPIO_Pin_2);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOG,GPIO_Pin_2);
  }
  
  if(buf[0]&0x4)
  {
    HT_GPIO_BitsSet(HT_GPIOG,GPIO_Pin_1);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOG,GPIO_Pin_1);
  }
  
  if(buf[0]&0x8)
  {
    HT_GPIO_BitsSet(HT_GPIOG,GPIO_Pin_0);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOG,GPIO_Pin_0);
  }
  
  if(buf[0]&0x10)
  {
    HT_GPIO_BitsSet(HT_GPIOB,GPIO_Pin_11);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOB,GPIO_Pin_11);
  }
  
  if(buf[0]&0x20)
  {
    HT_GPIO_BitsSet(HT_GPIOE,GPIO_Pin_6);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOE,GPIO_Pin_6);
  }
  
  if(buf[0]&0x40)
  {
    HT_GPIO_BitsSet(HT_GPIOE,GPIO_Pin_5);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOE,GPIO_Pin_5);
  }
  if(buf[0]&0x80)
  {
    HT_GPIO_BitsSet(HT_GPIOE,GPIO_Pin_4);
  }
  else
  {
    HT_GPIO_BitsReset(HT_GPIOE,GPIO_Pin_4);
  }
  
  
  InitPara7();		
}

void AllECClr( void )
{
}

#if 0
void CloseADC12( void )
{
//	RTCPS0CTL &= ~RT0PSIFG;				//10.11.26
//	RTCPS0CTL &= ~RT0PSIE;				//1/4096���ж�,�ж�ʹ�ܹ�
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
	HT_GPIOB->PTDIR &= ~GPIOB_VEMU_CTL;//PDIR_CTLQSY &= ~P_CTLQSY;			//�رյ����Ŵ��·	
#endif
//#if ( MEASCHIP == IDT90E32 )							//12.08.29	
//	P6SEL = 0xC0;		//V2
	HT_GPIOB->AFCFG &= ~(GPIOA_BAT_ADC0+GPIOA_BAT_ADC1);
	HT_GPIOB->IOCFG &= ~(GPIOA_BAT_ADC0+GPIOA_BAT_ADC1);
//#else
//	P6SEL = 0xFF;		//V2												//�¹���	//13.09.12
//#endif
	
//	HT_TBS->TBSIE = 0x00000000;
	HT_TBS->TBSCON &= ~(TBS_TBSCON_ADC0En + TBS_TBSCON_ADC1En); 
	HT_TBS->TBSIE &= ~(TBS_TBSIE_ADC0IE + TBS_TBSIE_ADC1IE);
	NVIC_DisableIRQ(TBS_IRQn);
}

void ADCStart( short Mode )
{
	SM.ClockBatAD = 0;
	SM.CommBatAD = 0;	
#if( AllFailChk == YesCheck )	
	SM.UaSampleSum = 0;
	SM.UbSampleSum = 0;
	SM.UcSampleSum = 0;
	SM.IaSampleSum = 0;
	SM.IbSampleSum = 0;
	SM.IcSampleSum = 0;	 
#else
	Mode = BATVOLTAD;		//ֻ����ص�ѹ
#endif
	SM.ADSampleMode = Mode;
	OpenADC12( Mode );
//	TBCCR0 = TBR + 0x418;		//4M 250us	1M  1MS
//#if( AllFailChk == YesCheck )	
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
	if( SM.ADChannelNo < 6 ) 
	{
		SM.SampleCnt = SAMPCYC*82+2;
		HT_GPIOB->PTDIR |= GPIOB_VEMU_CTL;//PDIR_CTLQSY |= P_CTLQSY;	//�رյ����Ŵ��·			
	}			
	else SM.SampleCnt = BATADCYC+2;
#else
	SM.SampleCnt = BATADCYC+2;
#endif
//	RTCPS0CTL &= ~RT0PSIFG;				//10.11.26
//	RTCPS0CTL |= RT0PSIE;				//1/4096���ж�,�ж�ʹ�ܿ�
}	

void CloseADC12( void )
{
//	RTCPS0CTL &= ~RT0PSIFG;				//10.11.26
//	RTCPS0CTL &= ~RT0PSIE;				//1/4096���ж�,�ж�ʹ�ܹ�
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
	HT_GPIOB->PTDIR &= ~GPIOB_VEMU_CTL;//PDIR_CTLQSY &= ~P_CTLQSY;			//�رյ����Ŵ��·	
#endif
//#if ( MEASCHIP == IDT90E32 )							//12.08.29	
//	P6SEL = 0xC0;		//V2
	HT_GPIOB->AFCFG &= ~(GPIOA_BAT_ADC0+GPIOA_BAT_ADC1);
	HT_GPIOB->IOCFG &= ~(GPIOA_BAT_ADC0+GPIOA_BAT_ADC1);
//#else
//	P6SEL = 0xFF;		//V2												//�¹���	//13.09.12
//#endif
	
//	HT_TBS->TBSIE = 0x00000000;
	HT_TBS->TBSCON &= ~(TBS_TBSCON_ADC0En + TBS_TBSCON_ADC1En); 
	HT_TBS->TBSIE &= ~(TBS_TBSIE_ADC0IE + TBS_TBSIE_ADC1IE);
	NVIC_DisableIRQ(TBS_IRQn);
}

void OpenADC12( short Mode )
{
//	if( Mode == CURRENTAD )	
//	{
//		SM.ADChannelNo = 3;											
//		ADC12MCTL0 = ADC12INCH_0 + ADC12SREF_1 + ADC12EOS;		
//	}	
///	else 
//	{
//		SM.ADChannelNo = 6;
		HT_TBS->TBSCON = TBS_TBSCON_ADC0En + TBS_TBSCON_ADC1En; 	
//	}	
	
	HT_TBS->TBSIE = TBS_TBSIE_ADC0IE + TBS_TBSIE_ADC1IE;
	NVIC_EnableIRQ(TBS_IRQn); 		
	Delay(1000);
}   


void ADCEnd( void )
{
//#if( AllFailChk == YesCheck )	
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
	unsigned short i;
	unsigned long Temp,Temp1;
	unsigned char Buff[14];
	unsigned char* Point;
//	unsigned char* Ptr;
	float f1,f2,f3;
	unsigned long long LValue;

	Point = Buff;

	if( SM.ADSampleMode == 0 )
	{	
		if((Flag.Power & F_IrmsCheck) != 0 )		//У׼
		{
			RAM_Fill( Point, 14);
			Flag.Power &= ~F_IrmsCheck;
//			for( i=0;i<6;i++ )
			for( i=3;i<6;i++ )						//ȡ��ȫʧѹ��3·��ѹ�����ͼ���
			{
//				Temp = *IrmsPara[i].SampleSum / SAMPCYC;				//10.07.29
//				RAM_Write( Point, (unsigned char*)&Temp, 4 );			//10.07.29	
				E2P_RData( Point, UaBase+i*7, 6 );						//10.07.29
//#if ( MEASCHIP == ADE7758 )
#if (( MEASCHIP == ADE7758 )||( MEASCHIP == ADE7878 ))	
				if( i<= 2 )	RAM_Write( Point+4, Real.Ua+i*2, 2 );
				else
				{
//					RAM_Write( Point+4, Real.Ia+(i-3)*3, 3 );
//					*(Point+7) = 0;
//					RAM_Write( (unsigned char*)&Temp1, Point+4, 4 );				
//					Temp1 /= 10;
//					RAM_Write( Point+4, (unsigned char*)&Temp1, 2 );										
					RAM_Write( Point+4, Real.Ia+1+(i-3)*4, 2 );
				}	
#else
				Read_ATTValue( ATUrmsA+i, Point+4 );				
				if( i > 2 )	
				{
//					*(Point+7) = 0;
//					Temp1 = BCD4_Long( Point+4 );				
//					Temp1 /= 10;
//					Long_BCD4( Point+4, Temp1 );
					RAM_Write( Point+4, Point+5, 2 );
				}	
#endif			
				E2P_WData( UaBase+i*7, Point, 6 );
			}	
		}
		else if( Flag.Power & F_IbIrmsCheck )
		{
			Flag.Power &= ~F_IbIrmsCheck;
			SM.HWCAllFailState |= 0xE2;			//10.10.06		
			for( i=0;i<3;i++ )
			{
				E2P_RData( Point, IaBase+i*7, 6 );
				Temp1 = *IrmsPara[i+3].SampleSum / SAMPCYC;					
				RAM_Write( Point, (unsigned char*)&Temp1, 4 );				
				E2P_WData( IaBase+i*7, Point, 6 );
			}			
		}	
		else if( Flag.Power & F_Ib10IrmsCheck )
		{
			Flag.Power &= ~F_Ib10IrmsCheck;
			SM.HWCAllFailState |= 0xE4;			//10.10.06		
			for( i=0;i<3;i++ )
			{
				Temp1 = *IrmsPara[i+3].SampleSum / SAMPCYC;					
				RAM_Write( Point+i*2, (unsigned char*)&Temp1, 2 );				
			}	
			E2P_WData( IOffSet, Point, 6 );			
		}	
		else if( Flag.Power & F_Ib00IrmsCheck )
		{
			Flag.Power &= ~F_Ib00IrmsCheck;
			SM.HWCAllFailState |= 0xE1;			//10.10.06		
			for( i=0;i<3;i++ )
			{
//				Temp1 = (*IrmsPara[i+3].SampleSum / SAMPCYC)/80;					
				Temp1 = (*IrmsPara[i+3].SampleSum / SAMPCYC)/82;					
				RAM_Write( (unsigned char*)&SM.IaOffset0+i*2, (unsigned char*)&Temp1, 2 );
			}	
			E2P_WData( IOffSet0, (unsigned char*)&SM.IaOffset0, 6 );			
		}	
		else
		{
			if(( Flag.Power & F_PwrUp ) == 0 )
			{
				E2P_RData( Point+8, IOffSet, 6 );					
//				for( i=0;i<6;i++ )
				for( i=3;i<6;i++ )						//ȡ��ȫʧѹ��3·��ѹ�����ͼ���
				{
					E2P_RData( Point, UaBase+i*7, 6 );
					RAM_Write( (unsigned char*)&Temp, Point, 4 );
					Temp1 = *IrmsPara[i].SampleSum / SAMPCYC;					
					if( i > 2 )
					{ 
						f1 = Temp;
						f2 = ((float)*(Point+8+1+(i-3)*2)*256 + *(Point+8+(i-3)*2))*10 ;
						f1 = f2 - f1;
						f2 = (float)Temp * 9;
						f3 = (float)Temp1*f2 - (float)Temp*f1;
						f1 = f3/( f2 - f1 );	
						if( f1 < 0 ) Temp1 = 0; //Test
						else Temp1 = (unsigned long)f1;
					}

//					if( i > 2 ) Temp1 *= 10;					//������λС��
					if( i > 2 ) Temp1 *= 100;					//������λС��
					LValue = Temp1;
//					Temp = (Temp1 * BCD2_Word(Point+4)) / Temp;	
//					Temp = (unsigned long)(((float)Temp1 * BCD2_Word(Point+4)) / Temp);	
					LValue *= BCD2_Word(Point+4);
					Temp = LValue / Temp;	
//					if(( i > 2 )&&( Temp <= In003A )) Temp = 0;	//�¹���		//13.08.30	
					if(( i > 2 )&&( Temp <= ( MSpec.RBaseCurrent / 25 ))) Temp = 0;	//�¹���		//13.08.30	

//					if(( i < 3 )&&( Temp >= 50 )) Temp += 25 ;	//����5V�ĵ�ѹ��2.5V				
					if( i < 3 )
					{
#if( V10Un == 22 )
//						if( Temp >= 50 )
						if( Temp >= 20 )
						{
							if( Temp <=250 ) Temp += 18 ;	//С��25V�ĵ�ѹ��2.5V	512K			//TT
							else Temp += 30 ;	//����5V�ĵ�ѹ��3.0V	512K			//TT							
						}	
#elif( V10Un == 10 )
//						if( Temp >= 40 ) 
						if( Temp >= 20 ) 
						{
							if( Temp <=116 ) Temp += 10 ;	//С��13V�ĵ�ѹ��0.8V	512K			//TT
							else Temp += 13 ;	//����4V�ĵ�ѹ��1.3V	512K			//TT					
						}	
#elif( V10Un == 6 )
						if( Temp >= 20 ) 
						{
							if( Temp <=100 ) Temp += 6 ;	//С��10V�ĵ�ѹ��0.5V	512K			//TT
							else Temp += 8 ;	//����3V�ĵ�ѹ��0.8V	512K			//TT
						}	
#else
#endif
					}
#if( LinkMode == Phase3Wire3 )	
					if((i==1)||(i==4)) Temp = 0;
#else
#endif				
//					Word_BCD(Para.Ua+i*2, Temp);
					if( i<= 2 )	
					{
						Word_BCD(Real.Ua+i*2, Temp);						
					}
					else
					{
//						Para.Ia[0] = 0;
//						Para.Ib[0] = 0;
//						Para.Ic[0] = 0;
//						Word_BCD(Para.Ia+(i-3)*3+1, Temp);		//��ȷ����λС��
//						Long_BCD3(Real.Ia+(i-3)*3, Temp);		//��ȷ����λС��
//						Long_BCD3(Real.SIa+(i-3)*3, Temp);		//��ȷ����λС��
						Long_BCD4(Real.Ia+(i-3)*4, Temp);		//��ȷ����λС��
						Long_BCD3(Real.SIa+(i-3)*3, Temp/10);	//��ȷ����λС��
					}		
				}
			}	
		}	
	}
	GetBatVolt();		
#else
	GetBatVolt();		
#endif
}	

void GetBatVolt( void )
{
	short i;
//	unsigned short Temp;
	unsigned long Temp;
	unsigned long* Ptr;
	unsigned char Buff[4];
	unsigned char* Point;
	unsigned char OldState;						//�¹���
#if ( ClockBatResisterIs2M == YesCheck	)			//ʱ�ӵ�ز���������5M��Ϊ2M //17.02.23
	float FTemp;
#endif
		
	OldState = Flag.MeterState1;				//�¹���
		
	Point = Buff;
	*Point = UCLOCKBATL; 
	*(Point+1) = UCLOCKBATH;
	*(Point+2) = UCOMMBATL; 
	*(Point+3) = UCOMMBATH; 
	Flag.MeterState1 &= ~(F_ClockBat+F_CommBat);

	Ptr = &SM.ClockBatAD; 
	for( i=0;i<2;i++ )
	{		
//		Word_BCD(Para.UClockBat+i*2, (*Ptr * 750) / (BATADCYC*4096));			//1/3��ѹ	//1/5��ѹ
/*		if( SM.ADSampleMode == CURRENTAD )
		{
			Temp = ((*Ptr * 750) / ((unsigned long)SAMPCYC*80*4096));
			if( i == 0 ) Temp -= 10;
			else Temp += 45;
		}
		else
		{
			Temp = (*Ptr * 750) / ((unsigned long)BATADCYC*4096);
			Temp -= 10;			
		}		
*/		
//		Temp = (*Ptr * 750) / ((unsigned long)BATADCYC*4096);
#if ( ClockBatResisterIs2M == YesCheck	)			//ʱ�ӵ�ز���������5M��Ϊ2M //17.02.23
		if( i == 0 )
		{
			FTemp = (float)*Ptr;	
			FTemp *= 4.9215;		//��ѹ����Ϊ510k��2M	
			FTemp /= 100000;
			Temp = (unsigned long)FTemp;
			Temp += 2;
			if(( Temp > 366 )&&( Temp < 386 )) Temp = 365;
		}
		else	 
		{
			Temp = (*Ptr)* 11;		//2016.09.29clf		//��ѹ����Ϊ1M��10M	
			Temp += 550000;          //17.02.22
			Temp /= 100000;
		}	
#else
		Temp = (*Ptr)* 11;		//2016.09.29clf
		Temp += 55000;
		Temp /= 100000;
#endif

//		if( Temp > 5 ) Temp -= 5;
		if( Temp < 100 ) Temp = 0;
#if( CommBatVolt ==	BatVolt6V )		
		if(( i == 1 )&&( Temp < 500 )) Temp = 0;
#endif		
		Word_BCD(Real.UClockBat+i*2, Temp );

//		if( Data_Comp( Real.UClockBat+i*2, Point, 2) >= 0 ) Flag.MeterState1 |= ByteBit[i+2];
		if( Data_Comp( Real.UClockBat+i*2, Point, 2) < 0 ) Flag.MeterState1 |= ByteBit[i+2];
		Ptr += 1;
		Point += 2;	
	}		
	
	if((( OldState & F_ClockBat ) == 0 )&&(( Flag.MeterState1 & F_ClockBat ) != 0 )) UpdateEventUpState( 3 );			//�¹���
	if((( OldState & F_CommBat ) == 0 )&&(( Flag.MeterState1 & F_CommBat ) != 0 )) UpdateEventUpState( 8 );			//�¹���
	
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	if(( Flag.Power & F_PwrUp ) != 0 )											//�ϵ�����²ű�����״̬��EEPROM	//�¹���	//14.04.28		
	{
		*Point = Flag.MeterState1 & ( F_ClockBat+F_CommBat );						
		*(Point+1) = OldState & ( F_ClockBat+F_CommBat );						
		Para.RBatState = *Point;			//16.05.09
		if( *Point != *(Point+1))												
		{
			E2P_WData( EBatState, Point, 1 );
		}	
	}	
#endif
		
}	


#if ( MEASCHIP == ADE7758 )
#elif ( MEASCHIP == ADE7878 )
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
#else

//#if ( MEASCHIP == ATT7022D )							//10.09.14
//#else													//10.09.14
#if ( MEASCHIP == ATT7022E )									//11.09.24
#else
float GetPhasePW( unsigned char Cmd )								//SmallCurrent
{
	unsigned short	i=1,Temp;
	float Value=0;
	unsigned char Buff[4];
	unsigned char* Data;
	unsigned long PS32;					//�¹���		//13.08.30	
	
	Data = Buff;
	
	if( (Flag.Power & F_PwrUp) == 0)	//ͣ���Ժ󷵻�ֵΪ�㡣
	{
		*Data = 0;
		*(Data + 1) = 0;	
		*(Data + 2) = 0;	
		return 0;
	}	

	PS32 = MSpec.RMeterConst / 100;		//�¹���		//13.08.30	

	ATT7022RdReg( Cmd, Data );

	if(( *(Data +2) & 0x80 ) == 0) i = 0;
	else    {
	            *(Data+2) ^= 0xff;     
	            *(Data+1) ^= 0xff;     
	            *Data ^= 0xff;     	                	                
	        }   
	Temp = *(Data + 1) * 256 + *Data;
	Value = (float)Temp + (float)*(Data + 2) * 65536;

	if( i == 1 ) 	Value += 1;
	Value = Value * 100;
	switch( Cmd )
	{
		case ATPWPA:		
		case ATPWPB:		
		case ATPWPC:		
		case ATPWQA:		
		case ATPWQB:	
		case ATPWQC:		
//�¹���		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//				Value =  ( Value * 25 ) / ( 2 * PS32 ) ;  //PS32����10��
//#else
				Value =  ( Value * 5 ) / ( 4 * PS32 ) ;   //Value = ( Value * 10 * 32 ) / (256 * ps32); 
//#endif
			break;
		case ATPWPZ:			//10.06.02
		case ATPWQZ:			//10.06.02
//�¹���		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//				Value = ( Value * 50 ) / PS32 ;            //PS32����10��
//#else
				Value = ( Value * 5 ) / PS32 ;            //Value = ( Value * 10 * 32 ) / ( 64 * PS32 );
//#endif
			break;
		default: break;
	}	
	return Value;
}
#endif

void GetSmallI( short Phase )										//SmallCurrent
{
    unsigned char Buff[4];
    unsigned char* Point;
	unsigned short Volt;
	double Temp;
	long Pt,Qt;

    Point = Buff;
        
	Volt = BCD2_Word( Real.Ua+Phase*2 );
	if( Volt < 150 )return;
	
	Pt = (long)GetPhasePW( ATPWPA + Phase );
	Qt = (long)GetPhasePW( ATPWQA + Phase );
	Temp = (double)Pt*Pt + (double)Qt*Qt;
	Temp = sqrt(Temp);
//	Temp = (Temp*10) / Volt;
	Temp = (Temp*100) / Volt;
	Long_BCD4(Point,(long)Temp);
//	RAM_Write( Real.Ia+Phase*3, Point, 3 );
	RAM_Write( Real.Ia+Phase*4, Point, 4 );
}
//#endif										//10.09.14

#if ( MEASCHIP == ATT7022E )									//11.09.24
#else
void GetATAngle( unsigned char Addr, unsigned char* Data )
{
	unsigned char Buff[8];
	unsigned char* Point;
		
	Point = Buff;
	
	if(( Addr <= ATAngleZ )&&( Addr >= ATAngleA ))
	{
		RAM_Write( Point, Data, 3 );
		RAM_Fill( Point+3, 5 );
		*(Point+5) = 0x18;
		switch( GetMQuad( Real.PWFlag,(short)(Addr - ATAngleA) ) )
		{
			case 3: _BCD3ADD( Point+4, Point);
					RAM_Write( Point, Point+4, 3 );
					break;
			case 4: *(Point+5) = 0x36;
			case 2:	_BCD4SUB( Point+4, Point );
					RAM_Write( Point, Point+4, 3 );
					break;
			default: break;	
		}	
		RAM_Write( Data, Point, 3 );		
	}	
	if( *(Data+1) == 0x36 ) *(Data+1) = 0;
}	
#endif

//����������Ϊ��ʱ���ж����޹������Ƿ񶼺�С������С����������Ϊ1.000							//10.06.02			
void GetPfValue( unsigned char* Source, short Phase )													
{
	unsigned long Pt;
	unsigned long Qt;
	unsigned long Value;
	
#if ( MEASCHIP == ATT7022E )									//11.09.24
#else
	if(( *Source != 0 )||(*(Source+1) != 0 )) return;		//�������ط��㣬������
#endif
	
	Pt = (unsigned long)GetPhasePW( ATPWPA + Phase );
	Qt = (unsigned long)GetPhasePW( ATPWQA + Phase );
//	Value = PW00002Ib;										//�¹���		//13.08.30
	Value = MSpec.RPW00002Ib;								//�¹���		//13.08.30	
	Value *= 2;												//�¹���		//13.10.25		
	if(( Pt < Value )&&( Qt < Value ))
	{
		*Source = 0x00;
		*(Source+1) = 0x10;			
	}	
}

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
//�����޹����ʶ���С����������Ϊ1.000,�����Ϊ0							//14.09.09			
void GetPfAngleValue( unsigned char* PfSource, unsigned char* AngleSource, short Phase )													
{
	unsigned long Pt;
	unsigned long Qt;
	unsigned long Value;
	
	Pt = (unsigned long)GetPhasePW( ATPWPA + Phase );
	Qt = (unsigned long)GetPhasePW( ATPWQA + Phase );
//	Value = PW00002Ib;										//�¹���		//13.08.30
	Value = MSpec.RPW00002Ib;								//�¹���		//13.08.30	
	Value *= 2;
	if(( Pt < Value )&&( Qt < Value ))
	{
		*PfSource = 0x00;
		*(PfSource+1) = 0x10;	
		*AngleSource = 0;
		*(AngleSource+1) = 0;		
	}	
}
#endif

void ModifyATAngle( unsigned char* Source, short Phase )
{
	unsigned short Temp,Temp1;
	
	Temp = BCD2_Word( Source );

	if((( Temp >= 895 )&&( Temp <= 905 ))||(( Temp >= 2695 )&&( Temp <= 2705 )))
	{
		Temp1 = BCD2_Word((unsigned char*)&Real.Pfa+Phase*2);
		if( Temp1 == 1000 ) 
		{
			*Source = 0;	
			*(Source+1) = 0;	
		}			
	}			
}	

void GetVoltAmper( void )
{
	unsigned char Buff[4];
	unsigned char* Point;
	unsigned long Value;
	long Value2;					//�¹���
	unsigned long A001Ib;			//�¹���		//13.08.30
	unsigned long A0001Ib;			//�¹���		//13.08.30
    short i;							
	
	Point = Buff;

	A001Ib = MSpec.RBaseCurrent / 100;		//�¹���		//13.08.30
//	A0001Ib = MSpec.RBaseCurrent / 1000;	//�¹���		//13.08.30
	A0001Ib = MSpec.RDisA0001Ib;			//�¹���		//13.10.23
	
//	RAM_Write( Real.PreUa, Real.Ua, 30 );	
//	RAM_Fill( Real.Ua, 30 );
	RAM_Write( Real.PreUa, Real.Ua, 34 );	
	RAM_Fill( Real.Ua, 34 );
	for( i=0;i<6;i++ )
	{
#if( LinkMode == Phase3Wire3 )	
		if((i==1)||(i==4)) continue;
#else
#endif		
		Read_ATTValue( ATUrmsA+i, Point );
		if( i < 3) 
		{
//			if( BCD2_Word( Point ) < 55 ) RAM_Fill( Point, 2 );				//��ѹС��5V����
			if( BCD2_Word( Point ) < 25 ) RAM_Fill( Point, 2 );				//��ѹС��2.5V����	//�¹���	//13.12.11
			RAM_Write( (unsigned char*)&Real.Ua+i*2, Point, 2 );					
		}	
		else 
		{
//			RAM_Write( (unsigned char*)&Real.Ia+(i-3)*3, Point, 3 );		
			RAM_Write( (unsigned char*)&Real.Ia+(i-3)*4, Point, 4 );		
            Value = BCD4_Long( Point );
			if( (Flag.Power & F_PwrUp) != 0 )									//SmallCurrent
			{																	//SmallCurrent
//				if( BCD3_Long( Point ) <= A001Ib )								//SmallCurrent
//				if( BCD4_Long( Point ) <= A001Ib )								//SmallCurrent
				if( Value <= A001Ib )		            						//SmallCurrent
				{																//SmallCurrent
//#if ( MEASCHIP == ATT7022D )					//10.09.14
//#else											//10.09.14	
					GetSmallI(i-3);												//SmallCurrent
					Value = BCD4_Long( (unsigned char*)&Real.Ia+(i-3)*4 );
//#endif											//10.09.14
					if( Value <= A0001Ib )
					{
						Value = 0;
						RAM_Fill( (unsigned char*)&Real.Ia+(i-3)*4, 4 );
					}	
//					RAM_Write( Point, (unsigned char*)&Real.Ia+(i-3)*3, 3 );	//SmallCurrent
					RAM_Write( Point, (unsigned char*)&Real.Ia+(i-3)*4, 4 );	//SmallCurrent
				}																//SmallCurrent
//				if( BCD4_Long( Point ) <= A0001Ib )
//				if( Value <= A0001Ib )
//				{
//					RAM_Fill( Point, 4 );
//				}	
			}																	//SmallCurrent	
//			Long_BCD3( Point, BCD4_Long( Point )/10 );		
            Value /= 10;
            Long_BCD4( Point, Value );		
			if(( Real.PWFlag & ByteBit[i-3] ) != 0 ) *(Point+2) |= 0x80;	//����ֵ������
			RAM_Write( (unsigned char*)&Real.SIa+(i-3)*3, Point, 3 );		
		}	
		if( i < 4 ) 
		{
#if( LinkMode == Phase3Wire3 )					//�¹���	//14.01.18
			if( i == 1 ) continue;				//�¹���	//14.01.18
#endif											//�¹���	//14.01.18	
			Read_ATTValue( ATFactorA + i , Point );
			switch( GetMQuad( Real.PWFlag, i ))
			{
				case 2:
//				case 4:
				case 3:
					*(Point+1) |= 0x80;	//���������ڶ���������ʱΪ����������
					break;
				default: break;						
			}	
			RAM_Write( (unsigned char*)&Real.Pfa+i*2, Point, 2 );	
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			if( i == 3 ) GetPfValue( (unsigned char*)&Real.Pfa+i*2, i );								//14.09.09
#else
			GetPfValue( (unsigned char*)&Real.Pfa+i*2, i );												//10.06.02	 	
#endif			
#if( LinkMode == Phase3Wire3 )	
			Real.Pfb[0] = 0;							//13.06.09	
			Real.Pfb[1] = 0;							//13.06.09	
#endif
		}	
		
//#if ( NLineCheck ==	YesCheck )
//		if( i > 2 ) continue;		
//		Read_ATTValue( ATYUaUb+i, Point );		//��ѹ���
//		RAM_Write( (unsigned char*)&Real.YUaUb+i*2, Point, 2 );
//#else
//#endif		
	}		
#if( LinkMode == Phase3Wire3 )					//�¹���	//14.01.18
	RAM_Fill( (unsigned char*)&Real.It, 4 );	//�¹���	//14.01.18
	RAM_Fill( (unsigned char*)&Real.SIt, 3 );	//�¹���	//14.01.18
#else											//�¹���	//14.01.18
	Read_ATTValue( ATIrmsZ, Point );		
    Value = BCD4_Long( Point );
//	if( BCD4_Long( Point ) <= A0002Ib )
//	if( Value <= A0002Ib )
//	if( Value < (A0002Ib*2) )					//13.02.01
	if( Value < (A0001Ib*4) )					//�¹���		//13.08.30
	{
		Value = 0;
		RAM_Fill( Point, 4 );
	}					
//	RAM_Write( (unsigned char*)&Real.It, Point, 3 );	
	RAM_Write( (unsigned char*)&Real.It, Point, 4 );	
//	Word_BCD( Para.It, BCD2_Word( Para.It )*10 );			
//	Long_BCD3( Point, BCD4_Long( Point )/10 );		
	Value /= 10;
	Long_BCD4( Point, Value );		
	RAM_Write( (unsigned char*)&Real.SIt, Point, 3 );	
#endif											//�¹���	//14.01.18	

	for( i=0;i<3;i++ )
	{
		Read_ATTValue( ATYUaUb+i, Point );		//��ѹ���
		RAM_Write( (unsigned char*)&Real.YUaUb+i*2, Point, 2 );
	}	
#if ( MEASCHIP == ATT7022E )									//11.09.24
	for( i=0;i<3;i++ )
	{
		if( Read_ATTValue( ATAngleA+i, Point ) == 1 )		//���
		{
			Word_BCD( Point, 3600 - BCD2_Word( Point ));			
		}	
		if( *(Point+1) == 0x36 ) *(Point+1) = 0;
		RAM_Write( (unsigned char*)&Real.AngleA+i*2, Point, 2 );
		ModifyATAngle( (unsigned char*)&Real.AngleA+i*2, i );				//10.06.02
	}	
#else
	for( i=0;i<4;i++ )
	{
		Read_ATTValue( ATAngleA+i, Point );		//���
		GetATAngle( ATAngleA+i, Point );
		RAM_Write( (unsigned char*)&Real.AngleA+i*2, Point, 2 );
		ModifyATAngle( (unsigned char*)&Real.AngleA+i*2, i );				//10.06.02
	}	
#endif	
#if( LinkMode == Phase3Wire3 )	
	Real.AngleB[0] = 0;							//13.06.09	
	Real.AngleB[1] = 0;							//13.06.09	
#endif

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	for( i=0;i<3;i++ )										//14.09.09
	{
#if( LinkMode == Phase3Wire3 )					//�¹���	//14.01.18
		if( i == 1 ) continue;					//�¹���	//14.01.18
#endif											//�¹���	//14.01.18	
		GetPfAngleValue( (unsigned char*)&Real.Pfa+i*2, (unsigned char*)&Real.AngleA+i*2, i );												 	
	}	
#endif

/*
	Real.PWFlag = 0;							//PWMD
	for( i=0;i<12;i++ )							//�й����޹������ڹ���
	{
#if( LinkMode == Phase3Wire3 )	
		if((i==1)||(i==5)||(i==9)) continue;
#else
#endif		
		Temp = Read_ATTValue( ATPWPA+i, Point );					//PWMD			
		RAM_Write( (unsigned char*)&Real.Pa+i*3, Point, 3 );
		if(( Temp == 1 )&&( i < 8 )) Real.PWFlag |= ByteBit[i];		//PWMD
	}		
*/
/*	if(( Flag.Power & F_PwrUp ) != 0 )				//ATChk
	{												//ATChk	
		ATT7022RdReg( ATVoltFlag, Point );			//ATChk	
		if(( *Point & 0x80 ) != 0 ) ATT7022RdReg( ATVoltFlag, Point );			//ATChk	
		Real.SFlag = *Point;						//ATChk	
		
	}else Real.SFlag = 0;							//ATCHK
*/	
	Read_ATTValue( ATFreq, Point );
	RAM_Write( (unsigned char*)&Real.Freq, Point, 2 );	
	
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T��7022�ڲ��¶ȴ�����	//10.08.03
	if((( Flag.Power & F_PwrUp ) != 0 )&&( SM.TestDisCnt == 0 ))	
	{
		ATT7022RdReg( ATTempD, Point );							
//		*(Point+1) = 0;											
//		i = (short)*Point * 10;									
		if( *Point > 128 ) i = *Point - 256;					
		else i = *Point;										
#if ( MEASCHIP == ATT7022E )				//11.09.24
		Value2 = i;
		Value2 *= 726;
		Value2 /= 100;								//�¹���
		i = Value2;
#else        
		i = i * 10;												
#endif
#if ( NEW7022E == YesCheck )
		i = 250 - i;								//�°�7022EУ׼��25��	
#else
		i = SM.RTempDelta - i;									
#endif
		Clk.Temp = i;											
		if( i<0 ) i = 0 - i;			
//		Word_BCD(&Real.Wendu[0], (unsigned short)i );				
//		if( Clk.Temp < 0 ) Real.Wendu[1] |= 0x80; 				
		Word_BCD(&Real.MChipWendu[0], (unsigned short)i );										//�¹���	
		if( Clk.Temp < 0 ) Real.MChipWendu[1] |= 0x80; 											//�¹���
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T��7022�ڲ��¶ȴ�����	//10.08.03	//�¹���
//		if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//�͵�ƽΪ8025T									//�¹���
//		{
			RAM_Write( (unsigned char*)&Real.Wendu, (unsigned char*)&Real.MChipWendu, 2 );		//�¹���								
//		}	
//#endif																						//�¹���	
	}
//#endif	
	
	if( SM.TestDisCnt == 0 ) 
	{
		GetSumVolt();				//�ۼ�ÿ���ܵ�ѹ			
		GetSumPower();				//�ۼ�ÿ���ܹ���
	}	

}	

void GetSFlag( void )
{
	unsigned char Buff[4];
	unsigned char* Point;
	
	Point = Buff;
	if(( Flag.Power & F_PwrUp ) != 0 )				//ATChk
	{												//ATChk	
		ATT7022RdReg( ATVoltFlag, Point );			//ATChk	
		if(( *Point & 0x80 ) != 0 ) ATT7022RdReg( ATVoltFlag, Point );			//ATChk	
		Real.SFlag = *Point;						//ATChk	
		
	}else Real.SFlag = 0;							//ATCHK
}	

#endif


void IncYearMonth( unsigned char* TMon )
{
	_BCD1INC(TMon);			//����ʱ���¼�1
	if( *TMon > 0x12 )
	{
		*TMon = 1; 			//��Ϊ1��
		_BCD1INC(TMon+1);	//���1
	}	 
}

#if ( PowerOnMonSaveMAX12 == YesCheck )						//11.06.27
#else
//Type = 0; ��һ������
//Type = 1; �ڶ�������
//Type = 2; ����������
short AdjustMon( short Type )
{
//	unsigned char Buff1[4];
//	unsigned char Buff2[4];
//	unsigned char Buff3[4];
	unsigned char Buff4[2];
//	unsigned char* LastTime;
//	unsigned char* TestTime;
//	unsigned char* CurTime;
//	unsigned short i;
	
//	LastTime = Buff1;
//	TestTime = Buff2;
//	CurTime = Buff3;
	
	E2P_RData( Buff4, ESave_Day1+Type*3, 2 );

	if( Data_Comp( Buff4, Clk.Hour, 2 ) != 0 ) return 0;
/*
	if(( Buff4[0] == 0x99 )&&( Buff4[1] == 0x99 )) return 0;		//�ǽ�����
	if(( Buff4[0] == 0xFF )&&( Buff4[1] == 0xFF )) return 0;		//�ǽ�����
	
	E2P_RData( LastTime, LastSaveTime, 4 );				//�ϴν���ʱ��

	RAM_Write( CurTime, Clk.Hour, 4 );					//��ǰʱ��

	if( Data_Comp( LastTime, CurTime, 4 ) == 1 ) 
	{
		RAM_Write( LastTime, CurTime, 4 );				//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
		E2P_WData( LastSaveTime, LastTime, 4 );			//�ϴν���ʱ��	
	}	
	if( Data_Comp( LastTime+1, CurTime+1, 3 ) == 0 ) return 0;							//����ǰʱ����ϴν���ʱ��Ϊͬһ�죬������
	
	RAM_Write( TestTime, LastTime, 4 );	
//	E2P_RData( TestTime, ESave_Day1+Type*3, 2 );	//ȡ�ϴν���ʱ������ºͽ����յ���ʱ��ɲ���ʱ��
	RAM_Write( TestTime, Buff4, 2 );				//ȡ�ϴν���ʱ������ºͽ����յ���ʱ��ɲ���ʱ��	
	if( *(LastTime+1) == *(TestTime+1) ) IncYearMonth( TestTime+2 );	//���ϴν��������������ͬ�������ʱ���1���¡�

	for( i=0;i<800;i++ )
	{
		WDTCTL = WDT_ARST_1000;
		if( Data_Comp( TestTime, CurTime, 4 ) == 1 ) return 0;	//����ʱ����ڵ�ǰʱ�䣬�����㣬�˳�			
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 ) break;	//����ʱ������ϴν���ʱ���ҵ�ǰʱ����ڵ��ڲ���ʱ�䣬�����		
		IncYearMonth( TestTime+2 );		//����ʱ���¼�1
	}	
	if( i == 800 ) return 0;			//ѭ����������������������˳���
*/

	E2P_WData( LastSaveTime, &Clk.Hour[0], 4 );	
	
	return 1; 	
}	
#endif

#if ( PowerOnMonSaveMAX12 == YesCheck )						//11.06.27
#if ( SaveDaySort == YesCheck )								//12.02.10
short JudgeSaveDay( unsigned char* Source )
{
	if(( AllIsBCD( Source, 2 ) != 0 )||( JudgeDayH( Source ) != 0 ))
	{
		return 1;	
	}	
	else return 0;	
}	

void SwapBuff( unsigned char* Dest, unsigned char* Source, short Len )
{
	unsigned char Buff[6];
	
	if( Len > 6 ) return;
	
	RAM_Write( Buff, Source, Len );
	RAM_Write( Source, Dest, Len );
	RAM_Write( Dest, Buff, Len );
	
}	

void SaveDayCompose( unsigned char* SaveDayBuff, unsigned char* SequenceTab ) 
{
	unsigned char Tab[3];
	unsigned char Flag[3];
	unsigned char Temp; 
	unsigned short i;
	unsigned short Start;
	unsigned short End;
	
	Tab[0] = 0;
	Tab[1] = 1;
	Tab[2] = 2;
	
	Flag[0] = 0;
	Flag[1] = 0;
	Flag[2] = 0;
	
	for( i=0;i<3;i++ )
	{
		if( JudgeSaveDay( SaveDayBuff+i*2 ) != 0 ) Flag[i] = 1;		//�Ƿ�Ϊ��Ч������
	}	

	Temp = Flag[0] + Flag[1] + Flag[2];									//��Ч����������

	if( Temp == 0 )														//ȫ��Ϊ��Ч������	
	{
		if( Data_Comp( SaveDayBuff, SaveDayBuff+2, 2 ) > 0 )		//��һ�����մ��ڵڶ�������
		{
			SwapBuff( SaveDayBuff, SaveDayBuff+2, 2 );			//��������������
			SwapBuff( &Tab[0], &Tab[1], 1 );							//�������������
		}
		if( Data_Comp( SaveDayBuff+2, SaveDayBuff+4, 2 ) > 0 )	//�µڶ������մ��ڵ���������
		{
			SwapBuff( SaveDayBuff+2, SaveDayBuff+4, 2 );			//��������������
			SwapBuff( &Tab[1], &Tab[2], 1 );				            //�����½��������
		}
		if( Data_Comp( SaveDayBuff, SaveDayBuff+2, 2 ) > 0 )		//�µ�һ�����մ����µڶ�������
		{                                                                                        
			SwapBuff( SaveDayBuff, SaveDayBuff+2, 2 );	        //��������������         
			SwapBuff( &Tab[0], &Tab[1], 1 );				            //�����½��������         
		}
	}	
	else if( Temp == 1 )												//����1��Ϊ��Ч������		
	{
		for( i=0;i<3;i++ )
		{
			if( Flag[i] != 0 ) break;									//�ҳ���Ч������
		}		
		switch( i )
		{
			case 0: Start = 1; End = 2; break;							//�ҳ���Ч������
			case 1: Start = 0; End = 2; break;	
			case 2: Start = 0; End = 1; break;	
			default: break;			
		}	
		if( i < 3 )
		{
			if( Data_Comp( SaveDayBuff+Start*2, SaveDayBuff+End*2, 2 ) > 0 )	//��Ч�����ռ��Ƿ���Ҫ����
			{
				SwapBuff( SaveDayBuff+Start*2, SaveDayBuff+End*2, 2 );	
				SwapBuff( &Tab[Start], &Tab[End], 1 );				
			}
		}	
	}	

	RAM_Write( SequenceTab, Tab, 3 );

	return;

}	

#endif

//ˢ���ϴν���ʱ��
void UpdateLastSaveTime( unsigned char* MonSaveDayBuff )			//�¹���
{
	unsigned char Buff[160];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char Buff3[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SaveTime;
	unsigned char* Ptr;	
	unsigned short i,j;
	unsigned char SequenceTab[3];
	unsigned char SaveDayBuff[6];
	unsigned char YearMonthBuff[2];
	unsigned char MonSaveCnt;
	
	LastTime = Buff1;
	CurTime = Buff2;
	SaveTime = Buff3;
	
	Ptr = Buff;
	
	E2P_RData( LastTime, LastSaveTime, 4 );							//�ϴν���ʱ��
	RAM_Write( CurTime, Clk.Hour, 4 );								//��ǰʱ��
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) return;
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
		E2P_WData( LastSaveTime, LastTime, 4 );							
		return;				
	}
	
#if ( SaveDaySort == YesCheck )										//12.02.10
	SequenceTab[0] = 0;
	SequenceTab[1] = 1;
	SequenceTab[2] = 2;
	RAM_Write( SaveDayBuff, MonSaveDayBuff, 6 );
	SaveDayCompose( SaveDayBuff, (unsigned char*)&SequenceTab[0] );
#else
	RAM_Write( SaveDayBuff, MonSaveDayBuff, 6 );
#endif	

	RAM_Fill( Ptr, 160 );
	RAM_Write( YearMonthBuff, CurTime+2, 2 );
	for( i=0;i<13;i++ )												//ȡ�����13���µĽ����ʱ��	
	{
		for( j=0;j<3;j++ )
		{
//			RAM_Write( Ptr+i*4+j*4, SaveDayBuff+j*2, 2 );			//ÿ��3������ʱ��
			RAM_Write( Ptr+(12-i)*12+j*4, SaveDayBuff+j*2, 2 );		//ÿ��3������ʱ��					//�¹���	//13.10.06
//			RAM_Write( Ptr+(12-i)*4+j*4+2, YearMonthBuff, 2 );		//��ǰ�£�����12���µĽ���ʱ��	
			RAM_Write( Ptr+(12-i)*12+j*4+2, YearMonthBuff, 2 );		//��ǰ�£�����12���µĽ���ʱ��		//�¹���	//13.10.06
		}	
		MonthDec( YearMonthBuff );									//����13����
	}	
	
	MonSaveCnt = 0;
	for( i=0;i<39;i++ )
	{
		TestTime = Ptr+(38-i)*4;									//�ӵ�ǰ�µĵ��������տ�ʼ����	
		if( DateTime4ValueCheck( TestTime ) != 0 ) continue;		//�������ڵ�ʱ��Ƿ�����Ҫ����ĳһ�����ղ�����ʱ 	//����9999��FFFF���ġ�����
		if( Data_Comp( TestTime, CurTime, 4 ) > 0 ) continue;		//�������ڵ�ʱ����ڵ�ǰʱ�䣬��δ��ʱ�䣬����������ж�
		
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//�������ڵ�ʱ������ϴν���ʱ�䣬����Ҫ�����ʱ���
		{
			RAM_Write( SaveTime, TestTime, 4 ); 					//������ʱ���	
			MonSaveCnt += 1;
			if( MonSaveCnt >= 12 ) break;							//��ಹ��12�ν���
		}
		else break;													//�����Ѿ��������ʱ����
	}
	
	if( MonSaveCnt != 0 )											
	{
		if( *SaveTime == 0 ) 
		{
			DayDec( SaveTime+1 );
			*SaveTime = 0x23; 										//13.10.06
		}	
		else *SaveTime = Byte_BCD( BCD_Byte(*SaveTime)-1 );			//���ϴν���ʱ�䱣��Ϊ�Ȳ⵽������Ľ������һ��Сʱ������Ч���в�����
		if( Data_Comp( SaveTime, LastTime, 4 ) == 1 )				//��������������������ϴν���ʱ�䣬������ʱ����Ϊ�ϴν���ʱ�䡣
		{
			E2P_WData( LastSaveTime, SaveTime, 4 );
		}	
	}		
}

void CreateMonthSaveTimeRec( unsigned short EventNo, unsigned char* SaveTime )							//�¹���
{
	unsigned char Buff[sizeof(MONTHSAVETIME_REC)];
	MONTHSAVETIME_REC* RecPtr;
	long LastPt;
	
	RecPtr = (MONTHSAVETIME_REC*)Buff;

	LastPt = PtAddrChk( EventNo, Next );		//��λ��һ����Կ���¼�¼��ַ		
	if( LastPt == -1 ) return;
	
	RAM_Write( RecPtr->HappenDate, SaveTime, 4 );
	E2P_WData( (unsigned short)LastPt, RecPtr->HappenDate, sizeof(MONTHSAVETIME_REC) );
	
}

//MonSaveDayBuff: ���������������ַ
//MonSaveCnt������������
short AdjustMon( unsigned char* MonSaveDayBuff, short* MonSaveCnt )
{
	unsigned char Buff[24];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SixSaveDay;	
	unsigned short i;
#if ( SaveDaySort == YesCheck )										//12.02.10
	unsigned char SequenceTab[3];
	unsigned char SaveDayBuff[6];
#endif
	
	LastTime = Buff1;
	CurTime = Buff2;
	
	SixSaveDay = Buff;
	
	E2P_RData( LastTime, LastSaveTime, 4 );							//�ϴν���ʱ��
	RAM_Write( CurTime, Clk.Hour, 4 );								//��ǰʱ��
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) 
	{
		*MonSaveCnt = 0;											//��ǰʱ��Ƿ��������㡣		
		return 0;				
	}	
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
		E2P_WData( LastSaveTime, LastTime, 4 );							
		*MonSaveCnt = 0;											
		return 0;				
	}	

	for( i=0;i<6;i++ )
	{
		RAM_Write( SixSaveDay+2+i*4, LastTime+2, 2 );				//�ϴν���ʱ������·��������
	}	

#if ( SaveDaySort == YesCheck )										//12.02.10
	SequenceTab[0] = 0;
	SequenceTab[1] = 1;
	SequenceTab[2] = 2;
	RAM_Write( SaveDayBuff, MonSaveDayBuff, 6 );
	SaveDayCompose( SaveDayBuff, (unsigned char*)&SequenceTab[0] );
	for( i=0;i<3;i++ )
	{
		RAM_Write( SixSaveDay+i*4, SaveDayBuff+i*2, 2 );			//�ϴν���ʱ������º��������������ǰ����������
		RAM_Write( SixSaveDay+12+i*4, SaveDayBuff+i*2, 2 );		//�ϴν���ʱ������µ���һ���º�������������ɺ�����������
		IncYearMonth( SixSaveDay+14+i*4 );
	}	
#else
	for( i=0;i<3;i++ )
	{
		RAM_Write( SixSaveDay+i*4, MonSaveDayBuff+i*2, 2 );			//�ϴν���ʱ������º��������������ǰ����������
		RAM_Write( SixSaveDay+12+i*4, MonSaveDayBuff+i*2, 2 );		//�ϴν���ʱ������µ���һ���º�������������ɺ�����������
		IncYearMonth( SixSaveDay+14+i*4 );
	}	
#endif
	
	TestTime = SixSaveDay;
	for( i=0;i<6;i++ )
	{
		if( DateTime4ValueCheck( TestTime ) != 0 ) 					//�������ڵ�ʱ��Ƿ�����Ҫ����ĳһ�����ղ�����ʱ
		{															//����9999��FFFF���ġ�����
			TestTime += 4;
			continue;	
		}	
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//�������ڵ�ʱ������ϴν���ʱ��
		{
			if( Data_Comp( CurTime, TestTime, 4 ) == -1 ) break;	//��ǰʱ����С�ڲ���ʱ�䣬�򲻽��н���
			else 
			{
//				CreateMonthSaveTimeRec( MonthSaveTime_No, TestTime );	//�¹���				
				RecordFreeze( SaveDayFreeze_No, DIRECT );			//�����ն���
#if ( LocalSmartMeter == YesCheck )
				if( ICComm.YearMonSaveMode == 0 ) RecordFreeze( StepSaveSWFreeze_No, DIRECT );	//���ݽ��㶳��		//�¶���
#endif
				if( *MonSaveCnt != 0 )
				{
					*MonSaveCnt -= 1;
					if( *MonSaveCnt == 0 ) 							//�����������12�Σ�12�κ󣬽���ǰʱ������ϴν���ʱ��
					{												//����ֹ���㡣	
						RAM_Write( TestTime, CurTime, 4 ); 
					}
				}	
				E2P_WData( LastSaveTime, TestTime, 4 );				//���㣬�ò������ڵ�ʱ����Ϊ�ϴν���ʱ��	
#if ( SaveDaySort == YesCheck )										//12.02.10
				return SequenceTab[i%3]+1;							//����1����һ�����ա�2���ڶ������ա�3������������
#else
				return (i%3)+1;										//����1����һ�����ա�2���ڶ������ա�3������������
#endif
			}	
		}	
		TestTime += 4;
	}	
	*MonSaveCnt = 0;												//�����Ͻ���������		
	return 0;	
}
#endif

#if ( PowerOnMonSaveMAX12 == YesCheck )						//11.06.27
//MonSaveCnt������������
short AdjustMonVolt( short* MonSaveCnt )
{
	unsigned char Buff[8];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SixSaveDay;	
	unsigned short i;
	
	LastTime = Buff1;
	CurTime = Buff2;	
	
	SixSaveDay = Buff;
	
	E2P_RData( LastTime, VoltLastSaveTime, 4 );						//�ϴε�ѹ�ϸ��ʽ���ʱ��
	RAM_Write( CurTime, Clk.Hour, 4 );								//��ǰʱ��
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) 
	{
		*MonSaveCnt = 0;											//��ǰʱ��Ƿ��������㡣		
		return 0;				
	}	
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
//		E2P_WData( LastSaveTime, LastTime, 4 );							
		E2P_WData( VoltLastSaveTime, LastTime, 4 );					//10.07.27		
		*MonSaveCnt = 0;											
		return 0;				
	}	

	RAM_Write( SixSaveDay+2, LastTime+2, 2 );						//�ϴε�ѹ�ϸ��ʽ���ʱ������·��������
	RAM_Write( SixSaveDay+6, LastTime+2, 2 );						//�ϴε�ѹ�ϸ��ʽ���ʱ������·��������
	*SixSaveDay = 0x00;												//��Ȼ�µĵ�һ����ʱ���������
	*(SixSaveDay+1) = 0x01;
	*(SixSaveDay+4) = 0x00;
	*(SixSaveDay+5) = 0x01;
	IncYearMonth( SixSaveDay+6 );									//�ڶ��������·ݼ�һ

	TestTime = SixSaveDay;
	for( i=0;i<2;i++ )
	{
		if( DateTime4ValueCheck( TestTime ) != 0 ) 					//�������ڵ�ʱ��Ƿ�
		{															//����
			TestTime += 4;
			continue;	
		}	
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//�������ڵ�ʱ������ϴν���ʱ��
		{
			if( Data_Comp( CurTime, TestTime, 4 ) == -1 ) break;	//��ǰʱ����С�ڲ���ʱ�䣬�򲻽��н���
			else 
			{
				if( *MonSaveCnt != 0 )
				{
					*MonSaveCnt -= 1;
					if( *MonSaveCnt == 0 ) 							//�����������12�Σ�12�κ󣬽���ǰʱ������ϴν���ʱ��
					{												//����ֹ���㡣	
						RAM_Write( TestTime, CurTime, 4 ); 
					}
				}	
				E2P_WData( VoltLastSaveTime, TestTime, 4 );			//���㣬�ò������ڵ�ʱ����Ϊ�ϴν���ʱ��	
				return 1;											//����1���н���
			}	
		}	
		TestTime += 4;
	}	
	*MonSaveCnt = 0;												//�����Ͻ���������		
	return 0;	
}

#else
short AdjustMonVolt( void )
{
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char Buff3[4];
	unsigned char Buff4[2];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned short i;
	
	LastTime = Buff1;
	TestTime = Buff2;
	CurTime = Buff3;
	
//	E2P_RData( Buff4, ESave_Day1+Type*3, 2 );
//	if(( Buff4[0] == 0x99 )&&( Buff4[1] == 0x99 )) return 0;		//�ǽ�����

	Buff4[0] = 0x00;
	Buff4[1] = 0x01;									//��ѹ�ϸ���Ϊ��Ȼ�½���1�����
		
	E2P_RData( LastTime, VoltLastSaveTime, 4 );				//�ϴν���ʱ��

	RAM_Write( CurTime, Clk.Hour, 4 );					//��ǰʱ��

	if( Data_Comp( LastTime, CurTime, 4 ) == 1 ) 
	{
		RAM_Write( LastTime, CurTime, 4 );				//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
		E2P_WData( VoltLastSaveTime, LastTime, 4 );		//�ϴν���ʱ��	
	}	
	if( Data_Comp( LastTime+1, CurTime+1, 3 ) == 0 ) return 0;							//����ǰʱ����ϴν���ʱ��Ϊͬһ�죬������
	
	RAM_Write( TestTime, LastTime, 4 );	
	RAM_Write( TestTime, Buff4, 2 );				//ȡ�ϴν���ʱ������ºͽ����յ���ʱ��ɲ���ʱ��	
	if( *(LastTime+1) == *(TestTime+1) ) IncYearMonth( TestTime+2 );	//���ϴν��������������ͬ�������ʱ���1���¡�

	for( i=0;i<800;i++ )
	{
//		WDTCTL = WDT_ARST_1000;
   		HT_FreeDog();
		if( Data_Comp( TestTime, CurTime, 4 ) == 1 ) return 0;	//����ʱ����ڵ�ǰʱ�䣬�����㣬�˳�			
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 ) break;	//����ʱ������ϴν���ʱ���ҵ�ǰʱ����ڵ��ڲ���ʱ�䣬�����		
		IncYearMonth( TestTime+2 );		//����ʱ���¼�1
	}	
	if( i == 800 ) return 0;			//ѭ����������������������˳���

	E2P_WData( VoltLastSaveTime, &Clk.Hour[0], 4 );	
	
	return 1; 	
}	
#endif

#if ( PowerOnMonSaveMAX12 == YesCheck )						//11.06.27
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
void MoveSaveDay1EC( void )									//�¹���	
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	
	Ptr = Buff;
	RAM_Fill( Ptr, 6 );
	
	E2P_RData( Ptr, L1_SaveDay1EC_Pp0, 4 );
	E2P_WData( L2_SaveDay1EC_Pp0, Ptr, 4 );
	E2P_RData( Ptr, L1_SaveDay1EC_Pn0, 4 );
	E2P_WData( L2_SaveDay1EC_Pn0, Ptr, 4 );
	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	E2P_WData( L1_SaveDay1EC_Pp0, Ptr, 4 );
	Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	E2P_WData( L1_SaveDay1EC_Pn0, Ptr, 4 );
	
}	
#endif

short Mon_Save(void)
{
//	short i,j,k;
	short i,k;
//	long LastPt;
	unsigned char Buff[6];
	short Mon_SaveCnt;
	unsigned char MDSaveState;
	
	//�Ȳ����¶������ݣ����⵱�����������������			//17.01.21
	Mon_SaveCnt = 12;
	for( k=0;k<12;k++ )														//����������12��				
	{	
		if((Flag.Power & F_PwrUp) == 0 ) break;								//��⵽��������жϽ���
		if( AdjustMonVolt( &Mon_SaveCnt ) == 1 )
		{
			RecordFreeze( MonthFreeze_No, DIRECT );			//�¶���
			InitMonQualifiyRateData();			
		}	
		if( Mon_SaveCnt == 0 ) break;
    }

	E2P_RData( (unsigned char*)&Buff[0], ESave_Day1, 2 );
	E2P_RData( (unsigned char*)&Buff[2], ESave_Day2, 2 );
	E2P_RData( (unsigned char*)&Buff[4], ESave_Day3, 2 );
	
	UpdateLastSaveTime( Buff );												//�¹���

	Mon_SaveCnt = 12;														//����������12��
	MDSaveState = 1;														//������һ�ν����ǣ��豣�浱ǰ����
	for( k=0;k<12;k++ )														//����������12��
	{	
		if((Flag.Power & F_PwrUp) == 0 ) break;								//��⵽��������жϽ���
		i = AdjustMon( Buff, &Mon_SaveCnt );
		if( i != 0 )
		{
//			ECSave();
			if( i == 1 ) 
			{
				if( MDSaveState == 1 ) MD_Rest(0);						//������һ�ν����豣�浱ǰ����
//				else MDZeroSave();										//��������ʱ��ֻ������ʷ������¼�����㼴�ɣ��Լӿ�����ٶȡ�
				MDSaveState = 0;						
#if ( SaveDay1CycleEC == YesCheck )											//�¹���							//12.10.04		
				MoveSaveDay1EC();										//�¹���
#endif
			}	
//			else 
//			{
//				LastPt = PtAddrChk( LastMD_No, Next );					//��λ��һ������������ַ������ΪȫFFFF
//				if( LastPt != -1 ) 
//				{
//					j = 0xFFFF;
//					DataFlash_Write( LastPt, (unsigned char*)&j, 2 );	//����Ӧ�������ռ�������Ϊ0xff
//					DataFlash_Write( LastPt+4*4096, (unsigned char*)&j, 2 );	//����Ӧ���������ݿռ�������Ϊ0xff
//				}	
//			}	    
		} 	
		if( Mon_SaveCnt == 0 ) break;
	}

/*	//�Ȳ����¶������ݣ����⵱�����������������			//17.01.21
	Mon_SaveCnt = 12;
	for( k=0;k<12;k++ )														//����������12��
	{	
		if((Flag.Power & F_PwrUp) == 0 ) break;								//��⵽��������жϽ���
		if( AdjustMonVolt( &Mon_SaveCnt ) == 1 )
		{
//			for( j=0;j<4;j++ )
//			{
//				LastPt = PtAddrChk( VoltCheck_No+j, Next );					//��λ��һ�ε�ѹ�ϸ��ʼ�¼��ַ		
//				if( LastPt == -1 ) continue;
//				E2P_WData( LastPt, VoltCheckPara[j].VoltChk->CheckTime, 27 );	//��ѹ�ϸ�������ת��		
//			}
			RecordFreeze( MonthFreeze_No, DIRECT );			//�¶���

//			InitAllQualifiyRateData();			
			InitMonQualifiyRateData();			
		}	
		if( Mon_SaveCnt == 0 ) break;
    }
*/

	return 0;
}

#else

short Mon_Save(void)
{
	short i,j;
	long LastPt;
	
	for( i=0;i<3;i++ )
	{
		if( AdjustMon(i) == 1 )
		{
			ECSave();
//			MD_Rest(0);
			if( i == 0 ) MD_Rest(0);									//��������ֻ�Ե�һ��������Ч
			else 
			{
				LastPt = PtAddrChk( LastMD_No, Next );					//��λ��һ������������ַ������ΪȫFFFF
				if( LastPt != -1 ) 
				{
//					j = 0;
					j = 0xFFFF;
					DataFlash_Write( LastPt, (unsigned char*)&j, 2 );	//����Ӧ�������ռ�������Ϊ0xff
					DataFlash_Write( LastPt+4*4096, (unsigned char*)&j, 2 );	//����Ӧ���������ݿռ�������Ϊ0xff
				}	
			}	
    
//			E2P_DataMove( L2VoltMonitor, L1VoltMonitor,23, 1 );
//			E2P_WData( L1VoltMonitor, Para.RVoltQualifiyRate, 23 );	//��ѹ�ϸ�������ת��		
//			Clr_E2PData( CMonVoltMonitor, 23, 1 );
//			InitQualifiyRateData();			

/*
			for( j=0;j<4;j++ )
			{
				LastPt = PtAddrChk( VoltCheck_No+j, Next );				//��λ��һ�ε�ѹ�ϸ��ʼ�¼��ַ		
//				E2P_WData( LastPt, VPara.VoltChk_Rec.CheckTime+27*j, 27 );	//��ѹ�ϸ�������ת��		
				E2P_WData( LastPt, VoltCheckPara[j].VoltChk->CheckTime, 27 );	//��ѹ�ϸ�������ת��		
//				Clr_E2PData(  VoltCheckPara[j].CurMonAddr, 27, 1 );	//��ѹ�ϸ�������ת��		//�����б��汾�µ�ѹ�ϸ��ʣ����ٱ�����EEPROM��
//				RAM_Fill( VoltCheckPara[j].VoltChk->CheckTime, 27 );
//				E2P_WFM( VoltCheckPara[j].CurMonAddr, VoltCheckPara[j].VoltChk->CheckTime, 27 );	 //���ѹ�ϸ�������		
			}
			InitAllQualifiyRateData();			
*/
		}	
	}	

	if( AdjustMonVolt() == 1 )
	{
		for( j=0;j<4;j++ )
		{
			LastPt = PtAddrChk( VoltCheck_No+j, Next );				//��λ��һ�ε�ѹ�ϸ��ʼ�¼��ַ		
			if( LastPt == -1 ) continue;
			E2P_WData( LastPt, VoltCheckPara[j].VoltChk->CheckTime, 27 );	//��ѹ�ϸ�������ת��		
		}
		InitAllQualifiyRateData();			
	}	

	return 0;
}
#endif

/*		//16.09.20		//698����Ҫ
void ECSave( void )
{
	long LastPt;
		
	LastPt = PtAddrChk( LastEC_No, Next );				//��λ��һ������������ַ		
	if( LastPt == -1 ) return;
	ECDataRecord( LastPt, 4*4096 );						//������

}	

void GetECClrRec( unsigned char* Dest, unsigned long Fls_Src )
{
	short i;
	
	Read_Flash( Dest, Fls_Src, 10 );
	for( i=0;i<24;i++ )
	{
//		Read_Flash( Dest+10+i*4, Fls_Src+10+ECClrAddrTab[i], 4 );
		Read_Flash( Dest+10+i*4, Fls_Src+11+ECClrAddrTab[i], 4 );		//����С��λ
	}
}	

#if ( TakeMDClrRecFrameApart ==	YesCheck )			//11.02.17
#else
void GetMDClrRec( unsigned char* Dest, unsigned long Fls_Src )
{
	short i;
    unsigned short Addr;
	
	Read_Flash( Dest, Fls_Src, 10 );
    Fls_Src += 10;
	for( i=0;i<24;i++ )
	{
        Addr = MDClrAddrTab[i];
        Addr -= CMon_MD_Pp0;
		Read_Flash( Dest+10+i*8, Fls_Src+Addr, 8 );
	}
}
#endif

void CreateECClrRec( void )
{
	unsigned char Buff[10];
	unsigned char* Ptr;	
	long LastPt;
		
	Ptr = Buff;

//	IncEventCunt( ECClr_No );
	LastPt = PtAddrChk( ECClr_No, Next );				//��λ��һ������������ַ		
	if( LastPt == -1 ) return;
	IncEventCunt( ECClr_No );
	RAM_Write( Ptr, Clk.Sec, 6 );
	RAM_Write( Ptr+6, Comm.OperateCode, 4 );			//�����ߴ���
	DataFlash_Write( LastPt, Buff, 10 );
	ECDataRecord( LastPt+10, 0 );
	
}								

void ECDataRecord( unsigned long Fls_Dest, unsigned short BackSpace )
{
	unsigned char Buff[280];
	unsigned char* Ptr;	
	unsigned short i;
	unsigned short Addr;
		
	WDTCTL = WDT_ARST_1000;
	Ptr = Buff;
	Addr = CMon_EC_P0;
	for( i=0;i<45;i++ )
	{
		Read_CMonEC( Ptr, Addr );
		*(Ptr+ECDIGIT) = ChkNum( Ptr, ECDIGIT );
		Ptr += (ECDIGIT+1);
		Addr += (ECDIGIT+1);		
	}
	DataFlash_Write( Fls_Dest, Buff, 45*(ECDIGIT+1) );
	WDTCTL = WDT_ARST_1000;
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace, Buff, 45*(ECDIGIT+1) );		//����
	
	WDTCTL = WDT_ARST_1000;
	Ptr = Buff;
	Addr = CMon_EC_Sp0;
	for( i=0;i<17;i++ )
	{
		Read_CMonEC( Ptr, Addr );
		*(Ptr+ECDIGIT) = ChkNum( Ptr, ECDIGIT );
		Ptr += (ECDIGIT+1);
		Addr += (ECDIGIT+1);		
	}
	DataFlash_Write( Fls_Dest+45*(ECDIGIT+1), Buff, 17*(ECDIGIT+1) );
	WDTCTL = WDT_ARST_1000;
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+45*(ECDIGIT+1), Buff, 17*(ECDIGIT+1) );		//����
	
	WDTCTL = WDT_ARST_1000;
	Ptr = Buff;
	Addr = CMon_EC_PpA0;
	for( i=0;i<25;i++ )
	{
		Read_CMonEC( Ptr, Addr );				//ȡ��A���������
		*(Ptr+ECDIGIT) = ChkNum( Ptr, ECDIGIT );
		Ptr += (ECDIGIT+1);
		Addr += (ECDIGIT+1);		
	}
	DataFlash_Write( Fls_Dest+(45+17)*(ECDIGIT+1), Buff, 25*(ECDIGIT+1) );
	WDTCTL = WDT_ARST_1000;
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+(45+17)*(ECDIGIT+1), Buff, 25*(ECDIGIT+1) );		//����
	
	WDTCTL = WDT_ARST_1000;
	Ptr = Buff;
	Addr = CMon_EC_PpB0;
	for( i=0;i<25;i++ )
	{
		Read_CMonEC( Ptr, Addr );				//ȡ��B���������
		*(Ptr+ECDIGIT) = ChkNum( Ptr, ECDIGIT );
		Ptr += (ECDIGIT+1);
		Addr += (ECDIGIT+1);		
	}
	DataFlash_Write( Fls_Dest+(45+17+25)*(ECDIGIT+1), Buff, 25*(ECDIGIT+1) );
	WDTCTL = WDT_ARST_1000;
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+(45+17+25)*(ECDIGIT+1), Buff, 25*(ECDIGIT+1) );		//����

	Ptr = Buff;
	Addr = CMon_EC_PpC0;
	for( i=0;i<25;i++ )
	{
		Read_CMonEC( Ptr, Addr );				//ȡ��C���������
		*(Ptr+ECDIGIT) = ChkNum( Ptr, ECDIGIT );
		Ptr += (ECDIGIT+1);
		Addr += (ECDIGIT+1);		
	}
	DataFlash_Write( Fls_Dest+(45+17+25+25)*(ECDIGIT+1), Buff, 25*(ECDIGIT+1) );
	WDTCTL = WDT_ARST_1000;
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+(45+17+25+25)*(ECDIGIT+1), Buff, 25*(ECDIGIT+1) );		//����

}								

#if ( PowerOnMonSaveMAX12 == YesCheck )						//11.06.27
void MDZeroDataRecord( unsigned long Fls_Dest, unsigned short BackSpace )
{
	unsigned char Buff[280];
	unsigned char* Ptr;	
	unsigned short i;
		
	Ptr = Buff;
	
	RAM_Fill( Ptr, 30*9);
	for( i=0;i<30;i++ ) *(Ptr+8+i*9) = 0xA5;
	
	DataFlash_Write( Fls_Dest, Buff, 20*9 );											//ȡ���޹���������������
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace, Buff, 20*9 );				//����
	
	DataFlash_Write( Fls_Dest+20*9, Buff, 30*9 );										//ȡ���޹������ޡ�������������	
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+20*9, Buff, 30*9 );		//����
	
	DataFlash_Write( Fls_Dest+50*9, Buff, 18*9 );										//ȡ��A����������	
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+50*9, Buff, 18*9 );		//����
	
	DataFlash_Write( Fls_Dest+68*9, Buff, 18*9 );										//ȡ��B����������
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+68*9, Buff, 18*9 );		//����

	DataFlash_Write( Fls_Dest+86*9, Buff, 18*9 );										//ȡ��C����������		
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+86*9, Buff, 18*9 );		//����
	
}								

void MDZeroSave( void )
{
	long LastPt;
		
	LastPt = PtAddrChk( LastMD_No, Next );				//��λ��һ������������ַ		
	if( LastPt == -1 ) return;
	MDZeroDataRecord( LastPt, 4*4096 );						//������

}
#endif

void MDSave( void )
{
	long LastPt;
		
	LastPt = PtAddrChk( LastMD_No, Next );				//��λ��һ������������ַ		
	if( LastPt == -1 ) return;
	MDDataRecord( LastPt, 4*4096 );						//������

}

//void CreateMDClrRec( void )
void CreateMDClrRec( unsigned char* OperateCode )
{
	unsigned char Buff[10];
	unsigned char* Ptr;	
	long LastPt;
		
	Ptr = Buff;

//	IncEventCunt( MDClr_No );
	LastPt = PtAddrChk( MDClr_No, Next );				//��λ��һ�����������¼��ַ		
	if( LastPt == -1 ) return;
	IncEventCunt( MDClr_No );
	RAM_Write( Ptr, Clk.Sec, 6 );
//	RAM_Write( Ptr+6, Comm.OperateCode, 4 );			//�����ߴ���
	RAM_Write( Ptr+6, OperateCode, 4 );					//�����ߴ���
	DataFlash_Write( LastPt, Buff, 10 );
	MDDataRecord( LastPt+10, 0 );
	
}								

void MDDataRecord( unsigned long Fls_Dest, unsigned short BackSpace )
{
	unsigned char Buff[280];
	unsigned char* Ptr;	
	unsigned short i;
	unsigned short Addr;
		
	Ptr = Buff;
	Addr = CMon_MD_Pp0;
	for( i=0;i<20;i++ )
	{
		E2P_RData( Ptr, Addr, 8 );			//ȡ���޹���������������
		*(Ptr+8) = ChkNum( Ptr, 8 );
		Ptr += 9;
		Addr += 9;
	}
	DataFlash_Write( Fls_Dest, Buff, 20*9 );
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace, Buff, 20*9 );		//����
	
	Ptr = Buff;
	Addr = CMon_MD_Q10;
	for( i=0;i<30;i++ )
	{
		E2P_RData( Ptr, Addr, 8 );			//ȡ���޹������ޡ�������������
		*(Ptr+8) = ChkNum( Ptr, 8 );
		Ptr += 9;
		Addr += 9;
	}
	DataFlash_Write( Fls_Dest+20*9, Buff, 30*9 );
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+20*9, Buff, 30*9 );		//����
	
	Ptr = Buff;
	Addr = CMon_MD_PpA0;
	for( i=0;i<18;i++ )
	{
		E2P_RData( Ptr, Addr, 8 );			//ȡ��A����������
		*(Ptr+8) = ChkNum( Ptr, 8 );
		Ptr += 9;
		Addr += 9;
	}
	DataFlash_Write( Fls_Dest+50*9, Buff, 18*9 );
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+50*9, Buff, 18*9 );		//����
	
	Ptr = Buff;
	Addr = CMon_MD_PpB0;
	for( i=0;i<18;i++ )
	{
		E2P_RData( Ptr, Addr, 8 );			//ȡ��B����������
		*(Ptr+8) = ChkNum( Ptr, 8 );
		Ptr += 9;
		Addr += 9;
	}
	DataFlash_Write( Fls_Dest+68*9, Buff, 18*9 );
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+68*9, Buff, 18*9 );		//����

	Ptr = Buff;
	Addr = CMon_MD_PpC0;
	for( i=0;i<18;i++ )
	{
		E2P_RData( Ptr, Addr, 8 );			//ȡ��C����������
		*(Ptr+8) = ChkNum( Ptr, 8 );
		Ptr += 9;
		Addr += 9;
	}
	DataFlash_Write( Fls_Dest+86*9, Buff, 18*9 );
	if( BackSpace != 0 ) DataFlash_Write( Fls_Dest+BackSpace+86*9, Buff, 18*9 );		//����
	
}								
*/

//���¼����������������㵱ǰ�Ѿ������ĵ�ǰ������
void MD_InitialCalPara( unsigned char NewMaxT )
{
//	MD.MDPCT = 0;
	MD.MDPMinCT = 0;
	MD.MD_PCT = 0;
	MD.MD_Cunt = 0;
	MD.MD_CuntT = 0;
	MD.MD_BufAds = 0;
	MD.MDInitialTime = NewMaxT;			//ͷһ�������ڲ���������
#if ( FeeMDIndependence	== YesCheck )		//11.01.28
	MD.FeeMDInitialTime = NewMaxT;			//ͷһ�������ڲ���������
#endif

	RAM_Fill( (unsigned char*)&MD.MD_CumPp, 128 );		
	MD.MD_CumPCnt = 0;	

	RAM_Fill( MD.MD_P, 33 );				//�¹���	//13.10.25

}	

/*
void MD_Rest( short Type )
{
	if( Type == 0 )
	{
//		MDSave();
		;
	}
	else 
	{
		;
	}	
	Clr_E2PData( CMon_MD_Pp0, 8, 104 );  	//����

	Clr_E2PData( PpMDOver_Pp0, 9, 6 );  	//�ڼ�����
	Clr_E2PData( FreezeMD_Pp0, 9, 2 );  	//�����ڼ�����

	MD.MDPMinCT = 0;
	MD.MD_PCT = 0;
	MD.MD_Cunt = 0;
	MD.MD_CuntT = 0;
	MD.MD_BufAds = 0;
	MD.MDInitialTime = Para.RMaxT;			//ͷһ�������ڲ���������
#if ( FeeMDIndependence	== YesCheck )		//11.01.28
	MD.FeeMDInitialTime = Para.RMaxT;		//ͷһ�������ڲ���������
#endif

	RAM_Fill( (unsigned char*)&MD.MD_CumPp, 128 );		
	MD.MD_CumPCnt = 0;	
	
	RAM_Fill( MD.MD_P, 33 );

}
*/
/*		//16.09.20

//EventIDAdr: �¼������ʶ��
//Type = 0:��������е��¼����㣬�¼������¼Ҳ�����1���¼����壬�¼������¼������� 
short AllEventClr( unsigned char* EventIDAdr, short Type )
{
	unsigned long ComID;
	unsigned char Buff[16];
	unsigned char* Ptr;
//	short i;
	short Temp;
	long Value;
		
	Ptr = Buff;
		
	if( Type == 0 ) ComID = 0xffffffff; 			//�������ʱ���¼�ȫ��
	else
	{
		RAM_Write( (unsigned char*)&ComID, EventIDAdr, 4 );
		if( *EventIDAdr != 0xff ) return 1;
	}	 
	if( ComID == 0xffffffff )
	{
//		Clr_E2PData( PhAFail_Pt, 2, 45 );
		Clr_E2PData( PhAFail_Pt, 2, 47 );
//		if( Type == 0 ) Clr_E2PData( MDClr_Pt, 2, 13 );
		if( Type == 0 ) Clr_E2PData( MDClr_Pt, 2, 17 );
		else
		{
			Clr_E2PData( MDClr_Pt, 2, 1 );
//			Clr_E2PData( TimeSet_Pt, 2, 11 );
			Clr_E2PData( TimeSet_Pt, 2, 15 );
		}	

//		Clr_E2PData( TideSwitch_Pt, 2, 14 );						//�¹���
		Clr_E2PData( TideSwitch_Pt, 2, 4 );							//�¹���	//13.10.16
		Clr_E2PData( FeePricePrg_Pt, 2, 8 );						//�¹���	//13.10.16

//		Clr_E2PData( PhAFail_Items, 1, 45 );
		Clr_E2PData( PhAFail_Items, 1, 47 );
//		if( Type == 0 ) Clr_E2PData( MDClr_Items, 1, 13 );
		if( Type == 0 ) Clr_E2PData( MDClr_Items, 1, 17 );
		else
		{
			Clr_E2PData( MDClr_Items, 1, 1 );
//			Clr_E2PData( TimeSet_Items, 1, 11 );
			Clr_E2PData( TimeSet_Items, 1, 15 );
		}	

//		Clr_E2PData( TideSwitch_Items, 1, 14 );						//�¹���
		Clr_E2PData( TideSwitch_Items, 1, 4 );						//�¹���	//13.10.16
		Clr_E2PData( FeePricePrg_Items, 1, 8 );						//�¹���	//13.10.16

		Temp = 0;
		ClrEventCuntTime( Type );
		RAM_Fill( (unsigned char*)&EVar.PhAFailVar.Cunt, sizeof(EVENT_STATE)*TamperNum );	//��������¼���������״̬��	
//		RAM_Fill( (unsigned char*)&EVar.MD_PpOverState, 6 );		//���������������״̬��	
		RAM_Fill( (unsigned char*)&EVar.MDOverPpVar.Cunt, 4*6 );	//�¹���	//13.10.23	
		EVar.AuxPWFail_State = 0x00;								//������Դ������ʼ״̬Ϊ�ϵ�

//		Clr_E2PData( PhAFail_OldState, 1, 29 );
//		Clr_E2PData( PhAFail_OldState, 1, 41 );
		Clr_E2PData( PhAFail_OldState, 1, 42 );

//		Clr_E2PData( TideSwitch_OldState, 1, 4 );					//�¹���
		Clr_E2PData( TideSwitch_OldState, 1, 7 );					//�¹���	//13.10.16

		Clr_E2PData( OpenCover_OldState, 1, 2 );					//10.11.05

		Clr_E2PData( EMaxVoltImbalanceRatio, 3, 3 );				//15.05.27
		RAM_Fill( Real.MaxVoltImbalanceRatio, 3 );					//15.05.27	
		RAM_Fill( Real.MaxImbalanceRatio, 3 );						//15.05.27
		RAM_Fill( Real.MaxBestImbalanceRatio, 3 );					//15.05.27		

//		for( i=0;i<4;i++ )											//�����б��汾�µ�ѹ�ϸ��ʣ����ٱ�����EEPROM��
//		{
//			Clr_E2PData(  VoltCheckPara[i].CurMonAddr, 27, 1 );		//���������EEPROM�еĵ�ѹ�ϸ�������		
//		}
		InitAllQualifiyRateData();									//���������RAM�еĵ�ѹ�ϸ�������
//		RAM_Fill((unsigned char*)&SM.CoverCnt, 6 );					//������ǿ���ť������
		RAM_Fill((unsigned char*)&SM.CoverState, 2 );				//�������״̬��
		RAM_Fill((unsigned char*)&SM.TerminalState, 2 );			//�������״̬��
		RAM_Fill( (unsigned char*)&EVar.RevPowerZVar.Cunt, 3 );		//����¼���������״̬��
		RAM_Fill( (unsigned char*)&EVar.AllFailVar.Cunt, 3 );		//����¼���������״̬��
		RAM_Fill( (unsigned char*)&EVar.PdownVar.Cunt, 3 );		//����¼���������״̬��
		
		Flag.PrgState = 0;											//10.07.01 
		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		RAM_Fill( Ptr, 12 );				//�¹���	//14.04.26
//		*Ptr |= 0xFE;						//�¹���	//14.04.26	//14.06.21
//		*(Ptr+1) |= 0x03;					//�¹���	//14.04.26	//14.06.21
		*(Ptr+10) |= 0x0A;					//�¹���	//14.04.26	//�¼�����͵�������־�����
		CommResetEventUpState( Ptr );		//�¹���	//14.04.26
		ResetEventUpStateSend( Ptr );		//�¹���	//14.06.21

//		Flag.RSwitchOldState = 0;  															//�¹���	//14.05.03	
		Flag.RSwitchOldState &= 0x03;		//��Ӱ������բ�ɹ��ϱ�							//�¹���	//14.06.26		
		Flag.RSwitchOldState |= 0x40;		//����̵����쳣�����ϱ�						//�¹���	//14.06.26
		E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
#else
		Flag.EventUpState[1] &= ~0x0C;								//�¹���	//13.10.16
		RAM_Fill((unsigned char*)&Flag.EventUpState[2], 8 );		//�¹���	//13.10.16
#endif
	}
	else
	{
		Temp = SingleEventClr( EventIDAdr, EventClr_No );  //�����¼����㣬�¼������¼���������
//		if( Temp == 1 )	B1SingleEventClr( EventIDAdr, EventClr_No );  //�����ļ�������¼��������¼�����	
		if( Temp == 1 )	Temp = B1SingleEventClr( EventIDAdr, EventClr_No );  //�����ļ�������¼��������¼�����		//10.06.02
		if( Temp != 0 ) Temp = 1;				//�쳣������01������		//10.06.02
	}
	if(( Type != 0 )&&( Temp == 0 ))			//�¼����㣬������¼
	{
//        IncEventCunt( EventClr_No );    
		Value = PtAddrChk( EventClr_No, Next );
		if( Value == -1 ) return 0;
        IncEventCunt( EventClr_No );    
		RAM_Write( Ptr, Clk.Sec, 6 );
		RAM_Write( Ptr+6, Comm.OperateCode, 4 );
		*EventIDAdr = 0xff;
        RAM_Write( Ptr+10, EventIDAdr, 4 );
		E2P_WData( (unsigned short)Value, Ptr, 14 );
	}	
	return Temp;						
}	

void ClrEventCuntTime( short Type )
{
	unsigned char Buff[100];
	unsigned char* Ptr;
	
	Ptr = Buff;
	RAM_Fill( Ptr, 100 );
	
	SM.AllFailSumTime = 0;
	SM.AllFailSumTimeChk = ChkNum( (unsigned char*)&SM.AllFailSumTime, 4 );				//10.11.05
	E2P_WData( PhFailCuntTime, Ptr, sizeof(PHASE_EVENT_TIMES)*4 );
//	E2P_WData( AllFailCuntTime, Ptr, sizeof(EVENT_TIMES)*6 );
	E2P_WData( AllFailCuntTime, Ptr, sizeof(EVENT_TIMES)*8 );
	E2P_WData( PftAOverCuntTime, Ptr, sizeof(EVENT_TIMES)*5 );							//�¹���
	E2P_WData( CuFailCuntTime, Ptr, sizeof(PHASE_EVENT_TIMES)*5 );
//	E2P_WData( PowerDown_Cunt, Ptr, 24 );
	E2P_WData( PowerDown_Cunt, Ptr, 8*3 );
//	if( Type == 0 ) E2P_WData( MDClr_Cunt, Ptr, 13*3 );
//	if( Type == 0 ) E2P_WData( MDClr_Cunt, Ptr, 15*3 );
	if( Type == 0 ) E2P_WData( MDClr_Cunt, Ptr, 16*3 );		//���ӷǷ��忨����
	else
	{
		E2P_WData( MDClr_Cunt, Ptr, 3 );		
//		E2P_WData( TimeSet_Cunt, Ptr, 11*3 );
//		E2P_WData( TimeSet_Cunt, Ptr, 13*3 );
		E2P_WData( TimeSet_Cunt, Ptr, 14*3 );				//���ӷǷ��忨����
	}	

//	E2P_WData( PhFailCuntZ, Ptr, 66 );			
	E2P_WData( PhFailCuntZ, Ptr, 6*3 );			

	E2P_WData( TideSwitch_Cunt, Ptr, 7*3 );											//�¹���	
	
}	


//����ֵ0���Ѵ���ǰ���ݱ�ʶ
//����ֵ1��δ�ҵ���ǰ���ݱ�ʶ
//����ֵ2����ǰ���ݱ�ʶ�Ƿ�
short SingleEventClr( unsigned char* Source, unsigned short EventClrNo )
{
	unsigned short 	Addr, Info, Len;
	unsigned long ComID;
	unsigned short EventNo;
//	unsigned short Addr;
	short i;
	unsigned char Buff[4];
	unsigned char* Ptr;
	short ItemAds;
	
	Ptr = Buff;	
//	if( *Source != 0xff ) return 1;
	if( *Source != 0xff ) return 2;
	RAM_Write( (unsigned char*)&ComID, Source, 4 );

	if(( ComID == 0x00FE00FF )||( ComID == 0x000D00FF ))//�¹���	//13.10.16	
	{
		return 2;										//����ʱ���¼�������¼������		
	}	

	if( ComID == 0x0310FFFF )							//�¹���	//13.10.16	//��ѹ�ϸ��ʼ�¼�ܺ�����һ����
	{
		RAM_Write( Ptr, Source, 4 );					
		for( i=0;i<4;i++ )								//���ܺͷ����¼�
		{
			*(Ptr+1) = i;
			SingleEventClr( Ptr, EventClrNo );
		}	
		return 0;		
	}	

	if( ComID == 0x0312FFFF )							//�¹���	//13.10.16	//�������޼�¼һ����
	{
		RAM_Write( Ptr, Source, 4 );					
		for( i=0;i<7;i++ )								//���ܺͷ����¼�
		{
			*(Ptr+1) = i;
			SingleEventClr( Ptr, EventClrNo );
		}
		ClearEventUpState( 70 );						//�¹���	//13.10.16	
		return 0;		
	}	

//	if(( ComID == 0x030100FF )||( ComID == 0x030200FF )||( ComID == 0x030300FF )||( ComID == 0x030400FF )
//		||( ComID == 0x030B00FF )||( ComID == 0x030C00FF )||( ComID == 0x030D00FF )||( ComID == 0x030E00FF )
//		||( ComID == 0x030F00FF )||( ComID == 0x031200FF ))
	if( ComID == 0x031200FF )
	{
		RAM_Write( Ptr, Source, 4 );
		*Ptr = 0;
		ItemAds = NGetEveComVar( &Addr, &Info, &Len, Ptr );
//		if( ItemAds == -1 ) return 1;
		if( ItemAds == -1 ) return 2;
		if(( Info & EVComm_Type ) == EVPARAM ) { Clr_E2PData( Addr, Len, 1 ); return 0; }
//		else return 1;	
		else return 2;	
	}		

	RAM_Write( Ptr, Source, 4 );
	*Ptr = 0x01;
	RAM_Write( (unsigned char*)&ComID, Ptr, 4 );
	
	for( i=0;i<EveComNum;i++ )
	{
		if( ComID == EveComTab[i].ComID ) 
		{
			if(( EveComTab[i].ComInfo & EVComm_Type ) == EVENO )
			{
              EventNo = EveComTab[i].Addr;
              break;
            } 
//			else return 1;	
			else return 2;	
		}
  	}
	if( i == EveComNum ) return 1;		

	RAM_Fill( Ptr, 4 );
//	if( EventNo == ECClr_No ) return 1;			//��������¼���������
	if( EventNo == ECClr_No ) return 2;			//��������¼���������
//	if( EventNo == EventClr_No )	return 1;	//�¼������¼���������
	if( EventNo == EventClr_No )	return 2;	//�¼������¼���������
	Addr = PointTab[EventNo].PtAddr;
	E2P_WData( Addr, Ptr, 2 );					//�¼���¼ָ������				
	Addr = PointTab[EventNo].StorageRecs;
	E2P_WData( Addr, Ptr, 1 );					//�¼���¼������	
//	if(( EventNo <= PhCBreak_No )||(( EventNo >= CuAFail_No)&&( EventNo <= PowerOverC_No ))			
//		||(( EventNo >= MDOverPp_No )&&( EventNo <= MDOverQ4_No )))
	if(( EventNo <= PhCBreak_No )||(( EventNo >= CuAFail_No)&&( EventNo <= PowerOverC_No )))	//�¹���	//13.10.23			
	{
		;		//�����¼��Ĵ������ۼ�ʱ�䵥����
	}	
	else	
	{	
		Addr = PointTab[EventNo].Cunt;	
		if( Addr != 0xFFFF ) E2P_WData( Addr, Ptr, 3 );		//�¼��ۼƴ�������				
		Addr = PointTab[EventNo].SumTime;	
		if( Addr != 0xFFFF ) E2P_WData( Addr, Ptr, 3 );		//�¼��ۼ�ʱ������		
	}
	
//	for( i=0;i<TamperNum;i++ )
//	{
//		if( EventNo == TamperTab[i].EventNo ) break;
// 	}
//	if( i != TamperNum ) 
//	{
//		RAM_Fill( (unsigned char*)TamperTab[i].Event_State, 3 );	//����¼���������״̬��
//		E2P_WData( TamperTab[i].EOldState, Ptr, 1 );	
//	}		

//																	//�¹���	//13.10.23	
//	for( i=MDOverPp_No;i<=MDOverQ4_No;i++ )
//	{
//		if( EventNo == i )
//		{
//			E2P_WData( MDOverPp_OldState+i-MDOverPp_No, Ptr, 1 );	
//			Ptr = &EVar.MD_PpOverState;
//			*(Ptr+i-MDOverPp_No) = 0;			
//		}	
//	}	
	
	for( i=0;i<MDOverTamperNum;i++ )									//�¹���	//13.10.23	
	{
		if( EventNo == MDOverTamperTab[i].EventNo ) break;
  	}
	if( i != MDOverTamperNum ) 											//�¹���	//13.10.23	
	{
		*Ptr = 0;
		RAM_Fill( (unsigned char*)MDOverTamperTab[i].Event_State, 3 );	//����¼���������״̬��
		E2P_WData( MDOverTamperTab[i].EOldState, Ptr, 1 );
		ClearEventUpState( MDOverTamperTab[i].UpBitSite );				//�¹���	//13.10.16	
	}		
	
	
	if(( EventNo >= VoltCheck_No )&&( EventNo <= VoltCCheck_No ))
	{		
		InitQualifiyRateData( EventNo - VoltCheck_No );	
	}	

//	if( EventNo == OpenCover_No ) RAM_Fill((unsigned char*)&SM.CoverCnt, 3 );			//���Ǽ�����
//	if( EventNo == OpenCover_No ) RAM_Fill((unsigned char*)&SM.CoverState, 2 );			//���Ǽ�����
	if( EventNo == OpenCover_No ) 
	{
		RAM_Fill((unsigned char*)&SM.CoverState, 2 );			//���Ǽ�����
		*Ptr = 0;												//10.11.05			
		E2P_WData( OpenCover_OldState, Ptr, 1 );				//10.11.05	
		ClearEventUpState( 10 );								//�¹���	//13.10.16	
	}	

//	if( EventNo == OpenTerminal_No ) RAM_Fill((unsigned char*)&SM.TerminalCnt, 3 );		//���˼�����
//	if( EventNo == OpenTerminal_No ) RAM_Fill((unsigned char*)&SM.TerminalState, 2 );		//���˼�����
	if( EventNo == OpenTerminal_No ) 
	{
		RAM_Fill((unsigned char*)&SM.TerminalState, 2 );		//���˼�����
		*Ptr = 0;												//10.11.05
		E2P_WData( OpenTerminal_OldState, Ptr, 1 );				//10.11.05	
		ClearEventUpState( 11 );								//�¹���	//13.10.16	
	}	

	*Ptr = 0;													//�¹���	//13.10.16	
	
	if( EventNo == AllFail_No ) 
	{
		RAM_Fill( (unsigned char*)&EVar.AllFailVar.Cunt, 3 );	//����¼���������״̬��
		E2P_WData( AllFail_OldState, Ptr, 1 );
		Flag.Power &= ~F_AllFail;
		SM.AllFailSumTime = 0;		
		SM.AllFailSumTimeChk = ChkNum( (unsigned char*)&SM.AllFailSumTime, 4 );				//10.11.05
		ClearEventUpState( 74 );								//�¹���	//13.10.16	
	}	

	if( EventNo == PdownRec_No ) 
	{
		E2P_WData( Pdown_OldState, Ptr, 1 );
		RAM_Fill( (unsigned char*)&EVar.PdownVar.Cunt, 3 );		//����¼���������״̬��
		ClearEventUpState( 69 );								//�¹���	//13.10.16	
	}	

	if( EventNo == TideSwitch_No ) 								//�¹���	//13.10.16		//��������
	{
		RAM_Fill( (unsigned char*)&EVar.TideSwitchVar.Cunt, 3 );//����¼���������״̬��
		E2P_WData( TideSwitch_OldState, Ptr, 1 );
		ClearEventUpState( 73 );								//�¹���	//13.10.16	
	}	

	if( EventNo == MagnetDisturb_No ) 							//�¹���	//13.10.16		//�㶨�ų�����
	{
		RAM_Fill( (unsigned char*)&EVar.MagnetDisturbVar.Cunt, 3 );	//����¼���������״̬��
		E2P_WData( MagnetDisturb_OldState, Ptr, 1 );
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		ClearEventUpState( 12 );								//�¹���	//14.04.26
#endif
	}	

	if( EventNo == RelayFault_No ) 								//�¹���	//13.10.16		//���ɿ�������
	{
		RAM_Fill( (unsigned char*)&EVar.RelayFaultVar.Cunt, 3 );//����¼���������״̬��
		E2P_WData( RelayFault_OldState, Ptr, 1 );
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		ClearEventUpState( 0 );									//�¹���	//14.04.26
		Flag.RSwitchOldState &= 0x03;														//�¹���	//14.05.03	
		Flag.RSwitchOldState |= 0x40;		//����̵����쳣�����ϱ�						//�¹���	//14.06.26
		E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
#endif
	}	

	if( EventNo == PowerFault_No ) 								//�¹���	//13.10.16		
	{
		E2P_WData( PowerFault_OldState, Ptr, 1 );
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		ClearEventUpState( 13 );								//�¹���	//14.04.26
#endif
	}	

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	if( EventNo == PrgRec_No ) 
	{
		Flag.PrgState = 0;			//10.07.01 
		ClearEventUpState( 48 );								//�¹���	//14.04.26
	}	
#else
	if( EventNo == PrgRec_No ) Flag.PrgState = 0;			//10.07.01 
#endif

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
#if( AuxPWFailCheck	== YesCheck )
	if( EventNo == AuxPWFail_No ) ClearEventUpState( 36 );			//�¹���	//14.04.26
#endif
	if( EventNo == TimeSet_No ) ClearEventUpState( 52 );			//�¹���	//14.04.26
	if( EventNo == SegTabPrg_No ) ClearEventUpState( 53 );			//�¹���	//14.04.26
	if( EventNo == ZonePrg_No ) ClearEventUpState( 54 );			//�¹���	//14.04.26
	if( EventNo == WeekCDPrg_No ) ClearEventUpState( 55 );			//�¹���	//14.04.26
	if( EventNo == HolidayPrg_No ) ClearEventUpState( 56 );			//�¹���	//14.04.26
	if( EventNo == ECPModePrg_No ) ClearEventUpState( 57 );			//�¹���	//14.04.26
	if( EventNo == ECQ1ModePrg_No ) ClearEventUpState( 58 );		//�¹���	//14.04.26
	if( EventNo == ECQ2ModePrg_No ) ClearEventUpState( 59 );		//�¹���	//14.04.26
	if( EventNo == SaveDayPrg_No ) ClearEventUpState( 60 );			//�¹���	//14.04.26
	if( EventNo == SecretKeyPrg_No ) ClearEventUpState( 63 );		//�¹���	//14.04.26
#endif
	
	return 0;
}	

short B1SingleEventClr( unsigned char* Source, unsigned short EventClrNo )
{
	unsigned short 	Addr;   //, Info, Len;
	unsigned long ComID;
	unsigned short EventNo;
//	unsigned short Addr;
	short i;
	unsigned char Buff[4];
	unsigned char* Ptr;
//	short ItemAds;
	
	Ptr = Buff;	
	if( *Source != 0xff ) return 1;				//�¹���	//13.10.14
//	if(( *Source != 0xff )||(( *(Source+1) != 0xff )&&( *(Source+1) != 0x00 )&&( *(Source+1) != 0x01 ))) return 1;			//10.06.02		//�¹���	//13.10.14
	RAM_Write( (unsigned char*)&ComID, Source, 4 );

//	if( ComID == 0x100001FF ) Clr_E2PData( PhFailCuntZ, 3, 6 );					//ʧѹ�ܴ������ۼ�ʱ�估��������ʱ��
	if(( ComID == 0x100000FF )||( ComID == 0x100001FF )||( ComID == 0x1000FFFF ))	//ʧѹ�ܴ������ۼ�ʱ�估��������ʱ��	//10.06.02
	{
		Clr_E2PData( PhFailCuntZ, 3, 6 ); 
		return 0; 
	}	

	if(( *(Source+1) == 0xff )&&( *(Source+2) == 0xff )	//�¹���	//13.10.16	//ʧѹ��Ƿѹ����ѹ�����ࡢʧ�������������������ʷ��򡢹��ؼ�¼����һ����
		&&((( *(Source+3) >= 0x10 )&&( *(Source+3) <= 0x13 ))||(( *(Source+3) >= 0x18 )&&( *(Source+3) <= 0x1C ))))	//�������¼�
	{
		RAM_Write( Ptr, Source, 4 );					
		*(Ptr+2) = 0x01;
		B1SingleEventClr( Ptr, EventClrNo );			//��A���¼�		
		*(Ptr+2) = 0x02;
		B1SingleEventClr( Ptr, EventClrNo );			//��B���¼�		
		*(Ptr+2) = 0x03;
		B1SingleEventClr( Ptr, EventClrNo );			//��C���¼�		
		if( *(Source+3) == 0x10 ) Clr_E2PData( PhFailCuntZ, 3, 6 ); 
		return 0;		
	}	

	if(( *(Source+1) == 0xff )&&( *(Source+2) == 0xff )&&( *(Source+3) == 0x1F ))		//�¹���	//13.10.16	//�������������ܺ�����һ����
	{
		RAM_Write( Ptr, Source, 4 );					
		for( i=0;i<4;i++ )
		{
			*(Ptr+2) = i;
			B1SingleEventClr( Ptr, EventClrNo );		
		}	
		return 0;		
	}	

	RAM_Write( Ptr, Source, 4 );
	*Ptr = 0x01;
	*(Ptr+1) = 0x01;										//10.06.02
	RAM_Write( (unsigned char*)&ComID, Ptr, 4 );
	
	for( i=0;i<B1EveComNum;i++ )
	{
		if( ComID == B1EveComTab[i].ComID ) 
		{
			if(( B1EveComTab[i].ComInfo & EVComm_Type ) == EVENO )
			{
              EventNo = B1EveComTab[i].Addr;
              break;
            } 
			else return 1;	
		}
  	}
	if( i == B1EveComNum ) return 1;		

	if( *(Source+1) != 0xff )					//�¹���	//13.10.14
	{											//�¹���	//13.10.14
		*Ptr = PointTab[EventNo].Items;			//�¹���	//13.10.14
		if( *(Source+1) > *Ptr ) return 1;		//�¹���	//13.10.14
	}											//�¹���	//13.10.14

	RAM_Fill( Ptr, 4 );
	Addr = PointTab[EventNo].PtAddr;
	E2P_WData( Addr, Ptr, 2 );					//�¼���¼ָ������				
	Addr = PointTab[EventNo].StorageRecs;
	E2P_WData( Addr, Ptr, 1 );					//�¼���¼������	

	Addr = PointTab[EventNo].Cunt;	
	if( Addr != 0xFFFF ) E2P_WData( Addr, Ptr, 3 );		//�¼��ۼƴ�������				
	Addr = PointTab[EventNo].SumTime;	
	if( Addr != 0xFFFF ) E2P_WData( Addr, Ptr, 3 );		//�¼��ۼ�ʱ������		
	
	for( i=0;i<TamperNum;i++ )
	{
		if( EventNo == TamperTab[i].EventNo ) break;
  	}
	if( i != TamperNum ) 
	{
		RAM_Fill( (unsigned char*)TamperTab[i].Event_State, 3 );	//����¼���������״̬��
		E2P_WData( TamperTab[i].EOldState, Ptr, 1 );
		ClearEventUpState( TamperTab[i].UpBitSite );				//�¹���	//13.10.16	
	}		

	if( EventNo == VoltImbalance_No ) 											//15.05.27	
	{
		RAM_Fill( Real.MaxVoltImbalanceRatio, 3 );
		E2P_WData( EMaxVoltImbalanceRatio, Real.MaxVoltImbalanceRatio, 3 );		//15.05.27
	}	
	if( EventNo == CuImbalance_No ) 											//15.05.27
	{
		RAM_Fill( Real.MaxImbalanceRatio, 3 );		
		E2P_WData( EMaxImbalanceRatio, Real.MaxImbalanceRatio, 3 );				//15.05.27
	}	
	if( EventNo == CuBestImbalance_No ) 										//15.05.27
	{																		
		RAM_Fill( Real.MaxBestImbalanceRatio, 3 );							
		E2P_WData( EMaxBestImbalanceRatio, Real.MaxBestImbalanceRatio, 3 );		//15.05.27
	}																		

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	if( EventNo == TripOFF_No ) 
	{
//											//�¹���	//14.06.26				
//		Flag.RSwitchOldState &= 0x06;  														//�¹���	//14.05.03	
//		E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
		ClearEventUpState( 14 );			//�¹���	//14.04.26
	}	
	if( EventNo == TripON_No ) 
	{
//											//�¹���	//14.06.26				
//		Flag.RSwitchOldState &= 0x05;  														//�¹���	//14.05.03	
//		E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//�¹���	//14.05.03	
		ClearEventUpState( 15 );				//�¹���	//14.04.26
	}	
#endif

	return 0;
}	

void ClrMeterClrRec( void )			//�����������¼
{
	unsigned char Buff[3];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	RAM_Fill( Ptr, 3 );
	E2P_WData( ECClr_Pt, Ptr, 2 );		//�¼���¼ָ������				
	E2P_WData( ECClr_Items, Ptr, 1 );	//�¼���¼������				
	E2P_WData( ECClr_Cunt, Ptr, 3 );	//�¼��ۼƴ�������					
}	

void AllDJClr( void )
{
//	Clr_E2PData( AppointFreeze_Pt, 2, 6 );
	Clr_E2PData( AppointFreeze_Pt, 2, 8 );
//	Clr_E2PData( AppointFreeze_Items, 1, 6 );
	Clr_E2PData( AppointFreeze_Items, 1, 8 );
	
}	
*/

void AllECClr( void )
{
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned short Length;
	short i;
    
	Point = Buff;
	Ptr = EC.RCMon_EC_Pp0;
	
	RAM_Fill( Point, ECDIGIT );
	*(Point+ECDIGIT) = 0xA5;
//	Length = (FRCMon_EC_FeC0/(ECDIGIT+1))+1;				//RAM��FRAM�е�����һ��	
	Length = FRCMon_EC_FeC0;
    Length /= (ECDIGIT+1);
    Length += 1;
	for( i=0;i<Length;i++ )
	{
		RAM_Write( Ptr+i*(ECDIGIT+1), Point, (ECDIGIT+1) );
	}	
	
	RAM_Fill( (unsigned char*)&ECP.PL_CumPp, ECUnitNum*2 );
	RAM_DataFill( (unsigned char*)&ECP.PL_ChkPp, ECUnitNum, 0xA5);

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
	RAM_Fill( (unsigned char*)&FeeECP.PL_CumPp1, 24*2 );
	RAM_DataFill( (unsigned char*)&FeeECP.PL_ChkPp1, 24, 0xA5);
#endif
	
#if ( MEASCHIP == ADE7878 )	
 	ECInt.EcPSumA = 0;				//A���й������ۼƻ���
	ECInt.EcPSumB = 0;				//B���й������ۼƻ���
	ECInt.EcPSumC = 0;				//C���й������ۼƻ���
	ECInt.EcQSumA = 0;				//A���޹������ۼƻ���
	ECInt.EcQSumB = 0;				//B���޹������ۼƻ���
	ECInt.EcQSumC = 0;				//C���޹������ۼƻ���
	
	RAM_DataFill( (unsigned char*)&ECInt.EcPSumAChk, 4, 0xA5);	
#endif


	ECInt.SMEcSumA = 0;
	ECInt.SMEcSumB = 0;
	ECInt.SMEcSumC = 0;
	ECInt.SMEcSum = 0;
	
	ECInt.YPulse_Cum = 0;			//�ж��й����������
	ECInt.WPulse_Cum = 0;			//�ж��޹����������
	RAM_DataFill( (unsigned char*)&ECInt.SMEcSumAChk, 4, 0xA5);
	ECInt.YPulse_CumChk = 0xA5;
	ECInt.WPulse_CumChk = 0xA5;
	
	Clr_FRAMData( FRCMon_EC_Pp0, ECDIGIT, Length );
//	Clr_E2PData( LastEC_Pt, 2, 1 );
//	Clr_E2PData( LastEC_Items, 1, 1 );

//	Clr_E2PData( YearSaveTime_Pt, 2, 2 );					//�¹���	//13.10.16	
//	Clr_E2PData( YearSaveTime_Items, 1, 2 );				//�¹���	//13.10.16		

	Clr_E2PData( CMon_EC_Pp0, ECDIGIT, 50 );				//10.04.20
	Clr_E2PData( CMon_EC_PpA0, ECDIGIT, 18 );
	Clr_E2PData( CMon_EC_PpB0, ECDIGIT, 18 );
	Clr_E2PData( CMon_EC_PpC0, ECDIGIT, 18 );
}

/*		//16.09.20
void AllMDClr( void )
{
//	Clr_E2PData( AppointFreeze_Pt, 2, 4 );
	Clr_E2PData( LastMD_Items, 1, 1 );
	Clr_E2PData( LastMD_Pt, 2, 1 );
	
}
*/
//////////////////////////////////////////////////////////////////////////
/*
void StepDataSwitch( void )			//���������л�				//�¹���
{
	unsigned char Buff[6];
	unsigned char* Ptr; 
	short i;
	
	Ptr = Buff;

	for( i=0;i<13;i++ )
	{
		E2P_WData( Case1Step1EC+i*5, Ptr, 4 );					
	}	
	for( i=0;i<4;i++ )
	{
		E2P_RData( Ptr, Case1YearSaveDay1+i*4, 3 );					
		E2P_WData( Case2YearSaveDay1+i*4, Ptr, 3 );					
	}	
}	
*/
#if ( LocalSmartMeter == YesCheck )
void FeeDataSwitch( void )			//���������л�			//�¹���	//13.11.22
{
	unsigned char Buff[6];
	unsigned char* Ptr; 
	int i;
	
	Ptr = Buff;

	for( i=0;i<32;i++ )
	{
		E2P_RData( Ptr, Case2Fee1Price+i*5, 4 );					
		E2P_WData( Case1Fee1Price+i*5, Ptr, 4 );					
	}	
}	

void GetFeePriceNum( void )									//�¹���	//13.11.30	
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned int i;		
	
	Ptr = Buff;
	
	for( i=0;i<4;i++ )
	{
		E2P_RData( Ptr+i*4, Case1Fee1Price+i*5, 4 );			
	}	
	
	if( IsAllData( Ptr, 16, 0x00 ) == 0 ) ICComm.RFeePriceNum = 0;
	else ICComm.RFeePriceNum = 0x04;
}
												
void FeeTabSwitch( void )									//�¹���	//13.11.22	
{
//	unsigned char Buff[12];
	unsigned char Buff[20];
	unsigned char* Point;
	int FeeTabNo;
	
	Point = Buff;
	
	if((Flag.Power & F_PwrUp) != 0)								//���粻���л�	//15.05.13
	{                                                           //���粻���л�	//15.05.13         
//		E2P_RData( Point,FeeSwitchTime, 5 );
		E2P_RData( Point+7,FeeSwitchTime, 7 );
		ChangeTimeHexToBCD( Point, Point+7 );
		RAM_Write( Point, Point+1, 5 );
		
		E2P_RData( Point+5,OldFeeSwitchTime, 5 );
		SM.FeeTabNo = 0;										//�ޱ����״��л�	//�¹���	//13.11.30
		if((IsAllData( Point, 5, 0x00 ) != 0)&&( Data_Comp( Point+5, Point, 5 ) != 0 ))		//�л�ʱ��Ϊ0�����л�
		{
			FeeTabNo = Data_Comp( Clk.Min, Point, 5 );
			if( FeeTabNo >= 0 )			//ʱ���л�
			{
				E2P_WData( OldFeeSwitchTime, Point, 5 );		//������һ���л�����	
				FeeDataSwitch();								//���������л�			//�¹���	//13.11.22
//				CreateFreezeRec( FeeSWFreeze_No );				//ִ�з��ʵ���л�����
				RecordFreeze( FeeSWFreeze_No, DIRECT );			//ִ�з��ʵ���л�����
			}
			else SM.FeeTabNo = 1;								//�б����״��л�	//�¹���	//13.11.30			
		}
	}															//���粻���л�	//15.05.13
	GetFeePriceNum();										//�¹���	//13.11.30
}	

//////////////////////////////////////////////////////////////////////////
void StepDataSwitch( void )			//���������л�			//�¹���	//13.11.29	
{
	unsigned char Buff[6];
	unsigned char Buff2[12];
	unsigned char Buff3[12];
	unsigned char* Ptr;
	unsigned char* OldPtr;	 
	unsigned char* NewPtr;	 
	int i;
	
	Ptr = Buff;
	OldPtr = Buff2;
	NewPtr = Buff3;

	for( i=0;i<13;i++ )
	{
		E2P_RData( Ptr, Case2Step1EC+i*5, 4 );					
		E2P_WData( Case1Step1EC+i*5, Ptr, 4 );					
	}	
	for( i=0;i<4;i++ )
	{
		E2P_RData( OldPtr+i*3, Case1YearSaveDay1+i*4, 3 );		
		E2P_RData( Ptr, Case2YearSaveDay1+i*4, 3 );		
		RAM_Write( NewPtr+i*3, Ptr, 3 );			
		E2P_WData( Case1YearSaveDay1+i*4, Ptr, 3 );					
	}
	YearMonSaveDaySwitchProcess( OldPtr, NewPtr );			
}	

void StepTabSwitch( void )									//�¹���	//13.11.22	
{
//	unsigned char Buff[12];
	unsigned char Buff[20];
	unsigned char* Point;
	int StepTabNo;
	
	Point = Buff;
	
//	E2P_RData( Point,StepSwitchTime, 5 );
	E2P_RData( Point+7,StepSwitchTime, 7 );
	ChangeTimeHexToBCD( Point, Point+7 );
	RAM_Write( Point, Point+1, 5 );

	E2P_RData( Point+5,OldStepSwitchTime, 5 );
	SM.StepTabNo = 0;										//�ޱ����״��л�	//�¹���	//13.11.30
	if((IsAllData( Point, 5, 0x00 ) != 0)&&( Data_Comp( Point+5, Point, 5 ) != 0 ))		//�л�ʱ��Ϊ0�����л�
	{
		StepTabNo = Data_Comp( Clk.Min, Point, 5 );
		if( StepTabNo >= 0 )			//ʱ���л�
		{
			E2P_WData( OldStepSwitchTime, Point, 5 );		//������һ���л�����	
			StepDataSwitch();											//�¹���
//			CreateFreezeRec( StepSWFreeze_No );				//ִ�з��ʵ���л�����
			RecordFreeze( StepSWFreeze_No, DIRECT );		//ִ�н��ݵ���л�����
		}
		else SM.StepTabNo = 1;								//�ޱ����״��л�	//�¹���	//13.11.30			
	}		
}	
#endif

void ZoneDataSwitch( void )			//ʱ�������л�				
{
	unsigned char Buff[50];
	unsigned char* Ptr; 
	
	Ptr = Buff;

	E2P_RData( Ptr, ESeason2, ZONENUMBER*3 );					
//	E2P_WData( ESeason2, Ptr, ZONENUMBER*3 );					
	E2P_WData( ESeason1, Ptr, ZONENUMBER*3 );					//17.03.20
}	

void SegTabDataSwitch( void )			//ʱ�������л�				
{
	unsigned char Buff[50];
	unsigned char* Ptr; 
	short i;
	
	Ptr = Buff;

	for( i=0;i<8;i++ )
	{
		E2P_RData( Ptr, Period2_Tab1+i*(SEGNUMBER*3+1), SEGNUMBER*3 );					
//		E2P_WData( Period2_Tab1+i*(SEGNUMBER*3+1), Ptr, SEGNUMBER*3 );	
		E2P_WData( Period1_Tab1+i*(SEGNUMBER*3+1), Ptr, SEGNUMBER*3 );		//17.03.20
	}				
}	

void SwapZoneSegTab( unsigned char* Data, unsigned int Items  )			//17.03.18
{
	unsigned char Buff[50];
	unsigned char TempBuff[5];
	unsigned char* Ptr;
	int i,j;
	
	Ptr = Buff;
	
	if( Items <= 1 ) return;												//�������£�����Ҫ����		//17.04.18	
	
//	if( Items > SEGNUMBER ) Items = SEGNUMBER;
	RAM_Write( Ptr, Data, Items*3 );
	for (j=0;j<(Items-1);j++)         									//��С��������
	{
		for(i=0;i<(Items-1-j);i++)         
		{             
			if( Data_Comp( Ptr+i*3+1, Ptr+(i+1)*3+1, 2) > 0 )			
			{                 
				RAM_Write( TempBuff, Ptr+i*3, 3 );
				RAM_Write( Ptr+i*3, Ptr+(i+1)*3, 3 );
				RAM_Write( Ptr+(i+1)*3, TempBuff, 3 );
			}   
		}
	}
	j = 0;	 
	for( i=0;i<(Items-2); i++ )											//��������������еĺ�һ���ú������ݸ��ǣ����Ŀհ��������������
	{
		if( Data_Comp( Ptr+j*3, Ptr+(j+1)*3, 3) == 0 )
		{
			RAM_Write( Ptr+(j+1)*3, Ptr+(j+2)*3, (Items-j-2)*3 );	
		}				
		else j++;
	}	         
	RAM_Write( Data, Ptr, Items*3 );
}	

short GetDaySeg(void)
{
	unsigned char Buff[50];
	unsigned char* Point;
	unsigned char Temp;
//	short i,ZoneNo,SegTabNo,StepTabNo;
	short i,ZoneNo,SegTabNo;					//�¹���	//13.12.16
	unsigned short Sum;
//	unsigned short Addr;                      //�¹���
    
	Point = Buff;

	SM.SegSwitchFlag = 0x00;				//�¹���	//13.10.06	

#if ( LocalSmartMeter == YesCheck )
	if((Flag.Power & F_PwrUp) != 0) FeeTabSwitch();
#endif

	E2P_RData( (unsigned char*)&SM.ZoneNo,EZoneNo, 1 );
	E2P_RData( (unsigned char*)&SM.SegTabNo,ESegTabNo, 1 );
//	E2P_RData( (unsigned char*)&SM.StepTabNo,EStepTabNo, 1 );			//�¹���
	if( SM.ZoneNo != 1 ) SM.ZoneNo = 0;						//V4
	if( SM.SegTabNo != 1 ) SM.SegTabNo = 0;					//V4
//	if( SM.StepTabNo != 1 ) SM.StepTabNo = 0;				//V4		//�¹���
#if ( LocalSmartMeter == YesCheck )
	ICComm.YearMonSaveMode = GetCurYearMonSaveMode( Point );				//ȡ��ǰ���㷽ʽ//�¹���	//13.11.28
	if((Flag.Power & F_PwrUp) != 0) StepTabSwitch();						//���粻���л�	//15.05.13								//�¹���	//13.11.22

	for( i=0;i<6;i++ )												//�¹���
	{
		E2P_RData( Point, Case1Step1EC+i*5, 4 ); 	
		ICEC.ECStep[i] = BCD4_Long( Point );
	}	
	ICEC.ECStepChk = ChkNum( (unsigned char*)ICEC.ECStep, 24 );		//�¹���
#else
	SM.StepTabNo = 0;													//�¹���
#endif

//																		//�¹���	//13.12.16
//	E2P_RData( Point,StepSwitchTime, 5 );
//	if( IsAllData( Point, 5, 0x00 ) != 0 )
//	{
//		StepTabNo = Data_Comp( Clk.Min, Point, 5 );
//		if( StepTabNo >= 0 )			//ʱ���л�
//		{
////			if( SM.StepTabNo != 0 ) SM.StepTabNo = 0;					//�¹���
////			else SM.StepTabNo = 1;										//�¹���
////			E2P_WData( EStepTabNo, (unsigned char*)&SM.StepTabNo,1 );	//�¹���	
//			RAM_Fill( Point, 5 );
//			E2P_WData( StepSwitchTime, Point, 5 );	
//			StepDataSwitch();											//�¹���
//			CreateFreezeRec( StepSWFreeze_No );		//ִ�н��ݵ���л�����
//		}			
//	}		
////	StepTabNo = SM.StepTabNo;											//�¹���

//																		//�¹���
//	E2P_RData( Point, StepSum, 1 ); 
//	Sum = BCD_Byte( *Point );
//	if(( Sum != 0 )&&( Sum <= 10 ))
//	{
//		if( SM.StepTabNo == 0 ) Addr = Case1Step1EC;
//		else Addr = Case2Step1EC;	
//		for( i=0;i<Sum;i++ )
//		{
//			E2P_RData( Point, Addr+i*5, 4 ); 	
//			ICEC.ECStep[i] = BCD4_Long( Point );
//		}	
//		ICEC.ECStepChk = ChkNum( (unsigned char*)ICEC.ECStep, 40 );
//	}	

//																	//�¹���	//13.10.06
//	for( i=0;i<6;i++ )												//�¹���
//	{
//		E2P_RData( Point, Case1Step1EC+i*5, 4 ); 	
//		ICEC.ECStep[i] = BCD4_Long( Point );
//	}	
//	ICEC.ECStepChk = ChkNum( (unsigned char*)ICEC.ECStep, 24 );		//�¹���

		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//15.05.13
	if((Flag.Power & F_PwrUp) != 0) 
	{
#endif
/*
		E2P_RData( Point,ZoneSwitchTime, 5 );
		if( IsAllData( Point, 5, 0x00 ) != 0 )
		{
			ZoneNo = Data_Comp( Clk.Min, Point, 5 );
			if( ZoneNo >= 0 )			//ʱ���л�
			{
				if( SM.ZoneNo != 0 ) SM.ZoneNo = 0;
				else SM.ZoneNo = 1;	
				E2P_WData( EZoneNo, (unsigned char*)&SM.ZoneNo,1 );
				RAM_Fill( Point, 5 );
				E2P_WData( ZoneSwitchTime, Point, 5 );	
				CreateFreezeRec( ZoneSWFreeze_No );		//ִ��ʱ���л�����
			}			
		}		
		ZoneNo = SM.ZoneNo;	
*/

		E2P_RData( Point+7,ZoneSwitchTime, 7 );
		ChangeTimeHexToBCD( Point, Point+7 );
		if( IsAllData( Point+1, 5, 0x00 ) != 0 )
		{
			ZoneNo = Data_Comp( Clk.Min, Point+1, 5 );
			if( ZoneNo >= 0 )			//ʱ���л�
			{
				RAM_Fill( Point, 7 );
				E2P_WData( ZoneSwitchTime, Point, 7 );	
				ZoneDataSwitch();		
				RecordFreeze( ZoneSWFreeze_No, DIRECT );
			}			
		}		
		ZoneNo = 0;	
			
/*
		E2P_RData( Point,SegTabSwitchTime, 5 );
		if( IsAllData( Point, 5, 0x00 ) != 0 )
		{
			SegTabNo = Data_Comp( Clk.Min, Point, 5 );
			if( SegTabNo >= 0 )			//ʱ���л�
			{
				if( SM.SegTabNo != 0 ) SM.SegTabNo = 0;
				else SM.SegTabNo = 1;	
				E2P_WData( ESegTabNo, (unsigned char*)&SM.SegTabNo,1 );
				RAM_Fill( Point, 5 );
				E2P_WData( SegTabSwitchTime, Point, 5 );			
				SegTabDataSwitch();
				CreateFreezeRec( SegTabSWFreeze_No );		//ִ��ʱ���л�����
				SM.SegSwitchFlag = 0x55;				//�¹���	//13.10.06	
			}			
		}		
		SegTabNo = SM.SegTabNo;	
*/		

		E2P_RData( Point+7,SegTabSwitchTime, 7 );
		ChangeTimeHexToBCD( Point, Point+7 );
		if( IsAllData( Point+1, 5, 0x00 ) != 0 )
		{
			ZoneNo = Data_Comp( Clk.Min, Point+1, 5 );
			if( ZoneNo >= 0 )			//ʱ���л�
			{
				RAM_Fill( Point, 7 );
				E2P_WData( SegTabSwitchTime, Point, 7 );	
				SegTabDataSwitch();		
				RecordFreeze( SegTabSWFreeze_No, DIRECT );
			}			
		}		
		SegTabNo = 0;	
		
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//15.05.13
	}
	else												//����״̬�²����Ƿ��л�
	{
//		ZoneNo = SM.ZoneNo;								
		ZoneNo = 0;								
//		SegTabNo = SM.SegTabNo;	
		SegTabNo = 0;	
	}	
#endif		
		
//	E2P_RData( Point,HolidaySum, 2 ); 
//	if((( Flag.BatState & F_E2PCheck ) != 0 )||( AllIsBCD( Point, 2 ) != 0 )) Sum = 0; 		//10.07.30
//	else Sum = BCD2_Word( Point );
	E2P_RData( Point,HolidaySum, 1 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 ) Sum = 0; 		//10.07.30
	else Sum = *Point;
	
//	if( Sum > 254 ) Sum = 0;					//�ڼ������Ƿ�	
	if( Sum > HOLIDAYNUMBER ) Sum = 0;					//�ڼ������Ƿ�

	for( i=0;i<Sum;i++ )
	{
		E2P_RData( Point,Holiday1+i*5, 6 ); 
//		if( Data_Comp( Clk.Day, Point+1, 2) == 0 )
		if( Data_Comp( &Clk.YearH, Point, 4) == 0 )
		{
			Temp = *(Point+5);
//			if(( Temp > 14 )||(Temp < 1)) Temp = 1;
			if(( Temp > 8 )||(Temp < 1)) Temp = 1;			//10.07.10
			GetSeg( Temp, SegTabNo );
			return 0;				
		}				
	}	
	
	E2P_RData( Point,EWeek_State, 1 ); 
	Temp = 1;
	if(( *Point & ( Temp << Clk.Week[0] )) == 0 )
	{
		E2P_RData( Point,EWeek_Code, 1 ); 
//		if(( *Point > 14 )||( *Point < 1 )) *Point = 1;
		if(( *Point > 8 )||( *Point < 1 )) *Point = 1;		//10.07.10
		GetSeg( *Point, SegTabNo );			
		return 0;
	}	
	
//	E2P_RData( Point,ESeason1+ZoneNo*(ZONENUMBER*3+1), ZONENUMBER*3 ); 
	E2P_RData( Point,ESeason1, ZONENUMBER*3 ); 
//	Temp = GetNo( Point, (unsigned char*)&Clk.Day[0], ZONENUMBER );			//�¹���	//13.09.28
	E2P_RData( (unsigned char*)&Temp, ZoneSum, 1 );							//�¹���	//13.09.28
	Temp = BCD_Byte( Temp );												//�¹���	//13.09.28		
	if( Temp > ZONENUMBER ) Temp = ZONENUMBER;								//�¹���	//13.09.28
	SwapZoneSegTab( Point, Temp  );			//17.03.18	
	Temp = GetNo( Point, (unsigned char*)&Clk.Day[0], Temp );				//�¹���	//13.09.28
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	if(( Temp > 8 )||( Temp < 1 )) Temp = 1;		//14.09.03
#endif
	GetSeg( Temp, SegTabNo );				

  return 0;
}

void GetSeg( unsigned char Num, short SegTabNo )
{
	if( Num != SM.OldSegNo ) 					//�¹���	//13.10.06	
	{											//�¹���	//13.10.06		
		SM.SegSwitchFlag = 0x55;				//�¹���	//13.10.06	
		SM.OldSegNo = Num;						//�¹���	//13.10.06	
	}											//�¹���	//13.10.06		

	E2P_RData( (unsigned char*)&Para.Seg[0],(SegTabNo*8+(Num-1))*(SEGNUMBER*3+1) + Period1_Tab1, SEGNUMBER*3 ); 	
	SwapZoneSegTab( (unsigned char*)&Para.Seg[0], SEGNUMBER  );			//17.03.18

}	

void GetFeeNo(void)
{
	unsigned char Temp;
	unsigned char Temp2;		
	
//	Temp = GetNo( (unsigned char*)&Para.Seg[0], (unsigned char*)&Clk.Min[0], SEGNUMBER );	//�¹���	//13.09.28
#if ( LocalSmartMeter == YesCheck )
	SM.StepNo = GetCurrentStepNo( 0 );															//12.12.22
	E2P_RData( (unsigned char*)&Temp, FeeSum, 1 );												//12.12.22
	Para.RFeeSum = BCD_Byte( Temp );						//������							//12.12.22
                                                                                            	
	if(( Para.RFeeSum == 0 )&&( SM.StepNo == 0 ))												//12.12.22
	{																							//12.12.22	
		Temp = 1;																				//12.12.22	//��������������ͬʱΪ0ʱ��Ĭ��ִ�з��ʵ��1		
	}	                                                                                    	//12.12.22
	else                                                                                    	//12.12.22
	{																							//12.12.22			
		E2P_RData( (unsigned char*)&Temp, SegSum, 1 );														//�¹���	//13.09.28
		Temp = BCD_Byte( Temp );																			//�¹���	//13.09.28		
		if( Temp > SEGNUMBER ) Temp = SEGNUMBER;															//�¹���	//13.09.28
		Temp = GetNo( (unsigned char*)&Para.Seg[0], (unsigned char*)&Clk.Min[0], Temp );					//�¹���	//13.09.28
	}	                                                                                        //12.12.22
#else	
    E2P_RData( (unsigned char*)&Temp, SegSum, 1 );											//�¹���	//13.09.28
	Temp = BCD_Byte( Temp );																//�¹���	//13.09.28		
	if( Temp > SEGNUMBER ) Temp = SEGNUMBER;												//�¹���	//13.09.28
	Temp = GetNo( (unsigned char*)&Para.Seg[0], (unsigned char*)&Clk.Min[0], Temp );		//�¹���	//13.09.28
#endif
	
	if(( Temp > 4 )||(Temp < 1)) Temp = 4;					//EC
	Disk.DisFeeNo  = Temp;									//EC
	
	E2P_RData( (unsigned char*)&Temp2, EFeeNo, 1 ); 		//EC
	if(( Temp2 <= 4 )&&(Temp2 >= 1)&&((Flag.BatState & F_E2PCheck) == 0 )) //EC		
	{														//EC		
		SM.FeeNo = Temp2;									//EC
	}														//EC
	if(( SM.FeeNo > 4 )||(SM.FeeNo < 1)) SM.FeeNo = Temp;	//EC
	if((Flag.Power & F_PwrUp) == 0)	return;					//EC
	
//	if( SM.FeeNo != Temp )
	if(( SM.FeeNo != Temp )||( SM.SegSwitchFlag == 0x55 ))	//�¹���	//13.10.06
	{
		MD.MD_CuntT = 0;
#if ( FeeMDIndependence	== YesCheck )		//11.01.28
		MD.MD_PCT = 0;
		MD.FeeMDInitialTime = Para.RMaxT;			//ͷһ�������ڲ���������
#endif
//		if( SM.RPulseOutMode == 5 ) 
		if( SM.RPulseOutMode == 2 ) 
//		if( Para.RComplexOutMode[0] == 5 ) 
		{	
//			PDIR_SECOUT |= P_SECOUT;		
			HT_GPIOD->PTDIR |= GPIOD_SECOUT;	
			SM.PulseWidthCnt = 16;
//			SM.PulseWidthCnt[0] = 16;
		}
//		CreateFreezeRec( FeeSWFreeze_No );					//�����л�����
	}
	SM.FeeNo = Temp;
	if( SM.FeeNo != Temp2 )	E2P_WData( EFeeNo, (unsigned char*)&Temp, 1 ); 	//EC	
	
}	

unsigned char GetNo(unsigned char* Point, unsigned char* Source, short ItemNums)
{
	unsigned char Temp;
	unsigned char Temp2;							//�¹���	//13.10.06		
	short	i,j;
	
	Temp2 = SM.OldSegItemNo;						//�¹���	//14.01.03	
	Temp = *(Point+(ItemNums-1)*3);

//	Temp2 = 0;										//�¹���	//13.10.06
	for( i=ItemNums;i>0;i-- )
	{
		j = (i-1)*3;
		if( Data_Comp( Source, Point+j+1, 2) >= 0 )
		{
			Temp = *(Point+j);	
			Temp2 = ( i - 1 );						//�¹���	//13.10.06			
			break;				
		}	
	}
	
	if( Point == (unsigned char*)&Para.Seg[0] )		//�¹���	//13.10.06	//�Ƿ�ʱ�α�����
	{												//�¹���	//13.10.06		
		if( Temp2 != SM.OldSegItemNo )				//�¹���	//13.10.06	
		{											//�¹���	//13.10.06		
			SM.SegSwitchFlag = 0x55;				//�¹���	//13.10.06	
			SM.OldSegItemNo = Temp2;				//�¹���	//13.10.06	
		}											//�¹���	//13.10.06		
	}												//�¹���	//13.10.06			
	
	return Temp;
}

void SetConstPara(void)		
{
//	unsigned char Buff[4];
//	unsigned char* Point;
//	unsigned short Temp;
//	unsigned short i;
//	unsigned char Buff2[34];											//10.06.03
//    unsigned char* Ptr;
	
//	Ptr = Buff2;														//10.06.03	
//	Point = Buff;
	
//													//�¹���		//13.08.30		
//#if ( METERTYPE == U220_I20A__80A320 )
//	*Point = 0x20;
//	*(Point+1) = 0x03;
//	*(Point+2) = 0x00;
//#elif(( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//	*Point = 0x40;
//	*(Point+1) = 0x02;
//	*(Point+2) = 0x00;
//#else
//	*Point = 0;
//	*(Point+1) = MeterConstL;
//	*(Point+2) = MeterConstH;
//#endif

//	Long_BCD4( Point, MSpec.RMeterConst );			//�¹���		//13.08.30			//13.09.16	
//
//	E2P_RData( Ptr, Meter_PConst, 3 );
//	if( Data_Comp( Ptr, Point, 3 ) != 0 ) E2P_WData( Meter_PConst, Point, 3 );	 	
//	E2P_RData( Ptr, Meter_QConst, 3 );
//	if( Data_Comp( Ptr, Point, 3 ) != 0 ) E2P_WData( Meter_QConst, Point, 3 );	 	

/*	
#if ( FlashTypeSelect == YesCheck )		//֧��Flash��������		//�¹���	//14.06.04	
	E2P_RData( Ptr, EFlashType, 1 );
	if(( *Ptr == 0x64 )||( IsFlashType64() == 0 ))
	{
		if((( Flag.BatState & F_E2PCheck ) == 0 )&&( IsFlashType32AndBelow() != 0 ))
		{
			Temp = 360;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 314;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte1, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte1, Point, 4 );	 	
		
			Temp = 360+314;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 410;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte2, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte2, Point, 4 );	 	
		
			Temp = 360+314+410;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 186;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte3, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte3, Point, 4 );	 	
		
			Temp = 360+314+410+186;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 300;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte4, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte4, Point, 4 );	 	
		
			Temp = 360+314+410+186+300;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 300;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte5, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte5, Point, 4 );	 	
		
			Temp = 360+314+410+186+600;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 160;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte6, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte6, Point, 4 );	 					
		}			
	}
	else
	{
		if((( Flag.BatState & F_E2PCheck ) == 0 )||( IsFlashType32AndBelow() == 0 ))
		{
			for( i=0;i<3;i++ )										//�������߿ռ���������ʼ��
			{		
				Temp = 224+i*50;
				RAM_Write( Point, (unsigned char*)&Temp, 2 );
				Temp = 50;
				RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
				E2P_RData( Ptr, LoadCtrlByte1+i*5, 4 );
				if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte1+i*5, Point, 4 );	 	
			}	
			Temp = 224+3*50;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 300;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte4, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte4, Point, 4 );	 	
		
			Temp = 224+3*50+300;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 300;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte5, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte5, Point, 4 );	 	
		
			Temp = 224+3*50+600;
			RAM_Write( Point, (unsigned char*)&Temp, 2 );
			Temp = 50;
			RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
			E2P_RData( Ptr, LoadCtrlByte6, 4 );
			if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte6, Point, 4 );	 		
		}	
	}		 
#else
//#if( FlashAT26DF321	== YesCheck )
	for( i=0;i<3;i++ )										//�������߿ռ���������ʼ��
	{		
		Temp = 224+i*50;
		RAM_Write( Point, (unsigned char*)&Temp, 2 );
		Temp = 50;
		RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
//		E2P_WData( LoadCtrlByte1+i*5, Point, 4 );	 	
		E2P_RData( Ptr, LoadCtrlByte1+i*5, 4 );
		if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte1+i*5, Point, 4 );	 	
	}	
	Temp = 224+3*50;
	RAM_Write( Point, (unsigned char*)&Temp, 2 );
	Temp = 300;
	RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
//	E2P_WData( LoadCtrlByte4, Point, 4 );	 	
	E2P_RData( Ptr, LoadCtrlByte4, 4 );
	if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte4, Point, 4 );	 	

	Temp = 224+3*50+300;
	RAM_Write( Point, (unsigned char*)&Temp, 2 );
	Temp = 300;
	RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
//	E2P_WData( LoadCtrlByte4, Point, 4 );	 	
//	E2P_WData( LoadCtrlByte5, Point, 4 );	 	
	E2P_RData( Ptr, LoadCtrlByte5, 4 );
	if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte5, Point, 4 );	 	

	Temp = 224+3*50+600;
	RAM_Write( Point, (unsigned char*)&Temp, 2 );
	Temp = 50;
	RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
//	E2P_WData( LoadCtrlByte4, Point, 4 );	 	
//	E2P_WData( LoadCtrlByte6, Point, 4 );	 	
	E2P_RData( Ptr, LoadCtrlByte6, 4 );
	if( Data_Comp( Ptr, Point, 4 ) != 0 ) E2P_WData( LoadCtrlByte6, Point, 4 );	 		

//#else
//	for( i=0;i<6;i++ )										//�������߿ռ���������ʼ��
//	{		
//		Temp = 192+i*50;
//		RAM_Write( Point, (unsigned char*)&Temp, 2 );
//		Temp = 50;
//		RAM_Write( Point+2, (unsigned char*)&Temp, 2 );
//		E2P_WData( LoadCtrlByte1+i*5, Point, 4 );	 	
//	}	
//#endif	
#endif
*/
}	

//short JustBandRate( unsigned char BandRate )
//{
//	short i;
//	
//	for( i=1;i<7;i++ )
//	{
//		if( BandRate == ByteBit[i] ) return 0;	
//	}	 
//	return 1;
//}	

short JustBandRate( unsigned char BandRate )
{
	if( BandRate > Baut9600) return 1;
	else return 0;
}	


void CheckFRAMState( void )
{
	unsigned char Buff[2];
	unsigned char* Ptr;
	unsigned char OldState;						//�¹���
	
	Ptr = Buff;
	
	OldState = Flag.AlarmFlg[3];				//�¹���
	
	Flag.AlarmFlg[3] &= ~F_AlarmFRAM;
	E2P_RFM( Ptr, FRAMCheck, 1 );	
	if( *Ptr != 0x55 ) 
	{
		*Ptr = 0x55;	
		E2P_WFM( FRAMCheck, Ptr, 1 );			
        *Ptr = 0x0;	
		E2P_RFM( Ptr, FRAMCheck, 1 );	
		if( *Ptr != 0x55 ) 	Flag.AlarmFlg[3] |= F_AlarmFRAM;
	}
	
	if((( OldState & F_AlarmFRAM ) == 0 )&&(( Flag.AlarmFlg[3] & F_AlarmFRAM ) != 0 )) 
	{
		UpdateEventUpState( 5 );		//�¹��� 	
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		UpdateEventUpState( 2 );		//�ڿ���ʼ������	//�¹��� 		//14.06.16
#endif
	}	

//15.01.30
	OldState = Flag.AlarmFlg[5];				//�¹���
	
	Flag.AlarmFlg[5] &= ~F_AlarmEEPROM;
	E2P_RData( Ptr, EEPROMCheck, 1 );	
	if( *Ptr != 0x55 ) 
	{
		*Ptr = 0x55;	
		E2P_WData( EEPROMCheck, Ptr, 1 );			
        *Ptr = 0x0;	
		E2P_RData( Ptr, EEPROMCheck, 1 );	
		if( *Ptr != 0x55 ) 	Flag.AlarmFlg[5] |= F_AlarmEEPROM;
	}
	
	if((( OldState & F_AlarmEEPROM ) == 0 )&&(( Flag.AlarmFlg[5] & F_AlarmEEPROM ) != 0 )) 
	{
		UpdateEventUpState( 5 );		//�¹��� 	
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		UpdateEventUpState( 2 );		//�ڿ���ʼ������	//�¹��� 		//14.06.16
#endif
	}	

}

void RpaInit(void)			
{
	SetConstPara();
	InitPara();
}	

//ÿ���ӷ�ʱ����EEPROM����
void EveryMinInitPara(void)
{
	if( Clk.Sec[0] == 0x50 ) InitPara1();			 	
	else if( Clk.Sec[0] == 0x23 ) InitPara2();			 	
	else if( Clk.Sec[0] == 0x29 ) InitPara3();			 	
	else if( Clk.Sec[0] == 0x40 ) InitPara4();			 	
	else if( Clk.Sec[0] == 0x45 ) InitPara5();			 	
	else if( Clk.Sec[0] == 0x10 ) InitPara7();			 	
}	

void InitPara(void)			
{
	InitPara1();			
	InitPara2();			
	InitPara3();			
	InitPara4();			
	InitPara5();			
	InitPara6();			
	InitPara7();			
}

#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
short GetStartupPower( void )						//���������������ڼ̵������ϼ��		//�¹���	//14.06.16
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned long Value1;
	unsigned long Value2;
#if( LinkMode == Phase3Wire3 )	
	float FValue;
#endif
	unsigned short Temp;
		
	Ptr = Buff;

	Value1 = MSpec.RBaseCurrent;	
	Value1 *= MSpec.RRatedVolt;				

#if( LinkMode == Phase3Wire3 )	
	FValue = Value1;
	FValue *= 1.732; 
	Value1 = (unsigned long )FValue;
#else
	Value1 *= 3;
#endif
	
	if( MSpec.RBaseCurrent >= 50000 )					//Ib=5A����Ϊֱ��ʽ��
	{
		Value1 /= 250;									//0.4%Ib			
	}
	else
	{
		E2P_RData( Ptr, ActiveGrade, 4 );                                                      	
		if( *(Ptr+3) == 0x30 )							//'0'	//0.2S��0.5S��
		{
			Value1 /= 1000;								//0.2S��0.5S������ʽ��������Ϊ0.1%Ib
		}	
		else Value1 /= 500;								//1������ʽ��������Ϊ0.2%Ib			
	}			

	Value2 = Value1 / 10000;							//0.1W
	Temp = Value1 % 10000;
//	if( Temp >= 5000 ) Value2 += 1;						//��ֵ����ʹ����������	
	
	if( Temp == 0 ) Temp = 1;							//��ֵ��СΪ0.1W;	
	Temp = Value2; 
	
	return Temp;	
}	
#endif

void GetMeterPARAM( void )		//�¹���	//13.09.16
{
	unsigned long Value;
	unsigned char Buff[24];
	unsigned char* Point;
	short i;//,j;
	short FlashState = 0;	
	unsigned short EAddr;
	unsigned char* RAds;
	unsigned char DataLen;
//	unsigned char FlashAddr;	
//	unsigned char Buff2[4];											
//    unsigned char* Ptr;
	
//	Ptr = Buff2;													
	Point = Buff;

	RAM_Fill( Point, 24 );
	MSpec.RMeterConst = 0;                                                  	
	for( i=0;i<MeterSpecNum;i++ )
	{	
		EAddr = MeterSpecTab[i].Addr;	
		RAds = MeterSpecTab[i].RAds;	
		DataLen = MeterSpecTab[i].DataLen;	
//		FlashAddr = MeterSpecTab[i].FlashAddr;	

       	E2P_RData( Point, EAddr, DataLen );		
/*		if(( Flag.BatState & F_E2PCheck ) != 0 )	
		{													
//			if(( P8OUT & P_EECTL ) != 0 )			//��Flash���ڹر�״̬����FLASH��������������ʱ��
			if(( HT_GPIOB->PTSET & GPIOB_VMEM_CTL ) != 0 )			//��Flash���ڹر�״̬����FLASH��������������ʱ��
			{
				FlashState = 1;	
//				P8OUT &= ~P_EECTL;
				HT_GPIOB->PTCLR |= GPIOB_VMEM_CTL; //P8OUT &= ~P_EECTL;
//				WDTCTL = WDT_ARST_1000;
   				HT_FreeDog();
				Delay(10000);
			}	

			for( j=0;j<2;j++ )															
			{																				
				Value = (unsigned long)(AT7022DataZonePage+j)*4096;					
				Value += 256;
				Read_Flash( Point, Value+FlashAddr, DataLen+1 );									
//				if( *(Point+DataLen+1) == ChkNum( Point, DataLen )) 									
				if( *(Point+DataLen) == ChkNum( Point, DataLen )) 		//14.01.24							
				{
					break;	
				}	
			}
			if( j == 2 ) 
			{
		       	E2P_RData( Point, EAddr, DataLen );		
			}	
		}											//10.08.17	
*/        
        RAM_Write( RAds, Point, DataLen );		
	}
	if(FlashState != 0 )			//��ԭFlash���ڹر�״̬����������ʱ��ر�FLASH��Դ��
	{
//		WDTCTL = WDT_ARST_1000;
   		HT_FreeDog();
		Delay(10000);
//		WDTCTL = WDT_ARST_1000;
   		HT_FreeDog();
		Delay(10000);
//		P8OUT |= P_EECTL;
		HT_GPIOB->PTSET |= GPIOB_VMEM_CTL;	//P8OUT |= P_EECTL;
	}	
	
/*	Long_BCD4( Point, MSpec.RMeterConst );			//�¹���		//13.08.30	//13.09.16			
                                                                                            
	E2P_RData( Ptr, Meter_PConst, 3 );                                                      
	if( Data_Comp( Ptr, Point, 3 ) != 0 ) E2P_WData( Meter_PConst, Point, 3 );	 	        
	E2P_RData( Ptr, Meter_QConst, 3 );                                                      
	if( Data_Comp( Ptr, Point, 3 ) != 0 ) E2P_WData( Meter_QConst, Point, 3 );	
*/
	E2P_RData( (unsigned char*)&Value, Meter_PConst, 4 );                                                      
	if( MSpec.RMeterConst != Value ) E2P_WData( Meter_PConst, (unsigned char*)&MSpec.RMeterConst, 4 );	 	        
	E2P_RData( (unsigned char*)&Value, Meter_QConst, 4 );                                                      
	if( MSpec.RMeterConst != Value ) E2P_WData( Meter_QConst, (unsigned char*)&MSpec.RMeterConst, 4 );	 	        
	
	if( MSpec.RDisA0001Ib > 50000 )	MSpec.RDisA0001Ib = MSpec.RBaseCurrent/500; 		//ȱʡ��С��ʾ��������Ϊ0.2%Ib		//�¹���	//13.10.23	        

#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
	Para.RStartupPower = GetStartupPower();		//���������������ڼ̵������ϼ��		//�¹���	//14.06.16
#endif

}

void InitPara1(void)			
{
	short Temp;
//	unsigned char Buff[4];			//�¹���	//14.03.22	
//	unsigned char* Ptr;				//�¹���	//14.03.22
	
//	Ptr = Buff;						//�¹���	//14.03.22	
	
	GetMeterPARAM();				//�¹���		//13.08.30
	
	E2P_RData( (unsigned char*)&SM.RPDCoverCheckCnt, EPDCoverCheckCnt, 1 );				//����󿪸��Ѽ�����	//�¹���
	E2P_RData( (unsigned char*)&SM.RPDTerminalCheckCnt, EPDTerminalCheckCnt, 1 );		//����󿪶��Ѽ�����	//�¹���

	E2P_RData( (unsigned char*)&SM.RPulseOutMode,PulseOutMode, 1 ); 
	
	if( SM.RPulseOutMode == 0 ) 
	{
//		Temp = EN32K1Hz;		
//		if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//�͵�ƽΪ8025T				//�¹���
//		{
			Temp = EN32K1Hz_8025T;		
//		}
//		else Temp = EN32K1Hz_DS3231;					
#if ( NewHardScheme2 == YesCheck )		//10.12.25
#else
//		PDIR_SECOUT |= P_SECOUT;		//V1
		HT_GPIOD->PTDIR |= GPIOD_SECOUT;	
#endif		
		SM.PulseWidthCnt = 0;
	}	
	else 
	{
//		Temp = EN32K;
//		if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//�͵�ƽΪ8025T				//�¹���
//		{
			Temp = EN32K_8025T;		
//		}
//		else Temp = EN32K_DS3231;					
	}
//	if(( Flag.Power & F_PwrUp ) != 0 )	HWTime( Temp );				//EC
	if(( Flag.Power & F_PwrUp ) != 0 )
	{
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))					//10.12.04
//		if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//�͵�ƽΪ8025T				//�¹���
//		{
			RepeatHWTime( Temp );											
//		}	
//#else
//		else																//�¹���
//		{
//			HWTime( Temp );				//EC
//			*Ptr = 0;									//�¹���	//14.03.22
//			E2P_RData( Ptr, ECrystalAging, 1 );   		//�¹���	//14.03.22
//			if(( Flag.BatState & F_E2PCheck ) != 0 ) 	//�¹���	//14.03.22
//			{
//				if(( *Ptr > 64 )&&( *Ptr < 192 ))
//				{ 
//					*Ptr = 0;
//					E2P_WData( ECrystalAging, Ptr, 1 );   	
//				}	
//			}
//			else										//�¹���	//14.03.22	
//			{
//				*(Ptr+1) = 0;
//    	       	E2PRead( Ptr+1, 0x10, 1, DS3231Ads );	//Crystal Aging	Offset	
//				if( *Ptr != *(Ptr+1) )
//				{
//					E2Pwrite( 0x10, Ptr, 1, DS3231Ads );			
//				}					
//			}		
//		}	
//#endif		
	}	
	
	E2P_RData( &Para.RRevPhaseTu, ERevPhaseTu, 1 );					
	Para.RRevPhaseTu = BCD_Byte(Para.RRevPhaseTu);

	E2P_RData( &Para.RItemDisType, ItemDisType, 1 );					
	
#if ( IDT90E32AS ==	YesCheck )	//12.09.05	
	E2P_RData( Para.RAT7022ChkSum, AT7022ChkSum, 2 );				//IDT��CRC����Ϊ2�ֽ�	//�¹���	//14.01.16	
#else
	E2P_RData( Para.RAT7022ChkSum, AT7022ChkSum, 3 );				//ATChk	
#endif

#if ( NEW7022E == YesCheck )
	E2P_RData( Para.RAT7022ChkSum2, AT7022ChkSum2, 3 );	
#endif

//#if ( MEASCHIP == IDT90E32 )
//	E2P_RData( Para.RIDT90E32ChkSum0, IDT90E32ChkSum0, 2 );				
//	E2P_RData( Para.RIDT90E32ChkSum1, IDT90E32ChkSum1, 2 );				
//	E2P_RData( Para.RIDT90E32ChkSum2, IDT90E32ChkSum2, 2 );				
//	E2P_RData( Para.RIDT90E32ChkSum3, IDT90E32ChkSum3, 2 );				
//#endif
	
	E2P_RData( (unsigned char*)&Para.RECDisDecDigits, ECDisDecDigits, 1 );
//	if( Para.RECDisDecDigits > 4 ) Para.RECDisDecDigits = 2;		//����ȱʡ��λС��
	if( Para.RECDisDecDigits > 3 ) Para.RECDisDecDigits = 2;		//����ȱʡ��λС��		//�¹���	//13.10.19
	E2P_RData( (unsigned char*)&Para.RMDDisDecDigits, MDDisDecDigits, 1 );
	if( Para.RMDDisDecDigits > 4 ) Para.RMDDisDecDigits = 4;		//����ȱʡ4λС��

	E2P_RData( (unsigned char*)&Para.RPp,UpperPower_ECPp0, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPn,UpperPower_ECPn0, 3 ); 
	
//#if( AllFailChk == YesCheck )		
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
	E2P_RData( (unsigned char*)&SM.IaOffset0, IOffSet0, 6 );
#else
#endif

#if ( AdjustErrorCheck == YesCheck )	//11.09.01
	InitErrorPara();
#endif

}

/*
void InitPara2(void)			
{
	E2P_RData( (unsigned char*)&Para.RPhFail_Valve,EPhFail_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RPhFail_EndU,EPhFail_EndU, 2 ); 
	E2P_RData( (unsigned char*)&Para.RPhFail_I,EPhFail_I, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPhFailTu,EPhFailTu, 1 ); 
	Para.RPhFailTu = BCD_Byte(Para.RPhFailTu);

	E2P_RData( (unsigned char*)&Para.RVoltLower_Valve,EVoltLower_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RVoltLowerTu,EVoltLowerTu, 1 ); 
	Para.RVoltLowerTu = BCD_Byte(Para.RVoltLowerTu);

	E2P_RData( (unsigned char*)&Para.RVoltOver_Valve,EVoltOver_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RVoltOverTu,EVoltOverTu, 1 ); 
	Para.RVoltOverTu = BCD_Byte(Para.RVoltOverTu);

	E2P_RData( (unsigned char*)&Para.RPhBreak_Valve,EPhBreak_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RPhBreak_I,EPhBreak_I, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPhBreakTu,EPhBreakTu, 1 ); 
	Para.RPhBreakTu = BCD_Byte(Para.RPhBreakTu);

	E2P_RData( (unsigned char*)&Para.RVoltImbalance_Valve,EVoltImbalance_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RVoltImbalanceTu,EVoltImbalanceTu, 1 ); 
	Para.RVoltImbalanceTu = BCD_Byte(Para.RVoltImbalanceTu);

	E2P_RData( (unsigned char*)&Para.RCuImbalance_Valve,ECuImbalance_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RCuImbalanceTu,ECuImbalanceTu, 1 ); 
	Para.RCuImbalanceTu = BCD_Byte(Para.RCuImbalanceTu);

	E2P_RData( (unsigned char*)&Para.RCuFail_Valve,ECuFail_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RCuFail_UpperI,ECuFail_UpperI, 3 ); 
	E2P_RData( (unsigned char*)&Para.RCuFail_LowerI,ECuFail_LowerI, 3 ); 
	E2P_RData( (unsigned char*)&Para.RCuFailTu,ECuFailTu, 1 ); 
	Para.RCuFailTu = BCD_Byte(Para.RCuFailTu);
	
	E2P_RData( (unsigned char*)&Para.RCuFailTu,ECuFailTu, 1 ); 
	Para.RCuFailTu = BCD_Byte(Para.RCuFailTu);

	E2P_RData( (unsigned char*)&Para.RMDOverPp_Tu,EMDOverPp_Tu, 1 ); 		//�����й����������жϳ���ʱ��
	Para.RMDOverPp_Tu = BCD_Byte(Para.RMDOverPp_Tu);

	E2P_RData( (unsigned char*)&Para.RMDOverPn_Tu,EMDOverPn_Tu, 1 ); 		//�����й����������жϳ���ʱ��
	Para.RMDOverPn_Tu = BCD_Byte(Para.RMDOverPn_Tu);

	E2P_RData( (unsigned char*)&Para.RMDOverQ_Tu,EMDOverQ_Tu, 1 ); 			//�޹����������жϳ���ʱ��
	Para.RMDOverQ_Tu = BCD_Byte(Para.RMDOverQ_Tu);

	E2P_RData( (unsigned char*)&Para.RAuxPWFailTu,AuxPWFailTu, 1 ); 		//������Դ�����жϳ���ʱ��
	Para.RAuxPWFailTu = BCD_Byte(Para.RAuxPWFailTu);

	E2P_RData( (unsigned char*)&Para.RVoltRevPhaseTu,VoltRevPhaseTu, 1 ); 	//��ѹ�������жϳ���ʱ��
	Para.RVoltRevPhaseTu = BCD_Byte(Para.RVoltRevPhaseTu);

	E2P_RData( (unsigned char*)&Para.RCuRevPhaseTu,CuRevPhaseTu, 1 ); 		//�����������Դ�����жϳ���ʱ��
	Para.RCuRevPhaseTu = BCD_Byte(Para.RCuRevPhaseTu);

	E2P_RData( (unsigned char*)&Para.RPowerDownTu,PowerDownTu, 1 ); 		//��Դ�����жϳ���ʱ��
	Para.RPowerDownTu = BCD_Byte(Para.RPowerDownTu);

	E2P_RData( (unsigned char*)&Para.RClockErr_Tu,ClockErr_Tu, 1 ); 		//ʱ�ӹ����жϳ���ʱ��
	Para.RClockErr_Tu = BCD_Byte(Para.RClockErr_Tu);

	E2P_RData( (unsigned char*)&Para.RMeasChipErr_Tu,MeasChipErr_Tu, 1 ); 	//����оƬ�����жϳ���ʱ��
	Para.RMeasChipErr_Tu = BCD_Byte(Para.RMeasChipErr_Tu);	
	
}	
*/

void InitPara2(void)			
{
	unsigned short Temp;
	
//	E2P_RData( (unsigned char*)&Para.RPhFail_Valve,EPhFail_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Temp,EPhFail_Valve, 2 ); 
	Word_BCD( (unsigned char*)&Para.RPhFail_Valve, Temp );
//	E2P_RData( (unsigned char*)&Para.RPhFail_EndU,EPhFail_EndU, 2 ); 
	E2P_RData( (unsigned char*)&Temp,EPhFail_EndU, 2 ); 
	Word_BCD( (unsigned char*)&Para.RPhFail_EndU, Temp );
//	E2P_RData( (unsigned char*)&Para.RPhFail_I,EPhFail_I, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPhFail_I,EPhFail_I, 4 ); 
	if( Para.RPhFail_I < 0 )  Para.RPhFail_I = 0 - Para.RPhFail_I;
	E2P_RData( (unsigned char*)&Para.RPhFailTu,EPhFailTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RVoltLower_Valve,EVoltLower_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Temp,EVoltLower_Valve, 2 ); 
	Word_BCD( (unsigned char*)&Para.RVoltLower_Valve, Temp );
	E2P_RData( (unsigned char*)&Para.RVoltLowerTu,EVoltLowerTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RVoltOver_Valve,EVoltOver_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Temp,EVoltOver_Valve, 2 ); 
	Word_BCD( (unsigned char*)&Para.RVoltOver_Valve, Temp );
	E2P_RData( (unsigned char*)&Para.RVoltOverTu,EVoltOverTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RPhBreak_Valve,EPhBreak_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Temp,EPhBreak_Valve, 2 ); 
	Word_BCD( (unsigned char*)&Para.RPhBreak_Valve, Temp );
//	E2P_RData( (unsigned char*)&Para.RPhBreak_I,EPhBreak_I, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPhBreak_I,EPhBreak_I, 4 ); 
	if( Para.RPhBreak_I < 0 )  Para.RPhBreak_I = 0 - Para.RPhBreak_I;
	E2P_RData( (unsigned char*)&Para.RPhBreakTu,EPhBreakTu, 1 ); 

	E2P_RData( (unsigned char*)&Para.RVoltImbalance_Valve,EVoltImbalance_Valve, 2 ); 
	if( Para.RVoltImbalance_Valve < 0 )  Para.RVoltImbalance_Valve = 0 - Para.RVoltImbalance_Valve;
	E2P_RData( (unsigned char*)&Para.RVoltImbalanceTu,EVoltImbalanceTu, 1 ); 

	E2P_RData( (unsigned char*)&Para.RCuImbalance_Valve,ECuImbalance_Valve, 2 ); 
	if( Para.RCuImbalance_Valve < 0 )  Para.RCuImbalance_Valve = 0 - Para.RCuImbalance_Valve;
	E2P_RData( (unsigned char*)&Para.RCuImbalanceTu,ECuImbalanceTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RCuFail_Valve,ECuFail_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Temp,ECuFail_Valve, 2 ); 
	Word_BCD( (unsigned char*)&Para.RCuFail_Valve, Temp );
//	E2P_RData( (unsigned char*)&Para.RCuFail_UpperI,ECuFail_UpperI, 3 ); 
	E2P_RData( (unsigned char*)&Para.RCuFail_UpperI,ECuFail_UpperI, 4 ); 
	if( Para.RCuFail_UpperI < 0 )  Para.RCuFail_UpperI = 0 - Para.RCuFail_UpperI;
//	E2P_RData( (unsigned char*)&Para.RCuFail_LowerI,ECuFail_LowerI, 3 ); 
	E2P_RData( (unsigned char*)&Para.RCuFail_LowerI,ECuFail_LowerI, 4 ); 
	if( Para.RCuFail_LowerI < 0 )  Para.RCuFail_LowerI = 0 - Para.RCuFail_LowerI;
	E2P_RData( (unsigned char*)&Para.RCuFailTu,ECuFailTu, 1 ); 
	
	E2P_RData( (unsigned char*)&Para.RCuFailTu,ECuFailTu, 1 ); 

	E2P_RData( (unsigned char*)&Para.RMDOverPp_Tu,EMDOverPp_Tu, 1 ); 		//�����й����������жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RMDOverPn_Tu,EMDOverPn_Tu, 1 ); 		//�����й����������жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RMDOverQ_Tu,EMDOverQ_Tu, 1 ); 			//�޹����������жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RAuxPWFailTu,AuxPWFailTu, 1 ); 		//������Դ�����жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RVoltRevPhaseTu,VoltRevPhaseTu, 1 ); 	//��ѹ�������жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RCuRevPhaseTu,CuRevPhaseTu, 1 ); 		//�����������Դ�����жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RPowerDownTu,PowerDownTu, 1 ); 		//��Դ�����жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RClockErr_Tu,ClockErr_Tu, 1 ); 		//ʱ�ӹ����жϳ���ʱ��

	E2P_RData( (unsigned char*)&Para.RMeasChipErr_Tu,MeasChipErr_Tu, 1 ); 	//����оƬ�����жϳ���ʱ��
	
}	

/*
void InitPara3(void)			
{
#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
	unsigned char Buff[10];			//�¹���	//14.06.19
	unsigned char* Ptr;	
	unsigned short Temp;
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
	unsigned long Value;		
#endif
	
	Ptr = Buff;
#endif

	E2P_RData( (unsigned char*)&Para.RCuOver_Valve,ECuOver_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RCuOverTu,ECuOverTu, 1 ); 
	Para.RCuOverTu = BCD_Byte(Para.RCuOverTu);

	E2P_RData( (unsigned char*)&Para.RCuBreak_Valve,ECuBreak_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RCuBreak_I,ECuBreak_I, 3 ); 
	E2P_RData( (unsigned char*)&Para.RCuBreakTu,ECuBreakTu, 1 ); 
	Para.RCuBreakTu = BCD_Byte(Para.RCuBreakTu);

	E2P_RData( (unsigned char*)&Para.RRevPower_Valve,ERevPower_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RRevPowerTu,ERevPowerTu, 1 ); 
	Para.RRevPowerTu = BCD_Byte(Para.RRevPowerTu);

	E2P_RData( (unsigned char*)&Para.RPowerOver_Valve,EPowerOver_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPowerOverTu,EPowerOverTu, 1 ); 
	Para.RPowerOverTu = BCD_Byte(Para.RPowerOverTu);

	E2P_RData( (unsigned char*)&Para.RExamVoltUpperLimit,ExamVoltUpperLimit, 2 ); 
	E2P_RData( (unsigned char*)&Para.RExamVoltLowerLimit,ExamVoltLowerLimit, 2 ); 

//	E2P_RData( (unsigned char*)&Para.RMDOverP_Valve,EMDOverP_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RMDOverPp_Valve,EMDOverPp_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RMDOverPn_Valve,EMDOverPn_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RMDOverQ_Valve,EMDOverQ_Valve, 3 ); 
//	E2P_RData( (unsigned char*)&Para.RMDOverTu,EMDOverTu, 1 ); 
//	Para.RMDOverTu = BCD_Byte(Para.RMDOverTu);

	E2P_RData( (unsigned char*)&Para.RPftOver_Valve,EPftOver_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RPftOverTu,EPftOverTu, 1 ); 
	Para.RPftOverTu = BCD_Byte(Para.RPftOverTu);

	E2P_RData( (unsigned char*)&Para.RCuBestImbalance_Valve,ECuBestImbalance_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RCuBestImbalanceTu,ECuBestImbalanceTu, 1 ); 
	Para.RCuBestImbalanceTu = BCD_Byte(Para.RCuBestImbalanceTu);

	E2P_RData( (unsigned char*)&Para.RTideSwitch_Valve,ETideSwitch_Valve, 3 ); 				//�¹���	
	E2P_RData( (unsigned char*)&Para.RTideSwitchTu,ETideSwitchTu, 1 ); 						//�¹���
	Para.RTideSwitchTu = BCD_Byte(Para.RTideSwitchTu);										//�¹���		

//	Para.RMagnetDisturbTu = 5;   															//�¹���
//	Para.RRelayFaultTu = 2;														  			//�¹���	//15.02.03

	E2P_RData( (unsigned char*)&Para.RMagnetDisturbTu, MagnetDisturbTu, 1 ); 				
	Para.RMagnetDisturbTu = BCD_Byte(Para.RMagnetDisturbTu);			
						
	E2P_RData( (unsigned char*)&Para.RRelayFaultTu,RelayFaultTu, 1 ); 						
	Para.RRelayFaultTu = BCD_Byte(Para.RRelayFaultTu);										

#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
//	E2P_RData( (unsigned char*)&Para.RILimitTrip_Valve, EILimitTrip_Valve, 3 ); 			//�̵�����բ���Ƶ�������ֵ(3)	xxx.xxxA
	E2P_RData( (unsigned char*)&Value, EILimitTrip_Valve, 3 ); 			//�̵�����բ���Ƶ�������ֵ(3)	xxx.xxxA
	Long_BCD3( Para.RILimitTrip_Valve, Value );

//	E2P_RData( Ptr, EUp30AClose24HOUR, 2 ); 			//����������������ʱ������		//�¹���	//14.06.19
//	Value = BCD2_Word( Ptr );																//�¹���	//14.06.19
	E2P_RData( (unsigned char*)&Temp, EUp30AClose24HOUR, 2 ); 			//����������������ʱ������		//�¹���	//14.06.19
	Value = Temp;
	Value *= 60;										//��λ������ת������				//�¹���	//14.06.19		
	Para.RUp30AClose24HOUR = Value;															//�¹���	//14.06.19
#endif

//	E2P_RData( Ptr, EInfraredValid30MIN, 1 ); 		//������֤ʱЧ30����					//�¹���	//14.06.20
//	Temp = BCD_Byte( *Ptr );																//�¹���	//14.06.20
//	Temp *= 60;										//��λ������ת������					//�¹���	//14.06.20		
//	Para.RInfraredValid30MIN = Temp;														//�¹���	//14.06.20
                                                                                                                
	E2P_RData( Ptr, EClearUpState30MIN, 1 );		//�ϱ�����״̬�ֺ���ʱ30��������		//�¹���	//14.06.20
//	Temp = BCD_Byte( *Ptr );																//�¹���	//14.06.20
//	Temp *= 60;										//��λ������ת������					//�¹���	//14.06.20		
//	Para.RClearUpState30MIN = Temp;															//�¹���	//14.06.20
	if(( Flag.BatState & F_E2PCheck ) == 0 )		//15.01.30
	{
		Temp = BCD_Byte( *Ptr );															//�¹���	//14.06.20
		Temp *= 60;									//��λ������ת������					//�¹���	//14.06.20		
		Para.RClearUpState30MIN = Temp;														//�¹���	//14.06.20
	}	
#endif

}
*/
void InitPara3(void)			
{
#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
	unsigned char Buff[10];			//�¹���	//14.06.19
	unsigned char* Ptr;	
	unsigned short Temp;
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
	unsigned long Value;		
#endif
	
	Ptr = Buff;
#endif

//	E2P_RData( (unsigned char*)&Para.RCuOver_Valve,ECuOver_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Para.RCuOver_Valve,ECuOver_Valve, 4 ); 
	if( Para.RCuOver_Valve < 0 )  Para.RCuOver_Valve = 0 - Para.RCuOver_Valve;
	E2P_RData( (unsigned char*)&Para.RCuOverTu,ECuOverTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RCuBreak_Valve,ECuBreak_Valve, 2 ); 
	E2P_RData( (unsigned char*)&Temp,ECuBreak_Valve, 2 ); 
	Word_BCD( (unsigned char*)&Para.RCuBreak_Valve, Temp );
//	E2P_RData( (unsigned char*)&Para.RCuBreak_I,ECuBreak_I, 3 ); 
	E2P_RData( (unsigned char*)&Para.RCuBreak_I,ECuBreak_I, 4 ); 
	if( Para.RCuBreak_I < 0 )  Para.RCuBreak_I = 0 - Para.RCuBreak_I;
	E2P_RData( (unsigned char*)&Para.RCuBreakTu,ECuBreakTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RRevPower_Valve,ERevPower_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RRevPower_Valve,ERevPower_Valve, 4 ); 
	if( Para.RRevPower_Valve < 0 )  Para.RRevPower_Valve = 0 - Para.RRevPower_Valve;
	E2P_RData( (unsigned char*)&Para.RRevPowerTu,ERevPowerTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RPowerOver_Valve,EPowerOver_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RPowerOver_Valve,EPowerOver_Valve, 4 ); 
	if( Para.RPowerOver_Valve < 0 )  Para.RPowerOver_Valve = 0 - Para.RPowerOver_Valve;
	E2P_RData( (unsigned char*)&Para.RPowerOverTu,EPowerOverTu, 1 ); 

	E2P_RData( (unsigned char*)&Para.RExamVoltUpperLimit,ExamVoltUpperLimit, 2 ); 
	E2P_RData( (unsigned char*)&Para.RExamVoltLowerLimit,ExamVoltLowerLimit, 2 ); 

//	E2P_RData( (unsigned char*)&Para.RMDOverP_Valve,EMDOverP_Valve, 3 ); 
//	E2P_RData( (unsigned char*)&Para.RMDOverPp_Valve,EMDOverPp_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RMDOverPp_Valve,EMDOverPp_Valve, 4 ); 
//	if( Para.RMDOverPp_Valve < 0 )  Para.RMDOverPp_Valve = 0 - Para.RMDOverPp_Valve;
//	E2P_RData( (unsigned char*)&Para.RMDOverPn_Valve,EMDOverPn_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RMDOverPn_Valve,EMDOverPn_Valve, 4 ); 
//	if( Para.RMDOverPn_Valve < 0 )  Para.RMDOverPn_Valve = 0 - Para.RMDOverPn_Valve;
//	E2P_RData( (unsigned char*)&Para.RMDOverQ_Valve,EMDOverQ_Valve, 3 ); 
	E2P_RData( (unsigned char*)&Para.RMDOverQ_Valve,EMDOverQ_Valve, 4 ); 
//	if( Para.RMDOverQ_Valve < 0 )  Para.RMDOverQ_Valve = 0 - Para.RMDOverQ_Valve;
//	E2P_RData( (unsigned char*)&Para.RMDOverTu,EMDOverTu, 1 ); 
//	Para.RMDOverTu = BCD_Byte(Para.RMDOverTu);

	E2P_RData( (unsigned char*)&Para.RPftOver_Valve,EPftOver_Valve, 2 ); 
	if( Para.RPftOver_Valve < 0 )  Para.RPftOver_Valve = 0 - Para.RPftOver_Valve;
	E2P_RData( (unsigned char*)&Para.RPftOverTu,EPftOverTu, 1 ); 

	E2P_RData( (unsigned char*)&Para.RCuBestImbalance_Valve,ECuBestImbalance_Valve, 2 ); 
	if( Para.RCuBestImbalance_Valve < 0 )  Para.RCuBestImbalance_Valve = 0 - Para.RCuBestImbalance_Valve;
	E2P_RData( (unsigned char*)&Para.RCuBestImbalanceTu,ECuBestImbalanceTu, 1 ); 

//	E2P_RData( (unsigned char*)&Para.RTideSwitch_Valve,ETideSwitch_Valve, 3 ); 				//�¹���	
	E2P_RData( (unsigned char*)&Para.RTideSwitch_Valve,ETideSwitch_Valve, 4 ); 				//�¹���	
	if( Para.RTideSwitch_Valve < 0 )  Para.RTideSwitch_Valve = 0 - Para.RTideSwitch_Valve;
	E2P_RData( (unsigned char*)&Para.RTideSwitchTu,ETideSwitchTu, 1 ); 						//�¹���

	Para.RMagnetDisturbTu = 5;   															//�¹���
	Para.RRelayFaultTu = 2;														  			//�¹���	//15.02.03

//	E2P_RData( (unsigned char*)&Para.RMagnetDisturbTu, MagnetDisturbTu, 1 ); 				//17.04.21
						
//	E2P_RData( (unsigned char*)&Para.RRelayFaultTu,RelayFaultTu, 1 ); 						//17.04.21

#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
//	E2P_RData( (unsigned char*)&Para.RILimitTrip_Valve, EILimitTrip_Valve, 3 ); 			//�̵�����բ���Ƶ�������ֵ(3)	xxx.xxxA
//	E2P_RData( (unsigned char*)&Value, EILimitTrip_Valve, 3 ); 			//�̵�����բ���Ƶ�������ֵ(3)	xxx.xxxA
//	Long_BCD3( Para.RILimitTrip_Valve, Value );
	E2P_RData( (unsigned char*)&Para.RILimitTrip_Valve, EILimitTrip_Valve, 4 ); 			//�̵�����բ���Ƶ�������ֵ(3)	xxx.xxxA

//	E2P_RData( Ptr, EUp30AClose24HOUR, 2 ); 			//����������������ʱ������		//�¹���	//14.06.19
//	Value = BCD2_Word( Ptr );																//�¹���	//14.06.19
	E2P_RData( (unsigned char*)&Temp, EUp30AClose24HOUR, 2 ); 			//����������������ʱ������		//�¹���	//14.06.19
	Value = Temp;
	Value *= 60;										//��λ������ת������				//�¹���	//14.06.19		
	Para.RUp30AClose24HOUR = Value;															//�¹���	//14.06.19
#endif

//	E2P_RData( Ptr, EInfraredValid30MIN, 1 ); 		//������֤ʱЧ30����					//�¹���	//14.06.20
//	Temp = BCD_Byte( *Ptr );																//�¹���	//14.06.20
//	Temp *= 60;										//��λ������ת������					//�¹���	//14.06.20		
//	Para.RInfraredValid30MIN = Temp;														//�¹���	//14.06.20
                                                                                                                
	E2P_RData( Ptr, EClearUpState30MIN, 1 );		//�ϱ�����״̬�ֺ���ʱ30��������		//�¹���	//14.06.20
//	Temp = BCD_Byte( *Ptr );																//�¹���	//14.06.20
//	Temp *= 60;										//��λ������ת������					//�¹���	//14.06.20		
//	Para.RClearUpState30MIN = Temp;															//�¹���	//14.06.20
	if(( Flag.BatState & F_E2PCheck ) == 0 )		//15.01.30
	{
		Temp = BCD_Byte( *Ptr );															//�¹���	//14.06.20
		Temp *= 60;									//��λ������ת������					//�¹���	//14.06.20		
		Para.RClearUpState30MIN = Temp;														//�¹���	//14.06.20
	}	
#endif

}


void InitPara4(void)			
{
//	unsigned char Buff[10];		//�¹���	//13.10.15
	unsigned char Buff[16];		//�¹���	//15.01.30
	unsigned char* Ptr;			//�¹���	//13.10.15
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
	unsigned long Value;		//�¹���	//13.10.15
#endif
	Ptr = Buff;					//�¹���	//13.10.15
	
	E2P_RData( (unsigned char*)&Para.RVoltUpperLimit,VoltUpperLimit, 2 ); 
	E2P_RData( (unsigned char*)&Para.RVoltLowerLimit,VoltLowerLimit, 2 ); 

	E2P_RData( (unsigned char*)&Para.RMeterRunMode1,MeterRunMode1, 1 ); 		//�������������
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
//	E2P_RData( (unsigned char*)&Para.REventUpMode,EEventUpMode, 8 ); 			//�¹���	//14.04.26
	E2P_RData( Ptr,EEventUpMode, 8 ); 			//�¹���	//14.04.26			//15.01.30
	if(( Flag.BatState & F_E2PCheck ) == 0 )									//15.01.30	
	{
		RAM_Write( (unsigned char*)&Para.REventUpMode, Ptr, 8 );	
		E2P_RFM( Ptr+8, FEventUpMode, 8 );										//16.05.17
		if( Data_Comp( Ptr, Ptr+8, 8 ) != 0 )									//16.05.17
		{																		//16.05.17
			E2P_WFM( FEventUpMode, Ptr, 8 );									//16.05.17								
		}																		//16.05.17
	}	
	else																		//16.05.17
	{                                                                           //16.05.17
		E2P_RFM( Ptr, FEventUpMode, 8 );									    //16.05.17
		if(( Flag.BatState & F_E2PCheck ) == 0 )                                //16.05.17
		{                                                                       //16.05.17
			RAM_Write( (unsigned char*)&Para.REventUpMode, Ptr, 8 );			//16.05.17
		}	                                                                    //16.05.17
	}                                                                           //16.05.17
#else
	E2P_RData( (unsigned char*)&Para.REventUpMode,EEventUpMode, 6 ); 			//�¹���	//13.10.10
#endif

//	E2P_RData( (unsigned char*)&Para.RAlarm_Control1,Alarm_Control1, 4 ); 	
//	E2P_RData( (unsigned char*)&Para.RAlarm_Control2,Alarm_Control2, 4 ); 	
//	E2P_RData( (unsigned char*)&Para.RAlarm_Control3,Alarm_Control3, 4 ); 	

	E2P_RData( (unsigned char*)&Para.RAlarm_Control1,Alarm_Control1, 8 ); 	
	E2P_RData( (unsigned char*)&Para.RAlarm_Control2,Alarm_Control2, 8 ); 	
	E2P_RData( (unsigned char*)&Para.RAlarm_Control3,Alarm_Control3, 8 ); 	
	
//	E2P_RData( (unsigned char*)&Para.RAlarm_Control,Alarm_Control, 2 ); 	
//	E2P_RData( (unsigned char*)&Comm.BandRateMode,EBandRateMode, 1 ); 

//	E2P_RData( (unsigned char*)&SM.LoadAdjMode,EBandRateMode, 1 ); 			//cru	//����е����ĸ�������
	E2P_RData( (unsigned char*)&SM.LoadAdjMode,ELoadAdjMode, 1 ); 			//cru	//����е����ĸ�������

	//E2P_RData( (unsigned char*)&Para.RPJLmode,EPJLmode, 1 ); 
//	E2P_RData( Point,Meter_Type, 4 ); 
//	if( (*(Point+1) & 0x80 ) == 0 )	Para.RPJLmode = 0;
//	else Para.RPJLmode =1;
	
//	E2P_RData( (unsigned char*)&Para.RPrgDly,EPrgDly, 2 );              //�¹���
//	Para.RPrgDly = BCD2_Word( (unsigned char*)&Para.RPrgDly ); 
//	if( Para.RPrgDly == 0 ) Para.RPrgDly = 20;
//	if( SM.PrgDly > Para.RPrgDly ) SM.PrgDly = Para.RPrgDly;            //�¹���

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	E2P_RData( (unsigned char*)&Para.RCuRevPhaseDisFlag, ECuRevPhaseDisFlag, 1 );      							//�¹���	//14.09.03
	if(( Flag.BatState & F_E2PCheck ) != 0 ) 							//�¹���	//13.10.15
	{																	//�¹���	//13.10.15
		Para.RCuRevPhaseDisFlag = 0x00;									//������ʾ		
		E2P_WData( ECuRevPhaseDisFlag, (unsigned char*)&Para.RCuRevPhaseDisFlag, 1 );      						
	}
	E2P_RData( (unsigned char*)&Para.RCommLCDDisplayTime, ECommLCDDisplayTime, 1 );      						//�¹���	//14.09.03
	if(( Flag.BatState & F_E2PCheck ) != 0 ) 							//�¹���	//13.10.15
	{																	//�¹���	//13.10.15
		Para.RCommLCDDisplayTime = 0x11;								//10��		
		E2P_WData( ECommLCDDisplayTime, (unsigned char*)&Para.RCommLCDDisplayTime, 1 );     
	}
	Para.RCommLCDDisplayTime = BCD_Byte( Para.RCommLCDDisplayTime );
#endif

#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
	E2P_RData( Ptr,ERelayPulseWide, 2 );      							//�¹���	//13.10.15
	if(( Flag.BatState & F_E2PCheck ) != 0 ) 							//�¹���	//13.10.15
	{																	//�¹���	//13.10.15
		SM.RRelayPulseWide = 26;										//�¹���	//13.10.15	//26*15.625=406ms
		* Ptr = 0x00; 													//�¹���	//13.10.15	
		*(Ptr + 1) = 0x04; 												//�¹���	//13.10.15
		E2P_WData( ERelayPulseWide, Ptr, 2 );      						//�¹���	//13.10.15	//Լ400ms
	}																	//�¹���	//13.10.15	
	else																//�¹���	//13.10.15
	{																	//�¹���	//13.10.15	
		SM.RRelayPulseWide = BCD2_Word( Ptr ); 							//�¹���	//13.10.15	
		Value = SM.RRelayPulseWide;										//�¹���	//13.10.15			
		Value *= 64;													//�¹���	//13.10.15			
		SM.RRelayPulseWide = Value / 1000;								//�¹���	//13.10.15		
		if(( Value % 1000 ) != 0 ) SM.RRelayPulseWide += 1; 			//�¹���	//13.10.15		
	}																	//�¹���	//13.10.15		
#endif

	E2P_RData( (unsigned char*)&Para.RActiveMode,ActiveMode, 1 ); 
	Para.RActiveModeChk = Para.RActiveMode + 0xA5;
//	E2P_RData( (unsigned char*)&Para.RQJLmode,EQJLmode, 1 ); 
	E2P_RData( (unsigned char*)&Para.RReActiveMode1,ReActiveMode1, 1 ); 
	Para.RReActiveMode1Chk = Para.RReActiveMode1 + 0xA5;
	E2P_RData( (unsigned char*)&Para.RReActiveMode2,ReActiveMode2, 1 ); 
	Para.RReActiveMode2Chk = Para.RReActiveMode2 + 0xA5;

	E2P_RData( (unsigned char*)&Para.RPJLmode,ECJLmode, 1 );				//10.07.06
	E2P_RData( (unsigned char*)&Para.RFeeMode,EFeeMode, 1 );		//10.07.29		
	if(( Para.RFeeMode != 1 )&&( Para.RFeeMode != 0 ))
	{
		Para.RFeeMode = 0;	
		E2P_WData( EFeeMode, (unsigned char*)&Para.RFeeMode, 1 );	//10.07.29		
	}		

	E2P_RData( Ptr, EProductState, 2 ); 												//�¹���	//14.04.09
	if(( *Ptr == 0x5A )&&( *(Ptr+1) == 0xA5 )) SM.FactoryState = 0x55;	//����״̬		//�¹���	//14.04.09	
	else SM.FactoryState = 0x00;										//�ǳ���״̬	//�¹���	//14.04.09	
	
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
//	E2P_RData( (unsigned char*)&Flag.RSwitchOldState, ESwitchOldState, 1 );				//�¹���	//14.05.03	
//	E2P_RFM( (unsigned char*)&Flag.EventUpState, FEventUpState, 12 );					//�¹���	//14.05.08		
	E2P_RData( Ptr, ESwitchOldState, 1 );				//�¹���	//14.05.03	//15.01.30
	if(( Flag.BatState & F_E2PCheck ) == 0 ) Flag.RSwitchOldState = *Ptr;		//15.01.30
	E2P_RFM( Ptr,FEventUpState, 12 ); 			//�¹���	//14.04.26			//15.01.30
	if(( Flag.BatState & F_E2PCheck ) == 0 )									//15.01.30
	{
//����ͣ�����Ƿѹ���󱨴洢������		//16.06.03
//		if(((( Flag.EventUpState[0] & 0x20 )!= 0 )&&(( *Ptr & 0x20 ) == 0 ))||((( Flag.EventUpState[0] & 0x04 )!= 0 )&&(( *Ptr & 0x04 ) == 0 )))
//		{
//			if(( Flag.EventUpState[0] & 0x20 )!= 0 ) *Ptr |= 0x20;	
//			if(( Flag.EventUpState[0] & 0x04 )!= 0 ) *Ptr |= 0x04;	
//			E2P_WFM( FEventUpState, Ptr, 12 ); 			
//		}	
		RAM_Write( (unsigned char*)&Flag.EventUpState, Ptr, 12 );	
	}	

//	MatchEventUpMode();																	//�¹���	//14.04.28		//14.06.21
	
	E2P_RData( Ptr, EBatState, 1 );														//�¹���	//14.04.28	
	if(( Flag.BatState & F_E2PCheck ) != 0 )		//16.05.09								
	{												//16.05.09	
		*Ptr = Para.RBatState;						//16.05.09
	}												//16.05.09		
	*Ptr &= F_ClockBat + F_CommBat;														//�¹���	//14.04.28	
	Flag.MeterState1 &= ~(F_ClockBat + F_CommBat);										//�¹���	//14.04.28	
	Flag.MeterState1 |= *Ptr;															//�¹���	//14.04.28	
#endif

}

void InitPara5(void)			
{
//	unsigned char Buff[14];
//	unsigned char* Point;
	
//	Point = Buff;

	E2P_RData( (unsigned char*)&Comm.BandRate1,RS485BaudRate1, 1 ); 
	E2P_RData( (unsigned char*)&Comm.BandRate2,RS485BaudRate2, 1 ); 
//	E2P_RData( (unsigned char*)&Comm.BandRate3,RS485BaudRate3, 1 ); 

	if( JustBandRate( Comm.BandRate1 ) != 0 ) 
	{
		Comm.BandRate1 = Baut2400;
		E2P_WData( RS485BaudRate1, (unsigned char*)&Comm.BandRate1,1 ); 		
	}	
	if( JustBandRate( Comm.BandRate2 ) != 0 )
	{
#if ( CarryComm == YesCheck )			//���ز�ͨ����485��2����ͨѶ����
		Comm.BandRate2 = Baut9600;
#else
		Comm.BandRate2 = Baut2400;
#endif
		E2P_WData( RS485BaudRate2, (unsigned char*)&Comm.BandRate2,1 ); 				
	}	
//#if ( CarryComm == YesCheck )															//�¹���	
//	if(( JustBandRate( Comm.BandRate3 ) != 0 )||( Comm.BandRate3 != Baut2400 ))			//�ز��̶�Ϊ2400������
//#else
//	if( JustBandRate( Comm.BandRate3 ) != 0 )
//#endif		
//	{
//		Comm.BandRate3 = Baut9600;
//		E2P_WData( RS485BaudRate3, (unsigned char*)&Comm.BandRate3,1 ); 				
//	}	

	E2P_RData( (unsigned char*)&Para.RMaxT,EMaxT, 1 ); 
	E2P_RData( (unsigned char*)&Para.RMaxDT,EMaxDT, 1 ); 
//	Para.RMaxT = BCD_Byte(Para.RMaxT);				//16.09.23
//	Para.RMaxDT = BCD_Byte(Para.RMaxDT);			//16.09.23
	if(( Para.RMaxT % Para.RMaxDT ) != 0 )
	{
//		Para.RMaxT = 0x15;
		Para.RMaxT = 15;
		Para.RMaxDT = 1;
		E2P_WData( EMaxT,(unsigned char*)&Para.RMaxT, 1 ); 
		E2P_WData( EMaxDT,(unsigned char*)&Para.RMaxDT, 1 ); 			
		Para.RMaxT = 15;
	}	
//	Para.RMaxDT = 1;		//Test
//	Para.RMaxT = 15;		//Test
	MD.MDPCT = Para.RMaxT / Para.RMaxDT;
	MD.MD_60T = 60 / Para.RMaxT;
//	E2P_RData( (unsigned char*)&Para.RDspInv,EDspInv, 1 );
//	if(( Para.RDspInv == 0 )||( IsBCD( Para.RDspInv ) != 0 )) Para.RDspInv = 6;		//10.10.06
//	Para.RDspInv = BCD_Byte(Para.RDspInv); 
	E2P_RData( (unsigned char*)&Para.RDspInv,Alternate_display_capture_period, 1 );

	E2P_RData( (unsigned char*)&Disk.AutoItemSum,AutoDisItemSum, 1 );
//	Disk.AutoItemSum = BCD_Byte( Disk.AutoItemSum ); 
	E2P_RData( (unsigned char*)&Disk.KeyItemSum,KeyDisItemSum, 1 );
//	Disk.KeyItemSum = BCD_Byte( Disk.KeyItemSum ); 

	E2P_RData( (unsigned char*)&Disk.RHighZeroFlag,EHighZeroFlag, 1 );   		//�¹���	//14.01.13
	if(( Flag.BatState & F_E2PCheck ) != 0 ) 									//�¹���	//13.10.15
	{																			//�¹���	//13.10.15
//		Disk.RHighZeroFlag = 0x55;												//�¹���	//13.10.15		
		Disk.RHighZeroFlag = 0x01;												//�¹���	//13.10.15	//16.06.07	
		E2P_WData( EHighZeroFlag, (unsigned char*)&Disk.RHighZeroFlag, 1 );   	//�¹���	//14.01.13
	}																			//�¹���	//13.10.15		
	
//	E2P_RData( (unsigned char*)&SM.RECMode,ECMode, 1 ); 

//	E2P_RData( (unsigned char*)&Para.Ue,EUe, 2 ); 
	E2P_RData( (unsigned char*)&Para.RTu,ETu, 1 ); 
	Para.RTu = BCD_Byte(Para.RTu);
//	E2P_RData( (unsigned char*)&Para.I0,EI0, 2 ); 

//#if ( MEASCHIP == ADE7758 )
//	E2P_RAdj( Point,zdydydlcs, 13 ); 	
//	RAM_Write( (unsigned char*)&Para.RefUa, Point, 12 );
//#else
//#endif

	Disk.HWBGDisTime = (unsigned short)Para.RDspInv * Disk.AutoItemSum * 2;
	Disk.HWBatOnTime = Disk.HWBGDisTime / 2;

//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T��7022�ڲ��¶ȴ�����		//10.08.03		//�¹���
	E2P_RData( (unsigned char*)&SM.RTempDelta,ETempDelta, 2 ); 
//#endif

	E2P_RData( (unsigned char*)&Para.RSafeModeSelect, SafeModeSelect, 1 );   			//17.04.07
	if((( Flag.BatState & F_E2PCheck ) != 0 )||( Para.RSafeModeSelect > 1 )) 			//17.04.07					
	{																			
		Para.RSafeModeSelect = 0x01;													
		E2P_WData( SafeModeSelect, (unsigned char*)&Para.RSafeModeSelect, 1 );   	
	}																			

	E2P_RData( (unsigned char*)&Para.RSafeModeParaSum, SafeModeParaSum, 1 );   			//17.04.07
	if((( Flag.BatState & F_E2PCheck ) != 0 )||( Para.RSafeModeParaSum > 100 )) 		//17.04.07						
	{																			
		Para.RSafeModeParaSum = 0x00;													
		E2P_WData( SafeModeParaSum, (unsigned char*)&Para.RSafeModeParaSum, 1 );   	
	}																			

#if ( CarryComm == YesCheck )			//�ز�ͨ��
	E2P_RData( (unsigned char*)&Para.RNewReportEventCarryRecs, NewReportEventL2Recs, 1 );   //17.05.20
#else
	Para.RNewReportEventCarryRecs = 0;		//17.05.20
#endif
}

void InitPara6(void)			
{
	GetDaySeg();
	GetFeeNo();
}

//��ȡ����ں���״̬
void GetPrivateKeyFlag( void )											//�¹���	
{
//	if( GetTrueBitSum_32Bit( Flag.SecretKeyState, Para.RSecretKeySum ) == Para.RSecretKeySum ) 
	if( IsAllData( (unsigned char*)&Flag.SecretKeyState, 4, 0x00 ) != 0 )		//��ȫ�㼴˽Կ
	{
		ICComm.RPrivateKeyFlag = 0;		//˽Կ
#if ( RemoteEnterCalib == YesCheck )								//�¹���	//14.05.23
		SM.RemoteCalibState = 0;
#endif
	}	
	else ICComm.RPrivateKeyFlag = 0x55;				//��Կ
}

//10.07.30
//��ȡ����ں���״̬
//void GetPrivateKeyFlag( void )										//�¹���			
//{
//	unsigned char Buff[4];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;
//	
//	E2P_RData( (unsigned char*)&ICComm.RPrivateKeyFlag, PrivateKeyFlag, 1 );
//	E2P_RData( Ptr, PassWord4State, 4 );													//Զ����Կ�޸�״̬
////	if(( *Ptr == 0x01 )||( *(Ptr+1) == 0x01 )||( *(Ptr+2) == 0x01 )||( *(Ptr+3) == 0x01 ))	//��һ��Ϊ˽Կ���������˽Կ״̬��
//	if( IsAllData( Ptr, 4, 0x01 ) == 0 )													//ȫ��Ϊ˽Կ���������˽Կ״̬��
//	{
//		if( ICComm.RPrivateKeyFlag != 0 )													//ԭ�����ڹ�Կ״̬
//		{
//			ICComm.RPrivateKeyFlag = 0;	
//			E2P_WData( PrivateKeyFlag, (unsigned char*)&ICComm.RPrivateKeyFlag, 1 );
//		}	
//	}																						//	
//	else																					//����һ��Ϊ��Կ�����빫Կ״̬				//12.10.05
////	if( IsAllData( Ptr, 4, 0x00 ) == 0 )													//ȫ��Ϊ��Կ״̬��������빫Կ״̬��
//	{
//		if( ICComm.RPrivateKeyFlag == 0 )													//ԭ������˽Կ״̬
//		{
//			ICComm.RPrivateKeyFlag = 0x55;	
//			E2P_WData( PrivateKeyFlag, (unsigned char*)&ICComm.RPrivateKeyFlag, 1 );
//		}	
//	}	
//}	
//
////State:��Կ״̬ 0����Կ 1��˽Կ
////Type: 1:�������� 2��������Կ 3�������֤ 4��������Կ
//void SetPassWState( unsigned char State, unsigned char Type )			//�¹���
//{
//	unsigned char Buff[6];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;
//	
//	if(( Type == 0 )||( Type > 4 )) return;
//	E2P_RData( Ptr, PassWord4State, 4 );													//Զ����Կ�޸�״̬
//	if( *(Ptr+Type-1) != State )
//	{
//		*(Ptr+Type-1) = State;
//		E2P_WData( PassWord4State, Ptr, 4 );												//Զ����Կ�޸�״̬
//	}	
//}	
//
////�����Կ״̬����Կ״̬
//void ClrPrivateKeyFlag( void )										//�¹���
//{
//	unsigned char Buff[4];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;
//
//	RAM_Fill( Ptr, 4 );
//	E2P_WData( PassWord4State, Ptr, 4 );	
//	ICComm.RPrivateKeyFlag = 0x55;	
//	E2P_WData( PrivateKeyFlag, (unsigned char*)&ICComm.RPrivateKeyFlag, 1 );
//}	

void InitPara7(void)			
{
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
	unsigned char Buff[14];
	unsigned char* Point;
	
	Point = Buff;
#endif

	E2P_RData( (unsigned char*)&Para.RSecretKeySum, ESecretKeySum, 1 ); 					//�¹���
//	Para.RSecretKeySum = BCD_Byte(Para.RSecretKeySum);										//�¹���	
	if( Para.RSecretKeySum > 32 )
	{
//		Para.RSecretKeySum = 1;
		Para.RSecretKeySum = 20;															//�¹���	//13.12.05	
		E2P_WData( ESecretKeySum, (unsigned char*)&Para.RSecretKeySum, 1 ); 				//�¹���
	}	
		
//	E2P_RData( (unsigned char*)SM.RUpdatedKeyState, EUpdatedKeyState, 4 ); 					//�¹���
//	if( GetTrueBitSum_32Bit( SM.RUpdatedKeyState, 32 ) > Para.RSecretKeySum )
//	{
//		RAM_Fill( SM.RUpdatedKeyState, 4 );
//		E2P_WData( EUpdatedKeyState, (unsigned char*)&SM.RUpdatedKeyState, 4 ); 			//�¹���		
//	}	

	E2P_RData( (unsigned char*)&Flag.SecretKeyState, ESecretKeyState, 4 ); 					//�¹���
//	E2P_RData( (unsigned char*)&SM.RUpdatedKeyPermit, EUpdatedKeyPermit, 1 ); 				//�¹���

	GetPrivateKeyFlag();							//10.07.29

#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
//	E2P_RData( (unsigned char*)&Para.RMeterTripState,MeterTripState, 1 );	
	E2P_RData( Point,MeterTripState, 1 );									//15.01.30		
	if(( Flag.BatState & F_E2PCheck ) == 0 ) Para.RMeterTripState = *Point;	//15.01.30
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	if(( ICComm.PreTripOFFKeepTime == 0 )&&(( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E )))	//�¹���	//14.04.29
	{
		if( Para.RMeterTripState == 0x1D ) 														//ֱ�Ӻ�բ
		{
			if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//������բ
		}
		else
		{
#if ( RelayONImmediately == YesCheck )
			if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//������բ
#else
			if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//�����բ
#endif
		}	
		ICComm.RelayOFFState = 0x00;				//�յ�Զ��ֱ�Ӻ�բ�����ȡ��������բ״̬	//�¹���	//13.10.25
		ICComm.TripOFFDelay = 0;	
		Para.RMeterTripState = 0x1B;											
		E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );	
	}

	E2P_RData( Comm.TripOFFOperateCode, ETripOFFOperateCode, 4 );	//�¹���	//14.05.12
	E2P_RData( Comm.TripONOperateCode, ETripONOperateCode, 4 );		//�¹���	//14.05.12
#endif
//	E2P_RData( (unsigned char*)&Para.RMeterAlarmState,MeterAlarmState, 1 );
//	E2P_RData( (unsigned char*)&Para.RMeterSafeState,MeterSafeState, 1 );
//	E2P_RData( (unsigned char*)&Para.RMeterRelayState,MeterRelayState, 1 );
	E2P_RData( Point,MeterAlarmState, 1 );										//15.01.30
	if(( Flag.BatState & F_E2PCheck ) == 0 ) Para.RMeterAlarmState = *Point;	//15.01.30
	E2P_RData( Point,MeterSafeState, 1 );										//15.01.30
	if(( Flag.BatState & F_E2PCheck ) == 0 ) Para.RMeterSafeState = *Point;		//15.01.30
	E2P_RData( Point,MeterRelayState, 1 );										//15.01.30	
	if(( Flag.BatState & F_E2PCheck ) == 0 ) Para.RMeterRelayState = *Point;	//15.01.30

//	E2P_RData( Point, TripOFFAlarmTime, 2 );
//	Para.RTripOFFAlarmTime = BCD2_Word( Point );
	E2P_RData( Point, TripOFFAlarmTime, 1 );
	Para.RTripOFFAlarmTime = *Point;
	Para.RTripOFFAlarmTime *= 60;
#endif																//�¹���	//13.11.06

	E2P_RData( (unsigned char* )&ICEC.IdentityState, EIdentityState, 1 );
#if ( LocalSmartMeter == YesCheck )
	if((Flag.Power & F_PwrUp) != 0) SM.StepNo = GetCurrentStepNo( 0 ); 

	E2P_RData( Point, EICInState, 1 );
	if((*Point & F_BuyEC ) != 0 ) ICComm.ICInState |= F_BuyEC;
	else ICComm.ICInState &= ~F_BuyEC;
	if((*Point & F_BuyECOver ) != 0 ) ICComm.ICInState |= F_BuyECOver;	//11.06.27
	else ICComm.ICInState &= ~F_BuyECOver;								//11.06.27

	E2P_RData( (unsigned char*)&Para.RPreTripState,EPreTripState, 1 );	//11.08.19

	E2P_RData( (unsigned char*)&ICComm.RJudgePermitFlag, JudgePermitFlag, 1 );				//�¹���	//13.12.10
	if( ICComm.RJudgePermitFlag != 0x55 )
	{
		ICComm.RJudgePermitFlag = 0x00;														//�¹���	//14.01.09	
		E2P_WData( JudgePermitFlag, (unsigned char*)&ICComm.RJudgePermitFlag, 1 );			//�¹���	//14.01.09
	}	
	E2P_RData( (unsigned char*)&ICComm.ROpenAccountFlag, OpenAccountFlag, 1 );				//�¹���	//13.12.05

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
	if(( Flag.Power & F_PwrUp ) != 0 )
	{
		E2P_RData( Comm.TripOFFOperateCode, ETripOFFOperateCode, 4 );	//�¹���	//14.05.12
		E2P_RData( Comm.TripONOperateCode, ETripONOperateCode, 4 );		//�¹���	//14.05.12
		if(( ICComm.PreTripOFFKeepTime == 0 )&&(( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E )))	//�¹���	//14.04.29
		{
			if( Para.RMeterTripState == 0x1D ) *Point = 1;
			else *Point = 0;
			ICComm.RelayOFFState = 0x00;				//�յ�Զ��ֱ�Ӻ�բ�����ȡ��������բ״̬	//�¹���	//13.10.25
			ICComm.TripOFFDelay = 0;	
			Para.RMeterTripState = 0x1B;											
			E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );	
			if( *Point == 1 ) 
			{
				KeyRelayOn();
				SM.CarrySTADelay = 0;					//ȡ���ز����ͽ�ֹ��բ���ƣ������ֹ		//�¹���	//14.05.10
				OperateRelay();
			}	
		}
	}	
#endif
#else
//	if((Flag.Power & F_PwrUp) != 0) SM.StepNo = GetCurrentStepNo( 0 ); 
	SM.StepNo = 0;																			//�¹���	//13.09.21		
#endif
}


/*
void GetSumVolt( void )							//�ۼ�ÿ���ܵ�ѹ			
{
	short i;
	unsigned short Temp;

	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )	
		if( i == 1 ) continue;
#endif
		Temp = BCD2_Word( Real.Ua + i*2 );	
		VPara.SumVolt += (unsigned long)Temp; 		//1���������ѹ�ۼӺ�
	}		
	VPara.SumVoltA += BCD2_Word( Real.Ua ); 		//1����A���ѹ�ۼӺ�
#if( LinkMode == Phase3Wire3 )
#else	
	VPara.SumVoltB += BCD2_Word( Real.Ub ); 		//1����B���ѹ�ۼӺ�
#endif	
	VPara.SumVoltC += BCD2_Word( Real.Uc ); 		//1����C���ѹ�ۼӺ�
	VPara.VoltSecCnt++;
	VPara.VoltSecCntA++;
	VPara.VoltSecCntB++;
	VPara.VoltSecCntC++;
}

void GetAverageVolt( void )				//����ÿ����ƽ����ѹ			
{
	RAM_Fill( VPara.AverageVolt, 8 );
	if( VPara.VoltSecCnt != 0 ) 
	{
#if( LinkMode == Phase3Wire3 )	
		Word_BCD( VPara.AverageVolt, (unsigned short)((VPara.SumVolt/2)/VPara.VoltSecCnt) );
#else
		Word_BCD( VPara.AverageVolt, (unsigned short)((VPara.SumVolt/3)/VPara.VoltSecCnt) );
#endif
	}

	if( VPara.VoltSecCntA != 0 ) 
	{
		Word_BCD( VPara.AverageVoltA, (unsigned short)(VPara.SumVoltA/VPara.VoltSecCntA) );
	}	
	
	if( VPara.VoltSecCntB != 0 ) 
	{
#if( LinkMode == Phase3Wire4 )	
		Word_BCD( VPara.AverageVoltB, (unsigned short)(VPara.SumVoltB/VPara.VoltSecCntB) );
#endif
	}

	if( VPara.VoltSecCntC != 0 ) 
	{
		Word_BCD( VPara.AverageVoltC, (unsigned short)(VPara.SumVoltC/VPara.VoltSecCntC) );
	}
	
	VPara.SumVolt = 0;
	VPara.SumVoltA = 0;
	VPara.SumVoltB = 0;
	VPara.SumVoltC = 0;
	RAM_Fill( &VPara.VoltSecCnt, 4 );
}		

//void GetQualifiyRate( void )
//{
//	unsigned short Temp;
//	unsigned long Value1,Value2,Value3;
//	short i;
//	VOLT_CHECK_REC*	VoltStPtr; 
//	unsigned char* AverageVolt;
//		
//	GetAverageVolt();
//
//	for( i=0;i<4;i++ )
//	{
//		VoltStPtr = VoltCheckPara[i].VoltChk;
//		AverageVolt = VoltCheckPara[i].AverVolt;
//		if( Data_Comp( AverageVolt, Para.RExamVoltUpperLimit, 2) >= 0  ) continue;		//��ѹ��������(2)			yyy.yV
//		if( Data_Comp( AverageVolt, Para.RExamVoltLowerLimit, 2) < 0  ) continue;		//��ѹ��������(2)			yyy.yV
//		if( Data_Comp( AverageVolt, Para.RVoltUpperLimit, 2) >= 0  ) _BCD3INC( VoltStPtr->UpperLimitTime );		
//		if( Data_Comp( AverageVolt, Para.RVoltLowerLimit, 2) < 0  ) _BCD3INC( VoltStPtr->LowerLimitTime );		
//		_BCD3INC( VoltStPtr->CheckTime );
//		if( Data_Comp( AverageVolt, VoltStPtr->MaxVolt, 2) > 0  )
//		{
//			RAM_Write( VoltStPtr->MaxVolt, AverageVolt, 2 );
//			RAM_Write( VoltStPtr->MaxVoltHpTime, Clk.Min, 4 );		
//		}
//		if(( IsAllData( VoltStPtr->LeastVolt, 2, 0x00 ) == 0 )
//			||( Data_Comp( AverageVolt, VoltStPtr->LeastVolt, 2) < 0 )) 
//		{
//			RAM_Write( VoltStPtr->LeastVolt, AverageVolt, 2 );
//			RAM_Write( VoltStPtr->LeastVoltHpTime, Clk.Min, 4 );				
//		}	
//		Value1 = BCD3_Long( VoltStPtr->UpperLimitTime );
//		Value2 = BCD3_Long( VoltStPtr->LowerLimitTime );
//		Value3 = BCD3_Long( VoltStPtr->CheckTime );
//		Temp = ((Value1+Value2)*10000)/Value3;
//		if( Temp == 10000 )
//		{			
//			VoltStPtr->AbnormalRate[0] = 0x00;	
//			VoltStPtr->AbnormalRate[1] = 0x00;	
//			VoltStPtr->AbnormalRate[2] = 0x01;	
//			RAM_Fill( VoltStPtr->NormalRate, 3 );
//		}	
//		else if( Temp == 0 )
//		{
//			VoltStPtr->NormalRate[0] = 0x00;	
//			VoltStPtr->NormalRate[1] = 0x00;	
//			VoltStPtr->NormalRate[2] = 0x01;	
//			RAM_Fill( VoltStPtr->AbnormalRate, 3 );
//		}	
//		else
//		{
//			Word_BCD( VoltStPtr->AbnormalRate, Temp );
//			VoltStPtr->AbnormalRate[2] = 0x00;
//			Word_BCD( VoltStPtr->NormalRate, 10000 - Temp );
//			VoltStPtr->NormalRate[2] = 0x00;
//		}
//		E2P_WFM( VoltCheckPara[i].CurMonAddr, VoltStPtr->CheckTime, 27 );	 //���ѹ�ϸ�������		
//	}	
//}	

void GetTotalQualifiyRate( void )			//�¹���
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned long Value1,Value2,Value3;
	unsigned long VoltNormalTime;
	short i,j;
	VOLT_CHECK_REC*	VoltStPtr; 
	
	Ptr = Buff;
	
	VoltStPtr = VoltCheckPara[0].VoltChk;
	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( Data_Comp( VoltCheckPara[i+1].AverVolt, Para.RExamVoltUpperLimit, 2) >= 0  ) break;		//��ѹ��������(2)			yyy.yV
	}	
	if( i != 3 ) return;		//����һ�೬��ѹ��������

	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( Data_Comp( VoltCheckPara[i+1].AverVolt, Para.RExamVoltLowerLimit, 2) < 0  ) break;		//��ѹ��������(2)			yyy.yV
	}	
	if( i != 3 ) return;		//����һ�೬��ѹ��������
	
	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( Data_Comp( VoltCheckPara[i+1].AverVolt, Para.RVoltUpperLimit, 2) >= 0  ) 
		{
			_BCD3INC( VoltStPtr->UpperLimitTime );	
			break;				//����һ�೬��ѹ����
		}	
	}	

	for( j=0;j<3;j++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( j == 1 ) continue;
#endif
		if( Data_Comp( VoltCheckPara[j+1].AverVolt, Para.RVoltLowerLimit, 2) < 0  ) 
		{
			_BCD3INC( VoltStPtr->LowerLimitTime );	
			break;				//����һ�೬��ѹ����
		}	
	}	
	
	E2P_RFM( Ptr, FVoltNormalTime, 3 );
	VoltNormalTime = BCD3_Long( Ptr );
	if(( i == 3 )&&( j == 3 ))	//���඼�ںϸ��ѹ��Χ��
	{
		VoltNormalTime += 1;
		Long_BCD3( Ptr, VoltNormalTime );
		E2P_WFM( FVoltNormalTime, Ptr, 3 );
	}	
	
	_BCD3INC( VoltStPtr->CheckTime );

	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( Data_Comp( VoltCheckPara[i+1].AverVolt, VoltStPtr->MaxVolt, 2) > 0 )
		{
			RAM_Write( VoltStPtr->MaxVolt, VoltCheckPara[i+1].AverVolt, 2 );
			RAM_Write( VoltStPtr->MaxVoltHpTime, Clk.Min, 4 );		
		}
    }

	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if(( IsAllData( VoltStPtr->LeastVolt, 2, 0x00 ) == 0 )
			||( Data_Comp( VoltCheckPara[i+1].AverVolt, VoltStPtr->LeastVolt, 2) < 0 )) 
		{
			RAM_Write( VoltStPtr->LeastVolt, VoltCheckPara[i+1].AverVolt, 2 );
			RAM_Write( VoltStPtr->LeastVoltHpTime, Clk.Min, 4 );				
		}	
	}
	
	Value3 = BCD3_Long( VoltStPtr->CheckTime );
	if( VoltNormalTime > Value3 ) VoltNormalTime = Value3;
	Value1 = (VoltNormalTime*10000)/Value3;
	Value2 = 10000 - Value1;
	Long_BCD3( VoltStPtr->NormalRate, Value1 );
	Long_BCD3( VoltStPtr->AbnormalRate, Value2 );
	
	E2P_WFM( VoltCheckPara[0].CurMonAddr, VoltStPtr->CheckTime, 27 );	 //���ѹ�ϸ�������		

}	

void GetQualifiyRate( void )			
{
	unsigned short Temp;
	unsigned long Value1,Value2,Value3;
	short i;
	VOLT_CHECK_REC*	VoltStPtr; 
	unsigned char* AverageVolt;
		
	GetAverageVolt();

	GetTotalQualifiyRate();				//�¹���
	
//	for( i=0;i<4;i++ )
	for( i=1;i<4;i++ )					//�¹���
	{
		VoltStPtr = VoltCheckPara[i].VoltChk;
		AverageVolt = VoltCheckPara[i].AverVolt;
		if( Data_Comp( AverageVolt, Para.RExamVoltUpperLimit, 2) >= 0  ) continue;		//��ѹ��������(2)			yyy.yV
		if( Data_Comp( AverageVolt, Para.RExamVoltLowerLimit, 2) < 0  ) continue;		//��ѹ��������(2)			yyy.yV
		if( Data_Comp( AverageVolt, Para.RVoltUpperLimit, 2) >= 0  ) _BCD3INC( VoltStPtr->UpperLimitTime );		
		if( Data_Comp( AverageVolt, Para.RVoltLowerLimit, 2) < 0  ) _BCD3INC( VoltStPtr->LowerLimitTime );		
		_BCD3INC( VoltStPtr->CheckTime );
		if( Data_Comp( AverageVolt, VoltStPtr->MaxVolt, 2) > 0  )
		{
			RAM_Write( VoltStPtr->MaxVolt, AverageVolt, 2 );
			RAM_Write( VoltStPtr->MaxVoltHpTime, Clk.Min, 4 );		
		}
		if(( IsAllData( VoltStPtr->LeastVolt, 2, 0x00 ) == 0 )
			||( Data_Comp( AverageVolt, VoltStPtr->LeastVolt, 2) < 0 )) 
		{
			RAM_Write( VoltStPtr->LeastVolt, AverageVolt, 2 );
			RAM_Write( VoltStPtr->LeastVoltHpTime, Clk.Min, 4 );				
		}	
		Value1 = BCD3_Long( VoltStPtr->UpperLimitTime );
		Value2 = BCD3_Long( VoltStPtr->LowerLimitTime );
		Value3 = BCD3_Long( VoltStPtr->CheckTime );
		Temp = ((Value1+Value2)*10000)/Value3;
		if( Temp == 10000 )
		{			
			VoltStPtr->AbnormalRate[0] = 0x00;	
			VoltStPtr->AbnormalRate[1] = 0x00;	
			VoltStPtr->AbnormalRate[2] = 0x01;	
			RAM_Fill( VoltStPtr->NormalRate, 3 );
		}	
		else if( Temp == 0 )
		{
			VoltStPtr->NormalRate[0] = 0x00;	
			VoltStPtr->NormalRate[1] = 0x00;	
			VoltStPtr->NormalRate[2] = 0x01;	
			RAM_Fill( VoltStPtr->AbnormalRate, 3 );
		}	
		else
		{
			Word_BCD( VoltStPtr->AbnormalRate, Temp );
			VoltStPtr->AbnormalRate[2] = 0x00;
			Word_BCD( VoltStPtr->NormalRate, 10000 - Temp );
			VoltStPtr->NormalRate[2] = 0x00;
		}
		E2P_WFM( VoltCheckPara[i].CurMonAddr, VoltStPtr->CheckTime, 27 );	 //���ѹ�ϸ�������		
	}	
}	

void InitQualifiyRateData( short PhaseNo )
{
	unsigned char* Ptr;											//�¹���

	Ptr = VoltCheckPara[PhaseNo].VoltChk->CheckTime;			//�¹���

	*VoltCheckPara[PhaseNo].SumV = 0;
	RAM_Fill( VoltCheckPara[PhaseNo].AverVolt, 2 );
	*VoltCheckPara[PhaseNo].SecCnt = 0;
//	RAM_Fill( VoltCheckPara[PhaseNo].VoltChk->CheckTime, 27 );
	RAM_Fill( Ptr, 27 );
//	E2P_WFM( VoltCheckPara[PhaseNo].CurMonAddr, VoltCheckPara[PhaseNo].VoltChk->CheckTime, 27 );	 //���ѹ�ϸ�������		
	E2P_WFM( VoltCheckPara[PhaseNo].CurMonAddr, Ptr, 27 );	 //���ѹ�ϸ�������	
	if( PhaseNo == 0 ) E2P_WFM( FVoltNormalTime, Ptr, 3 );	 //�屾�µ�ѹ�ϸ�ʱ��		//�¹���	
}

void InitAllQualifiyRateData( void )
{
	short i;
	
	for( i=0;i<4;i++ )
	{
		InitQualifiyRateData( i );
	}	
}	

*/

void GetSumVolt( void )						//�ۼ�ÿ���ܵ�ѹ			
{
//	short i;
//	unsigned short Temp;

//	for( i=0;i<3;i++ )
//	{
//#if( LinkMode == Phase3Wire3 )	
//		if( i == 1 ) continue;
//#endif
//		Temp = BCD2_Word( Real.Ua + i*2 );	
//		VPara.SumVolt += (unsigned long)Temp; 		//1���������ѹ�ۼӺ�
//	}		
	VPara.SumVoltA += BCD2_Word( Real.Ua ); 		//1����A���ѹ�ۼӺ�
#if( LinkMode == Phase3Wire3 )
#else	
	VPara.SumVoltB += BCD2_Word( Real.Ub ); 		//1����B���ѹ�ۼӺ�
#endif	
	VPara.SumVoltC += BCD2_Word( Real.Uc ); 		//1����C���ѹ�ۼӺ�
//	VPara.VoltSecCnt++;
	VPara.VoltSecCntA++;
	VPara.VoltSecCntB++;
	VPara.VoltSecCntC++;	
}	

void GetAverageVolt( void )					//����ÿ����ƽ����ѹ			
{
//	VPara.AverageVolt = 0;
	VPara.AverageVoltA = 0;
	VPara.AverageVoltB = 0;
	VPara.AverageVoltC = 0;

//	if( VPara.VoltSecCnt != 0 ) 
//	{
//#if( LinkMode == Phase3Wire3 )	
//		VPara.AverageVolt = (unsigned short)((VPara.SumVolt/2)/VPara.VoltSecCnt);
//#else
//		VPara.AverageVolt = (unsigned short)((VPara.SumVolt/3)/VPara.VoltSecCnt);
//#endif
//	}

	if( VPara.VoltSecCntA != 0 ) 
	{
		VPara.AverageVoltA = (unsigned short)(VPara.SumVoltA/VPara.VoltSecCntA);
	}	
	
	if( VPara.VoltSecCntB != 0 ) 
	{
#if( LinkMode == Phase3Wire4 )	
		VPara.AverageVoltB = (unsigned short)(VPara.SumVoltB/VPara.VoltSecCntB);
#endif
	}

	if( VPara.VoltSecCntC != 0 ) 
	{
		VPara.AverageVoltC = (unsigned short)(VPara.SumVoltC/VPara.VoltSecCntC);
	}

//	VPara.SumVolt = 0;
	VPara.SumVoltA = 0;
	VPara.SumVoltB = 0;
	VPara.SumVoltC = 0;

//	VPara.VoltSecCnt = 0;
	VPara.VoltSecCntA = 0;
	VPara.VoltSecCntB = 0;
	VPara.VoltSecCntC = 0;
}
	
/*
void GetTotalQualifiyRate( void )			//�¹���
{
	unsigned long Value1,Value2,Value3;
	unsigned long VoltNormalTime;
	short i,j;
	VOLT_CHECK_REC*	MonVoltStPtr; 
	VOLT_CHECK_REC*	DayVoltStPtr; 
	
	MonVoltStPtr = VoltCheckPara[0].MonVoltChk;
	DayVoltStPtr = VoltCheckPara[0].DayVoltChk;
	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( *VoltCheckPara[i+1].AverVolt >= (unsigned short)Para.RExamVoltUpperLimit ) break;	//��ѹ��������(2)			yyy.yV
	}	
	if( i != 3 ) return;		//����һ�೬��ѹ��������

	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( *VoltCheckPara[i+1].AverVolt < (unsigned short)Para.RExamVoltLowerLimit ) break;		//��ѹ��������(2)			yyy.yV
	}	
	if( i != 3 ) return;		//����һ�೬��ѹ��������
	
	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( i == 1 ) continue;
#endif
		if( *VoltCheckPara[i+1].AverVolt >= (unsigned short)Para.RVoltUpperLimit ) 
		{
			MonVoltStPtr->UpperLimitTime += 1;	
			DayVoltStPtr->UpperLimitTime += 1;	
			break;				//����һ�೬��ѹ����
		}	
	}	

	for( j=0;j<3;j++ )
	{
#if( LinkMode == Phase3Wire3 )			
		if( j == 1 ) continue;
#endif
		if( *VoltCheckPara[j+1].AverVolt < (unsigned short)Para.RVoltLowerLimit ) 
		{
			MonVoltStPtr->LowerLimitTime += 1;	
			DayVoltStPtr->LowerLimitTime += 1;				
			break;				//����һ�೬��ѹ����
		}	
	}	
	
//------------------------------------------------------------------------------------	��������
	E2P_RFM( (unsigned char*)&VoltNormalTime, FMonVoltNormalTime, 4 );
	if(( i == 3 )&&( j == 3 ))	//���඼�ںϸ��ѹ��Χ��
	{
		VoltNormalTime += 1;
		E2P_WFM( FMonVoltNormalTime, (unsigned char*)&VoltNormalTime, 3 );
	}	
	MonVoltStPtr->CheckTime += 1;

	Value3 = MonVoltStPtr->CheckTime;
	if( VoltNormalTime > Value3 ) VoltNormalTime = Value3;
	Value1 = (VoltNormalTime*10000)/Value3;
	Value2 = 10000 - Value1;
	MonVoltStPtr->NormalRate = Value1;
	MonVoltStPtr->AbnormalRate = Value2;
	E2P_WFM( VoltCheckPara[0].CurMonAddr, (unsigned char*)&MonVoltStPtr->CheckTime, 16 );	 //���ѹ�ϸ�������		

//------------------------------------------------------------------------------------	��������
	E2P_RFM( (unsigned char*)&VoltNormalTime, FDayVoltNormalTime, 4 );
	if(( i == 3 )&&( j == 3 ))	//���඼�ںϸ��ѹ��Χ��
	{
		VoltNormalTime += 1;
		E2P_WFM( FDayVoltNormalTime, (unsigned char*)&VoltNormalTime, 3 );
	}	
	DayVoltStPtr->CheckTime += 1;

	Value3 = DayVoltStPtr->CheckTime;
	if( VoltNormalTime > Value3 ) VoltNormalTime = Value3;
	Value1 = (VoltNormalTime*10000)/Value3;
	Value2 = 10000 - Value1;
	DayVoltStPtr->NormalRate = Value1;
	DayVoltStPtr->AbnormalRate = Value2;
	E2P_WFM( VoltCheckPara[0].CurDayAddr, (unsigned char*)&DayVoltStPtr->CheckTime, 16 );	 //���ѹ�ϸ�������		

}	
*/

void GetQualifiyRate( void )			
{
	unsigned short Temp;
	unsigned long Value1,Value2,Value3;
	short i,j;
	VOLT_CHECK_REC*	VoltStPtr; 
	unsigned short* AverageVolt;
		
	GetAverageVolt();

//	GetTotalQualifiyRate();				//�¹���
	
	for( j=0;j<2;j++ )
	{
//		for( i=1;i<4;i++ )					//�¹���
		for( i=0;i<3;i++ )					//�¹���
		{
			if( j == 0 ) VoltStPtr = VoltCheckPara[i].MonVoltChk;
			else VoltStPtr = VoltCheckPara[i].DayVoltChk;
			AverageVolt = VoltCheckPara[i].AverVolt;
			if( *AverageVolt >= Para.RExamVoltUpperLimit ) continue;		//��ѹ��������(2)			yyy.yV
			if( *AverageVolt < Para.RExamVoltLowerLimit ) continue;		//��ѹ��������(2)			yyy.yV
			if( *AverageVolt >= Para.RVoltUpperLimit ) VoltStPtr->UpperLimitTime += 1;		
			if( *AverageVolt < Para.RVoltLowerLimit ) VoltStPtr->LowerLimitTime += 1;		
			VoltStPtr->CheckTime += 1;
			Value1 = VoltStPtr->UpperLimitTime;
			Value2 = VoltStPtr->LowerLimitTime;
			Value3 = VoltStPtr->CheckTime;
			Temp = ((Value1+Value2)*10000)/Value3;
			if( Temp == 10000 )
			{			
				VoltStPtr->AbnormalRate = 10000;
				VoltStPtr->NormalRate = 0;
			}	
			else if( Temp == 0 )
			{
				VoltStPtr->AbnormalRate = 0;
				VoltStPtr->NormalRate = 10000;
			}	
			else
			{
				VoltStPtr->AbnormalRate = Temp;
				VoltStPtr->NormalRate = 10000 - Temp;
			}
			if( j == 0 ) E2P_WFM( VoltCheckPara[i].CurMonAddr, (unsigned char*)&VoltStPtr->CheckTime, 16 );		 	//���ѹ�ϸ�������		
			else E2P_WFM( VoltCheckPara[i].CurDayAddr, (unsigned char*)&VoltStPtr->CheckTime, 16 );	 				//���ѹ�ϸ�������		
		}	
	}
}	

//Type: 0:�����ݣ�1��������
void InitQualifiyRateData( short PhaseNo, short Type )
{
	unsigned short CurAddr;
	unsigned short VoltNormalTimeAddr;
	unsigned char* Ptr;											

	if( Type == 0 )																					//��		
	{
		Ptr = (unsigned char*)&VoltCheckPara[PhaseNo].MonVoltChk->CheckTime;			
		CurAddr = VoltCheckPara[PhaseNo].CurMonAddr;
		VoltNormalTimeAddr = FMonVoltNormalTime;
	}
	else																							//��
	{
		Ptr = (unsigned char*)&VoltCheckPara[PhaseNo].DayVoltChk->CheckTime;			
		CurAddr = VoltCheckPara[PhaseNo].CurDayAddr;
		VoltNormalTimeAddr = FDayVoltNormalTime;		
	}		

	*VoltCheckPara[PhaseNo].SumV = 0;
	*VoltCheckPara[PhaseNo].AverVolt = 0;
	*VoltCheckPara[PhaseNo].SecCnt = 0;

	RAM_Fill( Ptr, 16 );
	E2P_WFM( CurAddr, Ptr, 16 );	 //���ѹ�ϸ�������	
	if( PhaseNo == 0 ) E2P_WFM( VoltNormalTimeAddr, Ptr, 4 );	 //�屾�µ�ѹ�ϸ�ʱ��		
}


//����µ�ѹ�ϸ�������
void InitMonQualifiyRateData( void )
{
	short i;
	
	for( i=0;i<3;i++ )
	{
		InitQualifiyRateData( i, 0 );
	}
}	

//����յ�ѹ�ϸ�������
void InitDayQualifiyRateData( void )
{
	short i;
	
	for( i=0;i<3;i++ )
	{
		InitQualifiyRateData( i, 1 );
	}
}	


void GetSumPower( void )						//�ۼ�ÿ���ܹ���			
{
	short i;
	long* SumPower;
	unsigned char* RealPower;
	long Value;
	unsigned char Buff[4];

	for( i=0;i<12;i++ )
	{
		SumPower = PowerCheckPara[i].SumPower;
		RealPower = PowerCheckPara[i].RealPower;
		RAM_Write( Buff, RealPower, 3 );
		if(( Buff[2] & 0x80 ) != 0 )
		{
			Buff[2] &= ~0x80;
			Value = BCD3_Long( Buff );	
			*SumPower -= Value;
		}
		else 
		{
			Value = BCD3_Long( Buff );	
			*SumPower += Value;			
		}		
	}	
	SM.PowerSecCnt++;	
}	

void GetAveragePower( void )					//����ÿ����ƽ������			
{
	short i;
	long* SumPower;
	unsigned char* AverPower;
	long Value;

	if( SM.PowerSecCnt == 0 ) return;

	for( i=0;i<12;i++ )
	{
		SumPower = PowerCheckPara[i].SumPower;
		AverPower = PowerCheckPara[i].AverPower;
		Value = *SumPower;
		Value /= SM.PowerSecCnt;
		if( Value < 0 )
		{
			Value = 0 - Value;	
			Long_BCD3( AverPower, Value );
			*(AverPower+2) |= 0x80;
		}
		else 
		{
			Long_BCD3( AverPower, Value );	
		}
		*SumPower = 0;
	}
	SM.PowerSecCnt = 0;	

}

//void IncBatWorkTime( void )
//{
////	if( Real.BatWorkTimeChk == ChkNum( Real.RBat_WorkTime, 4 ) )
//	if(( Real.BatWorkTimeChk == ChkNum( Real.RBat_WorkTime, 4 ) )&&( AllIsBCD( Real.RBat_WorkTime, 4 ) == 0 ))	//10.11.05	
//	{
//		_BCD4INC( Real.RBat_WorkTime );	
////		Real.BatWorkTimeChk = ChkNum( Real.RBat_WorkTime, 4 );
//	}
//	else
//	{
//		E2P_RData( Real.RBat_WorkTime, Bat_WorkTime, 4 ); 
//		if(((Flag.BatState & F_E2PCheck) == 0 )&&( AllIsBCD( Real.RBat_WorkTime, 4 ) != 0 ))					//10.11.05	
//		{																										//10.11.05
//			RAM_Fill( Real.RBat_WorkTime, 4 );																	//10.11.05
//		}																										//10.11.05	
//	}		
//	Real.BatWorkTimeChk = ChkNum( Real.RBat_WorkTime, 4 );
//}

short JustState( unsigned char* Source, unsigned char* Dest )
{
	short i;
//	unsigned char Buff[4];
	unsigned char Buff[8];
	unsigned char* Ptr;
	
	Ptr = Buff;
//	RAM_Write( Ptr, Dest, 4 );
	RAM_Write( Ptr, Dest, 8 );
//	for( i=0;i<4;i++ )
	for( i=0;i<8;i++ )
	{
		*(Ptr+i) &= *(Source+i);			
	}	
//	if( IsAllData( Ptr, 4, 0x0 ) == 0 ) return 0;
	if( IsAllData( Ptr, 8, 0x0 ) == 0 ) return 0;
	else return 1;
	
}	

void ComparFMQAlarm( void )
{
//	if( Data_Comp( Flag.FMQOFFAlarmFlg, Flag.OLDFMQOFFAlarmFlg, 4 ) != 0 ) 
	if( Data_Comp( Flag.FMQOFFAlarmFlg, Flag.OLDFMQOFFAlarmFlg, 8 ) != 0 ) 
	{
		SM.FMQEnable = 0;	
	}	
}	

void CreatFMQOFFAlarmFlg( void )
{
	short i;

//	RAM_Write( Flag.FMQOFFAlarmFlg, Flag.AlarmFlg, 4 );
	RAM_Write( Flag.FMQOFFAlarmFlg, Flag.AlarmFlg, 8 );
//	for( i=0;i<4;i++ )
	for( i=0;i<8;i++ )
	{
		Flag.FMQOFFAlarmFlg[i] &= Para.RAlarm_Control1[i];			
	}	
//	if( Para.RMeterTripState == 0x1A ) Flag.FMQOFFAlarmFlg[3] |= 0x80;
	ComparFMQAlarm();
}	

void ComparRelayAlarm( void )					//�¹���		
{
	if( Data_Comp( Flag.RelayOFFAlarmFlg, Flag.OLDRelayOFFAlarmFlg, 8 ) != 0 ) 
	{
		SM.RelayBJEnable = 0;	
	}
	else SM.RelayBJEnable = 0xAA;				//�¹���	//13.10.19	
}	

void CreatRelayOFFAlarmFlg( void )				//�¹���
{
	short i;

	RAM_Write( Flag.RelayOFFAlarmFlg, Flag.AlarmFlg, 8 );
	for( i=0;i<8;i++ )
	{
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
		Flag.RelayOFFAlarmFlg[i] &= Para.RAlarm_Control3[i];			//�¹���	//14.06.19
#else
		Flag.RelayOFFAlarmFlg[i] &= Para.RAlarm_Control2[i];			
#endif
		Flag.OLDRelayOFFAlarmFlg[i] &= Flag.RelayOFFAlarmFlg[i];		//�¹���	//13.10.19		//�˳��¼����±������ݲ�һ��ʱ�����ٽ��м̵�������			
	}	
	ComparRelayAlarm();
}	

void Recd_ADD1( unsigned short CtAds, unsigned short DataLenth)
{
	unsigned char Buff[4];
	unsigned char* Point;
	
	Point = Buff;
	E2P_RData( Point, CtAds, DataLenth );
	switch( DataLenth )
	{
		case 2: _BCD2INC( Point );break;
		case 3: _BCD3INC( Point );break;
		case 4: _BCD4INC( Point );break;
		default: break;
	}	
	E2P_WData( CtAds, Point, DataLenth );
}

#if ( AllFail7022Chk == NoCheck )			//10.11.11
void AllFailCalibrateDelay( void )			//10.10.06
{
	unsigned char Buff[4];	
	unsigned char* Ptr;
	
	Ptr = Buff;
		
	if( SM.IbIrmsChk == ChkNum( (unsigned char*)&SM.Ib00IrmsDelay, 4 ) )
	{
		if((Flag.Power & F_PwrUp) == 0 )			//10.10.11
		{
			if( SM.Ib00IrmsDelay != 0 )
			{
				SM.Ib00IrmsDelay--;
//				if(( SM.Ib00IrmsDelay == 0 )&&((Flag.Power & F_PwrUp) == 0))
				if( SM.Ib00IrmsDelay == 0 )
				{
					E2P_RData( Ptr, EProductState, 2 ); 						
					if(( *Ptr == 0x5A )&&( *(Ptr+1) == 0xA5 ))
					{
						Flag.Power |= F_Ib00IrmsCheck;
						SM.HWCAllFailState |= 0xE0;			
					}
				}						
			}		
			if( SM.IbIrmsDelay != 0 )
			{
				SM.IbIrmsDelay--;
//				if(( SM.IbIrmsDelay == 0 )&&((Flag.Power & F_PwrUp) == 0))
				if( SM.IbIrmsDelay == 0 )
				{
					E2P_RData( Ptr, EProductState, 2 ); 						
					if(( *Ptr == 0x5A )&&( *(Ptr+1) == 0xA5 )) 
					{
						Flag.Power |= F_IbIrmsCheck;
						SM.HWCAllFailState |= 0xE0;			
					}
				}						
			}			
			if( SM.Ib10IrmsDelay != 0 )
			{
				SM.Ib10IrmsDelay--;
//				if(( SM.Ib10IrmsDelay == 0 )&&((Flag.Power & F_PwrUp) == 0))
				if( SM.Ib10IrmsDelay == 0 )
				{
					E2P_RData( Ptr, EProductState, 2 ); 						
					if(( *Ptr == 0x5A )&&( *(Ptr+1) == 0xA5 ))
					{
						Flag.Power |= F_Ib10IrmsCheck;
						SM.HWCAllFailState |= 0xE0;			
					}
				}						
			}		
			if( SM.Ib10MesuDelay != 0 )
			{
				SM.Ib10MesuDelay--;
//				if(( SM.Ib10MesuDelay == 0 )&&((Flag.Power & F_PwrUp) == 0))
				if( SM.Ib10MesuDelay == 0 )
				{
					E2P_RData( Ptr, EProductState, 2 ); 						
					if(( *Ptr == 0x5A )&&( *(Ptr+1) == 0xA5 )) 
					{
						SM.HWCAllFailState |= 0x08;			//10.10.04		
						E2P_WData( EIbMesuValue, Real.SIa, 9 );
					}	
				}						
			}			
		}
	}		
	else
	{
		RAM_Fill( (unsigned char*)&SM.Ib00IrmsDelay, 4 );
		SM.HWCAllFailState = 0;	
	}
	SM.IbIrmsChk = ChkNum( (unsigned char*)&SM.Ib00IrmsDelay, 4 );
}
#endif

#if ( NewHardScheme2 == YesCheck )		//11.04.29
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
int Un55VoltJudge( void ) 								//�¹���	//13.11.06
{
	int i;
	unsigned int Volt55Un;
	unsigned int Volt[6];
	unsigned long Value;								//�¹���	//13.08.30

#if( AuxPWFailCheck	== YesCheck )
	if(( HT_GPIOD->PTDAT & GPIOD_AUXChck ) != 0 ) return 0;		//������Դ��������
#endif

	Value = MSpec.RRatedVolt;							
#if( LinkMode == Phase3Wire3 )	
	Value *= 60;											
#else
	Value *= 55;											
#endif
	Volt55Un = Value / 100;								

	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )	
		if( i==1 ) continue;
#endif				
		Volt[i] = BCD2_Word( Real.Ua+i*2 );			
		Volt[i+3] = BCD2_Word( Real.PreUa+i*2 );			
		if(( Volt[i] >= Volt55Un )||( Volt[i+3] >= Volt55Un )) break;
	}	
	
	if( i == 3 ) return 1;
	else return 0;		//����һ�����55%Un(����)��60%Un(����)	
}	
#else																		//�¹���#if ( MagnKeepRelay == YesCheck  )
short Un68_78VoltJudge( void ) 
{
	short i;
	unsigned short Volt160Un;								//12.05.02	
	unsigned short Volt68Un;
	unsigned short Volt91Un;
	unsigned short Volt[6];
	unsigned long Value;								//�¹���	//13.08.30
	
	Value = MSpec.RRatedVolt;							//�¹���	//13.08.30
	Value *= 68;										//�¹���	//13.08.30	
	Volt68Un = Value / 100;								//�¹���	//13.08.30
	
	Value = MSpec.RRatedVolt;							//�¹���	//13.08.30	
	Value *= 91;                                        //�¹���	//13.08.30
	Volt91Un = Value / 100;                             //�¹���	//13.08.30

	Value = MSpec.RRatedVolt;							//�¹���	//13.08.30
	Value *= 160;                                       //�¹���	//13.08.30
	Volt160Un = Value / 100;                            //�¹���	//13.08.30

//														//�¹���	//13.08.30		
//#if ( V10Un	== 22 )	
//	Volt68Un = 1490;		//149.0V
////	Volt78Un = 1710;		//171.0V
//	Volt91Un = 2000;		//200.0V
//	Volt160Un = 3500;		//350.0V					//12.05.02
//#elif ( V10Un == 10 )	
//	Volt68Un = 680;			//68.0V
//	Volt91Un = 910;			//91.0V
//	Volt160Un = 1600;		//160.0V					//12.05.02
//#elif ( V10Un == 6 )	
//	Volt68Un = 390;			//39.0V
//	Volt91Un = 520;			//52.0V
//	Volt160Un = 923;		//92.3V						//12.05.02
//#elif ( V10Un == 38 )
//	Volt68Un = 2580;		//258.0V
//	Volt91Un = 3450;		//345.0V
//	Volt160Un = 6080;		//608.0V					//12.05.02	
//#endif

	for( i=0;i<3;i++ )
	{
		Volt[i] = BCD2_Word( Real.Ua+i*2 );	
		Volt[i+3] = BCD2_Word( Real.PreUa+i*2 );			
	}	

	for( i=0;i<6;i++ )									//12.05.02
	{													//12.05.02
#if( LinkMode == Phase3Wire3 )							//12.05.02
		if(( i==1 )||( i==4 )) continue;				//12.05.02
#endif													//12.05.02
		if( Volt[i] >= Volt160Un ) break;				//12.05.02
	}													//12.05.02

	if( i != 6 ) return 1;								//������һ���ѹ����160Un;		//12.05.02

	for( i=0;i<6;i++ )
	{
#if( LinkMode == Phase3Wire3 )	
		if(( i==1 )||( i==4 )) continue;
#endif				
		if( Volt[i] >= Volt91Un ) break;			
	}	

	if( i != 6 ) return 0;
	
#if( LinkMode == Phase3Wire3 )	
	if(( Volt[0] >= Volt68Un )&&( Volt[2] >= Volt68Un )) return 0;
#else
	if(( Volt[0] >= Volt68Un )&&( Volt[1] >= Volt68Un )&&( Volt[2] >= Volt68Un )) return 0;
#endif	

#if( LinkMode == Phase3Wire3 )	
	if(( Volt[3] >= Volt68Un )&&( Volt[5] >= Volt68Un )) return 0;
#else
	if(( Volt[3] >= Volt68Un )&&( Volt[4] >= Volt68Un )&&( Volt[5] >= Volt68Un )) return 0;
#endif	

	return 1;
}	
#endif	
#endif

#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else
#if ( IOver30ALimitTrip	== YesCheck )		//�¹���	//14.04.25	
short IOver30AJudge( void ) 
{
	short i;
	unsigned long Amp[6];
	unsigned long Value;											//�¹���	//14.04.25		

	Value = BCD3_Long( (unsigned char*)&Para.RILimitTrip_Valve );	//�¹���	//14.04.25
	if( Value == 0 ) return 0;										//�¹���	//14.04.25
	Value *= 10;													//�¹���	//14.04.25
		
	for( i=0;i<3;i++ )
	{
#if( LinkMode == Phase3Wire3 )	
		if( i==1 ) continue;
#endif				
		Amp[i] = BCD4_Long( Real.Ia+i*4 );	
		Amp[i+3] = BCD4_Long( Real.PreIa+i*4 );			
	}	

	for( i=0;i<6;i++ )
	{
#if( LinkMode == Phase3Wire3 )	
		if(( i==1 )||( i==4 )) continue;
#endif				
//		if( Amp[i] >= 300000 ) break;			
		if( Amp[i] >= Value ) break;								//�¹���	//14.04.25	
	}	

	if( i == 6 ) return 0;
	
	return 1;
}	
#endif
#endif

//#if ( PwDownKeepPrgDelay == YesCheck )							//12.12.21		//�¹���
//void initialPrgDly( void ) 
//{
//	unsigned char Buff1[6];
//	unsigned char Buff2[6];
//	unsigned char* Ptr;
//	unsigned char* Point;
//	unsigned short IntTemp;
//	unsigned long STime;
//	unsigned long ETime;
//
//	Ptr = Buff1;
//	Point = Buff2;
//
//	RAM_Write( Point, Clk.Min, 5 );	
//	if( DateTime5ValueCheck( Point ) != 0 ) return;
//	E2P_RFM( Ptr, FPdownTime, 6 );	 	//��ȡ�µ�󱣴�ĵ���ʱ��
//	if( DateTime5ValueCheck( Ptr+1 ) != 0 ) return;
//	E2P_RData( (unsigned char*)&Para.RPrgDly,EPrgDly, 2 );
//	Para.RPrgDly = BCD2_Word( (unsigned char*)&Para.RPrgDly ); 
//	E2P_RData( (unsigned char*)&IntTemp,EPrgRAMDelay, 2 );
//	if(( Flag.BatState & F_E2PCheck ) != 0 ) return;
//	if( IntTemp > Para.RPrgDly ) return;
//
//	STime = GetGmMin( Ptr+1 );
//	ETime = GetGmMin( Point );
//	if( STime > ETime ) SM.PrgDly = 0;
//	else 
//	{
//		ETime -= STime;
//		if( IntTemp >= ETime ) IntTemp -= ETime;
//		else IntTemp = 0;
//		SM.PrgDly = IntTemp;
//	}	
//	E2P_WData( EPrgRAMDelay, (unsigned char*)&IntTemp, 2 );
//}	
//#endif

short IsAwakeBatCom( void )								//�¹���
{	
	short i,j;
	
	for( i=0;i<3;i++ )
	{
		if( Comm.BatComBuff[i] == 0x68 )
		{
			j=(i+1) % 3;
//			if( Comm.BatComBuff[j] == 0x74 )
			if( Comm.BatComBuff[j] == 0x11 )			//13.09.14
			{
				j=(i+2) % 3;
//				if( Comm.BatComBuff[j] == 0x11 ) return 0;
				if( Comm.BatComBuff[j] == 0x04 ) return 0;		//13.09.14
			}				
		}				
	}	
	
	return 1;
}	

/*
//PwFlag:�����־ ��0x55:������ 0x00:��λ
void InitialBat_WorkTime( unsigned char PwFlag ) 
{
	unsigned char Buff1[6];
	unsigned char Buff2[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned long STime;
	unsigned long ETime;
	unsigned long Value;

	Ptr = Buff1;
	Point = Buff2;

	if(( Real.BatWorkTimeChk == ChkNum( Real.RBat_WorkTime, 4 ) )&&( AllIsBCD( Real.RBat_WorkTime, 4 ) == 0 )&&( PwFlag == 0x55 ))	
	{	
		RAM_Write( Point, Clk.Min, 5 );	
		if( DateTime5ValueCheck( Point ) != 0 ) return;
		E2P_RFM( Ptr, FPdownTime, 6 );	 	//��ȡ�µ�󱣴�ĵ���ʱ��
		if( DateTime5ValueCheck( Ptr+1 ) != 0 ) return;
    	
		Value = BCD4_Long( Real.RBat_WorkTime );
		STime = GetGmMin( Ptr+1 );
		ETime = GetGmMin( Point );
		if( STime < ETime ) 
		{
			ETime -= STime;
			Value += ETime;
			Long_BCD4( Real.RBat_WorkTime, Value );
			Real.BatWorkTimeChk = ChkNum( Real.RBat_WorkTime, 4 );
		}			
		E2P_WData( Bat_WorkTime, Real.RBat_WorkTime, 4 );
	}
	else
	{
		E2P_RData( Real.RBat_WorkTime, Bat_WorkTime, 4 ); 			
		if(((Flag.BatState & F_E2PCheck) != 0 )||( AllIsBCD( Real.RBat_WorkTime, 4 ) != 0 ))						
		{																										
			RAM_Fill( Real.RBat_WorkTime, 4 );																	
		}																											
		Real.BatWorkTimeChk = ChkNum( Real.RBat_WorkTime, 4 ); 	
	}
}
*/

//PwFlag:�����־ ��0x55:������ 0x00:��λ
void InitialBat_WorkTime( unsigned char PwFlag ) 
{
	unsigned char Buff1[6];
	unsigned char Buff2[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned long STime;
	unsigned long ETime;
	unsigned long Value;

	Ptr = Buff1;
	Point = Buff2;

	E2P_RData( Real.RBat_WorkTime, Bat_WorkTime, 4 ); 			
	if(((Flag.BatState & F_E2PCheck) != 0 )||( AllIsBCD( Real.RBat_WorkTime, 4 ) != 0 ))						
	{																										
		RAM_Fill( Real.RBat_WorkTime, 4 );																	
		E2P_WData( Bat_WorkTime, Real.RBat_WorkTime, 4 ); 			
		Real.BatWorkTimeChk = ChkNum( Real.RBat_WorkTime, 4 ); 	
	}																											

	if( PwFlag == 0x55 )	
	{	
		RAM_Write( Point, Clk.Min, 5 );	
		if( DateTime5ValueCheck( Point ) != 0 ) return;
		E2P_RFM( Ptr, FPdownTime, 6 );	 	//��ȡ�µ�󱣴�ĵ���ʱ��
		if( DateTime5ValueCheck( Ptr+1 ) != 0 ) return;
    	
		Value = BCD4_Long( Real.RBat_WorkTime );
		STime = GetGmMin( Ptr+1 );
		ETime = GetGmMin( Point );
		if( ETime > STime ) 
		{
			ETime -= STime;
			Value += ETime;
			Long_BCD4( Real.RBat_WorkTime, Value );
			Real.BatWorkTimeChk = ChkNum( Real.RBat_WorkTime, 4 );
		}			
		E2P_WData( Bat_WorkTime, Real.RBat_WorkTime, 4 );
	}
}
#endif
	
