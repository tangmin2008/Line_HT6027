//#include "TypeMeter.h"
#include "TypeRAM.h"
#include "TypeFRAM.h"
#include "Data.h"
#include "TypeE2p.h"
//#include "Display.h"
//#include "Measure.h"
//#include "Power.h"
//#include "Port.h"

// *****************************************************************
// define global data
// *****************************************************************

PARA		Para;				// 电表参数
CLK	        Clk;
FLAG		Flag;
SRAM 		SM;
METERSPEC	MSpec;				//电表规格		//新国网		//13.08.30
REAL            Real_Data[MAX_CH_NUM];
ENERGY          Energy_Data[MAX_CH_NUM];
DISKEY		Disk;
INTPULSE	ECInt;
ECRAM           EC;
ECPULSE		ECP;
PUSHSTATE       Push_state[MAX_CH_NUM];
const short DayTab[13] =
{
	0,0,31,59,90,120,151,181,212,243,273,304,334
};

//////////////用于DLMS协议///////////////////
const unsigned short MP_Fcstab[256] =
{
	0x0000,		0x1189, 	0x2312, 	0x329b, 	
	0x4624, 	0x57ad, 	0x6536, 	0x74bf,		
	0x8c48, 	0x9dc1, 	0xaf5a, 	0xbed3, 
	0xca6c, 	0xdbe5, 	0xe97e, 	0xf8f7,		
	0x1081, 	0x0108, 	0x3393, 	0x221a, 	
	0x56a5, 	0x472c, 	0x75b7, 	0x643e,
	0x9cc9, 	0x8d40, 	0xbfdb,		0xae52, 	
	0xdaed, 	0xcb64, 	0xf9ff, 	0xe876,		
	0x2102, 	0x308b, 	0x0210,		0x1399, 
	0x6726, 	0x76af,		0x4434, 	0x55bd,		
	0xad4a, 	0xbcc3, 	0x8e58, 	0x9fd1, 	
	0xeb6e, 	0xfae7, 	0xc87c, 	0xd9f5,
	0x3183, 	0x200a, 	0x1291, 	0x0318, 	
	0x77a7, 	0x662e, 	0x54b5, 	0x453c,		
	0xbdcb, 	0xac42, 	0x9ed9, 	0x8f50, 
	0xfbef, 	0xea66, 	0xd8fd, 	0xc974,		
	0x4204, 	0x538d, 	0x6116, 	0x709f, 	
	0x0420, 	0x15a9, 	0x2732, 	0x36bb,
	0xce4c, 	0xdfc5, 	0xed5e, 	0xfcd7, 	
	0x8868, 	0x99e1, 	0xab7a, 	0xbaf3,		
	0x5285, 	0x430c, 	0x7197, 	0x601e, 
	0x14a1, 	0x0528, 	0x37b3,		0x263a,		
	0xdecd, 	0xcf44, 	0xfddf, 	0xec56, 	
	0x98e9, 	0x8960, 	0xbbfb, 	0xaa72,
	0x6306, 	0x728f, 	0x4014, 	0x519d, 	
	0x2522, 	0x34ab, 	0x0630, 	0x17b9,		
	0xef4e, 	0xfec7, 	0xcc5c, 	0xddd5, 
	0xa96a, 	0xb8e3, 	0x8a78, 	0x9bf1,		
	0x7387, 	0x620e, 	0x5095, 	0x411c, 	
	0x35a3, 	0x242a, 	0x16b1, 	0x0738,
	0xffcf, 	0xee46, 	0xdcdd, 	0xcd54, 	
	0xb9eb, 	0xa862, 	0x9af9, 	0x8b70,		
	0x8408, 	0x9581, 	0xa71a, 	0xb693, 
	0xc22c, 	0xd3a5, 	0xe13e, 	0xf0b7,		
	0x0840,		0x19c9, 	0x2b52, 	0x3adb, 	
	0x4e64, 	0x5fed, 	0x6d76, 	0x7cff,
	0x9489, 	0x8500, 	0xb79b, 	0xa612, 	
	0xd2ad, 	0xc324, 	0xf1bf, 	0xe036,		
	0x18c1, 	0x0948, 	0x3bd3, 	0x2a5a, 
	0x5ee5, 	0x4f6c, 	0x7df7, 	0x6c7e,		
	0xa50a, 	0xb483, 	0x8618, 	0x9791, 	
	0xe32e, 	0xf2a7, 	0xc03c, 	0xd1b5,
	0x2942, 	0x38cb, 	0x0a50, 	0x1bd9, 	
	0x6f66, 	0x7eef, 	0x4c74, 	0x5dfd,		
	0xb58b, 	0xa402, 	0x9699, 	0x8710, 
	0xf3af, 	0xe226, 	0xd0bd, 	0xc134,		
	0x39c3, 	0x284a, 	0x1ad1, 	0x0b58, 	
	0x7fe7, 	0x6e6e, 	0x5cf5, 	0x4d7c,
	0xc60c, 	0xd785, 	0xe51e, 	0xf497, 	
	0x8028, 	0x91a1, 	0xa33a, 	0xb2b3,		
	0x4a44, 	0x5bcd, 	0x6956, 	0x78df, 
	0x0c60, 	0x1de9, 	0x2f72, 	0x3efb,		
	0xd68d, 	0xc704, 	0xf59f, 	0xe416, 	
	0x90a9, 	0x8120, 	0xb3bb, 	0xa232,
	0x5ac5, 	0x4b4c, 	0x79d7, 	0x685e, 	
	0x1ce1, 	0x0d68, 	0x3ff3, 	0x2e7a,		
	0xe70e, 	0xf687, 	0xc41c, 	0xd595, 
	0xa12a, 	0xb0a3, 	0x8238, 	0x93b1,		
	0x6b46, 	0x7acf, 	0x4854, 	0x59dd, 	
	0x2d62, 	0x3ceb, 	0x0e70, 	0x1ff9,
	0xf78f, 	0xe606, 	0xd49d, 	0xc514, 	
	0xb1ab, 	0xa022, 	0x92b9, 	0x8330,	
	0x7bc7, 	0x6a4e, 	0x58d5, 	0x495c, 
	0x3de3, 	0x2c6a, 	0x1ef1, 	0x0f78,
};

