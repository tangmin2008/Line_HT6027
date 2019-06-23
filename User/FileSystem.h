//Flash��ز���
#define FLASH_PAGE_SIZE			256
#define FLASH_SECTOR_SIZE		4096
//#define FLASH_SECTOR_COUNT		2048
//#define FLASH_SECTOR_COUNT		1800				//16.09.27
#define FLASH_SECTOR_COUNT		3600				//17.04.14
#define FLASH_BLOCK_SIZE		65536
#define FLASH_PAGES_PER_SECTOR	FLASH_SECTOR_SIZE/FLASH_PAGE_SIZE

#define FAT_SECTOR				1
#define FAT_INDEX_START_SECTOR	2
#define FAT_INDEX_END_SECTOR	5
#define FAT_INDEX_SECTOR_LEN	3
#define FAT_SECTOR_BAK			4

//FAT�е�Ŀ¼��	4090/10=409		5�ֽ��ļ�����2�ֽ�������2�ֽ��ļ���������1�ֽ�У���
#define FAT_DIR_LEN		10
#define FAT_DIRS		409

//FAT��ÿҳ������	4096/3=1365		//2�ֽ�������1�ֽ�У���
#define FAT_SECTOR_INDEXS		1365

//������ʼ����9		��0�����գ���1~9����ΪFAT
#define DATA_START_SECTOR		9

#define	FR_OK 				 	0	/* (0) Succeeded */
#define	FR_DISK_ERR				1	/* (1) A hard error occured in the low level disk I/O layer */
#define	FR_INT_ERR				2	/* (2) Assertion failed */
#define	FR_NOT_READY			3	/* (3) The physical drive cannot work */
#define	FR_NO_FILE				4	/* (4) Could not find the file */
#define	FR_NO_PATH				5	/* (5) Could not find the path */
#define	FR_INVALID_NAME			6	/* (6) The path name format is invalid */
#define	FR_DENIED				7	/* (7) Acces denied due to prohibited access or directory full */
#define	FR_EXIST				8	/* (8) Acces denied due to prohibited access */
#define	FR_INVALID_OBJECT		9	/* (9) The file/directory object is invalid */
#define	FR_WRITE_PROTECTED		10	/* (10) The physical drive is write protected */
#define	FR_INVALID_DRIVE		11	/* (11) The logical drive number is invalid */
#define	FR_NOT_ENABLED			12	/* (12) The volume has no work area */
#define	FR_NO_FILESYSTEM		13	/* (13) There is no valid FAT volume */
#define	FR_MKFS_ABORTED			14	/* (14) The f_mkfs() aborted due to any parameter error */
#define	FR_TIMEOUT				15	/* (15) Could not get a grant to access the volume within defined period */
#define	FR_LOCKED				16	/* (16) The operation is rejected according to the file shareing policy */
#define	FR_NOT_ENOUGH_CORE		17	/* (17) LFN working buffer could not be allocated */
#define	FR_TOO_MANY_OPEN_FILES	18	/* (18) Number of open files > _FS_SHARE */
#define	FR_INVALID_PARAMETER	19	/* (19) Given parameter is invalid */

short Erase_Sector(unsigned short nDest);
void File_Format( void );
unsigned short GetRemainSectors( unsigned short* RemainStartSector );
unsigned short GetNextRemainSector( unsigned short StartSector );
void WriteNextSectorNo( unsigned short CurSector, unsigned short NextSector );
unsigned short GetDirNo( unsigned char* FileName, unsigned short* RemainStartDir );
unsigned short GetRemainDirs( unsigned short* RemainStartDir );
unsigned short CreatFile( FILEPARA* FilePara, unsigned char* FileName );
unsigned short DeleteFileSector( unsigned short StartSector, unsigned short SectorSum );
unsigned short DeletFile( FILECURVEPARA* FileCurvePara, unsigned char* FileName );
unsigned short Open_File( FILECURVEPARA* FileCurvePara, unsigned char* FileName );
long GetAppointSector( unsigned short StartSector, unsigned short SectorSum );					

void GetDirTo_FBuff( void );																//17.03.08
unsigned short GetDirNo_Form_FBuff( unsigned char* FileName, unsigned short* RemainStartDir );		//17.03.08
unsigned short Open_File_In_FBuff( FILECURVEPARA* FileCurvePara, unsigned char* FileName );			//17.03.08























