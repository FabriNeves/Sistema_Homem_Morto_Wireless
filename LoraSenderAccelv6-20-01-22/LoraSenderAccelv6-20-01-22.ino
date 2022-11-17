#include <LoRa.h>
#include <SSD1306.h>
#include "Arduino.h"
#include <Wire.h>

#include <MMA8453_n0m1.h>

// Declara Objeto Accel
MMA8453_n0m1 accel;

#define PABOOST true

int limiteGraus = 171; // 0,3515625  
int tempo = 15;

bool pin0 = 1 ;

// Pinos do display (comunicação i2c)
const int DISPLAY_ADDRESS_PIN = 0x3c;
const int DISPLAY_SDA_PIN = 4;
const int DISPLAY_SCL_PIN = 15;
const int DISPLAY_RST_PIN = 16;


// Pinos do lora (comunicação spi)
const int LORA_SCK_PIN = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN = 18;
const int LORA_RST_PIN = 15;
const int LORA_DI00_PIN = 26;

// Frequência de comunicação
const int BAND = 927000000;

// Contador de pacotes enviados via lora
int counter = 0;

// Conta o tempo para Alarme;
unsigned tempo1 = 0;

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
 display.flipScreenVertically();
  // Setamos a fonte
 display.setFont(ArialMT_Plain_10);
  // Alinhamos a fonta à esquerda
 display.setTextAlignment(TEXT_ALIGN_LEFT);
}


// Função que inicializa o radio lora
bool loraBegin()
{
  // Iniciamos a comunicação SPI
  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
  // Setamos os pinos do lora
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI00_PIN);
  // Iniciamos o lora
  return LoRa.begin(BAND);
}

//Variaveis Globais
String packSize = "--";
String packet ;
String rssi = "RSSI --";
char dados[100];
String recebido = "";
unsigned int ini = 0; // Desabilitado
unsigned int Mod = 1; // Numero do Modulo

//Nivel de Bateria
unsigned int batLevel = 0; //Nivel de Bateria 0 ~ 100 %
float adc = 0.00081;  // 3.3v/4095
float v1;
float vbat, mVbat;
int contBat;

//Status Alarme
unsigned int statusSys = 1;

//Conta tempo para o Alarme
unsigned int contTempo;
bool sentido = true ;
void executeInExactly1000ms(void *pvParameters) {
  char *nomeDaTarefa;
  TickType_t momentoDoInicio;
  nomeDaTarefa    = (char *) pvParameters;

  momentoDoInicio = xTaskGetTickCount();
  for (;;) {
    // Serial.print("exatamente em 1000ms: ");
    //Serial.println(nomeDaTarefa);
    contTempo++;
    //Serial.println(contTempo);
    vTaskDelayUntil(&momentoDoInicio, pdMS_TO_TICKS(1000));
  }
}

// Rotina da Bateria

void leValorBat(void *pvParameters) {



  char *nomeDaTarefa;
  TickType_t momentoDoInicio;
  nomeDaTarefa    = (char *) pvParameters;

  momentoDoInicio = xTaskGetTickCount();
  for (;;) {

    //
    for (int i = 0; i < 30; i++) {

      //Lê v1
      v1 = analogRead(36);
      //Serial.println("Analog Puro"+String(v1));
      // Usando a proporcionalidade do divisor resistivo multiplicamos pelo fator do mesmo
      v1 = (v1 / 0.09383);
      //Serial.println("Proporcionalidade"+String(v1));
      v1 = v1 * adc;
      //Serial.println("Convertido pra V"+String(v1));
      vbat += float((v1 * 0.0033) + 1.42 + v1);
      //Serial.println("vbat Somado"+String(vbat));

      // Soma ao contador
      contBat++;
      //Serial.println("Contador"+String(counter));


      delay(100);
    }

    mVbat = vbat / contBat;
    if (mVbat < 3.6) {

      digitalWrite(12, HIGH);
    } else {

      digitalWrite(12, LOW);
    }

    mVbat = constrain(mVbat, 3.4, 4.2);
    batLevel = mVbat * 100;

    batLevel = map(batLevel, 340, 420, 0, 100);
    Serial.println("Média V Bat:" + String(batLevel));
    Serial.println("Média V Bat:" + String(mVbat));
    vbat = 0;
    contBat = 0;
    vTaskDelayUntil(&momentoDoInicio, pdMS_TO_TICKS(10000));
  }
}


