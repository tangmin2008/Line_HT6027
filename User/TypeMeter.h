/*
#define U220_I1A3200	0x01
#define U220_I1A6400	0x02
#define U220_I5A800		0x03
#define U220_I5A1600	0x04
#define U220_I10A400	0x05
#define U220_I10A800	0x06
#define U220_I15A400	0x07
#define U220_I15A800	0x08
#define U220_I20A200	0x09
#define U220_I20A400	0x10
#define U220_I30A200	0x11
#define U220_I30A400	0x12
#define U57_I1A12800	0x13
#define U100_I1A6400	0x14
#define U100_I1A12800	0x15
#define U57_I1A20000	0x16
*/
#define U220_I0_3A__1_2A25600   0x01
#define U220_I1A__6A6400	    0x02
#define U220_I1_5A__6A6400	    0x03
#define U220_I5A__20A800	    0x04
#define U220_I5A__30A800	    0x05
#define U220_I5A__40A400	    0x06
#define U220_I10A__40A400	    0x07
#define U220_I10A__60A400	    0x08
#define U220_I20A__80A200	    0x09
#define U57_I0_3A__1_2A100000   0x10
#define U57_I1A__6A20000	    0x11
#define U57_I1_5A__6A20000	    0x12
#define U100_I0_3A__1_2A100000  0x13
#define U100_I1A__6A20000	    0x14
#define U100_I1_5A__6A20000	    0x15

#define U220_I5A__20A1200	    0x16
#define U220_I5A__40A600	    0x17
#define U220_I10A__40A600	    0x18
#define U220_I10A__50A600	    0x19
#define U220_I20A__80A320	    0x20
#define U220_I20A__100A240	    0x21
#define U220_I20A__120A240	    0x22
#define U220_I15A__60A400	    0x23

#define U220_I5A__50A400	    0x24
#define U220_I5A__60A400	    0x25
#define U220_I5A__50A600	    0x26

#define U220_I2_5A__10A3200	    0x27
#define U220_I30A__100A240	    0x28

#define U220_I10A__100A240	    0x29

#define U220_I5A__6A6400	    0x30

#define U220_I5A__40A800	    0x31

#define U220_I10A__100A300	    0x32


#define DAYLIGHTSAVE YesCheck 	//����ʱ����
#define ACTIVE_FEE   NoCheck 		//�������ʹ���

#define Phase3Wire4	0x00		//��������
#define Phase3Wire3	0x01		//��������
#define SinglePhase	0x02	    //����
#define Threephase 	0x03		//����
//#define Crypt_Soft     0x04
//#define Crypt_Hardware 0x05
#define MeterPhase  SinglePhase
#define METERTYPE   U220_I10A__100A300

//#define Crypt_Base Crypt_Hardware

#define LinkMode 	Phase3Wire4		////��������
//#define LinkMode 	Phase3Wire3		////��������

//#if ( LinkMode == Phase3Wire4 )							//�¹���		//13.08.30	

//#define METERTYPE	U220_I0_3A__1_2A25600   
//#define METERTYPE	U220_I1A__6A6400	    
//#define METERTYPE	U220_I1_5A__6A6400	    
//#define METERTYPE	U220_I5A__20A800	    
//#define METERTYPE	U220_I5A__30A800	    
//#define METERTYPE	U220_I5A__40A400	    
//#define METERTYPE	U220_I10A__40A400	    
//#define METERTYPE	U220_I10A__60A400	    
//#define METERTYPE	U220_I20A__80A200	    
//#define METERTYPE	U57_I0_3A__1_2A100000   
//#define METERTYPE	U57_I1A__6A20000	    
//#define METERTYPE	U57_I1_5A__6A20000	    
//#define METERTYPE	U220_I5A__40A600	    

//#define METERTYPE	U220_I2_5A__10A3200	    
//#define METERTYPE	U220_I5A__50A600	    

//#define METERTYPE	U220_I1_5A__6A6400	    //����		//CMC
//#define METERTYPE	U220_I5A__20A1200	    
//#define METERTYPE	U220_I5A__30A800	    
//#define METERTYPE	U220_I5A__40A600	    
//#define METERTYPE	U220_I5A__60A400	    
//#define METERTYPE	U220_I10A__40A600	    
//#define METERTYPE	U220_I10A__60A400	    
//#define METERTYPE	U220_I15A__60A400	    
////#define METERTYPE	U220_I10A__100A300  	//����  
//#define METERTYPE	U220_I10A__100A240  	//����  
//#define METERTYPE	U220_I20A__80A320	    //����
//#define METERTYPE	U220_I20A__100A240	    //����

//#define METERTYPE	U220_I5A__60A400	    //�¹������ܱ�		//�¹���		//13.08.30
//#define METERTYPE	U220_I1_5A__6A6400	    //�¹������ܱ�

//#define METERTYPE	U220_I5A__40A800	    //�������ܱ�
//#define METERTYPE	U220_I15A__60A400	    //�������ܱ�
//#define METERTYPE	U220_I20A__80A320	    //�������ܱ�

//#define METERTYPE	U220_I10A__40A600	    //�������ܱ�
//#define METERTYPE	U220_I1_5A__6A6400	    //�������ܱ�
//#define METERTYPE	U220_I10A__60A400	    

//#define METERTYPE	U220_I10A__40A600	    
//#define METERTYPE	U220_I10A__60A400	    
//#define METERTYPE	U220_I15A__60A400	    
//#define METERTYPE	U220_I20A__100A240	    
//#define METERTYPE	U220_I20A__80A320	    
//#define METERTYPE	U220_I30A__100A240	    
//#define METERTYPE	U220_I10A__100A240    
//#define METERTYPE	U220_I10A__100A300    

//#define METERTYPE	U220_I5A__6A6400	    //����

//#else															//�¹���		//13.08.30		
//#define METERTYPE	U100_I0_3A__1_2A100000  
//#define METERTYPE	U100_I1A__6A20000	    
//#define METERTYPE	U100_I1_5A__6A20000	    //�������ܱ�		//�¹���		//13.08.30
//#define METERTYPE	U100_I1_5A__6A20000	    //ɽ�����ܱ�
//#endif														//�¹���		//13.08.30	

//#define CMonLength		560			//��ʷ�³���
#define CMonECLength		822			//��ʷ�µ�������
//#define CMonMDLength		999			//��ʷ����������
#define CMonMDLength		936			//��ʷ����������
//#define FreezeVarLength		74			//˲ʱ�����᳤��
#define FreezeVarLength		77			//˲ʱ�����᳤��
#define FREEZE_LEN			CMonECLength+CMonMDLength+FreezeVarLength	//���᳤��
//#define E2P_PGLEN		0x2800		//���ݱ���ҳ����		//PWMD
//#define E2P_PGLEN		0x7000		//���ݱ���ҳ����		
#define FRAM_PGLEN		0x400		//���ݱ���ҳ����		

#define ProfEAds    0xA8	//�������� + �¼���¼ EEPROM 24C512
//�������������
#define DataEAds    0xA4	//���� EEPROM 24C512
//#define AdjEAds     0xA0	//У����� EEPROM 24C02
//�������������
//#define DataEAds    0xA0	//#define DataEAds    0xA4	//���� EEPROM 24C64
#define AdjEAds     0xA6	//#define AdjEAds     0xA0	//У����� EEPROM 24C04

#define	FMAds		0xA0	//��ǰ�������������� FM24C16	
#define RTCAds		0x64	//RTC8025ʱ��оƬ��ַ
#define TCN75Ads	0x9E	//�¶ȴ�������ַ
#define DS3231Ads	0xD0	//ʱ��оƬDS3231��ַ
#define PCF8578Ads  0x78    //PCF8578��ַ			//10.08.03
#define BU97950Ads  0x7C    //BU97950��ַ			//10.11.11
#define BR8564Ads   0x70    //BR8564��ַ			//12.07.18

#define BYTE4		0x04
#define BYTE5		0x05
#define BYTE6		0x06

#define ECDIGIT		BYTE4//��������λ��
#define DATETIME	0x0C

//#define PowerLoadCurveSta	0x00000000
//#define PowerLoadCurveEnd	0x00037000		//220K  6�����й�����
//#define PowerLoadCurveEnd	0x00005000		//220K  6�����й�����

//#define GeneralLoadCurveSta	0x00037000		//220K
//#define GeneralLoadCurveEnd	0x00100000		//804K  ��ͨ�������߷�Χ�����ݿ���
//#define GeneralLoadCurveEnd	0x0003B000		//512K  ��ͨ�������߷�Χ�����ݿ���

#define YesCheck	0x00
#define NoCheck		0x01

