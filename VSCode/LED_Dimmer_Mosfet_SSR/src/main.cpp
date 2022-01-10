#include <Arduino.h>
#include <math.h>

#define GPIO_OUT_W1TC_REG (*((volatile uint32_t *)(0x3ff44000 + 0xC)))
#define GPIO_OUT_W1TS_REG (*((volatile uint32_t *)(0x3ff44000 + 0x8)))

#define OUT1 0b00000010000000000000000000000000 //GPIO25
#define OUT2 0b00000100000000000000000000000000 //GPIO26

hw_timer_t *timer0 = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

uint16_t zeroCrossCount = 0;

uint32_t delayOut1 = 0; //number of interrupts of delay for out1
uint32_t delayOut2 = 0; //number of interrupts of delay for out2

void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL_ISR(&timerMux);
  uint32_t currentRegValue = REG_READ(GPIO_IN1_REG);
  static uint8_t zeroCrossStartFlag = 0;
  static uint32_t delayOut1Count = 0;
  static uint32_t delayOut2Count = 0;

  static uint32_t startDelay = 0;

  if (!(currentRegValue & (1 << 2))) {
    zeroCrossCount++;
  }
  if (zeroCrossCount != 0 && (currentRegValue & (1 << 2))) {
    zeroCrossCount = 0;
  }

  if (zeroCrossCount >= 8 && zeroCrossStartFlag == 0) {
    zeroCrossStartFlag = 1;
    startDelay = 160;
  }

  if (delayOut1Count != 0) {
    delayOut1Count--;
    if (delayOut1Count == 0) {
      GPIO_OUT_W1TC_REG |= OUT1;
    }
  }
  if (delayOut2Count != 0) {
    delayOut2Count--;
    if (delayOut2Count == 0) {
      GPIO_OUT_W1TC_REG |= OUT2;
    }
  }

  if (zeroCrossStartFlag && startDelay == 0) {
    if (delayOut1 != 0) {
      delayOut1Count = delayOut1 + 40;
    }
    if (delayOut2 != 0) {
      delayOut2Count = delayOut2 + 40;
    }

    if (delayOut1Count != 0) {
      GPIO_OUT_W1TS_REG |= OUT1;
    }
    if (delayOut2Count != 0) {
      GPIO_OUT_W1TS_REG |= OUT2;
    }

    zeroCrossStartFlag = 0;
  }

  if (startDelay != 0) {
    startDelay--;
  }

  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);

  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);

  //10kHz timer interrupt
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAlarmWrite(timer0, 50, true);
  timerAlarmEnable(timer0);
}

void loop() {
  for(uint8_t i = 1; i <= 99; i++) {
    float val = asin(sqrt(float(i) / 100.0)) / (50.0 * 3.14159) / 0.00005;
    delayOut1 = val;
    Serial.println(delayOut1);
    delay(20);
  }
  for(uint8_t i = 99; i >= 1; i--) {
    float val = asin(sqrt(float(i) / 100.0)) / (50.0 * 3.14159) / 0.00005;
    delayOut1 = val;
    Serial.println(delayOut1);
    delay(20);
  }
}