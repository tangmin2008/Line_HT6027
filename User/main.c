#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ht6xxx_reset.h"
#include "TypeMeter.h"
#include "TypeRAM.h"
//#include <stdlib.h>
#include "TypeE2p.h"
//#include "msp430x54x.h"
//#include "In430.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Port.h"
#include "TypeFRAM.h"
#include "TypeFlash.h"
#include "Data.h"
#include "Interrupt.h"
#include "General.h"
#include "Time.h"
#include "Mem.h"
#include "Power.h"
#include "Initial.h"
#include "serial.h"
#include "IEC101.h"

#define EC_E2_W E2P_WData
#define EC_E2_R E2P_RData

short Get_CEVENT(int ch,int No,unsigned char *buf)
{
  if(ReadRecord(CEVENT0_USEADDR+ch*6,buf,No)==0)
    return ONE_RECORD_LEN;
  else
    return 0;
}
short Save_CEVENT(int ch)
{
  unsigned int i,year;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  
  if(Get_CEVENT(ch,0,tmp_buf))
    memcpy(&i,tmp_buf+6,4);
  else
  {
    i=0;
  }
  ++i;
  MoveCurrentTimeBCD_Hex();
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
  memcpy(tmp_buf+6,&i,4);
  LoadRecord(CEVENT0_USEADDR+ch*6,tmp_buf);
  return 0;
}

int CEVENT_Record_Num(ch)
{
  int Num;
  Num = Get_Record_Num(CEVENT0_USEADDR+ch*6);
  return Num;
}


short Get_JIAOSHI(int No,unsigned char *buf)
{
  if(ReadRecord(TIME_USEADDR,buf,No)==0)
    return ONE_RECORD_LEN;
  else
    return 0;
}
short Save_JIAOSHI(unsigned char *Time_buf)
{
  unsigned int i;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  if(Get_JIAOSHI(0,tmp_buf))
    memcpy(&i,tmp_buf+6,4);
  else
  {
    i=0;
  }
  ++i;
  memcpy(tmp_buf,Time_buf,6);
  memcpy(tmp_buf+6,&i,4);
  LoadRecord(TIME_USEADDR,tmp_buf);
  return 0;
}

int JIAOSHI_Record_Num()
{
  int Num;
  Num = Get_Record_Num(TIME_USEADDR);
  return Num;
}


void Clear_Save(int ch)
{
  unsigned short m_e2_ptr,m_e2_buf,year;
  unsigned char ptr_buf[4],tmp_buf[64];
  MoveCurrentTimeBCD_Hex();
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
  memcpy(tmp_buf+10,&Energy_Data[ch],ONE_RECORD_LEN-6);
  LoadRecord(CH0_CLR_USEADDR+ch*48,tmp_buf);
}

int GetClear_num(int ch)
{
  int Num;
  Num = Get_Record_Num(CH0_CLR_USEADDR+ch*48);
  return Num;
}

int GetClear_Record(int ch,int No,unsigned char *buf)
{
  if(ReadRecord(CH0_CLR_USEADDR+ch*48,buf,No))
    return 0;
  return 88;
}

//��һ���ֽ�ָ��
//�ڶ����ֽ�����
void Pn_Event_Save(int ch,int phase,unsigned char flag)
{
  unsigned short m_e2_ptr,m_e2_buf,year;
  unsigned char ptr_buf[4],tmp_buf[48];
  MoveCurrentTimeBCD_Hex();
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
  
  if(flag)
  {
    SM.PQNum[ch][phase]++;
    memcpy(tmp_buf+6,&SM.PQNum[ch][phase],4);
    memcpy(tmp_buf+10,&Energy_Data[ch],32);
    LoadRecord(CH0_PAP_USEADDR+ch*48+phase*3,tmp_buf);
  }
  else
  {
    memcpy(tmp_buf+6,&Energy_Data[ch],32);
    LoadRecord(CH0_PAN_USEADDR+ch*48+phase*3,tmp_buf);
  }
}

int GetPn_Event_num(int ch,int phase)
{
  int Num;
  Num = Get_Record_Num(CH0_PAP_USEADDR+ch*48+phase*3);
  return Num;
}

int GetPn_Event_Record(int ch,int phase,int No,unsigned char *buf)
{
  if(ReadRecord(CH0_PAP_USEADDR+ch*48+phase*3,buf,No))
    return 0;
  ReadRecord(CH0_PAN_USEADDR+ch*48+phase*3,buf+42,No);
  return 88;
}

