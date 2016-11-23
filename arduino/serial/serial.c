#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

int initialize() {
	DDRD |= _BV(DDD6);
	DDRD |= _BV(DDD5);
	DDRD |= _BV(DDD4);
	DDRD |= _BV(DDD3);
}

int moveBackward() {
	PORTD &= ~_BV(PORTD6);
	PORTD &= ~_BV(PORTD4);
	PORTD |= _BV(PORTD5);
	PORTD |= _BV(PORTD3);	
}

int moveForward() {	
	PORTD |= _BV(PORTD6);
	PORTD |= _BV(PORTD4);
	PORTD &= ~_BV(PORTD5);
	PORTD &= ~_BV(PORTD3);
}

int turnLeft() {	
	PORTD &= ~_BV(PORTD6);
	PORTD |= _BV(PORTD4);
	PORTD |= _BV(PORTD5);
	PORTD &= ~_BV(PORTD3);
}

int turnRight() {	
	PORTD |= _BV(PORTD6);
	PORTD &= ~_BV(PORTD4);
	PORTD &= ~_BV(PORTD5);
	PORTD |= _BV(PORTD3);
}

int stop() {	
	PORTD &= ~_BV(PORTD6);
	PORTD &= ~_BV(PORTD4);
	PORTD &= ~_BV(PORTD5);
	PORTD &= ~_BV(PORTD3);
}

void USART0Init(void) {
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

void USART0SendByte(uint8_t u8Data) {
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
}

uint8_t USART0ReceiveByte() {
	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};
	// Return received data
	return UDR0;
}

int main (void) {
	
	// Initialize USART0
    USART0Init();
    uint8_t recivedData;
    uint8_t lastData;
    
    // Initialize weels pins
	initialize();    
        
    while(1)
    {
        // Receive data
        recivedData = USART0ReceiveByte();
        //lastData = recivedData;
        
        if (recivedData==65) moveForward();
        else if (recivedData==66) moveBackward();
        else if (recivedData==82) turnRight();
        else if (recivedData==76) turnLeft();
        else if (recivedData==83) stop();        
        
	USART0SendByte(recivedData);	// echo
    }
}

//avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o serial.o serial.c
//avr-gcc -mmcu=atmega328p serial.o -o serial
//avr-objcopy -O ihex -R .eeprom serial serial.hex
//sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:serial.hex
