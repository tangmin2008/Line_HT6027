#ifndef __IEC101_H__
#define __IEC101_H__
typedef unsigned char 	u8;
typedef unsigned int  	u16;
typedef unsigned long  	u32;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
struct bb
{
  u8 l;
  u8 h;
};
union IEC101_ADD
{
  struct 
  {
    u16 l:8;
    u16 h:8;
  }Byte;
  u16 Word;
};
typedef u16 WORD;
typedef union
{
  u8 Byte[4];
  u16 Word[2];
  u32 Dword;
  
}FOUR_BYTE_TO_DWORD;
union IEC101_DADD
{
  u8 Byte[4];
  u16 Word[2];
  u32 Dword;
};
struct APP_STRUCT
{
  u8 byFull;
  u8 byFunCode;
  u8 LinkFunCode;
  u8 byMsgNum;
  u16 wFrameLen;
  u8 *lpByBuf;
  u8 byLinkBuf[320];
};
struct REAPP_STRUCT
{
   u8 *lpByBuf;
};
struct IEC101_STRUCT
{
  u32 wRecvNum;
  u32 RErrorN;
  u32 initstatus;
  u16 Sn;
  u8  Fop;  //�ļ�������ʶ
  char  Fname[32];
  u32 FId;
  u16 List_para[32];
  union IEC101_ADD wRecvCmmAdd;
  u32 FtuSccReset;
  u32 byFrameIntval;
  u32 wPSendNum;
  u32 pfile_ptr;
  u32 PWindow;
  u32 PWinTimer;
  u32 OrgnizeFrame;
  u8  byRecvBuf[256];
  u32 wRecvLen;
  u8 PReMsgType;
  u8 PReMsgType_bak;
  u8 byReason;
  u32 byPSGenStep;
  u8 frameno;
  u8 TypeProtocol;
  u8 byPSDdStep;
  u8 WaitYkXzRet;
  u8 WaitYkZxRet;
  u8 SendYkZxAck;
  u8 PReFrameType;
  u8 UnsolTimeInterval;
  u8 Pacd;
  u8 Pdfc;
  u8 PfcbC;
  u8 PfcvC;
  u8 haveset;
  u8 byQualify;
  u8 TypeSeReason;
  u8 bySourceAdd;
  u8 TypeCmmAdd;
  u8 TypeInfAdd;
  u8 byQOI;
  u8 byQCC;
  u8 byDCO;
  u8 byCOI;
  u8 firstData;
  u8 FlagPingH;
  u8 YcFN;
  u8 DdFN;
  u8 YxFN;
  u8 YcNPF[32];
  u8 DdNPF[32];
  u8 YxNPF[8];
  union IEC101_ADD wTester;
  union IEC101_DADD dwReadAd;
  union IEC101_DADD dwInfAdd;
  union IEC101_ADD wCmmAdd;
  union IEC101_ADD wLinkAdd;
  u8 TypeLinkAdd;
  struct APP_STRUCT PSeAppLayer;
  struct APP_STRUCT PSendFrame;
  struct APP_STRUCT PRecvFrame;
  struct REAPP_STRUCT PReAppLayer; 
  u8 pa_num;
  u8 pb_num;
  u8 pc_num;
  u8 pt_num;
  u8 pce_num;
  u8 pcc_num;
  u8 ptt_num;
};

/***************************************************************************
					IEC-870-5-101Ԥ����ͷ�ļ�
***************************************************************************/