#define ADE7758		0x00	
#define ATT7022		0x01
#define ADE7878		0x02	
#define ATT7022E	0x03								//11.09.24
#define IDT90E32	0x04								//11.09.24
#define RN8207		0x05
#define RN8209		0x06
#define HT7017		0x07

//#define ATT7022C	0x03
//#define ATT7022D	0x04
//#define MEASCHIP	ADE7758	//ѡ�����оƬ����	
//#define MEASCHIP	ATT7022	//ѡ�����оƬ����	
//#define MEASCHIP	RN8209//ѡ�����оƬ����		
#define MEASCHIP	ATT7022E//ѡ�����оƬ����		

//#define MEASCHIP	IDT90E32//ѡ�����оƬ����	
//#define MEASCHIP	ADE7878	//ѡ�����оƬ����	
//#define MEASCHIP	ATT7022D//ѡ�����оƬ����	

#if ( MEASCHIP == IDT90E32 )							//11.09.25
#define IDT90E32AS	YesCheck		//IDTоƬΪ90E32AS
//#define IDT90E32AS	NoCheck		//IDTоƬΪ90E32
#else
#define IDT90E32AS	NoCheck			//IDTоƬΪ90E32AS
#endif

#if ( MEASCHIP == ATT7022E )
#define NEW7022E	YesCheck		//�°�7022EоƬ		//�¹���
#else
#define NEW7022E	NoCheck			//���°�7022EоƬ	//�¹���
#endif

#define CPUOutputRTC_Pluse YesCheck

//#define ZjProtocol	YesCheck		//���㽭ʡͨѶ��Լ
#define ZjProtocol	NoCheck		//���㽭ʡͨѶ��Լ

#define N645Protocol	YesCheck		//��645-2007ͨѶ��Լ	//N645
//#define N645Protocol	NoCheck		//��645-2007ͨѶ��Լ		//N645

#define PwOnLCDControl	YesCheck		//�ϵ��Һ��5V��Դ	
//#define PwOnLCDControl	NoCheck
	
#define LCD8259		YesCheck			//8259Һ��		������Һ��
//#define LCD8259	NoCheck				//��8259Һ��

//#define LCD7862		YesCheck		//7862Һ��		������Һ��
#define LCD7862	NoCheck					//��7862Һ��

//#define LCDDrive_PCF8578		YesCheck   //PCF8578	//10.08.03
#define LCDDrive_PCF8578		NoCheck   //PCF8578

//#define LCDDrive_BU97950		YesCheck   //BU97950	//10.11.11
#define LCDDrive_BU97950		NoCheck   //BU97950

#define LCDDrive_BR8564			YesCheck   //BR8564	//2012.07.03
//#define LCDDrive_BR8564			NoCheck   //BR8564	//2012.07.03

//#define AuxPWFailCheck	YesCheck		//������Դ���0.2S��0.5S������
#define AuxPWFailCheck	NoCheck			//1������		�ѿر���޸�����Դ

#define LoadMFlash	YesCheck		//��������ʹ��FlashоƬ

//#define FlashAT26DF321	YesCheck	//������Ҫ����������	//17.03.27
//#define FlashAT26DF321	NoCheck

#define MagnKeepRelay YesCheck
//#define MagnKeepRelay NoCheck

#define CarryComm YesCheck			//���ز�ͨѶ
//#define CarryComm NoCheck				//���ز�ͨѶ
                       
#if ( CarryComm == YesCheck )             	
#define GPRSComm YesCheck				//��GPRSͨѶ
#else
#define GPRSComm NoCheck				//��GPRSͨѶ
#endif                     

#define LocalSmartMeter	YesCheck	//���طѿ����ܱ�	
//#define LocalSmartMeter	NoCheck			//���طѿ����ܱ�	

//#define ISO7816Config	YesCheck	//����7816Э��ͨѶ
#define ISO7816Config	NoCheck		//����UartЭ��ͨѶ

//#define GeneralSmartMeter	YesCheck	//��ͨ���ܱ�	
#define GeneralSmartMeter	NoCheck		//����ͨ���ܱ�	
               	
//#define JiangSuRequire YesCheck			//����Ҫ��
#define JiangSuRequire NoCheck		//�ǽ���Ҫ��
                                    	
//#define BeiJingRequire YesCheck		//����Ҫ��ѡ���յ�Զ�����������������բ��ѡ��˫485�ڡ�
#define BeiJingRequire NoCheck			//�ǽ���Ҫ��

//#define AnHuiRequire YesCheck			//����Ҫ��ʱ�α�ʱ������������Ϊȫ�㣬ѡ��˫485�ڡ�
#define AnHuiRequire NoCheck			//�ǰ���Ҫ��

//#define ShanDongRequire YesCheck		//ɽ��Ҫ�������һ�����޾���Ҫ��
#define ShanDongRequire NoCheck		//��ɽ��Ҫ��

//#define HuBeiRequire YesCheck			//����Ҫ�������һ����˫485 ��Զѡ��
#define HuBeiRequire NoCheck			//�Ǻ���Ҫ��

//#define HuaBeiRequire YesCheck			//����Ҫ��		//�ѿر���ʾ����������޹�1������޹�2	//17.03.25
#define HuaBeiRequire NoCheck			//�Ǽ���Ҫ��
                                    	
#if ( BeiJingRequire == YesCheck )   	
#define RelayONImmediately YesCheck		//�����Զ���բ
//#define RelayONImmediately NoCheck	//�����˹���բ

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//��485��
//#else
#define Two485Ports YesCheck			//˫485��
//#endif

#elif ( AnHuiRequire == YesCheck )   	//	
//#define RelayONImmediately YesCheck	//�����Զ���բ
#define RelayONImmediately NoCheck		//�����˹���բ

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//��485��
//#else
#define Two485Ports YesCheck			//˫485��
//#endif

#elif ( ShanDongRequire == YesCheck )   //	
#define RelayONImmediately NoCheck		//�����˹���բ

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//��485��
//#else
#define Two485Ports YesCheck			//˫485��
//#endif

#elif ( HuBeiRequire == YesCheck )   	//	
#if ( MagnKeepRelay == YesCheck )
#define RelayONImmediately NoCheck		//�����˹���բ
#else
#define RelayONImmediately YesCheck		//������բ
#endif

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//��485��
//#else
#define Two485Ports YesCheck			//˫485��
//#endif

#else
//#define RelayONImmediately YesCheck	//�����Զ���բ
#if ( MagnKeepRelay == YesCheck )
#define RelayONImmediately NoCheck		//�����˹���բ
#else
#define RelayONImmediately YesCheck		//�����Զ���բ
#endif

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//��485��
//#else
#define Two485Ports YesCheck			//˫485��
//#endif

#endif

#if ( JiangSuRequire == YesCheck )
#define RemoteFeeMeter	YesCheck		//Զ�̷������ܱ�	//15.12.14
#else
#define RemoteFeeMeter	NoCheck			//Զ�̷������ܱ�	//15.12.14
#endif


#define RX8025		0x00	//
#define DS3231		0x01	//
#define RX8025T		0x02	//V1000
////////#define RTCCHIP		RX8025	// ѡ��ʱ��оƬ����
//#define RTCCHIP		DS3231	// ѡ��ʱ��оƬ����
//#define RTCCHIP		RX8025T	// ѡ��ʱ��оƬ����			//V1000		//�¹���

//#define CarryCommWithOC 	YesCheck	//�ز������������룬STA�źŽ��߼�����					//10.11.23
#define CarryCommWithOC 	NoCheck		//�ز�����������޸��룬���������л���ƽ��STA�߼�������		//10.11.23

#define PRODUCTTYPE1	'S'				//�ͺ�SD32DH	
//#define PRODUCTTYPE2	'D'
#if ( CarryComm == YesCheck )
//#define PRODUCTTYPE2	'Z'				//�¹���
//#elif ( GPRSComm == YesCheck )		//�¹���
#define PRODUCTTYPE2	'N'
#else
#define PRODUCTTYPE2	'D'
#endif
//#define PRODUCTTYPE3	'0'
//#define PRODUCTTYPE3	'1'				//��Ӳ����������Ϊ1
#if ( MEASCHIP == ATT7022E )
#define PRODUCTTYPE3	'3'				//��Ӳ����������Ϊ3		//7022E
#elif ( MEASCHIP == IDT90E32 )
#define PRODUCTTYPE3	'4'				//��Ӳ����������Ϊ4		//90E32
#else
#define PRODUCTTYPE3	'2'				//��Ӳ����������Ϊ2
#endif
//#define PRODUCTTYPE4	'2'
#define PRODUCTTYPE4	'3'
#if ( GeneralSmartMeter	== YesCheck )						//��ͨ���ܱ�	//�¹���	//13.11.06
#define PRODUCTTYPE5	'A'													//�¹���	//13.11.06		
#else																		//�¹���	//13.11.06
#define PRODUCTTYPE5	'C'
#endif
//#define PRODUCTTYPE6	'K'
#if ( MagnKeepRelay == YesCheck )
#define PRODUCTTYPE6	'N'
#else
#define PRODUCTTYPE6	'W'
#endif

