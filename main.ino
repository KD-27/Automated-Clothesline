#include <NewPing.h>

// ultrasonic initiative
#define TRIGGER_PIN  12  
#define ECHO_PIN     11  
#define MAX_DISTANCE 200
int ultra_threshold_1 = 9;
int ultra_threshold_2 = 10;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int distance;

// LDR initiative
int const LDRpin = A1;
int value = 0;

// Bluetooth initiative
char blue;

// Rain sensor initiatives
int const sensorPin = A0;

// Motor initiative
int int1 = 7;
int int2 = 6;

// Switch initiative
const byte switchPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(LDRpin, INPUT);
  pinMode(int1, OUTPUT);
  pinMode(int2, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);

  // Setting the required speed for the motor
  pinMode(3, OUTPUT);
  analogWrite(3,100);
  
}

void loop() {
  if (Serial.available()){
    
    blue = Serial.read();
    Serial.println(blue);
    
    }
    
  byte switchState = digitalRead(switchPin);
  
  value = analogRead(LDRpin);
  unsigned int uS = sonar.ping();
  distance = sonar.convert_cm(uS);
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
  
  Serial.print(value);
  Serial.print('\t');
  Serial.println(rainSensor());
  Serial.println("--------------------");
  delay(100);
  
  if (blue == '0'&& switchState == HIGH ){
    Serial.println("System on");
    if ( value < 500 || rainSensor() > 10){ // Lesser light intensity or presence of rain
        ClothsMovingToTheShelter();
    }
    else if (value > 500 && rainSensor() < 10){ // Higher light intensity and absence of rain
        ClothsMovingAwayFromTheShelter();
    }
  }
  else if (blue == '1' || switchState == LOW){
    Serial.println("System off");
    ClothsMovingToTheShelter();
    }
}

// Actuator motion function 1
void ClothsMovingToTheShelter(){
  // checking whether the last cloth has reached the minimum value 
  if(distance > ultra_threshold_1){
    digitalWrite(int1, HIGH);
    digitalWrite(int2, LOW);
    delay(100);
    Serial.println("-------> Moving closer to the shelter");
  }
  else{
    digitalWrite(int1, LOW);
    digitalWrite(int2, LOW);
    delay(100);
  }  
}

// Actuator motion function 2
void ClothsMovingAwayFromTheShelter(){
  // checking whether the last cloth has reached the maximum value
  if (distance < ultra_threshold_2){
    digitalWrite(int1, LOW);
    digitalWrite(int2, HIGH);
    delay(100);
    Serial.println("<------- Moving away from the shelter");
  }
  else{
    digitalWrite(int1, LOW);
    digitalWrite(int2, LOW);
    delay(100);
  }
}

// Rain sensor function
int rainSensor() {
  int sensorValue = analogRead(sensorPin);  
  int outputValue = map(sensorValue, 0, 1023, 255, 0);
  return outputValue; 
}
