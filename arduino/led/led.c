#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 4000

int main (void){

/*set pin 5 of PORTB for output*/

DDRB |= _BV(DDB5);

while(1){

PORTB |= _BV(PORTB5);//0b00100000
_delay_ms(BLINK_DELAY_MS);

/*set pin 5 to turn led off*/
PORTB &= ~_BV(PORTB5);
_delay_ms(BLINK_DELAY_MS); 

}
}

//avr -gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o serial-write.o serial-write.c

//avr-gcc -mmcu=atmega328p serial-write.o -o serial-write 

//generating hex file
//avr-objcopy -O ihex -R.eeprom serial-write serial-write.hex






