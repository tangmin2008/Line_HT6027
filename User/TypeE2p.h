#include "TypeMeter.h"

//铁电存储器中保存当前所有电量，有一个备份(占前4页 4*2*128 )
//和当前有功正向、当前有功反向、当前视在、一、二、三、四象限需量缓存区（60分钟、120字节），无备份(后4页 4*2*128)
/***********************************************/

// 第一片24C512中存储的内容：
// 1：当前所有电量、需量及需量发生时间；
// 2：历史最近1-12个月的所有电量、需量及需量发生时间
// 3：节假日、周休日、季节、时段等电表参数
// 4：轮显项等参数
#define CMon_EC_Pp0                     0
#define	CMon_EC_Pn0                     CMon_EC_Pp0+5
#define CMon_EC_Qp0                     CMon_EC_Pn0+5
#define CMon_EC_Qn0                     CMon_EC_Qp0+5
#define CMon_EC_Pp1                     CMon_EC_Qn0+5
#define	CMon_EC_Pn1                     CMon_EC_Pp1+5
#define CMon_EC_Qp1                     CMon_EC_Pn1+5
#define CMon_EC_Qn1                     CMon_EC_Qp1+5
#define CMon_EC_Pp2                     CMon_EC_Qn1+5
#define	CMon_EC_Pn2                     CMon_EC_Pp2+5
#define CMon_EC_Qp2                     CMon_EC_Pn2+5
#define CMon_EC_Qn2                     CMon_EC_Qp2+5
#define CMon_EC_Pp3                     CMon_EC_Qn2+5
#define	CMon_EC_Pn3                     CMon_EC_Pp3+5
#define CMon_EC_Qp3                     CMon_EC_Pn3+5
#define CMon_EC_Qn3                     CMon_EC_Qp3+5
#define CMon_EC_Pp4                     CMon_EC_Qn3+5
#define	CMon_EC_Pn4                     CMon_EC_Pp4+5
#define CMon_EC_Qp4                     CMon_EC_Pn4+5
#define CMon_EC_Qn4                     CMon_EC_Qp4+5
#define CMon_EC_Pp5                     CMon_EC_Qn4+5
#define	CMon_EC_Pn5                     CMon_EC_Pp5+5
#define CMon_EC_Qp5                     CMon_EC_Pn5+5
#define CMon_EC_Qn5                     CMon_EC_Qp5+5
#define CMon_EC_Pp6                     CMon_EC_Qn5+5
#define	CMon_EC_Pn6                     CMon_EC_Pp6+5
#define CMon_EC_Qp6                     CMon_EC_Pn6+5
#define CMon_EC_Qn6                     CMon_EC_Qp6+5
#define CMon_EC_Pp7                     CMon_EC_Qn6+5
#define	CMon_EC_Pn7                     CMon_EC_Pp7+5
#define CMon_EC_Qp7                     CMon_EC_Pn7+5
#define CMon_EC_Qn7                     CMon_EC_Qp7+5
#define CMon_EC_Q01                     CMon_EC_Qn7+5
#define	CMon_EC_Q02                     CMon_EC_Q01+5
#define CMon_EC_Q03                     CMon_EC_Q02+5
#define CMon_EC_Q04                     CMon_EC_Q03+5
#define CMon_EC_Q11                     CMon_EC_Q04+5
#define	CMon_EC_Q12                     CMon_EC_Q11+5
#define CMon_EC_Q13                     CMon_EC_Q12+5
#define CMon_EC_Q14                     CMon_EC_Q13+5
#define CMon_EC_Q21                     CMon_EC_Q14+5
#define	CMon_EC_Q22                     CMon_EC_Q21+5
#define CMon_EC_Q23                     CMon_EC_Q22+5
#define CMon_EC_Q24                     CMon_EC_Q23+5
#define CMon_EC_Q31                     CMon_EC_Q24+5
#define	CMon_EC_Q32                     CMon_EC_Q31+5
#define CMon_EC_Q33                     CMon_EC_Q32+5
#define CMon_EC_Q34                     CMon_EC_Q33+5
#define CMon_EC_Q41                     CMon_EC_Q34+5
#define	CMon_EC_Q42                     CMon_EC_Q41+5
#define CMon_EC_Q43                     CMon_EC_Q42+5
#define CMon_EC_Q44                     CMon_EC_Q43+5
#define CMon_EC_Q51                     CMon_EC_Q44+5
#define	CMon_EC_Q52                     CMon_EC_Q51+5
#define CMon_EC_Q53                     CMon_EC_Q52+5
#define CMon_EC_Q54                     CMon_EC_Q53+5
#define CMon_EC_Q61                     CMon_EC_Q54+5
#define	CMon_EC_Q62                     CMon_EC_Q61+5
#define CMon_EC_Q63                     CMon_EC_Q62+5
#define CMon_EC_Q64                     CMon_EC_Q63+5
#define CMon_EC_Q71                     CMon_EC_Q64+5
#define	CMon_EC_Q72                     CMon_EC_Q71+5
#define CMon_EC_Q73                     CMon_EC_Q72+5
#define CMon_EC_Q74                     CMon_EC_Q73+5  //结束地址:0x0140

#define CMon_DAY0                       CMon_EC_Q74+5  
#define CDay_HOUR0                      CMon_DAY0+5
#define CDay_InvertVal0                 CDay_HOUR0+5
#define CDay_InvertTime0                CDay_InvertVal0+5
#define CDay_TrendVal0                  CDay_InvertTime0+5
#define CDay_TrendTime0                 CDay_TrendVal0+5

#define CMon_DAY1                       CDay_TrendTime0+5  
#define CDay_HOUR1                      CMon_DAY1+5
#define CDay_InvertVal1                 CDay_HOUR1+5
#define CDay_InvertTime1                CDay_InvertVal1+5
#define CDay_TrendVal1                  CDay_InvertTime1+5
#define CDay_TrendTime1                 CDay_TrendVal1+5

#define CMon_DAY2                       CDay_TrendTime1+5  
#define CDay_HOUR2                      CMon_DAY2+5
#define CDay_InvertVal2                 CDay_HOUR2+5
#define CDay_InvertTime2                CDay_InvertVal2+5
#define CDay_TrendVal2                  CDay_InvertTime2+5
#define CDay_TrendTime2                 CDay_TrendVal2+5

#define CMon_DAY3                       CDay_TrendTime2+5  
#define CDay_HOUR3                      CMon_DAY3+5
#define CDay_InvertVal3                 CDay_HOUR3+5
#define CDay_InvertTime3                CDay_InvertVal3+5
#define CDay_TrendVal3                  CDay_InvertTime3+5
#define CDay_TrendTime3                 CDay_TrendVal3+5

#define CMon_DAY4                       CDay_TrendTime3+5  
#define CDay_HOUR4                      CMon_DAY4+5
#define CDay_InvertVal4                 CDay_HOUR4+5
#define CDay_InvertTime4                CDay_InvertVal4+5
#define CDay_TrendVal4                  CDay_InvertTime4+5
#define CDay_TrendTime4                 CDay_TrendVal4+5

#define CMon_DAY5                       CDay_TrendTime4+5  
#define CDay_HOUR5                      CMon_DAY5+5
#define CDay_InvertVal5                 CDay_HOUR5+5
#define CDay_InvertTime5                CDay_InvertVal5+5
#define CDay_TrendVal5                  CDay_InvertTime5+5
#define CDay_TrendTime5                 CDay_TrendVal5+5

#define CMon_DAY6                       CDay_TrendTime5+5  
#define CDay_HOUR6                      CMon_DAY6+5
#define CDay_InvertVal6                 CDay_HOUR6+5
#define CDay_InvertTime6                CDay_InvertVal6+5
#define CDay_TrendVal6                  CDay_InvertTime6+5
#define CDay_TrendTime6                 CDay_TrendVal6+5

#define CMon_DAY7                       CDay_TrendTime6+5  
#define CDay_HOUR7                      CMon_DAY7+5
#define CDay_InvertVal7                 CDay_HOUR7+5
#define CDay_InvertTime7                CDay_InvertVal7+5
#define CDay_TrendVal7                  CDay_InvertTime7+5
#define CDay_TrendTime7                 CDay_TrendVal7+5


#define	ClockAdj			CDay_TrendTime7+4	//（2）//结束地址:0x088D
#define ID_TpsCode                      ClockAdj+4
//事件参数均改为HEX码
#define	EPhFail_Valve		        ID_TpsCode+7		//失压起始阀值(3)			yyy.y V 
#define AT7022ChkSum		        EPhFail_Valve+7		//7022校表数据校验和(4)		//ATChk		//V1000
#define AT7022ChkSum2		        AT7022ChkSum+6		//7022e校表数据校验和2(4)		//ATChk		//V1000

#define IEC_LINK_ADDR                     AT7022ChkSum2+6      //2
#define IEC_COMM_ADDR                     IEC_LINK_ADDR+4  //2
#define LOAD0_USEADDR                     IEC_COMM_ADDR+3  //负荷
#define LOAD0_CURADDR                     LOAD0_USEADDR+3
#define RAND0_USEADDR                     LOAD0_CURADDR+3   //瞬时
#define RAND0_CURADDR                     RAND0_USEADDR+3
#define FRZD0_USEADDR                     RAND0_CURADDR+3  //日冻结
#define FRZD0_CURADDR                     FRZD0_USEADDR+3
#define SHRP0_USEADDR                     FRZD0_CURADDR+3 //整点
#define SHRP0_CURADDR                     SHRP0_USEADDR+3
#define MOND0_USEADDR                     SHRP0_CURADDR+3 //考核日
#define MOND0_CURADDR                     MOND0_USEADDR+3

#define LOAD1_USEADDR                     MOND0_CURADDR+3  //负荷
#define LOAD1_CURADDR                     LOAD1_USEADDR+3
#define RAND1_USEADDR                     LOAD1_CURADDR+3   //瞬时
#define RAND1_CURADDR                     RAND1_USEADDR+3
#define FRZD1_USEADDR                     RAND1_CURADDR+3  //日冻结
#define FRZD1_CURADDR                     FRZD1_USEADDR+3
#define SHRP1_USEADDR                     FRZD1_CURADDR+3 //整点
#define SHRP1_CURADDR                     SHRP1_USEADDR+3
#define MOND1_USEADDR                     SHRP1_CURADDR+3 //考核日
#define MOND1_CURADDR                     MOND1_USEADDR+3

#define LOAD2_USEADDR                     MOND1_CURADDR+3  //负荷
#define LOAD2_CURADDR                     LOAD2_USEADDR+3
#define RAND2_USEADDR                     LOAD2_CURADDR+3   //瞬时
#define RAND2_CURADDR                     RAND2_USEADDR+3
#define FRZD2_USEADDR                     RAND2_CURADDR+3  //日冻结
#define FRZD2_CURADDR                     FRZD2_USEADDR+3
#define SHRP2_USEADDR                     FRZD2_CURADDR+3 //整点
#define SHRP2_CURADDR                     SHRP2_USEADDR+3
#define MOND2_USEADDR                     SHRP2_CURADDR+3 //考核日
#define MOND2_CURADDR                     MOND2_USEADDR+3

#define LOAD3_USEADDR                     MOND2_CURADDR+3  //负荷
#define LOAD3_CURADDR                     LOAD3_USEADDR+3
#define RAND3_USEADDR                     LOAD3_CURADDR+3   //瞬时
#define RAND3_CURADDR                     RAND3_USEADDR+3
#define FRZD3_USEADDR                     RAND3_CURADDR+3  //日冻结
#define FRZD3_CURADDR                     FRZD3_USEADDR+3
#define SHRP3_USEADDR                     FRZD3_CURADDR+3 //整点
#define SHRP3_CURADDR                     SHRP3_USEADDR+3
#define MOND3_USEADDR                     SHRP3_CURADDR+3 //考核日
#define MOND3_CURADDR                     MOND3_USEADDR+3

#define LOAD4_USEADDR                     MOND3_CURADDR+3  //负荷
#define LOAD4_CURADDR                     LOAD4_USEADDR+3
#define RAND4_USEADDR                     LOAD4_CURADDR+3   //瞬时
#define RAND4_CURADDR                     RAND4_USEADDR+3
#define FRZD4_USEADDR                     RAND4_CURADDR+3  //日冻结
#define FRZD4_CURADDR                     FRZD4_USEADDR+3
#define SHRP4_USEADDR                     FRZD4_CURADDR+3 //整点
#define SHRP4_CURADDR                     SHRP4_USEADDR+3
#define MOND4_USEADDR                     SHRP4_CURADDR+3 //考核日
#define MOND4_CURADDR                     MOND4_USEADDR+3

#define LOAD5_USEADDR                     MOND4_CURADDR+3  //负荷
#define LOAD5_CURADDR                     LOAD5_USEADDR+3
#define RAND5_USEADDR                     LOAD5_CURADDR+3   //瞬时
#define RAND5_CURADDR                     RAND5_USEADDR+3
#define FRZD5_USEADDR                     RAND5_CURADDR+3  //日冻结
#define FRZD5_CURADDR                     FRZD5_USEADDR+3
#define SHRP5_USEADDR                     FRZD5_CURADDR+3 //整点
#define SHRP5_CURADDR                     SHRP5_USEADDR+3
#define MOND5_USEADDR                     SHRP5_CURADDR+3 //考核日
#define MOND5_CURADDR                     MOND5_USEADDR+3

#define LOAD6_USEADDR                     MOND5_CURADDR+3  //负荷
#define LOAD6_CURADDR                     LOAD6_USEADDR+3
#define RAND6_USEADDR                     LOAD6_CURADDR+3   //瞬时
#define RAND6_CURADDR                     RAND6_USEADDR+3
#define FRZD6_USEADDR                     RAND6_CURADDR+3  //日冻结
#define FRZD6_CURADDR                     FRZD6_USEADDR+3
#define SHRP6_USEADDR                     FRZD6_CURADDR+3 //整点
#define SHRP6_CURADDR                     SHRP6_USEADDR+3
#define MOND6_USEADDR                     SHRP6_CURADDR+3 //考核日
#define MOND6_CURADDR                     MOND6_USEADDR+3

#define LOAD7_USEADDR                     MOND6_CURADDR+3  //负荷
#define LOAD7_CURADDR                     LOAD7_USEADDR+3
#define RAND7_USEADDR                     LOAD7_CURADDR+3   //瞬时
#define RAND7_CURADDR                     RAND7_USEADDR+3
#define FRZD7_USEADDR                     RAND7_CURADDR+3  //日冻结
#define FRZD7_CURADDR                     FRZD7_USEADDR+3
#define SHRP7_USEADDR                     FRZD7_CURADDR+3 //整点
#define SHRP7_CURADDR                     SHRP7_USEADDR+3
#define MOND7_USEADDR                     SHRP7_CURADDR+3 //考核日
#define MOND7_CURADDR                     MOND7_USEADDR+3

#define	CH0_PAP_USEADDR                   MOND7_CURADDR+3
#define	CH0_PBP_USEADDR                   CH0_PAP_USEADDR+3
#define	CH0_PCP_USEADDR                   CH0_PBP_USEADDR+3
#define	CH0_PAN_USEADDR                   CH0_PCP_USEADDR+3
#define	CH0_PBN_USEADDR                   CH0_PAN_USEADDR+3
#define	CH0_PCN_USEADDR                   CH0_PBN_USEADDR+3
#define	CH0_PTD_USEADDR                   CH0_PCN_USEADDR+3
#define	CH0_CLR_USEADDR                   CH0_PTD_USEADDR+3

#define	CH0_PAP_CURADDR                   CH0_CLR_USEADDR+3
#define	CH0_PBP_CURADDR                   CH0_PAP_CURADDR+3
#define	CH0_PCP_CURADDR                   CH0_PBP_CURADDR+3
#define	CH0_PAN_CURADDR                   CH0_PCP_CURADDR+3
#define	CH0_PBN_CURADDR                   CH0_PAN_CURADDR+3
#define	CH0_PCN_CURADDR                   CH0_PBN_CURADDR+3
#define	CH0_PTD_CURADDR                   CH0_PCN_CURADDR+3
#define	CH0_CLR_CURADDR                   CH0_PTD_CURADDR+3




#define	CH1_PAP_USEADDR                   CH0_CLR_CURADDR+3
#define	CH1_PBP_USEADDR                   CH1_PAP_USEADDR+3
#define	CH1_PCP_USEADDR                   CH1_PBP_USEADDR+3
#define	CH1_PAN_USEADDR                   CH1_PCP_USEADDR+3
#define	CH1_PBN_USEADDR                   CH1_PAN_USEADDR+3
#define	CH1_PCN_USEADDR                   CH1_PBN_USEADDR+3
#define	CH1_PTD_USEADDR                   CH1_PCN_USEADDR+3
#define	CH1_CLR_USEADDR                   CH1_PTD_USEADDR+3

#define	CH1_PAP_CURADDR                   CH1_CLR_USEADDR+3
#define	CH1_PBP_CURADDR                   CH1_PAP_CURADDR+3
#define	CH1_PCP_CURADDR                   CH1_PBP_CURADDR+3
#define	CH1_PAN_CURADDR                   CH1_PCP_CURADDR+3
#define	CH1_PBN_CURADDR                   CH1_PAN_CURADDR+3
#define	CH1_PCN_CURADDR                   CH1_PBN_CURADDR+3
#define	CH1_PTD_CURADDR                   CH1_PCN_CURADDR+3
#define	CH1_CLR_CURADDR                   CH1_PTD_CURADDR+3

#define	CH2_PAP_USEADDR                   CH1_CLR_CURADDR+3
#define	CH2_PBP_USEADDR                   CH2_PAP_USEADDR+3
#define	CH2_PCP_USEADDR                   CH2_PBP_USEADDR+3
#define	CH2_PAN_USEADDR                   CH2_PCP_USEADDR+3
#define	CH2_PBN_USEADDR                   CH2_PAN_USEADDR+3
#define	CH2_PCN_USEADDR                   CH2_PBN_USEADDR+3
#define	CH2_PTD_USEADDR                   CH2_PCN_USEADDR+3
#define	CH2_CLR_USEADDR                   CH2_PTD_USEADDR+3

#define	CH2_PAP_CURADDR                   CH2_CLR_USEADDR+3
#define	CH2_PBP_CURADDR                   CH2_PAP_CURADDR+3
#define	CH2_PCP_CURADDR                   CH2_PBP_CURADDR+3
#define	CH2_PAN_CURADDR                   CH2_PCP_CURADDR+3
#define	CH2_PBN_CURADDR                   CH2_PAN_CURADDR+3
#define	CH2_PCN_CURADDR                   CH2_PBN_CURADDR+3
#define	CH2_PTD_CURADDR                   CH2_PCN_CURADDR+3
#define	CH2_CLR_CURADDR                   CH2_PTD_CURADDR+3

#define	CH3_PAP_USEADDR                   CH2_CLR_CURADDR+3
#define	CH3_PBP_USEADDR                   CH3_PAP_USEADDR+3
#define	CH3_PCP_USEADDR                   CH3_PBP_USEADDR+3
#define	CH3_PAN_USEADDR                   CH3_PCP_USEADDR+3
#define	CH3_PBN_USEADDR                   CH3_PAN_USEADDR+3
#define	CH3_PCN_USEADDR                   CH3_PBN_USEADDR+3
#define	CH3_PTD_USEADDR                   CH3_PCN_USEADDR+3
#define	CH3_CLR_USEADDR                   CH3_PTD_USEADDR+3

#define	CH3_PAP_CURADDR                   CH3_CLR_USEADDR+3
#define	CH3_PBP_CURADDR                   CH3_PAP_CURADDR+3
#define	CH3_PCP_CURADDR                   CH3_PBP_CURADDR+3
#define	CH3_PAN_CURADDR                   CH3_PCP_CURADDR+3
#define	CH3_PBN_CURADDR                   CH3_PAN_CURADDR+3
#define	CH3_PCN_CURADDR                   CH3_PBN_CURADDR+3
#define	CH3_PTD_CURADDR                   CH3_PCN_CURADDR+3
#define	CH3_CLR_CURADDR                   CH3_PTD_CURADDR+3

#define	CH4_PAP_USEADDR                   CH3_CLR_CURADDR+3
#define	CH4_PBP_USEADDR                   CH4_PAP_USEADDR+3
#define	CH4_PCP_USEADDR                   CH4_PBP_USEADDR+3
#define	CH4_PAN_USEADDR                   CH4_PCP_USEADDR+3
#define	CH4_PBN_USEADDR                   CH4_PAN_USEADDR+3
#define	CH4_PCN_USEADDR                   CH4_PBN_USEADDR+3
#define	CH4_PTD_USEADDR                   CH4_PCN_USEADDR+3
#define	CH4_CLR_USEADDR                   CH4_PTD_USEADDR+3

#define	CH4_PAP_CURADDR                   CH4_CLR_USEADDR+3
#define	CH4_PBP_CURADDR                   CH4_PAP_CURADDR+3
#define	CH4_PCP_CURADDR                   CH4_PBP_CURADDR+3
#define	CH4_PAN_CURADDR                   CH4_PCP_CURADDR+3
#define	CH4_PBN_CURADDR                   CH4_PAN_CURADDR+3
#define	CH4_PCN_CURADDR                   CH4_PBN_CURADDR+3
#define	CH4_PTD_CURADDR                   CH4_PCN_CURADDR+3
#define	CH4_CLR_CURADDR                   CH4_PTD_CURADDR+3

#define	CH5_PAP_USEADDR                   CH4_CLR_CURADDR+3
#define	CH5_PBP_USEADDR                   CH5_PAP_USEADDR+3
#define	CH5_PCP_USEADDR                   CH5_PBP_USEADDR+3
#define	CH5_PAN_USEADDR                   CH5_PCP_USEADDR+3
#define	CH5_PBN_USEADDR                   CH5_PAN_USEADDR+3
#define	CH5_PCN_USEADDR                   CH5_PBN_USEADDR+3
#define	CH5_PTD_USEADDR                   CH5_PCN_USEADDR+3
#define	CH5_CLR_USEADDR                   CH5_PTD_USEADDR+3

