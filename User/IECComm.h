#include "ht6xxx.h"

//#define TM_120s	120
//#define TM_120s	6
//#define TM_120s	15
/**********************************
	Baut rate Define (SMCLK=4M)
**********************************/	
//#define Baut_300L	0x9D	
#define Baut_300L	0x9f	
#define Baut_300H	0x36
//#define Baut_300M	0xFF
//#define Baut_300M	0x55
#define Baut_300M	0x00

#define Baut_600L	0x4E
#define Baut_600H	0x1B
//#define Baut_600M	0x55
#define Baut_600M	0x00

#define Baut_1200L	0xA6
#define Baut_1200H	0x0D
//#define Baut_1200M	0x0D
#define Baut_1200M	0x00

#define Baut_2400L	0xD3
#define Baut_2400H	0x06
//#define Baut_2400M	0xEF
#define Baut_2400M	0x00

//#define Baut_2400L	0x05
//#define Baut_2400H	0x0D
//#define Baut_2400M	0x6B

//#define Baut_4800L	0xD3
//#define Baut_4800H	0x06
//#define Baut_4800M	0xEF

#define Baut_4800L	0x69
#define Baut_4800H	0x03
//#define Baut_4800M	0xEF
#define Baut_4800M	0x00

#define Baut_9600L	0xB4
#define Baut_9600H	0x01
//#define Baut_9600M	0xFF
#define Baut_9600M	0x00

#define Baut_57600L	0x5A
#define Baut_57600H	0x00

#define Baut_19200L	0xDA
#define Baut_19200H	0x00
//#define Baut_19200M	0x55
#define Baut_19200M	0x00

#define Baut300  0
#define Baut600  1
#define Baut1200 2
#define Baut2400 3
#define Baut4800 4
//#define Baut7200 5
//#define Baut9600 6
//#define Baut19200 7
/*
#define Baut9600  5
#define Baut19200 6
#define Baut38400 7
#define Baut57600 8
#define Baut115200 9
*/

#define IECIdle				  0	  // IECͨѶ����
#define IECStartCode		  1	  // IECͨѶ��ʼ
#define IECStartCodeOver	  2	  // IECͨѶ��ʼ֡����
#define IECBandRate			  3	  // IECͨѶ�л�������
#define IECBandRateOver		  4	  // IECͨѶ�л�������֡����
#define IECModeC			  5	  // IECͨѶģʽC
#define IECModeCOver		  6	  // IECͨѶģʽC֡����
#define IECModeCRXOver		  7	  // IECͨѶģʽC֡����	
#define IECModeHDLC			  8	  // IECͨѶģʽE,HDLC
#define IECModeHDLCOver		  9	  // IECͨѶģʽE,HDLC֡����
#define IECModeHDLCRXOver	  10	// IECͨѶģʽE,HDLC֡����
#define IECModeExit			  11	// IECͨѶ�˳�

//MACState						      //ͨѶ��1MACͨѶ����״̬
#define	NDMState			  0	// MACδ����
#define	NRMState			  1	// MAC������		

//LLCState						          // ͨѶ��1MACͨѶ����״̬
#define	LLCIdle				  0	// LLC����
#define	WaitRecNextState	  1	// LLC�ȴ�
#define	SendNextState		  2	// ����δ��
#define	UISendNextState		  3	// UI֡����δ��
#define	UIBlockSendNextState  4 //UI֡�ְ�����δ��..
//APPLState						// ͨѶ��1APPL����״̬
#define	APPLIdle			  0	// APPL����
#define	APPLON				  1	// APPL������

#define F_ComOK		0x01

#define HDLC_I 		0x10
#define HDLC_RR 	0x11
#define HDLC_RNR	0x15
#define HDLC_SNRM	0x93
#define HDLC_DISC	0x53
#define HDLC_UA		0x73
#define HDLC_DM		0x1F
#define HDLC_FRMR	0x97
#define HDLC_UI		0x13
#define HDLC_UI_N	0x03		//�޻���

//#define RS_MinSize   12
//#define RS_MinSize   14					//12.04.07
//#define RRx_MaxSize  RS_MinSize+128			

//#define HDLC_HeadPtr     	0
//#define HDLC_LenPtrH      	1
//#define HDLC_LenPtrL      	2
//#define HDLC_Addr0Ptr     	3
//#define HDLC_SClientAddr	3		//���͵Ŀͻ���ַ
//#define HDLC_RClientAddr	7		//���յ��Ŀͻ���ַ
//#define HDLC_CtlPtr      	8
//#define HDLC_HCSPtr	      	9
//#define HDLC_DataPtr    	11	
/*
#define HDLC_HeadPtr     	0
#define HDLC_LenPtrL      	1
#define HDLC_LenPtrH      	2
//#define HDLC_CtlPtr      	3
#define HDLC_SACtlPtr      	4
#define HDLC_SAPPtr      	5
#define HDLC_RClientAddr	7		//���յ��Ŀͻ���ַ
#define HDLC_CtlPtr      	8
#define HDLC_HCSPtr	      	9
#define HDLC_DataPtr    	11	
*/
#define HDLC_HeadPtr     	0
#define HDLC_LenPtrH      	1
#define HDLC_LenPtrL      	2
#define HDLC_Addr0Ptr     	3
#define HDLC_SClientAddr	3		//���͵Ŀͻ���ַ
#define HDLC_RClientAddr	7		//���յ��Ŀͻ���ַ
#define HDLC_CtlPtr      	8
#define HDLC_HCSPtr	      	9
#define HDLC_DataPtr    	11	

#define LAST_FRAGMENT		0		//����֡
#define SUB_FRAGMENT		1		//��֡

#define START_FRAME			0x00	//��֡�е���ʼ֡
#define LAST_FRAME			0x40	//��֡�е����֡
#define COMFIRM_FRAME		0x80	//��֡�е�ȷ��֡
#define SUB_FRAME			0xC0	//��֡�е��м�֡

//#define MAX_INFO_SIZE		200
#define MAX_INFRA_INFO_SIZE	200
//#define MAX_INFRA_INFO_SIZE	100
#define MAX_485_INFO_SIZE	512
//#define MAX_485_INFO_SIZE	100
#define MAX_CARRY_INFO_SIZE	255
//#define MAX_CARRY_INFO_SIZE	100
#define MAX_INFO_SIZE		230//128//128
#define GPRS_MAX_INFO_SIZE	128//256 

//#define RS_MinSize   14					//12.04.07
//#define RRx_MaxSize  RS_MinSize+128			

#define MAX_485_SIZE	 128
#define RRx_485_MaxSize  RS_MinSize+MAX_485_SIZE			

#define MAX_PDU_SIZE		512//2000

#define MAX_BLOCK_SIZE		2048
//�鴫���־..
#define	 DATABLOCKIDLE    		0     		// �޺�����0
#define	 FIRSTDATABLOCK   		1					// �����һ�鴫��..
#define	 MIDDATABLOCK     		2					// ���ݿ��м䴫��..
#define	 ENDDATABLOCK     		3					// ���һ�鴫��..
#define	 NEW_BLOCK        		4     		// �����ݿ��֡��Ҫ���´�����ݿ�����д���..
#define	 FIRSTMIDDATABLOCK      5					// ���ݿ��м䴫��..
#define	 ENDMIDDATABLOCK     	6					// ���һ�����ݿ��м䴫��..		

