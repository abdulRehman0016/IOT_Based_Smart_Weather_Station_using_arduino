
#include <Adafruit_BMP085.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>  
#include <dht.h>


String statusChWriteKey = "3GR24MX89U9D95II";

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

SoftwareSerial EspSerial(6, 7); // Rx,  Tx
#define HARDWARE_RESET 9
#define speed8266 9600


Adafruit_BMP085 bmp;
#define ledPin 10
#define dataPin 8
#define RainSenPin A0
#define LdrPin A1

// Variables to be used with timers
long writeTimingSeconds = 10; // ==> Define Sample time in seconds to send data
long startWriteTiming = 0;
long elapsedWriteTime = 0;

int spare = 0;
boolean error;

dht DHT;


void setup() {
     
  Serial.begin(9600);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

   if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  lcd.begin(20, 4);

  Serial.begin(speed8266);
    
  EspSerial.begin(9600);       // Communication with WiFi Module
  pinMode(HARDWARE_RESET,OUTPUT); 
  digitalWrite(HARDWARE_RESET, HIGH);

  EspHardwareReset(); //WiFi Module Reset
  startWriteTiming = millis(); // starting the "program clock"

}
  int readData = DHT.read22(dataPin);
  int sensorValue = analogRead(RainSenPin);  // Read the analog value from sensor
  int Ldrvalue = analogRead(LdrPin); 
  float h = DHT.humidity; 
  int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
   
  int LDRoutput = map(Ldrvalue, 0, 1023, 255, 0); 
  

void loop() {
    LCDv();
  
    start: // label
    error = 0;

    elapsedWriteTime = millis() - startWriteTiming;

    if (elapsedWriteTime > (writeTimingSeconds * 500))
    {
        
        writeThingSpeak();
        startWriteTiming = millis();
    }

    if (error == 1) // Resend if transmission is not completed
    {
        Serial.println(" <<<< ERROR >>>>");
        delay(2000);
        goto start; // go to label "start"
    }
     
  // BMP180();    //A4 to SCL & A5 to SDA
  // dhts();      //  Dht22 Pin 8
  // Rainsensor();  // A0 analog Input
  // LDR();  // A1 analog Input

}



void BMP180( void){
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.write(0xC2);
    Serial.write(0xB0);
    Serial.println("C"); 
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");

    // lcd.setCursor(0, 2);
    // lcd.print("Sea lvl pressure = ");
  	// lcd.print(bmp.readSealevelPressure());
	  // lcd.print("Pa");
    // delay(500); 

    

    

}


void dhts(void){
  int readData = DHT.read22(dataPin);
  float h = DHT.humidity; 
	// Gets the values of the humidity

  //Printing the results on the serial monitor
	// Serial.print("Temperature= ");
  // Serial.print(t);
  // Serial.write(0xC2);
  // Serial.write(0xB0);
  // Serial.print("C | ");
  // Serial.print("Humidity = ");
	// Serial.print(h);
	// Serial.println("%");
  
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Humidity = ");
	// lcd.print(h);
	// lcd.print("%   ");
	 
 }
  
void Rainsensor( void ){
  // int sensorValue = analogRead(RainSenPin);  // Read the analog value from sensor
  // int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
  // analogWrite(ledPin, outputValue); // generate PWM signal
  // Serial.print("Rain Value is = ");
  // Serial.println(outputValue);          // Return analog rain value
  // delay(500);
  // lcd.setCursor(0, 1);
  // lcd.print("Rain Value is = ");
	// lcd.print(outputValue);
//   if(outputValue < 85 )
// {
//   Serial.println("NOT Raining   ");
// }
// else if(outputValue > 85 && outputValue < 170)
// {
//    Serial.println("Raining Bit    ");
// }
// else if(outputValue > 180 && outputValue < 254)
// {
//    Serial.println("TOO MUCH RAINING   ");
// }
}
void LDR(void){
// int Ldrvalue = analogRead(LdrPin); 
// int LDRoutput = map(Ldrvalue, 0, 1023, 255, 0); 
// // Serial.print("Sunlight is = ");
// // Serial.println(LDRoutput);
// if(LDRoutput < 85 )
// {
//   Serial.println("NOT too suny   ");
// }
// else if(LDRoutput > 85 && LDRoutput < 170)
// {
//    Serial.println("Suny         ");
// }
// else if(LDRoutput > 180 && LDRoutput < 254)
// {
//    Serial.println("IT's Too sunny ");
// }
}