void Pt_Event_Save(int ch)
{
  unsigned short m_e2_ptr,m_e2_buf,year;
  unsigned char ptr_buf[4],tmp_buf[48];
  MoveCurrentTimeBCD_Hex();
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
  SM.PQNum[ch][3]++;
  memcpy(tmp_buf+6,&SM.PQNum[ch][3],4);
  memcpy(tmp_buf+10,&Energy_Data[ch],32);
  LoadRecord(CH0_PTD_USEADDR+ch*48,tmp_buf);
  if(SM.CalibCount != CALIBCOUNT1)
  {
    Insert_Push(TREND_TYPE,ch);
  }
}

int GetPt_Event_num(int ch)
{
  int Num;
  Num = Get_Record_Num(CH0_PTD_USEADDR+ch*48);
  return Num;
}

int GetPt_Event_Record(int ch,int No,unsigned char *buf)
{
  if(ReadRecord(CH0_PTD_USEADDR+ch*48,buf,No))
    return 0;
  return 42;
}

short Save_Data(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  memcpy(tmp_buf,Time_buf,6);
  for(i=0;i<MAX_CH_NUM;++i)
  {
    if(SM.CalibCount != CALIBCOUNT1)
    {
      Insert_Push(LOAR_TYPE,i);
    }
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    LoadRecord(LOAD0_USEADDR+i*30,tmp_buf);
  }
  return 0;
}

short Get_LoadData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(LOAD0_USEADDR+30*ch,buf,No);
  return ONE_RECORD_LEN;
}

int Load_Record_Num()
{
  int Num;
  Num = Get_Record_Num(LOAD0_USEADDR);
  return Num;
}

short Read_LastData(int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(LOAD0_USEADDR+30*ch,buf,0);
  return ONE_RECORD_LEN;
}

short Save_DayData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    LoadRecord(FRZD0_USEADDR+i*30,tmp_buf);
  }
  return 0;
}

short Get_DayData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(FRZD0_USEADDR+30*ch,buf,No);
  return ONE_RECORD_LEN;
}

int Day_Record_Num()
{
  int Num;
  Num = Get_Record_Num(FRZD0_USEADDR);
  return Num;
}

void compensate_day()
{
 unsigned char tmp_buf[ONE_RECORD_SIZE]; 
 unsigned char time_buf[8];
 int year;
 int i,j,k;
 for(i=0;i<MAX_CH_NUM;++i)
 {
   if(Day_Record_Num()==0)
     break;
   ReadRecord(FRZD0_USEADDR+30*i,tmp_buf,0);
   memcpy(time_buf,tmp_buf,6); 
   tmp_buf[0] = 0;
   tmp_buf[1] = 0;
   tmp_buf[2] = 0;
   tmp_buf[3] = Clk.DayH;
   tmp_buf[4] = Clk.Month;
   year = Clk.YearH;
   year = year*256 + Clk.YearL;
   tmp_buf[5]=year-2000;
   for(j=0;j<7;++j)
   {
     if((tmp_buf[3] == time_buf[3]) && (tmp_buf[4] == time_buf[4]) && (tmp_buf[5] == time_buf[5]))
     {
       break;
     }
     --tmp_buf[3];
     if(tmp_buf[3]==0)
     {
       --tmp_buf[4];
       if(tmp_buf[4]==0)
       {
         tmp_buf[4] = 12;
         --tmp_buf[5];
       }
       if((tmp_buf[4]==2) && (tmp_buf[5]%4==0))
        tmp_buf[3] = 29;
       else
        tmp_buf[3] = MonTab[tmp_buf[4]]; 
     }
   }

   for(j=0;j<7;++j)
   {
     if((tmp_buf[3] == Clk.DayH) && (tmp_buf[4] == Clk.Month) && (tmp_buf[5] == (year-2000)))
     {
       break;
     }
     ++tmp_buf[3];
     if(tmp_buf[3] > (((tmp_buf[4]==2 ) && ((tmp_buf[5]%4)==0))? 29:MonTab[tmp_buf[4]]))
     {
       tmp_buf[3]=1;
       ++tmp_buf[4];
       if(tmp_buf[4]>12)
       {
         tmp_buf[4] = 1;
         ++tmp_buf[5];
       }
     }	
     LoadRecord(FRZD0_USEADDR+i*30,tmp_buf);
   }
 }
}


short Save_HourData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    LoadRecord(SHRP0_USEADDR+i*30,tmp_buf);
  } 
  return 0;
}

