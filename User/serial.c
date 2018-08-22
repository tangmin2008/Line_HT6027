#include <string.h>
#include "ht6xxx_lib.h"
#include "serial.h"
struct S_Serial_ m_sserial[1];
void udelay(int us)
{
    int i,j;
    for(i=0;i<us;i++)
    {
      for(j=0;j<22;++j)
      {
        __NOP();
      }
     // HT_FreeDog();
    }
}

int8_t Serial_Open(uint8_t port,uint32_t baud,uint8_t bits,UARTParity_TypeDef check)
{
        UART_InitTypeDef UART_InitStructure;
	HT_UART_TypeDef * pUart;
	
	switch(port)
	{
		case 0:
			pUart=HT_UART0;
                        HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);		/*!< ʹ��UART0ģ��				*/
			break;
		case 1:
			pUart=HT_UART1;
                        HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART1EN, ENABLE);		/*!< ʹ��UART1ģ��				*/
			break;
		case 2:
			pUart=HT_UART2;
                        HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, ENABLE);
			break;
		default:
			return 0;
	}

	port = 0;
	
	m_sserial[port].send_pos=0;
        m_sserial[port].rx_len=0;
        m_sserial[port].rx_len=0;
        m_sserial[port].rx_pos=0;
	
         UART_InitStructure.UART_Logic = UartLogicPositive;		  /*!< UART�߼�Ϊ�� 			  */
	 UART_InitStructure.UART_StopBits = OneStopBits;			  /*!< 1λֹͣλ				  */
	 UART_InitStructure.UART_WordLength = WordLength_8Bits ;	  /*!< 8λ����λ				  */
	 UART_InitStructure.UART_Parity = check;		  /*!< ��У�� */
	 UART_InitStructure.UART_BaudRate = baud;					  /*!< �����ʼ������300		  */
	 UART_InitStructure.ReceiveEN = ENABLE;					  /*!< ����ʹ��   ע�����ж�ʹ��  */
	 UART_InitStructure.SendEN = ENABLE; 					  /*!< ����ʹ��   ע�����ж�ʹ��  */	  
	 //UART_InitStructure.SendEN = DISABLE;						  /*!< ����ʹ��   ע�����ж�ʹ��  */	  
	 
	 HT_UART_Init(pUart, &UART_InitStructure);
	 
	 HT_UART_ClearITPendingBit(pUart, UART_UARTSTA_RXIF);
	 HT_UART_ITConfig(pUart, UART_UARTCON_RXIE|UART_UARTCON_TXIE, ENABLE);
	return 1;
}

int16_t Serial_Write(uint8_t port,uint8_t *buf,uint16_t len)
{
	HT_UART_TypeDef * pUart;
	uint16_t i,j;
	if(len==0)
		return len;
	switch(port)
	{
		case 0:
			pUart=HT_UART0;
			break;
		case 1:
			pUart=HT_UART1;
			break;
		case 2:
			pUart=HT_UART2;
                        HT_GPIO_BitsReset(HT_GPIOB,GPIO_Pin_10);
			break;
		default:
			return 0;
	}
        port=0;
        j = 0;
        while(m_sserial[port].send_len && ++j<5000)
         udelay(100);// HT_FreeDog();
	j=0;
        //pUart->UARTCON &=~ (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
        pUart->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
	do
	{
		if(len>SERIAL_BUFFER_LEN)
			i=SERIAL_BUFFER_LEN;
		else
			i=len;
		memcpy(m_sserial[port].serial_tx_buf,buf,i);
		m_sserial[port].send_len=i;
		pUart->SBUF = m_sserial[port].serial_tx_buf[0];
		m_sserial[port].send_pos=1;
		len -= i;
		j += i;
	}while(len);
	return j;
}
int16_t Serial_Read(uint8_t port,uint8_t *buf,uint16_t len)
{
	uint16_t i;

	i=0;
	port=0;//
	while(m_sserial[port].rx_len!=m_sserial[port].rx_pos)
	{
		
		buf[i]=m_sserial[port].serial_rx_buf[m_sserial[port].rx_pos];
		m_sserial[port].rx_pos++;
		m_sserial[port].rx_pos%=SERIAL_BUFFER_LEN;
		i++;
		if(i>=len)
			break;
	}
	len=i;

	return len;
}

