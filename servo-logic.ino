// C++ code
//
/*
  OriginaL Sweep code

  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep

  Modified by Herschenglime
*/

#include <Servo.h>

int pos = 0;

Servo servo_9;

const int pomodoroButton = 7;
const int breakoutButton = 6;

int pomodoroState

bool pomodoroPressed = false;

bool isServoOpen = false;

void setup()
{
  Serial.begin(9600);
  servo_9.attach(9, 500, 2500);

  pinMode(pomodoroButton, INPUT_PULLUP);
  pinMode(breakoutButton, INPUT_PULLUP);
}

void loop()
{


  if (digitalRead(pomodoroButton) == HIGH) {
    pomodoroPressed = false;
  }

  Serial.print("Button state: ");
  Serial.println(pomodoroPressed);

  if (pomodoroPressed == true) {
    if (isServoOpen == false){
      // sweep the servo from 0 to 90 degrees in steps
      // of 1 degrees
      for (pos = 0; pos <= 90; pos += 1) {
        // tell servo to go to position in variable 'pos'
        servo_9.write(pos);
        // wait 15 ms for servo to reach the position
        delay(15); // Wait for 15 millisecond(s)
      }

      isServoOpen = !isServoOpen;

    }else {
      for (pos = 90; pos >= 0; pos -= 1) {
        // tell servo to go to position in variable 'pos'
        servo_9.write(pos);
        // wait 15 ms for servo to reach the position
        delay(15); // Wait for 15 millisecond(s)
      }

      isServoOpen = !isServoOpen;
    }

    pomodoroPressed = false;
    delay(10);
  }
}
