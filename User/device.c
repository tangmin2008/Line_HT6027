#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

#include "RsComm.h"
#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"

#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif

unsigned short GetClass19TabItemNo( unsigned char* OAD )
{
	unsigned short i;
	unsigned char Buff[4];
	unsigned char* NewOAD;
	
	NewOAD = Buff;
	RAM_Write( NewOAD, OAD, 4 );

	for( i=0;i<DL698_ClassId19Sum;i++ )
	{
		if( Data_Comp( NewOAD, (unsigned char*)DL698_ClassId19_Tab[i].OAD, 4 ) == 0 ) break;				//���Ҷ�Ӧ��OBIS��
	}	
	return i;	
}

//�����������ӿ����������ݣ�����ֵ��-1���޴��������-1���������ݳ���
//short GetClass19Data( unsigned char* Dest, unsigned char* OAD )
short GetClass19Data( short COM_No, unsigned char* Dest, unsigned char* OAD )		//17.04.09
{
	short i,j;
	unsigned short Addr;
	short Len;
//	short Temp;
	short Start,End;
	unsigned char DataType;
	unsigned char Element_Len;			
//	unsigned char BCD_Len;			//ԭBCD���ݳ���
//	char		  Multiple;			//�Ŵ���				������ʾ��С
	unsigned char* Ptr;
	unsigned short ComInfo;
	Len = -1;
	
	i = GetClass19TabItemNo( OAD );
	if( i != DL698_ClassId19Sum )
	{
		if( Judge_operating_authority( COM_No, OAD, DL698_ClassId19_Tab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
//		ComID = SwapCharToLong( OAD );
		*Dest = Get_data;								//�������ݷ���	
		DataType = DL698_ClassId19_Tab[i].DataType;
		ComInfo = DL698_ClassId19_Tab[i].ComInfo;
		if(( DataType == Data_array )||( DataType == Data_structure )) 					
		{	
			Start = 0;
			Element_Len = DL698_ClassId19_Tab[i].Element_Len;
			End = Element_Len;
			*(Dest+1) = DataType;
			*(Dest+2) = End;
			Ptr = Dest+3;
			Len = 3;
			i += 1;
		}	
		else
		{
			Start = 0;
			End = 1;					
			Ptr = Dest+1;
			Len = 1;
		}	 
		for( j=Start;j<End;j++ )
		{
			DataType = DL698_ClassId19_Tab[i].DataType;
			Element_Len = DL698_ClassId19_Tab[i].Element_Len;
			ComInfo = DL698_ClassId19_Tab[i].ComInfo;
			switch( ComInfo & Comm_Type )
			{
				case PARAM:
					Addr = DL698_ClassId19_Tab[i].Addr;
					if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
					{
						*Ptr = DataType;
						*(Ptr+1) = Element_Len;	
						if( DataType == D_Bit_String ) 	Element_Len /= 8;		
						E2P_RData( Ptr+2, Addr, Element_Len );
						if( DataType == D_OctetStr ) SwapData( Ptr+2, Element_Len );
						else if( DataType == D_Bit_String ) SwapBitString( Ptr+2, Element_Len );
						Element_Len += 1;
					}
					else
					{
						*Ptr = DataType;
						E2P_RData( Ptr+1, Addr, Element_Len );
						SwapData( Ptr+1, Element_Len );
					}		
					
					break;
				default: break;
			}	
			Ptr += Element_Len + 1;
			Len += Element_Len + 1;
			i++;
		}		
	}
	return Len;	
}


//ȡClass19������
short GetDeviceClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short Len;

	RAM_Write( Dest, Source, 4 );

	Len = GetDeviceClassDataTab( COM_No, Dest+4, Source );
	if( Len == -1 )  
	{
		*(Dest+4) = Get_data_access_result;						//�쳣��־
		*(Dest+5) = object_undefined; 						//�޶�Ӧ��OBIS��
		Len = 2;
	}
	else if( Len == -2 )										//17.04.09
	{
		*(Dest+4) = Get_data_access_result;						//�쳣��־
		*(Dest+5) = read_write_denied; 							//����Ȩ�޲���
		Len = 2;
	}	
	Len += 4;
	return Len;		
}		



//ȡPhaseEvent������
short GetDeviceClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Attribute;
	unsigned char* OAD;
	short Len;
    
    Len = -1;
	
	OAD = Source;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);

	*Dest = Get_data;								//�������ݷ���	
	if( Attribute == 1 )
	{
		*(Dest+1) = D_OctetStr;
		*(Dest+2) = 0x02;
		RAM_Write( Dest+3, OAD, 2 );
		Len = 5;
	}
	else if(( Attribute >= 2 )&&( Attribute <= 10 ))	
	{
//		Len = GetClass19Data( Dest, OAD );
		Len = GetClass19Data( COM_No, Dest, OAD );	//17.04.09
	}	


