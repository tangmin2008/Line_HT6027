/*
*********************************************************************************************************
*                                              HT60XX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : ecc_example.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __ECC_EXAMPLE_C
//#undef   __ECC_EXAMPLE_C

#ifdef   __ECC_EXAMPLE_C

#include "ecc_example.h"
#include "ht6xxx_lib.h"
#include "string.h"

#if defined  HT502x             /* This File Only support HT502x */ 

/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             ���ر���
*********************************************************************************************************
*/
/* the K array  for SHA-256*/
const uint32_t SHA256_K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
		0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
		0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
		0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
		0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
		0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
		0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
		0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
		0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
		0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
		0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
		0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
		0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Signature operation
*
* ����˵��: ��Բ��������ǩ���㷨��ECDSA-S
*
* ��ڲ���: ECC_ECDSA_InitStruct    ECC���߲�����ʼ�������ṹ��ָ�룬��Ҫ����5������: 
*														1) pECCpAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�p : pECCpAddr[0]Ϊ��λ
*														2) pECCaAddr				uint32_t *�ͱ�������ָ�����洢�����ϵ����߲���A : pECCaAddr[0]Ϊ��λ
*														3) pECCnAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�n : pECCnAddr[0]Ϊ��λ
*														4) pECCGxAddr				uint32_t *�ͱ�������ָ�����洢����x���� : pECCGxAddr[0]Ϊ��λ
*														5) pECCGyAddr  			uint32_t *�ͱ�������ָ�����洢����y���� : pECCGyAddr[0]Ϊ��λ
*						ECDSA_S_InputStruct    ECDSA-S������������ṹ��ָ�룬��Ҫ����3������: 
*								           	6) pdAddr						uint32_t *�ͱ�������ָ�����洢˽Կd : pdAddrΪ��λ            
*														7) pkAddr  					uint32_t *�ͱ�������ָ�����洢���������k: pkAddr[0]Ϊ��λ
*									   				8) pMAddr						uint32_t *�ͱ�������ָ�����洢HASH�����ϢժҪm: pMAddr[0]Ϊ��λ
*
*						ECDSA_S_OutputStruct   ECC���������ݽṹ��ָ�룬��Ҫ����10������: 
*													1) pRxAddr				uint32_t *�ͱ�����ECC��������x����/ECDSAǩ�����r: pRxAddr[0]Ϊ��λ
*													2) pRyAddr				uint32_t *�ͱ�����ECC��������y����/ECDSAǩ�����s: pRyAddr[0]Ϊ��λ
* ���ز���: Bool   = TRUE  ��ǩ�����
*									 = FALSE ��ǩ��fail��������������
* 
* ����˵��: 
*********************************************************************************************************
*/
Bool ECDSA_Signature(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Signature_InputTypedef* ECDSA_S_InputStruct,ECC_PointOperate_OutputTypedef* ECDSA_S_OutputStruct)
{
				
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,ENABLE);						/*��ECCģ��ʱ��									*/
	
		HT_ECC_ECDSA_Init(ECC_ECDSA_InitStruct);										/*ECC������ʼ��										*/
	
		HT_ECC_ECDSA_S(ECDSA_S_InputStruct);												/*����ǩ������										*/
	
		while(!HT_ECC_ITFlagStatusGet(ECC_IF));											/*��������ж�										*/
		HT_ECC_ClearITPendingBit(ECC_IF);														/*�����־												*/
	
		if(RESET == HT_ECC_ITFlagStatusGet(ECC_FLG_ECDSA_S))				/*�ж�ǩ�����Ա�־								*/
		{
				HT_ECC_PointOperate_Read(ECDSA_S_OutputStruct);					/*ǩ����ɣ����ǩ����ǩ					*/
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*�ر�ECCģ��ʱ��									*/
				return(TRUE);
		}
		else
		{
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*�ر�ECCģ��ʱ��									*/
				return(FALSE);																					/*ǩ��ʧ�ܣ�������������					*/
		}
}