void setup()
{
  Serial.begin(9600);
  Serial.print("Inclinação: ");
  Serial.println(90-(limiteGraus * 0.3515625));
  Serial.print("Tempo: ");
  Serial.println(tempo);


  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, !pin0); // 1 = HIGH , 0 = LOW
  xTaskCreatePinnedToCore(executeInExactly1000ms , "exeMs", 10000, (void *)"", 4, NULL, 1);
  xTaskCreatePinnedToCore(leValorBat , "valBat", 10000, (void *)"", 4, NULL, 1);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  pinMode(13, INPUT);
  pinMode(0, INPUT);
  //pinMode(12, OUTPUT);
  pinMode(17, OUTPUT);
  //digitalWrite(0, LOW);
  digitalWrite(17, HIGH);
  adcAttachPin(36);
  analogSetClockDiv(255); // 1338mS

  // Iniciamos o display
  if (!displayBegin())
  {
    // Se não deu certo, exibimos falha de display na serial
    // Serial.println("Display failed!");
    // E deixamos em loop infinito
    while (1);
  }
    // Configuramos o posicionamento da tela, fonte e o alinhamento do texto
  displayConfig();
 


  // Iniciamos o lora
  if (!loraBegin())
  {
    // Se não deu certo, exibimos falha de lora na serial
    // Serial.println("LoRa failed!");
    // E deixamos em loop infinito
    while (1);
  }
  Wire1.begin(SDA, SCL);
  accel.setI2CAddr(0x1C); //change your device address if necessary, default is 0x1C
  accel.dataMode(true, 2); //enable highRes 10bit, 2g range [2g,4g,8g]
}
void loop()
{
  digitalWrite(17, HIGH);
  int sleepPin;
  sleepPin = digitalRead(13);
  //Serial.println(sleepPin);
  if (sleepPin == pin0) {
    delay(100);
   esp_deep_sleep_start();
  }
  //Update de dados do Acelerometro.
  char acel[100];
  accel.update();
  int aX = accel.x();
  int aY = accel.y();
  int aZ = accel.z();
  int vAy = sqrt(pow(aY, 2));
  int vAz = sqrt(pow(aZ, 2));

  if (vAy > limiteGraus || vAz > limiteGraus) {
    digitalWrite(25, HIGH);
  } else {
    if(contTempo != 0){
      contTempo -= 1;
    }
   
    statusSys = 1;
    digitalWrite(25, LOW);
  }
  if (contTempo > tempo && contTempo < tempo + 1) {
    statusSys = 2;
  }
  if (contTempo > tempo + 1) {
    contTempo = tempo + 1;
    statusSys = 3;
  }
  counter++;

  // debug do acelerometro
  sprintf(acel, "x:%dy:%dz:%d", aX, aY, aZ);
  sprintf(dados, "Mod:%dBat:%dSta:%dD:%d", Mod, batLevel, statusSys, counter);
  EnviaLoRa(dados);
  Serial.print(acel);
  Serial.print(" ");
  Serial.println(dados);
  //debug
  display.clear();
  display.drawString(0, 10, "Dados" + String(counter) + " Button :" + String(sleepPin));
  display.drawString(0, 30, acel);
  display.drawString(0, 20, dados);
  display.drawString(0, 40, "T" + String(contTempo) + "Sta" + String(statusSys));
  display.display();

  if (counter == 200) {
    counter = 0;
  }

  delay(80);

}