/*				
	*Dest = Get_data;								//�������ݷ���	
	switch( Attribute )
	{
		case 1:										//logical_name
			*(Dest+1) = D_OctetStr;
			*(Dest+2) = 0x02;
			RAM_Write( Dest+3, OAD, 2 );
			Len = 5;
			break;	
		case 2:										//	
			*(Dest+1) = D_VisibleStr;
			*(Dest+2) = 0x0C;
			E2P_RData( Dest+3, DeviceDescriptor, 12 );			//�豸������
			Len = 15;
			break;	
		case 3:										//
			Len = GetClass19Data( Dest, OAD );
//			*(Dest+1) = Data_structure;
//			*(Dest+2) = 0x6;
//			*(Dest+3) = D_VisibleStr;
//			*(Dest+4) = 0x4;
//			E2P_RData( Dest+5, DLT698Maunf_No, 4 );				//���Ҵ���
//			*(Dest+9) = D_VisibleStr;
//			*(Dest+10) = 0x4;
//			E2P_RData( Dest+11, DLT698SoftVER, 4 );				//����汾��
//			*(Dest+15) = D_VisibleStr;
//			*(Dest+16) = 0x4;
//			E2P_RData( Dest+17, DLT698SoftVERDate, 6 );			//����汾����
//			*(Dest+23) = D_VisibleStr;
//			*(Dest+24) = 0x4;
//			E2P_RData( Dest+25, DLT698HardVER, 4 );				//Ӳ�����汾��
//			*(Dest+29) = D_VisibleStr;
//			*(Dest+30) = 0x4;
//			E2P_RData( Dest+31, DLT698HardVERdate, 6 );			//Ӳ�����汾����
//			*(Dest+37) = D_VisibleStr;
//			*(Dest+38) = 0x4;
//			E2P_RData( Dest+39, DLT698MaunfExpInfo, 8 );		//������չ��Ϣ
//			Len = 47;
			break;	
		case 4:										//	
			*(Dest+1) = D_date_time_s;
			E2P_RData( Dest+2, DLT698DateOfProduction, 7 );		//��������
			Len = 9;
			break;	
		case 5:										//���豸�б�	
			Len = GetClass19Data( Dest, OAD );
//			*(Dest+1) = Data_array;
//			E2P_RData( Dest+2, DLT698DeviceListNumber, 1 );		
//			if( *(Dest+2) > 4  )
//			{
//				*(Dest+2) = 0;
//				Len = 3;	
//			}	
//			else
//			{
//				for( i=0;i<*(Dest+2);i++ )
//				{
//					*(Dest+3+i*3) = D_OI;
//					E2P_RData( Dest+4+i*3, DLT698DeviceOI1+i*3, 2 );								
//				}		
//				Len = 3 + *(Dest+2)*3;
//			}			
			break;
		case 6:										//֧�ֹ�Լ�б�	
			Len = GetClass19Data( Dest, OAD );			
//			*(Dest+1) = Data_array;
//			E2P_RData( Dest+2, DLT698ProtocolNumber, 1 );		
//			if( *(Dest+2) > 4  )
//			{
//				*(Dest+2) = 0;
//				Len = 3;	
//			}	
//			else
//			{
//				Length = 0;
//				Ptr = Dest+3;
//				Len = 2;
//				for( i=0;i<*(Dest+2);i++ )
//				{
//					*Ptr = D_VisibleStr;
//					E2P_RData( Ptr+1, DLT698Protocol1+i*16, 16 );
//					if(*(Ptr+1) > 15 ) *(Ptr+1) = 15;
//					Length = *(Ptr+1) + 1;
//					Ptr += Length;
//					Len += Length;								
//				}		
//			}			
			break;
		case 7:										//��������ϱ�	
			*(Dest+1) = D_BOOLEAN;
			E2P_RData( Dest+2, DLT698FollowReport,  1 );	
			Len = 3;
			break;
		case 8:										//���������ϱ�
			*(Dest+1) = D_BOOLEAN;
			E2P_RData( Dest+2, DLT698InitiativeReport,  1 );	
			Len = 3;
			break;
		case 9:										//��������վͨ��
			*(Dest+1) = D_BOOLEAN;
			E2P_RData( Dest+2, DLT698AllowCall,  1 );	
			Len = 3;
			break;
		case 10:									//�ϱ�ͨ��
			Len = GetClass19Data( Dest, OAD );
//			*(Dest+1) = Data_array;
//			E2P_RData( Dest+2, DLT698ReportChannelNum, 1 );		
//			if( *(Dest+2) > 4  )
//			{
//				*(Dest+2) = 0;
//				Len = 3;	
//			}	
//			else
//			{
//				for( i=0;i<*(Dest+2);i++ )
//				{
//					*(Dest+3+i*5) = D_OAD;
//					E2P_RData( Dest+4+i*5, DLT698DeviceOI1+i*5, 4 );								
//				}		
//				Len = 3 + *(Dest+2)*5;
//			}			
			break;
		default:
			break;	
	}
*/		
	return Len;
}	


//-------------------------------------------------------------------------------------------------------
unsigned char SetDeviceClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = SetDeviceClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen  );
	return ErrorByte;		

}		

//ȡPhaseEvent������
unsigned char SetDeviceClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
//	unsigned char Buff[20];
//	short i;
//	unsigned short Addr;
	unsigned char ErrorByte=0;
//	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
//    short Item;
//	unsigned short GetLen;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char* Ptr;
//	unsigned char* Ptr;
	unsigned char* OAD;
//	unsigned char* WriteBufAds;
//	Len = -1;
	
//	Ptr = Buff;
	OAD = Source;
