#include <string.h>
#include <stdio.h>
#include "serial.h"
#include "iec101.h"
#include "TypeRAM.h"
#include "TypeE2p.h"
#include "data.h"
#define justinit 1
#define initend 2
#define notinit 0

#define IEC101_PORT  2

#define yxbw   1
#define substinit   2
#define readcmd   3
#define substchange 4
#define nofirstdata 5 

#define FIXDMSG  "LINELOSS/FIXD/fixd01.msg"
#define FIXDXML  "LINELOSS/FIXD/fixd01.xml"
#define RANDMSG  "LINELOSS/RAND/rand01.msg"
#define RANDXML  "LINELOSS/RAND/rand01.xml"
#define FRZDMSG  "LINELOSS/FRZD/frzd01.msg"
#define FRZDXML  "LINELOSS/FRZD/frzd01.xml"
#define SHARPMSG "LINELOSS/SHARPD/sharpd01.msg"
#define SHARPXML "LINELOSS/SHARPD/sharpd01.xml"
#define MONMSG   "LINELOSS/MONTHD/monthd01.msg"
#define MONXML   "LINELOSS/MONTHD/monthd01.xml"
#define EVTMSG   "LINELOSS/EVENTD/eventd01.msg"
#define EVTXML   "LINELOSS/EVENTD/eventd01.xml"
#define ONE_ENERGY_NUM     32

static char Block_buf[768];
static int Blk_size;
static int Blk_ptr;
static int Record_no;
static int Record_num;
static int m_Channel_no;
int Assamble_TimeMsgFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0;
  unsigned long ul_val;     
  sprintf(Out,"07,%04d%02d%02d_%02d:%02d:%02d,",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d\r\n",ul_val);
  memcpy(Out+Len,tmp,strlen(tmp));
  Len += strlen(tmp);
  return Len;
}

int Assamble_TimeXmlFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0;
  unsigned long ul_val;     
  sprintf(Out,"<DI eventType=\"07\"");
  Len = strlen(Out);       
  sprintf(Out+Len,"tm=\"%04d%02d%02d_%02d:%02d:%02d_00\" txt=\"",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len += strlen(Out+Len);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d\"/>\r\n",ul_val);
  memcpy(Out+Len,tmp,strlen(tmp));
  Len += strlen(tmp);
  return Len;
}

int Assamble_CEVENTMsgFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0;
  unsigned long ul_val;     
  sprintf(Out,"06,%04d%02d%02d_%02d:%02d:%02d,",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d\r\n",ul_val);
  memcpy(Out+Len,tmp,strlen(tmp));
  Len += strlen(tmp);
  return Len;
}

int Assamble_CEVENTXmlFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0;
  unsigned long ul_val;     
  sprintf(Out,"<DI eventType=\"06\"");
  Len = strlen(Out);       
  sprintf(Out+Len,"tm=\"%04d%02d%02d_%02d:%02d:%02d_00\" txt=\"",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len += strlen(Out+Len);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d\"/>\r\n",ul_val);
  memcpy(Out+Len,tmp,strlen(tmp));
  Len += strlen(tmp);
  return Len;
}

