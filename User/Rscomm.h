#ifndef __RSCOMM_H__
#define __RSCOMM_H__
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

#define Baut_19200L	0xDA
#define Baut_19200H	0x00
//#define Baut_19200M	0x55
#define Baut_19200M	0x00

/*
#define Baut300  0x02
#define Baut600  0x04
#define Baut1200 0x08
#define Baut2400 0x10
#define Baut4800 0x20
#define Baut9600 0x40
*/
//#define Baut300  0x01
//#define Baut600  0x02
//#define Baut1200 0x04
//#define Baut2400 0x08
//#define Baut4800 0x10
//#define Baut9600 0x20
//#define Baut19200 0x40

#define Baut300  0
#define Baut600  1
#define Baut1200 2
#define Baut2400 3
#define Baut4800 4
#define Baut7200 5
#define Baut9600 6
#define Baut19200 7


#define BitTime_1M 	0x36A
#define BitTime_4M 	0xDA7
//#define BitTime_1M 	0xDA7
#define HalfBitTime_1M 	0x1B5
#define HalfBitTime_4M 	0x6D3
//#define HalfBitTime_1M 	0x6D3

#define	MS10_4M	12096
#define MS10_1M 2524

//#define RS_MaxSize  99		//�ȴ����յ�����ֽ���
#define RS_MinSize  	12
#define RTx_MaxSize  RS_MinSize+207	//200	//N645	
#define RRx_MaxSize  RS_MinSize+207    //N645	

//#define CPTInsItem 	27		//CDParaTab ��˲ʱֵ���� 
//#define MS500		32		//500MS		//V1001
#define MS500		33		//500MS		//V1001
#define TM485		6		//500MS
#define MS4000		1000		//4000MS
//#define MS3000		192		//128*16 = 2S
#define MS3000		240		//128*16 = 2S

#define PASSWDERRCNT	0x03

#define CALIBCOUNT	60
#define CALIBCOUNT1	61

#define SAMPCYC		5		//AD���������������
//#define SAMPCYC		20		//AD���������������

#define CURRENTAD	0		//������������ص�ѹ�ɼ�
#define BATVOLTAD	1		//������ص�ѹ�ɼ�
//#define BATADCYC	128		//��ص�ѹ������			//V32
#define BATADCYC	64		//��ص�ѹ������			//V32

//#define PWUPTIME	75		//EC

#define	NAK		0x15
#define	ACK		0x06
#define	SOH		0x01
#define LF		0x0A
#define	CR		0x0D
#define STX		0x02
#define ETX		0x03
#define EOT		0x04
#define StartCode 	0x2F	//"/"
#define EndCode	0x21		//"!"
#define REQ		0x3F		//"?"

///////////////////////////
//     ComBuf define     //
///////////////////////////
//#define RS_HeadPtr   0
//#define RS_AddrPtr   1
//#define RS_Head0Ptr  7
//#define RS_CtlPtr    8
//#define RS_LenPtr    9

#define RS_Cmd			10
#define RS_CmdLPtr   10
#define RS_CmdHPtr   11
#define RS_DPtr      12
/*
#define Rs_Head     	Comm.Buf[0]
#define Rs_Addr     	Comm.Buf[1]
#define Rs_Addr0    	Comm.Buf[1]
#define Rs_Addr1    	Comm.Buf[2]
#define Rs_Addr2    	Comm.Buf[3]
#define Rs_Addr3    	Comm.Buf[4]
#define Rs_Addr4    	Comm.Buf[5]
#define Rs_Addr5    	Comm.Buf[6]
#define Rs_Head0    	Comm.Buf[7]
#define Rs_Ctl      	Comm.Buf[8]
#define Rs_Len      	Comm.Buf[9]

#define Rs_IDL    	Comm.Buf[10]
#define Rs_IDH     	Comm.Buf[11]
#define Rs_Data    	Comm.Buf[12]
#define Rs_WData    	Comm.Buf[16]
*/

