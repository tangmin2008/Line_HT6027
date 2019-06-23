
#define VCARD			GPIOB_VCARD_CTRL
#define VCARD_SET		HT_GPIOB->PTSET |= GPIOB_VCARD_CTRL
#define VCARD_CLR		HT_GPIOB->PTCLR |= GPIOB_VCARD_CTRL
#define VCARD_OUTPUT	HT_GPIOB->PTDIR |= GPIOB_VCARD_CTRL

#define CARDRST			GPIOE_CARDRST
#define CARDRST_SET		HT_GPIOE->PTSET |= GPIOE_CARDRST
#define CARDRST_CLR		HT_GPIOE->PTCLR |= GPIOE_CARDRST
#define CARDRST_OUTPUT	HT_GPIOE->PTDIR |= GPIOE_CARDRST

#define VESAM			GPIOD_VESAM_CTL
#define VESAM_SET		HT_GPIOD->PTSET |= GPIOD_VESAM_CTL
#define VESAM_CLR		HT_GPIOD->PTCLR |= GPIOD_VESAM_CTL
#define VESAM_OUTPUT	HT_GPIOD->PTDIR |= GPIOD_VESAM_CTL

#define ESAMCS			GPIOG_ESAM_CS
#define ESAMCS_SET		HT_GPIOG->PTSET |= GPIOG_ESAM_CS
#define ESAMCS_CLR		HT_GPIOG->PTCLR |= GPIOG_ESAM_CS

#define ESAMCLK			GPIOG_ESAM_CLK
#define ESAMCLK_SET		HT_GPIOG->PTSET |= GPIOG_ESAM_CLK
#define ESAMCLK_CLR		HT_GPIOG->PTCLR |= GPIOG_ESAM_CLK

#define ESAMMOSI		GPIOG_ESAM_MOSI
#define ESAMMOSI_SET	HT_GPIOG->PTSET |= GPIOG_ESAM_MOSI
#define ESAMMOSI_CLR	HT_GPIOG->PTCLR |= GPIOG_ESAM_MOSI

#define MS006			50		//50*0.25ms = 12.5ms

#define ICRS_MinSize  	5
#define ICRTx_MaxSize  ICRS_MinSize+285	//200
#define ICRRx_MaxSize  ICRS_MinSize+285

#define PSAM		0
#define ESAM		1

#define ENCRYPT		0		//����
#define DECRYPT		1		//����
#define MACOPRT		2		//MAC����

#define FILE_No1	0x01	//�ļ�1
#define FILE_No2	0x02	//�ļ�2
#define FILE_No3	0x03	//�ļ�3
#define FILE_No4	0x04	//�ļ�4
#define FILE_No5	0x05	//�ļ�5
#define FILE_No6	0x06	//�ļ�6
#define FILE_No7	0x07	//�ļ�7
#define FILE_No8	0x08	//�ļ�8
#define FILE_No9	0x09	//�ļ�9
#define FILE_No10	0x0A	//�ļ�10
#define FILE_No15	0x0F	//�ļ�15

#define CARD_BUYEC		0x01	//�û����翨
#define CARD_PREPARE	0x06	//Ԥ�ÿ�
/*
#define ICALARM_10	0x10	//��֤����
#define ICALARM_11	0x11	//ESAM��֤ʧ��
#define ICALARM_12	0x12	//�ͻ���Ų�ƥ��
#define ICALARM_13	0x13	//��ֵ��������
#define ICALARM_14	0x14	//���糬�ڻ�
#define ICALARM_15	0x15	//�ֳ��������ÿ��Ա����Ѿ�ʧЧ
#define ICALARM_16	0x16	//�޸���Կ����
#define ICALARM_17	0x17	//δ��Ǧ���
#define ICALARM_18	0x18	//��ǰ�ο�
#define ICALARM_19	0x19	//�޸ı�ſ������ÿ��޿����ŷ��䣩
#define ICALARM_20	0x20	//�޸���Կ������Ϊ��
#define ICALARM_21	0x21	//����ѿ����������������Ѿ��������ı�ƣ�
#define ICALARM_22	0x22	//���δ�������û������뻹δ�������ı�ƣ�
#define ICALARM_23	0x23	//���𻵻������Ϳ����緭�忨������Ƭ�ȣ�
#define ICALARM_24	0x24	//��Ƶ�ѹ���ͣ���ʱ����IC�����ܻᵼ�±�Ƹ�λ����IC����
#define ICALARM_25	0x25	//���ļ���ʽ���Ϸ�������֡ͷ��֡β��У���
#define ICALARM_26	0x26	//�����ʹ�
#define ICALARM_27	0x27	//�Ѿ����������¿��������¿�������д�������ݣ�
#define ICALARM_28	0x28	//�������󣨿�Ƭѡ���ļ������ļ���д�ļ���ȣ�
*/