#define	CH5_PAP_CURADDR                   CH5_CLR_USEADDR+3
#define	CH5_PBP_CURADDR                   CH5_PAP_CURADDR+3
#define	CH5_PCP_CURADDR                   CH5_PBP_CURADDR+3
#define	CH5_PAN_CURADDR                   CH5_PCP_CURADDR+3
#define	CH5_PBN_CURADDR                   CH5_PAN_CURADDR+3
#define	CH5_PCN_CURADDR                   CH5_PBN_CURADDR+3
#define	CH5_PTD_CURADDR                   CH5_PCN_CURADDR+3
#define	CH5_CLR_CURADDR                   CH5_PTD_CURADDR+3

#define	CH6_PAP_USEADDR                   CH5_CLR_CURADDR+3
#define	CH6_PBP_USEADDR                   CH6_PAP_USEADDR+3
#define	CH6_PCP_USEADDR                   CH6_PBP_USEADDR+3
#define	CH6_PAN_USEADDR                   CH6_PCP_USEADDR+3
#define	CH6_PBN_USEADDR                   CH6_PAN_USEADDR+3
#define	CH6_PCN_USEADDR                   CH6_PBN_USEADDR+3
#define	CH6_PTD_USEADDR                   CH6_PCN_USEADDR+3
#define	CH6_CLR_USEADDR                   CH6_PTD_USEADDR+3

#define	CH6_PAP_CURADDR                   CH6_CLR_USEADDR+3
#define	CH6_PBP_CURADDR                   CH6_PAP_CURADDR+3
#define	CH6_PCP_CURADDR                   CH6_PBP_CURADDR+3
#define	CH6_PAN_CURADDR                   CH6_PCP_CURADDR+3
#define	CH6_PBN_CURADDR                   CH6_PAN_CURADDR+3
#define	CH6_PCN_CURADDR                   CH6_PBN_CURADDR+3
#define	CH6_PTD_CURADDR                   CH6_PCN_CURADDR+3
#define	CH6_CLR_CURADDR                   CH6_PTD_CURADDR+3

#define	CH7_PAP_USEADDR                   CH6_CLR_CURADDR+3
#define	CH7_PBP_USEADDR                   CH7_PAP_USEADDR+3
#define	CH7_PCP_USEADDR                   CH7_PBP_USEADDR+3
#define	CH7_PAN_USEADDR                   CH7_PCP_USEADDR+3
#define	CH7_PBN_USEADDR                   CH7_PAN_USEADDR+3
#define	CH7_PCN_USEADDR                   CH7_PBN_USEADDR+3
#define	CH7_PTD_USEADDR                   CH7_PCN_USEADDR+3
#define	CH7_CLR_USEADDR                   CH7_PTD_USEADDR+3

#define	CH7_PAP_CURADDR                   CH7_CLR_USEADDR+3
#define	CH7_PBP_CURADDR                   CH7_PAP_CURADDR+3
#define	CH7_PCP_CURADDR                   CH7_PBP_CURADDR+3
#define	CH7_PAN_CURADDR                   CH7_PCP_CURADDR+3
#define	CH7_PBN_CURADDR                   CH7_PAN_CURADDR+3
#define	CH7_PCN_CURADDR                   CH7_PBN_CURADDR+3
#define	CH7_PTD_CURADDR                   CH7_PCN_CURADDR+3
#define	CH7_CLR_CURADDR                   CH7_PTD_CURADDR+3

#define	TIME_CURADDR                      CH7_CLR_CURADDR+3
#define	TIME_USEADDR                      TIME_CURADDR+3
#define	CEVENT0_USEADDR                   TIME_USEADDR+3
#define	CEVENT0_CURADDR                   CEVENT0_USEADDR+3
#define	CEVENT1_USEADDR                   CEVENT0_CURADDR+3
#define	CEVENT1_CURADDR                   CEVENT1_USEADDR+3
#define	CEVENT2_USEADDR                   CEVENT1_CURADDR+3
#define	CEVENT2_CURADDR                   CEVENT2_USEADDR+3
#define	CEVENT3_USEADDR                   CEVENT2_CURADDR+3
#define	CEVENT3_CURADDR                   CEVENT3_USEADDR+3
#define	CEVENT4_USEADDR                   CEVENT3_CURADDR+3
#define	CEVENT4_CURADDR                   CEVENT4_USEADDR+3
#define	CEVENT5_USEADDR                   CEVENT4_CURADDR+3
#define	CEVENT5_CURADDR                   CEVENT5_USEADDR+3
#define	CEVENT6_USEADDR                   CEVENT5_CURADDR+3
#define	CEVENT6_CURADDR                   CEVENT6_USEADDR+3
#define	CEVENT7_USEADDR                   CEVENT6_CURADDR+3
#define	CEVENT7_CURADDR                   CEVENT7_USEADDR+3