/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Verification operation
*
* ����˵��: ��Բ��������ǩ���㷨��ECDSA-V
*
* ��ڲ���: ECC_ECDSA_InitStruct    ECC���߲�����ʼ�������ṹ��ָ�룬��Ҫ����5������: 
*														1) pECCpAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�p : pECCpAddr[0]Ϊ��λ
*														2) pECCaAddr				uint32_t *�ͱ�������ָ�����洢�����ϵ����߲���A : pECCaAddr[0]Ϊ��λ
*														3) pECCnAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�n : pECCnAddr[0]Ϊ��λ
*														4) pECCGxAddr				uint32_t *�ͱ�������ָ�����洢����x���� : pECCGxAddr[0]Ϊ��λ
*														5) pECCGyAddr  			uint32_t *�ͱ�������ָ�����洢����y���� : pECCGyAddr[0]Ϊ��λ
* 					ECC_ECDSA_V_InputStruct    ECDSA-V������������ṹ��ָ�룬��Ҫ����10������: 
*								           	6) pECCDxAddr				uint32_t *�ͱ�������ָ�����洢��Կx���� : pECCDxAddr[0]Ϊ��λ            
*														7) pECCDyAddr  			uint32_t *�ͱ�������ָ�����洢��Կy���� : pECCDyAddr[0]Ϊ��λ
*									   				8) pMAddr						uint32_t *�ͱ�������ָ�����洢HASH�����ϢժҪm: pMAddr[0]Ϊ��λ
*														9) prAddr;					uint32_t *�ͱ�������ָ�����洢ǩ�����r����:prAddr[0]Ϊ��λ
*														10)psAddr;					uint32_t *�ͱ�������ָ�����洢ǩ�����s����:psAddr[0]Ϊ��λ
*
* ���ز���: Bool   = TRUE  ����֤ͨ��
*									 = FALSE ����֤fail
* 
* ����˵��: 
*********************************************************************************************************
*/
Bool ECDSA_Verification(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct)
{
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,ENABLE);						/*��ECCģ��ʱ��									*/
	
		HT_ECC_ECDSA_Init(ECC_ECDSA_InitStruct);										/*ECC������ʼ��										*/
	
		HT_ECC_ECDSA_V(ECDSA_V_InputStruct);												/*������֤����										*/
	
		while(!HT_ECC_ITFlagStatusGet(ECC_IF));											/*��������ж�										*/
		HT_ECC_ClearITPendingBit(ECC_IF);														/*�����־												*/
	
		if(SET == HT_ECC_ITFlagStatusGet(ECC_FLG_ECDSA_V))					/*�ж���֤��־										*/
		{
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*�ر�ECCģ��ʱ��									*/
				return(TRUE);																						/*��֤ͨ��												*/
		}
		else
		{
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*�ر�ECCģ��ʱ��									*/
				return(FALSE);																					/*��֤ʧ��												*/
		}
}
/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Verification operation
*
* ����˵��: ��Բ��������ǩ���㷨��ECDSA-V,����㷨for HT502XA
*
* ��ڲ���: ECC_ECDSA_InitStruct    ECC���߲�����ʼ�������ṹ��ָ�룬��Ҫ����5������: 
*														1) pECCpAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�p : pECCpAddr[0]Ϊ��λ
*														2) pECCaAddr				uint32_t *�ͱ�������ָ�����洢�����ϵ����߲���A : pECCaAddr[0]Ϊ��λ
*														3) pECCnAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�n : pECCnAddr[0]Ϊ��λ
*														4) pECCGxAddr				uint32_t *�ͱ�������ָ�����洢����x���� : pECCGxAddr[0]Ϊ��λ
*														5) pECCGyAddr  			uint32_t *�ͱ�������ָ�����洢����y���� : pECCGyAddr[0]Ϊ��λ
* 					ECC_ECDSA_V_InputStruct    ECDSA-V������������ṹ��ָ�룬��Ҫ����10������: 
*								           	6) pECCDxAddr				uint32_t *�ͱ�������ָ�����洢��Կx���� : pECCDxAddr[0]Ϊ��λ            
*														7) pECCDyAddr  			uint32_t *�ͱ�������ָ�����洢��Կy���� : pECCDyAddr[0]Ϊ��λ
*									   				8) pMAddr						uint32_t *�ͱ�������ָ�����洢HASH�����ϢժҪm: pMAddr[0]Ϊ��λ
*														9) prAddr;					uint32_t *�ͱ�������ָ�����洢ǩ�����r����:prAddr[0]Ϊ��λ
*														10)psAddr;					uint32_t *�ͱ�������ָ�����洢ǩ�����s����:psAddr[0]Ϊ��λ
*
* ���ز���: Bool   = TRUE  ����֤ͨ��
*									 = FALSE ����֤fail
* 
* ����˵��: 
*********************************************************************************************************
*/
Bool ECDSA_Verification_HalfSoft(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct)
{
		uint8_t i;
		uint32_t *pTemp0,*pTemp1,*pTemp2,*pTemp3,*pTemp4,*pTemp5;
		ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct_backup;
		ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct_backup;
		ECC_ModOperate_InputTypedef ECCtemp;
		ECC_PointOperate_InitTypedef ECCPoint_Init;
		ECC_PointOperate_InputTypedef ECCPointtemp;
		Parameter256bits_TypeDef	ECDSAtemp_w,ECDSAtemp_u1,ECDSAtemp_u2,ECCmodtemp;
		ECC_PointOperate_OutputTypedef EccPointtemp_X1,EccPointtemp_X2;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,ENABLE);						/*��ECCģ��ʱ��									*/
	
		ECC_ECDSA_InitStruct_backup = ECC_ECDSA_InitStruct;					/*��ʼ���ṹ��ָ�뱸��						*/
		ECDSA_V_InputStruct_backup = ECDSA_V_InputStruct;						/*��������ṹ��ָ�뱸��					*/
		pTemp0 = ECC_ECDSA_InitStruct_backup->pECCnAddr;
		pTemp1 = ECC_ECDSA_InitStruct_backup->pECCnAddr;
		pTemp2 = ECDSA_V_InputStruct_backup->prAddr;
		pTemp3 = ECDSA_V_InputStruct_backup->psAddr;
		pTemp4 = ECDSA_V_InputStruct_backup->prAddr;
		pTemp5 = ECDSA_V_InputStruct_backup->psAddr;
	
		/* 1)����ǩ���Ƿ�������[1, n-1]�ڵ�����.���κ�һ������ʧ��,�򷵻�("�ܾ���ǩ��")*/
		if((GREATER == BigDatCompare(pTemp0,pTemp2,8)) && 
			 (GREATER == BigDatCompare(pTemp1,pTemp3,8)) &&
			 (TRUE == BigDatNotZero(pTemp4,8)) &&
			 (TRUE == BigDatNotZero(pTemp5,8)) )
		{
		/* 2)����s����Ԫs1,������w = s1 mod n.    */
				ECCtemp.ModMode = ModInv_Mode;
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSA_V_InputStruct_backup->psAddr;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				HT_ECC_ModOperate_Read(ECDSAtemp_w.u32);
			
		/* 3)����u1 = (M*w) mod n �� u2 = (r*w) mod n. */	
				ECCtemp.ModMode = ModMul_Mode;
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSA_V_InputStruct_backup->pMAddr;
				ECCtemp.pPyAddr = ECDSAtemp_w.u32;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				HT_ECC_ModOperate_Read(ECDSAtemp_u1.u32);
			
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSA_V_InputStruct_backup->prAddr;
				ECCtemp.pPyAddr = ECDSAtemp_w.u32;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				HT_ECC_ModOperate_Read(ECDSAtemp_u2.u32);				
			
		/* 4)����X = u1*G +u2*D.  */
				ECCPoint_Init.pECCaAddr = ECC_ECDSA_InitStruct_backup->pECCaAddr;
				ECCPoint_Init.pECCpAddr = ECC_ECDSA_InitStruct_backup->pECCpAddr;
				HT_ECC_PointOperate_Init(&ECCPoint_Init);
				
				ECCPointtemp.PointMode = EccMul_Mode;
				ECCPointtemp.pECCPxAddr = ECC_ECDSA_InitStruct_backup->pECCGxAddr;
				ECCPointtemp.pECCPyAddr = ECC_ECDSA_InitStruct_backup->pECCGyAddr;
				ECCPointtemp.pKscalAddr = ECDSAtemp_u1.u32;
				HT_ECC_PointOperate(&ECCPointtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				EccPointtemp_X1.pRxAddr = ECDSAtemp_w.u32;							/*����ṹ��ָ���ʼ��						*/
				EccPointtemp_X1.pRyAddr = ECDSAtemp_u1.u32;
				HT_ECC_PointOperate_Read(&EccPointtemp_X1);							/*u1*G ����ECDSAtemp_w&ECDSAtemp_u1��	*/

				ECCPointtemp.PointMode = EccMul_Mode;										/* ���														*/
				ECCPointtemp.pECCPxAddr = ECDSA_V_InputStruct_backup->pECCDxAddr;
				ECCPointtemp.pECCPyAddr = ECDSA_V_InputStruct_backup->pECCDyAddr;
				ECCPointtemp.pKscalAddr = ECDSAtemp_u2.u32;
				HT_ECC_PointOperate(&ECCPointtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				EccPointtemp_X2.pRxAddr = ECDSAtemp_u2.u32;							/*����ṹ��ָ���ʼ��						*/
				EccPointtemp_X2.pRyAddr = ECCmodtemp.u32;				
				HT_ECC_PointOperate_Read(&EccPointtemp_X2);							/*u2*D ����ECDSAtemp_u2&ECCmodtemp��	*/

				
				ECCPointtemp.PointMode = EccAdd_Mode;										/* ���														*/
				ECCPointtemp.pECCPxAddr = ECDSAtemp_w.u32;							/*u1*G��x����											*/
				ECCPointtemp.pECCPyAddr = ECDSAtemp_u1.u32;							/*u1*G��y����											*/
				ECCPointtemp.pECCSxAddr = ECDSAtemp_u2.u32;							/*u2*D��x����											*/
				ECCPointtemp.pECCSyAddr = ECCmodtemp.u32;								/*u2*D��y����											*/
				HT_ECC_PointOperate(&ECCPointtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				EccPointtemp_X1.pRxAddr = ECDSAtemp_w.u32;							/*����ṹ��ָ���ʼ��						*/
				EccPointtemp_X1.pRyAddr = ECDSAtemp_u1.u32;
				HT_ECC_PointOperate_Read(&EccPointtemp_X1);							/*u1*G +u2*D ����ECDSAtemp_w&ECDSAtemp_u1��	*/
			
		/* 5)��X = �����, �򷵻�("�ܾ���ǩ��") */	
		/* 6)��X��x1����ת��Ϊ����t, ���� w = t mod n. */	
				for(i=0;i<8;i++)
				{
					ECCmodtemp.u32[i] = 0x0;
				}
				ECCtemp.ModMode = ModAdd_Mode;
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSAtemp_w.u32;											/*X��x1����												*/
				ECCtemp.pPyAddr = ECCmodtemp.u32;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*��������ж�										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*�����־												*/
				HT_ECC_ModOperate_Read(ECCmodtemp.u32);

		/* 7����w= r, �򷵻�("���ܸ�ǩ��"); ����,����("�ܾ���ǩ��").  */
				pTemp0 = ECDSA_V_InputStruct_backup->prAddr;
				if( EQUAL ==BigDatCompare(pTemp0,ECCmodtemp.u32,8))
				{
					return(TRUE);
				}
				else
				{
					return(FALSE);
				}
		}
		else
		{
			return(FALSE);
		}
}

BigdatCompare_TypeDef BigDatCompare(uint32_t *Data1,uint32_t *Data2,uint16_t	DataLen)
{
		uint16_t i;
		uint32_t *pTemp1,*pTemp2;
	
		pTemp1=Data1;
		pTemp2=Data2;
		for(i=0; i<DataLen; i++)
		{
			if((*(pTemp1+DataLen-i-1)) > (*(pTemp2+DataLen-i-1)))
			{
				return(GREATER);
			}
			else if((*(pTemp1+DataLen-i-1)) < (*(pTemp2+DataLen-i-1)))
			{
				return(LESS);
			}
		}
		return(EQUAL);
}

Bool BigDatNotZero(uint32_t *Data1,uint16_t	DataLen)
{
		uint16_t i;
		uint32_t *pTemp1;
		
		pTemp1=Data1;
		for(i=0; i<DataLen; i++)
		{
			if((*(pTemp1+DataLen-i-1)) != 0)
			{
				return(TRUE);
			}
		}
		return(FALSE);
}
/*
*********************************************************************************************************
*                              SHA-256 �㷨���ʵ��
*
* ����˵��: HASH�㷨��SHA-256
*
* ��ڲ���: SHA256_InputTypedef    SHA256�����ʼ�������ṹ��ָ�룬��Ҫ����5������: 
*														1) pMtestAddr				uint8_t *�ͱ�������ָ������������Ϣm : pMtestAddr[0]Ϊ��λ
*														2) LenMtest					uint64_t �ͱ��������������Ϣ���ȣ�byte��
* 					pHashAddr     uint8_t *�ͱ�������ָ����������HASH���: pHashAddr[0]Ϊ��λ
*
* ����˵��: 
*********************************************************************************************************
*/
void SHA256(SHA256_InputTypedef* SHA256_InputStruct,uint8_t *pHashAddr)
{
	SHA256_TempTypedef SHA256_Temp;
	
	SHA256_Init(&SHA256_Temp);
	SHA256_Process(&SHA256_Temp,SHA256_InputStruct,pHashAddr);
}

/* init the SHA256 HASH��ֵ */
void SHA256_Init(SHA256_TempTypedef* SHA256_Temp)
{
		SHA256_Temp->MtestLen = 0;																		//��Ϣ���ȳ�ʼ������λbit��
		SHA256_Temp->MnCurlen = 0;																		//ÿ����Ϣ��512-bits��ʵʱ���ȣ���λbyte��
    SHA256_Temp->HashValue[0] = 0x6A09E667UL;											//SHA256 HASH��ֵHn(0)��n=0~7
    SHA256_Temp->HashValue[1] = 0xBB67AE85UL;
    SHA256_Temp->HashValue[2] = 0x3C6EF372UL;
    SHA256_Temp->HashValue[3] = 0xA54FF53AUL;
    SHA256_Temp->HashValue[4] = 0x510E527FUL;
    SHA256_Temp->HashValue[5] = 0x9B05688CUL;
    SHA256_Temp->HashValue[6] = 0x1F83D9ABUL;
    SHA256_Temp->HashValue[7] = 0x5BE0CD19UL;
}

/* SHA256 ����		*/
/* ��ʼ��sha-256��Ϣ�ṹ, ����Ϣ����, ��ѹ����Ϣ�����64�ֽ�. */
void SHA256_Process(SHA256_TempTypedef* SHA256_Temp,SHA256_InputTypedef* SHA256_InputStruct,uint8_t *pHashAddr)
{
    uint8_t i;
	
		/* process ��1~N-1�� 512-bits message  ����ǰN-1�� ��������*/   
		while(SHA256_InputStruct->LenMtest--)
		{
				SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen] = *SHA256_InputStruct->pMtestAddr++;					
																																					/*  copy message to temp  		*/
				SHA256_Temp->MnCurlen++;
				if(SHA256_Temp->MnCurlen == 64)
				{
					SHA256_Compress(SHA256_Temp);																		/*  Compress 512-bit message  */
					SHA256_Temp->MtestLen += 512;																		/*  ��Ϣ�����ۼ� 512 bits			*/
					SHA256_Temp->MnCurlen = 0;																			/*	�ֶμ�����������					*/
				}
		}
		/* process the Nth 512-bits message  �������512-BIT �������� */
		SHA256_Temp->MtestLen += (SHA256_Temp->MnCurlen * 8);									/* �ۼ���Ϣ���ȣ���λbit�� 	  */
		SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen++] = 0x80;										/* ��Ϣ��λappend the '1' bit */
		/* if the length is currenlly above 56 bytes we append zeros�then compress. 
     * Then we can fall back to padding zeros and length ��encoding like normal.*/
		if(SHA256_Temp->MnCurlen >= 56)																				/* MnCurlen>=56,��4bytes��ȫ������*/
		{
				for(; SHA256_Temp->MnCurlen < 64;)
				{
						SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen++] = 0;							/* ���4bytes������						*/
				}
				SHA256_Compress(SHA256_Temp);																			/*  Compress 512-bit message  */
				SHA256_Temp->MnCurlen = 0;																				/*	�ֶμ�����������					*/
		}
		for(; SHA256_Temp->MnCurlen < 56;)
        SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen++] = 0;  								/* pad upto 56 bytes of zeroes */
    for(i = 56; i < 64; i++)
        SHA256_Temp->MnBuf[i] = (uint8_t)((SHA256_Temp->MtestLen >> ((63 - i) * 8)) & 255);
																																					/* append length 							*/
		SHA256_Compress(SHA256_Temp);																					/*  Compress 512-bit message  */
    /* copy output */
    for(i = 0; i < 32; i++)
        *pHashAddr++ = (uint8_t)((SHA256_Temp->HashValue[(31-i) >> 2] >> ((i & 3) << 3)) & 255);
																																						
}

