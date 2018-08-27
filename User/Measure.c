#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
#include "ht6xxx_lib.h"
#include "TypeFRAM.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
#include "Initial.h"
#include "Time.h"
#include "Power.h"
#include "ECProcess.h"
#include "Measure.h"
#include "COSEMComm.h"
#include <math.h>


#if( MEASCHIP == HT7017)

unsigned char UART_Write(  unsigned char Cmd, unsigned char* Data )
{
//	unsigned char Checksum;
	unsigned char Temp;
	int i,Delaysum;
	
	HT_FreeDog();
	Com_HT7017.Comm_Enable = DISABLE;
//	NVIC_DisableIRQ(TIMER_2_IRQn);
	for(i=0;i<3;i++)
	{//ÿһ��д��������֡���� 6 ���ֽڹ̶�����
		HT_UART3->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);

		Com_HT7017.MeasBuf_Tx[4] = 0x6A;//�����ֽ�֡ͷ���̶�Ϊ 0x6A
		Com_HT7017.MeasBuf_Tx[0] = 0x6A;
		Com_HT7017.MeasBuf_Tx[1] = Cmd + 0x80;
		Com_HT7017.MeasBuf_Tx[4] += Cmd + 0x80;
		Com_HT7017.MeasBuf_Tx[2] = *Data;//д�Ĵ����ǹ̶� 2 �ֽڴ��䣬���ֽ���ǰ
		Com_HT7017.MeasBuf_Tx[4] += *Data;
		Com_HT7017.MeasBuf_Tx[3] = *(Data + 1);
		Com_HT7017.MeasBuf_Tx[4] += *(Data + 1);
		Com_HT7017.MeasBuf_Tx[4] = ~Com_HT7017.MeasBuf_Tx[4];//������֡�ĸ�������ӣ�������λ�����Ľ����λȡ��
		Com_HT7017.MeasState |= ATT7053Set;
		Com_HT7017.MeasState &=~ (ATT7053CommOK + ATT7053Read);
		Delaysum = 3000; //22M 26us  580
		Com_HT7017.Tx_Ptr = 1;
		Com_HT7017.Rx_Ptr = 0;
		Com_HT7017.MeasBuf_Rx[0] = 0;
	//	HT_UART3->UARTCON &=~ UART_UARTCON_RXEN;
		HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;
		HT_UART3->SBUF = Com_HT7017.MeasBuf_Tx[0];			
		HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_TXIF); 
	//	RAM_Fill(Com_HT7017.MeasBuf_Rx, 10 );
	
	Delay(Measur_Delay);
	Delay(Measur_Delay);
	if(Com_HT7017.MeasBuf_Rx[0] == 0x54)
		{	
		Com_HT7017.MeasState &=~ ATT7053Set;
			return 0;
		}
/*
		while((Com_HT7017.MeasState & ATT7053CommOK) != ATT7053CommOK)
		{
//			Delay(10);
			Delay(1);
			Delaysum--;
			if(Delaysum == 0)
			{	
				break;
//				return 1;
			}	
		}
		if(Com_HT7017.MeasBuf_Rx[0] == 0x54)
		{	
//			Delaysum = 30000;
			Temp = 0;
		//	break;
		}
		else 
		{	
//			Delaysum = 30000;
			Temp = 1;
		}
//		NVIC_EnableIRQ(TIMER_2_IRQn);
	*/

	}
	
//	Com_HT7017.MeasState &=~ ATT7053Set;
	return Temp;
}


unsigned char UART_Read(  unsigned char Cmd, unsigned char* Data )
{
	unsigned char Checksum;
	int i,Delaysum;
	
	HT_FreeDog();
	for(i=0;i<2;i++)
	{
		//UCA2IE &=~ (UCRXIE + UCTXIE);
		HT_UART3->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[0] = 0x6A;
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[1] = Cmd;
		Com_HT7017.Cmd = Cmd;
		Checksum += Cmd;
		Com_HT7017.MeasBuf_Tx[5] = Checksum;
		Com_HT7017.MeasBuf_Tx[5] = ~Com_HT7017.MeasBuf_Tx[5];
		Com_HT7017.MeasState |= ATT7053Read;
		Com_HT7017.MeasState &=~ (ATT7053CommOK + ATT7053Set);
//		Delaysum = 2600;	//4M	2012.8.11
		Delaysum = 3000;	//580
		Com_HT7017.Tx_Ptr = 1;
	//	Com_HT7017.Rx_Ptr = 0;
	//	HT_UART3->UARTCON &=~ UART_UARTCON_RXEN;
		HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
		HT_UART3->SBUF = Com_HT7017.MeasBuf_Tx[0];			
/*		RAM_Fill(Com_HT7017.MeasBuf_Rx, 10 );
		while((Com_HT7017.MeasState & ATT7053CommOK) != ATT7053CommOK)
		{
//			Delay(30);
			Delay(1);
			Delaysum--;
			if(Delaysum == 0)
			{
//				Com_HT7017.Rx_Ptr=0;
//				RAM_Fill(Com_HT7017.MeasBuf_Rx, 10 );
//				RAM_Fill(Data, 3 );
//				return 1;	
				break;
			}
		}
			
		Checksum += (Com_HT7017.MeasBuf_Rx[0] + Com_HT7017.MeasBuf_Rx[1] + Com_HT7017.MeasBuf_Rx[2]);
		Checksum = ~Checksum;
		if(Checksum == Com_HT7017.MeasBuf_Rx[3])
			break;
		else
			{
			Com_HT7017.Rx_Ptr=0;
			RAM_Fill(Com_HT7017.MeasBuf_Rx, 5 ); 
			}
		*/
	}
/*	//����֡���ʱ�����ֽ���ǰ�����ֽ��ں󡣶��ڲ��� 3 ���ֽڵļĴ����� HT7017 ���ڲ��Ĵ���������֡�ĵ�λ���롣
	*Data = Com_HT7017.MeasBuf_Rx[2];
	*(Data + 1)= Com_HT7017.MeasBuf_Rx[1];
	*(Data + 2)= Com_HT7017.MeasBuf_Rx[0];
	Com_HT7017.MeasState &=~ ATT7053Read;
	*/
	return 0;
}
unsigned char UART_ReadInstan(  unsigned char Cmd )
{
	unsigned char Checksum,i;
	
	HT_FreeDog();
//	for(i=0;i<2;i++)
//	{
		//UCA2IE &=~ (UCRXIE + UCTXIE);
		HT_UART3->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[0] = 0x6A;
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[1] = Cmd;
		Com_HT7017.Cmd = Cmd;
		Checksum += Cmd;
		Com_HT7017.MeasBuf_Tx[5] = Checksum;
		Com_HT7017.MeasBuf_Tx[5] = ~Com_HT7017.MeasBuf_Tx[5];
		Com_HT7017.MeasState |= ATT7053Read;
		Com_HT7017.MeasState &=~ (ATT7053CommOK + ATT7053Set);
//		Delaysum = 2600;	//4M	2012.8.11
//		Delaysum = 3000;	//580
		Com_HT7017.Tx_Ptr = 1;
		Com_HT7017.Rx_Ptr = 0;
		HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
		HT_UART3->SBUF = Com_HT7017.MeasBuf_Tx[0];			

//	}
	return 0;
}

unsigned char MasureData_Get(void)
{
	unsigned char Checksum,i;
	unsigned char Error_Byte;
	unsigned char buff[4];
	unsigned char* Data_ptr;
	Data_ptr = buff;
	RAM_Fill(Data_ptr,4);
	Checksum = 0x6A;
	Checksum += Com_HT7017.Cmd;

	Checksum += (Com_HT7017.MeasBuf_Rx[0] + Com_HT7017.MeasBuf_Rx[1] + Com_HT7017.MeasBuf_Rx[2]);
	Checksum = ~Checksum;
	if(Checksum == Com_HT7017.MeasBuf_Rx[3])
		{
		*(Data_ptr + 3) = Com_HT7017.MeasBuf_Rx[2];
		*(Data_ptr + 2)= Com_HT7017.MeasBuf_Rx[1];
		*(Data_ptr + 1)= Com_HT7017.MeasBuf_Rx[0];
		Error_Byte = 0;
		for(i=0; i<Measur_Num; i++)
			{
			if(HT7017_DATATab[i].ReadCmd == Com_HT7017.Cmd) 
				break;
			}
		if(i==Measur_Num) 
			Error_Byte = 1;
		else
			{
			RAM_Write(HT7017_DATATab[i].ReadData,Data_ptr,4);
			}
		Com_HT7017.MeasState |= ATT7053CommOK;
		Com_HT7017.MeasState &=~ ATT7053Read;
		Com_HT7017.Rx_Ptr=0;
		RAM_Fill(Com_HT7017.MeasBuf_Rx,10);
		}
	else
		{
		Error_Byte = 1;
		}
	return Error_Byte;
}
#if 0
void ChkMeasChipRST(unsigned int phase)
{
	unsigned char *Point;
	
	Point = (unsigned char *)&MeasureHT7017.APowerPulP;	//2012.12.19
	
	if( ChkMeasChkSum(phase) == 1 )	
	{
		SoftResetMeas(phase);
		ATT7053Int(phase);
		RAM_Fill( Point+4*phase, 4);
	}
}

void CheckMeasChip(void)
{
	if((Flag.HT7017PowerOn&F_PhaATT7053) == F_PhaATT7053)
	{
	 	ATT7053Int(0); 
		MeasureHT7017.APowerPulP=0;   	 	
	}
	else
	{
		ChkMeasChipRST(0);
	}

/*
	if((Flag.ATT7053PowerOn&F_PhaATT7053) == F_PhaATT7053)
	{
	 	ATT7053Int(0); 
		Flag.APowerPulP=0;   	 	
	}
	else
	{
		ChkMeasChipRST(0);
	}
	
	if((Flag.ATT7053PowerOn&F_PhbATT7053) == F_PhbATT7053)
	{
	  	ATT7053Int(1); 
		Flag.BPowerPulP=0;      	  	
	}
	else
	{
		ChkMeasChipRST(1);		
	}

	if((Flag.ATT7053PowerOn&F_PhcATT7053) == F_PhcATT7053)
	{
		ATT7053Int(2);
		Flag.CPowerPulP=0;            		
	}
	else
	{
		ChkMeasChipRST(2);		
	}  
	*/
}