//'-'

//#define MANUFACTURER1	'J'				//�Ϻ�����
//#define MANUFACTURER2	'L'
#if ( JiangSuRequire == YesCheck )
//#define MANUFACTURER1	'D'				//����
//#define MANUFACTURER2	'H'
#define MANUFACTURER1	'J'				//�滪
#define MANUFACTURER2	'H'
#elif ( BeiJingRequire == YesCheck ) 
#define MANUFACTURER1	'J'				//�滪
#define MANUFACTURER2	'H'
#elif ( AnHuiRequire == YesCheck )   	//	
#define MANUFACTURER1	'J'				//�滪
#define MANUFACTURER2	'H'
#elif ( ShanDongRequire == YesCheck )   //	
#define MANUFACTURER1	'J'				//�滪
#define MANUFACTURER2	'H'
#else
#define MANUFACTURER1	'J'				//�滪
#define MANUFACTURER2	'H'
#endif

//#define USERREGION1		'J'			//����
//#define USERREGION2		'L'
#if ( JiangSuRequire == YesCheck )
#define USERREGION1		'J'				//����
#define USERREGION2		'S'
#elif ( BeiJingRequire == YesCheck ) 
#define USERREGION1		'B'				//����
#define USERREGION2		'J'
#elif ( AnHuiRequire == YesCheck )   		
#define USERREGION1		'A'				//����
#define USERREGION2		'H'
#elif ( ShanDongRequire == YesCheck )   		
#define USERREGION1		'S'				//ɽ��
#define USERREGION2		'D'
#elif ( HuBeiRequire == YesCheck )   	//	
#define USERREGION1		'H'				//����
#define USERREGION2		'B'
#elif ( HuaBeiRequire == YesCheck )		//����Ҫ��		//�ѿر���ʾ����������޹�1������޹�2	//17.03.25
#define USERREGION1		'N'				//����
#define USERREGION2		'C'
#else
#define USERREGION1		'G'				//����
#define USERREGION2		'W'
#endif

//'-'

/*
#if ( METERTYPE == U220_I1A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'C'				//1.5(6)A
#elif ( METERTYPE == U220_I20A400 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//20(80)A
#elif ( METERTYPE == U220_I10A800 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//10(80)A
#elif ( METERTYPE == U220_I10A400 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//10(80)A
#elif ( METERTYPE == U100_I1A12800 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//10(80)A
#endif
*/

/*															//�¹���		//13.08.30
#if ( METERTYPE == U220_I0_3A__1_2A25600 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'A'				//0.3(1.2)A
#elif ( METERTYPE == U220_I1A__6A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'D'				//1.0(6)A
#elif ( METERTYPE == U220_I1_5A__6A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'E'				//1.5(6)A
#elif ( METERTYPE == U220_I2_5A__10A3200 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'F'				//1.5(6)A
#elif ( METERTYPE == U220_I5A__6A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'H'				//5(6)A
#elif (( METERTYPE == U220_I5A__20A800 )||( METERTYPE == U220_I5A__20A1200 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'J'				//5(20)A
#elif ( METERTYPE == U220_I5A__30A800 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'K'				//5(30)A
#elif (( METERTYPE == U220_I5A__40A400 )||( METERTYPE == U220_I5A__40A600 )||( METERTYPE == U220_I5A__40A800 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'L'				//5(40)A
#elif (( METERTYPE == U220_I5A__50A400 )||( METERTYPE == U220_I5A__50A600 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'M'				//5(30)A
#elif ( METERTYPE == U220_I5A__60A400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'N'				//5(30)A
#elif (( METERTYPE == U220_I10A__40A400 )||( METERTYPE == U220_I10A__40A600 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'P'				//10(40)A
#elif ( METERTYPE == U220_I10A__60A400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'Q'				//10(60)A
#elif (( METERTYPE == U220_I20A__80A200 )||( METERTYPE == U220_I20A__80A320 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'U'				//20(80)A
#elif ( METERTYPE == U220_I15A__60A400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'S'				//10(60)A
#elif ( METERTYPE == U220_I20A__100A240 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'V'				//20(100)A
#elif ( METERTYPE == U220_I20A__120A240 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'W'				//20(120)A
#elif (( METERTYPE == U220_I10A__100A240 )||( METERTYPE == U220_I10A__100A300 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'R'				//20(100)A
#elif ( METERTYPE == U220_I30A__100A240 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'X'				//20(120)A
#elif ( METERTYPE == U57_I0_3A__1_2A100000 )
#define VOLTTYPE		'5'				//3*57.7V	
#define CURRENTTYPE		'A'				//0.3(1.2)A
#elif ( METERTYPE == U57_I1A__6A20000 )
#define VOLTTYPE		'5'				//3*57.7V	
#define CURRENTTYPE		'D'				//1.0(6)A
#elif ( METERTYPE == U57_I1_5A__6A20000 )
#define VOLTTYPE		'5'				//3*57.7V	
#define CURRENTTYPE		'E'				//1.5(6)A
#elif ( METERTYPE == U100_I0_3A__1_2A100000 )
#define VOLTTYPE		'1'				//3*100V
#define CURRENTTYPE		'A'				//0.3(1.2)A
#elif ( METERTYPE == U100_I1A__6A20000 )
#define VOLTTYPE		'1'				//3*100V
#define CURRENTTYPE		'D'				//1(6)A
#elif ( METERTYPE == U100_I1_5A__6A20000 )
#define VOLTTYPE		'1'				//3*100V
#define CURRENTTYPE		'E'				//1.5(6)A
#endif
*/

//'-'

//#define VERSION1		'1'				//�¹���
#define VERSION1		'3'				//698Э��
//#define VERSION2		'0'
#define VERSION2		'1'				//DLT645-2007-14	//�¹���	//14.05.03
#define VERSION3		'0'
//#define VERSION2		'1'				//����дESAM����ʱ�������෢��һ���ֽ�����		//12.12.30
//#define VERSION3		'1'
//#define VERSION3		'2'				//����2011��������б�		//11.07.19
//#define VERSION3		'3'				//����2011�������б�		//11.12.14
//#define VERSION3		'4'				//����2012���1���б�		//12.04.12	//��ϵ�����ʾ���ţ�ȫʧѹ�¼�¼����ʧѹ�¼�
//#define VERSION3		'5'				//����2012���2���б�Ҫ��(ʵ���ڵ������б�Ÿ�)		//12.10.04	//��ǰ�������ڡ���̼�¼����Կ״̬����
//#define VERSION3		'6'				//����2012���4���б�Ҫ��	//12.12.20		//ȡ�������ߴ���������²���һ����̼�¼
#define VERSION4		'.'
//#define VERSION5		'0'
//#define VERSION5		'1'				//���ӹ�����7�Ų���			//15.03.04
//#define VERSION5		'2'				//���������У��			//15.09.11
#define VERSION5		'0'				//
//'-'

#if ( LinkMode == Phase3Wire4 )
#define LINEMODE		'A'				//��������
#else
#define LINEMODE		'B'				//��������
#endif

//#if ( RTCCHIP == DS3231 )								//�¹���
//#define CLOCKTYPE		'A'				//DS3231		//�¹���
//#else													//�¹���
//#define CLOCKTYPE		'B'				//8025T			//�¹���
//#endif												//�¹���

#define LCDTYPE			'A'				//8259
////#define FLASHTYPE		'A'				//26DF161
//#if ( FlashAT26DF321 ==	YesCheck )
//#define FLASHTYPE		'B'				//26DF321
//#else
//#define FLASHTYPE		'C'				//S25FL032P
//#endif
//#define FLASHTYPE		'C'				//698Э��FLASHоƬ������64
#define FLASHTYPE		'D'				//698Э��FLASHоƬͳһ��128		//17.05.08

#if ( LCDDrive_PCF8578 == NoCheck )
#if ( LCDDrive_BU97950 == YesCheck )
#define LCDDRVTYPE		'C'				//BU97950
#elif ( LCDDrive_BR8564	== YesCheck )
#define LCDDRVTYPE		'D'				//BR8564
#else
#define LCDDRVTYPE		'A'				//EM1160
#endif
#else
#define LCDDRVTYPE		'B'				//PCF8578
#endif

