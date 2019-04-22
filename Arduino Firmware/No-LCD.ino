#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
#define X_MIN_PIN 3
#define X_MAX_PIN 2

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
#define Y_MIN_PIN 14
#define Y_MAX_PIN 15


#include "AccelStepper.h"

AccelStepper Xstepper(1, X_STEP_PIN, X_DIR_PIN);
AccelStepper Ystepper(1, Y_STEP_PIN, Y_DIR_PIN);

bool runConveyor = false;
int incomingByte = 0;


void setup()
{ 
  Serial.begin(9600);
  Serial.print(78, OCT);
  
  // My way of enabling the enable pin... probably very wrong
  pinMode(X_ENABLE_PIN , OUTPUT);
  digitalWrite(X_ENABLE_PIN , LOW); 

  pinMode(Y_ENABLE_PIN , OUTPUT);
   digitalWrite(Y_ENABLE_PIN , LOW); 

  Xstepper.setMaxSpeed(2000);
  Xstepper.setAcceleration(1000);
  Xstepper.moveTo(-10000000000);
}

void loop()
{ 
  if (Serial.available() > 0) {
    incomingByte = Serial.read(); // read the incoming byte:
    if(incomingByte==116)
    {
      Serial.println("stopping conveyor");
      runConveyor = false;
    }
    
    if(incomingByte==114)
    {
      Serial.println("starting conveyor");
      runConveyor = true;
    }
    
    if(incomingByte==101)
    {
      Serial.println("pushing");
      push();
    }
    
    Serial.print(" I received:");

    Serial.println(incomingByte);
  }
  
  if(runConveyor)
  {
    Xstepper.run();
  }
}

void push()
{
  Ystepper.setMaxSpeed(12000);
  Ystepper.setAcceleration(100000);

  Ystepper.moveTo(500);
  while(Ystepper.distanceToGo() != 0)
  {
  Ystepper.run();

  }

  Ystepper.moveTo(0);
  while(Ystepper.distanceToGo() != 0)
  {
  Ystepper.run();
  }
}
