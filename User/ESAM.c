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
#include "cpucard.h"

#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif
/*
unsigned char ESAM_ReadByte(void)
{
	unsigned short i;
	unsigned char Temp;
	
	Temp = 0;
	for( i=0;i<8;i++ )
	{
		__NOP();
//		Delay(2);						
		HT_GPIOG->PTCLR |= GPIOG_ESAM_CLK;
		__NOP();						
		__NOP();						
//		Delay(2);						
		HT_GPIOG->PTSET |= GPIOG_ESAM_CLK;
		__NOP();						
//		Delay(2);						
		Temp <<= 1;
		if(( HT_GPIOG->PTDAT & GPIOG_ESAM_MISO ) != 0 ) Temp |= 0x01;	
	}	
	return Temp;
}
*/
unsigned char ESAM_ReadByte(void)
{
	unsigned short i;
	unsigned char Temp;
	
	Temp = 0;
	for( i=0;i<8;i++ )
	{
		__NOP();
		HT_GPIOG->PTCLR |= GPIOG_ESAM_CLK;
		__NOP();						
		__NOP();						
		Temp <<= 1;
		if(( HT_GPIOG->PTDAT & GPIOG_ESAM_MISO ) != 0 ) Temp |= 0x01;	
		HT_GPIOG->PTSET |= GPIOG_ESAM_CLK;
		__NOP();						
//		Delay(2);						
//		Temp <<= 1;
//		if(( HT_GPIOG->PTDAT & GPIOG_ESAM_MISO ) != 0 ) Temp |= 0x01;	
	}	
	return Temp;
}

void ESAM_WriteByte(unsigned char Byte)
{
	unsigned short i;
	
	for( i=0;i<8;i++ )
	{
		__NOP();						
		HT_GPIOG->PTCLR |= GPIOG_ESAM_CLK;
		__NOP();						
		if(( Byte & 0x80 ) == 0x80 ) HT_GPIOG->PTSET |= GPIOG_ESAM_MOSI;
		else HT_GPIOG->PTCLR |= GPIOG_ESAM_MOSI;
		__NOP();						
		__NOP();						
		HT_GPIOG->PTSET |= GPIOG_ESAM_CLK;
		Byte <<= 1;
	}	
	HT_GPIOG->PTSET |= GPIOG_ESAM_MOSI;
}

//short ESAM_Busy(void)
//{
//	unsigned short	i;
//	unsigned char Temp;			
//
//	HT_GPIOG->PTCLR |= GPIOG_ESAM_CS;
//	for( i=0; i<7500; i++ ) {		
//		HT_FreeDog();
//		Delay(10);		
//		Temp = ESAM_ReadByte();	
//		if( Temp == 0x55 )	break;
//	}
//	Delay(3);		
//	HT_GPIOB->PTSET |= GPIOG_ESAM_CS;
//	
//	if( i<7500 ) return 0;	
//	else return -1;
//}

unsigned char GetESAM_LRC( unsigned char* Source, unsigned short Len )
{
	short i;
	unsigned char Temp;
	
	Temp = *Source;
	for( i=1;i<Len;i++ )
	{
        Source++;
		Temp ^= *Source;	
	}	
	Temp = ~Temp;
	
	return Temp;
}	

//���ͻ���ͽ��ջ���ֿ�
short ESAM_Operation( unsigned char* Dest, unsigned char* Source, unsigned short SendLen, unsigned short* RecLen  )
{
	unsigned short i,j,k;
	unsigned char Temp;		
	unsigned short Len;	
	unsigned char* Ptr;
	
	for( k=0;k<3;k++ )
	{
		Ptr = Source;
		HT_GPIOG->PTCLR |= GPIOG_ESAM_CS;
		Delay(250);
//		Delay(300);
		ESAM_WriteByte(0x55);	
		HT_FreeDog();
		for( i=0;i<SendLen;i++ )
		{
			HT_FreeDog();
			ESAM_WriteByte(*Ptr);
			Ptr++;
		}		
		Delay(15);
//		Delay(20);
		HT_GPIOG->PTSET |= GPIOG_ESAM_CS;
		Delay(60);
//		Delay(70);
    	
    	HT_GPIOG->PTCLR |= GPIOG_ESAM_MOSI;	//16.11.01
    	
		for( j=0;j<3;j++ )
		{
			HT_GPIOG->PTCLR |= GPIOG_ESAM_CS;
			Delay(250);
//			Delay(300);
//			Delay(500);
			
			for( i=0;i<7500;i++ ) 
			{		
				HT_FreeDog();
				Temp = ESAM_ReadByte();	
				if( Temp == 0x55 )	break;
				Delay(75);
//				Delay(220);
			}
			
			if( i == 7500 ) return -1;
    	
			*Dest = ESAM_ReadByte();	
			*(Dest+1) = ESAM_ReadByte();	
			*(Dest+2) = ESAM_ReadByte();	
			*(Dest+3) = ESAM_ReadByte();	
			Len = *(Dest+2);
			Len <<= 8; 
			Len += *(Dest+3);
			if( Len >= 2043 ) return -1;
			Ptr = Dest+4;
            Len += 1;                       //���һ�ֽ�LRC
			HT_FreeDog();
			for( i=0;i<Len;i++ ) 
			{		
				*Ptr = ESAM_ReadByte();	
				Ptr++;
			}
			
			Delay(15);
//			Delay(20);
			HT_GPIOG->PTSET |= GPIOG_ESAM_CS;
			Delay(60);
//			Delay(70);
//			Delay(220);
            
            Len -= 1;                       //���һ�ֽ�LRC
			if( *(Dest+Len+4) == GetESAM_LRC( Dest, Len+4 ) ) break;  
			else __NOP();          
		}	
	
		if( j == 3 ) return -1;
		else
		{
//			if(( *Dest != 0x6A )&&( *(Dest+1) != 0x90 ) ) break;	
			if(( *Dest == 0x90 )&&( *(Dest+1) == 0x00 ) ) break;
			else __NOP();	
		}	
	}
	
	if( k == 3 ) return -1;
	else 
	{
		*RecLen = Len;
		return 0;
	}	
}	

