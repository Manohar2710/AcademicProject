const int xpin = A0;  // x-axis of the accelerometer
const int ypin = A1; // y-axis
const int ledPin = 13;
const int ldrPin = A2;
boolean rms = false;
const int gasPin = A3; //GAS sensor output pin to Arduino analog A0 pin
int buzzer = 10;
int sensorThres = 300;
int period = 1000;
unsigned long time = 0;
int m;
#include <dht.h>
dht DHT;
#define DHT11_PIN 7

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(gasPin, INPUT);
  Serial.begin(9600);         // initialize the serial communications
}
void loop()
{
  rms = false;
  if (rms == false)
  {
    temp();
  }
  for (int i = 0; i < 2; i++)
  {
    ldr();
    acc();
  }
  int analogSensor = analogRead(gasPin);            // Checks if it has reached the threshold value
  if (millis() > time + period)
  {
    if (analogSensor > sensorThres)
    {
      // Serial.println(analogSensor);
      tone(buzzer, 3000);
    }
    else
    {
      noTone(buzzer);
    }
    time = millis();
  }
}

void acc()
{
  int x = analogRead(xpin); //read x pin value
  int y = analogRead(ypin); //read y pin value
  if (y < 500)
  {
    m = 2;
    SendSMS();                    //Forward
  }

  if (y > 400)
  {
    m = 4;
    SendSMS();                        //Reverse or Back
  }

  if (x > 400)
  {
    m = 6;
    SendSMS();                    //Left
  }

  if (x < 500)
  {
    m = 8;
    SendSMS();                   //Right
  }
  delay(2000);
}

void temp()
{
  int chk = DHT.read11(DHT11_PIN);
  //Serial.print("Temperature = ");
  //
  //Serial.print("Humidity = ");
  //Serial.println(DHT.humidity);

  if (DHT.temperature > 28)
  {
    Serial.println("AT+CMGF=1");
    delay(1000);
    Serial.println("AT+CMGS=\"+9190351064651\"\r"); //Change to destination phone number
    delay(1000);
    Serial.println(DHT.temperature);
    Serial.println("temperature is abnormal");
    delay(1000);
    Serial.println((char)26);                      //the stopping character Ctrl+Z
    delay(2000);
    rms = true;
  }


}
void ldr()
{
  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus >= 650)
  {

    digitalWrite(ledPin, LOW);

    Serial.println(" LED is ON");
    delay(1000);
  }

  else
  {

    digitalWrite(ledPin, HIGH);

    //Serial.println("LED IS OFF");
    //delay(2000);
  }
  //}
}

// GSM Modem SMS Sending Tutorial
// circuits4you.com

//void setup()
//{
//  Serial.begin(9600);   //Initialise serial to communicate with GSM Modem
//}

//void loop()
//{
//   delay(10000); //Give enough time for GSM to register on Network
// SendSMS();    //Send one SMS
//     delay(10000);     //Wait forever
//}

void SendSMS()
{
  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  Serial.println("AT+CMGS=\"+919035106465\"\r"); //Change to destination phone number
  delay(1000);
  switch (m)
  {
    case 2:
      Serial.println("I need water");//the content of the message
      break;
    case 4:
      Serial.println("I need food");
      break;
    case 6:
      Serial.println("I need to go to washroom");
      break;
    case 8:
      Serial.println("In emergency");
      break;
    default:
      ;

  }
  delay(200);
  Serial.println((char)26); //the stopping character Ctrl+Z
  delay(1000);
}



