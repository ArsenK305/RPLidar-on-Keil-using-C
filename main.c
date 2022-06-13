#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
//PC11=RX
//PC10=TX
//DMA STREAM2, voie 4


//At 115200 bauds, 100 bytes means 1ms time
//Practicle example: Imagine we received 10 bytes at 115200 bauds. 
//Each byte at 115200 bauds takes about 10us on UART line, total 100us. 
//IDLE line interrupt will notify application when it will detect for 1 character inactivity on RX line, 
//meaning after 10us after last character. 
//Application may react on this event and process data accordingly.

void inittimer12 (void);

int main(void){
	inittimer12();
	RCC->AHB1ENR|=(1<<2);
	__HAL_RCC_UART4_CLK_ENABLE();
	
	GPIO_InitTypeDef UART4_GPIO_Handler;
	UART4_GPIO_Handler.Pin= GPIO_PIN_10 | GPIO_PIN_11;
	UART4_GPIO_Handler.Mode=GPIO_MODE_AF_PP;
	UART4_GPIO_Handler.Pull=GPIO_PULLUP;
	UART4_GPIO_Handler.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	UART4_GPIO_Handler.Alternate= GPIO_AF8_UART4;
	HAL_GPIO_Init(GPIOC, &UART4_GPIO_Handler);
	
	UART_HandleTypeDef UART_Handler;
	UART_Handler.Instance= UART4;
	UART_Handler.Init.BaudRate= 115200;
	UART_Handler.Init.Mode= UART_MODE_TX_RX;
	UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;
	UART_Handler.Init.StopBits=UART_STOPBITS_1;
	UART_Handler.Init.OverSampling=UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART_Handler);
	printf("Init done\n");
	char Message[]={0xA5,0x50};
	char Reponse[]={};
	uint8_t UART4_rxBuffer[1] = {0};
	HAL_UART_Transmit(&UART_Handler, (uint8_t *)Message, 2, 10);
	char a;
	while(1)
    {
			
        	printf("while\n");

        
			HAL_UART_Receive(&UART_Handler,UART4_rxBuffer,1,10);
			a=UART4_rxBuffer[0];
			printf("%x\n",a);
    }
}

void inittimer12 (void) {
 
    RCC->AHB1ENR |=(1<<1);
    RCC->APB1ENR |=(1<<6) ;
    GPIOB->MODER|=(1<<29);
    GPIOB->MODER&=~(1<<28);
    GPIOB->AFR[1]=(9<<24);
    
    TIM12->PSC=44;
    TIM12->ARR=199;
    TIM12->CCMR1 &=~(3);
    TIM12->CCMR1 |=(3<<5);
    TIM12->CCMR1 &=~((1<<4));
    TIM12->CCER |=1;
    TIM12->CCER &=~(1<<1);
    TIM12->CCR1=149;
    TIM12->EGR |=1;
    TIM12->CR1 |=1;
    
}
//val[0]=
//val[1]=
//void init_DMA_UART(){
//	RCC->AHB1ENR|=(1<<21);
//	DMA1_Stream2->CR&=~(1<<0)
//	DMA1_Stream2->CR|=(4<<25)|(1<<24)|(1<<22)|(1<<18);
//	DMA1_Stream2->CR&=~(1<<23)|(1<<21);
//}
// Fonction permettant de rediriger le printf sur l'interface de debug (ITM)
int fputc(int ch, FILE *f){

    return (int)ITM_SendChar((uint32_t)ch);

	

}