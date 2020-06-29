// Arduino with load cell

// Put two known loads on the sensor and take readings. Put those values
// here.
float analogvalA = 28305;
float loadA = 500.42; // 100 g.
float analogvalB = 59620;
float loadB = 1496.6; // 2000 g.
// calculated by first measuring the analogvalueA and B.
// then place nothing on the load cell, the expected load should be 0 but
// it is not 0. Take the inverse of that load and place it in offset 

float offset=-2.5871047973; 

long time = 0;
int interval = 2000; // Take a reading every 2000 ms
int analogValue =0;
float analogValueAverage = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  analogValue = analogRead(A0);
  
  // running average - We smooth the readings a little bit
  analogValueAverage = 0.99*analogValueAverage + 0.01*analogValue;

  // Is it time to print? 
  if(millis() > time + interval){
    float load = ((loadB - loadA)/(analogvalB - analogvalA)) * (analogValue - analogvalA) + loadA + offset;
  
    Serial.print("analogValue: ");Serial.println(analogValueAverage);
    Serial.print("             load: ");Serial.println(load ,10);
    time = millis();

  }

  }
