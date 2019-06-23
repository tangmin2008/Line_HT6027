#include "TypeE2p.h"
#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
//#include "Measure.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "TypeFRAM.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
#include "RsComm.h"
#include "Initial.h"
#include "Time.h"
#include "Measure.h"
#include "Power.h"
#include "FreezeData.h"
#include "math.h"					//SmallCurrent
#include "cpucard.h"
#include "YearSave.h"									//�¹���	//13.11.29

#if ( LocalSmartMeter == YesCheck )
short JudgeYearSaveDay( unsigned char* Source )					//�¹���	//13.11.28
{
	if(( AllIsBCD( Source, 3 ) != 0 )||( JudgeDayH( Source ) != 0 )||( *( Source+2 ) == 0 )||( *( Source+2 ) > 0x12 ))	//���ڷǷ�
	{
		return 1;	
	}	
	else return 0;	
}	

unsigned char GetYearMonSaveMode( unsigned char* YearSaveDay )	//�¹���	//13.11.28	
{
	short i;
	unsigned char YearMonSaveMode;
	
	YearMonSaveMode = 0x00;									//�½���
	for( i=0;i<4;i++ )
	{
		if( JudgeYearSaveDay( YearSaveDay + i*3 ) == 0 )
		{
			YearMonSaveMode = 0x55;							//�����	
			break;	
		}	
	}	
	return YearMonSaveMode;
}	

void MonSaveBarely( void )												//�¹���	//13.11.29
{
	unsigned char Buff[4];
	unsigned char* SaveTime;
	
	SaveTime = Buff;

	RAM_Write( SaveTime, Clk.Hour, 4 );	
	E2P_WData( LastSaveTime, SaveTime, 4 );								//�����ϴ������ʱ��Ϊ��ǰʱ��
//	ECSave();
	RecordFreeze( SaveDayFreeze_No, DIRECT );			//�����ն���
	MD_Rest(0);															//������һ�ν����豣�浱ǰ����
//	CreateMonthSaveTimeRec( MonthSaveTime_No, SaveTime );	
	RecordFreeze( StepSaveSWFreeze_No, DIRECT );						//���ݽ��㶳��
	MoveSaveDay1EC();													//���浱ǰ�����½������					
}

void YearSaveBarely( void )												//�¹���	//13.11.29
{
	unsigned char Buff[4];
	unsigned char* SaveTime;
	
	SaveTime = Buff;
	
	RAM_Write( SaveTime, Clk.Hour, 4 );	
	E2P_WData( LastYearSaveTime, SaveTime, 4 );							//�����ϴ������ʱ��Ϊ��ǰʱ��
//	CreateYearSaveTimeRec( YearSaveTime_No, SaveTime );			
	RecordFreeze( StepSaveSWFreeze_No, DIRECT );						//���ݽ��㶳��
	MoveYearSaveDayEC();												//���㵱ǰ��������ڵ���
}	

void YearMonSaveDaySwitchProcess( unsigned char* OldData, unsigned char* NewData )		//�¹���	//13.11.28	
{
	unsigned char OldYearMonSaveMode;
	unsigned char NewYearMonSaveMode;
	
	OldYearMonSaveMode = GetYearMonSaveMode( OldData );	
	NewYearMonSaveMode = GetYearMonSaveMode( NewData );	
	ICComm.YearMonSaveMode = NewYearMonSaveMode;
	
	if( OldYearMonSaveMode == NewYearMonSaveMode )						//�л�ǰ��������Ͳ���
	{
		if( NewYearMonSaveMode	== 0x00 ) return;						//�л�ǰ���Ϊ�½���
		else															//�л�ǰ���Ϊ�����
		{
			if( Data_Comp( OldData, NewData, 12 ) == 0 ) return;		//�л���������ղ���
			else														//�л�ǰ��������ղ�һ�£�	
			{
				YearSaveBarely();										//��������㡣			
			}				
		}	
	}
	else if( OldYearMonSaveMode == 0x00 )								//�л�ǰΪ�½��ݡ��л���Ϊ�����
	{
		MonSaveBarely();												//���浱ǰ�����½������,ͬʱת��12���µ���
		MoveYearSaveDayEC();											//���㵱ǰ��������ڵ���
	}
	else																//�л�ǰΪ����ݡ��л���Ϊ�½���	
	{
		YearSaveBarely();												//��������㡣
	}		
}	

void YearDec( unsigned char *Data )								//�¹���	//13.11.28
{
	unsigned char Temp;

	Temp = BCD_Byte( *Data );		

	if( Temp == 0x00 ) Temp = 99;			//00��
	else Temp -= 1;							//���1	
	
	*Data = Byte_BCD( Temp );		

	return;
}

