// -------------------------------------------------------- Elecmake ------------------------------------------------------

// --------------------------------------------------------------- Wiring ------------------------------------------------------
// --------------------------------------------------------------  RFID SDA  >> Arduino  5  ------------------------------------
// --------------------------------------------------------------  RFID SCK  >> Arduino  13 ------------------------------------
// --------------------------------------------------------------  RFID MOSI >> Arduino  11 ------------------------------------
// --------------------------------------------------------------  RFID MISO >> Arduino  12 ------------------------------------
// --------------------------------------------------------------  RFID RQ   >> Arduino  no connection -------------------------
// --------------------------------------------------------------  RFID GND  >> Arduino  GND -----------------------------------
// --------------------------------------------------------------  RFID RST  >> Arduino  9 -------------------------------------
// --------------------------------------------------------------  RFID 3.3V >> Arduino  +3.3V ---------------------------------


// --------------------------------------------------------------  Relay GND  >> Arduino  GND -----------------------------------
// --------------------------------------------------------------  Relay VCC  >> Arduino  5V ------------------------------------
// --------------------------------------------------------------  Relay IN-1 >> Arduino  3 -------------------------------------


// --------------------------------------------------------------  Buzzer +  >> Arduino  2 --------------------------------------
// --------------------------------------------------------------  Buzzer -  >> Arduino  GND ------------------------------------


// --------------------------------------------------------------  LCD GND  >> Arduino  GND --------------------------------------
// --------------------------------------------------------------  LCD 16*2 VCL >> Arduino  5V -----------------------------------
// --------------------------------------------------------------  LCD 16*2 SDA >> Arduino  A4 -----------------------------------
// --------------------------------------------------------------  LCD 16*2 SCL >> Arduino  A5 ------------------------------------

// --------------------------------------------------------------  LED  1 >> Arduino  7 --------------------------------------
// --------------------------------------------------------------  LED  2 >> Arduino  GND ------------------------------------

#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define SS_PIN 5
#define RST_PIN 9
#define RELAY 3 //connect the relay to number 3 pin
#define BUZZER 2 // connect the buzzer to 2 pin
#define LED 7
#define ACCESS_DELAY 4000
#define DENIED_DELAY 1000

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
  lcd.begin(16,2);
  lcd.setCursor(0,0); 
  lcd.print("Put Cart On Rfid");
  lcd.setCursor(0,1);
  lcd.print("................");
  Serial.println("1");
  Serial.println();

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "A3 C7 92 0C") // enter your own card number after copying it from serial monitor
  {
    wdt_enable(WDTO_2S); 
    digitalWrite(LED, HIGH);  
    lcd.setCursor(0,1); 
    lcd.print("Access Granted");
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(RELAY, LOW);
    delay(ACCESS_DELAY);
    digitalWrite(LED, LOW);
    digitalWrite(RELAY, HIGH);
    wdt_reset();
    wdt_disable();
  }
  else   {
     wdt_enable(WDTO_2S);
    Serial.println(" Access denied");
    lcd.setCursor(0,1); 
    lcd.print("Access denied.");
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    noTone(BUZZER);
    wdt_reset();
    wdt_disable();
  }
  lcd.setCursor(0,1);
  lcd.print("................");
}