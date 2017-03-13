/*
Methods used in lightSensor to control LED's
*/

#include <avr/io.h>
#include <util/delay.h>
#include "ledStacker.h"
#include "pinDefines.h"
#include "scale16.h"
#include "organ.h"

#define END_GAME_LOSS_1 0b11110000
#define END_GAME_LOSS_2 0b00001111
#define END_GAME_WIN 0b01110111
#define BOTTOM_FILLED 0b00010001
#define BOTTOMROW_ZERO 0b11101110

uint8_t topLeft = 3;
uint8_t topRight = 7;

bool winner;

uint8_t getStart() {
    return ((rand() % 2) == 0) ? topLeft : topRight;
}

bool isWinner(){
  return winner;
}

bool isLedLit(uint8_t curState, uint8_t blockLocation) {
  return (blockLocation == (curState & blockLocation));
}

uint8_t checkBreak(uint8_t curBlock, uint8_t gameState){
  int limit = 6;
  uint16_t lightValue = readADC(LIGHT_SENSOR);
  uint8_t covertedValue = (lightValue >> 7);

  uint8_t temp;

  if (covertedValue > limit){
    temp = switchSides(curBlock);
    uint8_t theBit = getBit(gameState, temp);
    if(theBit == 0){
      curBlock = temp;
    }
  }
  return curBlock;
}

uint8_t getBit(uint8_t gameState, uint8_t nthBit){
  int mask =  1 << nthBit;
  int masked_n = gameState & mask;
  return (masked_n >> nthBit);
}

uint8_t switchSides(uint8_t curLocation){
  return (curLocation < 4) ? (curLocation + 4) : (curLocation - 4);
}

void blinkLed(uint8_t curLocation, uint8_t curState){
  LED_PORT = (1 << curLocation);
  LED_PORT |= curState; // needed to prevent occupied LEDs from flickering
}

bool isBottomRowFilled(uint8_t curState){
  return (BOTTOM_FILLED == (curState & BOTTOM_FILLED));
}


void removeRow(uint8_t curState){
  uint8_t newState;
  LED_PORT = (BOTTOMROW_ZERO & curState);
  _delay_ms(readADC(POT));
  newState = (BOTTOM_FILLED ^ curState);
  newState = newState >> 1;

  LED_PORT = newState;
}

bool gameOver(uint8_t curState, uint8_t curCount){
  bool endGame = false;

  if(END_GAME_LOSS_1 == (END_GAME_LOSS_1 & curState)){
    winner = false;
    endGame = true;
  }
  else if(END_GAME_LOSS_2 == (END_GAME_LOSS_2 & curState)){
    winner = false;
    endGame = true;
  }
  else if(curCount >= 10){
    endGame = true;
    winner = true;
  }
  else{
    winner = false;
  }
  return endGame;
}

void playTheme(bool win){
  uint16_t shortNoteDuration = (0xF000/3) - 2500;
  uint16_t noteDuration = (0xF000/3);
  uint16_t mediumNoteDuration =(0xF000/3) * 3;
  uint16_t longNoteDuration = (0xF000/3) * 6;
  uint16_t restDuration = (0xF000/3) - 20000;
  uint16_t shortRestDuration = (0xF000/3) - 15000;
  switch(win) {
    case 0:
      //Losing Theme
      playNote(B6, shortNoteDuration);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      playNote(A6, shortNoteDuration);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      rest(restDuration*6);
      playNote(Fx6, noteDuration*2);

      playNote(D7, noteDuration);
      rest(restDuration);
      playNote(Dx7, noteDuration);
      rest(restDuration);
      playNote(D7, noteDuration);
      rest(restDuration);
      playNote(Cx7, 35000);
      rest(restDuration);
      playNote(Dx7, 35000);
      rest(restDuration);
      playNote(Cx7, 35000);
      rest(restDuration);
      playNote(B6, shortNoteDuration);
      playNote(Ax6, shortNoteDuration);
      playNote(C7, longNoteDuration);
      playNote(C7, longNoteDuration);
      rest(longNoteDuration);
      rest(longNoteDuration);
      rest(longNoteDuration);
      rest(longNoteDuration);
      break;
    case 1:
      //Winning Theme
      playNote(Fx6, shortNoteDuration);
      rest(restDuration);
      playNote(A6, shortNoteDuration);
      rest(restDuration);
      playNote(B6, shortNoteDuration);
      rest(restDuration);
      playNote(Cx7, shortNoteDuration);
      rest(restDuration);
      playNote(E7, shortNoteDuration);
      rest(restDuration);
      playNote(Fx7, shortNoteDuration);
      rest(restDuration);
      playNote(Gx7, longNoteDuration);
      rest(restDuration);
      playNote(Fx7, longNoteDuration);
      rest(restDuration);
      rest(restDuration);

      playNote(G6, shortNoteDuration);
      rest(restDuration);
      playNote(A6, shortNoteDuration);
      rest(restDuration);
      playNote(B6, shortNoteDuration);
      rest(restDuration);
      playNote(D7, shortNoteDuration);
      rest(restDuration);
      playNote(E7, shortNoteDuration);
      rest(restDuration);
      playNote(Fx7, shortNoteDuration);
      rest(restDuration);
      playNote(A7, longNoteDuration);
      rest(restDuration);
      playNote(Fx7, longNoteDuration);
      rest(restDuration);

      playNote(Gx6, shortNoteDuration);
      rest(restDuration);
      rest(restDuration);
      playNote(Ax6, shortNoteDuration);
      rest(restDuration);
      rest(restDuration);
      playNote(C7, shortNoteDuration);
      rest(restDuration);
      rest(restDuration);
      playNote(Dx7, shortNoteDuration);
      rest(restDuration);
      rest(restDuration);
      playNote(F7, shortNoteDuration);
      rest(restDuration);
      rest(restDuration);
      playNote(G7, shortNoteDuration);
      rest(restDuration);
      rest(restDuration);

      playNote(Ax7, longNoteDuration);
      rest(restDuration);
      playNote(Ax7, shortNoteDuration);
      rest(restDuration);
      playNote(Ax7, shortNoteDuration);
      rest(restDuration);
      playNote(Ax7, shortNoteDuration);
      playNote(B7, longNoteDuration);
      playNote(B7, longNoteDuration);
      playNote(B7, longNoteDuration);
      rest(longNoteDuration);
      rest(longNoteDuration);
      rest(longNoteDuration);
      rest(longNoteDuration);
      break;
  }
}
