// Quick Demo of light sensor

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "pinDefines.h"
#include "ledStacker.h"
#include "organ.h"
#include "scale16.h"

#define SET 1
#define CLEAR 0

uint16_t readADC(uint8_t channel) {
  ADMUX = (0xf0 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

// -------- Functions --------- //
static inline void initADC0(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2);                   /* ADC clock prescaler /16 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
}

// -------- Main --------- //
int main(void) {

  initADC0();
  LED_DDR = 0xff;
  LED_PORT = CLEAR;
  SPEAKER_DDR |= (1 << SPEAKER);

  //duration and note that is played as the led's light up during gameplay
  uint16_t noteDuraton = 0xF000 /2;
  uint16_t fallingNote = C7;

  bool LEDSet;
  bool bottomReached;

  uint8_t count;
  uint8_t fallingBlock;
  uint8_t bottomLeft = 0;
  uint8_t bottomRight = 4;
  uint8_t gameState;

  //array to determine location of fallingBlock
  uint8_t leds[] = {0, 1, 2, 3, 4, 5, 6, 7};

  // ------ Event loop ------ //
  while (1) {
    gameState = CLEAR;
    count = 0;
    while(!gameOver(gameState, count)){
      fallingBlock = getStart();
      LEDSet = false;
      bottomReached = false;
      fallingBlock = checkBreak(fallingBlock, gameState);

      while(!LEDSet && !bottomReached){
        _delay_ms(readADC(POT));
        //check if next LED is set
        if(SET == getBit(gameState,fallingBlock)){
          fallingBlock++; //increment to set the the LED above the already set LED
          LEDSet = true;
        }
        //check if bottom fallingBlock has reached a bottom
        else if(leds[fallingBlock] == bottomLeft || leds[fallingBlock] == bottomRight){
          blinkLed(leds[fallingBlock], gameState);
          playNote(fallingNote, noteDuraton/6);
          bottomReached = true;
        }
        //move the fallingBlock
        else{
          fallingBlock = checkBreak(fallingBlock, gameState);
          blinkLed(leds[fallingBlock], gameState);
          playNote(fallingNote, noteDuraton/6);
          fallingBlock--;
        }
      }
      //Light the final location of the fallingBlock
      LED_PORT |=  1 << fallingBlock;
      gameState = LED_PORT;

      if(isBottomRowFilled(gameState)){
        removeRow(gameState);
        gameState = LED_PORT;
        count++;
      }
      else{
        gameState |= LED_PORT;
      }
    }
    playTheme(isWinner());
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
