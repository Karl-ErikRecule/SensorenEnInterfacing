// library to communicate with the LCD screen
#include <LiquidCrystal.h>

// initialize the LCD screen library with the numbers of the interface pins: LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String textLijn2 = "";

//initialisation of the menu
String menu[] = {"Verwarming:", "Airco:", "Motor overheat:", "Status auto:", "AutoLichten:", "RuitenWissers:", "ParkeerSensor:"};
int MenuItem = 0;

//Rotary encoder
#define outputA 10
#define outputB 9
#define drukknop 8

int buttonState = 0;
int encoderposPrevious = 0;
int encoderpos = 0;
int aState=LOW;
int aLastState=LOW;

//Temperature (sensor 20)
int anPinTemp = A0;
int tempAnalog;

//DHT
#define DHT11_PIN 7
#include <dht.h>
dht DHT;

//IR receiver
#include <IRremote.h>
int RECV_PIN = 22;
IRrecv irrecv(RECV_PIN); 
decode_results results; // decode_results class is defined in IRremote.h

//LED rood-groen
int redpin = 31; // select the pin for the red LED
int greenpin = 33; // select the pin for the green LED

//Photoresistor
int photoSensorPin = A2;

//buzzer
int buzzerPassive = 43;

//ultrasonic sensor
int echo = 36;
int trigger = 37;

//water sensor
int watersensor = A4;

// servo
#include <Servo.h> 
Servo myservo;
int servoPin = 6;
int value;
int pos = 0; 

//Global variables of all sensors
bool temperatuurThreshold = false;;
bool heating = false;
bool airconditioning = false;
bool gesloten = false;
bool statusLED = false;
float distanceUltraSonicSensor = 0.0;
bool waterSensor = false;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // rotatary encoder
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(drukknop, INPUT_PULLUP);
  aLastState = digitalRead(outputA);

  //Led:
  pinMode (redpin, OUTPUT);
  pinMode (greenpin, OUTPUT);
  
  // serial monitor
  Serial.begin(9600);
  
  // IR receiver
  irrecv.enableIRIn(); // Start the receiver

  // buzzer
  pinMode(buzzerPassive, OUTPUT);

  //ultrasonic sensor
  pinMode(trigger, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo, INPUT); // Sets the echoPin as an Input

  //watersensor
  pinMode(watersensor, INPUT);
  int waterSensorValue = 0;

  //servo
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object 
}

void loop() {
  // checks if the stick of the rotary encoder has turned 
  aState = digitalRead(outputA);
  if((aLastState == LOW) && (aState == HIGH)){
    if(digitalRead(outputB) == LOW){
      encoderpos--;
    }
    else
    {
      encoderpos++;
    }
    Serial.println("De encoder");
    Serial.println(encoderpos);
    Serial.println("De previous encoder");
    Serial.println(encoderposPrevious);

    // update the LCDScreen
    LCDScreen(encoderpos);
    delay(200);
    
    encoderposPrevious = encoderpos;
  }
  // update the LCDScreen
  LCDScreen(encoderpos);
  aLastState = aState;

  // checks if the car has been opened or closed
  autoGesloten();

  // checks if the night falls or it is daylight => night led green, day led red
  ledPhotoResistorRoodGroen(300);
}

//function to update the LCD
void LCDScreen(int MenuItem)
{
  // checks if the position 
  if(encoderpos > 6)
  {
    encoderpos = 0;
    encoderposPrevious = 0;
    MenuItem = 0;
  }
  else if(encoderpos < 0)
  {
    encoderpos = 6;
    encoderposPrevious = 6;
    MenuItem = 6;
  }
  switch (MenuItem) {
    case 0: //check heating
      textLijn2 = "Klik om te meten";
      if(digitalRead(drukknop)==LOW)
      {
        textLijn2= "Meting bezig...";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(menu[MenuItem]);
        lcd.setCursor(0, 1);
        lcd.print(textLijn2);
        verwarming(24,1);
      }
      if(heating)
        {
          menu[0] = "Verwarming: AAN";
        }
        else
        {
          menu[0] = "Verwarming: UIT";
        }
      
      break;
    case 1: //Check airco
      textLijn2 = "Klik om te meten";
      if(digitalRead(drukknop)==LOW)
      {
        textLijn2= "Meting bezig...";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(menu[MenuItem]);
        lcd.setCursor(0, 1);
        lcd.print(textLijn2);
        airco(60,5);
      }
      if(airconditioning)
        {
          menu[1] = "Airco: AAN";
        }
        else
        {
          menu[1] = "Airco: UIT";
        }
      break;
    case 2: //Check motor overheat
      tempThreshold(200);
      if(temperatuurThreshold)
      {
        textLijn2 = "Motortemp OK";
      }
      else
      {
        textLijn2 = "Motortemp ALARM!";
      }
    break;
    case 3: //Check car locked or not
      if(gesloten)
      {
        textLijn2 = "Auto Gesloten";
      }
      else
      {
        textLijn2 = "Auto Open";
      }
      break;
    case 4: //Check if lights need to be turned on
      if(statusLED)
      {
        textLijn2 = "Lampen Aan.";
      }
      else
      {
        textLijn2 = "Lampen Uit.";
      }
      break;
    case 5: //Check if its raining to enable the windscreenwipers.
      watersensorfunction();
      if(waterSensor)
      {
        textLijn2 = "Wissers Aan.";
      }
      else
      {
        textLijn2 = "Wissers Uit.";
      }
      break;
    case 6:  //Check parking sensors.
      ultrasonicSensor();
      textLijn2 = parkeerSensor(distanceUltraSonicSensor);
      break;
    default:
      // statements
      break;
    
  }
  Serial.println("De menuItem");
  Serial.println(MenuItem);

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(menu[MenuItem]);
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(textLijn2);

}

