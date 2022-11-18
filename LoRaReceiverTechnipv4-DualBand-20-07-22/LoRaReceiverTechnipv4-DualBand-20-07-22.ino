/*
 * LoraReceiverData
 * A for receiving and sending data using radio lora.
 *
 * Copyright (c) 2020 Iluminiun inc.
 * Email      : eng.fbneves@gmail.com
 * Author     : Fabrício Neves
 * Create Time: 9 April,2020
 * Change Log :
 *
 *
 */


#include <SPI.h>
#include <LoRa.h>
#include <SSD1306.h>
#include <Wire.h>

// Logo
#include "foxlogo.h"
#define posX 2
#define posY 0

// Pinos do display (comunicação i2c)
const int DISPLAY_ADDRESS_PIN = 0x4c;
const int DISPLAY_SDA_PIN = 6;
const int DISPLAY_SCL_PIN = 18;
const int DISPLAY_RST_PIN = 23;

// Pinos do lora (comunicação spi)
const int LORA_SCK_PIN = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN = 18;
const int LORA_RST_PIN = 15;
const int LORA_DI00_PIN = 26;

String packSize = "--";
String packet ;
String rssi = "RSSI --";
String recebido = "";
// Variavel recebe status do modulo
int d = 0;
int dTime = 100;
int counter = dTime;
int sCounter;
unsigned int Bat;
unsigned int statusAlarme = 0;
unsigned int staPorta = 1;
unsigned int Mod ;
int D;



// Frequência de comunicação
//const int BAND = 800E6;
int BAND = 800E6;

bool BandSta;

// Altura da fonte (correspondente a fonte ArialMT_Plain_10)
const int fontHeight = 10;

// Objeto do display
SSD1306 display(DISPLAY_ADDRESS_PIN, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

// Função que inicializa o display
bool displayBegin()
{
  // Reiniciamos o display
  pinMode(DISPLAY_RST_PIN, OUTPUT);
  digitalWrite(DISPLAY_RST_PIN, LOW);
  delay(1);
  digitalWrite(DISPLAY_RST_PIN, HIGH);
  delay(1);

  return display.init();
}


// Função que faz algumas configuções no display
void displayConfig()
{
  // Invertemos o display verticalmente
  //display.flipScreenVertically();
  // Setamos a fonte
  display.setFont(ArialMT_Plain_10);
  // Alinhamos a fonta à esquerda
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}




bool loraBegin()
{
  // Iniciamos a comunicação SPI
  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
  // Setamos os pinos do lora
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI00_PIN);
  // Iniciamos o lora
  return LoRa.begin(BAND);
}


void setup() {
  // Iniciamos a serial com velocidade de 9600
  Serial.begin(9600);

  // Exibimos "Starting..." na serial (debug)
  Serial.println("Starting...");
  pinMode(25, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(36, INPUT);
  pinMode(34, INPUT);
  pinMode(25, OUTPUT);

  BandSta = digitalRead(34);

  if (BandSta == 0) {
    delay(500);
    if (BandSta == 0) {
      BAND = 868E6;
    }
  } else if (BandSta == 1) {
    delay(500);
    if (BandSta == 1) {

      BAND = 915000000;
    }
  }

  digitalWrite(32, LOW);
  digitalWrite(33, LOW);

  // Iniciamos o display
  if (!displayBegin())
  {
    // Se não deu certo, exibimos falha de display na serial
    Serial.println("Display failed!");
    // E deixamos em loop infinito
    while (1);
  }

  // Configuramos o posicionamento da tela, fonte e o alinhamento do texto
  displayConfig();
  //Mostra a Logo
  display.clear();
  display.drawXbm(posX, posY, foxlogo_width, foxlogo_height, foxlogo_bits);
  display.display();
  delay(1000);



  if (!loraBegin())
  {
    // Se não deu certo, exibimos falha de lora na serial
    Serial.println("LoRa failed!");
    // E deixamos em loop infinito
    while (1);

  }
  display.clear();

}

void loop() {


  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    cbk(packetSize);
    counter = 0;
    delay(20);
  } else {
    counter++;

  }
  /*
    if (statusAlarme > 2) {

      digitalWrite(32, LOW);

    } else {
      digitalWrite(32, HIGH);
    }
    if (statusAlarme == 3) {

      digitalWrite(32, LOW);
      digitalWrite(33, LOW);

    } else {
      digitalWrite(32, HIGH);
      digitalWrite(33, HIGH);
    }
  */
  if (counter > dTime) {

    display.clear();
    display.drawXbm(posX, posY, foxlogo_width, foxlogo_height, foxlogo_bits);
    display.drawString(0, 0, "v4.2");
    display.drawString(80, 10, "Sem");
    display.drawString(80, 20, "Dados");
    display.drawString(90, 40, "Freq");
    display.drawString(80, 50 , String(BAND / 1000000) + "mHz");
    display.display();    
    counter = dTime;
  } else {    
  }


  // Porta

  staPorta = digitalRead(36);
  Serial.println(digitalRead(34));

  if (BandSta != digitalRead(34)) {
    delay(1000);
    if (BandSta != digitalRead(34)) {
      display.clear();
      display.drawString(10, 0, "Mudando Frequencia...");
      display.display();
      delay(1000);
      ESP.restart();
    }

  }




  delay(10);
}