#define ICALARM_31	0x31	//������			//�¹���	//13.09.24
#define ICALARM_32	0x32	//��Ч��Ƭ			//�¹���	//13.09.24
#define ICALARM_33	0x33	//�����ƥ��		//�¹���	//13.09.24
#define ICALARM_34	0x34	//�۵��������		//�¹���	//13.09.24
#define ICALARM_35	0x35	//�Ӵ�����			//�¹���	//13.09.24
#define ICALARM_36	0x36	//���ڻ�			//�¹���	//13.09.24

#define ICALARM_10	ICALARM_32	//��֤����
#define ICALARM_11	ICALARM_31	//ESAM��֤ʧ��
#define ICALARM_12	ICALARM_33	//�ͻ���Ų�ƥ��
#define ICALARM_13	ICALARM_34	//��ֵ��������
#define ICALARM_14	ICALARM_36	//���糬�ڻ�
//#define ICALARM_15	0x15	//�ֳ��������ÿ��Ա����Ѿ�ʧЧ
#define ICALARM_16	0x00	//�޸���Կ����
//#define ICALARM_17	0x17	//δ��Ǧ���
#define ICALARM_18	ICALARM_35	//��ǰ�ο�
//#define ICALARM_19	0x19	//�޸ı�ſ������ÿ��޿����ŷ��䣩
//#define ICALARM_20	0x20	//�޸���Կ������Ϊ��
#define ICALARM_21	ICALARM_34	//����ѿ����������������Ѿ��������ı�ƣ�
#define ICALARM_22	ICALARM_34	//���δ�������û������뻹δ�������ı�ƣ�
#define ICALARM_23	ICALARM_32	//���𻵻������Ϳ����緭�忨������Ƭ�ȣ�
#define ICALARM_24	ICALARM_31	//��Ƶ�ѹ���ͣ���ʱ����IC�����ܻᵼ�±�Ƹ�λ����IC����
#define ICALARM_25	ICALARM_34	//���ļ���ʽ���Ϸ�������֡ͷ��֡β��У���
#define ICALARM_26	ICALARM_32	//�����ʹ�
#define ICALARM_27	ICALARM_34	//�Ѿ����������¿��������¿�������д�������ݣ�
#define ICALARM_28	ICALARM_35	//�������󣨿�Ƭѡ���ļ������ļ���д�ļ���ȣ�

//������Ϣ�֣�
//ICALARM_31	//������															//�¹���	//13.12.03
#define ICERROR_01	0x01	//����ѹ����											//�¹���	//13.12.03
#define ICERROR_02	0x02	//����ESAM����											//�¹���	//13.12.03
#define ICERROR_03	0x03	//ESAM��λ��ESAM�𻵻�δ��װ��						//�¹���	//13.12.03

//ICALARM_32	//��Ч��Ƭ															//�¹���	//13.12.03
#define ICERROR_04	0x04	//��Ƭ��λ�����𻵻������Ϳ����練�忨������Ƭ�ȣ�	//�¹���	//13.12.03
#define ICERROR_05	0x05	//�����֤��ͨ�ųɹ��������Ĳ�ƥ�䣩					//�¹���	//13.12.03
#define ICERROR_06	0x06	//�ⲿ��֤��ͨ�ųɹ�������֤��ͨ����					//�¹���	//13.12.03
#define ICERROR_07	0x07	//δ���еĿ�Ƭ������Ƭʱ����6B00��						//�¹���	//13.12.03
#define ICERROR_08	0x08	//�����ʹ�												//�¹���	//13.12.03
#define ICERROR_09	0x09	//��Ƭ����Ϊ��Ȩ����Կ״̬��Ϊ��Կʱ�����Ԥ�ÿ���		//�¹���	//13.12.03
#define ICERROR_10	0x0A	//MACУ�����											//�¹���	//13.12.03

//ICALARM_33	//�����ƥ��															//�¹���	//13.12.03
#define ICERROR_11	0x0B	//��Ų�һ��											//�¹���	//13.12.03
#define ICERROR_12	0x0C	//�ͻ���Ų�һ��										//�¹���	//13.12.03
#define ICERROR_13	0x0D	//�����кŲ�һ��										//�¹���	//13.12.03

