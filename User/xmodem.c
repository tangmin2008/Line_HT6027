#include "TypeRAM.h"
#include "data.h"
#include "ht6xxx_lib.h"

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000
#define MAXRETRANS 25
static int last_error = 0;
/****************Portting Start *******************/
#include "string.h"
#define IEC101_PORT  2



static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

unsigned long GenerateCRC32(unsigned long oldcrc32, unsigned char * dataptr,unsigned short len) 
{ 
  unsigned long crc32; 
  unsigned long oldcrc; 
  unsigned char c,t; 
  while (len--) { 
    t= (oldcrc32 >> 24) & 0xFF; 		//要移出的字节的值 
    oldcrc=crc_32_tab[t]; 					//根据移出的字节的值查表 		
    c = *dataptr; 									//新移进来的字节值 
    oldcrc32= (oldcrc32 << 8) | c; 	//将新移进来的字节值添在寄存器末字节中 
    oldcrc32=oldcrc32^oldcrc; 			//将寄存器与查出的值进行xor运算 
    dataptr++;
  } 
  crc32=oldcrc32; 
  return crc32; 
}

unsigned short crc16_ccitt(const char *buf, int len)
{
	register int counter;
	register unsigned short crc = 0;
	for( counter = 0; counter < len; counter++)
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
	return crc;
}


void port_outbyte(unsigned char trychar)
{
  Serial_Write(IEC101_PORT,&trychar,1);
}

unsigned char port_inbyte(unsigned int time_out)
{
	unsigned char ch;
	int i;
	last_error = 0;
        for(i=0;i<time_out;++i)
        {
          ch = 0xff;
          Serial_Read(IEC101_PORT,&ch,1);
          if(ch!=0xff)
            break;
          udelay(100);
        }
        if(ch != 0xff)
          return ch;
	last_error = 1;
	return ch;
}
/****************Portting End*******************/
static int check(int crc, const unsigned char *buf, int sz)
{
  if (crc) 
  {
    unsigned short crc = crc16_ccitt(buf, sz);
    unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
    if (crc == tcrc)
      return 1;
  }
  else 
  {
    int i;
    unsigned char cks = 0;
    for (i = 0; i < sz; ++i) 
    {
      cks += buf[i];
    }
    if (cks == buf[sz])
      return 1;
  }
  
  return 0;
}

