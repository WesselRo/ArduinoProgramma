const int buttonPin = 7;
const int ledBusy = 12;
const int ledDone = 13;
const int ledWaiting = 8;
const int trigPin = 4;  
const int echoPin = 2; 

float duration, distance;  

int buttonState = 0;

bool waiting = false;

int neededDistance = 5;

void setup() {
  pinMode(ledBusy, OUTPUT);
  pinMode(ledDone, OUTPUT);
  pinMode(ledWaiting, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
	Serial.begin(9600);  
}


void loop() {
  // bereken afstand met ultrasoon
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;  

  Serial.print("Distance: ");  
	Serial.println(distance); 

  buttonState = digitalRead(buttonPin);
 
  // Als de PLC (knop) een signaal stuurt en checkt hij of er een frikandel ligt
  // Voorraad? roept hij de methode dropSnack() aan
  // Geen voorraad? wacht hij tot er een frikandel is voor hij dropSnack() aanroept
  if (buttonState == HIGH){
    if (distance < neededDistance){
      dropSnack();
      waiting = false;
    } else {
      waiting = true;
      digitalWrite(ledWaiting, HIGH);
    }
  }

  if (waiting && distance < neededDistance){
    digitalWrite(ledWaiting, LOW);
    dropSnack();
    waiting = false;
  }

  // kleine delay voor een goede werking van de ultrasoon sensor
  delay(100);
}

void dropSnack(){
  // stappenmotor 180 graden draaien
  digitalWrite(ledBusy, HIGH);
  delay (2000);
  digitalWrite(ledBusy, LOW);

  //doorgeven dat er een snack is gedropt
  digitalWrite(ledDone, HIGH);
  delay(500);
  digitalWrite(ledDone, LOW);
}