short Get_HourData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(SHRP0_USEADDR+30*ch,buf,No);
  return ONE_RECORD_LEN;
}

int Hour_Record_Num()
{       
  int Num;
  Num = Get_Record_Num(SHRP0_USEADDR);
  return Num;
}

short Save_MonthData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    LoadRecord(MOND0_USEADDR+i*30,tmp_buf);
  } 
  return 0;
}

short Get_MonthData(int No,int ch,unsigned char *buf)
{
  if(ch>8)
    return 0;
  ReadRecord(MOND0_USEADDR+30*ch,buf,No);
  return E2ONE_RECORD_LEN;
}
void compensate_month()
{
 unsigned char tmp_buf[ONE_RECORD_SIZE]; 
 unsigned char time_buf[8];
 int year;
 int i,j,k;
 for(i=0;i<MAX_CH_NUM;++i)
 {
   if(Month_Record_Num()==0)
     break;
   ReadRecord(MOND0_USEADDR+30*i,tmp_buf,0);
   memcpy(time_buf,tmp_buf,6);
   tmp_buf[0] = 0;
   tmp_buf[1] = 0;
   tmp_buf[2] = 0;
   tmp_buf[3] = 1;//Clk.DayH;
   tmp_buf[4] = Clk.Month;
   year = Clk.YearH;
   year = year*256 + Clk.YearL;
   tmp_buf[5]=year-2000;
   for(j=0;j<12;++j)
   {
     if((tmp_buf[4] == time_buf[4]) && (tmp_buf[5] == time_buf[5]))
     {
       break;
     }
     --tmp_buf[4];
     if(tmp_buf[4]==0)
     {
       tmp_buf[4] = 12;
       --tmp_buf[5];
     }
   }
   memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
   for(j=0;j<12;++j)
   {
     if((tmp_buf[4] == Clk.Month) && (tmp_buf[5] == (year-2000)))
     {
       break;
     }
     ++tmp_buf[4];
     if(tmp_buf[4]>12)
     {
       tmp_buf[4] = 1;
       ++tmp_buf[5];
     }	
     LoadRecord(MOND0_USEADDR+i*30,tmp_buf);
   }
 }
}
int Month_Record_Num()
{
  int Num;
  Num = Get_Record_Num(MOND0_USEADDR);
  return Num;
}

short Save_RandData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    LoadRecord(RAND0_USEADDR+i*30,tmp_buf);
  } 
  return 0;
}
short Get_RandData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(RAND0_USEADDR+30*ch,buf,No);
  return E2ONE_RECORD_LEN;
}
int Rand_Record_Num()
{
  int Num;
  Num = Get_Record_Num(RAND0_USEADDR);
  return Num;
}

  
    
int Get_LoadFile_Msg_Len(void)
{
  int Len,rec_ptr;
  rec_ptr=Load_Record_Num();
  Len = 51+rec_ptr*189; 
  return Len; 
}

int Get_LoadFile_Xml_Len(void)
{
  int Len,rec_ptr;
  rec_ptr=Load_Record_Num();
  Len = 100+rec_ptr*256; 
  return Len; 
}

void Save_ECRamBufAds()
{
  int i;
  unsigned short* ECRamBufAdsPtr;
  unsigned char* ECRamBufChkAdsPtr;
  unsigned short ECEAds;
  
  for( i=0;i<ECUnitNum;++i)
  {
    ECRamBufAdsPtr = ECRgTab[i].ECRamBufAds;
    ECRamBufChkAdsPtr = ECRgTab[i].ECRamBufChkAds;
    ECEAds = ECRgTab[i].ECEAds;
    if( *ECRamBufChkAdsPtr == ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 ) )
    {
      E2P_PWData(ECEAds,(unsigned char*)ECRamBufAdsPtr,2);
    }
  }
}

void ReLoad_ECRamBufAds()
{
  int i;
  unsigned short* ECRamBufAdsPtr;
  unsigned char* ECRamBufChkAdsPtr;
  unsigned short ECEAds;
  
  for( i=0;i<ECUnitNum;++i)
  {
    ECRamBufAdsPtr = ECRgTab[i].ECRamBufAds;
    ECRamBufChkAdsPtr = ECRgTab[i].ECRamBufChkAds;
    ECEAds = ECRgTab[i].ECEAds;
    E2P_PRData((unsigned char*)ECRamBufAdsPtr,ECEAds,2);
    *ECRamBufChkAdsPtr = ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 );
  }
}