// 为了提高编码效率，在实际运用中采用查表法来完成CRC-32校验
const unsigned long crc_32_tab[256]={ /* CRC32 余式表 */   
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,   
  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,   
  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,   
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,   
  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,   
  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,   
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,    
  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,   
  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,   
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,   
  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,   
  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,   
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,   
  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,   
  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,   
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,   
  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,   
  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,   
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,   
  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,   
  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,   
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,   
  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,   
  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,   
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,   
  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,   
  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,   
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,   
  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,   
  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,   
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,   
  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d   
};  


const unsigned char TimeMaxTab[3] =
{
	0x59,0x59,0x23
};

const unsigned char DateMaxTab[4] =
{
	0x07,0x31,0x12,0x99
};

const char MonTab[13] =
{
	0,31,28,31,30,31,30,31,31,30,31,30,31
};
const unsigned char BCDMonTab[13]=
{	0x31,0x31,0x28,0x31,0x30,0x31,0x30,
	0x31,0x31,0x30,0x31,0x30,0x31
};

const unsigned char WeekTab[] = {//èò?ê??D??ú±í  
(3 << 5) + 31,//1?? 
(6 << 5) + 29,//2?? 
(0 << 5) + 31,//3?? 
(3 << 5) + 30,//4?? 
(5 << 5) + 31,//5??   
(1 << 5) + 30,//6?? 
(3 << 5) + 31,//7?? 
(6 << 5) + 31,//8?? 
//(1 << 5) + 30,//9?? 
(2 << 5) + 30,//9?? 
(4 << 5) + 31,//10?? 
(0 << 5) + 30,//11?? 
(2 << 5) + 31 //12??
};

const unsigned char LCDRollOrderTab[21] =
{
0x14,0x12,0x13,0x10,0x0F,0x11,0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE   	
};
const LASTSUNDAY LastSunTab[]=
{
	0x26,0x29,	//; YEAR 00	
	0x25,0x28,	//; YEAR 01	
	0x31,0x27,	//; YEAR 02	
	0x30,0x26,	//; YEAR 03	
	0x28,0x31,	//; YEAR 04	
	0x27,0x30,	//; YEAR 05	
	0x26,0x29,	//; YEAR 06	
	0x25,0x28,	//; YEAR 07	
	0x30,0x26,	//; YEAR 08	
	0x29,0x25,	//; YEAR 09	
	0x28,0x31,	//; YEAR 10	
	0x27,0x30,	//; YEAR 11	
	0x25,0x28,	//; YEAR 12
	0x31,0x27,	//; YEAR 13
	0x30,0x26,	//; YEAR 14	
	0x29,0x25,	//; YEAR 15	
	0x27,0x30,	//; YEAR 16	
	0x26,0x29,	//; YEAR 17	
	0x25,0x28,	//; YEAR 18	
	0x31,0x27,	//; YEAR 19	
	0x29,0x25,	//; YEAR 20	
	0x28,0x31,	//; YEAR 21	
	0x27,0x30,	//; YEAR 22	
	0x26,0x29,	//; YEAR 23	
	0x31,0x27,	//; YEAR 24	
	0x30,0x26,	//; YEAR 25	
	0x29,0x25,	//; YEAR 26	
	0x28,0x31,	//; YEAR 27	
	0x26,0x29,	//; YEAR 28	
	0x25,0x28,	//; YEAR 29	
	0x31,0x27,	//; YEAR 30	
	0x30,0x26,	//; YEAR 31	
	0x28,0x31,	//; YEAR 32	
	0x27,0x30,	//; YEAR 33	
	0x26,0x29,	//; YEAR 34	
	0x25,0x28	//; YEAR 35			
};





	
const E2Para E2PTab[4] =
{
#if ( MeterPhase == SinglePhase )
///	ECDataEAds,	0x3000,	0x2,	0,	//24C512	
//	DataEAds,	0x80,	0x2,	0,	//24C64
//	FMAds,		0x3FF,	0x2,	0,	//FM24CL16	3V
	DataEAds,	0x8000,	0x2,	0,	//24C64
	FMAds,		0x400,	0x1,	0,	//FM24CL16	3V

	AdjEAds,	0x80,	0x2,	0,	//ATMEL24C04	
#endif	

#if ( MeterPhase == Threephase ) //三相		
	AdjEAds,	0x80,	0x2,	0,	//ATMEL24C04
	DataEAds,	0xFFF,	0x1,	0,	//24C64
	FMAds,		0x3FF,	0x1,	0,	//FM24CL16	3V	
#endif	
	ProfEAds,	0x00,	0x1,	0	//24C512
};

