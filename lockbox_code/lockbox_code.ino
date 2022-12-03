//both of the following libraries must be installed thru arduino editor

//lcd code adapted from http://www.arduino.cc/en/Tutorial/LiquidCrystal
#include <LiquidCrystal.h>
//external library for button handling: https://arduinogetstarted.com/tutorials/arduino-button-library
#include <ezButton.h> 
//servo functionality
#include <Servo.h>
int pos = 0;
Servo servo_9;
bool isServoOpen = false;


//button functionality
const int pomodoroPin = 7;
const int breakoutPin = 8;
const int piezo = 6;

//using ezButton library to abstract button logic
ezButton pomodoroButton(pomodoroPin);
ezButton breakoutButton(breakoutPin);



//timer functionality
const int workDuration = 25; //minutes, used for debugging
const int breakDuration = 5;
const int longBreakDuration = 15;


int minutes = workDuration;
int seconds = 0;
int tempMin = 0;
int tempSec = 0;
int count = 0;

//booleans
bool isPause = true;
bool isBreak = false;
bool eStop = false;

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup()
{
  //Serial.begin(9600); //open serial for debugging
  servo_9.attach(9, 500, 2500); //initialize servo and set range of motion
  servo_9.attach(9); //initialize servo and set range of motion

  //uncomment below on actual hardware
  //pomodoroButton.setDebounceTime(50);
  //breakoutButton.setDebounceTime(50);


  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD.
  // Print a message to the LCD.
  //lcd_1.print("hello world!"); example of how to print
  
  //https://www.instructables.com/How-to-use-a-Buzzer-Arduino-Tutorial/
  //set pinMode for piezo
  pinMode(piezo, OUTPUT);

}

void loop(){

  tempMin = minutes; //stores the initial minutes
  tempSec = seconds; //stores the initial seconds
  count++; //increments count for every time it loops through

  
  //until button is pressed this will cause the timer to flash
  if(isPause){
    servo_9.write(0);
    printTime(minutes, seconds);

    responsiveDelay(750); //pauses to show the minutes

    lcd_1.clear(); //clears the screen to black

    responsiveDelay(750); //pauses to show black screen
 
  } else { //counts down timer
    servo_9.write(90);
    
    if(minutes + seconds == 0) { //checks if time ran out
      if(!isBreak)
        breakTune();
      else
        workTune();
    
     
     isBreak = !isBreak; //changes break state
     isPause = true; //pauses and begins blinking
     if(tempMin == workDuration) //if the inital minutes were 25, switches to 5 for the short break
      minutes = breakDuration;
    else //if the initial minutes were not 25, it changes it to 25 for the reset time
      minutes = workDuration;
     if (count == 9){ //on the fifth iteration, it becomes a long break
        minutes = longBreakDuration;
        count = -1;
     }
    }
      

    printTime(minutes, seconds);

    responsiveDelay(1000); // Wait for 1000 milliseconds

    if(seconds == 0){ //resets the seconds to 59 if it hits 0
      seconds = 59;
    minutes -= 1;
    }
    else
      seconds -= 1;
    }
  
}

void printTime(int minutes, int seconds){
  lcd_1.setCursor(0, 0); //sets initial position

  lcd_1.print(minutes); //prints minutes

  lcd_1.print(':');

  if(seconds < 10) //prints a zero in the tens place if < 10
    lcd_1.print('0');

  lcd_1.print(seconds); //prints seconds
  
}

//https://forum.arduino.cc/t/button-and-delay-in-loop/42429/3

//function that allows for button input while a delay runs
void responsiveDelay(unsigned long duration)
{  
  unsigned long start = millis();

  while (millis() - start <= duration) {
    handleButtons();  // check the buttons 
  }
}


//handle button presses during break
void handleButtons()
{
  pomodoroButton.loop();
  breakoutButton.loop();  

  if (pomodoroButton.isPressed()){
    Serial.println("Pomodoro button pressed");

    if (isPause){
      Serial.println("Unpausing");
      isPause = !isPause;
    }
  }
  
  if (breakoutButton.isPressed()) {
    eStopTune();
    isPause = true;
    isBreak = false;
    count = 0;
    minutes = 25;
    seconds = 0;
    Serial.println("Emergency Stop Initiated!");
  }
    
  //for tinkercad only to make simulation not be really really slow
  delay(50);
}

void breakTune() {
          //play break starting tune (no input possible during this time)
        tone(piezo, 440, 1000);
        delay(50);
        tone(piezo, 587, 1000);
        delay(50);
        tone(piezo, 660, 1000);
        noTone(piezo);
}

void workTune() {
        //play break ending tune (no input possible during this time)
        tone(piezo, 660, 1000);
        delay(50);
        tone(piezo, 587, 1000);
        delay(50);
        tone(piezo, 440, 1000);
        noTone(piezo);
}


void eStopTune() {
        //play break ending tune (no input possible during this time)
        tone(piezo, 660, 1000);
          delay(250);
      noTone(piezo);

  
        tone(piezo, 660, 1000);
          delay(250);
      noTone(piezo);

  
        tone(piezo, 660, 1000);
          delay(250);
      noTone(piezo);
}
