#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "IECComm.h"

//#include "RsComm.h"
#include "interrupt.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#include "ApplComm.h"
#include "COSEMComm.h"
//#include "AESGCM.H"

// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ������ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ�����ݳ���
//short gsmString2Bytes(const char* pSrc, unsigned char* pDst, short nSrcLength)
short ASC_BCD(unsigned char* pDst, unsigned char* pSrc, short nSrcLength)
{
	short i;
	
	if( nSrcLength == 0 ) return 0;
	pDst += nSrcLength - 1;
	
//	for( i=0; i<nSrcLength; i+=2 )
	for( i=0; i<nSrcLength; i++ )
	{
		// �����4λ
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// �����4λ
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst |= *pSrc - '0';
		}
		else
		{
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
//		pDst++;
		pDst--;
	}

	// ����Ŀ�����ݳ���
	return nSrcLength / 2;
}

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// pSrc: Դ����ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���ݳ���
// ����: Ŀ���ַ�������
//short gsmBytes2String(const unsigned char* pSrc, char* pDst, short nSrcLength)
short BCD_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�
        short i;        

	pDst += nSrcLength*2 -1;

	for( i=0; i<nSrcLength; i++)
	{
		*pDst-- = tab[*pSrc & 0x0f];	// �����4λ
//		*pDst++ = tab[*pSrc >> 4];		// �����4λ
		*pDst-- = tab[*pSrc >> 4];		// �����4λ
//		*pDst++ = tab[*pSrc & 0x0f];	// �����4λ
		pSrc++;
	}

	// ����ַ����Ӹ�������
//	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nSrcLength * 2;
}

//short GetBandRate( unsigned char BandRate )
//{
//	short i;
//	
////	for( i=0;i<6;i++ )
//	for( i=0;i<7;i++ )
//	{
////		if(( BandRate & ByteBit[i+1] )!= 0 ) return i*3;		
//		if(( BandRate & ByteBit[i] )!= 0 ) return i*3;		
//	}			
//	return 2*3;
//}

short GetBandRate( unsigned char BandRate )
{
	unsigned short Temp;
	
	switch( BandRate )
	{
		case Baut300: Temp = 300; break; 	
		case Baut600: Temp = 600; break; 	
		case Baut1200: Temp = 1200; break; 	
		case Baut4800: Temp = 4800; break; 	
	//	case Baut7200: Temp = 7200; break; 	
		case Baut9600: Temp = 9600; break; 
		case Baut19200: Temp = 19200; break;
		case Baut38400: Temp = 38400; break;
		case Baut57600: Temp = 57600; break;
	//	case Baut115200: Temp = 115200; break;
		default: Temp = 57600; break; 	
	}	
	return Temp;
}


unsigned char GetTXDelay( unsigned char* Source )
{
	unsigned char Temp;
	unsigned char TXDelay;
	
	Temp = *Source;
	
	switch( Temp )
	{
		case Baut_300H:		TXDelay = 160;	break;	
		case Baut_600H:		TXDelay = 64;	break;	
		case Baut_1200H:	TXDelay = 32; 	break;	
		case Baut_2400H:	TXDelay = 16; 	break;	
		case Baut_4800H:	TXDelay = 8;	break;	
		case Baut_9600H:	TXDelay = 5;	break;	
		case Baut_19200H:	TXDelay = 4;	break;
	//	case Baut_57600H:	TXDelay = 500;	break;
		default: 			TXDelay = 7; 	break;				
	}		
	return TXDelay;
}	

short GetCOM_No( unsigned char* ComBuf )
{
	short COM_No=0;
	
	if( ComBuf == Comm.Buf0 ) COM_No = 0;
	if( ComBuf == Comm.Buf1 ) COM_No = 1;
	if( ComBuf == Comm.Buf2 ) COM_No = 2;
//	if( ComBuf == Comm.Buf3 ) COM_No = 3;

	return COM_No;
}		

unsigned char* GetComBuf( unsigned short COM_No )
{
	unsigned char* ComBuf=Comm.Buf0;
	
	if( COM_No == 0 ) ComBuf = Comm.Buf0;
	else if( COM_No == 1 ) ComBuf = Comm.Buf1;
	else if( COM_No == 2 ) ComBuf = Comm.Buf2;
//	else if( COM_No == 3 ) ComBuf = Comm.Buf3;

	return ComBuf;
}		

UART_InitTypeDef UART_InitStructure; 
void fnUSART_ZBCOM_Init(unsigned char Cfg)
{
	unsigned short Temp;
	Temp = GetBandRate( Cfg );					

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);		/*!< ʹ��UART0ģ��				*/
					
		/*!< UART0������Ϣ				 */
		UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART�߼�Ϊ��				*/
		UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1λֹͣλ					*/
		UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8λ����λ					*/
		UART_InitStructure.UART_Parity = UartParity_Disable;			/*!< żУ��*/
		UART_InitStructure.UART_BaudRate = Temp;					/*!< ������9600 				*/
		UART_InitStructure.ReceiveEN = ENABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/
		//UART_InitStructure.SendEN = ENABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/		
		UART_InitStructure.SendEN = DISABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/		
		
		HT_UART_Init(HT_UART0, &UART_InitStructure);
		
		HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF);
		HT_UART_ITConfig(HT_UART0, UART_UARTCON_RXIE, ENABLE);

	return;
}

//===(����)================================
void fnUSART_IRCom_Init(unsigned char Cfg)
{  
	unsigned short Temp;
	Temp = GetBandRate( Cfg );

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART1EN, ENABLE);		/*!< ʹ��UART1ģ��				*/
							  
	 /*!< UART1������Ϣ	,300,7,E,1		   */
	 UART_InitStructure.UART_Logic = UartLogicPositive;		  /*!< UART�߼�Ϊ�� 			  */
	 UART_InitStructure.UART_StopBits = OneStopBits;			  /*!< 1λֹͣλ				  */
	 UART_InitStructure.UART_WordLength = WordLength_8Bits ;	  /*!< 8λ����λ				  */
	 UART_InitStructure.UART_Parity = UartParity_EVEN;		  /*!< ��У�� */
	 UART_InitStructure.UART_BaudRate = Temp;					  /*!< �����ʼ������300		  */
	 UART_InitStructure.ReceiveEN = ENABLE;					  /*!< ����ʹ��   ע�����ж�ʹ��  */
	 //UART_InitStructure.SendEN = ENABLE; 					  /*!< ����ʹ��   ע�����ж�ʹ��  */	  
	 UART_InitStructure.SendEN = DISABLE;						  /*!< ����ʹ��   ע�����ж�ʹ��  */	  
	 
	 HT_UART_Init(HT_UART1, &UART_InitStructure);
	 
	// HT_UART1->IRDUTY = 0x00000000;								  //ռ�ձ�50%
	// HT_UART1->IRCON = 0x00000001;   //����38k��Դ��PLL,������������ԣ�ʹ��TX�������
	 HT_UART_ClearITPendingBit(HT_UART1, UART_UARTSTA_RXIF);
	 HT_UART_ITConfig(HT_UART1, UART_UARTCON_RXIE, ENABLE);

	return;
}
void fnUART2_Init(unsigned char Cfg)
{
	unsigned short Temp;

	Temp = GetBandRate( Cfg );

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, ENABLE);		/*!< ʹ��UART2ģ��				*/
		//����ͨѶ��ʼ������4800,7��e,1
		/*!< UART2������Ϣ	debugge			 */
		UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART�߼�Ϊ��				*/
		UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1λֹͣλ					*/
		UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8λ����λ					*/
		UART_InitStructure.UART_Parity = UartParity_EVEN;			/*!< żУ��*/
		UART_InitStructure.UART_BaudRate = Temp;					/*!< ������  				    */
		UART_InitStructure.ReceiveEN = ENABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/
		UART_InitStructure.SendEN = ENABLE;						    /*!< ����ʹ��	ע�����ж�ʹ��	*/		
		//UART_InitStructure.SendEN = DISABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/		
		
		HT_UART_Init(HT_UART2, &UART_InitStructure);
		HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF);
		HT_UART_ITConfig(HT_UART2, UART_UARTCON_RXIE, ENABLE);//�����ж�ʹ��
		HT_UART_ITConfig(HT_UART2, UART_UARTCON_TXIE, ENABLE);//�����ж�ʹ��

	return;
}

void fnUSART_RS485_1_Init(unsigned char Cfg)
{
	unsigned short Temp;
		Temp = GetBandRate( Cfg );					
	/***** ���´�����������UART2ͨѶ*****/
			HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);		/*!< ʹ��UART3ģ��				*/
		
			/*!< UART2������Ϣ				 */
			UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART�߼�Ϊ��				*/
			UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1λֹͣλ					*/
			UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8λ����λ					*/
			UART_InitStructure.UART_Parity = UartParity_Disable;		/*!< ��У�� 					*/
			UART_InitStructure.UART_BaudRate = Temp;					/*!< ������57600 				*/
			UART_InitStructure.ReceiveEN = ENABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/
			//UART_InitStructure.SendEN = ENABLE; 						/*!< ����ʹ��	ע�����ж�ʹ��	*/	
			UART_InitStructure.SendEN = DISABLE;
			
			HT_UART_Init(HT_UART3, &UART_InitStructure);
			HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_RXIF);
			HT_UART_ITConfig(HT_UART3, UART_UARTCON_RXIE, ENABLE);

	return;
}
void fnUSART_RS485_2_Init(unsigned char Cfg)
{
	unsigned short Temp;
		Temp = GetBandRate( Cfg );					
	/***** ���´�����������UART2ͨѶ*****/
			HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART4_7816_0EN, ENABLE);		/*!< ʹ��UART4ģ��				*/
		
			/*!< UART2������Ϣ				 */
			UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART�߼�Ϊ��				*/
			UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1λֹͣλ					*/
			UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8λ����λ					*/
			UART_InitStructure.UART_Parity = UartParity_Disable;		/*!< ��У�� 					*/
			UART_InitStructure.UART_BaudRate = Temp;					/*!< ������57600 				*/
			UART_InitStructure.ReceiveEN = ENABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/
			//UART_InitStructure.SendEN = ENABLE; 						/*!< ����ʹ��	ע�����ж�ʹ��	*/	
			UART_InitStructure.SendEN = DISABLE;
			
			HT_UART_Init(HT_UART4, &UART_InitStructure);
			HT_UART_ClearITPendingBit(HT_UART4, UART_UARTSTA_RXIF);
			HT_UART_ITConfig(HT_UART4, UART_UARTCON_RXIE, ENABLE);

	return;
}

void fnUART5_Init(unsigned char Cfg)
{
	unsigned short Temp;

	Temp = GetBandRate( Cfg );

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART5EN, ENABLE);		/*!< ʹ��UART5ģ��				*/
		//����ͨѶ��ʼ������9600,7��e,1
		/*!< UART3������Ϣ				 */
		UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART�߼�Ϊ��				*/
		UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1λֹͣλ					*/
		UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8λ����λ					*/
		UART_InitStructure.UART_Parity = UartParity_EVEN;			/*!< żУ��*/
		UART_InitStructure.UART_BaudRate = Temp;					/*!< ������  				    */
		UART_InitStructure.ReceiveEN = ENABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/
		UART_InitStructure.SendEN = ENABLE;						    /*!< ����ʹ��	ע�����ж�ʹ��	*/		
		//UART_InitStructure.SendEN = DISABLE;						/*!< ����ʹ��	ע�����ж�ʹ��	*/		
		
		HT_UART_Init(HT_UART5, &UART_InitStructure);
		HT_UART_ClearITPendingBit(HT_UART5, UART_UARTSTA_RXIF);
		HT_UART_ITConfig(HT_UART5, UART_UARTCON_RXIE, ENABLE);//�����ж�ʹ��
		HT_UART_ITConfig(HT_UART5, UART_UARTCON_TXIE, ENABLE);//�����ж�ʹ��

	return;
}

short GetXorCheck( unsigned char* ComBuf, unsigned char* XorCheck, short* CheckAds)
{
	short	i,j;
	*XorCheck = 0;
	for( i=0;i<RRx_MaxSize;i++)
	{
		if(( *(ComBuf+i) == SOH ) || ( *(ComBuf+i) == STX )) 
		break;
	}
	for( j=i+1;j<RRx_MaxSize;j++ )
	{
		*XorCheck ^= *(ComBuf+j); 
		if(( *(ComBuf+j) == ETX ) || ( *(ComBuf+j) == EOT ))
		{
			*CheckAds = j + 1;
			return 0;	
		}	
	}	
	return -1;
}	