//ICALARM_34	//�۵��������															//�¹���	//13.12.03
#define ICERROR_14	0x0E	//��Ƭ�ļ���ʽ���Ϸ�									//�¹���	//13.12.03
#define ICERROR_15	0x0F	//���翨����δ������									//�¹���	//13.12.03
#define ICERROR_16	0x10	//��������δ������										//�¹���	//13.12.03
#define ICERROR_17	0x11	//�����������											//�¹���	//13.12.03
#define ICERROR_18	0x12	//�û�����д��Ϣ�ļ���Ϊ��								//�¹���	//13.12.03

//ICALARM_35	//�Ӵ�����															//�¹���	//13.12.03
#define ICERROR_19	0x13	//������ƬͨѶ����										//�¹���	//13.12.03
#define ICERROR_20	0x14	//��ǰ�ο�												//�¹���	//13.12.03

//ICALARM_36	//���ڻ�															//�¹���	//13.12.03
#define ICERROR_21	0x15	//ʣ����ڻ�										//�¹���	//13.12.03

#define RELAY_ON	0x00	//�ű��̵ּ����պ�
#define RELAY_OFF	0x01	//�ű��̵ּ����Ͽ�

//#define CYC5000		5000	//�ȴ����ֽ�		//V6
//#define CYC5000		60000	//�ȴ����ֽ�		//V6
//#define CYC60000	60000	//�ȴ����ֽ�		//V6
#define CYC5000		65000*4	//�ȴ����ֽ�		//V6
//#define CYC60000	65000*4	//�ȴ����ֽ�		//V6
#define CYC60000	65000*8	//�ȴ����ֽ�		//V6	//15.09.26
#define CYC120000	65000*8	//�ȴ����ֽ�		//V6
//#define CYC120000	65000*5	//�ȴ����ֽ�		//V6
#define CYC240000	65000*16	//�ȴ����ֽ�		//V6
//#define CYC240000	65000*12	//�ȴ����ֽ�		//V6

short Card_CoolRST( short CARD_TYPE );
void Card_Release( short CARD_TYPE );
void GetAndCompareESAMPara( void );
void RelayDrive( unsigned char DriveType );
void JudgeSTA_State( void );								
void OperateRelay( void );

short SelectDF( unsigned short DirFile );
short RecDataDelay( short CARD_TYPE, unsigned long Cycle, unsigned short RecLength );
//short Card_CoolRST( short CARD_TYPE );
short Card_HotRST( short CARD_TYPE );
//void Card_Release( short CARD_TYPE );
void ICDataSend( short CARD_TYPE, short Length, unsigned long Cycle, unsigned short RecLength );
short DisperseAuthentication( short FileNo, unsigned char* Dest, unsigned char* Source );
short GetChallenge( short CARD_TYPE, unsigned char* Dest, unsigned short Length );
short GetResponse( short CARD_TYPE, unsigned char* Dest, short Length );
short ExternalAuthentication( short CARD_TYPE, unsigned char* Source, short FileNo, short Length );
short EnDecrypt( short CARD_TYPE, unsigned char* Dest, unsigned char* Source, short OperateType ,short FileNo, short Length );
short InternalAuthentication( short FileNo, unsigned char* DisperseGene );

short ESAMExternalAuthentication( short FileNo, short FrameType );
short PSAMExternalAuthentication( short FileNo, unsigned char* DisperseGene );
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length );
short UpdateBinary( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length );
void ICProce( void );
short GetFullFrame( short FrameType );
short Card_Purview( short FrameType );
void ICComm_Ctrl( short FrameType );

void IC_BuyECNumber( void );//�û����еı�����ÿ�
void IC_BuyEC( void );		//���翨
void IC_Check( void );		//��鿨
void IC_Relay( void );		//�̵�����⿨
void IC_IncEC( void );		//���ӵ�����
void IC_Prepare( void );	//Ԥ�ÿ�
void IC_ModifyNmber( void );//�޸ı�ſ�
void IC_Emergency( void );	//Ӧ����	
void IC_ModifyKey( void );	//Ӧ����	
void IC_FastTest( void );	//���ٲ��Կ�	
void IC_SegSet( void );		//����ʱ�����ÿ�
//short ReadRecord( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length );
short ReadRecord( short CARD_TYPE, unsigned char* Dest );
short UpdateRecord( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length );
short Write_ESAMBurse( void );
void SwapFrameData( unsigned char* Dest, unsigned char* Source, short ItemCount );
short UpdateKey( unsigned char* Source, short FileNo );

