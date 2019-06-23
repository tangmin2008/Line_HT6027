#include "General.h"
/**********************************************************************
	Memory copy in RAM
**********************************************************************/
void RAM_Write( unsigned char *Dest, unsigned char *Src, short Len )
{
   while ( Len>0 ){
      *Dest = *Src;
      Dest++;
      Src++;
      Len--;
   }
}

//�洢�ռ�����Ǩ
void RAM_Write_Rev( unsigned char *Dest, unsigned char *Src, short Len )
{
	Dest += Len - 1;
	Src += Len - 1;
   while ( Len>0 ){
      *Dest = *Src;
      Dest--;
      Src--;
      Len--;
   }
}
/**********************************************************************
	Fill a Data in RAM block
**********************************************************************/
void RAM_DataFill( unsigned char* Dest, short Len, unsigned char Data)
{
   while( Len>0 ){
      *Dest++ = Data;
      Len--;
   }
}

void RAM_Fill( unsigned char* Dest, short Len)
{
   while( Len>0 ){
      *Dest++ = 0;
      Len--;
   }
}

short IsAllData( unsigned char* Source, short Len, unsigned char Data )
{
	short i;
	for( i=0;i<Len;i++ )
	{
		if( *Source != Data )
		 	return 1;
		Source++;			
	}		
	return 0;	
}	

/**********************************************************************
	BCD Data Compare
**********************************************************************/
short Data_Comp( unsigned char *Data1, unsigned char *Data2, short Len)
{
   Data1+=Len-1;
   Data2+=Len-1;
   while( Len>0 ){
      if( (*Data1)>(*Data2) ) return 1;
      if( (*Data1)<(*Data2) ) return -1;
      Len--;
      Data1--;
      Data2--;
   }
   return 0;
}
//���ݸ�λ��ǰ
short Data_CompHL( unsigned char *Data1, unsigned char *Data2, short Len)
{
//   Data1+=Len-1;
//   Data2+=Len-1;
   while( Len>0 ){
      if( (*Data1)>(*Data2) ) return 1;
      if( (*Data1)<(*Data2) ) return -1;
      Len--;
//      Data1--;
//      Data2--;
      Data1++;
      Data2++;
   }
   return 0;
}

void SwapData( unsigned char* Dest, short Len )
{
	unsigned char Temp;
	short i;
	
	for( i=0;i<Len/2;i++ )
	{
		Temp = *(Dest+Len-1-i);	
		*(Dest+Len-1-i) = *(Dest+i);
		*(Dest+i) = Temp; 
	}	
}

void SwapDataBlock( unsigned char* Dest, unsigned char* TestBuff, short Items, short BlockLen )			//�¹���
{
	unsigned char* SBuff;
	unsigned char* DBuff;
	short i;
	
	for( i=0;i<Items/2;i++ )
	{
		SBuff = Dest+(Items-1-i)*BlockLen;
		DBuff = Dest+i*BlockLen;
		RAM_Write( TestBuff, SBuff, BlockLen );
		RAM_Write( SBuff, DBuff, BlockLen );
		RAM_Write( DBuff, TestBuff, BlockLen );
	}	
}

/*
//BitRange:1~32
unsigned char GetTrueBitSum_32Bit( unsigned char* Source, short BitRange )									//�¹���
{
	short i,j;
	unsigned char Sum=0;
	unsigned char Items;
		
	if( BitRange > 32 ) return 0;	
	for( i=0;i<4;i++ )
	{
		Items = BitRange;
		if( Items >= 8 ) 
		{
			Items = 8;
			BitRange -= 8;		
		}	
		else
		{
			Items = BitRange;	
			BitRange = 0;
		}
		
		for( j=0;j<Items;j++ )
		{
			if(( *(Source+i) & ByteBit[j] ) != 0 ) Sum++;				
		}		
		if( BitRange == 0 ) break;
	}		
	
	return Sum;	
}	


//BitNo:0~31
void SetOneBitTrue_32Bit( unsigned char* Source, short BitNo )									//�¹���
{

	if( BitNo >= 32 ) return;	
	*(Source+BitNo/8) |= ByteBit[BitNo%8];
	
}	

//BitNo:0~31
void SetOneBitFalse_32Bit( unsigned char* Source, short BitNo )									//�¹���
{

	if( BitNo >= 32 ) return;	
	*(Source+BitNo/8) &= ~ByteBit[BitNo%8];
	
}

//BitNo:0~31
short OneBitIsTrue_32Bit( unsigned char* Source, short BitNo )									//�¹���
{

	if( BitNo >= 32 ) return -1;	
	if(( *(Source+BitNo/8) & ByteBit[BitNo%8] ) != 0 ) return 0;
	else return -1;
	
}
*/
/*
//BitRange:1~128
unsigned char GetTrueBitSum_128Bit( unsigned char* Source, short BitRange )									//�¹���
{
	short i,j;
	unsigned char Sum=0;
	unsigned char Items;
		
	if( BitRange > 128 ) return 0;	
	for( i=0;i<16;i++ )
	{
		Items = BitRange;
		if( Items >= 8 ) 
		{
			Items = 8;
			BitRange -= 8;		
		}	
		else
		{
			Items = BitRange;	
			BitRange = 0;
		}
		
		for( j=0;j<Items;j++ )
		{
			if(( *(Source+i) & ByteBit[j] ) != 0 ) Sum++;				
		}		
		if( BitRange == 0 ) break;
	}		
	
	return Sum;	
}	
*/

#if( ECDisDecDigitsValid ==	YesCheck )		//13.03.03 ��ǰ������ʾС��λ��ʵ��Ч
//BCD���ݴ����ֽ���ǰ��ÿ����4λ��С10��
void LeftShiftData( unsigned char* Dest, unsigned char DataLen, unsigned char ShiftDigit )
{
	unsigned char TempH,TempL;
	short i,j;

	for( i=0;i<ShiftDigit;i++ )
	{
		for( j=0;j<DataLen;j++ )
		{
			TempL = *(Dest+j) >> 4;
			TempL &= 0x0F;
			if( j == (DataLen-1) ) TempH = 0;
			else
			{ 
				TempH = *(Dest+j+1) << 4;
				TempH &= 0xF0;
			}	
			*(Dest+j) = TempH + TempL;
		}	
	}	
}	
#endif

unsigned short SwapCharToShort( unsigned char* Data )
{
	unsigned short Temp;	
	
	Temp = *Data;
	Temp <<= 8;
	Temp += *(Data+1);	
    
    return Temp;
}

unsigned long SwapCharToLong( unsigned char* Data )
{
	unsigned long Temp;	
	short i;
	
	Temp = *Data;
	for( i=1;i<4;i++ )
	{
		Temp <<= 8;
		Temp += *(Data+i);	
	}
    
    return Temp;
}

void ShortToSwapChar( unsigned char* Dest, unsigned short Data )
{
	*(Dest+1) = (unsigned char)(Data & 0x00ff);
	*Dest = (unsigned char)(Data >> 8);
}

void LongToSwapChar( unsigned char* Dest, unsigned long Data )
{
	RAM_Write( Dest, (unsigned char*)&Data, 4 );
	SwapData( Dest, 4 );
}

void BCDStringToHEX( unsigned char* Dest, unsigned char* Source, unsigned short Len )
{
	short i;
	
	for( i=0;i<Len;i++ )
	{
		*(Dest+i) = BCD_Byte( *(Source+i) );	
	}		
}	

void HEXStringToBCD( unsigned char* Dest, unsigned char* Source, unsigned short Len )
{
	short i;
	
	for( i=0;i<Len;i++ )
	{
		*(Dest+i) = Byte_BCD( *(Source+i) );	
	}		
}	

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// pSrc: Դ����ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���ݳ���
// ����: Ŀ���ַ�������
//short gsmBytes2String(const unsigned char* pSrc, char* pDst, short nSrcLength)
short HEX_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�
    short i;        

//	pDst += nSrcLength*2 -1;

	for( i=0; i<nSrcLength; i++)
	{
//		*pDst-- = tab[*pSrc & 0x0f];	// �����4λ
		*pDst++ = tab[*pSrc >> 4];		// �����4λ
//		*pDst-- = tab[*pSrc >> 4];		// �����4λ
		*pDst++ = tab[*pSrc & 0x0f];	// �����4λ
		pSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nSrcLength * 2;
}

unsigned char SwapBit(unsigned char temp) 
{ 
     unsigned char mm = 0; 
     unsigned char i = 0x00; 
     for( i = 0x80;i!=0;i >>=1)	                           	//����߿�ʼ�ж� 
     { 
         if(temp & i)                                       //������λΪ1 
         {    
             mm >>= 1;                                      //����1λ 
             mm |= 0x80;        	                        //�����λ��1 
         } 
         else                                               //������λΪ0 
         { 
              mm >>= 1 ;                                    //���ƶ�һλ 
         } 
     }       
     return mm;                                             //���صߵ��������ֵ 
} 

void SwapBitString(unsigned char* Dest, short Len )
{
	short i;
	
	for( i=0;i<Len;i++ )
	{
		*Dest = SwapBit( *Dest );
		Dest++;
	}	
}	 






