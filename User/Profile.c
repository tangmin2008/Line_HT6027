#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ht6xxx_reset.h"
#include "TypeMeter.h"
#include "TypeRAM.h"
#include "TypeE2p.h"
#include "ht6xxx_lib.h"					
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

#define _R_PROFILE_RECORD   E2P_RData
#define _W_PROFILE_RECORD   E2P_WData

#define EC_E2_W E2P_WData
#define EC_E2_R E2P_RData

void GetCurvePara( CURVEPARA* CurvePara )
{
  unsigned char Buff[4];
  unsigned char* Ptr;
  
  Ptr = Buff;
  CurvePara->StartPageNo = ProfileTab[CurvePara->ChannelNo].StartAddr;		//�������ߴ洢�ռ���ʼҳ��
  CurvePara->Pages = ProfileTab[CurvePara->ChannelNo].EndAddr;				//�������ߴ洢�ռ���ҳ��			
  CurvePara->RecdLenth = ProfileTab[CurvePara->ChannelNo].RecdLenAddr; //��������ÿ����¼����
  CurvePara->RecsInPage = 4096 / CurvePara->RecdLenth;							//��������ÿҳ��¼��
  CurvePara->MaxCunt = CurvePara->Pages * CurvePara->RecsInPage;					//�������ߴ洢�ռ�������¼��
  CurvePara->RecdCunt = ( CurvePara->Pages - 1 ) * CurvePara->RecsInPage;			//�������ߴ洢�ռ��������Ч��¼��	//1ҳ��Ϊ��ת  
  _R_PROFILE_RECORD( (unsigned char*)&CurvePara->AvailRecs, ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, 2 );	//�Ѵ��¼��
  if( CurvePara->AvailRecs > CurvePara->RecdCunt ) CurvePara->AvailRecs = 0;										//�Ѵ��¼�����������¼�����Ƿ����Ѵ��¼�����㡣��ʾû����Ч��¼
  _R_PROFILE_RECORD( (unsigned char*)&CurvePara->CurRecNo,ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, 2 );
}	

//CurvePara:�������߲���
//type: -1:�²���һ����¼��������ָ����¼��
//����ֵ��ָ�����ߵ�ָ����¼����FLASHоƬ�е������ַ�� -1���ü�¼������
 long LoadPtrChk( CURVEPARA* CurvePara, long Type )
{
  unsigned char* ptr;
  unsigned char buff[4];
  unsigned long Temp,Temp1;
  unsigned long CurRecNo;			//��ǰ��¼��
  ptr=buff;
  HT_FreeDog();
  if( Type == -1 )
  {
    CurvePara->CurRecNo += 1;													//�·���һ����¼
    if( CurvePara->CurRecNo >= CurvePara->MaxCunt ) CurvePara->CurRecNo = 0;	//�����ܼ�¼����ָ���һ����
    if( CurvePara->AvailRecs == 0 ) CurvePara->CurRecNo = 0;			//��ͨ����һ�β�����¼��ָ���һ����
																					//��������ڵ�Ŀ���¼ָ��ҳ���߼����ַʱ������ǰҳ�Ȳ�������д�����ݣ���дFLASH������ϡ�		
    _W_PROFILE_RECORD(ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, (unsigned char*)&CurvePara->CurRecNo, 2 );//���浱ǰ��¼��		
    if( CurvePara->AvailRecs < CurvePara->RecdCunt )							//�Ѵ��¼��С�ڳ������Ƽ�¼��
    { 												
      CurvePara->AvailRecs++;													//�Ѵ��¼����1�����EEPROM	
      _W_PROFILE_RECORD( ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, (unsigned char*)&CurvePara->AvailRecs, 2 );	
      _R_PROFILE_RECORD( ptr,ProfileTab[CurvePara->ChannelNo].EntriesUseAddr,  2 );
    }
    CurRecNo = CurvePara->CurRecNo;			//��ǰ��¼��		
  }
  else
  {
    if( Type >= CurvePara->AvailRecs) return -1;									//������¼���ޣ����ؿա�	
    if( CurvePara->CurRecNo >= Type ) 
      CurRecNo = CurvePara->CurRecNo - Type;		//���ռ�¼��С�ڵ�ǰ��¼�ţ�ֱ�Ӵӵ�ǰ��¼��ǰ�����ҵ�Ŀ���¼
    else 
      CurRecNo = CurvePara->CurRecNo + CurvePara->MaxCunt - Type;
  }
  
  Temp1 = (CurRecNo % CurvePara->RecsInPage)*CurvePara->RecdLenth;	//Ŀ���¼�׵�ַ��������ҳ�ڵ��߼���ַ
  Temp = CurvePara->StartPageNo;					//���ɼ�¼�洢����FLASHоƬ�ڵ���ʼҳ��	
  Temp += CurRecNo/CurvePara->RecsInPage;				//Ŀ���¼��FLASHоƬ�ڵ�ҳ��
  Temp = Temp * 4096 + Temp1;						//Ŀ���¼�׵�ַ��FLASHоƬ�������ַ
  return Temp;								//���ص�ǰ��ָ��ļ�¼��
}
void LoadRecord( unsigned short E2_Addr,unsigned char *Buff)
{
  unsigned short ChannelNo;
  unsigned char* Point;
  unsigned long LongBuff[9];
  CURVEPARA* CurvePara;
  unsigned long Value;
  Point = Buff;
  
  for(ChannelNo=0;ChannelNo<113;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>112)
    return;
  CurvePara = (CURVEPARA*)LongBuff;
  HT_FreeDog();
  CurvePara->ChannelNo = ChannelNo;
  GetCurvePara( CurvePara );
  if( CurvePara->Pages == 0 ) return;		//�����߷���Ŀռ�Ϊ��
  Value = LoadPtrChk( CurvePara, -1 );
  DataFlash_Write( Value, Point, CurvePara->RecdLenth);
  return;
}

int ReadRecord( unsigned short E2_Addr,unsigned char *Buff,long LoadRecNo)
{
  unsigned long LongBuff[9];
  unsigned short ChannelNo;
  CURVEPARA* CurvePara;
  long LoadAddr;
  if( LoadRecNo < 0 ) return 1;					//�Ƿ�
  for(ChannelNo=0;ChannelNo<113;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>112)
    return 1;
  CurvePara = (CURVEPARA*)LongBuff;
  HT_FreeDog();
  CurvePara->ChannelNo = ChannelNo;
  GetCurvePara( CurvePara );
  if(CurvePara->Pages == 0) 
    return 1;		//�����߷���Ŀռ�Ϊ��
  LoadAddr = LoadPtrChk( CurvePara, LoadRecNo );
  if( LoadAddr != -1 )
  {
    Read_Flash( Buff, LoadAddr,CurvePara->RecdLenth);
    return 0;
  }
  return 2;
}

int Get_Record_Num(unsigned short E2_Addr)
{
  unsigned long LongBuff[9];
  unsigned short ChannelNo;
  CURVEPARA* CurvePara;	
  for(ChannelNo=0;ChannelNo<113;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>112)
    return 0;
  CurvePara = (CURVEPARA*)LongBuff;
  HT_FreeDog();
  CurvePara->ChannelNo = ChannelNo;
  GetCurvePara( CurvePara );
  return CurvePara->AvailRecs;
}