short ReadWriteESAM( unsigned char* Dest, unsigned char CLA, unsigned char INS, unsigned short Addr, unsigned short SendLen )
{
	unsigned short RecLen;
	unsigned char* SendBuff;
	unsigned char* RecBuff;
	
	SendBuff = FBuff.Buff;
	RecBuff = FBuff.Buff + 2048;
	
	*SendBuff = CLA;
	*(SendBuff+1) = INS;
	*(SendBuff+2) = Addr >> 8;
	*(SendBuff+3) = Addr & 0xFF;
	*(SendBuff+4) = SendLen >> 8;
	*(SendBuff+5) = SendLen & 0xFF;
	*(SendBuff+6+SendLen) = GetESAM_LRC( SendBuff, 6+SendLen );
	if( ESAM_Operation( RecBuff, SendBuff, 7+SendLen, &RecLen ) != 0 ) return -1;
	if(( *RecBuff == 0x90 )&&( *(RecBuff+1) == 0x00)) 
	{
		RAM_Write( Dest, RecBuff+4, RecLen );
		return RecLen;
	}
	else return -1;
}

short GetESAMSaftInfo( unsigned char* Dest, unsigned short Addr )
{
	return ReadWriteESAM( Dest, 0x80, 0x36, Addr, 0x00 );
}

//ȡClass21������
short GetESAMClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short Len;

	RAM_Write( Dest, Source, 4 );

	Len = GetESAMClassDataTab( COM_No, Dest+4, Source );
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



//ȡESAM������
short GetESAMClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 
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

	if(( Attribute >= 1 )&&( Attribute <= 7 ))													//17.04.09	
	{
		if( Judge_operating_authority( COM_No, OAD, 0x8410, read_authority ) != 0 ) return -2;	//17.04.09
 	}
	*Dest = Get_data;								//�������ݷ���	
	switch( Attribute )
	{
		case 1:										//logical_name
			*(Dest+1) = D_OctetStr;
			*(Dest+2) = 0x02;
			RAM_Write( Dest+3, OAD, 2 );
			Len = 5;
			break;	
		case 2:										//ESAM���к�
			*(Dest+1) = D_OctetStr;
			*(Dest+2) = 0x08;
			Len = GetESAMSaftInfo( Dest+3, 0x02 );
			if( Len != -1 ) Len += 3;
			break;	
		case 3:										//ESAM�汾��	
			*(Dest+1) = D_OctetStr;
			*(Dest+2) = 0x05;
			Len = GetESAMSaftInfo( Dest+3, 0x01 );
			if( Len != -1 ) Len += 3;
			break;	
		case 4:										//�Գ���Կ�汾	
			*(Dest+1) = D_OctetStr;
			*(Dest+2) = 0x10;
			Len = GetESAMSaftInfo( Dest+3, 0x04 );
			if( Len != -1 ) Len += 3;
			break;	
		case 5:										//�ỰʱЧ����
			*(Dest+1) = D_Unsigned32;
			Len = GetESAMSaftInfo( Dest+2, 0x05 );
			if( Len != -1 ) Len = 6;				//ȥ�������ĻỰʱЧʣ��ʱ��
			break;
		case 6:										//�ỰʱЧʣ��ʱ��	
			*(Dest+1) = D_Unsigned32;
			Len = GetESAMSaftInfo( Dest+2, 0x05 );
			if( Len != -1 )
			{
				RAM_Write( Dest+2, Dest+6, 4 );		//ȥ��ǰ���ʱЧ����
				Len = 6;					
			}
			break;
		case 7:										//��ǰ������
			*(Dest+1) = Data_structure;
			*(Dest+2) = 3;
			Len = GetESAMSaftInfo( Dest+10, 0x06 );
			if( Len != -1 )
			{
				*(Dest+3) = D_Unsigned32;
				RAM_Write( Dest+4, Dest+10, 4 );		
				*(Dest+8) = D_Unsigned32;
				RAM_Write( Dest+9, Dest+14, 4 );		
				*(Dest+13) = D_Unsigned32;
				RAM_Write( Dest+14, Dest+18, 4 );		
				Len = 18;					
			}
			break;
		case 8:										//֤��汾
			break;
		case 9:										//�ն�֤�����к�
			break;
		case 10:									//�ն�֤��
			break;
		case 11:									//��վ֤�����к�
			break;
		case 12:									//��վ֤��
			break;
		case 13:									//ESAM��ȫ�洢�����б�
			break;
		default:
			break;	
	}
	
	return Len;
}	

//-------------------------------------------------------------------------------------------------------
unsigned char SetESAMClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = SetESAMClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen  );
	return ErrorByte;		

}		

