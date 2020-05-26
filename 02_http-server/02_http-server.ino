#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "www.vhtel.eng.br";
const char* password = "password";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
int LEDPin = 13;
void setup(void){
  //the HTML of the web page
  page = "<h1>IFMG 4.0 NODEMCU WebServer</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>&nbsp;<a href=\"LEDblink\"><button>BLINK</button></a></p>";
  //make the LED pin output and initially turned off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
    // initialize digital pin LED_BUILTIN as an output.

  delay(1000);
  Serial.begin(115200);
  WiFi.hostname("IFMG4001");
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }



  delay(1000);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname ");
  Serial.println(WiFi.hostname());
  delay(1000);


  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED ON");
    delay(1000);
  });
  server.on("/LEDOff", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED OFF");
    delay(1000);
  });
  server.on("/LEDblink", [](){
    server.send(200, "text/html", page);
    //IF LOW
    if (digitalRead(LED_BUILTIN)==0){
      Serial.println("Entered IF LOW - LED ON");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
    //IF HIGH
     if (digitalRead(LED_BUILTIN)==1){
      Serial.println("Entered IF HIGH - LED OFF");
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
    }
    //digitalWrite(LED_BUILTIN, LOW);
    //delay(200);
    //digitalWrite(LED_BUILTIN, HIGH);
    //delay(200);
  });
  server.begin();
  Serial.println("Web server started!");
}

void loop(void){
  server.handleClient();
  //Serial.println(digitalRead(LED_BUILTIN));
  //Serial.print(digitalWrite(LED_BUILTIN()));
}
