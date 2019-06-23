#include "TypeMeter.h"
#ifndef __TYPERAM_H__
#define __TYPERAM_H__
// *******************************************************************
// Data type define
// *******************************************************************
typedef unsigned char BCD1[1];
typedef unsigned char BCD2[2];
typedef unsigned char BCD3[3];
typedef unsigned char BCD4[4];
typedef unsigned char BCD5[5];
typedef unsigned char BCD6[6];
typedef unsigned char BCD7[7];
typedef unsigned char BCD8[8];
typedef unsigned char BCD9[9];

typedef signed long long INT64S;
typedef unsigned int INT32U;
typedef signed int INT32S;
typedef unsigned char INT8U;
typedef signed short INT16S;
typedef unsigned short INT16U;

/*״̬�ֶ���------------------------*/
typedef union{
  unsigned short word;
  struct{
    unsigned char b0  :1;
    unsigned char b1  :1;
    unsigned char b2  :1;
    unsigned char b3  :1;
    unsigned char b4  :1;
    unsigned char b5  :1;
    unsigned char b6  :1;
    unsigned char b7  :1;
    unsigned char b8  :1;
    unsigned char b9  :1;
    unsigned char b10 :1;
    unsigned char b11 :1;
    unsigned char b12 :1;
    unsigned char b13 :1;
    unsigned char b14 :1;
    unsigned char b15 :1;
  }bit;
}WORD_STRUCT;

typedef union{
  unsigned char byte;
  struct{
    unsigned char b0  :1;
    unsigned char b1  :1;
    unsigned char b2  :1;
    unsigned char b3  :1;
    unsigned char b4  :1;
    unsigned char b5  :1;
    unsigned char b6  :1;
    unsigned char b7  :1;
  }bit;
}BYTE_STRUCT;
/*
typedef struct
{
	unsigned char* SDA_DIR;
	unsigned char* SCL_DIR;
	unsigned char* SDA_IN;
	unsigned char SDA;
	unsigned char SCL;
}I2CPARA;							//I2C�ӿ�оƬ��CPU�ϵ�IO�����ò���
*/

typedef struct
{
	unsigned long* SDA_DIR;
	unsigned long* SCL_DIR;
	unsigned long* SDA_IN;
	unsigned long SDA;
	unsigned long SCL;
}I2CPARA;							//I2C�ӿ�оƬ��CPU�ϵ�IO�����ò���

typedef struct
{
	unsigned char RCMon_EC_P0[ECDIGIT+1];	//��ǰ�����й��ܵ���(5+1)
	unsigned char RCMon_EC_P1[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_P2[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_P3[ECDIGIT+1];	//��ǰ�����й�ƽ����(5+1)
	unsigned char RCMon_EC_P4[ECDIGIT+1];	//��ǰ�����й��ȵ���(5+1)
	unsigned char RCMon_EC_P5[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_P6[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_P7[ECDIGIT+1];	//��ǰ�����й�ƽ����(5+1)
	unsigned char RCMon_EC_P8[ECDIGIT+1];	//��ǰ�����й��ȵ���(5+1)

	unsigned char RCMon_EC_Pp0[ECDIGIT+1];	//��ǰ�����й��ܵ���(5+1)
	unsigned char RCMon_EC_Pp1[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pp2[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pp3[ECDIGIT+1];	//��ǰ�����й�ƽ����(5+1)
	unsigned char RCMon_EC_Pp4[ECDIGIT+1];	//��ǰ�����й��ȵ���(5+1)
	unsigned char RCMon_EC_Pp5[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pp6[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pp7[ECDIGIT+1];	//��ǰ�����й�ƽ����(5+1)
	unsigned char RCMon_EC_Pp8[ECDIGIT+1];	//��ǰ�����й��ȵ���(5+1)

	unsigned char RCMon_EC_Pn0[ECDIGIT+1];	//��ǰ�����й��ܵ���(5+1)
	unsigned char RCMon_EC_Pn1[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pn2[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pn3[ECDIGIT+1];	//��ǰ�����й�ƽ����(5+1)
	unsigned char RCMon_EC_Pn4[ECDIGIT+1];	//��ǰ�����й��ȵ���(5+1)
	unsigned char RCMon_EC_Pn5[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pn6[ECDIGIT+1];	//��ǰ�����й������(5+1)
	unsigned char RCMon_EC_Pn7[ECDIGIT+1];	//��ǰ�����й�ƽ����(5+1)
	unsigned char RCMon_EC_Pn8[ECDIGIT+1];	//��ǰ�����й��ȵ���(5+1)

	unsigned char RCMon_EC_Qp0[ECDIGIT+1];	//��ǰ�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Qp1[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qp2[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qp3[ECDIGIT+1];	//��ǰ�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Qp4[ECDIGIT+1];	//��ǰ�����޹��ȵ���(5+1)
	unsigned char RCMon_EC_Qp5[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qp6[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qp7[ECDIGIT+1];	//��ǰ�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Qp8[ECDIGIT+1];	//��ǰ�����޹��ȵ���(5+1)

	unsigned char RCMon_EC_Qn0[ECDIGIT+1];	//��ǰ�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Qn1[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qn2[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qn3[ECDIGIT+1];	//��ǰ�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Qn4[ECDIGIT+1];	//��ǰ�����޹��ȵ���(5+1)
	unsigned char RCMon_EC_Qn5[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qn6[ECDIGIT+1];	//��ǰ�����޹������(5+1)
	unsigned char RCMon_EC_Qn7[ECDIGIT+1];	//��ǰ�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Qn8[ECDIGIT+1];	//��ǰ�����޹��ȵ���(5+1)

#if(MeterPhase==Threephase)
	unsigned char RCMon_EC_Q10[ECDIGIT+1];	//��ǰ1�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q11[ECDIGIT+1];	//��ǰ1�����޹������(5+1)
	unsigned char RCMon_EC_Q12[ECDIGIT+1];	//��ǰ1�����޹������(5+1)
	unsigned char RCMon_EC_Q13[ECDIGIT+1];	//��ǰ1�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q14[ECDIGIT+1];	//��ǰ1�����޹��ȵ���(5+1)
	unsigned char RCMon_EC_Q15[ECDIGIT+1];	//��ǰ1�����޹������(5+1)
	unsigned char RCMon_EC_Q16[ECDIGIT+1];	//��ǰ1�����޹������(5+1)
	unsigned char RCMon_EC_Q17[ECDIGIT+1];	//��ǰ1�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q18[ECDIGIT+1];	//��ǰ1�����޹��ȵ���(5+1)

	unsigned char RCMon_EC_Q20[ECDIGIT+1];	//��ǰ2�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q21[ECDIGIT+1];	//��ǰ2�����޹������(5+1)
	unsigned char RCMon_EC_Q22[ECDIGIT+1];	//��ǰ2�����޹������(5+1)
	unsigned char RCMon_EC_Q23[ECDIGIT+1];	//��ǰ2�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q24[ECDIGIT+1];	//��ǰ2�����޹��ȵ���(5+1)
	unsigned char RCMon_EC_Q25[ECDIGIT+1];	//��ǰ2�����޹������(5+1)
	unsigned char RCMon_EC_Q26[ECDIGIT+1];	//��ǰ2�����޹������(5+1)
	unsigned char RCMon_EC_Q27[ECDIGIT+1];	//��ǰ2�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q28[ECDIGIT+1];	//��ǰ2�����޹��ȵ���(5+1)

	unsigned char RCMon_EC_Q30[ECDIGIT+1];	//��ǰ3�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q31[ECDIGIT+1];	//��ǰ3�����޹������(5+1)
	unsigned char RCMon_EC_Q32[ECDIGIT+1];	//��ǰ3�����޹������(5+1)
	unsigned char RCMon_EC_Q33[ECDIGIT+1];	//��ǰ3�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q34[ECDIGIT+1];	//��ǰ3�����޹��ȵ���(5+1)
	unsigned char RCMon_EC_Q35[ECDIGIT+1];	//��ǰ3�����޹������(5+1)
	unsigned char RCMon_EC_Q36[ECDIGIT+1];	//��ǰ3�����޹������(5+1)
	unsigned char RCMon_EC_Q37[ECDIGIT+1];	//��ǰ3�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q38[ECDIGIT+1];	//��ǰ3�����޹��ȵ���(5+1)

	unsigned char RCMon_EC_Q40[ECDIGIT+1];	//��ǰ4�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q41[ECDIGIT+1];	//��ǰ4�����޹������(5+1)
	unsigned char RCMon_EC_Q42[ECDIGIT+1];	//��ǰ4�����޹������(5+1)
	unsigned char RCMon_EC_Q43[ECDIGIT+1];	//��ǰ4�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q44[ECDIGIT+1];	//��ǰ4�����޹��ȵ���(5+1)
	unsigned char RCMon_EC_Q45[ECDIGIT+1];	//��ǰ4�����޹������(5+1)
	unsigned char RCMon_EC_Q46[ECDIGIT+1];	//��ǰ4�����޹������(5+1)
	unsigned char RCMon_EC_Q47[ECDIGIT+1];	//��ǰ4�����޹�ƽ����(5+1)
	unsigned char RCMon_EC_Q48[ECDIGIT+1];	//��ǰ4�����޹��ȵ���(5+1)

	unsigned char RCMon_EC_Sp0[ECDIGIT+1];	//��ǰ���������ܵ���(5+1)
	unsigned char RCMon_EC_Sp1[ECDIGIT+1];	//��ǰ�������ڼ����(5+1)
	unsigned char RCMon_EC_Sp2[ECDIGIT+1];	//��ǰ�������ڷ����(5+1)
	unsigned char RCMon_EC_Sp3[ECDIGIT+1];	//��ǰ��������ƽ����(5+1)
	unsigned char RCMon_EC_Sp4[ECDIGIT+1];	//��ǰ�������ڹȵ���(5+1)
	unsigned char RCMon_EC_Sp5[ECDIGIT+1];	//��ǰ�������ڼ����(5+1)
	unsigned char RCMon_EC_Sp6[ECDIGIT+1];	//��ǰ�������ڷ����(5+1)
	unsigned char RCMon_EC_Sp7[ECDIGIT+1];	//��ǰ��������ƽ����(5+1)
	unsigned char RCMon_EC_Sp8[ECDIGIT+1];	//��ǰ�������ڹȵ���(5+1)

	unsigned char RCMon_EC_Sn0[ECDIGIT+1];	//��ǰ���������ܵ���(5+1)
	unsigned char RCMon_EC_Sn1[ECDIGIT+1];	//��ǰ�������ڼ����(5+1)
	unsigned char RCMon_EC_Sn2[ECDIGIT+1];	//��ǰ�������ڷ����(5+1)
	unsigned char RCMon_EC_Sn3[ECDIGIT+1];	//��ǰ��������ƽ����(5+1)
	unsigned char RCMon_EC_Sn4[ECDIGIT+1];	//��ǰ�������ڹȵ���(5+1)	
	unsigned char RCMon_EC_Sn5[ECDIGIT+1];	//��ǰ�������ڼ����(5+1)
	unsigned char RCMon_EC_Sn6[ECDIGIT+1];	//��ǰ�������ڷ����(5+1)
	unsigned char RCMon_EC_Sn7[ECDIGIT+1];	//��ǰ��������ƽ����(5+1)
	unsigned char RCMon_EC_Sn8[ECDIGIT+1];	//��ǰ�������ڹȵ���(5+1)
	#endif
/*
	unsigned char RCMon_EC_G0[ECDIGIT+1];	//��ǰ�����ܵ���(5+1)
	unsigned char RCMon_EC_Bp0[ECDIGIT+1];	//��ǰ�����й������ܵ���(5+1)				
	unsigned char RCMon_EC_Bn0[ECDIGIT+1];	//��ǰ�����й������ܵ���(5+1)				
	unsigned char RCMon_EC_Hp0[ECDIGIT+1];	//��ǰ�����й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_Hn0[ECDIGIT+1];	//��ǰ�����й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_Cu0[ECDIGIT+1];	//��ǰͭ���й��ܵ��ܲ�����(5+1)								
	unsigned char RCMon_EC_Fe0[ECDIGIT+1];	//��ǰ�����й��ܵ��ܲ�����(5+1)				
	
	unsigned char RCMon_EC_PpA0[ECDIGIT+1];	//��ǰA�������й��ܵ���(5+1)
	unsigned char RCMon_EC_PpA1[ECDIGIT+1];	//��ǰA�������й������(5+1)
	unsigned char RCMon_EC_PpA2[ECDIGIT+1];	//��ǰA�������й������(5+1)
	unsigned char RCMon_EC_PpA3[ECDIGIT+1];	//��ǰA�������й�ƽ����(5+1)
	unsigned char RCMon_EC_PpA4[ECDIGIT+1];	//��ǰA�������й��ȵ���(5+1)

	unsigned char RCMon_EC_PnA0[ECDIGIT+1];	//��ǰA�෴���й��ܵ���(5+1)
	unsigned char RCMon_EC_PnA1[ECDIGIT+1];	//��ǰA�෴���й������(5+1)
	unsigned char RCMon_EC_PnA2[ECDIGIT+1];	//��ǰA�෴���й������(5+1)
	unsigned char RCMon_EC_PnA3[ECDIGIT+1];	//��ǰA�෴���й�ƽ����(5+1)
	unsigned char RCMon_EC_PnA4[ECDIGIT+1];	//��ǰA�෴���й��ȵ���(5+1)

	unsigned char RCMon_EC_Q1A0[ECDIGIT+1];	//��ǰA��1�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q2A0[ECDIGIT+1];	//��ǰA��2�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q3A0[ECDIGIT+1];	//��ǰA��3�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q4A0[ECDIGIT+1];	//��ǰA��4�����޹��ܵ���(5+1)

	unsigned char RCMon_EC_SpA0[ECDIGIT+1];	//��ǰA�����������ܵ���(5+1)
	unsigned char RCMon_EC_SnA0[ECDIGIT+1];	//��ǰA�෴�������ܵ���(5+1)

	unsigned char RCMon_EC_GA0[ECDIGIT+1];	//��ǰA������ܵ���(5+1)
	unsigned char RCMon_EC_BpA0[ECDIGIT+1];	//��ǰA�������й������ܵ���(5+1)				
	unsigned char RCMon_EC_BnA0[ECDIGIT+1];	//��ǰA�෴���й������ܵ���(5+1)				
	unsigned char RCMon_EC_HpA0[ECDIGIT+1];	//��ǰA�������й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_HnA0[ECDIGIT+1];	//��ǰA�෴���й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_CuA0[ECDIGIT+1];	//��ǰA��ͭ���й��ܵ��ܲ�����(5+1)								
	unsigned char RCMon_EC_FeA0[ECDIGIT+1];	//��ǰA�������й��ܵ��ܲ�����(5+1)				

	unsigned char RCMon_EC_PpB0[ECDIGIT+1];	//��ǰB�������й��ܵ���(5+1)
	unsigned char RCMon_EC_PpB1[ECDIGIT+1];	//��ǰB�������й������(5+1)
	unsigned char RCMon_EC_PpB2[ECDIGIT+1];	//��ǰB�������й������(5+1)
	unsigned char RCMon_EC_PpB3[ECDIGIT+1];	//��ǰB�������й�ƽ����(5+1)
	unsigned char RCMon_EC_PpB4[ECDIGIT+1];	//��ǰB�������й��ȵ���(5+1)

	unsigned char RCMon_EC_PnB0[ECDIGIT+1];	//��ǰB�෴���й��ܵ���(5+1)
	unsigned char RCMon_EC_PnB1[ECDIGIT+1];	//��ǰB�෴���й������(5+1)
	unsigned char RCMon_EC_PnB2[ECDIGIT+1];	//��ǰB�෴���й������(5+1)
	unsigned char RCMon_EC_PnB3[ECDIGIT+1];	//��ǰB�෴���й�ƽ����(5+1)
	unsigned char RCMon_EC_PnB4[ECDIGIT+1];	//��ǰB�෴���й��ȵ���(5+1)

	unsigned char RCMon_EC_Q1B0[ECDIGIT+1];	//��ǰB��1�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q2B0[ECDIGIT+1];	//��ǰB��2�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q3B0[ECDIGIT+1];	//��ǰB��3�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q4B0[ECDIGIT+1];	//��ǰB��4�����޹��ܵ���(5+1)

	unsigned char RCMon_EC_SpB0[ECDIGIT+1];	//��ǰB�����������ܵ���(5+1)
	unsigned char RCMon_EC_SnB0[ECDIGIT+1];	//��ǰB�෴�������ܵ���(5+1)

	unsigned char RCMon_EC_GB0[ECDIGIT+1];	//��ǰB������ܵ���(5+1)
	unsigned char RCMon_EC_BpB0[ECDIGIT+1];	//��ǰB�������й������ܵ���(5+1)				
	unsigned char RCMon_EC_BnB0[ECDIGIT+1];	//��ǰB�෴���й������ܵ���(5+1)				
	unsigned char RCMon_EC_HpB0[ECDIGIT+1];	//��ǰB�������й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_HnB0[ECDIGIT+1];	//��ǰB�෴���й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_CuB0[ECDIGIT+1];	//��ǰB��ͭ���й��ܵ��ܲ�����(5+1)								
	unsigned char RCMon_EC_FeB0[ECDIGIT+1];	//��ǰB�������й��ܵ��ܲ�����(5+1)				

	unsigned char RCMon_EC_PpC0[ECDIGIT+1];	//��ǰC�������й��ܵ���(5+1)
	unsigned char RCMon_EC_PpC1[ECDIGIT+1];	//��ǰC�������й������(5+1)
	unsigned char RCMon_EC_PpC2[ECDIGIT+1];	//��ǰC�������й������(5+1)
	unsigned char RCMon_EC_PpC3[ECDIGIT+1];	//��ǰC�������й�ƽ����(5+1)
	unsigned char RCMon_EC_PpC4[ECDIGIT+1];	//��ǰC�������й��ȵ���(5+1)

	unsigned char RCMon_EC_PnC0[ECDIGIT+1];	//��ǰC�෴���й��ܵ���(5+1)
	unsigned char RCMon_EC_PnC1[ECDIGIT+1];	//��ǰC�෴���й������(5+1)
	unsigned char RCMon_EC_PnC2[ECDIGIT+1];	//��ǰC�෴���й������(5+1)
	unsigned char RCMon_EC_PnC3[ECDIGIT+1];	//��ǰC�෴���й�ƽ����(5+1)
	unsigned char RCMon_EC_PnC4[ECDIGIT+1];	//��ǰC�෴���й��ȵ���(5+1)

	unsigned char RCMon_EC_Q1C0[ECDIGIT+1];	//��ǰC��1�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q2C0[ECDIGIT+1];	//��ǰC��2�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q3C0[ECDIGIT+1];	//��ǰC��3�����޹��ܵ���(5+1)
	unsigned char RCMon_EC_Q4C0[ECDIGIT+1];	//��ǰC��4�����޹��ܵ���(5+1)

	unsigned char RCMon_EC_SpC0[ECDIGIT+1];	//��ǰC�����������ܵ���(5+1)
	unsigned char RCMon_EC_SnC0[ECDIGIT+1];	//��ǰC�෴�������ܵ���(5+1)

	unsigned char RCMon_EC_GC0[ECDIGIT+1];	//��ǰC������ܵ���(5+1)
	unsigned char RCMon_EC_BpC0[ECDIGIT+1];	//��ǰC�������й������ܵ���(5+1)				
	unsigned char RCMon_EC_BnC0[ECDIGIT+1];	//��ǰC�෴���й������ܵ���(5+1)				
	unsigned char RCMon_EC_HpC0[ECDIGIT+1];	//��ǰC�������й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_HnC0[ECDIGIT+1];	//��ǰC�෴���й�г���ܵ���(5+1)				
	unsigned char RCMon_EC_CuC0[ECDIGIT+1];	//��ǰC��ͭ���й��ܵ��ܲ�����(5+1)								
	unsigned char RCMon_EC_FeC0[ECDIGIT+1];	//��ǰC�������й��ܵ��ܲ�����(5+1)				
*/
}ECRAM;			//696�ֽ�
/*
typedef struct
{
	unsigned short MDBuf_Pp[60];		//��ǰ�����й�����������(2)
	unsigned short MDBuf_Pn[60];		//��ǰ�����й�����������(2)
	unsigned short MDBuf_Q1[60];		//��ǰ1�����޹�����������(2)
	unsigned short MDBuf_Q2[60];		//��ǰ2�����޹�����������(2)
	unsigned short MDBuf_Q3[60];		//��ǰ3�����޹�����������(2)
	unsigned short MDBuf_Q4[60];		//��ǰ4�����޹�����������(2)
	unsigned short MDBuf_Sp[60];		//��ǰ������������������(2)
	unsigned short MDBuf_Sn[60];		//��ǰ������������������(2)
}MDBUFZ;

typedef struct
{
	unsigned short MDBuf_PpA[60];		//��ǰA�������й�����������(2)
	unsigned short MDBuf_PnA[60];		//��ǰA�෴���й�����������(2)
	unsigned short MDBuf_Q1A[60];		//��ǰA��1�����޹�����������(2)
	unsigned short MDBuf_Q2A[60];		//��ǰA��2�����޹�����������(2)
	unsigned short MDBuf_Q3A[60];		//��ǰA��3�����޹�����������(2)
	unsigned short MDBuf_Q4A[60];		//��ǰA��4�����޹�����������(2)
	unsigned short MDBuf_SpA[60];		//��ǰA��������������������(2)
	unsigned short MDBuf_SnA[60];		//��ǰA�෴����������������(2)
}MDBUFA;

typedef struct
{
	unsigned short MDBuf_PpB[60];		//��ǰA�������й�����������(2)
	unsigned short MDBuf_PnB[60];		//��ǰA�෴���й�����������(2)
	unsigned short MDBuf_Q1B[60];		//��ǰA��1�����޹�����������(2)
	unsigned short MDBuf_Q2B[60];		//��ǰA��2�����޹�����������(2)
	unsigned short MDBuf_Q3B[60];		//��ǰA��3�����޹�����������(2)
	unsigned short MDBuf_Q4B[60];		//��ǰA��4�����޹�����������(2)
	unsigned short MDBuf_SpB[60];		//��ǰA��������������������(2)
	unsigned short MDBuf_SnB[60];		//��ǰA�෴����������������(2)
}MDBUFB;

typedef struct
{
	unsigned short MDBuf_PpC[60];		//��ǰC�������й�����������(2)
	unsigned short MDBuf_PnC[60];		//��ǰC�෴���й�����������(2)
	unsigned short MDBuf_Q1C[60];		//��ǰC��1�����޹�����������(2)
	unsigned short MDBuf_Q2C[60];		//��ǰC��2�����޹�����������(2)
	unsigned short MDBuf_Q3C[60];		//��ǰC��3�����޹�����������(2)
	unsigned short MDBuf_Q4C[60];		//��ǰC��4�����޹�����������(2)
	unsigned short MDBuf_SpC[60];		//��ǰC��������������������(2)
	unsigned short MDBuf_SnC[60];		//��ǰC�෴����������������(2)
}MDBUFC;

typedef struct
{
	unsigned long MD_CumPp;			//1�������й�������			//PWMD			
	unsigned long MD_CumPn;
	unsigned long MD_CumQ1;
	unsigned long MD_CumQ2;
	unsigned long MD_CumQ3;
	unsigned long MD_CumQ4;
	unsigned long MD_CumSp;			
	unsigned long MD_CumSn;
	
	unsigned long MD_CumPpA;		//1������A���й�������			//PWMD			
	unsigned long MD_CumPnA;
	unsigned long MD_CumQ1A;
	unsigned long MD_CumQ2A;
	unsigned long MD_CumQ3A;
	unsigned long MD_CumQ4A;
	unsigned long MD_CumSpA;		
	unsigned long MD_CumSnA;

	unsigned long MD_CumPpB;		//1������B���й�������			//PWMD			
	unsigned long MD_CumPnB;
	unsigned long MD_CumQ1B;
	unsigned long MD_CumQ2B;
	unsigned long MD_CumQ3B;
	unsigned long MD_CumQ4B;
	unsigned long MD_CumSpB;		
	unsigned long MD_CumSnB;

	unsigned long MD_CumPpC;		//1������C���й�������			//PWMD			
	unsigned long MD_CumPnC;
	unsigned long MD_CumQ1C;
	unsigned long MD_CumQ2C;
	unsigned long MD_CumQ3C;
	unsigned long MD_CumQ4C;
	unsigned long MD_CumSpC;		
	unsigned long MD_CumSnC;

	unsigned short MD_CumPCnt;			//1�������й������ۼӴ���	//PWMD			
	unsigned short MD_CumQCnt;			//1�������޹������ۼӴ���	//PWMD			
	unsigned short MD_CumSCnt;			//1���������ڹ����ۼӴ���	//PWMD			

//�������������������ڷ����л�ʱ�����㣬���������������������ڷ����л�ʱ����
	unsigned char MD_Cunt;				//�����Ӽ�������ʱ��Ҫ��ǰ��ȡ���������建��������
	unsigned char MD_CuntT;				//�����Ӽ����������ʱ��Ҫ��ǰ��ȡ���������建��������
	unsigned char MD_BufAds;			//��ǰ������������ַ

	unsigned char MDPCT;				//�������������������������ʱ�����������Ƭ��
	unsigned char MD_PCT;				//����Ƭ��������	
	unsigned char MDPMinCT;				//��������ʱ���ڷ��Ӽ�����
	unsigned char MD_60T;				//60����/��������

	unsigned char MDInitialTime;		//�ϵ硢���㡢ʱ�ӵ������������ڱ��
	unsigned char FeeMDInitialTime;		//ʱ���л�������������ڱ��

	unsigned char MD_Sec;				//��
	unsigned char MD_Sec_Third;			//1/3��
	unsigned char MD_Min;				//����
	unsigned char MD_Fee;				//��ǰ��������
	
	unsigned char AvePowerTime;			//1����ƽ�����ʼ�����ʼʱ��
	BCD3		AvePower_P;				//1����ƽ������			

	BCD3		MD_P;					//��ǰ�й�����			
	BCD3		MD_Q;					//��ǰ�޹�����			
	BCD3		MD_S;					//��ǰ��������			
	            
	BCD3		MD_Pp;					//�����й�����
	BCD3		MD_Pn;					//�����й�����
	BCD3		MD_Q1;					//��һ�����޹�����
	BCD3		MD_Q2;					//�ڶ������޹�����
	BCD3		MD_Q3;					//���������޹�����
	BCD3		MD_Q4;					//���������޹�����
	BCD3		MD_Sp;					//������������			
	BCD3		MD_Sn;					//������������			

}MDVAR;					//3840�ֽ�
*/
#pragma pack(2)
#if 0
typedef struct
{
  unsigned char cmon_day;
  unsigned char cday_hour;
  unsigned int  invert_val;
  unsigned char invert_time;
  unsigned int  trend_val;
  unsigned char trend_time;
}EC_PARA;
#else
typedef struct
{
  float cmon_day;
  float cday_hour;
  float invert_val;
  float invert_time;
  float trend_val;
  float trend_time;
}EC_PARA;
#endif
#pragma pack()
typedef struct
{
	unsigned short PL_CumPp[8];				//��ǰ�����й��������������(2)
	unsigned short PL_CumPn[8];				//��ǰ�����й��������������(2)
	unsigned short PL_CumQp[8];				//��ǰ�����޹��������������(2)
	unsigned short PL_CumQn[8];				//��ǰ�����޹��������������(2)
        unsigned short PL_CumQ[8][4];				//��ǰ4�����޹��������������(2)

	unsigned char PL_ChkPp[8];				//��ǰ�����й��������������У���(1)
	unsigned char PL_ChkPn[8];				//��ǰ�����й��������������У���(1)
	unsigned char PL_ChkQp[8];				//��ǰ�����޹��������������У���(1)
	unsigned char PL_ChkQn[8];				//��ǰ�����޹��������������У���(1)
        unsigned char PL_ChkQ[8][4];				//��ǰ4�����޹��������������У���(1)

}ECPULSE;					//180�ֽ�

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
typedef struct
{
	unsigned short PL_CumPp1;				//��ǰ�����й�����1�������������(2)
	unsigned short PL_CumPp2;				//��ǰ�����й�����2�������������(2)
	unsigned short PL_CumPp3;				//��ǰ�����й�����3�������������(2)
	unsigned short PL_CumPp4;				//��ǰ�����й�����4�������������(2)

	unsigned short PL_CumPn1;				//��ǰ�����й�����1�������������(2)
	unsigned short PL_CumPn2;				//��ǰ�����й�����2�������������(2)
	unsigned short PL_CumPn3;				//��ǰ�����й�����3�������������(2)
	unsigned short PL_CumPn4;				//��ǰ�����й�����4�������������(2)

	unsigned short PL_CumQp1;				//��ǰ�����޹�����1�������������(2)
	unsigned short PL_CumQp2;				//��ǰ�����޹�����2�������������(2)
	unsigned short PL_CumQp3;				//��ǰ�����޹�����3�������������(2)
	unsigned short PL_CumQp4;				//��ǰ�����޹�����4�������������(2)

	unsigned short PL_CumQn1;				//��ǰ�����޹�����1�������������(2)
	unsigned short PL_CumQn2;				//��ǰ�����޹�����2�������������(2)
	unsigned short PL_CumQn3;				//��ǰ�����޹�����3�������������(2)
	unsigned short PL_CumQn4;				//��ǰ�����޹�����4�������������(2)

	unsigned short PL_CumQ11;				//��ǰ1�����޹�����1�������������(2)
	unsigned short PL_CumQ12;				//��ǰ1�����޹�����2�������������(2)
	unsigned short PL_CumQ13;				//��ǰ1�����޹�����3�������������(2)
	unsigned short PL_CumQ14;				//��ǰ1�����޹�����4�������������(2)

	unsigned short PL_CumQ21;				//��ǰ2�����޹�����1�������������(2)
	unsigned short PL_CumQ22;				//��ǰ2�����޹�����2�������������(2)
	unsigned short PL_CumQ23;				//��ǰ2�����޹�����3�������������(2)
	unsigned short PL_CumQ24;				//��ǰ2�����޹�����4�������������(2)

	unsigned short PL_CumQ31;				//��ǰ3�����޹�����1�������������(2)
	unsigned short PL_CumQ32;				//��ǰ3�����޹�����2�������������(2)
	unsigned short PL_CumQ33;				//��ǰ3�����޹�����3�������������(2)
	unsigned short PL_CumQ34;				//��ǰ3�����޹�����4�������������(2)

	unsigned short PL_CumQ41;				//��ǰ4�����޹�����1�������������(2)
	unsigned short PL_CumQ42;				//��ǰ4�����޹�����2�������������(2)
	unsigned short PL_CumQ43;				//��ǰ4�����޹�����3�������������(2)
	unsigned short PL_CumQ44;				//��ǰ4�����޹�����4�������������(2)

	unsigned char PL_ChkPp1;			//��ǰ�����й�����1�������������У���(1)
	unsigned char PL_ChkPp2;			//��ǰ�����й�����2�������������У���(1)
	unsigned char PL_ChkPp3;			//��ǰ�����й�����3�������������У���(1)
	unsigned char PL_ChkPp4;			//��ǰ�����й�����4�������������У���(1)

	unsigned char PL_ChkPn1;			//��ǰ�����й�����1�������������У���(1)
	unsigned char PL_ChkPn2;			//��ǰ�����й�����2�������������У���(1)
	unsigned char PL_ChkPn3;			//��ǰ�����й�����3�������������У���(1)
	unsigned char PL_ChkPn4;			//��ǰ�����й�����4�������������У���(1)

	unsigned char PL_ChkQp1;			//��ǰ�����޹�����1�������������У���(1)
	unsigned char PL_ChkQp2;			//��ǰ�����޹�����2�������������У���(1)
	unsigned char PL_ChkQp3;			//��ǰ�����޹�����3�������������У���(1)
	unsigned char PL_ChkQp4;			//��ǰ�����޹�����4�������������У���(1)

	unsigned char PL_ChkQn1;			//��ǰ�����޹�����1�������������У���(1)
	unsigned char PL_ChkQn2;			//��ǰ�����޹�����2�������������У���(1)
	unsigned char PL_ChkQn3;			//��ǰ�����޹�����3�������������У���(1)
	unsigned char PL_ChkQn4;			//��ǰ�����޹�����4�������������У���(1)

	unsigned char PL_ChkQ11;			//��ǰ1�����޹�����1�������������У���(1)
	unsigned char PL_ChkQ12;			//��ǰ1�����޹�����2�������������У���(1)
	unsigned char PL_ChkQ13;			//��ǰ1�����޹�����3�������������У���(1)
	unsigned char PL_ChkQ14;			//��ǰ1�����޹�����4�������������У���(1)

	unsigned char PL_ChkQ21;			//��ǰ2�����޹�����1�������������У���(1)
	unsigned char PL_ChkQ22;			//��ǰ2�����޹�����2�������������У���(1)
	unsigned char PL_ChkQ23;			//��ǰ2�����޹�����3�������������У���(1)
	unsigned char PL_ChkQ24;			//��ǰ2�����޹�����4�������������У���(1)

	unsigned char PL_ChkQ31;			//��ǰ3�����޹�����1�������������У���(1)
	unsigned char PL_ChkQ32;			//��ǰ3�����޹�����2�������������У���(1)
	unsigned char PL_ChkQ33;			//��ǰ3�����޹�����3�������������У���(1)
	unsigned char PL_ChkQ34;			//��ǰ3�����޹�����4�������������У���(1)

	unsigned char PL_ChkQ41;			//��ǰ4�����޹�����1�������������У���(1)
	unsigned char PL_ChkQ42;			//��ǰ4�����޹�����2�������������У���(1)
	unsigned char PL_ChkQ43;			//��ǰ4�����޹�����3�������������У���(1)
	unsigned char PL_ChkQ44;			//��ǰ4�����޹�����4�������������У���(1)

}FeeECPULSE;					//72�ֽ�
#endif

#if ( MEASCHIP == IDT90E32 )
typedef struct
{
	unsigned short PL_RegPT;				//��ǰ�й��Ĵ������������(2)
	unsigned short PL_RegPA;				//��ǰA���й��Ĵ������������(2)
	unsigned short PL_RegPB;				//��ǰB���й��Ĵ������������(2)
	unsigned short PL_RegPC;				//��ǰC���й��Ĵ������������(2)
//	unsigned short PL_RegPn;				//��ǰ�����й��Ĵ������������(2)
//	unsigned short PL_RegPnA;				//��ǰA�෴���й��Ĵ������������(2)
//	unsigned short PL_RegPnB;				//��ǰB�෴���й��Ĵ������������(2)
//	unsigned short PL_RegPnC;				//��ǰC�෴���й��Ĵ������������(2)

	unsigned short PL_RegQT;				//��ǰ�޹��Ĵ������������(2)
	unsigned short PL_RegQA;				//��ǰA���޹��Ĵ������������(2)
	unsigned short PL_RegQB;				//��ǰB���޹��Ĵ������������(2)
	unsigned short PL_RegQC;				//��ǰC���޹��Ĵ������������(2)
//	unsigned short PL_RegQn;				//��ǰ�����޹��Ĵ������������(2)
//	unsigned short PL_RegQnA;				//��ǰA�෴���޹��Ĵ������������(2)
//	unsigned short PL_RegQnB;				//��ǰB�෴���޹��Ĵ������������(2)
//	unsigned short PL_RegQnC;				//��ǰC�෴���޹��Ĵ������������(2)

	unsigned short PL_RegST;				//��ǰ���ڼĴ������������(2)
	unsigned short PL_RegSA;				//��ǰA�����ڼĴ������������(2)
	unsigned short PL_RegSB;				//��ǰB�����ڼĴ������������(2)
	unsigned short PL_RegSC;				//��ǰC�����ڼĴ������������(2)

	unsigned char PL_SparePp;			//��ǰ�����й�0.1CF����ʣ�����������(2)
	unsigned char PL_SparePn;			//��ǰ�����й�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ1;			//��ǰ1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ2;			//��ǰ2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ3;			//��ǰ3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ4;			//��ǰ4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSp;			//��ǰ��������0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSn;			//��ǰ��������0.1CF����ʣ�����������(2)

	unsigned char PL_SparePpA;			//��ǰA�������й�0.1CF����ʣ�����������(2)
	unsigned char PL_SparePnA;			//��ǰA�෴���й�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ1A;			//��ǰA��1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ2A;			//��ǰA��2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ3A;			//��ǰA��3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ4A;			//��ǰA��4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSpA;			//��ǰA����������0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSnA;			//��ǰA�෴������0.1CF����ʣ�����������(2)

	unsigned char PL_SparePpB;			//��ǰB�������й�0.1CF����ʣ�����������(2)
	unsigned char PL_SparePnB;			//��ǰB�෴���й�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ1B;			//��ǰB��1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ2B;			//��ǰB��2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ3B;			//��ǰB��3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ4B;			//��ǰB��4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSpB;			//��ǰB����������0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSnB;			//��ǰB�෴������0.1CF����ʣ�����������(2)

	unsigned char PL_SparePpC;			//��ǰC�������й�0.1CF����ʣ�����������(2)
	unsigned char PL_SparePnC;			//��ǰC�෴���й�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ1C;			//��ǰC��1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ2C;			//��ǰC��2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ3C;			//��ǰC��3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareQ4C;			//��ǰC��4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSpC;			//��ǰC����������0.1CF����ʣ�����������(2)
	unsigned char PL_SpareSnC;			//��ǰC�෴������0.1CF����ʣ�����������(2)

	unsigned char PL_ChkSparePp;		//��ǰ�����й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSparePn;		//��ǰ�����й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ1;		//��ǰ1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ2;		//��ǰ2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ3;		//��ǰ3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ4;		//��ǰ4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSp;		//��ǰ��������0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSn;		//��ǰ��������0.1CF����ʣ�����������(2)

	unsigned char PL_ChkSparePpA;		//��ǰA�������й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSparePnA;		//��ǰA�෴���й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ1A;		//��ǰA��1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ2A;		//��ǰA��2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ3A;		//��ǰA��3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ4A;		//��ǰA��4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSpA;		//��ǰA����������0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSnA;		//��ǰA�෴������0.1CF����ʣ�����������(2)
                        
	unsigned char PL_ChkSparePpB;		//��ǰB�������й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSparePnB;		//��ǰB�෴���й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ1B;		//��ǰB��1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ2B;		//��ǰB��2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ3B;		//��ǰB��3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ4B;		//��ǰB��4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSpB;		//��ǰB����������0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSnB;		//��ǰB�෴������0.1CF����ʣ�����������(2)
                        
	unsigned char PL_ChkSparePpC;		//��ǰC�������й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSparePnC;		//��ǰC�෴���й�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ1C;		//��ǰC��1�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ2C;		//��ǰC��2�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ3C;		//��ǰC��3�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareQ4C;		//��ǰC��4�����޹�0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSpC;		//��ǰC����������0.1CF����ʣ�����������(2)
	unsigned char PL_ChkSpareSnC;		//��ǰC�෴������0.1CF����ʣ�����������(2)

}REGPULSE;
#endif

typedef struct
{


	unsigned long SMEcSumA;				//0.01��A�����ڵ����ۼƻ���
	unsigned long SMEcSumB;				//0.01��B�����ڵ����ۼƻ���
	unsigned long SMEcSumC;				//0.01��C�����ڵ����ۼƻ���
	unsigned long SMEcSum;				//0.01�������ڵ����ۼƻ���
	
	unsigned char YPulse_Mode[8];			//�й���������λ�Ĵ���
	unsigned char WPulse_Mode[8];			//�޹���������λ�Ĵ���

	unsigned char YPulse_Cum[8];			//�ж��й����������
	unsigned char WPulse_Cum[8];			//�ж��޹����������
	
	unsigned char SMEcSumAChk;			//0.01��A�����ڵ����ۼƻ���
	unsigned char SMEcSumBChk;			//0.01��B�����ڵ����ۼƻ���
	unsigned char SMEcSumCChk;			//0.01��C�����ڵ����ۼƻ���
	unsigned char SMEcSumChk;			//0.01�������ڵ����ۼƻ���
	
	unsigned char YPulse_CumChk[8];		//�ж��й����������
	unsigned char WPulse_CumChk[8];		//�ж��޹����������
	
}INTPULSE;					

#if ( MEASCHIP == ADE7878 )	
typedef struct
{
	unsigned long* PhaseECSum;			//ADE7878������ۼӻ�����
	unsigned char* PhaseECSumChk;		//ADE7878������ۼӻ�����У���
}PHASE_ECBUF;	
#endif
/*
typedef struct
{
	unsigned char Cunt;		//�¼��ȶ�ʱ��
	unsigned char State;	//�¼�ǰ1��״̬			//33��δ����	55���¼�����
	unsigned char OldState;	//�¼���ǰ״̬
	unsigned char Sec;		//�¼�ʱ��
	
}EVENT_STATE;				//�¼�״̬����
	
typedef struct
{
	EVENT_STATE PhFailVar;			//�����ʧѹ״̬�ֽṹ    
	EVENT_STATE PhAFailVar;			//���A��ʧѹ״̬�ֽṹ    
	EVENT_STATE PhBFailVar;			//���B��ʧѹ״̬�ֽṹ 
	EVENT_STATE PhCFailVar;			//���C��ʧѹ״̬�ֽṹ 

	EVENT_STATE PhLowerVar;			//�����Ƿѹ״̬�ֽṹ 
	EVENT_STATE PhALowerVar;		//���A��Ƿѹ״̬�ֽṹ 
	EVENT_STATE PhBLowerVar;		//���B��Ƿѹ״̬�ֽṹ 
	EVENT_STATE PhCLowerVar;		//���C��Ƿѹ״̬�ֽṹ 

	EVENT_STATE PhOverVar;			//����ܹ�ѹ״̬�ֽṹ 
	EVENT_STATE PhAOverVar;			//���A���ѹ״̬�ֽṹ 
	EVENT_STATE PhBOverVar;			//���B���ѹ״̬�ֽṹ 
	EVENT_STATE PhCOverVar;			//���C���ѹ״̬�ֽṹ 

	EVENT_STATE PhBreakVar;			//����ܶ���״̬�ֽṹ 
	EVENT_STATE PhABreakVar;		//���A�����״̬�ֽṹ 
	EVENT_STATE PhBBreakVar;		//���B�����״̬�ֽṹ 
	EVENT_STATE PhCBreakVar;		//���C�����״̬�ֽṹ 

	EVENT_STATE VoltRevPhaseVar;	//����ѹ������״̬�ֽṹ 
	EVENT_STATE CuRevPhaseVar;		//����ѹ������״̬�ֽṹ 

	EVENT_STATE VoltImbalanceVar;	//����ѹ��ƽ��״̬�ֽṹ 
	EVENT_STATE CuImbalanceVar;		//��������ƽ��״̬�ֽṹ 
	EVENT_STATE CuBestImbalanceVar;	//���������ز�ƽ��״̬�ֽṹ 

	EVENT_STATE CuFailVar;			//�����ʧ��״̬�ֽṹ 
	EVENT_STATE CuAFailVar;			//���A��ʧ��״̬�ֽṹ 
	EVENT_STATE CuBFailVar;			//���B��ʧ��״̬�ֽṹ 
	EVENT_STATE CuCFailVar;			//���C��ʧ��״̬�ֽṹ 

	EVENT_STATE CuOverVar;			//����ܹ���״̬�ֽṹ 
	EVENT_STATE CuAOverVar;			//���A�����״̬�ֽṹ 
	EVENT_STATE CuBOverVar;			//���B�����״̬�ֽṹ 
	EVENT_STATE CuCOverVar;			//���C�����״̬�ֽṹ 

	EVENT_STATE CuBreakVar;			//����ܶ���״̬�ֽṹ 
	EVENT_STATE CuABreakVar;		//���A�����״̬�ֽṹ 
	EVENT_STATE CuBBreakVar;		//���B�����״̬�ֽṹ 
	EVENT_STATE CuCBreakVar;		//���C�����״̬�ֽṹ 

	EVENT_STATE RevPowerVar;		//����ܳ���״̬�ֽṹ 
	EVENT_STATE RevPowerAVar;		//���A�ೱ��״̬�ֽṹ 
	EVENT_STATE RevPowerBVar;		//���B�ೱ��״̬�ֽṹ 
	EVENT_STATE RevPowerCVar;		//���C�ೱ��״̬�ֽṹ 
	
	EVENT_STATE PowerOverVar;		//����ܹ���״̬�ֽṹ 
	EVENT_STATE PowerOverAVar;		//���A�����״̬�ֽṹ 
	EVENT_STATE PowerOverBVar;		//���B�����״̬�ֽṹ 
	EVENT_STATE PowerOverCVar;		//���C�����״̬�ֽṹ 

	EVENT_STATE PftOverVar;			//��������س���״̬�ֽṹ 
	
	EVENT_STATE TideSwitchVar;		//�����״̬�ֽṹ 				//�¹���

//	EVENT_STATE PftAOverVar;		//���A�๦�����س���״̬�ֽṹ 	//�¹���
//	EVENT_STATE PftBOverVar;		//���B�๦�����س���״̬�ֽṹ 	//�¹���
//	EVENT_STATE PftCOverVar;		//���C�๦�����س���״̬�ֽṹ 	//�¹���

	EVENT_STATE MagnetDisturbVar;	//�㶨�ų�����״̬�ֽṹ 			//�¹���
	EVENT_STATE RelayFaultVar;		//���ɿ�������״̬�ֽṹ 			//�¹���

	EVENT_STATE RevPowerZVar;		//����ܳ���״̬�ֽṹ 
	EVENT_STATE AllFailVar;			//ȫʧѹ״̬�ֽṹ 
	EVENT_STATE PdownVar;			//����״̬�ֽṹ 

	EVENT_STATE MDOverPpVar;		//�����й���������״̬		��		//�¹���	//13.10.23 
	EVENT_STATE MDOverPnVar;		//�����й���������״̬		��		//�¹���	//13.10.23 
	EVENT_STATE MDOverQ1Var;		//��һ�����޹���������״̬		��	//�¹���	//13.10.23 
	EVENT_STATE MDOverQ2Var;		//�ڶ������޹���������״̬		��	//�¹���	//13.10.23 
	EVENT_STATE MDOverQ3Var;		//���������޹���������״̬		��	//�¹���	//13.10.23 
	EVENT_STATE MDOverQ4Var;		//���������޹���������״̬		��	//�¹���	//13.10.23 

	EVENT_STATE reserveVar1;		//����״̬�ֽṹ 					//�¹���	//13.10.23 
	EVENT_STATE reserveVar2;		//����״̬�ֽṹ 					//�¹���	//13.10.23 	

//																		//�¹���	//13.10.23 		
//	unsigned char 	MD_PpOverState;	//�����й���������״̬		��
//	unsigned char 	MD_PnOverState;	//�����й���������״̬		��      ��
//	unsigned char 	MD_Q1OverState;	//��һ�����޹���������״̬		��  ��
//	unsigned char 	MD_Q2OverState;	//�ڶ������޹���������״̬		��  ��
//	unsigned char 	MD_Q3OverState;	//���������޹���������״̬		��  ��
//	unsigned char 	MD_Q4OverState;	//���������޹���������״̬		��  ��
	
	unsigned char	AuxPWFail_State;//������Դ״̬
	unsigned char	reserve;		//����		
	
}EVENTVAR;							//93�ֽ�			

typedef struct
{
	unsigned long EventAmpHour;			//��ѹ�¼��ڼ����ۼӰ�ʱ�� = L1 xxx.xxxA * 1�� + L2 xxx.xxxA * 1�� + L3 xxx.xxxA * 1��
	unsigned long EventAmpHourA;        //��ѹ�¼��ڼ�A���ۼӰ�ʱ�� = L1 xxx.xxxA * 1��                                       
	unsigned long EventAmpHourB;        //��ѹ�¼��ڼ�B���ۼӰ�ʱ�� = L2 xxx.xxxA * 1��                                       
	unsigned long EventAmpHourC;        //��ѹ�¼��ڼ�C���ۼӰ�ʱ�� = L3 xxx.xxxA * 1��                                       
	
	unsigned char EventAmpHourChk;		//��ѹ�¼��ڼ����ۼӰ�ʱ��У���	
	unsigned char EventAmpHourChkA;		//��ѹ�¼��ڼ�A���ۼӰ�ʱ��У���	
	unsigned char EventAmpHourChkB;		//��ѹ�¼��ڼ�B���ۼӰ�ʱ��У���	
	unsigned char EventAmpHourChkC;		//��ѹ�¼��ڼ�C���ۼӰ�ʱ��У���	

}EVEAHOUR;

typedef struct
{
	unsigned short	EventNo;
	EVEAHOUR*		EveAHour;
	unsigned short 	AmpHourEAddr;		//�¼��а�ʱ����EEPROM�е���ַ
}EVENTAMPERHOUR;

typedef struct
{
	EVEAHOUR	PhFailAmpHour;			//��ʧѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhAFailAmpHour;			//A��ʧѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhBFailAmpHour;			//B��ʧѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhCFailAmpHour;			//C��ʧѹ�ڼ��ܼ������ۼӰ�ʱ��

	EVEAHOUR	PhLowerAmpHour;			//��Ƿѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhALowerAmpHour;		//A��Ƿѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhBLowerAmpHour;		//B��Ƿѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhCLowerAmpHour;		//C��Ƿѹ�ڼ��ܼ������ۼӰ�ʱ��
	
	EVEAHOUR	PhOverAmpHour;			//�ܹ�ѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhAOverAmpHour;			//A���ѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhBOverAmpHour;			//B���ѹ�ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhCOverAmpHour;			//C���ѹ�ڼ��ܼ������ۼӰ�ʱ��
	
	EVEAHOUR	PhBreakAmpHour;			//�ܶ����ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhABreakAmpHour;		//A������ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhBBreakAmpHour;		//B������ڼ��ܼ������ۼӰ�ʱ��
	EVEAHOUR	PhCBreakAmpHour;		//C������ڼ��ܼ������ۼӰ�ʱ��

}AMPERHOUR;

*/

typedef struct
{
  unsigned int HDLC_ADDR;
  unsigned int PW;
  BCD3	RAT7022ChkSum[MAX_CH_NUM];			//7022У������У���	//ATChk
  BCD3	RAT7022ChkSum2[MAX_CH_NUM];			//7022EУ������У���2
} PARA;

typedef struct
{
	unsigned long* AD_CS_PTDIR;
	unsigned long* AD_CS_PTSET;
	unsigned long* AD_CS_PTCLR;
	unsigned long* AD_RST_PTDIR;
	unsigned long* AD_RST_PTSET;
	unsigned long* AD_RST_PTCLR;
	unsigned long AD_CS;
	unsigned long AD_RST;
}SPIPARA;

typedef struct
{
#if 0  
  unsigned long	Pt; 					//�����й�����(4)
  unsigned long	Pa; 					//A���й�����(4)	
  unsigned long	Pb; 					//B���й�����(4)	
  unsigned long	Pc; 					//C���й�����(4)	

  unsigned long	Qt; 					//�����޹�����(4)
  unsigned long	Qa; 					//A���޹�����(4)	
  unsigned long	Qb; 					//B���޹�����(4)	
  unsigned long	Qc; 					//C���޹�����(4)	
  	
  unsigned long	St; 					//�������ڹ���(4)
  unsigned long	Sa; 					//A�����ڹ���(4)	
  unsigned long	Sb; 					//B�����ڹ���(4)	
  unsigned long	Sc; 					//C�����ڹ���(4)	
  
  unsigned long        Ia; 			//A�����(4)		XXXX.XXXX A
  unsigned long	Ib; 			//B�����(4)		XXXX.XXXX A
  unsigned long	Ic; 			//C�����(4)		XXXX.XXXX A
  
  unsigned long        Ua; 			//A���ѹ(4)
  unsigned long	Ub; 			//B���ѹ(4)
  unsigned long	Uc; 			//C���ѹ(4)
  
  unsigned long	AFreq;			//A��Ƶ��(4)
  unsigned long	BFreq;			//B��Ƶ��(4)
  unsigned long	CFreq;			//C��Ƶ��(4)
  
  unsigned long	Pft;			//���๦������(4)
  unsigned long	Pfa;			//A�๦������(4)
  unsigned long	Pfb;			//B�๦������(4)
  unsigned long	Pfc;			//C�๦������(4)
  
  unsigned long        Angle_Ia; 			//A������Ƕ�
  unsigned long        Angle_Ib; 			//B������Ƕ�
  unsigned long        Angle_Ic; 			//C������Ƕ�
  
  unsigned long        Angle_Ua; 			//A������Ƕ�
  unsigned long        Angle_Ub; 			//B������Ƕ�
  unsigned long        Angle_Uc; 			//C������Ƕ�
#else
  unsigned long	AFreq;			//A��Ƶ��(4)
  
  unsigned long        Ia; 			//A�����(4)		XXXX.XXXX A
  unsigned long	Ib; 			//B�����(4)		XXXX.XXXX A
  unsigned long	Ic; 			//C�����(4)		XXXX.XXXX A
 
  unsigned long        Ua; 			//A���ѹ(4)
  unsigned long	Ub; 			//B���ѹ(4)
  unsigned long	Uc; 			//C���ѹ(4)
  
  signed long	        Pa; 					//A���й�����(4)	
  signed long	        Pb; 					//B���й�����(4)	
  signed long	        Pc; 					//C���й�����(4)	
  signed long	        Pt; 					//�����й�����(4)
  
  
  signed long	        Qa; 					//A���޹�����(4)	
  signed long	        Qb; 					//B���޹�����(4)	
  signed long	        Qc; 					//C���޹�����(4)	
  signed long	        Qt; 					//�����޹�����(4)
  
  
  unsigned long	Sa; 					//A�����ڹ���(4)	
  unsigned long	Sb; 					//B�����ڹ���(4)	
  unsigned long	Sc; 					//C�����ڹ���(4)	
  unsigned long	St; 					//�������ڹ���(4)
  
  
  unsigned long	Pfa;			//A�๦������(4)
  unsigned long	Pfb;			//B�๦������(4)
  unsigned long	Pfc;			//C�๦������(4)
  unsigned long	Pft;			//���๦������(4)
  
  //unsigned long	BFreq;			//B��Ƶ��(4)
 // unsigned long	CFreq;			//C��Ƶ��(4)
#endif  
}REAL;	

typedef struct
{
  unsigned long	Pp; 					//�����й�
  unsigned long	Qp; 					//�����޹�	
  unsigned long	Q1; 					//1����
  unsigned long	Q4; 					//4����
  unsigned long	Pn; 					//�����й�
  unsigned long	Qn; 					//�����޹�
  unsigned long	Q2; 					//2����
  unsigned long	Q3; 					//3����
  signed   long	Pa; 					//A���й�����(4)	
  signed   long	Pb; 					//B���й�����(4)	
  signed   long	Pc; 					//C���й�����(4)	
  signed   long	Pt; 					//�����й�����(4)
}ENERGY;

//	Clock Timer Variable
typedef struct
{
//	long	AdjCT;
//	long 	Adj_Fact;
	short		Temp;
//	char	RTCOffset;

	char		Sec_64;
   	BCD1 		Sec;
   	BCD1 		Min;
   	BCD1 		Hour;
   	BCD1 		Day;
   	BCD1 		Mon;
   	BCD1 		Year;
   	BCD1		Week;

	unsigned char YearH;		//HEXʱ��	
	unsigned char YearL;
	unsigned char Month;
	unsigned char DayH;
	
	unsigned char WeekH;
	unsigned char HourH;
	unsigned char MinH;
	unsigned char SecH;
	unsigned char SecPer;
	unsigned char DeviatH;
	unsigned char DeviatL;
	unsigned char State;
	
//	unsigned char	Check;
	unsigned char daylightsaving;		//����ʱ״̬��־(1:��ʼ����;2:����;0:��)..

} CLK;
	
typedef struct
{
	unsigned char Type;			//��ʾ������ 0��OAD;1��ROAD
	unsigned char OAD[4];		//����ʾ������
	unsigned char ROAD[4];		//����ʾ������
	unsigned char No;			//�����
}CURDISOBJ;
	
// Display  & Key registers
typedef struct
{
	enum {
		AutoMode,				// ����ģʽ
		KeyMode,				// ����ģʽ
		LookMode,				// Һ���鿴ģʽ
	}TouchDisMode;				// ������ʾģʽ

	enum {
		FaultType,				// ������
		EventType,				// �¼���
		AutoType,				// ������
		KeyType,				// ������
	}ContentDisType;			// ������ʾģʽ

	unsigned short FaultDisFlag;	//������ʾ��־λ
	unsigned short EventDisFlag;	//�¼���ʾ��־λ

   unsigned short HWBatOnTime;			//���ⴥ��ͣ������ʱ��
   unsigned short HWBGDisTime;			//���ⴥ����������ʱ��
	short ManuCode;				//���ұ���
	BCD2 UserCode;				//�û�����	
	BCD2 OldUserCode;				//�û�����					//V30
//	unsigned char TouchDisMode;			//������ʾģʽ 0������ 1������ 2��Һ���鿴
//	unsigned char ContentDisMode;		//������ʾģʽ 0���������쳣 1���¼����쳣 2�������� 3��������

	unsigned char FullDisDelay;			// �ϵ���ʾ20��ȫ��
    signed char FaultItemPtr;			// �������ָ��
    signed char EventItemPtr;			// �¼����ָ��
    unsigned char FlautItemSum;			// ��������ʾ������
    unsigned char EventItemSum;			// �¼�����ʾ������

//	unsigned char Mode;					// ģʽ��
//	unsigned char KeyTmr;				// ����ɨ�趨ʱ
//	unsigned char KeyLPCT;				// ��������ʱ������
    unsigned char FlsCT;				// ��˸������
    unsigned char DisInv;				// ���Լ��������
    signed char KeyItemPtr;				// ���Ա�ָ��
    signed char AutoItemPtr;			// ���Ա�ָ��
    unsigned char KeyItemSum;			// ��ʾ������
    unsigned char AutoItemSum;			// ����������
//    unsigned char ItemSum;			// ��ʾ������
//    unsigned char LXItemSum;			// ����������
	unsigned char DisFeeNo;
    unsigned char LCDDisEnable;			//LCD��ʾʹ��
   
    CURDISOBJ CurDisObj;				//��ǰ��ʾ������
    
	unsigned char CurDisID[7];			// ��ǰ��ʾ��ID�롢���ڲ���š���ʾ�û���
//    unsigned char LXTab[16];			// LCD��λ����
    unsigned char LCDSeg[32];			// LCD��λ����
//    unsigned char FLCDSeg[32];			// LCD��λ����

	unsigned char DayFreezeDelay;		//�޸�ʱ�����ʱ�жϲ��ն���ʱ��	//�¹���
	BCD5 BeforeDayFreezeTime;			//���ն���ǰʱ��					//�¹���	

//#if( ECDisDecDigitsValid ==	YesCheck )			//13.03.03 ��ǰ������ʾС��λ��ʵ��Ч
	unsigned char DigitsValidFlg;		//��ǰ������ʾС��λ��ʵ��Ч��ǣ���ʾǰ��λ0x55����ʾ������
//	BCD7	DisReserve;					//����8�ֽڿռ�		
	unsigned char 	RHighZeroFlag;	//��λ����ʾ��־(2)	0����λ��ʾ0����������λ����ʾ0��	//�¹���	//14.01.13
	BCD6	DisReserve;					//����8�ֽڿռ�										//�¹���	//14.01.13	
//#else
//	BCD8	DisReserve;					//����8�ֽڿռ�		
//	unsigned char 	RHighZeroFlag;	//��λ����ʾ��־(2)	0����λ��ʾ0����������λ����ʾ0��	//�¹���	//14.01.13
//	BCD7	DisReserve;					//����8�ֽڿռ�										//�¹���	//14.01.13	
//#endif
	unsigned char Mode;					// ģʽ��

	unsigned char Mode1ItemPtr; 		// ģʽ1���Ա�ָ��
	unsigned char Mode2ItemPtr; 		// ģʽ2���Ա�ָ��
	unsigned char Mode3ItemPtr; 		// ģʽ3���Ա�ָ��
	unsigned short Class_Id;				//��ǰ��ʾ�����
	unsigned char Logical_Name[6];		//��ǰ��ʾ���OBIS��
	unsigned char Attribute;			//��ǰ��ʾ���ڸ����е�����	
	unsigned short  Data_Index;	  //��ʾˢ��..
//	unsigned char LCDSeg[24];			// LCD��λ����
     unsigned short ItemPtr;				// ���Ա�ָ��   
//    unsigned char ItemSum;			// ��ʾ������
    unsigned char ItemNo;				// ��ʾ�����

} DISKEY;


typedef struct
{
    unsigned short E2_PTR;
    unsigned short E2_ADDR;
}EVTPARALIST;
/*
typedef struct
{
	BCD4	STime;
	BCD4	ETime;
	BCD4	CTime;		//���ڳ��ո��ɱ�
	unsigned char Mode;		//0:�������ɡ�1:���ո��ɱ�2:���Ե��¼
} DLOAD;	

typedef struct
{
	BCD3	STime;
	BCD3	ETime;
	unsigned char Mode;		//0:������1:�����ڳ��ա�2:ȫ��
	short		Ptr;			//��ǰ��¼ָ��
} TRECORD;	
*/
/*******************************************************************
	Even Log Profile Information Structure Define
*******************************************************************/
// Bit Flag
	// Power flags define
#define  F_PwrUp     	0x02
#define  F_IrmsCheck	0x01		//0:�������1������У׼
#define  F_AllFail		0x04		//0:��ȫʧѹ;1:ȫʧѹ
#define  F_ByPass 		0x40		//������·���� 
//#define  F_ErrMeas		0x08		//����оƬ����
//#define  F_FailIrmsMeas	0x10		//У�����һ�ε������ 0���Ѽ�����1��δ���
//#define	 F_NCheckLoss	0x20		//���߼�⡣0��������1��������

#define  F_IbIrmsCheck	0x08		//0:�������1��ȫʧѹ100%Ib����У׼
#define  F_Ib10IrmsCheck	0x80	//0:�������1��ȫʧѹ10%Ib����У׼
#define  F_Ib00IrmsCheck	0x10	//0:�������1��ȫʧѹ0%Ib����У׼


	// Run flags define
#define  F_ComOK1   0x01
//#define	 F_THW		0x02	//�����485����
#define  F_ComOK3   0x02
#define  F_Batt     0x04
//#define	 F_T485		0x08	//�����485����
#define  F_Hold		0x08		//16.10.20
#define	 F_AdsOk	0x10
#define	 F_MimaOk	0x20
#define  F_ComOK2   0x40
#define  F_Parity   0x80

	// Clk flags define
#define  F_Sec       0x01
#define  F_Min       0x02
#define  F_Hour      0x04
#define  F_Day       0x08

#define  F_DisSec    0x10
#define  F_HalfSec   0x20		//V1001		//PWMD
#define  F_ThirdSec  0x40		//PWMD		//����֮һ�롢���ڼ�������

	// DisK Flag Bit
#define  F_KeyInt    	0x80
#define  F_DspALT		0x40
//#define	 F_HWKeyInt    	0x20	//V30
#define  F_KeyStart    	0x20

#define  F_Flash		0x08
//#define  F_KeyS	    	0x04		//�¹���	//13.10.17	������Һ������оƬ�״��ϵ���
#define  F_KeyS	    	0x04		//����

#define  F_Key       	0x03
#define  F_Key1      	0x01		//V30 �����ã���Ϊʵʱ�����Ƿ���ʾ�ı�� ��1 ��ʾ����0����ʾ�����µ���ԲȦ1ָʾ
#define  F_Key2      	0x02

	// E.C. Direction
//#define  F_Ph     0x70
//#define  F_PhA    0x10
//#define  F_PhB    0x20
//#define  F_PhC    0x40

//#define  F_Ph     0x10	
//#define  F_PhA    0x01
//#define  F_PhB    0x02
//#define  F_PhC    0x04

//#define  F_Dir    0x03
//#define  F_PDir	0x02
//#define  F_QDir	0x01

//#define  F_TamTmr    0xC0

	// TamFlg Register flags define
#define  F_TamStop   0x01
#define  F_TamDrop   0x02
#define  F_TamResv   0x04
#define  F_TamPole   0x08
#define  F_TamPass   0x10
#define  F_TamUout   0x20
#define  F_TamLoad   0x40
#define  F_TamSequ   0x80

	// ErrFlg Register flags define
	
#define  F_AlarmCoverOpen	0x01		//���Ǳ���//#define  F_AlarmPhFail	0x01		//���౨��
#define  F_AlarmTCoverOpen	0x02		//���Ǳ���//#define  F_AlarmAllCuFail	0x02		//ȫ��������
#define  F_AlarmMagnet	0x04		//�ų�����//#define  F_AlarmMeas	0x04		//����оƬ����
#define  F_AlarmCRCFail	0x08		//CRCУ�����
#define  F_AlarmPhs 	0x10		//�����򱨾� 
#define  F_AlarmReverse	0x20		//���ʷ��򱨾�
#define  F_AlarmClock	0x40		// 32768Hz Crystal Error
#define  F_AlarmE2P   	0x80

//#define  F_ErrMeas		0x01		//����оƬ����
#define  F_ErrBatLow   	0x02
#define  F_ErrDir   	0x04
#define  F_ErrPhs 		0x08		// 
#define  F_ErrBatOver	0x10		// 
#define  F_WatchDog		0x20
//#define  F_ErrClock		0x40		// 32768Hz Crystal Error
//#define  F_ErrE2P   	0x80

	// AlarmFlg[0] Register flags define
#define  F_AlarmPhFail	0x01		//ʧѹ����
#define  F_AlarmCuFail	0x02		//ʧ������
#define  F_AlarmMeas	0x04		//����оƬ����
#define  F_AlarmBatLow  0x08		//��ص͵�ѹ����
#define  F_AlarmPhs 	0x10		//�����򱨾� 
//#define  F_AlarmNLine 	0x20	//�����߱��� 
#define  F_AlarmRevPower 0x20		//���ʷ��򱨾� 
#define  F_AlarmClock	0x40		//DS3231
//#define  F_AlarmByPass 	0x40	//������·���� 
#define  F_AlarmE2P   	0x80

	// AlarmFlg[1] Register flags define
#define  F_AlarmPhBreak	0x01		//���౨��
#define  F_AlarmPhLower	0x02		//Ƿѹ����
#define  F_AlarmPhOver	0x04		//��ѹ����
#define  F_AlarmCuBreak 0x08		//��������
#define  F_AlarmCuOver 	0x10		//�������� 
#define  F_AlarmCuPhs 	0x20		//���������򱨾�	//10.06.02 
#define  F_AlarmMDOver	0x40		//�������ޱ���
#define  F_AlarmCover  	0x80		//����Ǳ���

	// AlarmFlg[2] Register flags define
#define  F_AlarmFlash		0x01	//FLASH����
#define  F_AlarmXT1			0x02	//XT1���񱨾�		//V4
#define  F_AlarmCuBestIm	0x04	//�������ز�ƽ��
#define  F_AlarmCuImbalance	0x08	//������ƽ��
#define  F_AlarmPWOver		0x10	//����
#define  F_AlarmCommBatLow	0x20	//ͣ����ص�ѹ��
#define  F_AlarmTerminal  	0x40	//����ť�Ǳ���
#define  F_AlarmVoltImbalance	0x80//��ѹ��ƽ��

	// AlarmFlg[3] Register flags define
#define  F_AlarmRemoteTrip	0x80	//Զ����բ
#define  F_AlarmRemote		0x40	//Զ�̱���	
#define  F_AlarmBuyEC		0x20	//ʣ����С�ڱ������1
#define  F_AlarmRelayOFF	0x10	//�̵����Ͽ�

#define  F_AlarmPftOver		0x01	//�������س�����
#define  F_AlarmFRAM	   	0x08	//�������

	// AlarmFlg[4] Register flags define
#define  F_AlarmRelayErr	0x01	//���ƻ�·����		//ɽ��	//�¹���	//13.10.08
#define  F_AlarmESAMErr		0x02	//ESAM����			//ɽ��	//�¹���	//13.10.08
#define  F_AlarmAllFail		0x10	//ȫʧѹ����		//�Ϻ�	//�¹���	//13.09.21
#define  F_AlarmPhRevPower 	0x20	//���๦�ʷ��򱨾�  //����  //�¹���	//13.09.21

	// AlarmFlg[5] Register flags define
#define  F_AlarmEEPROM   	0x01	//EEPROM����	 	//15.01.30

	// AlarmFlg[6] Register flags define
	// AlarmFlg[7] Register flags define

	// FirstFlg Register flags define
//#define  F_PhAFail	0x01		//��һ��A�����δ��¼
//#define  F_PhBFail	0x02		//��һ��B�����δ��¼
//#define  F_PhCFail	0x04		//��һ��C�����δ��¼
//#define  F_CuAFail	0x08		//��һ��A�����δ��¼
//#define  F_CuBFail	0x10		//��һ��A�����δ��¼
//#define  F_CuCFail	0x20		//��һ��A�����δ��¼
//#define  F_PhFail	0x40		//��һ�ζ���δ��¼			//V1000

/*
#define  F_ErrRAM    0x01
#define  F_ErrMeas   0x02		//����оƬ����
#define  F_ErrE2P    0x04
#define  F_ErrDFls   0x08
#define  F_ErrCry1 	0x10		// 32768Hz Crystal Error
#define  F_ErrCry2 	0x20		// 8MHz Crystal Error
#define  F_WatchDog	0x40
#define  F_ErrANY    0x80
*/
//LoadFlag Register flags define   
#define  F_LoadCom	0x01		//ͨѶģʽ3����һ�ηֿ����ݷ���Ϊ�ո�������
#define  F_TampCom	0x02  		//ͨѶģʽ3����һ�ηֿ����ݷ���Ϊ�Ե��¼����
   
//BatState Register flags define   
//#define  F_ClockBat	0x01		//ʱ�ӵ��״̬  0:Ƿѹ��1������ 
//#define  F_CommBat	0x02		//ͣ�����״̬   
#define	 F_E2PCheck	0x04		//������E2PROM�Ƿ���ȷ��0����ȷ��1������У��;���	//EC	

/*   
#if ( CD4051 == YesCheck )
//4051�������״̬   
#define		P_BATCH		0x01	//V2 0:��AD����״̬��1��AD����״̬��
#define		P_BATCHB	0x02	//V2
#define		P_LOWPOW	0x04	//V2
#define		P_PRGRM		0x08	//V2
#define		P_KEYSF		0x10	//V2
#define		P_KEYXF		0x20	//V2
#define		P_SETEN 	0x40	//V2
#define		P_VDD 		0x80	//V2
//4067�������״̬   
#elif ( CD4067 == YesCheck )
#define		P_VASY		0x0001	//A���ѹ 	 
#define		P_VBSY		0x0002	//B���ѹ 	 
#define		P_VCSY		0x0004	//C���ѹ 	 
#define		P_IAAD		0x0008	//A����� 	 
#define		P_IBAD		0x0010	//B����� 	 
#define		P_ICAD		0x0020	//C����� 	 
#define		P_BATCH		0x0040	//V2 0:��AD����״̬��1��AD����״̬��
#define		P_BATCHB	0x0080	//V2
#define		P_KEYD		0x0100	//V2
#define		P_KEYG		0x0200	//V2
#define		P_LOWPOW	0x0400	//V2
#define		P_PRGRM		0x0800	//V2
#define		P_KEYSF		0x1000	//V2
#define		P_KEYXF		0x2000	//V2
#define		P_SETEN 	0x4000	//V2
#define		P_VDD 		0x8000	//V2
#else
#endif   
*/

//MeterState1 Register flags define   
#define 	F_MDJSMode	0x0002		//�������㷽ʽ	0�����1������	   
#define 	F_ClockBat	0x0004		//ʱ�ӵ��		0��������1��Ƿѹ	   
#define 	F_CommBat	0x0008		//ͣ�����		0��������1��Ƿѹ	   
#define 	F_PDir		0x0010		//�й����ʷ���	0������1������	   
#define 	F_QDir		0x0020		//�޹����ʷ���	0������1������	   

#define 	F_ESAMErr	0x0200		//ESAM״̬		0��������1������

#define 	F_RelayErr		0x0100		//���ƻ�·����	0��������1������		//�¹���
#define 	F_ProgramErr	0x1000		//�ڲ��������	0��������1������		//�¹���
#define 	F_MemErr		0x2000		//�洢������	0��������1������		//�¹���
#define 	F_Overdraft		0x4000		//͸֧״̬		0��������1������		//�¹���
#define 	F_ClkErr		0x8000		//ʱ�ӹ���		0��������1������		//�¹���

   
//MeterState2 Register flags define   
#define 	F_PDirA		0x0001		//A���й����ʷ���	0������1������	   
#define 	F_PDirB		0x0002		//B���й����ʷ���	0������1������	   
#define 	F_PDirC		0x0004		//C���й����ʷ���	0������1������	   
#define 	F_QDirA		0x0010		//A���޹����ʷ���	0������1������	   
#define 	F_QDirB		0x0020		//B���޹����ʷ���	0������1������	   
#define 	F_QDirC		0x0040		//C���޹����ʷ���	0������1������	   

//MeterState3 Register flags define   
//#define 	F_Zone		0x0001		//��ǰ����ʱ��	0����һ�ס�1���ڶ���	   
#define 	F_Seg		0x0001		//��ǰ����ʱ��	0����һ�ס�1���ڶ���	   
#define 	F_MainPW	0x0000		//����Դ
#define 	F_AuxPW		0x0002		//������Դ
#define 	F_BatPW		0x0004		//��ص�Դ
#define 	F_PrgAllow	0x0008		//�������		0����ֹ��1�����
#define 	F_RelayOpen	0x0010		//�̵���״̬	0��ͨ��	 1����
#define 	F_Zone		0x0020		//��ǰ����ʱ��	0����һ�ס�1���ڶ���	//�������  
#define 	F_RelayOdState	0x0040	//�̵�������״̬ 0��ͨ�� 1����
#define 	F_PreTrip	0x0080		//Ԥ��բ����״̬ 0���ޡ� 1����
#define 	F_EC		0x0100		//������Ԥ���ѱ�
#define 	F_Cash		0x0200		//�����Ԥ���ѱ�
//#define 	F_Fee		0x0400		//��ǰ���з�ʱ����	0����һ�ס�1���ڶ���	//�¹�������   
//#define 	F_Step		0x0800		//��ǰ���н���	0����һ�ס�1���ڶ���		//�¹�������   

#define 	F_Safe			0x1000	//����״̬									//�¹���
#define 	F_IdentValid	0x2000	//�����֤״̬								//�¹���
#define 	F_LocalOpen		0x4000	//���ؿ���									//�¹���
#define 	F_RemoteOpen	0x8000	//Զ�̿���									//�¹���	

//MeterState4��5��6 Register flags define //A��B��C�����״̬   
#define 	F_PhFail	0x0001		//ʧѹ
#define 	F_PhLower	0x0002		//Ƿѹ
#define 	F_PhOver	0x0004		//��ѹ
#define 	F_CuFail	0x0008		//ʧ��
#define 	F_CuOver	0x0010		//����
#define 	F_PWOver	0x0020		//����
#define 	F_PWRev		0x0040		//��������
#define 	F_PhBreak	0x0080		//����
#define 	F_CuBreak	0x0100		//����

//MeterState7 Register flags define //�������״̬ 
#define 	F_VoltRevPhase	0x0001	//��ѹ������
#define 	F_CuRevPhase	0x0002	//����������
#define 	F_VoltImbalance	0x0004	//��ѹ��ƽ��
#define 	F_CuImbalance	0x0008	//������ƽ��
#define 	F_AuxPWFail		0x0010	//������Դʧ��
#define 	F_Pdown			0x0020	//����
#define 	F_MDOver		0x0040	//��������
#define 	F_PftOver		0x0080	//�������س�����
#define 	F_CuBestImbalance	0x0100	//�������ز�ƽ��

#define 	F_Cover			0x0200	//�����									//�¹���	
#define 	F_Terminal		0x0400	//����ť��									//�¹���	

//PrgState Register flags define //��̼�¼״̬ 
#define 	F_PrgRec		0x0001		//��̼�¼
#define 	F_SegPrg		0x0002		//ʱ�α�̼�¼
#define 	F_ZonePrg		0x0004		//ʱ����̼�¼
#define 	F_HolidayPrg	0x0008		//�ڼ��ձ�̼�¼
#define 	F_SaveDayPrg	0x0010		//�����ձ�̼�¼
#define 	F_RemotePrgRec	0x0020		//Զ�̱�̼�¼(ͨ����ȫ��֤��λ)

//EventOutState Register flags define 	//�¼��������״̬ 
#define 	F_CoverOpen		0x0001		//�����

//KeyStateFlg Register flags define 	//����״̬ 			//11.07.19
#define 	F_KeySFlag		0x01		//�Ϸ���
#define 	F_KeyXFlag		0x02		//�·���
#define 	F_KeyPFlag		0x04		//��̼�
#define 	F_KeyCFlag		0x08		//���Ǽ�
#define 	F_KeyTFlag		0x10		//���˼�

#define 	F_KeyRFlag		0x20		//0����բ 1:��բ ������ü̵���״̬	//�¹���	//13.09.27

typedef struct
{
  unsigned char 	Power;
  unsigned char 	Run;
  unsigned char	Clk;
  unsigned char	Disk;
  unsigned char	Error;
  unsigned char 	BatState;
  unsigned char AlarmFlg[4];
} FLAG;



typedef struct
{
  unsigned char PowerUpTime;
  unsigned short TestDisCnt;
  unsigned short BatUpTime;
  unsigned short TestCnt;
  unsigned long BatteryWTime;
  unsigned long ClockBatAD; 
  unsigned int CalibCount;
  unsigned int AT7022ChkSumCnt[MAX_CH_NUM];
  unsigned int MeasChipErrCnt;
  unsigned int State[8];
  unsigned int PulseWidthCnt[8];
  unsigned int SumPluseECP0[8];
  unsigned int PQFlag[8];
  unsigned int PQFlag_b[8];
  unsigned int PQNum[8][4];
  unsigned long        Angle_Ia[8]; 			//A������Ƕ�
  unsigned long        Angle_Ib[8]; 			//B������Ƕ�
  unsigned long        Angle_Ic[8]; 			//C������Ƕ�
  
  unsigned long        Angle_Ua[8]; 			//A������Ƕ�
  unsigned long        Angle_Ub[8]; 			//B������Ƕ�
  unsigned long        Angle_Uc[8]; 			//C������Ƕ�
  unsigned char P_Time[8][4];
  unsigned char Delay30Min;
  unsigned char Delay60Sec;
  unsigned char rebootflag;
} SRAM;

//�ܼ������ѹ����¼�ṹ
typedef struct
{
	unsigned long CheckTime;		//��ѹ���ʱ��(4)
	unsigned short NormalRate;		//��ѹ�ϸ���(2)
	unsigned short AbnormalRate;	//��ѹ������(2)
	unsigned long UpperLimitTime;	//��ѹ������ʱ��(4)
	unsigned long LowerLimitTime;	//��ѹ������ʱ��(4)
}VOLT_CHECK_REC;					//16�ֽ�

typedef struct
{
//	VOLT_CHECK_REC	MonVoltChk_Rec;	//���µ�ѹ�ϸ���ͳ������(16)
	VOLT_CHECK_REC	MonVoltChk_RecA;//����A���ѹ�ϸ���ͳ������(16)
	VOLT_CHECK_REC	MonVoltChk_RecB;//����B���ѹ�ϸ���ͳ������(16)
	VOLT_CHECK_REC	MonVoltChk_RecC;//����C���ѹ�ϸ���ͳ������(16)

//	VOLT_CHECK_REC	DayVoltChk_Rec;	//���յ�ѹ�ϸ���ͳ������(16)
	VOLT_CHECK_REC	DayVoltChk_RecA;//����A���ѹ�ϸ���ͳ������(16)
	VOLT_CHECK_REC	DayVoltChk_RecB;//����B���ѹ�ϸ���ͳ������(16)
	VOLT_CHECK_REC	DayVoltChk_RecC;//����C���ѹ�ϸ���ͳ������(16)

//	unsigned long SumVolt;			//����1�����ڵ�ѹ�ۼ�ֵ
	unsigned long SumVoltA;			//A��1�����ڵ�ѹ�ۼ�ֵ
	unsigned long SumVoltB;			//B��1�����ڵ�ѹ�ۼ�ֵ
	unsigned long SumVoltC;			//C��1�����ڵ�ѹ�ۼ�ֵ

//	unsigned short AverageVolt;		//����1�����ڵ�ѹƽ��ֵ
	unsigned short AverageVoltA;	//A��1�����ڵ�ѹƽ��ֵ
	unsigned short AverageVoltB;	//B��1�����ڵ�ѹƽ��ֵ
	unsigned short AverageVoltC;	//C��1�����ڵ�ѹƽ��ֵ

//	unsigned char VoltSecCnt;		//1�����ڵ�ѹ�������
	unsigned char VoltSecCntA;		//1������A���ѹ�������
	unsigned char VoltSecCntB;		//1������B���ѹ�������
	unsigned char VoltSecCntC;		//1������C���ѹ�������

}VOLTPARA;	

typedef struct
{
	VOLT_CHECK_REC*	MonVoltChk;
	VOLT_CHECK_REC*	DayVoltChk;
	unsigned long* SumV;
	unsigned short* AverVolt;
	unsigned char* SecCnt;
	unsigned short CurMonAddr;
	unsigned short CurDayAddr;
}VOLTCHECKPARA;


/*
typedef struct
{
	unsigned short ECBufAds;
	unsigned short ECBufAdsPtr;
	unsigned short ECRgAds;
	unsigned char* ECRamBufAds;
} ECRg;
*/
//#if ( MEASCHIP == IDT90E32 )
//typedef struct
//{
//	unsigned short ECEAds;
//	unsigned short ECFAds;
//	unsigned char* ECRAds;
//	unsigned short* ECRamBufAds;
//	unsigned char* ECRamBufChkAds;
//	unsigned char* SparePLRamBufAds;		//0.1CF����ʣ�����屣��
//	unsigned char* SparePLRamBufChkAds;		//0.1CF����ʣ�����屣��У��
//	unsigned short ECRgMode;		//0:�����������ʣ�1�����������ʡ�
//} ECRg;
//#else
typedef struct
{
	unsigned short ECEAds;
	unsigned short ECFAds;
	unsigned long* ECRAds;
	unsigned short* ECRamBufAds;
	unsigned char* ECRamBufChkAds;
	unsigned short ECRgMode;		//0:�����������ʣ�1������������
} ECRg;
//#endif

typedef struct
{
	unsigned short StartAds;		//������ʼ��ַ
	unsigned short EndAds;		//���������ַ
	unsigned short ECFAds;		//����FRAM�׵�ַ
	unsigned char* ECRAds;		//����RAM�׵�ַ
	unsigned char* Mode;		//���ģʽ��	
	unsigned short ECRgMode;		//0:�����������ʣ�1�����������ʡ�

}COMBECPARA;					//��ϵ������� 		


typedef struct
{
	short DevAds;
	short NextPage;
	unsigned char BakNum;
	unsigned char CorrectMode;
} E2Para;

/*
typedef struct
{
	//short (*p)(void);
	unsigned char (*p)(void);
	unsigned char* Cunt;	
	unsigned char* State;		// 
	unsigned char* OldState;	
}TAMPER;	
*/

//typedef struct						//N645
//{
//	unsigned short PtAddr;
//	unsigned short StartAddr;
//	unsigned short RecdCunt;
//	unsigned short RecdLenth;	
//} NPTPARA;							//645


#if( ZjProtocol	== YesCheck )		//���㽭ʡͨѶ��Լ
// 	ZjState					// �㽭ʡͨѶ��Լ�������״̬
#define zjIdle			0	// ͨѶ����
#define zjBufBusy		1	// ��ʼ�����ݣ�ͨѶ����æ
#define zjRead			2	// ��������
#define zjDataSend1Wait	3	// ���ݰ�1���͵ȴ�
#define zjDataSend2		4	// ���ݰ�2����
#define zjDataSend2Wait	5	// ���ݰ�2���͵ȴ�
#define zjDataSend3		6	// ���ݰ�3����
#define zjDataSend3Wait	7	// ���ݰ�3���͵ȴ�
#define zjDataSend4		8	// ���ݰ�4����
#define zjDataSend4Wait	9	// ���ݰ�4���͵ȴ�
#define zjExit			10	// ͨѶ�˳�
#else
#endif
/*
#if( N645Protocol	== YesCheck )	//��645-2007ͨѶ��Լ		//N645
// 	NState					// ͨѶ��Լ�������״̬				//N645
#define NIdle			0	// ͨѶ����							//N645	
#define NBufBusy		1	// ��ʼ�����ݣ�ͨѶ����æ			//N645
#define NRead			2	// ��������							//N645		
#define NDataSend1Wait	3	// ���ݰ�1���͵ȴ�					//N645
#define NDataSend2		4	// ���ݰ�2����						//N645	
#define NExit			5	// ͨѶ�˳�							//N645

#define NEW645PRO		0	//645-2007							//...
#define OLD645PRO		1	//645-1997
#define ZJPRO			2	//�㽭ʡ��Լ

#endif															//N645	
*/

	// Run flags define
#define  FComOK0   0x01		//����0������ϱ�־(���ͻ����)
#define  FComOK1   0x02		//����1������ϱ�־(���ͻ����)
#define  FComOK2   0x04		//����2������ϱ�־(���ͻ����)
#define  FComOK3   0x08		//����3������ϱ�־(���ͻ����)
/*
typedef struct
{
	unsigned short	CommIntReserve1;	//�������Ϳռ�1
	unsigned short	CommIntReserve2;	//�������Ϳռ�2
	unsigned short	CommIntReserve3;	//�������Ϳռ�3
	unsigned short	CommIntReserve4;	//�������Ϳռ�4

	unsigned short   BatOnDly;			// ͣ����ع���ʱ����ʱ

	unsigned short	HWYKRecL;			// ����ң�ؽ��յ��ֽ�					//V30
	unsigned short	HWYKRecH;			// ����ң�ؽ��ո��ֽ�(���ֽڵķ���)		//V30
	unsigned short 	RecCount;			// �˿ڽ���������			
	unsigned short 	BTime0;				// ������ַ��䶨ʱ
	unsigned short 	BTime1;				// 485��1�ַ��䶨ʱ
	unsigned short 	BTime2;				// 485��2�ַ��䶨ʱ
	unsigned short 	BTime3;				// 485��3�ַ��䶨ʱ
	unsigned short	BandRateTime1;		//485��1������ά�ֶ�ʱ 2S		
	unsigned short	BandRateTime2;		//485��2������ά�ֶ�ʱ 2S			
	unsigned short	BandRateTime3;		//485��3������ά�ֶ�ʱ 2S			

  	unsigned char 	Buf0[220];			// �����ͨ�Ż���		
  	unsigned char 	Buf1[220];			// 485��1ͨ�Ż���		
  	unsigned char 	Buf2[220];			// 485��2ͨ�Ż���		
  	unsigned char 	Buf3[220];			// 485��3ͨ�Ż���		
  	unsigned char 	Mode0;				// �����ͨѶģʽ
  	unsigned char 	Mode1;				// 485��1ͨѶģʽ
  	unsigned char 	Mode2;				// 485��2ͨѶģʽ
  	unsigned char 	Mode3;				// 485��3ͨѶģʽ
  	unsigned char 	Ptr0;				// ����ڻ�����ָ��
  	unsigned char 	Ptr1;				// 485��1������ָ��
  	unsigned char 	Ptr2;				// 485��2������ָ��
  	unsigned char 	Ptr3;				// 485��3������ָ��

	unsigned char	SendDTime0;			// ������ʱ
	unsigned char	SendDTime1;			// ������ʱ
	unsigned char	SendDTime2;			// ������ʱ
	unsigned char	SendDTime3;			// ������ʱ
	unsigned char 	TX_Delay0;			// ������ʱ1
	unsigned char 	TX_Delay1;			// ������ʱ1
	unsigned char 	TX_Delay2;			// ������ʱ2
	unsigned char 	TX_Delay3;			// ������ʱ1
	unsigned char 	TX_FESum0;			// ����FE����1
	unsigned char 	TX_FESum1;			// ����FE����2
	unsigned char 	TX_FESum2;			// ����FE����3
	unsigned char 	TX_FESum3;			// ����FE����4

	unsigned char	Run;				//���б�־

    //	unsigned char   T485DLY;            // 485״̬����ʱ��
	unsigned char	RxTx_Data;			// 485��1������շ����ֽ�
	signed char	BitCount;					// 485��1������շ����ֽڼ���

	unsigned char 	BandRate1;	//485��2������
	unsigned char 	BandRate2;	//485��3������
	unsigned char 	BandRate3;	//485��4������
	unsigned char 	BandRateMode;	//������ģʽ 0�����裻1������Ӧ   

//	unsigned char   BatOnDly;			// ͣ����ع���ʱ����ʱ
	unsigned char   CPAOnDly;			// ͣ���ڼ�Ƚ�������ʱ	
	
	unsigned char 	RecTmr0;			// ���ռ�¼��ʱ
	unsigned char 	RecTmr1;			// ���ռ�¼��ʱ
	unsigned char 	RecTmr2;			// ���ռ�¼��ʱ
	unsigned char 	RecTmr3;			// ���ռ�¼��ʱ
	unsigned char 	PrgTmr;				// ��̼�¼��ʱ
	
	unsigned char	EdgeState;			// ����ң�ؽ��ն˵�ƽ״̬				//V30
	unsigned char 	RecBitCnt;			// ����ң�ؽ���BIT����					//V30
	unsigned char   BitWidthCnt;		// ����ң�ؽ��յ�BITʱ��(��250usΪ��λ)	//V30
	unsigned char   RecByte;			// ����ң�ؽ��յ����ֽ�					//V30
	unsigned char   RecByte1;			// ����ң�ؽ��յ����ֽ�					//V30
	unsigned char   RecByte2;			// ����ң�ؽ��յ����ֽ�					//V30
	unsigned char   RecByte3;			// ����ң�ؽ��յ����ֽ�					//V30
	unsigned char   RecByte4;			// ����ң�ؽ��յ����ֽ�					//HW
	unsigned char   RecByteCnt;			// ����ң�ؽ��յ����ֽڼ���				//V30	
	unsigned char   RecTimeDelay;		// ����ң�ؽ��յ����ֽڼ���ʱ			//V30

#if( ZjProtocol	== YesCheck )		//���㽭ʡͨѶ��Լ
	unsigned char	ZjState1;		//1���㽭ʡͨѶ��Լ�������״̬
	unsigned char	ZjState2;		//2���㽭ʡͨѶ��Լ�������״̬
	unsigned char	ZjLength1;		//1����淢�ͳ���
	unsigned char	ZjLength2;		//2����淢�ͳ���
	unsigned char	ZjCheck1;		//1����淢��У���
	unsigned char	ZjCheck2;		//2����淢��У���
	unsigned char	ZjBTime1Rec;	//1�ڽ����ַ������ʱ ������ʾһ֡����
	unsigned char	ZjBTime2Rec;	//2�ڽ����ַ������ʱ ������ʾһ֡����
	unsigned char	ZjState3;		//3�����ͨѶ��Լ�������״̬
	unsigned char	ZjLength3;		//3����淢�ͳ���
	unsigned char	ZjCheck3;		//3����淢��У���
	unsigned char	ZjBTime3Rec;	//3�ڽ����ַ������ʱ ������ʾһ֡����
#else
#endif

	BCD4			OperateCode;	//�����ߴ���
	BCD4			TripONOperateCode;	//��բ�����ߴ���
	BCD4			TripOFFOperateCode;	//��բ�����ߴ���

	unsigned char 	BatComBuff[3];	//ͣ�����ѻ�����				//�¹���

	BCD8			CommReserve;	//����8�ֽڿռ�

} COMM;
*/

//ICComm.Power flags define
#define	 F_FmqbjEn		0x08		//��F_ECAlarm1״̬�·���������ʹ�ܡ�0������������1�������ر�			//V8
#define	 F_ECAlarm1FMQ	0x80		//F_ECAlarm1״̬�� 0:��������״̬ 1������û������ڿ��ٲ���״̬�²����鿨������������	 //V8

//ICComm.Run����
#define  F_ICComOK1   	0x04
#define  F_ICParity1  	0x08
#define  F_ICComOK2   	0x10
#define  F_ICParity2   	0x20

//ICMeterState Register flags define   
#define  F_RELAY	0x01		//�̵���״̬
#define  F_EEPROM	0x02		//EEPROM״̬
#define  F_BAT		0x04		//���״̬
#define  F_CLOCK	0x08		//ʱ��״̬
#define  F_REVEC	0x10		//�������״̬
#define  F_DRELAY	0x80		//��ʾ�ü̵���״̬		�����Ϻ���鿨������������룬���಻�䡣//V1000

//ICECState Register flags define   
#define F_ECNormal		0x00	//ʣ��������ڱ�������1��
#define F_ECAlarm1		0x01	//ʣ��������ڱ�������2��С�ڵ��ڱ�������1��
#define F_ECAlarm20		0x02	//ʣ���������0��С�ڵ��ڱ�������2��					//��û�в���û���(��բ����)��
#define F_ECAlarm21		0x03	//ʣ���������0��С�ڵ��ڱ�������2������û���(�ָ��õ�)��
#define F_ECOver		0x04	//ʣ���������0��͸֧��������0���Ƿ���բ����͸֧�����޶������
#define F_ECOverdraft	0x05	//͸֧��
#define F_ECOverEnd		0x06	//͸֧�������ꣻ		//V7
#define F_ECExceed		0x07	//����͸֧����޶	//�������	//11.06.27
#define F_ECEnd			0x08	//ʣ���������0��͸֧��������0��͸֧�����޶����0��			//11.08.19

#define F_NULL			0x55	//��(���ڼ̵�����)							//V7
  
//ICOutState Register flags define   
#define F_ECRelay		0x01	//�õ�̵�������״̬ 0: ��բ��1����բ
#define F_AlarmRelay	0x02	//�����̵�������״̬ 0: ��բ��1����բ
#define F_ECRelayLED	0x04	//�õ�̵�������״ָ̬ʾ�� 0: Ϩ��1������
#define F_AlarmRelayLED	0x08	//�����̵�������״ָ̬ʾ�� 0: Ϩ��1������
  
//ICInState Register flags define   
#define F_BuyEC			0x01	//�û����忨״̬ 0: δ�������1�����		//�����жϴ�С�ڵ��ڱ���2��͸֧����ǰ�õ�̵����Ƿ���բ
#define F_RegECLimt		0x02	//ʣ�����״̬ 0: ʣ�����С�ڶڻ������޶1��ʣ�����С�ڶڻ������޶�	//����ʣ�������˸��ʾ
#define F_BuyECOK		0x04	//�û������״̬ 0: ʧ�ܣ�1���ɹ�		//�����жϴӲ��û����Ƿ�Ҫ��ʾ������		//V7
#define F_BuyECOver		0x08	//͸֧�û����忨״̬ 0: δ�������1�����		//�����жϴ�С�ڵ���0��͸֧����ǰ�õ�̵����Ƿ���բ		//V7
#define F_CheckEC		0x10	//��鿨�忨״̬ 0: δ�������1�����		//�����жϴ�С�ڵ��ڱ���2��͸֧����ǰ�õ�̵����Ƿ���բ(���ٲ��Կ�״̬)	//V7
#define F_CheckECOver	0x20	//��鿨�忨״̬ 0: δ�������1�����		//�����жϴ�С�ڵ���0��͸֧����ǰ�õ�̵����Ƿ���բ(���ٲ��Կ�״̬)		//V7

#define F_Alarm2RelayONPermit	0x40	//�����жϴ�С�ڵ��ڱ���2��͸֧����ǰ�õ�̵�����բ��忨�������բ��
										//���ܺ�բ����������,��������EEPROM��	//11.04.23

typedef struct
{
	unsigned long	TripOFFDelay;		// ��բ��ʱʱ��
#if ( DLT645_2007_14 ==	YesCheck )	//�¹���	//14.04.25
	unsigned long   PreTripOFFKeepTime; //Զ��Ԥ��բ����ʱ��(��λ����)		//�¹���	//14.04.29
#endif
	unsigned long	LongReserve1;		// ����					//�¹���	//14.02.12

	enum {
		ICDisIdle,						// �������Ի���
		ICDisCardReading,				// ��ʾ"--------"
		ICDisBfRemainEC,				// ��ʾ��������ǰʣ�����
		ICDisRemainEC,					// ��ʾ�������Ӻ�ʣ�����
		ICDisMeter_No1,					// ��ʾ��Ÿ�4λ
		ICDisMeter_No2,					// ��ʾ��ŵ�6λ
		ICDisErr,						// ��ʾ�����
		ICDisExit,						// �����˳�
	} ICDisState;						// �忨����ʾ״̬

	unsigned short	ControlComExeState;	// ��������ִ��״̬
	unsigned short	ControlComErrState;	// �����������״̬

//	unsigned short	TripOFFDelay;		// ��բ��ʱʱ��
	unsigned short 	ICBTime1;			// IC��1�ַ��䶨ʱ
	unsigned short 	ICBTime2;			// IC��1�ַ��䶨ʱ
  	unsigned short 	ICPtr1;				// IC��1������ָ��
  	unsigned short 	ICPtr2;				// IC��1������ָ��
	unsigned short 	ICSendLength1;		// IC��1������շ����ֽ���
	unsigned short 	ICSendLength2;	    // IC��2������շ����ֽ���
	unsigned short 	ICRecLength1;		// IC��1��������ֽ���
	unsigned short 	ICRecLength2;	    // IC��2��������ֽ���
#if ( LocalSmartMeter == YesCheck )
	unsigned short	InsertCardState;	// �忨״̬��					//�¹���	//13.12.05
#endif

	unsigned short 	RelayDrivePulseWidth;	// �ű��̵ּ�������������	150����		//�¹���	//14.02.12
	unsigned short	IntReserve1;		// ����							//�¹���	//14.02.12

  	unsigned char 	ICBuf1[330];		// PSAM��ͨ�Ż���
  	unsigned char 	ICBuf2[330];		// ESAM��ͨ�Ż���
	unsigned char	ICRxTx_Data1;		// IC��1������շ����ֽ�
	signed char		ICBitCount1;		// IC��1������շ����ֽڼ���
	unsigned char	ICRxTx_Data2;		// IC��2������շ����ֽ�
	signed char		ICBitCount2;		// IC��2������շ����ֽڼ���
	
	unsigned char	Power;				//
	unsigned char	Run;				// ����״̬
	unsigned char	ICMeterState;		//
	unsigned char	ICECState;			//
	unsigned char	ICOutState;			//
	unsigned char	ICInState;			//
	
	unsigned char	ICAlarmFlg;			// ������־��
	unsigned char	ICOperateState;		// IC���忨����״̬�� 00���忨����������0x01�������룬0x02:�������̽�����0x03:��ǰ�ο�
	unsigned char	ICDisDelay;			// IC���忨��ʾ������ʱ
	unsigned char	ICRDStateDelay;		// IC���忨�Ƿ�ɹ���ʾ����ʱ
	
	unsigned char 	RPrivateKeyFlag;	// ˽Կ״̬��00����Կ״̬������
	unsigned char	ByteReCnt;			// �ֽ��ظ�����
	unsigned char	StrReCnt;			// �ַ����ظ�����	

	unsigned char 	CardType;			//������,��ʾ��
	unsigned char 	RelayErrDelay;		//�̵�����բ����ʱ����բ10�������������ɼ���������ʾ�̵������ϡ�
	unsigned char 	SoundAlarmDelay;	//ʣ�����С�ڱ�������1��ÿ�ɼ���һ���������壬����3�롣
	
	unsigned char 	Alarm2State;		//ʣ�������0����������2֮���״̬ 0�����ڸ����䣬1���ڸ����䣬�̵�����բ(��բ����)��
										//2���ڸ���������û�����ָ����硣
	unsigned char 	RelayTestDelay;		//�̵������Կ�������ʱ

	unsigned char   ModifyKeyClrDly;	//��Կ�޸Ŀ����ϵ�10���Ӻ��������������
	
//#if ( MagnKeepRelay	== YesCheck )
	unsigned char 	RelayDriveType;		//�ű��̵ּ�����������	0x00���պϣ�0x01���Ͽ��� ���಻������ 
	unsigned char 	RelayDriveCnt;		//�ű��̵ּ�����������	3��
//	unsigned char 	RelayDrivePulseWidth;	//�ű��̵ּ�������������	150����
	unsigned char 	CharReserve1;		//����							//�¹���	//14.02.12
	unsigned char 	RelayONState;		//���ɿ���ִ�к�բ״̬��ʼ��	0����ִ��	0xAA:������պ�բָ��(������忨) 0x55:ִ��
	unsigned char 	RelayOFFState;		//���ɿ���ִ����բբ״̬��ʼ��		0����ִ��	0x55:ִ��
	unsigned char 	CardRelayDelay;		//�̵������������Чʱ����ʱ��	//�ڸ���ʱʱ����ִ�м̵�������������ʱ����ִ���������ж�����բ����

	unsigned char	ErrorMessage;		//������Ϣ��			//�¹���	//13.12.02
	unsigned char	OperateCommHead[5];	//��������ͷ			//�¹���	//13.12.02	
	unsigned char	ErrorResponse[2];	//������Ӧ״̬			//�¹���	//13.12.02
	unsigned char	InsertCardECCnt[2];	//�忨ʱ�ܹ������		//�¹���	//13.12.02	
	unsigned char	InsertCardRemainEC[4];	//�忨ʱʣ����	//�¹���	//13.12.02

	unsigned char	ICDisAlarmFlg;		//��ʾ������־��		//�¹���	//13.12.20
	
//#else
//#endif		
	unsigned char	ParaUpdateFlag;		//�������±�־λ
	unsigned char 	YearMonSaveMode;	//���½���ģʽ	0x00:�½���	�����������											//�¹���	//13.11.29
	unsigned char	RFeePriceNum;		//���ʵ����	0�����ʵ��Ϊȫ0	��������Ч���ʵ��								//�¹���	//13.11.30
	unsigned char 	CardFaultFMQDelay;	//����ʧ�ܣ�����������������		����Ϊ625ms������Ϊ125ms���������ҲΪ125ms		//�¹���	//13.12.05
	unsigned char	ROpenAccountFlag;	//�����ɹ���־   Bit0: 0:���ؿ��� 1������δ����  Bit1: 0:Զ�̿��� 1��Զ��δ����  	//�¹���	//13.12.05   
	unsigned char	RJudgePermitFlag;	//���жϺ�բ�������־(2)	0x55:���жϣ����������ж�							  	//�¹���	//13.12.10   

//	BCD8			IccommReserve;		//����8�ֽڿռ�
	unsigned char 	RDisPrivateKeyFlag;	//��ʾ��˽Կ״̬��00����Կ״̬������		//12.08.04
	BCD7			IccommReserve;		//����8�ֽڿռ�
	
//#else
//#endif
	unsigned char   SystemTitle[8];     //ϵͳ�������߼��豸��ת������--8�ֽڹ̶�..
	unsigned char   ClientSystemTitle[8];     //�ͻ���ϵͳ������AARQ�����߼�ת������--8�ֽڲ��̶�..
	unsigned char	CommErrTime[4]; 	//ͨѶ�ڴ������(ʱ�����������ʱ������2��Сʱ����)..
	unsigned char	SHEZHIErrTime;		//���ô������..

}ICCOMM;

typedef struct
{
	unsigned long	RRemainEC;				//ʣ����		
	unsigned long	ROverdraftEC;			//͸֧���		
	unsigned long	RPermitTripONLimt;		//��բ������		
//	unsigned long 	ECStep[10];
	unsigned long 	ECStep[6];				//�¹���
	unsigned short 	IdentityValidDelay;		//�����֤��Чʱ��				== 0 ��Ч ������
	unsigned short 	InfraredValidDelay;		//������֤��Чʱ��				== 0 ��Ч �������		//�¹���
	BCD8	RConsumECSumBCD;				//16�����ۼ��õ���(8)	HEX		XXXXXXXXXX.XXXXXX
	BCD4	Random1;						//�����1(��վ�·�)		
	BCD4	Random2;						//�����2(ESAM�ϴ�)		
	BCD8	DisperseGene;					//Զ�̷�ɢ����(���+�����ֽ�0)
	BCD8	LocalDisperseGene;				//���ط�ɢ����(���Ż���+�����ֽ�0)
	
	unsigned char 	IdentityValidDelayChk;	//�����֤��Чʱ��У���		== 0 ��Ч ������
	unsigned char 	InfraredValidDelayChk;	//������֤��Чʱ��У���		== 0 ��Ч �������		//�¹���
	unsigned char 	RConsumECSumBCDChk;		//10�����ۼ��õ���У���	

	unsigned char 	DecDelay;				//15����ESAM��Ǯ��ʱ
	unsigned char 	RemoteDecDelay;			//Զ��15����ESAM��Ǯ��ʱ

	unsigned char   IdentityMACFaultCnt;	//MACʧ�ܴ���
	unsigned char   IdentityFaultCnt;		//�����֤ʧ�ܴ���
	unsigned char   IdentityMACFaultCntChk;	//MACʧ�ܴ���
	unsigned char   IdentityFaultCntChk;	//�����֤ʧ�ܴ���
	unsigned char   IdentityState;			//�����֤����״̬
	unsigned char	ECStepChk;				//���ݵ���У���

	unsigned char	InfraredRequestValid;	//���ں�����֤��ȴ�������ָ֤���ڼ䣬ESAM�����е磬������֤��������ͨ������0x01;		//�¹���
											//������ָ֤��ͨ�������µ�������֤������;	

	unsigned char use_DK;		//�ж��Ƿ��DK..
	unsigned char use_HLSname;	//1--�����ã�3--���ü���..
	unsigned char use_cipher_information; //ʹ�ü����û���Ϣ����..	
	unsigned char use_SHA256;	//0--�����ã�1--����..
	unsigned char d_key[16];	//16�ֽڵ�dedicated key..
	unsigned char BAOLIU[16];

}RPESAM;

typedef struct
{
	unsigned char* TabPtr;	//֡������ָ��
	unsigned char Type;		//֡������		
	unsigned char Len;		//֡�ܳ���		
	unsigned char ItemCount;//֡��������
}FramePARA;


typedef struct
{
	unsigned short 	DisID;
	unsigned short 	ComID;
	unsigned short 	Addr;
}COMDISTAB;

typedef struct
{
	unsigned short 	ComID;
	unsigned short 	Addr;
	unsigned short	State;
}CDPARATAB;

typedef struct
{
	unsigned short 	ComID;
	unsigned long 	Addr;
  	unsigned short 	ComInfo;
}COMTAB;

typedef struct					//N645
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
}NCOMTAB;						//N645
/*
typedef struct
{
	unsigned short 	DisID;
	unsigned short 	Addr;
	unsigned char 	Length;
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
}DISTAB;
*/
typedef struct
{
	unsigned short 	ComID;
	unsigned char 	*Addr;
  	unsigned short 	ComInfo;
}BATCOMTAB;
/*
#if( N645Protocol	== YesCheck )	//��645-2007ͨѶ��Լ		//N645
typedef struct
{
	unsigned char 	*Buf;			// ͨѶ������
	unsigned char 	*Mode;			// ͨѶģʽ
  	unsigned char 	*Ptr;			// ������ָ��
//  	unsigned char 	*BTime;			// �ַ��䶨ʱ
  	unsigned short 	*BTime;			// �ַ��䶨ʱ
  	unsigned char 	*SendDTime;		// ������ʱ
  	unsigned char 	*NState;		// ͨѶ״̬
  	unsigned char 	*NLength;		// ���ͳ���
}COMMPARA;
#else
*/
/*
typedef struct
{
	unsigned char 	*Buf;			// ͨѶ������
	unsigned char 	*Mode;			// ͨѶģʽ
  	unsigned char 	*Ptr;			// ������ָ��
//  	unsigned char 	*BTime;			// �ַ��䶨ʱ
  	unsigned short 	*BTime;			// �ַ��䶨ʱ
  	unsigned char 	*SendDTime;		// ������ʱ
  	unsigned char 	*RecTmr;		// ͨѶ��ʱ
  	unsigned char 	*FESum;			// ͨѶFE����
}COMMPARA;
*/
typedef struct
{
  unsigned char*	Buf;			// ͨѶ������
  unsigned char*	Mode;			// ͨѶģʽ
  unsigned char*	SendDTime;		// ������ʱ
  unsigned char*	RecTmr; 		// ͨѶ��ʱ
  unsigned short*	  Ptr;			// ������ָ��
  unsigned short*		BTime;			// �ַ��䶨ʱ
  
	unsigned char*	IECState;		// ͨѶ��IECͨѶ����״̬
	unsigned char*	MACState;		// ͨѶ��MACͨѶ����״̬
	unsigned char*	LLCState;		// ͨѶ��MACͨѶ����״̬
	unsigned char*	TX_Delay;		// ������ʱ
	unsigned char*	Run;			// 485�����б�־
	unsigned char*	BandRate;		// 485�ڲ�����
	unsigned char*	FTime;			// ֡�䶨ʱ
//	unsigned char*	RxdCnt; 		// ����֡������
//	unsigned char*	SendCnt;		// ����֡������
	unsigned char*	RxdFrameNo; 	// ����֡������
	unsigned char*	TxdFrameNo; 	// ����֡������
	unsigned char*	LastRecCommand; // ����յ�������
	unsigned char*	APPLState;		// ͨѶ��APPL����״̬

	unsigned long*	BlockNumber;	// ����GET��ͨѶ���
	unsigned short*	HBTime; 		// 485���ַ��䶨ʱ
    unsigned short*	RecDataPtr; 	// 485�ڻ�����ָ��
    unsigned short*	SendDataPtr;	// 485�ڻ�����ָ��
    unsigned short*	SendByteSum;	// 485��LLC��������
    unsigned short*	  SetBlockPtr;	  // �����û���ָ��

    unsigned char*	RecDataBuf; 	// 485�ڽ���ͨ�Ż���		
    unsigned char*	SendDataBuf;	// 485�ڷ���ͨ�Ż���		
    unsigned char*	HMode;			// 485��ͨѶģʽ
    unsigned char*	HState; 		// 485��ͨ��״̬
	unsigned char*	HBandRate;		// 485�ڲ�����
	unsigned char*	HRecTmr;		// ���ռ�¼��ʱ
	unsigned char*	HPrgTmr;		// ��̼�¼��ʱ
	unsigned char*	Conformance;	// ͨѶ��Ӧ�����ӵ�һ���Կ����
	unsigned char*	SubsBlockPara;	// ���ں���֡�����
	unsigned char*	SetBlockBuff;	// �����û�����ַ
	//unsigned char reserv1;
	//unsigned char reserv2;
}COMMPARA;

//#endif														//N645

#if( ZjProtocol	== YesCheck )		// ���㽭ͨѶ��Լ
typedef struct
{
	unsigned char 	*Buf;			// ͨѶ������
	unsigned char 	*Mode;			// ����ԼͨѶģʽ
  	unsigned char 	*Ptr;			// ������ָ��
  	unsigned short 	*BTime;			// �ַ��䶨ʱ
  	unsigned char 	*SendDTime;		// ������ʱ
	unsigned char	*ZjState;		// �㽭��ԼͨѶ״̬
	unsigned char	*ZjLength;		// �㽭��Լÿ�����ݷ��ͳ���
	unsigned char	*ZjCheck;		// �㽭��Լÿ������У���
}ZJCOMMPARA;
#else
#endif

/*									//PWMD	
typedef struct
{
	unsigned short 	MDBufAds;		// ������������ַ
	unsigned short 	*MDCum;			// 1�����ۼ�������
}MDPARA;
*/
/*								
typedef struct	
{
	unsigned short 	MDEAds;
	unsigned short* 	MDBufAds;		// ������������ַ
	unsigned long* 	MDCum;			// 1�����ۼƹ���
	unsigned short 	MDRgMode;		//0:�����������ʣ�1�����������ʡ�
}MDPARA;
*/
typedef struct
{
//	unsigned short	MDBufAds;		// ������������ַ
//	unsigned short	*MDCum; 		// 1�����ۼ�������
	unsigned long		*MDCum; 		// 1�����ۼ�������	
	
}MDPARA;

typedef struct
{
	unsigned short 	MDEAds;
	unsigned short* 	MDBufAds;		// ������������ַ
	unsigned char* 	Mode;			//���ģʽ��	
	unsigned short 	MDRgMode;		//0:�����������ʣ�1�����������ʡ�

}COMBMDPARA;						//��ϵ������� 		


typedef struct
{
	unsigned long* 	SampleSum;		// n�������ڲ����������ۼӺ�
	unsigned char* 	Irms;			// �������Чֵ
}IRMSPARA;

typedef struct
{
	unsigned short 	ECAddr;			//���ڶ�����ַ
  	unsigned short 	EnterDay;		//���뼾������
	unsigned short	OutDay;			//�˳���������
	unsigned char*  ZoneNo;			//��ǰʱ����
}SEASONPARA;

#if( SecurityCheck == YesCheck )				
typedef struct
{
	unsigned short 	CuLow;			//��������
  	unsigned short 	CuHigh;			//��������
	unsigned short	CosLow;			//������������
	unsigned short	CosHigh;		//������������
}SECURITYPARA;
#else
#endif

#if ( MEASCHIP == ADE7758 )
typedef struct
{
	unsigned char 	ADERgAddr;		//7758�Ĵ�����ַ
  	unsigned char 	Length;			//�Ĵ����ֽڳ���
}ADEPara;

typedef struct
{
	unsigned char*  RamRgAddr;		//˲ʱֵRAM��ַ		
	unsigned char 	ATTRgAddr;		//ATT7022�Ĵ�����ַ
  	unsigned char 	Length;			//�Ĵ����ֽڳ���
}RamPara;

typedef struct
{
	short CommID;		//ͨѶ����
	short Length;		//�鳤��
  	short Cycle;		//������
}ADESwapPara;
#else
#endif

#if ( ZjProtocol == YesCheck)
typedef struct
{
	unsigned short 	ComID;
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
}ZJCOMTAB;
#else
#endif




//A��B��C��ʧѹ��Ƿѹ����ѹ�����ࡢʧ�����������������������򡢹����ܴ��������ۼ�ʱ��ṹ
typedef struct
{
	BCD3	ATimes;					//A���ܴ���
	BCD3	ATime;					//A�����ۼ�ʱ��
	BCD3	BTimes;					//B���ܴ���
	BCD3	BTime;					//B�����ۼ�ʱ��
	BCD3	CTimes;					//C���ܴ���
	BCD3	CTime;					//C�����ۼ�ʱ��

}PHASE_EVENT_TIMES;				//18�ֽ�


//ȫʧѹ��������Դʧ�硢��ѹ�����������򡢵�ѹ��������ƽ���ܴ��������ۼ�ʱ��ṹ
typedef struct
{
	BCD3	Times;					//�ܴ���
	BCD3	Time;					//���ۼ�ʱ��

}EVENT_TIMES;					//6�ֽ�

/*
//A��B��C��ʧѹ��Ƿѹ����ѹ�������¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)
	BCD4	AmongECPp0;				//�ڼ��й������ܵ�������(4)
	BCD4	AmongECPn0;				//�ڼ��й������ܵ�������(4)
	BCD4	AmongECQp0;				//�ڼ�����޹�1�ܵ�������(4)
	BCD4	AmongECQn0;				//�ڼ�����޹�2�ܵ�������(4)
	BCD4	AmongECPpA;				//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECPnA;				//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECQpA;				//�ڼ�A������޹�1�ܵ�������(4)
	BCD4	AmongECQnA;				//�ڼ�A������޹�2�ܵ�������(4)
	BCD2	Ua;						//�쳣����ʱ��A���ѹ(2)
	BCD3	Ia;						//�쳣����ʱ��A�����(3)
	BCD3	Pa;						//�쳣����ʱ��A���й�����(3)
	BCD3	Qa;						//�쳣����ʱ��A���޹�����(3)
	BCD2	Pfa;					//�쳣����ʱ��A�๦������(2)
	BCD4	AmongECPpB;				//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECPnB;				//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECQpB;				//�ڼ�B������޹�1�ܵ�������(4)
	BCD4	AmongECQnB;				//�ڼ�B������޹�2�ܵ�������(4)
	BCD2	Ub;						//�쳣����ʱ��B���ѹ(2)
	BCD3	Ib;						//�쳣����ʱ��B�����(3)
	BCD3	Pb;						//�쳣����ʱ��B���й�����(3)
	BCD3	Qb;						//�쳣����ʱ��B���޹�����(3)
	BCD2	Pfb;					//�쳣����ʱ��B�๦������(2)
	BCD4	AmongECPpC;				//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECPnC;				//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECQpC;				//�ڼ�C������޹�1�ܵ�������(4)
	BCD4	AmongECQnC;				//�ڼ�C������޹�2�ܵ�������(4)
	BCD2	Uc;						//�쳣����ʱ��C���ѹ(2)
	BCD3	Ic;						//�쳣����ʱ��C�����(3)
	BCD3	Pc;						//�쳣����ʱ��C���й�����(3)
	BCD3	Qc;						//�쳣����ʱ��C���޹�����(3)
	BCD2	Pfc;					//�쳣����ʱ��C�๦������(2)
	BCD4	AmongAh;				//�쳣�����ڼ��ܰ�ʱ��(4)
	BCD4	AmongAhA;				//�쳣�����ڼ�A�లʱ��(4)
	BCD4	AmongAhB;				//�쳣�����ڼ�B�లʱ��(4)
	BCD4	AmongAhC;				//�쳣�����ڼ�C�లʱ��(4)

}VOLT_REC;					//131�ֽ�
*/
/*
//A��B��C��ʧѹ��Ƿѹ����ѹ�������¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD2	Ua;						//�쳣����ʱ��A���ѹ(2)
	BCD3	Ia;						//�쳣����ʱ��A�����(3)
	BCD3	Pa;						//�쳣����ʱ��A���й�����(3)
	BCD3	Qa;						//�쳣����ʱ��A���޹�����(3)
	BCD2	Pfa;					//�쳣����ʱ��A�๦������(2)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD2	Ub;						//�쳣����ʱ��B���ѹ(2)
	BCD3	Ib;						//�쳣����ʱ��B�����(3)
	BCD3	Pb;						//�쳣����ʱ��B���й�����(3)
	BCD3	Qb;						//�쳣����ʱ��B���޹�����(3)
	BCD2	Pfb;					//�쳣����ʱ��B�๦������(2)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD2	Uc;						//�쳣����ʱ��C���ѹ(2)
	BCD3	Ic;						//�쳣����ʱ��C�����(3)
	BCD3	Pc;						//�쳣����ʱ��C���й�����(3)
	BCD3	Qc;						//�쳣����ʱ��C���޹�����(3)
	BCD2	Pfc;					//�쳣����ʱ��C�๦������(2)
	BCD4	AmongAh;				//�쳣�����ڼ��ܰ�ʱ��(4)
	BCD4	AmongAhA;				//�쳣�����ڼ�A�లʱ��(4)
	BCD4	AmongAhB;				//�쳣�����ڼ�B�లʱ��(4)
	BCD4	AmongAhC;				//�쳣�����ڼ�C�లʱ��(4)

}VOLT_REC;					//195�ֽ�
*/

//A��B��C��ʧѹ��Ƿѹ����ѹ�������¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD2	Ua;						//�쳣����ʱ��A���ѹ(2)
	BCD3	Ia;						//�쳣����ʱ��A�����(3)
	BCD3	Pa;						//�쳣����ʱ��A���й�����(3)
	BCD3	Qa;						//�쳣����ʱ��A���޹�����(3)
	BCD2	Pfa;					//�쳣����ʱ��A�๦������(2)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD2	Ub;						//�쳣����ʱ��B���ѹ(2)
	BCD3	Ib;						//�쳣����ʱ��B�����(3)
	BCD3	Pb;						//�쳣����ʱ��B���й�����(3)
	BCD3	Qb;						//�쳣����ʱ��B���޹�����(3)
	BCD2	Pfb;					//�쳣����ʱ��B�๦������(2)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD2	Uc;						//�쳣����ʱ��C���ѹ(2)
	BCD3	Ic;						//�쳣����ʱ��C�����(3)
	BCD3	Pc;						//�쳣����ʱ��C���й�����(3)
	BCD3	Qc;						//�쳣����ʱ��C���޹�����(3)
	BCD2	Pfc;					//�쳣����ʱ��C�๦������(2)
	BCD4	AmongAh;				//�쳣�����ڼ��ܰ�ʱ��(4)
	BCD4	AmongAhA;				//�쳣�����ڼ�A�లʱ��(4)
	BCD4	AmongAhB;				//�쳣�����ڼ�B�లʱ��(4)
	BCD4	AmongAhC;				//�쳣�����ڼ�C�లʱ��(4)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}VOLT_REC;					//195�ֽ�


//ȫʧѹ��¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	Amper;			//ȫʧѹ����ֵ(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)

}ALLFAIL_REC;				//15�ֽ�

//������Դʧ�硢�����¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)

}POWER_REC;					//12�ֽ�	
//}ASSISTPW_REC;				//12�ֽ�

//�����¼�ṹ
//typedef struct
//{
//	BCD3	StaTime;		//�쳣��ʼʱ��(3)
//	BCD3	StaDate;		//�쳣��ʼ����(3)
//	BCD3	EndTime;		//�쳣����ʱ��(3)
//	BCD3	EndDate;		//�쳣��������(3)

//} POWER_DOWN_REC;				//20�ֽ�

/*
//��ѹ��������ƽ���¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)
	BCD2	ImbalanceRate;			//��ƽ����(2)
	BCD4	AmongECPp0;				//�ڼ��й������ܵ�������(4)
	BCD4	AmongECPn0;				//�ڼ��й������ܵ�������(4)
	BCD4	AmongECQp0;				//�ڼ�����޹�1�ܵ�������(4)
	BCD4	AmongECQn0;				//�ڼ�����޹�2�ܵ�������(4)
	BCD4	AmongECPpA;				//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECPnA;				//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECQpA;				//�ڼ�A������޹�1�ܵ�������(4)
	BCD4	AmongECQnA;				//�ڼ�A������޹�2�ܵ�������(4)
	BCD4	AmongECPpB;				//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECPnB;				//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECQpB;				//�ڼ�B������޹�1�ܵ�������(4)
	BCD4	AmongECQnB;				//�ڼ�B������޹�2�ܵ�������(4)
	BCD4	AmongECPpC;				//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECPnC;				//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECQpC;				//�ڼ�C������޹�1�ܵ�������(4)
	BCD4	AmongECQnC;				//�ڼ�C������޹�2�ܵ�������(4)

}IMBALANCE_REC;				//78�ֽ�
*/
/*
//��ѹ��������ƽ���¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD3	ImbalanceRate;			//��ƽ����(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}IMBALANCE_REC;				//143�ֽ�
*/

//��ѹ��������ƽ���¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD3	ImbalanceRate;			//��ƽ����(3)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}IMBALANCE_REC;				//143�ֽ�


/*
//A��B��C��ʧ����������������¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)
	BCD4	AmongECPp0;		//�ڼ��й������ܵ�������(4)
	BCD4	AmongECPn0;		//�ڼ��й������ܵ�������(4)
	BCD4	AmongECQp0;		//�ڼ�����޹�1�ܵ�������(4)
	BCD4	AmongECQn0;		//�ڼ�����޹�2�ܵ�������(4)
	BCD4	AmongECPpA;		//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECPnA;		//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECQpA;		//�ڼ�A������޹�1�ܵ�������(4)
	BCD4	AmongECQnA;		//�ڼ�A������޹�2�ܵ�������(4)
	BCD2	Ua;				//�쳣����ʱ��A���ѹ(2)
	BCD3	Ia;				//�쳣����ʱ��A�����(3)
	BCD3	Pa;				//�쳣����ʱ��A���й�����(3)
	BCD3	Qa;				//�쳣����ʱ��A���޹�����(3)
	BCD2	Pfa;			//�쳣����ʱ��A�๦������(2)
	BCD4	AmongECPpB;		//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECPnB;		//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECQpB;		//�ڼ�B������޹�1�ܵ�������(4)
	BCD4	AmongECQnB;		//�ڼ�B������޹�2�ܵ�������(4)
	BCD2	Ub;				//�쳣����ʱ��B���ѹ(2)
	BCD3	Ib;				//�쳣����ʱ��B�����(3)
	BCD3	Pb;				//�쳣����ʱ��B���й�����(3)
	BCD3	Qb;				//�쳣����ʱ��B���޹�����(3)
	BCD2	Pfb;			//�쳣����ʱ��B�๦������(2)
	BCD4	AmongECPpC;		//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECPnC;		//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECQpC;		//�ڼ�C������޹�1�ܵ�������(4)
	BCD4	AmongECQnC;		//�ڼ�C������޹�2�ܵ�������(4)
	BCD2	Uc;				//�쳣����ʱ��C���ѹ(2)
	BCD3	Ic;				//�쳣����ʱ��C�����(3)
	BCD3	Pc;				//�쳣����ʱ��C���й�����(3)
	BCD3	Qc;				//�쳣����ʱ��C���޹�����(3)
	BCD2	Pfc;			//�쳣����ʱ��C�๦������(2)

}CURRENT_REC;				//115�ֽ�
*/
/*
//A��B��C��ʧ����������������¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD2	Ua;						//�쳣����ʱ��A���ѹ(2)
	BCD3	Ia;						//�쳣����ʱ��A�����(3)
	BCD3	Pa;						//�쳣����ʱ��A���й�����(3)
	BCD3	Qa;						//�쳣����ʱ��A���޹�����(3)
	BCD2	Pfa;					//�쳣����ʱ��A�๦������(2)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD2	Ub;						//�쳣����ʱ��B���ѹ(2)
	BCD3	Ib;						//�쳣����ʱ��B�����(3)
	BCD3	Pb;						//�쳣����ʱ��B���й�����(3)
	BCD3	Qb;						//�쳣����ʱ��B���޹�����(3)
	BCD2	Pfb;					//�쳣����ʱ��B�๦������(2)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD2	Uc;						//�쳣����ʱ��C���ѹ(2)
	BCD3	Ic;						//�쳣����ʱ��C�����(3)
	BCD3	Pc;						//�쳣����ʱ��C���й�����(3)
	BCD3	Qc;						//�쳣����ʱ��C���޹�����(3)
	BCD2	Pfc;					//�쳣����ʱ��C�๦������(2)

}CURRENT_REC;					//179�ֽ�
*/

//A��B��C��ʧ����������������¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD2	Ua;						//�쳣����ʱ��A���ѹ(2)
	BCD3	Ia;						//�쳣����ʱ��A�����(3)
	BCD3	Pa;						//�쳣����ʱ��A���й�����(3)
	BCD3	Qa;						//�쳣����ʱ��A���޹�����(3)
	BCD2	Pfa;					//�쳣����ʱ��A�๦������(2)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD2	Ub;						//�쳣����ʱ��B���ѹ(2)
	BCD3	Ib;						//�쳣����ʱ��B�����(3)
	BCD3	Pb;						//�쳣����ʱ��B���й�����(3)
	BCD3	Qb;						//�쳣����ʱ��B���޹�����(3)
	BCD2	Pfb;					//�쳣����ʱ��B�๦������(2)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD2	Uc;						//�쳣����ʱ��C���ѹ(2)
	BCD3	Ic;						//�쳣����ʱ��C�����(3)
	BCD3	Pc;						//�쳣����ʱ��C���й�����(3)
	BCD3	Qc;						//�쳣����ʱ��C���޹�����(3)
	BCD2	Pfc;					//�쳣����ʱ��C�๦������(2)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}CURRENT_REC;					//179�ֽ�

/*
//��ѹ������������A��B��C�ೱ�����򡢹��ؼ�¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)
	BCD4	AmongECPp0;				//�ڼ��й������ܵ�������(4)
	BCD4	AmongECPn0;				//�ڼ��й������ܵ�������(4)
	BCD4	AmongECQp0;				//�ڼ�����޹�1�ܵ�������(4)
	BCD4	AmongECQn0;				//�ڼ�����޹�2�ܵ�������(4)
	BCD4	AmongECPpA;				//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECPnA;				//�ڼ�A���й������ܵ�������(4)
	BCD4	AmongECQpA;				//�ڼ�A������޹�1�ܵ�������(4)
	BCD4	AmongECQnA;				//�ڼ�A������޹�2�ܵ�������(4)
	BCD4	AmongECPpB;				//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECPnB;				//�ڼ�B���й������ܵ�������(4)
	BCD4	AmongECQpB;				//�ڼ�B������޹�1�ܵ�������(4)
	BCD4	AmongECQnB;				//�ڼ�B������޹�2�ܵ�������(4)
	BCD4	AmongECPpC;				//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECPnC;				//�ڼ�C���й������ܵ�������(4)
	BCD4	AmongECQpC;				//�ڼ�C������޹�1�ܵ�������(4)
	BCD4	AmongECQnC;				//�ڼ�C������޹�2�ܵ�������(4)

}PHASE_POWER_REC;					//76�ֽ�		
*/

/*
//��ѹ������������A��B��C�ೱ�����򡢹��ؼ�¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}PHASE_POWER_REC;				//140�ֽ�
*/

//��ѹ������������A��B��C�ೱ�����򡢹��ؼ�¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	EndECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	EndECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	EndECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	EndECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	EndECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	EndECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	EndECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	EndECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}PHASE_POWER_REC;				//140�ֽ�


/*
//�ܼ������ѹ����¼�ṹ
typedef struct
{
	BCD3	CheckTime;				//��ѹ���ʱ��(3)
	BCD3	NormalRate;				//��ѹ�ϸ���(3)
	BCD3	AbnormalRate;			//��ѹ������(3)
	BCD3	UpperLimitTime;			//��ѹ������ʱ��(3)
	BCD3	LowerLimitTime;			//��ѹ������ʱ��(3)
	BCD2	MaxVolt;				//��ߵ�ѹ(2)
	BCD4	MaxVoltHpTime;			//��ߵ�ѹ����ʱ��(4)
	BCD2	LeastVolt;				//��͵�ѹ(2)
	BCD4	LeastVoltHpTime;		//��͵�ѹ����ʱ��(4)

}VOLT_CHECK_REC;				//27�ֽ�
*/

//�������޼�¼�ṹ
typedef struct
{
	BCD3	StaTime;		//�쳣��ʼʱ��(3)
	BCD3	StaDate;		//�쳣��ʼ����(3)
	BCD3	EndTime;		//�쳣����ʱ��(3)
	BCD3	EndDate;		//�쳣��������(3)
	BCD3	MaxDemand;				//�쳣�ڼ��������(3)
	BCD5	MaxDemandHpTime;		//�쳣�ڼ������������ʱ��(5)

} MD_OVER_REC;				//20�ֽ�


//��̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//��̷���ʱ��(3)
	BCD3	HappenDate;				//��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4	ID01;					//���ݱ�ʶ��1(4)
	BCD4	ID2;					//���ݱ�ʶ��2(4)
	BCD4	ID3;					//���ݱ�ʶ��3(4)
	BCD4	ID4;					//���ݱ�ʶ��4(4)
	BCD4	ID5;					//���ݱ�ʶ��5(4)
	BCD4	ID6;					//���ݱ�ʶ��6(4)
	BCD4	ID7;					//���ݱ�ʶ��7(4)
	BCD4	ID8;					//���ݱ�ʶ��8(4)
	BCD4	ID9;					//���ݱ�ʶ��9(4)
	BCD4	ID10;					//���ݱ�ʶ��10(4)

}PRG_REC;					//50�ֽ�	


//���������¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//�������㷢��ʱ��(3)
	BCD3	HappenDate;				//�������㷢������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4	ECPp0;					//��������ǰ�����й��ܵ���(4)
	BCD4	ECPn0;					//��������ǰ�����й��ܵ���(4)
	BCD4	ECQ10;					//��������ǰ1�����޹��ܵ���(4)
	BCD4	ECQ20;					//��������ǰ2�����޹��ܵ���(4)
	BCD4	ECQ30;					//��������ǰ3�����޹��ܵ���(4)
	BCD4	ECQ40;					//��������ǰ4�����޹��ܵ���(4)
	BCD4	ECPpA0;					//��������ǰA�������й��ܵ���(4)
	BCD4	ECPnA0;					//��������ǰA�෴���й��ܵ���(4)
	BCD4	ECQ1A0;					//��������ǰA��1�����޹��ܵ���(4)
	BCD4	ECQ2A0;					//��������ǰA��2�����޹��ܵ���(4)
	BCD4	ECQ3A0;					//��������ǰA��3�����޹��ܵ���(4)
	BCD4	ECQ4A0;					//��������ǰA��4�����޹��ܵ���(4)
	BCD4	ECPpB0;					//��������ǰB�������й��ܵ���(4)
	BCD4	ECPnB0;					//��������ǰB�෴���й��ܵ���(4)
	BCD4	ECQ1B0;					//��������ǰB��1�����޹��ܵ���(4)
	BCD4	ECQ2B0;					//��������ǰB��2�����޹��ܵ���(4)
	BCD4	ECQ3B0;					//��������ǰB��3�����޹��ܵ���(4)
	BCD4	ECQ4B0;					//��������ǰB��4�����޹��ܵ���(4)
	BCD4	ECPpC0;					//��������ǰC�������й��ܵ���(4)
	BCD4	ECPnC0;					//��������ǰC�෴���й��ܵ���(4)
	BCD4	ECQ1C0;					//��������ǰC��1�����޹��ܵ���(4)
	BCD4	ECQ2C0;					//��������ǰC��2�����޹��ܵ���(4)
	BCD4	ECQ3C0;					//��������ǰC��3�����޹��ܵ���(4)
	BCD4	ECQ4C0;					//��������ǰC��4�����޹��ܵ���(4)

}EC_CLR_REC;				//106�ֽ�


//���������¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//�������㷢��ʱ��(3)
	BCD3	HappenDate;				//�������㷢������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD3	MDPp0;					//��������ǰ�����й����������(3)
	BCD5	MDTPp0;					//��������ǰ�����й��������������ʱ��(5)
	BCD3	MDPn0;					//��������ǰ�����й����������(3)
	BCD5	MDTPn0;					//��������ǰ�����й��������������ʱ��(5)
	BCD3	MDQ10;					//��������ǰ1�����޹����������(3)
	BCD5	MDTQ10;					//��������ǰ1�����޹��������������ʱ��(5)
	BCD3	MDQ20;					//��������ǰ2�����޹����������(3)
	BCD5	MDTQ20;					//��������ǰ2�����޹��������������ʱ��(5)
	BCD3	MDQ30;					//��������ǰ3�����޹����������(3)
	BCD5	MDTQ30;					//��������ǰ3�����޹��������������ʱ��(5)
	BCD3	MDQ40;					//��������ǰ4�����޹����������(3)
	BCD5	MDTQ40;					//��������ǰ4�����޹��������������ʱ��(5)

	BCD3	MDPpA0;					//��������ǰA�������й����������(3)
	BCD5	MDTPpA0;				//��������ǰA�������й��������������ʱ��(5)
	BCD3	MDPnA0;					//��������ǰA�෴���й����������(3)
	BCD5	MDTPnA0;				//��������ǰA�෴���й��������������ʱ��(5)
	BCD3	MDQ1A0;					//��������ǰA��1�����޹����������(3)
	BCD5	MDTQ1A0;				//��������ǰA��1�����޹��������������ʱ��(5)
	BCD3	MDQ2A0;					//��������ǰA��2�����޹����������(3)
	BCD5	MDTQ2A0;				//��������ǰA��2�����޹��������������ʱ��(5)
	BCD3	MDQ3A0;					//��������ǰA��3�����޹����������(3)
	BCD5	MDTQ3A0;				//��������ǰA��3�����޹��������������ʱ��(5)
	BCD3	MDQ4A0;					//��������ǰA��4�����޹����������(3)
	BCD5	MDTQ4A0;				//��������ǰA��4�����޹��������������ʱ��(5)

	BCD3	MDPpB0;					//��������ǰB�������й����������(3)
	BCD5	MDTPpB0;				//��������ǰB�������й��������������ʱ��(5)
	BCD3	MDPnB0;					//��������ǰB�෴���й����������(3)
	BCD5	MDTPnB0;				//��������ǰB�෴���й��������������ʱ��(5)
	BCD3	MDQ1B0;					//��������ǰB��1�����޹����������(3)
	BCD5	MDTQ1B0;				//��������ǰB��1�����޹��������������ʱ��(5)
	BCD3	MDQ2B0;					//��������ǰB��2�����޹����������(3)
	BCD5	MDTQ2B0;				//��������ǰB��2�����޹��������������ʱ��(5)
	BCD3	MDQ3B0;					//��������ǰB��3�����޹����������(3)
	BCD5	MDTQ3B0;				//��������ǰB��3�����޹��������������ʱ��(5)
	BCD3	MDQ4B0;					//��������ǰB��4�����޹����������(3)
	BCD5	MDTQ4B0;				//��������ǰB��4�����޹��������������ʱ��(5)

	BCD3	MDPpC0;					//��������ǰC�������й����������(3)
	BCD5	MDTPpC0;				//��������ǰC�������й��������������ʱ��(5)
	BCD3	MDPnC0;					//��������ǰC�෴���й����������(3)
	BCD5	MDTPnC0;				//��������ǰC�෴���й��������������ʱ��(5)
	BCD3	MDQ1C0;					//��������ǰC��1�����޹����������(3)
	BCD5	MDTQ1C0;				//��������ǰC��1�����޹��������������ʱ��(5)
	BCD3	MDQ2C0;					//��������ǰC��2�����޹����������(3)
	BCD5	MDTQ2C0;				//��������ǰC��2�����޹��������������ʱ��(5)
	BCD3	MDQ3C0;					//��������ǰC��3�����޹����������(3)
	BCD5	MDTQ3C0;				//��������ǰC��3�����޹��������������ʱ��(5)
	BCD3	MDQ4C0;					//��������ǰC��4�����޹����������(3)
	BCD5	MDTQ4C0;				//��������ǰC��4�����޹��������������ʱ��(5)

}MD_CLR_REC;				//202�ֽ�


//�¼������¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//�¼����㷢��ʱ��(3)
	BCD3	HappenDate;				//�¼����㷢������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4	EventID;				//�¼��������ݱ�ʶ��1(4)
}EVENT_CLR_REC;			//14�ֽ�

//Уʱ�����¼�ṹ
typedef struct
{
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD3	BeforeTime;				//Уʱǰʱ��(3)
	BCD3	BeforeDate;				//Уʱǰ����(3)
	BCD3	AfterTime;				//Уʱ��ʱ��(3)
	BCD3	AfterDate;				//Уʱ������(3)

}TIME_REC;			//14�ֽ�


//ʱ�α��̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//ʱ�α��̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�α��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD3	Period1Tab1[14];		//ʱ�α���ǰ��һ�׵�1��ʱ�α�����(3*14)
	BCD3	Period1Tab2[14];		//ʱ�α���ǰ��һ�׵�2��ʱ�α�����(3*14)
	BCD3	Period1Tab3[14];		//ʱ�α���ǰ��һ�׵�3��ʱ�α�����(3*14)
	BCD3	Period1Tab4[14];		//ʱ�α���ǰ��һ�׵�4��ʱ�α�����(3*14)
	BCD3	Period1Tab5[14];		//ʱ�α���ǰ��һ�׵�5��ʱ�α�����(3*14)
	BCD3	Period1Tab6[14];		//ʱ�α���ǰ��һ�׵�6��ʱ�α�����(3*14)
	BCD3	Period1Tab7[14];		//ʱ�α���ǰ��һ�׵�7��ʱ�α�����(3*14)
	BCD3	Period1Tab8[14];		//ʱ�α���ǰ��һ�׵�8��ʱ�α�����(3*14)
	BCD3	Period2Tab1[14];		//ʱ�α���ǰ�ڶ��׵�1��ʱ�α�����(3*14)
	BCD3	Period2Tab2[14];		//ʱ�α���ǰ�ڶ��׵�2��ʱ�α�����(3*14)
	BCD3	Period2Tab3[14];		//ʱ�α���ǰ�ڶ��׵�3��ʱ�α�����(3*14)
	BCD3	Period2Tab4[14];		//ʱ�α���ǰ�ڶ��׵�4��ʱ�α�����(3*14)
	BCD3	Period2Tab5[14];		//ʱ�α���ǰ�ڶ��׵�5��ʱ�α�����(3*14)
	BCD3	Period2Tab6[14];		//ʱ�α���ǰ�ڶ��׵�6��ʱ�α�����(3*14)
	BCD3	Period2Tab7[14];		//ʱ�α���ǰ�ڶ��׵�7��ʱ�α�����(3*14)
	BCD3	Period2Tab8[14];		//ʱ�α���ǰ�ڶ��׵�8��ʱ�α�����(3*14)

}SEGTAB_PRG_REC;			//682�ֽ�


//ʱ�����̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//ʱ�����̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�����̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD3	Period1_Season1;		//ʱ������ǰ��һ�׵�1ʱ������(3)
	BCD3	Period1_Season2;		//ʱ������ǰ��һ�׵�2ʱ������(3)
	BCD3	Period1_Season3;		//ʱ������ǰ��һ�׵�3ʱ������(3)
	BCD3	Period1_Season4;		//ʱ������ǰ��һ�׵�4ʱ������(3)
	BCD3	Period1_Season5;		//ʱ������ǰ��һ�׵�5ʱ������(3)
	BCD3	Period1_Season6;		//ʱ������ǰ��һ�׵�6ʱ������(3)
	BCD3	Period1_Season7;		//ʱ������ǰ��һ�׵�7ʱ������(3)
	BCD3	Period1_Season8;		//ʱ������ǰ��һ�׵�8ʱ������(3)
	BCD3	Period1_Season9;		//ʱ������ǰ��һ�׵�9ʱ������(3)
	BCD3	Period1_Season10;		//ʱ������ǰ��һ�׵�10ʱ������(3)
	BCD3	Period1_Season11;		//ʱ������ǰ��һ�׵�11ʱ������(3)
	BCD3	Period1_Season12;		//ʱ������ǰ��һ�׵�12ʱ������(3)
	BCD3	Period1_Season13;		//ʱ������ǰ��һ�׵�13ʱ������(3)
	BCD3	Period1_Season14;		//ʱ������ǰ��һ�׵�14ʱ������(3)

	BCD3	Period2_Season1;		//ʱ������ǰ�ڶ��׵�1ʱ������(3)
	BCD3	Period2_Season2;		//ʱ������ǰ�ڶ��׵�2ʱ������(3)
	BCD3	Period2_Season3;		//ʱ������ǰ�ڶ��׵�3ʱ������(3)
	BCD3	Period2_Season4;		//ʱ������ǰ�ڶ��׵�4ʱ������(3)
	BCD3	Period2_Season5;		//ʱ������ǰ�ڶ��׵�5ʱ������(3)
	BCD3	Period2_Season6;		//ʱ������ǰ�ڶ��׵�6ʱ������(3)
	BCD3	Period2_Season7;		//ʱ������ǰ�ڶ��׵�7ʱ������(3)
	BCD3	Period2_Season8;		//ʱ������ǰ�ڶ��׵�8ʱ������(3)
	BCD3	Period2_Season9;		//ʱ������ǰ�ڶ��׵�9ʱ������(3)
	BCD3	Period2_Season10;		//ʱ������ǰ�ڶ��׵�10ʱ������(3)
	BCD3	Period2_Season11;		//ʱ������ǰ�ڶ��׵�11ʱ������(3)
	BCD3	Period2_Season12;		//ʱ������ǰ�ڶ��׵�12ʱ������(3)
	BCD3	Period2_Season13;		//ʱ������ǰ�ڶ��׵�13ʱ������(3)
	BCD3	Period2_Season14;		//ʱ������ǰ�ڶ��׵�14ʱ������(3)

}ZONE_PRG_REC;			//94�ֽ�

//�����ձ�̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//ʱ�α��̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�α��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	unsigned char Week_Code;		//�����ձ��ǰ���õ���ʱ�α��

}WEEK_PRG_REC;				//11�ֽ�

//�ڼ��ձ�̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//ʱ�α��̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�α��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4 	Holiday[254];			//�ڼ��ձ��ǰ��1~254�ڼ�������

}HOLIDAY_PRG_REC;			//1026�ֽ�

//��Ϸ�ʽ��̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//ʱ�α��̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�α��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	unsigned char 	Mode;			//��Ϸ�ʽ���ǰ����Ϸ�ʽ������

}ECMODE_PRG_REC;			//11�ֽ�

//�����ձ�̼�¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//ʱ�α��̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�α��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD2	SaveDay1;				//�����ձ��ǰÿ�µ�1����������(2)
	BCD2	SaveDay2;				//�����ձ��ǰÿ�µ�2����������(2)
	BCD2	SaveDay3;				//�����ձ��ǰÿ�µ�3����������(2)

}SAVEDAY_PRG_REC;			//16�ֽ�


//���˸Ǽ�¼�ṹ
typedef struct
{
	BCD3	OpenStaTime;			//����ʼʱ��(3)
	BCD3	OpenStaDate;			//����ʼ����(3)
	BCD3	OpenEndTime;			//�򿪽���ʱ��(3)
	BCD3	OpenEndDate;			//�򿪽�������(3)
	BCD4	BeforeECPp0;			//��ǰ�����й��ܵ���(4)
	BCD4	BeforeECPn0;			//��ǰ�����й��ܵ���(4)
	BCD4	BeforeECQ10;			//��ǰ1�����޹��ܵ���(4)
	BCD4	BeforeECQ20;			//��ǰ2�����޹��ܵ���(4)
	BCD4	BeforeECQ30;			//��ǰ3�����޹��ܵ���(4)
	BCD4	BeforeECQ40;			//��ǰ4�����޹��ܵ���(4)
	BCD4	AfterECPp0;				//���Ϻ������й��ܵ���(4)
	BCD4	AfterECPn0;				//���Ϻ����й��ܵ���(4)
	BCD4	AfterECQ10;				//���Ϻ�1�����޹��ܵ���(4)
	BCD4	AfterECQ20;				//���Ϻ�2�����޹��ܵ���(4)
	BCD4	AfterECQ30;				//���Ϻ�3�����޹��ܵ���(4)
	BCD4	AfterECQ40;				//���Ϻ�4�����޹��ܵ���(4)

}OPEN_REC;					//60�ֽ�

//�����¼�ṹ
typedef struct
{
	BCD5	HappenDate;				//��������(5)
	BCD2	AfterBuyCnt;			//������ܹ������(2) 
	BCD4	CurBuyEC;				//������(4)           
	BCD4	BeforeRemainEC;			//����ǰʣ�����(4)   
	BCD4	AfterRemainEC;			//�����ʣ�����(4)   
	BCD4	AfterBuyECSum;			//������ۼƹ�����(4) 												

}BUYEC_REC;					//23�ֽ�

//��բ��¼�ṹ
typedef struct
{
	BCD3	HappenTime;				//����ʱ��(3)
	BCD3	HappenDate;				//��������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4	ECPp0;					//����ʱ�й������ܵ���(4) 
	BCD4	ECPn0;					//����ʱ�й������ܵ���(4) 
	BCD4	ECQ10;					//����ʱ��һ�����޹��ܵ���(4)
	BCD4	ECQ20;					//����ʱ�ڶ������޹��ܵ���(4)
	BCD4	ECQ30;					//����ʱ���������޹��ܵ���(4)
	BCD4	ECQ40;					//����ʱ���������޹��ܵ���(4)

}TRIP_REC;					//34�ֽ�

/*
//�������س��޼�¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)

}PFT_REC;					//44�ֽ�
*/

//�������س��޼�¼�ṹ
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD3	EndTime;				//�쳣����ʱ��(3)
	BCD3	EndDate;				//�쳣��������(3)
	BCD4	EndECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	EndECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	EndECQn0;				//����ʱ������޹�2�ܵ���(4)

}PFT_REC;					//44�ֽ�


//��ȫ��֤��дͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
  	unsigned short 	FileNo;
  	unsigned short 	DataStAddr;
	unsigned char  	DisItems;	//�ڲ���ʾС����
	unsigned char  	DisPoint;   //С������ʾ��ʽ  	
}SAUCOMTAB;


//����ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
//	unsigned char	State1;
//	unsigned char	State2;
//	unsigned char	State3;	
//	unsigned char	State4;
//	unsigned char	State5;
//	unsigned char	State6;
}ECCOMTAB;

//����ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
//	unsigned char	State1;
//	unsigned char	State2;
//	unsigned char	State3;	
//	unsigned char	State4;
//	unsigned char	State5;
//	unsigned char	State6;
}MDCOMTAB;

//����ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
//	unsigned char	State1;
//	unsigned char	State2;
//	unsigned char	State3;	
//	unsigned char	State4;
//	unsigned char	State5;
//	unsigned char	State6;
}REALCOMTAB;

//�¼�ͨѶ���
//Addr:
//1)EEPROM��ַ
//2)�¼���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char  	DisItems;	//�ڲ���ʾС����
//	unsigned char  	DisPoint;   //С������ʾ��ʽ
	unsigned char  	TimeItems;  //����������ʱ������
}EVECOMTAB;

//�α���ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char  	DisItems;	//�ڲ���ʾС����
	unsigned char  	DisPoint;   //С������ʾ��ʽ
}PARACOMTAB;

