/**
 * @file output_event.c
 * @brief ulazi
 * @author Antinina
 * @date 07-05-2023
 * @version v1.0
*/
#include "output_event.h"



void logTran (state_t state, state_t next_state)
{
	usartPutString("\n\r Transsaction from ");

	switch(state)
	{
	case ON:
		usartPutString("ON");
		break;
	case OFF:
		usartPutString("OFF");
		break;
	case SECONDS_INPUT:
		usartPutString("SECONDS_INPUT");
		break;
	case MINUTES_INPUT:
		usartPutString("MINUTES_INPUT");
		break;
	case RUN:
		usartPutString("RUN");
		break;


	}

	usartPutString("\n\r to state ");

	switch(next_state)
		{
	case ON:
			usartPutString("ON");
			break;
		case OFF:
			usartPutString("OFF");
			break;
		case SECONDS_INPUT:
			usartPutString("SECONDS_INPUT");
			break;
		case MINUTES_INPUT:
			usartPutString("MINUTES_INPUT");
			break;
		case RUN:
			usartPutString("RUN");
			break;


		}
	usartPutString(".\n\r");
}

void logTime ( uint16_t mm, uint16_t ss )
{
	usartPutString("\n\rTime ");
	usartPrintInteger(mm);
	usartPutString(" : ");
	usartPrintInteger(ss);
	usartPutString(".\n\r");

}

void logCut()
{
	usartPutString("Cutting!\n\r");
}

void ledOff()
{
	pinSetValue(PORT_B, 5, LOW);
}

void ledOn()
{
	pinSetValue(PORT_B, 5, HIGH);
}

void ledBlink()
{
	pinSetValue(PORT_B, 5, HIGH);
	timer0DelayMs(500);
	pinSetValue(PORT_B, 5, LOW);
	timer0DelayMs(500);
}
