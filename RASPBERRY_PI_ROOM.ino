#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EMailSender.h> 
#include <AccelStepper.h> 
// WiFi
const char *ssid = ""; // Enter your WiFi name
const char *password = "";  // Enter WiFi password
// MQTT Broker
const char *mqtt_broker = "192.168.1.28"; // Enter your WiFi or Ethernet IP
const char *topic = "TEST";
const int mqtt_port = 1883;
const int radiator = D5;
const int mains=D6;
const int window=D7;
const int motion=D8;
int detection=0;
int pastmillis=0;
const String heating[6]={"bedroom1h","bedroom2h","livingroomh","kitchenh","officeh","bathroomh"};
const String lighting[6]={"bedroom1","bedroom2","livingroom","kitchen","office","bathroom"};
const String room1="kitchen";
int revolution = 4096*3;  
String past_value="";
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
EMailSender emailSend("esp8266server124@gmail.com", "kfwhpfeyrhusxkck"); 
#define user_email "17cheungw@bluecoatstudent.org.uk"  
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
 Serial.begin(115200);
 stepper.setMaxSpeed(500);
 stepper.setAcceleration(100);
 pinMode(motion,OUTPUT);
 pinMode(radiator, OUTPUT);
 pinMode(mains, OUTPUT);
 pinMode(window,INPUT);
 pinMode(motion,INPUT);
 digitalWrite(radiator, LOW);
 digitalWrite(mains, LOW);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 
 while (!client.connected()) {
 String client_id = "esp8266-client-";
 client_id += String(WiFi.macAddress());
 
 Serial.printf("The client %s connects to mosquitto mqtt broker\n", client_id.c_str());
 
 if (client.connect(client_id.c_str())) {
  Serial.println("Public emqx mqtt broker connected");
 } else {
  Serial.print("failed with state ");
  Serial.print(client.state());
  delay(2000);
 }
}
 client.subscribe(topic);

}
void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");

 String command="";
 for (int i = 0; i < length; i++) {
  Serial.print((char) payload[i]);
  command=command+(char) payload[i];


 }
 if (past_value!=command){
 Serial.println();
 Serial.println(" - - - - - - - - - - - -");
 int index = command.indexOf(' ');
 String state = command.substring(0,index);
 String room = command.substring(index+1, command.length());
 Serial.println(state);
 Serial.println(room);
 int selection=-1;
 int type;
 for (int i=0; i < 6; i++){
   if (heating[i]==room){
     type=0;
     selection=i;
     break;
   }
 }
if (selection==-1){
  for (int i=0; i < 6; i++){ 
  if (lighting[i]==room){
     type=1;
     selection=i;
     break;
  }
}
 Serial.println("room selected: "+lighting[selection]);
}
else{
  Serial.println("room selected: "+heating[selection]);
}


 if (type==0){
      if (room1 == lighting[selection]){
        if (state=="on"){
          Serial.println("heating in "+ room1+ " is on");
          digitalWrite(radiator, HIGH);
          stepper.moveTo(-revolution);
          stepper.runToPosition();
          if (digitalRead(window)==LOW){
            EMailSender::EMailMessage message; 
            message.subject = "HEATING CONDITIONS"; 
            message.message = "the window in "+room1+" is open"; 
            EMailSender::Response resp = emailSend.send(user_email, message);
        } 
        }
        else{
          Serial.println("heating in "+ room1+ " is off");
          digitalWrite(radiator, LOW);
        }
        
      }
  }
else if (type==1){
      if (room1 == lighting[selection]){
        if (state=="on"){
          Serial.println("lights in "+ room1+ " is on");
          digitalWrite(mains, HIGH);
        }
        else{
          Serial.println("lights in "+ room1+ " is off");
          digitalWrite(mains, LOW);
        }
        
      }
  }}
  past_value=command;
  if (digitalRead(motion)==LOW){
    detection=millis()-pastmillis;

  }
  else {
    pastmillis=millis();
    detection=0;
  }
  if (detection>30000){
    digitalWrite(mains,LOW);
    digitalWrite(radiator,LOW);
    String newdata="off kitchen";    
    client.publish("new_data",newdata.c_str());
    EMailSender::EMailMessage message; 
    message.subject = "ACTIVITY IN KITCHEN CLOSED"; 
    message.message = "No motion detected in "+room1+" ,all processes switched off"; 
    EMailSender::Response resp = emailSend.send(user_email, message);

  }
  delay(100);

}
void loop() {
 client.loop();
}