//307
int Assamble_XmlFormat(int sect,char *In,char *Out)
{
  int Len=0,i;
  long l_val;
  unsigned long ul_val;
  float f_val;
  char tmp[64];
  sprintf(Out,"\t<DataRec sect=\"%d\" tm=\"%04d%02d%02d_%02d%02d%02d\">\r\n",sect,2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  for(i=0;i<8;++i)
  {
    memcpy(&ul_val,In+6+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"\t\t<DI val=\"%.3f\"/>\r\n",f_val);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  for(i=0;i<4;++i)
  {
    memcpy(&l_val,In+38+i*4,4);
    f_val = l_val;
    f_val = f_val/1000;
    sprintf(tmp,"\t\t<DI val=\"%.3f\"/>\r\n",f_val);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  strcpy(Out+Len,"\t</DataRec>\r\n");
  Len += strlen("\t</DataRec>\r\n");
  return Len; 
}
//200
//unsigned short InfoAddrList[12]={0x6401,0x};
int Assamble_MsgFormat(int sect,char *In,char *Out)
{
  int Len=0,i;
  signed long l_val;
  unsigned long ul_val;
  float f_val;
  char tmp[64];
  for(i=0;i<6;++i)
  {
    if(In[i]>60)
      In[i]=0;
  }
  sprintf(Out,"12,%04d-%02d-%02d %02d:%02d:%02d",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+6+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      if(strlen(tmp)>12)
        tmp[12] = 0;
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
            
  for(i=0;i<4;++i)
  {
    memcpy(&l_val,In+38+i*4,4);
    f_val = l_val;
    f_val = f_val/1000;
    sprintf(tmp,",%04X,",0x4008+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=10)
    {
      if(strlen(tmp)>12)
        tmp[12] = 0;
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',10-strlen(tmp));
      Len += 10-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\r\n",2);
  Len +=2;
  return Len;
}
//278
int Assamble_PnMsgFormat(int sect,char *In,char *Out)
{
  char tmp[64];
  int Len=0,i;
  unsigned long ul_val;
  float f_val;
  sprintf(Out,"%02d,",sect);
  Len=3;
  sprintf(Out+Len,"%04d-%02d-%02d %02d:%02d:%02d,",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d",ul_val);
  if(strlen(tmp)>=8)
  {
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  else
  {
    memset(Out+Len,' ',8-strlen(tmp));
    Len += 8-strlen(tmp);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  Out[Len++]=0x38;
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  sprintf(Out+Len,",%04d-%02d-%02d %02d:%02d:%02d",2000+In[47],In[46],In[45],In[44],In[43],In[42]);
  Len = strlen(Out);
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\r\n",2);
  Len +=2;
  return Len;
}
//146
int Assamble_PtMsgFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0,i;
  unsigned long ul_val;
  float f_val;
  sprintf(Out,"%02d,",4);
  Len=3;
  sprintf(Out+Len,"%04d-%02d-%02d %02d:%02d:%02d,",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d,8",ul_val);
  if(strlen(tmp)>=8)
  {
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  else
  {
    memset(Out+Len,' ',8-strlen(tmp));
    Len += 8-strlen(tmp);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
//  Out[++Len]=',';
//  Out[++Len]=0x38;
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\r\n",2);
  Len +=2;
  return Len;
}

int Assamble_PtXmlFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0,i;
  unsigned long ul_val;
  float f_val;
  sprintf(Out,"<DI eventType=\"04\"");
  Len = strlen(Out);
  sprintf(Out+Len,"tm=\"%04d%02d%02d_%02d:%02d:%02d_00\" txt=\"",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len += strlen(Out+Len);
  memcpy(&ul_val,In+6,4);
  sprintf(tmp,"%d,8",ul_val);
  if(strlen(tmp)>=8)
  {
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  else
  {
    memset(Out+Len,' ',8-strlen(tmp));
    Len += 8-strlen(tmp);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
//  Out[Len++]=0x38;
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\"/>\r\n",5);
  Len +=5;
   return Len;
}


int Assamble_CMsgFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0,i;
  unsigned long ul_val;
  float f_val;
  sprintf(Out,"%02d,",5);
  Len=3;
  sprintf(Out+Len,"%04d-%02d-%02d %02d:%02d:%02d,",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len = strlen(Out);
  //memcpy(&ul_val,In+6,4);
  ul_val = 0;
  sprintf(tmp,"%d,8",ul_val);
  if(strlen(tmp)>=8)
  {
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  else
  {
    memset(Out+Len,' ',8-strlen(tmp));
    Len += 8-strlen(tmp);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }

  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\r\n",2);
  Len +=2;
  return Len;
}


int Assamble_CXmlFormat(char *In,char *Out)
{
  char tmp[64];
  int Len=0,i;
  unsigned long ul_val;
  float f_val;
  sprintf(Out,"<DI eventType=\"05\"");
  Len = strlen(Out);
  sprintf(Out+Len,"tm=\"%04d%02d%02d_%02d:%02d:%02d_00\" txt=\"",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len += strlen(Out+Len);
  //memcpy(&ul_val,In+6,4);
  ul_val = 1;
  sprintf(tmp,"%d,8",ul_val);
  if(strlen(tmp)>=8)
  {
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  else
  {
    memset(Out+Len,' ',8-strlen(tmp));
    Len += 8-strlen(tmp);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }

  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\"/>\r\n",5);
  Len +=5;
   return Len;
}

int Assamble_PnXmlFormat(int ch,int phase,char *In,char *Out)
{
  char tmp[64];
  int Len=0,i;
  unsigned long ul_val;
  float f_val;
  sprintf(Out,"<DI eventType=\"%02d\"",phase+1);
  Len = strlen(Out);       
  sprintf(Out+Len,"tm=\"%04d%02d%02d_%02d:%02d:%02d_00\" txt=\"",2000+In[5],In[4],In[3],In[2],In[1],In[0]);
  Len += strlen(Out+Len);
  ul_val=GetPn_Event_num(ch,phase);
  sprintf(tmp,"%d,",ul_val);
  if(strlen(tmp)>=8)
  {
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  else
  {
    memset(Out+Len,' ',8-strlen(tmp));
    Len += 8-strlen(tmp);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
  }
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  sprintf(Out+Len,",%04d-%02d-%02d %02d:%02d:%02d",2000+In[47],In[46],In[45],In[44],In[43],In[42]);
  Len = strlen(Out);
  for(i=0;i<8;++i)
  {
    sprintf(tmp,",%04X,",0x6401+i);
    memcpy(Out+Len,tmp,strlen(tmp));
    Len += strlen(tmp);
    memcpy(&ul_val,In+10+i*4,4);
    f_val = ul_val;
    f_val = f_val/1000;
    sprintf(tmp,"%.3f",f_val);
    if(strlen(tmp)>=8)
    {
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
    else
    {
      memset(Out+Len,' ',8-strlen(tmp));
      Len += 8-strlen(tmp);
      memcpy(Out+Len,tmp,strlen(tmp));
      Len += strlen(tmp);
    }
  }
  memcpy(Out+Len,"\"/>\r\n",5);
  Len +=5;
  return Len;
}

typedef struct {
  u16 info_addr;
  u16 data_len;
  unsigned char *info_data;
}S_SNINFO;
#if MAX_CH_NUM==1
unsigned char s_Devtype[]="FTU";
#else
unsigned char s_Devtype[]="DTU";
#endif
unsigned char s_Operation[]="N/A";
unsigned char s_Manufacture[]="�滪";
unsigned char s_Hardwarever[]="B";
unsigned char s_Firmwarever[]="01.01";
unsigned char s_FirmwareCrc[]="0x7777";
unsigned char s_Protocolver[]="V1.000";
unsigned char s_Model[]="JH4000";
unsigned char s_Id[]="201710280001";
S_SNINFO s_sninfo[]=
{
  0x8001,3,s_Devtype,
  0x8002,3,s_Operation,
  0x8003,4,s_Manufacture,
  0x8004,1,s_Hardwarever,
  0x8005,5,s_Firmwarever,
  0x8006,6,s_FirmwareCrc,
  0x8007,6,s_Protocolver,
  0x8008,6,s_Model,
  0x8009,12,s_Id,
};


unsigned char *File_List[]={FIXDMSG,FIXDXML,RANDMSG,RANDXML,FRZDMSG,FRZDXML,SHARPMSG,SHARPXML,MONMSG,MONXML,EVTMSG,EVTXML};

static struct IEC101_STRUCT m_IEC101;
struct IEC101_STRUCT *lpIEC101=&m_IEC101;	//IEC101��Լ˽������ָ��

//175
int Send_LoadXmlFile_Head(char *buf)
{
      int byMsgNum=0;
      char tmp[64];
      strcpy(buf + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      strcpy(buf + byMsgNum,"FIXD");
      byMsgNum += strlen("FIXD");
      strcpy(buf + byMsgNum,"\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//96\" interval=\"15min\">\r\n");
      byMsgNum += strlen("\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");
      sprintf(tmp,"%d\" interval=\"15min\">\r\n",Record_num);
      strcpy(buf + byMsgNum,tmp);
      byMsgNum += strlen(tmp);
      return byMsgNum;
}
//158
int Send_RandXmlFile_Head(char *buf)
{
      int byMsgNum=0;
      char tmp[64];
      strcpy(buf + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      strcpy(buf + byMsgNum,"RAND");
      byMsgNum += strlen("RAND");
      strcpy(buf + byMsgNum,"\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      byMsgNum += strlen("\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      sprintf(tmp,"%d\">\r\n",Record_num);
      strcpy(buf + byMsgNum,tmp);
      byMsgNum += strlen(tmp);
      return byMsgNum;
}
//158
int Send_FrzdXmlFile_Head(char *buf)
{
      int byMsgNum=0;
      char tmp[64];
      strcpy(buf + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      strcpy(buf + byMsgNum,"FRZD");
      byMsgNum += strlen("FRZD");
      strcpy(buf + byMsgNum,"\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      byMsgNum += strlen("\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      sprintf(tmp,"%d\">\r\n",Record_num);
      strcpy(buf + byMsgNum,tmp);
      byMsgNum += strlen(tmp);
      return byMsgNum;
}
//160
int Send_SharpdXmlFile_Head(char *buf)
{
      int byMsgNum=0;
      char tmp[64];
      strcpy(buf + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      strcpy(buf + byMsgNum,"SHARPD");
      byMsgNum += strlen("SHARPD");
      strcpy(buf + byMsgNum,"\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      byMsgNum += strlen("\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      sprintf(tmp,"%d\">\r\n",Record_num);
      strcpy(buf + byMsgNum,tmp);
      byMsgNum += strlen(tmp);
      return byMsgNum;
}
//160
int Send_MonthdXmlFile_Head(char *buf)
{
      int byMsgNum=0;
      char tmp[64];
      strcpy(buf + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      strcpy(buf + byMsgNum,"MONTHD");
      byMsgNum += strlen("MONTHD");
      strcpy(buf + byMsgNum,"\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      byMsgNum += strlen("\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"");//3\">\r\n");
      sprintf(tmp,"%d\">\r\n",Record_num);
      strcpy(buf + byMsgNum,tmp);
      byMsgNum += strlen(tmp);
      return byMsgNum;
}
//205
int Send_XmlDataType1(char * buf)
{
  int byMsgNum=0;
  strcpy(buf + byMsgNum,"\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25602\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25603\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25604\" type=\"float\" unit=\"kVARh\" />\r\n");
  byMsgNum += strlen("\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25602\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25603\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25604\" type=\"float\" unit=\"kVARh\" />\r\n");
    return byMsgNum;
}
//205
int Send_XmlDataType2(char * buf)
{
  int byMsgNum=0;
  strcpy(buf + byMsgNum,"\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25605\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25606\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25607\" type=\"float\" unit=\"kVARh\" />\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25605\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25606\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25607\" type=\"float\" unit=\"kVARh\" />\r\n");
    return byMsgNum;
}
//208
int Send_XmlDataType3(char * buf)
{
  int byMsgNum=0;
  strcpy(buf + byMsgNum,"\t\t<DI ioa=\"16392\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16393\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16394\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16395\" type=\"float\" unit=\"W\" />\r\n \
  \t</DataAttr>\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"16392\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16393\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16394\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16395\" type=\"float\" unit=\"W\" />\r\n \
  \t</DataAttr>\r\n");
    return byMsgNum;
}
                         
//160
int Send_EventDXmlFile_Head(char *buf)
{
      int byMsgNum=0;
      char tmp[64];
      strcpy(buf + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"");
      strcpy(buf + byMsgNum,"EVENTD");
      byMsgNum += strlen("EVENTD");
      strcpy(buf + byMsgNum,"\" fileVer=\"1.00\" devID=\"201709030019\" />\r\n");
      byMsgNum += strlen("\" fileVer=\"1.00\" devID=\"201709030019\" />\r\n");
      return byMsgNum;
}
#define YX_BIT   0x80
u8 GetYx(u16 wYxTNo,u8 yx_bit)
{
  int ch;
  int yx_no;
  u8 flag;
  ch = wYxTNo/4;
  if(ch>=MAX_CH_NUM)
    return 0;
  yx_no =  wYxTNo%4;
  if(SM.PQFlag_b[ch]&(1<<yx_no))
    flag = 1;
  else
    flag = 0;
  return flag;
}                         
long GetYc(unsigned short ycno)
{
  long *Ptr;
  Ptr=(long*)&Real_Data[ycno/23];
  return Ptr[ycno%23];
}
short GetDd(unsigned short kwhno,unsigned char *buf)
{
  int m_ch,ptr_v;
  unsigned long *Ptr;
  unsigned long m_Lval;
  unsigned char tmp_buf[64];
  float f_val;
  m_ch=kwhno/8;
  ptr_v=kwhno%8;
  if(ptr_v<8)
  {
   // Ptr=(unsigned long*)&Energy_Data[m_ch];
    //memcpy(buf,&Ptr[ptr_v],4);
    Ptr=(unsigned long*)&Energy_Data[m_ch];
    f_val = Ptr[ptr_v]; 
    f_val = f_val/1000;
    memcpy(buf,(unsigned char *)&f_val,4);
    //*(buf) = tmp_buf[0];//(u8)nVal;	// ң��ֵ
//		*(lpby + byMsgNum ++) = tmp_buf[1];//(u8)(nVal >> 8);
  //                      *(lpby + byMsgNum ++) = tmp_buf[2];//(u8)(nVal >> 16);
    //                    *(lpby + byMsgNum ++) = tmp_buf[3];//(u8)(nVal >> 24);
    return 4;
  }
#if 0  
  else
  {
    Read_LastData(m_ch,tmp_buf);
    memset(buf,0,12);
   // memcpy(buf,tmp_buf+6+4*(ptr_v-8),4);
    //Ptr = (unsigned long*)tmp_buf[6+4*(ptr_v-8)];
    //f_val = Ptr[0];
    memcpy(&m_Lval,tmp_buf+6+4*(ptr_v-8),4);
    f_val = m_Lval;
    f_val = f_val/1000;
    memcpy(buf,(unsigned char *)&f_val,4);
    memcpy(buf+6,tmp_buf,6);
    return 12;
  }
#endif
}
//���ܣ����մ�����
//���룺void
//�����void
void IEC101RErrorProcess(void)
{
	lpIEC101->wRecvNum = 0;
	lpIEC101->RErrorN++;	   	//��Լ���մ�������
	if(lpIEC101->RErrorN>=MaxErrorN)
	{
		lpIEC101->RErrorN=0;	   //��Լ���մ�������
		if((lpIEC101->initstatus == initend) &&(lpIEC101->FtuSccReset == FALSE))
	   	{
	    		//fnInitSCC(ChanlNo,SCCERROR); //��λͨ��
	   		lpIEC101->FtuSccReset = TRUE; //��վ״̬�ı�
		}
	}
}
                         
void Read_Para(unsigned char *buf)
{
  unsigned char Len,Count,i;
  unsigned short yc_no;
  unsigned long val;
  Len = buf[0];
  *(buf-1) |=0x80;
  yc_no = Len;
  switch(buf[1])
  {
  case 0x7A:
    GetATT7022ECalibrateReg(buf+4,buf[2]);
    yc_no = Len+3;
    break;
  case 0x7B:
    yc_no = buf[3];
    yc_no = (yc_no<<8)|buf[2];
    val = GetYc(yc_no);
    memcpy(buf+4,&val,4);
    yc_no = Len+4;
    buf[0] = yc_no;
    break;
  case 0x7E:
    break;
  default:
    break;
  }
  buf=buf-9;
  Count =0;
  
  for(i=0;i<yc_no+10;++i)
    Count += buf[i];
  buf[i] = Count;
  buf[i+1] = 0x16;
  Serial_Write(IEC101_PORT,buf,yc_no+12);
}
void Write_Para(unsigned char *buf)
{
  unsigned char Len;
  Len = buf[0];
  *(buf-1) |=0x80;
  switch(buf[1])
  {
  case 0x7A:
    ComAdjWrite(buf+3,buf[2]);
    break;
  case 0x7E:
    NCom_WriteCPU_RTC(buf+2);
    break;
  case 0x7d:
    if((buf[2]==0xaa) && (buf[3]==0x55))
      Flag.Power |=F_IrmsCheck;
    else if((buf[2]==0x55) && (buf[3]==0xaa))
      NVIC_SystemReset();
    break;
  default:
    break;
  }
  Serial_Write(IEC101_PORT,buf-9,Len+12);
}                         
//��·����պ���
//���ܣ���·���գ��Ѵ��ڽ��ջ������е����ݽ��һ��
//      ������lpIEC101->PRecvFrame.byLinkBuf
//      ȡ����·���ַ
//���룺void
//�����void

void Iec101LinkRecv(void)
{
  u8 Count,cmd;
  int i,num,ptr;
  Count=Serial_Read(IEC101_PORT,lpIEC101->byRecvBuf+lpIEC101->wRecvNum,128);
  if(Count)
  {
    lpIEC101->wRecvNum +=Count;
    if(lpIEC101->byRecvBuf[0]==0x7A)
    {
      if(lpIEC101->wRecvNum>4)
      {
        lpIEC101->wRecvNum=0;
        if(lpIEC101->byRecvBuf[3]==0xf8)
           GetATT7022ECalibrateReg(lpIEC101->byRecvBuf+2,lpIEC101->byRecvBuf[1]);
        else     
           ComAdjWrite(lpIEC101->byRecvBuf+2,lpIEC101->byRecvBuf[1]);
        Serial_Write(IEC101_PORT,lpIEC101->byRecvBuf,6);
      }
      return;
    }
    if(lpIEC101->byRecvBuf[0]==0x7E)
    {
      Count = 0;
      for(i=0;i<5;++i)
        Count += lpIEC101->byRecvBuf[i];
      if(lpIEC101->wRecvNum>5)
      {
        if(lpIEC101->byRecvBuf[5] == Count)
        {  
          lpIEC101->byRecvBuf[1]=NCom_WriteCPU_RTC(lpIEC101->byRecvBuf+1);
          Serial_Write(IEC101_PORT,lpIEC101->byRecvBuf,2);
        }
        lpIEC101->wRecvNum = 0;
      }
      return;
    }
    if(lpIEC101->byRecvBuf[0]==0x7D)
    {
      Count = 0;
      for(i=0;i<5;++i)
        Count += lpIEC101->byRecvBuf[i];
      if(lpIEC101->wRecvNum>5)
      {
        
        if(lpIEC101->byRecvBuf[1] == 0xaa && lpIEC101->byRecvBuf[2] == 0x55)
        {
          Flag.Power |=F_IrmsCheck;
        }
        if(lpIEC101->byRecvBuf[1] == 0x55 && lpIEC101->byRecvBuf[2] == 0xaa)
        {
          NVIC_SystemReset();
        }
        Serial_Write(IEC101_PORT,lpIEC101->byRecvBuf,lpIEC101->wRecvNum);
        lpIEC101->wRecvNum = 0;
      }
      return;
    }
    if(lpIEC101->byRecvBuf[0]==0xfe)
    {
      if(lpIEC101->wRecvNum>12)
      {
        for(i=0;i<10;++i)
        {
          if(lpIEC101->byRecvBuf[i]!=0xfe)
          {
            break;
          }
        }
        ptr = i;
        if(lpIEC101->byRecvBuf[i]!=0x68)
        {
          lpIEC101->wRecvNum=0;
          return;
        }
        while((lpIEC101->byRecvBuf[++i]!=0x68) && (i<lpIEC101->wRecvNum)) ;
        if(lpIEC101->wRecvNum>(i+4))
        {
          if(lpIEC101->byRecvBuf[i+2] <= (lpIEC101->wRecvNum-i-5))
          {
            Count = 0;
            for(i=ptr,num=lpIEC101->byRecvBuf[i+9]+10+i;i<num;++i)
              Count += lpIEC101->byRecvBuf[i];
            if(Count==lpIEC101->byRecvBuf[i])
            {
              cmd = lpIEC101->byRecvBuf[ptr+8];
              switch(cmd)
              {
              case 0x11:  //������
                Read_Para(lpIEC101->byRecvBuf+ptr+9);
                break;
              case 0x14:  //д����
                Write_Para(lpIEC101->byRecvBuf+ptr+9);
                break;
              }
            }
            lpIEC101->wRecvNum = 0;
          }
        }
      }
      return;
    }
    for(i=0;i<lpIEC101->wRecvNum;++i)
    {  
      if(lpIEC101->byRecvBuf[i]!=F_STARTCODE && lpIEC101->byRecvBuf[i]!=V_STARTCODE)
      {
        continue;
      }
      if(lpIEC101->byRecvBuf[i]==F_STARTCODE)
      {
        if(lpIEC101->wRecvNum-i>=5 && (lpIEC101->byRecvBuf[lpIEC101->wRecvNum-i-1]==0x16))
        {  
          memcpy(lpIEC101->PRecvFrame.byLinkBuf,lpIEC101->byRecvBuf+i,lpIEC101->wRecvNum-i);
          lpIEC101->wRecvCmmAdd.Byte.l = lpIEC101->PRecvFrame.byLinkBuf[2];
          if(lpIEC101->TypeLinkAdd==2)
          {
            lpIEC101->wRecvCmmAdd.Byte.h = lpIEC101->PRecvFrame.byLinkBuf[3];
          }
          lpIEC101->wRecvNum=0;
          lpIEC101->PRecvFrame.byFull=1;
        }
        break;
      }
      if(lpIEC101->byRecvBuf[i]==V_STARTCODE)
      {
        if(lpIEC101->wRecvNum-i<5)
          return;
        if((lpIEC101->byRecvBuf[i]==lpIEC101->byRecvBuf[i+3])
					&& (lpIEC101->byRecvBuf[i+1]==lpIEC101->byRecvBuf[i+2]))
        {
          lpIEC101->wRecvLen = lpIEC101->byRecvBuf[i+1] + 6;
          if((lpIEC101->wRecvNum-i)>=lpIEC101->wRecvLen)
          {
            //Serial_Write(2,"Long Frame\r\n",12);
            memcpy(lpIEC101->PRecvFrame.byLinkBuf,lpIEC101->byRecvBuf+i,lpIEC101->wRecvNum-i);
            lpIEC101->wRecvCmmAdd.Byte.l = lpIEC101->PRecvFrame.byLinkBuf[5];
            if(lpIEC101->TypeLinkAdd==2)
            {
              lpIEC101->wRecvCmmAdd.Byte.h = lpIEC101->PRecvFrame.byLinkBuf[6];
            }
            lpIEC101->wRecvNum=0;
            lpIEC101->PRecvFrame.byFull=1;
          }
          break;
        }
      }
    }
    if(lpIEC101->wRecvNum)
    {
      if(i)
      {
        if(lpIEC101->wRecvNum>i)
        {
          memcpy(lpIEC101->byRecvBuf,lpIEC101->byRecvBuf+i,lpIEC101->wRecvNum-i);
          lpIEC101->wRecvNum=lpIEC101->wRecvNum-i;
        }
        else
        {
          lpIEC101->wRecvNum=0;
        }
      }
    }
  }
}
//�Ӷ���·����մ�����
//���ܣ�������·������ִ����Ӧ�Ĳ�����
//      1�����ݹ�����ִ�ж���
//      2��FCBλ�жϺ�����
//���룺��·������
//�����void

void PLinkRecvProcessF(u8 byConField)
{
  lpIEC101->PRecvFrame.byFunCode = byConField & 0x0F;
  if (lpIEC101->initstatus == notinit)	    
  {
    if ( (lpIEC101->PRecvFrame.byFunCode == CALL_LINK)
              ||(lpIEC101->PRecvFrame.byFunCode == RESET_LINK) )
    {
      if (lpIEC101->PRecvFrame.byFunCode == CALL_LINK)
      {
        lpIEC101->PSeAppLayer.LinkFunCode = 0x0B;
        if (lpIEC101->haveset == TRUE)
        {
          lpIEC101->initstatus = justinit;
        }
      }		
      else if (lpIEC101->PRecvFrame.byFunCode == RESET_LINK)
      {
        lpIEC101->PfcbC = 0;
        lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
	lpIEC101->haveset = TRUE;
        lpIEC101->initstatus = justinit;
        lpIEC101->PWinTimer=0;
        lpIEC101->byPSGenStep=0;
        lpIEC101->byPSDdStep=0;
        lpIEC101->PReMsgType=0;
      }
      lpIEC101->PSeAppLayer.byMsgNum = 0;
      lpIEC101->PSeAppLayer.byFull = 1;
    }
    return;
  }
#if 0 
  //if ( ((byConField & 0x20)==(lpIEC101->PfcbC<<5)) && (byConField & 0x10)&& lpIEC101->PfcvC)
  if((byConField & 0x10) && ((byConField & 0x20)==(lpIEC101->PfcbC<<5)))
  {
    lpIEC101->PSendFrame.byFull = 1;
    return;
  }
#endif  
  lpIEC101->PfcvC = byConField & 0x10;
  if((byConField & 0x40)==0)
  {
    switch(lpIEC101->PRecvFrame.byFunCode)
    {
    case RESET_LINK:
      if (lpIEC101->initstatus == justinit)
        {
          lpIEC101->PReFrameType = CALL_DATA1;
          lpIEC101->firstData = substinit;		// ��վ��ʼ��
          lpIEC101->wCmmAdd.Byte.l = 1;
          lpIEC101->wCmmAdd.Byte.h = 0;
          lpIEC101->byCOI = 2;
          lpIEC101->Pacd = 3;
          lpIEC101->Pdfc = 1;
          lpIEC101->PSendFrame.byFunCode = 3;
          lpIEC101->byPSGenStep=0;
          lpIEC101->byPSDdStep=0;
	}
       // else
       // {
      //    lpIEC101->PReMsgType=0;
      //  }
      break;
    case LINK_GOOD:
      lpIEC101->PReFrameType = lpIEC101->PRecvFrame.byFunCode;
      lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
      lpIEC101->Pacd = 2;
      break;
    default:
      lpIEC101->PSendFrame.byFull=0;
      break;
    }
    return;
  }
 
  if ( (lpIEC101->PRecvFrame.byFunCode == CALL_LINK) 
		||(lpIEC101->PRecvFrame.byFunCode == RESET_LINK)
		||(lpIEC101->PRecvFrame.byFunCode == CALL_ACD) )
  {
    if (lpIEC101->PRecvFrame.byFunCode == RESET_LINK)
    {
     // lpIEC101->PReMsgType=0;
      lpIEC101->PfcbC = 0;
      lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
      lpIEC101->firstData = nofirstdata;
      lpIEC101->initstatus = justinit;
      lpIEC101->byPSGenStep=0;
      lpIEC101->byPSDdStep=0;
     // lpIEC101->PWinTimer=0;
    }
    if (lpIEC101->PRecvFrame.byFunCode == CALL_LINK)
    {
      lpIEC101->PSeAppLayer.LinkFunCode = 0x0B;
      if (lpIEC101->haveset == TRUE)
        {
          lpIEC101->initstatus = justinit;
        }
    }
    if (lpIEC101->PRecvFrame.byFunCode == CALL_ACD)
      lpIEC101->PSeAppLayer.LinkFunCode = 0x0B;
		
    lpIEC101->PSeAppLayer.byMsgNum = 0;
    lpIEC101->PSeAppLayer.byFull = 1;
  }
  if(lpIEC101->PfcvC)
  //if ((lpIEC101->PRecvFrame.byFunCode == CALL_DATA1)||(lpIEC101->PRecvFrame.byFunCode == CALL_DATA2))
  {
    lpIEC101->PReFrameType = lpIEC101->PRecvFrame.byFunCode;
    lpIEC101->PfcbC = byConField & 0x20 ? 1 : 0;	// FCVλ��Ч,�򱣴�FCBλ
  }       
}
//�Ӷ�Ӧ�ò���մ�����
//���ܣ�������·�����ּ����ͱ�ʶ����Ӧ�Ĵ���
//���룺void
//�����void

void PLinkRecvProcessV(u8 byConField)
{
  u8 byFrameCount,i; 
#if 0       
  if ( ((byConField & 0x20)==(lpIEC101->PfcbC<<5)) && (byConField & 0x10))
  {
    lpIEC101->PSendFrame.byFull = 1;
    return;
  }
#endif 
  if ( ((byConField & 0x20)==(lpIEC101->PfcbC<<5)) && (byConField & 0x10)&& lpIEC101->PfcvC)
  {
    lpIEC101->PSendFrame.byFull = 1;
    return;
  }
  lpIEC101->PfcvC = byConField & 0x10;
  lpIEC101->PReAppLayer.lpByBuf = lpIEC101->PRecvFrame.byLinkBuf;
  lpIEC101->PRecvFrame.byFunCode = byConField & 0x0F;
  if(3==lpIEC101->PRecvFrame.byFunCode)
  {
    if(3==lpIEC101->UnsolTimeInterval)    //�ȷ���·ȷ���ٵȴ��ٻ���������
    {
      lpIEC101->PReFrameType = 3;
      lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
      lpIEC101->PSeAppLayer.byMsgNum = 0;
      lpIEC101->PSeAppLayer.byFull = 1;
    }
  }
  if(lpIEC101->PfcvC)
    lpIEC101->PfcbC = byConField & 0x20 ? 1 : 0;	// FCVλ��Ч,�򱣴�FCBλ
  byFrameCount=5+lpIEC101->TypeLinkAdd; //ָ�����ͱ�ʶ
  lpIEC101->PReMsgType = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];	// ���ͱ�ʶ
  lpIEC101->byQualify = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// �ɱ�ṹ�޶���
  lpIEC101->byReason  = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// ����ԭ��
  if(lpIEC101->TypeSeReason==2)
    lpIEC101->bySourceAdd = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];  //����ԭ��Ϊ�����ֽ�ʱȡ��Դ��ַ fu 2005.9.1
  lpIEC101->wCmmAdd.Byte.l = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// Ӧ�÷������ݵ�Ԫ�Ĺ�����ַ
  if(lpIEC101->TypeCmmAdd==2)
    lpIEC101->wCmmAdd.Byte.h = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// Ӧ�÷������ݵ�Ԫ�Ĺ�����ַ
  lpIEC101->dwInfAdd.Dword = 0;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    lpIEC101->dwInfAdd.Byte[i] =  lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]; 
  lpIEC101->byPSGenStep = 0;
  lpIEC101->byPSDdStep = 0;
  switch(lpIEC101->PRecvFrame.byFunCode)
  {
  case TRAN_CONFIRM_DATA:
      lpIEC101->PSeAppLayer.LinkFunCode = YES_ACK;
      break;
  case CALL_DATA1:
  case CALL_DATA2:
      lpIEC101->PSeAppLayer.LinkFunCode = RESP_NO_DATA;
      break;
  case CALL_ACD:
  case CALL_LINK:
      lpIEC101->PSeAppLayer.LinkFunCode = LINK_GOOD;
      break;
  }
  if(lpIEC101->FlagPingH==0)
  {
    lpIEC101->PSeAppLayer.byFull = 0;
    lpIEC101->PSendFrame.byFull = 0;
  }
  switch(lpIEC101->PReMsgType)
  {
  case GeneralCall: //���ٻ�
	lpIEC101->byQOI = lpIEC101->PReAppLayer.lpByBuf[byFrameCount];//[lpIEC101->PRecvFrame.wFrameLen-3]; //ָ��QOI
        if ( (lpIEC101->byReason == ACT) || (lpIEC101->byReason == DEACT) )
          lpIEC101->byPSGenStep = 0;
        else if (lpIEC101->byReason == REQ)
          lpIEC101->byPSGenStep = lpIEC101->byQOI - 20;
    //  lpIEC101->PReFrameType = CALL_DATA2; //zzl ����������վ
  //    lpIEC101->PSeAppLayer.byFull=0;
        lpIEC101->frameno = 0;
        break;
  case C_CS_NA_1:  //Уʱ
	{
                lpIEC101->PReFrameType = 0xff;
                if(lpIEC101->byReason == ACT)
                  SettimeToCan(lpIEC101->PReAppLayer.lpByBuf+byFrameCount); //fulianqiang 2005.9.12,��ʱ����������ʹ�ó�����Ѱַ
	}
        break;
  case C_CI_NA_1: //�ٻ����
	{
          lpIEC101->byQCC = lpIEC101->PReAppLayer.lpByBuf[byFrameCount];//[lpIEC101->PRecvFrame.wFrameLen-3];
	//lpIEC101->byPSDdStep = (lpIEC101->byQCC & 0x3F);  //ȡʵ�ʽ��յ���ֵ��ԭ����ȥһ��
          break;
	}
  case C_RD_NA_1: //������
	{
		lpIEC101->dwReadAd.Dword = lpIEC101->dwInfAdd.Dword;
	}
        break;
  case C_RC_NA_1:
  case C_DC_NA_1:
  case C_SC_NA_1: //����˫��ң��	
	{
                byFrameCount -= lpIEC101->TypeInfAdd;
		lpIEC101->byDCO = lpIEC101->PReAppLayer.lpByBuf[byFrameCount];
                byFrameCount+=2;
                if(lpIEC101->PReAppLayer.lpByBuf[byFrameCount]==0)
                {
                  if(lpIEC101->byDCO%2) //����
                  {
                    Clear_E2R((lpIEC101->byDCO-1)/2);
                  }
                  else  //�¼�
                  {
                    Clear_EVT2R((lpIEC101->byDCO-2)/2);
                  }
                }
		lpIEC101->WaitYkXzRet = 0;
		lpIEC101->WaitYkZxRet = 0;
		lpIEC101->SendYkZxAck = 0;
	}
        break;
  case C_TS_NA_1:  //����
	{
		lpIEC101->wTester.Byte.l =lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
		lpIEC101->wTester.Byte.h =lpIEC101->PReAppLayer.lpByBuf[byFrameCount]; 
	}
        break;
  case C_RR_NA_1:
    break;
  case C_RS_NA_1:
          lpIEC101->PReFrameType = 0xff;
          byFrameCount -= lpIEC101->TypeInfAdd;
          lpIEC101->Sn = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
          lpIEC101->Sn = (lpIEC101->Sn) | (lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]<<8);
          if(lpIEC101->byQualify) 
          {
            for(i=0;i<lpIEC101->byQualify;++i)
            {
              lpIEC101->List_para[i]=lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
              lpIEC101->List_para[i]=(lpIEC101->List_para[i]) | (lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]<<8);
            }
          }
          else //��ֵ��������в���
          {
          }
    break;
  case C_WS_NA_1:
    byFrameCount -= lpIEC101->TypeInfAdd;
    lpIEC101->Sn = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
    lpIEC101->Sn = (lpIEC101->Sn) | (lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]<<8);
    //if(lpIEC101->byQualify)
    {
      unsigned char sn_tag;
      //unsigned char tt_buf[8];
      float f_val;
      float *p_val;
      unsigned char *p_buf;
      short i;
      u16 info_addr;
      sn_tag = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
      switch(sn_tag)
      {
      case 0x00: //�̻�
        for(i=0;i<MAX_CH_NUM;++i)
        {  
          p_buf =(unsigned char*)&(m_ecpara[i].cmon_day);
          E2P_WData(CMon_DAY0+i*25,p_buf,24);
        }
        break;
      case 0x40:  //ȡ��
        
        for(i=0;i<MAX_CH_NUM;++i)
        {  
          p_buf =(unsigned char*)&(m_ecpara[i].cmon_day);
          E2P_RData(p_buf,CMon_DAY0+i*25,24);
        }
        break;
      case 0x80:
        for(i=0;i<lpIEC101->byQualify;++i)
        {
          info_addr = lpIEC101->PReAppLayer.lpByBuf[byFrameCount+1];
          info_addr = (info_addr<<8)+(lpIEC101->PReAppLayer.lpByBuf[byFrameCount]);
          byFrameCount +=2;
          //memcpy(tt_buf,lpIEC101->PReAppLayer.lpByBuf+byFrameCount,8);
          memcpy(&f_val,lpIEC101->PReAppLayer.lpByBuf+byFrameCount+2,4);
          p_val = &(m_ecpara[0].cmon_day);
          *(p_val+(info_addr-0x8021)) = f_val;
          byFrameCount += 2 + lpIEC101->PReAppLayer.lpByBuf[byFrameCount+1];
        }
        break;
      case 0x79:
        for(i=0;i<lpIEC101->byQualify;++i)
        {
          info_addr = lpIEC101->PReAppLayer.lpByBuf[byFrameCount+1];
          info_addr = (info_addr<<8)+(lpIEC101->PReAppLayer.lpByBuf[byFrameCount]);
          byFrameCount +=2;
          ComAdjWrite(lpIEC101->PReAppLayer.lpByBuf+byFrameCount+2,info_addr-0x8021);
          byFrameCount += 2 + lpIEC101->PReAppLayer.lpByBuf[byFrameCount+1];
        }
        break;
      default:
        break;
      }
    }
    break;
  case F_FR_NA_1:
    lpIEC101->PReFrameType = 0xff;
    if(lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]==0x02) //�������ݰ�����
    {
      lpIEC101->Fop = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
      if(lpIEC101->Fop!=3)
      {  
        memcpy(&(lpIEC101->FId),lpIEC101->PReAppLayer.lpByBuf+byFrameCount,4);
        byFrameCount += 4;
      }
      memset(lpIEC101->Fname,0,32);
      if(lpIEC101->PReAppLayer.lpByBuf[byFrameCount])
      {
        memcpy(lpIEC101->Fname,lpIEC101->PReAppLayer.lpByBuf+byFrameCount+1,lpIEC101->PReAppLayer.lpByBuf[byFrameCount]);
       // byFrameCount += lpIEC101->PReAppLayer.lpByBuf[byFrameCount];
      }
      byFrameCount += lpIEC101->PReAppLayer.lpByBuf[byFrameCount]+1;
      lpIEC101->byDCO = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
    }
    else
    {
      lpIEC101->PReMsgType = 0;
    }
    break;
  default:
    {
      lpIEC101->PReMsgType = 0;
    }
    break;
  }
}

//��·����մ�����
//���ܣ�ȡ����·�����֣����ô�����
//���룺void
//�����void

void Iec101LinkRecvPro(void)
{
  u8 byConField;
  if (lpIEC101->PRecvFrame.byFull == 1)
  {
    lpIEC101->Pacd = 0;
    lpIEC101->Pdfc = 0;
    if(lpIEC101->PRecvFrame.byLinkBuf[0] == F_STARTCODE)
    {
      if(lpIEC101->TypeLinkAdd==2)
      {
	       if( (lpIEC101->wRecvCmmAdd.Word == lpIEC101->wLinkAdd.Word)||(lpIEC101->wRecvCmmAdd.Word ==0xFFFF))
	        {   
	           byConField = lpIEC101->PRecvFrame.byLinkBuf[1];
	           PLinkRecvProcessF(byConField);
		   lpIEC101->OrgnizeFrame = 1 ;
		}
		else
		   lpIEC101->OrgnizeFrame = 0 ;
	    }
	    else
	    {
	       if( (lpIEC101->wRecvCmmAdd.Byte.l == lpIEC101->wLinkAdd.Byte.l)||(lpIEC101->wRecvCmmAdd.Byte.l ==0xFF))
 		{
		   byConField = lpIEC101->PRecvFrame.byLinkBuf[1];
 		   PLinkRecvProcessF(byConField);
		   lpIEC101->OrgnizeFrame = 1 ;
		}
		else
 		   lpIEC101->OrgnizeFrame = 0 ;
	    }
	 }
	 else if(lpIEC101->PRecvFrame.byLinkBuf[0] == V_STARTCODE)
	 {
	    if(lpIEC101->TypeLinkAdd==2)
	    {
	       if( (lpIEC101->wRecvCmmAdd.Word == lpIEC101->wLinkAdd.Word)||(lpIEC101->wRecvCmmAdd.Word ==0xFFFF))
	        {   
	           byConField = lpIEC101->PRecvFrame.byLinkBuf[4];
	           PLinkRecvProcessV(byConField);
		   lpIEC101->OrgnizeFrame = 1;
		}
		else
		   lpIEC101->OrgnizeFrame = 0 ;
	    }
	    else
	    {
	       if( (lpIEC101->wRecvCmmAdd.Byte.l == lpIEC101->wLinkAdd.Byte.l)||(lpIEC101->wRecvCmmAdd.Byte.l ==0xFF))
 		{
		   byConField = lpIEC101->PRecvFrame.byLinkBuf[4];
 		   PLinkRecvProcessV(byConField);
		   lpIEC101->OrgnizeFrame =1;
		}
		else
		   lpIEC101->OrgnizeFrame = 0;
	    }
	}
	lpIEC101->PRecvFrame.byFull=0;
	if((lpIEC101->FtuSccReset ==TRUE)&&(lpIEC101->OrgnizeFrame == 1))
	{
		lpIEC101->FtuSccReset = FALSE;	  //ͨ���ָ�����
	}
    }   
}
//��CAN��Уʱ
void SettimeToCan(u8* pbyBuf)
{ 
  unsigned char Time_buf[8];
    unsigned short year;

    MoveCurrentTimeBCD_Hex();
    Time_buf[0]=Clk.SecH;
    Time_buf[1]=Clk.MinH;
    Time_buf[2]=Clk.HourH;
    Time_buf[3]=Clk.DayH;
    Time_buf[4]=Clk.Month;
    year = Clk.YearH;
    year = year*256 + Clk.YearL;
    Time_buf[5]=year-2000;
    Save_JIAOSHI(Time_buf);
#if 0
        unsigned short wTotalMs;
	wTotalMs = (unsigned short)pbyBuf[1];
	wTotalMs = pbyBuf[0]|(wTotalMs<<8);
	sClock.wMS = wTotalMs % 1000;
	sClock.bySecond = (u8)(wTotalMs / 1000);
	//Уʱ֡�ķ����ֽ����λʱ����Ч��־δ��
	sClock.byMinute = pbyBuf[2] & 0x3F;
	sClock.byHour = pbyBuf[3] & 0x1F;
	sClock.byDay = pbyBuf[4] & 0x1F; 
	lpIEC101->byDayofWeek  = (pbyBuf[4] & 0xE0) >> 5;
	sClock.byMonth = pbyBuf[5] & 0x0F;
	sClock.wYear = (pbyBuf[6] & 0x7F)+2000;     
#else
        unsigned char pRTCTime[8];
        unsigned short wTotalMs;
	wTotalMs = (unsigned short)pbyBuf[1];
	wTotalMs = pbyBuf[0]|(wTotalMs<<8);
        pRTCTime[0] = (pbyBuf[4] & 0xE0) >> 5; //week
        pRTCTime[1] = (pbyBuf[6] & 0x7F); //year
        pRTCTime[2] = pbyBuf[5] & 0x0F;//month
        pRTCTime[3] = pbyBuf[4] & 0x1F; //day
        pRTCTime[4] = pbyBuf[3] & 0x1F;
        pRTCTime[5] = pbyBuf[2] & 0x3F;
        pRTCTime[6] = (u8)(wTotalMs / 1000);
        HT_RTC_Write(pRTCTime);
        GetTime();
        MoveCurrentTimeBCD_Hex();
#endif        
}
//�Ӷ�Ӧ�ò���մ�����
//���ܣ�������·�����ּ����ͱ�ʶ����Ӧ�Ĵ���
//���룺void
//�����void

void SleaveIec101Frame(void)
{
	//��·����պ���
	Iec101LinkRecv();
	//��·����մ�����
	Iec101LinkRecvPro();
}
//���ӴӶ�����
void WatchPWindow(void)
{ 
	if ((lpIEC101->PReMsgType) || lpIEC101->PSeAppLayer.byFull || lpIEC101->PSendFrame.byFull)
	{
		lpIEC101->PWindow = 1;
	}
        if((lpIEC101->PRecvFrame.byFunCode == RESET_LINK) && lpIEC101->FlagPingH && (lpIEC101->initstatus == justinit))
        {
          lpIEC101->PWindow = 1;
        }
}
u8  OrgnizeTDdMsg(u8 bySendReason,u8 byFrameNo)     
{
  int i;
  FOUR_BYTE_TO_DWORD  dwInfoAd;
  int dwDdVal;
  u8 tmp_buf[ONE_RECORD_LEN];
  float f_val;
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 j,byMsgNum = 0;
  u8 byDdNPF;	//ÿ֡�����
  int dwDdAdd;
  byDdNPF=lpIEC101->DdNPF[byFrameNo];	//ȡ��ʵ�ʵ�ÿ֡�����,���һ֡���ܲ���32
  if (byFrameNo<lpIEC101->DdFN)	//֡��С����Ҫ���͵ĵ��֡��
  {
    
    *(lpby + byMsgNum ++) = M_IT_TC_1;	
    *(lpby + byMsgNum ++) = 0x80|byDdNPF;
    *(lpby + byMsgNum ++) = bySendReason;
    if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
      *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
    if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
      *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
    if(lpIEC101->TypeProtocol)
    {
      if(ONE_ENERGY_NUM!=8)
        dwDdAdd =  IEC101_DDSA_2002+8;
    }
    else
      dwDdAdd =  IEC101_DDSA;
    dwInfoAd.Dword = dwDdAdd+(byFrameNo)*ONE_ENERGY_NUM;
    dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
    for(j=0;j<lpIEC101->TypeInfAdd;j++)
      *(lpby + byMsgNum ++) = dwInfoAd.Byte[j];
    for (i = 0; i < byDdNPF;++i)
    {
      Read_LastData(byFrameNo,tmp_buf);
      memset(lpby + byMsgNum,0,12);
      memcpy(&dwDdVal,tmp_buf+6+4*i,4);
      f_val = dwDdVal;
      f_val = f_val/1000;
      memcpy(lpby + byMsgNum,(unsigned char *)&f_val,4);
      memcpy(lpby + byMsgNum+6,tmp_buf,6);
      byMsgNum +=12;  
    }
  }
  return byMsgNum;
}
//����һ������
void SearchFirstData(void)
{     u8 byMsgNum;
      PUSHSTATE m_push;
    	//short wYxChang,wSendNo;
    	if(lpIEC101->FlagPingH)	  //�����ƽ�ⷽʽ
    	{
          if((lpIEC101->PRecvFrame.byFunCode == RESET_LINK) && (lpIEC101->PWinTimer>200) && (lpIEC101->initstatus == justinit))
          {
            lpIEC101->PWinTimer = 0;
//            lpIEC101->PSeAppLayer.byFull = 1;
//            lpIEC101->PSeAppLayer.LinkFunCode = CALL_LINK;
//            lpIEC101->Pacd = 2;
            SendData1();
          }
          if(lpIEC101->PRecvFrame.byFunCode == LINK_GOOD)
          {
            lpIEC101->PSeAppLayer.byFull = 1;
            lpIEC101->PSeAppLayer.LinkFunCode = RESET_LINK;
            lpIEC101->PRecvFrame.byFunCode = 0;
            lpIEC101->Pacd = 2;
          }
        /*  if(lpIEC101->wTester.Byte.l && lpIEC101->PWinTimer>200)
          {
            lpIEC101->PWinTimer = 0;
            lpIEC101->PSeAppLayer.LinkFunCode=TRAN_CONFIRM_DATA;
            lpIEC101->PSendFrame.byFunCode = TRAN_CONFIRM_DATA;
            lpIEC101->PReMsgType = C_TS_NA_1; 
          }*/
          if(Get_Valid_Push(&m_push))
          {
            switch(m_push.Type)
            {
            case LOAR_TYPE:
              byMsgNum = OrgnizeTDdMsg(SPONT,m_push.Ch);
              lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
              lpIEC101->PSeAppLayer.LinkFunCode = 4;
              break;
            default:
              break;
            }
            if(lpIEC101->PSeAppLayer.byMsgNum)
            {
              lpIEC101->Pacd = 2;
              //  lpIEC101->PSendFrame.byFull = 1;
              lpIEC101->PSeAppLayer.byFull = 1;
            }
          }
          return;
        }
	if (lpIEC101->firstData != nofirstdata)	// ����1���û�����,��ͣ����
   		return;
	if (lpIEC101->initstatus == justinit)
	{
		lpIEC101->firstData = substinit;		// ��վ��ʼ��
	}
#if 0       
	else if((lpIEC101->YxChangeC != sHostSoe.bySoeWrite)&&(initend==lpIEC101->initstatus))
	{
		wYxChang=lpIEC101->YxChangeC ;
		while (wYxChang != sHostSoe.bySoeWrite)
		{
			//����SOE��Ӧ��ң�ű�λ
			wSendNo = sHostSoe.SoeQueue[wYxChang].byYXSendSN;
			//�����ж�Ŀ���ǰ��շ��ͱ�ң�ű�λfulianqiang 2006.1.10
			if((wSendNo <= lpIEC101->SendYxN)&&(pYXSendTab[wSendNo].byUnitType==FTU))    //�жϷ���˳������Ƿ��д�ң��,�����ң�ű�λ
			{
				lpIEC101->firstData = yxbw;		// ң�ű�λ
				lpIEC101->YxChangeC= wYxChang;
				break;
			}
			if((++wYxChang) >= MAX_SOE_NUM )   //�ƶ�soe��ָ��
			      	wYxChang = 0;
		 }
	}
#endif        
    	if(nofirstdata==lpIEC101->firstData)
	{
		if (lpIEC101->dwReadAd.Dword)
	    	{
	   		lpIEC101->firstData = readcmd;		// �ɶ���������Ѱַ����Ϣ�������
	   	}
	}
	if (lpIEC101->firstData != nofirstdata)	// ������1���û�����
	{
		lpIEC101->Pacd = 1;			// ACD��λ
	}
	else
		lpIEC101->Pacd=0;        
}
//��ң�ű�λ��Ϣ��
u8 OrgnizeYxbwMsg(u8 *lpby)
{
#if 0 
        WORD wSendNo;
	FOUR_BYTE_TO_DWORD  dwInfoAd;
	u8 i,byMsgNum = 0;
	u8 * lpInfoNum;		   	// ��Ϣ����Ŀ��ַ û��Ҫ�Ӹ�ָ��fulianqiang 2005.9.2
	*(lpby + byMsgNum ++) = 1;   //���ͱ�ʶ	
	lpInfoNum = lpby + byMsgNum ++;	   //�ṹ�޶���lpby+1��Ӧ�ĵ�ַ
	*lpInfoNum = 0;
	*(lpby + byMsgNum ++) = 3;  //����ԭ��
	if(lpIEC101->TypeSeReason==2)		 //�Ƿ������ֽڵĴ���ԭ��
	   *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;		
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	//���SOE���е�ң�ű�λ������
	while (lpIEC101->YxChangeC != sHostSoe.bySoeWrite)
	{
		//����SOE��Ӧ��ң�ű�λ
		wSendNo = sHostSoe.SoeQueue[lpIEC101->YxChangeC].byYXSendSN;
		//�����ж�Ŀ���ǰ��շ��ͱ�ң�ű�λfulianqiang 2006.1.10
		if(wSendNo <= lpIEC101->SendYxN)		//�жϷ���˳������Ƿ��д�ң��
		{
                	dwInfoAd.Dword = wSendNo + IEC101_YXSA;
			dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	        	for(i=0;i<lpIEC101->TypeInfAdd;i++)		//��Ϣ���ַ
	           		*(lpby + byMsgNum ++) = dwInfoAd.Byte[i];
			if (sHostSoe.SoeQueue[lpIEC101->YxChangeC].byOnOff == 0x10)
				*(lpby + byMsgNum ++) = 0;	
			else
				*(lpby + byMsgNum ++) = 1;
			if( ++(*lpInfoNum) >= IEC101_YXBWNPF)
				break;
		}
		if((++lpIEC101->YxChangeC) >= MAX_SOE_NUM )   //�ƶ�soe��ָ��
		      	lpIEC101->YxChangeC = 0;

	}
	return byMsgNum;
#else
        return 0;
#endif        
}
//���ʼ��������Ϣ��
u8 OrgnizeInitEndMsg(u8* pbyMsg)
{
 
	u8 i,byMsgNum = 0;         
	*(pbyMsg + byMsgNum++) = M_EI_NA_1;   //���ͱ�ʶ
	*(pbyMsg + byMsgNum++) = 1;
	*(pbyMsg + byMsgNum++) = INIT;
	if(lpIEC101->TypeSeReason==2)		 //�Ƿ������ֽڵĴ���ԭ��
	   *(pbyMsg + byMsgNum ++) = lpIEC101->bySourceAdd;		
       	*(pbyMsg + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           *(pbyMsg + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
        for(i=0;i<lpIEC101->TypeInfAdd;i++)		//��Ϣ���ַ fulianqiang 2005.9.2
              *(pbyMsg + byMsgNum ++) = 0; 
	*(pbyMsg + byMsgNum++) = lpIEC101->byCOI;        
	return byMsgNum;
}
//���������Ϣ��
u8 OrgnizeReadDataMsg(u8* lpby)
{
	//int wYxTNo;
	u8 i;
	int wYcTNo;
	long  iYcVal;
	//int wDdTNo;
	int dwDdVal;
	u8 byMsgNum = 1;
#if 1       
	*(lpby + byMsgNum ++) = 1;	// �ṹ�޶���
	*(lpby + byMsgNum ++) = SPONT;	//ͻ��
	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;      //fulianqiang 2005.9.2
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
        for(i=0;i<lpIEC101->TypeInfAdd;i++)
	    *(lpby + byMsgNum ++) = lpIEC101->dwReadAd.Byte[i];
	if (lpIEC101->TypeProtocol)	//2002��101��Լ
	{
#if 0          
		if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YXSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YXEA_2002) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 1;	 //���ͱ�ʶ
			wYxTNo = lpIEC101->dwReadAd.Word[0] - 1;
			byYxVal = GetYx(wYxTNo,YX_BIT);
			if (byYxVal)
				*(lpby + byMsgNum ++) = 1;
			else
				*(lpby + byMsgNum ++) = 0;
		}
		else
#endif                  
                if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YCSA_2002) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YCEA_2002) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 21;
			wYcTNo = lpIEC101->dwReadAd.Word[0] - IEC101_YCSA_2002;
			iYcVal = GetYc(wYcTNo);
			*(lpby + byMsgNum ++) = (u8)iYcVal;
			*(lpby + byMsgNum ++) = (u8)(iYcVal >> 8);
		}
		else if( (lpIEC101->dwReadAd.Word[0] >= IEC101_DDSA_2002) && (lpIEC101->dwReadAd.Word[0] <= IEC101_DDEA_2002) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 15;
			//wDdTNo = lpIEC101->dwReadAd.Word[0] - IEC101_DDSA_2002;
			//dwDdVal = GetDd(wDdTNo);
                        dwDdVal = 0x12345678;
			*(lpby + byMsgNum ++) = (u8)dwDdVal;
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 8);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 16);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 24);
			*(lpby + byMsgNum ++) = 0;
		}
		else
		{
			byMsgNum = 0;
		}
	}
	else
	{
#if 0          
		if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YXSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YXEA) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 1;
			wYxTNo = lpIEC101->dwReadAd.Word[0] - 1;
			byYxVal = GetYx(wYxTNo,YX_BIT);
			if (byYxVal)
				*(lpby + byMsgNum ++) = 1;
			else
				*(lpby + byMsgNum ++) = 0;
		}
		else
#endif                 
                if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YCSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YCEA) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 21;
			wYcTNo = lpIEC101->dwReadAd.Word[0] - IEC101_YCSA;
			iYcVal = GetYc(wYcTNo);
			*(lpby + byMsgNum ++) = (u8)iYcVal;
			*(lpby + byMsgNum ++) = (u8)(iYcVal >> 8);
		}
		else if( (lpIEC101->dwReadAd.Word[0] >= IEC101_DDSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_DDEA) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 15;
			//wDdTNo = lpIEC101->dwReadAd.Word[0] - IEC101_DDSA;	   //fulianqiang 2005.9.2
			dwDdVal = 0x12345678;//GetDd(wDdTNo);
			*(lpby + byMsgNum ++) = (u8)dwDdVal;
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 8);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 16);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 24);
			*(lpby + byMsgNum ++) = 0;
		}
		else
		{
			byMsgNum = 0;
		}
	}
#endif        
	return byMsgNum;
}
//����վ״̬�仯��Ϣ��
u8 OrgnizeSubChgMsg(void)
{
	return 0;
}

//����һ������
void SendData1(void)
{
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 byMsgNum = 0;
        u8 firstData = lpIEC101->firstData;
        lpIEC101->firstData = nofirstdata;
        lpIEC101->Pacd = 0 ;
	switch (firstData)
	{
		case yxbw:
			byMsgNum = OrgnizeYxbwMsg(lpby);
                        if(byMsgNum)
                        {
                          lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
                          lpIEC101->PSeAppLayer.LinkFunCode = 8;
                        }
			break;
		case substinit:
			byMsgNum = OrgnizeInitEndMsg(lpby);
			lpIEC101->initstatus = initend;
			lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
			lpIEC101->PSeAppLayer.LinkFunCode = 3;
			break;
	       	case readcmd:
	     		byMsgNum = OrgnizeReadDataMsg(lpby);
	       		lpIEC101->dwReadAd.Dword = 0;	     
	       		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
	      		lpIEC101->PSeAppLayer.LinkFunCode = 8;
                        break;
		case substchange:
			byMsgNum = OrgnizeSubChgMsg();
			lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
			lpIEC101->PSeAppLayer.LinkFunCode = 8;
			break;
		case nofirstdata:
			byMsgNum = 0;
			lpIEC101->PSeAppLayer.byMsgNum = 0;
			lpIEC101->PSeAppLayer.LinkFunCode = 9;
                        if(lpIEC101->initstatus!=initend)
                        {
                          lpIEC101->PSeAppLayer.byFull = 1;
                          if(lpIEC101->UnsolTimeInterval==0)
                          {
                            byMsgNum = OrgnizeInitEndMsg(lpby);
                            lpIEC101->initstatus = initend;
                            lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
                            lpIEC101->PSeAppLayer.LinkFunCode = 3;
                          }
                          lpIEC101->Pacd = 2;
                        }
			break;
	}
  	//lpIEC101->Pacd = 2;
 	//lpIEC101->firstData = nofirstdata;        
}
//��SOE��Ϣ��
u8 OrgnizeSoeMsg(void)
{
	//int wSendNo;
	//int  dwInfoAd;
	//int wMs;
//	u8 *lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 byMsgNum = 0;
	//u8 * lpInfoNum;
#if 0        
	if( lpIEC101->PC56Time2a )
		*(lpby + byMsgNum ++) = 30;
	else
		*(lpby + byMsgNum ++) = 2;	 //���ͱ�ʶ	
	lpInfoNum = lpby + byMsgNum ++;
	*(lpby + byMsgNum ++) = 3;				
	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	
	//��SOEʵʱ��ȡSOE������֡������Ϣ�壩
	while (lpChanl[ChanlNo].ReadSoeC!=sHostSoe.bySoeWrite)
	{
		wSendNo = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].byYXSendSN;
		if(wSendNo <= lpIEC101->SendYxN)
		{
			dwInfoAd.Dword = IEC101_YXSA + wSendNo ;
			dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	        	for(i=0;i<lpIEC101->TypeInfAdd;i++)
	           		*(lpby + byMsgNum ++) = dwInfoAd.Byte[i];
			if (sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].byOnOff == 0x10)
				*(lpby + byMsgNum ++) = 0;	
			else
				*(lpby + byMsgNum ++) = 1;	
			wMs =(sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.bySecond)*1000
				+sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.wMS;
			*(lpby + byMsgNum ++) = (u8)wMs;
			*(lpby + byMsgNum ++) = (u8)(wMs >> 8);
			*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byMinute;
			if(lpIEC101->PC56Time2a)
			{
		        	*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byHour;
		 		*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byDay;
		 		*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byMonth;
		 		*(lpby + byMsgNum ++) = ( sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.wYear)-2000;
				if (++bySendSoeNum >= IEC101_SOENPF_TIME56)
					break;

		 	}
		 	else
		 	{
				if (++bySendSoeNum >= IEC101_SOENPF_TIME24)
					break;
		 	}
		}
		//�ƶ�soe��ָ��  
		if (++lpChanl[ChanlNo].ReadSoeC >= MAX_SOE_NUM)
			lpChanl[ChanlNo].ReadSoeC = 0;
	}
	if (bySendSoeNum)		
		*lpInfoNum = bySendSoeNum;	
	else
		byMsgNum = 0;			// ��SOE���������Ϊ������
#endif        
	return byMsgNum;
}
//��ң��Խ��ֵ��Ϣ��
u8 OrgnizeYcOverMsg(void)
{
	//int i;
	//int  dwYcAdd;
	//int  nTempYc;
	//int  iThreshold;	//����ֵ
	//int  iDifference;	//��ֵ
	//u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	//u8  j,byMsgNum = 0,bySendYcOverNum = 0;
	//u8* lpInfoNum;
#if 0        
	*(lpby + byMsgNum ++) = 21;			// ���ͱ�ʶ21
	lpInfoNum = lpby + byMsgNum ++;		//��Ϣ����Ŀ
	*(lpby + byMsgNum ++) = SPONT;				// ����ԭ��<3>:ͻ��
	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	if(lpIEC101->TypeProtocol)	 //�ж��Ƿ�2002���101��Լ
 	     dwYcAdd.Dword =  IEC101_YCSA_2002;
 	else 
 	     dwYcAdd.Dword =  IEC101_YCSA;
	dwYcAdd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	for (i=lpIEC101->nStartYcOver;i < lpIEC101->SendYcN;i++)
	{
		nTempYc = GetYc(i);
		iDifference=nTempYc-lpIEC101->nLastYcVal[i];	//ȡ����ң��Ĳ�ֵ
		if (iDifference<0)	//��ֵΪ����
			iDifference=-iDifference;	//����������Ϊ������
		iThreshold=(INT)((LONG)lpIEC101->nLastYcVal[i]*(LONG)lpIEC101->byRange/1000);	//��������ֵ				
		if (iThreshold<0)	//����ֵΪ����
			iThreshold=-iThreshold;	//����������Ϊ������
		if (iDifference>iThreshold)	//��ľ���ֵ�����趨������ֵ
		{
			lpIEC101->nLastYcVal[i] = nTempYc;
			dwYcAdd.Dword += i;
	        	for(j=0;j<lpIEC101->TypeInfAdd;j++)		//��Ϣ���ַ fulianqiang 2005.9.2
	          	    *(lpby + byMsgNum ++) = dwYcAdd.Byte[j];		    
			dwYcAdd.Dword -= i;
			*(lpby + byMsgNum ++) = (u8)nTempYc;	// ң��ֵ
			*(lpby + byMsgNum ++) = (u8)(nTempYc >> 8);
			if(++bySendYcOverNum >= IEC101_YCCNPF)
				break;
		}
	}
	if (i == lpIEC101->SendYcN)
		lpIEC101->nStartYcOver = 0;
	else
		lpIEC101->nStartYcOver = i;
	if (bySendYcOverNum)
		*lpInfoNum = bySendYcOverNum;
	else
		byMsgNum = 0;
#endif        
	return 0;
}
//���Ͷ�������
void SendData2(void)
{
	u8 byMsgNum = 0;
#if 1        
       	SearchFirstData();
	if (lpIEC101->firstData == nofirstdata)
	{
		lpIEC101->PSeAppLayer.LinkFunCode = 9;
		lpIEC101->PSeAppLayer.byMsgNum = 0;
		return;
	}
#if 0       
	//�¼�˳���¼����
	if (lpChanl[ChanlNo].ReadSoeC!=sHostSoe.bySoeWrite)
	{
		byMsgNum = OrgnizeSoeMsg();
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 8;
	}
#endif        
   	if (!byMsgNum)					// ��������ֵ��ң��
	{
		byMsgNum = OrgnizeYcOverMsg();
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 8;
	}      	    
	if (!byMsgNum)
	{
		lpIEC101->PSeAppLayer.byMsgNum = 0;
		//lpIEC101->PSeAppLayer.LinkFunCode = 0xE5;
                lpIEC101->PSeAppLayer.LinkFunCode = YES_ACK;
	} 
#endif        
}
//�����ٻ�ȷ����Ϣ��
u8 OrgnizeGenAck(u8 bySendReason)
{
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 i,byMsgNum = 0;        
	*(lpby + byMsgNum ++) = GeneralCall;		// 100
	*(lpby + byMsgNum ++) = 0x01;
	*(lpby + byMsgNum ++) = bySendReason;
	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

       //	*(lpby + byMsgNum ++) = lpIEC101->byCmmAdd;
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
        for(i=0;i<lpIEC101->TypeInfAdd;i++)
	   *(lpby + byMsgNum ++) = 0;
	*(lpby + byMsgNum ++) = lpIEC101->byQOI;       
	return byMsgNum;
}
//��ȫң����Ϣ��
u8 OrgnizeYxMsg(u8 *lpby,u8 bySendReason,u8 byFrameNo)
{
  u8 j,byYxVal;
  u8 byMsgNum = 0;
#if 1        
  //WORD wStartAd;
  FOUR_BYTE_TO_DWORD  dwStartAd;
  int nYxNo;
  int i;
  u8  byYxNPF;	//ÿ֡ң����
  static WORD wStartYxAd[16] = {0x001, 0x005, 0x009, 0x00d, 0x011, 0x015, 0x019, 0x01d, 
  0x021, 0x241, 0x281, 0x2C1, 0x301, 0x341, 0x381, 0x3C1};
  byYxNPF=lpIEC101->YxNPF[byFrameNo];	//ȡ��ʵ�ʵ�ÿ֡ң����,���һ֡���ܲ���64
  if (byFrameNo<lpIEC101->YxFN)	//֡��С����Ҫ���͵�ң��֡��
  {
    *(lpby + byMsgNum ++) = 1;		// 1���ͱ�ʶ
    *(lpby + byMsgNum ++) = 0x80 | byYxNPF;	  //�ɱ�ṹ�޶���
    *(lpby + byMsgNum ++) = bySendReason;
    if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
      *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
    
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
    if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
      *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
    
    dwStartAd.Word[0] = wStartYxAd[byFrameNo];
    dwStartAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
    for(j=0;j<lpIEC101->TypeInfAdd;j++)	  //��Ϣ���ַ fulianqiang 2005.9.2
      *(lpby + byMsgNum ++) = dwStartAd.Byte[j];
    nYxNo = byFrameNo * IEC101_YXNPF;
    for (i = nYxNo; i < nYxNo + byYxNPF; i++)
    {
      byYxVal = GetYx(i,YX_BIT);
      if (byYxVal)
        *(lpby + byMsgNum ++) = 1;		// ��Ʒ�������ĵ�����Ϣ 6.4.1
      else
        *(lpby + byMsgNum ++) = 0;
    }
  }
#endif        
  return byMsgNum;
}
//��ȫң����Ϣ��
u8 OrgnizeYcMsg(u8* lpby,u8 bySendReason,u8 byFrameNo)
{
	u8 byMsgNum = 0;
	int i;
	int wYcNo;
       //	WORD wStartAd;      
        union IEC101_DADD dwStartAd;
        unsigned char databuf[8];
	//unsigned long  nVal;
        float f_val;
	u8 j,byYcNPF;	//ÿ֡ң����
	static unsigned short wStartYcAd[8] = {0x701, 0x741, 0x781, 0x7C1, 0x801, 0x841, 0x881, 0x8C1};
	//static unsigned short wStartYcAd2002[8] = {0x4001, 0x4020, 0x403f, 0x404e, 0x406c, 0x407b, 0x4099, 0x40a8};
        static unsigned short wStartYcAd2002[8] = {0x4001, 0x4018, 0x402f, 0x4046, 0x405d, 0x4074, 0x408b, 0x40a2};
	byYcNPF=lpIEC101->YcNPF[byFrameNo];	//ȡ��ʵ�ʵ�ÿ֡ң����,���һ֡���ܲ���64
	if (byFrameNo<lpIEC101->YcFN)	//֡��С����Ҫ���͵�ң��֡��
	{
		*(lpby + byMsgNum ++) = 0x0d;		// 21
		*(lpby + byMsgNum ++) = 0x80 | byYcNPF;
		*(lpby + byMsgNum ++) = bySendReason;
         	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
                   *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
		*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       		if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
           	    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;

		if (lpIEC101->TypeProtocol)	//2002��101��Լ
			dwStartAd.Word[0] = wStartYcAd2002[byFrameNo];
		else
			dwStartAd.Word[0] = wStartYcAd[byFrameNo];
		dwStartAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	        for(j=0;j<lpIEC101->TypeInfAdd;j++)
	            *(lpby + byMsgNum ++) = dwStartAd.Byte[j];
		wYcNo = byFrameNo * IEC101_YCNPF;
		for (i = wYcNo; i < wYcNo + byYcNPF; i++)
		{
			//Ϊÿһ·ң�⸳ֵ
			f_val = GetYc(i);
                        switch(i%23)
                        {
                        case 1:
                        case 2:
                        case 3:
                          f_val = f_val/10000;
                          break;
                        case 0:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                        case 11:
                        case 12:
                        case 13:
                        case 14:
                        case 15:
                        case 16:
                        case 17:
                        case 18:
                        case 19:
                        case 20:
                        case 21:
                        case 22:
                          f_val = f_val/1000;
                          break;
                        case 4:
                        case 5:
                        case 6:
                          f_val = f_val/100;
                          break;
                        default:
                          break;
                        }
                        memcpy(databuf,&f_val,4);
			*(lpby + byMsgNum ++) = databuf[0];//(u8)nVal;	// ң��ֵ
			*(lpby + byMsgNum ++) = databuf[1];//(u8)(nVal >> 8);
                        *(lpby + byMsgNum ++) = databuf[2];//(u8)(nVal >> 16);
                        *(lpby + byMsgNum ++) = databuf[3];//(u8)(nVal >> 24);
                        *(lpby + byMsgNum ++) = 0;
		}
	}        
	return byMsgNum;
}
//�������ٻ�����
void SendGeneralData(void)
{ 
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 byMsgNum = 0;
  u8 bySendReason;
  u8 byFrameNo;
  if (lpIEC101->byPSGenStep == 0)
  {
    if (lpIEC101->byReason == ACT)
      bySendReason = ACTCON;
    else if (lpIEC101->byReason == DEACT)
      bySendReason = DEACTCON;
    byMsgNum = OrgnizeGenAck(bySendReason);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 3;//8;
    lpIEC101->byPSGenStep++;
  }
  else if ( (lpIEC101->byPSGenStep >= 1) && (lpIEC101->byPSGenStep <= MAX_CH_NUM) )
  {
    bySendReason = lpIEC101->byQOI;
    byFrameNo = (lpIEC101->byPSGenStep - 1) * 2;
    if (lpIEC101->frameno)
    {
      byFrameNo ++;
      lpIEC101->byPSGenStep++;
    }
    lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
    byMsgNum = OrgnizeYxMsg(lpby,bySendReason,byFrameNo);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 3;
    if (!byMsgNum)	//û��ʵ����Ϣ������
    {
      if (lpIEC101->byQOI==INTROGEN)
			//���ٻ�ʱ��Ҫ���͵�ң���ѷ���,ֱ����������ң��֡,�����ڷ����ٻ�����,�������Ϳ���Ϣ��Ӧ֡
      {
        lpIEC101->byPSGenStep = 9;	//��������ң��֡
        lpIEC101->frameno = 0;		//��һ���ڵĵ�һ֡��ʼ����
				//ִ����һ���ĳ���
        bySendReason = lpIEC101->byQOI;
	byFrameNo = (lpIEC101->byPSGenStep - 9) * 2;
        if (lpIEC101->frameno)	//��֡Ϊһ��,����1
        {
          byFrameNo ++;
          lpIEC101->byPSGenStep++;
	}
	lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
	byMsgNum = OrgnizeYcMsg(lpby,bySendReason,byFrameNo);
	lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
	lpIEC101->PSeAppLayer.LinkFunCode = 3;
        if (!byMsgNum)	//û��ʵ����Ϣ������
        {
          if (lpIEC101->byQOI==INTROGEN)	
          //���ٻ�ʱ��Ҫ���͵�ң���ѷ���,ֱ����������֡,�����ڷ����ٻ�����,�������Ϳ���Ϣ��Ӧ֡
          {
            lpIEC101->byPSGenStep = 16;	//�������ٻ�����֡
            bySendReason = ACTTERM;
            byMsgNum = OrgnizeGenAck(bySendReason);
            lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
            lpIEC101->PSeAppLayer.LinkFunCode = 3;	//���ٻ�����֡������Ϊ8
          }
          else	//�����ٻ�
		lpIEC101->PSeAppLayer.LinkFunCode = 9;	//�����������ݵ�ȷ��֡
        }
      }
      else	//�����ٻ�
        lpIEC101->PSeAppLayer.LinkFunCode = 9;	//�����������ݵ�ȷ��֡
    }
  }
  else if (lpIEC101->TypeProtocol==0)//97-101
  {
    if((lpIEC101->byPSGenStep >= 9) && (lpIEC101->byPSGenStep <= 12))
    {
      bySendReason = lpIEC101->byQOI;
      if(lpIEC101->byReason==1)
        bySendReason=lpIEC101->byReason;
      byFrameNo = (lpIEC101->byPSGenStep - 9) * 2;
      if (lpIEC101->frameno)	//��֡Ϊһ��,����1
      {
        byFrameNo ++;
        lpIEC101->byPSGenStep++;
      }
      if(lpIEC101->byPSGenStep == 13)
        lpIEC101->byPSGenStep = 15;
      lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
      byMsgNum = OrgnizeYcMsg(lpby,bySendReason,byFrameNo);
      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
      lpIEC101->PSeAppLayer.LinkFunCode = 8;
      if(lpIEC101->byReason==1)
        lpIEC101->PSeAppLayer.LinkFunCode = 4;
      if (!byMsgNum)	//û��ʵ����Ϣ������
      {
        if (lpIEC101->byQOI==INTROGEN)	
        //���ٻ�ʱ��Ҫ���͵�ң���ѷ���,ֱ����������֡,�����ڷ����ٻ�����,�������Ϳ���Ϣ��Ӧ֡
        {
          lpIEC101->byPSGenStep = 16;	//�������ٻ�����֡
          bySendReason = ACTTERM;
          byMsgNum = OrgnizeGenAck(bySendReason);
          lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
          lpIEC101->PSeAppLayer.LinkFunCode = 8;	//���ٻ�����֡������Ϊ8
        }
        else	//�����ٻ�
          lpIEC101->PSeAppLayer.LinkFunCode = 9;	//�����������ݵ�ȷ��֡
      }
    }
  }
  else if(lpIEC101->TypeProtocol!=0)//2002-101
  {
    if((lpIEC101->byPSGenStep >= 9) && (lpIEC101->byPSGenStep <= 14))
    {
      bySendReason = lpIEC101->byQOI;
      if(lpIEC101->byReason==1)
        bySendReason=lpIEC101->byReason;
      byFrameNo = (lpIEC101->byPSGenStep - 9) * 2;
      if (lpIEC101->frameno)	//��֡Ϊһ��,����1
      {
        byFrameNo ++;
        lpIEC101->byPSGenStep++;
      }
      lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
      byMsgNum = OrgnizeYcMsg(lpby,bySendReason,byFrameNo);
      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
      lpIEC101->PSeAppLayer.LinkFunCode = 3;
      if(lpIEC101->byReason==1)
        lpIEC101->PSeAppLayer.LinkFunCode = 4;
      if (!byMsgNum)	//û��ʵ����Ϣ������
      {
        if (lpIEC101->byQOI==INTROGEN)	
          //���ٻ�ʱ��Ҫ���͵�ң���ѷ���,ֱ����������֡,�����ڷ����ٻ�����,�������Ϳ���Ϣ��Ӧ֡
        {
          lpIEC101->byPSGenStep = 16;	//�������ٻ�����֡
          bySendReason = ACTTERM;
          byMsgNum = OrgnizeGenAck(bySendReason);
          lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
          lpIEC101->PSeAppLayer.LinkFunCode = 3;	//���ٻ�����֡������Ϊ8
        }
	else	//�����ٻ�
          lpIEC101->PSeAppLayer.LinkFunCode = 9;	//�����������ݵ�ȷ��֡
      }
    }
  }
  else if (lpIEC101->byPSGenStep == 15)
  {
    bySendReason = ACTTERM;
    byMsgNum = OrgnizeGenAck(bySendReason);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 8;	//���ٻ�����֡������Ϊ8
    lpIEC101->byPSGenStep++;
  }       
}
//��ʱ����Ϣ��
u8 OrgnizeTimeMsg(void)
{ 
	unsigned short wMS;
        unsigned char Buff[8];
        unsigned char* Point;
        Point = Buff;
        HT_RTC_Read(Point);
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 j,byMsgNum = 0;
	//u8 byTimeValid = 0;

	*(lpby + byMsgNum ++) = 103;	// 103
	*(lpby + byMsgNum ++) = 1;
        if(lpIEC101->byReason == ACT)
          *(lpby + byMsgNum ++) = ACTCON;
        else
          *(lpby + byMsgNum ++) = REQ;
	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       	if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
             *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;

	for(j=0;j<lpIEC101->TypeInfAdd;j++)
	    *(lpby + byMsgNum ++) = 0;

	//ȡϵͳʱ��
	wMS = *(Point+6) * 1000;
	*(lpby + byMsgNum ++) = (u8)wMS;					// ����
	*(lpby + byMsgNum ++) = (u8)(wMS >> 8);
	*(lpby + byMsgNum ++) = 0 << 7 | *(Point+5);	// IV = 0 ��Ч RES1 = 0
	*(lpby + byMsgNum ++) = *(Point+4);					// SU = 0 ��׼ʱ�� RES2 = 0
	//*(lpby + byMsgNum ++) = *(Point+3) | (*Point) << 5;
	*(lpby + byMsgNum ++) = *(Point+3);
        *(lpby + byMsgNum ++) = *(Point+2);					// RES3 = 0
	*(lpby + byMsgNum ++) = *(Point+1);
       	return byMsgNum;       
}
                         
void Calibrate_AT7022E(void)
{
  unsigned short Devads;
  ComAdjWrite(lpIEC101->PReAppLayer.lpByBuf,Devads);
}

void SendTimeAck(void)
{
  u8 byMsgNum = 0;
  byMsgNum = OrgnizeTimeMsg();
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;       
}
//���ٻ����ȷ����Ϣ��
u8 OrgnizeDdAckMsg(u8 bySendReason)
{ 
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 j,byMsgNum = 0;
	*(lpby + byMsgNum ++) = C_CI_NA_1;		// 101
	*(lpby + byMsgNum ++) = 0x81;
	*(lpby + byMsgNum ++) = bySendReason;
	if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       	if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
             *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	for(j=0;j<lpIEC101->TypeInfAdd;j++)
	    *(lpby + byMsgNum ++) = 0;

	*(lpby + byMsgNum ++) = lpIEC101->byQCC;
	return byMsgNum;      
}
//����������Ϣ��
u8 OrgnizeDdMsg(u8 bySendReason,u8 byFrameNo)
{
  int i;
  FOUR_BYTE_TO_DWORD  dwInfoAd;
  int dwDdVal;
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 j,byMsgNum = 0;
  u8 byDdNPF;	//ÿ֡�����
  int dwDdAdd;
  byDdNPF=lpIEC101->DdNPF[byFrameNo];	//ȡ��ʵ�ʵ�ÿ֡�����,���һ֡���ܲ���32
  if (byFrameNo<lpIEC101->DdFN)	//֡��С����Ҫ���͵ĵ��֡��
  {
/*    
    if(byFrameNo%2)
    {
      *(lpby + byMsgNum ++) = M_IT_TC_1;	
      *(lpby + byMsgNum ++) = 0x80|byDdNPF;
     }
     else*/
     {
      *(lpby + byMsgNum ++) = M_IT_NB_1;	
      *(lpby + byMsgNum ++) = 0x80|byDdNPF;
     }
      *(lpby + byMsgNum ++) = bySendReason;
      if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
        *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
      *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
      if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
      *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
      if(lpIEC101->TypeProtocol)
        dwDdAdd =  IEC101_DDSA_2002;
      else
        dwDdAdd =  IEC101_DDSA;
      dwInfoAd.Dword = dwDdAdd+(byFrameNo)*ONE_ENERGY_NUM;
      dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
      for(j=0;j<lpIEC101->TypeInfAdd;j++)
        *(lpby + byMsgNum ++) = dwInfoAd.Byte[j];
      for (i = byFrameNo * 8; i < byFrameNo * 8 + byDdNPF; i++)
      {
        
        dwDdVal=GetDd(i,lpby + byMsgNum);
        byMsgNum +=dwDdVal;
        *(lpby + byMsgNum ++) = 0;                        
      }
  }
  return byMsgNum;
}
//Ϊ�˼���97��2002����ٻ��������ֽ��������
//1���Ƚ�ֱ�۵Ľ���취����дһ��ƺ����ֱ���ã������ô˷���
//2����һ�ֽ��������bl2002Ϊ�Ƿ�2002����б�������ڵ�Ⱥ����������´����滻ԭ���Ĵ��룬�η������Լ��ٴ�����
		//if((��bl2000==TRUE)&&(lpIEC101->byQCC&0X3F)==5))||(��bl2000==FALSE)&&(lpIEC101->byQCC&0X3F)==1)))
		 //   bySendReason = REQCGCN;
		 //else
		 //   bySendReason = (lpIEC101->byQCC & 0x3f) - 1 + REQCOGCN;// ������lpIEC101->byPSDdStep��ֵ��ʱ��������
		 //���������
		 //if(bl2002)
		 //{
		 //  if(  lpIEC101->byQCC&0X3F)==5)
		 //  lpIEC101->byPSDdStep=0;
		 //  else
		 //  lpIEC101->byPSDdStep=lpIEC101->byQCC&0X3F;
		 //}
		 //else
		 // lpIEC101->byPSDdStep=lpIEC101->byQCC&0X3F-1; 
		 //�����ĵط�����Ҫ��   
//���͵������
void SendDdData(void)
{
        
}
//2002��ĵ���ٻ�����	  //��2005.8.30
void SendDdData2002(void)
{
        u8 byMsgNum = 0;
        u8 bySendReason;
        u8 byFrameNo;
	if (lpIEC101->byPSDdStep == 17 || lpIEC101->byPSDdStep == 0)
	{
		if (lpIEC101->byReason == ACT)
			bySendReason = ACTCON;
		else if (lpIEC101->byReason == DEACT)
			bySendReason = DEACTCON;
		byMsgNum = OrgnizeDdAckMsg(bySendReason);
		lpIEC101->byPSDdStep=0;
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 3;
	}
	else if ( (lpIEC101->byPSDdStep >= 1) && (lpIEC101->byPSDdStep <= 16) )
	{
		byFrameNo = lpIEC101->byPSDdStep - 1;
		if ( (lpIEC101->byQCC & 0x3f) == 5)
			bySendReason = REQCOGCN;		// 37��Ӧ֡�Ĵ���ԭ��
		else if ( ((lpIEC101->byQCC & 0x3f) >= 1) 
			&& ((lpIEC101->byQCC & 0x3f) <= 4) )
			bySendReason = (lpIEC101->byQCC & 0x3f) - 1 + REQCOGCN;
		byMsgNum = OrgnizeDdMsg(bySendReason,byFrameNo);
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 3;
		if ( lpIEC101->byPSDdStep==lpIEC101->DdFN)
		     lpIEC101->byPSDdStep=17;
		if (!byMsgNum)	//û��ʵ�ʵ����Ϣ������
		{
			if (bySendReason==REQCOGCN)
			//��Ӧ�������ٻ�ʱ��Ҫ���͵ĵ���ѷ���,ֱ����������֡,�����ڷ����ٻ�����,�������Ϳ���Ϣ��Ӧ֡
			{
				lpIEC101->byPSDdStep = 17;	
				bySendReason = ACTTERM;
                               // lpIEC101->byReason=ACTTERM;
				byMsgNum = OrgnizeDdAckMsg(bySendReason);
				lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
				lpIEC101->PSeAppLayer.LinkFunCode = 3;
			}
			else	//�����ٻ�
				lpIEC101->PSeAppLayer.LinkFunCode = 9;	//�����������ݵ�ȷ��֡
		}	
	}       
	else if(lpIEC101->byPSDdStep == 18)
	{
               // lpIEC101->byReason=ACTTERM;
		bySendReason = ACTTERM;
		byMsgNum = OrgnizeDdAckMsg(bySendReason);
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 3;
	}     
	lpIEC101->byPSDdStep++;
        
}

//�����������ȷ��֡
void SendReadDataAck(void)
{
  lpIEC101->PSeAppLayer.byMsgNum = 0;
  lpIEC101->PSeAppLayer.LinkFunCode = 0;

}
//��ң�ط�У��Ϣ��
u8 OrgnizeYkRetMsg(u8 bySendReason)
{

	return 0;
}
//��CAN���·�ң������
void ReceiveYktoCan(void)
{
	
}
//��ң�ط�У
void SendYkReply(void)
{
	
}
//��ͨ��������Ϣ��
u8 OrgnizeTestMsg(void)
{
  u8 *lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 j,byMsgNum = 0;
  *(lpby + byMsgNum ++) = 104;			// 104
  *(lpby + byMsgNum ++) = 0;//1;
  *(lpby + byMsgNum ++) = ACTCON;		// 7
  if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
    *(lpby + byMsgNum ++) = 0;//lpIEC101->bySourceAdd;
  *(lpby + byMsgNum ++) = 0x01;//lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
    *(lpby + byMsgNum ++) = 0x00;//lpIEC101->wCmmAdd.Byte.h;
  for(j=0;j<lpIEC101->TypeInfAdd;j++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = lpIEC101->wTester.Byte.l;
  *(lpby + byMsgNum ++) = lpIEC101->wTester.Byte.h;
  lpIEC101->wTester.Byte.l =0;
  lpIEC101->wTester.Byte.h =0;
  return byMsgNum;
}
//����ͨ������ȷ��
void SendChanTestAck(void)
{
  u8 byMsgNum = 0;
  byMsgNum = OrgnizeTestMsg();
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  lpIEC101->Pacd = 0x2;
  lpIEC101->Pdfc = 1;
}

void Dir_Send(void)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf,flag;
  u16 i,byMsgNum = 0; 
  int Record_num;
  if(lpIEC101->byPSGenStep>(MAX_CH_NUM*3-1))
  {
    return;
  }
  *(lpby + byMsgNum ++) = F_FR_NA_1;
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = REQ;
  if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x02; //
  *(lpby + byMsgNum ++) = 0x02;
  *(lpby + byMsgNum ++) = 0x00;
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);byMsgNum+=4;
  if(lpIEC101->byPSGenStep>=(MAX_CH_NUM*3-1))
  {
    *(lpby + byMsgNum ++) = 0x00;
  }
  else
  {
    *(lpby + byMsgNum ++) = 0x01;
  }
  *(lpby + byMsgNum ++) = 0x04;
  for(i=0;i<4;++i)
  {
    *(lpby + byMsgNum) = strlen((char const *)File_List[i+(lpIEC101->byPSGenStep%3)*4]);
    memcpy((lpby + byMsgNum + 1),File_List[i+(lpIEC101->byPSGenStep%3)*4],*(lpby + byMsgNum));
    *(lpby + byMsgNum +  *(lpby + byMsgNum) - 4) +=  lpIEC101->byPSGenStep/3;
    byMsgNum += (*(lpby + byMsgNum)+1);
    *(lpby + byMsgNum++) = 0;
    flag =0;
    if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],".xml"))
    {
      flag = 1;
    }
    if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],"FIXD"))
    {
        Record_num=Load_Record_Num();
        if(flag)
        {
          Record_num = Record_num*307;
          Record_num += 793;
        }
        else
        {
          Record_num = Record_num*200;
        }
    }
    else if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],"RAND"))
    {
      Record_num=Rand_Record_Num();
      if(flag)
        {
          Record_num = Record_num*307;
          Record_num += 776;
        }
        else
        {
          Record_num = Record_num*200;
        }
    }
    else if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],"FRZD"))
    {
        Record_num=Day_Record_Num();
        if(flag)
        {
          Record_num = Record_num*307;
          Record_num += 776;
        }
        else
        {
          Record_num = Record_num*200;
        }
    }
    else if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],"SHARPD"))
    {
      Record_num=Hour_Record_Num();
      if(flag)
        {
          Record_num = Record_num*307;
          Record_num += 778;
        }
        else
        {
          Record_num = Record_num*200;
        }
    }
    else if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],"MONTHD"))
    {
      Record_num=Month_Record_Num();
      if(flag)
        {
          Record_num = Record_num*307;
          Record_num += 778;
        }
        else
        {
          Record_num = Record_num*200;
        }
    }
    if(strstr(File_List[i+(lpIEC101->byPSGenStep%3)*4],"EVENTD"))
    {
      Record_num=GetPn_Event_num(0,0)+GetPn_Event_num(0,1)+GetPn_Event_num(0,2);
      Record_num = Record_num*424;
    }
    memcpy(lpby + byMsgNum,&Record_num,4);
    byMsgNum += 4;