//�������� ��·�㹦���붨��
#define RESET_LINK		0		// ��λԶ����·
#define RESET_SESSION		1     		// ��λԶ���ն˵��û�����
#define CALL_LINK_2		2
#define TRAN_CONFIRM_DATA       3  
#define TRAN_DATA		4	      // ��������/�޻ش�֡
#define CALL_ACD		8	      		// ��Ӧ֡Ӧ˵������Ҫ��
#define CALL_LINK		9		// �ٻ���·״̬
#define CALL_DATA1		10		// �ٻ��û�1������
#define CALL_DATA2		11		// �ٻ��û�2������
#define IEC101_SWITCH_PROTOCOL  15	//��101��Լת��Ϊά����Լ
//�Ӷ����� ��·�㹦���붨��
#define YES_ACK                 0     //�϶�ȷ��
#define NO_ACK                  1     //��ȷ��
#define RESP_DATA               8     //��������Ӧ
#define RESP_NO_DATA            9     //������
#define LINK_GOOD               11    //��·����
#define LINK_NO_WORK            14    //��·����δ����
#define LINK_NO_OVER            15    //��·����δ���
#define LINK_BUSY               1     //��·æ
// �䳤֡���ͱ�ʶ����

// ��վ����վ������Ϣ
#define M_SP_NA_1	1		// ����ʱ��ĵ�����Ϣ
#define M_SP_TA_1	2		// ��ʱ��ĵ�����Ϣ
#define M_DP_NA_1	3		// ����ʱ���˫����Ϣ
#define M_DP_TA_1	4		// ��ʱ���˫����Ϣ
#define M_ST_NA_1	5		// ��λ����Ϣ(��ѹ���ֽ�ͷ��Ϣ)
#define M_ST_TA_1	6		// ��ʱ�경λ����Ϣ(��ѹ���ֽ�ͷ)
#define M_BO_NA_1	7		// ��վԶ���ն�״̬
#define M_ME_NA_1	9		// ����ֵ
#define M_ME_TA_1	10		// ��ʱ��Ĳ���ֵ
#define M_IT_NA_1	15		// �������������
#define M_IT_TA_1	16		// ��ʱ��ĵ������������
#define M_EP_TA_1	17		// ��ʱ��ļ̵籣�����غ�բ�豸�����¼�
#define M_EP_TB_1	18		// ��ʱ��ļ̵籣��װ�ó��������¼�
#define M_EP_TC_1	19		// ��ʱ��ļ̵籣��װ�ó��������·��Ϣ�¼�
#define M_PS_NA_1	20		// ����״̬��λ����ĳ��鵥����Ϣ
#define M_ME_ND_1	21		// ����Ʒ�������Ĳ���ֵ
#define M_BD_NA_1	232		// BCD��(ˮλֵ)

// ��վ����վ �ڿ��Ʒ���Ĺ�����Ϣ
#define C_SC_NA_1	45		// ����ң������
#define C_DC_NA_1	46		// ˫��ң������
#define C_RC_NA_1	47		// ��������
#define C_SE_NA_1	48		// �趨����
#define C_BO_NA_1	51		// 32λ��λ��

// ��վ����վ �ڼ��ӷ����ϵͳ��Ϣ
#define M_EI_NA_1	70		// ��ʼ������

// ��վ����վ �ڿ��Ʒ����ϵͳ��Ϣ
#define GeneralCall	100		// �ٻ�����C_IC_NA_1
#define C_CI_NA_1	101		// ���������ٻ�����
#define C_RD_NA_1	102		// ����������
#define C_CS_NA_1	103		// ʱ��ͬ������
#define C_TS_NA_1	104		// ��������
#define C_RP_NA_1	105		// ��λ��������
#define C_CD_NA_1	106		// ��ʱ�������

// �ڿ��Ʒ���Ĳ���
#define P_ME_NA_1	110		// װ�ز�������
#define P_AC_NA_1	113		// �������

#define C_RR_NA_1       201             //����ֵ����
#define C_RS_NA_1       202             //����ֵ�Ͳ���
#define C_WS_NA_1       203             //д��ֵ�Ͳ���
#define M_IT_NB_1       206
#define M_IT_TC_1       207 
#define F_SR_NA_1       211             //������������ͽ���

// �ļ���������ͱ�ʶ
#define F_FR_NA_1       210             //�ļ�����
#define F_FR_DA_1       1   //��Ŀ¼����
#define F_FR_DA_2       2   //��Ŀ¼ȷ��
#define F_FR_FA_1       3    //���ļ�����
#define F_FR_FA_2       4    //���ļ�ȷ��
#define F_FR_FA_3       5    //���ļ�����
#define F_FR_FA_4       6    //���ļ�����ȷ��
#define F_FW_FA_1       7    //д�ļ�����
#define F_FW_FA_2       8    //д�ļ�ȷ��
#define F_FW_FA_3       9    //д�ļ�����
#define F_FW_FA_4       10    //д�ļ�����ȷ��
#if 0
#define F_FR_NA_1	120		// �ļ���׼����
#define F_SR_NA_1	121		// ����׼����
#define F_SC_NA_1	122		// �ٻ�Ŀ¼��ѡ���ļ����ٻ��ļ����ٻ���
#define F_LS_NA_1	123		// ���Ľڡ����Ķ�
#define F_AF_NA_1	124		// ȷ���ļ���ȷ�Ͻ�
#define F_SG_NA_1	125		// ��
#define F_DR_NA_1	126		// Ŀ¼
#endif

// ����ԭ��
#define PER_CYC		1		// ���ڡ�ѭ��
#define BACK		2		// ����ɨ��
#define SPONT		3		// ͻ��
#define INIT		4		// ��ʼ��
#define REQ		5		// ���������
#define ACT		6		// ����
#define ACTCON		7		// ����ȷ��
#define DEACT		8		// ֹͣ����
#define DEACTCON	9		// ֹͣ����ȷ��
#define ACTTERM		10		// �������
#define RETREM		11		// Զ����������ķ�����Ϣ
#define RETLOC		12		// ������������ķ�����Ϣ
#define FILETRANS	13		// �ļ�����

#define INTROGEN	20		// ��Ӧ���ٻ�
#define INTRO1		21		// ��Ӧ��1���ٻ�
#define INTRO2		22		// ��Ӧ��2���ٻ�
#define INTRO3		23		// ��Ӧ��3���ٻ�
#define INTRO4		24		// ��Ӧ��4���ٻ�
#define INTRO5		25		// ��Ӧ��5���ٻ�
#define INTRO6		26		// ��Ӧ��6���ٻ�
#define INTRO7		27		// ��Ӧ��7���ٻ�
#define INTRO8		28		// ��Ӧ��8���ٻ�
#define INTRO9		29		// ��Ӧ��9���ٻ�
#define INTRO10		30		// ��Ӧ��10���ٻ�
#define INTRO11		31		// ��Ӧ��11���ٻ�
#define INTRO12		32		// ��Ӧ��12���ٻ�
#define INTRO13		33		// ��Ӧ��13���ٻ�
#define INTRO14		34		// ��Ӧ��14���ٻ�
#define INTRO15		35		// ��Ӧ��15���ٻ�
#define INTRO16		36		// ��Ӧ��16���ٻ�

#define REQCOGCN	37		// ��Ӧ���������ٻ�
#define REQCO1		38		// ��Ӧ��1��������ٻ�
#define REQCO2		39		// ��Ӧ��2��������ٻ�
#define REQCO3		40		// ��Ӧ��3��������ٻ�
#define REQCO4		41		// ��Ӧ��4��������ٻ�
			 
// ��Ϣ���ַԤ����

// ң��--����ң��ռ1����ַ��˫��ң��ռ2����ַ
#define IEC101_YXSA		0x001		// ң����Ϣ����ʼ��ַ
#define IEC101_YXEA		0x400		// ң����Ϣ�������ַ
#define IEC101_YXNUM	        1024		// ң����Ϣ�ܸ���
#define IEC101_YXEA_2002	0x1000		// ң����Ϣ�������ַ
#define IEC101_YXNUM_2002	4096		// ң����Ϣ�ܸ���
// �̵籣�������¼�--ÿ���¼�ռ2����ַ
#define IEC101_SEPSA	0x401		// �̵籣�������¼���ʼ��ַ
#define IEC101_SEPEA	0x500		// �̵籣�������¼�������ַ
#define IEC101_SEPNUM	256 / 2		// �̵籣�������¼��ܸ���

// �̵籣��װ�þ��������¼����¼�˳���¼--ÿ����Ϣ��¼ռ8����ַ
#define IEC101_SPESA	0x501		// �̵籣���¼�˳���¼��ʼ��ַ
#define IEC101_SPEEA	0x600		// �̵籣���¼�˳���¼������ַ
#define IEC101_SPENUM	256 / 8		// �̵籣���¼�˳���¼�ܸ���

// �̵籣��װ�ó��������·��Ϣ--ÿ����Ϣռ4����ַ
#define IEC101_PTOSA	0x601		// �̵籣�����������·��Ϣ��ʼ��ַ
#define IEC101_PTOEA	0x700		// �̵籣�����������·��Ϣ������ַ
#define IEC101_PTONUM	256 / 4		// �̵籣�����������·��Ϣ�ܸ���

// ң��--ÿ��ң��ռ1����ַ
#define IEC101_YCSA		0x701		// ң����Ϣ����ʼ��ַ
#define IEC101_YCEA		0x900		// ң����Ϣ�������ַ
#define IEC101_YCNUM	512			// ң����Ϣ�ܸ���
#define IEC101_YCSA_2002	0x4001		// ң����Ϣ����ʼ��ַ
#define IEC101_YCEA_2002	0x5000		// ң����Ϣ�������ַ
#define IEC101_YCNUM_2002	4096		// ң����Ϣ�ܸ���

// ������ַ��Χ--��ң����Ϣ���ַһһ��Ӧ
#define IEC101_PARASA	0x901		// ������ʼ��ַ
#define IEC101_PARAEA	0xB00		// ����������ַ

// ң�أ�����--ÿ��ң�ض���ռ1����ַ
#define IEC101_YKSA		0xB01		// ң����Ϣ����ʼ��ַ
#define IEC101_YKEA		0xB80		// ң����Ϣ�������ַ
#define IEC101_YKNUM	128			// ң����Ϣ�ܸ���
#define IEC101_YKSA_2002		0x6001		// ң����Ϣ����ʼ��ַ
#define IEC101_YKEA_2002		0x6200		// ң����Ϣ�������ַ
// �趨--һ����ռ��һ����Ϣ���ַ
#define IEC101_SETSA	0xB81		// �趨��Ϣ����ʼ��ַ
#define IEC101_SETEA	0xC00		// �趨��Ϣ�������ַ
#define IEC101_SETNUM	128			// �趨��Ϣ�ܸ���

// �������������--ÿһ����ռ��1����Ϣ���ַ
#define IEC101_DDSA		0xC01		// �����Ϣ����ʼ��ַ
#define IEC101_DDEA		0xC80		// �����Ϣ�������ַ
#define IEC101_DDNUM	        128			// �����Ϣ�ܸ���
#define IEC101_DDSA_2002	0x6401		// �����Ϣ����ʼ��ַ
#define IEC101_DDEA_2002	0x6600		// �����Ϣ�������ַ
#define IEC101_DDNUM_2002	512			// �����Ϣ�ܸ���

// ��λ����Ϣ(��ѹ���ֽ�ͷ)--һ����ռ��һ����Ϣ���ַ
#define IEC101_STEPSA	0xC81		// ��λ����Ϣ��ʼ��ַ
#define IEC101_STEPEA	0xCA0		// ��λ����Ϣ������ַ
#define IEC101_STEPSA_2002	0x6601		// ��λ����Ϣ��ʼ��ַ
#define IEC101_STEPEA_2002	0x6620		// ��λ����Ϣ������ַ

#define IEC101_STEPNUM	32			// ��λ����Ϣ�ܸ���

// ������״̬��Ϣ
#define IEC101_BINSA	0xCA1		// ������״̬��Ϣ��ʼ��ַ
#define IEC101_BINEA	0xCC0		// ������״̬��Ϣ������ַ
#define IEC101_BINNUM	32			// ������״̬��Ϣ�ܸ���

// BCD��--һ����ռ��һ����Ϣ���ַ
#define IEC101_BCDSA	0xCC1		// BCD��Ϣ��ʼ��ַ
#define IEC101_BCDEA	0xCE0		// BCD��Ϣ������ַ
#define IEC101_BCDNUM	32			// BCD��Ϣ�ܸ���

// ��վԶ���ն�״̬��ַ
#define IEC101_RTUSA	0xCE9

// �ļ������ַ
#define IEC101_FTA		0xCEA		

// ��־�ֽ�
#define F_STARTCODE		0x10		// ����֡��ʼ�ֽ�
#define V_STARTCODE		0x68		// �䳤֡��ʼ�ֽ�
#define STOPCODE		0x16		// ֹͣ�ֽ�

//ң�����ó���
#define YK_WAIT_TIMES 100	//ң�ط�У�ȴ�����

//������������
//ÿ֡�������Ϊ255����ȥ�������֣���·��ַ�����ͱ�ʶ���޶��ʣ�����ԭ�򣬹�����ַ����Щ���Ϊ
//9���ֽڣ��������Ϊ255-9=246������ˣ�
#define IEC101_YXBWNPF 		60	//64 //ÿ֡���ң�ű�λ�� 246/4=61.5
//#define IEC101_SOENPF 32		//ÿ֡���SOE��
#define IEC101_SOENPF_TIME56	 22		//ÿ֡���SOE��	 246/11= 22.36
#define IEC101_SOENPF_TIME24	 32		//ÿ֡���SOE��	 246/7=	35.1

#define IEC101_YCCNPF 16		//ÿ֡���仯ң����
#define IEC101_YCNPF 23 //31			//ÿ֡���ң����(ÿ��ң���2֡����)
#define IEC101_YXNPF 64			//ÿ֡���ң����(ÿ��ң�ŷ�2֡����)
#define IEC101_DDNPF 8			//ÿ֡�������   246/8=30.75
#define IEC101_YCNPG 128		//ÿ�����ң����(ÿ��ң���2֡����)
#define IEC101_YXNPG 128	   	//ÿ�����ң����(ÿ��ң�ŷ�2֡����)
//#define IEC101_DDNPG 32			//ÿ���������

//������մ������ó���
#define MaxErrorN 5	//�����մ�����
#define MaxDogN 256	//���dog��
#define MAXRESENDTIME 25 //ȫ����ѭ������ʱ��

//��������
void SearchFirstData(void);
void SendData1(void);
void SendData2(void);
void InitIEC101Prot(void);
void InitIEC101Func(u8 ChanNo);
void SettimeToCan(u8* pbyBuf);
void SendGeneralData(void);
void SendTimeAck(void);
void SendDdData(void);
void SendDdData2002(void);
void SendReadDataAck(void);
void SendYkReply(void);
void SendChanTestAck(void);
void PAppSendProcess(void);
void Iec101LinkRecvProPH(void);
u8 OrgnizeYxbwMsg(u8 *lpby);
u8 OrgnizeSoeMsg(void);
u8 OrgnizeYcOverMsg(void);
u8 OrgnizeInitEndMsg(u8*);
u8 OrgnizeReadDataMsg(u8* lpby);
u8 OrgnizeSubChgMsg(void);
u16 OrgnizeVarFrame(u8 *lpby,u8 byMsgNum,u8 byConField);
u8 OrgnizeFixFrame(u8 *lpby,u8 byConField) ;
void IEC101Process(void);
short Read_LastData(int ch,unsigned char *buf);
//ȫ�ֱ�������
extern struct IEC101_STRUCT *lpIEC101;	//IEC101��Լ˽������ָ��
#endif

