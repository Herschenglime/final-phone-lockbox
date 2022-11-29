//ezButton library for cleaner button use
//- temporarily dumped so it works in the smulation
#ifndef ezButton_h
#define ezButton_h

#define COUNT_FALLING 0
#define COUNT_RISING  1
#define COUNT_BOTH    2

void printTime(int minutes, int seconds);

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

// C++ code
//
/*
  LiquidCrystal Library - Hello World

   Demonstrates the use of a 16x2 LCD display.
  The LiquidCrystal library works with all LCD
  displays that are compatible with the  Hitachi
  HD44780 driver. There are many of them out
  there, and you  can usually tell them by the
  16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008  by David
  A. Mellis
  library modified 5 Jul 2009  by Limor Fried
  (http://www.ladyada.net)
  example added 9 Jul 2009  by Tom Igoe
  modified 22 Nov 2010  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

#include <LiquidCrystal.h>
// #include <ezButton.h> //uncomment in actual code

//servo functionality
#include <Servo.h>
int pos = 0;
Servo servo_9;
bool isServoOpen = false;


//button functionality
const int pomodoroPin = 7;
const int breakoutPin = 8;

//using ezButton library to abstract button logic
ezButton pomodoroButton(pomodoroPin);
ezButton breakoutButton(breakoutPin);


//timer functionality
int minutes = 25;
int seconds = 0;
int tempMin = 0;
int tempSec = 0;
int count = 0;

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup()
{
  Serial.begin(9600); //open serial for debugging
  //servo_9.attach(9, 500, 2500); //initialize servo and set range of motion
  servo_9.attach(9); //initialize servo and set range of motion

  //uncomment below on actual hardware
  //pomodoroButton.setDebounceTime(50);
  //breakoutButton.setDebounceTime(50);


  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD.
  pinMode(6, INPUT);
  // Print a message to the LCD.
  //lcd_1.print("hello world!"); example of how to print
}

void loop(){
  pomodoroButton.loop();
  breakoutButton.loop();

/*
  tempMin = minutes; //stores the initial minutes
  tempSec = seconds; //stores the initial seconds
  count++; //increments count for every time it loops through
*/

  if (pomodoroButton.isPressed())
    Serial.println("Pomodoro button pressed");

   if (breakoutButton.isPressed())
  	Serial.println("ABORT ABORT ABORT");


  delay(50);

  /*
  //until button is pressed this will cause the timer to flash
  while(true){
    pomodoroButton.loop();
    breakoutButton.loop();


    printTime(minutes, seconds);

    delay(750); //pauses to show the minutes

    lcd_1.clear(); //clears the screen to black

    delay(750); //pauses to show black screen

      //stand-in emergency releease funcitonality
    if (breakoutButton.isPressed())
    Serial.println("ABORT ABORT ABORT");

    if (pomodoroButton.isPressed())
      Serial.println("Pomodoro button pressed");

    if(pomodoroButton.isPressed())
      break;
  }

  while(minutes + seconds > 0){ //as long as the time is not 0,counts down timer

    printTime(minutes, seconds);

  	delay(1000); // Wait for 1000 milliseconds

  	if(seconds == 0){ //resets the seconds to 59 if it hits 0
    	seconds = 59;
		minutes -= 1;
  	}
  	else
  		seconds -= 1;
  }
  	if(tempMin == 25) //if the inital minutes were 25, switches to 5 for the short break
    	minutes = 5;
  	else //if the initial minutes were not 25, it changes it to 25 for the reset time
    	minutes = 25;
  	if (count % 5 == 0) //on the fifth iteration, it becomes a long break
      minutes = 15;
}

void printTime(int minutes, int seconds){
  lcd_1.setCursor(0, 0); //sets initial position

  	lcd_1.print(minutes); //prints minutes

  	lcd_1.print(':');

  	if(seconds < 10) //prints a zero in the tens place if < 10
    	lcd_1.print('0');

  	lcd_1.print(seconds); //prints seconds
  */
}
