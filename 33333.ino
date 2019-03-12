//Library Stepper Rotation
#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10     // IN3 on the ULN2003 driver 1
#define motorPin4  11     // IN4 on the ULN2003 driver 
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

//Maksimal Jangkauan Slider dalam Step
const int maxRange = 4000;

// Button Pin
const int buttonLeftPin = 5;
const int buttonRightPin = 6;

//0.225 degree / step

// Defines pins numbers step motor 1
const int dirPin = 3;
const int stepPin = 4;

// Relay/Shutter Pin
const int shutterPin = 12;

unsigned long currentMillis;
unsigned long previousMillis = 0;

//char data = 0; //Variable for storing received data

String command;
int speedd;
int delayy;

//80 - 8000
//boolean dir;

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

///////////////////////////////////////////////
//Library Stepper Slider
#include "AccelStepper.h"
AccelStepper stepperX(1, 4, 3);   // 1 = Easy Driver interface
// UNO Pin 2 connected to STEP pin of Easy Driver
// UNO Pin 3 connected to DIR pin of Easy Driver
// Stepper Travel Variables
int move_finished = 1; // Used to check if move is completed
boolean modeA = false;
boolean modeB = false;
boolean modeAB = false;
boolean modeBA = false;
//////////////////////////////////////////////

boolean moveClockwise = true;
boolean stepperA = false;

boolean stateShutt = LOW;

boolean rt = false;

boolean loopMode = false;
int positionn;

boolean modeRotate = false;
boolean flag = false;
boolean modeRotateA = false;
boolean modeRotateB = false;
boolean modeRotateA2 = false;
boolean modeRotateB2 = false;
boolean modeRotate2 = false;
boolean stepperB = false;

boolean modeTimelapseR = false;
boolean modeTimelapseL = false;
boolean modeTimelapse = false;

void stopp() {
  command = "-";
  //  if (stepperA == true && moveClockwise == true) {
  //    moveClockwise = false;
  //    stepperA = false;
  //  } else if (stepperA == true && moveClockwise == false) {
  //    moveClockwise = true;
  //    stepperA = false;
  //  }
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(shutterPin, HIGH);
  modeRotate = false;
  modeRotate2 = false;

  modeRotateA = false;
  modeRotateB = false;

  modeRotateA2 = false;
  modeRotateB2 = false;

  stepperB = false;

  modeTimelapseR = false;
  modeTimelapseL = false;
  modeTimelapse = false;

  stepperA = false;
  rt = false;
  loopMode = false;

  command = "";
  stepperX.stop();
  stepperX.runSpeed();
}

int speedUp(int speedd) {
  int newCustomSlider = map(speedd, 1, 9, 100, 3000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  int newCustomRotation = map(speedd, 1, 9, 40, 100);
  /////////////////////////////////////////////////////////////
  //  Set Max Speed and Acceleration of each Steppers
  stepperX.setMaxSpeed(newCustomSlider);      // Set Max Speed of X axis
  stepperX.setAcceleration(1500);  // Acceleration of X axis
  stepperX.setSpeed(newCustomSlider);
  /////////////////////////////////////////////////////////////
  stepper1.setMaxSpeed(newCustomRotation);
  stepper1.setAcceleration(1500.0);
  stepper1.setSpeed(newCustomRotation);

  return newCustomSlider;
}

int speedUpHyperlapse(int speedd) {
  int newCustomSlider = map(speedd, 1, 9, 50, 150); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  int newCustomRotation = map(speedd, 1, 9, 2, 20);
  /////////////////////////////////////////////////////////////
  //  Set Max Speed and Acceleration of each Steppers
  stepperX.setMaxSpeed(newCustomSlider);      // Set Max Speed of X axis
  stepperX.setAcceleration(1500);  // Acceleration of X axis
  stepperX.setSpeed(newCustomSlider);
  /////////////////////////////////////////////////////////////
  stepper1.setMaxSpeed(newCustomRotation);
  stepper1.setAcceleration(1500.0);
  stepper1.setSpeed(newCustomRotation);

  return newCustomSlider;
}

int stateShutter = HIGH;
int cnt;
int cntTarget = 100;

void shutter(int mode) {
  currentMillis = millis();
  if (mode == 0) {

    for (int i = 0; i < 400; i++) {
      cnt = cnt + 1;
      Serial.println(cnt);
      if (cnt == cntTarget && stateShutter == HIGH) { //&& (currentMillis - previousMillis >= 1000)) {
        digitalWrite(shutterPin, LOW);
        previousMillis = currentMillis;
        stateShutter = LOW;
        Serial.println("1");
        cntTarget = cntTarget + 300;
        Serial.print("cntTarget :");
        Serial.println(cntTarget);

      } else if (cnt == cntTarget && stateShutter == LOW) { //&& (currentMillis - previousMillis >= 1000)) {
        digitalWrite(shutterPin, HIGH);
        stateShutter = HIGH;
        Serial.println("2");
        cntTarget = 100;
        cnt = 0;
        Serial.print("cntTarget :");
        Serial.println(cntTarget);
      }
    }
  } 
}

boolean flagCommand = false;
boolean flagSpeed = false;
boolean flagRotation = false;
boolean flagFrame = false;

int duration;
long durationSecond;
long delayMiliSeconds;
int aShoot;
int totalShoot;
int aStep;
long totalStep;

int rangeStep;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial");
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(shutterPin, OUTPUT);

  digitalWrite(dirPin, HIGH); //Enables the motor to move in a particular direction

  digitalWrite(shutterPin, HIGH);

  stepper1.setMaxSpeed(100.0);
  stepper1.setAcceleration(1500.0);
  stepper1.setSpeed(100);
  /////////////////////////////////////////////////////////////
  //  Set Max Speed and Acceleration of each Steppers
  stepperX.setMaxSpeed(500.0);      // Set Max Speed of X axis
  stepperX.setAcceleration(1500.0);  // Acceleration of X axis
  stepperX.setSpeed(500.0);
  /////////////////////////////////////////////////////////////
}

