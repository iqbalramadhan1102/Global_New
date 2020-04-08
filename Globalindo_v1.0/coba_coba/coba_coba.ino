#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

SoftwareSerial esp (3, 2);

const char* ssid = "Sewing_Arduino";
const char* pass = "admin1234";
const char* php = " /globalindo/kirim.php";
const char* host = "192.168.2.9";
const uint16_t port = 80;
int status = WL_IDLE_STATUS;
int statrfid = 0;
int oneortwo = 0;
String perangkat = "9022";
String absen;
String container;
String value1;
String value2;
String mood;
String jenis;
String vardock, varabs, varkon;
char stat1, stat2, stat3;
String mystring, mystring1;
String subb, oldsubb;
String Respon = "";
String kirim, oldkirim;
String stat;
char cperangkat[5];
char cvarabs[7];
char cvarkon[7];
char cvardock[7];
int countingnocard, counting2cards, initial;
int counternocard = 10;
int counter2cards = 10;
#define led_r 5
#define led_b 6
#define led_g 7
#define buzzer 8
#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522 (SS_PIN, RST_PIN);

long waktuMulai;
WiFiEspClient client;


void detectcard() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  byte buffer2[8];
  block = 1;
  len = 18;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    
    return 0;
  }
  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    
  }
  else
  {

  }
  //PRINT LAST NAME
  for (uint8_t i = 0; i < 7; i++) {
    mystring += (char)buffer2[i];
    mystring1 = mystring;
  }
  //depannya 0 absen 1 kontener 2 docking
  delay(100);
  //  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
void init_awal() {

  Serial.begin(9600);
  pinMode(led_r, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(led_g, OUTPUT);
  delay(100);
  digitalWrite(led_r, HIGH);
  digitalWrite(led_g, LOW);
  digitalWrite(led_b, LOW);

  esp.begin(9600);
  WiFi.init(&esp);

  delay(20);
//  Serial.println(WiFi.status());
  if (WiFi.status() == WL_NO_SHIELD)
  {
  Serial.println(WiFi.status());
    //    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  //  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    Serial.println(WiFi.status());
    //    Serial.print("Attempting to connect to WPA SSID: ");
    //    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    // you're connected now, so print out the data
    //    Serial.println("You're connected to the network");

  }
}
void setup() 
{  
  perangkat.toCharArray(cperangkat, 5);
  pinMode(buzzer, OUTPUT);
  SetSockStatus();
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, HIGH);
  digitalWrite(led_b, LOW);
  delay(200);
  SPI.begin();
  mfrc522.PCD_Init();
  oneortwo = 1;
  initial = 1;
  counting2cards = 11;
  init_awal();
}