const unsigned char CByteBit[8] = 
{ 
	0x80, 0x40, 0x20, 0x10, 
	0x08, 0x04, 0x02, 0x01
};
const unsigned short ByteBit[16] = 
{ 
	0x0001, 0x0002, 0x0004, 0x0008, 
	0x0010, 0x0020, 0x0040, 0x0080,
	0x0100, 0x0200, 0x0400, 0x0800, 
	0x1000, 0x2000, 0x4000, 0x8000 	 
};

//CRC16的表格//
const unsigned short Crc16tab[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


const ECRg ECRgTab[ECUnitNum] =
{
	CMon_EC_Pp0,  FRCMon_EC_Pp0, &Energy_Data[0].Pp, &ECP.PL_CumPp[0], &ECP.PL_ChkPp[0], 0x01,			//当前有功正向电量
	CMon_EC_Pn0,  FRCMon_EC_Pn0, &Energy_Data[0].Pn, &ECP.PL_CumPn[0], &ECP.PL_ChkPn[0], 0x01,			//当前有功反向电量
        CMon_EC_Qp0,  FRCMon_EC_Qp0, &Energy_Data[0].Qp, &ECP.PL_CumQp[0], &ECP.PL_ChkQp[0], 0x01,			//当前无功正向电量
	CMon_EC_Qn0,  FRCMon_EC_Qn0, &Energy_Data[0].Qn, &ECP.PL_CumQn[0], &ECP.PL_ChkQn[0], 0x01,			//当前无功反向电量
 	CMon_EC_Q01,  FRCMon_EC_Qn7, &Energy_Data[0].Q1, &ECP.PL_CumQ[0][0], &ECP.PL_ChkQ[0][0], 0x01,
        CMon_EC_Q02,  FRCMon_EC_Qn7, &Energy_Data[0].Q2, &ECP.PL_CumQ[0][1], &ECP.PL_ChkQ[0][1], 0x01,
        CMon_EC_Q03,  FRCMon_EC_Qn7, &Energy_Data[0].Q3, &ECP.PL_CumQ[0][2], &ECP.PL_ChkQ[0][2], 0x01,
        CMon_EC_Q04,  FRCMon_EC_Qn7, &Energy_Data[0].Q4, &ECP.PL_CumQ[0][3], &ECP.PL_ChkQ[0][3], 0x01,
        CMon_EC_Pp1,  FRCMon_EC_Pp1, &Energy_Data[1].Pp, &ECP.PL_CumPp[1], &ECP.PL_ChkPp[1], 0x01,			//当前有功正向电量
	CMon_EC_Pn1,  FRCMon_EC_Pn1, &Energy_Data[1].Pn, &ECP.PL_CumPn[1], &ECP.PL_ChkPn[1], 0x01,			//当前有功反向电量
 	CMon_EC_Qp1,  FRCMon_EC_Qp1, &Energy_Data[1].Qp, &ECP.PL_CumQp[1], &ECP.PL_ChkQp[1], 0x01,			//当前无功正向电量
	CMon_EC_Qn1,  FRCMon_EC_Qn1, &Energy_Data[1].Qn, &ECP.PL_CumQn[1], &ECP.PL_ChkQn[1], 0x01,			//当前无功反向电量
        CMon_EC_Q11,  FRCMon_EC_Qn7, &Energy_Data[1].Q1, &ECP.PL_CumQ[1][0], &ECP.PL_ChkQ[1][0], 0x01,
        CMon_EC_Q12,  FRCMon_EC_Qn7, &Energy_Data[1].Q2, &ECP.PL_CumQ[1][1], &ECP.PL_ChkQ[1][1], 0x01,
        CMon_EC_Q13,  FRCMon_EC_Qn7, &Energy_Data[1].Q3, &ECP.PL_CumQ[1][2], &ECP.PL_ChkQ[1][2], 0x01,
        CMon_EC_Q14,  FRCMon_EC_Qn7, &Energy_Data[1].Q4, &ECP.PL_CumQ[1][3], &ECP.PL_ChkQ[1][3], 0x01,
        CMon_EC_Pp2,  FRCMon_EC_Pp2, &Energy_Data[2].Pp, &ECP.PL_CumPp[2], &ECP.PL_ChkPp[2], 0x01,			//当前有功正向电量
	CMon_EC_Pn2,  FRCMon_EC_Pn2, &Energy_Data[2].Pn, &ECP.PL_CumPn[2], &ECP.PL_ChkPn[2], 0x01,			//当前有功反向电量
        CMon_EC_Qp2,  FRCMon_EC_Qp2, &Energy_Data[2].Qp, &ECP.PL_CumQp[2], &ECP.PL_ChkQp[2], 0x01,			//当前无功正向电量
	CMon_EC_Qn2,  FRCMon_EC_Qn2, &Energy_Data[2].Qn, &ECP.PL_CumQn[2], &ECP.PL_ChkQn[2], 0x01,			//当前无功反向电量
        CMon_EC_Q21,  FRCMon_EC_Qn7, &Energy_Data[2].Q1, &ECP.PL_CumQ[2][0], &ECP.PL_ChkQ[2][0], 0x01,
        CMon_EC_Q22,  FRCMon_EC_Qn7, &Energy_Data[2].Q2, &ECP.PL_CumQ[2][1], &ECP.PL_ChkQ[2][1], 0x01,
        CMon_EC_Q23,  FRCMon_EC_Qn7, &Energy_Data[2].Q3, &ECP.PL_CumQ[2][2], &ECP.PL_ChkQ[2][2], 0x01,
        CMon_EC_Q24,  FRCMon_EC_Qn7, &Energy_Data[2].Q4, &ECP.PL_CumQ[2][3], &ECP.PL_ChkQ[2][3], 0x01,
        CMon_EC_Pp3,  FRCMon_EC_Pp3, &Energy_Data[3].Pp, &ECP.PL_CumPp[3], &ECP.PL_ChkPp[3], 0x01,			//当前有功正向电量
	CMon_EC_Pn3,  FRCMon_EC_Pn3, &Energy_Data[3].Pn, &ECP.PL_CumPn[3], &ECP.PL_ChkPn[3], 0x01,			//当前有功反向电量
        CMon_EC_Qp3,  FRCMon_EC_Qp3, &Energy_Data[3].Qp, &ECP.PL_CumQp[3], &ECP.PL_ChkQp[3], 0x01,			//当前无功正向电量
	CMon_EC_Qn3,  FRCMon_EC_Qn3, &Energy_Data[3].Qn, &ECP.PL_CumQn[3], &ECP.PL_ChkQn[3], 0x01,			//当前无功反向电量
        CMon_EC_Q31,  FRCMon_EC_Qn7, &Energy_Data[3].Q1, &ECP.PL_CumQ[3][0], &ECP.PL_ChkQ[3][0], 0x01,
        CMon_EC_Q32,  FRCMon_EC_Qn7, &Energy_Data[3].Q2, &ECP.PL_CumQ[3][1], &ECP.PL_ChkQ[3][1], 0x01,
        CMon_EC_Q33,  FRCMon_EC_Qn7, &Energy_Data[3].Q3, &ECP.PL_CumQ[3][2], &ECP.PL_ChkQ[3][2], 0x01,
        CMon_EC_Q34,  FRCMon_EC_Qn7, &Energy_Data[3].Q4, &ECP.PL_CumQ[3][3], &ECP.PL_ChkQ[3][3], 0x01,
#if    MAX_CH_NUM>4        
        CMon_EC_Pp4,  FRCMon_EC_Pp4, &Energy_Data[4].Pp, &ECP.PL_CumPp[4], &ECP.PL_ChkPp[4], 0x01,			//当前有功正向电量
	CMon_EC_Pn4,  FRCMon_EC_Pn4, &Energy_Data[4].Pn, &ECP.PL_CumPn[4], &ECP.PL_ChkPn[4], 0x01,			//当前有功反向电量
        CMon_EC_Qp4,  FRCMon_EC_Qp4, &Energy_Data[4].Qp, &ECP.PL_CumQp[4], &ECP.PL_ChkQp[4], 0x01,			//当前无功正向电量
	CMon_EC_Qn4,  FRCMon_EC_Qn4, &Energy_Data[4].Qn, &ECP.PL_CumQn[4], &ECP.PL_ChkQn[4], 0x01,			//当前无功反向电量
        CMon_EC_Q41,  FRCMon_EC_Qn7, &Energy_Data[4].Q1, &ECP.PL_CumQ[4][0], &ECP.PL_ChkQ[4][0], 0x01,
        CMon_EC_Q42,  FRCMon_EC_Qn7, &Energy_Data[4].Q2, &ECP.PL_CumQ[4][1], &ECP.PL_ChkQ[4][1], 0x01,
        CMon_EC_Q43,  FRCMon_EC_Qn7, &Energy_Data[4].Q3, &ECP.PL_CumQ[4][2], &ECP.PL_ChkQ[4][2], 0x01,
        CMon_EC_Q44,  FRCMon_EC_Qn7, &Energy_Data[4].Q4, &ECP.PL_CumQ[4][3], &ECP.PL_ChkQ[4][3], 0x01,
#endif        
#if    MAX_CH_NUM>5        
        CMon_EC_Pp5,  FRCMon_EC_Pp5, &Energy_Data[5].Pp, &ECP.PL_CumPp[5], &ECP.PL_ChkPp[5], 0x01,			//当前有功正向电量
	CMon_EC_Pn5,  FRCMon_EC_Pn5, &Energy_Data[5].Pn, &ECP.PL_CumPn[5], &ECP.PL_ChkPn[5], 0x01,			//当前有功反向电量
        CMon_EC_Qp5,  FRCMon_EC_Qp5, &Energy_Data[5].Qp, &ECP.PL_CumQp[5], &ECP.PL_ChkQp[5], 0x01,			//当前无功正向电量
	CMon_EC_Qn5,  FRCMon_EC_Qn5, &Energy_Data[5].Qn, &ECP.PL_CumQn[5], &ECP.PL_ChkQn[5], 0x01,			//当前无功反向电量
        CMon_EC_Q51,  FRCMon_EC_Qn7, &Energy_Data[5].Q1, &ECP.PL_CumQ[5][0], &ECP.PL_ChkQ[5][0], 0x01,
        CMon_EC_Q52,  FRCMon_EC_Qn7, &Energy_Data[5].Q2, &ECP.PL_CumQ[5][1], &ECP.PL_ChkQ[5][1], 0x01,
        CMon_EC_Q53,  FRCMon_EC_Qn7, &Energy_Data[5].Q3, &ECP.PL_CumQ[5][2], &ECP.PL_ChkQ[5][2], 0x01,
        CMon_EC_Q54,  FRCMon_EC_Qn7, &Energy_Data[5].Q4, &ECP.PL_CumQ[5][3], &ECP.PL_ChkQ[5][3], 0x01,
#endif        
#if     MAX_CH_NUM>6        
        CMon_EC_Pp6,  FRCMon_EC_Pp6, &Energy_Data[6].Pp, &ECP.PL_CumPp[6], &ECP.PL_ChkPp[6], 0x01,			//当前有功正向电量
	CMon_EC_Pn6,  FRCMon_EC_Pn6, &Energy_Data[6].Pn, &ECP.PL_CumPn[6], &ECP.PL_ChkPn[6], 0x01,			//当前有功反向电量
        CMon_EC_Qp6,  FRCMon_EC_Qp6, &Energy_Data[6].Qp, &ECP.PL_CumQp[6], &ECP.PL_ChkQp[6], 0x01,			//当前无功正向电量
	CMon_EC_Qn6,  FRCMon_EC_Qn6, &Energy_Data[6].Qn, &ECP.PL_CumQn[6], &ECP.PL_ChkQn[6], 0x01,			//当前无功反向电量
        CMon_EC_Q61,  FRCMon_EC_Qn7, &Energy_Data[6].Q1, &ECP.PL_CumQ[6][0], &ECP.PL_ChkQ[6][0], 0x01,
        CMon_EC_Q62,  FRCMon_EC_Qn7, &Energy_Data[6].Q2, &ECP.PL_CumQ[6][1], &ECP.PL_ChkQ[6][1], 0x01,
        CMon_EC_Q63,  FRCMon_EC_Qn7, &Energy_Data[6].Q3, &ECP.PL_CumQ[6][2], &ECP.PL_ChkQ[6][2], 0x01,
        CMon_EC_Q64,  FRCMon_EC_Qn7, &Energy_Data[6].Q4, &ECP.PL_CumQ[6][3], &ECP.PL_ChkQ[6][3], 0x01,
#endif        
#if     MAX_CH_NUM>7        
        CMon_EC_Pp7,  FRCMon_EC_Pp7, &Energy_Data[7].Pp, &ECP.PL_CumPp[7], &ECP.PL_ChkPp[7], 0x01,			//当前有功正向电量
	CMon_EC_Pn7,  FRCMon_EC_Pn7, &Energy_Data[7].Pn, &ECP.PL_CumPn[7], &ECP.PL_ChkPn[7], 0x01,			//当前有功反向电量
        CMon_EC_Qp7,  FRCMon_EC_Qp7, &Energy_Data[7].Qp, &ECP.PL_CumQp[7], &ECP.PL_ChkQp[7], 0x01,			//当前无功正向电量
	CMon_EC_Qn7,  FRCMon_EC_Qn7, &Energy_Data[7].Qn, &ECP.PL_CumQn[7], &ECP.PL_ChkQn[7], 0x01,			//当前无功反向电量
        CMon_EC_Q71,  FRCMon_EC_Qn7, &Energy_Data[7].Q1, &ECP.PL_CumQ[7][0], &ECP.PL_ChkQ[7][0], 0x01,
        CMon_EC_Q72,  FRCMon_EC_Qn7, &Energy_Data[7].Q2, &ECP.PL_CumQ[7][1], &ECP.PL_ChkQ[7][1], 0x01,
        CMon_EC_Q73,  FRCMon_EC_Qn7, &Energy_Data[7].Q3, &ECP.PL_CumQ[7][2], &ECP.PL_ChkQ[7][2], 0x01,
        CMon_EC_Q74,  FRCMon_EC_Qn7, &Energy_Data[7].Q4, &ECP.PL_CumQ[7][3], &ECP.PL_ChkQ[7][3], 0x01,
#endif        
};

const PROFILETAB ProfileTab[] =
{
  LOAD0_USEADDR,0,3,ONE_RECORD_SIZE,LOAD0_USEADDR,LOAD0_USEADDR,LOAD0_CURADDR,
  RAND0_USEADDR,3,2,ONE_RECORD_SIZE,RAND0_USEADDR,RAND0_USEADDR,RAND0_CURADDR,
  FRZD0_USEADDR,5,2,ONE_RECORD_SIZE,FRZD0_USEADDR,FRZD0_USEADDR,FRZD0_CURADDR,
  SHRP0_USEADDR,7,7,ONE_RECORD_SIZE,SHRP0_USEADDR,SHRP0_USEADDR,SHRP0_CURADDR,
  MOND0_USEADDR,14,2,ONE_RECORD_SIZE,MOND0_USEADDR,MOND0_USEADDR,MOND0_CURADDR,
  CH0_PAP_USEADDR,16,2,48,CH0_PAP_USEADDR,CH0_PAP_USEADDR,CH0_PAP_CURADDR,
  CH0_PBP_USEADDR,18,2,48,CH0_PBP_USEADDR,CH0_PBP_USEADDR,CH0_PBP_CURADDR,
  CH0_PCP_USEADDR,20,2,48,CH0_PCP_USEADDR,CH0_PCP_USEADDR,CH0_PCP_CURADDR,
  CH0_PAN_USEADDR,22,2,48,CH0_PAN_USEADDR,CH0_PAN_USEADDR,CH0_PAN_CURADDR,
  CH0_PBN_USEADDR,24,2,48,CH0_PBN_USEADDR,CH0_PBN_USEADDR,CH0_PBN_CURADDR,
  CH0_PCN_USEADDR,26,2,48,CH0_PCN_USEADDR,CH0_PCN_USEADDR,CH0_PCN_CURADDR,
  CH0_PTD_USEADDR,28,2,48,CH0_PTD_USEADDR,CH0_PTD_USEADDR,CH0_PTD_CURADDR,
  CH0_CLR_USEADDR,30,2,48,CH0_CLR_USEADDR,CH0_CLR_USEADDR,CH0_CLR_CURADDR,
  
  LOAD1_USEADDR,32,3,ONE_RECORD_SIZE,LOAD1_USEADDR,LOAD1_USEADDR,LOAD1_CURADDR,
  RAND1_USEADDR,35,2,ONE_RECORD_SIZE,RAND1_USEADDR,RAND1_USEADDR,RAND1_CURADDR,
  FRZD1_USEADDR,37,2,ONE_RECORD_SIZE,FRZD1_USEADDR,FRZD1_USEADDR,FRZD1_CURADDR,
  SHRP1_USEADDR,39,7,ONE_RECORD_SIZE,SHRP1_USEADDR,SHRP1_USEADDR,SHRP1_CURADDR,
  MOND1_USEADDR,46,2,ONE_RECORD_SIZE,MOND1_USEADDR,MOND1_USEADDR,MOND1_CURADDR,
  CH1_PAP_USEADDR,48,2,48,CH1_PAP_USEADDR,CH1_PAP_USEADDR,CH1_PAP_CURADDR,
  CH1_PBP_USEADDR,50,2,48,CH1_PBP_USEADDR,CH1_PBP_USEADDR,CH1_PBP_CURADDR,
  CH1_PCP_USEADDR,52,2,48,CH1_PCP_USEADDR,CH1_PCP_USEADDR,CH1_PCP_CURADDR,
  CH1_PAN_USEADDR,54,2,48,CH1_PAN_USEADDR,CH1_PAN_USEADDR,CH1_PAN_CURADDR,
  CH1_PBN_USEADDR,56,2,48,CH1_PBN_USEADDR,CH1_PBN_USEADDR,CH1_PBN_CURADDR,
  CH1_PCN_USEADDR,58,2,48,CH1_PCN_USEADDR,CH1_PCN_USEADDR,CH1_PCN_CURADDR,
  CH1_PTD_USEADDR,60,2,48,CH1_PTD_USEADDR,CH1_PTD_USEADDR,CH1_PTD_CURADDR,
  CH1_CLR_USEADDR,62,2,48,CH1_CLR_USEADDR,CH1_CLR_USEADDR,CH1_CLR_CURADDR,
  
  LOAD2_USEADDR,64,3,ONE_RECORD_SIZE,LOAD2_USEADDR,LOAD2_USEADDR,LOAD2_CURADDR,
  RAND2_USEADDR,67,2,ONE_RECORD_SIZE,RAND2_USEADDR,RAND2_USEADDR,RAND2_CURADDR,
  FRZD2_USEADDR,69,2,ONE_RECORD_SIZE,FRZD2_USEADDR,FRZD2_USEADDR,FRZD2_CURADDR,
  SHRP2_USEADDR,71,7,ONE_RECORD_SIZE,SHRP2_USEADDR,SHRP2_USEADDR,SHRP2_CURADDR,
  MOND2_USEADDR,78,2,ONE_RECORD_SIZE,MOND2_USEADDR,MOND2_USEADDR,MOND2_CURADDR,
  CH2_PAP_USEADDR,80,2,48,CH2_PAP_USEADDR,CH2_PAP_USEADDR,CH2_PAP_CURADDR,
  CH2_PBP_USEADDR,82,2,48,CH2_PBP_USEADDR,CH2_PBP_USEADDR,CH2_PBP_CURADDR,
  CH2_PCP_USEADDR,84,2,48,CH2_PCP_USEADDR,CH2_PCP_USEADDR,CH2_PCP_CURADDR,
  CH2_PAN_USEADDR,86,2,48,CH2_PAN_USEADDR,CH2_PAN_USEADDR,CH2_PAN_CURADDR,
  CH2_PBN_USEADDR,88,2,48,CH2_PBN_USEADDR,CH2_PBN_USEADDR,CH2_PBN_CURADDR,
  CH2_PCN_USEADDR,90,2,48,CH2_PCN_USEADDR,CH2_PCN_USEADDR,CH2_PCN_CURADDR,
  CH2_PTD_USEADDR,92,2,48,CH2_PTD_USEADDR,CH2_PTD_USEADDR,CH2_PTD_CURADDR,
  CH2_CLR_USEADDR,94,2,48,CH2_CLR_USEADDR,CH2_CLR_USEADDR,CH2_CLR_CURADDR,
  
  LOAD3_USEADDR,96,3,ONE_RECORD_SIZE,LOAD3_USEADDR,LOAD3_USEADDR,LOAD3_CURADDR,
  RAND3_USEADDR,99,2,ONE_RECORD_SIZE,RAND3_USEADDR,RAND3_USEADDR,RAND3_CURADDR,
  FRZD3_USEADDR,101,2,ONE_RECORD_SIZE,FRZD3_USEADDR,FRZD3_USEADDR,FRZD3_CURADDR,
  SHRP3_USEADDR,103,7,ONE_RECORD_SIZE,SHRP3_USEADDR,SHRP3_USEADDR,SHRP3_CURADDR,
  MOND3_USEADDR,110,2,ONE_RECORD_SIZE,MOND3_USEADDR,MOND3_USEADDR,MOND3_CURADDR,
  CH3_PAP_USEADDR,112,2,48,CH3_PAP_USEADDR,CH3_PAP_USEADDR,CH3_PAP_CURADDR,
  CH3_PBP_USEADDR,114,2,48,CH3_PBP_USEADDR,CH3_PBP_USEADDR,CH3_PBP_CURADDR,
  CH3_PCP_USEADDR,116,2,48,CH3_PCP_USEADDR,CH3_PCP_USEADDR,CH3_PCP_CURADDR,
  CH3_PAN_USEADDR,118,2,48,CH3_PAN_USEADDR,CH3_PAN_USEADDR,CH3_PAN_CURADDR,
  CH3_PBN_USEADDR,120,2,48,CH3_PBN_USEADDR,CH3_PBN_USEADDR,CH3_PBN_CURADDR,
  CH3_PCN_USEADDR,122,2,48,CH3_PCN_USEADDR,CH3_PCN_USEADDR,CH3_PCN_CURADDR,
  CH3_PTD_USEADDR,124,2,48,CH3_PTD_USEADDR,CH3_PTD_USEADDR,CH3_PTD_CURADDR,
  CH3_CLR_USEADDR,126,2,48,CH3_CLR_USEADDR,CH3_CLR_USEADDR,CH3_CLR_CURADDR,
  
  LOAD4_USEADDR,128,3,ONE_RECORD_SIZE,LOAD4_USEADDR,LOAD4_USEADDR,LOAD4_CURADDR,
  RAND4_USEADDR,130,2,ONE_RECORD_SIZE,RAND4_USEADDR,RAND4_USEADDR,RAND4_CURADDR,
  FRZD4_USEADDR,132,2,ONE_RECORD_SIZE,FRZD4_USEADDR,FRZD4_USEADDR,FRZD4_CURADDR,
  SHRP4_USEADDR,134,7,ONE_RECORD_SIZE,SHRP4_USEADDR,SHRP4_USEADDR,SHRP4_CURADDR,
  MOND4_USEADDR,141,2,ONE_RECORD_SIZE,MOND4_USEADDR,MOND4_USEADDR,MOND4_CURADDR,
  CH4_PAP_USEADDR,143,2,48,CH4_PAP_USEADDR,CH4_PAP_USEADDR,CH4_PAP_CURADDR,
  CH4_PBP_USEADDR,145,2,48,CH4_PBP_USEADDR,CH4_PBP_USEADDR,CH4_PBP_CURADDR,
  CH4_PCP_USEADDR,147,2,48,CH4_PCP_USEADDR,CH4_PCP_USEADDR,CH4_PCP_CURADDR,
  CH4_PAN_USEADDR,149,2,48,CH4_PAN_USEADDR,CH4_PAN_USEADDR,CH4_PAN_CURADDR,
  CH4_PBN_USEADDR,151,2,48,CH4_PBN_USEADDR,CH4_PBN_USEADDR,CH4_PBN_CURADDR,
  CH4_PCN_USEADDR,153,2,48,CH4_PCN_USEADDR,CH4_PCN_USEADDR,CH4_PCN_CURADDR,
  CH4_PTD_USEADDR,155,2,48,CH4_PTD_USEADDR,CH4_PTD_USEADDR,CH4_PTD_CURADDR,
  CH4_CLR_USEADDR,157,2,48,CH4_CLR_USEADDR,CH4_CLR_USEADDR,CH4_CLR_CURADDR,
  
  LOAD5_USEADDR,159,3,ONE_RECORD_SIZE,LOAD5_USEADDR,LOAD5_USEADDR,LOAD5_CURADDR,
  RAND5_USEADDR,162,2,ONE_RECORD_SIZE,RAND5_USEADDR,RAND5_USEADDR,RAND5_CURADDR,
  FRZD5_USEADDR,164,2,ONE_RECORD_SIZE,FRZD5_USEADDR,FRZD5_USEADDR,FRZD5_CURADDR,
  SHRP5_USEADDR,166,7,ONE_RECORD_SIZE,SHRP5_USEADDR,SHRP5_USEADDR,SHRP5_CURADDR,
  MOND5_USEADDR,173,2,ONE_RECORD_SIZE,MOND5_USEADDR,MOND5_USEADDR,MOND5_CURADDR,
  CH5_PAP_USEADDR,175,2,48,CH5_PAP_USEADDR,CH5_PAP_USEADDR,CH5_PAP_CURADDR,
  CH5_PBP_USEADDR,177,2,48,CH5_PBP_USEADDR,CH5_PBP_USEADDR,CH5_PBP_CURADDR,
  CH5_PCP_USEADDR,179,2,48,CH5_PCP_USEADDR,CH5_PCP_USEADDR,CH5_PCP_CURADDR,
  CH5_PAN_USEADDR,181,2,48,CH5_PAN_USEADDR,CH5_PAN_USEADDR,CH5_PAN_CURADDR,
  CH5_PBN_USEADDR,183,2,48,CH5_PBN_USEADDR,CH5_PBN_USEADDR,CH5_PBN_CURADDR,
  CH5_PCN_USEADDR,185,2,48,CH5_PCN_USEADDR,CH5_PCN_USEADDR,CH5_PCN_CURADDR,
  CH5_PTD_USEADDR,187,2,48,CH5_PTD_USEADDR,CH5_PTD_USEADDR,CH5_PTD_CURADDR,
  CH5_CLR_USEADDR,189,2,48,CH5_CLR_USEADDR,CH5_CLR_USEADDR,CH5_CLR_CURADDR,
  
  LOAD6_USEADDR,191,3,ONE_RECORD_SIZE,LOAD6_USEADDR,LOAD6_USEADDR,LOAD6_CURADDR,
  RAND6_USEADDR,194,2,ONE_RECORD_SIZE,RAND6_USEADDR,RAND6_USEADDR,RAND6_CURADDR,
  FRZD6_USEADDR,196,2,ONE_RECORD_SIZE,FRZD6_USEADDR,FRZD6_USEADDR,FRZD6_CURADDR,
  SHRP6_USEADDR,198,7,ONE_RECORD_SIZE,SHRP6_USEADDR,SHRP6_USEADDR,SHRP6_CURADDR,
  MOND6_USEADDR,205,2,ONE_RECORD_SIZE,MOND6_USEADDR,MOND6_USEADDR,MOND6_CURADDR,
  CH6_PAP_USEADDR,207,2,48,CH6_PAP_USEADDR,CH6_PAP_USEADDR,CH6_PAP_CURADDR,
  CH6_PBP_USEADDR,209,2,48,CH6_PBP_USEADDR,CH6_PBP_USEADDR,CH6_PBP_CURADDR,
  CH6_PCP_USEADDR,211,2,48,CH6_PCP_USEADDR,CH6_PCP_USEADDR,CH6_PCP_CURADDR,
  CH6_PAN_USEADDR,213,2,48,CH6_PAN_USEADDR,CH6_PAN_USEADDR,CH6_PAN_CURADDR,
  CH6_PBN_USEADDR,215,2,48,CH6_PBN_USEADDR,CH6_PBN_USEADDR,CH6_PBN_CURADDR,
  CH6_PCN_USEADDR,217,2,48,CH6_PCN_USEADDR,CH6_PCN_USEADDR,CH6_PCN_CURADDR,
  CH6_PTD_USEADDR,219,2,48,CH6_PTD_USEADDR,CH6_PTD_USEADDR,CH6_PTD_CURADDR,
  CH6_CLR_USEADDR,221,2,48,CH6_CLR_USEADDR,CH6_CLR_USEADDR,CH6_CLR_CURADDR,
  
  LOAD7_USEADDR,223,3,ONE_RECORD_SIZE,LOAD7_USEADDR,LOAD7_USEADDR,LOAD7_CURADDR,
  RAND7_USEADDR,226,2,ONE_RECORD_SIZE,RAND7_USEADDR,RAND7_USEADDR,RAND7_CURADDR,
  FRZD7_USEADDR,228,2,ONE_RECORD_SIZE,FRZD7_USEADDR,FRZD7_USEADDR,FRZD7_CURADDR,
  SHRP7_USEADDR,230,7,ONE_RECORD_SIZE,SHRP7_USEADDR,SHRP7_USEADDR,SHRP7_CURADDR,
  MOND7_USEADDR,237,2,ONE_RECORD_SIZE,MOND7_USEADDR,MOND7_USEADDR,MOND7_CURADDR,
  CH7_PAP_USEADDR,239,2,48,CH7_PAP_USEADDR,CH7_PAP_USEADDR,CH7_PAP_CURADDR,
  CH7_PBP_USEADDR,241,2,48,CH7_PBP_USEADDR,CH7_PBP_USEADDR,CH7_PBP_CURADDR,
  CH7_PCP_USEADDR,243,2,48,CH7_PCP_USEADDR,CH7_PCP_USEADDR,CH7_PCP_CURADDR,
  CH7_PAN_USEADDR,245,2,48,CH7_PAN_USEADDR,CH7_PAN_USEADDR,CH7_PAN_CURADDR,
  CH7_PBN_USEADDR,247,2,48,CH7_PBN_USEADDR,CH7_PBN_USEADDR,CH7_PBN_CURADDR,
  CH7_PCN_USEADDR,249,2,48,CH7_PCN_USEADDR,CH7_PCN_USEADDR,CH7_PCN_CURADDR,
  CH7_PTD_USEADDR,251,2,48,CH7_PTD_USEADDR,CH7_PTD_USEADDR,CH7_PTD_CURADDR,
  CH7_CLR_USEADDR,253,2,48,CH7_CLR_USEADDR,CH7_CLR_USEADDR,CH7_CLR_CURADDR,
  TIME_USEADDR,255,2,12,TIME_USEADDR,TIME_USEADDR,TIME_CURADDR,
  CEVENT0_USEADDR,257,2,12,CEVENT0_USEADDR,CEVENT0_USEADDR,CEVENT0_CURADDR,
  CEVENT1_USEADDR,259,2,12,CEVENT1_USEADDR,CEVENT1_USEADDR,CEVENT1_CURADDR,
  CEVENT2_USEADDR,261,2,12,CEVENT2_USEADDR,CEVENT2_USEADDR,CEVENT2_CURADDR,
  CEVENT3_USEADDR,263,2,12,CEVENT3_USEADDR,CEVENT3_USEADDR,CEVENT3_CURADDR,
  CEVENT4_USEADDR,265,2,12,CEVENT4_USEADDR,CEVENT4_USEADDR,CEVENT4_CURADDR,
  CEVENT5_USEADDR,267,2,12,CEVENT5_USEADDR,CEVENT5_USEADDR,CEVENT5_CURADDR,
  CEVENT6_USEADDR,269,2,12,CEVENT6_USEADDR,CEVENT6_USEADDR,CEVENT6_CURADDR,
  CEVENT7_USEADDR,271,2,12,CEVENT7_USEADDR,CEVENT7_USEADDR,CEVENT7_CURADDR,
};

EC_PARA m_ecpara[MAX_CH_NUM];