//	OICode = SwapCharToShort( OAD );
//	WriteBufAds = Source+4;
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);

	if(( Attribute >= 2 )&&( Attribute <= 10 ))
	{
//		ErrorByte = SetClass19Data( Source, DataLen, ItemLen );			
		ErrorByte = SetClass19Data( COM_No, Source, DataLen, ItemLen );			//17.04.09
	}
	else ErrorByte = object_class_inconsistent;	
							
/*	switch( Attribute )
	{
		case 2:										//	
			if( *WriteBufAds != D_VisibleStr ) return type_unmatched;
			if( *(WriteBufAds+1) > 12 ) return type_unmatched;
			RAM_Write( Ptr, WriteBufAds+1, *(WriteBufAds+1) );
			RAM_Fill( Ptr+(*(WriteBufAds+1)), 12-(*(WriteBufAds+1)));
			E2P_WData( DeviceDescriptor, Ptr, 12 );			//�豸������
			*ItemLen += *(WriteBufAds+1) + 2;
			break;	
		case 3:										//
			ErrorByte = SetClass19Data( Source, DataLen, ItemLen );
			break;	
		case 4:										//	
			if( *WriteBufAds != D_date_time_s ) return type_unmatched;		//��������
			E2P_WData( DLT698DateOfProduction, WriteBufAds+1, 7 );
			*ItemLen += 8;
			break;	
		case 5:										//���豸�б�	
			ErrorByte = SetClass19Data( Source, DataLen, ItemLen );
			break;
		case 6:										//֧�ֹ�Լ�б�	
			ErrorByte = SetClass19Data( Source, DataLen, ItemLen );
			break;
		case 7:										//��������ϱ�	
			if( *WriteBufAds != D_BOOLEAN ) return type_unmatched;
			E2P_WData( DLT698FollowReport, WriteBufAds+1, 1 );
			*ItemLen += 2;
			break;
		case 8:										//���������ϱ�
			if( *WriteBufAds != D_BOOLEAN ) return type_unmatched;
			E2P_WData( DLT698InitiativeReport, WriteBufAds+1, 1 );
			*ItemLen += 2;
			break;
		case 9:										//��������վͨ��
			if( *WriteBufAds != D_BOOLEAN ) return type_unmatched;
			E2P_WData( DLT698AllowCall, WriteBufAds+1, 1 );
			*ItemLen += 2;
			break;
		case 10:									//�ϱ�ͨ��
			ErrorByte = SetClass19Data( Source, DataLen, ItemLen );
			break;
		default:
			ErrorByte = object_class_inconsistent;
			break;	
	}
*/	
	return ErrorByte;
}	

//unsigned char SetClass19Data( unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen )
unsigned char SetClass19Data( unsigned short COM_No, unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen )	//17.04.09
{
	unsigned char Buff[100];
	unsigned char *Ptr;
	unsigned char ErrorByte=0;
	short i,j;
	unsigned short Addr;
	short Start,End;
	unsigned char DataType;
	unsigned char Element_Len;			
	unsigned short ComInfo;
	unsigned char OAD[4];
	unsigned char Len;
	
	Ptr = Buff;
	for( i=0;i<DL698_ClassId19Sum;i++ )
	{
		if( Data_Comp( Source, (unsigned char*)DL698_ClassId19_Tab[i].OAD, 4 ) == 0 ) break;				//���Ҷ�Ӧ��OAD��
	}	
	if( i == DL698_ClassId19Sum ) { return object_undefined; }

	if( Judge_operating_authority( COM_No, Source, DL698_ClassId19_Tab[i].SaftInfo, write_authority ) != 0 ) return read_write_denied;	//17.04.09

	DataType = *(Source+4);
	if( DL698_ClassId19_Tab[i].DataType != DataType ) { return type_unmatched; }
	ComInfo = DL698_ClassId19_Tab[i].ComInfo;
	RAM_Write( OAD, Source, 4 );
//	ComID = SwapCharToLong( OAD );

	Source += 4;
	*ItemLen += 4;
	if(( DataType == Data_array )||( DataType == Data_structure )||( DataType == D_TI )) 					
	{
		Start = 0;
		Element_Len = DL698_ClassId19_Tab[i].Element_Len;
        if( DataType == Data_array ) 
        {
          if( *(Source+1) <= Element_Len ) Element_Len = *(Source+1); 
          else return type_unmatched;  
        }  
		End = Element_Len;
        Source += 2;
        *ItemLen += 2;
		i += 1;
		if( End == 0 ) return object_class_inconsistent;
	}	
	else
	{
		Start = 0;
		End = 1;					
	}	 
	for( j=Start;j<End;j++ )
	{
		DataType = DL698_ClassId19_Tab[i].DataType;
		if( *Source != DataType ) { return type_unmatched; }
		DataLen -= 1;
		Element_Len = DL698_ClassId19_Tab[i].Element_Len;
		if( DataType == D_Bit_String ) Element_Len /= 8;
//		if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
//		{
//			if( DataLen > (Element_Len+2) ) { return type_unmatched; }
//		}
//		else
//		{
//			if( DataLen < Element_Len ) { return type_unmatched; }			
//		}		
		ComInfo = DL698_ClassId19_Tab[i].ComInfo;
		Addr = DL698_ClassId19_Tab[i].Addr;

		switch( ComInfo & Comm_Type )
		{
			case PARAM:
				Addr = DL698_ClassId19_Tab[i].Addr;
				if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
				{
					Len = *(Source+1);
					if( DataType == D_Bit_String ) 	Len /= 8;
					if(( Len > 100 )||( Len > Element_Len )) return read_write_denied;					//����100����������		
					RAM_Write( Ptr, Source+2, Len );
					if( DataType == D_OctetStr ) SwapData( Ptr, Len );
					else if( DataType == D_Bit_String ) SwapBitString( Ptr, Len );
					RAM_Fill( Ptr+Len, Element_Len - Len );					
					E2P_WData( Addr, Ptr, Element_Len );
					Element_Len = Len + 1;						
				}
				else
				{
					SwapData( Source+1, Element_Len );
					E2P_WData( Addr, Source+1, Element_Len );
				}		
				break;
			default: 
				break;
		}	
		Source += Element_Len + 1;
		DataLen -= Element_Len;
		*ItemLen += Element_Len + 1;
		i++;
	}
			
	return ErrorByte;
}	



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionDeviceClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionDeviceClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen  );
	return ErrorByte;		

}	

