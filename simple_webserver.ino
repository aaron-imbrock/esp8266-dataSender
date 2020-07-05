#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <FS.h>
#include <ArduinoJson.h>

#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
// current temperature & humidity
float t = 0.0;
float h = 0.0;

ESP8266WebServer server;
 
const char* device_id = WiFi.macAddress();c
const char* ssid = "";
const char* password = "";

void setup()
{
	dht.begin();
	WiFi.begin(ssid, password);
	Serial.begin(115200);
	while(WiFi.status()!=WL_CONNECTED)
	{
		Serial.print(".");
		delay(1000);
	}
	Serial.println("");
	Serial.print("IP Address: ");
	Serial.print(WiFi.localIP());

	server.on("/",[]{server.send(200, "text/plain", String(t).c_str());});
	// TODO: Print output of all values to /
	// NEXT: Line 36 only temp gets printed, add humidity
	// [] Build json object with all values; post to server
	// [] create config file with SSID and pw. Check if exists and read values from there
	// [] Deep sleep
	server.begin();
}

void loop()
{
	float newT = dht.readTemperature();
	float newH = dht.readHumidity();
	if (isnan(newT)) 
	{
		Serial.println("Failed to read from DHT Sensor...");
		delay(1000);
	}
	else
	{
		h = newH;
		t = newT;
		Serial.println("");
		Serial.printf("Humidity: %f \n", h);
		Serial.printf("Temperature: %f \n", t);
		delay(5000);
	}
	
	server.handleClient();
}