#define NRs_ID0Ptr  		10
#define NRs_ID1Ptr  		11
#define NRs_ID2Ptr  		12
#define NRs_ID3Ptr  		13
#define NRs_RDataPtr 		14
#define NRs_PassWPtr 		14
#define NRs_OPCodePtr 		18
#define NRs_WDataPtr    	22			//N645	
/////////////////////////
// Control byte define //
/////////////////////////
#define NRS_CTL_ExtRead	 0x0D		//�Զ����
#define NRS_CTL_ExtWrite 0x0E		//�Զ���д

#define NRS_CTL_AdjTime  0x08			//N645
#define NRS_CTL_Read     0x11			//...
#define NRS_CTL_ReadSubs 0x12			//...	
#define NRS_CTL_ReadAdr  0x13			//...
#define NRS_CTL_Write    0x14
#define NRS_CTL_WrMetID  0x15
#define NRS_CTL_Freeze   0x16
#define NRS_CTL_ChgBaut  0x17
#define NRS_CTL_ChgPswd  0x18
#define NRS_CTL_RstMD    0x19
#define NRS_CTL_MeterClr 0x1A
#define NRS_CTL_EventClr 0x1B			//N645
#define NRS_CTL_Trip	 0x1C			//N645	//����բ������������
#define NRS_CTL_MutiPort 0x1D			//N645  //�๦�ܿ�
#define NRS_CTL_SAuRW 	 0x03			//��ȫ��֤��д			
#define NRS_CTL_ZBReadAdr  0x1F			//�����Զ�����ز�����ַ

/*
#define	NWRITE	0x8000													//N645	
#define	NREAD	0x4000													//...
#define	NBACK	0x2000			//���ޱ��ݣ�0���ޱ��� 1���б���

#define NPARAM		0x0100		//״̬��������ʱ��
#define NOTHER		0x0200		//����
#define NDATABLK	0x0300		//���ݿ�
#define NATT702X	0x0400		//ATT702X����
#define NLOAD		0x0500		//��������
#define NEVENT		0x0600		//ѭ���¼���¼
*/
#define NComm_Type 0x0F00		//ͨѶ�����							//N645




#define Rs_HeadPtr     	0
#define Rs_AddrPtr     	1
#define Rs_Addr0Ptr    	1
#define Rs_Addr1Ptr    	2
#define Rs_Addr2Ptr    	3
#define Rs_Addr3Ptr    	4
#define Rs_Addr4Ptr    	5
#define Rs_Addr5Ptr    	6
#define Rs_Head0Ptr    	7
#define Rs_CtlPtr      	8
#define Rs_LenPtr      	9

#define Rs_IDLPtr  		0
#define Rs_IDHPtr  		1
#define Rs_DataPtr 		2
#define Rs_WDataPtr    	1

#define RsZj_Head0Ptr   0
#define RsZj_LenPtr1    1
#define RsZj_LenPtr2    2
#define RsZj_Head1Ptr   3
#define RsZj_AddrPtr    4
#define RsZj_IDLPtr 	5
#define RsZj_IDHPtr 	6
#define RsZj_DataPtr	7

//#define Rs_CRC      	Comm.Buf[RS_Cmd+Rs_Len]
//#define Rs_End      	Comm.Buf[RS_Cmd+Rs_Len+1]

// ComInfo define
#define EN_WR			0xC000
#define EN_W			0x8000
#define EN_R			0x4000

#define RS_REG			0x3800
#define REG_RAM		0*0x0800
#define REG_IFlash 	1*0x0800
#define REG_Eeprom	2*0x0800
#define REG_DFlash	3*0x0800
#define REG_CMD 		4*0x0800

#define RS_LEN			0x00FF

//efine RS_CMD			0x0700

#define CMD_CLK1				0x0000
#define CMD_CLK2  			0x0001

#define CMD_LP_ReadRec   	0x0010
#define CMD_LP_Channel   	0x0011
#define CMD_LP_RegCode   	0x0012
#define CMD_LP_TimeInv   	0x0013
#define CMD_LP_MaxRecNum  	0x0014
#define CMD_LP_ReadPtr   	0x0015
#define CMD_LP_RecNum   	0x0016
#define CMD_LP_CRecTime   	0x0017