//ȡESAM������
unsigned char SetESAMClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
//	unsigned char Buff[20];
//	short i;
//	unsigned short Addr;
	unsigned char ErrorByte=object_class_inconsistent;
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

	switch( Attribute )
	{
		case 2:										//ESAM���к�
			break;	
		case 3:										//ESAM�汾��	
			break;	
		case 4:										//�Գ���Կ�汾	
			break;	
		case 5:										//�ỰʱЧ����
			break;
		case 6:										//�ỰʱЧʣ��ʱ��	
			break;
		case 7:										//��ǰ������
			break;
		case 8:										//֤��汾
			break;
		case 9:										//�ն�֤�����к�
			break;
		case 10:									//�ն�֤��
			break;
		case 11:									//��վ֤�����к�
			break;
		case 12:									//��վ֤��
			break;
		case 13:									//ESAM��ȫ�洢�����б�
			break;
		default:
			ErrorByte = object_class_inconsistent;
			break;	
	}
	return ErrorByte;
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionESAMClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionESAMClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen );
	return ErrorByte;		

}	

unsigned char ActionESAMClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char Buff[150];
	unsigned long ComID;
//	unsigned long Value;
	short i;
	unsigned short Addr;
	unsigned char ErrorByte=object_class_inconsistent;
	short Len;
	short Length;
	unsigned char* Ptr;
	unsigned char* OMD;
	unsigned char Method;
	unsigned char* ESAMSendBuff;
//	unsigned char* ESAMRecBuff;
	unsigned char AttachLen;			
	unsigned char* SIDPtr;
	unsigned char* AttachPtr;
	unsigned char* MACPtr;
	unsigned char* DataPtr;
#if ( LocalSmartMeter == YesCheck )
    short Items;
    unsigned char OperateType;		
#endif
    
	ESAMSendBuff = FBuff.Buff;
//	ESAMRecBuff = FBuff.Buff + 2048;
	
	OMD = Source;