//    *(lpby + byMsgNum++) = 0x88+i+lpIEC101->byPSGenStep*6;
//    *(lpby + byMsgNum++) = 0;
//    *(lpby + byMsgNum++) = 0;
//    *(lpby + byMsgNum++) = 0;
    *(lpby + byMsgNum++) = 0x50;
    *(lpby + byMsgNum++) = 0x46; 
    *(lpby + byMsgNum++) = 0x20;
    *(lpby + byMsgNum++) = 0x0A;
    *(lpby + byMsgNum++) = 0x1B;
    *(lpby + byMsgNum++) = 0x0A;
    *(lpby + byMsgNum++) = 0x11;
  }
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  lpIEC101->byPSGenStep++;
}

void File_Recv(void)
{
}

//�ļ��ٻ�ȷ����Ϣ��
u8 SendFileGenAck(u8 bySendReason)
{
  char* lpby = (char *)lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0;
  u32 m_filelen;
  *(lpby + byMsgNum ++) = F_FR_NA_1;		// 210
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = bySendReason;
  if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x2;    
  *(lpby + byMsgNum ++) = 0x4;
  *(lpby + byMsgNum ++) = 0x0;
  *(lpby + byMsgNum ++) = strlen(lpIEC101->Fname);
  strcpy(lpby + byMsgNum,lpIEC101->Fname);
  byMsgNum += strlen(lpIEC101->Fname);
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);
  byMsgNum += 4;
  memcpy(lpby + byMsgNum,&(m_filelen),4);
  byMsgNum += 4;
  return byMsgNum;
}