void DLMS_CommInit(unsigned char COM_No)
{
	switch( COM_No )
	{
		case 0:
			if( Comm.TX_Delay0 == 0 ) 
			{	
				Comm.Mode0 = 1;
				Comm.Ptr0 = 0;
			//	GPIO->PB |= P_SW4851;
			//	UART0->CTRL |=0x0c; 	//��������ж�,������մ����ж�
			//	UART0->STA = 0x3d;		//������жϱ�־�����մ����־
			
				HT_UART0->UARTCON |= UART_UARTCON_RXIE + UART_UARTCON_RXEN;//��������ж�	
				HT_UART0->UARTSTA &=~ (UART_UARTSTA_RXIF + UART_UARTSTA_TXIF);
				//HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF);
				Comm.Run0 &= ~F_ComOK;
			}
			break;
		case 1:
			if( Comm.TX_Delay1 == 0 ) 
			{
                          Comm.Ptr1 = 0;
                          HT_UART1->UARTCON |= UART_UARTCON_RXIE + UART_UARTCON_RXEN;//��������ж�   
			  HT_UART1->UARTSTA &=~ (UART_UARTSTA_RXIF + UART_UARTSTA_TXIF);
                          Comm.Run1 &= ~F_ComOK;
			}
			break;	
		case 2:
			if( Comm.TX_Delay2 == 0 ) 
			{	
				Comm.Mode2 = 1;
				Comm.Ptr2 = 0;
			//	HT_GPIOE->PTSET |= GPIOE_Disp_CTL;
				//UART3->CTRL |=0x0c; 	//��������ж�,������մ����ж�
				//UART3->STA = 0x3d;		//������жϱ�־�����մ����־
				
				HT_UART3->UARTCON |= UART_UARTCON_RXIE + UART_UARTCON_RXEN;//��������ж�	
				HT_UART3->UARTSTA &=~ (UART_UARTSTA_RXIF + UART_UARTSTA_TXIF);
			//	HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF);
				Comm.Run2 &= ~F_ComOK;
			}

			break;			
		default: break;		
	}
}


void DLMS_CommMode( short COM_No )
{
  	unsigned char* ComMode;
  	unsigned short* ComPtr;
  	unsigned short* ComBTime;
  	unsigned char* ComRun;
	
  	ComMode = CommPara[COM_No].Mode;
  	ComPtr = CommPara[COM_No].Ptr;
  	ComBTime = CommPara[COM_No].BTime;
  	ComRun = CommPara[COM_No].Run;
	
	switch( *ComMode )
	{
		case 0: 
			DLMS_CommInit( COM_No ); 
			break;	
		case 1: if( *ComPtr == 0 ) break;
				if(( *ComRun & F_ComOK ) == 0 )
				{
					if( *ComBTime == 0 ) 
					{	
						DLMS_CommInit( COM_No );	
					}
				}	
				else 
					{

					}
				break;
		case 2: if(( *ComRun & F_ComOK ) != 0 )
				{
					DLMS_CommInit( COM_No );
				}					
		default: break;				
	}	
	
}

void CommRest( short COM_No )  		//�ָ�����ʼ״̬
{	
	switch( COM_No )
	{
		case 0:												//�ز�
 			fnUSART_ZBCOM_Init(Baut57600);
			Comm.IECState0 = IECModeHDLC;
			Comm.MACState0 = NDMState;
			Comm.LLCState0 = LLCIdle;
			Comm.LastRecCommand0 = 0x00;
			HComm.APPLState0 = APPLIdle;					// ͨѶ��1APPL����״̬
			Comm.TX_Delay0 = 0;				
			Comm.BTime0 = 0;								// �ַ��䶨ʱ
			Comm.RecTmr0 = 0;
			Comm.Run0 &= ~F_ComOK;
			break;	
		case 1:												//			
			fnUSART_IRCom_Init(Baut9600);
			Comm.IECState1 = IECIdle;								//mode E
			Comm.MACState1 = NDMState;
			Comm.LLCState1 = LLCIdle;
			Comm.LastRecCommand1 = 0x00;
			HComm.APPLState1 = APPLIdle;					// ͨѶ��1APPL����״̬
			Comm.TX_Delay1 = 0;				
			Comm.BTime1 = 0;								// �ַ��䶨ʱ
			Comm.RecTmr1 = 0;
			Comm.Run1 &= ~F_ComOK;

			break;	
		case 2:												//uart3,485ͨ��1
			fnUSART_RS485_1_Init(Baut57600);
			Comm.IECState2 = IECModeHDLC;
			Comm.MACState2 = NDMState;
			Comm.LLCState2 = LLCIdle;
			Comm.LastRecCommand2 = 0x00;
			HComm.APPLState2 = APPLIdle;					// ͨѶ��1APPL����״̬
			Comm.TX_Delay2 = 0;				
			Comm.BTime2 = 0;								// �ַ��䶨ʱ
			Comm.RecTmr2 = 0;
			Comm.Run2 &= ~F_ComOK;
			HT_GPIOE->PTSET |= SW485_1;
			break;
		default: break;
	}
	//CommInit( COM_No );
	DLMS_CommInit( COM_No );
}	

unsigned short MP_FCS(unsigned short fcs,unsigned short c)
{
	unsigned short FS1,FS2,FS3,FS;

	FS1=(fcs)^(c);
	FS2=FS1&0x00FF;
	FS3=MP_Fcstab[FS2];
	FS=(fcs>>8)^FS3;

	return FS;
}

unsigned short GetCheckCode_16BitFcs(unsigned char *pucData,unsigned short usDataLen)
{
	unsigned short crc=0;

//	crc = FCS_START;
	crc = 0xffff;
	while( usDataLen-- > 0 )
		crc = MP_FCS(crc,*pucData++);
	crc ^= 0xFFFF;

	return crc;
}

//unsigned short GetFrameLength( unsigned char* ComBuf )
//{
//	unsigned short Len;
//		
////	Len = *(ComBuf+1) & 0x1F;
//	Len = *(ComBuf+1) & 0x07;
//	Len *= 256;
//	Len += *(ComBuf+2);
//    
//    return Len;
//}	
/*
void FillHCS_FCS_Flag( unsigned char* ComBuf )
{
	unsigned short usCheckCode=0;
	unsigned short usDataLen=0;

	usDataLen = 10;
	usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );
	
	usDataLen = GetFrameLength( ComBuf );

	if( usDataLen >= 12 ) 
	{
		usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
		RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );
	}	
	*(ComBuf+usDataLen+1) = 0x7E;
}
*/

//unsigned short GetMeterAddress( unsigned char* ComBuf, unsigned char* ClientAddr, unsigned short* ServerAddrHigh, unsigned short* ServerAddrLow )
//{
//	short i;
//	unsigned short Len=0;
//	unsigned short SAPAddr;
//	unsigned short DevAddr;
//	
//	for( i=3;i<7;i++ )
//	{
//		Len++;
//		if(( *(ComBuf+i) & 0x01 ) != 0 ) break;			
//	}	
//	
////	if( Len == 2 )
////	{
////		if(( *(ComBuf+5) & 0x01 ) == 0 ) return 1;		// �ͻ�����ַ��Ϊ1�ֽ�
////		else *ClientAddr = *(ComBuf+5) / 2;
////		
////		*ServerAddrHigh = *(ComBuf+3);
////		*ServerAddrHigh /= 2; 	
////		*ServerAddrLow = *(ComBuf+4);
////		*ServerAddrLow /= 2; 	
////	}
////	else 
//	if( Len == 4 )
//	{
//		if(( *(ComBuf+7) & 0x01 ) == 0 ) return 1;		// �ͻ�����ַ��Ϊ1�ֽ�
//		else *ClientAddr = *(ComBuf+7) / 2;
//
//		*ServerAddrHigh = *(ComBuf+3);
//		*ServerAddrHigh *= 64;
//		*ServerAddrHigh += (*(ComBuf+4)/2);
//		*ServerAddrLow = *(ComBuf+5);
//		*ServerAddrLow *= 64; 	
//		*ServerAddrLow += (*(ComBuf+6)/2);
//
//		E2P_RData( (unsigned char*)&SAPAddr, ESAP, 2 );					//HDLC��ַ(3)				unsigned16	0x0010~0x3ffd:���ĩ4λ��1000������1234+1000=2234=0x08BA
//		E2P_RData( (unsigned char*)&DevAddr, HDLC_device_address, 2 );	//��������ַ��ϢSAP��ַ(3)	0x0001
////		if( SAPAddr != 0x0001 ) SAPAddr = 0x0001;		//Test
////		if( DevAddr != 9743 ) DevAddr = 9743;			//Test
//		
////		if(( *ServerAddrHigh != 0x0001 )||( *ServerAddrLow != DevAddr )) return 1;
//		if((PIN_SETEN & P_SETEN ) == 0 ) return 0;
////		if(( *ServerAddrHigh != SAPAddr )||( *ServerAddrLow != DevAddr )) return 1;
//		if((( *ServerAddrHigh != SAPAddr )&&( *ServerAddrHigh != 0x3FFF ))||(( *ServerAddrLow != DevAddr )&&( *ServerAddrLow != 0x3FFF ))) return 1;		//13.03.15
//
//	}
//	else return 1;										//��������ַ���Ȳ���
//	
//	return 0;	 	
//	
//}	

void Put16BitFcs( unsigned char* Source, unsigned char* Dest, unsigned short Len )
{
	unsigned short usCheckCode=0;
	
	usCheckCode=GetCheckCode_16BitFcs( Source,(unsigned short)(Len-2) );
	RAM_Write( Dest, (unsigned char*)&usCheckCode, 2 );
}	 

void ClrRSNumber( unsigned char* ComBuf )
{
	unsigned short COM_No;
	
	COM_No = GetCOM_No(ComBuf);
	
	*CommPara[COM_No].TxdFrameNo = 0;
	*CommPara[COM_No].RxdFrameNo = 0;
}	

void ClrRSBuffPara( unsigned char* ComBuf )
{
	unsigned short COM_No;
	
	COM_No = GetCOM_No(ComBuf);
	
	*CommPara[COM_No].RecDataPtr = 0;
	*CommPara[COM_No].SendDataPtr = 0;
	*CommPara[COM_No].SendByteSum = 0;
//	RAM_Fill( CommPara[COM_No].Conformance, 3);
	RAM_Fill( CommPara[COM_No].Conformance, 4);								//���һ���Կ��LLS��ȫ״̬
	RAM_Fill( CommPara[COM_No].SubsBlockPara, 20);
	
}	
unsigned short GetInformationField( unsigned char* Dest )
{
	*Dest = 0x81;			//��ʽ��ʶ��
	*(Dest+1) = 0x80;		//���ʶ��
	*(Dest+2) = 0x12;		//�鳤(18�ֽ�)
	*(Dest+3) = 0x05;		//������ʶ��(�����Ϣ�ֶγ��ȣ�����)
	*(Dest+4) = 0x01;		//�������ȣ�1�ֽڣ�
	*(Dest+5) = 0x80;		//����ֵ
	*(Dest+6) = 0x06;		//������ʶ��(�����Ϣ�ֶγ��ȣ�����)
	*(Dest+7) = 0x01;		//�������ȣ�1�ֽڣ�
	*(Dest+8) = 0x80;		//����ֵ
	*(Dest+9) = 0x07;		//������ʶ��(���ڴ�С,����)
	*(Dest+10) = 0x04;		//�������ȣ�4�ֽڣ�
	*(Dest+11) = 0x00;		//����ֵ(ֵ���ֽ�)
	*(Dest+12) = 0x00;		//����ֵ
	*(Dest+13) = 0x00;		//����ֵ
	*(Dest+14) = 0x01;		//����ֵ(ֵ���ֽ�)
	*(Dest+15) = 0x08;		//������ʶ��(���ڴ�С, ����)
	*(Dest+16) = 0x04;		//��������
	*(Dest+17) = 0x00;		//����ֵ(ֵ���ֽ�)
	*(Dest+18) = 0x00;		//����ֵ
	*(Dest+19) = 0x00;		//����ֵ
	*(Dest+20) = 0x01;		//����ֵ(ֵ���ֽ�)
	
	return 21;
/*	
    *Dest = 0x81;			//��ʽ��ʶ��
	*(Dest+1) = 0x80;		//���ʶ��
	*(Dest+2) = 0x14;		//�鳤(20�ֽ�)
	*(Dest+3) = 0x05;		//������ʶ��(�����Ϣ�ֶγ��ȣ�����)
	*(Dest+4) = 0x02;		//�������ȣ�1�ֽڣ�
	*(Dest+5) = 0x03;		//����ֵ	
	*(Dest+6) = 0x00;		//����ֵ
	*(Dest+7) = 0x06;		//������ʶ��(�����Ϣ�ֶγ��ȣ�����)
	*(Dest+8) = 0x02;		//�������ȣ�1�ֽڣ�
	*(Dest+9) = 0x03;		//����ֵ
	*(Dest+10) = 0x00;
	*(Dest+11) = 0x07;		//������ʶ��(���ڴ�С,����)
	*(Dest+12) = 0x04;		//�������ȣ�4�ֽڣ�
	*(Dest+13) = 0x00;		//����ֵ(ֵ���ֽ�)
	*(Dest+14) = 0x00;		//����ֵ
	*(Dest+15) = 0x00;		//����ֵ
	*(Dest+16) = 0x01;		//����ֵ(ֵ���ֽ�)
	*(Dest+17) = 0x08;		//������ʶ��(���ڴ�С, ����)
	*(Dest+18) = 0x04;		//��������
	*(Dest+19) = 0x00;		//����ֵ(ֵ���ֽ�)
	*(Dest+20) = 0x00;		//����ֵ
	*(Dest+21) = 0x00;		//����ֵ
	*(Dest+22) = 0x01;		//����ֵ(ֵ���ֽ�)

return 23;
*/
}
//����֡ͷ
//FrameLen:֡����
//SegSubFlag: �ֶα�־ 0�����һ֡��1�����к���֡
//ClientAddr: �ͻ�����ַ��Ŀ�ĵ�ַ
void CreatFrameHead( unsigned char* ComBuf, unsigned char ClientAddr, unsigned short FrameLen, unsigned char SegmentFlag )
{
	unsigned short Temp;
//	unsigned char Buff[4];
	
//	RAM_Write( (unsigned char*)&Buff, ComBuf+HDLC_SClientAddr, 4 );
	*ComBuf = 0x7E;
//	*(ComBuf+1) = FrameLen / 128;
	*(ComBuf+1) = FrameLen / 256;
	*(ComBuf+1) &= 0x07;						//ȡ���ȵĸ�3BIT	
	*(ComBuf+1) |= 0xA0;						//֡����
	if( SegmentFlag != 0 ) *(ComBuf+1) |= 0x08; //�к���֡
//	*(ComBuf+2) = FrameLen % 128;				//ȡ���ȵĵ��ֽ�	
	*(ComBuf+2) = FrameLen % 256;				//ȡ���ȵĵ��ֽ�	
	*(ComBuf+3) = ClientAddr;					
//	RAM_Write( ComBuf+4, (unsigned char*)&Buff, 4 );

	E2P_RData( (unsigned char*)&Temp, ESAP, 2 );					//��������ַ��ϢSAP��ַ(3)	0x0001
	*(ComBuf+4) = Temp / 128;					//ÿ�ֽڵ����λΪ0��ʾ�����ֽ���ȻΪ��ַ��Ϊ1�������ַ����
	*(ComBuf+4) *= 2;
	*(ComBuf+5) = Temp % 128;					//ÿ�ֽ�ֻ����7BIT��Ч��ַ
	*(ComBuf+5) *= 2;
//	*(ComBuf+5) |= 0x01;						//������ַ����

//	*(ComBuf+4) = 0x00;
//	*(ComBuf+5) = 0x02; 						//�߶�HDLC��ַ����1
	E2P_RData( (unsigned char*)&Temp, HDLC_device_address, 2 ); //HDLC��ַ(3)				unsigned16	0x0010~0x3ffd:���ĩ4λ��1000������1234+1000=2234=0x08BA
//	Temp = 9743;
	*(ComBuf+6) = Temp / 128;					//ÿ�ֽڵ����λΪ0��ʾ�����ֽ���ȻΪ��ַ��Ϊ1�������ַ����
	*(ComBuf+6) *= 2;
	*(ComBuf+7) = Temp % 128;					//ÿ�ֽ�ֻ����7BIT��Ч��ַ
	*(ComBuf+7) *= 2;
	*(ComBuf+7) |= 0x01;						//������ַ����
}
//����UI-RR֡
void CreatUIRRFrame( unsigned char* ComBuf )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
	Len = 10;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�FCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_UI;
 	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	

