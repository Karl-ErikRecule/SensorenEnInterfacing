int redpin = 6; // select the pin for the red LED
int greenpin = 5; // select the pin for the green LED
int val;
int pin1=redpin;
int pin2=greenpin;
int temp;
void setup () {
   pinMode (redpin, OUTPUT);
   pinMode (greenpin, OUTPUT);
}
void loop () {
   digitalWrite(pin1,LOW);
   digitalWrite(pin2,HIGH);
   temp = pin1;
   pin1 = pin2;
   pin2 = temp;
   delay(500);
   
}