/*  Compress 512-bit message  */
void SHA256_Compress(SHA256_TempTypedef* SHA256_Temp)
{
		uint32_t	Sha_a, Sha_b,Sha_c,Sha_d,Sha_e,Sha_f,Sha_g,Sha_h,Sha_W[64], Sha_T1, Sha_T2;
		uint8_t	i;
	
		/* ��ʼ����������a��b,c,d,e,f,g,h	��Initialize the eight working variables	*/
		Sha_a = SHA256_Temp->HashValue[0];
		Sha_b = SHA256_Temp->HashValue[1];
		Sha_c = SHA256_Temp->HashValue[2];
		Sha_d = SHA256_Temp->HashValue[3];
		Sha_e = SHA256_Temp->HashValue[4];
		Sha_f = SHA256_Temp->HashValue[5];
		Sha_g = SHA256_Temp->HashValue[6];
		Sha_h = SHA256_Temp->HashValue[7];
		/* ׼����Ϣ���ȱ�Prepare the message schedule  */
    for(i=0; i<16; i++)																			    /* copy the state into 512-bits into W[i] */
        Sha_W[i] = (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 0]) << 24) |
									 (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 1]) << 16) |
									 (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 2]) << 8)  |
									 (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 3]));
    for(i=16; i<64; i++)																				/* fill W[16..63] */
        Sha_W[i] = Gamma1(Sha_W[i-2]) + Sha_W[i-7] + Gamma0(Sha_W[i-15]) + Sha_W[i-16];

    /* 64�ε������㣬Compress */
    for(i=0; i<64; i++) 
		{
        Sha_T1 = Sha_h + Sigma1(Sha_e) + Ch(Sha_e,Sha_f,Sha_g) + SHA256_K[i] + Sha_W[i];
        Sha_T2 = Sigma0(Sha_a) + Maj(Sha_a,Sha_b,Sha_c);
        Sha_h = Sha_g;
        Sha_g = Sha_f;
        Sha_f = Sha_e;
        Sha_e = Sha_d + Sha_T1;
        Sha_d = Sha_c;
				Sha_c = Sha_b;
        Sha_b = Sha_a;
        Sha_a = Sha_T1 + Sha_T2;
    }

    /* ����hashֵ��feedback */
    SHA256_Temp->HashValue[0] += Sha_a;
		SHA256_Temp->HashValue[1] += Sha_b;
		SHA256_Temp->HashValue[2] += Sha_c;
		SHA256_Temp->HashValue[3] += Sha_d;
		SHA256_Temp->HashValue[4] += Sha_e;
		SHA256_Temp->HashValue[5] += Sha_f;
		SHA256_Temp->HashValue[6] += Sha_g;
		SHA256_Temp->HashValue[7] += Sha_h;
}

