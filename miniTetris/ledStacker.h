
// ------------- Function prototypes -------------- //

// Plays a note for the given duration.  None of these times are
//  calibrated to actual notes or tempi.  It's all relative to TIMEBASE.
#include <stdbool.h>


uint8_t getStart();
bool isWinner();

bool isLedLit(uint8_t curState, uint8_t blockLocation);
uint8_t checkBreak(uint8_t curBlock, uint8_t gameState);
uint8_t switchSides(uint8_t curLocation);
uint8_t getBit(uint8_t gameState, uint8_t nthBit);
void blinkLed(uint8_t curLocation, uint8_t curState);
bool isBottomRowFilled(uint8_t curState);
void removeRow(uint8_t curState);
bool gameOver(uint8_t curState, uint8_t count);
