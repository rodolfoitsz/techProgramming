#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define MOVING_TIME 100
#define ROTATING_TIME 500

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


void adc_init(void){
 
 ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
 ADMUX |= (1<<REFS0);                //Voltage reference from Avcc (5v)
 ADCSRA |= (1<<ADEN);                //Turn on ADC
 ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}
 
uint16_t read_adc(uint8_t channel){
 ADMUX &= 0xF0;                    //Clear the older channel that was read
 ADMUX |= channel;                //Defines the new ADC channel to be read
 ADCSRA |= (1<<ADSC);                //Starts a new conversion
 while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
 return ADCW;                    //Returns the ADC value of the chosen channel
}


void move() {
	moveForward();
	_delay_ms(MOVING_TIME);
	stop();
}

uint8_t tmp[4];
uint8_t* sense() {
	
	
	tmp[0] = read_adc(0);
	
	turnLeft();
	_delay_ms(ROTATING_TIME);
	stop();
	tmp[1] = read_adc(0);
	
	turnLeft();
	_delay_ms(ROTATING_TIME);
	stop();
	tmp[2] = read_adc(0);

	turnLeft();
	_delay_ms(ROTATING_TIME);
	stop();
	tmp[3] = read_adc(0);

	
	
	return tmp;
}


void sendData(){

	// sense and send results
		uint8_t *resultFromSensing = sense();

		USART0SendByte(resultFromSensing[0]);
		USART0SendByte(resultFromSensing[1]);
		USART0SendByte(resultFromSensing[2]);
		USART0SendByte(resultFromSensing[3]);



}


int main (void) {
	
    // Initialize USART0
    USART0Init();
    uint8_t recivedData;
    uint8_t lastData;
    
    // Initialize weels pins
	initialize();    
        
	// Initialize light sensor
	adc_init();
	
	int exit = 0;


	    sendData();

    while(!exit)
    {
 	
		
        // Receive data
        recivedData = USART0ReceiveByte();
       
         if (recivedData==105) 				// turn left and go in that dir
		{

       sendData();

		}

        
        if (recivedData==65) 				// turn left and go in that dir
		{

       

		}
		else if (recivedData==66) 			// don't turn, simply go 
		{		
			move();
		}
		else if (recivedData==67) 			// turn right and go in that dir
		{
			turnRight();
			_delay_ms(ROTATING_TIME);
			stop();
			move();
		}
		else if (recivedData==68)			// has arrived. STOP
			exit++;        
        
    }
}

//avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o serial.o serial.c
//avr-gcc -mmcu=atmega328p serial.o -o serial
//avr-objcopy -O ihex -R .eeprom serial serial.hex
//sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:serial.hex
