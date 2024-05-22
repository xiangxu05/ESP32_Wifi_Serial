#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include "libraries/usercommon.h"
#include "libraries/rootHtml.h"
#include <Preferences.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


bool connected = 0;
WebServer webserver(80);
Preferences preferences;

void handleRoot(){

  preferences.begin("settings", false);
  String sta_ssid = preferences.getString("ssid", "znlhsys_2.4G");
  String sta_password = preferences.getString("password", "znlhsysbistu");
  String ssid_name = preferences.getString("SSID_NAME", "ESP_TCPserverV1");
  String security_key = preferences.getString("SECURITY_KEY", "12345678.");
  String local_ip = preferences.getString("LOCAL_IP", "192.168.1.1");
  String subnet = preferences.getString("SUBNET", "255.255.255.0");
  String gateway = preferences.getString("GATEWAY", "192.168.1.1");
  String server_port = preferences.getString("SERVERPORT", "8899");

  preferences.end();
  // 格式化HTML页面
  const int bufferSize = 4096;
  char html[bufferSize];

  // 使用 snprintf 格式化HTML内容
  snprintf(html, bufferSize, root_html,
           sta_ssid.c_str(), sta_password.c_str(), ssid_name.c_str(), security_key.c_str(),
           local_ip.c_str(), subnet.c_str(), gateway.c_str(), server_port.c_str());

  webserver.send(200, "text/html", html);

}

void handleSubmit() {
  if (webserver.hasArg("ssid") && webserver.hasArg("password") && webserver.hasArg("apSSID") &&
      webserver.hasArg("apPassword") && webserver.hasArg("localIP") && webserver.hasArg("subnetMask") &&
      webserver.hasArg("gateWay") && webserver.hasArg("tcpPort")) {
    
    preferences.begin("settings", false);
    // 存储每个参数到Preferences
    preferences.putString("ssid", webserver.arg("ssid"));
    preferences.putString("password", webserver.arg("password"));
    preferences.putString("SSID_NAME", webserver.arg("apSSID"));
    preferences.putString("SECURITY_KEY", webserver.arg("apPassword"));
    preferences.putString("LOCAL_IP", webserver.arg("localIP"));
    preferences.putString("SUBNET", webserver.arg("subnetMask"));
    preferences.putString("GATEWAY", webserver.arg("gateWay"));
    preferences.putString("SERVERPORT", webserver.arg("tcpPort"));

    // // 输出调试信息到串口监视器
    // Serial.println("Form submitted:");
    // Serial.println("SSID: " + preferences.getString("ssid", "znlhsys_2.4G"));
    // Serial.println("Password: " + preferences.getString("password", "znlhsysbistu"));
    // Serial.println("AP SSID: " + preferences.getString("SSID_NAME", "ESP_TCPserverV1"));
    // Serial.println("AP Password: " + preferences.getString("SECURITY_KEY", "12345678."));
    // Serial.println("Local IP: " + preferences.getString("LOCAL_IP", "192.168.1.1"));
    // Serial.println("Subnet Mask: " + preferences.getString("SUBNET", "255.255.255.0"));
    // Serial.println("Gate Way: " + preferences.getString("GATEWAY", "192.168.1.1"));
    // Serial.println("TCP Port: " + preferences.getString("SERVERPORT", "8899"));
    
    preferences.end();

    // 重定向回根页面
    webserver.sendHeader("Location", "/");
    webserver.send(303);
  } else {
    webserver.send(400, "text/plain", "Missing required parameters");
  }

  // // 返回成功页面
  // server.send(200, "text/html", "<html><body><h1>Configuration Saved! Rebooting...</h1></body></html>");

  // // 重启ESP32以应用新配置
  // delay(2000); // 等待2秒，让用户看到成功消息
  // ESP.restart();
}

