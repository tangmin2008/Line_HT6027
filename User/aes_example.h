/*
**********************************************************************************************************
*                                              HT60XX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : example.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __AES_EXAMPLE_H__
#define __AES_EXAMPLE_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
#include "ht6xxx_aes&rand.h"
	
#if defined HT6x2x  ||  defined  HT501x             /* This File Only support HT6x2x and HT501x */ 
    
/*
*********************************************************************************************************
*                                           ȫ�ֺ�/�ṹ��
*********************************************************************************************************
*/
	
#pragma pack(8)
/* 
* @brief  GCM������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			uint8_t *pInitVectorAddr;						//��ʼ������ַָ��
			uint64_t LenIV;											//��ʼ�������ݳ��ȣ�byte��
			uint8_t	*pPtextAddr;								//�������ݵ�ַָ��
			uint64_t LenPtext;									//�������ݳ��ȣ�byte��
			uint8_t	*pAdataAddr;								//������֤���ݵ�ַָ��
			uint64_t LenAdata;									//������֤���ݳ��ȣ�byte��
}GCM_Encrypt_InputTypedef;										/*!< end of group GCM_Encrypt_InputTypedef*/

/* 
* @brief  GCM������������ṹ����
*/
typedef struct
{
			uint8_t	*pCtextAddr;									//�Ѽ����������ĵ�ַָ��
			uint64_t LenCtext;										//�Ѽ����������ĳ��ȣ�byte��
			Parameter128bits_TypeDef*	AuthTag;		//��֤��ǩ���ݵ�ַָ��
}GCM_Encrypt_OutputTypedef;											/*!< end of group GCM_Encrypt_OutputTypedef*/

/* 
* @brief  GCM������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			uint8_t *pInitVectorAddr;						//��ʼ������ַָ��
			uint64_t LenIV;											//��ʼ�������ݳ��ȣ�byte��
			uint8_t	*pCtextAddr;								//�������ݵ�ַָ��
			uint64_t LenCtext;									//�������ݳ��ȣ�byte��
			uint8_t	*pAdataAddr;								//������֤���ݵ�ַָ��
			uint64_t LenAdata;									//������֤���ݳ��ȣ�byte��
			Parameter128bits_TypeDef*	AuthTag;	//��֤��ǩ���ݵ�ַָ��
}GCM_Decrypt_InputTypedef;										/*!< end of group GCM_Encrypt_InputTypedef*/


/* 
* @brief  GCM������������ṹ����
*/
typedef struct
{
			uint8_t	*pPtextAddr;									//�ѽ����������ݵ�ַָ��
			uint64_t LenPtext;										//�ѽ����������ݳ��ȣ�byte��
}GCM_Decrypt_OutputTypedef;									/*!< end of group GCM_Encrypt_OutputTypedef*/
#pragma pack()

/* 
* @brief  ECB������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			uint8_t	*pPtextAddr;								//�������ݵ�ַָ��
			uint64_t LenPtext;									//�������ݳ��ȣ�byte��
}ECB_Encrypt_InputTypedef;										/*!< end of group ECB_Encrypt_InputTypedef*/

/* 
* @brief  ECB������������ṹ����
*/
typedef struct
{
			uint8_t	*pCtextAddr;									//�Ѽ����������ĵ�ַָ��
			uint64_t LenCtext;										//�Ѽ����������ĳ��ȣ�byte��
}ECB_Encrypt_OutputTypedef;											/*!< end of group ECB_Encrypt_OutputTypedef*/

/* 
* @brief  ECB������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			uint8_t	*pCtextAddr;								//�������ݵ�ַָ��
			uint64_t LenCtext;									//�������ݳ��ȣ�byte��
}ECB_Decrypt_InputTypedef;										/*!< end of group ECB_Decrypt_InputTypedef*/

/* 
* @brief  ECB������������ṹ����
*/
typedef struct
{
			uint8_t	*pPtextAddr;									//�ѽ������ĵ�ַָ��
			uint64_t LenPtext;										//�ѽ������ĳ��ȣ�byte��
}ECB_Decrypt_OutputTypedef;											/*!< end of group ECB_Decrypt_OutputTypedef*/

/* 
* @brief  CBC������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			uint8_t	*pPtextAddr;								//�������ݵ�ַָ��
			uint64_t LenPtext;									//�������ݳ��ȣ�byte��
			Parameter128bits_TypeDef* pInitVectorAddr;				//��ʼ������ַָ��
}CBC_Encrypt_InputTypedef;										/*!< end of group CBC_Encrypt_InputTypedef*/

/* 
* @brief  CBC������������ṹ����
*/
typedef struct
{
			uint8_t	*pCtextAddr;									//�Ѽ����������ĵ�ַָ��
			uint64_t LenCtext;										//�Ѽ����������ĳ��ȣ�byte��
}CBC_Encrypt_OutputTypedef;											/*!< end of group CBC_Encrypt_OutputTypedef*/

