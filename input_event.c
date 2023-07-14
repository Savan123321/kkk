/**
 * @file input_event.c
 * @brief ulazi
 * @author Antinina
 * @date 07-05-2023
 * @version 1.0
*/

#include "input_event.h"





uint8_t checkKeypad ( int8_t c)
{
	if( keypadScan() == c )
	{
		return 1;
	}
	else
		return 0;
}



uint8_t secPass ( uint32_t t0 )
{

	uint32_t t1 =timer0Millis();

	if( t1-t0 >= 1000)
		return 1;
	else
		return 0;

}
