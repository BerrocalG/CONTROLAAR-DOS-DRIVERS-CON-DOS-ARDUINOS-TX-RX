
/*
#define F_CPU 16000000UL  
#include <avr/io.h>  
#include <util/delay.h>
#include <avr/interrupt.h>

char sistema_encendido = 0;  
void config_PCI(void) {
    PCICR |= (1 << PCIE0);     
    PCMSK0 |= (1 << PCINT0);   
    PCMSK0 |= (1 << PCINT1);   
    
    DDRB &= ~0x03;  
    PORTB |= 0x03;     
}

ISR(PCINT0_vect) {
    _delay_ms(100);  

    if (!(PINB & 0x02)) {  
        PORTD &= ~0xC0;          
        sistema_encendido = 0;   
    }
    else if (!(PINB & 0x01)) {  
        PORTD |= 0x80;    
        sistema_encendido = 1;   
    }
}

int main(void) {
    DDRD |= 0xC0;  
    PORTD &= ~0xC0;                  

    config_PCI();
    sei();  

    while (1) {
    }
}
*/

// receptor
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile char señal = 0;

void config_USART(void) {
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
    UBRR0 = 103; 
}

ISR(USART_RX_vect) {
    UDR0 = señal;
}

int main(void) {
    DDRD |= 0xc0; // Motor conectado a PD7
    PORTD &= ~0xc0; // Apagado al inicio

    config_USART();
    sei();

    while (1) {
        if (señal == '1') {
            PORTD |= 0xc0;  
            señal = 0;
        } else if (señal == '0') {
            PORTD &= ~0x80; 
            señal = 0;
        }
    }
}