void EC_MeasA(void)
{
  int i;
  unsigned short* ECRamBufAdsPtr;
  unsigned char* ECRamBufChkAdsPtr;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned int Ps2;
  Ps2 = MSpec.RMeterConst/EN_DOTS;
  for( i=0;i<ECUnitNum;++i)
  {
    ECRamBufAdsPtr = ECRgTab[i].ECRamBufAds;
    ECRamBufChkAdsPtr = ECRgTab[i].ECRamBufChkAds;
    ECRAds = ECRgTab[i].ECRAds;
    if( *ECRamBufChkAdsPtr != ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 ) )
    {
      *ECRamBufAdsPtr = 0;
      *ECRamBufChkAdsPtr = 0;
    }
    if(*ECRamBufAdsPtr>=Ps2)
    {
      *ECRAds += (*ECRamBufAdsPtr/Ps2);//*ECRamBufAdsPtr;
      ECEAds = ECRgTab[i].ECEAds;
      EC_E2_W(ECEAds,(unsigned char*)ECRAds,4);
      *ECRamBufAdsPtr %= Ps2;
      *ECRamBufChkAdsPtr = ChkNum((unsigned char*)ECRamBufAdsPtr,2);
    }
  }
}
void EC_ClearA(void)
{
  int i;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned char tmpbuf[8];
  memset(tmpbuf,0,8);
  for( i=0;i<ECUnitNum;++i)
  {
    ECRAds = ECRgTab[i].ECRAds;
    ECEAds = ECRgTab[i].ECEAds;
    EC_E2_W(ECEAds,tmpbuf,4);
    *ECRAds = 0;
  }
}
void Read_E2R()
{
  int i;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  for( i=0;i<ECUnitNum;++i)
  {
    ECRAds = ECRgTab[i].ECRAds;
    ECEAds = ECRgTab[i].ECEAds;
    EC_E2_R((unsigned char*)ECRAds,ECEAds,4);
  }
}

void Clear_E2R(int chan)
{
  int i,j;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned char tmpbuf[8];
  memset(tmpbuf,0,8);
  if(chan==0)
  {
    chan=ECUnitNum;
    for(i=0;i<MAX_CH_NUM;++i)
    {
      Clear_Save(i);
    }
    i = 0; 
  }
  else
  {
    Clear_Save(chan-1);
    i = 8*(chan-1);
    chan = 8*chan;
  }
  while(i<chan)
  {
    ECRAds = ECRgTab[i].ECRAds;
    ECEAds = ECRgTab[i].ECEAds;
    memset(tmpbuf,0,4);
    EC_E2_W(ECEAds,tmpbuf,4);
    EC_E2_R(tmpbuf,ECEAds,4);
    memcpy(ECRAds,tmpbuf,4);
    ECEAds = ECRgTab[i].ECEAds;
    E2P_PWData(ECEAds,tmpbuf,2);
    *ECRgTab[i].ECRamBufAds = 0;
    *ECRgTab[i].ECRamBufChkAds = 0xa5;
    ++i;
  }
}

void Clear_EVT2R(int chan)
{
  int i;
  unsigned char ptr_buf[4];
  memset(ptr_buf,0,4);
  if(chan==0)
  {
    chan=16*MAX_CH_NUM;
    for(i=0;i<MAX_CH_NUM;++i)
    {
      Save_CEVENT(i);
    }
    i = 0; 
  }
  else
  {
    Save_CEVENT(chan-1);
    i = 16*(chan-1);
    chan = 16*chan;
  }
  while(i<chan)
  {
    if((i+1)%8)
      EC_E2_W(CH0_PAP_USEADDR+3*i,ptr_buf,2);
    ++i;
  }
}

void Read_E2R1()
{
  int i;
  unsigned char ptr_buf[4];
  memset(ptr_buf,0,4);
  for( i=0;i<26*MAX_CH_NUM;++i)
  {
    EC_E2_W(LOAD0_USEADDR+3*i,ptr_buf,2);
  }
}