unsigned char ActionDeviceClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char Buff[4];
//	short i,j;
//	unsigned char* Addr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
//	unsigned short OICode;
//	short Temp;
//	unsigned short GetLen;
//	short Start,End;
//	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
	unsigned char* Ptr;
	unsigned char* OMD;
//	unsigned char* WriteBufAds;
	unsigned char Method;
//	Len = -1;
	unsigned short* ComBTime;

	OMD = Source;
//	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OMD );
	Method = *(OMD+2);
	ComBTime = CommPara[COM_No].BTime;
	Ptr = Buff;
	
	switch( Method )
	{
		case 1:										//logical_name
			break;	
		case 2:										//	
			break;	
		case 3:										//���ݳ�ʼ��
			*ItemLen += 5;
			if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//ʱ���ǩ��Ч		//17.04.30
			if( Judge_operating_authority( COM_No, OMD, 0x0010, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//			if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//ʱ���ǩ��Ч		//17.04.17

			*ComBTime = MS4000;
//			if(( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 ) 
//			{
////				format_disk();
////				f_mount(0,&fs);		  //��һ�β�����Ҫ�ȸ�ʽ������
//				File_Format();
//			}	
			RecordStartEvent( ECClr_No );
			SM.MeterClrState = MeterClrStep2;							//���������������		//11.01.14
			SM.MeterClrStateChk = MeterClrStep2 + 0xA5A5;				//11.01.14	

/*			AllECClr();
//			ClrEventData(1);
			ClrFreezeData();
			*ComBTime = MS4000;
			MD_Rest(1);
			*ComBTime = MS4000;
//			if((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))	//�¹���	//14.04.09
            if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )&&( IsFactoryTestState() == 0 )) 	//�¹���	//14.04.09
			{
				Clr_E2PData( Bat_WorkTime, 4, 1 );   //���ع���ʱ��
				RAM_Fill( Real.RBat_WorkTime, 4 );
				Real.BatWorkTimeChk = 0xA5;
//				ClrMeterClrRec();		//�����������¼ 10.05.11
				ClrEventData(0);
			}
			else ClrEventData(1);
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
			Clr_E2PData( L1_SaveDay1EC_Pp0, 4, 4 );  //�����һ�����ε�һ�����ս���ʱ�������й�����		//12.10.04
#endif																									//12.10.04
			Clr_E2PData( L1_YearSaveDayEC_Pp0, 4, 4 );  //�����һ������������ս���ʱ�������й�����	//�¹���
			Clr_E2PData( EFstCntPA, 2, 3 );  			//���������й���������������					//�¹���	//13.12.28
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			Flag.MeterState1 &= ~( F_ClockBat+F_CommBat );		//�������󣬵��״̬�������ж�		//�¹���	//14.06.22
			*Ptr = 0x00;																				//�¹���	//14.06.22		
			Para.RBatState = *Ptr;						//16.05.09
			E2P_WData( EBatState, Ptr, 1 );																//�¹���	//14.06.22	
#endif
//			RecordEndEvent( ECClr_No, 0 );
*/			
//			*ItemLen += 5;
			ErrorByte = 0;
			break;	
		case 4:										//�ָ���������
			if( Judge_operating_authority( COM_No, OMD, 0x0010, active_authority ) != 0 ) return read_write_denied;	//17.04.09
			if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 )) return read_write_denied;	//17.05.19

			*ComBTime = MS4000;
			InitDisplayTab();
			InitEventTab();
			*ComBTime = MS4000;
			InitComPara();
			ClrEventData(1);
			SM.InitFreezeState = InitFreezeStep1;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			ErrorByte = 0;
			break;	
		case 5:										//�¼���ʼ��
			*ItemLen += 5;
			if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//ʱ���ǩ��Ч		//17.04.30
			if( Judge_operating_authority( COM_No, OMD, 0x0010, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//			if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//ʱ���ǩ��Ч		//17.04.17
			*ComBTime = MS4000;
//			ClrEventData(2);						//17.04.14
			*Ptr = 0x30;
			*(Ptr+1) = 0x15;
			*(Ptr+2) = 0x05;
			*(Ptr+3) = 0x00;
			E2P_WData( EventClr_OMD1, Ptr, 4 );
//			RAM_Fill( SM.FollowReportOAD, 4 );		//17.04.20      //17.05.19
			RecordStartEvent( EventClr_No );
			
			SM.EventClrState = EventClrStep1;							//�����¼���������		//17.04.14
			SM.EventClrStateChk = EventClrStep1 + 0xA5A5;				//17.04.14	

//			RecordEndEvent( EventClr_No, 0 );
//			*ItemLen += 5;
			ErrorByte = 0;
			break;
		case 6:										//������ʼ��	
			*ItemLen += 5;
			if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//ʱ���ǩ��Ч		//17.04.30
			if( Judge_operating_authority( COM_No, OMD, 0x0010, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//			if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//ʱ���ǩ��Ч		//17.04.17
			RecordStartEvent( MDClr_No );
			MD_Rest(1);
//			RecordEndEvent( MDClr_No, 0 );
//			*ItemLen += 5;
			ErrorByte = 0;
			break;
		default:
			break;	
	}	
	return ErrorByte;
}


void MeterClrCycle( void )
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	switch( SM.MeterClrState )	
	{
		case MeterClrIdle:			// ����������
			SM.MeterClrStateChk = MeterClrIdle;			
			break;
		case MeterClrStep1:			// ������㲽��1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
//			CreateECClrRec();		//��������ǰ��¼					//10.04.26				
         	RecordStartEvent( ECClr_No );
			SM.MeterClrState = MeterClrStep2;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep2:			// ������㲽��1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			Flag.RSwitchOldState |= 0x03;		//��������բ�ɹ��¼�						//�¹���	//14.06.28		
#endif
			AllECClr();		//�����ǰ��������ʷ����ָ�롢����
			MD_Rest(1);		//�嵱ǰ����
//			AllMDClr();		//�����ʷ����ָ�롢����
//			RAM_DataFill( Ptr, 4, 0xff);		//�¼�ȫ��
//			AllEventClr( Ptr, 0 );
//			AllDJClr();
//			Clr_E2PData( LoadRecState1, 4, 6 );  //�帺������
			ClrFreezeData();
			if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))	//�¹���	//14.04.09  
			{
				Clr_E2PData( Bat_WorkTime, 4, 1 );   //���ع���ʱ��
				RAM_Fill( Real.RBat_WorkTime, 4 );
				Real.BatWorkTimeChk = 0xA5;
//				ClrMeterClrRec();		//�����������¼ 10.05.11
				ClrEventData(0);
			}
			else ClrEventData(1);
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
			Clr_E2PData( L1_SaveDay1EC_Pp0, 4, 4 );  //�����һ�����ε�һ�����ս���ʱ�������й�����		//12.10.04
#endif																									//12.10.04
			Clr_E2PData( L1_YearSaveDayEC_Pp0, 4, 4 );  //�����һ������������ս���ʱ�������й�����	//�¹���
			Clr_E2PData( EFstCntPA, 2, 3 );  			//���������й���������������					//�¹���	//13.12.28
#if ( LocalSmartMeter == YesCheck )		//17.04.22
			RecordStartEvent( BuyMoney_No );
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			Flag.MeterState1 &= ~( F_ClockBat+F_CommBat );		//�������󣬵��״̬�������ж�		//�¹���	//14.06.22
			*Ptr = 0x00;																				//�¹���	//14.06.22		
			Para.RBatState = *Ptr;						//16.05.09
			E2P_WData( EBatState, Ptr, 1 );																//�¹���	//14.06.22	
#endif
			break;
		default: 
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;			
			break;	
	}
}