#if ( AuxPWFailCheck ==	YesCheck )
#define AUXPWTYPE		'A'				//��������Դ
#else
#define AUXPWTYPE		'B'				//�޸�����Դ
#endif

#if ( MagnKeepRelay == YesCheck )
#define RELAYTYPE		'A'				//���ɿ�������
#else
#define RELAYTYPE		'B'				//���ɿ������û�û��
#endif

#if (( CarryComm == NoCheck )&&( GPRSComm == NoCheck ))
#define COMMTYPE		'A'				//���ز���GPRSͨѶ	
#elif ( CarryComm == YesCheck )
//#define COMMTYPE		'B'				//���ز�ͨѶ	//�¹���
//#elif ( GPRSComm == YesCheck )						//�¹���
#define COMMTYPE		'C'				//��GPRSͨѶ	
#else
#endif

#if ( BeiJingRequire == YesCheck )   	
#define VERSION6		'0'				//������������բ��˫485��
#elif ( AnHuiRequire == YesCheck )   		
#define VERSION6		'0'				//���գ�ʱ��ʱ���������ȫ�㡢˫485��
#elif ( ShanDongRequire == YesCheck )   		
#define VERSION6		'0'				//ɽ���������һ��
#elif ( HuBeiRequire == YesCheck )   	//	
#define VERSION6		'0'				//�����������һ��
#elif ( JiangSuRequire == YesCheck )
#if ( ClockBatResisterIs2M == YesCheck	)			//ʱ�ӵ�ز���������5M��Ϊ2M //17.02.23
#define VERSION6		'1'				//����
#else
#define VERSION6		'0'				//����ǰ��40ֻ698��
#endif
#else
//#define VERSION6		'0'				//�·���		10.08.04
//#define VERSION6		'1'				//�·���		//�¹���	//13.10.28
//#define VERSION6		'2'				//14��1���͵��Ժ		//�¹���	//14.01.11
//#define VERSION6		'3'				//14��1���͵��Ժ		//�¹���	//14.01.24	//У��������ݴ�FLASH�ָ�
//#define VERSION6		'4'				//14��2���͵��Ժ		//�¹���	//14.02.11	//15����ʱ�μ��
//#define VERSION6		'5'				//14��4���͵��Ժ		//�¹���	//14.03.22	//��Զ�̱���δ������DS3231��ʼֵУ׼

//#define VERSION6		'0'				//14��4�µ��ԺҪ��		//�¹���	//14.05.06	//�����ϱ����������ߵ�
//#define VERSION6		'1'				//14��6�µ��ԺҪ��		//�¹���	//14.07.09	//������ϢFLASH�������CRC�����Ӳ���CRC����
//#define VERSION6		'2'				///14.07.14		//�����Զ�������������ñ����ŵ�
//#define VERSION6		'3'				///14.07.25		//7022Eû��У����У��ʽ�£�����ѹֵȱ��һλС��
//#define VERSION6		'4'				///14.09.03		//���ӵ�ع���ʱ��ġ�ʱ�䡱���ŵȣ��͵��Ժ

//#define VERSION6		'0'				///14.03.04		//���ӹ�����7�Ų���			
//#define VERSION6		'1'				///15.09.30		//������Կ������ʱ���ظ�����һ��,����FLASH�ͺű�ʶ
//#define VERSION6		'2'				//16.01.22		//���ɼ�����ڵ���60ʱ�Ż���������
//#define VERSION6		'3'				//16.04.12		//��λʱ����β�������ⱳ�⻽�Ѵ���
//#define VERSION6		'4'				//16.06.06		//����ͣ�����Ƿѹ���󱨴洢�����ϼ��洢�����Ϻ��󱨵��Ƿѹ

#define VERSION6		'0'				//698Э��

//#define VERSION6		'0'				//���ֽڱ��������֣�����EEPROM  ����
//#define VERSION6		'1'				//IO��
//#define VERSION6		'2'				//ʱ���л�����ص�ѹ
//#define VERSION6		'3'				//��ǰ����������ϵ�����������RAM��  10.04.12
//#define VERSION6		'3'				//10.07.24 Զ��
//#define VERSION6		'4'				//10.07.21
//#define VERSION6		'3'				//���ڹ۲��Ƿ�ˢ�£����಻��
//#define VERSION6		'4'				//���ֽڱ��������֣�����EEPROM
//#define VERSION6		'4'				//����ʧЧ��

#endif

/*
//�汾��Ϣ��
//�͵�һ�ֽڣ� �汾��
//�͵ڶ��ֽڣ� ���ӹ�Լ
//�ߵڶ��ֽڣ� ʹ�õ���
//�ߵ�һ�ֽڣ� �û�����

//�û�����
#define HUALI	0x01	//����	
#define WANSHENG 0x02	//��ʤ
#define SANXING	0x03	//����
#define NEWERGO	0x04	//���ո�
#define DONGYA	0x05	//��������
#define HENGYE	0x06	//���˺�ҵ
#define XILI	0x09	//����
#define JHBADA	0x13	//�𻪰˴�
#define DONGHUI	0x14	//����
//ʹ�õ���
#define HENAN	0x01	//����
#define BEIJING	0x02	//����
#define ZHEJIANG	0x03	//�㽭
#define ZJSHAOXING	0x07	//�㽭����
#define ZJXIAOSHAN	0x20	//�㽭��ɽ
#define ZJQVZHOU	0x19	//�㽭����

//���ӹ�Լ
#define BUGUI	0x01	//������
#define BU_ZHENGZHOU	0x02	//����+֣�ݹ�Լ
//#define BU_ZHEJIANG		0x13	//����+�㽭��Լ		//07��8���б�
#define BU_ZHEJIANG		0x23	//����+�㽭��Լ		//08��4���б�

//�汾��
#define VER0	0x00	//0.0�汾	06.02.15
#define VER1	0x01	//1.0�汾	06.02.15
#define VER2	0x02	//2.0�汾
#define VER3	0x03	//3.0�汾	06.05.13  �㽭	��������ȫʧѹ��ѹ�����ж�����
#define VER4	0x04	//4.0�汾	06.05.13  ����  ��������ȫʧѹ��ѹ�����ж�����

#define VER11	0x11	//1.0�汾	//4067	���� ��ʤ
#define VER13	0x13	//1.0�汾	//4067	���� ��ʤ
#define VER14	0x14	//1.0�汾	//4051  ���� ��ʤ
#define VER16	0x16	//1.0�汾	//4051  ���� ��ʤ

#define VER21	0x21	//2.0�汾	//4067	���� ��ʤ
#define VER23	0x23	//2.0�汾	//4067	���� ��ʤ
#define VER24	0x24	//2.0�汾	//4051  ���� ��ʤ
#define VER26	0x26	//2.0�汾	//4051  ���� ��ʤ

#define VER31	0x31	//3.0�汾	//4067	���� ��ʤ
#define VER33	0x33	//3.0�汾	//4067	���� ��ʤ
#define VER34	0x34	//3.0�汾	//4051  ���� ��ʤ
#define VER36	0x36	//3.0�汾	//4051  ���� ��ʤ


//#define MANUFACTURER	NEWERGO		//���ո�
//#define MANUFACTURER	DONGHUI		//����
//#define MANUFACTURER	XILI		//����
#define MANUFACTURER	WANSHENG	//��ʤ			//��ҵ
//#define MANUFACTURER	SANXING		//����
//#define MANUFACTURER	HENGYE		//���˺�ҵ		//����
//#define MANUFACTURER	JHBADA		//�𻪰˴�

//#define USERREGION		ZJSHAOXING	//�㽭����
#define USERREGION		ZHEJIANG	//�㽭
//#define USERREGION		ZJXIAOSHAN	//�㽭��ɽ
//#define USERREGION		ZJQVZHOU	//�㽭����

#if ( ZjProtocol	== YesCheck )	//���㽭ͨѶ��Լ)
#define PROTOCOL	BU_ZHEJIANG
#else
#endif

//������ʤ
//#define SOFTVERSION VER14		//4051  ���� ��ʤ
//#define SOFTVERSION VER16		//4051  ���� ��ʤ

//#define SOFTVERSION VER24		//4051  ���� ��ʤ
//#define SOFTVERSION VER26		//4051  ���� ��ʤ		//���಻�нǶ�

//#define SOFTVERSION VER34		//4051  ���� ��ʤ
//#define SOFTVERSION VER36		//4051  ���� ��ʤ

//��������
#define SOFTVERSION VER0
//#define SOFTVERSION VER1
//#define SOFTVERSION VER2		//��ҵ0.5S
//#define SOFTVERSION VER3
//#define SOFTVERSION VER4		//�����������߶��಻�нǶ�

//���º�ҵ
//#define SOFTVERSION VER0
//#define SOFTVERSION VER1
//#define SOFTVERSION VER2		//��ҵ0.5S
//#define SOFTVERSION VER3
//#define SOFTVERSION VER4		//�����������߶��಻�нǶ�

//#define SMALLSOFTVER	VER2
#define SMALLSOFTVER	VER3	//ʱ��BUG�޸�



#if ( MANUFACTURER == XILI )
//#define MEASCHIP	ADE7758	//ѡ�����оƬ����	
#define MEASCHIP	ATT7022	//ѡ�����оƬ����	
//#define LCD4041		YesCheck		//4041Һ��
#define LCD4041		NoCheck			//3713Һ��
#define LCD4712		YesCheck	//4712Һ��
//#define LCD4712		NoCheck			//��4712Һ��
//#define LCD4773		YesCheck	//4773Һ��		//��4712�Ĳ���Ϊ N->��ʤ
#define LCD4773		NoCheck			//��4773Һ��
//#define LCD4713		YesCheck	//4713Һ��
#define LCD4713		NoCheck			//��4713Һ��
//#define LCD3713		YesCheck		//3713Һ��
#define LCD3713		NoCheck			//��3713Һ��
#define CD4051		YesCheck	//ʹ�ð�ѡһת��
//#define CD4051		NoCheck				//��ʹ�ð�ѡһת��
//#define CD4067		YesCheck		//ʹ��16ѡһת��
#define CD4067		NoCheck		//��ʹ��16ѡһת��


#elif ( MANUFACTURER == WANSHENG )							//��ʤ
//#define MEASCHIP	ADE7758	//ѡ�����оƬ����	
//#define MEASCHIP	ATT7022	//ѡ�����оƬ����	
#define MEASCHIP	ADE7878	//ѡ�����оƬ����	
//#define LCD4041		YesCheck		//4041Һ��
#define LCD4041		NoCheck			//3713Һ��
#define LCD4712		YesCheck	//4712Һ��
//#define LCD4712		NoCheck			//��4712Һ��
#define LCD4773		YesCheck	//4773Һ��		//��4712�Ĳ���Ϊ N->��ʤ
//#define LCD4773		NoCheck			//��4773Һ��
//#define LCD4713		YesCheck	//4713Һ��
#define LCD4713		NoCheck			//��4713Һ��
//#define LCD3713		YesCheck		//3713Һ��
#define LCD3713		NoCheck			//��3713Һ��
#define CD4051		YesCheck	//ʹ�ð�ѡһת��
//#define CD4051		NoCheck				//��ʹ�ð�ѡһת��
//#define CD4067		YesCheck		//ʹ��16ѡһת��
#define CD4067		NoCheck		//��ʹ��16ѡһת��

#endif


#define MSPF449		0x00
#define MSPF437		0x01
//#define CPUCHIP		MSPF437	// ѡ��Ƭ��оƬ����
#define CPUCHIP		MSPF449	// ѡ��Ƭ��оƬ����
*/