void ProcHalfSec(void)
{
  int i;
  unsigned int tmp_p;
  short Quad;
  Flag.Clk &= ~F_HalfSec;
  if((Flag.Power & F_PwrUp) == 0)
      return;
  HT_GPIO_BitsToggle(HT_GPIOB,GPIO_Pin_5);
  for(i=0;i<MAX_CH_NUM;++i)
  {
    IEC101Process();
    ATT7022RdReg(ATVoltFlag,(unsigned char*)&(SM.State[i]),i);
    ATT7022RdReg(ATPZ,(unsigned char*)&tmp_p,i);
    //tmp_p = 80;  //test
    tmp_p &= 0xffffff;
    if(tmp_p>100)
      tmp_p = 0;
    if(tmp_p)
    {
      if(SM.State[i]&0x1000)
      {
        ECP.PL_CumPn[i] += tmp_p;
        ECP.PL_ChkPn[i]=ChkNum((unsigned char*)&ECP.PL_CumPn[i],2);
      }
      else
      {
        ECP.PL_CumPp[i] += tmp_p;
        ECP.PL_ChkPp[i]=ChkNum((unsigned char*)&ECP.PL_CumPp[i],2);
      }
    }
    ATT7022RdReg(ATQZ,(unsigned char*)&tmp_p,i);
    tmp_p &= 0xffffff;
    if(tmp_p>100)
      tmp_p = 0;
    if(tmp_p)
    {
      if(SM.State[i]&0x2000)
      {
        ECP.PL_CumQn[i] += tmp_p;
        ECP.PL_ChkQn[i]=ChkNum((unsigned char*)&ECP.PL_CumQn[i],2);
      }
      else
      {
        ECP.PL_CumQp[i] += tmp_p;
        ECP.PL_ChkQp[i]=ChkNum((unsigned char*)&ECP.PL_CumQp[i],2);
      }
      Quad=GetMQuad((SM.State[i]>>12)&0x3,0);
      //Quad = (SM.State[i]>>12)&0x3;
      ECP.PL_CumQ[i][Quad] += tmp_p;
      ECP.PL_ChkQ[i][Quad]=ChkNum((unsigned char*)&ECP.PL_CumQ[i][Quad],2);
    }
  }
  EC_MeasA();
}
/***************************************************
	Working for Every Second
	1. Decreasing Second Timer
	2. Energy Measurement
	3. Maxium Demand Measurement
	4. Even Log File
***************************************************/

