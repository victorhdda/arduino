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

const char* ssid = "ifmg4.0";
const char* password = "password";
ESP8266WebServer server(80);

//String page = "";
int LEDPin = 13;

String page = "<h1>IFMG 4.0 Controle do Portao Eletronico-teste</h1><p><a href=\"LEDblink\"><button style='height:200px;width:400px;font-size:50px'>ABRIR/FECHAR</button></a></p>";

void handleRoot() {
  server.send(200, "text/html", page);

  //server.send(200, "text/html", "<h1>You are connected</h1>");
}



void setup() {
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

  WiFi.mode(WIFI_STA); //set wifi mode to station only
  WiFi.begin(ssid, password); //connect to wifi

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


  server.on("/", handleRoot);
  server.on("/LEDblink", []() {
    server.send(200, "text/html", page);
    digitalWrite(4, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED ON");
    delay(1000);
    digitalWrite(4, HIGH);
    Serial.println("LED ON");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);

  });

  server.begin();

  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
}