void YearInc( unsigned char *Data )								//�¹���	//13.11.28
{
	unsigned char Temp;

	Temp = BCD_Byte( *Data );		

	Temp += 1;
	if( Temp >= 100 ) Temp = 0;
	*Data = Byte_BCD( Temp );		

	return;
}

//ˢ���ϴ������ʱ��
void UpdateLastYearSaveTime( unsigned char* YearSaveDayBuff )			//�¹���	//13.11.28
{
	unsigned char Buff[100];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char Buff3[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SaveTime;
	unsigned char* Ptr;	
	unsigned short i,j;
	unsigned char SaveDayBuff[12];
	unsigned char YearSaveCnt;
	unsigned char Year;
	
	LastTime = Buff1;
	CurTime = Buff2;
	SaveTime = Buff3;
	
	Ptr = Buff;
	
	E2P_RData( LastTime, LastYearSaveTime, 4 );						//�ϴ������ʱ��
	RAM_Write( CurTime, Clk.Hour, 4 );								//��ǰʱ��
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) return;
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
		E2P_WData( LastYearSaveTime, LastTime, 4 );							
		return;				
	}
	
	RAM_Write( SaveDayBuff, YearSaveDayBuff, 12 );

	RAM_Fill( Ptr, 100 );
	Year = Clk.Year[0];
	for( i=0;i<5;i++ )												//ȡ�����5��Ľ����ʱ��	
	{
		for( j=0;j<4;j++ )
		{
			RAM_Write( Ptr+(4-i)*16+j*4, SaveDayBuff+j*3, 3 );		//ÿ��4������ʱ��					//15.05.15
			*( Ptr+(4-i)*16+j*4+3 ) = Year;							//��ǰ����5��Ľ���ʱ��				//15.05.15
		}	
		YearDec( (unsigned char*)&Year );							//����1��		
	}	
	
	YearSaveCnt = 0;
	for( i=0;i<20;i++ )
	{
		TestTime = Ptr+(19-i)*4;									//�ӵ�ǰ��ĵ��ĸ�������տ�ʼ����	
		if( DateTime4ValueCheck( TestTime ) != 0 ) continue;		//�������ڵ�ʱ��Ƿ�����Ҫ����ĳһ�����ղ�����ʱ 	//����9999��FFFF���ġ�����
		if( Data_Comp( TestTime, CurTime, 4 ) > 0 ) continue;		//�������ڵ�ʱ����ڵ�ǰʱ�䣬��δ��ʱ�䣬����������ж�
		
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//�������ڵ�ʱ������ϴν���ʱ�䣬����Ҫ�����ʱ���
		{
			RAM_Write( SaveTime, TestTime, 4 ); 					//������ʱ���	
			YearSaveCnt += 1;
			if( YearSaveCnt >= 4 ) break;							//��ಹ��4�ν���
		}
		else break;													//�����Ѿ��������ʱ����
	}
	
	if( YearSaveCnt != 0 )											
	{
		if( *SaveTime == 0 )
		{
			DayDec( SaveTime+1 ); 					
			*SaveTime = 0x23; 										//13.10.06
		}	
		else *SaveTime = Byte_BCD( BCD_Byte(*SaveTime)-1 );			//���ϴν���ʱ�䱣��Ϊ�Ȳ⵽������Ľ������һ��Сʱ������Ч���в�����
		if( Data_Comp( SaveTime, LastTime, 4 ) == 1 )				//��������������������ϴν���ʱ�䣬������ʱ����Ϊ�ϴν���ʱ�䡣
		{
			E2P_WData( LastYearSaveTime, SaveTime, 4 );
		}	
	}		
}

/*
void CreateYearSaveTimeRec( unsigned short EventNo, unsigned char* SaveTime )			//�¹���	//13.11.28
{
	unsigned char Buff[sizeof(YEARSAVETIME_REC)];
	YEARSAVETIME_REC* RecPtr;
	long LastPt;
	
	RecPtr = (YEARSAVETIME_REC*)Buff;

	LastPt = PtAddrChk( EventNo, Next );			
	if( LastPt == -1 ) return;
	
	RAM_Write( RecPtr->HappenDate, SaveTime, 4 );
	GetYearCycleInsideEC( RecPtr->StaECPp0, 0, 1 );
	E2P_WData( (unsigned short)LastPt, RecPtr->HappenDate, sizeof(YEARSAVETIME_REC ));
}
*/