unsigned char ChkMeasChkSum(unsigned int phase)
{
	unsigned char Buff[4];
	unsigned char *Point;
	
	Point = Buff;
	/*
	switch(phase)
	{
		case 0:
	        PDIR_CSA |= P_CSA;
            PDIR_CSC &= ~P_CSC;
	        PDIR_CSB &= ~P_CSB;
			break;
		case 1:
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB |= P_CSB;
			PDIR_CSC &= ~P_CSC;				
			break;
		default:	
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB &= ~P_CSB;
			PDIR_CSC |= P_CSC;				
			break;	
	}	
	*/
	ATT7053RdReg(SUMChecksum,Point);	
	
	if(Data_Comp(Point, Para.MeasChkSum, 3)!=0)
	{
		E2P_RData( Para.MeasChkSum,MeasChkSum, 3 );
		if(Data_Comp(Point, Para.MeasChkSum, 3)!=0)
		{
			ATT7053RdReg(SUMChecksum,Point);
			if(Data_Comp(Point, Para.MeasChkSum, 3)!=0)
			{
			/*
				PDIR_CSA &= ~P_CSA;
				PDIR_CSC &= ~P_CSC;
				PDIR_CSB &= ~P_CSB;
				*/
				return 1;
			}
		}			
	}
/*	
	PDIR_CSA &= ~P_CSA;
	PDIR_CSC &= ~P_CSC;
	PDIR_CSB &= ~P_CSB;
	*/
	return 0;	
}
#endif
void SoftResetMeas(unsigned int phase)
{
	unsigned char Buff[3];
/*	
	switch(phase)
	{
		case 0:
	        PDIR_CSA |= P_CSA;
            PDIR_CSC &= ~P_CSC;
	        PDIR_CSB &= ~P_CSB;
			break;
		case 1:
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB |= P_CSB;
			PDIR_CSC &= ~P_CSC;				
			break;
		default:	
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB &= ~P_CSB;
			PDIR_CSC |= P_CSC;				
			break;	
	}	
*/
	Buff[0]=0x00;
	Buff[1]=0x55;
	UART_Write(SRSTREG_Address,Buff);	//�����λATT7053	
/*	
	PDIR_CSA &= ~P_CSA;
	PDIR_CSC &= ~P_CSC;
	PDIR_CSB &= ~P_CSB;	
	*/
	Delay(300);
}

short HT7017Init(void)
	{
		unsigned long Value;
		unsigned int  Temp;
		unsigned char datbuff[5];
		unsigned char* Point;
		
		Point = datbuff;
		HT_FreeDog();
	/*
			UART_ReadInstan(IDCode);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			MasureData_Get();
			RAM_Write(Point,(unsigned char *)&Measurement.HT7017_ID[1],3);

			if((datbuff[2]!= 0xB0)||(datbuff[1]!= 0x53)||(datbuff[0]!= 0x70))
			{		
			Flag.HT7017PowerOn |= F_PhaATT7053;
				return 1;			
			}
			else
			{
			Flag.HT7017PowerOn &= ~F_PhaATT7053;
			}	
			
			datbuff[0]=0x7E;
			datbuff[1]=0x7F;
			UART_Write(EMUIE_Address,Point);			
			Delay(Measur_Delay);
			Delay(Measur_Delay);
	//	//	WPCFG = 0xBC����ʾд�����򿪣�ֻ�ܲ���40H��45H��У������Ĵ��������ɲ���50H��71H��У������Ĵ���//
	//	//	WPCFG = 0xA6����ʾд�����򿪣�ֻ�ܲ���50H��71H��У������Ĵ��������ɲ���40H��45H��У������Ĵ�����//
			datbuff[0]=0x00;
			datbuff[1]=0xBC;
			UART_Write(WPREG_Address,Point);	//ʹ��40-45д����//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value = 0x0000BC;	//EMUCFG
			
		//	Value += 0x000088;	//FreqCFG
	
		//	Value += 0x00007E;	//ModuleEn
					
			//datbuff[0]=0x40;//�����Ƿ������Ĵ��������� 0��0�������� 0�� 1�������� 0��
			//datbuff[1]=0x12;//��������ģʽ���ƣ� 1=ʸ����,���������ۼ�ģʽѡ��1�������ۼӺ�ģʽ��
			datbuff[0]=0xBE;//�����Ƿ������Ĵ��������� 0��0�������� 0�� 1�������� 0��
			//datbuff[0]=0x9E;
			datbuff[1]=0x12;
			UART_Write(EMUCFG_Address,Point);	//EMU ���üĴ���
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x004012;		
		//	datbuff[0]=0x00;
		//	datbuff[1]=0xC8;
			datbuff[0]=0x01;
			datbuff[1]=0x48;
			UART_Write(FreqCFG_Address,Point);	//ʱ��/����Ƶ�����üĴ���
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x000148;
			
			datbuff[0]=0x00;
			datbuff[1]=0x7E;
			UART_Write(ModuleEn_Address,Point);	//EMU ģ��ʹ�ܼĴ���
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x00007E;

			datbuff[0]=0x00;
			datbuff[1]=0x0F;
			UART_Write(ANAEN,Point);	//ADC���ؼĴ���//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x000007;	//ADC���ؼĴ���

			Value += 0x0000A6;	//IOCFG
					
			datbuff[0]=0x00;
			datbuff[1]=0xA6;
			UART_Write(WPREG_Address,Point);	//ʹ��50-7Cд����//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x0000A6;	

	//		datbuff[0]=0xFE;
	//		datbuff[1]=0x98;
	//		UART_Write(QPhsCal_Address,Point);	//�� femu=1MHz ʱ�� 60Hz �ź�Ƶ���´˼Ĵ���д�� FE98H ���ɣ�Ҳ����Ҫ����У׼
	//		Delay(Measur_Delay);

			E2P_RAdj( datbuff, I2Gain, 2 );//����ͨ�� 2 ��������
			SwapData(Point,2);
			UART_Write(I2Gain_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			
			E2P_RAdj( datbuff, HFCONST, 2 );//��Ƶ���峣������
			SwapData(Point,2);
			UART_Write(HFConst_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			
	//		ATT7053RdReg(WPREG,Point);	//ʹ��50-71д����//
	//-----------��ѹ������Чֵƫ��У��----------
			E2P_RAdj( datbuff, I1RMSOFFSET, 2 );
			UART_Write(I1RMSOFFSET_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			E2P_RAdj( datbuff, I2RMSOFFSET, 2 );
			UART_Write(I2RMSOFFSET_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			E2P_RAdj( datbuff, URMSOFFSET, 2 );
			UART_Write(URMSOFFSET_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			
			E2P_RAdj( datbuff, AGPBase, 2 );
			SwapData(Point,2);
	//		datbuff[0]=0x13;
	//		datbuff[1]=0x20;
			UART_Write(GP1,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
	//		ATT7053RdReg(GP1,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			UART_Write(GQ1,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(GS1,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
	//		ATT7053WtReg(GP2,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Temp = (*Point*256 + *(Point+1));
			
			Value += Temp;	//GP1		
			Value += Temp;	//GQ1
			Value += Temp;	//GS1

			E2P_RAdj( datbuff, AGPBase2, 2 );
			SwapData(Point,2);
			UART_Write(GP2_Address,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(GQ2_Address,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(GS2_Address,Point);	//eeprom 3�ֽڣ�ʵ��ʹ��2�ֽ�//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = (*Point*256 + *(Point+1));
			
			Value += Temp;	//GP1		
			Value += Temp;	//GQ1
			Value += Temp;	//GS1
			
			Value += 0x00FF00;	//QPhsCal

			//��Ҫ�����ļĴ���//
			datbuff[0]=0x00;
			datbuff[1]=0x00;
			UART_Write(ADCCON,Point); //����1�Ŵ�16������ѹ�Ŵ�1��,����2�Ŵ�4��//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x00003C;	//ADCCON

			Value += 0x000040 + 0x000040;	//PQStart,HFConst
	
			Value += 0x000010 + 0x000020 + 0x000004;	//CHk,IPTAMP,ZCrossCurrent
	
			E2P_RAdj( Point, E2AP1OFFSET, 2 );		//2012.12.19
			UART_Write(P1OFFSET,Point+1);	//P1OFFSET
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(P1OFFSETH_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = *(Point+1);
			Value += Temp;
			Value += *Point;
			
			E2P_RAdj( Point, E2AP2OFFSET, 2 );		//2012.12.19
			UART_Write(P2OFFSET,Point+1);	//P1OFFSET
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(P2OFFSETH_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = *(Point+1);
			Value += Temp;
			Value += *Point;
					
			E2P_RAdj( datbuff, AGPhs, 2 );
			SwapData(Point,2);
			UART_Write(GPhs1,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = (*Point*256 + *(Point+1));
			Value += Temp;	
			
			E2P_RAdj( datbuff, AGPhs2, 2 );
			SwapData(Point,2);
			UART_Write(GPhs2,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = (*Point*256 + *(Point+1));
			Value += Temp;	
				
	//		readep(&rambuf[1],EE_PHSA_7053,3,eplxjbcs);
	//		rambuf[0]=0;
	//		jchkprg(&rambuf[1]);
	//		wr7053(rambuf,0x6d+0X80,3); //��λУ׼
				
			datbuff[0]=0x00;
			datbuff[1]=0x00;
			datbuff[2]=0x00;
			UART_Write(WPREG_Address,Point);	//�ر�ʹ��			
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			RAM_Write( Para.MeasCheckSum, (unsigned char*)&Value, 3 ); 		//ATChk 
			E2P_WData( MeasChkSum, (unsigned char *)&Value, 3 );
			
	//	}
*/	
	//	ATT7053RdReg(ADCCON,Point);
	//	ATT7053RdReg(GP1,Point);
	//	ATT7053RdReg(Rms_U,Point);
		/*
		PDIR_CSA &= ~P_CSA;
		PDIR_CSC &= ~P_CSC;
		PDIR_CSB &= ~P_CSB;
		*/
		Flag.HT7017PowerOn &=~ F_Reset7053;	
		return 0;
	}
short Read_ATTValue( unsigned char cmd, unsigned char* Point , unsigned char length )
{
	unsigned char buff[4];
	unsigned char* ptr;
	short return_value=0;
	ptr = buff;
	RAM_Fill(ptr,4);
	if(UART_Read(cmd,ptr)==0)
		{
			SwapData(ptr,4);
			RAM_Write(Point,ptr,4);
			return_value=0;
		}
	else
		return_value=-1;
	return return_value;
}

