#include <IRremote.h>

int RECV_PIN = 11; // define input pin on Arduino 
IRrecv irrecv(RECV_PIN); 
decode_results results; // decode_results class is defined in IRremote.h

bool gesloten = false;

void setup() { 
  Serial.begin(9600); 
  irrecv.enableIRIn(); // Start the receiver 
} 

void loop() { 
  autoGesloten();
}

bool autoGesloten()
{
    if (irrecv.decode(&results)) 
    {
      if(gesloten == false)
      {
        gesloten = true;
        Serial.println("Auto gesloten");
      }
      else if(gesloten == true)
      {
        gesloten = false;
        Serial.println("Auto open");
      }
      delay(250);
//    Serial.println(results.value, HEX); 
    irrecv.resume(); // Receive the next value 
    } 
}
