/**
 * @file output_event.h
 * @brief ulazi
 * @author Antinina
 * @date 07-05-2023
 * @version v1.0
*/

#ifndef OUTPUT_EVENT_H_
#define OUTPUT_EVENT_H_

#include <avr/io.h>
#include "usart0.h"
#include "timer0.h"
#include "pin.h"
#include "keypad.h"

///moguca stanja unutar automata konacnih stanja
typedef enum state_t {ON, OFF, RUN, MINUTES_INPUT, SECONDS_INPUT} state_t;

/**
 * Funkcija koja preko serijskog terminala ispisuje poruku u formatu:
 *  "Transsactionon from {state} to {state_next}."
 * @param state tipa enum state_t oznacava tekuce stanje
 * @param next_state tipa enum state_t oznacava sledece stanje
 * @return Nema povratnu vrednost
 */
void logTran (state_t state, state_t next_state);

/**
 * Funkcija koja preko serijskog terminala ispisuje poruku u formatu:
 *  "Time {mm}:{ss}."
 * @param mm tipa uint16_t oznacava minute
 * @param ss tipa uint16_t oznacava sekunde
 * @return Nema povratnu vrednost
 */
void logTime ( uint16_t mm, uint16_t ss );

/**
 * Funkcija koja putem serijskog terminala ispisuje poruku "Cutting!"
 * @param Bez parametara
 * @return Nema povratnu vrednost
 */
void logCut();

/**
 * Funkcija koja gasi diodu na Arduino UNO ploci
 */
void ledOff();

/**
 * Funkcija koja pali diodu na Arduino UNO ploci
 */
void ledOn();

/**
 * Funkcija koja pali i gasi diodu na Arduino UNO ploci u vremenskom interval od 1 sekunde
 */
void ledBlink();

#endif /* OUTPUT_EVENT_H_ */