//Temperatuursensor (sensor 20) die registratie 
void tempThreshold(int threshold)
{
  // Read the analog interface
  tempAnalog = analogRead(anPinTemp); 
  if(tempAnalog >= threshold )
  {
      Serial.println("Boven threshold. Uitgelezen waarde= " + String(tempAnalog) + " Threshold: " + String(threshold));
      temperatuurThreshold = true;
  }
  else
  {
      Serial.println("Onder threshold. Uitgelezen waarde= " + String(tempAnalog) + " Threshold: " + String(threshold));
      temperatuurThreshold = false;
  }
}

//Methode gebruikt voor de verwarming.
float temperatuur()
{
    int chk = DHT.read11(DHT11_PIN);
    float temp = DHT.temperature;
    delay(2000);
    return temp;
}

//Methode gebruikt voor de airco.
float humidity()
{
    int chk = DHT.read11(DHT11_PIN);
    float humidity = DHT.humidity;
    delay(2500);
    return humidity;
}

//Methode voor de verwarming
void verwarming(int thresholdTemp, int hysterese)
{
  int chk = DHT.read11(DHT11_PIN);
  delay(2500);
  float temp = temperatuur();
//  Serial.println("Gemeten temperatuur: " + String(temp));
  if(temp >= (thresholdTemp + hysterese))
  {
    heating = false;
  }
  else if(temp <= (thresholdTemp - hysterese))
  {
    heating = true;
  }
}

//Methode voor de airco
void airco(int thresholdHumidity, int hysterese)
{
  int chk = DHT.read11(DHT11_PIN);
  float hum = humidity();
  delay(2500);
//  Serial.println("Gemeten humidity: " + String(hum));
  if(hum >= (thresholdHumidity + hysterese))
  {
    airconditioning = true;
  }
  else if(hum <= (thresholdHumidity - hysterese))
  {
    airconditioning = false;
  }
}

//Methode voor het openen en sluiten van de auto
bool autoGesloten()
{
  bool flagChanged = false;
    if (irrecv.decode(&results)) 
    {
      if(gesloten == false)
      {
        gesloten = true;
        flagChanged = true;
        Serial.println("Auto gesloten");
      }
      else if(gesloten == true)
      {
        gesloten = false;
        flagChanged = true;
        Serial.println("Auto open");
      }
      delay(250);
//    Serial.println(results.value, HEX); 
      irrecv.resume(); // Receive the next value 
      if(flagChanged)
      {
        if(gesloten)
        {
          textLijn2 = "Auto Gesloten";
          SoundBuzzer(1,150,50);
          SoundBuzzer(1,150,50);
        }
        else
        {
          textLijn2 = "Auto Open";
          SoundBuzzer(1,150,50);
          SoundBuzzer(1,150,50);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Status auto:" );
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        lcd.print(textLijn2);
        delay(2000);
        LCDScreen(encoderpos);
      }
    } 
}

//Methode om de LED aan te sturen, aan hand van photoresistor
void ledPhotoResistorRoodGroen(int thresHold){
  int input = analogRead(photoSensorPin);
  //Serial.println("photoresistor: " + String(input));
  if(input > thresHold){
    digitalWrite(redpin, LOW);
    digitalWrite(greenpin, HIGH);
    statusLED = true;
  }
  else{
    digitalWrite(redpin, HIGH);
    digitalWrite(greenpin, LOW); 
    statusLED = false;
  }
}

// ultrasonic sensor code
void ultrasonicSensor()
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

  distanceUltraSonicSensor = distance;
}

void SoundBuzzer(int delayInmiliSecondsBuzzer, int delayAfterBuzzer, int lengthSound)
{
   for (int i = 0; i <lengthSound; i++) 
    {
      digitalWrite (buzzerPassive, HIGH) ;
      delay (delayInmiliSecondsBuzzer) ;
      digitalWrite (buzzerPassive, LOW) ;
      delay (delayInmiliSecondsBuzzer) ;
    }          
    delay(delayAfterBuzzer);
}

String parkeerSensor(float distanceObject)
{
  String textReturn= "Everything fine";
  if (distanceObject < 20)
  {
     //led(0,255,0);
     SoundBuzzer(1,200,100);
     textReturn = "Less then 20cm";
  }
  else if ( distanceObject >= 20 && distanceObject < 50)
  {
     //led(215,5,255);
     SoundBuzzer(2,300,100);
     textReturn = "between 20-50cm";
  }
  else if ( distanceObject >= 50 && distanceObject < 100)
  {
    //led(255,0,0);
    SoundBuzzer(3,400,100);
    textReturn = "between 50-100cm";
  }
  return textReturn;
}

void watersensorfunction()
{
  int waterSensorValue = analogRead(watersensor);
  Serial.println("The watersensorvalue is ");
  Serial.println(waterSensorValue); 
  if(waterSensorValue > 50)
  {
   // Make servo go to 0 degrees 
   waterSensor = true;
   myservo.write(0); 
   delay(1000); 
   // Make servo go to 180 degrees 
   myservo.write(50); 
   delay(1000); 
   Serial.println("the servo is turning");
  }
  else
  {
    waterSensor = false;
  }
}