//	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OMD );
	Method = *(OMD+2);
	
	Ptr = Buff;
	
	if(( Method >= 3 )&&( Method <= 10 ))			//17.04.09
	{
		if( Judge_operating_authority( COM_No, OMD, 0x0010, active_authority ) != 0 ) return read_write_denied;	//17.04.09
	}
	switch( Method )
	{
		case 1:										//��λ
			break;	
		case 2:										//ִ��	
			break;	
		case 3:										//ESAM���ݶ�ȡ
			if( *(Source+4) != D_SID ) return type_unmatched;
//			Len = *(Source+5+4);
//			RAM_Write( ESAMSendBuff+6, Source+10+2, Len-2 );
//			Len = ReadWriteESAM( Dest+6, *(Source+5), *(Source+6), SwapCharToShort( Source+7 ), SwapCharToShort( Source+10 ) );		//��֤SID_MAC
			SIDPtr = Source+5;
			AttachLen = *(SIDPtr+4);
			AttachPtr = SIDPtr+5;
			RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
			Len = ReadWriteESAM( Dest+6, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID
			if( Len == -1 ) return security_unmatched;
			else
			{
				*Dest = 0x00;						//DAR = 0
				*(Dest+1) = 0x01;					//������������ OPTIONAL=1
				*(Dest+2) = D_OctetStr;
			}
			*BackLen = Len + 1;						//��1�ֽ���������	
			FillVarLengthData( Dest+3, Dest+6, BackLen, Len );		//���������ֽ�
            ErrorByte = 0;
			break;	
		case 4:										//���ݸ���		SID_MAC
			if(( *(Source+4) != Data_structure )||( *(Source+5) != 0x02)||( *(Source+6) != D_OctetStr)) return type_unmatched;
			if( *(Source+7) == 0x82 ) 
			{
				Len = SwapCharToShort( Source+8 );
				SIDPtr = Source+11+Len;				//SID���������ͣ�����
				DataPtr = Source+10;
			}	
			else if( *(Source+7) == 0x81 ) 
			{
				Len = *(Source+8);
				SIDPtr = Source+10+Len;				//SID���������ͣ�����
				DataPtr = Source+9;
			}	
			else 
			{
				Len = *(Source+7);
				SIDPtr = Source+9+Len;				//SID���������ͣ�����
				DataPtr = Source+8;
			}	
			Length = Len - 5;                       //ȥ����ŵ�OAD�뼰����    
			ComID = SwapCharToLong( DataPtr );
			if(( ComID == 0x40020200 )&&( ICComm.RPrivateKeyFlag == 0 )) return temporary_failure;		//˽Կ�²��������ñ��
			AttachLen = *(SIDPtr+4);
			AttachPtr = SIDPtr+5;
			MACPtr = AttachPtr + AttachLen;
//			RAM_Write( ESAMSendBuff+6, DataPtr, Len );
//			RAM_Write( ESAMSendBuff+6+Len, AttachPtr+AttachLen+1, 4 );					//MAC
			RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
			if( ComID == 0x40020200 )														//�������ʱ��OAD+����
			{
				RAM_Write( ESAMSendBuff+6+AttachLen-2, DataPtr, Len );
				RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, MACPtr+1, 4 );					//MAC
			}
			else																			//�Ǳ������ʱ����OAD������	
			{
				RAM_Write( ESAMSendBuff+6+AttachLen-2, DataPtr+5, Len-5 );					
				RAM_Write( ESAMSendBuff+6+AttachLen-2+Len-5, MACPtr+1, 4 );					//MAC				
			}		
			Len = ReadWriteESAM( Dest+6, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID_MAC
			if( Len == -1 ) return security_unmatched;
			ComID = SwapCharToLong( DataPtr );
            ErrorByte = 0;
			if( ComID == 0x40020200 )				//���
			{
				RAM_Fill( Ptr, 8);
//				RAM_Write( Ptr, DataPtr+5, 8 );	
//				SwapData( Ptr, 8 );
				if( Length > 8 ) Length = 8;	
				RAM_Write( Ptr, DataPtr+5, Length );	
				SwapData( Ptr, Length );
				E2P_WData( EMeter_No, Ptr, 8 );
			}
#if ( LocalSmartMeter == YesCheck )
			else if( ComID == 0x40030200 )			//�ͻ����
			{
				RAM_Write( Ptr, DataPtr+5, 6 );	
				SwapData( Ptr, 6 );
				E2P_WData( Client_No, Ptr, 6 );
			}
			else if( ComID == 0x40190200 )			//���õ��
			{
				Items = *(DataPtr+4)/4;
				if( *(DataPtr+4) > 128 ) return type_unmatched;					//���128�ֽ�
				RecordStartEvent( FeePricePrg_No );
				RAM_Write( Ptr, DataPtr+5, *(DataPtr+4) );				
				Addr = Case2Fee1Price;
				for( i=0;i<Items; i++ )
				{
					SwapData( Ptr, 4 );
					E2P_WData( Addr, Ptr, 4 );
					Addr += 5;
					Ptr += 4;
				}	
			}		
			else if( ComID == 0x401B0200 )			//���ý��ݵ��
			{
				if( *(DataPtr+4) != 64 ) return type_unmatched;					//�̶�64�ֽ�
				RecordStartEvent( StepParaPrg_No );
				Addr = Case2Step1EC;
				RAM_Write( Ptr, DataPtr+5, 64 );				
				for( i=0;i<13; i++ )
				{
					SwapData( Ptr, 4 );
					E2P_WData( Addr, Ptr, 4 );
					Addr += 5;
					Ptr += 4;
				}
				Addr = Case2YearSaveDay1;
				for( i=0;i<13; i++ )
				{
					SwapData( Ptr, 3 );
					E2P_WData( Addr, Ptr, 3 );
					Addr += 4;
					Ptr += 3;
				}					
			}		
			else if(( ComID == 0x401E0201 )||( ComID == 0x401E0202 ))				//�������1��2
			{
				if( *(DataPtr+4) != 4 ) return type_unmatched;						//�̶�4�ֽ�
				if( ComID == 0x401E0201 ) Addr = AlarmEC1;
				else Addr = AlarmEC2;
				RAM_Write( Ptr, DataPtr+5, 4 );			//4�ֽ�BCD
				SwapData( Ptr, 4 );
				E2P_WData( Addr, Ptr, 4 );
			}	
			else if( ComID == 0x401E0200 )				//�������1��2
			{
				if( *(DataPtr+4) != 8 ) return type_unmatched;						//�̶�8�ֽ�
				RAM_Write( Ptr, DataPtr+5, 8 );			//��4�ֽ�BCD
				SwapData( Ptr, 4 );
				E2P_WData( AlarmEC1, Ptr, 4 );
				SwapData( Ptr+4, 4 );
				E2P_WData( AlarmEC2, Ptr+4, 4 );				
			}	
			else if( ComID == 0x400A0200 )				//���׷�ʱ�����л�ʱ��
			{
				WriteFeeStepSwitchTime( FeeSwitchTime, OldFeeSwitchTime, DataPtr+5 );
			}
			else if( ComID == 0x400B0200 )				//���׽��ݵ���л�ʱ��
			{
				WriteFeeStepSwitchTime( StepSwitchTime, OldStepSwitchTime, DataPtr+5 );
			}
#endif
			else if(( ComID == 0x401C0200 )||( ComID == 0x401D0200 ))				//�������
			{
				if( ComID == 0x401C0200 ) Addr = CurrentRate;
				else Addr = VoltRate;
				RAM_Write( Ptr, DataPtr+5, 3 );		//3�ֽ�BCD
				SwapData( Ptr, 3 );
				WriteVoltCurrentRate( Addr, Ptr );
			}	
			else 
			{
				return object_class_inconsistent;
			}	
			
			CreatPrgRecEvent( DataPtr );
#if ( LocalSmartMeter == YesCheck )
			if(( ComID == 0x401C0200 )||( ComID == 0x401D0200 )||( ComID == 0x40020200 )||( ComID == 0x40030200 )) 	//11.07.18
			{
				if( WriteESAMRunInfoFile() != 0 ) 									//�޸ĵ�ѹ������ȺͿͻ����ʱдESAM������Ϣ�ļ�
				{
					return object_class_inconsistent; 	
				}		
			}	
#endif
			break;	
		case 5:										//Э��ʧЧ
			*Dest = 0x00;							//DAR = 0
			*(Dest+1) = 0x01;						//������������ OPTIONAL=1
			*(Dest+2) = D_date_time_s;
			RAM_Write( Dest+3, &Clk.YearH, 7 );			
			*BackLen = 8;	
            ErrorByte = 0;
			break;
		case 6:										//Ǯ������(��������ֵ���˷�)	
#if ( LocalSmartMeter == YesCheck )
			if(( *(Source+4) != Data_structure )||( *(Source+5) != 0x06)||( *(Source+6) != D_Integer8)) return type_unmatched;
			OperateType = *(Source+7);						//Ǯ����������
			if( OperateType == 0x02 )						//�˷�	
			{
				ErrorByte = RefundmentProcss698( Source );	
				return ErrorByte;
			}
			else
			{
				ErrorByte = OpenAndRecharge( OperateType, Source );	
				return ErrorByte;
			}					
#else
			ErrorByte = object_class_inconsistent;
#endif
			break;
		case 7:										//��Կ����	SID_MAC
			if(( *(Source+4) != Data_structure )||( *(Source+5) != 0x02)||( *(Source+6) != D_OctetStr)) return type_unmatched;
			if( *(Source+7) == 0x82 ) 
			{
				Len = SwapCharToShort( Source+8 );
				SIDPtr = Source+11+Len;				//SID���������ͣ�����
				DataPtr = Source+10;
			}	
			else if( *(Source+7) == 0x81 ) 
			{
				Len = *(Source+8);
				SIDPtr = Source+10+Len;				//SID���������ͣ�����
				DataPtr = Source+9;
			}	
			else 
			{
				Len = *(Source+7);
				SIDPtr = Source+9+Len;				//SID���������ͣ�����
				DataPtr = Source+8;
			}	
			AttachLen = *(SIDPtr+4);
			AttachPtr = SIDPtr+5;
			MACPtr = AttachPtr + AttachLen;
			
			if( SwapCharToLong( SIDPtr ) != 0x812E0000) return security_unmatched;
			RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
			RAM_Write( ESAMSendBuff+6+AttachLen-2, DataPtr, Len );
			RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, MACPtr+1, 4 );					//MAC
			Len = ReadWriteESAM( Dest+6, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID_MAC
			if( Len == -1 ) return security_unmatched;
			RecordStartEvent( SecretKeyPrg_No );
			GetCurSecretKeyState();
            ErrorByte = 0;
			break;
		case 8:										//֤�����	
		case 9:										//����Э��ʱЧ	SID	
			if(( *(Source+4) != Data_structure )||( *(Source+5) != 0x02)||( *(Source+6) != D_OctetStr)) return type_unmatched;
			if( *(Source+7) == 0x82 ) 
			{
				Len = SwapCharToShort( Source+8 );
				SIDPtr = Source+11+Len;				//SID���������ͣ�����
				DataPtr = Source+10;
			}	
			else if( *(Source+7) == 0x81 ) 
			{
				Len = *(Source+8);
				SIDPtr = Source+10+Len;				//SID���������ͣ�����
				DataPtr = Source+9;
			}	
			else 
			{
				Len = *(Source+7);
				SIDPtr = Source+9+Len;				//SID���������ͣ�����
				DataPtr = Source+8;
			}	
			AttachLen = *(SIDPtr+4);
			AttachPtr = SIDPtr+5;
//			MACPtr = AttachPtr + AttachLen;
			
			RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
			RAM_Write( ESAMSendBuff+6+AttachLen-2, DataPtr, Len );
			Len = ReadWriteESAM( Dest+6, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID
			if( Len == -1 ) return security_unmatched;
            ErrorByte = 0;
			break;
		case 10:									//Ǯ����ʼ��	SID_MAC
#if ( LocalSmartMeter == YesCheck )
			if( ICComm.RPrivateKeyFlag == 0x00 ) return security_unmatched;		//˽Կ�²��������
			
			if(( *(Source+4) != Data_structure )||( *(Source+5) != 0x02)||( *(Source+6) != D_Unsigned32)) return type_unmatched;
			Len = 4;
			SIDPtr = Source+8+4;					//SID���������ͣ�����
			DataPtr = Source+7;
			AttachLen = *(SIDPtr+4);
			AttachPtr = SIDPtr+5;
			MACPtr = AttachPtr + AttachLen;
			
			RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
			RAM_Write( ESAMSendBuff+6+AttachLen-2, DataPtr, Len );
			RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, MACPtr+1, 4 );					//MAC
			Len = ReadWriteESAM( Dest+6, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID_MAC
			if( Len == -1 )
			{
				AddIdentityFaultCnt(); 
				return security_unmatched;
			}else InitFaultCnt();	

			Clr_AccountFlag();
			Clr_ReOverEC();
//			SwapData( DataPtr, 4 );	
			AddBuyECSumBCD( DataPtr );						 //�ۼ��ۼƹ�����					

			RAM_Fill( Ptr, 8);
			E2P_WData( BfERemainEC, Ptr, 4 );							//��ʼ��ǰʣ�����
			E2P_WData( BuyECCnt, Ptr, 4 );																	
			E2P_WData( UserCardNo, Ptr, 8 ); 						//�忨���к�
			E2P_WData( ILInsert_Cunt, Ptr, 3 );
			E2P_WData( BuyECFlag, Ptr, 1 );								//����ɹ�
			ClearJudgePermitFlag();					//����к�բ�������־	//�¹���	//13.12.10

			Para.RMeterTripState = 0x1B;
			E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );	

			Para.RMeterAlarmState = 0x2B;
			E2P_WData( MeterAlarmState, (unsigned char*)&Para.RMeterAlarmState, 1 );	

//			if( WriteESAMRunInfoFile() != 0 ) { Err_Byte |= 0x01; break; }	//дESAM������Ϣ�ļ�
			if( WriteESAMRunInfoFile() != 0 ) return object_class_inconsistent; 	//дESAM������Ϣ�ļ�

//			if(((PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) ClrMeterClrRec();			//�¹���	//13.10.09	//14.04.09
        	if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) ResetSignleClass7Event( ECClr_No );		//�����������¼
			SM.MeterClrState = MeterClrStep1;							//���������������		
   			GetECState();										

			if(( ICComm.ICECState == F_ECAlarm20 )||( ICComm.ICECState == F_ECOver ))	//ȡ��Ԥ��բ	//�¹���	//13.12.23	
			{
				ExitPreTripState();
				ICComm.RelayOFFState = 0;
				ICComm.RelayONState = 0x00;
			}
			KeyRelayOn();												//�������բ״̬�£�ֱ�Ӻ�բ
			ClearJudgePermitFlag();										//�¹���	//13.12.10    					
#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
   			ClearAllEventUpState();						//�¹���	//14.04.28    
#endif
	   		ICComm.ICInState &= ~F_RegECLimt;			// ȡ���ڻ���ʾ	//�¹���	//14.06.06
            ErrorByte = 0;
#else
			if(( *(Source+4) != Data_structure )||( *(Source+5) != 0x02)||( *(Source+6) != D_Unsigned32)) return type_unmatched;
			Len = 4;
			SIDPtr = Source+8+4;					//SID���������ͣ�����
			DataPtr = Source+7;
			AttachLen = *(SIDPtr+4);
			AttachPtr = SIDPtr+5;
			MACPtr = AttachPtr + AttachLen;
			
			RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
			RAM_Write( ESAMSendBuff+6+AttachLen-2, DataPtr, Len );
			RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, MACPtr+1, 4 );					//MAC
			Len = ReadWriteESAM( Dest+6, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID_MAC
			if( Len == -1 ) return security_unmatched;
            ErrorByte = 0;
#endif
			break;
		default:
			break;	
	}	
	return ErrorByte;
}