//����UA֡
void CreatUAFrame( unsigned char* ComBuf )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
	Len = GetInformationField( ComBuf+HDLC_DataPtr );
	Len += 12;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�HCS+2�ֽ�FCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_UA;
 	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	

//����DM֡
void CreatDMFrame( unsigned char* ComBuf )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
//	Len = GetInformationField( ComBuf+HDLC_DataPtr );
//	Len += 12;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�HCS+2�ֽ�FCS
	Len = 10;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�HCS+2�ֽ�FCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_DM;
 	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	
//����RR֡
void CreatRRFrame( unsigned char* ComBuf, unsigned char RxdFrameNo )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
	Len = 10;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�FCS
	*(ComBuf+HDLC_CtlPtr) = RxdFrameNo;
	*(ComBuf+HDLC_CtlPtr) *= 32;
	*(ComBuf+HDLC_CtlPtr) |= HDLC_RR;
	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	

//����I֡
void CreatIFrame( unsigned char* ComBuf, unsigned char RxdFrameNo, unsigned char TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag )
{
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
//	FrameLen += 10; 								//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�FCS
	FrameLen += 12; 								//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�FCS+2�ֽ�HCS
	*(ComBuf+HDLC_CtlPtr) = RxdFrameNo;
	*(ComBuf+HDLC_CtlPtr) *= 32;
	*(ComBuf+HDLC_CtlPtr) += TxdFrameNo*2;
	*(ComBuf+HDLC_CtlPtr) |= HDLC_I;
	CreatFrameHead( ComBuf, ClientAddr, FrameLen, SegmentFlag );
}
//����UI֡
void CreatUIFrame( unsigned char* ComBuf, unsigned short FrameLen, unsigned char SegmentFlag )
{
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
//	FrameLen += 10;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�FCS
	FrameLen += 12;									//2�ֽ�֡��ʽ�ͳ���+1�ֽ�Ŀ�ĵ�ַ+4�ֽ�Դ��ַ+1�ֽڿ����ֽ�+2�ֽ�FCS+2�ֽ�HCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_UI;
 	CreatFrameHead( ComBuf, ClientAddr, FrameLen, SegmentFlag );
}	

void HDLC_RNRFrameProcess( unsigned char* ComBuf )
{
     return; 
}  
/*
void HDLC_Ctrl(unsigned char* ComBuf)
{
	unsigned short COM_No;
//  	unsigned char Temp;
//  	unsigned char Temp2;
//  	unsigned short ComID;
//	unsigned char* ComRecTmr;
//	unsigned short* BTime;
	unsigned char* MACState;
//	unsigned char* RRRFrameNo;
//	unsigned char* SSSFrameNo;
	unsigned char SControlByte;
	unsigned char ControlByte;
//	unsigned char RespondCtlByte;
//	unsigned char RecFrameNo;
//	unsigned char SendFrameNo;

	COM_No = GetCOM_No(ComBuf);

	MACState = CommPara[COM_No].MACState;
	
//    ComRecTmr = CommPara[GetCOM_No(ComBuf)].RecTmr;
//	BTime = CommPara[GetCOM_No(ComBuf)].BTime;

	SControlByte = *(ComBuf+HDLC_CtlPtr);
	
	ControlByte = GetControlByte( SControlByte );
	if( *MACState == NDMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;			//����NRMģʽ
				ClrRSNumber( ComBuf );			//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
			case HDLC_RR:				
			case HDLC_RNR:		
			case HDLC_DISC:				
//			case HDLC_UI:
				ClrRSNumber( ComBuf );			//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatDMFrame( ComBuf );
				break;		
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;
			default: break;
					
		}
	}
	else if( *MACState == NRMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;			//�ظ�����NRMģʽ	
				ClrRSNumber( ComBuf );			//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
				HDLC_InformationFrameProcess( ComBuf );			
				break;
			case HDLC_RR:				
				HDLC_RRFrameProcess( ComBuf );			
				break;
			case HDLC_RNR:		
				HDLC_RNRFrameProcess( ComBuf );			
				break;
			case HDLC_DISC:				
				*MACState = NDMState;
				ClrRSNumber( ComBuf );			//������ͽ������к�
				ClrRSBuffPara( ComBuf );
//				CreatDMFrame( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;			
			default: break;
		}		
	}	
}
*/
/*
void HDLC_UIFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
  	unsigned short Length;
//	unsigned char* RxdFrameNo;
//	unsigned char pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned short* SendDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char ControlByte;
	unsigned short* SendByteSum;	// 485��LLC��������

	COM_No = GetCOM_No(ComBuf);	
	
//	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
//	pSendFrameNo /= 2;
		
	ControlByte = *(ComBuf+HDLC_CtlPtr);
//	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	SendDataPtr = CommPara[COM_No].SendDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
	LLCState = CommPara[COM_No].LLCState;
	SendByteSum = CommPara[COM_No].SendByteSum;
//	if( *RxdFrameNo != pSendFrameNo ) 
//	{
//		CreatRRFrame( ComBuf, *RxdFrameNo );				//�յ���֡���ڴ���֡��Ų�һ�£������ط�
//	}	
//	else
//	{
//		*RxdFrameNo += 1;
//		*RxdFrameNo &= 0x07;								//����֡��ż�1
		
		Length = GetFrameLength( ComBuf );
//		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 )||((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) )			//CTT���������PDU���෢��һ���ֽ�	//UI֡��֧�ַ�֡
		{
			//����receive_pdu_size��֡	
			_NOP();
			*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
			*SendDataPtr = 0;
			if( ControlByte == HDLC_UI )
			{ 
				*LLCState = UISendNextState;
				LLCStateCycle( COM_No );	
			}	
		}
		else
		{
			Length -= 12;
			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
			*RecDataPtr += Length;
//			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
//			{
//				*LLCState = WaitRecNextState;
//				CreatRRFrame( ComBuf, *RxdFrameNo );			//�к���֡��������һ֡
//			}	
//			else
//			{
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC֡��ʽ����
				{
					*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
				}	
				else
				{
					RecBuff += 3;							//����LLC֡ͷ 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//��������֡����
				}
				*SendDataPtr = 0;
				if( ControlByte == HDLC_UI )
				{
					*LLCState = UISendNextState;
					LLCStateCycle( COM_No );	
				}
				else
				{
					*SendByteSum = 0;	
					*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
				}	
//			}	
		}		
//	}	
	if( ControlByte == HDLC_UI_N ) *ComBuf = 0;		//��Ӧ����
}				
*/
/*
void HDLC_InformationFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
  	unsigned short Length;
	unsigned char* RxdFrameNo;
	unsigned char pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
//	unsigned char* LLCState;

	COM_No = GetCOM_No(ComBuf);	
	
	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
	pSendFrameNo /= 2;
		
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
//	LLCState = CommPara[COM_No].LLCState;

	if( *RxdFrameNo != pSendFrameNo ) 
	{
		CreatRRFrame( ComBuf, *RxdFrameNo );				//�յ���֡���ڴ���֡��Ų�һ�£������ط�
	}	
	else
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x07;								//����֡��ż�1
		
		Length = GetFrameLength( ComBuf );
//		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 ) )			//CTT���������PDU���෢��һ���ֽ�
		{
			//����receive_pdu_size��֡	
			_NOP();
			*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
			*CommPara[COM_No].SendDataPtr = 0;
			*CommPara[COM_No].LLCState = SendNextState;
			LLCStateCycle( COM_No );	
		}
		else
		{
			Length -= 12;
			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );			//�к���֡��������һ֡
			}	
			else
			{
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC֡��ʽ����
				{
					*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
				}	
				else
				{
					RecBuff += 3;							//����LLC֡ͷ 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*CommPara[COM_No].SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//��������֡����
				}
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				LLCStateCycle( COM_No );	
			}	
		}		
	}	
}				
*/

/*
void HDLC_RRFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
// 	unsigned short Length;
	unsigned char* RxdFrameNo;
//	unsigned char pSendFrameNo;
//	unsigned short* RecDataPtr;
//	unsigned char* RecBuff;
//	unsigned char* SendBuff;
	unsigned char* LLCState;

	COM_No = GetCOM_No(ComBuf);	
	
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	LLCState = CommPara[COM_No].LLCState;

//	*RxdFrameNo += 1;
//	*RxdFrameNo &= 0x07;									//����֡��ż�1
	
	if( *LLCState == LLCIdle )
	{
		*RxdFrameNo = 0;
		CreatRRFrame( ComBuf, *RxdFrameNo );				//���û��׼���ϴ�֡
	}
	else if( *LLCState == WaitRecNextState )
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x07;								//����֡��ż�1
		CreatRRFrame( ComBuf, *RxdFrameNo );				//����վ�·���һ֡
	}
	else if( *LLCState == SendNextState )
	{
		*RxdFrameNo &= 0x07;								//����֡���ά�ֲ���
		LLCStateCycle( COM_No );							//����ϴ�֡
	}		 
}	
*/

/*
void LLCStateCycle( unsigned short COM_No )
{
	unsigned char* RxdFrameNo;
	unsigned char* TxdFrameNo;
	unsigned short* SendDataPtr;
	unsigned short* SendSum;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char* ComBuf;
	unsigned char SegmentFlag;
	unsigned short FrameLen;
	
    LLCState = CommPara[COM_No].LLCState;
    
	switch( *LLCState )	
	{
		case LLCIdle:				// LLC����
			break;
		case WaitRecNextState:		// LLC�ȴ�
			break;
		case SendNextState:			// ����δ��
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			if( (*SendSum) > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = SendNextState;
			}	
			else 
			{
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			*SendDataPtr += FrameLen;
			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			*TxdFrameNo += 1;
			*TxdFrameNo &= 0x07;
			break;
		case UISendNextState:			// ����δ��
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
//			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
//			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			if( (*SendSum) > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = UISendNextState;
			}	
			else 
			{
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			*SendDataPtr += FrameLen;
//			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			CreatUIFrame( ComBuf, FrameLen, SegmentFlag );
//			*TxdFrameNo += 1;
//			*TxdFrameNo &= 0x07;
			break;
		default:
		  	*LLCState = LLCIdle;
		  	break;	
	}	
}
*/
//=================================================================================================