#define UCLOCKBATL	0x20
#define UCLOCKBATH	0x03	// ʱ�ӵ�ط�ֵ3.2V

#define BatVolt6V	0x00		// ͣ����ص�ѹ = 6V	
#define BatVolt3V	0x01		// ͣ����ص�ѹ = 3V	

#define CommBatVolt	BatVolt6V		//ͣ����ص�ѹ = 6V	
//#define CommBatVolt	BatVolt3V		//ͣ����ص�ѹ = 3V	

#if ( CommBatVolt == BatVolt6V )	//ͣ����ص�ѹ = 6V	
#define UCOMMBATL	0x20	
#define UCOMMBATH	0x05	//ͣ����ط�ֵ5.2V
#else
//#define UCOMMBATL	0x50	
//#define UCOMMBATH	0x02	// ͣ����ط�ֵ2.5V
#define UCOMMBATL	0x20							//3.6Vͣ�����
#define UCOMMBATH	0x03	//ͣ����ط�ֵ5.2V
#endif

//#define YesCheck	0x00
//#define NoCheck		0x01
#define AllFailChk 	YesCheck	//���ȫʧѹ���
//#define AllFailChk 	NoCheck		//�����ȫʧѹ

#define AllFail7022Chk 		NoCheck	//ͨ��7022���ȫʧѹ���	//10.11.11
//#define AllFail7022Chk 	NoCheck		//��ͨ��7022���ȫʧѹ		//10.11.11

#define ESAMPowerControl 	YesCheck	//ESAM��Դ����				//10.12.02
//#define ESAMPowerControl 	NoCheck	//ESAM��Դ����				//10.12.02

#define NewHardScheme2		YesCheck	//�·���2Ӳ��			//10.12.25

#define FeeMDIndependence	YesCheck	//����������������			//11.01.28
//#define FeeMDIndependence	NoCheck		//����������������			//11.01.28

//#define FeeECIndependence	YesCheck	//���ʵ�����������			//11.01.15
#define FeeECIndependence	NoCheck		//���ʵ�����������			//11.01.15

#define TakeMDClrRecFrameApart		YesCheck		//���������¼�֡���	//11.02.17
//#define TakeMDClrRecFrameApart	NoCheck

#define PowerOnMonSaveMAX12		YesCheck	//�ϵ�������12��		//11.06.27
//#define PowerOnMonSaveMAX12	NoCheck		//�ϵ粻����			//11.06.27

//#define CombinationFlagDisplay	YesCheck	//����ϡ��ַ���ʾ	//11.07.19
#define CombinationFlagDisplay	NoCheck			//����ϡ��ַ�����ʾ	//11.07.19

//#define MinusSignDisplay		YesCheck	//������ʾ				//11.07.19
#define MinusSignDisplay		NoCheck		//���Ų���ʾ			//11.07.19

#define SaveDay1CycleEC		YesCheck	//��ǰ��������й��ۼ��õ������㷽ʽ�ǰ����õĵ�1�������ս���	//2012.10.04
//#define SaveDay1CycleEC	NoCheck		//��ǰ��������й��ۼ��õ������㷽ʽ�ǰ����õ�3�������ս���

//#define PrgRecRestWithOperateCode YesCheck	//�����ߴ���������²���һ����̼�¼		//2012.10.04
#define PrgRecRestWithOperateCode NoCheck	//�����ߴ��������ά��ԭ�б�̼�¼		//2012.10.04

#define SaveDaySort			YesCheck		//����������ʱ�䲻����˳������Ҳ����ȷ���㣬������������һ������	//12.12.05
//#define SaveDaySort		NoCheck			//

//#define PwDownKeepPrgDelay	YesCheck		//�޵������µ����ά�ֱ����Чʱ��׼ȷ��ʱ	//12.12.21		//�¹���
//#define PwDownKeepPrgDelay	NoCheck		//

#if ( FeeECIndependence	== YesCheck )
#define ECDisDecDigitsValid		YesCheck	//��ǰ������ʾС��λ��ʵ��Ч
#else
#define ECDisDecDigitsValid		NoCheck		//��ʾ��ǰ������3��4λС������Ϊ����
#endif

#define LASTBLOCKProcess	YesCheck		//�������ߵ����һ���ж��Ƿ����޺���֡����Ҫ�෢һ���յĺ���֡	//13.03.16
//#define LASTBLOCKProcess	NoCheck			//

#define DLT645_2007_14		YesCheck		//DL/T645-2007-14���±���Ҫ��	//14.04.25
//#define DLT645_2007_14	NoCheck			//DL/T645-2007-14���±���Ҫ��

#define InfraredCarry38k	YesCheck		//Զ������38K�ز�			//15.01.22	
//#define InfraredCarry38k	NoCheck			//Զ������32K�ز�	

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
#define ExactECRead			YesCheck		//��ȷ���ܶ�ȡ
#if ( MagnKeepRelay == YesCheck )
#define IOver30ALimitTrip		YesCheck	//��������30A������բ	//�¹���	//14.04.25	
#else
#define IOver30ALimitTrip		NoCheck		//��������30A��������բ	
#endif

#define RemoteEnterCalib	YesCheck		//֧��Զ�̽���У��״̬	//�¹���	//14.04.25	
//#define FlashTypeSelect		YesCheck		//֧��Flash��������		//�¹���	//14.06.04	
#define FlashTypeSelect		NoCheck		//֧��Flash��������		//�¹���	//14.06.04	
#define ModifyTimeSkipDelay	NoCheck			//��֧���޸�ʱ������������բ��30�����ϱ���λ��ʱ		//�¹���	//14.06.19	

#else
#define ExactECRead		NoCheck			//�޾�ȷ���ܶ�ȡ
#define IOver30ALimitTrip		NoCheck		//��������30A��������բ	//11.06.29	

