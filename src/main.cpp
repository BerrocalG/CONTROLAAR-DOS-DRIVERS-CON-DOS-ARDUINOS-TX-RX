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