void ReadHT7017_V(void)
{
	unsigned char Buff[4];
	unsigned char *Point;
	unsigned long Temp;
	unsigned long long Temp2;
	Point = Buff;
/*	
	Temp = GetBuffToLong(Measurement.HT7017_U);
	if(MeasureHT7017.Urms_K!=0)
		Temp2 = MeasureHT7017.Urms_K;
	else
		Temp2 = 1;
	Temp2 *= Temp;
	Temp2 /= 100000;
	Temp2 /= 1000;//������λС��
	Real.Uab = (unsigned short)Temp2;
	Word_BCD(Para.Ua,Real.Uab);
*/	
}
void ReadHT7017_HZ(void)
{
	unsigned char Buff[4];
	unsigned char *Point;
	unsigned long Temp2;
	Point = Buff;
	
	Temp2 = GetBuffToLong(Measurement.HT7017_Hz);
	Temp2 = 1000000/Temp2;//EMU ʱ�ӣ�femu��ѡ��Ϊ 1MHz 
	Temp2/=2;

	Real.Freq = (unsigned short)Temp2;//hex
	Real.Freq*=100;//2λС��
	Word_BCD(Point,Real.Freq);
	
	RAM_Write( Para.Fre, Point, 2 );//bcd
}

void ReadHT7017_I(void)
{
	unsigned long Temp;	
	unsigned long long Temp2;
	/* 
	Temp = GetBuffToLong(Measurement.HT7017_Ia);
	if(MeasureHT7017.Irms1_K!=0)
		Temp2 = MeasureHT7017.Irms1_K;
	else
		Temp2 = 1;
	Temp2 *= Temp;
	Temp2 /= 100000;
	Temp2 /= 1000;//������λС��
	Real.Ia = (unsigned short)Temp2;
	Word_BCD((unsigned char*)&Para.Ia,Real.Ia);

	
	Temp = GetBuffToLong(Measurement.HT7017_Ib);	
	if(MeasureHT7017.Irms2_K!=0)
		Temp2 = MeasureHT7017.Irms2_K;
	else
		Temp2 = 1;
	Temp2 *= Temp;
	Temp2 /= 100000;
	Temp2 /= 1000;//������λС��
	Real.Ib = (unsigned short)Temp2;
	*/
}

void ReadHT7017_P(void)
{
	unsigned char Databuff[4];
	unsigned char* Point;
	int State;
//	unsigned long int Power; 
	long Power1,Power2,Power,angle,power_factor; 
	long Temp;
	long long temp2,temp3;
	double anglezA;
	Power1 = 0;
	Power2 = 0;
	Point = Databuff;
#if 0
	if(MeasureHT7017.Power1_K==0)
		MeasureHT7017.Power1_K=1;
	if(MeasureHT7017.Power2_K==0)
		MeasureHT7017.Power2_K=1;

		Temp = GetBuffToLong(Measurement.HT7017_P1);
		if(Temp<0x800000)
			Power1 = Temp;
		else
			{
			Power1 = -(0x1000000-Temp);
			}
		MeasureHT7017.APowerPulP= Power1;
		temp2 = MeasureHT7017.Power1_K;
		temp2 *= Power1;
	Power = temp2;
	//-----��һͨ���Ƕ�--------	
		temp2 = GetBuffToLong(Measurement.HT7017_U);
		temp3 = MeasureHT7017.APowerPulP;
		anglezA = (double)((temp3*100)/(temp2*GetBuffToLong(Measurement.HT7017_Ia)/100000));
		anglezA/=1000;
		if(anglezA > 100000) anglezA = 100000;
		Real.Angle_Ia = (unsigned short)(acos(anglezA)*1800/3.1416)*10;
		
/*	temp2 = Real.Uab;
	temp3 = Power;
	anglezA = (double)((temp3*100)/(temp2*Real.Ia));
	anglezA/=1000;
	if(anglezA > 100000) anglezA = 100000;
	Real.Angle_Ia = (unsigned short)(acos(anglezA)*1800/3.1416)*10;
*/
		Temp = GetBuffToLong(Measurement.HT7017_P2);
		if(Temp<0x800000)
			Power2 = Temp;
		else
			{
			Power2 = -(0x1000000-Temp);
			}
		MeasureHT7017.BPowerPulP= Power2;
		temp2 = MeasureHT7017.Power2_K;
		temp2 *= Power2;
		Power+=temp2;
		Power = Power/100000;
		Real.Pa = Power*100;//wλС��
		if(Real.Pa<0)
			Real.Pa = -Real.Pa;
		//-----�ڶ�ͨ���Ƕ�--------
		
		temp2 = GetBuffToLong(Measurement.HT7017_U);
		temp3 = MeasureHT7017.BPowerPulP;
		anglezA = (double)((temp3*100)/(temp2*GetBuffToLong(Measurement.HT7017_Ib)/100000));
		anglezA/=1000;
		if(anglezA > 100000) anglezA = 100000;
		Real.Angle_Ib= (unsigned short)(acos(anglezA)*1800/3.1416)*10;
/*		temp3 = temp2;
		temp2 = Real.Uab;
		anglezA = (double)((temp3*100)/(temp2*Real.Ib));
		anglezA/=1000;
		if(anglezA > 100000) anglezA = 100000;
		Real.Angle_Ib= (unsigned short)(acos(anglezA)*1800/3.1416)*10;
*/
		Temp = GetBuffToLong(Measurement.HT7017_S1);
		if(Temp<0x800000)
			Power1 = Temp;
		else
			{
			Power1 = -(0x1000000-Temp);
			}
		MeasureHT7017.SPowerPulP = Power1;
		if(MeasureHT7017.PowerS_K!=0)
			temp2 =MeasureHT7017.PowerS_K;
		else
			temp2 = 1;
		temp2 *= Power1;
		temp2/=100000;
		Power1 = temp2;
		Real.St = Power1*100;;

		Temp = GetBuffToLong(Measurement.HT7017_Q1);
		if(Temp<0x800000)
			Power1 = Temp;
		else
			{
			Power1 = -(0x1000000-Temp);
			}
		MeasureHT7017.APowerPulQ= Power1;	
		temp2 = MeasureHT7017.Power2_K;
		temp2 *= Power1;

		Temp = GetBuffToLong(Measurement.HT7017_Q2);
		if(Temp<0x800000)
			Power2 = Temp;
		else
			{
			Power2 = -(0x1000000-Temp);
			}
		MeasureHT7017.BPowerPulQ= Power2;
		temp2 = MeasureHT7017.Power2_K;
		temp2 *= Power2;
	
	power_factor = ((MeasureHT7017.APowerPulP+MeasureHT7017.BPowerPulP)*100)/MeasureHT7017.SPowerPulP;
	//power_factor*=10;
	if(power_factor<0)
		power_factor = -power_factor;
	Real.Pfa = (unsigned char)power_factor;
	Word_BCD((unsigned char *)& Para.Pt,(unsigned short)power_factor);
#endif
}
void Instan_Value(void)
{
	ReadHT7017_V();
	ReadHT7017_I();
	ReadHT7017_HZ();
	ReadHT7017_P();

}

void Energy_Read(void)
{
	unsigned long Temp;

		Temp =	GetBuffToLong(Measurement.HT7017_P);
		ECInt.YPulse_Cum = (unsigned char)Temp;
		ECInt.YPulse_CumChk += ECInt.YPulse_Cum ;
	
		Temp = GetBuffToLong(Measurement.HT7017_Q);
		ECInt.WPulse_Cum = (unsigned char)Temp;
		ECInt.WPulse_CumChk += ECInt.WPulse_Cum ;	
		
		RAM_Fill(Measurement.HT7017_P,4);
		RAM_Fill(Measurement.HT7017_Q,4);
		if(Measurement.HT7017_EMU_Status[3]&0x02)
			Real.PWFlag |= 0x10;//�й�����
		else
			Real.PWFlag &=~ 0x10;
		if(Measurement.HT7017_EMU_Status[3]&0x01)
			Real.PWFlag |= 0x01;//�޹�����
		else
			Real.PWFlag &=~ 0x01;
	
}

#endif


void GetSPIPara( SPIPARA* SPIPara, unsigned short Devads )	//��ȡ��SPI�ӿ�оƬ��CPU�ϵ�IO������
{
	switch( Devads )
	{
		case 0:		
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOA->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOA->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOA->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOA->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOA->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOA->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOA_EMU_CS1;
			SPIPara->AD_RST = GPIOA_EMU_RST1;
			break;
		case 1:		
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOD->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOD->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOD->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOD->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOD->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOD->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOD_EMU_CS2;
			SPIPara->AD_RST = GPIOD_EMU_RST2;
			break;
		case 2:		
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOD->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOD->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOD->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOD->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOD->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOD->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOD_EMU_CS3;
			SPIPara->AD_RST = GPIOD_EMU_RST3;
			break;
		case 3:		
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOG->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOG->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOG->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOD->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOD->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOD->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOG_EMU_CS4;
			SPIPara->AD_RST = GPIOD_EMU_RST4;
			break;
		case 4:		
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOA->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOA->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOA->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOD->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOD->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOD->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOA_EMU_CS5;
			SPIPara->AD_RST = GPIOD_EMU_RST5;
			break;
		case 5:		
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOH->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOH->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOH->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOA->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOA->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOA->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOH_EMU_CS6;
			SPIPara->AD_RST = GPIOA_EMU_RST6;
			break;
		case 6:	
                        HT_GPIOB->IOCFG &= (~GPIOB_EMU_CS7);
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOB->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOB->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOB->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOA->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOA->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOA->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOB_EMU_CS7;
			SPIPara->AD_RST = GPIOA_EMU_RST7;
			break;						
		default: 
                        HT_GPIOB->IOCFG &= (~GPIOB_EMU_RST8);
			SPIPara->AD_CS_PTDIR = (unsigned long*)&HT_GPIOH->PTDIR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTSET = (unsigned long*)&HT_GPIOH->PTSET;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_CS_PTCLR = (unsigned long*)&HT_GPIOH->PTCLR;//(unsigned char*)&PDIR_SDA;
			SPIPara->AD_RST_PTDIR = (unsigned long*)&HT_GPIOB->PTDIR;//(unsigned char*)&PDIR_SCL;
			SPIPara->AD_RST_PTSET = (unsigned long*)&HT_GPIOB->PTSET;//(unsigned char*)&PDIR_SCL;	
			SPIPara->AD_RST_PTCLR = (unsigned long*)&HT_GPIOB->PTCLR;//(unsigned char*)&PDIR_SCL;			
			SPIPara->AD_CS = GPIOH_EMU_CS8;
			SPIPara->AD_RST = GPIOB_EMU_RST8;
			break;				
	}
}

