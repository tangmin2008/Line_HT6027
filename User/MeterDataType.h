

//*******************************************************************
// Data type define
//*******************************************************************

typedef struct
{
   unsigned int	DataID;			//ID��
  /* unsigned char	DataOBIS1;		//OBIS��
   unsigned char	DataOBIS2;		//OBIS��
   unsigned char	DataOBIS3;		//OBIS��
   unsigned char	DataOBIS4;		//OBIS��
   unsigned char	DataOBIS5;		//OBIS��
   unsigned char	DataOBIS6;		//OBIS��
   */
   unsigned long 	DataAdd;		//��ַ
   unsigned char 	DataLen;		//���ݳ���
   unsigned char 	DataRWENStorage;	//���ݶ�дȨ�޺ʹ洢����
  /* unsigned char 	DataIntNum;		//��������λ��
   unsigned char 	DataDecimalsNum;	//����С��λ��
   unsigned int 	DataDisUnit;		//������ʾ��λ����ʶ
   */
} METERDATATAB;


/*******************************************************************
	Even Log Profile Information Structure Define
*******************************************************************/
#define MeterDataTabNum	215//222+48+63+5+16//8		//�����������
#if(MeterPhase==Threephase)
#define MeterRealTabNum	8			//��ѹ������ʵʱ����
#else
#define MeterRealTabNum	8			//��ѹ������ʵʱ����
#endif
#if(MeterPhase==Threephase)
#define MeterECTabNum	8*(Total_Fee+1)	
#else
#define MeterECTabNum	4*(Total_Fee+1)
#endif
#define MeterDemandTabNum	4*(Total_Fee+1)
#define MeterMAXTabNum	4*9

extern const METERDATATAB MeterDataTab[MeterDataTabNum];
extern const ID_OBIS Double_RealTab[MeterRealTabNum];
extern const ID_OBIS Double_ECTab[MeterECTabNum];
extern const ID_MAXDemand_OBIS MaxDemandTab[MeterMAXTabNum];
extern const ID_OBIS Double_DemandTab[MeterDemandTabNum];

//DataLen���ݳ���
#define  DLen_0		0x00
#define  DLen_1		0x01		
#define  DLen_2		0x02	
#define  DLen_3		0x03	
#define  DLen_4		0x04	
#define  DLen_5		0x05	
#define  DLen_6		0x06	
#define  DLen_7		0x07	
#define  DLen_8		0x08	
#define  DLen_9		0x09
#define  DLen_10		0x0A
#define  DLen_11		0x0B	
#define  DLen_12		0x0C
#define  DLen_13		0x0D
#define  DLen_14		0x0E
#define  DLen_15		0x0F
#define  DLen_16		0x10
#define  DLen_17		0x11
#define  DLen_18		0x12
#define  DLen_19		0x13
#define  DLen_20		0x14
#define  DLen_21		0x15
#define  DLen_22		0x16
#define  DLen_23		0x17
#define  DLen_24		0x18
#define  DLen_25		0x19
#define  DLen_26		0x1A
#define  DLen_27		0x1B
#define  DLen_28		0x1C
#define  DLen_29		0x1D
#define  DLen_30		0x1E
#define  DLen_31		0x1F
#define  DLen_32		0x20
#define  DLen_33		0x21
#define  DLen_34		0x22
#define  DLen_35		0x23
#define  DLen_36		0x24
#define  DLen_37		0x25
#define  DLen_38		0x26
#define  DLen_39		0x27
#define  DLen_40		0x28
#define  DLen_41		0x29
#define  DLen_42		0x2A

#define  DLen_43		0x2B
#define  DLen_44		0x2c
#define  DLen_45		0x2d
#define  DLen_46		0x2e
#define  DLen_47		0x2f
#define  DLen_48		0x30
#define  DLen_49		0x31
#define  DLen_50		0x32
#define  DLen_51		0x33
#define  DLen_52		0x34
#define  DLen_53		0x35
#define  DLen_54		0x36
	
//DataRWENStorage���ݶ�дȨ�޺ʹ洢����
#define  F_DRS_SYNData		0x20		//��Ҫͬ�����ݱ�־λ
#define  F_DRS_WriteEN		0x40		//����дʹ�ܱ�־λ
#define  F_DRS_ReadEN		0x80		//���ݶ�ʹ�ܱ�־λ

#define  DRS_Energy		0x01		//��������
#define  DRS_instantaneous	0x02		//˲ʱ������
#define  DRS_Eeprom1		0x03		//EEPROM����1
#define  DRS_Eeprom2		0x04		//EEPROM����2
#define  DRS_Eeprom3		0x05		//EEPROM����3
#define  DRS_Flash		0x06		//FLASH����
#define  DRS_Ram		0x07		//RAM����
#define  DRS_ferroelectric	0x08		//��������
#define  DRS_TimeIC		0x09		//ʱ������

//DataDisUnit ������ʾ��λ����ʶ

#define  DDU_kwh		0x01		//��λ
#define  DDU_kw			0x02		//��λ
#define  DDU_kvarh		0x03		//��λ
#define  DDU_kvar		0x04		//��λ
#define  DDU_V			0x05		//��λ
#define  DDU_A			0x06		//��λ
#define  DDU_Hz			0x07		//��λ
#define  DDU_kVAH		0x08		//��λ
#define  DDU_Time		0x09		//��λ
#define  DDU_Date		0x0A		//��λ



