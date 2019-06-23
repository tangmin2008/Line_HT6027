//����������д20130525: image_transfer..

//image_transfer status ::= enum  ---detailed status of image transfer process
//{		
#define  TRANSFER_NOT_INITIATED  		0
#define  TRANSFER_INITIATED      		1
#define  VERIFICATION_INITIATED  		2
#define  VERIFICATION_SUCCESSFUL 		3
#define  VERIFICATION_FAILED        4
#define  ACTIVATION_INITIATED       5
#define  ACTIVATION_SUCCESSFUL      6
#define  ACTIVATION_FAILED       		7

//Image_Tranfer_Flag ��־λ
#define IMAGE_FLS4K_NEED    		 (0x01)		//FLASH(4K)��Ҫ�����ȶ�����д�����,Ϊ����ACTION2ֱ��д��..
#define ACTION1_INTI_FLAG        (0x02)		//ACTION1ִ�б�־..		�����������..
#define ACTION2_TRAN_FLAG        (0x04)		//ACTION2ִ�б�־..
#define ACTION3_VERIFY_FLAG      (0x08)		//ACTION3ִ�б�־..
#define ACTION4_ACTIVATE_FLAG    (0x10)		//ACTION1ִ�в���ͨ����־..
#define ACTIVATE_TIME_FLAG       (0x20)		//ʱ�䵽�����־..
#define FLAG_BIT6                (0x40)
#define FLAG_BIT7                (0x80)
//	== 0; //��־����..//��ʼ��..


#define	CPU_FLS_PAGE		0x0200							//cpu �߳�����ÿ����Ϊ0x200 = 512���ֽ�..
//INFO=1800-19FF,ֻ��дINFOA��Ҫ��LOCKA����,INFOÿ����128���ֽ�..
#define	CPU_INFOA  			0x1980							//0x1980-19FF..
#define	CPU_INFOB  			0x1900							//0x1900-197F..
#define	CPU_INFOC  			0x1880							//0x1880-18FF..
#define	CPU_INFOD  			0x1800							//0x1800-187F..


//}

#define IMAGE_BLOCK_SIZE   256			// ���Ҫ����Ҫѡ����ڵ���256..
short GetImage_transferClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetImage_transferClassData( unsigned char *Source , unsigned short DataLen );
unsigned char ActionImage_transferClassData( unsigned char *Source, unsigned short DataLen );

unsigned long GenerateCRC32(unsigned long oldcrc32, unsigned char * dataptr,unsigned short len);
unsigned long Flash_GenerateCRC32(unsigned long Addr,unsigned long len );

//�����(������ʱ�伤��)..
unsigned char Activate_Image_Transfer(unsigned char Enter_Flag );
unsigned char Verify_Image_Transfer(unsigned char Enter_Flag );			//У��..
///void Image_tranfer_init(void);
///void Image_tranfer_NewOrOld(void);
