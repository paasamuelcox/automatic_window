#define blind 10


//VARS

const int numReadings = 10; // number of readings for averaging
const int numReadingsFinal = 10; // number of readings for averaging

const long interval = 30000; // buffer time to stop the blinds going up and down //10 secs

int lowLight = 925; // change to overcast light
int highLight = 950; // change to sunshine

int counter;

/// End of Vars

unsigned long previousMillis = 0;        // will store last time LED was updated
boolean releaseLatch = false;


unsigned long previousMillis2 = 0;        // will store last time LED was updated

boolean blindDown = true;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;               // the average

int ldr_value = 0;        //variable to store LDR values

int inputPin = A0;

void setup() {

  // initialize serial communication with computer:
  Serial.begin(9600);
  // pinmode for the blind
  pinMode(blind, OUTPUT);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // subtract the last reading:

  unsigned long currentMillis = millis();
    unsigned long currentMillis2 = millis();


  if (average < lowLight && releaseLatch && blindDown) { // wind it up
    digitalWrite(blind, HIGH);
    delay(510);
    digitalWrite(blind, LOW);
    releaseLatch = false;
    previousMillis = currentMillis;

    blindDown = false;
    Serial.print("Winding up");
    Serial.println(average);
  }

  if (average > highLight && releaseLatch && !blindDown) { //wind it down
    digitalWrite(blind, HIGH);
    delay(510);
    digitalWrite(blind, LOW);
    releaseLatch = false;
    previousMillis = currentMillis;

    blindDown = true;

    Serial.print("Winding down");
    Serial.println(average);
  }


  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    releaseLatch = true;

  }



  if (currentMillis2 - previousMillis2 >= 1000) { //1000
    
    // save the last time you blinked the LED
    previousMillis2 = currentMillis2;

  total = total - readings[readIndex];
  // read from the sensor:
  
  readings[readIndex] = analogRead(inputPin);
  
  ldr_value = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
   Serial.println(average);
     
  }
}