/*
*********************************************************************************************************
*                                ECDSA EXAMPLES FOR HT502x
*
* ����˵��: ���ÿ⺯��ʵ��ECDSAǩ������֤
*
* ��ڲ���: ��                            
*
* ���ز���: uint8_t
*						   			bit0��	0,signiture fail 
*														1,signiture pass 
*							 			bit1��	0,verification fail
*														1,verification pass
*							 			bit2��	0,halfsoftware verification fail
*														1,halfsoftware verification pass
*										bit3��	0,SHA256 fail
*														1,SHA256 pass
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/

/*��������*/	
Parameter256bits_TypeDef 	ECC_p,ECC_a,ECC_b,ECC_Gx,ECC_Gy,ECC_n;		//ECC����(Ep(a,b))����������Ľ�p������ϵ��a,b������G(x,y)������Ľ�n
Parameter256bits_TypeDef	ECC_Dx,ECC_Dy,ECC_M,ECC_s,ECC_r;		//��ԿD(x,y)����ϢժҪM��ǩ�����(r,s)
Parameter256bits_TypeDef	ECC_d,ECC_k;												//˽Կd�����������k
Parameter1024bits_TypeDef	SHA_m;															//��Ϣ
Parameter256bits_TypeDef	SHA_M;															//��ϢժҪSHA_M=sha256(SHA_m)
uint64_t	LenSHA_m;																						//��Ϣ���ȣ�byte��

uint8_t ECC_ECDSA_Example(void)
{
		CMU_InitTypeDef  CMU_InitStructure;
		ECC_ECDSA_InitTypedef ECC_ECDSA_InitStruct;
		ECC_ECDSA_Signature_InputTypedef ECDSA_S_InputStruct;
		ECC_PointOperate_OutputTypedef ECDSA_S_OutputStruct;
		ECC_ECDSA_Verification_InputTypedef ECDSA_V_InputStruct;
		Parameter256bits_TypeDef ECC_tmp0,ECC_tmp1; 
		SHA256_InputTypedef SHA256_InputStruct;
		uint8_t i,ReturnTemp;

/*ECDSA���������ʼ��*/
		ReturnTemp = 0x0;
		ECDSA_TestCase_Init();
/*����ϵͳʱ��*/    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv16;				/*!<	HT502XA �˷�Ƶ������ڵ���16��Ƶ��ECDSA��֤�����PKV������ܹ����������������㲻��Ӱ��*/
    HT_CMU_Init(&CMU_InitStructure);
	
//----ESDSAǩ������-----------hardwareʵ��--------		
/*ECDSA��ʼ���ṹ��ָ��*/												/*�˴�ÿ���������㶼Ҫ��ʼ���ṹ��ָ��*/
		ECC_ECDSA_InitStruct.pECCaAddr = ECC_a.u32;
		ECC_ECDSA_InitStruct.pECCpAddr = ECC_p.u32;
		ECC_ECDSA_InitStruct.pECCGxAddr = ECC_Gx.u32;
		ECC_ECDSA_InitStruct.pECCGyAddr = ECC_Gy.u32;
		ECC_ECDSA_InitStruct.pECCnAddr = ECC_n.u32;
/*ECDSAǩ����������ṹ���ʼ��*/	
		ECDSA_S_InputStruct.pdAddr = ECC_d.u32;
		ECDSA_S_InputStruct.pkAddr = ECC_k.u32;
		ECDSA_S_InputStruct.pMAddr = ECC_M.u32;
/*ECDSA����ṹ���ʼ��*/
		ECDSA_S_OutputStruct.pRxAddr = ECC_tmp0.u32;
		ECDSA_S_OutputStruct.pRyAddr = ECC_tmp1.u32;
		for(i=0;i<8;i++)
		{
			ECC_tmp0.u32[i] = 0x00000000;
			ECC_tmp1.u32[i] = 0x00000000;
		}
/*ECDSAǩ������*/		
		if(TRUE == ECDSA_Signature(&ECC_ECDSA_InitStruct,&ECDSA_S_InputStruct,&ECDSA_S_OutputStruct))
		{
			if( (EQUAL ==BigDatCompare(ECC_tmp0.u32,ECC_r.u32,8)) && 
					(EQUAL ==BigDatCompare(ECC_tmp1.u32,ECC_s.u32,8)))
			{
					ReturnTemp |= 0x1;						//PASS
			}
			else
			{
					ReturnTemp &= (~0x1);					//FAIL
			}				
		}
		else
		{
			ReturnTemp &= (~0x1);							//FAIL
		}
//----ESDSAǩ������-----------hardwareʵ��--------END		
		
//----ECDSA��֤����-----------hardwareʵ��--------	
/*ECDSA��ʼ���ṹ��ָ��*/											/*�˴�ÿ���������㶼Ҫ��ʼ���ṹ��ָ��*/
		ECC_ECDSA_InitStruct.pECCaAddr = ECC_a.u32;
		ECC_ECDSA_InitStruct.pECCpAddr = ECC_p.u32;
		ECC_ECDSA_InitStruct.pECCGxAddr = ECC_Gx.u32;
		ECC_ECDSA_InitStruct.pECCGyAddr = ECC_Gy.u32;
		ECC_ECDSA_InitStruct.pECCnAddr = ECC_n.u32;		
/*ECDSA��֤��������ṹ���ʼ��*/
		ECDSA_V_InputStruct.pECCDxAddr = ECC_Dx.u32;
		ECDSA_V_InputStruct.pECCDyAddr = ECC_Dy.u32;
		ECDSA_V_InputStruct.pMAddr = ECC_M.u32;
		ECDSA_V_InputStruct.prAddr = ECC_r.u32;
		ECDSA_V_InputStruct.psAddr = ECC_s.u32;
/*ECDSA��֤����*/	
		if(TRUE == ECDSA_Verification(&ECC_ECDSA_InitStruct,&ECDSA_V_InputStruct))
		{
				ReturnTemp |= (0x2);							//PASS
		}
		else
		{
				ReturnTemp &= (~0x2);							//FAIL
		}
//----ECDSA��֤����-----------hardwareʵ��--------END
		
//----ECDSA��֤����------half softwareʵ��--------	
/*ECDSA��ʼ���ṹ��ָ��*/											/*�˴�ÿ���������㶼Ҫ��ʼ���ṹ��ָ��*/
		ECC_ECDSA_InitStruct.pECCaAddr = ECC_a.u32;
		ECC_ECDSA_InitStruct.pECCpAddr = ECC_p.u32;
		ECC_ECDSA_InitStruct.pECCGxAddr = ECC_Gx.u32;
		ECC_ECDSA_InitStruct.pECCGyAddr = ECC_Gy.u32;
		ECC_ECDSA_InitStruct.pECCnAddr = ECC_n.u32;		
/*ECDSA��֤��������ṹ���ʼ��*/
		ECDSA_V_InputStruct.pECCDxAddr = ECC_Dx.u32;
		ECDSA_V_InputStruct.pECCDyAddr = ECC_Dy.u32;
		ECDSA_V_InputStruct.pMAddr = ECC_M.u32;
		ECDSA_V_InputStruct.prAddr = ECC_r.u32;
		ECDSA_V_InputStruct.psAddr = ECC_s.u32;
/*ECDSA��֤����*/	
		if(TRUE == ECDSA_Verification_HalfSoft(&ECC_ECDSA_InitStruct,&ECDSA_V_InputStruct))
		{
				ReturnTemp |= (0x4);
		}
		else
		{
				ReturnTemp &= (~0x4);
		}		
//----ECDSA��֤����------half softwareʵ��--------END
		
//----SHA256����--------------softwareʵ��--------			
/*SHA256 ���������ʼ��*/
		SHA256_TestCase_Init();
/*SHA256��ʼ���ṹ��ָ��*/											/*�˴�ÿ���������㶼Ҫ��ʼ���ṹ��ָ��*/
		SHA256_InputStruct.pMtestAddr = SHA_m.u8;
		SHA256_InputStruct.LenMtest = LenSHA_m;
/*SHA256����*/		
		SHA256(&SHA256_InputStruct,SHA_M.u8);
		if( EQUAL == BigDatCompare(ECC_M.u32,SHA_M.u32,8))
		{
				ReturnTemp |= (0x8);
		}
		else
		{
				ReturnTemp &= (~0x8);
		}
//----SHA256����--------------softwareʵ��--------END	
		
		return(ReturnTemp);
}

