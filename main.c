#include "io430.h"

#define ADDRESS 0x31	//'1'

char data[100];
unsigned char count=0;

int main( void )
{
	Init_LaunchPAD();
	__bis_SR_register(GIE);
	while(1)
	{
		__no_operation();
	}
	return 0;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
/*{
	while (!(IFG2&UCA0TXIFG));	// USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF;	// TX -> RXed character
}*/
{
	if(UCA0CTL1&UCDORM)	//接收地址模式
	{
		if(UCA0RXBUF==ADDRESS)
		{
			UCA0CTL1&=~UCDORM;	//开工准备接收后续字符
			count=0;
			IFG2&=~UCA0RXIFG;	//手动复位接收中断
		}
	}
	else
	{
		data[count]=UCA0RXBUF;	//接收字符
		if(data[count]==0)	//接收字符串完成
		{
			UCA0CTL1|=UCDORM;	//恢复只在地址中断
		}
		else
		{
			while (!(IFG2&UCA0TXIFG));	// USCI_A0 TX buffer ready?
			//UCA0TXBUF = UCA0RXBUF;	// TX -> RXed character
			UCA0TXBUF=data[count];
			count++;
		}
	}
}