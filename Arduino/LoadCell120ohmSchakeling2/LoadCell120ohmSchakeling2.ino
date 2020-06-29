#include <Wire.h>
#include <MCP342X.h>
MCP342X myADC;
int outputPin = 6;

// Arduino with load cell

// Put two known loads on the sensor and take readings. Put those values
// here.
float analogvalA = 28305;//19298 200.12
float loadA = 500.42; //200.12 g.
float analogvalB = 59620;
float loadB = 1496.6; // 1496.6 59620 28305 500.42
float offset= 0;
bool offsetBool = false;

static int32_t analogValue;
float iterator = 0;
float meanWeights = 0;
float load = 0;

long time = 0;
int interval = 50; // Take a reading every 100 ms


void setup() {
  // put your setup code here, to run once:
  Wire.begin();  // join I2C bus
  TWBR = 12;  // 400 kHz (maximum)
 
  Serial.begin(9600); // Open serial connection to send info to the host
  while (!Serial) {}  // wait for Serial comms to become ready
  Serial.println("Starting up");
  Serial.println("Testing device connection...");
  Serial.println(myADC.testConnection() ? "MCP342X connection successful" : "MCP342X connection failed");
 
  myADC.configure( MCP342X_MODE_CONTINUOUS |
                   MCP342X_CHANNEL_1 |
                   MCP342X_SIZE_18BIT |
                   MCP342X_GAIN_1X
                 );
 
  Serial.println(myADC.getConfigRegShdw(), HEX);
}

void loop() {
  myADC.startConversion();
  myADC.getResult(&analogValue);

    // Is it time to print?
  if(iterator == 20){
    if(offsetBool == true)
    {
      meanWeights = meanWeights / 20;
      Serial.print("analogValue: "); Serial.println(analogValue,10);
      Serial.print("             load: "); Serial.println(meanWeights + offset, 10);
      iterator = 0;
      meanWeights = 0;
    }
    else
    {
      offset = -1 * offset/20;
      Serial.print("Offset: "); Serial.println(offset,10);
      iterator = 0;
      offsetBool = true;
    }

  }
  
  else if (millis() > time + interval) {
    load = ((loadB - loadA) / (analogvalB - analogvalA)) * (analogValue - analogvalA) + loadA;

    if(offsetBool == true)
    {
      meanWeights = load + meanWeights;
    }
    else
    {
      offset = load + offset;
    }

    iterator = iterator +1;
    time = millis();

  }

  

  }