unsigned short GetFrameLength( unsigned char* ComBuf )
{
	unsigned short Len;
   
	Len = *(ComBuf+1) & 0x07;
	Len *= 256;
	Len += *(ComBuf+2);
    return Len;
}	

void FillHCS_FCS_Flag( unsigned char* ComBuf )
{
	unsigned short usCheckCode=0;
	unsigned short usDataLen=0;

	usDataLen = 10;
	usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );

	usDataLen = GetFrameLength( ComBuf );

	if( usDataLen >= 12 ) 
	{
		usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
		RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );
	}	
	*(ComBuf+usDataLen+1) = 0x7E;
}

unsigned short DLMS_GetMeterAddress( unsigned char* ComBuf, unsigned char* ClientAddr, unsigned short* ServerAddrHigh, unsigned short* ServerAddrLow )
{
	short i;
	unsigned short Len=0;
	unsigned short SAPAddr;
	unsigned short DevAddr;
	for( i=3;i<7;i++ )
	{
		Len++;
		if(( *(ComBuf+i) & 0x01 ) != 0 ) break;			
	}	
	
	if( Len == 4 )
	{
		if(( *(ComBuf+7) & 0x01 ) == 0 ) return 1;		// �ͻ�����ַ��Ϊ1�ֽ�
		else *ClientAddr = *(ComBuf+7) / 2;

		*ServerAddrHigh = *(ComBuf+3);
		*ServerAddrHigh *= 64;
		*ServerAddrHigh += (*(ComBuf+4)/2);
		*ServerAddrLow = *(ComBuf+5);
		*ServerAddrLow *= 64; 	
		*ServerAddrLow += (*(ComBuf+6)/2);

		E2P_RData( (unsigned char*)&SAPAddr, ESAP, 2 );					//HDLC��ַ(3)				unsigned16	0x0010~0x3ffd:���ĩ4λ��1000������1234+1000=2234=0x08BA
		E2P_RData( (unsigned char*)&DevAddr, HDLC_device_address, 2 );	//��������ַ��ϢSAP��ַ(3)	0x0001
		
		if((PIN_SETEN & P_SETEN ) == 0 ) 	return 0;		
		if((( *ServerAddrHigh != SAPAddr )&&( *ServerAddrHigh != 0x3FFF ))||(( *ServerAddrLow != DevAddr )&&( *ServerAddrLow != 0x3FFF ))) 
			return 1;
	}
	else 
		return 1;										//��������ַ���Ȳ���..
	
	return 0;	 	
	
}

unsigned char DLMS_HDLC_CheckFrame( unsigned char COM_No,unsigned char* ComBuf )
{//֡У��
	unsigned char ClientAddr;
	unsigned short usCheckCode=0;
	unsigned short usDataLen=0;
	unsigned short ServerAddrHigh;
	unsigned short ServerAddrLow;
	unsigned short Check=0;

	if(( *(ComBuf+1) & 0xf0 ) != 0xA0 )	
		return 1;				//MAC�Ӳ���HDLC֡��ʽ����3
	if( DLMS_GetMeterAddress( ComBuf, (unsigned char*)&ClientAddr, (unsigned short*)&ServerAddrHigh, (unsigned short*)&ServerAddrLow ) != 0 )
		return 1;
	
	HComm.ClientAddr[COM_No] = ClientAddr;
	
	usDataLen = 10;
	usCheckCode=GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( (unsigned char*)&Check, ComBuf+usDataLen-1, 2 );
	if( usCheckCode != Check )
		return 1;
	
	usDataLen = GetFrameLength( ComBuf );

	if( usDataLen == 10 ) return 0;
	if( usDataLen < 12 ) return 1;
	
	usCheckCode=GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( (unsigned char*)&Check, ComBuf+usDataLen-1, 2 );
	if( usCheckCode != Check ) 
		return 1;                        

	return 0;
}
unsigned char GetControlByte( unsigned char SControlByte )
{
	unsigned char ControlByte;
	
	ControlByte = SControlByte;
	
	if(( ControlByte & 0x11 ) == 0x10 ) ControlByte = HDLC_I;				//I ֡
	else if(( ControlByte & 0x1f ) == 0x11 ) ControlByte = HDLC_RR;			//RR ֡
	else if(( ControlByte & 0x1f ) == 0x15 ) ControlByte = HDLC_RNR;		//RNR ֡
	else
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
			case HDLC_DISC:				
			case HDLC_UI:
			case HDLC_UI_N:
				break;			
			default: 
				ControlByte = HDLC_FRMR;		
				break;		
		}	
	}	
	return ControlByte;
}
unsigned short GetHDLCParaValue( unsigned char* Source, unsigned short* Len, unsigned long* Value )
{
	short i;
	unsigned short ErrorByte=0;
	unsigned short Length=0;
	unsigned char* Data;
	
	*Value=0;
	Length = *(Source+1);
	if( Length > 4 ) return 1;			//ÿ�����ݳ������4�ֽ�
	switch( *Source )
	{
		case 0x05:
		case 0x06:
			break;
		case 0x07:
			if(( Length != 0 )&&( Length != 4 )) ErrorByte = 1;		//���մ��ڳ����ֽ����̶�Ϊ4
			break;
		case 0x08:
			if(( Length != 0 )&&( Length != 4 )) ErrorByte = 1;		//���ʹ��ڳ����ֽ����̶�Ϊ4
			break;
    default: 
			ErrorByte = 1;
			break;					
	}	

	*Len = Length+2;					//�����ʶ���ͳ����ֽڱ���		
	if( Length == 0 ) return 0;			

	if( ErrorByte == 0 )
	{	
		Data = (unsigned char*)Value; 
		for( i=0;i<Length;i++ )
		{
			*(Data+i) = *(Source+2+Length-i-1);
		}	
	}
	
	return ErrorByte;
}

unsigned char JudgeHDLCParaInformationField( unsigned char* Source, short Length )
{
	unsigned long Value;
	unsigned long HDLC[4];
	unsigned char* Ptr;
	unsigned short Len;

	HDLC[0] =128;
	HDLC[1] =128;
	HDLC[2] =1;
	HDLC[3] =1;

	if( Length == 0 ) return 0;					//SNRM����Ϣ֡���ݣ�ʹ��ȱʡֵ	
	Length -= 2;								//ȥ��FCS	
	if( Length < 3 ) return 1;					//����һ������
	if( *Source != 0x81 ) return 1;				//��ʽ��ʶ����
	if( *(Source+1) != 0x80 ) return 1;			//���ʶ����
	if( *(Source+2) != (Length-3) ) return 1;	//�鳤�ȴ�
	Ptr = Source+3;
	Length -= 3;
	while( Length )
	{
		if( GetHDLCParaValue( Ptr, (unsigned short*)&Len, (unsigned long*)&Value ) != 0 ) return 1;
		else
		{
			if( Length >= Len ) Length -= Len;
			else return 1;
			if( Len > 2 )								//SNRM֡���ж�Ӧ����
			{
				switch( *Ptr )
				{
					case 5: HDLC[0] = Value; break;		//���ճ���
					case 6: HDLC[1] = Value; break;		//���ͳ���
					case 7: HDLC[2] = Value; break;		//���մ�����
					case 8: HDLC[3] = Value; break;		//���ʹ�����
					default: break;
				}									
			}	
			Ptr += Len;
		}			
    }
	if( HDLC[0] > 128 ) HDLC[0] = 128;					//���������������ֽ�Ϊ128�ֽ�
	if( HDLC[1] > 128 ) HDLC[1] = 128;					//��������������ֽ�Ϊ128�ֽ�

	return 0;
}