#if ( LocalSmartMeter == YesCheck )
unsigned char WriteWalletFile( unsigned char* Source )
{
	unsigned char Buff[50];
	short Len;
	unsigned char* Ptr;
	unsigned char* ESAMSendBuff;
	unsigned char AttachLen;			
	unsigned char* SIDPtr;
	unsigned char* AttachPtr;
	unsigned char* MACPtr;
	unsigned char* DataPtr;
    
	ESAMSendBuff = FBuff.Buff;
	
	Ptr = Buff;

	SIDPtr = Source+6+2+5+5+8+1;					//SID���������ͣ�����
	AttachLen = *(SIDPtr+4);
	AttachPtr = SIDPtr+5;
	MACPtr = AttachPtr + AttachLen;
	
	RAM_Write( ESAMSendBuff+6, AttachPtr+2, AttachLen-2 );						//������Ϣ
	Len = 0;
	DataPtr = Source+6+2+1;
	RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, DataPtr, 4 );					//������
	Len += 4;
	DataPtr += 5;
	RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, DataPtr, 4 );					//�������
	Len += 4;
	DataPtr += 5+1;																//��1�ֽڳ����ֽ�	
	RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, DataPtr, 6 );					//����
	Len += 6;
	RAM_Write( ESAMSendBuff+6+AttachLen-2+Len, MACPtr+1, 4 );					//MAC
	Len = ReadWriteESAM( Ptr, *SIDPtr, *(SIDPtr+1), SwapCharToShort( SIDPtr+2 ), SwapCharToShort( AttachPtr ) );		//��֤SID_MAC
	if( Len == -1 )
	{
		AddIdentityFaultCnt(); 
		return security_unmatched;
	}else InitFaultCnt();	
	
	return 0;
}	


