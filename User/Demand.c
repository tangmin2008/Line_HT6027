#include "TypeRAM.h"
#include "TypeE2p.h"
#include "Data.h"
#include "ht6xxx_lib.h"
#include "COSEMComm.h"
#include "Mem.h"
#include "ProfileGeneric.h"
#include "Demand.h"
#include "General.h"
#include "Time.h"

void Action_Demand_reg_Reset( short ItemNo )
{
	unsigned short i;
//	MDBUFF* MDBuffPtr; 
	HT_FreeDog();
	/*
	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[ItemNo].MDBufAds; 
	MDBuffPtr->MD_BufAds = 0;										//��ǰ������������ַ	
	for(i=0;i<30;i++)
	{
		MDBuffPtr->MDBuf[i] = 0;									//����ʱ����������������
	}
	MDBuffPtr->MD_Cum = 0;											//����ʱ����������
	MDBuffPtr->period_counter = 0;
	MDBuffPtr->current_average_value = 0;
	MDBuffPtr->last_average_value = 0;
	MDBuffPtr->MD_Cunt = 0;											//�����Ӽ�������ʱ��Ҫ��ǰ��ȡ���������建��������       
	MDBuffPtr->MD_CuntT = 0;                                        //�����Ӽ����������ʱ��Ҫ��ǰ��ȡ���������建��������   
	MDBuffPtr->Status = 0;
	MDBuffPtr->FeeNo = SM.FeeNo;									//�������ʺ�
	RAM_Write( MDBuffPtr->capture_time, &Clk.YearH, 12);			//��ǰ��ʼʱ��
	RAM_Write( MDBuffPtr->start_time_current, &Clk.YearH, 12);		//��ǰ��ʼʱ��
	*/
	RAM_Fill(DLMS_Demand_regTab[ItemNo].MDEAds,5);
	RAM_Fill(DLMS_Demand_regTab[ItemNo].MDTEAds,7);
	
}

unsigned char SetDemand_registerClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned short Addr;
    unsigned short i;
    unsigned long Value;
	MDBUFF* MDBuffPtr; 
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	
	i = GetDemandRegTabItemNo( Logical_Name );							//���Ҷ�Ӧ��OBIS��
	if( i == DLMS_DemandRegSum ) { return object_undefined; }

	switch( *Attribute )
	{
		case 8:													//period
			if( *WriteBufAds != D_Unsigned32 ) 
			{
				ErrorByte = other_reason;
				break;
			}	
		//	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[i].MDBufAds; 
			Addr = DLMS_Demand_regTab[i].EperiodAds;
			E2P_RData( Ptr, Addr, 4 );
			//=================
			if(Data_CompHL(WriteBufAds+1,Ptr,4)!=0)
	//		if(MDBuffPtr->period != Value)   //��С����ʱ�䲻ͬ�������ԭ�д洢��������
				Action_Demand_reg_Reset( i );	
			// ------------------
	//		MDBuffPtr->period = Value;
			RAM_Write( Ptr, WriteBufAds+1, 4 );
			SwapData( Ptr, 4 );
			RAM_Write( (unsigned char*)&Value, Ptr, 4 );	
			E2P_WData( Addr, Ptr, 4 );
			break;
		case 9:													//number_of_periods
			if( *WriteBufAds != D_Unsigned16 ) 
			{
				ErrorByte = other_reason;
				break;
			}	
		//	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[i].MDBufAds; 
			RAM_Write( Ptr, WriteBufAds+1, 2 );
			SwapData( Ptr, 2 );
			Value = 0;
			RAM_Write( (unsigned char*)&Value, Ptr, 2 );	
			Addr = DLMS_Demand_regTab[i].Enumber_of_periodsAds;
			if( Value > 30 )
			{
				ErrorByte = other_reason;
				break;
			}			
		//	MDBuffPtr->number_of_periods = Value;
			E2P_WData( Addr, Ptr, 2 );
			break;		
		default: 
			ErrorByte = read_write_denied;
			break;			
	}
	return ErrorByte;
}
void DateInc( unsigned char* Date )
{
	unsigned short Temp;
	
	HT_FreeDog();	//Watchdog
	Temp = GetBuffToInteger( Date );
	
	*(Date+3) += 1;
	if( *(Date+3) > ((( *(Date+2)==2 ) && ( Temp & 0x03 )==0 )? 29:MonTab[*(Date+2)]))
	{
		*(Date+3) = 1;
		*(Date+2) += 1;
		if( *(Date+2) > 12 )
		{						// Month
			*(Date+2) = 1;
			Temp += 1;
			FillIntegerToBuff( Date, Temp );
		}
	}		
}