#define CMD_EL_ReadRec   	0x0020
#define CMD_EL_ReadData   	0x0021
#define CMD_EL_EvenCode   	0x0022
#define CMD_EL_MaxRecNum  	0x0023
#define CMD_EL_ReadPtr   	0x0024
#define CMD_EL_RecNum   	0x0025

/////////////////////////
// Control byte define //
/////////////////////////
#define RS_CTL_Read     0x01
#define RS_CTL_ReadCont 0x02
#define RS_CTL_ReRead   0x03
#define RS_CTL_Write    0x04
#define RS_CTL_AdjTime  0x08
#define RS_CTL_WrMetID  0x0A
#define RS_CTL_ChgBaut  0x0C
#define RS_CTL_ChgPswd  0x0F
#define RS_CTL_RstMD    0x10

///////////////////////
// State byte define //
///////////////////////
#define RS_State_IVData	0x01		//�Ƿ�����or �����Ƿ�
#define RS_State_IVID	0x02		//���ݱ�ʶ��
#define RS_State_PswE	0x04		//�����  
#define NRS_State_Baut	0x08		//ͨѶ���ʲ����޸�  
#define RS_State_ZoneOver	0x10	//ʱ������
#define RS_State_SegOver	0x20	//ʱ������
#define RS_State_FeeOver	0x40	//��������

#define CHANNEL0	0				//����ͨ��0
#define CHANNEL1	1				//����ͨ��1
#define CHANNEL2	2				//����ͨ��2
#define CHANNEL3	3				//����ͨ��3

#define ATT7022Num	20

//#define ComNum	354
//#define ComNum	346+6+35+19
//#define ComNum	478
//#define SEGNUMBER	24
#define SEGNUMBER	14
#define ZONENUMBER 14
#define HOLIDAYNUMBER		20

#define BATONDLY	30

//#define LOADLENGTH	68
#define LOADLENGTH	70

#define	READ	0x4000
#define	WRITE	0x8000

//�α������
#define BCD		0x2000		//������ΪBCD��

//�¼����
#define FLASH	0x8000
#define EEPROM	0x4000
#define EVOTHER	0x3000		//RAM
#define EVPARAM	0x2000		//����
#define EVENO	0x1000		//�¼���
#define RS_EVLEN	0x07FF
#define EVComm_Type 0x3000	//ͨѶ�����


#define MAXPOW	0x0100		//�������
#define POWTM	0x0200		//�������ʱ��
#define PARAM	0x0300		//״̬��������ʱ��
#define OTHER	0x0400		//����
#define DATABLK	0x0500		//���ݿ�
#define ATT702X	0x0600		//ATT702X����
#define SPECFUN	0x0700		//������Ч

#define Comm_Type 0x0700	//ͨѶ�����

#define ZJNULL		0x0100		//����������
#define ZJPOWTM		0x0200		//����������ʱ��
#define ZJPARAM		0x0300		//���״̬��������ʱ��
#define ZJOTHER		0x0400		//�������
#define ZJDATABLK	0x0500		//������ݿ�
//#define ZJATT702X	0x0600		//���ATT702X����
//#define ZJSPECFUN	0x0700		//���������Ч
#define ZJRAM		0x0600		//���������Ч
#define ZJDATABLKM	0x0700		//�������ݿ�

#define ZJComm_Type 0x0700	//���ͨѶ�����

//#define ZJRECTIME	10		
#define ZJRECTIME	5		
//#define ZJRECTIME	4			//V1000		

#define ZONETAB	0x0000		//����ʱ����
#define HDTAB	0x0000		//�����������ձ�
#define SEGTAB	0x0800		//����ʱ�α�

#define TLDATE	0x1000		//��������
#define TLTIME	0x1800		//����ʱ��
#define TLPOINT	0x2000		//����ָ��

#define DXPOINT	0x0000		//�����¼ָ������
#define LSPOINT	0x0001		//��ʷ������¼ָ������

//#define MissPhase	0	//����
//#define LastEc		1	//��ʷ����

/*
#define PhFail_No		0	//����
#define CUFail_No		1	//����
#define AmperOver_No	2	//����
#define Rev_No			3	//����
#define PhBkSta_No		4	//ʧѹ��ʼ
#define PhBkEnd_No		5	//ʧѹ����
#define LastEc_No		6	//��ʷ����

#define	APhBk_No		4	//A��ʧѹ
#define	BPhBk_No		5	//B��ʧѹ
#define	CPhBk_No		6	//C��ʧѹ
*/
/*
#define AmperOver_No	0	//����
#define Rev_No			1	//����
#define PhAFail_No		2	//A�����
#define PhBFail_No		3	//B�����
#define PhCFail_No		4	//C�����
#define CuAFail_No		5	//A�����
#define CuBFail_No		6	//B�����
#define CuCFail_No		7	//C�����
#define LastEc_No		8	//��ʷ����
#define PrgRecd_No		9	//��̼�¼
#define PdownRecd_No	10	//��̼�¼
*/
											//V1000

/*											//V1001
#define PhFail_No		0	//����
#define AmperOver_No	1	//����
#define Rev_No			2	//����
#define PhAFail_No		3	//A�����
#define PhBFail_No		4	//B�����
#define PhCFail_No		5	//C�����
#define CuAFail_No		6	//A�����
#define CuBFail_No		7	//B�����
#define CuCFail_No		8	//C�����
#define APhBk_No		9	//A��ʧѹ		//V1000
#define BPhBk_No		10	//B��ʧѹ
#define CPhBk_No		11	//C��ʧѹ
#define LastEc_No		12	//��ʷ����
#define PrgRecd_No		13	//��̼�¼
#define PdownRecd_No	14	//�µ��¼
*/

#define Current		0	//��ǰ��
#define Next		-1	//��һ��

#define M32(adr)    (*((uint32_t *) (adr)))

//short Proc_Comm(void);
//void CommInit(void);
void CommInit( short COM_No );
//void CommRest(void);
void CommRest( short COM_No );
//short GetXorCheck( unsigned char* XorCheck, short* CheckAds);
//void CommMode(void);
void CommMode( short COM_No );
//short CommModeRec(void);
//short Proc_Read(void);	
//short Proc_Write(void);	
//short Read_ATTValue( unsigned char Cmd, unsigned char* Data );
void DayInc( unsigned char* Data );
//void DayDec( unsigned char* Data );
//unsigned short SumLoadAds(unsigned char* Dest );
//void LoadBCD_ASC( unsigned char* Dest, unsigned char* Src, unsigned char* Ptr, short Length);
//short LoadTimeChk(void);
//short GetRecordAds( unsigned char* Dest, unsigned short PtrAds );
//short JustDay( unsigned char* Dest, short i, short PtrAds );
//short GetRecordData( unsigned char* Point,short Type);
//short GetLoadData(unsigned char* Point, short Type);
//unsigned char Com_Read(void);
unsigned char Com_Read(unsigned char* ComBuf);
//unsigned char Com_Write(void);
unsigned char Com_Write(unsigned char* ComBuf);
//void Proc_Read( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len);
//void Proc_Read( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf);
void Proc_Read( short ItemAds, unsigned long Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf);
//short GetSegNum( void );
//unsigned char ComAdjWrite(void);
unsigned char ComAdjWrite(unsigned char* ComBuf ,unsigned short Devads);
//unsigned short PtAddrChk( unsigned short PtNum, short Type );
long PtAddrChk( unsigned short PtNum, short Type );

//short GetSegNum( unsigned char* Source );
short GetSegNum( unsigned char* Source, short Number );
short GetEC_MD( unsigned char* Dest, unsigned short* DataLength, unsigned short Com_Id );
short GetP( short* Item,unsigned short Com_Id );
short GetQ( short* Item,unsigned short Com_Id );
short Addr_9_9( unsigned char* Source );
short MeterAds_Comp( unsigned char* Source );
//void Proc_Write( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
//unsigned char Proc_Write( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
unsigned char Proc_Write( short ItemAds, unsigned long Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
void Clr_E2PData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle );
short GetLSEC_MD( unsigned char* Dest, unsigned short* DataLength, unsigned short Com_Id );
void GetUserCode( void );