//�˷�Ԥ����
unsigned char RefundmentProcss698( unsigned char* Source )			
{
 	unsigned char Buff[20];					
 	unsigned char Buff2[20];				
 	unsigned char* Point;					
 	unsigned char* Ptr;
 	unsigned char* Data;
	unsigned char ErrorByte;

	Ptr = Buff;
    Point = Buff2;
    Data = Source+6+2+1;
    
	if( Write_ESAMBurse() != 0 ) { Flag.MeterState1 |= F_ESAMErr; return esam_validation_failure; } //��ESAM�ڵ�ʣ����ʧ��

	E2P_RData( Ptr, BuyECFlag, 1 );									//����ɹ������Ƿ��˷ѹ���־
	if( *Ptr != 0 ) { return scope_of_access_violated; } //����ɹ��������˷ѹ�һ��
	
	RAM_Fill( Ptr, 12);
	GetReOverEC( Ptr, Ptr+4 );

	RAM_Write( Point, Data, 4 );
	SwapData( Point, 4 );
	if( Data_Comp( Point, Ptr, 4 ) > 0 ) { return scope_of_access_violated; }		//�˷ѽ�����ʣ����ܾ��˷�

	ErrorByte = WriteWalletFile( Source );
	if( ErrorByte != 0 ) { return ErrorByte;; }		
	
	E2P_WData( EFeeBack, Point, 4 );								//�����˷ѽ��
	RecordStartEvent( FeeBack_No );
	DecBuyECSumBCD( Data ); 										//�˷��ۼƹ�����		
	*Ptr = 0x55;
	E2P_WData( BuyECFlag, Ptr, 1 );									//��¼���˷ѹ���־
	RecordEndEvent( FeeBack_No, 0 );

	RAM_Fill( Ptr, 12);
	GetReOverEC( Ptr, Ptr+4 );
	GetICECState( Ptr, Ptr+4 );

	return 0;
}