void Data_timeAddSec( unsigned char* DateTime, unsigned long Seconds )
{
	unsigned long Value1;
	unsigned long Value2;
	unsigned short Temp;
	
	Value1 = *(DateTime+7);				//��
	Value1 += Seconds;
	*(DateTime+7) = Value1 % 60;
	Value2 = Value1 / 60;
	if( Value2 == 0 ) return;

	Value1 = *(DateTime+6);				//��
	Value1 += Value2;
	*(DateTime+6) = Value1 % 60;
	Value2 = Value1 / 60;
	if( Value2 == 0 ) return;

	Value1 = *(DateTime+5);				//ʱ
	Value1 += Value2;
	*(DateTime+5) = Value1 % 24;
	Value2 = Value1 / 24;	
	if( Value2 == 0 ) return;
	
	Value1 = *(DateTime+4);				//��
	Value1 -= 1;
	Value1 += Value2;
	*(DateTime+4) = ( Value1 % 7 ) + 1; //1~7

	Temp = Value2;						//��
	while( Temp > 0 )
	{
		DateInc( DateTime );		
		Temp--;							//13.04.10					
	}	
}	
void MD_Compare(unsigned char Item, unsigned char type )
{
	unsigned char Item_Num=0;
	unsigned long Value,Old,temp_data;
	if(type==0)
	    Item_Num = (Total_Fee+1)*Item;
	else
		Item_Num = (Total_Fee+1)*Item+SM.FeeNo;
	
	RAM_Write((unsigned char *)&Value,DLMS_Demand_regTab[Item].MDEAds,4);
	E2P_RData( (unsigned char*)&Old, DLMS_MDTab[Item_Num].Addr, 4);
   if(Value>Old)
	{
//	E2P_WData(DLMS_MDTab[Item_Num].Time_Addr,&Clk.HourH,4);
//	E2P_WData(DLMS_MDTab[Item_Num].Date_Addr,&Clk.YearH,5);

	E2P_WData(DLMS_MDTab[Item_Num].Addr,DLMS_Demand_regTab[Item].MDEAds,4);
	E2P_WData(DLMS_MDTab[Item_Num].MDTAddr,DLMS_Demand_regTab[Item].MDTEAds,6);
	//�����ۼ��������
	E2P_RData( (unsigned char*)&temp_data, DLMS_CUMDTab[Item_Num].Addr, 4);
	temp_data+=Value;
	E2P_WData(DLMS_CUMDTab[Item_Num].Addr,(unsigned char*)&temp_data,4);
	
	//���������ۼ��������
	//E2P_RData( (unsigned char*)&temp_data, DLMS_MAXTab[Item_Num].ContCum_Addr, 4);
	//temp_data+=Value;
	//E2P_WData(DLMS_MAXTab[Item_Num].ContCum_Addr,(unsigned char*)&temp_data,4);

	}
}

void MD_ParaInitial( void )
{
	unsigned short i;
	MDBUFF* MDBuffPtr; 
	unsigned short PAddr;
	unsigned short NAddr;
	unsigned short Temp;
	
//	E2P_RData((unsigned char*)&Temp,Demand_Rolling_Time,1);
	
	for( i=0;i<DLMS_DemandRegSum;i++ )
	{
	/*	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[i].MDBufAds; 
		PAddr = DLMS_Demand_regTab[i].EperiodAds; 
		NAddr = DLMS_Demand_regTab[i].Enumber_of_periodsAds; 
		E2P_RData( (unsigned char*)&MDBuffPtr->period, PAddr, 4 );
		E2P_RData( (unsigned char*)&Temp, NAddr, 2 );
	//	MDBuffPtr->period = 60;
	//	MDBuffPtr->number_of_periods = Temp;
	*/
	
	RAM_Fill(DLMS_Demand_regTab[i].MDEAds,5);
	RAM_Fill(DLMS_Demand_regTab[i].MDTEAds,7);
	}
}	
#if 0
short Get_current_average_value( unsigned short Item )
{
	unsigned short j,k;
	unsigned long PulseCum;				
	unsigned long Value1=0,Value2=0;		
	unsigned char  Cunt;										//��������ʱ��Ҫ��ǰ��ȡ�������������建��������
	MDBUFF* MDBuffPtr; 
	///unsigned long  LValue;	
	unsigned long long LValue;
	unsigned char number_of_periods;
	unsigned short PS32;

	HT_FreeDog();	//Watchdog
	
	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[Item].MDBufAds; 
	Cunt = MDBuffPtr->MD_Cunt;		
	PulseCum = MDBuffPtr->MD_Cum;
	number_of_periods = MDBuffPtr->number_of_periods;
	
	if(( Cunt == number_of_periods )&&( Cunt != 0 )) Cunt -= 1;  
	for( j=0;j<Cunt;j++)
	{
		if( MDBuffPtr->MD_BufAds < j ) k = MDBuffPtr->MD_BufAds + 30; 
		else k = MDBuffPtr->MD_BufAds;
    
		Value1 = MDBuffPtr->MDBuf[k-j];
		PulseCum += Value1;	
//		Value2 += MDBuffPtr->period;
	}
		LValue = (unsigned long long)PulseCum * 3600 * 100;							//0.0001kwh		
	///LValue = (PulseCum * 3600*100  ) ;				//V10	2016-6-24-fire
//	Value2 *= PS32;
	Value2 = number_of_periods;//MDBuffPtr->number_of_periods;
	Value2 *= MDBuffPtr->period;
	Value2 *= (PS32);
	if( Value2 == 0 ) MDBuffPtr->current_average_value = 0;
	else
	{
		LValue = LValue / Value2;
		Value1 = (unsigned long)LValue;
		MDBuffPtr->current_average_value = Value1;
	}	
	return 0;
}
#endif
void E2P_ECDataMove( unsigned short Dest, unsigned short Source,short DataLenth, short Cycle )
{
	unsigned char Buff[40];
	unsigned char* Point;
	short i;
	
	Point = Buff;
	for( i=0;i<Cycle;i++ )
	{
		E2P_RData( Point, Source, DataLenth );
		E2P_WData( Dest, Point , DataLenth );
		Dest += DataLenth +1;
		Source += DataLenth +1;	
	}
}	

void MD_Comp_bcd( unsigned long Value, short MD_Ads, short MDT_Ads )
{
	unsigned char Buff[4];
	unsigned char* Point;
	unsigned char i=0;
	unsigned long Old;
	Point=Buff;
	RAM_Fill(Point,4);
	E2P_RData( Point, MD_Ads , 3);//��������BCD��ʽ������ֵ
	Old = BCD4_Long(Point);
	///if( Data_Comp( PointSrc, Point, 3 ) == 1)
	if( Data_Comp( (unsigned char*)&Value, (unsigned char*)&Old, 3 ) == 1)
	{
		RAM_Fill(Point,4);
		Long_BCD4(Point,Value);
		E2P_WData( MD_Ads , Point, 3);				//д�������
		E2P_WData( MDT_Ads , &Clk.Min[0], 5);

		E2P_WData( CMon_MD_Top1, Point, 3 );
		E2P_WData( CMon_MDT_Top1, &Clk.Min[0], 5);
		//��ʷ��¼�������κ���
		for(i=0;i<15;i++)
		{	
			E2P_ECDataMove( CMon_MD_Top12 - i*5, CMon_MD_Top12 - (i + 1)*5,3, 1 );
			E2P_ECDataMove( CMon_MDT_Top12 - i*7, CMon_MDT_Top12 - (i+1)*7 ,5, 1 );
		}
	}
}
#if 0
short MD_MeasZF( unsigned short Item )							//��������
	{
		unsigned char Buff[4];
		unsigned char* Point;
		unsigned short j,k,m;
		unsigned long PulseCum[2];				
		unsigned long* PulseCumPt;				
		unsigned long Value1=0,Value2=0;		
		unsigned char*  MD_Cunt;									//��������ʱ��Ҫ��ǰ��ȡ�������������建��������
		unsigned char*  MD_CuntT;								//�����������ʱ��Ҫ��ǰ��ȡ�������������建��������
		MDBUFF* MDBuffPtr; 
		unsigned char number_of_periods;
		//unsigned long  LValue;
		unsigned long long LValue;
		
		unsigned short PS32;	
		PS32 = (Measur_Const/100);
		Point = Buff;
		PulseCumPt = PulseCum;
	
		MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[Item].MDBufAds; 
		MD_Cunt = &MDBuffPtr->MD_Cunt;		
		MD_CuntT = &MDBuffPtr->MD_CuntT;		
		number_of_periods = MDBuffPtr->number_of_periods;
		
		HT_FreeDog();	//Watchdog
		for( j=0;j<2;j++ )
		{
			*(PulseCumPt + j) = 0;
		}	
		
		*MD_Cunt += 1;
		if( *MD_Cunt >= number_of_periods )
		{
			Data_timeAddSec( MDBuffPtr->start_time_current, MDBuffPtr->period );		//��δ��Ųһ����������
			*MD_Cunt = number_of_periods;
		}	
			
		if( MDBuffPtr->FeeNo != SM.FeeNo )							//�����һ���������䷢���������л�
		{
			MDBuffPtr->FeeNo = SM.FeeNo;
			*MD_CuntT = 0;											//������������һ�����ʼ����
		}	
		else *MD_CuntT += 1;
		
		if( *MD_CuntT > number_of_periods ) *MD_CuntT = number_of_periods;
	
		for( j=0;j<*MD_Cunt;j++)
		{
			if( MDBuffPtr->MD_BufAds < j ) k = MDBuffPtr->MD_BufAds + 30; 
			else k = MDBuffPtr->MD_BufAds;
	
			Value1 = MDBuffPtr->MDBuf[k-j];
			*PulseCumPt += Value1;					
			if( *MD_CuntT > j ) *(PulseCumPt + 1 ) += Value1;	
		}		
	
		for( m=0;m<2;m++)
		{
			RAM_Fill( Point, 4 );																		
			Value2 = *(PulseCumPt + m); 	
			LValue = (unsigned long long)Value2 * 3600*100 ;									//0.0001kwh 	
		/// LValue = (Value2 * 3600 * 100 ) / (PS32/10);				//V10	2016-6-24-fire
			Value2 = number_of_periods;
			if( Value2 == 0 ) Value1 = 0;
			else
			{
				Value2 = Value2 * (PS32) * MDBuffPtr->period;
				LValue = LValue / Value2;
				Value1 = (unsigned long)LValue;
			}	
		/// Long_BCD4( Point, Value1 ); //ת����BCD����,��E2P�����������ֽڴ洢
		/// test_data = BCD4_Long( Point);
			///RAM_Write( Point, (unsigned char*)&Value1, 4);	//Hex����	
			//////////////д����/////////////////
			///E2P_WData( DLMS_Demand_regTab[Item].MDEAds , Point, 3);				
			///E2P_WData( DLMS_Demand_regTab[Item].MDTEAds , &Clk.Min[0], 5);
			////////////д�������////////////////
		
		///RAM_Write( Point, (unsigned char*)&Value1, 4);			
		if( m == 0 ) //��������
			{
					if(Item==0)
						MD_Comp_bcd( Value1, DLMS_MDTab[10].Addr, DLMS_MDTab[10].MDTAddr);//1-0:1.6.0.255  Active energy import (+A)
					else if(Item==1)
							MD_Comp_bcd( Value1, DLMS_MDTab[5].Addr, DLMS_MDTab[5].MDTAddr);//1-0:2.6.0.255  Active energy export (-A)
					else 
						MD_Comp_bcd( Value1, DLMS_MDTab[0].Addr, DLMS_MDTab[0].MDTAddr);//1-0:15.6.0.255  Active energy Combined total (|+A|+|-A|)
			}	
			else 
				{
				//������
				if(Item==0)
					MD_Comp_bcd( Value1, DLMS_MDTab[10+SM.FeeNo].Addr, DLMS_MDTab[10+SM.FeeNo].MDTAddr);//1-0:1.6.0.255  Active energy import (+A)
				else if(Item==1)
						MD_Comp_bcd( Value1, DLMS_MDTab[5+SM.FeeNo].Addr, DLMS_MDTab[5+SM.FeeNo].MDTAddr);//1-0:2.6.0.255	Active energy export (-A)
				else 
					MD_Comp_bcd( Value1, DLMS_MDTab[SM.FeeNo].Addr, DLMS_MDTab[SM.FeeNo].MDTAddr);//1-0:15.6.0.255	Active energy Combined total (|+A|+|-A|)
				}
				
			//����ֵ��д��eeprom�У����Դ˱����д洢��������Ҳ�Ƕ������ݣ�ʵʱ���ݡ� fire-2016-6-28
			MDBuffPtr->current_average_value = Value1;	//Hex����
			MDBuffPtr->last_average_value = Value1; 	//Hex����
			RAM_Write( MDBuffPtr->capture_time, &Clk.YearH, 12);		//��������ʱ�� //Hex����
			
		}
	
	  return 0;
	}

