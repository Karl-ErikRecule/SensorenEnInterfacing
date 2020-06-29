// Arduino with load cell
#include  <Wire.h>
#include  <MCP342X.h>
MCP342X myADC;


// Put two known loads on the sensor and take readings. Put those values
// here.
float analogvalA = 196;
float loadA = 500.42; // 500.42 g.
float analogvalB = 429;
float loadB = 1496.6; // 1496.6 g.
float offset = -0.1923217773;


long time = 0;
int interval = 500; // Take a reading every 500 ms
float load = 0;
static int32_t  analogValue;


void setup() {
  
  Serial.begin(9600);

  Wire.begin();  // join I2C bus
  TWBR = 12;  // 400 kHz (maximum)

  // wait for Serial comms to become ready
  while (!Serial) {}  
  Serial.println("Starting up");
  Serial.println("Testing device connection...");
  Serial.println(myADC.testConnection() ? "MCP342X connection successful" : "MCP342X connection failed");

  myADC.configure( MCP342X_MODE_CONTINUOUS |
                   MCP342X_CHANNEL_1 |
                   MCP342X_SIZE_18BIT |
                   MCP342X_GAIN_4X
                 );

  Serial.println(myADC.getConfigRegShdw(), HEX); // Serial comms is ready
}

void loop() {
  myADC.startConversion();
  myADC.getResult(&analogValue);


  // time to print?
  if (millis() > time + interval) {
    load = ((loadB - loadA) / (analogvalB - analogvalA)) * (analogValue - analogvalA) + loadA + offset;

    Serial.print("analogValue: "); Serial.println(analogValue);
    Serial.print("             load: "); Serial.println(load , 10);
    time = millis();

  }

}