void RS1SendOpen( void );
void RS2SendOpen( void );
short GetEC2( unsigned char* Dest,unsigned short Com_Id );

#if( ZjProtocol	== YesCheck )		// ���㽭ʡͨѶ��Լ
void ZjStateCycle( short COM_No );
void ContactZj( short COM_No );
//void ZjProSend( short COM_No, unsigned char* ComBuf, unsigned char* ZjState );
void ZjProSend( short COM_No, unsigned char* ComBuf);
void JudgeProtocol( short COM_No );
short ZjMeterAds_Comp( unsigned char* Source );
void ZjComm_Ctrl(unsigned char* ComBuf);
short ZjGetComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComBuf );
unsigned char ZjCom_Read(unsigned char* ComBuf);
void ZjProc_Read( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
#else
#endif
short GetBandRate( unsigned char BandRate );
short BuffDataIsZero( unsigned char* Source, short Length );
//short JustSeg( unsigned char* Source, short Number );
unsigned char JustSeg( unsigned char* Source, short Number );
unsigned char JustHourM( unsigned char* Source );
unsigned char JustDayH( unsigned char* Source );
unsigned char JustMonD( unsigned char* Source );
unsigned char JustZone( unsigned char* Source, short Number );
unsigned char JustHoliday( unsigned char* Source, short Number );

//void ClrEventData( unsigned char* ComBuf, unsigned short StartNo, unsigned short EndNo );

short NGetECComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
void NProc_ReadEC( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
short NGetMDComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
void NProc_ReadMD( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
//short NGetRealComVar( unsigned char* RAddr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
short NGetRealComVar( unsigned char** RAddr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
void NProc_ReadReal( short ItemAds, unsigned char* Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
unsigned char NCom_ReadSubs(unsigned char* ComBuf);
unsigned char NProc_ReadSubsLoad( unsigned char* ComBuf );
unsigned char NCom_Read(unsigned char* ComBuf);
short NGetEveComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
unsigned char NProc_ReadEve( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
short NGetParaComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
void NProc_ReadPara( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
short NGetDJComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
unsigned char NProc_ReadDJ( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
unsigned char NCom_Write(unsigned char* ComBuf );
unsigned char NProc_Write( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );

void Clr_FRAMData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle );

unsigned char NCom_ExtWrite(unsigned char* ComBuf );
unsigned char NProc_WriteExt( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
unsigned char NCom_ExtRead( unsigned char* ComBuf );
short NGetExtComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComBuf );
void NProc_ReadExt( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );

short GetCOM_No( unsigned char* ComBuf );
unsigned char NProc_ReadLoad( unsigned char* ComBuf );
unsigned char NProc_ReadSubsDJ( unsigned char* ComBuf );
unsigned char NProc_ReadSubsEve( unsigned char* ComBuf );

void MeterClrCycle( void );
unsigned char JudgeValid( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
unsigned char GetTXDelay( unsigned char* Source );
short MeterAds_Match( unsigned char* Source );

unsigned char JudgeFreezeDateTime( unsigned char* Source );
void MDClrCycle( void );
unsigned char ComAdjRead(unsigned char* ComBuf );

unsigned short GetRecItemStartAddr( unsigned short EventNo, unsigned short ItemNo );
void ClrRecNullItemContent( unsigned char* Dest, unsigned short EventNo, unsigned char SItemNo );

short NB1GetEveComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
unsigned char NB1Proc_ReadEve( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
void GetLSCycleEC( unsigned char* Dest, unsigned short Addr, unsigned char Cycle );
void GetEveryCycleInsideEC( unsigned char* Dest, unsigned char L1Cycle, unsigned char L2Cycle );			//�¹���
void GetCycleInsideEC( unsigned char* Dest, unsigned char L1Cycle, unsigned char L2Cycle );
//void CreatPrgRec( unsigned char* Source, unsigned short Info );
void CreatPrgRec( unsigned char* Source, unsigned short Info, unsigned char* OperateCode, short Type );
short CreatSinglePrgRec( unsigned long ComID, unsigned short Addr );
void CreatSegPrgRec( void );
void CreatZonePrgRec( void );
void CreatHolidayPrgRec( void );
void CreatSaveDayPrgRec( void );

unsigned char JudgeHourM( unsigned char* Source );
unsigned char JudgeDayH( unsigned char* Source );
unsigned char JudgeMonD( unsigned char* Source );
unsigned char JudgeSeg( unsigned char* Source, unsigned short Length );
unsigned char JudgeZone( unsigned char* Source, unsigned short Length );

unsigned char NProc_ReadHourDJ( unsigned char* ComBuf );
void WritePassWd_Err( unsigned short Addr, unsigned char Temp );

void GetYearCycleInsideEC( unsigned char* Dest, unsigned char L1Cycle, unsigned char L2Cycle );			//�¹���

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
void UpdateEventUpState( unsigned short BitSite );			//�¹���	//14.04.26
void ClearEventUpState( unsigned short BitSite );				//�¹���	//14.04.26
short AllResetBitIsAlreadyUpState( unsigned char* Source );	//�¹���	//14.06.21
void CommResetEventUpState( unsigned char* Source );		//�¹���	//14.04.26
void ResetEventUpStateSend( unsigned char* Source );		//�¹���	//14.06.21
void ClearAllEventUpState( void );							//�¹���	//14.04.26
void ClearEventUpStateSendBit( void );						//�¹���	//14.04.28
short GetEventUpAddCntFrame( unsigned char* Dest );			//�¹���	//14.04.26
void CreatEventUpSubsFrame( unsigned char* ComBuf );		//�¹���	//14.04.26
//void MatchEventUpMode( void );							//�¹���	//14.04.26	//14.06.21
short IsMatchEventUpMode( short ByteNo, short BitNo );			//�¹���	//14.06.21
short IsOccurEventUpState( void );							//�¹���		
#else
void CreatEventUpSubsFrame( unsigned char* ComBuf );		//�¹���
short IsOccurEventUpState( void );							//�¹���		
#endif

short IsOccurEventUpSubsFrame( void );						//�¹���		

void DayDec( unsigned char* Data );							//�¹���
void MonthDec( unsigned char* Data );						//�¹���

short IsInfraredValid( unsigned char* ComBuf );				//�¹���
short NeedInfraredIdentity( unsigned long ComID );			//�¹���
short IsInfraredIdentityValidComID( unsigned char* ComBuf, unsigned char CtlByte, unsigned char* ComID  );		//�¹���
short IsInfraredIdentityValid( unsigned char* ComBuf, unsigned char CtlByte );									//�¹���

void MatchEventUpMode( void );					//�¹���	//13.10.10

short IsFactoryTestState( void );								//�¹���	//14.04.09		

#if ( ExactECRead == YesCheck )			//��ȡ��ȷ����	//�¹��� 	//14.04.25				
short IsExactECID( unsigned long ComID );					//�¹��� 	//14.04.25				
#endif

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
#if ( ModifyTimeSkipDelay == YesCheck )						//�¹���	//14.06.19
void SaveTimeBeforeModification( void );	//�¹���	//14.05.28
#if ( IOver30ALimitTrip	== YesCheck )		//�¹���	//14.04.25
void AdjustUp30ATimeCntAfterTimeModify( void );				//�¹���	//14.05.28
#endif
void AdjustClearUpStateDelayAfterTimeModify( void );		//�¹���	//14.05.28
#endif
#endif

#if ( FlashTypeSelect == YesCheck )		//֧��Flash��������		//�¹���	//14.06.04	
short IsFlashType32AndBelow( void );
short IsFlashType64( void );
#endif

short BroadC_Time( unsigned char* Source );
#endif

