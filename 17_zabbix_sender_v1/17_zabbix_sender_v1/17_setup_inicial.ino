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