unsigned char JudgeHDLC_FRMR( unsigned char* ComBuf )
{
	short COM_No;
    unsigned short Length;
	unsigned char Buff[6];
	unsigned char Buff2[4];
	unsigned char* MACState;
	unsigned char* Ptr;
//	unsigned char* sRxdCnt;
//	unsigned char* sSendCnt;
	unsigned char pRxdFrameNo;
	unsigned char ErrState=0;
//	unsigned char Temp;
	unsigned char ControlByte;
    unsigned char TxdFrameNo;
	unsigned char RxdFrameNo;
    
	COM_No = GetCOM_No(ComBuf);

	MACState = CommPara[COM_No].MACState;
	
	Ptr = Buff;
	
	Length = GetFrameLength( ComBuf );
	//if( Length > 142 ) ErrState |= 0x04;									//��Ϣ֡����
	if(COM_No==0)
		{
			if( Length > 350 )
				ErrState |= 0x04;									//��Ϣ֡����
		}
		else
		{
				///	if( Length > 142 ) 
				if( Length > 140 )	//CTT2.6 ����141�ֽ� 2016-8-22-fire
						ErrState |= 0x04;									//��Ϣ֡����
		}
		
//	if( GetControlByte( *(ComBuf+HDLC_CtlPtr) ) == HDLC_FRMR ) ErrState |= 0x01;	//�����ֽڷǷ�

	ControlByte = GetControlByte( *(ComBuf+HDLC_CtlPtr) );
	
	if( ControlByte == HDLC_UI_N ) __NOP();			//�ޱ��֡
	
	if( *MACState == NDMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
			case HDLC_UI:
			case HDLC_UI_N:
			case HDLC_DISC:				
			case HDLC_I:	
			case HDLC_RR:				
			case HDLC_RNR:		
				break;
			default: 
				ControlByte = HDLC_FRMR;									//NDMģʽ�£�ֻ��������3������
				break;
		}
	}

	*CommPara[COM_No].LastRecCommand = ControlByte;
	
	if(( ControlByte == HDLC_SNRM )&&( ErrState == 0 ))						//��Ϣ֡���ȺϷ�������ж�SNRM����
	{
		if( JudgeHDLCParaInformationField( ComBuf+HDLC_DataPtr, Length-10 ) != 0 )
			ErrState |= 0x01;	//SNRM�������ݷǷ�	
	}	

	if( ControlByte == HDLC_FRMR ) 
		ErrState |= 0x01;						//�����ֽڷǷ�
	
	TxdFrameNo = *CommPara[COM_No].TxdFrameNo;
	RxdFrameNo = *CommPara[COM_No].RxdFrameNo;

	if(( ControlByte == HDLC_UI )||( ControlByte == HDLC_UI_N ))			//�ޱ��֡
	{
		if((HComm.APPL_A8_BUSY[COM_No]&0x0f)==0x05)
		{
		 	if( ControlByte == HDLC_UI )	
				ErrState |= 0x07;						//��������ŷ�֡״̬������������ŵĲ���..		 				 	
		}
		TxdFrameNo = 0;
		RxdFrameNo = 0;
	}	

	if(( ControlByte == HDLC_I )||( ControlByte == HDLC_RR )||( ControlByte == HDLC_RNR ))	//����ŵ�֡
	{	
		pRxdFrameNo = *(ComBuf+HDLC_CtlPtr) & 0xE0;
		pRxdFrameNo /= 32;
			
		if( pRxdFrameNo != TxdFrameNo ) 
	//if(( pRxdFrameNo != TxdFrameNo )&&(SM.Module_Type!=Module_GPRS)) //gprsģʽʱ���ж�֡���	
			ErrState |= 0x08;							//����վ�ķ��ͽ������кŲ���������վ�������յ�֡��źʹ�վ׼�����͵�֡��Ų�һ�¡�
		if((HComm.APPL_A8_BUSY[COM_No]&0x0f)==0x0a)
		{
		 	ErrState |= 0x07;						//��������ŷ�֡״̬������������ŵĲ���..
		}
//		else{
//			HComm.APPL_A8_BUSY[COM_No] = 0;					//����..
//		}					
	}
	
	if( ErrState != 0 )														//֡��ʽ�쳣
	{
		Buff2[0] = *(ComBuf+HDLC_CtlPtr);		
		Buff2[1] = TxdFrameNo * 2 + RxdFrameNo * 32 + (Buff2[0]&0x10);		
		Buff2[2] = ErrState;		

		*ComBuf = 0x7E;
		*(ComBuf+1) = 0xA0;
		*(ComBuf+2) = 15;
		RAM_Write( Ptr, ComBuf+3, 4 );
		*(ComBuf+3) = *(ComBuf+7);
		RAM_Write( ComBuf+4, Ptr, 4 );
		
		*(ComBuf+HDLC_CtlPtr) = HDLC_FRMR;
		RAM_Write( ComBuf+HDLC_DataPtr, (unsigned char*)&Buff2, 3 );		//�쳣֡��Ϣ����
		
		HComm.APPL_A8_BUSY[COM_No] = 0;
	}
	
	return ErrState;
			
}
void HDLC_UIFrameProcess( unsigned char* ComBuf )
{
#if 0 
		unsigned short COM_No;
  		unsigned short Length;
//	unsigned char* RxdFrameNo;
//	unsigned char pSendFrameNo;
		unsigned short* RecDataPtr;
		unsigned short* SendDataPtr;
		unsigned char* RecBuff;
		unsigned char* SendBuff;
		unsigned char* LLCState;
		unsigned char ControlByte;
		unsigned short* SendByteSum;	// 485��LLC��������
		
		unsigned short FrameLen,i;
		unsigned char * RepeatPtr;				
		unsigned long* BlockNumber;
        unsigned long  RdFc_number=0;

		COM_No = GetCOM_No(ComBuf);
		    	
		BlockNumber = CommPara[COM_No].BlockNumber;
    	RepeatPtr = HComm.RepeatBuff;
        
//	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
//	pSendFrameNo /= 2;
		
		ControlByte = *(ComBuf+HDLC_CtlPtr);
//	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
		RecDataPtr = CommPara[COM_No].RecDataPtr;
		SendDataPtr = CommPara[COM_No].SendDataPtr;
		RecBuff = CommPara[COM_No].RecDataBuf;
		SendBuff = CommPara[COM_No].SendDataBuf;	
		LLCState = CommPara[COM_No].LLCState;
		SendByteSum = CommPara[COM_No].SendByteSum;
//	if( *RxdFrameNo != pSendFrameNo ) 
//	{
//		CreatRRFrame( ComBuf, *RxdFrameNo );				//�յ���֡���ڴ���֡��Ų�һ�£������ط�
//	}	
//	else
//	{
//		*RxdFrameNo += 1;
//		*RxdFrameNo &= 0x07;								//����֡��ż�1
		
//	if(((HComm.APPL_A8_BUSY[COM_No]&0x0f)==0x05)||((HComm.APPL_A8_BUSY[COM_No]&0x80)!=0))
//	{
//		// 	if( ControlByte == HDLC_UI )	ErrState |= 0x01;						//��������ŷ�֡״̬������������ŵĲ���..		 	
//		// HComm.APPL_A8_BUSY[COM_No] = 0;		
//		*ComBuf = 0;		//��Ӧ����
//		//*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��		
//		return;
//	}
		
		Length = GetFrameLength( ComBuf );
		
		//����ȫ�֣����ܣ��жϣ����ܺ�ִ��ͬ�޼���״̬..
	//	Decrypt_OR_Encrypt(COM_No,ComBuf+HDLC_DataPtr,&Length,0);   //�жϽ������ͣ������ͱ�ʶ����ӽ�������,use_crypt:0--���룬1--����..
    //����FC����sky20160305..
     if(( crypt.glo_request[COM_No] == GLO_GET_Request )||( crypt.glo_request[COM_No] == GLO_SET_Request )||( crypt.glo_request[COM_No] == GLO_ACTION_Request ))   //ȫ�ּ���ģʽ..
    	{
    		//HComm.UK_FCRXCount++;
    		//�ж϶Դ�..ָ��Ҫ���ڽ���ָ��Ÿ���ͨ��..
    			RAM_Write( (unsigned char*)&RdFc_number ,&crypt.glo_FC[1],4 );
	  	 		SwapData( (unsigned char*)&RdFc_number, 4 );
	  	 		if(HComm.UK_FCRXCount>RdFc_number)
	  	 		{
						//����receive_pdu_size��֡	
						__NOP();
						*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
						*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
						*SendDataPtr = 0;
						if( ControlByte == HDLC_UI )
						{ 
								*LLCState = UISendNextState;
								LLCStateCycle( COM_No );	
						}		  	 			
	  	 		} 
    	}
    	else
    	{
       if(( crypt.glo_request[COM_No] == GLO_GET_Request )||( crypt.glo_request[COM_No] == GLO_SET_Request )||( crypt.glo_request[COM_No] == GLO_ACTION_Request ))   //ȫ�ּ���ģʽ..
  		 {
   				//�ж϶Դ�..ָ��Ҫ���ڽ���ָ��Ÿ���ͨ��..
    			RAM_Write( (unsigned char*)&RdFc_number ,&crypt.glo_FC[1],4 );
	  	 		SwapData( (unsigned char*)&RdFc_number, 4 );
	  	 		if(HComm.DK_FCRXCount>RdFc_number)
	  	 		{
						//����receive_pdu_size��֡	
						__NOP();
						*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
						*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
						*SendDataPtr = 0;
						if( ControlByte == HDLC_UI )
						{ 
								*LLCState = UISendNextState;
								LLCStateCycle( COM_No );	
						}		  	 			
	  	 		}   		 	
  		 }    		
    	}
    
		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
//		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 )||((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) )			//CTT���������PDU���෢��һ���ֽ�	//UI֡��֧�ַ�֡
		{
			//����receive_pdu_size��֡	
			__NOP();
			*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
			*SendDataPtr = 0;
			if( ControlByte == HDLC_UI )
			{ 
					*LLCState = UISendNextState;
					LLCStateCycle( COM_No );	
			}	
		}
		else
		{
			Length -= 12;
			if(HComm.APPL_A8_BUSY[COM_No] == 0x0A)
				{      //UI֡ʱ�ķ�֡����..
					if(( *(ComBuf+HDLC_DataPtr) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+1) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+2) == 0x00 ))
					{
						if(Length>3)
						{
							Length -=3;
							RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );
						}else{
							RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
						}
					}
					else
					{
	                 RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
	                }
				}else
				{				
			        RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
		        }
		  
			// RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				
				if( ControlByte == HDLC_UI_N ) *ComBuf = 0;		//��Ӧ����
				else 	CreatUIRRFrame( ComBuf );		//�к���֡��������һ֡				

				// �����жϣ�����Ƿ�֡�����򲻽��ܲ�����ͬ����Ĳ���  //2013.06.24..
				HComm.APPL_A8_BUSY[COM_No] = 0x0A;	//��֡����״̬..							
			}	
			else
			{
				HComm.APPL_A8_BUSY[COM_No] = 0x0;	//��֡����״̬..											
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC֡��ʽ����
				{
					*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
				}	
				else
				{
					RecBuff += 3;							//����LLC֡ͷ
					PushSetup.SendNumber = 0;
					if((ControlByte == HDLC_UI )&&(*RecDataPtr == 10 )&&( *RecBuff == 0xc0 )&&( *(RecBuff+1) == 0x02 )&&( *(RecBuff+3) == 0x00 )&&( *(RecBuff+4) == 0x00 ))			//���������ݿ�֡��ʽ�ж�..
							{								
									PushSetup.SendNumber =  (*(RecBuff+5))*256 + *(RecBuff+6);	
							}		
					//			//���Ӵ������£������ط��Ĺ���(�ж�BLOCK���ݿ�)(������������ز��źŲ��������ط�����)..
					if((PushSetup.SendNumber!=0)&&(*BlockNumber!=0))   
					{
							if((PushSetup.SendNumber +1 == *BlockNumber )&&(*(RepeatPtr+148)!= 0)) 
							{ 
									FrameLen = *(RepeatPtr+148);
									for(i=0;i<FrameLen;i++) *(ComBuf+i) = *(RepeatPtr+i) ;  //chongfa
									*CommPara[COM_No].RecDataPtr = 0;
		      				return;  
		      		}
		      		if(PushSetup.SendNumber != *BlockNumber )
		      		{
		      				*CommPara[COM_No].RecDataPtr = 0;
		      				return;  
		      		}
		     	}
//					*SendBuff = 0xE6;
//					*(SendBuff+1) = 0xE7;
//					*(SendBuff+2) = 0x00;
//					SendBuff += 3;

//								  //======================================		
								  
 					//add UI-datablock trans  sky20140519..
					// if((HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(*RecDataPtr <= 12 )&&(COM_No==1))
                                       // if((HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(COM_No==1))
					//if((*LLCState) == UIBlockSendNextState)
 				if(((*LLCState) == UIBlockSendNextState)&&(HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(*RecDataPtr == 10 ))
// 				if(((*LLCState) == UIBlockSendNextState)&&(HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(*RecDataPtr == 10 )&&(COM_No==0)) 
					{
							if( ControlByte == HDLC_UI )
							{
								//7E A0 16 FE FE FE FF 23 13 A1 20 E6 E6 00 c0 02 c1 00 00 00 01 73 7F 7E
								//if((*RecDataPtr <= 12 )||( *RecBuff != 0xc0 )||( *(RecBuff+1) != 0x02 )||( *(RecBuff+4) != 0x00 )||( *(RecBuff+5) != 0x00 ))			//���������ݿ�֡��ʽ����
									if((*RecDataPtr <= 10 )&&( *RecBuff == 0xc0 )&&( *(RecBuff+1) == 0x02 )&&( *(RecBuff+3) == 0x00 )&&( *(RecBuff+4) == 0x00 ))			//���������ݿ�֡��ʽ�ж�..
									{								
											*LLCState = UIBlockSendNextState;
									    PushSetup.SendNumber =  (*(RecBuff+5))*256 + *(RecBuff+6);									    									    
											LLCStateCycle( COM_No );
									}
									else
									{
											*SendBuff = 0xE6;
											*(SendBuff+1) = 0xE7;
											*(SendBuff+2) = 0x00;
											SendBuff += 3;										
											*SendByteSum = 3 + CreatExceptionResponseAPDU( SendBuff );
											*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
											*LLCState = UIBlockSendNextState;
											HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;
											LLCStateCycle( COM_No );
									}	
							}
							else
							{
									*SendByteSum = 0;	
									*RecDataPtr = 0;
								  HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;																						//��ʼ�����ջ���ָ��
							}
				 }
				 else	
				 {							
							*SendBuff = 0xE6;
							*(SendBuff+1) = 0xE7;
							*(SendBuff+2) = 0x00;
							SendBuff += 3;				 	
				 			*SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//��������֡����
				 //}
				 //}
							*SendDataPtr = 0;
							if( ControlByte == HDLC_UI )
							{
								if(COM_No==0) *LLCState = UIBlockSendNextState;
//								if(COM_No==1) *LLCState = UIBlockSendNextState; //test..
								else *LLCState = UISendNextState;
//								//test..
//								 *LLCState = UIBlockSendNextState;
								LLCStateCycle( COM_No );	
							}
							else
							{
								*SendByteSum = 0;	
								*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
							}
					}
				}	
			}	
	}		
//	}	
	if( ControlByte == HDLC_UI_N ) *ComBuf = 0;		//��Ӧ����
#endif     
}
void HDLC_InformationFrameProcess( unsigned char* ComBuf )
{
#if 0 
	unsigned short COM_No;
    unsigned short Length;
	unsigned char* RxdFrameNo;
	unsigned char pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
//	unsigned char* LLCState;
	unsigned char* SetBlockBuff;
	unsigned short* SetBlockPtr;
	

	COM_No = GetCOM_No(ComBuf);	
	
	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
	pSendFrameNo /= 2;
		
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
//	LLCState = CommPara[COM_No].LLCState;

	//��ʱstart..
	SetBlockBuff = CommPara[COM_No].SetBlockBuff;
	SetBlockPtr = CommPara[COM_No].SetBlockPtr;
	//��ʱend..

	if( *RxdFrameNo != pSendFrameNo ) 
	{
		CreatRRFrame( ComBuf, *RxdFrameNo );				//�յ���֡���ڴ���֡��Ų�һ�£������ط�
	}	
	else
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x07;								//����֡��ż�1
		
		Length = GetFrameLength( ComBuf );
		//����ȫ�֣����ܣ��жϣ����ܺ�ִ��ͬ�޼���״̬..
		Decrypt_OR_Encrypt(COM_No,ComBuf+HDLC_DataPtr,&Length,0);   //�жϽ������ͣ������ͱ�ʶ����ӽ�������,use_crypt:0--���룬1--����..

    //�Լ�����FC����sky20160306..
    if(( crypt.glo_request[COM_No] == GLO_GET_Request )||( crypt.glo_request[COM_No] == GLO_SET_Request )||( crypt.glo_request[COM_No] == GLO_ACTION_Request ))   //ȫ�ּ���ģʽ..
    {
        HComm.UK_FCRXCount++;
    }
              
		//��ʱSTART-���ӷ�֡���ݳ���512����sky20140711..
			 if(((HComm.APPL_A8_BUSY[COM_No] == 0x05)&&(( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) ))||(crypt.Special_A8[COM_No] == 0x06)){
			 		if(crypt.Special_A8[COM_No] != 0x06){
			 			*SetBlockPtr = 0;
			 			RAM_Write( SetBlockBuff,RecBuff,(*RecDataPtr));
			 		}
			 		crypt.Special_A8[COM_No] = 0x06;   			//����512��save to datablock buff..	
			 		
		if((Length < 12 )||(( *RecDataPtr + Length - 12 ) > (MAX_BLOCK_SIZE+1)))			//֡���ȳ���..
		{	
			//����receive_pdu_size��֡				
			__NOP();
			*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
			*SetBlockPtr = 0;
			*CommPara[COM_No].SendDataPtr = 0;
			*CommPara[COM_No].LLCState = SendNextState;
			LLCStateCycle( COM_No );
			crypt.Special_A8[COM_No] = 0;  	
		}
		else			
		{ 		
			Length -= 12;
			if(HComm.APPL_A8_BUSY[COM_No] == 0x05){						//�����֡�ķ�֡����..
				if(( *(ComBuf+HDLC_DataPtr) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+1) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+2) == 0x00 ))
				{
					if( Length>3 )
					{
						Length -=3;
						RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );
					}else{
								RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
							}
				}else{
                RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
             }
			}else{				
			        RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
		        }
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );		//�к���֡��������һ֡
				// �����жϣ�����Ƿ�֡�����򲻽��ܲ�����ͬ����Ĳ���  //2013.06.24..
				HComm.APPL_A8_BUSY[COM_No] = 0x05;	//��֡����״̬..			
			}	
			else
			{
				HComm.APPL_A8_BUSY[COM_No] = 0;			//�޷�֡����״̬..
				crypt.Special_A8[COM_No] = 0;  
				if((*RecDataPtr <= 3 )||( *SetBlockBuff != 0xE6 )||( *(SetBlockBuff+1) != 0xE6 )||( *(SetBlockBuff+2) != 0x00 ))			//LLC֡��ʽ����
				{
					*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
				}	
				else
				{
					SetBlockBuff += 3;							//����LLC֡ͷ					 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*CommPara[COM_No].SendByteSum = 3 + APPLProcess( COM_No, SetBlockBuff, SendBuff );		//��������֡����,Ҫ�����ֽ�������
				}
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				LLCStateCycle( COM_No );	
			}
		}	
			return;			 			
			 }
		//��ʱEND...
		
