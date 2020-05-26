
/* Create a WiFi access point and provide, a web server and control builtin led */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "IFMG4.0";
const char *password = "password";

ESP8266WebServer server(80);

//String page = "";
int LEDPin = 13;

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */

String page = "<h1>IFMG 4.0 NODEMCU WebServer</h1><p><a href=\"LEDOn\"><button style='height:200px;width:200px;font-size:50px;'>ON</button></a>&nbsp;<a href=\"LEDOff\"><button style='height:200px;width:200px;font-size:50px;'>OFF</button></a></p><p><a href=\"LEDblink\"><button style='height:200px;width:400px;font-size:50px'>BLINK</button></a></p>";

void handleRoot() {
  server.send(200, "text/html", page);

	//server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {

    //the HTML of the web page
  //page = "<h1>IFMG 4.0 NODEMCU WebServer</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>&nbsp;<a href=\"LEDblink\"><button>BLINK</button></a></p>";
  //make the LED pin output and initially turned off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
    // initialize digital pin LED_BUILTIN as an output.


	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
	//server.begin();



  //server.on("/", [](){
    //server.send(200, "text/html", page);
 // });

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

	Serial.println("HTTP server started");

}

void loop() {
	server.handleClient();

}