static void flushinput(void)
{
  //fflush(stdin);
}
int xmodemReceive()
{
  unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
  unsigned char *p;
  int bufsz, crc = 0;
  unsigned char trychar = 'C';
  unsigned char packetno = 1;
  int i, c, len = 0;
  int retry, retrans = MAXRETRANS;;
  unsigned long Crc32_check=0;

  flushinput();
  for(;;) 
  {
    for( retry = 0; retry < 100; ++retry) 
    {
      HT_FreeDog();
      if (trychar) 
        port_outbyte(trychar);
      c = port_inbyte((DLY_1S));
      if (last_error == 0) 
      {
        switch (c) 
        {
        case SOH:
          bufsz = 128;
          goto start_recv;
        case STX:
          bufsz = 1024;
          goto start_recv;
        case EOT:
          flushinput();
          port_outbyte(ACK);
          DataFlash_Write(0x200000,&len,4);
          DataFlash_Write(0x200004,&Crc32_check,4);
          memcpy(xbuff,&len,4);
          memcpy(xbuff+4,&Crc32_check,4);
          Crc32_check=GenerateCRC32(0xffffFFff, &xbuff[0],8);
          DataFlash_Write(0x200008,&Crc32_check,4);
          Flag.Power &= ~F_IrmsCheck;
          return len; /* normal end */
        case CAN:
          c = port_inbyte(DLY_1S);
          
          if (c == CAN) 
          {
            flushinput();
            port_outbyte(ACK);
            return -1; /* canceled by remote */
          }
          break;
        default:
          break;
        }
      }
    }
    if (trychar == 'C') 
    { 
      trychar = NAK; 
      continue; 
    }
    flushinput();
    port_outbyte(CAN);
    port_outbyte(CAN);
    port_outbyte(CAN);
    return -2; /* sync error */
    
  start_recv:
    if (trychar == 'C') crc = 1;
    trychar = 0;
    p = xbuff;
    *p++ = c;
    for (i = 0;  i < (bufsz+(crc?1:0)+3); /*++i*/) 
    {
      i+=Serial_Read(IEC101_PORT,p+i,142);
    }
    
    if (xbuff[1] == (unsigned char)(~xbuff[2]) && 
        (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
          check(crc, &xbuff[3], bufsz)) 
    {
      if (xbuff[1] == packetno)	
      {
        int count;
        count = bufsz;
        if (count > 0) 
        {
          DataFlash_Write(0x200000+4096+len,&xbuff[3],count);
          Crc32_check = GenerateCRC32(Crc32_check, &xbuff[3],count);
          len += count;
        }
        ++packetno;
        retrans = MAXRETRANS+1;
      }
      if (--retrans <= 0) 
      {
        flushinput();
        port_outbyte(CAN);
        port_outbyte(CAN);
        port_outbyte(CAN);
        return -3; /* too many retry error */
      }
      port_outbyte(ACK);
      continue;
    }
  reject:
    flushinput();
    port_outbyte(NAK);
  }
}
#define Buff_Len 512
unsigned char CheckBuf1[512] ;
unsigned char CheckBuf2[512] ;

void EraseCode(void)
{
  unsigned int i;    
  for(i=0;i<16;i++)
  {
       HT_Flash_PageErase(0x0400*i);
       HT_FreeDog();
  }          
}

unsigned long Flash_GenerateCRC32(unsigned long Addr,unsigned long len )
{
  unsigned long lenth;
  unsigned char Buff[520];
  unsigned char *Ptr;
  unsigned short i;
  unsigned long Crc32_check=0;
  
  i = 0;
  while( len > 0)
  {
    Ptr = Buff;		
    lenth = len/512;
    if(lenth==0){
      lenth = len;
    }else{
      lenth = 512;
    }
    HT_FreeDog();
    HT_Flash_ByteRead( Ptr,(unsigned long)Addr+(unsigned long)i*512, lenth );
    Crc32_check = GenerateCRC32(Crc32_check, Ptr,lenth); 
    i++;
    len -=lenth;		
  }
  return Crc32_check; 
}

unsigned char CRC_NewProgram(void)
{	
  unsigned long Crc32_Temp,Crc32_Data;
  unsigned char buff[4];
  unsigned char Error_Byte;
  unsigned char* Ptr;
  int long_temp;
  Ptr = buff;
  Error_Byte = 0;
  Read_Flash((unsigned char*)&long_temp,0x200000 ,4 );
  Crc32_Data = Flash_GenerateCRC32( 0,long_temp);
  Read_Flash((unsigned char*)&Crc32_Temp,0x200004,4 );
  if(Crc32_Temp==Crc32_Data)
    Error_Byte = 0;
  else
    Error_Byte = 1;
  return Error_Byte;
}
unsigned char DownloadNewProgram(void)
{//下载新程序
  unsigned long dst_addr;
  unsigned long src_addr;	
  int long_temp;	
  unsigned short	i,Num;
  unsigned char Error_Byte;	
  unsigned char buff[4];
  unsigned char* Ptr;
  
  Ptr = buff;
  dst_addr = 0;
  src_addr = 0x200000+4096;
  EraseCode();//擦除片上程序
  Read_Flash((unsigned char*)&long_temp,0x200000,4);
  Num = long_temp/Buff_Len;
  if(long_temp%Buff_Len>0)
    Num+=1;
  for(i=0;i<Num;i++)
  {
    HT_FreeDog();
    Read_Flash(CheckBuf2,src_addr,Buff_Len);
    HT_Flash_ByteWrite(CheckBuf2,dst_addr,Buff_Len);
    
    HT_Flash_ByteRead(CheckBuf1,dst_addr,Buff_Len);
    if(Data_CompHL(CheckBuf1,CheckBuf2,Buff_Len)==0)
    {
      src_addr+=Buff_Len;
      dst_addr+=Buff_Len;
    }
    else
    {
      dst_addr = 0;
      src_addr = 0x200000+4096;
      EraseCode();//擦除片上程序
      i=0;				
    }
  }
  Error_Byte = CRC_NewProgram();
  return Error_Byte;
}


unsigned char CopyOldProgram(void)
{//保存旧程序
  unsigned long dst_addr;
  unsigned long src_addr;
  unsigned short  i;	
  unsigned char Copy_Flag;
  
  src_addr = 0;
  dst_addr = 0x280000;
  
  Read_Flash((unsigned char *)&Copy_Flag,0x20000C,1);
  if(Copy_Flag==1) return 1;
  for(i=0;i<20;i++)
  {
    HT_Flash_ByteRead(CheckBuf1,src_addr,Buff_Len);
    DataFlash_Write(dst_addr,CheckBuf1,Buff_Len);
    Read_Flash(CheckBuf2,dst_addr,Buff_Len);
    if(Data_CompHL(CheckBuf1,CheckBuf2,Buff_Len)==0)
    {
      src_addr+=Buff_Len;
      dst_addr+=Buff_Len;
    }
    else
    {
      src_addr = 0;
      dst_addr = 0x280000;
      i=0;
    }
  }
  Copy_Flag = 1;
  DataFlash_Write(0x20000C,&Copy_Flag,1);
  return 0;
}

unsigned char DownloadOldProgram(void)
{//恢复旧程序
  unsigned long dst_addr;
  unsigned long src_addr;
  unsigned short  i;	
  dst_addr = 0;
  src_addr = 0x280000;
  for(i=0;i<20;i++)
  {
    Read_Flash(CheckBuf2,src_addr,Buff_Len);
    HT_Flash_ByteWrite(CheckBuf2,dst_addr,Buff_Len);
    
    HT_Flash_ByteRead(CheckBuf1,dst_addr,Buff_Len);
    if(Data_CompHL(CheckBuf1,CheckBuf2,Buff_Len)==0)
    {
      src_addr+=Buff_Len;
      dst_addr+=Buff_Len;
    }
    else
    {
      dst_addr = 0;
      src_addr = 0x280000;
      i=0;
    }
  }
  return 0;
}
void Progarm_Update(void)
{
  unsigned char buff[16];
  int Len;
  unsigned int Crc32_check,Crc32_check_s;
  
  Read_Flash(buff, 0x200000,12);
  Crc32_check = GenerateCRC32(0xffffffff, buff,8); 
  memcpy(&Crc32_check_s,buff+8,4);
  memcpy(&Len,buff,4);
  if((Crc32_check_s!=Crc32_check) || (Len<0x1000) || (Len>0x4000))
  {	
    return;
  }
  __disable_irq();
  CopyOldProgram();
  while(1)
  {        
    if(DownloadNewProgram())
    {
      DownloadOldProgram();
    }
    BlockErase(0x200000);
    NVIC_SystemReset();
  }
}