/*
short ASC_BCD(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short BCD_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short GetBandRate( unsigned char BandRate );
unsigned char GetTXDelay( unsigned char* Source );
void CommInit( short COM_No );
void CommRest( short COM_No );  		//�ָ�����ʼ״̬
short GetXorCheck( unsigned char* ComBuf, unsigned char* XorCheck, short* CheckAds);
void CommMode( short COM_No );
short CommModeRec( short COM_No );
unsigned short MP_FCS(unsigned short fcs,unsigned short c);
unsigned short GetCheckCode_16BitFcs(unsigned char *pucData,unsigned short usDataLen);
unsigned short GetFrameLength( unsigned char* ComBuf );
void FillHCS_FCS_Flag( unsigned char* ComBuf );
unsigned short GetMeterAddress( unsigned char* ComBuf, unsigned char* ClientAddr, unsigned short* ServerAddrHigh, unsigned short* ServerAddrLow );
void Put16BitFcs( unsigned char* Source, unsigned char* Dest, unsigned short Len );
unsigned char HDLC_CheckFrame( unsigned char* ComBuf );
unsigned char GetControlByte( unsigned char SControlByte );
unsigned short GetInformationField( unsigned char* Dest );
void CreatFrameHead( unsigned char* ComBuf, unsigned char ClientAddr, unsigned short FrameLen, unsigned char SegmentFlag );
void CreatUAFrame( unsigned char* ComBuf );
void CreatDMFrame( unsigned char* ComBuf );
void CreatRRFrame( unsigned char* ComBuf, unsigned char RxdFrameNo );
void CreatIFrame( unsigned char* ComBuf, unsigned char RxdFrameNo, unsigned char TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag );
unsigned short GetHDLCParaValue( unsigned char* Source, unsigned short* Len, unsigned long* Value );
unsigned char JudgeHDLCParaInformationField( unsigned char* Source, short Length );
unsigned char JudgeHDLC_FRMR( unsigned char* ComBuf );
void ClrRSNumber( unsigned char* ComBuf );
void ClrRSBuffPara( unsigned char* ComBuf );
void HDLC_Ctrl(unsigned char* ComBuf);
void HDLC_InformationFrameProcess( unsigned char* ComBuf );
void HDLC_RRFrameProcess( unsigned char* ComBuf );
void LLCStateCycle( unsigned short COM_No );
void HDLC_UIFrameProcess( unsigned char* ComBuf );
*/

short ASC_BCD(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short BCD_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short GetBandRate( unsigned char BandRate );
unsigned char GetTXDelay( unsigned char* Source );
short GetCOM_No( unsigned char* ComBuf );
unsigned char* GetComBuf( unsigned short COM_No );
//void CommInit( short COM_No );
void CommRest( short COM_No );  		//�ָ�����ʼ״̬
unsigned short MP_FCS(unsigned short fcs,unsigned short c);
unsigned short GetCheckCode_16BitFcs(unsigned char *pucData,unsigned short usDataLen);
void Put16BitFcs( unsigned char* Source, unsigned char* Dest, unsigned short Len );
void ClrRSNumber( unsigned char* ComBuf );
void ClrRSBuffPara( unsigned char* ComBuf );
void HDLC_RNRFrameProcess( unsigned char* ComBuf );
unsigned short GetFrameLength( unsigned char* ComBuf );
unsigned short GetAddrLength( unsigned char* ComBuf );
void FillHCS_FCS_Flag( unsigned char* ComBuf );
void HDLC_Ctrl(unsigned char* ComBuf);
void HDLC_InformationFrameProcess( unsigned char* ComBuf );
//void CreatFrameHead( unsigned char* ComBuf, unsigned short FrameLen, unsigned char SegmentFlag );
void CreatFrameHead( unsigned char* ComBuf, unsigned char ClientAddr, unsigned short FrameLen, unsigned char SegmentFlag );

//void CreatRRFrame( unsigned char* ComBuf, unsigned short RxdFrameNo );
void CreatRRFrame( unsigned char* ComBuf, unsigned char RxdFrameNo );
void CreatIFrame( unsigned char* ComBuf, unsigned char RxdFrameNo, unsigned char TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag );

//void CreatIFrame( unsigned char* ComBuf, unsigned short TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag, unsigned char FrameFlag );
void HDLC_RRFrameProcess( unsigned char* ComBuf );
unsigned short GetMaxInfoSize( unsigned short COM_No );
void LLCStateCycle( unsigned short COM_No );

short DLMS_CommModeRec( short COM_No );
void DLMS_CommMode( short COM_No );
void DLMS_CommInit(unsigned char COM_No);
void fnUART5_Init(unsigned char Cfg);

