/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  
  
  ----> Modificado por Valerio Gomez
  

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro/UNO/Dos, na mentira or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(2, 3);

int suma = 0;    //iniciamos la variable suma en cero que nos contara las veces que se haga lectua de una huella
int on = 100;   //tiempo del destello de la luz huella

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{ pinMode (8,OUTPUT);   // Seleccionamos el pin 8 como salida, para activar nuestra moto "RELAY 1"
  pinMode (9,OUTPUT);   // Seleccionamos el pin 9 como salida, para encender nuestra moto (star) "RELAY 2"
  pinMode (10,OUTPUT);  // Seleccionamos el pin 10 como salida, para controlar señal de encendido "RELAY 3"
  pinMode (11,OUTPUT);  // Seleccionamos el pin 11 como salida, para encender el sensor de huella "Transistor Q5 NPN"
  digitalWrite(8, HIGH);    // iniciamos la salida 8 en OFF
  digitalWrite(9, HIGH);    // iniciamos la salida 9 en OFF
  digitalWrite(10, HIGH);   // iniciamos la salida 10 en OFF
  digitalWrite(11, HIGH);   // iniciamos la salida 11 en ON Transistor Q5 Activo
  
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  delay(on);            //don't ned to run this at full speed.
  
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}


  
  
  
// returns -1 if failed, otherwise returns ID #

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;





  // Encendido y Activacion de  Moto 

  
  if(finger.fingerID == 6)    // si detecta huella 6
  {  
    digitalWrite(8, LOW);     // Se aciva la moto "RELAY 1 ON"
    suma = suma + 1;
   
  }
   
   
  if(finger.fingerID == 7)   // si detecta huella 7
  {  
    digitalWrite(8, LOW);    // Se aciva la moto "RELAY 1 ON"
    suma = suma + 1;
   
  }
   if(suma == 2) {          // si se reconoce por segunda vez la huella registrada
    digitalWrite(10, LOW);  // se activa la señal de encendido "RELAY 3"
    delay(700);
    digitalWrite(9, LOW);    // se enciende (star) la moto "RELAY 2 ON"
    delay(700);              // Tiempo de encendido
    digitalWrite(9,HIGH);    // "RELAY 2 OFF"
    digitalWrite(11,LOW);    // sensor de huella OFF 

    //on = 60000;
   }
   
   
  
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
