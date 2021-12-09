#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27, 16, 2);
//buzzer
const int buzzer = 9;
//keypad
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
//password
const String password = "1234"; // change your password here
String input_password;


void setup() {
  Serial.begin(9600);
  while (!Serial);
SPI.begin();
//BUZZER
 pinMode(buzzer, OUTPUT);
mfrc522.PCD_Init();
//lcd
 lcd.init();          // Initiate the LCD module
  lcd.backlight();
mfrc522.PCD_Init();  

 

}

void loop() {
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
if (content.substring(1) == "AC DF 0E 22") //change here the UID of the card/cards that you want to give access
  {
   Serial.println("Authorized access");
lcd.setCursor(0,0);
lcd.clear();
lcd.print("Authorized");
lcd.setCursor(1,0);
lcd.print("access");
    delay(3000);
  }
   if (content.substring(1) == "CA 2D 8E 19") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
lcd.setCursor(0,0);
lcd.clear();
lcd.print("Authorized");
lcd.setCursor(1,0);
lcd.print("access");
    delay(3000);
  }
  char key = keypad.getKey();

  if (key){
    Serial.println(key);

    if(key == '*') {
      input_password = ""; // clear input password
    } else if(key == '#') {
      if(password == input_password) {
        Serial.println("password is correct");
        // DO YOUR WORK HERE
lcd.setCursor(0,0);
lcd.clear();
lcd.print("Authorized");
lcd.setCursor(1,0);
lcd.print("access");
        
      } else {
 Serial.println("password is incorrect, try again");
lcd.setCursor(0,0);
lcd.clear();
lcd.print("BLOCKED");

      }

      input_password = ""; // clear input password
    } 
    else {
      input_password += key; // append new character to input password string
    }
  }


 else   {
    Serial.println(" Access denied");
lcd.setCursor(0,0);
lcd.clear();
lcd.print("BLOCKED");
tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);   
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);    

    delay(3000);
  }
} 
 