unsigned char OpenAndRecharge( unsigned char OperateType, unsigned char* Source )
{
 	unsigned char Buff[20];					
// 	unsigned char Buff2[20];				
// 	unsigned char* Point;					
 	unsigned char* Ptr;
 	unsigned char* Data;
 	unsigned long Value1,Value2,Value3,Value4;
	unsigned char ErrorByte;

	Ptr = Buff;
//   Point = Buff2;
    Data = Source+6+2+1;												//�������������ֽ�
    
    if( OperateType > 0x01 ) return scope_of_access_violated;
	
	if( ICComm.RPrivateKeyFlag != 0x00 ) return scope_of_access_violated;		////��ȫ��˽Կ�²���

	E2P_RData( (unsigned char*)&ICComm.ROpenAccountFlag, OpenAccountFlag, 1 );				//���������
	if(( ICComm.ROpenAccountFlag & 0x03 )== 0x03 )										//���δ����	
	{                                                           		
		if( OperateType == 0x01 ) {	return scope_of_access_violated; }			//��ֵ		//01
	}                                                           		
	else                                                        		
	{                                                           		
//		RAM_Write( Ptr, WriteBufAds+12, 6 );							//ȡ�ͻ����
		RAM_Write( Ptr, Data+11, 6 );									//ȡ�ͻ����
		SwapData( Ptr, 6 );
		E2P_RData( Ptr+6, Client_No, 6 );								//�ÿͻ����
		if( Data_Comp( Ptr, Ptr+6, 6 ) != 0 ) { return client_No_unmatched; } //�ͻ���Ų��ȣ���ֵ��ֹ
//		SwapData( WriteBufAds+12, 6 );	
//		SwapData( WriteBufAds+18, 4 );	
//		if( UpdateBinaryMAC( ESAM, WriteBufAds+12, FILE_No2, 36, 6+4, 0, ICEC.DisperseGene ) ) { AddIdentityFaultCnt(); Err_Byte |= 0x04; break; } //����ESAM������Ϣ�ļ��еĿͻ����					
//		else InitFaultCnt();	
	}		
	
//	RAM_Write( Ptr, WriteBufAds+4, 4 );									//�������
	RAM_Write( Ptr, Data+5, 4 );										//�������
	SwapData( Ptr, 4 );	
	RAM_Write( (unsigned char*)&Value2, Ptr, 4 );						//PSAM�Ĺ������
	if( OperateType == 0x00 )											//����
	{
		if( Value2 > 1 ) { return recharge_error;  }								//��ֵ��������
	}
	
//	if( ReadRecord( ESAM, Ptr, FILE_No3, 0x0C, 4 ) != 0 ) { return 0x01; }		//��ESAMǮ���ļ��ڵĹ������
	if( ReadRecord( ESAM, Ptr ) != 0 ) { return other_reason; }					//��ESAMǮ���ļ��ڵĹ��������������ͻ����	//01
	RAM_Write( Ptr, Ptr+4, 4 );											//��ESAMǮ���ļ��ڵĹ������	//�¹���	//14.01.06				
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value1, Ptr, 4 );						//ESAM�Ĺ������				

	if( Value2 < Value1 ) { return  recharge_error; }					//��ֵ��������				
	if( Value2 > (Value1 + 1) )  { return  recharge_error; }			//��ֵ��������				
	if( Value2 == (Value1 + 1) )
	{
		Value4 = Value2;
		if( Write_ESAMBurse() != 0 ) { Flag.MeterState1 |= F_ESAMErr; return esam_validation_failure; } //��ESAM�ڵ�ʣ����ʧ��		//04
	
		GetReOverEC( Ptr, Ptr+4 );										//ʣ�����
	
//		RAM_Write( Ptr+8, WriteBufAds, 4 );
		RAM_Write( Ptr+8, Data, 4 );
		SwapData( Ptr+8, 4 );
		RAM_Write( (unsigned char*)&Value1, Ptr, 4 );
		RAM_Write( (unsigned char*)&Value2, Ptr+8, 4 );
		Value1 += Value2;												//ʣ������ӱ��ι�����
	
		E2P_RData( Ptr+8, RegrateECLimt, 4 );
		Value2 = BCD4_Long( Ptr+8 );
		if( Value2 == 0 ) Value2 = 99999999;							//�ڻ��޶�=0������999999.99Ԫ����	//�¹���	//13.12.10
		RAM_Write( (unsigned char*)&Value3, Ptr+4, 4 );
		Value2 += Value3;												//�ڻ��޶�+͸֧����	
	
		if( Value1 <= Value2 )
		{
			E2P_WData( BfERemainEC, Ptr, 4 );							//��������ǰʣ�����
			E2P_WData( BfOverdraftEC, Ptr+4, 4 );						//��������ǰ͸֧����
	
			ErrorByte = WriteWalletFile( Source );
			if( ErrorByte != 0 ) { return ErrorByte; }		

//			SwapData( WriteBufAds, 4 );	
//			SwapData( WriteBufAds+4, 4 );	
//			SwapData( WriteBufAds+8, 4 );					
//			if( UpdateBinaryIncMAC( ESAM, WriteBufAds, ICEC.DisperseGene ) != 0 ) { AddIdentityFaultCnt(); Err_Byte |= 0x04; break; } 	//����������͹�����д��ESAM
//			else InitFaultCnt();	
			
			RecordStartEvent( BuyMoney_No );
//			RAM_Write( Ptr, WriteBufAds, 4 );							//������
			RAM_Write( Ptr, Data, 4 );									//������
			AddBuyECSumBCD( Ptr );					 					//�ۼ��ۼƹ�����		
		
			E2P_WData( BuyECCnt, (unsigned char*)&Value4, 4 );
			*Ptr = 0x0;
			E2P_WData( BuyECFlag, Ptr, 1 );								//����ɹ�
//			CreatBuyECRec();											//���������¼
			RecordEndEvent( BuyMoney_No, 0 );

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
			ICComm.PreTripOFFKeepTime = 0; 								//�¹���	//14.06.22	
#endif
		}
		else
		{
			return hoard_over; 											//���糬�ڻ�		
		}
	}
	else if( OperateType == 0x01 )										//��ֵ
	{
		return repeated_recharge; 										//��ֵ�������Ϊ�ظ���ֵ
	}
		
	if( OperateType == 0x00 )											//����
	{
		if(( ICComm.ROpenAccountFlag & 0x03 )== 0x03 )					//���δ����
		{
//			RAM_Write( Ptr, WriteBufAds+12, 6 );						//ȡ�ͻ����
			RAM_Write( Ptr, Data+11, 6 );						//ȡ�ͻ����
			SwapData( Ptr, 6 );	
//			SwapData( WriteBufAds+12, 6 );	
//			SwapData( WriteBufAds+18, 4 );	
//			if( UpdateBinaryMAC( ESAM, WriteBufAds+12, FILE_No2, 36, 6+4, 0, ICEC.DisperseGene ) ) { AddIdentityFaultCnt(); Err_Byte |= 0x04; break; } //����ESAM������Ϣ�ļ��еĿͻ����					
//			else InitFaultCnt();	
			E2P_WData( Client_No, Ptr, 6 );								//�ÿͻ����					
		}	

		ICComm.ROpenAccountFlag &= ~0x02;			
		E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );			//��Զ�̿������
	}				
	if( WriteESAMRunInfoFile() != 0 ) { return other_reason; }		//дESAM������Ϣ�ļ�

	return 0;	
}		

