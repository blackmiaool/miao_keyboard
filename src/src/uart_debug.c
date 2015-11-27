/* Includes */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <rt_misc.h>


//USART2
USART_TypeDef* COM_USART        = USART6;
GPIO_TypeDef*  COM_TX_PORT      = GPIOC;
GPIO_TypeDef*  COM_RX_PORT      = GPIOC;
const uint32_t COM_USART_CLK    = RCC_APB2Periph_USART6;
const uint32_t COM_TX_PORT_CLK  = RCC_AHB1Periph_GPIOC;
const uint32_t COM_RX_PORT_CLK  = RCC_AHB1Periph_GPIOC;
const uint16_t COM_TX_PIN       = GPIO_Pin_6;
const uint16_t COM_RX_PIN       = GPIO_Pin_7;
const uint16_t COM_TX_PIN_SOURCE= GPIO_PinSource6;
const uint16_t COM_RX_PIN_SOURCE= GPIO_PinSource7;
const uint16_t COM_TX_AF        = GPIO_AF_USART6;
const uint16_t COM_RX_AF        = GPIO_AF_USART6;

void uart_debug_init()
{    
	  USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	  /* USARTx configured as follow:
	        - BaudRate = 230400 baud
	        - Word Length = 8 Bits
	        - One Stop Bit
	        - No parity
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Receive and transmit enabled
	  */
	  USART_InitStructure.USART_BaudRate = 115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	  /* Enable GPIO clock */
	  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK | COM_RX_PORT_CLK, ENABLE);
	  /* Enable UART clock */
	  RCC_APB2PeriphClockCmd(COM_USART_CLK, ENABLE);

	  /* Connect PXx to USARTx_Tx*/
	  GPIO_PinAFConfig(COM_TX_PORT, COM_TX_PIN_SOURCE, COM_TX_AF);

	  /* Connect PXx to USARTx_Rx*/
	  GPIO_PinAFConfig(COM_RX_PORT, COM_RX_PIN_SOURCE, COM_RX_AF);

	  /* Configure USART Tx as alternate function  */
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(COM_TX_PORT, &GPIO_InitStructure);

	  /* Configure USART Rx as alternate function  */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN;
	  GPIO_Init(COM_RX_PORT, &GPIO_InitStructure);

	  /* USART configuration */
	  USART_Init(COM_USART, &USART_InitStructure);

	  /* Enable USART */
	  USART_Cmd(COM_USART, ENABLE);
        
		/* Flush the USART buffer */
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
		
}

/*----------------------------------------------------------------------------
  SendChar
  Write character to Serial Port.
 *----------------------------------------------------------------------------*/

int SendChar (int ch)  {

//    if (ch=='\n')  {
//    
//        USART_SendData(USART6, (uint8_t)'\r');
//        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
//	    {}   
//    } 
      
    USART_SendData(USART6, (uint8_t) ch);

	/* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
	{}
    
  	return (ch);
}

/*----------------------------------------------------------------------------
  GetKey
  Read character to Serial Port.
 *----------------------------------------------------------------------------*/
int GetKey (void)  {

  int  ret;  
  /* Loop until incomming data */
  while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
  {}

  ret = USART_ReceiveData(USART2);

  return ret;
}

struct __FILE {
  int handle;                 // Add whatever you need here 
};
//FILE __stdout;
//FILE __stdin;

/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) {
  return (SendChar(ch));
}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) {
  return (SendChar(GetKey()));
}

/*----------------------------------------------------------------------------
  _ttywrch
 *----------------------------------------------------------------------------*/
void _ttywrch(int ch) {
 SendChar (ch);
}

/*----------------------------------------------------------------------------
  ferror
 *----------------------------------------------------------------------------*/
int ferror(FILE *f) {
                              // Your implementation of ferror
  return EOF;
}

/*----------------------------------------------------------------------------
  _sys_exit
 *----------------------------------------------------------------------------*/
void _sys_exit(int return_code) {
label:  goto label;           // endless loop
}