#if( AllFail7022Chk == YesCheck )							//10.11.11
void WriteATByte( unsigned char Cmd )
{
	unsigned short i;
	
	for(i=0;i<8;i++ )
	{
		Delay(DELEY2);	
//		PDIR_ADSCLK &= ~P_ADSCLK;
		HT_GPIOG->PTSET |= GPIOG_EMU_CLK;//POUT_ADSCLK |= P_ADSCLK;
		Delay(DELEY4);	
//		if(( Cmd & ByteBit[7-i] ) != 0 ) PDIR_ADDIN &= ~P_ADDIN;
//		else PDIR_ADDIN |= P_ADDIN;
		if(( Cmd & ByteBit[7-i] ) != 0 ) HT_GPIOG->PTSET |= GPIOG_EMU_DIN;//POUT_ADDIN |= P_ADDIN;
		else HT_GPIOG->PTCLR |= GPIOG_EMU_DIN; //POUT_ADDIN &= ~P_ADDIN;
		Delay(DELEY2);								//11.08.03
//		PDIR_ADSCLK |= P_ADSCLK;
		HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;//POUT_ADSCLK &= ~P_ADSCLK;
		Delay(DELEY2);	
	}	
}

//**********************************************************************
// SPI READ
//**********************************************************************
void ATT7022Rd(unsigned char Cmd, unsigned char* Data, short Length ,unsigned short Devads)
{
	unsigned short i,j;
	unsigned char Temp;
	
	unsigned char Buff1[18];
	SPIPARA* SPIPara;	
	SPIPara = (SPIPARA*)Buff1;
	GetSPIPara( SPIPara, Devads );
	
//	PDIR_ADCS &= ~P_ADCS;
	*SPIPara->AD_CS_PTSET |= SPIPara->AD_CS;//HT_GPIOC->PTSET |= GPIOC_EMU_CS;//POUT_ADCS |= P_ADCS;
	Delay(DELEY2);	
//	PDIR_ADSCLK |= P_ADSCLK;
	HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;//POUT_ADSCLK &= ~P_ADSCLK;
	Delay(DELEY2);	
//	PDIR_ADCS |= P_ADCS;
	*SPIPara->AD_CS_PTCLR &= ~SPIPara->AD_CS;//HT_GPIOC->PTCLR |= GPIOC_EMU_CS;//POUT_ADCS &= ~P_ADCS;
	Delay(DELEY5);
	WriteATByte( Cmd );
	Delay(DELEY5);
	
	for( i=0;i<Length;i++ )
	{
		Temp = 0;
		for( j=0;j<8;j++ )
		{
			Delay(DELEY2);	
//			PDIR_ADSCLK &= ~P_ADSCLK;
			HT_GPIOG->PTSET |= GPIOG_EMU_CLK;//POUT_ADSCLK |= P_ADSCLK;
			Delay(DELEY4);	
			if(( HT_GPIOG->PTDAT & GPIOG_EMU_DOUT ) != 0 ) Temp |= ByteBit[7-j];//if(( PIN_MDOUT & P_MDOUT ) != 0 ) Temp |= ByteBit[7-j];
//			PDIR_ADSCLK |= P_ADSCLK;
			HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;//POUT_ADSCLK &= ~P_ADSCLK;
			Delay(DELEY2);	
		}	
		*(Data+Length-1-i) = Temp; 		
	}	
//	PDIR_ADCS &= ~P_ADCS;
	*SPIPara->AD_CS_PTSET |= SPIPara->AD_CS;//HT_GPIOC->PTSET |= GPIOC_EMU_CS;//POUT_ADCS |= P_ADCS;
	Delay(DELEY2);	
//	PDIR_ADSCLK &= ~P_ADSCLK;
	HT_GPIOG->PTSET |= GPIOG_EMU_CLK;//POUT_ADSCLK |= P_ADSCLK;
	Delay(DELEY2);	
//    PDIR_ADDIN &= ~P_ADDIN;
    	HT_GPIOG->PTSET |= GPIOG_EMU_DIN;//POUT_ADDIN |= P_ADDIN;
	Delay(DELEY2);								//11.08.03
}

void ATT7022WR(unsigned char Cmd, unsigned char* Data, short Length,unsigned short Devads )
{
	unsigned short i;
	
	unsigned char Buff1[18];
	SPIPARA* SPIPara;	
	SPIPara = (SPIPARA*)Buff1;
	GetSPIPara( SPIPara, Devads );
	
//	PDIR_ADCS &= ~P_ADCS;
	*SPIPara->AD_CS_PTSET |= SPIPara->AD_CS;//HT_GPIOC->PTSET |= GPIOC_EMU_CS;//POUT_ADCS |= P_ADCS;
	Delay(DELEY2);	
//	PDIR_ADSCLK |= P_ADSCLK;
	HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;//POUT_ADSCLK &= ~P_ADSCLK;
	Delay(DELEY2);	
//	PDIR_ADCS |= P_ADCS;
	*SPIPara->AD_CS_PTCLR &= ~SPIPara->AD_CS;//HT_GPIOC->PTCLR |= GPIOC_EMU_CS;//POUT_ADCS &= ~P_ADCS;
	Delay(DELEY5);
	WriteATByte( Cmd );
	Delay(DELEY5);
	
	for( i=0;i<Length;i++ )
	{
		WriteATByte(*(Data+Length-1-i) );
	}	
//	PDIR_ADCS &= ~P_ADCS;
	*SPIPara->AD_CS_PTSET |= SPIPara->AD_CS;//HT_GPIOC->PTSET |= GPIOC_EMU_CS;//POUT_ADCS |= P_ADCS;
	Delay(DELEY2);	
//	PDIR_ADSCLK &= ~P_ADSCLK;
	HT_GPIOG->PTSET |= GPIOG_EMU_CLK;//POUT_ADSCLK |= P_ADSCLK;
	Delay(DELEY2);	
//	PDIR_ADDIN &= ~P_ADDIN;
	HT_GPIOG->PTSET |= GPIOG_EMU_DIN;//POUT_ADDIN |= P_ADDIN;
	Delay(DELEY2);								//11.08.03
}
#else
void WriteATByte( unsigned char Cmd )
{
  unsigned short i;
  for(i=0;i<8;i++ )
  {
    Delay(DELEY2);	
    HT_GPIOG->PTSET |= GPIOG_EMU_CLK;
    Delay(DELEY2);	
    if(( Cmd & ByteBit[7-i] ) != 0 ) 
      HT_GPIOG->PTSET |= GPIOG_EMU_DIN;
    else 
      HT_GPIOG->PTCLR |= GPIOG_EMU_DIN;
    HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;	
  }	
}

//**********************************************************************
// SPI READ
//**********************************************************************
void ATT7022Rd(unsigned char Cmd, unsigned char* Data, short Length,unsigned short Devads )
{
  unsigned short i,j;
  unsigned char Temp;
  unsigned char Buff1[64];
  SPIPARA* SPIPara;	
  SPIPara = (SPIPARA*)Buff1;
  GetSPIPara( SPIPara, Devads );
  HT_GPIOG->PTDIR   &= (~GPIOG_EMU_DOUT);
  HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;
  HT_GPIOG->PTDIR |= GPIOG_EMU_CLK;
  *SPIPara->AD_CS_PTCLR = SPIPara->AD_CS;
  *SPIPara->AD_CS_PTDIR |= SPIPara->AD_CS;
  Delay(DELEY5);
  WriteATByte( Cmd );
  Delay(DELEY5);
  for( i=0;i<Length;i++ )
  {
    Temp = 0;
    for( j=0;j<8;j++ )
    {
      Delay(DELEY2);
      HT_GPIOG->PTSET |= GPIOG_EMU_CLK;
      Delay(DELEY2);	
      if(( HT_GPIOG->PTDAT &GPIOG_EMU_DOUT ) != 0 ) 
        Temp |= ByteBit[7-j];
      HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;	
    }
    *(Data+Length-1-i) = Temp; 		
  }
  *SPIPara->AD_CS_PTSET = SPIPara->AD_CS;
}

void ATT7022WR(unsigned char Cmd, unsigned char* Data, short Length,unsigned short Devads )
{
  unsigned short i;
  unsigned char Buff1[64];
  SPIPARA* SPIPara;	
  SPIPara = (SPIPARA*)Buff1;
  GetSPIPara( SPIPara, Devads );	
  HT_GPIOG->PTCLR |= GPIOG_EMU_CLK;
  HT_GPIOG->PTDIR |= GPIOG_EMU_DIN;
  *SPIPara->AD_CS_PTDIR |= SPIPara->AD_CS;
  *SPIPara->AD_CS_PTCLR |= SPIPara->AD_CS;
  WriteATByte( Cmd );
  Delay(DELEY5);
  for( i=0;i<Length;i++ )
  {
    WriteATByte(*(Data+Length-1-i) );
  }
  *SPIPara->AD_CS_PTSET |= SPIPara->AD_CS;
  HT_GPIOG->PTSET |= GPIOG_EMU_CLK;
}



void ATT7022WtReg(  unsigned char Cmd, unsigned char* Data ,unsigned short Devads)
{
  unsigned char Buff[3];
  unsigned char* Point;
  short i;
  
  Point = Buff;
  HT_FreeDog();
#if ( MEASCHIP == ATT7022E )									//11.09.24
  *(Data+2) = 0;
#endif
  for( i=0;i<3;i++)
  {
    ATT7022WR( Cmd, Data, 3,Devads );
    ATT7022Rd( ATSPIWBuffA, Point, 3,Devads );
    if( Data_Comp(Data, Point, 3) == 0 ) break;
  }
}