long currentMil;

void loop() {
  currentMil = millis();

  Serial.println("Loop");
  
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:

    if (inputString.startsWith("@", 0)) {
      Serial.println("SLIDER");

      if (inputString.substring(1, 2)) {
        command = inputString.substring(1, 2);
        Serial.print("Command :");
        Serial.println(command);
      }

      if (inputString.substring(2, 3)) {
        speedd = inputString.substring(2, 3).toInt();
        speedUp(speedd);
        Serial.print("Speed :");
        Serial.println(speedd);
      }

      if (command.equals("A")) {
        Serial.println("==A==");
        stepperX.moveTo(-maxRange);
        modeAB = true;
//        delay(500);
      } else if (command.equals("B")) {
        Serial.println("==B==");
        stepperX.moveTo(maxRange);
        modeBA = true;
//        delay(500);
      } else if (command.equals("Q")) {
        Serial.println("==AB==");
        stepperX.moveTo(maxRange);
        modeAB = true;
        delay(500);
      } else if (command.equals("W")) {
        Serial.println("==BA==");
        stepperX.moveTo(-maxRange);
        modeBA = true;
        delay(500);
      } else if (command.equals("E")) {
        Serial.println("==Rotate Right==");
        moveClockwise = true;
        rt = true;
        delay(500);
      } else if (command.equals("R")) {
        Serial.println("==Rotate Left==");
        moveClockwise = false;
        rt = true;
        delay(500);
      } else if (command.equals("L")) {
        Serial.println("==L==");
        moveClockwise = true;
        positionn = maxRange;
        loopMode = true;
      }

    } else if (inputString.startsWith("#", 0)) {
      Serial.println("SLIDER + ROTATION");

      if (inputString.substring(1, 2)) {
        command = inputString.substring(1, 2);
        Serial.print("Command :");
        Serial.println(command);
      }

      if (inputString.substring(2, 3)) {
        speedd = inputString.substring(2, 3).toInt();
        speedUp(speedd);
        Serial.print("Speed :");
        Serial.println(speedd);
      }

      if (command.equals("A")) {
        Serial.println("==A==");
        moveClockwise = false;
        stepperA = true;
        stepperX.moveTo(-maxRange);
        modeAB = true;
        delay(500);
      } else if (command.equals("B")) {
        Serial.println("==B==");
        moveClockwise = true;
        stepperA = true;
        stepperX.moveTo(maxRange);
        modeBA = true;
        delay(500);
      } else if (command.equals("Q")) {
        Serial.println("==AB==");
        moveClockwise = true;
        stepperA = true;
        stepperX.moveTo(maxRange);
        modeAB = true;
        delay(500);
      } else if (command.equals("W")) {
        Serial.println("==BA==");
        moveClockwise = false;
        stepperA = true;
        stepperX.moveTo(-maxRange);
        modeBA = true;
        delay(500);
      } else if (command.equals("E")) {
        Serial.println("==Rotate Right==");
        moveClockwise = true;
        rt = true;
      } else if (command.equals("R")) {
        Serial.println("==Rotate Left==");
        moveClockwise = false;
        rt = true;
      } else if (command.equals("L")) {
        Serial.println("==L==");
        moveClockwise = true;
        stepperA = true;
        positionn = maxRange;
        loopMode = true;
      }

    } else if (inputString.startsWith("$", 0)) {
      Serial.println("HYPERLAPSE");

      if (inputString.substring(1, 2)) {
        command = inputString.substring(1, 2);
        Serial.print("Command :");
        Serial.println(command);
      }

      if (inputString.substring(2, 4)) {
        duration = inputString.substring(2, 4).toInt();
        Serial.print("Duration :");
        Serial.println(duration);
      }

      if (inputString.substring(4, 5)) {
        delayy = inputString.substring(4, 5).toInt();
        delayMiliSeconds = delayy * 1000;
        Serial.print("Delay :");
        Serial.println(delayy);
      }

      if (inputString.substring(5, 6)) {
        speedd = inputString.substring(5, 6).toInt();
        Serial.print("Speed :");
        Serial.println(speedd);
        speedd = speedUpHyperlapse(speedd);
      }

      durationSecond = duration * 60;
      aShoot = delayy + 2;
      totalShoot = durationSecond /  aShoot;

      aStep = speedd * 2;
      totalStep = aStep * totalShoot;

      Serial.print("Duration :");
      Serial.println(durationSecond);
      Serial.print("Total Shoot :");
      Serial.println(totalShoot);
      Serial.print("Total Step :");
      Serial.println(totalStep);

      //      rangeStep = 1000 / frame;

      if (command.equals("A")) {
        Serial.println("==Goto A Rotate==");
        stepperX.moveTo(maxRange);
        flag = false;
        modeRotateA2 = true;
      } else if (command.equals("B")) {
        Serial.println("==Goto B Rotate==");
        stepperX.moveTo(-maxRange);
        flag = false;
        modeRotateB2 = true;
      } else if (command.equals("Q")) {
        Serial.println("==Goto A==");
        stepperX.moveTo(maxRange);
        flag = false;
        modeRotateA = true;
      } else if (command.equals("W")) {
        Serial.println("==Goto B==");
        stepperX.moveTo(-maxRange);
        flag = false;
        modeRotateB = true;
      } else if (command.equals("T")) {
        Serial.println("==Timelapse R==");
        flag = false;
        modeTimelapseR = true;
      } else if (command.equals("Y")) {
        Serial.println("==TimeLapse L==");
        flag = false;
        modeTimelapseL = true;
      } else if (command.equals("U")) {
        Serial.println("Timelapse");
        flag = false;
        modeTimelapse = true;
      } else if (command.equals("E")) {
        Serial.println("==Rotate Right==");
        moveClockwise = true;
        rt = true;
      } else if (command.equals("R")) {
        Serial.println("==Rotate Left==");
        moveClockwise = false;
        rt = true;
      } else if (command.equals("L")) {
        Serial.println("==L==");
        stepperA = true;
        moveClockwise = true;
        positionn = maxRange;
        loopMode = true;
      }

    } else if (inputString.startsWith("X", 0)) {
      Serial.println("Stop");
      stopp();
      //      customDelayMapped = speedUp(0);
    } else if (inputString.startsWith("S", 0)) {
      Serial.println("Shutter");
      shutter(0);
    } else {
      //      Serial.println("-");
    }

    inputString = "";
    stringComplete = false;
  }


  if (loopMode == true) {
    stepperX.moveTo(positionn);  // Set new move position for X Stepper
    if (stepperX.distanceToGo() != 0) { //|| (stepperZ.distanceToGo() !=0)) {
      stepperX.run();  // Move Stepper X into position
    }
    if ((stepperX.distanceToGo() == 0) && (positionn == maxRange)) { //&& (stepperZ.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      moveClockwise = false;
      positionn = -maxRange;
    } else if ((stepperX.distanceToGo() == 0) && (positionn == -maxRange)) {
      Serial.println("COMPLETED!");
      moveClockwise = true;
      positionn = maxRange;
    }
  }

  if (rt == true) {
    //    stepper.step(moveClockwise);
    if (moveClockwise) {
      stepper1.moveTo(20000);
      stepper1.run();
    } else if (!moveClockwise) {
      stepper1.moveTo(-20000);
      stepper1.run();
    }
  }


  if (modeAB) {
    if (stepperX.distanceToGo() != 0) { //|| (stepperZ.distanceToGo() !=0)) {
      stepperX.run();  // Move Stepper X into position
    }
    if ((move_finished == 0) && (stepperX.distanceToGo() == 0)) { //&& (stepperZ.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      Serial.println("");
      move_finished = 1; // Reset move variable
      modeAB = false;
      moveClockwise = false;
      stepperA = false;
    }
  }

  if (modeBA) {
    if (stepperX.distanceToGo() != 0) { //|| (stepperZ.distanceToGo() !=0)) {
      stepperX.run();  // Move Stepper X into position
    }
    if ((move_finished == 0) && (stepperX.distanceToGo() == 0)) { //&& (stepperZ.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      Serial.println("");
      move_finished = 1; // Reset move variable
      modeBA = false;
      moveClockwise = true;
      stepperA = false;
    }
  }


  if ((stepperA == true) && (moveClockwise == true)) {

    stepper1.moveTo(4000);
    if (stepper1.distanceToGo() == 0) {
      stepper1.moveTo(stepper1.currentPosition());
    }
    stepper1.run();

  } else if ((stepperA == true) && (moveClockwise == false)) {

    stepper1.moveTo(-4000);
    if (stepper1.distanceToGo() == 0) {
      stepper1.moveTo(stepper1.currentPosition());
    }
    stepper1.run();
  }

  if (modeRotate) {
    if (stepperX.distanceToGo() != 0) { //|| (stepperZ.distanceToGo() !=0)) {
      stepperX.run();  // Move Stepper X into position
    }
    if (stepperX.distanceToGo() == 0) { //&& (stepperZ.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      Serial.println("");
      modeRotateA = false;
      modeRotateB = false;

      moveClockwise = false;
      stepperA = false;

      stateShutt = true;
    }
  }

  //10 menit = 600 detik
  //delay kamera = 5 detik
  // 5 + 2 = 7 detik 1 kali shoot
  // 600 / 7 = ada 85 kali shoot

  //stepper running 2 detik = speed * 2 detik
  // 5 * 2 = 10 step
  // 10 * 85 = 850 step

  if (modeRotateA) {
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      //    if (modeAB) {
      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      stepperX.moveTo(maxRange);
      modeRotate = true;
      Serial.println("1");
    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      modeRotate = false;
      Serial.println("2");
    }
  }
  if (modeRotateB) {
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      stepperX.moveTo(-maxRange);
      modeRotate = true;
      Serial.println("1");
    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      modeRotate = false;
      Serial.println("2");
    }
  }



  if (modeRotate2) {
    if (stepperX.distanceToGo() != 0) { //|| (stepperZ.distanceToGo() !=0)) {
      stepperX.run();  // Move Stepper X into position
    }
    if ((move_finished == 0) && (stepperX.distanceToGo() == 0)) { //&& (stepperZ.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      Serial.println("");
      move_finished = 1; // Reset move variable
      modeAB = false;
      moveClockwise = false;
      stepperA = false;

      stateShutt = true;
    }
  }

  //10 menit = 600 detik
  //delay kamera = 5 detik
  // 5 + 2 = 7 detik 1 kali shoot
  // 600 / 7 = ada 85 kali shoot

  //stepper running 2 detik = speed * 2 detik
  // 5 * 2 = 10 step
  // 10 * 85 = 850 step
  //max step = 30000

  //30000 - 850 = 2.150
  //35.0000 - 30.000 = - 5.000



  if (modeRotateA2) {
//    delay(5000);
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      stepperX.moveTo(maxRange);
      modeRotate2 = true;
      stepperB = true;
      moveClockwise = true;
      Serial.println("1");


    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      modeRotate2 = false;
      stepperB = false;
      Serial.println("2");
    }
  }

 
  if (modeRotateB2) {
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      stepperX.moveTo(-maxRange);
      modeRotate2 = true;
      stepperB = true;
      moveClockwise = false;
      Serial.println("1");

    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      modeRotate2 = false;
      stepperB = false;
      Serial.println("2");
    }
  }


  if ((stepperB == true) && (moveClockwise == true)) {
    stepper1.moveTo(2000);
    stepper1.run();

  } else if ((stepperB == true) && (moveClockwise == false)) {
    stepper1.moveTo(-2000);
    stepper1.run();
  }



  if (modeTimelapseR) {
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      stepperB = true;
      moveClockwise = true;
      Serial.println("1");

    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      stepperB = false;
      Serial.println("2");
    }
  }
  if (modeTimelapseL) {
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      stepperB = true;
      moveClockwise = false;
      Serial.println("1");

    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      stepperB = false;
      Serial.println("2");
    }
  }


  if (modeTimelapse) {
    if (flag ==  false && currentMil - previousMillis >= delayMiliSeconds) {

      digitalWrite(shutterPin, HIGH);
      previousMillis = currentMil;
      flag = true;
      Serial.println("1");

    } else if (flag == true && currentMil - previousMillis >= 2000) {
      digitalWrite(shutterPin, LOW);
      previousMillis = currentMil;
      flag = false;
      Serial.println("2");
    }
  }
 
  ////////////////////////////////////////
}



/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '~') {
      move_finished = 0; // Set variable for checking move of the Steppers
      stringComplete = true;
    }
  }
}
