// Include Header file
#include <mega128.h>
#include <delay.h>
#include <stdio.h>
#include <math.h>

// Define Constants
#define DELAY_TIME 2930
#define ACTIVE 0x00
#define DEACTIVE 0x01

bool _DEBUG = False;

void USART_INIT(void); // USART initializing
void USART_TX( unsigned char ); // USART transmitting
interrupt [USART0_RXC] void USART0_RX_ISR( void ); // USART recieve interrupt
void MARK( unsigned char ); // GPIO control function
void REQUEST( unsigned char ); // USART control function

void main( void )
{
    USART_INIT();
    
    DDRB = 0x01;
    PORTB = DEACTIVE; // B Register intializing
        
    #asm("sei") // accept all interrupts
    
    while(1) // do nothing
    {
    }

}

void USART_INIT(void)
{
    UCSR0A = 0x02; // asynchronous mode
    UCSR0B = 0x98; // receive interrupt, TXE, RXE
    UCSR0C = 0x06; // 8 bit data
    UBRR0H = 0;
    UBRR0L = 0; // baudrate 2M bps
}

void USART_TX ( unsigned char data ) // USART Transmitting
{
    while( !(UCSR0A & (1<<UDRE0) ));
    UDR0 = data;
}

interrupt [USART0_RXC] void USART0_RX_ISR( void ) // USART Receive Interrupt
{
    unsigned char data = UDR0; // read the data
    REQUEST( data );
}

void REQUEST( unsigned char data )
{
    switch(data)
    {
        case 0x4c: // if 79 recieved
            USART_TX( 0x57 ); // 87 transmitting
            break;
        
        case 0x00:
            PORTB = DEACTIVE;
            break;
    }

    if( _DEBUG )
        USART_TX( data );
    
    if( data >= 0x01 && data <= 0x0c)
        PORTB = ACTIVE;
        delay_us( DELAY_TIME );
        MARK( data );
} 


void MARK( unsigned char freq )
{
    short i = 0;
    
    for( i=0; i < freq; i++)
    {
        PORTB = DEACTIVE;
        delay_us( DELAY_TIME );
        PORTB = ACTIVE;
        delay_us( DELAY_TIME );
    } 

}
