#include "laserMethods.h"

void targetsUp(void){
  OCR1A = PULSE_UP;
  DDRB |= (1 << PB1);
  _delay_ms(1500);
  DDRB &= ~(1 << PB1);
}
void targetsDown(void){
  OCR1A = PULSE_DOWN;
  DDRB |= (1 << PB1);
  _delay_ms(1500);
  DDRB &= ~(1 << PB1);
}

void showScore(void){
  switch (targetsHit) {
    case 1:
      LED_PORT |= (SET << LED0);        /* set first LED if one target is hit */
      break;
    case 2:
      LED_PORT |= (SET << 0b00001001);  /* set first two LEDs if two targets hit */
  }
}

void checkBreak(void){
  int limit = 6;
  uint16_t tempValue;
  uint8_t covertedFrontValue;
  uint8_t covertedBackValue;

  uint16_t secondCheck;
    /* Since the active target is being checked first for a good shot, a second
      check needs to be made for a bad shot. */
  switch (activeTarget) {
    case FRONT_SENSOR:
      tempValue = readADC(FRONT_SENSOR);
      covertedFrontValue = (tempValue >> 7);
      secondCheck = BACK_SENSOR;
      break;
    case BACK_SENSOR:
      tempValue = readADC(BACK_SENSOR);
      covertedBackValue = (tempValue >> 7);
      secondCheck = FRONT_SENSOR;
      break;
  }

  switch (secondCheck) {
    case FRONT_SENSOR:
      tempValue = readADC(FRONT_SENSOR);
      covertedFrontValue = (tempValue >> 7);
      break;
    case BACK_SENSOR:
      tempValue = readADC(BACK_SENSOR);
      covertedBackValue = (tempValue >> 7);
      break;
  }

  if(covertedFrontValue > limit){
    if(activeTarget != FRONT_SENSOR){
      badShot = true;
      LED_PORT &= ~(0 << FRONT_LED);
    }else{
      targetsHit++;
      LED_PORT &= ~(0 << FRONT_LED);
    }
  }
  else if(covertedBackValue > limit){
    if(activeTarget != BACK_SENSOR){
      badShot = true;
      LED_PORT &= ~(0 << BACK_LED);
    }else{
      targetsHit++;
      LED_PORT &= ~(0 << BACK_LED);
    }
  }
}

void releaseLaserConrol(void){
  DDRB |= (1 << PB2);
  laserEnabled = true;
  uint8_t GoodShot = targetsHit + 1;

  while(GoodShot != targetsHit && !badShot){  /* stay in loop until target is hit (button ISR) */
    OCR1B = 1980 - readADC(POT);
    while (TCNT1 < 5000) {;
    }                          /* delay until pulse part of cycle done */
  }

  DDRB |= (1 << PB1);

  DDRB &= ~(1 << PB2);
  laserEnabled = false;
}

void lightLED(void){
  uint8_t range = 7 - 6 + 1;
  uint8_t randomNumber;

  randomNumber = rand() % range + 6;
  switch (randomNumber) {
    case FRONT_LED:
      LED_PORT |= (SET << LED7); //LED7 (Right LED) - Front Sensor
      activeTarget = FRONT_SENSOR;
      break;
    case BACK_LED:
      LED_PORT |= (SET << LED6); //LED6 (Mid LED) - Back Sensor
      activeTarget = BACK_SENSOR;
      break;
  }
}