//�α���ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
}MANUPARATAB;

//����ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;		//������������������ݿ��е���������ݿ��׵�ַ����Ե�ַ
  	unsigned short 	ComInfo;
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char  	DisItems;	//�ڲ���ʾС����
}DJCOMTAB;

#if ( DLT645_2007_14 ==	YesCheck )			//�¹���	//14.04.25
typedef struct						//�¹���	//14.05.04
{
	unsigned long ComID;			//���ɼ�¼��������
	unsigned char ChannelNo;		//��������
	unsigned char StartAddr;		//��ȡ�����ڱ�����¼����ʼ��ַ
	unsigned char Length;			//��ȡ���ݳ���
	unsigned char FillInMode;		//�����ڵļ�¼��䷽ʽ				//0����FF����0������һ���ݵ�����
}NEWLOADPARA;

typedef struct						//�¹���	//14.05.05
{
	unsigned short LoadRecInterval;	//���ɼ�¼���ʱ��
	BCD5	StartTime;				//������ʼʱ��
	BCD5	EndTime;				//���߽���ʱ��
	unsigned char ChannelNo;		//��������
	unsigned char StartAddr;		//��ȡ�����ڱ�����¼����ʼ��ַ
	unsigned char Length;			//��ȡ���ݳ���
	unsigned char LoadMode;			//����ģʽ��
}NEWSEARCHPARA;

