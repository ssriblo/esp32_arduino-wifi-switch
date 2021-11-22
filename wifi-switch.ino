#include <WiFi.h>
#include <Preferences.h>

#define ATTEMP_MS 500

#define SECRET_SSID         ""
#define SECRET_PASSWORD     "" 

#define SECRET_SSID2         ""
#define SECRET_PASSWORD2     "" 
String wifiTag[] = {"WL_IDLE_STATUS", "WL_NO_SSID_AVAIL", "WL_SCAN_COMPLETED", "WL_CONNECTED", "WL_CONNECT_FAILED", "WL_CONNECTION_LOST", "WL_DISCONNECTED"};
wl_status_t wifiStatus;
typedef enum {
  DISCONNECT_FIRST  = 0,
  RECONNECT_FIRST   = 1,
  WITHOUT_BEGIN     = 2
} wifi_setup_mode;

Preferences preferences;
const int dummy = 0;

void wifi_setup(char* ssid, char* password, wifi_setup_mode mode, char* dotPrompt, int attemps){
  int wifi_try_cntr = 0;
  if(DISCONNECT_FIRST == mode){
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF); // отключаем WIFI   
  }
  if(RECONNECT_FIRST == mode) {
    WiFi.reconnect();
    Serial.println("Try Reconnect");
  }
  if(WITHOUT_BEGIN == mode){
    // nothing todo    
  }else{
    WiFi.mode(WIFI_OFF); 
    WiFi.begin(ssid, password);
  }
  wifiStatus = WiFi.status();
  Serial.println(wifiTag[wifiStatus]);
  wifi_try_cntr = 0;
  while((WiFi.status() != WL_CONNECTED) && (wifi_try_cntr < attemps) ) {
    delay(ATTEMP_MS);
    wifi_try_cntr++;
    Serial.print(dotPrompt);
  }
  Serial.print("Connecting time, s:  ");
  Serial.println(float(wifi_try_cntr)*(float(ATTEMP_MS)/1000.0));
  wifiStatus = WiFi.status();
  Serial.println(wifiTag[wifiStatus]);
  Serial.print("Connected-1 to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP()); 
  // RSSI print does not work, let dismiss it
}

void setup() {
  int wifi_try_cntr = 0;
  
  Serial.begin(115200);

  while(1){
    Serial.println("\n************** START ****************");
    wifi_setup(SECRET_SSID, SECRET_PASSWORD, DISCONNECT_FIRST, ".", 50);
    delay(1000);
    wifi_setup(SECRET_SSID2, SECRET_PASSWORD2, DISCONNECT_FIRST, ",", 50);
    break;
  }
}

void loop() {  
  int wifi_try_cntr = 0;

  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) ) {
    wifi_setup(SECRET_SSID2, SECRET_PASSWORD2, WITHOUT_BEGIN, ",", 10);
    wifi_setup(SECRET_SSID2, SECRET_PASSWORD2, WITHOUT_BEGIN, ",", 10);
    wifi_setup(SECRET_SSID2, SECRET_PASSWORD2, RECONNECT_FIRST, ",", 50);

  }else{
    wifiStatus = WiFi.status();
    Serial.println(wifiTag[wifiStatus]);
  }
  delay(2000);
}
