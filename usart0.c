#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "usart0.h"

#define UART_BUFFER_SIZE 64

int8_t Rx_Buffer[UART_BUFFER_SIZE];		// Prijemni FIFO bafer
volatile uint8_t Rx_Buffer_Size = 0;	// Broj karaktera u prijemnom baferu
volatile uint8_t Rx_Buffer_First = 0;
volatile uint8_t Rx_Buffer_Last = 0;

ISR(USART_RX_vect)
{
	Rx_Buffer[Rx_Buffer_Last++] = UDR0;		// Ucitavanje primljenog karaktera
	Rx_Buffer_Last &= UART_BUFFER_SIZE - 1;	// Povratak na pocetak u slucaju prekoracenja
	if (Rx_Buffer_Size < UART_BUFFER_SIZE)
		Rx_Buffer_Size++;					// Inkrement brojaca primljenih karaktera
}

void usartInit(uint32_t baud)
{
	/*
	 * Inicijalizacija indikatora
	 * U2Xn = 0: Onemogucena dvostruka brzina
	 * MPCMn = 0: Onemogucen multiprocesorski rezim
	 */
	UCSR0A = 0x20;

	/*
	 * RXCIEn = 1: Dozvola prekida izavanog okoncanjem prijema
	 * RXENn = 1: Dozvola prijema
	 * TXENn = 1: Dozvola slanja
	 */
	UCSR0B = 0x98;

	/*
	 * UMSELn[1:0] = 00: Asinroni rezim
	 * UPMn[1:0] = 00: Bit pariteta se ne koristi
	 * USBSn = 0: Koristi se jedan stop bit
	 * UCSzn[2:0] = 011: 8-bitni prenos
	 */
	UCSR0C = 0x06;

	uint16_t UBRRn = F_CPU / (16 * baud) - 1;
	UBRR0H = UBRRn >> 8;
	UBRR0L = UBRRn & 0xff;

	sei();	// I = 1 (dozvola prekida)
}

uint8_t usartAvailable()
{
	return Rx_Buffer_Size;		// Ocitavanje broja karaktera u prijemnom baferu
}

void usartPutChar(int8_t c)
{
	while(!(UCSR0A & 0x20));	// Ceka da se setuje UDREn (indikacija da je predajni bafer prazan)
	UDR0 = c;					// Upis karaktera u predajni bafer
}

void usartPutString(int8_t *s)
{
	while(*s != 0)				// Petlja se izvrsava do nailaska na nul-terminator
	{
		usartPutChar(*s);		// Slanje tekuceg karaktera
		s++;					// Azuriranje pokazivaca na tekuci karakter
	}
}

void usartPutString_P(const int8_t *s)
{
	while (1)
	{
		int8_t c = pgm_read_byte(s++);	// Citanje sledeceg karaktera iz programske memorije
		if (c == '\0')					// Izlazak iz petlje u slucaju
			return;						// Nailaska na terminator
		usartPutChar(c);				// Slanje karaktera
	}
}

int8_t usartGetChar()
{
	int8_t c;

	if (!Rx_Buffer_Size)						// Bafer je prazan?
		return -1;
	c = Rx_Buffer[Rx_Buffer_First++];			// Citanje karaktera iz prijemnog bafera
	Rx_Buffer_First &= UART_BUFFER_SIZE - 1;	// Povratak na pocetak u slucaju prekoracenja
	Rx_Buffer_Size--;							// Dekrement brojaca karaktera u prijemnom baferu

	return c;
}

uint8_t usartGetString(int8_t *s)
{
	uint8_t len = 0;

	while(Rx_Buffer_Size) 			// Ima karaktera u faferu?
		s[len++] = usartGetChar();	// Ucitavanje novog karaktera

	s[len] = 0;						// Terminacija stringa
	return len;						// Vraca broj ocitanih karaktera
}

int8_t usartPeek()
{

	int8_t c;

		if (!Rx_Buffer_Size)						// Bafer je prazan?
			return -1;
		c = Rx_Buffer[Rx_Buffer_First];			// Izmena samo da ne uvecava brojac

		return c;

}

int16_t usartParseInt()
{

	int8_t c;
	int16_t res=0; //promenljiva u koju smestam rezultat
	int8_t sign=1; //ako je 1 onda je + predznak, ako je -1 onda - predznak

	while((usartPeek()<'0' && usartPeek()!='-') || usartPeek()>'9')
		{
			c=usartGetChar(); //da bi citao sledeci
		}

	if(usartPeek() == '-')
		{
		sign=-1;
		c=usartGetChar();
		}


	while(usartPeek()>='0' && usartPeek()<='9')
		{
			c=usartGetChar();
			res=res*10+(c-'0');//ili ti 48 umesto nule ASCII
		}

	return res*sign;

}

int8_t* initString(int16_t duzina){
	int8_t niz[duzina];
	int8_t i=0;
	for(i=0;i<duzina;i++){
		niz[i]=' ';

	}
	niz[duzina]='\0';
	return niz;
}
void usartPrintInteger(int32_t value){
	int8_t duzina=0;
	int32_t pom=value;

	if(value==0)usartPutChar('0');
	else{
	if( value < 0) {
		usartPutChar('-');
		value=0-value;
	}



	while(pom!=0){

	    	pom=pom/10;
	    	duzina++;

	        }

	int8_t *s = initString(duzina);

	while(value!=0){

	*(s+duzina-1)=value%10 + '0';
	value=value/10;
	duzina--;
		}

	usartPutString(s);
	}

}

float usartParseFloat(){
float res;
int8_t c;
int16_t ceo=usartParseInt();
int16_t dec=0;
if(usartPeek() == '.'){
c=usartGetChar();
dec=usartParseInt();
}
while(dec!=0){
res=(res+(dec%10))/10;
dec=dec/10;
}

if(ceo >= 0){
res=(float)ceo+res;
return res;
}
else{
res=(float)(0-ceo)+res;
return 0-res;


}
}
void usartPrintFloat(float value, uint8_t num_of_digits){

	int8_t i=0;
	int32_t ceo_deo=0;
	float dec_deo=0;

	if( value < 0) {
		usartPutChar('-');
		value=0-value;
	}

	ceo_deo = (int32_t)value;
	dec_deo = value-ceo_deo;
	usartPrintInteger(ceo_deo);


	if(num_of_digits > 0)
		usartPutChar('.');

	for(i = 0; i < num_of_digits; i++){

		dec_deo = dec_deo * 10;
		int8_t cifra = (int8_t)dec_deo;
		usartPutChar(cifra+'0');
		dec_deo = dec_deo - cifra;

	}


	usartPutString("\n\r");

}
