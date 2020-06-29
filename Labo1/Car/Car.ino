
// led
int g = 9;
int r = 11;
int b = 10;

// buzzer
int buzzerPassive = 8;

//ultrasonic sensor
int echo = 6;
int trigger = 5;

// defining variables
float distanceUltraSonicSensor;

void setup() {
  // put your setup code here, to run once:

  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(buzzerPassive, OUTPUT);

  pinMode(trigger, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  
}

void loop() {
  // put your main code here, to run repeatedly:


  distanceUltraSonicSensor = ultrasonicSensor();
  
  buzzer(distanceUltraSonicSensor);

  
}

float ultrasonicSensor()
{
  float distance = 0;
  long duration = 0;
  
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
  
}



void led(int green, int red, int blue)
{
  // LED 
  analogWrite(g, green);
  analogWrite(r, red);
  analogWrite(b, blue);

}

void buzzer(float distance)
{

  if (distance < 20)
  {
     led(0,255,0);
     SoundBuzzer(1,200);
  }
  else if ( distance >= 20 && distance < 50)
  {
     led(215,5,255);
     SoundBuzzer(2,300);
     
  }
  else if ( distance >= 50 && distance < 100)
  {
    led(255,0,0);
    SoundBuzzer(3,400);
  }
 
}

void SoundBuzzer(int delayInmiliSecondsBuzzer, int delayAfterBuzzer)
{
   for (int i = 0; i <100; i++) 
    {
      digitalWrite (buzzerPassive, HIGH) ;
      delay (delayInmiliSecondsBuzzer) ;
      digitalWrite (buzzerPassive, LOW) ;
      delay (delayInmiliSecondsBuzzer) ;
    }          
    delay(delayAfterBuzzer);

}