void EventClrCycle( void )								//17.04.14
{
	switch( SM.EventClrState )	
	{
		case EventClrIdle:			// ����������
			SM.EventClrStateChk = EventClrIdle;			
			break;
		case EventClrStep1:			// ������㲽��1
			if( SM.EventClrStateChk != ( SM.EventClrState +0xA5A5 ) ) break;
			ClrEventData(2);						//17.04.14
			SM.EventClrState = EventClrIdle;
			SM.EventClrStateChk = EventClrIdle;
			break;
		default: 
			SM.EventClrState = EventClrIdle;
			SM.EventClrStateChk = EventClrIdle;			
	}
}
//Type: 0:ȫ�������1�����˵�������¼��2�����˵�������¼���¼���¼
void ClrEventData( short Type )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	short i;	

	Ptr = Buff;
	
	RAM_Fill( Ptr, 4 );
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{				
		if(( Type == 1 )&&( i == ECClr_No )) continue;
		if(( Type == 2 )&&(( i == ECClr_No )||( i == EventClr_No ))) continue;
		E2P_WData( DL698_Event_ProfileTab[i].CuntAddr, Ptr, 4 );
		E2P_WData( DL698_Event_ProfileTab[i].TimeAddr, Ptr, 4 );
		E2P_WData( DL698_Event_ProfileTab[i].EntriesUseAddr, Ptr, 2 );
		E2P_WData( DL698_Event_ProfileTab[i].EFinalRecNoAddr, Ptr, 4 );
		if( DL698_Event_ProfileTab[i].EventNo == PrgRec_No ) ClearPrgRecOAD();
		ClearEventVar( i );
		ClearMDOverTamperTabVar( i );
	}
	
	InitMonQualifiyRateData();
	InitDayQualifiyRateData();
	
	Clr_E2PData( EMaxVoltImbalanceRatio, 3, 3 );				
	RAM_Fill( Real.MaxVoltImbalanceRatio, 3 );					
	RAM_Fill( Real.MaxImbalanceRatio, 3 );						
	RAM_Fill( Real.MaxBestImbalanceRatio, 3 );						

	RAM_Fill((unsigned char*)&SM.CoverState, 2 );				//�������״̬��
	RAM_Fill((unsigned char*)&SM.TerminalState, 2 );			//�������״̬��
	
	RAM_Fill( Ptr, 12 );				
