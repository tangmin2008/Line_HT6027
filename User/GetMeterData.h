#ifndef _GETMETERDATA_H
#define _GETMETERDATA_H

#define	RD8213DLY	10//�趨ʱ��x���룬���xMs֮�ڶ�û���ݽ��գ�����Ϊ�����ѽ������,��ʱ��0.5���붨ʱ������
#define	READ8213OK	0x01
#define	WRITE8213OK 0x02
#define Comm8213OK	0x04
#define WriteSuccess 0x06

extern void SynData1s (void);
extern int ID_Read( unsigned char* RAM_Dest,unsigned int ID_Src, int Lenth );
extern int ID_Write( unsigned int ID_Dest, unsigned char* RAM_Src, int Lenth);
extern int ID_Read2(void);
extern unsigned char SYNData_Get(void);

#endif