void LCDv(void){
  int readData = DHT.read22(dataPin);
  int sensorValue = analogRead(RainSenPin);  // Read the analog value from sensor
  int Ldrvalue = analogRead(LdrPin); 
  float h = DHT.humidity; 
  int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
  analogWrite(ledPin, outputValue);
  int LDRoutput = map(Ldrvalue, 0, 1023, 255, 0); 

    
    lcd.setCursor(0, 0);
    lcd.print("Temperature= ");
  	lcd.print(bmp.readTemperature());
	  lcd.print(F("\xDF"));
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Pressure = ");
  	lcd.print(bmp.readPressure());
	  lcd.print(" Pa");
    lcd.setCursor(0, 2);
    lcd.print("Altitude = ");
  	lcd.print(bmp.readAltitude(101500));
	  lcd.print(" M");
    lcd.setCursor(0, 3);
    lcd.print("Humidity = ");
	  lcd.print(h);
	  lcd.print("% ");
    delay(1500);
    lcd.clear();
    if(outputValue < 85 )
    {
    //Serial.println("NOT Raining   ");
    lcd.setCursor(0, 0);
    lcd.print("NOT Raining   ");
  
}
else if(outputValue > 85 && outputValue < 170)
{
  // Serial.println("Raining Bit    ");
   lcd.setCursor(0, 0);
   lcd.print("Raining Bit    ");
}
else if(outputValue > 171 && outputValue < 254)
{
  // Serial.println("TOO MUCH RAINING   ");
   lcd.setCursor(0, 0);
   lcd.print("TOO MUCH RAINING   ");
}
    // lcd.setCursor(0, 0);
    // lcd.print("Rain Value is = ");
	  // lcd.print(outputValue);
    if(LDRoutput < 85 )
{
  //Serial.println("NOT too suny   ");
  lcd.setCursor(0, 1);
  lcd.print("NOT TOO SUNNY   ");
}
else if(LDRoutput > 85 && LDRoutput < 170)
{
   //Serial.println("Suny         ");
   lcd.setCursor(0, 1);
   lcd.print("It's SUNNY         ");
}
else if(LDRoutput > 171 && LDRoutput < 254)
{
   //Serial.println("IT's Too sunny ");
   lcd.setCursor(0, 1);
   lcd.print("It's TOO SUNNY    ");
}
    // lcd.setCursor(0, 1);
    // lcd.print("Sunlight is = ");
    // lcd.print(LDRoutput);
    delay(1000);
    

}

/********* Conexao com TCP com Thingspeak *******/
void writeThingSpeak(void)
{
    startThingSpeakCmd();

  
    // preparacao da string GET
    String getStr = "GET /update?api_key=";
    getStr += statusChWriteKey;
    getStr += "&field1=";
    getStr += String(bmp.readTemperature());
    int Ldrvalue = analogRead(LdrPin);
    int LDRoutput = map(Ldrvalue, 0, 1023, 255, 0);
    getStr += "&field2=";
    getStr += String(LDRoutput);
    int sensorValue = analogRead(RainSenPin);  // Read the analog value from sensor
    int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
    getStr += "&field3=";
    getStr += String(outputValue);
    getStr += "&field4=";
    getStr += String(bmp.readAltitude(101500));
    getStr += "&field5=";
    getStr += String(bmp.readPressure());
    getStr += "&field6=";
    getStr += String(DHT.humidity);
    getStr += "&field7=";
    getStr += String(spare);
    getStr += "\r\n\r\n";

    sendThingSpeakGetCmd(getStr);
}

/********* Reset ESP *************/
void EspHardwareReset(void)
 {
    Serial.println("Reseting.......");
    digitalWrite(HARDWARE_RESET, LOW);
    delay(500);
    digitalWrite(HARDWARE_RESET, HIGH);
    delay(3000); // Time needed to start reading
    Serial.println("RESET");
}

/********* Start communication with ThingSpeak*************/
void startThingSpeakCmd(void)
 {
    EspSerial.flush(); // clears the buffer before starting to record
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += "184.106.153.149"; //  IP address of api.thingspeak.com
    cmd += "\",80";
    EspSerial.println(cmd);
    Serial.print("sent ==> Start cmd: ");
    Serial.println(cmd);

    if (EspSerial.find("Error"))
    {
        Serial.println("AT+CIPSTART error");
        return;
    }
}

/********* send a GET cmd to ThingSpeak *************/
String sendThingSpeakGetCmd(String getStr)
 {
    String cmd = "AT+CIPSEND=";
    cmd += String(getStr.length());
    EspSerial.println(cmd);
    Serial.print("sent ==> lenght cmd: ");
    Serial.println(cmd);

    if (EspSerial.find((char *)">"))
    {
        EspSerial.print(getStr);
        Serial.print("sent ==> getStr: ");
        Serial.println(getStr);
        delay(500); // time to process the GET, without this delay it shows busy in the next command

        String messageBody = "";
        while (EspSerial.available())
        {
            String line = EspSerial.readStringUntil('\n');
            if (line.length() == 1)
            { // actual content starts after empty line (that has length 1)
                messageBody = EspSerial.readStringUntil('\n');
            }
        }
        Serial.print("MessageBody received: ");
        Serial.println(messageBody);
        return messageBody;
    }
    else
    {
        EspSerial.println("AT+CIPCLOSE");                   // alert user
        Serial.println("ESP8266 CIPSEND ERROR: RESENDING"); // Resend...
        spare = spare + 1;
        error = 1;
        return "error";
    }
}