void loop()
{
  Serial.println(WiFi.status());
  if (WiFi.status() == 1 || WiFi.status() == 0) //konek
  {
    detectcard();
    delay(50);
    subb = mystring1.substring(0, 1);
    //    if (oldsubb == "" || subb == "")
    //    {
    //      subb = "-";
    //      oldsubb = "-";
    //    }
    if (subb == "")
    {
      subb = oldsubb;
    }
    Serial.print(mystring1);
    Serial.print(" ");
    Serial.print(subb);
    Serial.print(" ");
    Serial.println(oldsubb);
    delay(200);
    if ((oldsubb != subb) && (oldsubb != "") && (subb != "")) //kondisi ada 2 kartu
    {
      if (((oldsubb == "0") && (subb == "1")) || ((oldsubb == "1") && (subb == "0")))
      {
        if (mystring.startsWith("00"))
        {
          varabs = mystring1;
          varabs.toCharArray(cvarabs, 7);
        }
        if (mystring.startsWith("10"))
        {
          varkon = mystring1;
          varkon.toCharArray(cvarkon, 7);
        }
        stat1 = '1';
        stat2 = '1';
        stat3 = '0';
        Serial.println("pekerja & kontener");
        oneortwo = 2;
        counting2cards = 0;
        stat = "pk";
        digitalWrite(led_r, LOW);
        digitalWrite(led_g, HIGH);
        digitalWrite(led_b, LOW);
      }
      if (((oldsubb == "0") && (subb == "2")) || ((oldsubb == "2") && (subb == "0")))
      {
        if (mystring1.startsWith("00"))
        {
          varabs = mystring1;
          varabs.toCharArray(cvarabs, 7);
        }
        if (mystring1.startsWith("20"))
        {
          vardock = mystring1;
          vardock.toCharArray(cvardock, 7);
        }
        digitalWrite(led_r, LOW);
        digitalWrite(led_g, HIGH);
        digitalWrite(led_b, LOW);
        stat1 = '1';
        stat2 = '0';
        stat3 = '1';
        Serial.println("pekerja & docking");
        oneortwo = 2;
        counting2cards = 0;
        stat = "pd";
      }
      if (((oldsubb == "1") && (subb == "2")) || ((oldsubb == "2") && (subb == "1")))
      {
        if (mystring1.startsWith("10"))
        {
          varkon = mystring1;
          varkon.toCharArray(cvarkon, 7);
        }
        if (mystring1.startsWith("20"))
        {
          vardock = mystring1;
          vardock.toCharArray(cvardock, 7);
        }
        digitalWrite(led_r, LOW);
        digitalWrite(led_g, HIGH);
        digitalWrite(led_b, LOW);
        stat1 = '0';
        stat2 = '1';
        stat3 = '1';
        Serial.println("kontener & docking");
        oneortwo = 2;
        counting2cards = 0;
        stat = "kd";
      }
      else
      {

      }
    }
    //    delay(50);
    if (oneortwo != 2) //mendeteksi 1 or 2 kartu
    {
      counting2cards += 1;
      //      Serial.print("2cards : ");
      //      Serial.print(counting2cards);
      //      Serial.print(" ");
      if (counting2cards > counter2cards)
      {
        if (mystring1.startsWith("00"))// && stat.length() != 1)
        {
          jenis = "pekerja";
          stat1 = '1';
          stat2 = '0';
          stat3 = '0';
          varabs = mystring1;
          varabs.toCharArray(cvarabs, 7);
          oneortwo = 1;
          stat = "p";
          buzzer_tone(1);
          digitalWrite(led_r, HIGH);
          digitalWrite(led_g, LOW);
          digitalWrite(led_b, LOW);
          delay(100);
          digitalWrite(led_r, LOW);
          digitalWrite(led_g, LOW);
          digitalWrite(led_b, LOW);
          buzzer_tone(0);
        }
        else if (mystring1.startsWith("10"))// && stat.length() != 1)
        {
          jenis = "kontainer";
          stat1 = '0';
          stat2 = '1';
          stat3 = '0';
          varkon = mystring1;
          varkon.toCharArray(cvarkon, 7);
          oneortwo = 1;
          stat = "k";
          buzzer_tone(1);
          digitalWrite(led_r, HIGH);
          digitalWrite(led_g, LOW);
          digitalWrite(led_b, LOW);
          delay(100);
          digitalWrite(led_r, LOW);
          digitalWrite(led_g, LOW);
          digitalWrite(led_b, LOW);
          buzzer_tone(0);
        }
        else if (mystring1.startsWith("20"))// && stat.length() != 1)
        {
          jenis = "docking";
          stat1 = '0';
          stat2 = '0';
          stat3 = '1';
          vardock = mystring1;
          vardock.toCharArray(cvardock, 7);
          oneortwo = 1;
          stat = "d";
          digitalWrite(led_r, LOW);
          digitalWrite(led_g, HIGH);
          digitalWrite(led_b, LOW);
        }
      }
    }
    if (initial < 3)
    {
      kirim += cperangkat;
      kirim += "~";
      kirim += cvarabs;
      kirim += "~";
      kirim += cvarkon;
      kirim += "~";
      kirim += cvardock;
      kirim += "~";
      kirim += stat1;
      kirim += "~";
      kirim += stat2;
      kirim += "~";
      kirim += stat3;
    }
    else if ((mystring1 != "") && (initial > 4)) // kondisi ada kartu
    {
      kirim += cperangkat;
      kirim += "~";
      kirim += cvarabs;
      kirim += "~";
      kirim += cvarkon;
      kirim += "~";
      kirim += cvardock;
      kirim += "~";
      kirim += stat1;
      kirim += "~";
      kirim += stat2;
      kirim += "~";
      kirim += stat3;
      delay(50);
      countingnocard = 0;
      if (oldkirim != kirim)
      {
        if (client.connect(host, 80))
        {
          digitalWrite(led_r, LOW);
          digitalWrite(led_g, LOW);
          digitalWrite(led_b, HIGH);
          buzzer_tone(1);
          delay(1000);
          buzzer_tone(0);
          delay(50);
          String url;
          String mood;
          url += php;
          url += "?mood=";
          url += cperangkat;
          url += "~";
          url += cvarabs;
          url += "~";
          url += cvarkon;
          url += "~";
          url += cvardock;
          url += "~";
          url += stat1;
          url += "~";
          url += stat2;
          url += "~";
          url += stat3;
          Serial.println(url);
          client.print("GET");
          client.print(url);
          client.println(" HTTP/1.1");
          client.print("Host: ");
          client.println(host);
          client.println("Connection: close");
          client.println();
        }
      }
      oldkirim = kirim;
    }
    else if ((mystring1 == "") && (initial > 4))//ga ada kartu yg nempel
    {
      //    delay(100);
      countingnocard += 1;
      Serial.print(" nocard : ");
      Serial.println(countingnocard);
      if (countingnocard > counternocard)
      {
        buzzer_tone(1);
        digitalWrite(led_r, HIGH);
        digitalWrite(led_g, LOW);
        digitalWrite(led_b, LOW);
        delay(100);
        digitalWrite(led_r, LOW);
        digitalWrite(led_g, LOW);
        digitalWrite(led_b, LOW);
        buzzer_tone(0);
        delay(300);
        subb = "-";
        oldsubb = "-";
        kirim += cperangkat;
        kirim += "~";
        kirim += cvarabs;
        kirim += "~";
        kirim += cvarkon;
        kirim += "~";
        kirim += cvardock;
        kirim += "~0~0~0";
        if (oldkirim != kirim)
        {
          if (client.connect(host, 80))
          {
            digitalWrite(led_r, LOW);
            digitalWrite(led_g, LOW);
            digitalWrite(led_b, HIGH);
            buzzer_tone(1);
            delay(1000);
            buzzer_tone(0);
            delay(50);
            String url;
            String mood;
            url += php;
            url += "?mood=";
            url += cperangkat;
            url += "~";
            url += cvarabs;
            url += "~";
            url += cvarkon;
            url += "~";
            url += cvardock;
            url += "~0~0~0";
            Serial.println(url);
            client.print("GET");
            client.print(url);
            client.println(" HTTP/1.1");
            client.print("Host: ");
            client.println(host);
            client.println("Connection: close");
            client.println();
          }
        }
        oldkirim = kirim;
      }
    }
    if (counting2cards > 30)
    {
      counting2cards = 30;
    }
    if (countingnocard > 30)
    {
      countingnocard = 30;
    }
    delay(100);
    if (subb != "")
    {
      oldsubb = subb;
    }
    SetSockStatus();
    mystring = "";
    mystring1 = "";
    kirim = "";
    oneortwo = 0;
    initial++;
  }
  if (WiFi.status() == 3) //kondisi ketika wifinya putus
  {
    Serial.println(WiFi.status());
    delay(1000);
    digitalWrite(led_r, HIGH);
    digitalWrite(led_g, LOW);
    digitalWrite(led_b, LOW);
    buzzer_tone(1);
    delay(50);
    buzzer_tone(0);
    delay(50);
    buzzer_tone(1);
    delay(50);
    buzzer_tone(0);
    delay(50);
    buzzer_tone(1);
    delay(50);
    buzzer_tone(0);
    delay(50);
  }
  else if (WiFi.status() == 255) //kondisi ketika esp nya coplok
  {
    Serial.println(WiFi.status());
    delay(1000);
    digitalWrite(led_r, HIGH);
    digitalWrite(led_g, LOW);
    digitalWrite(led_b, LOW);
    buzzer_tone(1);
    delay(50);
    buzzer_tone(0);
    delay(50);
    buzzer_tone(1);
    delay(50);
    buzzer_tone(0);
    delay(50);
  }
  
  if (initial > 5)
  {
    initial = 5;
  }

}