short ATT7022RdReg(  unsigned char Cmd, unsigned char* Data,unsigned short Devads )				//11.08.03
{
  unsigned char Buff[3];        
  unsigned char* Point;
  short i;
  Point = Buff;
  HT_FreeDog();
  for( i=0;i<3;i++)	
  {
    ATT7022Rd( Cmd, Data, 3,Devads );
    ATT7022Rd( ATSPIRBuff, Point, 3,Devads );
    if( Data_Comp(Data, Point, 3) == 0 ) 
    {
      if(( IsAllData( Data, 3, 0x00 ) == 0 )||( IsAllData( Data, 3, 0xff ) == 0 )) 
        SM.MeasChipErrCnt++;
      else 
        SM.MeasChipErrCnt = 0;
      break;
    }
    else if( i>= 2 ) 
    {
      RAM_Fill( Data, 3 ); 
      SM.MeasChipErrCnt++;
      if( SM.MeasChipErrCnt > 80 ) SM.MeasChipErrCnt = 80;
      return -1;													//11.08.03		
    }
  }
  if( SM.MeasChipErrCnt > 80 ) SM.MeasChipErrCnt = 80;
  return 0;															//11.08.03	
}
#endif
void Read_CMonEC( unsigned char* Dest, unsigned short ECRgAds )
	{
		unsigned short i;
		unsigned short ECEAds;
		unsigned short ECFAds;
		unsigned char* ECRAds;
		
	//	if( CombECJust( Dest, ECRgAds ) == 0 ) return;	//��ϵ����ж� 
		
		for( i=0;i<ECUnitNum;i++ )
		{
			ECEAds = ECRgTab[i].ECEAds; 
			//ECRAds = ECRgTab[i].ECRAds; 
			ECFAds = ECRgTab[i].ECFAds;
	
			if( ECEAds == ECRgAds ) break;				//�ܵ���
			else if(( i < (ECUnitNum-1) )&&( ECEAds < ECRgAds )&&( ECRgTab[i+1].ECEAds > ECRgAds )) //���ʵ���
			{
				if(((( ECRgAds - ECEAds ) % (ECDIGIT+1) ) == 0 )&&( ECRgTab[i].ECRgMode == 0 )) //�з��ʵ����ҷ��ʵ�����ַ��ȷ
				{
					ECRAds += ECRgAds - ECEAds; 	//���ʵ���RAM��ַ
					ECFAds += ECRgAds - ECEAds; 	//���ʵ��������ַ
					break;
				}	
			}			
		}	
		if( i == ECUnitNum ) 
		{
			RAM_Fill( Dest, ECDIGIT );				//������ַ����
			return; 		
		}	
		GetRamEC( Dest, ECRAds, ECFAds );			//��ȡRAM��FRAM�еĵ���
	
	}

void Read_CMonEC4( unsigned char* Dest, unsigned short ECRgAds )
{
	unsigned char Buff[6];
	
	Read_CMonEC( Buff, ECRgAds );	
	RAM_Write( Dest, Buff+1, 4 );
	
}	

void Read_CMonEC4_HEX_U( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	unsigned long Value;
	
	Read_CMonEC4( Dest, ECRgAds );
	Value = BCD4_Long( Dest );
	RAM_Write( Dest, (unsigned char*)&Value, 4 );
}	

void Read_CMonEC4_HEX_S( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	long Value;
	short Temp=0;
	
	Read_CMonEC4( Dest, ECRgAds );
	if(( *(Dest+3) & 0x80 ) != 0 ) 
	{
		*(Dest+3) &= ~0x80;
		Temp = 1;
	}	
	Value = BCD4_Long( Dest );
	if( Temp == 1 ) Value = 0 - Value;
	RAM_Write( Dest, (unsigned char*)&Value, 4 );
}	

void Read_CMonEC8_HEX_U( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	unsigned long long LValue;
	
	Disk.DigitsValidFlg = 0x55;										//���0.01kWh����β��		
	Read_CMonEC( Dest, ECRgAds );
	Disk.DigitsValidFlg = 0x00;
	LValue = *(Dest+4);
	LValue <<= 32;
	LValue += BCD4_Long( Dest );
	RAM_Write( Dest, (unsigned char*)&LValue, 8 );
}	

void Read_CMonEC8_HEX_S( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	long long LValue;
	short Temp=0;
	
	Disk.DigitsValidFlg = 0x55;										//���0.01kWh����β��		
	Read_CMonEC( Dest, ECRgAds );
	Disk.DigitsValidFlg = 0x00;
	if(( *(Dest+4) & 0x80 ) != 0 ) 
	{
		*(Dest+4) &= ~0x80;
		Temp = 1;
	}	
	LValue = *(Dest+4);
	LValue <<= 32; 
	LValue += BCD4_Long( Dest );
	if( Temp == 1 ) LValue = 0 - LValue;
	RAM_Write( Dest, (unsigned char*)&LValue, 8 );
}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
void FeeECPulseAdd( unsigned short* PL_Cum, unsigned char* PL_Chk, unsigned char Pulse_Cum )
{
	unsigned short* PL_CumPtr;
	unsigned char* PL_ChkPtr;
	unsigned short Temp;
	
	if(( SM.FeeNo < 1 )||( SM.FeeNo > Total_Fee )) return;				//��ǰ���ʺŷǷ�

	Temp = SM.FeeNo;
	Temp -= 1; 
	PL_CumPtr = PL_Cum;
	PL_CumPtr += Temp;
	PL_ChkPtr = PL_Chk;
	PL_ChkPtr += Temp;
//�¹���		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//	*PL_CumPtr += (unsigned short)Pulse_Cum * 10;	
//#else
	*PL_CumPtr += Pulse_Cum;	
//#endif
	*PL_ChkPtr = ChkNum( (unsigned char*)PL_CumPtr, 2 );
}
#endif

short GetMQuad( unsigned char PWFlag, short Reg )
{
    short Quad=0;
        
//	PWFlag = PWFlag >> Reg;	
	PWFlag &= 0x3;
	switch( PWFlag )
	{
		case 0: Quad = 0; break;	
		case 0x1: Quad = 1; break;	
		case 0x2: Quad = 3; break;	
		case 0x3: Quad = 2; break;	
		default: break;
	}	
	return 	Quad;
}

#if 0
short RAMECIsOK( unsigned char* ECRAds )
{
	if(( ChkNum( ECRAds, ECDIGIT ) == *(ECRAds+ECDIGIT) )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 )) return 0;
	else return 1;
}	

short E2PECIsOK( unsigned char* ECRAds )
{
	if((( Flag.BatState & F_E2PCheck ) == 0 )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 )) return 0;
	else return 1;
}
//ȡ��ǰ���������ַ��ECRgTab���ڶ�Ӧ���к�
short GetECRgTabRowNo( unsigned short ECFAds )		
{
	short i;
	unsigned short StartNo,EndNo;
	
	for( i=0;i<ECUnitNum;i++ )
	{
		StartNo = ECRgTab[i].ECFAds;
		EndNo = StartNo + 5*(ECDIGIT+1);
		if( ECRgTab[i].ECRgMode == 0 )			//������������
		{
			if(( ECFAds >= StartNo )&&( ECFAds < EndNo )&&((( ECFAds-StartNo)%(ECDIGIT+1)) == 0 )) break;			
		}
		else
		{
			if( StartNo == ECFAds ) break;		//����������
		}		
	}	
	if( i == ECUnitNum ) return -1;
	else return i;
}
void ECBCDAdd( unsigned char* Dest, unsigned char* Source )
{
#if ( ECDIGIT == BYTE5 )
	_BCD5ADD( Dest, Source );	
#else
	_BCD4ADD( Dest, Source );	
#endif
}	

void ECBCDSub( unsigned char* Dest, unsigned char* Source )
{
#if ( ECDIGIT == BYTE5 )
	_BCD5SUB( Dest, Source );	
#else
	_BCD4SUB( Dest, Source );	
#endif
}

//���RAM�еķ�ʱ����֮���Ƿ������
//Source: ��ŵ�����RAM��
//Type: 0��RAM��������ÿ��������У��ͣ� 1��ÿ���������һ��У��͡�
short TotalFeeECCheck( unsigned char* Source, short Type )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	unsigned short Len;
	short i;
#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
	unsigned long Value1;
	unsigned long Value2;
#endif
	
	Ptr = Buff;
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Fill( Ptr, ECDIGIT );
	for( i=0;i<4;i++ )
	{
		ECBCDAdd( Ptr, Source+(i+1)*Len );
	}	
#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
#if ( ECDIGIT == BYTE5 )
	Value1 = BCD4_Long( Source+1 );
	Value2 = BCD4_Long( Ptr+1 );
#else
	Value1 = BCD4_Long( Source );
	Value2 = BCD4_Long( Ptr );
#endif
	if(( Value1 >= Value2 )&&(( Value1 - Value2 ) <= 4 )) return 0;		//�ܵ��ڷ�ʱ����֮�� 
	else return 1;
#else
	if( Data_Comp( Ptr, Source, ECDIGIT ) != 0 ) return 1;		//�ܲ����ڷ�ʱ����֮�� 
	else return 0;
#endif
}

//����EEPROM�б����ֵ�������ݻָ�
void ResumeEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds )
{
	short i;
	short ECRgTabRowNo; 
	unsigned char Buff[30];
	unsigned char* Ptr;
	unsigned char Buff2[10];
	unsigned char* Point;
	unsigned short Addr;
	unsigned char State=0;
	unsigned char* TempPtr;
	
	Ptr = Buff;
	Point = Buff2;
	
	ECRgTabRowNo = GetECRgTabRowNo( ECFAds );
	if( ECRgTabRowNo == -1 ) return;									//����FRAM������ַ
                                                                		
	Addr = ECRgTab[ECRgTabRowNo].ECEAds;								//ȡ��Ӧ��EEPROM�е����ĵ�ַ
	if( ECRgTab[ECRgTabRowNo].ECRgMode == 0 )							//������������
	{                                                           		
		TempPtr = Ptr;                                          		
		for( i=0;i<5;i++ )												//��ȡEEPROM�е��ܼ������ʵ���	
		{                                                       		
			E2P_RData( TempPtr, Addr, ECDIGIT );	            		
			if( E2PECIsOK( TempPtr ) != 0 )                     		
			{                                                   		
				State = 1;												//EEPROM�����д�	
				break;                                          		
			}	                                                		
			TempPtr += ECDIGIT;                                 		
			Addr += ECDIGIT+1;                                  		
		}	                                                    		
		if( State == 0 )                                        		
		{                                                       		
			if( TotalFeeECCheck( Ptr, 0 ) != 0 ) State = 1;				//�ܲ����ڷ�ʱ����֮�� 
		}	
		E2P_RFM( Point, ECFAds, ECDIGIT );								//�ٴζ�ȡFRAM���ݡ�	
		if( E2PECIsOK( Point ) == 0 ) 
		{
			RAM_Write( Dest, Point, ECDIGIT );							//�����е�����ȡ��ȷ�����������Ϊ׼��������RRPROM��ȡ�õ�����
			RAM_Write( ECRAds, Dest, ECDIGIT );							//RAM�е�ֵ�ָ�Ϊ��ȷ���������ݻ򱸷�EEPROM����
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
		}	
		else
		{																//�������ݻ�������	
			if( State == 0 ) 											//EEPROM������ȷ������У��͡�BCD�롢�ܵ��ڷ�ʱ����֮��
			{
				TempPtr = ECRgTab[ECRgTabRowNo].ECRAds;
				Addr = ECRgTab[ECRgTabRowNo].ECFAds;					//ȡ�����е�����ַ
				RAM_Write( Dest, Ptr+(((ECFAds-Addr)/(ECDIGIT+1))*ECDIGIT), ECDIGIT );	//�ָ����������е�ǰ���ֵ //10.12.08					
				for( i=0;i<5;i++ )
				{
					E2P_WFM( Addr, Ptr+i*ECDIGIT, ECDIGIT );			//��EEPROM�еı��������滻�������еĴ�������������ܵ��µ������١�	
					RAM_Write( TempPtr, Ptr+i*ECDIGIT, ECDIGIT );
					*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
					TempPtr += ECDIGIT+1;
					Addr += ECDIGIT+1;
				}	
			}	
			else 
			{
				RAM_Write( Dest, Point, ECDIGIT );						//���硢EEPROM���ݾ����ԣ������һ�ζ�ȡ���������ݷ��أ�����ˢ��RAM����
				return;					
			}	
		}	
	}
	else
	{																	//�޷��ʵ���
		E2P_RData( Ptr, Addr, ECDIGIT );	
		if( E2PECIsOK( Ptr ) != 0 ) 
		{
			State = 1;													//EEPROM�������Լ����
		}	
		E2P_RFM( Point, ECFAds, ECDIGIT );								//�ٴζ�ȡFRAM���ݡ�	
		if( E2PECIsOK( Point ) == 0 ) 
		{
			RAM_Write( Ptr, Point, ECDIGIT );							//�����е�����ȡ��ȷ�����������Ϊ׼��������RRPROM��ȡ�õ�����
		}	
		else
		{																//�������ݻ�������	
			if( State == 0 ) E2P_WFM( ECFAds, Ptr, ECDIGIT );			//��EEPROM�еı��������滻�������еĴ�����������ܵ��µ������١�		
			else 
			{
				RAM_Write( Dest, Point, ECDIGIT );						//���硢EEPROM���ݾ����ԣ������һ�ζ�ȡ���������ݷ��أ�����ˢ��RAM����
				return;					
			}	
		}	
		RAM_Write( Dest, Ptr, ECDIGIT );								//������ȷ���������ݻ򱸷�EEPROM����
		RAM_Write( ECRAds, Dest, ECDIGIT );								//RAM�е�ֵ�ָ�Ϊ��ȷ���������ݻ򱸷�EEPROM����
		*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
	}		
	return;	
}

void GetRamEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds ) 
{
	
	if( RAMECIsOK( ECRAds ) == 0 )
	{
		RAM_Write( Dest, ECRAds, ECDIGIT );					//RAM�е���У�����ȣ���ȡRAM������
#if( ECDisDecDigitsValid ==	YesCheck )			//13.03.03 ��ǰ������ʾС��λ��ʵ��Ч
		if( Disk.DigitsValidFlg == 0x55) AddECMantissa( Dest, ECRAds ); 	//��ʾʱ�����0.01kWh����β��
#endif
	}
	else 
	{
		E2P_RFM( Dest, ECFAds, ECDIGIT );					//RAM��У��ʹ��󣬶�ȡFRAM���ݡ�	
		if( E2PECIsOK( Dest ) == 0 ) 
		{
			RAM_Write( ECRAds, Dest, ECDIGIT );				//RAM�е�ֵ�ָ�ΪFRAM�е�ֵ
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
		}
		else
		{
			if((Flag.Power & F_PwrUp) != 0) 				//ͣ��״̬�²������ݻָ�	
			{
				ResumeEC( Dest, ECRAds, ECFAds );
			}	
		}		
	}	
}

void Read_CMonEC( unsigned char* Dest, unsigned short ECRgAds )
{
	unsigned short i;
	unsigned short ECEAds;
	unsigned short ECFAds;
	unsigned char* ECRAds;
	
///	if( CombECJust( Dest, ECRgAds ) == 0 ) return; 	//��ϵ����ж� 
	
	for( i=0;i<ECUnitNum;i++ )
	{
		ECEAds = ECRgTab[i].ECEAds; 
		ECRAds = ECRgTab[i].ECRAds; 
		ECFAds = ECRgTab[i].ECFAds;

		if( ECEAds == ECRgAds ) break;				//�ܵ���
		else if(( i < (ECUnitNum-1) )&&( ECEAds < ECRgAds )&&( ECRgTab[i+1].ECEAds > ECRgAds ))	//���ʵ���
		{
			if(((( ECRgAds - ECEAds ) % (ECDIGIT+1) ) == 0 )&&( ECRgTab[i].ECRgMode == 0 ))	//�з��ʵ����ҷ��ʵ�����ַ��ȷ
			{
				ECRAds += ECRgAds - ECEAds;		//���ʵ���RAM��ַ
				ECFAds += ECRgAds - ECEAds;		//���ʵ��������ַ
				break;
			}	
		}			
	}	
	if( i == ECUnitNum ) 
	{
		RAM_Fill( Dest, ECDIGIT );				//������ַ����
		return;			
	}	
	GetRamEC( Dest, ECRAds, ECFAds );			//��ȡRAM��FRAM�еĵ���

}

//�ܼ�RAM�еķ�ʱ����
//Source: ��ŵ�����RAM��
//Type: 0��RAM��������ÿ��������У��ͣ� 1��ÿ���������һ��У��͡�
void FeeECSub( unsigned char* Source, short Type )
{
	unsigned char Buff[(ECDIGIT+1)*5];
	unsigned char* Ptr;
	unsigned short Len;
	short i;
	unsigned short FeeNo;

	unsigned char Buff2[ECDIGIT+1];					//11.06.13
	unsigned char* Point;							//11.06.13	

	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > 4 )) return;				//��ǰ���ʺŷǷ�
	
	Ptr = Buff;
	Point = Buff2;									//11.06.13
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Write( Ptr, Source, (ECDIGIT+1)*5 );

	RAM_Fill( Point, Len );							//11.06.13
	for( i=1;i<5;i++ )								//11.06.13
	{												//11.06.13
		if( i == FeeNo ) continue;					//11.06.13
		ECBCDAdd( Point, Ptr+i*Len );				//11.06.13
	}												//11.06.13
	if( Data_Comp( Point, Ptr, Len ) > 0 ) return;	//11.06.13		//��ʱ����֮�ʹ����ܵ��������ʵ�����������

	for( i=1;i<5;i++ )
	{
		if( i == FeeNo ) continue;
		ECBCDSub( Ptr, Ptr+i*Len );
	}	
	RAM_Write( Source+FeeNo*Len, Ptr, ECDIGIT );
}

//����ǰ����������������RAM
void AddCurrentEC( unsigned char* ECRAds, unsigned short ECFAds, unsigned char* Source )
{
	unsigned char* TempPtr;
	unsigned short FeeNo;

	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > 4 )) return;				//��ǰ���ʺŷǷ�
	
	ECBCDAdd( ECRAds, Source );								//ȡRAM���ܵ���Ϊ��׼���ӵ���
	*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
	TempPtr = ECRAds+FeeNo*(ECDIGIT+1);						//ȡRAM�ڵ�ǰ���ʵ���Ϊ��׼���ӵ���
#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
	ECBCDAdd( TempPtr, Source+6 );
#else
	ECBCDAdd( TempPtr, Source );
#endif
	*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
	E2P_WFM( ECFAds, ECRAds, ECDIGIT );						//����У���ֱ�д������
	E2P_WFM( ECFAds+FeeNo*(ECDIGIT+1), TempPtr, ECDIGIT );								
	
}

//Source: ��ǰ������������RAM����ʱ��ŵ���ַ
//AddData: ���ӵĵ���
//Type: 0: �ܳ��� 1����ʱ��������
short ModifyCurrentEC( unsigned char* ECRAds, unsigned short ECFAds, unsigned char* Source, unsigned char* AddData, short Type )
{
	unsigned short Addr;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned short FeeNo;
	
	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > 4 )) return 1;					//��ǰ���ʺŷǷ�
	TempPtr1 = Source+ECDIGIT;
	for( i=1;i<5;i++ )
	{
		if( i == FeeNo )
		{
//			TempPtr1 += ECDIGIT+1;								//��ǰ���ʵ�������ȷ�������ض�		
			TempPtr1 += ECDIGIT;								//��ǰ���ʵ�������ȷ�������ض�		//10.05.12
			continue;
		}	
		E2P_RFM( TempPtr1, ECFAds+i*(ECDIGIT+1), ECDIGIT );		//������������ʵ���
		if( E2PECIsOK( TempPtr1 ) != 0 ) break;					//�жϴ������ж����Ƿ�Ϸ�	
//		TempPtr1 += ECDIGIT+1;																					
		TempPtr1 += ECDIGIT;									//10.05.12	
	}	
	if( Type == 0 ) 
	{
		TempPtr1 = Source;
		Addr = ECFAds;
	}
	else
	{
		TempPtr1 = Source + FeeNo*ECDIGIT;
		Addr = ECFAds+FeeNo*(ECDIGIT+1);		
	}		
	E2P_RFM( TempPtr1, Addr, ECDIGIT );							//�ٴζ�ȡ�����еĵ�ǰ�ܵ������ʱ����
//	if( E2PECIsOK( TempPtr1 ) == 0 ) 							//������ȷ
	if(( E2PECIsOK( TempPtr1 ) == 0 )||( i != 5 )) 				//������ȷ���޷��޸�������Ϊ׼
	{
		RAM_Write( ECRAds, Source, ECDIGIT );
		RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Source+FeeNo*ECDIGIT, ECDIGIT );
		AddCurrentEC( ECRAds, ECFAds, AddData );
	}	
	else if( i == 5 )
	{
		if( Type == 0 ) FeeECAdd( Source, 0 );					//�ܵ��ڷ�ʱ����֮��
		else FeeECSub( Source, 0 );								//��ʱ���������ܼ�ȥ�����ʱ����			
		TempPtr1 = ECRAds;
		for( i=0;i<5;i++ )
		{
			RAM_Write( TempPtr1, Source+i*ECDIGIT, ECDIGIT );
			*(TempPtr1+ECDIGIT) = ChkNum( TempPtr1, ECDIGIT );								
			TempPtr1 += ECDIGIT+1; 
		}	
		AddCurrentEC( ECRAds, ECFAds, AddData );
	}