//�ļ���Ϣ��
u8 SendFileInfo(u8 bySendReason)
{
  char* lpby = (char *)lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0,pos,bySum=0,j;
  char *p_filename;
  u32 m_filelen;
  *(lpby + byMsgNum ++) = F_FR_NA_1;		// 210
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = 5;
  if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x2;    
  *(lpby + byMsgNum ++) = 0x5;
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);
  byMsgNum += 4;
  m_filelen = 0;
  memcpy(lpby + byMsgNum,&(m_filelen),4);
  byMsgNum += 4;
  *(lpby + byMsgNum ++) = 0x1;
  pos = byMsgNum;
#if 0  
  strcpy(lpby + byMsgNum,lpIEC101->Fname+14);
  byMsgNum += strlen(lpIEC101->Fname+14);
#else
  p_filename = lpIEC101->Fname;
  if(p_filename)
  {
    p_filename = (char *)strstr((const char *)p_filename,"/");
    if(p_filename)
    {
      p_filename += 1;
      p_filename = (char *)strstr(p_filename,"/");
    }
    p_filename += 1;
    strcpy(lpby + byMsgNum,p_filename);
    byMsgNum += strlen(p_filename);
    Record_no=0;
    if(strstr(lpIEC101->Fname,"FIXD"))
    {
        Record_num=Load_Record_Num();
    }
    else if(strstr(lpIEC101->Fname,"RAND"))
    {
      Record_num=Rand_Record_Num();
    }
    else if(strstr(lpIEC101->Fname,"FRZD"))
    {
        Record_num=Day_Record_Num();
    }
    else if(strstr(lpIEC101->Fname,"SHARPD"))
    {
      Record_num=Hour_Record_Num();
    }
    else if(strstr(lpIEC101->Fname,"MONTHD"))
    {
      Record_num=Month_Record_Num();
    }
    
      p_filename=strstr(lpIEC101->Fname,".");
      if(p_filename)
      {
        m_Channel_no = *(p_filename-1)-0x30 + (*(p_filename-2)-0x30)*10;
        if(m_Channel_no)
          m_Channel_no -= 1;
      }
      if(strstr(lpIEC101->Fname,"EVENTD"))
      {
        lpIEC101->pa_num = GetPn_Event_num(m_Channel_no,0);
        if(lpIEC101->pa_num>10)
          lpIEC101->pa_num = 10;
        lpIEC101->pb_num = GetPn_Event_num(m_Channel_no,1);
        if(lpIEC101->pb_num>10)
          lpIEC101->pb_num = 10;
        lpIEC101->pc_num = GetPn_Event_num(m_Channel_no,2);
        if(lpIEC101->pc_num>10)
          lpIEC101->pc_num = 10;
        lpIEC101->pt_num = GetPt_Event_num(m_Channel_no);
        if(lpIEC101->pt_num>10)
          lpIEC101->pt_num = 10;
        lpIEC101->ptt_num= JIAOSHI_Record_Num();
        if(lpIEC101->ptt_num>10)
          lpIEC101->ptt_num = 10;
        lpIEC101->pcc_num=GetClear_num(m_Channel_no);
        if(lpIEC101->pcc_num>10)
          lpIEC101->pcc_num = 10;
        lpIEC101->pce_num = CEVENT_Record_Num(m_Channel_no);
        Record_num=lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->ptt_num+lpIEC101->pcc_num+lpIEC101->pce_num;
      }
  }