#endif

//�Զ���ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
}EXTCOMTAB;

//�Զ����¼�ͨѶ���
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
	unsigned char  	State7;	
	unsigned char  	DisPoint;   //С������ʾ��ʽ
}ZDYEVECOMTAB;


//����ͨѶ���
typedef struct
{
	unsigned short 	ECAddr;		//�õ������ݵ���Դ��ַ
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
	unsigned char  	DisPoint;   //С������ʾ��ʽ
	unsigned char  	DSign;   	//��ʾ����
	unsigned char	Length;
	unsigned char  	StartAddr;	//��С�������ݼ�¼�ڲ���ʼ��ַ
}PHFAILDISTAB;

typedef struct
{
	unsigned short ID23;
	unsigned short EventNo;	
}IDEVENO;
/*
typedef struct
{
	unsigned char (*p)(void);
	unsigned short	EventNo;
	EVENT_STATE*	Event_State;
	unsigned char* Delay;
	unsigned short	EOldState;
	unsigned short	UpBitSite;	//�¹���
}TAMPER;	
*/
//��ʷ��¼�ṹ����
//��¼�����EEPROM�Ķ��壺
//EventNo���¼���¼��
//PtAddr:��ǰ��¼�׵�ַָ��(EEPROMֱ�ӵ�ַ)
//StorageRecs���Ѵ��¼�����������Ϊ�����ȡ���ܼ�¼�����������������ӡ���¼����ʱֻ�轫�����㼴��
//StartAddr:��¼��ʼ��ַ(EEPROMֱ�ӵ�ַ)
//EndAddr:��¼������ַ(EEPROMֱ�ӵ�ַ)
//��¼�����FLASH�Ķ��壺
//PtAddr:��ǰ��¼�׵�ַָ��(�ü�¼��ռ�õ�FLASH�ռ��ڵ���Ե�ַ)
//StorageRecs���Ѵ��¼�����������Ϊ�����ȡ���ܼ�¼�����������������ӡ���¼����ʱֻ�轫�����㼴��
//StartAddr:��¼��ʼ��ַ:FLASH��ʼҳ��												//(��ǰ�μ�¼����EEPROMֱ�ӵ�ַ)
//EndAddr:��¼������ַ(ռ��FLASHҳ��)												//(���ֽ�FLASH��ʼҳ�š����ֽڣ�ռ��FLASHҳ��)
//RecdCunt:�ܼ�¼��
//RecdLenth:ÿ����¼����
//InArrayAddr: ÿ����¼��ÿС�����ݳ��ȡ�
//Items: ÿ����¼��С������
//MemType: ��¼�洢���͡�		0��EEPROM,	0x55:FLASH
//Cunt: �ۼƴ�����		�޴�������ʱ��ֵΪ0xFFFF
//SumTime: �ۼ�ʱ�䡣	�޴�������ʱ��ֵΪ0xFFFF	