/* 
* @brief  CBC������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			uint8_t	*pCtextAddr;								//�������ݵ�ַָ��
			uint64_t LenCtext;									//�������ݳ��ȣ�byte��
			Parameter128bits_TypeDef* pInitVectorAddr;				//��ʼ������ַָ��
}CBC_Decrypt_InputTypedef;										/*!< end of group CBC_Decrypt_InputTypedef*/

/* 
* @brief  CBC������������ṹ����
*/
typedef struct
{
			uint8_t	*pPtextAddr;									//�ѽ������ĵ�ַָ��
			uint64_t LenPtext;										//�ѽ������ĳ��ȣ�byte��
}CBC_Decrypt_OutputTypedef;											/*!< end of group CBC_Decrypt_OutputTypedef*/

/* 
* @brief  CTR��/������������ṹ����
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//���ܽ���ģʽѡ��
			uint32_t *pKeyAddr;									//��Կ��ַָ��
			Parameter128bits_TypeDef* Counter0;	//Counter0��ַָ��
			uint8_t	*pInttextAddr;							//���ӽ������ݵ�ַָ��
			uint64_t LenInttext;								//���ӽ������ݳ��ȣ�byte��
}CTR_Encrypt_InputTypedef;										/*!< end of group CTR_Encrypt_InputTypedef*/


/* 
* @brief  GCM������������ṹ����
*/
typedef struct
{
			uint8_t	*pOuttextAddr;							//�Ѽ�/�����������ĵ�ַָ��
			uint64_t LenOuttext;								//�Ѽ�/�����������ĳ��ȣ�byte��
}CTR_Encrypt_OutputTypedef;									/*!< end of group GCM_Encrypt_OutputTypedef*/

/*
*********************************************************************************************************
*                                             ȫ�ֱ���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ȫ�ֺ�������
*********************************************************************************************************
*/
void GCM_GhashHeader_init(AESKeyMode_TypeDef AESKeyMode,uint32_t *pKeyAddr,uint32_t *pOutDataAddr);
void GCM_GhashAuth(uint32_t *GhashHeaderAddr,uint8_t *pADataAddr,uint64_t LenAData,uint8_t *pCtextAddr,uint64_t LenCtext,Parameter128bits_TypeDef* OutData);
void GCM_Counter0_init(uint8_t *pInitVectorAddr,uint64_t LenIV,uint32_t *GhashHeaderAddr,Parameter128bits_TypeDef* Counter0);
void GCM_Counter_increase(Parameter128bits_TypeDef* Counter);
void GCM_CTR_Encrypt(uint8_t *pInttextAddr,uint64_t LenInttext,Parameter128bits_TypeDef* Counter0,AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr,uint8_t *pOuttextAddr);
void GCM_TagAuth(Parameter128bits_TypeDef* GhashAuthData,Parameter128bits_TypeDef* Counter0,AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr,Parameter128bits_TypeDef* AuthTag);

void GCM_Encrypt(int COM_No,GCM_Encrypt_InputTypedef* GCM_InputStruct, GCM_Encrypt_OutputTypedef* GCM_OutputStruct);
Bool GCM_Decrypt(int COM_No,GCM_Decrypt_InputTypedef* GCM_InputStruct, GCM_Decrypt_OutputTypedef* GCM_OutputStruct);

void ECB_Encrypt(ECB_Encrypt_InputTypedef* ECB_InputStruct,ECB_Encrypt_OutputTypedef* ECB_OutputStruct);
Bool ECB_Decrypt(ECB_Decrypt_InputTypedef* ECB_InputStruct,ECB_Decrypt_OutputTypedef* ECB_OutputStruct);

void CBC_Encrypt(CBC_Encrypt_InputTypedef* CBC_InputStruct,CBC_Encrypt_OutputTypedef* CBC_OutputStruct);
Bool CBC_Decrypt(CBC_Decrypt_InputTypedef* CBC_InputStruct,CBC_Decrypt_OutputTypedef* CBC_OutputStruct);

void CTR_Encrypt(CTR_Encrypt_InputTypedef* CTR_InputStruct,CTR_Encrypt_OutputTypedef* CTR_OutputStruct);

void AES_GCM_Example(uint8_t TestCase);
void GCM_TestCase_Init(uint8_t TestCase);

void AES_CTR_Example(void);
void CTR_TestCase_Init(void);

void AES_CBC_Example(void);
void CBC_TestCase_Init(void);

void AES_ECB_Example(void);
void ECB_TestCase_Init(void);
int AES_GCM_Decrypt_OR_Encrypt_H( unsigned short COM_No,unsigned char* Ciphert,unsigned short *nCtextLen,unsigned char use_crypt );   //�жϽ������ͣ������ͱ�ʶ����ӽ�������,use_crypt:0--���룬1--����..
int Fun_Key_Wrap_128Bits(unsigned char* key,unsigned char* Ciphert,unsigned char* Plaint);
int Fun_Key_Unwrap_128Bits(unsigned char* key,unsigned char* Ciphert,unsigned char* Plaint);
void Test_KeyWrap(void);

#endif                                        /* This File Only support HT6x2x and HT501x */

#ifdef __cplusplus
}
#endif

#endif /* __EXAMPLE_H__ */ 