#endif  
  strcpy(lpby + byMsgNum,"  v1.0\r\n");
  byMsgNum += strlen("  v1.0\r\n");
  sprintf(lpby + byMsgNum,"201710300001,%2d,2\r\n",Record_num);
  byMsgNum += strlen(lpby + byMsgNum);
  if((Record_num==0) && strstr(lpIEC101->Fname,".msg"))
  {
   *(lpby + pos - 1)=0;
   lpIEC101->byPSGenStep=254;
 }
  Blk_size=Blk_ptr=0;
  for(j=pos;j<byMsgNum;j++)
    bySum = bySum + *(lpby + j);
  *(lpby + byMsgNum ++) = bySum;
  return byMsgNum;
}


//�ļ�����
u8 SendFileData(u8 bySendReason)
{
  char * lpby = (char *)lpIEC101->PSeAppLayer.lpByBuf;
  char tmpbuf[100];
  int Len;
  u32 i,byMsgNum = 0,pos,bySum=0,j;
  u32 m_filelen;
  *(lpby + byMsgNum ++) = F_FR_NA_1;		// 210
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = 5;
  if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum) = 0x2;
  ++byMsgNum;
  *(lpby + byMsgNum) = 0x5;
  ++byMsgNum;
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);
  byMsgNum += 4;
  m_filelen = (lpIEC101->byPSGenStep-1)*256;
  memcpy(lpby + byMsgNum,&(m_filelen),4);
  byMsgNum += 4; 
  pos = byMsgNum;
  byMsgNum ++;//
  if(strstr(lpIEC101->Fname,".msg"))
  {
    if(Blk_size<=Blk_ptr)
    {
      if(Record_no<Record_num)
      {
        if(strstr(lpIEC101->Fname,"FIXD"))
        {
          Get_LoadData(Record_no,m_Channel_no,tmpbuf);
        }
        else if(strstr(lpIEC101->Fname,"RAND"))
        {
          Get_RandData(Record_no,m_Channel_no,tmpbuf);
        }
        else if(strstr(lpIEC101->Fname,"FRZD"))
        {
          Get_DayData(Record_no,m_Channel_no,tmpbuf);
        }
        else if(strstr(lpIEC101->Fname,"SHARPD"))
        {
          Get_HourData(Record_no,m_Channel_no,tmpbuf);
        }
        else if(strstr(lpIEC101->Fname,"MONTHD"))
        {
          Get_MonthData(Record_no,m_Channel_no,tmpbuf);
        }
        if(strstr(lpIEC101->Fname,"EVENTD"))
        {
          if(Record_no<lpIEC101->pa_num)
          {
            GetPn_Event_Record(m_Channel_no,0,Record_no,tmpbuf);
            Blk_size=Assamble_PnMsgFormat(1,tmpbuf,Block_buf);
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num))
          {
            GetPn_Event_Record(m_Channel_no,1,Record_no-lpIEC101->pa_num,tmpbuf);
            Blk_size=Assamble_PnMsgFormat(2,tmpbuf,Block_buf);
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num))
          {
            GetPn_Event_Record(m_Channel_no,2,Record_no-lpIEC101->pa_num-lpIEC101->pb_num,tmpbuf);
            Blk_size=Assamble_PnMsgFormat(3,tmpbuf,Block_buf);
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num)) 
          {
            GetPt_Event_Record(m_Channel_no,Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num,tmpbuf);
            Blk_size=Assamble_PtMsgFormat(tmpbuf,Block_buf);
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num))
          {
            GetClear_Record(m_Channel_no,Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num-lpIEC101->pt_num,tmpbuf);
            Blk_size=Assamble_CMsgFormat(tmpbuf,Block_buf);
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num+lpIEC101->pce_num))
          {
            Get_CEVENT(m_Channel_no,Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num-lpIEC101->pt_num-lpIEC101->pcc_num,tmpbuf);
            Blk_size=Assamble_CEVENTMsgFormat(tmpbuf,Block_buf);
          }
          else
          {
            Get_JIAOSHI(Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num-lpIEC101->pt_num-lpIEC101->pcc_num-lpIEC101->pce_num,tmpbuf);
            Blk_size=Assamble_TimeMsgFormat(tmpbuf,Block_buf);
          }
        }
        else
        {
          Blk_size=Assamble_MsgFormat(Record_no,tmpbuf,Block_buf);
        }
        Blk_ptr = 0;
