#include <WiFi.h>
#include <esp_now.h>
uint8_t rec[6] = {0x5C, 0x01, 0x3B, 0x47, 0x5F, 0x84};

typedef struct msg{
    char 
    data[32];
    int throttle;
}msg;
msg message;
esp_now_peer_info_t peerInfo;
void OnDatasent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Last Packet Send Status: ");
    Serial.println(String(status));
    if(status == ESP_NOW_SEND_SUCCESS){
        Serial.println("Delivery Success");
    }
    else{
        Serial.println("Delivery Fail");
    }
}
//upload this
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_err_t status = esp_now_init();
    if (status != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else {
        Serial.println("Transmission beginning");
    }
    memcpy(peerInfo.peer_addr, rec, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if(esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    esp_now_register_send_cb(OnDatasent);
}

void loop(){
    memcpy(message.data,"the transmission is on going",sizeof(message));

    message.throttle=1234;
    esp_err_t result = esp_now_send(rec, (uint8_t *) &message,sizeof(message));
    if (result == ESP_OK) {
        Serial.println("Message sent");
    }
    else {
        Serial.println("Error sending the message");
    
    }
    delay(2000);

}