//		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 ) )			//CTT���������PDU���෢��һ���ֽ�
		{
			//����receive_pdu_size��֡	
			__NOP();
			*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
			*CommPara[COM_No].SendDataPtr = 0;
			*CommPara[COM_No].LLCState = SendNextState;
			LLCStateCycle( COM_No );
			crypt.Special_A8[COM_No] = 0;  	
		}
		else
		{
			Length -= 12;
			crypt.Special_A8[COM_No] = 0;
			if(HComm.APPL_A8_BUSY[COM_No] == 0x05){						//�����֡�ķ�֡����..
				if(( *(ComBuf+HDLC_DataPtr) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+1) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+2) == 0x00 ))
				{
					if(Length>3)
					{
						Length -=3;
						RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );
					}else{
								RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
							}
				}else{
                RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
             }
			}else{				
			        RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
		        }
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );		//�к���֡��������һ֡
				// �����жϣ�����Ƿ�֡�����򲻽��ܲ�����ͬ����Ĳ���  //2013.06.24..
				HComm.APPL_A8_BUSY[COM_No] = 0x05;	//��֡����״̬..			
			}	
			else
			{
				HComm.APPL_A8_BUSY[COM_No] = 0;			//��֡����״̬..
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC֡��ʽ����
				{
					*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//��ʼ�����ջ���ָ��
				}	
				else
				{
					RecBuff += 3;							//����LLC֡ͷ					 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*CommPara[COM_No].SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//��������֡����
				}
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				LLCStateCycle( COM_No );	
			}	
		}		
	}
#endif	
}

void HDLC_Ctrl(unsigned char* ComBuf)
	{
	unsigned short COM_No;
	unsigned char* MACState;
	unsigned char SControlByte;
	unsigned char ControlByte;
	COM_No = GetCOM_No(ComBuf);
/*	
///2016-8-26,ȥ�������������жϣ�ΪCTT����
//���ڽ���״̬�����¼����ͣ�����ʱ������skyblue201410..
	if(COM_No==2)  PushSetup.Time = 30;
	
	//���ؿ���������������3�Σ����˳�..
	if(((COM_No==1)||(COM_No==0))&&((PIN_SETEN & P_SETEN ) != 0))
		{
			if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)   //�������������࣬��ܾ����ʲ���..
			{
				//			case HDLC_DISC:			
				ClrRSNumber( ComBuf );				//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatDMFrame( ComBuf );
				return;	
			} 
		}
	*/
	MACState = CommPara[COM_No].MACState;
	
//  ComRecTmr = CommPara[GetCOM_No(ComBuf)].RecTmr;
//	BTime = CommPara[GetCOM_No(ComBuf)].BTime;

	SControlByte = *(ComBuf+HDLC_CtlPtr);
	
	ControlByte = GetControlByte( SControlByte );
	if( *MACState == NDMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;					//����NRMģʽ
				ClrRSNumber( ComBuf );				//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
			case HDLC_RR:				
			case HDLC_RNR:		
			case HDLC_DISC:				
//			case HDLC_UI:
				ClrRSNumber( ComBuf );				//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatDMFrame( ComBuf );
				break;		
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;
			default: break;
					
		}
	}
	else if( *MACState == NRMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;					//�ظ�����NRMģʽ	
				ClrRSNumber( ComBuf );				//������ͽ������к�
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
				HDLC_InformationFrameProcess( ComBuf );			
				break;
			case HDLC_RR:				
				HDLC_RRFrameProcess( ComBuf );			
				break;
			case HDLC_RNR:		
				HDLC_RNRFrameProcess( ComBuf );			
				break;
			case HDLC_DISC:				
				*MACState = NDMState;
				ClrRSNumber( ComBuf );				//������ͽ������к�
				ClrRSBuffPara( ComBuf );
//				CreatDMFrame( ComBuf );     //CTTͨ����Ҫ��UA..P3�޷�ͨ��..
				CreatUAFrame( ComBuf );        
				break;
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;			
			default: break;
		}		
	}	
}
/*
//fire--2017��6��5��21:25:23
{
////  	unsigned short Length;
//	unsigned short AddrLen;
//	unsigned short HDLC_DataPtr;
//	unsigned char FrameType;
//
////	Length = GetFrameLength( ComBuf );
//	AddrLen = GetAddrLength( ComBuf );
//	HDLC_DataPtr = AddrLen + 8;						//֡ͷ(1)������(2)��������(1)����ַͷ��(1)���ͻ�����ַ(1)��HCS(2)
//
//	if((*(ComBuf+HDLC_CtlPtr) & 0x20 ) != 0 ) 								//��֡
//	{
//		FrameType = *(ComBuf+HDLC_DataPtr + 1) & 0xC0;
//
////		pSendFrameNo = *(ComBuf+HDLC_DataPtr + 1) & 0x3F;
////		pSendFrameNo *= 256;
////		pSendFrameNo += *(ComBuf+HDLC_DataPtr);
//		
//		if( FrameType == COMFIRM_FRAME ) HDLC_RRFrameProcess( ComBuf );
//		else HDLC_InformationFrameProcess( ComBuf );	
//	}		
//	else HDLC_InformationFrameProcess( ComBuf );
	HDLC_InformationFrameProcess( ComBuf );
}

void HDLC_InformationFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
 
	unsigned short Length;
	unsigned short* TxdFrameNo;
	unsigned short* RxdFrameNo;
	unsigned short pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
//	unsigned char* LLCState;
	unsigned short AddrLen;
	unsigned short HDLC_DataPtr;
//	unsigned char ControlByte;
	unsigned char FrameType;
	unsigned char* SecurityState;			
	unsigned char* TimeTag;			//17.04.18

	COM_No = GetCOM_No(ComBuf);	
	
//	ControlByte = *(ComBuf+HDLC_CtlPtr);
	
	TxdFrameNo = CommPara[COM_No].TxdFrameNo;
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
	SecurityState = CommPara[COM_No].SecurityState;	// ͨѶ�ڰ�ȫ״̬		0x00: �ǰ�ȫ��֤���� 0x01������ 0x02:����
	TimeTag = CommPara[COM_No].TimeTag;				//ͨѶ��ʱ���ǩ	optional:1�ֽڣ�data_time_s:7�ֽڣ�TI:3�ֽ�		//17.04.18
	
	Length = GetFrameLength( ComBuf );
	AddrLen = GetAddrLength( ComBuf );
	HDLC_DataPtr = AddrLen + 8;						//֡ͷ(1)������(2)��������(1)����ַͷ��(1)���ͻ�����ַ(1)��HCS(2)
	if((( *RecDataPtr + (Length - (HDLC_DataPtr+1)) ) > (MAX_PDU_SIZE+1) )||(Length < (HDLC_DataPtr+1) ) )			//CTT���������PDU���෢��һ���ֽ�
	{
		*ComBuf = 0xFF;				//PDU������
		*RecDataPtr = 0;
	}
	else
	{
		if((*(ComBuf+HDLC_CtlPtr) & 0x20 ) != 0 ) 								//��֡
		{
			FrameType = *(ComBuf+HDLC_DataPtr + 1) & 0xC0;						//���ֽ���ǰ
			pSendFrameNo = *(ComBuf+HDLC_DataPtr + 1) & 0x3F;
			pSendFrameNo *= 256;
			pSendFrameNo += *(ComBuf+HDLC_DataPtr);

//			FrameType = *(ComBuf+HDLC_DataPtr) & 0xC0;							//���ֽ���ǰ
//			pSendFrameNo = *(ComBuf+HDLC_DataPtr) & 0x3F;
//			pSendFrameNo *= 256;
//			pSendFrameNo += *(ComBuf+HDLC_DataPtr+1);
			
			switch( FrameType )
			{
				case START_FRAME:								//��ʼ֡
					*RxdFrameNo = pSendFrameNo;
					*RecDataPtr = 0;
					break;
				case LAST_FRAME:								//���֡
				case SUB_FRAME:									//�м�֡
					if( (*RxdFrameNo+1) != pSendFrameNo )		//Ҫ�����յ��ı����յ��Ĵ�1
					{
						*CommPara[COM_No].LLCState = WaitRecNextState;
						CreatRRFrame( ComBuf, *RxdFrameNo );				//�յ���֡���ڴ���֡��Ų�һ�£������ط�
						return;
					}else *RxdFrameNo += 1;	
					break;
				case COMFIRM_FRAME:								//ȷ��֡	
					*RxdFrameNo += 1;							//����֡��ż�1
					break;
				default: 
					break;				
			}	
//			*RxdFrameNo += 1;
//			*RxdFrameNo &= 0x3F;								//����֡��ż�1
			*RxdFrameNo &= 0x3FFF;								

			if( FrameType == COMFIRM_FRAME )					//ȷ��֡
			{
//				*ComBuf = 0xFF;				
				if( *CommPara[COM_No].SendByteSum == 0 )		//���ĳ��������ݷ���
				{
					*ComBuf = 0xFF;				
					*CommPara[COM_No].SendDataPtr = 0;
	                *RecDataPtr = 0;
					*CommPara[COM_No].LLCState = LLCIdle;
					*TxdFrameNo = 0;
					*RxdFrameNo = 0;
					LLCStateCycle( COM_No );	
				}
				else
				{
//					*CommPara[COM_No].SendDataPtr = 0;
					*CommPara[COM_No].LLCState = SendNextState;
//					*TxdFrameNo += 1;
					LLCStateCycle( COM_No );	
				}
				return;
			}	

			Length -= (HDLC_DataPtr+3);												//ȥ����ַ���ּ�У��	
//			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����,�˵���֡��ʽ����Ϣ
			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+2, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����,�˵���֡��ʽ����Ϣ
			*RecDataPtr += Length;
			
			if( FrameType == LAST_FRAME )						//���֡
			{
				*SecurityState = 0;								// ͨѶ�ڰ�ȫ״̬		0x00: �ǰ�ȫ��֤���� 0x01������ 0x02:����	//17.04.07
				*TimeTag = 0;									//ͨѶ��ʱ���ǩ	optional:1�ֽڣ�data_time_s:7�ֽڣ�TI:3�ֽ�		//17.04.18
				*CommPara[COM_No].SendByteSum = APPLProcess( COM_No, RecBuff, SendBuff );		//��������֡����
				*SecurityState = 0;								// ͨѶ�ڰ�ȫ״̬		0x00: �ǰ�ȫ��֤���� 0x01������ 0x02:����
				if(( *CommPara[COM_No].SendByteSum == 0 )||(( *(ComBuf+HDLC_SACtlPtr) & 0x80) != 0 ))		//���ĳ�������ַ����㲥��ַ�������ݷ���
				{
					*ComBuf = 0xFF;				
					*CommPara[COM_No].SendDataPtr = 0;
	                *RecDataPtr = 0;
					*CommPara[COM_No].LLCState = LLCIdle;
					*TxdFrameNo = 0;
					*RxdFrameNo = 0;
					LLCStateCycle( COM_No );	
				}
				else
				{
					*CommPara[COM_No].SendDataPtr = 0;
					*CommPara[COM_No].LLCState = SendNextState;
					*TxdFrameNo = 0;
					LLCStateCycle( COM_No );	
				}
			}
			else
			{		
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );			//�к���֡��������һ֡
			}	
		}	
		else
		{
			Length -= (HDLC_DataPtr+1);												//ȥ����ַ���ּ�У��
			if(( *(ComBuf+HDLC_DataPtr) == APDU_SECURITY_Request )&&( *(ComBuf+HDLC_DataPtr+1) == 0x00 )&&(( *(ComBuf+HDLC_DataPtr+2) & 0x80 ) == 0x00 )
				&&( *(ComBuf+HDLC_DataPtr+3) == APDU_GET_Request )&&( *(ComBuf+HDLC_DataPtr+4) == get_request_next ))
			{
				*TimeTag = 0;									//ͨѶ��ʱ���ǩ	optional:1�ֽڣ�data_time_s:7�ֽڣ�TI:3�ֽ�		//17.04.18
				*CommPara[COM_No].SendByteSum = GetSECURITYRequestNextProcess( COM_No, RecBuff, SendBuff, ComBuf+HDLC_DataPtr );
			}	
			else if(( *(ComBuf+HDLC_DataPtr) == APDU_GET_Request )&&( *(ComBuf+HDLC_DataPtr+1) == get_request_next ))
			{
				*TimeTag = 0;									//ͨѶ��ʱ���ǩ	optional:1�ֽڣ�data_time_s:7�ֽڣ�TI:3�ֽ�		//17.04.18
				*CommPara[COM_No].SendByteSum = GetRequestNextProcess( COM_No, RecBuff, SendBuff, ComBuf+HDLC_DataPtr );
			}
			else
			{		
				*RecDataPtr = 0;														//�޷�֡����Զָ��0
				RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//�������յ�����Ϣ֡�����������ݽ��ջ�����
				*RecDataPtr += Length;				
				*SecurityState = 0;								// ͨѶ�ڰ�ȫ״̬		0x00: �ǰ�ȫ��֤���� 0x01������ 0x02:����	//17.04.07
				*TimeTag = 0;									//ͨѶ��ʱ���ǩ	optional:1�ֽڣ�data_time_s:7�ֽڣ�TI:3�ֽ�		//17.04.18
				*CommPara[COM_No].SendByteSum = APPLProcess( COM_No, RecBuff, SendBuff );		//��������֡����
				*SecurityState = 0;								// ͨѶ�ڰ�ȫ״̬		0x00: �ǰ�ȫ��֤���� 0x01������ 0x02:����
			}
			if(( *CommPara[COM_No].SendByteSum == 0 )||(( *(ComBuf+HDLC_SACtlPtr) & 0x80) != 0 ))		//���ĳ�������ַ����㲥��ַ�������ݷ���
			{
				*ComBuf = 0xFF;				
				*CommPara[COM_No].SendDataPtr = 0;
                *RecDataPtr = 0;
				*CommPara[COM_No].LLCState = LLCIdle;
				*TxdFrameNo = 0;
				*RxdFrameNo = 0;
				LLCStateCycle( COM_No );	
			}
			else
			{
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				*TxdFrameNo = 0;
				LLCStateCycle( COM_No );	
			}
		}	
	}		
	
}				


//����֡ͷ
//FrameLen:֡����
//SegSubFlag: �ֶα�־ 0�����һ֡��1�����к���֡
//ClientAddr: �ͻ�����ַ��Ŀ�ĵ�ַ
//fire--2017��6��5��21:28:26
void CreatFrameHead( unsigned char* ComBuf, unsigned short FrameLen, unsigned char SegmentFlag )
{

	*ComBuf = 0x68;
	*(ComBuf+HDLC_LenPtrL) = FrameLen % 256;
	*(ComBuf+HDLC_LenPtrH) = FrameLen / 256;
	*(ComBuf+HDLC_CtlPtr) = 0xC3;							//֡����
	if( SegmentFlag != 0 ) *(ComBuf+HDLC_CtlPtr) |= 0x20; 	//�к���֡
	*(ComBuf+HDLC_SACtlPtr) &= 0x3F;						//��Ӧ֡�ĵ�ַ����ֻ���ǵ���ַ
}	

//����RR֡
void CreatRRFrame( unsigned char* ComBuf, unsigned short RxdFrameNo )
{
	unsigned char Buff[6];
	unsigned short FrameLen;
	unsigned short AddrLen;
	unsigned short HDLC_DataPtr;

	FrameLen = 0;
	AddrLen = GetAddrLength( ComBuf );
	E2P_RData( Buff, EMeter_Addr, 6 );
	RAM_Write( ComBuf+HDLC_SAPPtr, Buff, AddrLen );

	HDLC_DataPtr = AddrLen + 8;						//֡ͷ(1)������(2)��������(1)����ַͷ��(1)���ͻ�����ַ(1)��HCS(2)
	FrameLen += HDLC_DataPtr + 1;
	*(ComBuf+HDLC_DataPtr) = RxdFrameNo % 256;				//���ֽ���ǰ
	*(ComBuf+HDLC_DataPtr+1) = RxdFrameNo / 256;
	*(ComBuf+HDLC_DataPtr+1) |= COMFIRM_FRAME;	
//	*(ComBuf+HDLC_DataPtr+1) = RxdFrameNo % 256;			//���ֽ���ǰ
//	*(ComBuf+HDLC_DataPtr) = RxdFrameNo / 256;
//	*(ComBuf+HDLC_DataPtr) |= COMFIRM_FRAME;	
    FrameLen += 2;                                          //��2�ֽڷ�֡��ʽ��    
 	CreatFrameHead( ComBuf, FrameLen, SUB_FRAGMENT );
}	

//����I֡
void CreatIFrame( unsigned char* ComBuf, unsigned short TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag, unsigned char FrameFlag )
{
	unsigned char Buff[6];
	unsigned short AddrLen;
	unsigned short HDLC_DataPtr;

	AddrLen = GetAddrLength( ComBuf );
	E2P_RData( Buff, EMeter_Addr, 6 );
	RAM_Write( ComBuf+HDLC_SAPPtr, Buff, AddrLen );
	
	HDLC_DataPtr = AddrLen + 8;						//֡ͷ(1)������(2)��������(1)����ַͷ��(1)���ͻ�����ַ(1)��HCS(2)
	FrameLen += HDLC_DataPtr + 1;
	if( SegmentFlag == SUB_FRAGMENT )
	{
		*(ComBuf+HDLC_DataPtr) = TxdFrameNo % 256;	            //���ֽ���ǰ
		*(ComBuf+HDLC_DataPtr+1) = TxdFrameNo / 256;
		*(ComBuf+HDLC_DataPtr+1) |= FrameFlag;	
//		*(ComBuf+HDLC_DataPtr+1) = TxdFrameNo % 256;	        //���ֽ���ǰ
//		*(ComBuf+HDLC_DataPtr) = TxdFrameNo / 256;
//		*(ComBuf+HDLC_DataPtr) |= FrameFlag;	
        FrameLen += 2;                                          //��2�ֽڷ�֡��ʽ��    
	}
 	CreatFrameHead( ComBuf, FrameLen, SegmentFlag );
}	
*/

