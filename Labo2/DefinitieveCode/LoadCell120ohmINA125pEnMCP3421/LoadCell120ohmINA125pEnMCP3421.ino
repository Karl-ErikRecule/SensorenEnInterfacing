// Arduino with load cell
#include <Wire.h>
#include <MCP342X.h>
MCP342X myADC;

// Put two known loads on the sensor and take readings. Put those values
// here.
float analogvalA = 28305;//19298 200.12
float loadA = 500.42; //500.42
float analogvalB = 59620;
float loadB = 1496.6; // 1496.6 59620 OF 28305 500.42

// variables
float offset= 0;
bool offsetBool = false;

static int32_t analogValue;
float iterator = 0;
float meanWeights = 0;
float load = 0;

long time = 0;
int interval = 50; // Take a reading every 50 ms


void setup() {
  Wire.begin();  // join I2C bus
  TWBR = 12;  // 400 kHz (maximum)

 // Open serial connection to send info to the host
  Serial.begin(9600); 
  while (!Serial) {}  // wait for Serial comms to become ready
  Serial.println("Starting up");
  Serial.println("Testing device connection...");
  Serial.println(myADC.testConnection() ? "MCP342X connection successful" : "MCP342X connection failed");
 
  myADC.configure( MCP342X_MODE_CONTINUOUS |
                   MCP342X_CHANNEL_1 |
                   MCP342X_SIZE_18BIT |
                   MCP342X_GAIN_1X
                 );
                 
 // serial connection is setted up and ready
  Serial.println(myADC.getConfigRegShdw(), HEX); 
}

void loop() {
  myADC.startConversion();
  myADC.getResult(&analogValue);

    // time to print?
    // if iterator is 20 and offsetBool is true then give the average measurements of the weights
    // if iterator is 20 and offsetBool is false then give the offset the average measurements of the weights
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
  // if the time difference is bigger then 50 ms interval then measure and calculate the load
  else if (millis() > time + interval) {
    load = ((loadB - loadA) / (analogvalB - analogvalA)) * (analogValue - analogvalA) + loadA;
    // if offsetBool is true then add the load with the meanWeights otherwise add the load
    // to the offset. (this is done in the beginnen of the script)
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