#define RemoteEnterCalib	YesCheck		//֧��Զ�̽���У��״̬	//�¹���	//14.04.25	
//#define FlashTypeSelect		YesCheck		//֧��Flash��������		//�¹���	//14.06.04	
#define FlashTypeSelect		NoCheck		//֧��Flash��������		//�¹���	//14.06.04	
#define ModifyTimeSkipDelay	NoCheck			//��֧���޸�ʱ������������բ��30�����ϱ���λ��ʱ		//�¹���	//14.06.19	

#endif

#if ( GeneralSmartMeter	== YesCheck )		//��ͨ���ܱ�	//�¹���	//13.11.06
#define HardWareB40Exist		NoCheck		//Ӳ����B40��Դ���ɿ鲻����	//11.06.29
#elif ( CarryComm == NoCheck )			
#define HardWareB40Exist		NoCheck		//Ӳ����B40��Դ���ɿ鲻����	//11.06.29
#else
#define HardWareB40Exist		YesCheck	//Ӳ����B40��Դ���ɿ����	//11.06.29	//������B40
#endif

//#define AdjustErrorCheck	YesCheck	//����У��ѡ��		//15.09.05
#define AdjustErrorCheck	NoCheck		//����У��ѡ��	//15.09.05

#define FRAMPulseValidCheck YesCheck		//�����FRAM�������Ƿ���Ч���		//16.01.26
//#define FRAMPulseValidCheck NoCheck		//�����FRAM�������Ƿ���Ч���		//16.01.26

#define ClockBatResisterIs2M YesCheck		//ʱ�ӵ�ز���������5M��Ϊ2M
//#define ClockBatResisterIs2M NoCheck		//ʱ�ӵ�ز�������5M

#if ( LinkMode == Phase3Wire4 )
//#define ZeroCuChk	YesCheck   //���ߵ������
#define ZeroCuChk	NoCheck   //���ߵ��������

//#define NLineCheck	YesCheck  //�����߼��
#define NLineCheck	NoCheck   //�޵����߼��
#else
#define ZeroCuChk	NoCheck   //���ߵ��������
#define NLineCheck	NoCheck   //�޵����߼��
#endif

#define PhotoDiode	0x00		//���������ܴ���
#define PhotoTriode	0x01		//���������ܴ���
//#define IRTrigger	PhotoDiode 	//���������ܴ���
#define IRTrigger	PhotoTriode //���������ܴ���

#define LoadRecCheck	YesCheck 	//�и������߼�¼
//#define LoadRecCheck	NoCheck 	//�޸������߼�¼

#if ( LoadRecCheck == YesCheck )
#define GeneralLoadCurve 	YesCheck	//��ͨ��������	
//#define GeneralLoadCurve 	NoCheck		//��ͨ��������	
#else
#define GeneralLoadCurve 	NoCheck		//��ͨ��������	
#endif

#define	SecurityCheck	YesCheck 	//�а�ȫ��֤
//#define	SecurityCheck	NoCheck 	//�ް�ȫ��֤

//#define HWKeyAllDis		YesCheck 		//����ң�����ɲ鿴������	//HW	//��ʤ
#define HWKeyAllDis		NoCheck 	//����ң�����ɲ鿴������					//����

/*
#define ZjProtocol	YesCheck		//���㽭ʡͨѶ��Լ
//#define ZjProtocol	NoCheck		//���㽭ʡͨѶ��Լ

//#define LCD4041		YesCheck		//4041Һ��
#define LCD4041	NoCheck			//3713Һ��

#define LCD4712		YesCheck	//4712Һ��
//#define LCD4712		NoCheck			//��4712Һ��

//#define LCD4773		YesCheck	//4773Һ��		//��4712�Ĳ���Ϊ N->��ʤ
#define LCD4773		NoCheck			//��4773Һ��

//#define LCD4713		YesCheck	//4713Һ��
#define LCD4713		NoCheck			//��4713Һ��

//#define LCD3713		YesCheck		//3713Һ��
#define LCD3713		NoCheck			//��3713Һ��

//#define CD4051		YesCheck	//ʹ�ð�ѡһת��
#define CD4051	NoCheck				//��ʹ�ð�ѡһת��

#define CD4067		YesCheck		//ʹ��16ѡһת��
//#define CD4067		NoCheck		//��ʹ��16ѡһת��
*/
#define CrawlLimit	YesCheck		//Ǳ����������		//ADE7758��1������Ҫ
//#define CrawlLimit	NoCheck		//��Ǳ����������	//ADE7758��0.5������Ҫ

//#if (( CD4051 == YesCheck )||( CD4067 == YesCheck )) 
//#define LoadMFlash	YesCheck		//��������ʹ��FlashоƬ
//#define LoadMFlash	NoCheck		//��������ʹ��E2PROMоƬ
//#else
//#define LoadMFlash	NoCheck			//��������ʹ��E2PROMоƬ
//#endif

//#if ( USERREGION == ZJQVZHOU )		//�㽭����		//V03
//#define RunOverCheck	YesCheck
//#else
//#define RunOverCheck	NoCheck
//#endif

#define RunOverCheck	NoCheck

//DS3231
#define RS1			0x0008
#define RS2			0x0010
#define EN32kHz		0x0800
#define INTCN		0x0004

//8025
#define	Hour24		0x0020
#define CLEN1		0x0800
#define CLEN2		0x0010
#define CT0			0x0001
#define CT1			0x0002
#define CT2			0x0003
#define	XST			0x2000

//RX8025T
#define CSEL0		0x4000
#define UIE			0x2000

//#if ( RTCCHIP == RX8025 )																//�¹���
//#define EN32K1Hz	XST+Hour24+CT0+CT1		//0x2023	32kHz+���������                
//#define EN32K		XST+Hour24				//0x2020	32kHz+���������				
//#define DisEn32K1Hz	XST+Hour24+CLEN1+CLEN2	//0x2830	�����						
//
//#elif ( RTCCHIP == RX8025T )				//V1000										//�¹���
//#define EN32K1Hz	CSEL0+UIE				//0x6000	32kHz+��������� 2��ˢ���¶�ϵ��
//#define EN32K		CSEL0					//0x4000	32kHz+���������
//
//#else
//
//#define EN32K1Hz	EN32kHz				//0x0800		32kHz+���������
//#define EN32K		EN32kHz+INTCN		//0x0804		32kHz+���������
//#define DisEn32K1Hz	INTCN				//0x0004		�����					
//#endif																				//�¹���		

#define EN32K1Hz_8025T		CSEL0+UIE			//0x6000	32kHz+��������� 2��ˢ���¶�ϵ��		//�¹���		
#define EN32K_8025T			CSEL0				//0x4000	32kHz+���������						//�¹���			

#define EN32K1Hz_DS3231		EN32kHz				//0x0800		32kHz+���������					//�¹���			
#define EN32K_DS3231		EN32kHz+INTCN		//0x0804		32kHz+���������					//�¹���		
#define DisEn32K1Hz_DS3231	INTCN				//0x0004		�����								//�¹���		

#define RecordSAds	0xC000	//24C512��
#define RecordEAds	0xD5E0	//24C512��
#define LoadBLen	26			//2+2+2+2+2+2+2+4+4+3=25 +1(У��) = 26
#define RecordBLen	28			//6+1(У��)+1+1+2+2+2+2+2+2+2+4+1(У��) = 28	

#if ( MEASCHIP == ADE7758 )

//#0C098H----	ADDRESSL,#00H---------LONG,#14t	
//(��1�ֽ�+A�й�����2�ֽ�+B�й�����2�ֽ�+C�й�����2�ֽ�+A�޹�����2�ֽ�+B�޹�����2�ֽ�+C�޹�����2�ֽ�+У���ֽڣ�
//ADE7758У������
#define	YGAGN0	0x00	//��	
#define	YGAGN1	0x01	//�й��޹�У����������
#define	YGAGN2	0x02
#define	YGBGN1	0x03
#define	YGBGN2	0x04
#define	YGCGN1	0x05
#define	YGCGN2	0x06
#define	WGAGN1	0x07
#define	WGAGN2	0x08
#define	WGBGN1	0x09
#define	WGBGN2	0x0A
#define	WGCGN1	0x0B
#define	WGCGN2	0x0C

// #0C099H----	ADDRESSL,#XIANGJIAO---LONG,4T	
//��A��ǲ���1�ֽ�+B��ǲ���1�ֽ�+C��ǲ���1�ֽڣ�
#define XIANGJIAO 0x10

// #0C09AH----	ADDRESSL,#WTTOSD------LONG,7T	
//��A����Ư��2�ֽ�+B����Ư��2�ֽ�+C����Ư��2�ֽڣ�
#define WTTOSD XIANGJIAO+4

