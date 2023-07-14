/*
 * main.c
 *
 *  Created on: 07.05.2023.
 *      Author: Nina
 */

#include<stdint.h>
#include "input_event.h"
#include "output_event.h"
#include <avr/io.h>
#include "usart0.h"

#include "keypad.h"


uint8_t rows[4] = {11, 10, 9, 8};
uint8_t cols[4] = {7, 6, 5, 4};



uint16_t min(uint16_t a,uint16_t b)
{
	if(a<b)return a;
	else if(a>=b)return b;
	//m=min(m*10+(button-48),99);
}

int main()
{
	//postavke sistema
	timer0InteruptInit();
	usartInit(9600);
	keypadInit(rows, cols);
	pinInit(PORT_B, 5, OUTPUT);

	int8_t button;
	int8_t pom;

	uint32_t t0;
	uint16_t m=0;
	uint16_t s=0;
	state_t state = OFF;

	while(1)
	{

		switch(state)
				{
				case OFF:
					if(checkKeypad ('A'))
					{
						ledOn();
						logTran(OFF,ON);
						state = ON;
					}
					break;

				case ON:
					if(checkKeypad ('B'))
					{
						ledOff();
						logTran(ON,OFF);
						state = OFF;
					}
					else if(checkKeypad ('D'))
					{

						m=0;
						ledBlink();
						logTran(ON,MINUTES_INPUT);
						state= MINUTES_INPUT;
					}
					else if(checkKeypad ('C'))
					{
						s=0;
						ledBlink();
						logTran(ON,SECONDS_INPUT);
						state= SECONDS_INPUT;
					}
					break;

				case MINUTES_INPUT:
					if(checkKeypad ('B'))
					{
						ledOff();
						logTran(MINUTES_INPUT, OFF);
						state = OFF;
					}
					else if(checkKeypad ('C'))
					{

						s=0;
						ledBlink();
						logTran(MINUTES_INPUT, SECONDS_INPUT);
						state= SECONDS_INPUT;
					}
					else if(checkKeypad ('*'))
					{
						//t0=ms;

						ledBlink();
						logTran(MINUTES_INPUT, RUN);
						state= RUN;
					}
					else if(button>= '0' && button<= '9' && m<=99)
					{	ledBlink();
						m=min(m*10+(button-48),99);
						logTime(m,s);
						state= MINUTES_INPUT;
					}
					break;

				case SECONDS_INPUT:
					if(checkKeypad ('B'))
					{

						ledOff();
						logTran(SECONDS_INPUT, OFF);
						state = OFF;
					}
					else if(checkKeypad ('*'))
					{
						t0=timer0Millis();

						ledBlink();
						logTran(SECONDS_INPUT, RUN);
						state= RUN;
					}
					else if(checkKeypad ('D'))
					{
						m=0;
						ledBlink();
						logTran(SECONDS_INPUT, MINUTES_INPUT);
						state= MINUTES_INPUT;
					}
					else if(button>= '0' && button<= '9' && s<=59)
					{	ledBlink();
					s=min(s*10+(button-48),59);
					logTime(m,s);
					state= SECONDS_INPUT;
					}
					break;

				case RUN:
					if(checkKeypad ('B'))
					{

						ledOff();
						logTran(RUN, OFF);
						state = OFF;
					}
					else if(secPass(t0) && m==0 && s==0)
					{
						ledOff();
						logTran(RUN, OFF);
						state = OFF;
					}
					else if(secPass(t0)  && s==0)
					{
						ledBlink();
						m--;
						s=59;
						logTime(m,s);

						state = RUN;
					}
					else if(secPass(t0)  && s>0)
					{
						ledBlink();
						m--;
						s--;
						logTime(m,s);

						state = RUN;
					}
					break;


				}
		button=keypadScan();
		if(button==0x00 && pom==0)
		{


					pom=1;
		}
		else if(button!=0x00 && pom==1)
		{
			pom=0;
		}

	}





	return 0;

}
