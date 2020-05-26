/*
The MIT License (MIT)
Copyright (c) 2015 Pedro Minatel
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>

const char* ssid = "ifmg4.0";
const char* password = "password";

ESP8266WebServer server(80);

String page = "";
int LEDPin = 13;

IRsend irsend(14); //FUNÇÃO RESPONSÁVEL PELO MÉTODO DE ENVIO DO SINAL IR / UTILIZA O GPIO14(D5)



void handleRoot() {
  server.send(200, "text/html", page);

  //server.send(200, "text/html", "<h1>You are connected</h1>");
}



int tamanho = 68; //TAMANHO DA LINHA RAW(68 BLOCOS)
int frequencia = 32; //FREQUÊNCIA DO SINAL IR(32KHz)

// BOTÃO LIGA / DESLIGA
unsigned int LD[84] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,4700,250,1450,1000,600,1000,3850,250,650,1000,1500,950,3050,1000,600,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1400,1050,600,1000,600,1000,600,1000,600,1000,1400,1000,2200};
// BOTÃO MODE (seleciona funções (modos) pré-programadas (auto-cool-dry-heat)
unsigned int MODE[72] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,1450,1000,4700,250,1450,1000,600,1000,3850,250,6400,250,6400,250,6400,250,6400,250,4750,1000,1400,1050,600,1000,3850,1000,600,1000,600,1000,550};
// BOTÃO TIME (Definição de tempo definido de funcionamento)
unsigned int TIME[76] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,3900,1000,600,1000,1400,250,1450,1000,4700,250,1450,1000,600,1000,3850,250,6400,250,6400,250,650,1000,600,1000,4700,250,6400,250,4750,1000,1400,1050,3850,1000,600,1000,600,1000,600,1000,550};
// BOTÃO DOWN (Diminui a temperatura em um grau C)
unsigned int DOWN[86] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,4700,250,1450,1000,600,1000,3850,250,650,1000,1400,1000,3850,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1400,1050,600,1000,600,1000,600,1000,600,1000,1400,1000,600,1000,600,1000,550};
// BOTÃO UP (Aumenta a temperaura um grau C)
unsigned int UP[80] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,4700,250,650,1000,1400,1000,3850,250,650,1000,1400,1000,3850,250,6400,250,6400,250,6400,250,3100,1000,3050,1050,1400,1000,600,1000,600,1000,2250,1000,600,1000,550};
// BOTÃO FAN (Ventilação do ar parado)
unsigned int FAN[78] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,4700,250,1450,1000,600,1000,3850,250,6400,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1400,1050,1400,1000,1400,1000,1400,1000,600,1000,600,1000,550};
// BOTÃO SLEEP (Variação definida de tepertura, o que proporciona uma noite melhor)
unsigned int SLP[82] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,4700,250,1450,1000,600,1000,3850,250,650,1000,5500,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1400,1050,600,1000,600,1000,1400,1000,1400,1000,600,1000,600,1000,550};
// BOTÃO DISPLAY (Acende/apaga o display)
unsigned int DPLY[86] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,2250,1000,2250,250,1450,1000,600,1000,3850,250,650,1000,5500,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1400,1050,600,1000,600,1000,1400,1000,600,1000,600,1000,600,1000,600,1000,550};
// BOTÃO HEALTH (Limpeza e purificação do ar, através de filtragens especiais)
unsigned int HLTH[84] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,1050,1400,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,600,1000,600,250,650,1000,600,1000,2250,1000,2250,250,1450,1000,600,1000,3850,250,650,1000,5500,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1400,1050,600,1000,600,1000,1400,1000,600,1000,600,1000,2200};
// BOTÃO TURBO (Resfriamento instantâneo do ambiente)
unsigned int TBO[68] = {}; // LINHAS RAWS INDISPONIVEIS
// BOTÃO SWING LATERAL ( Alteração na direção de distribuição, através da manipulação LATERAL das paletas)
unsigned int SL[82] = {3100,1600,600,1000,600,1000,3050,1000,1400,250,650,1000,600,1000,1400,1000,2250,1000,600,950,1500,1000,600,1000,2250,1000,1400,250,650,1000,5500,250,6400,250,2300,1000,2250,1000,1400,250,650,1000,600,1000,4700,250,1450,1000,600,1000,3850,250,650,1000,2250,1000,600,1000,600,1000,1400,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1450,1050,600,1000,600,1000,3050,1000,2200};
// BOTÃO SWING VERTICAL ( Alteração na direção de distribuição, através da manipulação VERTICAL das paletas)
unsigned int SV[100] = {3050,1650,550,1050,550,1050,550,300,600,300,600,250,600,1050,550,250,600,300,600,1050,550,1050,550,300,600,1050,550,250,600,250,600,1050,550,1050,550,300,600,1050,550,1050,550,250,600,300,600,1050,550,300,600,300,600,1050,550,300,600,300,600,300,600,250,600,250,600,250,600,300,600,250,600,300,600,250,600,300,600,300,600,300,600,300,600,300,600,250,600,300,600,1050,550,300,600,300,600,1050,550,300,600,300,600};
// BOTÃO ECO (Modo econômico de energia)
unsigned int ECO[100] = {3100,1600,600,1000,600,1000,3050,1000,1400,300,600,1050,600,1000,550,300,600,1050,550,300,600,300,600,1050,550,1050,550,300,600,1050,550,1050,550,300,600,300,600,1050,550,300,600,300,1000,650,2550,500,300,600,300,600,250,600,300,600,250,600,300,3750,1000,300,2250,1000,2250,1000,1400,1050,600,1000,600,1000,4700,250,1450,1000,600,1000,3850,250,650,1000,5500,250,6400,250,6400,250,6400,250,3100,1000,1400,1000,1550,850,650,1000,550,200,50};



void setup() {

  page = "<html><head><style> .button {    background-color: #4CAF50; /* Green */    border: none;    color: white;    padding: 18px 36px;    text-align: center;    text-decoration: none;    display: inline-block;  font-size: 16px;margin: 4px 2px; cursor: pointer;}.button2 {background-color: #008CBA;} /* Blue */.button3 {background-color: #f44336;} /* Red */ .button4 {background-color: #e7e7e7; color: black;} /* Gray */ .button5 {background-color: #555555;} /* Black */</style></head><body><br><h1>IFMG 4.0 Controle do Ar Condicionado</h1></p><br><a href=\"Power\"><button class="button button3">POWER</button><a href=\"Up\"><button class="button ">UP</button><a href=\"Down\"><button class="button ">DOWN</button><BR><a href=\"Mode\"><button class="button button4">MODE</button><a href=\"Eco\"><button class="button">ECO</button><a href=\"Timer\"><button class="button button4">TIMER</button><br><a href=\"Fan\"><button class="button button4">FAN</button><a href=\"Sleep\"><button class="button button4">SLEEP</button><a href=\"Display\"><button class="button button4">DISPLAY</button><br><a href=\"SwingL\"><button class="button button4">SWING ◄► </button><a href=\"SwingV\"><button class="button button4">SWING ▲▼ </button><BR><a href=\"Health\"><button class="button button4">HEALTH</button><a href=\"Turbo\"><button class="button button4">TURBO</button></body></html>";

  Serial.begin(115200);
  delay(1000);

   //prepare LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // prepare GPIO2
  pinMode(4, OUTPUT);
  digitalWrite(4, 1);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  //server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());

  irsend.begin(); //INICIALIZA A FUNÇÃO
  Serial.begin(115200); //INICIALIZA A PORTA SERIAL

    server.on("/", handleRoot);

    server.on("/Power", [](){
    server.send(200, "text/html", page);
        irsend.sendRaw(LD,84,frequencia);
        Serial.println("Comando enviado: Liga / Desliga");
        delay(50);
    }
    server.on("/Up", [](){
    server.send(200, "text/html", page);
        irsend.sendRaw(UP,80,frequencia);
        Serial.println("Comando enviado: UP");
        delay(50);
    }
    server.on("/Down", [](){
    server.send(200, "text/html", page);
        irsend.sendRaw(DOWN,86,frequencia);
        Serial.println("Comando enviado: DOWN");
        delay(50);
    }
    server.on ("/Mode", [](){
    server.send (200, "text/html", page);
        irsend.sendRaw(MODE,72,frequencia);
        Serial.println("Comando enviado: MODE");
        delay(50);
    }
    server.on ("/Eco", [](){
    server.send (200, "text/html", page);
       irsend.sendRaw(ECO,100,frequencia);
        Serial.println("Comando enviado: ECO");
        delay(50);
    }
    server.on ("/Timer", [](){
    server.send (200, "text/html", page);
        irsend.sendRaw(TIME,76,frequencia);
        Serial.println("Comando enviado: TIME");
        delay(50);
    }
    server.on ("/Fan", [](){
    server.send (200, "text/html", page);
       irsend.sendRaw(FAN,78,frequencia);
        Serial.println("Comando enviado: FAN");
        delay(50);
    }
    server.on ("/Sleep", [](){
    server.send (200, "text/html", page);
      irsend.sendRaw(SLP,82,frequencia);
        Serial.println("Comando enviado: SLEEP");
        delay(50);
    }
    server.on ("/Display", [](){
    server.send (200, "text/html", page);
      irsend.sendRaw(DPLY,86,frequencia);
        Serial.println("Comando enviado: DISPLAY");
        delay(50);
    }
    server.on ("/SwingL", [](){
    server.send (200, "text/html", page);
      irsend.sendRaw(SL,82,frequencia);
        Serial.println("Comando enviado: SWING LATERAL");
        delay(50);
    }
    server.on ("/SwingV", [](){
    server.send (200, "text/html", page);
      irsend.sendRaw(SV,100,frequencia);
        Serial.println("Comando enviado: SWING VERTICAL");
        delay(50);
    }
    server.on ("/Health", [](){
    server.send(200, "text/html", page);

  });

  server.begin();

  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
}
