/**
 * @file input_event.h
 * @brief ulazi
 * @author Antinina
 * @date 07-05-2023
 * @version v1.0
*/

#ifndef INPUT_EVENT_H_
#define INPUT_EVENT_H_

#include <avr/io.h>
#include "usart0.h"
#include "timer0.h"
#include "keypad.h"


/**
 * Funkcija koja proverava da li je pritisnut taster c na matricnoj tastaturi
 * @param  tipa int8_t odgovarajuci taster koji proveravamo
 * @return tipa uint8_t 1 ukoliko je dati taster pritisnut, u suprotnom 0
 */

uint8_t checkKeypad ( int8_t c);

/**
 * Funkcija koja proverava da li je protekla 1 sekunda od trenutka t0
 * @param  tipa uint32_t Pocetni vremenski trenutak od kog merimo proteklo vreme
 * @return tipa uint8_t 1 ukoliko je prosla 1 sekunda od t0, u suprotnom 0
 */
uint8_t secPass ( uint32_t t0 );


#endif /* INPUT_EVENT_H_ */