//	else return 1;	 

	return 0;
}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
void FeeECPulseAdd( unsigned short* PL_Cum, unsigned char* PL_Chk, unsigned char Pulse_Cum )
{
	unsigned short* PL_CumPtr;
	unsigned char* PL_ChkPtr;
	unsigned short Temp;
	
	if(( SM.FeeNo < 1 )||( SM.FeeNo > 4 )) return;				//��ǰ���ʺŷǷ�

	Temp = SM.FeeNo;
	Temp -= 1; 
	PL_CumPtr = PL_Cum;
	PL_CumPtr += Temp;
	PL_ChkPtr = PL_Chk;
	PL_ChkPtr += Temp;
//�¹���		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//	*PL_CumPtr += (unsigned short)Pulse_Cum * 10;	
//#else
	*PL_CumPtr += Pulse_Cum;	
//#endif
	*PL_ChkPtr = ChkNum( (unsigned char*)PL_CumPtr, 2 );
}
#endif
#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
short ECRgSum( short ECItem, unsigned short Data, unsigned short FData )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[30];
//	unsigned char Buff2[6];
	unsigned char Buff2[12];
	unsigned char* Ptr;
	unsigned char* Source;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned char FStateZ=0;
	unsigned char FStateF=0;
	unsigned char RState=0;
	unsigned short FeeNo;
	
	Ptr = Buff;
	Source = Buff2;
	
	RAM_Fill( Ptr, 30 );
//	RAM_Fill( Source, 6 );
	RAM_Fill( Source, 12 );
	Word_BCD( Source+1, Data );											//��λС����Ч��
	Word_BCD( Source+7, FData );											//��λС����Ч��
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	FeeNo = SM.FeeNo;

	if(( FeeNo < 1 )||( FeeNo > 4 )) return 1;							//��ǰ���ʺŷǷ�
	
	if( ECRgTab[ECItem].ECRgMode == 0 )									//�����ʵ���
	{	
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    	//�ܵ����Ƿ�Ϸ�	
		TempPtr1 = Ptr+FeeNo*ECDIGIT;
		E2P_RFM( TempPtr1, ECFAds+FeeNo*(ECDIGIT+1), ECDIGIT );
		if( E2PECIsOK( TempPtr1 ) != 0 ) FStateF = 1;                   //��ʱ�����Ƿ�Ϸ� 		
		
		if(( FStateZ == 0 )&&( FStateF == 0 ))							//�ܺ͵�ǰ���ʵ������������ȷ
		{
			RAM_Write( ECRAds, Ptr, ECDIGIT );
			RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
			AddCurrentEC( ECRAds, ECFAds, Source );
		}	
		else															//���ܵ����ͷ�ʱ��������1�����ɹ�
		{
			if( TotalFeeECCheck( ECRAds, 1 ) == 0 )						//�ж�RAM���ĵ������Ƿ���ڷ�ʱ����֮��
			{
				for( i=0;i<5;i++ )
				{
					if( RAMECIsOK( ECRAds+i*(ECDIGIT+1) ) != 0 )		//RAM���ĸ������Լ��Ƿ���ȷ
					{
						RState = 1;										//�е����Ƿ���У�鲻�Ի�Ϊ��BCD��	
						break;	
					}	
				}	
			}	
			else RState = 1;											//RAM���������������ʱ֮�Ͳ���
			if( RState == 0 )											//RAM�ڵ������ݾ��Ϸ���
			{				
				AddCurrentEC( ECRAds, ECFAds, Source );							
			}
			else														//RAM�ڵ�������	
			{
				if(( FStateZ == 1 )&&( FStateF == 0 ))					//�ܴ��󡢵�ǰ���ʵ�����������ȷ
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 0 );
				}
				else if(( FStateZ == 0 )&&( FStateF == 1 ))				//����ȷ�͵�ǰ���ʵ������������
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 1 );
				}
//				else return 1;		
				else 
				{
					ResumeEC( Ptr, ECRAds, ECFAds );					//��RAM��������д�����EEPROM�ڵ����ָ�	//10.12.08
					ResumeEC( Ptr+FeeNo*ECDIGIT, ECRAds+FeeNo*(ECDIGIT+1), ECFAds+FeeNo*(ECDIGIT+1) );		//��RAM��������д�����EEPROM�ڵ����ָ�	//10.12.08
					RAM_Write( ECRAds, Ptr, ECDIGIT );										//ȫ��������Ϊ׼
					RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
					AddCurrentEC( ECRAds, ECFAds, Source );							
//					return 1;		
				}	
			}		
		}	
	}
	else
	{
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    		
		if( RAMECIsOK( ECRAds ) != 0 ) RState = 1;

		if(( FStateZ == 1 )&&( RState == 1 )) ResumeEC( Ptr, ECRAds, ECFAds );				//��RAM��������д�����EEPROM�ڵ����ָ�	//10.12.08

//		if(( FStateZ == 0 )||( RState == 0 ))
//		{
//			if( FStateZ == 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//����������Ϊ׼
			if( RState != 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//ֻҪRAM���Ծ�����������Ϊ׼
			ECBCDAdd( ECRAds, Source );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, ECRAds, ECDIGIT );						
//		}	 		
//		else return 1;		
	}	
	return 0;
}	
#else
short ECRgSum( short ECItem, unsigned short Data )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[30];
	unsigned char Buff2[6];
	unsigned char* Ptr;
	unsigned char* Source;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned char FStateZ=0;
	unsigned char FStateF=0;
	unsigned char RState=0;
	unsigned short FeeNo;
	
	Ptr = Buff;
	Source = Buff2;
	
	RAM_Fill( Ptr, 30 );
	RAM_Fill( Source, 6 );
	Word_BCD( Source+1, Data );											//��λС����Ч��
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	FeeNo = SM.FeeNo;

	if(( FeeNo < 1 )||( FeeNo > 4 )) return 1;							//��ǰ���ʺŷǷ�
	
	if( ECRgTab[ECItem].ECRgMode == 0 )									//�����ʵ���
	{	
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    	//�ܵ����Ƿ�Ϸ�	
		TempPtr1 = Ptr+FeeNo*ECDIGIT;
		E2P_RFM( TempPtr1, ECFAds+FeeNo*(ECDIGIT+1), ECDIGIT );
		if( E2PECIsOK( TempPtr1 ) != 0 ) FStateF = 1;                   //��ʱ�����Ƿ�Ϸ� 		
		
		if(( FStateZ == 0 )&&( FStateF == 0 ))							//�ܺ͵�ǰ���ʵ������������ȷ
		{
			RAM_Write( ECRAds, Ptr, ECDIGIT );
			RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
			AddCurrentEC( ECRAds, ECFAds, Source );
		}	
		else															//���ܵ����ͷ�ʱ��������1�����ɹ�
		{
			if( TotalFeeECCheck( ECRAds, 1 ) == 0 )						//�ж�RAM���ĵ������Ƿ���ڷ�ʱ����֮��
			{
				for( i=0;i<5;i++ )
				{
					if( RAMECIsOK( ECRAds+i*(ECDIGIT+1) ) != 0 )		//RAM���ĸ������Լ��Ƿ���ȷ
					{
						RState = 1;										//�е����Ƿ���У�鲻�Ի�Ϊ��BCD��	
						break;	
					}	
				}	
			}	
			else RState = 1;											//RAM���������������ʱ֮�Ͳ���
			if( RState == 0 )											//RAM�ڵ������ݾ��Ϸ���
			{				
				AddCurrentEC( ECRAds, ECFAds, Source );							
			}
			else														//RAM�ڵ�������	
			{
				if(( FStateZ == 1 )&&( FStateF == 0 ))					//�ܴ��󡢵�ǰ���ʵ�����������ȷ
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 0 );
				}
				else if(( FStateZ == 0 )&&( FStateF == 1 ))				//����ȷ�͵�ǰ���ʵ������������
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 1 );
				}
//				else return 1;		
				else 
				{
					ResumeEC( Ptr, ECRAds, ECFAds );					//��RAM��������д�����EEPROM�ڵ����ָ�	//10.12.08
					ResumeEC( Ptr+FeeNo*ECDIGIT, ECRAds+FeeNo*(ECDIGIT+1), ECFAds+FeeNo*(ECDIGIT+1) );		//��RAM��������д�����EEPROM�ڵ����ָ�	//10.12.08
					RAM_Write( ECRAds, Ptr, ECDIGIT );										//ȫ��������Ϊ׼
					RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
					AddCurrentEC( ECRAds, ECFAds, Source );							
//					return 1;		
				}	
			}		
		}	
	}
	else
	{
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    		
		if( RAMECIsOK( ECRAds ) != 0 ) RState = 1;

		if(( FStateZ == 1 )&&( RState == 1 )) ResumeEC( Ptr, ECRAds, ECFAds );				//��RAM��������д�����EEPROM�ڵ����ָ�	//10.12.08

//		if(( FStateZ == 0 )||( RState == 0 ))
//		{
//			if( FStateZ == 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//����������Ϊ׼
			if( RState != 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//ֻҪRAM���Ծ�����������Ϊ׼
			ECBCDAdd( ECRAds, Source );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, ECRAds, ECDIGIT );						
//		}	 		
//		else return 1;		
	}	
	return 0;
}	
#endif

