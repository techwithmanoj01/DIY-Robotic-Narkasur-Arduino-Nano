#include <Servo.h>

Servo leftHand;
Servo rightHand;
Servo waist;

// Arduino Nano pins
#define LEFT_PIN 9
#define RIGHT_PIN 10
#define WAIST_PIN 11

// Hand positions
const int LEFT_UP = 140;
const int LEFT_DOWN = 30;

const int RIGHT_UP = 40;
const int RIGHT_DOWN = 150;

// Waist movement: 120 degree total twist
const int WAIST_MIN = 30;
const int WAIST_MAX = 150;

// SPEED SETTINGS
const unsigned long HAND_SPEED = 8;
const unsigned long WAIST_SPEED = 14;
const unsigned long HAND_PAUSE = 120;

// Current servo positions
int leftPos = LEFT_DOWN;
int rightPos = RIGHT_DOWN;
int waistPos = 90;

int waistDirection = 1;

// Hand movement sequence
const int handSequence[6][2] = {
  {LEFT_UP, RIGHT_UP},
  {LEFT_DOWN, RIGHT_DOWN},
  {LEFT_UP, RIGHT_UP},
  {LEFT_DOWN, RIGHT_DOWN},
  {LEFT_UP, RIGHT_DOWN},
  {LEFT_DOWN, RIGHT_UP}
};

int stepIndex = 0;

unsigned long lastHandUpdate = 0;
unsigned long lastWaistUpdate = 0;
unsigned long pauseStart = 0;

bool handPaused = false;

void setup() {

  leftHand.attach(LEFT_PIN);
  rightHand.attach(RIGHT_PIN);
  waist.attach(WAIST_PIN);

  leftHand.write(leftPos);
  rightHand.write(rightPos);
  waist.write(waistPos);
}

void loop() {

  unsigned long currentTime = millis();

  // CONTINUOUS WAIST TWIST
  if (currentTime - lastWaistUpdate >= WAIST_SPEED) {

    lastWaistUpdate = currentTime;

    waistPos += waistDirection;

    if (waistPos >= WAIST_MAX) {
      waistPos = WAIST_MAX;
      waistDirection = -1;
    }

    if (waistPos <= WAIST_MIN) {
      waistPos = WAIST_MIN;
      waistDirection = 1;
    }

    waist.write(waistPos);
  }

  // FASTER HAND MOVEMENT
  if (!handPaused &&
      currentTime - lastHandUpdate >= HAND_SPEED) {

    lastHandUpdate = currentTime;

    int targetLeft = handSequence[stepIndex][0];
    int targetRight = handSequence[stepIndex][1];

    if (leftPos < targetLeft) leftPos++;
    if (leftPos > targetLeft) leftPos--;

    if (rightPos < targetRight) rightPos++;
    if (rightPos > targetRight) rightPos--;

    leftHand.write(leftPos);
    rightHand.write(rightPos);

    // Check if both hands reached position
    if (leftPos == targetLeft &&
        rightPos == targetRight) {

      handPaused = true;
      pauseStart = currentTime;
    }
  }

  // SHORT HAND PAUSE
  // Waist keeps twisting continuously
  if (handPaused &&
      currentTime - pauseStart >= HAND_PAUSE) {

    handPaused = false;

    stepIndex++;

    if (stepIndex >= 6) {
      stepIndex = 0;
    }
  }
}