short MD_BlockMeas( unsigned short Item )	//�̶�ʱ����������
{
	unsigned char Buff[4];
	unsigned char* Point;
	unsigned short j,k,m;
	unsigned long PulseCum[2];				
	unsigned long* PulseCumPt;              
	unsigned long Value1=0,Value2=0;		
	unsigned char*  MD_Cunt;			//��������ʱ��Ҫ��ǰ��ȡ�������������建��������
	unsigned char*  MD_CuntT;			//�����������ʱ��Ҫ��ǰ��ȡ�������������建��������
	MDBUFF* MDBuffPtr; 
	unsigned char number_of_periods;
	//unsigned long  LValue;
	unsigned long long LValue;
	unsigned short PS32;
	Point = Buff;
	PulseCumPt = PulseCum;
	PS32 = Measur_Const;

	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[Item].MDBufAds; 
	MD_Cunt = &MDBuffPtr->MD_Cunt;		
	MD_CuntT = &MDBuffPtr->MD_CuntT;		
	number_of_periods = MDBuffPtr->number_of_periods;
		
	*MD_Cunt += 1;
		
	if( MDBuffPtr->FeeNo != SM.FeeNo )							//�����һ���������䷢���������л�
	{
		MDBuffPtr->FeeNo = SM.FeeNo;
		*MD_CuntT = 0;											//������������һ�����ʼ����
	}	
	else *MD_CuntT += 1;

	if( *MD_CuntT >= number_of_periods )//�ﵽ��Ҫ����Ҫ��Ĺ̶�ʱ��Σ���ʼ��������
		{ 

		for( j=0;j<2;j++ )
		{
			*(PulseCumPt + j) = 0;
		}	

		for( j=0;j<*MD_Cunt;j++)
		{
		/*	if( MDBuffPtr->MD_BufAds < j ) k = MDBuffPtr->MD_BufAds + 60; 
			else k = MDBuffPtr->MD_BufAds;

			Value1 = MDBuffPtr->MDBuf[k-j];
			*/
			Value1 = MDBuffPtr->MDBuf[j];
			*PulseCumPt += Value1;					
			if( *MD_CuntT > j ) *(PulseCumPt + 1 ) += Value1;	
		}		
		*MD_CuntT = 0;

		for( m=0;m<2;m++)
		{
			RAM_Fill( Point, 4 );																		
			Value2 = *(PulseCumPt + m);		
			LValue = (unsigned long long)Value2	* 3600*100 ;									//0.0001kwh		
		///	LValue = (Value2 * 3600 * 100 ) / (PS32/10);				//V10	2016-6-24-fire
			Value2 = number_of_periods;
			if( Value2 == 0 ) Value1 = 0;
			else
			{
				Value2 = Value2 * (PS32) * MDBuffPtr->period;
				LValue = LValue / Value2;
				Value1 = (unsigned long)LValue;
			}	

			MD_Compare( Value1, Item, m);
				
			//����ֵ��д��eeprom�У����Դ˱����д洢��������Ҳ�Ƕ������ݣ�ʵʱ���ݡ� fire-2016-6-28
			MDBuffPtr->current_average_value = Value1;	//Hex����
			MDBuffPtr->last_average_value = Value1; 	//Hex����
			RAM_Write( MDBuffPtr->capture_time, &Clk.YearH, 12);		//��������ʱ�� //Hex����
		
	}
}
  return 0;
}