#if ( LocalSmartMeter == YesCheck )
	E2P_WData( ILInsert_Cunt, Ptr, 3 );
#endif
	*(Ptr+10) |= 0x0A;						//�¼�����͵�������־�����
	CommResetEventUpState( Ptr );		
	ResetEventUpStateSend( Ptr );		
//	E2P_WData( PhFailCuntZ, Ptr, 4 );				
//	E2P_WData( PhFailTimeZ, Ptr, 4 );				
//	E2P_WData( PhFailStartTime, Ptr, 7 );				
//	E2P_WData( PhFailEndTime, Ptr, 7 );				
	ClearPhFailData();
//	InitialNewEventList( Type );		//17.03.24
	InitialNewReportEventList( Type );	//17.05.19
	
	Flag.RSwitchOldState &= 0x03;		//��Ӱ������բ�ɹ��ϱ�								
	Flag.RSwitchOldState |= 0x40;		//����̵����쳣�����ϱ�						
	E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );		
	
	UpdateMeterState4_7();		
	
}	

void ClrFreezeData( void )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
        short Temp;
        unsigned short EntriesUseAddr;
        unsigned short PtAddr;
//	short i,j;	
	short i;	

	Ptr = Buff;
	
	RAM_Fill( Ptr, 4 );
	for( i=0;i<DL698_Freeze_ProfileSum;i++ )
	{				
//		Temp = 0;
//		E2P_RData( (unsigned char*)&Temp, DL698_Freeze_ProfileTab[i].CapNumAddr, 1 );
		Temp = GetFreezeCaptureItems( i );
		EntriesUseAddr = DL698_Freeze_ProfileTab[i].EntriesUseAddr;
		PtAddr = DL698_Freeze_ProfileTab[i].PtAddr;
		Clr_E2PData( EntriesUseAddr, 2, Temp );				
		Clr_E2PData( PtAddr, 2, Temp );				
//		for( j=0;j<Temp;j++ )
//		{
//			E2P_WData( EntriesUseAddr, Ptr, 2 );
//			E2P_WData( PtAddr, Ptr, 2 );
//			EntriesUseAddr += 3;
//			PtAddr += 3;			
//		}
		
		E2P_WData( DL698_Freeze_ProfileTab[i].FinalRecNoAddr, Ptr, 4 );
	}
}

//==============================================================================================
void InitDisplayTab( void )
{
	unsigned short i,j;
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned short CapNumAddr;		//��ʾ������ַ
	unsigned short CapObjAddr;		//��ʾ���ַ
	unsigned short PeriodAddr;
	unsigned short InitCapNumber;	//��ʾ��ʼ������
	unsigned char* InitCapObjTab;	//��ʾ��ʼ������
	
	Ptr = Buff;
	for( i=0;i<2;i++ )
	{
		InitCapNumber = DL698_Display_ParaTab[i].InitCapNumber;
		*Ptr = InitCapNumber;
		CapNumAddr = DL698_Display_ParaTab[i].CapNumAddr;
		E2P_WData( CapNumAddr, Ptr, 1 );
		PeriodAddr = DL698_Display_ParaTab[i].PeriodAddr;
		*Ptr = 5;
		E2P_WData( PeriodAddr, Ptr, 1 );
		CapObjAddr = DL698_Display_ParaTab[i].CapObjAddr;
		InitCapObjTab = (unsigned char*)DL698_Display_ParaTab[i].InitCapObjTab;
		for( j=0;j<InitCapNumber;j++ )
		{
			RAM_Write( Ptr, InitCapObjTab+j*10, 10 );
			E2P_WData( CapObjAddr+j*11, Ptr, 10 );			
		}			
	}	
}	


