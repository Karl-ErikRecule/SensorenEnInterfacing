int analogPin = A0; // KY-028 analog interface
int analogVal; //analog readings

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(tempThreshold(200));
  delay(1000);
}

bool tempThreshold(int threshold)
{
  // Read the analog interface
  analogVal = analogRead(analogPin); 
  if(analogVal >= threshold )
  {
      Serial.println("Boven threshold. Uitgelezen waarde= " + String(analogVal) + " Threshold: " + String(threshold));
      return true;
  }
  else
  {
      Serial.println("Onder threshold. Uitgelezen waarde= " + String(analogVal) + " Threshold: " + String(threshold));
      return false;
  }
}
