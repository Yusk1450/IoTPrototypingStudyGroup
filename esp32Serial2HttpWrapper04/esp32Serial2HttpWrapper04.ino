/*
 * ESP32を用いたシリアル通信-HTTP通信の中継スケッチ 
 * フォーマットに従ったシリアル通信で受け取った文字列を分割し、
 * Wifi接続、HTTPのGETリクエストを行う。(POSTリクエストは仮対応)
 */

/* 文字列のスプリット関数は以下のサイトを参考にしました。
 * http://robotastics.wpblog.jp/技術情報/arduino/文字列を区切り文字で分割するsplit関数/
*/

//配列の個数を求めるマクロ
#define SIZE_OF_ARRAY(ary)  (sizeof(ary)/sizeof((ary)[0]))

HardwareSerial mySerial2(2); // UART2 (RX=16, TX=17)
char buff[512];
int buffLength = 512;
int counter=0;
boolean flug;

//WiFi//////////////////////////
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
//////////////////////////WiFi//

//WiFi//////////////////////////
WiFiMulti wifiMulti;
//


//その他//////////////////////////
uint8_t mac0[6];        //号機情報用MACアドレス
String strMacAddress;   //号機情報用MACアドレス文字列


//文字分割関数//////////////////////////
/// <summary>
/// 入力した文字列を区切り文字で分割する
/// </summary>
/// <param name="*result">分割された文字列を格納する配列</param>
/// <param name="resultsize">配列の要素数</param>
/// <param name="data">区切り文字を含む文字列のアドレス</param>
/// <param name="delimiter">区切り文字</param>
/// <returns>成功で配列の要素数，要素数不足の場合は-1</returns>
int split(String *result, size_t resultsize, String data, char delimiter){
    int index = 0;
    int datalength = data.length();
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            if ( index > (resultsize - 1)) return -1;
        }
        else result[index] += tmp;
    }
    return (index + 1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp_efuse_mac_get_default(mac0);
  char tempAdd[18];
  sprintf(tempAdd,"%02X:%02X:%02X:%02X:%02X:%02X", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);
  strMacAddress = String(tempAdd);
  Serial.println("Default Mac Address =" + strMacAddress);

  
  mySerial2.begin(9600);
  counter=0;
  flug = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //シリアル受信
  while (mySerial2.available()) {
    Serial.print("#");
    buff[counter] = mySerial2.read();
    // 文字数buffLength以上 or 改行
    if (counter > buffLength || buff[counter] == '\n') {
      buff[counter] = '\0';
      counter=0;
      flug = true;
    }
    else {
      counter++;
    }
  }
  
  if(flug){
    String msg = String((char*) buff);                      //区切り文字を含む文字列(受信文字列)
    int index;                                              //分割後の要素数
    String splitstring[10] = {"\0"};                        //分割された文字列を格納する配列
    size_t arraysize = SIZE_OF_ARRAY(splitstring);          //配列の要素数      
    char delimiter = ',';                                   //区切り文字
    index = split(splitstring, arraysize, msg, delimiter); 
    Serial.println("");
    Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$");
    Serial.println(msg);
    Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$");
    for(int i=0;i<index;i++){
      //Serial.println("");
      Serial.println(splitstring[i]);
      Serial.println("/////////////////////////");
    }

    if(splitstring[0] == "S"){
      if(wifiConnection(splitstring[1],splitstring[2])){
        mySerial2.write("1");
      }
      else{
        mySerial2.write("0");
      }
      
    }
    else if(splitstring[0] == "G"){
      if(httpGet(splitstring[1])){
        mySerial2.write("1");
      }
      else{
        mySerial2.write("0");
      }
      
    }
    else if(splitstring[0] == "P"){
      if(httpPost(splitstring[1],splitstring[2])){
        mySerial2.write("1");
      }
      else{
        mySerial2.write("0");
      }
      
    }

    flug= false;
  } 
  else{
    Serial.print(".");
    delay(1000);
  }

}