void ProcSec(void)
{
  char Buff[8];
  char* Point;
  int i,j,flag_p;
  signed long *si_val;
  unsigned int i_val;

  Flag.Clk &= ~F_Sec;
  Point = Buff;
  
  memset( Point,0,6 );
  
  if((Flag.Power & F_PwrUp) != 0)
  {
    if(SM.TestCnt != 0)
    {
       SM.TestCnt--;
    }
    if(SM.TestDisCnt != 0)
    {
       SM.TestDisCnt--;
       if(SM.TestDisCnt==0 && SM.rebootflag)
      {
          NVIC_SystemReset();
      }
    }
#if 0    
    for(Buff[0]=0;Buff[0]<MAX_CH_NUM;++Buff[0])
    {
     // Energy_Data[Buff[0]].Pn++;
      Energy_Data[Buff[0]].Pp = Buff[0];
      Energy_Data[Buff[0]].Pn +=Buff[0];
      Energy_Data[Buff[0]].Qn++;
      Energy_Data[Buff[0]].Q1++;
      Energy_Data[Buff[0]].Q2++;
      /*
      Energy_Data[Buff[0]].Pa = rand();
      Energy_Data[Buff[0]].Pb = rand();
      Energy_Data[Buff[0]].Pc = rand();
      Energy_Data[Buff[0]].Pt = Energy_Data[Buff[0]].Pa+Energy_Data[Buff[0]].Pb+Energy_Data[Buff[0]].Pc;
      Real_Data[Buff[0]].Qa++;*/
    }
#endif  
    IEC101Process();
    for(i=0;i<MAX_CH_NUM;++i)
    {
      
      ATT7022RdReg(PFlag,(unsigned char*)&(SM.PQFlag[i]),i);
#if 0      
      Energy_Data[i].Pa = GetPhasePW(ATPWPA,i);
      Energy_Data[i].Pb = GetPhasePW(ATPWPB,i);
      Energy_Data[i].Pc = GetPhasePW(ATPWPC,i);
      Energy_Data[i].Pt = GetPhasePW(ATPWPZ,i);
#else
      Read_ATTValue(ATPWPA,(unsigned char *)&Energy_Data[i].Pa,i);
      Read_ATTValue(ATPWPB,(unsigned char *)&Energy_Data[i].Pb,i);
      Read_ATTValue(ATPWPC,(unsigned char *)&Energy_Data[i].Pc,i);
      Read_ATTValue(ATPWPZ,(unsigned char *)&Energy_Data[i].Pt,i);
      if(SM.PQFlag[i]&0x1)
      {
        Energy_Data[i].Pa *=-1;
      }
      if(SM.PQFlag[i]&0x2)
      {
        Energy_Data[i].Pb *=-1;
      }
      if(SM.PQFlag[i]&0x4)
      {
        Energy_Data[i].Pc *=-1;
      }
      if(SM.PQFlag[i]&0x8)
      {
        Energy_Data[i].Pt *=-1;
      }
#endif      
      Real_Data[i].Pa = Energy_Data[i].Pa;
      Real_Data[i].Pb = Energy_Data[i].Pb;
      Real_Data[i].Pc = Energy_Data[i].Pc;
      Real_Data[i].Pt = Energy_Data[i].Pt;
#if 0      
      Real_Data[i].Qa = GetPhasePW(ATPWQA,i);
      Real_Data[i].Qb = GetPhasePW(ATPWQB,i);
      Real_Data[i].Qb = GetPhasePW(ATPWQB,i);
      Real_Data[i].Qt = GetPhasePW(ATPWQZ,i);
#else
      Read_ATTValue(ATPWQA,(unsigned char *)&Real_Data[i].Qa,i);
      Read_ATTValue(ATPWQB,(unsigned char *)&Real_Data[i].Qb,i);
      Read_ATTValue(ATPWQC,(unsigned char *)&Real_Data[i].Qc,i);
      Read_ATTValue(ATPWQZ,(unsigned char *)&Real_Data[i].Qt,i);
      if(SM.PQFlag[i]&0x10)
      {
        Real_Data[i].Qa *=-1;
      }
      if(SM.PQFlag[i]&0x20)
      {
        Real_Data[i].Qb *=-1;
      }
      if(SM.PQFlag[i]&0x40)
      {
        Real_Data[i].Qc *=-1;
      }
      if(SM.PQFlag[i]&0x80)
      {
        Real_Data[i].Qt *=-1;
      }
#endif      
      Read_ATTValue(ATPWSA,(unsigned char *)&Real_Data[i].Sa,i);
      Read_ATTValue(ATPWSB,(unsigned char *)&Real_Data[i].Sb,i);
      Read_ATTValue(ATPWSC,(unsigned char *)&Real_Data[i].Sc,i);
      Read_ATTValue(ATPWSZ,(unsigned char *)&Real_Data[i].St,i);
      Read_ATTValue(ATFreq,(unsigned char *)&Real_Data[i].AFreq,i);
      Read_ATTValue(ATUrmsA,(unsigned char *)&Real_Data[i].Ua,i);
      Read_ATTValue(ATUrmsB,(unsigned char *)&Real_Data[i].Ub,i);
      Read_ATTValue(ATUrmsC,(unsigned char *)&Real_Data[i].Uc,i);
      Read_ATTValue(ATIrmsA,(unsigned char *)&Real_Data[i].Ia,i);
      Read_ATTValue(ATIrmsB,(unsigned char *)&Real_Data[i].Ib,i);
      Read_ATTValue(ATIrmsC,(unsigned char *)&Real_Data[i].Ic,i);
      Read_ATTValue(ATFactorA,(unsigned char *)&Real_Data[i].Pfa,i);
      Read_ATTValue(ATFactorB,(unsigned char *)&Real_Data[i].Pfb,i);
      Read_ATTValue(ATFactorC,(unsigned char *)&Real_Data[i].Pfc,i);
      Read_ATTValue(ATFactorZ,(unsigned char *)&Real_Data[i].Pft,i);
      Read_ATTValue(ATAngleA,(unsigned char *)&SM.Angle_Ia[i],i);
      Read_ATTValue(ATAngleB,(unsigned char *)&SM.Angle_Ib[i],i);
      Read_ATTValue(ATAngleC,(unsigned char *)&SM.Angle_Ic[i],i);
      i_val = 0;
      for(j=0;j<3;++j)
      {
        if(*((&Real_Data[i].Ia)+j)==0)
        {
          *((&Real_Data[i].Pfa)+j)=0;
        }
        i_val += *((&Real_Data[i].Ia)+j);
      }
      if(i_val==0)
        Real_Data[i].Pft = 0;
      i_val = 0;
      for(j=0;j<3;++j)
      {
        i_val += *((&Real_Data[i].Ua)+j);
      }
      if(i_val==0)
        Real_Data[i].AFreq = 50000;
      
    }
    IEC101Process();
#if 1   
  //  Pn_Event_Save(0,1,1);
    //Pt_Event_Save(0);
    for(i=0;i<MAX_CH_NUM;++i)
    {
      flag_p = SM.PQFlag[i]^SM.PQFlag_b[i];
      SM.PQFlag_b[i]=SM.PQFlag[i];
      if((flag_p&0xf))
      {
        si_val = &Real_Data[i].Pa;
        for(j=0;j<4;++j)
        {
          if(flag_p&(1<<j))
          {
            if(abs(si_val[j])>289)
            {
              SM.P_Time[i][j]=60;
            }
          }
        }
      }
      for(j=0;j<3;++j) //���ʷ���
      {
        if(SM.P_Time[i][j])
        {
          --SM.P_Time[i][j];
          if(SM.P_Time[i][j]==0)
          {
            Pn_Event_Save(i,j,SM.PQFlag[i]&(1<<j));
          }
        }
      }
      
      if(SM.P_Time[i][3]) //����
      {
        --SM.P_Time[i][3];
        if(SM.P_Time[i][3]==0)
        {
            Pt_Event_Save(i);
        }
      }
    }
#endif    
#if 0    
    if(SM.TestDisCnt==0)
    {
        SM.TestDisCnt=2;
        //sprintf(Buff,"%05d\r\n",SM.ClockBatAD);
        Buff[0]=0xaa;
        Buff[1]=0x55;
        Buff[2]=0x00;
        Buff[3]=0x11;
        Buff[4]=0x22;
        Serial_Write(2,Buff,5);
    } 
#endif
    IEC101Process();
  //ATT7022EStateCheckRun(Clk.SecH%MAX_CH_NUM);    
  }
  else
  {
    HT_GPIO_BitsToggle(HT_GPIOB,GPIO_Pin_5);
  }
}


/***************************************************
	Working for Every Minute
	1. Decreasing Minute Timer
	2. Fee judgement
	3. Load Profile
***************************************************/
void ProcMin(void)
 {
    unsigned char Time_buf[8];
    unsigned short year,i;
    Flag.Clk &= ~F_Min;
    if((Flag.Power & F_PwrUp) == 0)
      return;
    //GetTime();
    //Pt_Event_Save(0);
    MoveCurrentTimeBCD_Hex();
    Time_buf[0]=Clk.SecH;
    Time_buf[1]=Clk.MinH;
    Time_buf[2]=Clk.HourH;
    Time_buf[3]=Clk.DayH;
    Time_buf[4]=Clk.Month;
    year = Clk.YearH;
    year = year*256 + Clk.YearL;
    Time_buf[5]=year-2000;
    if((Clk.MinH%15)==0)
    {
      IEC101Process();
      Save_Data(Time_buf);
    }
    
    //Save_RandData(Time_buf);
 //   Save_MonthData(Time_buf);
    ATT7022EStateCheckRun(Clk.MinH%MAX_CH_NUM);
    if((Real_Data[0].Ua>8000) && (Real_Data[0].Ub<80))
    {
      if(Para.PW==0x34)
      {
        Para.PW=0x33;
        HT_GPIO_BitsReset(HT_GPIOC,GPIO_Pin_4);
        for(i=0;i<MAX_CH_NUM;i++)
        {
          ATT7022Init(i);	
        }
      }
    }
    else
    {
      if(Para.PW==0x33)
      {
        Para.PW=0x34;
        HT_GPIO_BitsSet(HT_GPIOC,GPIO_Pin_4);
        for(i=0;i<MAX_CH_NUM;i++)
        {
          ATT7022Init(i);
        }
      }
    }
}	

/***************************************************
	Working for Every Minute
	1. Decreasing Minute Timer
***************************************************/
void ProcHour(void)
{
  unsigned char Time_buf[8];
  unsigned short year;
  Flag.Clk &= ~F_Hour;
  if((Flag.Power & F_PwrUp) == 0)
      return;
  GetTime();
  MoveCurrentTimeBCD_Hex();
  Time_buf[0]=Clk.SecH;
  Time_buf[1]=Clk.MinH;
  Time_buf[2]=Clk.HourH;
  Time_buf[3]=Clk.DayH;
  Time_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  Time_buf[5]=year-2000;
  IEC101Process();
  Save_HourData(Time_buf);
}