typedef struct
{
	unsigned short EventNo;
	unsigned short PtAddr;
	unsigned short StorageRecs;	
	unsigned short StartAddr;
	unsigned short EndAddr;
	unsigned short RecdCunt;
	unsigned short RecdLenth;	
	unsigned char* InArrayAddr;
	unsigned char Items;
	unsigned char MemType;
	unsigned short Cunt;
	unsigned short SumTime;	
} PTPARA;	

typedef struct
{

	unsigned long 	Comm0FirstLoadNo1;		//����0��1�ฺ�������¼��
	unsigned long 	Comm0FirstLoadNo2;		//����0��2�ฺ�������¼��
	unsigned long 	Comm0FirstLoadNo3;		//����0��3�ฺ�������¼��
	unsigned long 	Comm0FirstLoadNo4;		//����0��4�ฺ�������¼��
	unsigned long 	Comm0FirstLoadNo5;		//����0��5�ฺ�������¼��
	unsigned long 	Comm0FirstLoadNo6;		//����0��6�ฺ�������¼��
                                          
	unsigned long 	Comm1FirstLoadNo1;		//����1��1�ฺ�������¼��
	unsigned long 	Comm1FirstLoadNo2;		//����1��2�ฺ�������¼��
	unsigned long 	Comm1FirstLoadNo3;		//����1��3�ฺ�������¼��
	unsigned long 	Comm1FirstLoadNo4;		//����1��4�ฺ�������¼��
	unsigned long 	Comm1FirstLoadNo5;		//����1��5�ฺ�������¼��
	unsigned long 	Comm1FirstLoadNo6;		//����1��6�ฺ�������¼��
                                          
	unsigned long 	Comm2FirstLoadNo1;		//����2��1�ฺ�������¼��
	unsigned long 	Comm2FirstLoadNo2;		//����2��2�ฺ�������¼��
	unsigned long 	Comm2FirstLoadNo3;		//����2��3�ฺ�������¼��
	unsigned long 	Comm2FirstLoadNo4;		//����2��4�ฺ�������¼��
	unsigned long 	Comm2FirstLoadNo5;		//����2��5�ฺ�������¼��
	unsigned long 	Comm2FirstLoadNo6;		//����2��6�ฺ�������¼��
                                          
	unsigned long 	Comm3FirstLoadNo1;		//����3��1�ฺ�������¼��
	unsigned long 	Comm3FirstLoadNo2;		//����3��2�ฺ�������¼��
	unsigned long 	Comm3FirstLoadNo3;		//����3��3�ฺ�������¼��
	unsigned long 	Comm3FirstLoadNo4;		//����3��4�ฺ�������¼��
	unsigned long 	Comm3FirstLoadNo5;		//����3��5�ฺ�������¼��
	unsigned long 	Comm3FirstLoadNo6;		//����3��6�ฺ�������¼��

//	BCD5		 	Comm0Load1STime;		//����0��1�ฺ�������¼ʱ��
//	BCD5		 	Comm0Load2STime;		//����0��2�ฺ�������¼ʱ��
//	BCD5		 	Comm0Load3STime;		//����0��3�ฺ�������¼ʱ��
//	BCD5		 	Comm0Load4STime;		//����0��4�ฺ�������¼ʱ��
//	BCD5		 	Comm0Load5STime;		//����0��5�ฺ�������¼ʱ��
//	BCD5		 	Comm0Load6STime;		//����0��6�ฺ�������¼ʱ��

//	unsigned char	Comm0Load1Num;			//����0��1�ฺ��ʣ���¼��
//	unsigned char	Comm0Load2Num;			//����0��2�ฺ��ʣ���¼��
//	unsigned char	Comm0Load3Num;			//����0��3�ฺ��ʣ���¼��
//	unsigned char	Comm0Load4Num;			//����0��4�ฺ��ʣ���¼��
//	unsigned char	Comm0Load5Num;			//����0��5�ฺ��ʣ���¼��
//	unsigned char	Comm0Load6Num;			//����0��6�ฺ��ʣ���¼��

//	BCD5		 	Comm1Load1STime;		//����1��1�ฺ�������¼ʱ��
//	BCD5		 	Comm1Load2STime;		//����1��2�ฺ�������¼ʱ��
//	BCD5		 	Comm1Load3STime;		//����1��3�ฺ�������¼ʱ��
//	BCD5		 	Comm1Load4STime;		//����1��4�ฺ�������¼ʱ��
//	BCD5		 	Comm1Load5STime;		//����1��5�ฺ�������¼ʱ��
//	BCD5		 	Comm1Load6STime;		//����1��6�ฺ�������¼ʱ��

//	unsigned char	Comm1Load1Num;			//����1��1�ฺ��ʣ���¼��
//	unsigned char	Comm1Load2Num;			//����1��2�ฺ��ʣ���¼��
//	unsigned char	Comm1Load3Num;			//����1��3�ฺ��ʣ���¼��
//	unsigned char	Comm1Load4Num;			//����1��4�ฺ��ʣ���¼��
//	unsigned char	Comm1Load5Num;			//����1��5�ฺ��ʣ���¼��
//	unsigned char	Comm1Load6Num;			//����1��6�ฺ��ʣ���¼��

//	BCD5		 	Comm2Load1STime;		//����2��1�ฺ�������¼ʱ��
//	BCD5		 	Comm2Load2STime;		//����2��2�ฺ�������¼ʱ��
//	BCD5		 	Comm2Load3STime;		//����2��3�ฺ�������¼ʱ��
//	BCD5		 	Comm2Load4STime;		//����2��4�ฺ�������¼ʱ��
//	BCD5		 	Comm2Load5STime;		//����2��5�ฺ�������¼ʱ��
//	BCD5		 	Comm2Load6STime;		//����2��6�ฺ�������¼ʱ��

//	unsigned char	Comm2Load1Num;			//����2��1�ฺ��ʣ���¼��
//	unsigned char	Comm2Load2Num;			//����2��2�ฺ��ʣ���¼��
//	unsigned char	Comm2Load3Num;			//����2��3�ฺ��ʣ���¼��
//	unsigned char	Comm2Load4Num;			//����2��4�ฺ��ʣ���¼��
//	unsigned char	Comm2Load5Num;			//����2��5�ฺ��ʣ���¼��
//	unsigned char	Comm2Load6Num;			//����2��6�ฺ��ʣ���¼��

//	BCD5		 	Comm3Load1STime;		//����3��1�ฺ�������¼ʱ��
//	BCD5		 	Comm3Load2STime;		//����3��2�ฺ�������¼ʱ��
//	BCD5		 	Comm3Load3STime;		//����3��3�ฺ�������¼ʱ��
//	BCD5		 	Comm3Load4STime;		//����3��4�ฺ�������¼ʱ��
//	BCD5		 	Comm3Load5STime;		//����3��5�ฺ�������¼ʱ��
//	BCD5		 	Comm3Load6STime;		//����3��6�ฺ�������¼ʱ��

//	unsigned char	Comm3Load1Num;			//����3��1�ฺ��ʣ���¼��
//	unsigned char	Comm3Load2Num;			//����3��2�ฺ��ʣ���¼��
//	unsigned char	Comm3Load3Num;			//����3��3�ฺ��ʣ���¼��
//	unsigned char	Comm3Load4Num;			//����3��4�ฺ��ʣ���¼��
//	unsigned char	Comm3Load5Num;			//����3��5�ฺ��ʣ���¼��
//	unsigned char	Comm3Load6Num;			//����3��6�ฺ��ʣ���¼��

	BCD4			Comm0SubsFrameID[4];	//����0����֡ͨѶID��
	unsigned char 	Comm0LoadNum;			//����0����֡ʣ���¼��		���Ǹ�������ʱ��ָ��֡�����
//	BCD5		 	Comm0LoadSTime;			//����0����֡��ʼʱ��
	unsigned char 	Comm0FrameNo;			//����0����֡���

	BCD4			Comm1SubsFrameID[4];	//����1����֡ͨѶID��
	unsigned char 	Comm1LoadNum;			//����1����֡ʣ���¼��		���Ǹ�������ʱ��ָ��֡�����
//	BCD5		 	Comm1LoadSTime;			//����1����֡��ʼʱ��
	unsigned char 	Comm1FrameNo;			//����1����֡���
	
	BCD4			Comm2SubsFrameID[4];	//����2����֡ͨѶID��
	unsigned char 	Comm2LoadNum;			//����2����֡ʣ���¼��		���Ǹ�������ʱ��ָ��֡�����
//	BCD5		 	Comm2LoadSTime;			//����2����֡��ʼʱ��
	unsigned char 	Comm2FrameNo;			//����2����֡���
	
	BCD4			Comm3SubsFrameID[4];	//����3����֡ͨѶID��
	unsigned char 	Comm3LoadNum;			//����3����֡ʣ���¼��		���Ǹ�������ʱ��ָ��֡�����
//	BCD5		 	Comm3LoadSTime;			//����3����֡��ʼʱ��
	unsigned char 	Comm3FrameNo;			//����3����֡���
	
}LOADPARA;					//�������߲��� 		

typedef struct
{
	unsigned long*	FirstLoadNo;	// �����¼��
//	unsigned char*	STime;			// �����¼ʱ��
//	unsigned char*	Sum;			// ʣ���¼��
	unsigned char*	SubsPara;		// ����֡����
	
}COMMLOADPARA;

//typedef struct
//{
//	unsigned short CurRecNo;		//��ǰ��¼��
//   unsigned short AvailRecs;		//��ǰ�Ѵ洢����Ч��¼��
//	unsigned short MaxCunt;			//�洢�ռ�����¼��
//	unsigned short RecdCunt;		//�洢�ռ������Ч��¼����ȥ��4K��ת�غ�
//	unsigned short  StartPageNo;	//�洢�ռ���ʼҳ��
//	unsigned short  Pages;			//�洢�ռ���ռ��ҳ��
//	unsigned short  RecsInPage;		//ÿҳ��¼��
//	unsigned short RecdLenth;		//��¼����
//	unsigned char ChannelNo;		//ͨ����
//}CURVEPARA;

typedef struct
{
	unsigned short CurRecNo;			//��ǰ��¼��
        unsigned short AvailRecs;		//��ǰ�Ѵ洢����Ч��¼��
	unsigned short MaxCunt;			//�洢�ռ�����¼��
	unsigned short RecdCunt;			//�洢�ռ������Ч��¼����ȥ��4K��ת�غ�
	unsigned short  StartPageNo;		//�洢�ռ���ʼҳ��
	unsigned short  Pages;			//�洢�ռ���ռ��ҳ��
	unsigned short  RecsInPage;		//ÿҳ��¼��
	unsigned short  RecdLenth;		//��¼����
	unsigned short  ChannelNo;		//ͨ����
}CURVEPARA;

//typedef struct						//cru
//{
//	unsigned char Buff1[256];
//	unsigned char Buff2[256];
//}FLASHBUFF;

typedef struct						//cru
{
	unsigned char Buff[128];
}FBUFF;

#if ( FeeECIndependence	== YesCheck )		//11.01.15	���ʵ�����������
typedef struct
{
	unsigned short* ECRamBufAds;
	unsigned char* ECRamBufChkAds;
}FeeECRg;
#endif

//���ʵ�۱�̼�¼�ṹ				//�¹���	
typedef struct
{
	BCD3	HappenTime;				//ʱ�α��̷���ʱ��(3)
	BCD3	HappenDate;				//ʱ�α��̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4	ActiveFee1Price;		//��ǰ�׷���1����(4)
	BCD4	ActiveFee2Price;		//��ǰ�׷���2����(4)
	BCD4	ActiveFee3Price;		//��ǰ�׷���3����(4)
	BCD4	ActiveFee4Price;		//��ǰ�׷���4����(4)
	BCD4	PassiveFee1Price;		//�����׷���1����(4)
	BCD4	PassiveFee2Price;		//�����׷���2����(4)
	BCD4	PassiveFee3Price;		//�����׷���3����(4)
	BCD4	PassiveFee4Price;		//�����׷���4����(4)

}FEEPRICE_PRG_REC;					//42�ֽ�

//���ݲ�����̼�¼�ṹ				//�¹���
typedef struct
{
	BCD3	HappenTime;				//���ݲ�����̷���ʱ��(3)
	BCD3	HappenDate;				//���ݲ�����̷�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	BCD4	ActiveStep1EC;			//��ǰ�׵�1����ֵ(4)
	BCD4	ActiveStep2EC;			//��ǰ�׵�2����ֵ(4)
	BCD4	ActiveStep3EC;			//��ǰ�׵�3����ֵ(4)
	BCD4	ActiveStep4EC;			//��ǰ�׵�4����ֵ(4)
	BCD4	ActiveStep5EC;			//��ǰ�׵�5����ֵ(4)
	BCD4	ActiveStep6EC;			//��ǰ�׵�6����ֵ(4)
	BCD4	ActiveStep1Price;		//��ǰ�׵�1���ݵ��(4)
	BCD4	ActiveStep2Price;		//��ǰ�׵�2���ݵ��(4)
	BCD4	ActiveStep3Price;		//��ǰ�׵�3���ݵ��(4)
	BCD4	ActiveStep4Price;		//��ǰ�׵�4���ݵ��(4)
	BCD4	ActiveStep5Price;		//��ǰ�׵�5���ݵ��(4)
	BCD4	ActiveStep6Price;		//��ǰ�׵�6���ݵ��(4)
	BCD4	ActiveStep7Price;		//��ǰ�׵�7���ݵ��(4)
	BCD3	ActiveYearSaveDay1;		//��ǰ���1������(3)
	BCD3	ActiveYearSaveDay2;		//��ǰ���2������(3)
	BCD3	ActiveYearSaveDay3;		//��ǰ���3������(3)
	BCD3	ActiveYearSaveDay4;		//��ǰ���4������(3)
	BCD4	PassiveStep1EC;			//�����׵�1����ֵ(4)
	BCD4	PassiveStep2EC;			//�����׵�2����ֵ(4)
	BCD4	PassiveStep3EC;			//�����׵�3����ֵ(4)
	BCD4	PassiveStep4EC;			//�����׵�4����ֵ(4)
	BCD4	PassiveStep5EC;			//�����׵�5����ֵ(4)
	BCD4	PassiveStep6EC;			//�����׵�6����ֵ(4)
	BCD4	PassiveStep1Price;		//�����׵�1���ݵ��(4)
	BCD4	PassiveStep2Price;		//�����׵�2���ݵ��(4)
	BCD4	PassiveStep3Price;		//�����׵�3���ݵ��(4)
	BCD4	PassiveStep4Price;		//�����׵�4���ݵ��(4)
	BCD4	PassiveStep5Price;		//�����׵�5���ݵ��(4)
	BCD4	PassiveStep6Price;		//�����׵�6���ݵ��(4)
	BCD4	PassiveStep7Price;		//�����׵�7���ݵ��(4)
	BCD3	PassiveYearSaveDay1;	//�������1������(3)
	BCD3	PassiveYearSaveDay2;	//�������2������(3)
	BCD3	PassiveYearSaveDay3;	//�������3������(3)
	BCD3	PassiveYearSaveDay4;	//�������4������(3)

}STEPPARA_PRG_REC;					//138�ֽ�


//������¼�ṹ					//�¹���
typedef struct
{
	BCD4	HappenDate;				//�����ʱ��(3)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	
}YEARSAVETIME_REC;					//8�ֽ�

//�½����¼�ṹ					//�¹���
typedef struct
{
	BCD4	HappenDate;				//�½���ʱ��(3)
	
}MONTHSAVETIME_REC;					//4�ֽ�

//��Կ���¼�¼�ṹ					//�¹���
typedef struct
{
	BCD3	HappenTime;				//��Կ���·���ʱ��(3)
	BCD3	HappenDate;				//��Կ���·�������(3)
	BCD4	OperatorCode;			//�����ߴ���(4)
	unsigned char Cunt;				//��Կ���µ�������(1)
	BCD4	KeyState;				//����ǰ����Կ״̬��(4)

}SECRETKEY_PRG_REC;					//15�ֽ�

//�쳣�忨��¼�ṹ					//�¹���
typedef struct
{
	BCD3	HappenTime;				//��Կ���·���ʱ��(3)
	BCD3	HappenDate;				//��Կ���·�������(3)
	BCD8	CardNo;					//CPU�����к�(8)	
	unsigned char ErrorByte;		//������Ϣ��(1)
	BCD5	CommandHead;			//��������ͷ(5)
	BCD2	ErrorResponse;			//������Ӧ״̬(2)
	BCD2	RBuyECCnt;				//�忨ʱ�ܹ������(2)
	BCD4	RemainEC;				//�忨ʱʣ����(4)
	BCD4	ECPp0;					//�忨ʱ�����й��ܵ���(4)
	BCD4	ECPn0;					//�忨ʱ�����й��ܵ���(4)
	
}CARDINFAULT_REC;					//36�ֽ�

//�˷Ѽ�¼�ṹ						//�¹���
typedef struct
{
	BCD3	HappenTime;				//�˷ѷ���ʱ��(3)
	BCD3	HappenDate;				//�˷ѷ�������(3)
	BCD2	RBuyECCnt;				//�˷�ǰ�������(2)
	BCD4	FeeBackSum;				//�˷ѽ��(4)
	BCD4	BeforeRemainEC;			//�˷�ǰʣ����(4)
	BCD4	AfterRemainEC;			//�˷Ѻ�ʣ����(4)

}FEEBACK_REC;						//20�ֽ�

//�㶨�ų����ż�¼�ṹ				//�¹���
typedef struct
{
	BCD3	StaTime;				//����ʱ��(3)
	BCD3	StaDate;				//��������(3)
	BCD3	EndTime;				//����ʱ��(3)
	BCD3	EndDate;				//��������(3)
	BCD4	StaECPp0;				//����ǰ�����й��ܵ���(4)
	BCD4	StaECPn0;				//����ǰ�����й��ܵ���(4)
	BCD4	EndECPp0;				//�����������й��ܵ���(4)
	BCD4	EndECPn0;				//���������й��ܵ���(4)
	
}MAGNETDISTURB_REC;					//36�ֽ�

//���ɿ���������¼�ṹ			//�¹���
typedef struct
{
	BCD3	StaTime;				//����ʱ��(3)
	BCD3	StaDate;				//��������(3)
	BCD3	EndTime;				//����ʱ��(3)
	BCD3	EndDate;				//��������(3)
	unsigned char EndState;			//����״̬(1)
	BCD4	StaECPp0;				//����ǰ�����й��ܵ���(4)
	BCD4	StaECPn0;				//����ǰ�����й��ܵ���(4)
	BCD4	EndECPp0;				//�����������й��ܵ���(4)
	BCD4	EndECPn0;				//���������й��ܵ���(4)
	
}RELAYFAULT_REC;					//29�ֽ�

//��Դ�쳣��¼�ṹ					//�¹���
typedef struct
{
	BCD3	StaTime;				//����ʱ��(3)
	BCD3	StaDate;				//��������(3)
	BCD3	EndTime;				//����ʱ��(3)
	BCD3	EndDate;				//��������(3)
	BCD4	ECPp0;					//����ʱ�����й��ܵ���(4)
	BCD4	ECPn0;					//����ʱ�����й��ܵ���(4)
	
}POWERFAULT_REC;					//20�ֽ�

//�ܳ��������¼�ṹ				//�¹���
typedef struct
{
	BCD3	StaTime;				//�쳣��ʼʱ��(3)
	BCD3	StaDate;				//�쳣��ʼ����(3)
	unsigned char StaDir;			//����ʱ�����й����ʷ���(1)
	BCD4	StaECPp0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECPn0;				//����ʱ���й������ܵ���(4)
	BCD4	StaECQp0;				//����ʱ������޹�1�ܵ���(4)
	BCD4	StaECQn0;				//����ʱ������޹�2�ܵ���(4)
	BCD4	StaECPpA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECPnA;				//����ʱ��A���й������ܵ���(4)
	BCD4	StaECQpA;				//����ʱ��A������޹�1�ܵ���(4)
	BCD4	StaECQnA;				//����ʱ��A������޹�2�ܵ���(4)
	BCD4	StaECPpB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECPnB;				//����ʱ��B���й������ܵ���(4)
	BCD4	StaECQpB;				//����ʱ��B������޹�1�ܵ���(4)
	BCD4	StaECQnB;				//����ʱ��B������޹�2�ܵ���(4)
	BCD4	StaECPpC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECPnC;				//����ʱ��C���й������ܵ���(4)
	BCD4	StaECQpC;				//����ʱ��C������޹�1�ܵ���(4)
	BCD4	StaECQnC;				//����ʱ��C������޹�2�ܵ���(4)

}TIDESWITCH_REC;				//71�ֽ�

#if( ECDisDecDigitsValid ==	YesCheck )		//13.03.03 ��ǰ������ʾС��λ��ʵ��Ч
typedef struct
{
	unsigned char* ECRAds;
	unsigned short* ECRamBufAds;
	unsigned char* ECRamBufChkAds;
} ECDisDigitRg;
#endif

//��������						//�¹���
typedef struct
{
	unsigned long	RBaseCurrent;			//��������(4)					//HEX	0.0001A				//�¹���		
	unsigned long	RMaxCurrent;			//������(4)                   //HEX 	0.0001A             //�¹���
	unsigned long	RMeterConst;			//����й�����(3)               //HEX                       //�¹���
	unsigned long	LongReserve1;			//����1		
	unsigned long	LongReserve2;			//����2
	
	unsigned short	RRatedVolt;				//���ѹ(2)					//HEX	0.1V				//�¹���		
	unsigned short	RMDPS32;				//��������ϵ��(2)               //HEX                       //�¹���
	unsigned short	R7022C_HFConst;			//7022C��Ƶ���峣��(2)          //HEX                       //�¹���
	unsigned short	R7022E_HFConst;			//7022E��Ƶ���峣��(2)          //HEX                       //�¹���
	unsigned short	R7022E_PStartup;		//7022E����������ֵ(2)          //HEX                       //�¹���
	unsigned short	R7022E_IStart;			//7022E����������ֵ(2)          //HEX                       //�¹���
	unsigned short	R90E32_PStartup;		//90E32AS����������ֵ(2)        //HEX                       //�¹���
	unsigned short	R90E32_PCreeping;		//90E32AS����Ǳ����ֵ(2)        //HEX                       //�¹���
	unsigned short	RPW00002Ib;				//��С������ֵ(2)               //HEX 	0.001W              //�¹���
	unsigned short	RDisA0001Ib;			//��С��ʾ������ֵ(2)           //HEX 0.0001A               //�¹���	//13.10.23

	unsigned short	IntReserve1;			//����1		
	unsigned short	IntReserve2;			//����2
	unsigned short	IntReserve3;			//����3		
	unsigned short	IntReserve4;			//����4
	
	unsigned char	R90E32_AmperMult;		//90E32AS�������ʷŴ���(1)    //HEX ���λ��1��ʾ��С(��) //�¹���

	unsigned char	CharReserve1;			//����1		
	unsigned char	CharReserve2;			//����2
	
}METERSPEC;

typedef struct						//�¹���	//13.09.16
{
	unsigned long ComID;
	unsigned short Addr;
	unsigned char* RAds;
	unsigned char DataLen;
	unsigned char FlashAddr;	
}METERPARA;

#if ( AdjustErrorCheck == YesCheck )	//15.09.05
//VoltAmperFtFreqState;			//��ѹ������������Ƶ��״̬
#define Un115Flag	0x0001		//��ѹ����115%Un״̬
#define Un110Flag	0x0002		//��ѹ����110%Un״̬
#define Un100Flag	0x0004		//��ѹ����100%Un״̬
#define Un90Flag	0x0008		//��ѹ����90%Un״̬
#define Un80Flag	0x0010		//��ѹ����80%Un״̬
#define ImaxFlag	0x0020		//��������Imax״̬
#define Ib100Flag	0x0040		//��������100%Ib״̬
#define Ib10Flag	0x0080		//��������10%Ib״̬
#define Ib05Flag	0x0100		//��������5%Ib״̬
#define Ib02Flag	0x0200		//��������2%Ib״̬
#define Ft1_0Flag	0x0400		//������������1.0״̬
#define Ft0_5Flag	0x0800		//������������0.5״̬
#define F51HzFlag	0x1000		//Ƶ�ʴ���51Hz״̬
#define F50HzFlag	0x2000		//Ƶ�ʴ���50Hz״̬
#define F49HzFlag	0x4000		//Ƶ�ʴ���49Hz״̬

#define F49Hz1_0	F49HzFlag+Un100Flag+Ft1_0Flag
#define F49Hz0_5L	F49HzFlag+Un100Flag+Ft0_5Flag
#define F51Hz1_0	F51HzFlag+Un100Flag+Ft1_0Flag
#define F51Hz0_5L	F51HzFlag+Un100Flag+Ft0_5Flag

#define Un115_1_0	F50HzFlag+Un115Flag+Ft1_0Flag
#define Un110_1_0	F50HzFlag+Un110Flag+Ft1_0Flag
#define Un110_0_5L	F50HzFlag+Un110Flag+Ft0_5Flag
#define Un90_1_0	F50HzFlag+Un90Flag+Ft1_0Flag
#define Un90_0_5L	F50HzFlag+Un90Flag+Ft0_5Flag
#define Un80_1_0	F50HzFlag+Un80Flag+Ft1_0Flag

