// Arduino with load cell
#include  <Wire.h>
#include  <MCP342X.h>
MCP342X myADC;
int outputPin = 6;


// Put two known loads on the sensor and take readings. Put those values
// here.
float analogvalA = 114;
float loadA = 200; // 100 g.
float analogvalB = 529;
float loadB = 2000; // 2000 g.
float offset=-26.5180664062;


long time = 0;
int interval = 500; // Take a reading every 500 ms

float analogValueAverage = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin();  // join I2C bus
  TWBR = 12;  // 400 kHz (maximum)
 
  while (!Serial) {}  // wait for Serial comms to become ready
  Serial.println("Starting up");
  Serial.println("Testing device connection...");
  Serial.println(myADC.testConnection() ? "MCP342X connection successful" : "MCP342X connection failed");
 
  myADC.configure( MCP342X_MODE_CONTINUOUS |
                   MCP342X_CHANNEL_1 |
                   MCP342X_SIZE_18BIT |
                   MCP342X_GAIN_4X
                 );
 
  Serial.println(myADC.getConfigRegShdw(), HEX);
}

void loop() {
  static int32_t  analogValue;
  myADC.startConversion();
  myADC.getResult(&analogValue);
//  Serial.println("analogVal: " + String(analogValue));
  
  // running average - We smooth the readings a little bit
 // analogValueAverage = 0.99*analogValueAverage + 0.01*analogValue;

  // Is it time to print? 
  if(millis() > time + interval){
    float load = ((loadB - loadA)/(analogvalB - analogvalA)) * (analogValue - analogvalA) + loadA + offset;
  
    Serial.print("analogValue: ");Serial.println(analogValue);
    Serial.print("             load: ");Serial.println(load ,10);
    time = millis();

  }

  }


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