void HDLC_RRFrameProcess( unsigned char* ComBuf )
{

	unsigned short COM_No;
	unsigned char* RxdFrameNo;
	unsigned char* LLCState;

	COM_No = GetCOM_No(ComBuf);	
	
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	LLCState = CommPara[COM_No].LLCState;

	if( *LLCState == LLCIdle )
	{
		*RxdFrameNo = 0;
		CreatRRFrame( ComBuf, *RxdFrameNo );				//���û��׼���ϴ�֡
	}
	else if( *LLCState == WaitRecNextState )
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x3F;								//����֡��ż�1
		CreatRRFrame( ComBuf, *RxdFrameNo );				//����վ�·���һ֡
	}
	else if( *LLCState == SendNextState )
	{
		*RxdFrameNo &= 0x3F;								//����֡���ά�ֲ���
		LLCStateCycle( COM_No );							//����ϴ�֡
	}	
	
}	


unsigned short GetMaxInfoSize( unsigned short COM_No )
{
	if( COM_No == 0 ) return MAX_INFRA_INFO_SIZE-25;
	else if( COM_No == 1 ) return MAX_485_INFO_SIZE-25;
#if ( CarryComm == YesCheck )			//���ز�ͨ����485��2����ͨѶ����
	else return MAX_CARRY_INFO_SIZE-25;
#else
	else return MAX_485_INFO_SIZE-25;
#endif
}	
// EndFlag==0;��ʼ EndFlag ==1 ����
//��ͷ��E6 E7 00   C4 02 C1    00 00 00   00 01 00   82 XX XX
short CreatGetDataObjectListHead( short COM_No, unsigned char* Dest, unsigned char BlockEndFlag )   
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long* BlockNumber;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	
	*Dest = 0xE6;
	*(Dest+1) = 0xE7;
	*(Dest+2) = 0;										
	*(Dest+3) = 0xc4;
	*(Dest+4) = 0x02;
	
	if(((HComm.Send_state_InvokeId&0xf0)==0xc0) || ((HComm.Send_state_InvokeId&0xf0)==0x80))
	{
			if(COM_No==0) *(Dest+5) =  HComm.Send_state_InvokeId;		
			else *(Dest+5) =  HComm.Send_state_InvokeId;			  //��Ҫ����ʵ�ʽ����滻����������test
  }
  else *(Dest+5) = 0xc1;
  		
	if(BlockEndFlag!=0) *(Dest+6) = 0x01;    //last-block=0��ʾ�������һ֡���ݣ������������ݿ顣
	else *(Dest+6) = 0;
		
	*BlockNumber += 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+7, Ptr, 4 );			//block-number ������=1Ϊ����ĵ�һ�顣
	*(Dest+11) = 0x00;								//resultѡ��0��raw-data�����������ݲ���A-XDR�����ΪOCTETSTRING����Ϊ��ASN.1��û�й涨��OCTETSTRING�ĳ��ȣ��������µ�OCTETSTRING���ñ䳤��ʽ���루A-XDR����涨��
	*(Dest+12) = 0x82;								//���BITλ1��ʾ���ֽ�Ϊ��������룬02��ʾ��������ֽ�Ϊ����ֵ
	*(Dest+13) = 0x00;
	*(Dest+14) = 0x00;								//���ݳ��� 
    
   return 15;
}