void Clr_RemainEC( void );
void Clr_EC( void );
void Clr_PrePara( void );
void Clr_Event( void );

void Clr_AccountFlag( void );
void RS_Decrypt( unsigned char* ComBuf );

void GetReOverEC( unsigned char* ReDest, unsigned char* OverDest );
void GetICECState( unsigned char* ReDest, unsigned char* OverDest );
void GetECState( void );
void Clr_ReOverEC( void );
//short InDecrease( short CARD_TYPE, unsigned char* Source, short StartAddr, unsigned char Ins );
short InDecrease( short CARD_TYPE, unsigned char* Source );
unsigned char GetInDeData( unsigned char* Dest, unsigned char* Source );
short Write_ESAMPeriod( void );
//void RelayDrive( unsigned char DriveType );

short DisperseAuthenticationCardNo( short FileNo, unsigned char* DisperseGene );
short DisperseAuthenticationRand( unsigned char* Dest, unsigned char* Source );

short ReadBinaryIncMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short S_StartAddr );
short UpdateBinaryIncMAC( short CARD_TYPE, unsigned char* Source, unsigned char* DisperseGene );
short ReadBinaryRecMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short D_FileNo, short S_StartAddr );		//�¹���	//13.11.23
short UpdateBinaryRecMAC( short CARD_TYPE, unsigned char* Source, short D_FileNo );

short ReadBinaryMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short S_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene );
short ReadRecMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene );
short UpdateBinaryMAC( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length, short W_Type, unsigned char* DisperseGene );

void CommInitIC( short CARD_TYPE );
void CYCDelay(unsigned short cycle); /*����cycleΪϣ������������1/6��*/ 

void BCD3_HEX1( unsigned char* Dest, unsigned short Addr );
void GetBackBuyECFile1( unsigned char* Dest );
void GetLSEC( unsigned char* Dest, unsigned short Mon_No );
void GetBackBuyECFile2( unsigned char* Dest );
void SwapSegFeeFile( unsigned char* Source );
void SwapSegFeeFile( unsigned char* Source );
void FillSegFee( unsigned char* Dest, unsigned short DNumber, unsigned short SNumber );
short SegFeeFileProsess( unsigned char* Source, unsigned short SFileNo );

void CreateTripStartRec( unsigned char TripType );
void CreateTripEndRec( void );
void CreateILInsertRec( void );
void CreatICPrgRec( unsigned char CardType );

void IC_IOState( void );
void AddBuyECSumBCD( unsigned char* Source ); 		//�ۼ��ۼƹ�����

void CreatBuyECRec( void );						//���������¼

void IncConsumECSum( unsigned short Num, short PriceType );		//15.07.31
void GetFRAMConsumECSum( unsigned char* Dest );
unsigned long GetCurrentECPrice( void );
unsigned short GetCurrentStepNo( short Type  );
unsigned long GetCurrentStepPrice( void );
void GetCycleInsideECPp0( unsigned char* Dest  );
//void GetAndCompareESAMPara( void );
unsigned long GetDecData( unsigned char* Source );
//void OperateRelay( void );
short WriteESAMRunInfoFile( void );
void SaveBuyECState( void );

short WriteESAMRunInfoFilePassWordState( unsigned char* Source );

void KeyRelayOn( void );									//11.06.27

short CompareEncrypt( unsigned char* Dest, unsigned char* Source, unsigned short Length );		//�¹���

short SelectDF_ESAM( unsigned short DirFile );			//�¹���
//void JudgeSTA_State( void );						//�¹���			

short UpdateBinaryDecMAC( short CARD_TYPE, unsigned char* Source );				//�¹���	//13.11.27
void DecBuyECSumBCD( unsigned char* Source ); 		//�˷��ۼƹ�����		//�¹���	//13.11.27

void GetYearCycleInsideECPp0( unsigned char* Dest );						//�¹���	//13.11.28
void ProcessICAlarmFlg( void );												//�¹���	//13.12.03
void InitialAbnormalInsertCardMeassage( void );								//�¹���	//13.12.02
short JudgeMeterVolt80Un( void );												//�¹���	//13.12.03
void SetJudgePermitFlag( void );											//�¹���	//13.12.10			
void ClearJudgePermitFlag( void );											//�¹���	//13.12.10			

unsigned long GetCurrentFeePrice( void );		                             //�¹���    //13.12.12
void ExitPreTripState( void );													//11.08.19
unsigned long GetBuyECSum( void ); 							//ȡ�ۼƹ�����
