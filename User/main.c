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
  
  m_e2_ptr = s_PnEvt[ch*3+phase].E2_PTR;
  m_e2_buf = s_PnEvt[ch*3+phase].E2_ADDR;
  E2P_RData(ptr_buf,m_e2_ptr,2);
  
  ptr_buf[0] %=10;
  if(flag)
  {
    memcpy(tmp_buf+6,&SM.PQNum[ch][phase],4);
    memcpy(tmp_buf+10,&Energy_Data[ch],32);
    E2P_PWData(m_e2_buf+ptr_buf[0]*84,tmp_buf,42);
    if(ptr_buf[1]<10)
      ptr_buf[1]++;
  }
  else
  {
    memcpy(tmp_buf+6,&Energy_Data[ch],32);
    E2P_PWData(m_e2_buf+ptr_buf[0]*84+44,tmp_buf,38);
    ptr_buf[0]++;
  }
  E2P_WData(m_e2_ptr,ptr_buf,2);
}

int GetPn_Event_num(int ch,int phase)
{
  unsigned short m_e2_ptr;
  unsigned char ptr_buf[4];
  m_e2_ptr = s_PnEvt[ch*3+phase].E2_PTR;
  E2P_RData(ptr_buf,m_e2_ptr,2);
  return ptr_buf[1];
}

int GetPn_Event_Record(int ch,int phase,int No,unsigned char *buf)
{
  unsigned short m_e2_ptr,m_e2_buf;
  int rec_ptr;
  unsigned char ptr_buf[4];
  m_e2_ptr = s_PnEvt[ch*3+phase].E2_PTR;
  m_e2_buf = s_PnEvt[ch*3+phase].E2_ADDR;
  E2P_RData(ptr_buf,m_e2_ptr,2);
  if(ptr_buf[1]<10)
    rec_ptr=No%10;
  else
    rec_ptr=(ptr_buf[0]+No)%10;
  E2P_PRData(buf,m_e2_buf+rec_ptr*84,42);
  E2P_PRData(buf+42,m_e2_buf+rec_ptr*84+44,38);
  return ptr_buf[1];
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
  memcpy(tmp_buf+6,&SM.PQNum[ch][3],4);
  memcpy(tmp_buf+10,&Energy_Data[ch],32);
  m_e2_ptr = s_PtEvt[ch].E2_PTR;
  m_e2_buf = s_PtEvt[ch].E2_ADDR;
  E2P_RData(ptr_buf,m_e2_ptr,2);
  ptr_buf[0] %=10;
  E2P_PWData(m_e2_buf+ptr_buf[0]*44,tmp_buf,42);
  if(ptr_buf[1]<10)
      ptr_buf[1]++;
  ptr_buf[0]++;
  E2P_WData(m_e2_ptr,ptr_buf,2);
}

int GetPt_Event_num(int ch)
{
  unsigned short m_e2_ptr;
  unsigned char ptr_buf[4];
  m_e2_ptr = s_PtEvt[ch].E2_PTR;
  E2P_RData(ptr_buf,m_e2_ptr,2);
  return ptr_buf[1];
}

int GetPt_Event_Record(int ch,int No,unsigned char *buf)
{
  unsigned short m_e2_ptr,m_e2_buf;
  int rec_ptr;
  unsigned char ptr_buf[4];
  m_e2_ptr = s_PnEvt[ch].E2_PTR;
  m_e2_buf = s_PnEvt[ch].E2_ADDR;
  E2P_RData(ptr_buf,m_e2_ptr,2);
  if(ptr_buf[1]<10)
    rec_ptr=No%10;
  else
    rec_ptr=(ptr_buf[0]+No)%10;
  E2P_PRData(buf,m_e2_buf+rec_ptr*44,42);
  return ptr_buf[1];
}

short Save_Data(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
 // unsigned short year;
  
  E2P_RData(Record_buf,LoadRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  rec_ptr %=LOAD_RECORD_NUM;
#if 0  
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
#else
  memcpy(tmp_buf,Time_buf,6);
#endif  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    DataFlash_Write(i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,0,DAY_DATA_ADDR,ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=LOAD_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=LOAD_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(LoadRecord_Time,Record_buf,8); 
  return 0;
}

short Get_LoadData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>MAX_CH_NUM)
    return 0;
  E2P_RData(Record_buf,LoadRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=LOAD_RECORD_NUM;
  Read_Flash(buf,ch*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

int Load_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,LoadRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[1])
  {
    Num=LOAD_RECORD_NUM;
  }
  else
  {
    Num=Record_buf[0];
  }
  return Num;
}

short Read_LastData(int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>MAX_CH_NUM)
    return 0;
  E2P_RData(Record_buf,LoadRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  if(rec_ptr)
    rec_ptr--;
  else
    rec_ptr=LOAD_RECORD_NUM-1;
  rec_ptr %=LOAD_RECORD_NUM;
  Read_Flash(buf,ch*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

short Save_DayData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  
  E2P_RData(Record_buf,FrzdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  rec_ptr %=DAY_RECORD_NUM;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    DataFlash_Write(DAY_DATA_ADDR+i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,DAY_DATA_ADDR,HOUR_DATA_ADDR,ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=DAY_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=DAY_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(FrzdRecord_Time,Record_buf,8); 
  return 0;
}

short Get_DayData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>MAX_CH_NUM)
    return 0;
  E2P_RData(Record_buf,FrzdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=DAY_RECORD_NUM;
  Read_Flash(buf,DAY_DATA_ADDR+ch*DAY_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

int Day_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,FrzdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[1])
  {
    Num=DAY_RECORD_NUM;
  }
  else
  {
    Num=Record_buf[0];
  }
  return Num;
}

short Save_HourData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  
  E2P_RData(Record_buf,ShrpdRecord_Time,9); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  rec_ptr += Record_buf[1]*256;
  rec_ptr %=HOUR_RECORD_NUM;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    DataFlash_Write(HOUR_DATA_ADDR+i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,HOUR_DATA_ADDR,END_DATA_ADDR,ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=HOUR_RECORD_NUM)
    Record_buf[2]=1;
  rec_ptr %=HOUR_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  Record_buf[1]=(rec_ptr>>8)&0xff;
  memcpy(Record_buf+3,tmp_buf,6);
  E2P_WData(ShrpdRecord_Time,Record_buf,9); 
  return 0;
}

short Get_HourData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>MAX_CH_NUM)
    return 0;
  E2P_RData(Record_buf,ShrpdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  rec_ptr += Record_buf[1]*256;
  if(Record_buf[2])
  {
    rec_ptr = (rec_ptr+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=HOUR_RECORD_NUM;
  Read_Flash(buf,HOUR_DATA_ADDR+ch*HOUR_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

int Hour_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,ShrpdRecord_Time,9); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[2])
  {
    Num=HOUR_RECORD_NUM;
  }
  else
  {
    Num = Record_buf[0];
    Num += Record_buf[1]*256;
  }
  return Num;
}

short Save_MonthData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  
  E2P_RData(Record_buf,MonthdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  if(rec_ptr>=MONTH_RECORD_NUM)
    rec_ptr = 0;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    E2P_PWData(i*MONTH_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,tmp_buf,E2ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=MONTH_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=MONTH_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(MonthdRecord_Time,Record_buf,8); 
  return 0;
}

short Get_MonthData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>8)
    return 0;
  E2P_RData(Record_buf,MonthdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=MONTH_RECORD_NUM;
  E2P_PRData(buf,ch*MONTH_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,E2ONE_RECORD_LEN);
  return E2ONE_RECORD_LEN;
}

int Month_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,MonthdRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[1])
  {
    Num=MONTH_RECORD_NUM;
  }
  else
  {
    Num = Record_buf[0];
  }
  return Num;
}

short Save_RandData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //�롢�֡�ʱ���ա��¡��ꡢ��¼ 
  
  E2P_RData(Record_buf,RandRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  rec_ptr = Record_buf[0];
  if(rec_ptr>=RAND_RECORD_NUM)
    rec_ptr = 0;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    E2P_PWData(RAND_DATA_ADDR+i*RAND_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,tmp_buf,E2ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=RAND_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=RAND_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(RandRecord_Time,Record_buf,8); 
  return 0;
}
short Get_RandData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>MAX_CH_NUM)
    return 0;
  E2P_RData(Record_buf,RandRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=RAND_RECORD_NUM;
  E2P_PRData(buf,RAND_DATA_ADDR+ch*RAND_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,E2ONE_RECORD_LEN);
  return E2ONE_RECORD_LEN;
}
int Rand_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,RandRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[1])
  {
    Num=RAND_RECORD_NUM;
  }
  else
  {
    Num = Record_buf[0];
  }
  return Num;
}

  
    
int Get_LoadFile_Msg_Len(void)
{
  int Len,rec_ptr;
  unsigned char Record_buf[16];
  E2P_RData(Record_buf,LoadRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[1])
     rec_ptr = LOAD_RECORD_NUM;
  else
    rec_ptr = Record_buf[0];
  Len = 51+rec_ptr*189; 
  return Len; 
}

int Get_LoadFile_Xml_Len(void)
{
  int Len,rec_ptr;
  unsigned char Record_buf[16];
  E2P_RData(Record_buf,LoadRecord_Time,8); //��¼ָ�� �롢�֡�ʱ���ա��¡���
  if(Record_buf[1])
     rec_ptr = LOAD_RECORD_NUM;
  else
    rec_ptr = Record_buf[0];
  Len = 100+rec_ptr*256; 
  return Len; 
}

void EC_MeasA(void)
{
  int i;
  unsigned short* ECRamBufAdsPtr;
  unsigned char* ECRamBufChkAdsPtr;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned int Ps2;
  Ps2 = MSpec.RMeterConst/1000;
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
      E2P_WData(ECEAds,(unsigned char*)ECRAds,4);
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
    E2P_WData(ECEAds,tmpbuf,4);
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
    E2P_RData((unsigned char*)ECRAds,ECEAds,4);
  }
}

void Read_E2R1()
{
  int i;
  unsigned short m_e2_ptr;
  unsigned char ptr_buf[4];
  memset(ptr_buf,0,8);
  for( i=0;i<24;++i)
  {
    m_e2_ptr = s_PnEvt[i].E2_PTR;
    E2P_WData(m_e2_ptr,ptr_buf,2);
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
    ATT7022RdReg(ATVoltFlag,(unsigned char*)&(SM.State[i]),i);
    ATT7022RdReg(ATPZ,(unsigned char*)&tmp_p,i);
    tmp_p &= 0xffffff;
    if(tmp_p)
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
    ATT7022RdReg(ATQZ,(unsigned char*)&tmp_p,i);
    tmp_p &= 0xffffff;
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
    }
#if 0   
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
            if(abs(si_val[j])>40)
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
    unsigned short year;
    Flag.Clk &= ~F_Min;
    if((Flag.Power & F_PwrUp) == 0)
      return;
    //GetTime();
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
      Save_Data(Time_buf);
    }
    
    Save_RandData(Time_buf);
 //   Save_MonthData(Time_buf);
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

void main(void)
{
  unsigned int i;
  unsigned char flash_id[16];
  HT_FreeDog();
  //i = 5000;
  i=E2END_DATA_ADDR;
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
    VarInit();
    GetTime();
    MoveCurrentTimeBCD_Hex();																									//10.11.22
    while (1)
    {
      HT_FreeDog();						
      if(((Flag.Power & F_PwrUp) == 0) && ( PowerCheck() == 1 ))		
      {
       // Flag.BatState=0;
        PwrOnInit();	
        InitPara();			
        InitPara5();
        Serial_Open(2,9600,8,UartParity_Disable);
	InitPara6();
       // Serial_Write(2,"Start Debug\r\n",13);   
        InitIEC101Prot();
        Read_FlashID(flash_id);
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
        for(i=0;i<MAX_CH_NUM;i++)
        {
          ATT7022Init(i);	//Test
        }
        break;
      }	
      if(((Flag.Power & F_PwrUp) != 0) && ( PowerCheck() == 0 ))
      {
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
      //if (Flag.Clk & F_Min) ProcMin();
      //if (Flag.Clk & F_Hour) ProcHour();
      //if (Flag.Clk & F_Day) ProcDay();
      
//      if(Flag.Power & F_PwrUp)
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