#define SelfHeat	F50HzFlag+Un100Flag+ImaxFlag

//������
typedef struct
{
//	unsigned short	RSpeCalibPermit;			//0x1234:��������У��
//	unsigned short 	RSpeCalibState;				//0x4567:��λ7022����������У��״̬
	unsigned short 	VoltAmperFtFreqState;		//��ѹ������������Ƶ��״̬

//	unsigned short 	SelfHeatEnterDelay;			//����ģʽ������ʱ	��
	
	unsigned char 	F49HzImax1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F49HzIb1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F49Hz05Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F49Hz02Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F49HzImax0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	F49HzIb0_5L_Delay;			//����״̬��3����ʱ
	unsigned char 	F49Hz10Ib0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	F49Hz05Ib0_5L_Delay;		//����״̬��3����ʱ
	
	unsigned char 	F51HzImax1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F51HzIb1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F51Hz05Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F51Hz02Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	F51HzImax0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	F51HzIb0_5L_Delay;			//����״̬��3����ʱ
	unsigned char 	F51Hz10Ib0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	F51Hz05Ib0_5L_Delay;		//����״̬��3����ʱ

	unsigned char 	Un115Imax1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un115Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un115_05Ib1_0_Delay;		//����״̬��3����ʱ
	unsigned char 	Un115_02Ib1_0_Delay;		//����״̬��3����ʱ
	
	unsigned char 	Un110Imax1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un110Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un110_05Ib1_0_Delay;		//����״̬��3����ʱ
	unsigned char 	Un110_02Ib1_0_Delay;		//����״̬��3����ʱ
	unsigned char 	Un110Imax0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	Un110Ib0_5L_Delay;			//����״̬��3����ʱ
	unsigned char 	Un110_10Ib0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	Un110_05Ib0_5L_Delay;		//����״̬��3����ʱ

	unsigned char 	Un90Imax1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un90Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un90_05Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un90_02Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un90Imax0_5L_Delay;			//����״̬��3����ʱ
	unsigned char 	Un90Ib0_5L_Delay;			//����״̬��3����ʱ
	unsigned char 	Un90_10Ib0_5L_Delay;		//����״̬��3����ʱ
	unsigned char 	Un90_05Ib0_5L_Delay;		//����״̬��3����ʱ

	unsigned char 	Un80Imax1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un80Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un80_05Ib1_0_Delay;			//����״̬��3����ʱ
	unsigned char 	Un80_02Ib1_0_Delay;			//����״̬��3����ʱ

	unsigned char 	SelfHeat_Delay;				//����״̬��240����ʱ

	unsigned char	RErrorMode;					//0:���� 
	
	unsigned char	RSpeAdjustPermit;			//�������㲹������(2)		����0x67;����������
	
}ERRORRAM;

//���ת�����
typedef struct
{
	unsigned short 	ComID;
	unsigned short 	Addr;		//�������������EEPROM�е�ַ
	unsigned char*  StateDelay;
  	unsigned short 	ComInfo;
	unsigned short	ComID1;
	unsigned short	ModeState;
	unsigned char	Mode;
	unsigned char	Resve;	
}ERRORCOMTAB;

#endif

/*
typedef struct
{
	unsigned char*	Buf;			// ͨѶ������
	unsigned char*	Mode;			// ͨѶģʽ
  	unsigned short*	Ptr;			// ������ָ��
  	unsigned short*	BTime;			// �ַ��䶨ʱ
  	unsigned char*	SendDTime;		// ������ʱ
  	unsigned char*	RecTmr;			// ͨѶ��ʱ
  	unsigned char*	FESum;			// ͨѶFE����
	unsigned char* 	PrgTmr;			// ��̼�¼��ʱ
	unsigned char*	IECState;		// ͨѶ��IECͨѶ����״̬
	unsigned char*	MACState;		// ͨѶ��MACͨѶ����״̬
	unsigned char*	LLCState;		// ͨѶ��MACͨѶ����״̬
	unsigned char*	TX_Delay;		// ������ʱ
	unsigned char*	Run;			// 485�����б�־
//	unsigned char* 	BandRate;		// 485�ڲ�����
	unsigned char* 	RComDCB;		// 485�ڲ����ʵȲ���		[0]:�����ʡ�[1]:У��λ��[2]:����λ��[3]:ֹͣλ��[4]:����
	unsigned char* 	FTime;			// ֡�䶨ʱ
	unsigned short* RxdFrameNo;		// ����֡������
	unsigned short* TxdFrameNo;		// ����֡������
	unsigned char* 	LastRecCommand;	// ����յ�������
	unsigned short*	MaxInfoSize;	// �շ�֡���ߴ�

	unsigned char* 	ClientAddr;		// �ͻ�����ַ						//17.04.11
	unsigned long* 	ConnectValidTime;	//����Ӧ��������Чʱ��			//17.04.11
	unsigned long* 	ConnectValidDelay;	//����Ӧ��������Чʱ����ʱ
	unsigned short* BlockNumber;	// ����GET��ͨѶ���
	unsigned char*	APPLState;		// ͨѶ��APPL����״̬
	unsigned short* HBTime;			// 485���ַ��䶨ʱ
  	unsigned short* RecDataPtr;		// 485�ڻ�����ָ��
  	unsigned short* SendDataPtr;	// 485�ڻ�����ָ��
  	unsigned short* SendByteSum;	// 485��LLC��������
  	unsigned char* 	RecDataBuf;		// 485�ڽ���ͨ�Ż���		
  	unsigned char* 	SendDataBuf;	// 485�ڷ���ͨ�Ż���		
  	unsigned char* 	HMode;			// 485��ͨѶģʽ
  	unsigned char*	HState;			// 485��ͨ��״̬
//	unsigned char* 	HBandRate;		// 485�ڲ�����
	unsigned char* 	HRecTmr;		// ���ռ�¼��ʱ
	unsigned char* 	HPrgTmr;		// ��̼�¼��ʱ
	unsigned char*	Conformance;	// ͨѶ��Ӧ�����ӵ�һ���Կ����
	unsigned char*	FunConformance;	// ͨѶ��Ӧ�����ӵĹ���һ���Կ����	//17.04.11
	unsigned char*	SubsBlockPara;	// ���ں���֡�����
//	unsigned char*	SetBlockBuff;	// �����û�����ַ
//	unsigned short*	SetBlockPtr;	// �����û���ָ��
  	unsigned short*	MaxApduSize;	// �������APDU�ߴ�

	unsigned char* SecurityState;		// ͨѶ�ڰ�ȫ״̬		0x00: �ǰ�ȫ��֤���� 0x01������ 0x02:����	
	unsigned char* SecurityVerifyType;	// ͨѶ�ڰ�ȫ��֤����	0��SIG_MAC 1��RN 2��RN_MAC 3��SID
	unsigned char* TimeTag;				// ͨѶ��ʱ���ǩ	optional:1�ֽڣ�data_time_s:7�ֽڣ�TI:3�ֽ�
	
}DL698COMMPARA;
*/

////IECState						//ͨѶ��1IECͨѶ����״̬
//#define IECIdle				0	// IECͨѶ����
//#define IECStartCode		1	// IECͨѶ��ʼ
//#define IECStartCodeOver	2	// IECͨѶ��ʼ֡����
//#define IECBandRate			3	// IECͨѶ�л�������
//#define IECBandRateOver		4	// IECͨѶ�л�������֡����
//#define IECModeC			5	// IECͨѶģʽC
//#define IECModeCOver		6	// IECͨѶģʽC֡����
//#define IECModeCRXOver		7	// IECͨѶģʽC֡����	
//#define IECModeHDLC			8	// IECͨѶģʽE,HDLC
//#define IECModeHDLCOver		9	// IECͨѶģʽE,HDLC֡����
//#define IECModeHDLCRXOver	10	// IECͨѶģʽE,HDLC֡����
//#define IECModeExit			11	// IECͨѶ�˳�
/*
// 	IECState					// ModbusͨѶ��Լ�������״̬
#define IECIdle				0	// ͨѶ����
#define IECBufBusy			1	// ��ʼ�����ݣ�ͨѶ����æ
#define IECRead				2	// ��������
#define IECDataSend1Wait	3	// ���ݰ�1���͵ȴ�
#define IECDataSend2		4	// ���ݰ�2����
#define IECDataSend2Wait	5	// ���ݰ�2���͵ȴ�
#define IECDataSend3		6	// ���ݰ�3����
#define IECDataSend3Wait	7	// ���ݰ�3���͵ȴ�
#define IECDataSend4		8	// ���ݰ�4����
#define IECDataSend4Wait	9	// ���ݰ�4���͵ȴ�
#define IECExit				10	// ͨѶ�˳�

//MACState						//ͨѶ��1MACͨѶ����״̬
#define	NDMState			0	// MACδ����
#define	NRMState			1	// MAC������		

//LLCState						// ͨѶ��1MACͨѶ����״̬
#define	LLCIdle				0	// LLC����
#define	WaitRecNextState	1	// LLC�ȴ�
#define	SendNextState		2	// ����δ��
#define	UISendNextState		3	// UI֡����δ��
*/
typedef struct
{
  unsigned short	CommIntReserve1;	//�������Ϳռ�1
  unsigned short	CommIntReserve2;	//�������Ϳռ�2
  unsigned short	CommIntReserve3;	//�������Ϳռ�3

  unsigned short   BatOnDly;			// ͣ����ع���ʱ����ʱ
  unsigned short 	RecCount;			// �˿ڽ���������			

  unsigned short 	Frame_BTime0;		// 485��1֡���ַ��䶨ʱ 1/2ms
  unsigned short 	Frame_BTime1;		// 485��2֡���ַ��䶨ʱ 1/2ms
  unsigned short 	Frame_BTime2;		// 485��2֡���ַ��䶨ʱ 1/2ms
                                                        
  unsigned short 	BTime0;				// 485��1�ַ��䶨ʱ
  unsigned short 	BTime1;				// 485��2�ַ��䶨ʱ
  unsigned short 	BTime2;				// 485��3�ַ��䶨ʱ

  unsigned char 	RxdCnt0;			// ����֡������1
  unsigned char 	RxdCnt1;			// ����֡������2
  unsigned char 	RxdCnt2;			// ����֡������3


	unsigned char	SendCnt0;			// ����֡������1
	unsigned char	SendCnt1;			// ����֡������2
	unsigned char	SendCnt2;			// ����֡������2




  	unsigned short 	Ptr0;				// 485��1������ָ��
  	unsigned short 	Ptr1;				// 485��2������ָ��
  	unsigned short 	Ptr2;				// 485��3������ָ��


  	unsigned char 	Buf0[256];			// 485��1ͨ�Ż���		
  	unsigned char 	Buf1[256];			// 485��2ͨ�Ż���		
 	unsigned char 	Buf2[256];			// 485��3ͨ�Ż���		
		

	unsigned char 	Mode0;				// 485��1ͨѶģʽ
  	unsigned char 	Mode1;				// 485��2ͨѶģʽ
  	unsigned char 	Mode2;				// 485��3ͨѶģʽ

	unsigned char	SendDTime0;			// ������ʱ1
	unsigned char	SendDTime1;			// ������ʱ2
	unsigned char	SendDTime2;			// ������ʱ3

	unsigned char 	TX_Delay0;			// ������ʱ1
	unsigned char 	TX_Delay1;			// ������ʱ2
	unsigned char 	TX_Delay2;			// ������ʱ3

	unsigned char	Run0;				//485��1���б�־
	unsigned char	Run1;				//485��2���б�־
	unsigned char	Run2;				//485��3���б�־


	unsigned char 	BandRate0;			//485��1������
	unsigned char 	BandRate1;			//485��2������
	unsigned char 	BandRate2;			//485��3������


	unsigned char 	RecTmr0;			// ���ռ�¼��ʱ
	unsigned char 	RecTmr1;			// ���ռ�¼��ʱ
	unsigned char 	RecTmr2;			// ���ռ�¼��ʱ
	unsigned char 	RecTmr3;			// ���ռ�¼��ʱ


	unsigned char 	FTime0;				// ֡�䶨ʱ1
	unsigned char 	FTime1;				// ֡�䶨ʱ2
	unsigned char 	FTime2;				// ֡�䶨ʱ3


	unsigned char IECState0;	// ͨѶ��1IECͨѶ����״̬
	unsigned char IECState1;	// ͨѶ��2IECͨѶ����״̬
	unsigned char IECState2;	// ͨѶ��3IECͨѶ����״̬

	unsigned char MACState0;	// ͨѶ��1MACͨѶ����״̬                                                         
	unsigned char MACState1;	// ͨѶ��2MACͨѶ����״̬                                                         
	unsigned char MACState2;	// ͨѶ��3MACͨѶ����״̬                                                         
                                                                                                                 

    unsigned char LLCState0;	// ͨѶ��1MACͨѶ����״̬                                                     
    unsigned char LLCState1;	// ͨѶ��2MACͨѶ����״̬                                                     
    unsigned char LLCState2;	// ͨѶ��2MACͨѶ����״̬                                                     
                                                    

    unsigned char LastRecCommand0;	// ͨѶ��1����յ�������
    unsigned char LastRecCommand1;	// ͨѶ��2����յ�������
    unsigned char LastRecCommand2;	// ͨѶ��3����յ�������


	unsigned char	Run;				//���б�־

	unsigned char 	BandRateMode;	//������ģʽ 0�����裻1������Ӧ   
	unsigned char   CPAOnDly;			// ͣ���ڼ�Ƚ�������ʱ	

	BCD4			OperateCode;	//�����ߴ���
	BCD4			TripONOperateCode;	//��բ�����ߴ���
	BCD4			TripOFFOperateCode;	//��բ�����ߴ���

	unsigned char 	BatComBuff[3];	//ͣ�����ѻ�����				//�¹���

	unsigned char 	TX_CardDelay;		//��������ʱ
	BCD8			CommReserve;		//����8�ֽڿռ�

}COMM;

//APPLState						// ͨѶ��1APPL����״̬
#define	APPLIdle			0	// APPL����
#define	APPLON1				1	// APPL������1	//��������		
#define	APPLON2				2	// APPL������2	//һ������	
#define	APPLON3				3	// APPL������3	//�ԳƼ���	//17.04.07
#define	APPLON4				4	// APPL������4	//����ǩ��	//17.04.07

#define SUBS_PARA_LEN		100		//�鴫�����֡�������泤��

typedef struct
{
	unsigned long	BlockNumber0;		//����0GET��ͨѶ���
	unsigned long	BlockNumber1;		//����1GET��ͨѶ���
	unsigned long	BlockNumber2;		//����1GET��ͨѶ���
	unsigned long	BlockNumber3;		//����1GET��ͨѶ���

	unsigned short CommIntReserve1;	//�������Ϳռ�1
	unsigned short CommIntReserve2;	//�������Ϳռ�2

	unsigned short SetBlockPtr0;		// 485��1�����û���ָ��
	unsigned short SetBlockPtr1;		// 485��2�����û���ָ��
	unsigned short SetBlockPtr2;		// 485��2�����û���ָ��
	unsigned short SetBlockPtr3;		// 485��2�����û���ָ��

	unsigned short 	HBTime0;			// 485��1�ַ��䶨ʱ
	unsigned short 	HBTime1;			// 485��2�ַ��䶨ʱ
	unsigned short 	HBTime2;			// 485��2�ַ��䶨ʱ
	unsigned short 	HBTime3;			// 485��2�ַ��䶨ʱ

	unsigned short 	RecDataPtr0;		// 485��1������ָ��
	unsigned short 	SendDataPtr0;		// 485��1������ָ��
	unsigned short 	RecDataPtr1;		// 485��2������ָ��
	unsigned short 	SendDataPtr1;		// 485��2������ָ��
	unsigned short 	RecDataPtr2;		// 485��2������ָ��
	unsigned short 	SendDataPtr2;		// 485��2������ָ��
	unsigned short 	RecDataPtr3;		// 485��2������ָ��
	unsigned short 	SendDataPtr3;		// 485��2������ָ��

	unsigned short 	SendByteSum0;		// 485��1LLC��������
	unsigned short 	SendByteSum1;		// 485��2LLC��������
	unsigned short 	SendByteSum2;		// 485��2LLC��������
	unsigned short 	SendByteSum3;		// 485��2LLC��������

	unsigned char	RecDataBuf0[256];	// 485��1����ͨ�Ż���		//530---2016-6-16-fire
	unsigned char	SendDataBuf0[256];	// 485��1����ͨ�Ż���		//530---2016-6-16-fire
	unsigned char	RecDataBuf1[256];	// 485��2����ͨ�Ż���		//150
	unsigned char	SendDataBuf1[256];	// 485��2����ͨ�Ż���		//150
	unsigned char	RecDataBuf2[256];	// 485��2����ͨ�Ż���		//150
	unsigned char	SendDataBuf2[256];	// 485��2����ͨ�Ż���		//150


	unsigned char	HMode0; 			// 485��1ͨѶģʽ
	unsigned char	HMode1; 			// 485��2ͨѶģʽ	
	unsigned char	HMode2; 			// 485��2ͨѶģʽ
	unsigned char	HMode3; 			// 485��2ͨѶģʽ
	
	unsigned char		HState0;			// 485��1ͨ��״̬
	unsigned char		HState1;			// 485��2ͨ��״̬
	unsigned char		HState2;			// 485��2ͨ��״̬
	unsigned char		HState3;			// 485��2ͨ��״̬

//	unsigned char	HRun;					// ���б�־
										   
	unsigned char	HBandRate0; 	// 485��1������
	unsigned char	HBandRate1; 		// 485��2������
	unsigned char	HBandRate2; 		// 485��2������
	unsigned char	HBandRate3; 		// 485��2������

	unsigned char	HRecTmr0;			// ���ռ�¼��ʱ
	unsigned char	HRecTmr1;			// ���ռ�¼��ʱ
	unsigned char	HRecTmr2;			// ���ռ�¼��ʱ
	unsigned char	HRecTmr3;			// ���ռ�¼��ʱ

	unsigned char	HPrgTmr0;			// ��̼�¼��ʱ
	unsigned char	HPrgTmr1;			// ��̼�¼��ʱ
	unsigned char	HPrgTmr2;			// ��̼�¼��ʱ
	unsigned char	HPrgTmr3;			// ��̼�¼��ʱ
	
	unsigned char APPLState0;			// ͨѶ��1APPL����״̬
	unsigned char APPLState1;			// ͨѶ��2APPL����״̬
	unsigned char APPLState2;			// ͨѶ��2APPL����״̬
	unsigned char APPLState3;			// ͨѶ��2APPL����״̬

	BCD4 Conformance_block0;			// ͨѶ��1Ӧ�����ӵ�һ���Կ����	//ǰ���ֽ�Ϊһ���Կ��������һ�ֽ�Ϊ��ȫ����0���ް�ȫ���� 0x07: ��Ͱ�ȫ����	
	BCD4 Conformance_block1;			// ͨѶ��2Ӧ�����ӵ�һ���Կ����	//ǰ���ֽ�Ϊһ���Կ��������һ�ֽ�Ϊ��ȫ����0���ް�ȫ���� 0x07: ��Ͱ�ȫ����	
	BCD4 Conformance_block2;			// ͨѶ��2Ӧ�����ӵ�һ���Կ����	//ǰ���ֽ�Ϊһ���Կ��������һ�ֽ�Ϊ��ȫ����0���ް�ȫ���� 0x07: ��Ͱ�ȫ����	
	BCD4 Conformance_block3;			// ͨѶ��2Ӧ�����ӵ�һ���Կ����	//ǰ���ֽ�Ϊһ���Կ��������һ�ֽ�Ϊ��ȫ����0���ް�ȫ���� 0x07: ��Ͱ�ȫ����	
	unsigned char	SubsBlockPara0[SUBS_PARA_LEN];	//����0����֡�����
	unsigned char	SubsBlockPara1[SUBS_PARA_LEN];	//����1����֡�����
	unsigned char	SubsBlockPara2[SUBS_PARA_LEN];	//����1����֡�����
	unsigned char	SubsBlockPara3[SUBS_PARA_LEN];	//����1����֡�����

//	enum {
//		APPLIdle,				// APPL����
//		APPLON, 				// APPL������		
//	} APPLState0;				// ͨѶ��1APPL����״̬

//add sky20140519..
	unsigned char APPL_A8_BUSY[4];		   // �жϽ����Ƿ��ڷ�֡����״̬..
//	enum {
//	  DATABLOCKIDLE,		// �ޣ�0
//		FIRSTDATABLOCK, 			// �����һ�鴫��..
//		MIDDATABLOCK,					// ���ݿ��м䴫��..
//		ENDDATABLOCK,					// ���һ�鴫��..
//	  NEW_BLOCK,				// �����ݿ��֡��Ҫ���´�����ݿ�����д���..
//	} COSEM_BlockState; 			// ���ݿ鴫��״̬	
  unsigned char COSEM_BlockState[4];   // �ж���Ҫ�ֿ���ٷְ����������ݿ鴫���״̬..
//	unsigned char BIGBlockNumber[4];	 // ������ݿ�����---����Ӧ�ò�����ݼ���..
  unsigned short BIGBlockNumber[4];	 // ������ݿ�����---����Ӧ�ò�����ݼ���..
//������������ǰ���ַ�;
 unsigned char Send_state_InvokeId;   // 
 
	//�ͻ�����ַ--�ж�Ȩ�ޣ�
	  unsigned char ClientAddr[4];
	BCD8			CommReserve;	//����8�ֽڿռ�
	unsigned short  BK_FCRXCount;	// ����..
	unsigned short  DK_FCRXCount;	// ����..	
	unsigned short  DK_FCTXCount;	//
	
	unsigned short  UK_FCRXCount;	// ����..
	unsigned short  UK_FCTXCount;	//
		 
	unsigned char  RepeatBuff[256];
	
}HCOMM;

#pragma pack(8) 
//���������֤����..�ӽ��ܵı���Ҫ4���ֽڶ��뷽ʽ�洢���ڴ���
typedef struct
{
  unsigned char 	Special_A8[4];  		 //�����֡����--������512�ֽڣ�̩����ʱ��..
  unsigned char		aarqHLS[4];					 //��ͬͨѶ�ڣ��ֱ���..
  unsigned char 	type[4];          	 //�������ͣ�10--��֤���ܣ�20--Ӧ�ò����ݼ��ܣ�30--��֤����
  unsigned char 	glo_request[4];   	 //ȫ�����ܿ���ʶ���ʶ..
  
  unsigned char   glo_FC[6];					 //
  unsigned char   choice_algorithm;    //ѡ������㷨..
  unsigned char   Challenge_Len;  
  unsigned char   S_Challenge[16];		 //�������������..
  unsigned char   C_Challenge[16];     //�ͻ��˵������..
   unsigned char 	AK[16];
   unsigned char 	EK[16]; 
   unsigned char 	IV[12];
  unsigned char 	SH[5];
  // ����ͣ���ж�ʱ��..
  unsigned char    CutPower;            //0x0A---��ʾͣ���һ�Σ����ϵ磬�����֮�����ٽ����ж�..  
  unsigned char    GPRS_STAND;          //GPRSģʽ���ϱ�־0x0B.. ģ��ε���λ..
  unsigned char    comm_online;         //1--���ߣ� 0-- ������;
  
}CRYPT;						//�ֽ�	
#pragma pack()
//����40��--�澯�¼�..
typedef struct
{
	//�ж�..			
	unsigned char Type; //���͵�����
	unsigned char Ch;   //���͵�ͨ��
        unsigned char valid;
			
}PUSHSTATE;
/*
//����ͨѶ���
typedef struct
{
	unsigned short 	OI;
	unsigned short 	Addr;
  	unsigned short 	SaftInfo;		//��ȫģʽ����
	unsigned char	FeeNum;			//������+�Ƿ�֧�־�ȷ����
	unsigned char 	Unit;			//��λ
	unsigned char	State1;			//Һ������1
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
}DL698_ECTAB;

typedef struct
{
	unsigned short 	OI;
	unsigned short 	Addr;
  	unsigned short 	SaftInfo;		//��ȫģʽ����
	unsigned char	FeeNum;			//������
	unsigned char 	Unit;			//��λ
	unsigned char	State1;			//Һ������1
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
}DL698_MDTAB;

typedef struct
{
	unsigned char* 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	SaftInfo;		//��ȫģʽ����
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			
	unsigned char	BCD_Len;			//ԭBCD���ݳ���
	signed char			Multiple;			//�Ŵ���				������ʾ��С
}DL698_CLASSID3_TAB;

typedef struct
{
	unsigned char* 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	SaftInfo;		//��ȫģʽ����
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			//�����ṹʱ��ʾԪ�ظ���
	unsigned char	BCD_Len;			//ԭBCD���ݳ���
	signed 	 char	Multiple;			//�Ŵ���				������ʾ��С
}DL698_CLASSID6_TAB;

typedef struct
{
	unsigned short 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	SaftInfo;		//��ȫģʽ����
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			//�����ṹʱ��ʾԪ�ظ���
	unsigned char	BCD_Len;			//ԭBCD���ݳ���
	signed char			Multiple;			//�Ŵ���				������ʾ��С
}DL698_CLASSID24_5_TAB;

typedef struct
{
	unsigned short 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	ComInfo;
  	unsigned short 	SaftInfo;		//��ȫģʽ����
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			//�����ṹʱ��ʾԪ�ظ���
}DL698_CLASSID8_TAB;

//��ʼ����ʾ�ಶ��������
typedef struct
{
	unsigned char 	Data_Type;				//CHOICE 0��OAD 1��ROAD
	unsigned char 	OAD1[4];				//��ʾ��OAD
	unsigned char 	OAD2[4];				//ROAD�е�OAD
  	unsigned char 	Dis_Number;				//�����
}INI_DISPLAY_TAB;

//��ʼ����ʾ�ಶ��������
typedef struct
{
	unsigned char 	OAD[4];				//�¼���¼��OAD
}INI_EVENT_TAB;

//��ʼ�������ಶ��������
typedef struct
{
	unsigned short	Period;				//��������
	unsigned char 	OAD[4];				//�¼���¼��OAD
	unsigned short	Deep;				//�������
}INI_FREEZE_TAB;

typedef struct
{
	unsigned short	BufAddr;
	unsigned short	BufLen;
	unsigned char 	OAD[4];				//�¼���¼��OAD	
}INI_EVENT_HEAD_TAB;

typedef struct
{
	unsigned short EventNo;			//�¼���
	unsigned short UpState;			//�¼��������ϱ�״̬	//17.03.23
	unsigned short CuntAddr;		//�¼��ܴ�����ַ
	unsigned short TimeAddr;		//�¼��ۼ�ʱ���ַ
  	unsigned short SaftInfo;		//��ȫģʽ����			//17.04.09	
	unsigned short TimeValidFlag;	//�¼��ۼ�ʱ����Ч��־	0����Ч����Զ����0	1����Ч��ʵʱ�ۼ�
	unsigned short PtAddr;			//��ǰ��¼�׵�ַָ��(�ü�¼��ռ�õ�FLASH�ռ��ڵ���Ե�ַ)
	unsigned char OAD[4];
	unsigned short Class_Id;		//���
	unsigned short StartAddr;		//��¼��ʼ��ַ:FLASH��ʼҳ��												
	unsigned short EndAddr;			//��¼������ַ(ռ��FLASHҳ��)												
	unsigned short CapNumAddr;		//���߲���������ַ
	unsigned short CapNumMax;		//������󲶻�����(����ռ�������յ�����)
	unsigned short RecdLenAddr;		//ÿ����¼���ȵ�ַ(���ݲ������ݵõ�)
	unsigned short CapObjAddr;		//�������ַ
	unsigned short EntriesUseAddr;	//��Ч��Ŀ����ַ���������Ϊ�����ȡ���ܼ�¼�����������������ӡ���¼����ʱֻ�轫�����㼴��
	unsigned short EntriesAddr;		//�߼������Ŀ����ַ(��ȡʱ�����Ŀ��)
	unsigned short EReportFlagAddr;	//�ϱ���ʶ��ַ
	unsigned short EValidFlagAddr;	//��Ч��ʶ��ַ
	unsigned short ECurRecNoAddr;	//��ǰ��¼�ŵ�ַ
	unsigned short EFinalRecNoAddr;	//���¼�¼�ŵ�ַ,�������ڱ�ʾ�¼���¼���
	unsigned short InitCapNumber;	//���߳�ʼ����������
	unsigned char* InitCapObjTab;	//���߳�ʼ����������
	unsigned short EventHeadNumber;	//�¼�ͷ��������
	INI_EVENT_HEAD_TAB* EventHeadTab;	//�¼�ͷ��������
}DL698_EVENT_PROFILE_TAB;	

typedef struct
{
	unsigned short PtAddr;			//��ǰ��¼�׵�ַָ��(�ü�¼��ռ�õ�FLASH�ռ��ڵ���Ե�ַ)
	unsigned short CapNumAddr;		//���߲���������ַ
  	unsigned short SaftInfo;		//��ȫģʽ����			//17.04.09	
	unsigned char OAD[4];
	unsigned short CapNumMax;		//������󲶻�����(����ռ�������յ�����)
	unsigned short RecdLenAddr;		//ÿ����¼�����׵�ַ(������Ӧ�ڲ������ݵõ�)
	unsigned short CapObjAddr;		//�������ַ
	unsigned short EntriesUseAddr;	//��Ч��Ŀ���׵�ַ���õ�ַ�ж�Ӧ�����Ϊ�����ȡ���ܼ�¼�����������������ӡ���¼����ʱֻ�轫�����㼴��
	unsigned short FinalRecNoAddr;	//���¼�¼�ŵ�ַ,�������ڱ�ʾ�¼���¼���
	unsigned short InitCapNumber;	//���߳�ʼ����������
	unsigned char* InitCapObjTab;	//���߳�ʼ����������
}DL698_FREEZE_PROFILE_TAB;	

typedef struct
{
	unsigned short CapNumAddr;		//��ʾ������ַ
	unsigned short CapNumMax;		//��ʾ�������(����ռ�������յ�����)
  	unsigned short SaftInfo;		//��ȫģʽ����			//17.04.09	
	unsigned char OAD[4];
	unsigned short CapObjAddr;		//��ʾ���ַ
	unsigned short PeriodAddr;
	unsigned short InitCapNumber;	//��ʾ��ʼ������
//	unsigned char* InitCapObjTab;	//��ʾ��ʼ������
	const INI_DISPLAY_TAB* InitCapObjTab;	//��ʾ��ʼ������
}DL698_DISPLAY_PARA_TAB;	
*/
/*
typedef struct
{
	unsigned short CurRecNo;		//��ǰ��¼��
	unsigned short Space;			//������
	unsigned char OAD[4];
	unsigned short RecdCunt;		//�洢�ռ�����¼��
    unsigned short AvailRecs;		//��ǰ�Ѵ洢����Ч��¼��
	unsigned short RecdLenth;		//��¼����
	unsigned short ChannelNo;		//ͨ����
	unsigned char  BakFlag;			//�Ƿ��б��ݱ�־0���ޱ��ݣ�1�б��ݣ�ÿ�����ݴ�1�ֽ�У���
}FILECURVEPARA;
*/				//16.10.12
typedef struct
{
	unsigned short CurRecNo;		//��ǰ��¼��
	unsigned short Space;			//������
	unsigned char OAD[4];
	unsigned short MaxCunt;			//�洢�ռ�����¼��
	unsigned short RecdCunt;		//�洢�ռ�����¼����ȥ��4K��ת�غ�
    unsigned short AvailRecs;		//��ǰ�Ѵ洢����Ч��¼��
	unsigned short  Pages;			//�洢�ռ���ռ��ҳ��
	unsigned short  RecsInPage;		//ÿҳ��¼��
	unsigned short RecdLenth;		//��¼����
	unsigned short ChannelNo;		//ͨ����
	unsigned short DirNo;			//�ļ�Ŀ¼���
	unsigned short StartSectorNo;	//��ʼ������
	unsigned char  BakFlag;			//�Ƿ��б��ݱ�־0���ޱ��ݣ�1�б��ݣ�ÿ�����ݴ�1�ֽ�У���
}FILECURVEPARA;