void LLCStateCycle( unsigned short COM_No )
{
#if 0 
	unsigned char* RxdFrameNo;
	unsigned char* TxdFrameNo;
	unsigned short* SendDataPtr;
	unsigned short* SendSum;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char* ComBuf;
	unsigned char SegmentFlag;
	unsigned char* COSEM_BlockState; 
	unsigned short FrameLen,i;
	
	unsigned short RepeatLEN = MAX_INFO_SIZE + 20;
	unsigned short SendMAX_INFO;
	unsigned char * RepeatPtr;				
	//unsigned long* BlockNumber;


	//BlockNumber = CommPara[COM_No].BlockNumber;
    RepeatPtr = HComm.RepeatBuff;
    
    LLCState = CommPara[COM_No].LLCState;
    COSEM_BlockState = &HComm.COSEM_BlockState[COM_No];
      	
/*  if((COM_No==2)&&(crypt.GPRS_STAND == 0x0B)) //GPRS/PLCͨ��
  {
 		  //����ʱ����Ҫ�����ܲ���:7���ֽڣ�0xCx +0xXX +0x20 + 0x01 + 0x23 +0x45 + 0x67..
		  if((crypt.type[COM_No] == 0x20)||(crypt.type[COM_No] == 0x30)||(crypt.type[COM_No] == 0x10)){
		  	if(crypt.type[COM_No] == 0x20) SendMAX_INFO = GPRS_MAX_INFO_SIZETX - 10;
		  	else SendMAX_INFO = GPRS_MAX_INFO_SIZETX - 22;
			}else{
							SendMAX_INFO = GPRS_MAX_INFO_SIZETX;
						}
	}else
	*/
	//{
		  //����ʱ����Ҫ�����ܲ���:7���ֽڣ�0xCx +0xXX +0x20 + 0x01 + 0x23 +0x45 + 0x67..
		  if((crypt.type[COM_No] == 0x20)||(crypt.type[COM_No] == 0x30)||(crypt.type[COM_No] == 0x10)){
		  	if(crypt.type[COM_No] == 0x20) SendMAX_INFO = MAX_INFO_SIZE - 10;
		  	else SendMAX_INFO = MAX_INFO_SIZE - 22;
			}else{
							SendMAX_INFO = MAX_INFO_SIZE;
						}
	//}
     
	switch( *LLCState )	
	{
		case LLCIdle:				// LLC����
			break;
		case WaitRecNextState:		// LLC�ȴ�
			break;
		case SendNextState:			// ����δ��
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

		//	if( (*SendSum) > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			if( (*SendSum) > SendMAX_INFO ) FrameLen = SendMAX_INFO;		
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				HComm.APPL_A8_BUSY[COM_No] |= 0x80;		//��֡���Ͳ���״̬..
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = SendNextState;
			}	
			else 
			{
				HComm.APPL_A8_BUSY[COM_No] &= ~0x80;	//��֡���ͷ�֡����..						
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	//д��Ҫ���͵�����
			*SendDataPtr += FrameLen;
			//���ܺ��Ͳ���..
			Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // ����sky20140624..
			//�������ж��Ƿ���Ҫ����..
			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			*TxdFrameNo += 1;
			*TxdFrameNo &= 0x07;
			FrameLen +=14;                                                                
			for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
			*(RepeatPtr+RepeatLEN) = FrameLen;					
			break;
		case UISendNextState:			// UI����δ��
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
//			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
//			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			if( (*SendSum) > SendMAX_INFO ) FrameLen = SendMAX_INFO;
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				HComm.APPL_A8_BUSY[COM_No] |= 0x80;		//��֡����æ����״̬..				
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = UISendNextState;
			}	
			else 
			{
				HComm.APPL_A8_BUSY[COM_No] &= ~0x80;	//��֡���ͷ�֡����..		
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			*SendDataPtr += FrameLen;
//			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			//���ܺ��Ͳ���..
			Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // ����sky20140624..
			//�������ж��Ƿ���Ҫ����..
			CreatUIFrame( ComBuf, FrameLen, SegmentFlag );
//			*TxdFrameNo += 1;
//			*TxdFrameNo &= 0x07;
			FrameLen +=14;                                                                
			for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
			*(RepeatPtr+RepeatLEN) = FrameLen;							
			break;
				
	 case UIBlockSendNextState:			// ����128���ֽ�/�����������-UI�ְ�����δ��//�޸�����128��300..
	
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			SendSum = CommPara[COM_No].SendByteSum;

			SegmentFlag = LAST_FRAGMENT;   //�ְ�������A0������A8����..
			
			if(((*SendSum) > SendMAX_INFO )&&(*COSEM_BlockState==DATABLOCKIDLE))
			{
				*COSEM_BlockState = NEW_BLOCK;
			}
			else
			{
				if(( (*SendSum) <= SendMAX_INFO )&&((*COSEM_BlockState==DATABLOCKIDLE)||(*COSEM_BlockState==FIRSTDATABLOCK)||(*COSEM_BlockState==MIDDATABLOCK)||(*COSEM_BlockState==ENDDATABLOCK)))  //һ֡���Դ����꣬�����ٷְ�..
						{
							  FrameLen = *SendSum;
							  
								*SendSum -= FrameLen;
			
						//		SegmentFlag = LAST_FRAGMENT;
								*LLCState = LLCIdle;
								*CommPara[COM_No].RecDataPtr = 0;
								RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
								*SendDataPtr += FrameLen;	
									//���ܺ��Ͳ���..
								Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // ����sky20140624..
								//�������ж��Ƿ���Ҫ����..							
								CreatUIFrame( ComBuf, FrameLen, SegmentFlag );
								*COSEM_BlockState = DATABLOCKIDLE;
								FrameLen +=14;                                                                
								for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
								*(RepeatPtr+RepeatLEN) = FrameLen;							
								return;			//����..
						}
			}
			
	 //�ְ��ֱ�־���Ƿ���Ҫ��Ӱ�ͷ�����¼�������ֽ���..
	 if( (*SendSum) > (SendMAX_INFO-15) )       //�ٰ�ͷ��C4 02 C1 00 00 00 00 01 00 82 XX XX
	 		{
				 FrameLen = (SendMAX_INFO-15);
				 *SendSum -= FrameLen;
				 
//				 switch( *COSEM_BlockState )	
//					{
//						case FIRSTDATABLOCK:				// ��һ���״�..
//						case MIDDATABLOCK:          // �м���״�..
//						case ENDDATABLOCK:					// ���һ�鴫��..														
//							break;
//						case FIRSTMIDDATABLOCK:				// �����һ���м䴫��..
//						case ENDMIDDATABLOCK:					// ���һ���м䴫��..
//						case NEW_BLOCK:							  // ���������ݿ�..
//							break;							
//					default:
//					  	*COSEM_BlockState = 0;
//					  	break;	
//					}
						//��ͷ��E6 E7 00   C4 02 C1    00 00 00   00 01 00   82 XX XX
				 if((*COSEM_BlockState==FIRSTDATABLOCK)||(*COSEM_BlockState==MIDDATABLOCK)||(*COSEM_BlockState==ENDDATABLOCK))   //�׿鴫��..
				 {
				 	  if(*COSEM_BlockState==ENDDATABLOCK)
				 	  {
				 	  		*COSEM_BlockState = ENDMIDDATABLOCK;				 	  		
				 	  }
				 	  else
				 	  {
				 	  		*COSEM_BlockState = FIRSTMIDDATABLOCK;				 	  		
				 	  }
				 	  // ��־�ı�..
					 	if(*(SendBuff+*SendDataPtr+6) != 0) *(SendBuff+*SendDataPtr+6) = 0;    // ������־δ��..
					 	//֡���ȼ���..
					 	if(( *(SendBuff+*SendDataPtr+12) & 0x80 ) != 0 )
							{
								if( *(SendBuff+*SendDataPtr+12) == 0x81 )										//���ݳ����Ƿ�Ϊ1�ֽ� 
								{
									*(SendBuff+*SendDataPtr+13) = (FrameLen-14);							//��Ч�������ݳ���									
								}			
								else 
								{
//									*(SendBuff+*SendDataPtr+13) = 0;   //����255������..
//									*(SendBuff+*SendDataPtr+14) = (FrameLen-15);

									*(SendBuff+*SendDataPtr+13) = (FrameLen-15)/ 256;   //�޸�20160305iran..
									*(SendBuff+*SendDataPtr+14) = (FrameLen-15)% 256;

								}	
							}	
							else
							{
								*(SendBuff+*SendDataPtr+12) = FrameLen - 13;
							}	
						RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );
				    *SendDataPtr += FrameLen;
				 }
				 else
				 	{
				 		//�������ݿ��ͷ..
				 		CreatGetDataObjectListHead( COM_No, ComBuf+HDLC_DataPtr,0 );
				 		//*(ComBuf+HDLC_DataPtr+14) = FrameLen; 
				 		// if(*COSEM_BlockState == NEW_BLOCK) *COSEM_BlockState = ENDMIDDATABLOCK;    //�������20140620..
				 		//   RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr, FrameLen );
				 		if(*COSEM_BlockState == NEW_BLOCK){
				 				*COSEM_BlockState = ENDMIDDATABLOCK; 
				 				*(ComBuf+HDLC_DataPtr+14) = FrameLen-7;   
				 				RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr+7, FrameLen-7 );
				 				*SendDataPtr += FrameLen;
				     		//FrameLen += 15;
				     		FrameLen += 8;
				 		}else{
				 					*(ComBuf+HDLC_DataPtr+14) = FrameLen;   				 		
				 	   			RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr, FrameLen );
				 	   			*SendDataPtr += FrameLen;
				     			FrameLen += 15;
				 	   		 }

				 	}
			}					
			else
			{
				 FrameLen = *SendSum;				 
				 *SendSum -= FrameLen;
				 				 		
				 //����֡--�������ݿ��ͷ..
				 if(*COSEM_BlockState == FIRSTMIDDATABLOCK) 	CreatGetDataObjectListHead( COM_No, ComBuf+HDLC_DataPtr,0 );
				 else 	CreatGetDataObjectListHead( COM_No, ComBuf+HDLC_DataPtr,1 );
				 *(ComBuf+HDLC_DataPtr+14) = FrameLen; 
				 
				 RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr, FrameLen );
				 *SendDataPtr += FrameLen;
				 FrameLen += 15;
			}
			
			if( *SendSum > 0 ) 
			{	
				// SegmentFlag = LAST_FRAGMENT;   							//�ְ�������A0������A8����..
				*CommPara[COM_No].RecDataPtr = 0;
				*LLCState = UIBlockSendNextState;
			}	
			else          //*SendSum == 0
			{
				//SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
				*COSEM_BlockState = DATABLOCKIDLE;
			}
			//���ܺ��Ͳ���..
			Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // ����sky20140624..
			//�������ж��Ƿ���Ҫ����..											
			CreatUIFrame( ComBuf, FrameLen, SegmentFlag );    //����UI֡..
			FrameLen += 14;
			for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
			*(RepeatPtr+RepeatLEN) = FrameLen;
			PushSetup.SendNumber = 0;	
			break;
		default:
		  	*LLCState = LLCIdle;
		  	PushSetup.SendNumber  = 0;
		  	break;	
	}
#endif	
}
short DLMS_CommModeRec( short COM_No )
{
	unsigned char Buff[35];
	unsigned char* Point;
	unsigned char XorCheck;
	short CheckAds;
	unsigned short i;
  	short Temp;
	unsigned long Value;

  	unsigned char* ComBuf;
  	unsigned char* ComMode;
  	unsigned short* ComPtr;
  	unsigned char* ComRun;
  	unsigned char* ComState;
  	unsigned char* MACState;
  	ComBuf = CommPara[COM_No].Buf;
  	ComMode = CommPara[COM_No].Mode;
  	ComPtr = CommPara[COM_No].Ptr;
  	ComRun = CommPara[COM_No].Run;
  	ComState = CommPara[COM_No].IECState;
	MACState = CommPara[COM_No].MACState;
	Point = Buff;
	switch( *ComState )
	{
		case IECModeHDLCRXOver:		// IECͨѶģʽE,HDLC֡����
				if( DLMS_HDLC_CheckFrame( COM_No,ComBuf ) != 0 ) 
				{
					*ComState = IECModeHDLC;
					DLMS_CommInit( COM_No );
				 	return 0;					
				}
				
				if( JudgeHDLC_FRMR( ComBuf ) != 0 )
				{
					FillHCS_FCS_Flag( ComBuf );
					break;						
				}	
				HDLC_Ctrl( ComBuf );
				if( *ComBuf == 0x7E )
				{
					FillHCS_FCS_Flag( ComBuf );
				}
				else
				{
					*ComState = IECModeHDLC;
					DLMS_CommInit( COM_No );
				 	return 0;					
				}
				
				break;		
		default: DLMS_CommInit( COM_No );
				 return 0;
	}	
	*ComMode += 1;
	*ComRun &= ~F_ComOK; 
	*ComPtr = 1;
	HT_FreeDog();
	switch( COM_No )
	{
		case 0: 
		//	HT_GPIOA->PTDIR &= ~GPIOA_PLC_SET;//PDIR_SWZB |= P_SWZB;	
		//	HT_UART0->UARTSTA &= ~UART_UARTSTA_RXIF;//UCA2IFG &= ~UCTXIFG;				//13.05.17
			HT_UART0->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
			HT_UART0->SBUF = *ComBuf;//UCA2TXBUF = 0xFE;			
			break;			
		case 1:	
			HT_UART1->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);	
			HT_UART1->SBUF = *ComBuf;//UCA1TXBUF = 0xFE;

			break;			
		case 2:	
			HT_GPIOE->PTCLR |= SW485_1;//PDIR_SWZB |= P_SWZB;	
		//	HT_UART3->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA2IFG &= ~UCTXIFG;				//13.05.17
			HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
			HT_UART3->SBUF = *ComBuf;//UCA2TXBUF = 0xFE;			
			break;		
		case 3:	
			HT_GPIOE->PTCLR |= SW485_2;//PDIR_SWZB |= P_SWZB;	
		//	HT_UART4->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA2IFG &= ~UCTXIFG;				//13.05.17
			HT_UART4->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
			HT_UART4->SBUF = *ComBuf;//UCA2TXBUF = 0xFE;			
			break;			
		default:break;				
	}	
	return 0;
}
#if 0
void LLCStateCycle( unsigned short COM_No )
{
//	unsigned short* RxdFrameNo;
	unsigned short* TxdFrameNo;
	unsigned short* SendDataPtr;
	unsigned short* SendSum;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char* ComBuf;
	unsigned char SegmentFlag;
	unsigned char FrameFlag;
	unsigned short FrameLen;
	unsigned short HDLC_DataPtr;
	unsigned short MaxInfoSize;
	unsigned short AddrLen;
    
    LLCState = CommPara[COM_No].LLCState;
    
	switch( *LLCState )	
	{
		case LLCIdle:				// LLC����
			break;
		case WaitRecNextState:		// LLC�ȴ�
			break;
		case SendNextState:			// ����δ��
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
//			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			MaxInfoSize = GetMaxInfoSize( COM_No );
			if( (*SendSum) > MaxInfoSize ) FrameLen = MaxInfoSize;
			else FrameLen = *SendSum;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				SegmentFlag = SUB_FRAGMENT;						//��֡����
				if( *TxdFrameNo == 0 )
				{
					FrameFlag = START_FRAME;					//��ʼ֡
				}	
				else FrameFlag = SUB_FRAME;						//�м�֡
				*LLCState = SendNextState;
			}	
			else 
			{
				if( *TxdFrameNo == 0 )
				{ 
					SegmentFlag = LAST_FRAGMENT;				//����֡
					*LLCState = LLCIdle;
					*CommPara[COM_No].RecDataPtr = 0;
				}
				else
				{
					SegmentFlag = SUB_FRAGMENT;					//��֡
					FrameFlag = LAST_FRAME;						//���һ֡
					*CommPara[COM_No].RecDataPtr = 0;
				}		
			}	
			
			AddrLen = GetAddrLength( ComBuf );
			HDLC_DataPtr = AddrLen + 8;						//֡ͷ(1)������(2)��������(1)����ַͷ��(1)���ͻ�����ַ(1)��HCS(2)
			if( SegmentFlag == SUB_FRAGMENT )
			{
				RAM_Write( ComBuf+HDLC_DataPtr+2, SendBuff+*SendDataPtr, FrameLen );	//����֡��ʽ��2�ֽ�
			}
			else	
			{
				RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			}	
			*SendDataPtr += FrameLen;
//			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			CreatIFrame( ComBuf, *TxdFrameNo, FrameLen, SegmentFlag, FrameFlag );
			*TxdFrameNo += 1;
			*TxdFrameNo &= 0x3F;
			break;
		default:
		  	*LLCState = LLCIdle;
		  	break;	
	}	
}
#endif