void EC_Meas(void)
{
	unsigned char YPulse_Cum=0;		
	unsigned char WPulse_Cum=0;	
	unsigned char YPulse_CumChk;		
	unsigned char WPulse_CumChk;
	unsigned char Temp2;	
	unsigned short Temp;	
	short	Meter_Quad,i;
	unsigned short PS32;					//�¹���		//13.08.30	
#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
	unsigned short* ECRamBufAdsPtr;
	unsigned char* ECRamBufChkAdsPtr;
	unsigned short* FeeECRamBufAdsPtr;
	unsigned char* FeeECRamBufChkAdsPtr;
	unsigned short Temp3;
#endif
#if ( MEASCHIP == IDT90E32 )
	unsigned short TempInt;	
#endif

//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	//�Ȱ�1000���峣����
	PS32 = (Measur_Const/100);//MSpec.RMeterConst / 10;						//�¹���		//13.08.30	

	if(( ECInt.YPulse_Cum != 0 )||( ECInt.WPulse_Cum != 0 ))		
	{														
		__disable_irq();//_DINT();
		YPulse_Cum = ECInt.YPulse_Cum;
		WPulse_Cum = ECInt.WPulse_Cum;
		YPulse_CumChk = ECInt.YPulse_CumChk;
		WPulse_CumChk = ECInt.WPulse_CumChk;
		ECInt.YPulse_Cum = 0;
		ECInt.WPulse_Cum = 0;
		ECInt.YPulse_CumChk = 0xA5;
		ECInt.WPulse_CumChk = 0xA5;
		__enable_irq();//_EINT();
		Temp2 = YPulse_Cum + 0xA5;
		if( YPulse_CumChk != Temp2 ) YPulse_Cum = 0;						
		Temp2 = WPulse_Cum + 0xA5;
		if( WPulse_CumChk != Temp2 ) WPulse_Cum = 0;												

#if ( FRAMPulseValidCheck == YesCheck )			//16.01.26
		if(( YPulse_Cum != 0 )||( WPulse_Cum != 0 ))
		{
			if( SM.PulseFlag == 0 )		//�Ѳ���������		//16.01.26
			{
				E2P_WFM( FPulseValidFlag, (unsigned char*)&SM.PulseFlag, 1 );	 	//�µ�󱣴�RAM�ڵĲ���0.01Kwh�����������Ч��־				
			}
			SM.PulseFlag = 1;	
		}	
#endif		
	}															
	
	Meter_Quad = GetMQuad( Real.PWFlag,3 );		//ȡ��ǰ����		
	if( YPulse_Cum !=0 )
	{
		switch (Meter_Quad )
		{
			case 2:
			case 3:	
					ECP.PL_CumPn +=  YPulse_Cum;
					ECP.PL_ChkPn = ChkNum( (unsigned char*)&ECP.PL_CumPn, 2 );
					MDBufPn.MD_Cum += YPulse_Cum;	//�й���������
					ECP.PL_CumPn_Dis += YPulse_Cum;

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
					FeeECPulseAdd( &FeeECP.PL_CumPn1, &FeeECP.PL_ChkPn1, YPulse_Cum );
#endif
					break;		//������ϼƣ� 	
			case 1:
			case 4: 
					ECP.PL_CumPp +=  YPulse_Cum; 
					ECP.PL_ChkPp = ChkNum( (unsigned char*)&ECP.PL_CumPp, 2 );
					MDBufPp.MD_Cum += YPulse_Cum;	//�й���������
					ECP.PL_CumPp_Dis += YPulse_Cum;

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
					FeeECPulseAdd( &FeeECP.PL_CumPp1, &FeeECP.PL_ChkPp1, YPulse_Cum );
#endif
					break;
			default: break;
		}
	}	
	if( WPulse_Cum !=0 )
	{
		switch (Meter_Quad )
		{
			case 1:
				ECP.PL_CumQ1 += WPulse_Cum; ECP.PL_ChkQ1 = ChkNum( (unsigned char*)&ECP.PL_CumQ1, 2 );
				ECP.PL_CumQp += WPulse_Cum; ECP.PL_ChkQp = ChkNum( (unsigned char*)&ECP.PL_CumQp, 2 );
				break; 	
			case 2:
				ECP.PL_CumQ2 += WPulse_Cum; ECP.PL_ChkQ2 = ChkNum( (unsigned char*)&ECP.PL_CumQ2, 2 );
				ECP.PL_CumQp += WPulse_Cum; ECP.PL_ChkQp = ChkNum( (unsigned char*)&ECP.PL_CumQp, 2 );
				break; 	
			case 3:
				ECP.PL_CumQ3 += WPulse_Cum; ECP.PL_ChkQ3 = ChkNum( (unsigned char*)&ECP.PL_CumQ3, 2 );
				ECP.PL_CumQn+= WPulse_Cum; ECP.PL_ChkQn= ChkNum( (unsigned char*)&ECP.PL_CumQn, 2 );
				break; 	
			case 4:
				ECP.PL_CumQ4 += WPulse_Cum; ECP.PL_ChkQ4 = ChkNum( (unsigned char*)&ECP.PL_CumQ4, 2 );
				ECP.PL_CumQn+= WPulse_Cum; ECP.PL_ChkQn= ChkNum( (unsigned char*)&ECP.PL_CumQn, 2 );
				break;		
			default: break;
		}

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
		switch (Meter_Quad )
		{
			case 1:	FeeECPulseAdd( &FeeECP.PL_CumQ11, &FeeECP.PL_ChkQ11, WPulse_Cum ); break;
            case 2: FeeECPulseAdd( &FeeECP.PL_CumQ21, &FeeECP.PL_ChkQ21, WPulse_Cum ); break;
            case 3: FeeECPulseAdd( &FeeECP.PL_CumQ31, &FeeECP.PL_ChkQ31, WPulse_Cum ); break;
            case 4: FeeECPulseAdd( &FeeECP.PL_CumQ41, &FeeECP.PL_ChkQ41, WPulse_Cum ); break;
			default: break;
		}   
#endif      

	}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
	for( i=0;i<ECUnitNum;i++ )						//����0.01��д����洢��
	{
		ECRamBufAdsPtr = ECRgTab[i].ECRamBufAds;
		ECRamBufChkAdsPtr = ECRgTab[i].ECRamBufChkAds;
		if( *ECRamBufChkAdsPtr != ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 ) )
		{
			*ECRamBufAdsPtr = 0;
			*ECRamBufChkAdsPtr = 0;
		}	

		Temp = 0;
#if ( MEASCHIP == IDT90E32 )
		if( i < 8 ) TempInt = PS32;				
		else TempInt = PS32*10;						//�������޹����������ڵ��������Ǵ�IDT90E32�ж�������λΪ0.1CF	
		if( *ECRamBufAdsPtr >= TempInt )
		{
			Temp = *ECRamBufAdsPtr / TempInt;
			*ECRamBufAdsPtr -= Temp * TempInt;
			*ECRamBufChkAdsPtr = ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 );
		}
#else
		if( *ECRamBufAdsPtr >= PS32 )
		{
			Temp = *ECRamBufAdsPtr / PS32;
			*ECRamBufAdsPtr -= Temp * PS32;
			*ECRamBufChkAdsPtr = ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 );
		}
#endif
		if( i < 8 )
		{
			Temp3 = 0;
			FeeECRamBufAdsPtr = FeeECRgTab[i].ECRamBufAds;
			FeeECRamBufAdsPtr += (SM.FeeNo-1);
			FeeECRamBufChkAdsPtr = FeeECRgTab[i].ECRamBufChkAds;
			FeeECRamBufChkAdsPtr += (SM.FeeNo-1);
			if( *FeeECRamBufChkAdsPtr != ChkNum( (unsigned char*)FeeECRamBufAdsPtr, 2 ) )
			{
				*FeeECRamBufAdsPtr = 0;
				*FeeECRamBufChkAdsPtr = 0;
			}	
			if( *FeeECRamBufAdsPtr >= PS32 )
			{
				Temp3 = *FeeECRamBufAdsPtr / PS32;
				*FeeECRamBufAdsPtr -= Temp3 * PS32;
				*FeeECRamBufChkAdsPtr = ChkNum( (unsigned char*)FeeECRamBufAdsPtr, 2 );
			}
		}
		else Temp3 = Temp;
		if(( Temp > 100 )||( Temp3 > 100 )) continue;				//���ݳ�������������
		if(( Temp == 0 )&&( Temp3 == 0 )) continue;					//�����壬���ء�		//11.03.01
		ECRgSum( i, Temp, Temp3 );
/*
#if( LocalSmartMeter ==	YesCheck )									//17.04.22
		if((( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 )))&&( Temp != 0 )) IncConsumECSum( Temp, 0 );	//���������й������õ���							//15.07.31	
		if((( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 )))&&( Temp3 != 0 )) IncConsumECSum( Temp3, 1 );	//���������й������õ���	//�¹���	//13.11.30	//15.07.31	
#endif

#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else																		//�¹���	//13.11.06
#if ( MagnKeepRelay == NoCheck )											//10.08.07
		if((( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 )))&&( Temp != 0 ))							//�̵������ñ�����բ���ܵ���ÿ��0.01kWh������բһ�Ρ�	
		{
			if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayDrivePulseWidth == 0 )) 	//����բ�Ҳ���������բ	//10.08.07
			{
				RelayDrive( RELAY_OFF );						
			}					
		}	
#endif
#endif					//�¹���	//13.11.06
*/
	}

#else
	for( i=0;i<ECUnitNum;i++ )						//����0.01��д����洢��
	{
		if( *ECRgTab[i].ECRamBufChkAds != ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 ) )
		{
			*ECRgTab[i].ECRamBufAds = 0;
			*ECRgTab[i].ECRamBufChkAds = 0;
		}	
		
#if ( MEASCHIP == IDT90E32 )
		if( i < 6 ) TempInt = PS32;				
		else TempInt = PS32*10;						//�������޹����������ڵ��������Ǵ�IDT90E32�ж�������λΪ0.1CF	
		if( *ECRgTab[i].ECRamBufAds >= TempInt )
		{
			Temp = *ECRgTab[i].ECRamBufAds / TempInt;
			*ECRgTab[i].ECRamBufAds -= Temp * TempInt;
#else
		if( *ECRgTab[i].ECRamBufAds >= PS32 )
		{
			Temp = *ECRgTab[i].ECRamBufAds / PS32;
			*ECRgTab[i].ECRamBufAds -= Temp * PS32;
#endif
			*ECRgTab[i].ECRamBufChkAds = ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 );
			if( Temp > 100 ) continue;				//���ݳ�������������
			ECRgSum( i, Temp);

//			if( Temp > 100 ) 
//			{
//				*ECRgTab[i].ECRamBufAds -= Temp * PS32;
//				*ECRgTab[i].ECRamBufChkAds = ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 );			
//				continue;				//���ݳ�������������				
//			}	
//			if( ECRgSum( i, Temp) == 0 )
//			{
//				*ECRgTab[i].ECRamBufAds -= Temp * PS32;
//				*ECRgTab[i].ECRamBufChkAds = ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 );			
//			}	
//			else continue;	

/*
#if( LocalSmartMeter ==	YesCheck )									//17.04.22
			if(( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 ))) IncConsumECSum( Temp );	//���������й��õ���
#endif

#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#else																		//�¹���	//13.11.06
//#if (( JiangSuRequire == YesCheck )&&( MagnKeepRelay == NoCheck))			//10.08.07
#if ( MagnKeepRelay == NoCheck )											//10.08.07
			if( i == 0 )							//�̵������ñ�����բ��ÿ��0.01kWh������բһ�Ρ�	
			{
//				if( Para.RMeterRelayState == 0x1A ) //����բ
				if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayDrivePulseWidth == 0 )) 	//����բ�Ҳ���������բ	//10.08.07
				{
					RelayDrive( RELAY_OFF );						
//					if( Un55VoltJudge() == 0 ) RelayDrive( RELAY_OFF );					//10.01.18					
				}					
			}	
#endif
#endif		
*/
		}						
	}		
#endif		
}	
#endif

