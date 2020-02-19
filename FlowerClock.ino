#include <Time.h>
#include <TimeLib.h>

#include <CapacitiveSensor.h>
/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
//CapacitiveSensor   cs_4_5 = CapacitiveSensor(6,5);        // 10 megohm resistor between pins 5 & 6, pin 5 is sensor pin, add wire, foil
//CapacitiveSensor   cs_4_8 = CapacitiveSensor(7,8);        // 10 megohm resistor between pins 7 & 8, pin 8 is sensor pin, add wire, foil

long sensor1Old;
//long sensor2Old;
//long sensor3Old;

int seconds;
int oldSeconds;
bool touched = false;
bool alarm = false;

int alarmHours = 0;
int alarmMinutes = 0;
int alarmSeconds = 0;

int buttonPin = 3;
int hourButton = 7;
int minuteButton = 6;
int secondButton = 5;
int timeState;

int readingButton;
int readingHours;
int readingMinutes;
int readingSeconds;

int oldReadingHours;
int oldReadingMinutes;
int oldReadingSeconds;

bool alarmOn = false;

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   pinMode(buttonPin, INPUT_PULLUP);
   pinMode(hourButton, INPUT);
   pinMode(minuteButton, INPUT);
   pinMode(secondButton, INPUT);
   Serial.begin(9600);
   setTime(0,0,0,12,2,2020);
}

void loop()                    
{
  //THIS STORES SECONDS IN STATE
    seconds = second();
    
  if (alarmOn) {
    Serial.print("BEEP BEEP BEEP");
    Serial.println();

    //THIS COLLECTS THE INPUT FROM THE TOGGLE SWITCH
    readingButton = digitalRead(buttonPin);

    if (readingButton == 0) {
      alarm = true;
    } else {
      alarm = false;
      alarmOn = false;
      alarmHours = 0;
      alarmMinutes = 0;
      alarmSeconds = 0;
    }

    if (seconds != oldSeconds) {
      Serial.print(hour());
      Serial.print(":");
      Serial.print(minute());
      Serial.print(":");
      Serial.print(second());
      Serial.println();
    }
  } else {
    //THIS STORES SECONDS IN STATE
    seconds = second();
  
    //THIS COLLECTS THE INPUT FROM THE TOGGLE SWITCH
    readingButton = digitalRead(buttonPin);
    
    //THIS COLLECTS THE INPUT FROM THE BUTTONS
    readingHours = digitalRead(hourButton);
    readingMinutes = digitalRead(minuteButton);
    readingSeconds = digitalRead(secondButton);

    //FOR CAP SENSE
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);

    //THIS IS WHETHER THE PLANT IS BEING TOUCHED OR NOT
    if (abs(total1 - sensor1Old) > 10000) {
      touched = true;
    } else {
      touched = false;
    }

    if (readingButton == 0) {
      alarm = true;
    } else {
      alarm = false;
      alarmOn = false;
      alarmHours = 0;
      alarmMinutes = 0;
      alarmSeconds = 0;
    }

    if (alarm) {
      //if the alarm hours equals the hours() and etc
      if (alarmHours == hour() && alarmMinutes == minute() && alarmSeconds == second()) {
        alarmOn = true;
      }
    }

    //THIS CHECKS THAT EACH BUTTON HAS BEEN PRESSED IF THE PLANT IS TOUCHED (alarm version)
    if (alarm && touched) {
      if ((readingHours != oldReadingHours) && (readingHours == 0)) {
        alarmHours += 1;
        Serial.print("Alarm set to: ");
        Serial.print(alarmHours);
        Serial.print(":");
        Serial.print(alarmMinutes);
        Serial.print(":");
        Serial.print(alarmSeconds);
        Serial.println();
        delay(10);
      } else if ((readingMinutes != oldReadingMinutes) && (readingMinutes == 0)) {
        alarmMinutes += 1;
        Serial.print("Alarm set to: ");
        Serial.print(alarmHours);
        Serial.print(":");
        Serial.print(alarmMinutes);
        Serial.print(":");
        Serial.print(alarmSeconds);
        Serial.println();
        delay(10);
      } else if ((readingSeconds != oldReadingSeconds) && (readingSeconds == 0)) {
        alarmSeconds += 1;
        Serial.print("Alarm set to: ");
        Serial.print(alarmHours);
        Serial.print(":");
        Serial.print(alarmMinutes);
        Serial.print(":");
        Serial.print(alarmSeconds);
        Serial.println();
        delay(10);
      }
    } else if (!alarm && touched) {
      if ((readingHours != oldReadingHours) && (readingHours == 0)) {
        adjustTime(3600);
        Serial.print("The time is now set to: ");
        Serial.print(hour());
        Serial.print(":");
        Serial.print(minute());
        Serial.print(":");
        Serial.print(second());
        Serial.println();
        delay(10);
      } else if ((readingMinutes != oldReadingMinutes) && (readingMinutes == 0)) {
        adjustTime(60);
        Serial.print("The time is now set to: ");
        Serial.print(hour());
        Serial.print(":");
        Serial.print(minute());
        Serial.print(":");
        Serial.print(second());
        Serial.println();
        delay(10);
      } else if ((readingSeconds != oldReadingSeconds) && (readingSeconds == 0)) {
        adjustTime(1);
        Serial.print("The time is now set to: ");
        Serial.print(hour());
        Serial.print(":");
        Serial.print(minute());
        Serial.print(":");
        Serial.print(second());
        Serial.println();
        delay(10);
      }
    } else {
      //THIS PRINTS OUT THE TIME EVERY SECOND
      if (seconds != oldSeconds) {
      Serial.print(hour());
      Serial.print(":");
      Serial.print(minute());
      Serial.print(":");
      Serial.print(second());
      Serial.println();
    }
    }

    //THIS PUTS EVERYTHING IN STATE 
    oldReadingHours = readingHours;
    oldReadingMinutes = readingMinutes;
    oldReadingSeconds = readingSeconds;

  }

    //THIS STORES SECONDS AS OLD SECONDS
    oldSeconds = seconds;
    
}