void ProcDay(void)
{
  unsigned char Time_buf[8];
  unsigned short year;
  Flag.Clk &= ~F_Day;
  if((Flag.Power & F_PwrUp) == 0)
      return;
  Time_buf[0]=Clk.SecH;
  Time_buf[1]=Clk.MinH;
  Time_buf[2]=Clk.HourH;
  Time_buf[3]=Clk.DayH;
  Time_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  Time_buf[5]=year-2000;
  Save_DayData(Time_buf);
  if(Clk.DayH==1)
  {
    Save_MonthData(Time_buf);
  }
}	
#define COM_PARITY   UartParity_Disable  
//#define COM_PARITY   UartParity_EVEN
void main(void)
{
  unsigned int i;
  unsigned char flash_id[16];
  HT_FreeDog();
  i = 5000;
  //i=E2END_DATA_ADDR;
  while(i--);
  fnTarget_Init();
 // Flag.BatState=0;
  while(1)
  {	
    HT_FreeDog();
    for(i=0;i<5000;i++)
    {
      HT_FreeDog();
      __NOP();
    }
    
    GetTime();
    //Read_E2R();
    MoveCurrentTimeBCD_Hex();																									//10.11.22
    while (1)
    {
      HT_FreeDog();						
      if(((Flag.Power & F_PwrUp) == 0) && ( PowerCheck() == 1 ))		
      {
        SM.CalibCount =0;
       // Flag.BatState=0;
        PwrOnInit();
        VarInit();	
        InitPara();			
        InitPara5();
        Serial_Open(2,9600,8,COM_PARITY);
        //Serial_Open(2,9600,8,UartParity_EVEN);
	InitPara6();
//        flash_id[0]=0x80;
//        flash_id[1]=0x10;
//        flash_id[2]=0x00;
//        flash_id[3]=0x00;
//        Sample_Error(flash_id);
        Load_InfoData();
       // Serial_Write(2,"Start Debug\r\n",13);   
        InitIEC101Prot();
   //     Read_FlashID(flash_id);
#if 0        
        BlockErase(0);
        Read_Flash(flash_id,0,10);
        memset(flash_id,0xaa,10);
        Write_Flash(0,flash_id,10);
        memset(flash_id,0,10);
        Read_Flash(flash_id,0,10);
        memset(flash_id,0x55,16);
        E2P_PWData(0,flash_id,16);
        memset(flash_id,0,10);
        E2P_PRData(flash_id,0,16);
        memset(flash_id,0x33,16);
        E2P_WData(0,flash_id,16);
        memset(flash_id,0,10);
        E2P_RData(flash_id,0,16);
        memset(flash_id,0,10);
        E2P_WData(LoadRecord_Time,flash_id,8);
        E2P_WData(RandRecord_Time,flash_id,8);
        E2P_WData(FrzdRecord_Time,flash_id,8);
        E2P_WData(ShrpdRecord_Time,flash_id,8);
        E2P_WData(MonthdRecord_Time,flash_id,8);
#endif
        //EC_ClearA();
        //Read_E2R1();
        Read_E2R();
        ReLoad_ECRamBufAds();
        for(i=0;i<MAX_CH_NUM;i++)
        {
          ATT7022Init(i);
        }
        compensate_day();
        compensate_month();
        //Clear_E2R(0);
        //Clear_EVT2R(0);
        break;
      }	
      if(((Flag.Power & F_PwrUp) != 0) && ( PowerCheck() == 0 ))
      {
        Save_ECRamBufAds();
        break;
//       // Flag.BatState=1;
//        //PwrDnInit();
      }	
//    
      
//      if(( Flag.Clk & F_ThirdSec )) 	
//      {		
//        Flag.Clk &= ~F_ThirdSec;								
//      }
      if(Flag.Clk& F_HalfSec)
      {
        ProcHalfSec();
      }
      if (Flag.Clk & F_Sec) ProcSec();		
      if (Flag.Clk & F_Min) ProcMin();
      if (Flag.Clk & F_Hour) ProcHour();
      if (Flag.Clk & F_Day) ProcDay();
      
      if(Flag.Power & F_IrmsCheck)
      {
        udelay(10000);
        Serial_Open(2,115200,8,UartParity_Disable);
        xmodemReceive();
        udelay(10000);
        //Serial_Open(2,9600,8,UartParity_EVEN);
        Serial_Open(2,9600,8,COM_PARITY);
      }
      else
      {	
        IEC101Process();
      }			
      
      if(((Flag.Power & F_PwrUp) == 0))
      { 
        Flag.Run |= F_Hold;
        EnterHold();						
        if((HT_PMU->WAKEIF&0x001000f4))
        {	
          if((Flag.Run & F_Hold)!=0)
          {
            Flag.Run &= ~F_Hold;
            ExitHold();
            HT_FreeDog();						
          }
        }				
      }		
    }
  }
}	