#define	ID_ADDR                           CEVENT7_CURADDR+3
#define	PW_ADDR                           ID_ADDR+12
#define CONST_H                           PW_ADDR+2  
#define BASE_CURRENT                      CONST_H+5
#define PW_IB                             BASE_CURRENT+5

#define ErrorRegAddr			 E2P_PGLEN-0x800

#define E2P_PGLEN		         0x1000//0x0B00//0xAAA//0x1554		//数据备份页长度

#define E2ONE_RECORD_LEN                 54
#define MONTH_RECORD_NUM                 12
#define E2ONE_RECORD_SIZE                60
#define MONTH_RECORD_SIZE                (MONTH_RECORD_NUM*E2ONE_RECORD_SIZE)

#define RAND_RECORD_NUM                   3
#define RAND_RECORD_SIZE                (RAND_RECORD_NUM*E2ONE_RECORD_SIZE)


#define ONE_RECORD_LEN                   54
#define LOAD_RECORD_NUM                  96
#define ONE_RECORD_SIZE                  64
#define LOAD_RECORD_SIZE                 (LOAD_RECORD_NUM*ONE_RECORD_SIZE)

#define DAY_RECORD_NUM                   40
#define DAY_RECORD_SIZE                  (DAY_RECORD_NUM*ONE_RECORD_SIZE)

#define HOUR_RECORD_NUM                  264
#define HOUR_RECORD_SIZE                 (HOUR_RECORD_NUM*ONE_RECORD_SIZE)


//FLASH
#define LOAD_DATA_ADDR                   0
#define DAY_DATA_ADDR                    0xC000 //负荷记录留48k空间
#define HOUR_DATA_ADDR                  (0xC000+0x5000) //20k空间
#define END_DATA_ADDR                  (HOUR_DATA_ADDR+0x21000) //132K空间


#define FlashChip		YesCheck

///////////   FLASH	//前300页为负荷记录
#if (FlashChip == YesCheck)
#define	Mon_Data_Addr	 0x25800//0x19000	//(1840)三页多
#else 
#define	Mon_Data_Addr	 0x7000///0x1554//0x19000	//(1840)三页多
#endif
#define	Tamper_Data_Addr	 Mon_Data_Addr + 0xA00	//(6000)



#define FlashAT45DB		NoCheck		//ATMEL的AT25DF041A系列芯片 AT26DF081、AT26DF161
#define FlashAT161Series	YesCheck	//ATMEL的AT45161芯片 容量2M字节

//#define GeneralLoadCurveSta		0x00000000		//0
//#define GeneralLoadCurveEnd		0x00090000		//384K  普通负荷曲线范围，
//#define GeneralMonCurveSta		0x00090000		//0
//#define GeneralMonCurveEnd		0x000A0000		//64K  历史数据范围，
//#define GeneralTamperCurveSta	0x000A0000		//0
//#define GeneralTamperCurveEnd	0x000B0000		//64K  事件记录范围，

//------------------DLMS Profile tab数据存储----------------
#define EventLog1Page					0x00000000
///#define EventLog1Page					GeneralTamperCurveEnd			//事件1记录首页码(6)
#define EventLog2Page					EventLog1Page+6*1						//事件2记录首页码(6)
#define EventLog3Page					EventLog1Page+6*2						//事件3记录首页码(6)
#define EventLog4Page					EventLog1Page+6*3						//事件4记录首页码(6)
#define EventLog5Page					EventLog1Page+6*4						//事件5记录首页码(6)
#define EventLog6Page					EventLog1Page+6*5						//事件6记录首页码(6)
#define EventLog7Page					EventLog1Page+6*6						//事件7记录首页码(6)
#define EventLog8Page					EventLog1Page+6*7						//事件8记录首页码(6)
#define EventLog9Page					EventLog1Page+6*8						//事件9记录首页码(6)
#define EventLog10Page					EventLog1Page+6*9						//事件10记录首页码(6)
#define EventLog11Page					EventLog1Page+6*10					//事件11记录首页码(6)