short MD_Meas( void )											//��������
{
	unsigned char priod,method;
	unsigned short i;
	MDBUFF* MDBuffPtr; 
	
	HT_FreeDog();	//Watchdog
	
/*	if(SM.Demand_Interval_Remain>0)
		SM.Demand_Interval_Remain--;
	else
		{
		E2P_RData((unsigned char *) &priod,Demand_Rolling_Time,1);
		if(priod>60)
			{
			priod=60;
			E2P_WData(Demand_Rolling_Time,(unsigned char *) &priod,1);
			}
		SM.Demand_Interval_Remain = priod;
		SM.Demand_Interval_Remain*=60;
		}
	*/
	for( i=0;i<DLMS_DemandRegSum;i++ )
	{
		MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[i].MDBufAds; 
		
		if( MDBuffPtr->FeeNo != SM.FeeNo )							//�����һ���������䷢���������л�
		{
			MDBuffPtr->FeeNo = SM.FeeNo;
			MDBuffPtr->MD_CuntT=0;		//������������һ�����ʼ����
		}	
		MDBuffPtr->period_counter += 1;
		if( MDBuffPtr->period_counter >= MDBuffPtr->period )
		{
			MDBuffPtr->MD_BufAds++;
			if( MDBuffPtr->MD_BufAds >= priod ) MDBuffPtr->MD_BufAds = 0;
			MDBuffPtr->MDBuf[MDBuffPtr->MD_BufAds] = MDBuffPtr->MD_Cum;//1�����ڵ����������ݵ�������
			MDBuffPtr->MD_Cum = 0;
			MDBuffPtr->period_counter = 0;
			
		//	E2P_RData((unsigned char *)&method,Demand_Measur_Method,1);
		//	if(method==Rolling_method)
				MD_MeasZF(i);		//���ʽ��������
		//	else if(method==Interval_method)
		//		MD_BlockMeas(i);	//�̶�ʱ��μ��㷽ʽ
			/*	
			#if ( LastAveragePf ==	YesCheck )
				if(i==0) Get_LastAveragePf_value( 0 );
			#endif
			*/
//			RAM_Write( MDBuffPtr->start_time_current, &Clk.YearH, 12);				//��ǰ��ʼʱ��
//			Data_timeAddSec( MDBuffPtr->start_time_current, MDBuffPtr->period );		//��δ��Ųһ����������
		}
		else Get_current_average_value(i);	
	}
    return 0;	
}
#endif
short MD_Meas( void )											//��������
{
unsigned char i;
	for(i=0;i<5;i++)
		{
		MD_Compare(i, 0);
		}
        return 0;
}
void Clr_E2PECData( short Addr, short DataLenth, short Cylcle )
{
//  	unsigned char Buff[24];
  	unsigned char Buff[6];
  	unsigned char* Point;
  	unsigned char i;
  	
  	Point = Buff;
	
	RAM_Fill( Point, DataLenth );
	
	for( i=0;i<Cylcle;i++ )
	{
   		E2P_WData( Addr, Point, DataLenth );
   		///E2P_WECData( Addr, Point, DataLenth );
   		Addr += DataLenth+1;
	}	
}
void BCDTime_Reset( short Addr, short DataLenth, short Cylcle )
{
//		unsigned char Buff[24];
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char i;
	Ptr = Buff;
	
	for( i=0;i<Cylcle;i++ )
	{
		RAM_Write( Ptr, &Clk.Min[0], 5);			//��λʱ��
		E2P_WData( Addr, Ptr, DataLenth ); 
		Addr += DataLenth+1;
	}	
}

