#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "pinDefines.h"
#include "USART.h"

#define MAX_TARGETS       2
#define CLEAR             0
#define SET               1
#define FRONT_SENSOR      LIGHT_SENSOR
#define BACK_SENSOR       CAP_SENSOR

#define FRONT_LED         7
#define BACK_LED          6

#define SCORE_LEDS        0b00101001

#define PULSE_DOWN        2000
#define PULSE_UP          1600

volatile uint8_t targetsHit;
volatile bool badShot;
volatile uint16_t activeTarget;
volatile bool laserEnabled;

void checkBreak(void);
void showScore(void);
void releaseLaserConrol(void);
void lightLED(void);
void targetsUp(void);
void targetsDown(void);