#define Billing_period1Page				EventLog11Page+6*1				//账单记录1首页码(6)		//12个月结算
#define Billing_period2Page				EventLog11Page+6*2			//账单记录1首页码(20)		//31天冻结
#define Load_profile1Page				EventLog11Page+6*3			//负荷曲线1首页码(20)		
#define Load_profile2Page				Load_profile1Page+40				//负荷曲线1首页码(20)	
#define PowerQuality_profilePage			Load_profile2Page+6				//负荷曲线2首页码(200)		

#define Profile_Page_end				PowerQuality_profilePage+40

#define GeneralMonCurveSta		Profile_Page_end		//0
#define GeneralMonCurveEnd		Profile_Page_end+0x10000		//64K  历史数据范围，
#define GeneralTamperCurveSta	GeneralMonCurveEnd		//0
//#define GeneralTamperCurveEnd	0x000E0000		//64K  事件记录范围，
#define GeneralTamperCurveEnd	GeneralTamperCurveSta+0x10000		//64K  事件记录范围，
#define GeneralLoadCurveSta		GeneralTamperCurveEnd		//0
#define GeneralLoadCurveEnd		GeneralLoadCurveSta+0x60000		//384K  普通负荷曲线范围，


//以下是按照4MFLASH进行操作的，升级部分放到FLASH的最后512K字节部分..
//定义FLASH存储空间，目前定义空间为AT25DF041A的最后（256*2+4）K =516K字节的空间。
//此FLASH每一页为4K即4096字节，檫除也只能是4K一檫除
// 2 BYTE
#define FLASH_512	 0x200  			//512..
#define FLASH_4K	 0x1000  			//4096  = 4*1024..
#define FLASH_16K	 0x4000  			//16384 = 16*1024..
// 3 BYTE
#define FLASH_64K	 0x10000
#define FLASH_252K	 0x3F000			//252K..
#define FLASH_256K	 0x40000			//256K..
#define FLASH_184K	 0x2E000	//8213内部flash存储用户程序的空间192K-8K=184K

//升级程序参数--备用..
//895 37F000h 37FFFFh
#define NEW_FIRMWARE_STATE			(unsigned long)(FLASH_4K)*383 				// 37F000h = 895*4K(1024) //升级初始化，前面4个字节初始化为：0xA5 B6 C7 D8
// page
// 4K = 4096
// 升级程序暂时定义为：128+252K字节（包括8个字节的CRC校验）如需要扩充到256K自己的升级，则需要在每块后面扩4K，即252K-->256K..
//升级程序改成可变长度，只要不小于104个字节一块就可以，地址则也需做灵活处理程序里认起始和结束地址..
// NEW FIRMWARE UPGRADE -..
#define UPGRADE_NEW_START				(unsigned long)(FLASH_4K)*384					// 0x180000 = 384*4K(1024) 
//4k --暂时定义存放128字节用，以及版本信息等..
#define UPGRADE_START_PROGRAM		UPGRADE_NEW_START + FLASH_4K 					// 388*4K(1024)
#define UPGRADE_VECTOR_START 	 	UPGRADE_NEW_START + 73*FLASH_512 			// 中断向量表起始地址..
#define UPGRADE_VECTOR_END 	    	UPGRADE_NEW_START + 74*FLASH_512 			// 中断向量结束也是0x10000地址的开始 <1---74>..
//<75---504>.. 
#define UPGRADE_END_PROGRAM			UPGRADE_START_PROGRAM + FLASH_252K 		// 898--959 //4K(1024)

// 保存升级前的程序 0LD FIRMWARE
#define CURRENT_NEW_START				(unsigned long)(FLASH_4K)*448					//(384+64)*FLASH_4K) 									
// 4k---存放当前文件，不使用128个字节头文件，直接存放当前512字节的应用程序中断向量表，檫除应用程序..
#define CURRENT_START_PROGRAM		CURRENT_NEW_START + FLASH_4K 					// 452*4K(1024)

#define CURRENT_VECTOR_START 	    CURRENT_NEW_START + 73*FLASH_512 			// 当前中断向量表起始地址..
#define CURRENT_VECTOR_END 	    	CURRENT_NEW_START + 74*FLASH_512 			// 当前中断向量结束也是0x10000地址的开始 <1---74>..
//<75---504> ，一共是504块512个字节数据..
#define CURRENT_END_PROGRAM			CURRENT_START_PROGRAM + FLASH_252K 		// 898--959 //4K(1024)

