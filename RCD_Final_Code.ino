int currentin = 0;
int currentout = 0;
long currentinmean = 0;
long currentoutmean = 0;
int counter = 0;
int counterled = 0;
boolean toggle = false;
int trip = 0;
int threshold = 3; //This value can be changed if you find the RCD doesnt trip when it should or trips too often
#define current1 A0
#define current2 A2
#define resetled 3
#define reset 7
#define test 5
#define coil 6
void setup() {
  // put your setup code here, to run once:
  pinMode (coil, OUTPUT);
  pinMode (currentin, INPUT);
  pinMode (currentout, INPUT);
  pinMode (reset, INPUT_PULLUP);
  pinMode (test, INPUT_PULLUP);
  pinMode (resetled, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin (9600);
}

void loop() {

  if (trip == 0) {
      counterled = 0;
      toggle = false;
    digitalWrite(resetled, LOW);
  } // switches off rest LED when circuit is reset

  if (trip == 1) { // this code flashes the reset LED
      if (counterled < 250 && toggle == false) {
      counterled = counterled + 5;
      }
      if (toggle == true) {
      counterled = counterled - 10;
      }
      if (counterled == 250 || counterled == 0) {
      toggle = !toggle;
      }
      analogWrite(resetled, counterled);
      }
// This section of code calculates the mean current in and the out of the circuit as the value fluctuates due to lumpy dc
  currentinmean = 0;
  currentoutmean = 0;
  for (int i = 0; i < 100 ; i++) {
    currentin = analogRead (current1);
    currentout = analogRead (current2);
    currentinmean = currentinmean + currentin;
    currentoutmean = currentoutmean + currentout;
     }
  currentinmean = currentinmean / 100;
  currentoutmean = currentoutmean / 100;

//This section of code is used in testing to make sure the values change. The counter value means it doesnt report each run through the code
if (counter == 50) {
    Serial.print("Current in = ");
    Serial.println(currentinmean);
    Serial.print("Current Out = ");
    Serial.println(currentoutmean);
    counter = 0;
  }
 // This section is for the test button which shows the relay is working
  if (digitalRead(test) == LOW) {
    digitalWrite (coil, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("test");
    toggle = false;
    counterled = 0;
    trip = 1;
  }

//This is where the RCD trips if the current in is different to the current out. Threshold can be changed if you find the circuit is too sensitive or not sensitive enough
  if (currentoutmean > currentinmean + threshold || currentoutmean < currentinmean - threshold && trip == 0) {
    digitalWrite (coil, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    trip = 1;
  } 

//This section of code trips the coil if the current into or out of the device drops below a certain level just in case the fault causes a voltage drop in the sensors, reducing their function
  if (currentoutmean < 150 ||  currentinmean < 150 || currentoutmean > 950 ||  currentinmean > 950 & trip == 0) {
    digitalWrite (coil, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    trip = 1;
  }
  //Serial.println(digitalRead(2));

// This is the reset code 
  if (digitalRead (reset) == LOW)
  { digitalWrite (coil, LOW);
  digitalWrite(LED_BUILTIN, LOW);
    trip = 0;
  }
 counter++;
}