// #0C09BH----	ADDRESSL,#zdydydlcs---LONG,13T	��
// A��ѹϵ��2�ֽ�+B��ѹϵ��2�ֽ�+C��ѹϵ��2�ֽ�+A����ϵ��2�ֽ�+B����ϵ��2�ֽ�+C����ϵ��2�ֽڣ�
#define zdydydlcs WTTOSD+7

// #0C09CH		addressl,#DYAYUAN3-----long,18t	
//��A��ѹԭʼֵ3�ֽ�+B��ѹԭʼֵ3�ֽ�+C��ѹԭʼֵ3�ֽ�+A����ԭʼֵ3�ֽ�+B����ԭʼֵ3�ֽ�+C����ԭʼֵ3�ֽڣ�
//		;����ѹ����ԭʼֵ������7758��������ֵ��

//#0C09DH----	ADDRESSL,---------LONG,#14t	
//(��1�ֽ�+A�й�����2�ֽ�+B�й�����2�ֽ�+C�й�����2�ֽ�+A�޹�����2�ֽ�+B�޹�����2�ֽ�+C�޹�����2�ֽ�+У���ֽڣ�
//ADE7758У������
#define	YGAGN048Hz	zdydydlcs+13	//48Hz����	

//#0C09DH----	ADDRESSL,---------LONG,#14t	
//(��1�ֽ�+A�й�����2�ֽ�+B�й�����2�ֽ�+C�й�����2�ֽ�+A�޹�����2�ֽ�+B�޹�����2�ֽ�+C�޹�����2�ֽ�+У���ֽڣ�
//ADE7758У������
#define	YGAGN052Hz	YGAGN048Hz+14	//52Hz����	

#else
#endif

// Time Backup

/*		//2016.09.19
#define PhAFail_No			0			//A��ʧѹ��¼��				
#define PhBFail_No			1			//B��ʧѹ��¼��				
#define PhCFail_No			2			//C��ʧѹ��¼��				
#define PhALower_No			3			//A��Ƿѹ��¼��				
#define PhBLower_No			4			//B��Ƿѹ��¼��				
#define PhCLower_No			5			//C��Ƿѹ��¼��				
#define PhAOver_No			6			//A���ѹ��¼��				
#define PhBOver_No			7			//B���ѹ��¼��				
#define PhCOver_No			8			//C���ѹ��¼��				
#define PhABreak_No			9			//A������¼��				
#define PhBBreak_No			10			//B������¼��				
#define PhCBreak_No			11			//C������¼��				
#define AllFail_No			12			//ȫʧѹ��¼��				
#define AuxPWFail_No		13			//������Դʧѹ��¼��				
#define VoltRevPhase_No		14			//��ѹ�������¼��				
#define CuRevPhase_No		15			//�����������¼��				
#define VoltImbalance_No	16			//��ѹ��ƽ���¼��				
#define CuImbalance_No		17			//������ƽ���¼��
#define CuBestImbalance_No	18			//�������ز�ƽ���¼��				
#define CuAFail_No			19			//A��ʧ����¼��				
#define CuBFail_No			20			//B��ʧ����¼��				
#define CuCFail_No			21			//C��ʧ����¼��				
#define CuAOver_No			22			//A�������¼��				
#define CuBOver_No			23			//B�������¼��
#define CuCOver_No			24			//C�������¼��
#define CuABreak_No			25			//A�������¼��				
#define CuBBreak_No			26			//B�������¼��				
#define CuCBreak_No			27			//C�������¼��				
#define RevPowerA_No		28			//A�ೱ�������¼��
#define RevPowerB_No		29			//B�ೱ�������¼��
#define RevPowerC_No		30			//C�ೱ�������¼��
#define PowerOverA_No		31			//A����ؼ�¼��
#define PowerOverB_No		32			//B����ؼ�¼��
#define PowerOverC_No		33			//C����ؼ�¼��
#define PftOver_No			34			//�ܹ������س��޼�¼��

#define VoltCheck_No		35			//��ѹ�ϸ���ͳ�Ƽ�¼�� 
#define VoltACheck_No		36			//A���ѹ�ϸ���ͳ�Ƽ�¼�� 
#define VoltBCheck_No		37			//B���ѹ�ϸ���ͳ�Ƽ�¼�� 
#define VoltCCheck_No		38			//C���ѹ�ϸ���ͳ�Ƽ�¼�� 

#define PdownRec_No			39			//�µ��¼��

#define MDOverPp_No			40			//�����й��������޼�¼��
#define MDOverPn_No			41			//�����й��������޼�¼��
#define MDOverQ1_No			42			//1�����޹��������޼�¼��
#define MDOverQ2_No			43			//2�����޹��������޼�¼��
#define MDOverQ3_No			44			//3�����޹��������޼�¼��
#define MDOverQ4_No			45			//4�����޹��������޼�¼��

#define PrgRec_No			46			//��̼�¼��
#define ECClr_No			47			//���������¼��
#define MDClr_No			48			//���������¼��
#define EventClr_No			49			//�¼������¼��
#define TimeSet_No			50			//Уʱ��¼��
#define SegTabPrg_No		51			//ʱ�α��̼�¼��
#define ZonePrg_No			52			//ʱ����̼�¼��
#define WeekCDPrg_No		53			//�����ձ�̼�¼��
#define HolidayPrg_No		54			//�ڼ��ձ�̼�¼��
#define ECPModePrg_No		55			//�й���Ϸ�ʽ��̼�¼��
#define ECQ1ModePrg_No		56			//�޹���Ϸ�ʽ1��̼�¼��
#define ECQ2ModePrg_No		57			//�޹���Ϸ�ʽ2��̼�¼��
#define SaveDayPrg_No		58			//�����ձ�̼�¼��
#define OpenCover_No		59			//����Ǽ�¼��
#define OpenTerminal_No		60			//����ť�м�¼��
#define BuyEC_No			61			//��������¼��
#define BuyMoney_No			62			//�������¼��
#define TripOFF_No			63			//��բ��¼��
#define TripON_No			64			//��բ��¼��

#define AppointFreeze_No	65			//��ʱ�����¼��
#define InstantFreeze_No	66			//˲ʱ�����¼�� 				
#define ZoneSWFreeze_No		67			//ʱ�����л������¼�� 				
#define SegTabSWFreeze_No	68			//ʱ�α��л������¼�� 				
#define FeeSWFreeze_No		69			//���ʵ���л������¼�� 				
#define StepSWFreeze_No		70			//�����л������¼�� 				
#define HourFreeze_No		71			//���㶳���¼�� 				
#define DayFreeze_No		72			//�ն����¼�� 				

#define LastEC_No			73			//��ʷ������¼��
#define LastMD_No			74			//��ʷ��������¼��

#define TideSwitch_No		75			//���������¼��			//�¹���

#define PftAOver_No			76			//A�๦�����س��޼�¼��		//�¹���
#define PftBOver_No			77			//B�๦�����س��޼�¼��		//�¹���
#define PftCOver_No			78			//C�๦�����س��޼�¼��		//�¹���

#define YearSaveTime_No		79			//�����ʱ���¼��			//�¹���
#define MonthSaveTime_No	80			//�½���ʱ���¼��			//�¹���

#define FeePricePrg_No		81			//���ʵ�۱�̼�¼�� 		//�¹���		
#define StepParaPrg_No		82			//���ݲ�����̼�¼�� 		//�¹���		
#define SecretKeyPrg_No		83			//��Կ��̼�¼�� 			//�¹���	
#define CardInFault_No		84			//�쳣�忨��¼�� 			//�¹���	
#define FeeBack_No			85			//�˷Ѽ�¼�� 				//�¹���
#define MagnetDisturb_No	86			//�㶨�ų����ż�¼�� 		//�¹���		
#define RelayFault_No		87			//���ɿ���������¼�� 		//�¹���		
#define PowerFault_No		88			//��Դ�쳣��¼�� 			//�¹���			
*/