void InitEventTab( void )
{
    short i,j;
    unsigned short GetLen;
  unsigned short CapObjAddr;		//�������ַ
	unsigned short InitCapNumber;	//���߳�ʼ����������
	unsigned char* InitCapObjTab;	//���߳�ʼ����������

	unsigned char ErrorByte;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char* Point;

	Point = Buff;
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		ErrorByte = 0;
		InitCapNumber = DL698_Event_ProfileTab[i].InitCapNumber;
//		if(( InitCapNumber == 0x0000 )||( InitCapNumber > DL698_Event_ProfileTab[i].CapNumMax)) continue;	//���¼��޲������������
		if( InitCapNumber > DL698_Event_ProfileTab[i].CapNumMax ) continue;	//���¼��޲������������
//		if( InitCapNumber != 0x0000 )										//��Ĭ�Ϲ�������		//�޹�����ҲҪ��ʼ��
//		{
			InitCapObjTab = DL698_Event_ProfileTab[i].InitCapObjTab;
        	
			GetLen = 0;
			Ptr = InitCapObjTab;
			for( j=0;j<InitCapNumber;j++ )
			{
				RAM_Write( Point, Ptr, 4 );
				if( Judge_capture_objects_content( Point, &GetLen, 0 ) != 0 ) 
				{
					ErrorByte = 1;
					break; 
				}	
				Ptr += 4;		
			}	
			if( ErrorByte != 0 ) continue;								//��ʼ���������зǷ����ݣ�����
			else 
			{
				InitialEventProfilePara( i, GetLen );					//��ʼ���¼����ߵĲ��ֲ���
				Ptr = InitCapObjTab;
				CapObjAddr = DL698_Event_ProfileTab[i].CapObjAddr;
				for( j=0;j<InitCapNumber;j++ )
				{
					RAM_Write( Point, Ptr, 4 );
					E2P_WData( CapObjAddr, Point, 4 );						
					CapObjAddr += 5;
					Ptr += 4;
				}		
			}
//		}	
		Buff[0] = InitCapNumber;
		E2P_WData( DL698_Event_ProfileTab[i].CapNumAddr, Buff, 1 );						
		Buff[0] = 0;
		E2P_WData( DL698_Event_ProfileTab[i].EReportFlagAddr, Buff, 1 );				//ȱʡ���ϱ�
		Buff[0] = 1;
		E2P_WData( DL698_Event_ProfileTab[i].EValidFlagAddr, Buff, 1 );					//ȱʡ��Ч
		if( DL698_Event_ProfileTab[i].Class_Id == Class_id_PhaseEvent ) i += 3;			//�����¼�ֻ����һ��,ѭ���Զ�i++һ��
	}

}

void InitComPara( void )
{
	unsigned char Buff[8] = { 'F','2','0','2','0','2','0','1' };
	unsigned char Buff2[5] = { 2,2,8,1,0 };
	
	E2P_WData( PortDescriptor0, Buff, 8 );		//����
	Buff[3] = '1';
	E2P_WData( PortDescriptor1, Buff, 8 );		//4851
	Buff[7] = '2';
	E2P_WData( PortDescriptor2, Buff, 8 );		//4852
	Buff[3] = '9';
	Buff[7] = '1';
	E2P_WData( PortDescriptor3, Buff, 8 );		//ģ��
	Buff[3] = '5';
	E2P_WData( RealyDescriptor, Buff, 8 );		//�̵���

	E2P_WData( EComDCB0, Buff2, 5 );			//����
	Buff2[0] = 3;								//2400
	E2P_WData( EComDCB1, Buff2, 5 );			//4851
	E2P_WData( EComDCB2, Buff2, 5 );			//4852
	Buff2[0] = 6;								//2400
	E2P_WData( EComDCB3, Buff2, 5 );			//ģ��

	Comm.BandRate1 = Baut2400;
	E2P_WData( RS485BaudRate1, (unsigned char*)&Comm.BandRate1,1 ); 		
#if ( CarryComm == YesCheck )			//���ز�ͨ����485��2����ͨѶ����
	Comm.BandRate2 = Baut9600;
#else
	Comm.BandRate2 = Baut2400;
#endif
	E2P_WData( RS485BaudRate2, (unsigned char*)&Comm.BandRate2,1 ); 				
	
	Buff[0] = '0';
	Buff[1] = '0';
	E2P_WData( ModuleMaunf_No, Buff, 2 );		
	E2P_WData( ModuleChip_No, Buff, 2 );		
	Buff[0] = 0x07;
	Buff[1] = 0xD0;
	Buff[2] = 0x01;
	Buff[3] = 0x01;
	Buff[4] = 0x06;
	E2P_WData( ModuleVERDate, Buff, 5 );
	Buff[0] = 0;		
	Buff[1] = 0;		
	E2P_WData( ModuleSoftVER, Buff, 2 );		
	
	E2P_WData( MeterRunMode1, Buff, 1 );		
	Buff[0] = 1;		
	E2P_WData( SwitchAttribute, Buff, 1 );		
	
}	


void FillInitFreezeItemBuff( unsigned char* Dest, unsigned char* Period, unsigned char* OAD, unsigned char* Deep )
{
	RAM_Fill( Dest, 4 );
	Dest += 4;
	*Dest = Data_structure;
	*(Dest+1) = 0x03;
	*(Dest+2) = D_Unsigned16;
	*(Dest+3) = *(Period+1);
	*(Dest+4) = *Period;
	*(Dest+5) = D_OAD;
	RAM_Write( Dest+6, OAD, 4 );
	*(Dest+10) = D_Unsigned16;
	*(Dest+11) = *(Deep+1);
	*(Dest+12) = *Deep;
}