//YearSaveDayBuff: ����ĸ����������ַ
//YearSaveCnt������������
short AdjustYear( unsigned char* YearSaveDayBuff, short* YearSaveCnt )					//�¹���	//13.11.28
{
	unsigned char Buff[32];
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
	
	E2P_RData( LastTime, LastYearSaveTime, 4 );							//�ϴν���ʱ��
	RAM_Write( CurTime, Clk.Hour, 4 );								//��ǰʱ��
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) 
	{
		*YearSaveCnt = 0;											//��ǰʱ��Ƿ��������㡣		
		return 0;				
	}	
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//��ǰʱ����С���ϴν���ʱ�䣬���ϴν���ʱ��=��ǰʱ��
		E2P_WData( LastYearSaveTime, LastTime, 4 );							
		*YearSaveCnt = 0;											
		return 0;				
	}	

	for( i=0;i<8;i++ )
	{
		RAM_Write( SixSaveDay+3+i*4, LastTime+3, 1 );				//�ϴ������ʱ�������������
	}	

	for( i=0;i<4;i++ )
	{
		RAM_Write( SixSaveDay+i*4, YearSaveDayBuff+i*3, 3 );		//�ϴν���ʱ������º��������������ǰ����������
		RAM_Write( SixSaveDay+16+i*4, YearSaveDayBuff+i*3, 3 );		//�ϴν���ʱ������µ���һ���º�������������ɺ�����������
		YearInc(SixSaveDay+19+i*4);
	}	
	
	TestTime = SixSaveDay;
	for( i=0;i<8;i++ )
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
//				CreateYearSaveTimeRec( YearSaveTime_No, TestTime );	//�¹���				
				RecordFreeze( StepSaveSWFreeze_No, DIRECT );		//���ݽ��㶳��
				if( *YearSaveCnt != 0 )
				{
					*YearSaveCnt -= 1;
					if( *YearSaveCnt == 0 ) 						//������������4�Σ�4�κ󣬽���ǰʱ������ϴν���ʱ��
					{												//����ֹ���㡣	
						RAM_Write( TestTime, CurTime, 4 ); 
					}
				}	
				E2P_WData( LastYearSaveTime, TestTime, 4 );			//���㣬�ò������ڵ�ʱ����Ϊ�ϴν���ʱ��	
				return (i%4)+1;										//����1����һ������ա�2���ڶ�������ա�3������������ա�3�������������
			}	
		}	
		TestTime += 4;
	}	
	*YearSaveCnt = 0;												//�����Ͻ���������		
	return 0;	
}

void MoveYearSaveDayEC( void )											//�¹���	//13.11.28
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	
	Ptr = Buff;
	RAM_Fill( Ptr, 6 );
	
	E2P_RData( Ptr, L1_YearSaveDayEC_Pp0, 4 );
	E2P_WData( L2_YearSaveDayEC_Pp0, Ptr, 4 );
	E2P_RData( Ptr, L1_YearSaveDayEC_Pn0, 4 );
	E2P_WData( L2_YearSaveDayEC_Pn0, Ptr, 4 );
	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	E2P_WData( L1_YearSaveDayEC_Pp0, Ptr, 4 );
	Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	E2P_WData( L1_YearSaveDayEC_Pn0, Ptr, 4 );
	
}	

unsigned char GetCurYearMonSaveMode( unsigned char* Dest )						//�¹���	//13.11.29
{
	short i;
	unsigned char YearMonSaveMode;
	
	for( i=0;i<4;i++ )
	{
		E2P_RData( Dest+i*3, Case1YearSaveDay1+i*4, 3 );		
	}	
	YearMonSaveMode = GetYearMonSaveMode( Dest );
	
	return YearMonSaveMode;
}	

short Year_Save(void)															//�¹���	//13.11.28
{
	short i,k;
	unsigned char Buff[12];
	short Year_SaveCnt;
	
	ICComm.YearMonSaveMode = GetCurYearMonSaveMode( Buff );
	if( ICComm.YearMonSaveMode == 0 ) return 0;
	
	UpdateLastYearSaveTime( Buff );											

	Year_SaveCnt = 4;														//������������4��
	for( k=0;k<4;k++ )														//������������4��
	{	
		if((Flag.Power & F_PwrUp) == 0 ) break;								//��⵽��������жϽ���
		i = AdjustYear( Buff, &Year_SaveCnt );
		if( i != 0 )
		{
			MoveYearSaveDayEC();											
		} 	
		if( Year_SaveCnt == 0 ) break;
	}

	return 0;
}

#endif

