#define PhFail_No			0			//��ʧѹ��¼��				
#define PhAFail_No			1			//A��ʧѹ��¼��				
#define PhBFail_No			2			//B��ʧѹ��¼��				
#define PhCFail_No			3			//C��ʧѹ��¼��				
#define PhLower_No			4			//��Ƿѹ��¼��				
#define PhALower_No			5			//A��Ƿѹ��¼��				
#define PhBLower_No			6			//B��Ƿѹ��¼��				
#define PhCLower_No			7			//C��Ƿѹ��¼��				
#define PhOver_No			8			//�ܹ�ѹ��¼��				
#define PhAOver_No			9			//A���ѹ��¼��				
#define PhBOver_No			10			//B���ѹ��¼��				
#define PhCOver_No			11			//C���ѹ��¼��				
#define PhBreak_No			12			//�ܶ����¼��				
#define PhABreak_No			13			//A������¼��				
#define PhBBreak_No			14			//B������¼��				
#define PhCBreak_No			15			//C������¼��				
#define CuFail_No			16			//��ʧ����¼��				
#define CuAFail_No			17			//A��ʧ����¼��				
#define CuBFail_No			18			//B��ʧ����¼��				
#define CuCFail_No			19			//C��ʧ����¼��				
#define CuOver_No			20			//�ܹ�����¼��				
#define CuAOver_No			21			//A�������¼��				
#define CuBOver_No			22			//B�������¼��
#define CuCOver_No			23			//C�������¼��
#define CuBreak_No			24			//�ܶ�����¼��				
#define CuABreak_No			25			//A�������¼��				
#define CuBBreak_No			26			//B�������¼��				
#define CuCBreak_No			27			//C�������¼��				
#define RevPower_No			28			//�ܳ��������¼��
#define RevPowerA_No		29			//A�ೱ�������¼��
#define RevPowerB_No		30			//B�ೱ�������¼��
#define RevPowerC_No		31			//C�ೱ�������¼��
#define PowerOver_No		32			//�ܹ��ؼ�¼��
#define PowerOverA_No		33			//A����ؼ�¼��
#define PowerOverB_No		34			//B����ؼ�¼��
#define PowerOverC_No		35			//C����ؼ�¼��

/*
#define PhAFail_No			0			//A��ʧѹ��¼��				
#define PhBFail_No			1			//B��ʧѹ��¼��				
#define PhCFail_No			2			//C��ʧѹ��¼��				
#define PhFail_No			3			//��ʧѹ��¼��				
#define PhALower_No			4			//A��Ƿѹ��¼��				
#define PhBLower_No			5			//B��Ƿѹ��¼��				
#define PhCLower_No			6			//C��Ƿѹ��¼��				
#define PhLower_No			7			//��Ƿѹ��¼��				
#define PhAOver_No			8			//A���ѹ��¼��				
#define PhBOver_No			9			//B���ѹ��¼��				
#define PhCOver_No			10			//C���ѹ��¼��				
#define PhOver_No			11			//�ܹ�ѹ��¼��				
#define PhABreak_No			12			//A������¼��				
#define PhBBreak_No			13			//B������¼��				
#define PhCBreak_No			14			//C������¼��				
#define PhBreak_No			15			//�ܶ����¼��				
#define CuAFail_No			16			//A��ʧ����¼��				
#define CuBFail_No			17			//B��ʧ����¼��				
#define CuCFail_No			18			//C��ʧ����¼��				
#define CuFail_No			19			//��ʧ����¼��				
#define CuAOver_No			20			//A�������¼��				
#define CuBOver_No			21			//B�������¼��
#define CuCOver_No			22			//C�������¼��
#define CuOver_No			23			//�ܹ�����¼��				
#define CuABreak_No			24			//A�������¼��				
#define CuBBreak_No			25			//B�������¼��				
#define CuCBreak_No			26			//C�������¼��				
#define CuBreak_No			27			//�ܶ�����¼��				
#define RevPowerA_No		28			//A�ೱ�������¼��
#define RevPowerB_No		29			//B�ೱ�������¼��
#define RevPowerC_No		30			//C�ೱ�������¼��
#define RevPower_No			31			//�ܳ��������¼��
#define PowerOverA_No		32			//A����ؼ�¼��
#define PowerOverB_No		33			//B����ؼ�¼��
#define PowerOverC_No		34			//C����ؼ�¼��
#define PowerOver_No		35			//�ܹ��ؼ�¼��
*/
#define MDOverPp_No			36			//�����й��������޼�¼��
#define MDOverPn_No			37			//�����й��������޼�¼��
#define MDOverQ1_No			38			//1�����޹��������޼�¼��
#define MDOverQ2_No			39			//2�����޹��������޼�¼��
#define MDOverQ3_No			40			//3�����޹��������޼�¼��
#define MDOverQ4_No			41			//4�����޹��������޼�¼��
#define PftOver_No			42			//�ܹ������س��޼�¼��
                                            
#define AllFail_No			43			//ȫʧѹ��¼��				
#define AuxPWFail_No		44			//������Դʧѹ��¼��				
#define VoltRevPhase_No		45			//��ѹ�������¼��				
#define CuRevPhase_No		46			//�����������¼��				
#define PdownRec_No			47			//�µ��¼��
#define PrgRec_No			48			//��̼�¼��
                                            
#define ECClr_No			49			//���������¼��
#define MDClr_No			50			//���������¼��
#define EventClr_No			51			//�¼������¼��
#define TimeSet_No			52			//Уʱ��¼��
#define SegTabPrg_No		53			//ʱ�α��̼�¼��
#define ZonePrg_No			54			//ʱ����̼�¼��
#define WeekCDPrg_No		55			//�����ձ�̼�¼��
#define SaveDayPrg_No		56			//�����ձ�̼�¼��
#define OpenCover_No		57			//����Ǽ�¼��
#define OpenTerminal_No		58			//����ť�м�¼��
                                            
#define VoltImbalance_No	59			//��ѹ��ƽ���¼��				
#define CuImbalance_No		60			//������ƽ���¼��
#define TripOFF_No			61			//��բ��¼��
#define TripON_No			62			//��բ��¼��
                                            
#define HolidayPrg_No		63			//�ڼ��ձ�̼�¼��
#define ECPModePrg_No		64			//�й���Ϸ�ʽ��̼�¼��
#define ECQModePrg_No		65			//�޹���Ϸ�ʽ��̼�¼��                            
#define FeePricePrg_No		66			//���ʵ�۱�̼�¼�� 		//�¹���		
#define StepParaPrg_No		67			//���ݲ�����̼�¼�� 		//�¹���		
#define SecretKeyPrg_No		68			//��Կ��̼�¼�� 			//�¹���	
#define CardInFault_No		69			//�쳣�忨��¼�� 			//�¹���	
#define BuyMoney_No			70			//�������¼��
#define FeeBack_No			71			//�˷Ѽ�¼�� 				//�¹���
#define MagnetDisturb_No	72			//�㶨�ų����ż�¼�� 		//�¹���		
#define RelayFault_No		73			//���ɿ���������¼�� 		//�¹���		
#define PowerFault_No		74			//��Դ�쳣��¼�� 			//�¹���			
                                            
#define CuBestImbalance_No	75			//�������ز�ƽ���¼��				
#define ClockErr_No			76			//ʱ�ӹ���
#define MeterICErr_No		77			//���ܼ���оƬ����
#define ComModuleChange_No	78			//ͨѶģ����


/*
#define InstantFreeze_No	0			//˲ʱ�����¼�� 				
#define SecFreeze_No		1			//�붳���¼�� 				
#define MinFreeze_No		2			//���Ӷ����¼�� 				
#define HourFreeze_No		3			//Сʱ�����¼�� 				
#define DayFreeze_No		4			//�ն����¼�� 				
#define SaveDayFreeze_No	5			//�����ն����¼�� 				
#define MonthFreeze_No		6			//�¶����¼�� 				
#define YearFreeze_No		8			//�궳���¼�� 				
#define ZoneSWFreeze_No		9			//ʱ�����л������¼�� 				
#define SegTabSWFreeze_No	10			//��ʱ�α��л������¼�� 				
#define FeeSWFreeze_No		11			//���ʵ���л������¼�� 				
#define StepSWFreeze_No		12			//�����л������¼�� 				
#define StepSaveSWFreeze_No	13			//���ݽ��㶳���¼�� 				
*/

#define InstantFreeze_No	0			//˲ʱ�����¼�� 				
#define SecFreeze_No		1			//�붳���¼�� 				
#define MinFreeze_No		2			//���Ӷ����¼�� 				
#define HourFreeze_No		3			//Сʱ�����¼�� 				
#define DayFreeze_No		4			//�ն����¼�� 				
#define SaveDayFreeze_No	5			//�����ն����¼�� 				
#define MonthFreeze_No		6			//�¶����¼�� 				
#define YearFreeze_No		7			//�궳���¼�� 				
#define ZoneSWFreeze_No		8			//ʱ�����л������¼�� 				
#define SegTabSWFreeze_No	9			//��ʱ�α��л������¼�� 				
#define FeeSWFreeze_No		10			//���ʵ���л������¼�� 				
#define StepSWFreeze_No		11			//�����л������¼�� 				
#define StepSaveSWFreeze_No	12			//���ݽ��㶳���¼�� 				

#define LOAR_TYPE               1
#define TREND_TYPE              2
#define MAX_CH_NUM              4