                  /* Quick interactive demo running servo with Timer 1 */

// ------- Preamble -------- //
#include "laserMethods.h"

static inline void initTimers(void) {
    /* Set up Timer1 (16bit) to give a pulse every 20ms */
              /* Use Fast PWM mode, counter max in ICR1 */
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  TCCR1B |= (1 << CS10);  /* /1 prescaling -- counting in microseconds */
  ICR1 = 20000;                                    /* TOP value = 20ms */
  TCCR1A |= (1 << COM1A1);        /* Direct output on PB1 / OC1A (Target Servos) */
  TCCR1A |= (1 << COM1B1);        /* Direct output on PB2 / OC1B (Laser Servos) */
}

uint16_t readADC(uint8_t channel) {
  ADMUX = (0xf0 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

static inline void initADC0(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2);                   /* ADC clock prescaler /16 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
}

ISR(INT0_vect) { /* Run every time there is a button pressed */
  if(laserEnabled){
    if (bit_is_set(BUTTON_PIN, BUTTON)) {
      LED_PORT &= ~(1 << LED4);
      checkBreak(); //check if active target was hit
    }
    else {
      LED_PORT |= (1 << LED4);
    }
  }
}

void initInterrupt0(void) {
  EIMSK |= (1 << INT0); /* enable INT0 */
  EICRA |= (1 << ISC00); /* trigger when button changes */
  sei(); /* set (global) interrupt enable bit */
}

int main(void) {
  // -------- Inits --------- //
  initADC0();
  LED_DDR = 0xff;
  LED_PORT = CLEAR;
  BUTTON_PORT |= (1 << BUTTON);
  initInterrupt0();

  initTimers();
  initUSART();

  // ------ Event loop ------ //
  while (1) {
    targetsHit = 0;
    badShot = false;

    while(targetsHit < 3 && !badShot){
      showScore();
      targetsUp();
      lightLED();
      releaseLaserConrol();
      targetsDown();
    }
    if(targetsHit > MAX_TARGETS) {                     /* WIN */
      for(int i=0;i < 6;i++){
        LED_PORT |= (SET << 0b00101001);
        _delay_ms(1000);
        LED_PORT &= ~(SET << 0b00101001);
        _delay_ms(1000);
      }
    }
    else{                                   /* LOSE */
      LED_PORT |= (SET << 0b11000000);
      _delay_ms(1000);
      LED_PORT &= ~(SET << LED6);
      _delay_ms(1000);
      LED_PORT &= ~(SET << LED7);
      _delay_ms(1000);
     }
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