int WIFI_SET_APSTA(String ssid_name,String security_key,String ssid,String password,String local_ip,String subnet,String gateway)
{
	long iRetval = -1;
	WiFi.mode(WIFI_AP_STA);

  IPAddress lan_gateway,lan_subnet,lan_ip;
  lan_ip.fromString(local_ip);
  lan_gateway.fromString(gateway);
  lan_subnet.fromString(subnet);
  WiFi.softAPConfig(lan_ip, lan_gateway, lan_subnet);
  WiFi.softAP(ssid_name, security_key, 3, 1);

  iRetval = WiFi.softAP(ssid_name, security_key);
  if (iRetval) {
        Serial.println("Soft AP started successfully.");
        Serial.print("SSID: ");
        Serial.println(ssid_name);
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("Failed to start Soft AP.");
        return WIFIAPUNBUILED;
    }
	
	// 连接到WiFi网络
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    
    Serial.println();
    Serial.print("WiFi connected. Local IP address: ");
    Serial.println(WiFi.localIP());
  return iRetval;
}
 
static void TCP_Server_user()
{
	String RxBuff;
  String txBuff;
  if(connected == 0){
    Serial.println("\nwaiting for connect...");
    connected = 1;
  }
	client=server.available();
	if(client)
	{
		Serial.println("get client,welcome!");
		while(client.connected()||client.available()||Serial.available())
		{
			if(client.available())
			{ 
        while(client.available()){
          char c=client.read();
          RxBuff +=c;
        }
        if(RxBuff == "AT_DigitalUp"){
          digitalWrite(18, HIGH);
          digitalWrite(19, HIGH);
        }else if(RxBuff == "AT_DigitalDown"){
          digitalWrite(18, LOW);
          digitalWrite(19, LOW);
        }else{
          Serial.print(RxBuff);
        }
        RxBuff="";
			}
      if(Serial.available()){
        while(Serial.available()){
          char c=Serial.read();
          txBuff+=c;
        }
        if(txBuff == "AT_DigitalUp"){
          digitalWrite(18, HIGH);
          digitalWrite(19, HIGH);
        }else if(txBuff == "AT_DigitalDown"){
          digitalWrite(18, LOW);
          digitalWrite(19, LOW);
        }
        else{
          client.print(txBuff);
        }
        txBuff="";
      }
		}
    client.stop();
    connected = 0;
    Serial.println("no clinet now.");
	}
}

void DisplayBanner(char const *AppName)
{
	Serial.printf("\n\n\n\r");
	Serial.printf("\t\t *************************************************\n\r");
	Serial.printf("\t\t         %s Application            \n\r", AppName);
	Serial.printf("\t\t *************************************************\n\r");
	Serial.printf("\n\n\n\r");
}

// void handleWebServer(void* pvParameters) {
//   while (true) {
//     webserver.handleClient();
//     delay(10); // 可选的延迟，以避免占用过多 CPU 时间
//   }
// }

void setup()
{
	Serial.begin(115200);
	delay(100);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  digitalWrite(18, LOW);
  digitalWrite(19, LOW);
  // 打开 NVS
  preferences.begin("settings", false);

  String ssid_name = preferences.getString("SSID_NAME", "ESP_TCPserverV1");
  String security_key = preferences.getString("SECURITY_KEY", "12345678.");
  String sta_ssid = preferences.getString("ssid", "znlhsys_2.4G");
  String sta_password = preferences.getString("password", "znlhsysbistu");
  String local_ip = preferences.getString("LOCAL_IP", "192.168.1.1");
  String subnet = preferences.getString("SUBNET", "255.255.255.0");
  String gateway = preferences.getString("GATEWAY", "192.168.1.1");
  String server_port = preferences.getString("SERVERPORT", "8899");

  // 关闭 NVS
  preferences.end();

	DisplayBanner(BOARD_NAME);
	Serial.println("\t\t********wifi building**********\r\n");
	if (WIFI_SET_APSTA(ssid_name,security_key,sta_ssid,sta_password,local_ip,subnet,gateway))
	{
		Serial.println("\t\t********wifi builded**********\r\n");
		server.begin((uint16_t)server_port.toInt());
		Serial.print("server listening port:");
		Serial.print((uint16_t)server_port.toInt());
	}

  webserver.on("/", handleRoot);
  webserver.on("/submit", handleSubmit);
  webserver.onNotFound([](){webserver.send(200,"text/html;charset=utf-8","没有找到页面！");});
  webserver.begin();

  //xTaskCreate(TCP_Server_user, "TCP_Server_user", 4096, NULL, 1, NULL);
  //xTaskCreate(handleWebServer, "handleWebServer", 4096, NULL, 2, NULL);
}
 
void loop()
{
	TCP_Server_user();
  webserver.handleClient();
}