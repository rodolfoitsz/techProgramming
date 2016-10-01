#include <avr/io.h>
#include <util/delay.h>

#define RED 4000
#define YELLOW 1000
#define GREEN 4000

int main (void){

/*set pin 5 of PORTB for output*/

DDRB |= _BV(DDB5);
DDRB |= _BV(DDB4);
DDRB |= _BV(DDB3);
DDRB |= _BV(DDB2);
DDRB |= _BV(DDB1);


while(1){

PORTB |= _BV(PORTB5);// PORTB5
PORTB |= _BV(PORTB2);
_delay_ms(RED);

/*set pin 5 to turn led off*/
PORTB &= ~_BV(PORTB5);


PORTB |= _BV(PORTB4);//0b00010000
_delay_ms(YELLOW);
/*set pin 4 to turn led off*/
PORTB &= ~_BV(PORTB4);
PORTB &= ~_BV(PORTB2);

PORTB |= _BV(PORTB3);//0b00100000
PORTB |= _BV(PORTB1);
_delay_ms(GREEN);

/*set pin 3 to turn led off*/
PORTB &= ~_BV(PORTB3);
PORTB &= ~_BV(PORTB1);


}
}

//avr -gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o serial-write.o serial-write.c

//avr-gcc -mmcu=atmega328p serial-write.o -o serial-write 

//generating hex file
//avr-objcopy -O ihex -R.eeprom serial-write serial-write.hex





