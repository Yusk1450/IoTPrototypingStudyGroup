
boolean wifiConnection(String SSIDString,String PasswordString){
   Serial.println();
    Serial.println();
    
    Serial.printf("[SETUP] WAIT ...\n");
    delay(1000);

    SSIDString.trim();
    PasswordString.trim();
    char ssid[SSIDString.length()+1];
    char passwd[PasswordString.length()+1];
    SSIDString.toCharArray(ssid,SSIDString.length()+1);
    PasswordString.toCharArray(passwd,PasswordString.length()+1);
    Serial.print("[");
    Serial.print(ssid);
    Serial.print("]");
    Serial.print("[");
    Serial.print(passwd);
    Serial.println("]");
    wifiMulti.addAP(ssid, passwd);
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connecting....");
      delay(500);
    }
    Serial.println("Wifi connection check ....OK!");

    while(!httpGet("http://www.google.com/")) {
      //Serial.print(".");
      delay(100);
      Serial.println("Get access test ....NG!");
      while (wifiMulti.run() != WL_CONNECTED) {
        Serial.println("Wifi  reconnecting....");
        delay(500);
      }
      Serial.println("Wifi connection check ....OK!");
    }
    Serial.println("Get access test ....OK!");
    //delay(1000); 
    return true; 
}


boolean httpPost(String urlStr,String postStr) {
  urlStr.trim();
  postStr.trim();
  boolean rtnCode =false;
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        Serial.print("WiFi:[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin(urlStr); //HTTP
        http.setTimeout(100000);
        http.addHeader("Host", "www.google.com");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.setUserAgent("Arduino Post Client");
        
        http.addHeader("Connection", "close");
        
        Serial.print("WiFi:[HTTP] POST...\n");
        // start connection and send HTTP header
        int contLen = postStr.length();
        char postCharArry[contLen + 1];

        postStr.toCharArray(postCharArry,contLen + 1);

        int httpCode = http.POST((uint8_t*)postCharArry,contLen);

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("WiFi:[HTTP] POST... Response code: %d\n", httpCode);

            // file found at server
            String payload = http.getString();
            Serial.print("payload: ");
            Serial.println(payload);
            if(httpCode == HTTP_CODE_OK) {
//                String payload = http.getString();
//                Serial.print("payload: ");
//                Serial.println(payload);
                rtnCode =true;
            }
        } else {
            Serial.println(httpCode);
            Serial.printf("WiFi:[HTTP] POST... failed, error: %d %s\n", httpCode ,http.errorToString(httpCode).c_str());
            
        }

        http.end();
    }
    else{
      Serial.println("WiFi:wifiMulti.run() != WL_CONNECTED)");
    }

    delay(100);
    return rtnCode;
}



boolean httpGet(String urlStr) {
  urlStr.trim();
  boolean rtnCode =false;
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        Serial.print("Test:[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        Serial.print("[");
        Serial.print(urlStr);
        Serial.print("]");
        http.begin(urlStr); //HTTP

        Serial.print("Test:[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("Test:[HTTP] GET... Response code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                //String payload = http.getString();
                //Serial.println(payload);
                Serial.println("Test:[HTTP] GET... HTTP_CODE_OK");
                rtnCode =true;
            }
        } else {
            Serial.printf("Test:[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            
        }

        http.end();
    }

    delay(100);
    return rtnCode;
}