typedef struct
{
	unsigned short DirAddr;
	unsigned short StartSector;
	unsigned short Sectors;
}FILEPARA;

typedef struct
{
	long* SumPower;
	unsigned char* RealPower;
	unsigned char* AverPower;
}POWERCHECKPARA;

//BIT0��ʾ���һ���¼���¼
typedef struct						//17.03.24					
{
	unsigned short StartState;		//�¼�����״̬		0��δ���� 1���ѷ���
	unsigned short StartUpState;	//�¼������ϱ�״̬  0��δ�ϱ� 1�����ϱ�
	unsigned short EndState;		//�¼�����״̬		0��δ���� 1���ѷ���
	unsigned short EndUpState;		//�¼������ϱ�״̬	0��δ�ϱ� 1�����ϱ�
}EVENT_UPSTATE;
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//------------------DLMS---------------------------------------------------------------
//DATAͨѶ���
typedef struct
{
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	unsigned char	State;				//����
}DLMS_RAM_DATATAB;

//DATAͨѶ���
typedef struct
{
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	//unsigned char	State;				//����
	unsigned short 	State;
	unsigned short	Acess;
}DLMS_DATATAB;
//����ͨѶ���
typedef struct
{
	unsigned short 	Addr;
	unsigned short 	MDTAddr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	Unit;
	unsigned char	State;				//����
}DLMS_MDTAB;

typedef struct
{
	unsigned short 	Time_Addr;
	unsigned short 	Date_Addr;
  	unsigned short 	MaxVal_Addr;
	unsigned short 	Cum_Addr;
	unsigned short	ContCum_Addr;
}DLMS_MAXKW;

typedef struct
{
	unsigned char	Demand_P[5];
	unsigned char	Demand_PFee[5];	
	unsigned char	Demand_Pp[5];
	unsigned char	Demand_PpFee[5];
	unsigned char	Demand_Pn[5];
	unsigned char	Demand_PnFee[5];
	unsigned char	Demand_Qp[5];
	unsigned char	Demand_QpFee[5];
	unsigned char	Demand_Qn[5];
	unsigned char	Demand_QnFee[5];
#if(MeterPhase==Threephase)
	unsigned char	Demand_Q10[5];
	unsigned char	Demand_Q10Fee[5];
	unsigned char	Demand_Q20[5];
	unsigned char	Demand_Q20Fee[5];
	unsigned char	Demand_Q30[5];
	unsigned char	Demand_Q30Fee[5];
	unsigned char	Demand_Q40[5];
	unsigned char	Demand_Q40Fee[5];
#endif
	unsigned char	Demand_PT[7];
	unsigned char	Demand_PTFee[7];	
	unsigned char	Demand_PpT[7];
	unsigned char	Demand_PpTFee[7];
	unsigned char	Demand_PnT[7];
	unsigned char	Demand_PnTFee[7];
	unsigned char	Demand_QpT[7];
	unsigned char	Demand_QpTFee[7];
	unsigned char	Demand_QnT[7];
	unsigned char	Demand_QnTFee[7];
#if(MeterPhase==Threephase)
	unsigned char	Demand_Q10T[7];
	unsigned char	Demand_Q10TFee[7];
	unsigned char	Demand_Q20T[7];
	unsigned char	Demand_Q20TFee[7];
	unsigned char	Demand_Q30T[7];
	unsigned char	Demand_Q30TFee[7];
	unsigned char	Demand_Q40T[7];
	unsigned char	Demand_Q40TFee[7];
#endif

}Demand_Reg;

//�����Ĵ���ͨѶ���
typedef struct
{
//	unsigned short 		MDEAds;					//����ֵ��ַ
//	unsigned short 		MDTEAds;				//��������ʱ���ַ	
    unsigned char*  MDEAds;
	unsigned char*	MDTEAds;
	unsigned char 	Logical_Name[6];
//	unsigned short* 	MDBufAds;				//�����ṹ��ַ
	unsigned short 		EperiodAds;				//����ʱ���ַ
	unsigned short 		Enumber_of_periodsAds;  //��������ַ 
}DLMS_DEMAND_REGTAB;

typedef struct
{
	unsigned short MDBuf[60];			//����ʱ����������������
	unsigned short MD_Cum;				//����ʱ����������						
	
	unsigned long current_average_value;
	unsigned long last_average_value;
	unsigned char capture_time[12];
	unsigned char start_time_current[12];
	unsigned short period;				//����ʱ�����
	unsigned short period_counter;		//����ʱ�����������
	unsigned char number_of_periods;
	
//�������������������ڷ����л�ʱ�����㣬���������������������ڷ����л�ʱ����
	unsigned char MD_Cunt;				//��������ʱ��Ҫ��ǰ��ȡ�������������建��������
	unsigned char MD_CuntT;				//�����������ʱ��Ҫ��ǰ��ȡ�������������建��������
	unsigned char MD_BufAds;			//��ǰ������������ַ
	unsigned char Status;
	unsigned char FeeNo;				//�������ʺ�
	unsigned char State1;
	unsigned char State2;
}MDBUFF;
//����ͨѶ���
typedef struct
{
	unsigned char* ECRAds;
	unsigned short ECFAds;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	Unit;
	unsigned char	State;				//����
}DLMS_ECTAB;

//����ͨѶ���
typedef struct
{
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	State;				//����
}DLMS_REALTAB;
//��ʼ���������߲���������
typedef struct
{
	unsigned short 	  ClassId;
	unsigned char 	Logical_Name[6];
  unsigned char 	Attribute;
  unsigned char 	DataLen;				//�������ݳ���
}INI_PROFILE_TAB;

//������ʱ���ͨѶ���
typedef struct
{
	unsigned short 	Addr;
  unsigned short 	ComInfo;
	unsigned char Logical_Name[6];
	unsigned char	State1;				//����
	unsigned char	State2;				//����
}DLMS_SIGLE_ACTTAB;
//�ű�ͨѶ���
typedef struct
{
	unsigned short 	NumAddr;			//�ű���
  	unsigned short 	ComInfo;			
	unsigned char 	Logical_Name[6];
	unsigned short 	IdentAddr;			//�ű���ʶ��1��ַ
	unsigned short 	ActsNumAddr;		//�ű���ʶ��1����˵������ַ
	unsigned short 	ActsAddr;			//�ű���ʶ��1����˵����
  	unsigned short 	Len;				//�ű���ʶ������˵��������
	unsigned char	State1;				//����
	unsigned char	State2;				//����
}DLMS_SCRIPTPARATAB;
//�ű�ͨѶ���
typedef struct
{
	unsigned short 	Script_No;			//�ű���
	unsigned char 	Logical_Name[6];
	unsigned short 	Script_Ident;		//�ű���ʶ��
}DLMS_SCRIPTTAB;
//Cosem�������
typedef struct
{
	unsigned short 	ClassId;
  	unsigned char 	AttributeNum;
	unsigned char 	MethodNum;
  	unsigned char 	RListLen;
  	unsigned char 	WListLen;
}COSEM_PARATAB;

typedef struct
{
  	unsigned char 	AccessMode;			//���Է��ʷ�ʽ
  	unsigned char 	DataType;			//������������
  	unsigned char 	AllDataLen;			//�������ݳ��ȣ������������͵�
  	unsigned char 	DataLen;			//�������ݳ���
}ATTRIBUTE_PARATAB;
//�Ĵ���������ͨѶ���
typedef struct
{
	unsigned short 	Addr;
  unsigned short  ComInfo;
	unsigned char Logical_Name[6];
	unsigned short 	State1; 			//����
	unsigned short 	State2; 			//����
}DLMS_REG_ACTIVATIONTAB;
typedef struct
{
	unsigned short PtAddr;			//��ǰ��¼�׵�ַָ��(�ü�¼��ռ�õ�FLASH�ռ��ڵ���Ե�ַ)
	unsigned short StartAddr;			//��¼��ʼ��ַ:FLASH��ʼҳ��												
	unsigned short EndAddr;			//��¼������ַ(ռ��FLASHҳ��)												
	unsigned short RecdLenAddr;		//ÿ����¼���ȵ�ַ(���ݲ������ݵõ�)
	unsigned short EntriesUseAddr;	        //��Ч��Ŀ����ַ���������Ϊ�����ȡ���ܼ�¼�����������������ӡ���¼����ʱֻ�轫�����㼴��
	unsigned short EntriesAddr;		//�߼������Ŀ����ַ(��ȡʱ�����Ŀ��)
	unsigned short ECurRecNoAddr;		//��ǰ��¼�ŵ�ַ
}PROFILETAB;	
//�Ĵ���ͨѶ���
typedef struct
{
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned short	State;				//����
}DLMS_REGISTERTAB;

//��ʼ�����ʺŶ���..
typedef struct
{	
	unsigned char 	FEE_OCTET_STRING[5];
//	unsigned char	FEE_OCTET_STRING;
	unsigned char   state1;						//����..	
 }INI_FEE_TAB;
 //�Ĵ�������ͨѶ���
 typedef struct
 {
	 unsigned short	 Addr;
	 unsigned short	 ComInfo;
	 unsigned char	 Logical_Name[6];
	 unsigned char	 State1;			 //����
	 unsigned char	 State2;			 //����
 }DLMS_REG_MONITORTAB;
 
 //�ۼ�����ͨѶ���
 typedef struct
 {
	 unsigned short	 Addr;
	 unsigned short	 ComInfo;
	 unsigned char	 Logical_Name[6];
	 unsigned char	 Unit;
	 unsigned char	 State; 			 //����
 }DLMS_CUMDTAB;
 typedef struct
 {
	 //�ж�..			 
	 unsigned char	Ram_Auto_Reclosing_relay_Num_Normal;					 //�������޺��Զ��غ�բ����(2)			 Data::Unsigned8
	 unsigned short   Ram_Reconnection_timeout_Normal; 							 //�������޺��غ�բ��ʱʱ��(1)		 Register::Unsigned8 --����..
	 unsigned char	Ram_Auto_Reclosing_relay_Num_emergency; 			 //�������޺��Զ��غ�բ����(����)(2)	 Data::Unsigned8
	 unsigned short   Ram_Reconnection_timeout_emergency;						 //�������޺��غ�բ��ʱʱ��(����)(1)	 Register::Unsigned8--����..
	 //ʵ�����м�¼..
	 unsigned char	Ram_Auto_Reclosing_relay_Num_cnt;							 //�������޺��Զ��غ�բ����(2)
	 unsigned short   Ram_Reconnection_timeout_Normal_cnt; 					 //�������޺��غ�բ��ʱʱ��(3)		 Register::Unsigned16 --����*60--�����ж�
	 unsigned char	Ram_Auto_Reclosing_relay_Num_emergency_cnt; 	 //�������޺��Զ��غ�բ����(����)(2)	 Data::Unsigned8
	 unsigned short   Ram_Reconnection_timeout_emergency_cnt;				 //�������޺��غ�բ��ʱʱ��(����)(3)	 Register::Unsigned16--����*60--�����ж�
 
 
	 unsigned long	 Ram_Limiter_monitored_value;			 //ȡ����ֵ..
	 
	 //��������ֵ..
	 unsigned char	 Limiter_monitored_NO;							 //�ж�ȡ�ĸ�ֵ��Ϊ�ж����� 0--���й�(��+��)����.. 1--�����й�����
	 
	 unsigned short		 Limiter_RelayMode; 							 //�̵���ģʽʹ���ж�..
	 unsigned short	LastTime_Limiter_RelayMode; 			 //�����ϴεļ̵���ģʽʹ���ж�..
	 unsigned long	 Ram_Limiter_threshold_active;			 //������ֵ..
	 unsigned long	 Ram_Limiter_min_over_threshold_duration;	 //4�ֽ�Խ���ж�ʱ��..	 
	 unsigned long	 Ram_Limiter_min_under_threshold_duration;	 //4�ֽڻָ��ж�ʱ��..		 
	 
	 //����,����..
	 unsigned short	 AvePowerTime;			 //1����ƽ�����ʼ�����ʼʱ��(�ϵ��15���Ӻ�ſ�ʼ����)
	 unsigned long	 AvePower_Total;	 //�й��ܹ���
	 unsigned long	 AvePower_Pp;			 //�й�������
	 
	 //�¼��жϲ���..
	 //�¼��жϼĴ���..
	 unsigned long	 over_threshold_duration_cnt;				 //�ж�����Խ��ʱ��..
	 unsigned long	 min_under_threshold_duration_cnt;		 //�ж������ָ�Խ��ʱ��..
	 
   BYTE_STRUCT	 flg_Threshold_MDEvent1;	   //��������ֵ�¼�..
	 BYTE_STRUCT   flg_Threshold_MDEvent1BK;	 //��������ֵ�ϴ�״̬..
	 
	 BYTE_STRUCT   Threshold_MDEvent;			 //��������ֵ�¼�(��ע��ʼ�ͽ���)..
	 
	 BYTE_STRUCT   LAST_OVER_Event; 				 //�ж��¼���բ����ʱ���Ƿ�0x11--��ʼ,�������..
   
 
 }LIMITER;
 typedef struct
{	
	//�ж�ֵ//quality_threshold_judge..
	unsigned short 	Ram_Threshold_for_voltage_sag;    		//1,	0,	12,	31,	0,	255,D_Unsigned16,	0xFF,	Unit_V,	0,//Ƿѹ��ֵ(3)	Register::Unsigned16  
	unsigned short 	Ram_Threshold_for_voltage_swell;			//1,	0,	12,	35,	0,	255,D_Unsigned16,	0xFF,	Unit_V,	0,//��ѹ��ֵ(3)	Register::Unsigned16  
	unsigned short 	Ram_Threshold_for_missing_voltage;		//1,	0,	12,	39,	0,	255,D_Unsigned16,	0xFF,	Unit_V,	0,//ʧѹ��ֵ(3) Register::Unsigned16  
	
	unsigned short 		Ram_Time_threshold_for_voltage_sag;		//1,	0,	12,	43,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,//Ƿѹ�жϳ���ʱ��(3)   	Register::Unsigned16  
	unsigned short 		Ram_Time_threshold_for_voltage_swell;	//1,	0,	12,	44,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,//��ѹ�жϳ���ʱ��(3)       Register::Unsigned16
	unsigned short 		Ram_Time_threshold_for_voltage_cut;		//1,	0,	12,	45,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,//ʧѹ�жϳ���ʱ��(3)       Register::Unsigned16
	unsigned short	 	Ram_Time_threshold_for_long_power_failure;	//0,0,96,7,20,255,D_Unsigned16,	0x00,	Unit_s,	0,	//��ʱ���Դ���ϳ���ʱ�䷧ֵ(5)	Register::Unsigned32 //2013.6.19��2���ֽ�..   
	
	//�¼���¼..                                                	                                               
	unsigned short Ram_Duration_of_last_voltage_sag_in_phase_L1;				//1,	0,	32,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//A��Ƿѹ����ʱ��(3)    	Register::Unsigned16  
	unsigned short Ram_Duration_of_last_voltage_swell_in_phase_L1;			//1,	0,	32,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//A���ѹ����ʱ��(3)    	Register::Unsigned16  
	                                                	                                               
///	unsigned short Ram_Duration_of_last_voltage_sag_in_phase_L2;				//1,	0,	52,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//B��Ƿѹ����ʱ��(3)    	Register::Unsigned16   
///	unsigned short Ram_Duration_of_last_voltage_swell_in_phase_L2;			//1,	0,	52,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,  //B���ѹ����ʱ��(3)    	Register::Unsigned16  
	                                                	                                               
///	unsigned short Ram_Duration_of_last_voltage_sag_in_phase_L3;				//1,	0,	72,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//C��Ƿѹ����ʱ��(3)    	Register::Unsigned16   
///	unsigned short Ram_Duration_of_last_voltage_swell_in_phase_L3;			//1,	0,	72,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,  //C���ѹ����ʱ��(3)    	Register::Unsigned16  
	
	unsigned short Ram_Short_power_failure_L1;							//A���ʱ���Դ���ϳ���ʱ��(5)			Register::Unsigned32         
///	unsigned short Ram_Short_power_failure_L2;							//B���ʱ���Դ���ϳ���ʱ��(5)			Register::Unsigned32           
///	unsigned short Ram_Short_power_failure_L3;							//C���ʱ���Դ���ϳ���ʱ��(5)			Register::Unsigned32       

	unsigned short Ram_Short_power_failure_any;						//�������ʱ���Դ���ϳ���ʱ��(5)		Register::Unsigned32
	
	unsigned short Ram_Duration_of_long_power_failure_in_phase_L1;							//0,	0,	96,	7,	16,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //A�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32         
///	unsigned short Ram_Duration_of_long_power_failure_in_phase_L2;							//0,	0,	96,	7,	17,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32         
///	unsigned short Ram_Duration_of_long_power_failure_in_phase_L3;							//0,	0,	96,	7,	18,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C�೤ʱ���Դ���ϳ���ʱ��(5)	Register::Unsigned32     
	
	unsigned short Ram_Duration_of_long_power_failure_in_any_phase;						//0,	0,	96,	7,	19,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //�����೤ʱ���Դ���ϳ���ʱ��(5)Register::Unsigned32
		
	unsigned short Ram_Total_duration_of_long_power_failure_in_phase_L1;				//0,	0,	96,	7,	22,	255,D_Unsigned32,	0x00,	Unit_s,	0,	//A�೤ʱ���Դ�����ܳ���ʱ��(5)Register::Unsigned32   
///	unsigned short Ram_Total_duration_of_long_power_failure_in_phase_L2;				//0,	0,	96,	7,	23,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B�೤ʱ���Դ�����ܳ���ʱ��(5)Register::Unsigned32   
///	unsigned short Ram_Total_duration_of_long_power_failure_in_phase_L3;				//0,	0,	96,	7,	24,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C�೤ʱ���Դ�����ܳ���ʱ��(5)Register::Unsigned32   ��λ��s

	unsigned short Ram_Total_Duration_of_long_power_failure_in_any_phase;			//0,	0,	96,	7,	19,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //�����೤ʱ���Դ���ϳ���ʱ��(5)Register::Unsigned32

	//��¼��ѹֵ..
	unsigned short Ram_Magnitude_of_last_voltage_sag_in_phase_L1;						//Ƿѹ��͵�ѹͳ��
	unsigned short Ram_Magnitude_of_last_voltage_swell_in_phase_L1;					//Ƿѹ��ߵ�ѹͳ��
///	unsigned short Ram_Magnitude_of_last_voltage_sag_in_phase_L2;						//Ƿѹ��͵�ѹͳ��
///	unsigned short Ram_Magnitude_of_last_voltage_swell_in_phase_L2;					//Ƿѹ��ߵ�ѹͳ��
///	unsigned short Ram_Magnitude_of_last_voltage_sag_in_phase_L3;						//Ƿѹ��͵�ѹͳ��
///	unsigned short Ram_Magnitude_of_last_voltage_swell_in_phase_L3;					//Ƿѹ��ߵ�ѹͳ��
		
	//�¼��жϼĴ���..
	unsigned short    phsreverseCNT;													//��ѹ������..
	unsigned short    MissingNeutralCNT;       								//�����ߵ������..
	unsigned short    guoyaCNT_a,duanxiangCNT_a,qianyaCNT_a,duanxianglongCNT_a;
/// 	unsigned short    guoyaCNT_b,duanxiangCNT_b,qianyaCNT_b,duanxianglongCNT_b;
///	unsigned short    guoyaCNT_c,duanxiangCNT_c,qianyaCNT_c,duanxianglongCNT_c;
    unsigned char   Deal_EVENT_NO;
  
  //�̵�����բʧ�ܼ��:�Ƿ񻹻��е�������..
    unsigned short    DisconnectFailureCNT;
  
  //���ӳ�����ʱ�����ָ������¼�1��¼..
    unsigned char   Delay_TimeFlag[3];   //����ͬ�����ඨ��..
  
  //��ѹ�ϸ��ʹ̶��ۼƼ�¼ʱ��--10����..
    unsigned short Volt_RecordTime;
  
/*-----------------------------------------------------------------------------------------------------*/
  WORD_STRUCT    flgEvent1;		         //��ѹ�¼���¼������..A--BIT0;B--BIT1;C--BIT2;

  WORD_STRUCT    flgEvent1BK;          //�¼���¼������..

  BYTE_STRUCT    guoyaEvent,qianyaEvent,duanxiangEvent,PowerFailureEvent,PowerFailurelongEvent;
 
  WORD_STRUCT    flgEvent2;		         //�����¼��������ߵ�..
  WORD_STRUCT    flgEvent2BK;          //�¼���¼������..
 
  //��ѹƽ��ֵ..
    unsigned char  AveVoltTime;
 	unsigned short   AveVoltUa;   //A���ѹƽ��ֵ..
/// 	unsigned short   AveVoltUb;   //B���ѹƽ��ֵ..
///	unsigned short   AveVoltUc;   //C���ѹƽ��ֵ..
	//��ѹƽ��ֵ����BUFF..
 	unsigned short   AveVoltUaBuff[10];   //A���ѹƽ��ֵ..
/// 	unsigned short   AveVoltUbBuff[10];   //B���ѹƽ��ֵ..
///	unsigned short   AveVoltUcBuff[10];   //C���ѹƽ��ֵ..
	
	//�̳�ͣ���¼�������ʱ����ʱ��¼����..
	unsigned short   fail_short_times;
	unsigned short   fail_long_times;			 //			
	unsigned short  fail_total_seconds;
	unsigned short  temp_long_powerfailure_any;
	unsigned short  temp_long_powerfailure_L1;
///	unsigned short  temp_long_powerfailure_L2;
///	unsigned short  temp_long_powerfailure_L3;		
}QUALITY_EVENT;
 typedef struct
 {
	 unsigned char RegAddr;
	 unsigned char Length;
 }MEASREG;
 typedef struct
 {
	 unsigned char MarSun;
	 unsigned char OctSun;	 
 }LASTSUNDAY;
 
 typedef struct
 {
	unsigned short Dis_Num;
	unsigned short Class_id;
	unsigned char  OBIS_Num[6];
	unsigned char  Attribute;
	unsigned short Index;
 } DISPLAYNUM;
 
 typedef struct
{
//	unsigned char NeedRecCnt;			//��Ҫ���յ����ݳ���//
//	unsigned char NeedSendCnt;		//��Ҫ���͵ĳ���//
	unsigned char MeasBuf_Tx[10];	//�շ��ͻ�����//	
	unsigned char MeasBuf_Rx[10];	//�շ��ͻ�����//
//	unsigned char CommOk;			//ͨ����ɱ�־��0X55=ͨ�����//
	unsigned char Tx_Ptr;			//8207ͨ��ʱ��ָ��//		
	unsigned char Cmd;			//8207ͨ��ʱ��ָ��//	
	unsigned char Rx_Ptr;			//8207ͨ��ʱ��ָ��//
	unsigned char CommDly;			//ͨ�Ŷ�ʱ���������ʱ�����ͨ�ų���//
//	unsigned char ReadPowerState;
	unsigned char MeasState;
	unsigned char Comm_Enable;
	unsigned char Comm_Ptr;

}COMM_Measure;
 //˫�˵�Uartͨ��//
 typedef struct
 {
	 unsigned char NeedRecCnt;			 //��Ҫ���յ����ݳ���//
	 unsigned char NeedSendCnt; 	 //��Ҫ���͵ĳ���//
	 unsigned char RecBuf8213[100];  //���ջ�����//
	 unsigned char SendBuf8213[100]; //���ͻ�����//
	 unsigned char PhaType; 		 //���1=A�࣬2=B�࣬3=C��//
	 unsigned char Tx_Ptr;			 //8207ͨ��ʱ��ָ��//		 
	 unsigned char Rx_Ptr;			 //8207ͨ��ʱ��ָ��//
	 
	 unsigned char Cmd; 		 //8207ͨ��ʱ��ָ��//	 
	 unsigned char CommOk;			 //ͨ����ɱ�־��0X55=ͨ�����//
	 unsigned char Ptr8213; 		 //8207ͨ��ʱ��ָ��//
	 unsigned char CommDly; 		 //ͨ�Ŷ�ʱ���������ʱ�����ͨ�ų���//
	 unsigned char ReadPowerState;
	 
 }COMM8213;

 typedef struct
 {
	 unsigned char ReadCmd;			 //�Ĵ�����ַ
	 unsigned char* ReadData; 	 //��������ֵ
 }HT7017_Measure;

 typedef struct
 {
 	unsigned short 	Cmd;
	unsigned char length;
	unsigned char Comm_Enable;
	unsigned char Real_Ptr;			
	unsigned char EC_Ptr; 
	unsigned char Demand_Ptr; 
	unsigned char Data_Select; 
 }Double_Commu;

  typedef struct
 {
	unsigned char* ECRAds;
 //	unsigned short 	Addr;
	unsigned short  DataID;		 //ID��
//	unsigned char 	Logical_Name[6];
	unsigned char	len;
 } ID_OBIS;
 typedef struct
 {
 	unsigned short 	Addr;
	unsigned short  DataID;		 //ID��	
	unsigned short 	Time_Addr;
	unsigned short  TimeID;		 //ID��
	unsigned char 	Logical_Name[6];
	unsigned char	len;
 } ID_MAXDemand_OBIS;
 typedef struct
 {
	 unsigned char HT7017_U[4]; 
	 unsigned char HT7017_Ia[4]; 
	 unsigned char HT7017_Ib[4]; 	 
	 unsigned char HT7017_Hz[4]; 
	 unsigned char HT7017_P1[4]; 
	 unsigned char HT7017_Q1[4]; 
	 unsigned char HT7017_S1[4]; 
	 unsigned char HT7017_P2[4]; 
	 unsigned char HT7017_Q2[4]; 
	 
	 unsigned char HT7017_P[4]; 
	 unsigned char HT7017_Q[4]; 
	 unsigned char HT7017_EMU_Status[4]; 

	 unsigned char HT7017_ID[4]; 
	 unsigned char HT7017_I2Gain[4]; 
	 unsigned char HT7017_HFConst[4]; 
	 unsigned char HT7017_P1Gain[4]; 
	 unsigned char HT7017_Gphs1[4]; 
	 unsigned char HT7017_OffsetH[4]; 
	 unsigned char HT7017_OffsetL[4]; 
	 unsigned char HT7017_P2Gain[4]; 
	 unsigned char HT7017_Gphs2[4]; 
	 
 }HT7017_Meas;
 
typedef struct
{
	 long APowerPulP;
	 long BPowerPulP;
	 long SPowerPulP;
	 long APowerPulQ;
	 long BPowerPulQ;

	unsigned long Ia;
	unsigned long Ib;
//---------����ϵ��-----------
	unsigned long Urms_K;
	unsigned long Irms1_K;
	unsigned long Irms2_K;
	unsigned long Power1_K;
	unsigned long Power2_K;
	unsigned long PowerS_K;

}MEASURE7053;
//�û�Ȩ�޹���ű����..
typedef struct
{
	unsigned char		Class[2];
	unsigned char 	OBIS_Name[6];     //���磺00 01���ࣺID�� 00 00 2A 00 00 FF ���߼��豸����6���ֽڣ� 02�����ԣ�
	unsigned char	  State_Reserve1;	  //����1�ֽڿռ�..
	unsigned char	  State_Reserve2;	  //����1�ֽڿռ�..
}OBIS_TAB;
//�û�Ȩ�޹���ű����..
typedef struct
{
	unsigned char 	OBIS_Name[9];     //���磺00 01���ࣺID�� 00 00 2A 00 00 FF ���߼��豸����6���ֽڣ� 02�����ԣ�
	unsigned char	  State_Reserve1;	  //����1�ֽڿռ�..
}CLIENT_TAB;
#endif