void UART2_IRQHandler()
{
  uint8_t port;
  port=0;
  if(SET == HT_UART_ITFlagStatusGet(HT_UART2, UART_UARTSTA_RXIF))
	{	
                HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF); 
		m_sserial[port].serial_rx_buf[m_sserial[port].rx_len]=HT_UART2->SBUF;
		m_sserial[port].rx_len++;
		m_sserial[port].rx_len%=SERIAL_BUFFER_LEN;
	}
	if(SET == HT_UART_ITFlagStatusGet(HT_UART2, UART_UARTSTA_TXIF))
	{	
                HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_TXIF);
		if(m_sserial[port].send_pos!=m_sserial[port].send_len)
		{
			HT_UART2->SBUF = m_sserial[port].serial_tx_buf[m_sserial[port].send_pos];
			m_sserial[port].send_pos++;
		}
		else
		{
			m_sserial[port].send_pos=0;
			m_sserial[port].send_len=0;
                        HT_UART2->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
                        HT_UART2->UARTCON |= (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
                        HT_GPIO_BitsSet(HT_GPIOB,GPIO_Pin_10);
		}				
	}
	return;
}

void UART0_IRQHandler()
{
  uint8_t port;
  port=0;
  if(SET == HT_UART_ITFlagStatusGet(HT_UART0, UART_UARTSTA_RXIF))
	{	
                HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF); 
		m_sserial[port].serial_rx_buf[m_sserial[port].rx_len]=HT_UART0->SBUF;
		m_sserial[port].rx_len++;
		m_sserial[port].rx_len%=SERIAL_BUFFER_LEN;
	}
	if(SET == HT_UART_ITFlagStatusGet(HT_UART0, UART_UARTSTA_TXIF))
	{	
                HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_TXIF);
		if(m_sserial[port].send_pos!=m_sserial[port].send_len)
		{
			HT_UART0->SBUF = m_sserial[port].serial_tx_buf[m_sserial[port].send_pos];
			m_sserial[port].send_pos++;
		}
		else
		{
			m_sserial[port].send_pos=0;
			m_sserial[port].send_len=0;
                        HT_UART0->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
                        HT_UART0->UARTCON |= (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
                        HT_GPIO_BitsSet(HT_GPIOB,GPIO_Pin_10);
		}				
	}
	return;
}


void CM_Gprs_Interrupt_Handle(HT_UART_TypeDef * pUart)
{
        if(SET == HT_UART_ITFlagStatusGet(pUart, UART_UARTSTA_RXIF))
	{	
                HT_UART_ClearITPendingBit(pUart, UART_UARTSTA_RXIF); 
		m_sserial[0].serial_rx_buf[m_sserial[0].rx_len]=pUart->SBUF;
		m_sserial[0].rx_len++;
		m_sserial[0].rx_len%=SERIAL_BUFFER_LEN;
	}
	if(SET == HT_UART_ITFlagStatusGet(pUart, UART_UARTSTA_TXIF))
	{	
                HT_UART_ClearITPendingBit(pUart, UART_UARTSTA_TXIF);
		if(m_sserial[0].send_pos!=m_sserial[0].send_len)
		{
			pUart->SBUF = m_sserial[0].serial_tx_buf[m_sserial[0].send_pos];
			m_sserial[0].send_pos++;
		}
		else
		{
			m_sserial[0].send_pos=0;
			m_sserial[0].send_len=0;
                        
                        pUart->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
                        pUart->UARTCON |= (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
		}				
	}
	return;
}


