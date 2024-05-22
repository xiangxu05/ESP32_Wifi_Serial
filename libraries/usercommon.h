 #ifndef __USERCOMMON__H__
#define __USERCOMMON__H__
 
// #define BOARD_NAME        "xiangxu's ESP32"
// #define BOARD_VERSION     "1.0.0"
// #define SSID_NAME         "ESP_TCPserverV1"
// #define SECURITY_KEY      "12345678."
// #define LOCAL_IP "192.168.1.1"
// #define SUBNET "255.255.255.0"
// #define GATEWAY "192.168.1.1"
// #define SERVERPORT "8899"

#define ERR_PRINT(x)      Serial.printf("Error [%d] happened \n\r")   
#define WIFIAPBUILDED          1
#define WIFIAPUNBUILED         0
#define MAX_SERVERVE_GETCLIENT 2
#define FOREVER() \
            {\
                while(1); \
            }
#define ASSERT_ON_ERROR(error_code) {\
                 if(error_code < 0) \
                   {\
                        ERR_PRINT(error_code);\
                        return error_code;\
                 }\
            }
bool TCP_SERVER_BUILDED=0;
//IPAddress local_IP(192,168,1,1);
//IPAddress subnet(255,255,255,0);
//IPAddress gateway(192,168,1,1);
const char* ssid     = "znlhsys_2.4G"; // Change this to your WiFi SSID
const char* password = "znlhsysbistu"; // Change this to your WiFi password

// const char* BOARD_NAME = "xiangxu's ESP32";
// const char* SSID_NAME = "ESP_TCPserverV1";
// const char* SECURITY_KEY = "12345678.";

const IPAddress serverIP(192, 168, 19, 108);
//uint16_t serverPort = 8899;
WiFiClient serverclient[MAX_SERVERVE_GETCLIENT];
WiFiClient client;
WiFiServer server;
#endif