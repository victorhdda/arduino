#include <ESP8266WiFi.h>

float variavel1;
float variavel2;




const char* ssid = "ifmg4.0";
const char* password = "password";
const char* server = "192.168.43.30";
int porta = 10051;
WiFiClient client;

long previousMillis = 0;
long interval = 10000;


void setup() {
  // put your setup code here, to run once:
  config_inicial();

}

void loop() {
  // put your main code here, to run repeatedly:


  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval){
    previousMillis = currentMillis;

    variavel1 +=1;
    variavel2 +=1;

    zabbix_sender();
  }
}


void config_inicial(){

  Serial.begin(115200);

  Serial.print("Conectando na rede");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print (WiFi.localIP());
  Serial.println();
}







void zabbix_sender(void){
  String host = "nodemcu";
  String itens[] = {"variavel1","variavel2"};
  float valores[] = {variavel1, variavel2};

  for (int i=0;i<(sizeof(valores)/sizeof(int));i++){

    if (client.connect(server,porta)){

      Serial.print("Conectado no Zabbix!\n");
      String zabbix = "";

      zabbix += String("{\"request\":\"sender data\", \"data\":");
      zabbix += String("[{");
      zabbix += String("\"host\":") + String("\"") + String(host) + String("\"")+String(",");
      zabbix += String("\"key\":") + String("\"") + String(itens[i]) + String("\"")+String(",");
      zabbix += String("\"value\":") + String("\"") + String(valores[i]) + String("\"");
      zabbix += String("}]}");

      Serial.print("Post via Zabbix Sender: ");
      Serial.print(zabbix);

      client.print(zabbix);

      zabbix = "";


      unsigned long timeout = millis();
      while (client.available() == 0){
        if (millis()-timeout >5000){
          Serial.println(">>> Timeout !");
          client.stop();
          return;
        }

      }

      while (client.available()){
        String resposta = client.readStringUntil('\r');
        Serial.print("Resposta do Zabbix: \n");
        Serial.print(resposta);
        Serial.print("\n\n");

      }
    }

    client.stop();
  }
}