void WriteFeeStepSwitchTime( unsigned short Addr, unsigned short OldAddr, unsigned char* Source )
{
	unsigned char Buff[7];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	*Ptr = 0;
	RAM_Write( Ptr+1, Source, 5 );		//5�ֽ�BCD
	SwapData( Ptr+1, 5 );
	ChangeTimeBCDToHex( Ptr, Ptr );
	E2P_WData( Addr, Ptr, 7 );
	RAM_Fill( Ptr, 5);							
	E2P_WData( OldAddr, Ptr, 5 );					
}	

void ReadFeeStepSwitchTime( unsigned char* Dest, unsigned short Addr, unsigned short OldAddr )
{
	unsigned char Buff[7];
	unsigned char Buff2[5];
	unsigned char Buff3[7];
	unsigned char* Ptr;
	unsigned char* Point;
	
	Ptr = Buff;
	Point = Buff2;
	
	E2P_RData( Ptr, Addr, 7 );
	E2P_RData( Point, OldAddr, 5 );
	ChangeTimeHexToBCD( Buff3, Ptr );
	if( Data_Comp( Buff3+1, Point, 5 ) == 0 )		
	{
		RAM_Fill( Dest, 7 );							//���л������л�ʱ�䷵��ʱ����
	}
	else
	{
		SwapData( Ptr, 7 );
		RAM_Write( Dest, Ptr, 7 );
	}						
}	

#endif

//��ѹ��������ȴ�2λС������
void WriteVoltCurrentRate( unsigned short Addr, unsigned char* Source )
{
	unsigned long Value;
	
	Value = BCD3_Long( Source );
	Value *= 100;
	E2P_WData( Addr, (unsigned char*)&Value, 4 );
}	

void GetCurSecretKeyState( void )
{
	unsigned char Buff[20];
	unsigned char Buff2[5];
	unsigned char* Ptr;
	
	Ptr = Buff;
	RAM_DataFill( Buff2, 5, 0xff);
	Buff2[0] &= 0x7F;
	Buff2[4] &= 0xF0;
	
	if( ReadWriteESAM( Ptr, 0x80, 0x36, 0x0004, 0x0000 ) == -1 ) return;
	E2P_RData( (unsigned char*)&Flag.SecretKeyState, ESecretKeyState, 4 );
//	if( IsAllData( Ptr, 16, 0x00 ) == 0 )				//ȫ0Ϊ��Կ
//	if( GetTrueBitSum_128Bit( Ptr, 128 ) < 35 )		//��Կ
	Buff[0] &= 0x7F;
	Buff[4] &= 0xF0;
	if( Data_Comp( Ptr, Buff2, 5 ) != 0 )
	{
		if( IsAllData( (unsigned char*)&Flag.SecretKeyState, 4, 0x00 ) != 0 )
		{
			RAM_DataFill( Flag.SecretKeyState, 4, 0x00);
			E2P_WData( ESecretKeyState, (unsigned char*)&Flag.SecretKeyState, 4 );			
		}
		ICComm.RPrivateKeyFlag = 0x55;				//��Կ	
	}
	else
	{
		if( IsAllData( (unsigned char*)&Flag.SecretKeyState, 4, 0xFF ) != 0 )
		{
			RAM_DataFill( Flag.SecretKeyState, 4, 0xFF);
			E2P_WData( ESecretKeyState, (unsigned char*)&Flag.SecretKeyState, 4 );			
		}
		ICComm.RPrivateKeyFlag = 0;		//˽Կ
#if ( RemoteEnterCalib == YesCheck )								//�¹���	//14.05.23
		SM.RemoteCalibState = 0;
#endif
	}		
}	