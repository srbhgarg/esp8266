#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "TELUS2260";
const char* password = "2sc72yu6c2";
//const char* ssid = "SMG92";
//const char* password = "txtb5270";


const char* mqtt_server = "broker.hivemq.com"; //musquitto: "37.187.106.16";
const char* clientID = "NodeMCUHome";

const char* outTopic = "Freehome/lightsout";
const char* inTopic = "Freehome/lightsin";
int pinNum = 12;

// To connect to wifi
WiFiClient espClient;
// To connect to mqtt server
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Status: WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
   // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Trying MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...

      //PUBLISH the outtopic to tell that we are connected
      client.publish(outTopic, "0");
      
      //Listen to inTopic for updates
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
      
    }
  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  // Conver the incoming byte array to a string
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;

  Serial.print("Message arrived on topic: [");
  Serial.print(topic);
  Serial.print("], ");
  Serial.println(message);
 // MQTT message received, write functionality here
 if ((char)payload[0] == '0') {
    digitalWrite(pinNum, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.println("Lights -> LOW");
  } else if ((char)payload[0] == '1') {
    digitalWrite(pinNum, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("Lights -> HIGH");
  } 
 
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(pinNum, OUTPUT);
 //WIFI
  setup_wifi();    
//MQTT server
  client.setServer(mqtt_server, 1883); //1883 unencrypted
  client.setCallback(callback); // this function will be called when msg arrives
}



void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(WiFi.localIP());
if(1)
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
}