/*void MD_Rest( void )
*�������㣬�����洢��ֵ�Լ��洢ʱ�䣬������
*/
void MD_Rest( void )
{
	unsigned char i;
	unsigned short temp=0,Addr;
	unsigned char Buff[5];
	unsigned char* ptr;
	ptr=Buff;
	for( i=0;i<DLMS_DemandRegSum;i++ )
	{
		Action_Demand_reg_Reset( i );	//��������
	}		
	
	RAM_Fill(ptr,5);
/*	Addr = Del_MaxKW;
	for(i=0;i<30;i++)
		{
		E2P_WData(Addr,ptr,4);//�����������,���ۼƼ������ۼ��������
		Addr+=5;
		}
	E2P_WData(Time_Last_Reset,&Clk.HourH,4);
	E2P_WData(Date_Last_Reset,&Clk.YearH,5);	
	for(i=0;i<5;i++)
		{
		E2P_WData(Del_Time_MaxKW + 11*i,&Clk.HourH,4);
		E2P_WData(Del_Date_MaxKW + 11*i,&Clk.YearH,5);	
		E2P_WData(Rec_Time_MaxKW + 11*i,&Clk.HourH,4);
		E2P_WData(Rec_Date_MaxKW + 11*i,&Clk.YearH,5);
		}
	E2P_WData(Num_Demand_Reset,ptr,2);//�����
	E2P_WData(Battery_carry_time,ptr,4);//����ʹ��ʱ��
*/	
	GetTime();
	Clr_E2PECData( CMon_MD_P0, BYTE4, 9*9 );  //�������
	BCDTime_Reset( CMon_MDT_P0, BYTE6, 9*9 );  //�������ʱ��
	HT_FreeDog();

	Clr_E2PECData( CMon_MD_Top1, BYTE4, 12 );  //����ʷ�������
	BCDTime_Reset( CMon_MDT_Top1, BYTE6, 12 );  //����ʷ�������ʱ��
	
}
void initialMeterConstPara( void )
{
	unsigned short Temp;
	unsigned long Value;
	
	Value = 0;
///	E2P_WData( Billing_period_counter,(unsigned char*)&Value, 4 ); 
	E2P_WData( No_of_config_program,(unsigned char*)&Value, 2 ); 
	E2P_WData( Battery_use_time_counter,(unsigned char*)&Value, 4 ); 
/*	///fire 2016-6-27
Value = PS32*100;
	E2P_WData( Meter_constant_Active_LED,(unsigned char*)&Value, 4 ); 
	E2P_WData( Meter_constant_Reactive_LED,(unsigned char*)&Value, 4 ); 
	E2P_WData( Meter_constant_Apparent_LED,(unsigned char*)&Value, 4 ); 
	E2P_WData( Meter_constant_Active_Pulse,(unsigned char*)&Value, 4 ); 
	E2P_WData( Meter_constant_Reactive_Pulse,(unsigned char*)&Value, 4 ); 
	E2P_WData( Meter_constant_Apparent_Pulse,(unsigned char*)&Value, 4 ); 
	Temp = 1;
	E2P_WData( Reading_factor_for_power,(unsigned char*)&Temp, 2 ); 
	E2P_WData( Reading_factor_for_energy,(unsigned char*)&Temp, 2 ); 
	E2P_WData( Ratio_current_numerator,(unsigned char*)&Temp, 2 ); 
	E2P_WData( Ratio_voltage_numerator,(unsigned char*)&Temp, 2 ); 
	E2P_WData( Ratio_current_denominator,(unsigned char*)&Temp, 2 ); 
	E2P_WData( Ratio_voltage_denominator,(unsigned char*)&Temp, 2 ); 
//	Value = V10Un*1000;
*/
	Value = 230;//V10Un*100;
	E2P_WData( Nominal_voltage,(unsigned char*)&Value, 4 ); 
	//Value = A001Ib;
//	Value *= 100;
	Value = 5;
	E2P_WData( Basic_current,(unsigned char*)&Value, 4 ); 
	Value = 50;
	E2P_WData( Nominal_frequency,(unsigned char*)&Value, 4 ); 
	Value = 60;//������
///	Value = AImax;
					//��λС�� 
	E2P_WData( Maximum_current,(unsigned char*)&Value, 4 ); 
///	Temp = 80;
///	E2P_WData( Output_pulse_duration,(unsigned char*)&Temp, 2 ); 
	E2P_WData( Date_of_last_config_program, (unsigned char*)&Clk.YearH, 12 );
	E2P_WData( Date_of_last_calibration, (unsigned char*)&Clk.YearH, 12 );
	E2P_WData( Date_of_Battery_installation, (unsigned char*)&Clk.YearH, 12 );
	
}	
 //�����¼���¼..
 void clr_para_monitor( void )
 {
	 if( SM.Init_Flag != 0x5A5A )
	 {
		quality_event.flgEvent1.word = 0;
		quality_event.flgEvent1BK.word = 0;
		 
	   quality_event.PowerFailureEvent.byte = 0; 
	   quality_event.PowerFailurelongEvent.byte =0;
	   
	 quality_event.duanxianglongCNT_a = 0;		 
	/// quality_event.duanxianglongCNT_b = 0;	   
	/// quality_event.duanxianglongCNT_c = 0;	
	 quality_event.Ram_Duration_of_long_power_failure_in_any_phase = 0;
	 quality_event.Delay_TimeFlag[0] = 0;  //������ʱ�俪ʼ��־..
	 quality_event.Delay_TimeFlag[1] = 0;  
	 quality_event.Delay_TimeFlag[2] = 0;  
   
   }else{
				 quality_event.flgEvent1.word &= 0xFc00;
				 quality_event.flgEvent1BK.word &= 0xFc00;
				 //quality_event.Delay_TimeFlag[0] = 0;  //������ʱ�俪ʼ��־..
				 //quality_event.Delay_TimeFlag[1] = 0;  
				 //quality_event.Delay_TimeFlag[2] = 0;  
   }
   
   quality_event.flgEvent2.word = 0;
   quality_event.flgEvent2BK.word = 0;
	 
   quality_event.guoyaEvent.byte = 0;	  
   quality_event.qianyaEvent.byte = 0; 
   quality_event.duanxiangEvent.byte = 0;  
   
   
   //ʵʱ���������ʱ������..
   quality_event.guoyaCNT_a = 0;		   
///   quality_event.guoyaCNT_b = 0;		 
///   quality_event.guoyaCNT_c = 0;
   
   quality_event.qianyaCNT_a = 0;
///   quality_event.qianyaCNT_b = 0;		 
///   quality_event.qianyaCNT_c = 0;
   
   quality_event.duanxiangCNT_a = 0;	   
 ///  quality_event.duanxiangCNT_b = 0;	 
 ///  quality_event.duanxiangCNT_c = 0;
 
   
   quality_event.Deal_EVENT_NO =0;
 
   quality_event.phsreverseCNT = 0;
   quality_event.MissingNeutralCNT = 0; 	 //ȱ�಻�жϵ����߼��..
   
   quality_event.DisconnectFailureCNT = 0;
   
   //�����¼�..
///   part_event.DisplayBatCNT = 0;
 }

 //��Ҫ�������¼���¼����
