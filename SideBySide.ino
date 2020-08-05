#include <ESP8266WiFi.h>
/*#include <WiFiClientSecure.h>*/
#include <WiFiClient.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Pushsafer.h>

int repeat = -1;
int batStat = -1;
float batVolt;
// Pushsafer private or alias key
#define PushsaferKey "XXXXXXXXXX"

/*WiFiClientSecure client;*/
WiFiClient client;
Pushsafer pushsafer(PushsaferKey, client);
const int pin = 5;

void blink() {
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN,HIGH);
}
void setup() {
  
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    res = wm.autoConnect("SideBySide","password"); // password protected ap
 
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("Connect");
    }


  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pushsafer.debug = true;

  // Take a look at the Pushsafer.com API at
  // https://www.pushsafer.com/en/pushapi
  
  struct PushSaferInput input;
  input.message = "Connected!";
  input.title = "Congrats!";
  input.sound = "8";
  input.vibration = "1";
  input.icon = "125";
  input.iconcolor = "#FFCCCC";
  input.priority = "1";
  input.device = "a";
  input.url = "";
  input.urlTitle = "";
  input.picture = "";
  input.picture2 = "";
  input.picture3 = "";
  input.time2live = "";
  input.retry = "";
  input.expire = "";
  input.answer = "";

  Serial.println(pushsafer.sendEvent(input));
  pinMode(pin,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.println("Sent");
  digitalWrite(LED_BUILTIN,LOW);
  // client.stop();

}

void loop() {
  
  delay(1000);
  batVolt = analogRead(A0)*0.00452355;
  if (batVolt < 3.3) {
    blink();
  }
  if (batVolt < 3.3 && batStat == -1){
  struct PushSaferInput input;
  input.message = "Low Battery!";
  input.title = "ALERT!";
  input.sound = "12";
  input.vibration = "3";
  input.icon = "2";
  input.iconcolor = "#FF0000";
  input.priority = "2";
  input.device = "a";
  input.url = "";
  input.urlTitle = "";
  input.picture = "";
  input.picture2 = "";
  input.picture3 = "";  
  input.time2live = "";
  input.retry = "";
  input.expire = "";
  input.answer = "";

  Serial.println(pushsafer.sendEvent(input));
  Serial.println("COMPROMISED");
  batStat = 0;  
    }
    
  else if(batVolt > 3.3 && batStat == 0) {
    batStat = -1;   
  }
  
  if( digitalRead(pin)== LOW && repeat == -1) {
  struct PushSaferInput input;
  input.message = "Alert, deviced compromised!";
  input.title = "ALERT!";
  input.sound = "52";
  input.vibration = "3";
  input.icon = "5";
  input.iconcolor = "#FF0000";
  input.priority = "2";
  input.device = "a";
  input.url = "";
  input.urlTitle = "";
  input.picture = "";
  input.picture2 = "";
  input.picture3 = "";  
  input.time2live = "";
  input.retry = "";
  input.expire = "";
  input.answer = "";

  Serial.println(pushsafer.sendEvent(input));
  Serial.println("COMPROMISED");
  repeat = 0;  
  }
  
  else if (digitalRead(pin) == HIGH && repeat == 0) {
  struct PushSaferInput input;
  input.message = "Device Restablished!";
  input.title = "Device On";
  input.sound = "51";
  input.vibration = "3";
  input.icon = "95";
  input.iconcolor = "#00FF00";
  input.priority = "2";
  input.device = "a";
  input.url = "";
  input.urlTitle = "";
  input.picture = "";
  input.picture2 = "";
  input.picture3 = "";  
  input.time2live = "";
  input.retry = "";
  input.expire = "";
  input.answer = "";
  Serial.println(pushsafer.sendEvent(input));
  Serial.println("SET");
  repeat = -1;
  }
}