void InitFreezeTab( unsigned short ItemNo )
{
    short i,j;
  	unsigned short CapObjAddr;		//�������ַ
	unsigned short InitCapNumber;	//���߳�ʼ����������
	unsigned char* InitCapObjTab;	//���߳�ʼ����������
	unsigned char Buff[20];
	unsigned char Buff2[50];
	unsigned char Buff3[100];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short CaptureItems;
	unsigned short Number;
	
	Ptr = Buff;
	Point = Buff2;
	RAM_Fill( Buff3, 100 );

	CaptureItems = GetFreezeCaptureItems( ItemNo );						//�Ѵ�������
	InitCapNumber = DL698_Freeze_ProfileTab[ItemNo].InitCapNumber;		//Ԥ��ʼ��������	
	if( InitCapNumber > 100 ) return;									//���100��		
	InitCapObjTab = DL698_Freeze_ProfileTab[ItemNo].InitCapObjTab;
    CapObjAddr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;  			
      	
    Number = 0;
    for( i=0;i<CaptureItems;i++ )
    {
    	E2P_RData( Ptr, CapObjAddr+Number*10, 9 );
    	for( j=0;j<InitCapNumber;j++ )
    	{
    		if( Data_Comp( Ptr, InitCapObjTab+j*8, 8 ) == 0 )			//�����ж����е������Ƿ���׼����ʼ����һ��
    		{
    			Buff3[j] = 0x01;										//һ��
    			break;	
    		}
    	}
    	if( j == InitCapNumber )										//���е����߲��ڳ�ʼ������У���ɾ��		
    	{
    		DeleteFreezeCapture_OAD( ItemNo, Ptr+2 );					//ɾ����������
//    		CaptureItems = GetFreezeCaptureItems( ItemNo );				//��ȡ������
    	}
    	else Number++;
    }
    
    for( i=0;i<InitCapNumber;i++ )										//���ó�ʼ������
    {
    	if( Buff3[i] == 0x01 ) continue;								//�����OAD�Ѵ��ڣ����账��
    	FillInitFreezeItemBuff( Point, InitCapObjTab+i*8, InitCapObjTab+2+i*8, InitCapObjTab+6+i*8 );	//���ó�����һ���������߸�ʽ	
    	AddFreezeCapture_OAD( ItemNo, Point );							//����һ������				
    }
}    

void InitFreezeCycle( void )
{
	switch( SM.InitFreezeState )	
	{
		case InitFreezeIdle:			// ��ʼ�������������
			SM.InitFreezeStateChk = InitFreezeIdle;			
			break;
		case InitFreezeStep1:			// ��ʼ�������������1
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( InstantFreeze_No );
			SM.InitFreezeState = InitFreezeStep2;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep2:			// ��ʼ�������������2
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( MinFreeze_No );
			SM.InitFreezeState = InitFreezeStep3;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep3:			// ��ʼ�������������3
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( HourFreeze_No );
			SM.InitFreezeState = InitFreezeStep4;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep4:			// ��ʼ�������������4
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( DayFreeze_No );
			SM.InitFreezeState = InitFreezeStep5;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep5:			// ��ʼ�������������5
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( SaveDayFreeze_No );
			SM.InitFreezeState = InitFreezeStep6;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep6:			// ��ʼ�������������6
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( MonthFreeze_No );
			SM.InitFreezeState = InitFreezeStep7;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep7:			// ��ʼ�������������7
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( YearFreeze_No );
			SM.InitFreezeState = InitFreezeStep8;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep8:			// ��ʼ�������������8
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( ZoneSWFreeze_No );
			SM.InitFreezeState = InitFreezeStep9;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
			break;
		case InitFreezeStep9:			// ��ʼ�������������9
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( SegTabSWFreeze_No );
#if ( LocalSmartMeter == YesCheck ) 
			SM.InitFreezeState = InitFreezeStep10;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;
#else
			SM.InitFreezeState = InitFreezeIdle;
			SM.InitFreezeStateChk = InitFreezeIdle;
#endif			
			break;
#if ( LocalSmartMeter == YesCheck ) 
		case InitFreezeStep10:			// ��ʼ�������������10
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( FeeSWFreeze_No );
			SM.InitFreezeState = InitFreezeStep11;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;;
			break;
		case InitFreezeStep11:			// ��ʼ�������������10
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( StepSWFreeze_No );
			SM.InitFreezeState = InitFreezeStep12;
			SM.InitFreezeStateChk = SM.InitFreezeState + 0xA5A5;;
			break;
		case InitFreezeStep12:			// ��ʼ�������������10
			if( SM.InitFreezeStateChk != ( SM.InitFreezeState +0xA5A5 ) ) break;
			InitFreezeTab( StepSaveSWFreeze_No );
			SM.InitFreezeState = InitFreezeIdle;
			SM.InitFreezeStateChk = InitFreezeIdle;
			break;
#endif
		default: 
			SM.InitFreezeState = InitFreezeIdle;
			SM.InitFreezeStateChk = InitFreezeIdle;			
			break;	
	}
}

//ɾ�����ж����ļ��������ļ���ʽ����ɾ����������
void DeletAllFreezeFile( void )
{
	unsigned short Addr;
    unsigned short CaptureItems;
    short i;
	
//	if(( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 ) return; 			//У���̽Ӳ�����ִ��		//�������ļ���ʽ��ͬ��ִ��	//17.03.30
	
//	for( i=0;i<StepSaveSWFreeze_No;i++ )					
	for( i=0;i<(StepSaveSWFreeze_No+1);i++ )						//17.03.30
	{
		Addr = DL698_Freeze_ProfileTab[i].CapNumAddr;
		CaptureItems = 0;
		E2P_WData( Addr, (unsigned char*)&CaptureItems, 1 );		//���ÿ���������ߵ��ļ���
	}	
	
	return;
}	



