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

// including external button library to make debouncing easier
/*
 * Copyright (c) 2019, ArduinoGetStarted.com. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the ArduinoGetStarted.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARDUINOGETSTARTED.COM "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARDUINOGETSTARTED.COM BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ezButton_h
#define ezButton_h


#define COUNT_FALLING 0
#define COUNT_RISING  1
#define COUNT_BOTH    2

class ezButton
{
	private:
		int btnPin;
		unsigned long debounceTime;
		unsigned long count;
		int countMode;

		int previousSteadyState;  // the previous steady state from the input pin, used to detect pressed and released event
		int lastSteadyState;      // the last steady state from the input pin
		int lastFlickerableState; // the last flickerable state from the input pin

		unsigned long lastDebounceTime; // the last time the output pin was toggled

	public:
		ezButton(int pin);
		ezButton(int pin, int mode);
		void setDebounceTime(unsigned long time);
		int getState(void);
		int getStateRaw(void);
		bool isPressed(void);
		bool isReleased(void);
		void setCountMode(int mode);
		unsigned long getCount(void);
		void resetCount(void);
		void loop(void);
};

#endif

ezButton::ezButton(int pin): ezButton(pin, INPUT_PULLUP) {};

ezButton::ezButton(int pin, int mode) {
	btnPin = pin;
	debounceTime = 0;
	count = 0;
	countMode = COUNT_FALLING;

	pinMode(btnPin, mode);

	previousSteadyState = digitalRead(btnPin);
	lastSteadyState = previousSteadyState;
	lastFlickerableState = previousSteadyState;

	lastDebounceTime = 0;
}

void ezButton::setDebounceTime(unsigned long time) {
	debounceTime = time;
}

int ezButton::getState(void) {
	return lastSteadyState;
}

int ezButton::getStateRaw(void) {
	return digitalRead(btnPin);
}

bool ezButton::isPressed(void) {
	if(previousSteadyState == HIGH && lastSteadyState == LOW)
		return true;
	else
		return false;
}

bool ezButton::isReleased(void) {
	if(previousSteadyState == LOW && lastSteadyState == HIGH)
		return true;
	else
		return false;
}

void ezButton::setCountMode(int mode) {
	countMode = mode;
}

unsigned long ezButton::getCount(void) {
	return count;
}

void ezButton::resetCount(void) {
	count = 0;
}

void ezButton::loop(void) {
	// read the state of the switch/button:
	int currentState = digitalRead(btnPin);
	unsigned long currentTime = millis();

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch/button changed, due to noise or pressing:
	if (currentState != lastFlickerableState) {
		// reset the debouncing timer
		lastDebounceTime = currentTime;
		// save the the last flickerable state
		lastFlickerableState = currentState;
	}

	if ((currentTime - lastDebounceTime) >= debounceTime) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// save the the steady state
		previousSteadyState = lastSteadyState;
		lastSteadyState = currentState;
	}

	if(previousSteadyState != lastSteadyState){
		if(countMode == COUNT_BOTH)
			count++;
		else if(countMode == COUNT_FALLING){
			if(previousSteadyState == HIGH && lastSteadyState == LOW)
				count++;
		}
		else if(countMode == COUNT_RISING){
			if(previousSteadyState == LOW && lastSteadyState == HIGH)
				count++;
		}
	}
}

//actual code starts

#include <Servo.h>

int pos = 0;

Servo servo_9;

const int pomodoroPin = 7;
const int breakoutPin = 6;

bool isServoOpen = false;

//using ezButton library to create easy button
ezButton pomodoroButton(pomodoroPin);
ezButton breakoutButton(breakoutPin);


void setup()
{
  Serial.begin(9600);
  servo_9.attach(9, 500, 2500);

  //pomodoroButton.setDebounceTime(50);
}

void loop(){
  pomodoroButton.loop();
  breakoutButton.loop();

  if (breakoutButton.isPressed())
    Serial.println("ABORT ABORT ABORT");

  if (pomodoroButton.isPressed()) {

    Serial.println("Button is pressed");

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

    } else {
      for (pos = 90; pos >= 0; pos -= 1) {
        // tell servo to go to position in variable 'pos'
        servo_9.write(pos);
        // wait 15 ms for servo to reach the position
        delay(15); // Wait for 15 millisecond(s)
      }

      isServoOpen = !isServoOpen;
    }

  }

delay(50);
}
