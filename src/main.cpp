
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

//emisor
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile char senal2 = 0;

void config_USART(void) {
    UBRR0 = 0x67;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}

void enviar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void config_PCINT(void) {
    PCICR |= (1 << PCIE0);           
    PCMSK0 |= (1 << PCINT0) | (1 << PCINT1); 
}

ISR(PCINT0_vect) {
    if (!(PINB & 0x01)) {
        _delay_ms(50);
        if (!(PINB & 0x01)) {
            enviar('1');  
        }
    }
    if (!(PINB & 0x02)) {
        _delay_ms(50);
        if (!(PINB & 0x02)) {
            enviar('0'); 
        }
    }
}

ISR(USART_RX_vect) {
    senal2 = UDR0;
}

int main(void) {
    DDRD |= 0xC0;     
    PORTD &= ~0xC0;   

    DDRB &= ~0x03;    
    PORTB |= 0x03;    

    config_USART();
    config_PCINT();
    sei();

    while (1) {
        if (senal2 == '1') {
            PORTD |= 0xC0;  
            senal2 = 0;
        } else if (senal2 == '0') {
            PORTD &= ~0x80; 
            senal2 = 0;
        }
    }
}