//        memcpy(lpby + byMsgNum,Block_buf,Blk_size);
//        byMsgNum += Blk_size;
      }
      ++Record_no;
    }
    if((Blk_size-Blk_ptr)>245)
    {
        Len = 230;
        memcpy(lpby + byMsgNum,Block_buf+Blk_ptr,Len); 
        byMsgNum +=Len;
        Blk_ptr +=Len;
      }
      else
      {
        memcpy(lpby + byMsgNum,Block_buf+Blk_ptr,Blk_size-Blk_ptr); 
        byMsgNum +=Blk_size-Blk_ptr;
        Blk_ptr=Blk_size;
        if(Record_no>=Record_num)
        {
          lpIEC101->byPSGenStep=254;
        }
      }
   
//    
//    if(Record_no>=Record_num)
//    {
//      Record_no++;
//      lpIEC101->byPSGenStep=254;
//    }
  }
  else if(strstr(lpIEC101->Fname,".xml"))
  {
    switch(lpIEC101->byPSGenStep)
    {
    case 2:
      if(strstr(lpIEC101->Fname,"FIXD"))
      {
        byMsgNum +=Send_LoadXmlFile_Head(lpby + byMsgNum);
      }
      else if(strstr(lpIEC101->Fname,"RAND"))
      {
        byMsgNum +=Send_RandXmlFile_Head(lpby + byMsgNum);
      }
      else if(strstr(lpIEC101->Fname,"FRZD"))
      {
        byMsgNum +=Send_FrzdXmlFile_Head(lpby + byMsgNum);
      }
      else if(strstr(lpIEC101->Fname,"SHARPD"))
      {
        byMsgNum +=Send_SharpdXmlFile_Head(lpby + byMsgNum);
      }
      else if(strstr(lpIEC101->Fname,"MONTHD"))
      {
        byMsgNum +=Send_MonthdXmlFile_Head(lpby + byMsgNum);
      }
      else if(strstr(lpIEC101->Fname,"EVENTD"))
      {
        byMsgNum +=Send_EventDXmlFile_Head(lpby + byMsgNum);
        lpIEC101->byPSGenStep = 5;
      }
      //strcpy(lpby + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"FIXD\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"96\" interval=\"15min\">\r\n");
      //byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"FIXD\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"96\" interval=\"15min\">\r\n");
      break;
    case 3:
#if 0      
      strcpy(lpby + byMsgNum,"\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25602\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25603\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25604\" type=\"float\" unit=\"kVARh\" />\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25602\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25603\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25604\" type=\"float\" unit=\"kVARh\" />\r\n");
#else
    byMsgNum += Send_XmlDataType1(lpby + byMsgNum);
#endif    
      break;
    case 4:
#if 0      
      strcpy(lpby + byMsgNum,"\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25605\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25606\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25607\" type=\"float\" unit=\"kVARh\" />\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25605\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25606\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25607\" type=\"float\" unit=\"kVARh\" />\r\n");
#else
    byMsgNum += Send_XmlDataType2(lpby + byMsgNum);
#endif    
      break;
    case 5:
#if 0      
      strcpy(lpby + byMsgNum,"\t\t<DI ioa=\"16392\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16393\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16394\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16395\" type=\"float\" unit=\"W\" />\r\n \
  \t</DataAttr>\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"16392\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16393\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16394\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16395\" type=\"float\" unit=\"W\" />\r\n \
  \t</DataAttr>\r\n");
#else
    byMsgNum += Send_XmlDataType3(lpby + byMsgNum);
#endif    
    //Record_no=0;
    //Record_num=10;
    break;
    default:
#if 0      
      if(lpIEC101->byPSGenStep%2==0)
      {
      strcpy(lpby + byMsgNum,"\t<DataRec sect=\"1\" tm=\"20171025_113000\">\r\n \
      \t\t<DI val=\"10.718\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n \
      \t\t<DI val=\"0.0\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n");
      byMsgNum += strlen("\t<DataRec sect=\"1\" tm=\"20171025_113000\">\r\n \
      \t\t<DI val=\"10.718\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n \
      \t\t<DI val=\"0.0\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n");
      }
      else
      {
        strcpy(lpby + byMsgNum,"\t\t<DI val=\"10.718\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n \
        \t<DI val=\"0.0\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n\t</DataRec>\r\n");
        byMsgNum += strlen("\t\t<DI val=\"10.718\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n \
        \t<DI val=\"0.0\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n\t</DataRec>\r\n");
        if(lpIEC101->byPSGenStep>=9)
        {
          strcpy(lpby + byMsgNum,"</DataFile>\r\n");
          byMsgNum += strlen("</DataFile>\r\n");
        }
      }
#endif
      if(Blk_size<=Blk_ptr)
      {
        if(Record_no<Record_num)
        {
          //Get_LoadData(Record_no,m_Channel_no,tmpbuf);
          if(strstr(lpIEC101->Fname,"FIXD"))
      {
        Get_LoadData(Record_no,m_Channel_no,tmpbuf);
      }
      else if(strstr(lpIEC101->Fname,"RAND"))
      {
        Get_RandData(Record_no,m_Channel_no,tmpbuf);
      }
      else if(strstr(lpIEC101->Fname,"FRZD"))
      {
        Get_DayData(Record_no,m_Channel_no,tmpbuf);
      }
      else if(strstr(lpIEC101->Fname,"SHARPD"))
      {
        Get_HourData(Record_no,m_Channel_no,tmpbuf);
      }
      else if(strstr(lpIEC101->Fname,"MONTHD"))
      {
        Get_MonthData(Record_no,m_Channel_no,tmpbuf);
      }
      else if(strstr(lpIEC101->Fname,"EVENTD"))
      {
          if(Record_no<lpIEC101->pa_num)
          {
            GetPn_Event_Record(m_Channel_no,0,Record_no,tmpbuf);
            if(Record_no==0)
            {
              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->pa_num);
              Blk_size = Assamble_PnXmlFormat(m_Channel_no,0,tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
              Blk_size=Assamble_PnXmlFormat(m_Channel_no,0,tmpbuf,Block_buf);
            if((Record_no>=(lpIEC101->pa_num-1)))
            {
              //Record_no = GetPn_Event_num(m_Channel_no,0)-1;
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num))
          {
            GetPn_Event_Record(m_Channel_no,1,Record_no-lpIEC101->pa_num,tmpbuf);
            if(Record_no==lpIEC101->pa_num)
            { 
              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->pb_num);
              Blk_size = Assamble_PnXmlFormat(m_Channel_no,1,tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
            {
              Blk_size=Assamble_PnXmlFormat(m_Channel_no,1,tmpbuf,Block_buf);
            }
            if(Record_no>=(lpIEC101->pa_num+lpIEC101->pb_num-1))
            {
              //Record_no = GetPn_Event_num(m_Channel_no,0)+GetPn_Event_num(m_Channel_no,1)-1;
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num))
          {
            GetPn_Event_Record(m_Channel_no,2,Record_no-lpIEC101->pa_num-lpIEC101->pb_num,tmpbuf);
            if(Record_no==(lpIEC101->pa_num+lpIEC101->pb_num))
            { 

              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->pc_num);
              Blk_size = Assamble_PnXmlFormat(m_Channel_no,2,tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
            {
              Blk_size=Assamble_PnXmlFormat(m_Channel_no,2,tmpbuf,Block_buf);
            }
            if(Record_no>=(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num-1))
            {
              //Record_no = GetPn_Event_num(m_Channel_no,0)+GetPn_Event_num(m_Channel_no,1)+GetPn_Event_num(m_Channel_no,2)-1;
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num))
          {
            GetPt_Event_Record(m_Channel_no,Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num,tmpbuf);
            
            if(Record_no==(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num))
            { 

              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->pt_num);
              Blk_size = Assamble_PtXmlFormat(tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
            {
              Blk_size=Assamble_PtXmlFormat(tmpbuf,Block_buf);
            }
            if(Record_no>=(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num-1))
            {
              //Record_no = GetPn_Event_num(m_Channel_no,0)+GetPn_Event_num(m_Channel_no,1)+GetPn_Event_num(m_Channel_no,2)-1;
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num))
          {
            GetClear_Record(m_Channel_no,Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num-lpIEC101->pt_num,tmpbuf);
            if(Record_no==(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num))
            { 

              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->pcc_num);
              Blk_size = Assamble_CXmlFormat(tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
            {
              Blk_size=Assamble_CXmlFormat(tmpbuf,Block_buf);
            }
            if(Record_no>=(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num-1))
            {
              //Record_no = GetPn_Event_num(m_Channel_no,0)+GetPn_Event_num(m_Channel_no,1)+GetPn_Event_num(m_Channel_no,2)-1;
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }
          }
          else if(Record_no<(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num+lpIEC101->pce_num))
          {
            Get_CEVENT(m_Channel_no,Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num-lpIEC101->pt_num-lpIEC101->pcc_num,tmpbuf);
            if(Record_no==(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num))
            {
//              if(JIAOSHI_Record_Num()>10)
//                i=10;
//              else
//                i=JIAOSHI_Record_Num();
              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->pce_num);
              Blk_size = Assamble_CEVENTXmlFormat(tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
            {
              Blk_size=Assamble_CEVENTXmlFormat(tmpbuf,Block_buf);
            }
            if(Record_no>=(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pce_num+lpIEC101->pcc_num-1))
            {
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }
          }
          else
          {
            Get_JIAOSHI(Record_no-lpIEC101->pa_num-lpIEC101->pb_num-lpIEC101->pc_num-lpIEC101->pt_num-lpIEC101->pcc_num-lpIEC101->pce_num,tmpbuf);
            if(Record_no==(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->pcc_num+lpIEC101->pce_num))
            {
//              if(JIAOSHI_Record_Num()>10)
//                i=10;
//              else
//                i=JIAOSHI_Record_Num();
              sprintf(Block_buf,"<DataRec num=\"%02d\">\r\n",lpIEC101->ptt_num);
              Blk_size = Assamble_TimeXmlFormat(tmpbuf,Block_buf+20);
              Blk_size += 20;
            }
            else
            {
              Blk_size=Assamble_TimeXmlFormat(tmpbuf,Block_buf);
            }
            if(Record_no>=(lpIEC101->pa_num+lpIEC101->pb_num+lpIEC101->pc_num+lpIEC101->pt_num+lpIEC101->ptt_num+lpIEC101->pcc_num+lpIEC101->pce_num-1))
            {
              //Record_no = GetPn_Event_num(m_Channel_no,0)+GetPn_Event_num(m_Channel_no,1)+GetPn_Event_num(m_Channel_no,2)+JIAOSHI_Record_Num()-1;
              strcpy(Block_buf + Blk_size,"</DataRec>\r\n");
              Blk_size += strlen("</DataRec>\r\n");
            }   
          }
      }
          Record_no++;
          if(strstr(lpIEC101->Fname,"EVENTD")==0)
            Blk_size=Assamble_XmlFormat(Record_no,tmpbuf,Block_buf);
          Blk_ptr=0;
        }
        if(Record_no>=Record_num)
        {
          strcpy(Block_buf + Blk_size,"</DataFile>\r\n");
          Blk_size += strlen("</DataFile>\r\n");
        }
      }
      if((Blk_size-Blk_ptr)>200)
      {
        for(Len=200;Len<220;++Len)
        {
          if((Block_buf[Blk_ptr+Len-2]=='\r') &&  (Block_buf[Blk_ptr+Len-1]=='\n'))
            break;
        }
        memcpy(lpby + byMsgNum,Block_buf+Blk_ptr,Len); 
        byMsgNum +=Len;
        Blk_ptr +=Len;
      }
      else
      {
        memcpy(lpby + byMsgNum,Block_buf+Blk_ptr,Blk_size-Blk_ptr); 
        byMsgNum +=Blk_size-Blk_ptr;
        Blk_ptr=Blk_size;
        if(Record_no>=Record_num)
        {
          lpIEC101->byPSGenStep=254;
        }
      }
      break;
    }
  }
  if((Record_no<Record_num) || (Blk_ptr<Blk_size))
    *(lpby + pos) = 0x1;
  else
    *(lpby + pos) = 0x0;
  pos++;
   for(j=pos;j<byMsgNum;j++)
    bySum = bySum + *(lpby + j);
  *(lpby + byMsgNum ++) = bySum;
  return byMsgNum;
}

void File_Send(void)
{
  u8 bySendReason;
  u8 byMsgNum=0;
  if(lpIEC101->byPSGenStep)
  {
    if(lpIEC101->byPSGenStep==1)
    {
      byMsgNum=SendFileInfo(1);
//      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
//      lpIEC101->PSeAppLayer.LinkFunCode = 3;
    }
    else if(lpIEC101->byPSGenStep<254)
    {
      byMsgNum=SendFileData(1); 
    }
  }
  else
  {
    if (lpIEC101->byReason == ACT)
      bySendReason = ACTCON;
    else if (lpIEC101->byReason == DEACT)
      bySendReason = DEACTCON;
    byMsgNum = SendFileGenAck(bySendReason);
//    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
//    lpIEC101->PSeAppLayer.LinkFunCode = 3;
  }
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  ++lpIEC101->byPSGenStep;
}

void File_Server(void)
{
  switch(lpIEC101->Fop)
  {
  case F_FR_DA_1:
    Dir_Send();
    break;
  case F_FR_FA_1:
    File_Send();
    break;
  case F_FW_FA_1:
      File_Recv();
    break;
  default:
    break;
  }
}
void Sn_Read(void)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0,j,flag=1;        
  *(lpby + byMsgNum ++) = C_RS_NA_1;
  if(lpIEC101->byQualify==0)
    *(lpby + byMsgNum ++) = 6 * MAX_CH_NUM;
  else
    *(lpby + byMsgNum ++) = lpIEC101->byQualify;
  *(lpby + byMsgNum ++) = ACTCON;
  if (lpIEC101->TypeSeReason==2)			//����ԭ�������ֽ�
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //�Ƿ������ֽڵ�Ӧ�÷���Ԫ��ַ
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  *(lpby + byMsgNum ++) = lpIEC101->Sn&0xff; 
  *(lpby + byMsgNum ++) = (lpIEC101->Sn>>8)&0xff; 
  *(lpby + byMsgNum ++) = 0x00;
 
  for(i=lpIEC101->byPSGenStep*20;i<lpIEC101->byQualify;++i)
  {
    *(lpby + byMsgNum ++) = lpIEC101->List_para[i]&0xff; 
    *(lpby + byMsgNum ++) = (lpIEC101->List_para[i]>>8)&0xff; 
    *(lpby + byMsgNum ++) = 4;
    for(j=0;j<9;++j)
    {
      if(lpIEC101->List_para[i]==s_sninfo[j].info_addr)
        break;
    }
    if(j<9)
    {
      *(lpby + byMsgNum ++) = s_sninfo[j].data_len;
      memcpy(lpby + byMsgNum,s_sninfo[j].info_data,s_sninfo[j].data_len);
      byMsgNum += s_sninfo[j].data_len;
    }
    else
    {
      *(lpby + byMsgNum ++) = 0;
    }
    if((i-lpIEC101->byPSGenStep*20)>=19)
    {
      flag = 0;
      break;
    }
  }
  if(lpIEC101->byQualify==0)
  {
    float *p_F;
    p_F = &m_ecpara[0].cmon_day;
    for(i=lpIEC101->byPSGenStep*20;i<6 * MAX_CH_NUM;++i)
    {
      *(lpby + byMsgNum ++) = (0x8021+i)&0xff; 
      *(lpby + byMsgNum ++) = ((0x8021+i)>>8)&0xff; 
      *(lpby + byMsgNum ++) = 0x26;
      *(lpby + byMsgNum ++) = 4;
      memcpy(lpby + byMsgNum,p_F+i,4);
      byMsgNum +=4;
      if((i-lpIEC101->byPSGenStep*20)>=19)
      {
        flag = 0;
        break;
      }
    }
  }
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  lpIEC101->byPSGenStep++;
  if(flag)
    lpIEC101->PReMsgType = 0;
}
//����Ϣ֡
void AppVFrame(void)
{
  switch (lpIEC101->PReMsgType)
  {
  case GeneralCall:
      SendGeneralData();
      lpIEC101->PSeAppLayer.byFull = 1;
      if ( (lpIEC101->byReason == ACT) && (lpIEC101->byPSGenStep == 16) )
        lpIEC101->PReMsgType = 0;
      else if( (lpIEC101->byReason == REQ) && (!lpIEC101->frameno) )
        lpIEC101->PReMsgType = 0;
      else if(lpIEC101->byReason == DEACT)
        lpIEC101->PReMsgType = 0;
      if(lpIEC101->PReMsgType==0)
      lpIEC101->byPSGenStep=0;
      break;
    case C_CS_NA_1:				//Уʱȷ��
      SendTimeAck();
      lpIEC101->PSeAppLayer.byFull = 1;
      lpIEC101->PReMsgType = 0;
      break;
    case C_CI_NA_1:				//�ٻ����
      if ( lpIEC101->TypeProtocol)//2002���101��Լ//��2005.8.30
        SendDdData2002();
      else
        SendDdData();
      lpIEC101->PSeAppLayer.byFull = 1;
    
      if( (lpIEC101->byReason == ACT) && (lpIEC101->byPSDdStep == 17) )
        lpIEC101->PReMsgType = 0;
      else if(lpIEC101->byReason == REQ)
        lpIEC101->PReMsgType = 0;
    else if((lpIEC101->byReason == DEACT))
      lpIEC101->PReMsgType = 0;
    else if(lpIEC101->byPSDdStep > 18)
      lpIEC101->PReMsgType = 0;
     if(lpIEC101->PReMsgType==0)
      lpIEC101->byPSDdStep=0;
    break;
   case C_RD_NA_1:				//������ȷ��
     SendReadDataAck();
     lpIEC101->PSeAppLayer.byFull = 1;
     lpIEC101->PReMsgType = 0;
     break;
    case C_SC_NA_1:				//����ң��
    case C_DC_NA_1:				//ң��
    case C_RC_NA_1:				//��������
      SendYkReply();
      if( (lpIEC101->WaitYkXzRet == 0) && (lpIEC101->WaitYkZxRet == 0) )
      {
        lpIEC101->PSeAppLayer.byFull = 1;
        lpIEC101->PReMsgType = 0;
      }
      if (lpIEC101->SendYkZxAck == 1)
      {
        lpIEC101->PSeAppLayer.byFull = 1;
        lpIEC101->SendYkZxAck = 0;
      }
      break;
  case  C_TS_NA_1:
    SendChanTestAck();
    lpIEC101->PReMsgType = 0;
    lpIEC101->PWinTimer=0;
    lpIEC101->PSeAppLayer.byFull = 1;
    break;
  case C_RS_NA_1:
    Sn_Read();
    if(lpIEC101->PSeAppLayer.byMsgNum)
      lpIEC101->PSeAppLayer.byFull = 1;
    else
    {
      lpIEC101->PReMsgType = 0;
    }
    //lpIEC101->PReMsgType = 0;
    break;
  case C_WS_NA_1:
    //Calibrate_AT7022E();
    break;
  case F_FR_NA_1: //�ļ�����
    File_Server();
    if(lpIEC101->PSeAppLayer.byMsgNum)
      lpIEC101->PSeAppLayer.byFull = 1;
    else
    {
      if(lpIEC101->FlagPingH==0)
      {
        SendData2();
        lpIEC101->PSeAppLayer.byFull = 1;
        lpIEC101->PReFrameType = 0xff;
      }
    }
    if(lpIEC101->FlagPingH==0)
      lpIEC101->PSeAppLayer.byFull = 1;
    break;
  default:
    if(lpIEC101->FlagPingH)
    {
      lpIEC101->PSeAppLayer.byFull = 0;
      lpIEC101->PReMsgType = 0x0;
    }
    else
    {
      SendData2();
      lpIEC101->PSeAppLayer.byFull = 1;
      lpIEC101->PReFrameType = 0xff;
    }
    break;
  }
}
//�Ӷ�Ӧ�ò㷢�ʹ�����
void PAppSendProcess(void)
{
  if (lpIEC101->PReFrameType == CALL_DATA1)
  {
    SendData1();
    lpIEC101->PSeAppLayer.byFull = 1;
    lpIEC101->PReFrameType = 0xff;
  }
  else  
  {
    if(3==lpIEC101->UnsolTimeInterval)
    {
     // if(lpIEC101->PReFrameType)// == CALL_DATA2)
      {
        //if( 0==lpIEC101->PReFrameType)
        if((lpIEC101->PReFrameType == CALL_DATA2 && lpIEC101->FlagPingH) || (lpIEC101->FlagPingH && (0==lpIEC101->PReFrameType)))
        {
          SendData2();
          lpIEC101->PSeAppLayer.byFull = 1;
          lpIEC101->PReFrameType = 0xff;
        }
        else
        {
          //if (((lpIEC101->PWinTimer >= 200)&& (lpIEC101->PReFrameType==0x3)))
          //if((lpIEC101->OrgnizeFrame) && (lpIEC101->PWinTimer >= 100))
          if(lpIEC101->OrgnizeFrame)
          { 
           // lpIEC101->PWinTimer =0 ;
            lpIEC101->PReFrameType=0x3;
            AppVFrame();
            if(lpIEC101->PSeAppLayer.byFull==0 && lpIEC101->PReMsgType)
            {
              lpIEC101->PReMsgType = 0;
            }
            lpIEC101->PReFrameType = 0xff;
          }
        }
      }
    }
    else
    {
    // if(lpIEC101->PReFrameType == CALL_DATA2 && (lpIEC101->byPSGenStep==0 && lpIEC101->byPSDdStep==0))
      if(lpIEC101->PReFrameType == CALL_DATA2 && lpIEC101->PReMsgType==0)
     //if(lpIEC101->PReMsgType==0)
      {
        SendData2();
	lpIEC101->PSeAppLayer.byFull = 1;
	lpIEC101->PReFrameType = 0xff;
       }      
        else
        {
          if(lpIEC101->OrgnizeFrame)
          {  
            AppVFrame();
            if(lpIEC101->PSeAppLayer.byFull==0 && lpIEC101->PReMsgType)
            {
              SendData2();
              lpIEC101->PSeAppLayer.byFull = 1;
              lpIEC101->PReMsgType = 0;
            }
          }
          lpIEC101->PReFrameType = 0xff;
        }     
    }
  }

}
//Ӧ�ò㷢�ʹ�����
void Iec101AppSendPro(void)
{
	if ( (lpIEC101->PReFrameType!=0xff || lpIEC101->PReMsgType)
		&& (lpIEC101->PSendFrame.byFull == 0)&&(lpIEC101->PSeAppLayer.byFull == 0))
	{
		PAppSendProcess();
	}
}
//��䳤��·֡
u16 OrgnizeVarFrame(u8 *lpby,u8 byMsgNum,u8 byConField)	//fulianqiang 2005.9.2
{
	int i;
	u8 bySum = 0;
        if(byMsgNum>248)
          byMsgNum = 248;
	//u8 byMsgNum = lpIEC101->PSeAppLayer.byMsgNum;
	*(lpby + 0) = V_STARTCODE;	// �䳤֡��ʼ�ֽ�
	*(lpby + 1) = byMsgNum + 1 + lpIEC101->TypeLinkAdd;
	*(lpby + 2) = byMsgNum + 1 + lpIEC101->TypeLinkAdd;
	*(lpby + 3) = V_STARTCODE;	// �䳤֡��ʼ�ֽ�
	*(lpby + 4) = byConField;
	*(lpby + 5) = lpIEC101->wLinkAdd.Byte.l;
	if( lpIEC101->TypeLinkAdd ==2)
	   *(lpby + 6) = lpIEC101->wLinkAdd.Byte.h;
	for (i = 4; i < byMsgNum+5+lpIEC101->TypeLinkAdd ; i++)
		bySum = bySum + *(lpby + i);
	*(lpby + byMsgNum + 5 + lpIEC101->TypeLinkAdd) = bySum;
	*(lpby + byMsgNum + 6 + lpIEC101->TypeLinkAdd) = STOPCODE;	// ֹͣ�ֽ�
	return byMsgNum + 7 + lpIEC101->TypeLinkAdd;
}
//�鶨����·֡
u8 OrgnizeFixFrame(u8 *lpby,u8 byConField)
{
  u8 j,bySum = 0,i=0;
  *(lpby + i++) = F_STARTCODE;	// ����֡��ʼ�ֽ�
  *(lpby + i++) = byConField;
  *(lpby + i++) = lpIEC101->wLinkAdd.Byte.l;
  if( lpIEC101->TypeLinkAdd ==2)
    *(lpby + i++) = lpIEC101->wLinkAdd.Byte.h;
  for(j=1;j<i;j++)
    bySum = bySum + *(lpby + j);
  *(lpby + i++) = bySum;
  *(lpby + i++) = STOPCODE;	// ֹͣ�ֽ�
  return i;
}
//�Ӷ���·�㷢�ʹ�����
void PLinkSendProcess(void)
{
	u8 byConField;
	lpIEC101->PSendFrame.byFunCode = lpIEC101->PSeAppLayer.LinkFunCode;
        
       if(lpIEC101->FlagPingH)       
         byConField = 0x80 | (lpIEC101->Pacd << 5) /*| (lpIEC101->Pdfc << 4)*/
					| lpIEC101->PSendFrame.byFunCode;
       else
          byConField = (lpIEC101->Pacd << 5) | (lpIEC101->Pdfc << 4)
					| lpIEC101->PSendFrame.byFunCode;
	if (lpIEC101->PSeAppLayer.byMsgNum)
	{
		lpIEC101->PSendFrame.wFrameLen = 
                OrgnizeVarFrame(lpIEC101->PSendFrame.byLinkBuf,lpIEC101->PSeAppLayer.byMsgNum,byConField);
                lpIEC101->PSeAppLayer.byMsgNum=0;  //zzl add
	}
	else
	{
		if (lpIEC101->PSendFrame.byFunCode == 0xE5)
		{
			lpIEC101->PSendFrame.byLinkBuf[0] = 0xE5;
			lpIEC101->PSendFrame.wFrameLen = 1;
		}
		else
		{
			lpIEC101->PSendFrame.wFrameLen = 
                        OrgnizeFixFrame(lpIEC101->PSendFrame.byLinkBuf,byConField);
		}
	}
	lpIEC101->PSendFrame.byFull = 1;
}
//��·�㷢�ʹ�����
void Iec101LinkSendPro(void)
{
	if (lpIEC101->PSeAppLayer.byFull == 1)
	{
		PLinkSendProcess();
		lpIEC101->PSeAppLayer.byFull = 0;
	}
}
//��·�㷢�ͺ���
void Iec101LinkSend(void)
{
  if(lpIEC101->byFrameIntval)
    return;
  if (lpIEC101->PSendFrame.byFull == 1)
  {
    Serial_Write(IEC101_PORT,lpIEC101->PSendFrame.byLinkBuf,lpIEC101->PSendFrame.wFrameLen);
  //if (lpIEC101->wPSendNum >= lpIEC101->PSendFrame.wFrameLen)
    {
      lpIEC101->byFrameIntval = (lpIEC101->PSendFrame.wFrameLen);
      lpIEC101->PSendFrame.byFull = 0;
      lpIEC101->wPSendNum = 0;
#if 0
      if ((lpIEC101->PReFrameType == 0) && (lpIEC101->PReMsgType == 0) )
      {
        lpIEC101->PWindow = 0;
        lpIEC101->PWinTimer = 0;
        lpIEC101->OrgnizeFrame = 0 ;
      }
#else
      if((lpIEC101->PReFrameType == 0xff))
      {
        lpIEC101->OrgnizeFrame = 0 ;
        lpIEC101->PWindow = 0;
      }
      lpIEC101->PWinTimer = 0;
#endif            
          }
	}
}
void OrgnizeIec101Frame(void)
{
	//Ӧ�ò㷢�ʹ�����
	Iec101AppSendPro();
	SearchFirstData();  //����һ�����ݷ�����Ӧ��֡������·֡ǰfulianqiang 2005.10.14
	//��·�㷢�ʹ�����
	Iec101LinkSendPro();
	//��·�㷢�ͺ���
   //	Iec101LinkSend();
}
//��Լ����ʱ����⺯��
void Iec101WatchTime(void)
{
  if(lpIEC101==0)
    return;
  if(lpIEC101->byFrameIntval)
      lpIEC101->byFrameIntval--;
  if (lpIEC101->PWindow == 1)
  {
    //lpIEC101->byFrameIntval++;
    
    lpIEC101->PWinTimer++;
  }
/*  
  if(3==lpIEC101->UnsolTimeInterval)
  {
    if(lpIEC101->OrgnizeFrame && (lpIEC101->PWinTimer>100))
    {
      if(lpIEC101->PReFrameType!=0xff && lpIEC101->PReFrameType!=0)
      {  
        lpIEC101->PWinTimer = 0;
        lpIEC101->PReFrameType = 0x3;
        lpIEC101->PSendFrame.byFull = 1;
      }
    }
  }*/
  if (lpIEC101->PWinTimer >= 60000) //��Լ60����ʱ��,���뱣֤�Ͳ�����ʱ,���ٻ���Ϣ�ܹ��������
  {
    lpIEC101->PWindow = 0;
    lpIEC101->PWinTimer = 0;
    lpIEC101->PReFrameType = 0xff;
    lpIEC101->PReMsgType = 0;
    lpIEC101->PSendFrame.byFull = 0;
    lpIEC101->wPSendNum = 0;
  }
}
//101��Լ֡������
void IEC101Process(void)
{
      if(lpIEC101->byFrameIntval)
        return;
	//��֡����
	SleaveIec101Frame();
        
	//���ӴӶ�����
	WatchPWindow();

	//��֡����
	OrgnizeIec101Frame();
	Iec101LinkSend();//��·�㷢��
#if 1
	//��Լ��ʱ������
	//Iec101WatchTime();
#endif        

}
//��ʼ����Լ˽�����ݺ���
void InitIEC101Func(u8 ChanNo)
{
    
#if 0 
	WORD i,n;
	lpIEC101=&(lpChanl[ChanNo].ChanlPrivM.IEC101ProtPrivM);	//��ʼ����ͨ����Լ˽������ָ��
	lpIEC101->byCOI = 0;				// ��ʼ��ԭ��:���ص�Դ���غ���
	lpIEC101->FtuSccReset = FALSE;//ͨ����λ��־
	//��ʼ�� SOE �Ķ�ָ��
	lpChanl[ChanNo].ReadSoeC = sHostSoe.bySoeWrite;
	//���SOE���е�ң�ű�λ������
	lpIEC101->YxChangeC = sHostSoe.bySoeWrite;	
	lpIEC101->PWindow = 0;
	lpIEC101->PWinTimer = 0;
	lpIEC101->wRecvNum = 0;
	lpIEC101->wRecvLen = BUFLENGTH;
	lpIEC101->wPSendNum = 0;
	lpIEC101->byFrameIntval = 0;
	lpIEC101->firstData = nofirstdata;
	lpIEC101->TypeProtocol = pChannelPara[ChanNo].wChanPara2;
	if(lpIEC101->TypeProtocol == 0)	   //���ѡ��97���101������Ĳ�����Чfulianqiang 2005.9.2
	{
	   lpIEC101->TypeLinkAdd = 1;
	   lpIEC101->TypeSeReason = 1;
	   lpIEC101->TypeCmmAdd = 1;
	   lpIEC101->TypeInfAdd = 2;
	}
	else				   //2002���101��Ҫ��Ĳ���
	{
		if(pChannelPara[ChanNo].wChanPara3==2)
	   	   lpIEC101->TypeLinkAdd = pChannelPara[ChanNo].wChanPara3; //��"ͨ������3"����վ��ַ����fu 2005.9.1
		else
		   lpIEC101->TypeLinkAdd = 1;
		if( pChannelPara[ChanNo].wChanPara4 ==2)
		   lpIEC101->TypeSeReason = pChannelPara[ChanNo].wChanPara4; //��"ͨ������4"���ô���ԭ������fu 2005.9.1
		else
		   lpIEC101->TypeSeReason = 1;
		if(pChannelPara[ChanNo].wChanPara5==2)
		   lpIEC101->TypeCmmAdd = pChannelPara[ChanNo].wChanPara5; //��"ͨ������5"����Ӧ�÷���Ԫ��ַ����fu 2005.9.1
		else
		   lpIEC101->TypeCmmAdd = 1;
		if( pChannelPara[ChanNo].wChanPara6==3 ||  pChannelPara[ChanNo].wChanPara6 == 1)  //�������ֽڽ����ڽṹ����Ϣ�����ַ������������²�ͬ��Ϣ�����ַ�������ĿΪ65536����������λλ�飩
		   lpIEC101->TypeInfAdd = pChannelPara[ChanNo].wChanPara6; //��"ͨ������6"������Ϣ���ַ����fu 2005.9.1
		else
		   lpIEC101->TypeInfAdd = 2;
	}
	lpIEC101->wLinkAdd.Word =pChannelPara[ChanNo].wChanPara0;	//��"ͨ������0"����վ��ַ
	lpIEC101->wCmmAdd.Word = lpIEC101->wLinkAdd.Word;

	if(pChannelPara[ChanNo].wChanPara7 > 0)
	       lpIEC101->PC56Time2a = TRUE;
	else
	       lpIEC101->PC56Time2a = FALSE;

	lpIEC101->initstatus = notinit;
	lpIEC101->haveset = FALSE;
	lpIEC101->PfcbC = 0;
	lpIEC101->OrgnizeFrame = 0 ;
	lpIEC101->Pfcv = 0;
	lpIEC101->Pacd = 0;
	lpIEC101->Pdfc = 0;
	lpIEC101->PRecvFrame.byFull = 0;
	lpIEC101->PReAppLayer.byFull = 0;
	lpIEC101->PSeAppLayer.byFull = 0;
	lpIEC101->PSendFrame.byFull = 0;
	lpIEC101->PSeAppLayer.lpByBuf = lpIEC101->PSendFrame.byLinkBuf + 5 + lpIEC101->TypeLinkAdd;
	lpIEC101->PReFrameType = 0;
	lpIEC101->PReMsgType = 0;
	lpIEC101->dwReadAd.Dword = 0;	//fulianqiang 2005.9.2
	lpIEC101->WaitYkXzRet = 0;
	lpIEC101->WaitYkZxRet = 0;
	lpIEC101->SendYkZxAck = 0;
	lpIEC101->byYKWaitTimes=0;	//ң�ط�У�ȴ���������
	lpIEC101->nStartYcOver = 0;
	for (i=0;i<YCSendTabSize;i++)
	{
		lpIEC101->nLastYcVal[i]=GetYc(i);
	}
   	if (pChannelPara[ChanNo].wChanPara1)	//ң��Խ��ǧ�ֱ�
	{
		if (pChannelPara[ChanNo].wChanPara1 < 1000)
			lpIEC101->byRange = (u8)pChannelPara[ChanNo].wChanPara1;
		else
			lpIEC101->byRange = 1;
	}
	else
	{ 
		lpIEC101->byRange = 1;
	} 
	//ʵ�ʷ�������ʼ��
	if (pChannelPara[ChanNo].wSendYCNum<=YCSendTabSize)	//�жϲ�����Ч��
		lpIEC101->SendYcN=pChannelPara[ChanNo].wSendYCNum;//��ʼ������ң����
	else
		lpIEC101->SendYcN=YCSendTabSize;
	if (pChannelPara[ChanNo].wSendYXNum<=YXSendTabSize)
		lpIEC101->SendYxN=pChannelPara[ChanNo].wSendYXNum;//��ʼ������ң����
	else
		lpIEC101->SendYxN=YXSendTabSize;
	if (pChannelPara[ChanNo].wSendYMNum<=YMSendTabSize)
		lpIEC101->SendDdN=pChannelPara[ChanNo].wSendYMNum;//��ʼ�����͵����
	else
		lpIEC101->SendDdN=YMSendTabSize;
	lpIEC101->SendYkN=IEC101_YKNUM;
	//��ʼ����Ҫ���͵�ң��֡��,����һ֡�İ�һ֡����
	lpIEC101->YcFN=(u8)((lpIEC101->SendYcN+IEC101_YCNPF-1)/IEC101_YCNPF);
	//��ʼ����Ҫ���͵�ң��֡��,����һ֡�İ�һ֡����
	lpIEC101->YxFN=(u8)((lpIEC101->SendYxN+IEC101_YXNPF-1)/IEC101_YXNPF);
	//��ʼ����Ҫ���͵ĵ��֡��,����һ֡�İ�һ֡����
	lpIEC101->DdFN=(u8)((lpIEC101->SendDdN+IEC101_DDNPF-1)/IEC101_DDNPF);
	for(i=0;i<IEC101_YCNUM/IEC101_YCNPF;i++)
		lpIEC101->YcNPF[i]=0;
	for(i=0;i<IEC101_YXNUM/IEC101_YXNPF;i++)
		lpIEC101->YxNPF[i]=0;
	for(i=0;i<IEC101_DDNUM/IEC101_DDNPF;i++)
		lpIEC101->DdNPF[i]=0;
	//��ʼ��ÿ֡ң����,���һ֡ң����Ϊʵ����Ҫ���͵�ң����
	n=lpIEC101->YcFN;
	for(i=1;i<=n;i++)
		lpIEC101->YcNPF[i-1]=IEC101_YCNPF;
	if ((n>0)&&(lpIEC101->SendYcN%IEC101_YCNPF!=0))
		lpIEC101->YcNPF[n-1]=lpIEC101->SendYcN%IEC101_YCNPF;
	//��ʼ��ÿ֡ң����,���һ֡ң����Ϊʵ����Ҫ���͵�ң����
	n=lpIEC101->YxFN;
	for (i=1;i<=n;i++)
		lpIEC101->YxNPF[i-1]=IEC101_YXNPF;
	if ((n>0)&&(lpIEC101->SendYxN%IEC101_YXNPF!=0))
		lpIEC101->YxNPF[n-1]=lpIEC101->SendYxN%IEC101_YXNPF;
	//��ʼ��ÿ֡�����,���һ֡�����Ϊʵ����Ҫ���͵ĵ����
	n=lpIEC101->DdFN;
	for (i=1;i<=n;i++)
		lpIEC101->DdNPF[i-1]=IEC101_DDNPF;
	if ((n>0)&&(lpIEC101->SendDdN%IEC101_DDNPF!=0))
		lpIEC101->DdNPF[n-1]=lpIEC101->SendDdN%IEC101_DDNPF;
    //������ʼ����
     //	if(pChannelPara[ChanNo].wChanPara8==0)	      //��������ʱ����
     //		lpIEC101->UnsolTimeInterval=10;
     //	else
    //	        lpIEC101->UnsolTimeInterval = pChannelPara[ChanNo].wChanPara8; 
       lpIEC101->UnsolTimeInterval = pChannelPara[ChanNo].wChanPara8; //Ϊ3ʱ���ù����ٻ���ʽ������Ϊ�����Ϸ�ʽ
#endif      
}
//��ʼ��IEC101��Լ�����ṹ
void InitIEC101Prot(void)
{
    int i;
#if 0    
    lpIEC101->TypeLinkAdd=1;
    lpIEC101->TypeCmmAdd=1;
    lpIEC101->TypeInfAdd=1;
    lpIEC101->TypeSeReason=1;
#else
    lpIEC101->TypeCmmAdd=2;
    lpIEC101->TypeLinkAdd=2;
    lpIEC101->TypeInfAdd=2;
    lpIEC101->TypeSeReason=2;
#endif    
    lpIEC101->wLinkAdd.Word=1;
    lpIEC101->initstatus = notinit;
    lpIEC101->haveset = FALSE;
    lpIEC101->FlagPingH = 1;
    lpIEC101->UnsolTimeInterval=3;
    lpIEC101->firstData = nofirstdata;
   
    
    lpIEC101->TypeProtocol=1;
    lpIEC101->PSeAppLayer.lpByBuf = lpIEC101->PSendFrame.byLinkBuf + 5 + lpIEC101->TypeLinkAdd;
    lpIEC101->YcFN = MAX_CH_NUM;
    for(i=0;i<20;++i)
    {
      lpIEC101->YcNPF[i]=23;//23;
    }
    lpIEC101->DdFN = MAX_CH_NUM;
    for(i=0;i<20;++i)
    {
      lpIEC101->DdNPF[i]=8;
    }
    lpIEC101->YxFN = MAX_CH_NUM;
    for(i=0;i<20;++i)
    {
      lpIEC101->YxNPF[i]=4;//*MAX_CH_NUM;
    }
#if 0 
	ProtocolDisp[IEC101ProtNo].ProtocolType=IEC101ProtNo;	//��Լ����
	ProtocolDisp[IEC101ProtNo].RecvBuffLen=RECEBUFSIZE; 	//���ջ���������
	ProtocolDisp[IEC101ProtNo].TranBuffLen=TRANSBUFSIZE;	//���ͻ���������
	ProtocolDisp[IEC101ProtNo].SyncByte[0]=0xFF;	 		//ͬ����
	ProtocolDisp[IEC101ProtNo].SyncByte[1]=0x68;	 		//ͬ����
	ProtocolDisp[IEC101ProtNo].WordWidth=1;		 			//�ֿ��
	ProtocolDisp[IEC101ProtNo].FrameProcess=IEC101Process;	//֡������ָ��
	ProtocolDisp[IEC101ProtNo].InitProcess=InitIEC101Func; 	//��ʼ��������ָ��
#endif        
}

void SetACD(void)
{
  lpIEC101->Pacd = 1; //zzl test
}