/*
*********************************************************************************************************
*                              ECDSA test case initial
*
* ����˵��: ESDSA testcase ��ʼ��
*
* ��ڲ���: 
*
* ����˵��: 
*********************************************************************************************************
*/
void ECDSA_TestCase_Init(void)
{

		ECC_p.u64[3] = 0xFFFFFFFF00000001;			//p=2[256]-2[224]+2[192]+2[96]-1
		ECC_p.u64[2] = 0x0000000000000000;
		ECC_p.u64[1] = 0x00000000FFFFFFFF;
		ECC_p.u64[0] = 0xFFFFFFFFFFFFFFFF;
	
		ECC_a.u64[3] = 0xFFFFFFFF00000001;			//a=-3 mod p
		ECC_a.u64[2] = 0x0000000000000000;
		ECC_a.u64[1] = 0x00000000FFFFFFFF;
		ECC_a.u64[0] = 0xFFFFFFFFFFFFFFFC;
	
		ECC_b.u64[3] = 0x5AC635D8AA3A93E7;			//b
		ECC_b.u64[2] = 0xB3EBBD55769886BC;
		ECC_b.u64[1] = 0x651D06B0CC53B0F6;
		ECC_b.u64[0] = 0x3BCE3C3E27D2604B;
	
		ECC_Gx.u64[3] = 0x6B17D1F2E12C4247;			//Gx
		ECC_Gx.u64[2] = 0xF8BCE6E563A440F2;
		ECC_Gx.u64[1] = 0x77037D812DEB33A0;
		ECC_Gx.u64[0] = 0xF4A13945D898C296;
	
		ECC_Gy.u64[3] = 0x4FE342E2FE1A7F9B;			//Gy
		ECC_Gy.u64[2] = 0x8EE7EB4A7C0F9E16;
		ECC_Gy.u64[1] = 0x2BCE33576B315ECE;
		ECC_Gy.u64[0] = 0xCBB6406837BF51F5;		
	
		ECC_n.u64[3] = 0xFFFFFFFF00000000;			//n
		ECC_n.u64[2] = 0xFFFFFFFFFFFFFFFF;
		ECC_n.u64[1] = 0xBCE6FAADA7179E84;
		ECC_n.u64[0] = 0xF3B9CAC2FC632551;

		ECC_M.u64[3] = 0x76823489310398B8;			//hash(m)
		ECC_M.u64[2] = 0x30799E95A8A1819B;
		ECC_M.u64[1] = 0x0FC44253C1ED704C;
		ECC_M.u64[0] = 0xB6030C8248BA22FE;
		
		ECC_d.u64[3] = 0x101D35748FA6C773;			//˽Կd
		ECC_d.u64[2] = 0xDEF6BBBC4E31D720;
		ECC_d.u64[1] = 0xD7CB07C369060856;
		ECC_d.u64[0] = 0xA4EE9F7A9905C315;
		
		ECC_k.u64[3] = 0x732B578FCD30BA63;			//���������k
		ECC_k.u64[2] = 0x6965A177596065A1;
		ECC_k.u64[1] = 0x835C026B3BBCE990;
		ECC_k.u64[0] = 0xF360AD51AB6EC9CA;

		ECC_Dx.u64[3] = 0xCA9FDDF2F526D6A3;			//��ԿD(x,y)
		ECC_Dx.u64[2] = 0x3D1CE42A5B1DDB74;
		ECC_Dx.u64[1] = 0xF1809CADAB9AC14C;
		ECC_Dx.u64[0] = 0x84B1AF00A3B7CE8E;	
		
		ECC_Dy.u64[3] = 0xC5F7CD9057622549;
		ECC_Dy.u64[2] = 0x17B1C6D5E6535613;
		ECC_Dy.u64[1] = 0xC2BB7079F54E7F2D;
		ECC_Dy.u64[0] = 0x9B4A8CFB91A2C035;	

		ECC_s.u64[3] = 0xFDAB2982C6FBE1A1;		//ǩ�����(r,s)
		ECC_s.u64[2] = 0x24C854F8C94FD52A;
		ECC_s.u64[1] = 0xE4B694D04554B84F;
		ECC_s.u64[0] = 0x74FF7D4FA543B25D;			

		ECC_r.u64[3] = 0x124C6384C629842A;
		ECC_r.u64[2] = 0x92ED98844EA505FF;
		ECC_r.u64[1] = 0x0BF33C456AA4ABA8;
		ECC_r.u64[0] = 0x9C3D84EC3ABDF10E;	
	
}
/*
*********************************************************************************************************
*                              SHA256 test case initial
*
* ����˵��: SHA256 testcase ��ʼ��
*
* ��ڲ���: 
*
* ����˵��: 
*********************************************************************************************************
*/
void SHA256_TestCase_Init(void)
{
			uint8_t i;
//			uint8_t Mtemp[] = "qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy87923523bjk";
																											/* ��Ϣԭ��*/
			for(i=0;i<16;i++)
			{
				SHA_m.u64[i] = 0x0;														/*������Ϣ����*/
			}
//			for(i=0;i<sizeof(temp);i++)
//			{
//				SHA_m.u8[i] = Mtemp[i];											 /*��ʼ����Ϣ����*/
//			}

//case 0
		strcpy(SHA_m.u8,"qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy879qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy879");
																											/*��ʼ����Ϣ����*/
		ECC_M.u64[3] = 0x99bf8093cd9c98db;			//hash(m)
		ECC_M.u64[2] = 0x84fe52e83ebe9a3c;
		ECC_M.u64[1] = 0xa959180f2cc96663;
		ECC_M.u64[0] = 0x0ea3ca5e65d55e52;
		LenSHA_m = 896/8;			
//case 1
//		strcpy(SHA_m.u8,"qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy87923523bjk");
//																											/*��ʼ����Ϣ����*/
//		ECC_M.u64[3] = 0xb98b0260087cab2f;			//hash(m)
//		ECC_M.u64[2] = 0x0c04042874323779;
//		ECC_M.u64[1] = 0x9942fdbbc878ec7a;
//		ECC_M.u64[0] = 0x25e463312fefd726;
//		LenSHA_m = 512/8;
//case2
//		strcpy(SHA_m.u8,"qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy879");
//																											/*��ʼ����Ϣ����*/
//		ECC_M.u64[3] = 0x98c65f99b6613bc4;			//hash(m)
//		ECC_M.u64[2] = 0xa76a82abe6756b87;
//		ECC_M.u64[1] = 0xbf8f4fcd00fabeba;
//		ECC_M.u64[0] = 0x8173a25d7264e783;			
//		LenSHA_m = 448/8;

}


#endif                                        /* This File Only support HT6x2x and HT501x */


#endif  /* __EXAMPLE_C */