void clr_event_Ram( void )
 {	
	 	unsigned char Buff[12];
 		unsigned char *Ptr;
 		unsigned char i;
 	
 		Ptr = Buff;
 		RAM_Fill( Ptr, 10);
 		
 		clr_para_monitor();		//����¼�����������..
 		quality_event.fail_short_times  = 0;
 		quality_event.fail_long_times = 0;
 		quality_event.fail_total_seconds = 0;
 		
 		//ʱ������..
		quality_event.Ram_Duration_of_last_voltage_sag_in_phase_L1  =0;							//1,	0,	32,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//A��Ƿѹ����ʱ��(3)    	Register::Unsigned16  
		quality_event.Ram_Duration_of_last_voltage_swell_in_phase_L1=0;							//1,	0,	32,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//A���ѹ����ʱ��(3)    	Register::Unsigned16  
                                                	                                               
	///	quality_event.Ram_Duration_of_last_voltage_sag_in_phase_L2  =0;							//1,	0,	52,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//B��Ƿѹ����ʱ��(3)    	Register::Unsigned16   
 	///	quality_event.Ram_Duration_of_last_voltage_swell_in_phase_L2=0;							//1,	0,	52,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,  //B���ѹ����ʱ��(3)    	Register::Unsigned16  
	                                                	                                               
	///	quality_event.Ram_Duration_of_last_voltage_sag_in_phase_L3 =0;							//1,	0,	72,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//C��Ƿѹ����ʱ��(3)    	Register::Unsigned16   
	///	quality_event.Ram_Duration_of_last_voltage_swell_in_phase_L3=0;							//1,	0,	72,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,  //C���ѹ����ʱ��(3)    	Register::Unsigned16  
		
		quality_event.Ram_Short_power_failure_L1 =0;							//0,	0,	96,	7,	16,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //A�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32         
	///	quality_event.Ram_Short_power_failure_L2 =0;							//0,	0,	96,	7,	17,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32         
	///	quality_event.Ram_Short_power_failure_L3 =0;							//0,	0,	96,	7,	18,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32     
		quality_event.Ram_Short_power_failure_any =0;						  //0,	0,	96,	7,	19,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //�����೤ʱ���Դ���ϳ���ʱ��(5)Register::Unsigned32

		quality_event.Ram_Duration_of_long_power_failure_in_phase_L1=0;							//0,	0,	96,	7,	16,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //A�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32         
	///	quality_event.Ram_Duration_of_long_power_failure_in_phase_L2=0;							//0,	0,	96,	7,	17,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32         
	///	quality_event.Ram_Duration_of_long_power_failure_in_phase_L3=0;							//0,	0,	96,	7,	18,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32     
		quality_event.Ram_Duration_of_long_power_failure_in_any_phase=0;						//0,	0,	96,	7,	19,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //�����೤ʱ���Դ���ϳ���ʱ��(5)Register::Unsigned32
				
		quality_event.Ram_Total_duration_of_long_power_failure_in_phase_L1=0;				//0,	0,	96,	7,	22,	255,D_Unsigned32,	0x00,	Unit_s,	0,	//A�೤ʱ���Դ�����ܳ���ʱ��(5)Register::Unsigned32   
	///	quality_event.Ram_Total_duration_of_long_power_failure_in_phase_L2=0;				//0,	0,	96,	7,	23,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B�೤ʱ���Դ�����ܳ���ʱ��(5)Register::Unsigned32   
	///	quality_event.Ram_Total_duration_of_long_power_failure_in_phase_L3=0;				//0,	0,	96,	7,	24,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C�೤ʱ���Դ�����ܳ���ʱ��(5)Register::Unsigned32   ��λ��s
		quality_event.Ram_Total_Duration_of_long_power_failure_in_any_phase=0;			//������..
						

//		FillLongToBuff( Ptr, Ram_Duration_of_long_power_failure_in_phase_L1 ); //A��
 			E2P_WData( Duration_of_long_power_failure_in_phase_L1, Ptr, 4 );
 			E2P_WData( Duration_of_long_power_failure_in_any_phase, Ptr, 4 );  //����������..
 			
 			E2P_WData( Total_Duration_of_long_power_failure_in_any_phase, Ptr, 4 );
 			
//	 		FillLongToBuff( Ptr, Ram_Total_duration_of_long_power_failure_in_phase_L1 ); //A��
 			E2P_WData( Total_duration_of_long_power_failure_in_phase_L1, Ptr, 4 );
 			
 		//��������..
 		for(i=0;i<4;i++)
 		{
 			E2P_WData( Number_of_power_failures_in_phase_L1+i*3, Ptr, 2 );
 		}
 		
 		//�����ͷ���..
 		for(i=0;i<6;i++)
 		{
 			E2P_WData( Number_of_voltage_sags_in_phase_L1+i*3, Ptr, 2 );
 		}
 		
}
/*
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
			if(( Para.RMeterTripState == 0x1A )&&( Para.RMeterRelayState == 0x1A )) ; 	//Զ����բ������բ��������ɺ���..
			else if( Para.RMeterRelayState != 0x1A )
			{
				ICComm.RelayOFFState = 0x55;									//׼����բ..
				Para.RMeterTripState = 0x1A;				
				E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				SM.DisconnectorControlCode = Remote_disconnection;
				LoadRecord( Disconnector_Control_No );
				OperateRelay();
				SM.Event_all_flag |= Disconnector_Control_BIT;
					SM.Error_all_flag |= Disconnector_Control_BIT;
			}else{
						 if(( Para.RMeterTripState == 0x1B )&&( Para.RMeterRelayState == 0x1A ))
						 	{
								ICComm.RelayOFFState = 0x0;
								Para.RMeterTripState = 0x1A;				
								E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
								SM.DisconnectorControlCode = Remote_disconnection;
								LoadRecord( Disconnector_Control_No );
								// OperateRelay();   //ʵ���Ѿ�������բ״̬..
								SM.Event_all_flag |= Disconnector_Control_BIT;
									SM.Error_all_flag |= Disconnector_Control_BIT;
						 	}
						}						
		//}
			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();
		}else ErrorByte = other_reason;							
	}
	else if( Method == 2 )
	{
//		if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))
		if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 ))
		{
			if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//�����բ..
		}
//		else if(( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x04 ))
		else if(( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x04 )||( Para.RMeterControlMode == 0x06 ))
		{
			if( Para.RMeterRelayState == 0x1A ) 
			{
				ICComm.RelayONState = 0x55;	//������բ.. 			
				SM.DisconnectorControlCode = Remote_connection;
				LoadRecord( Disconnector_Control_No );
				SM.Event_all_flag |= Disconnector_Control_BIT;
					SM.Error_all_flag |= Disconnector_Control_BIT;
//				limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;		//��վ��բ���������¼..
//				limiter.Ram_Auto_Reclosing_relay_Num_cnt =0;
			}	
		}	
		Para.RMeterTripState = 0x1B;
		E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
			// ��������¼��жϱ�־..
			Clr_local_control_para_monitor();		
	}	
	else ErrorByte = other_reason;										

	return ErrorByte;
}	
*/
//(���)������λ
void MaxDemand_Reset(void)
{
	MD_Rest();

/*	unsigned char Buff[5];
	unsigned char* ptr;
	unsigned char i;
	unsigned short Addr;
	ptr = Buff;
	RAM_Fill(ptr,5);
	Addr = Del_MaxKW;
	for(i=0;i<30;i++)
		{
		E2P_WData(Addr,ptr,4);//�����������,�����ۼƼ������ۼ��������
		Addr+=15;
		}
	E2P_WData(Time_Last_Reset,&Clk.HourH,4);
	E2P_WData(Date_Last_Reset,&Clk.YearH,5);

	E2P_RData((unsigned char *)&Addr,Num_Demand_Reset,2);
	Addr+=1;
	E2P_WData(Num_Demand_Reset,(unsigned char *)&Addr,2);
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
unsigned short GetClass3RegTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;				
	
	for( i=0;i<DLMS_RegSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_RegTab[i].Logical_Name, 6 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;
}	

unsigned char ActionClass3_register( unsigned char *Source, unsigned short DataLen )
{
    unsigned short i;
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
		
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	RAM_Fill(Ptr,6);
	i = GetClass3RegTabItemNo( Logical_Name );							//���Ҷ�Ӧ��OBIS��
	if( i == DLMS_DemandRegSum ) { return object_undefined; }
	if(DLMS_RegTab[i].State&Action_All!=Action_All){ return object_undefined; }

	if( Method == 1 )
	{
		if((*Logical_Name==1)&&(*(Logical_Name+1)==65)&&(*(Logical_Name+2)==0)&&(*(Logical_Name+3)==139)&&(*(Logical_Name+4)==0)&&(*(Logical_Name+5)==255))
			MaxDemand_Reset();
		else
			{
			E2P_WData(DLMS_RegTab[i].Addr,Ptr,DLMS_RegTab[i].ComInfo&0x0f);
			}
	}
	else ErrorByte = other_reason;										//����2��֧��

	return ErrorByte;
}


unsigned char ActionDemand_registerClassData( unsigned char *Source, unsigned short DataLen )
{
    unsigned short i;
//	unsigned char Buff[6];
//	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
		
//	Ptr = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	i = GetDemandRegTabItemNo( Logical_Name );							//���Ҷ�Ӧ��OBIS��
	if( i == DLMS_DemandRegSum ) { return object_undefined; }

	if( Method == 1 )
	{
		Action_Demand_reg_Reset( i );
	}
	else ErrorByte = other_reason;										//����2��֧��

	return ErrorByte;
}


