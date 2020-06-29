int sensorPin = A0; 
int ledPin = 6;
int value;
void setup() {
  pinMode(sensorPin,INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
} 

void loop() {
  value = analogRead(sensorPin);
  dimbaarLicht(value,670,75,ledPin);
  
  Serial.println(value);
  delay(100); 
}

void dimbaarLicht(int input,int maxInput,int ledThreshold,int pin){
  if(input >ledThreshold){
    analogWrite(pin, (input-ledThreshold)/((maxInput-ledThreshold)/255));
  }
  else{
    digitalWrite(pin,LOW);  
  }
}
