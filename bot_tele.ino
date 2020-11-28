#include "CTBot.h"
#include "DHT.h"

#include <WiFiManager.h>


//Initialize DHT
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Create bot variable
CTBot myBot;

/*WiFi setup
String ssid = "Wififififi";
String pass = "23454321";*/ //not needed anymore

//Set telegram bot token
String token = "1288884748:AAGjf9IHp_lfSJOcGDREyRm9hLp_tzMxNoU";

//Pins
int relay1 = D2;
int relay2 = D3;
int sensor = A0;
int statled = D6;
int watled = D4;


void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(statled, OUTPUT);
  pinMode(watled, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting");

  WiFiManager wm;

  wm.autoConnect("WiFi Setup"); //use WiFiManager instead
  //myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  
  if (myBot.testConnection()){
    Serial.println("\nConnected!");
    digitalWrite(statled, HIGH);
  }
  else {
    Serial.println("\nConnection failed");
  }

}

void loop() {
  TBMessage msg;
  
  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("/SIRAM")) {
      digitalWrite(relay1, HIGH); //Menyalakan inverter
      delay(2000);
      digitalWrite(relay2, HIGH); //Menyalakan pompa air
      myBot.sendMessage(msg.sender.id, "Menyiram.....");
      delay(10000);
      digitalWrite(relay1, LOW); //Matikan inverter
      digitalWrite(relay2, LOW); //matikan pompa air
    }
    else if (msg.text.equalsIgnoreCase("/STATUS")) {
      dht.begin();
      delay(500);
      int t = dht.readTemperature();
      int h = dht.readHumidity();
      if (isnan(t)){
        myBot.sendMessage(msg.sender.id, "Gagal membaca data dari sensor.");
      }
      else {
        String balasan;
        balasan = (String)"Suhu udara saat ini adalah " + t + (String)"°C dengan kelembaban udara " + h + (String)"%.";
        myBot.sendMessage(msg.sender.id, balasan);
      }
    }
    else if (msg.text.equalsIgnoreCase("/HELP")) {
      myBot.sendMessage(msg.sender.id, "Ketikkan perintah berikut :\n/status : untuk melihat status\n/relay1on : untuk mengaktifkan relay1\n/relay1off : untuk mematikan relay1\n/relay2on : untuk mengaktifkan relay2\n/relay2off : untuk mematikan relay2");
    }
    else {
      myBot.sendMessage(msg.sender.id, "Tekan /help untuk info lebih lanjut");
    }
  }
  //delay (500);
}
