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
      zabbix += String("\"value\":") + String("\"") + String(valores(i)) + String("\"")+String(",");
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
        String resposta = client.readStringUtil('\r');
        Serial.print("Resposta do Zabbix: ");
        Serial.print(resposta);
        Serial.print("\n\n");
        
      }    
    }

    client.stop();
  }
}

