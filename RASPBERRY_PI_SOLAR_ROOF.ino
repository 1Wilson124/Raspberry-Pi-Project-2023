
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
String newdata;
Servo myservo;
Servo myservo1;
const int ADC_pin = A0;  
int sensor_reading = 0;  
float calibration = 0.36; // Check Battery voltage using multimeter & add/subtract the value
int bat_percentage;
const char *ssid = ""; 
const char *password = "";  
const char *mqtt_broker = "192.168.1.28"; 
const char *topic = "TEST";
const char *topic2="new_data";
const int mqtt_port = 1883;
const int water = D1;
const int photo= D2;
const int photo1=D3;
String past_value="";
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
 myservo.attach(D4);
 myservo1.attach(D5);

 Serial.begin(115200);
 pinMode(water, OUTPUT);
 digitalWrite(water, LOW);
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
 if (command!=past_value){
 if (command=="clean solar") {
   digitalWrite(water, HIGH);
   Serial.print("solar panel is cleaning");
   delay(5000);
   digitalWrite(water, LOW);
 }
 if (command="reposition"){
   for (int Q = 0; Q<100; Q++){
   int pos1=0;
   int pos2=180;
   int diff=0;
   int diff1=0;
   int diff2;
   int diff3;
   int anomoly=0;
   for (int i=0; i>5 ;i++){
      pinMode(photo, OUTPUT);
      digitalWrite(photo, LOW);
      delay(100);
      pinMode(photo, INPUT);
      int diff = 0;
      int t=micros();
      while (digitalRead(photo)==LOW){
        if (anomoly!=0){
        diff2=micros()-t;
        }
        anomoly++;
      }
    diff=diff+diff2;
   }
   for (int i=0; i>5; i++){
      pinMode(photo1, OUTPUT);
      digitalWrite(photo1, LOW);
      delay(100);
      pinMode(photo1, INPUT);
      int diff = 0;
      int t=micros();
      while (digitalRead(photo1)==LOW){
        diff3=micros()-t;

      }
      diff1=diff1+diff3;
   }
    diff=diff/4;
    diff1=diff1/4;
    int bigdiff=diff-diff1;
    if (bigdiff>100 && bigdiff<-100){
      if (bigdiff<-100 && pos1<=180 && pos2<=180 && pos1>=0 && pos2>=0 ){
        int pos3=5;
        pos1=pos1-pos3;
        pos2=pos2-pos3;
        myservo.write(pos1);
        myservo1.write(pos2);
        delay(500);
      }
      else if (bigdiff>100 && pos1<=180 && pos2<=180 && pos1>=0 && pos2>=0 ){
        int pos3=5;
        pos1=pos1-pos3;
        pos2=pos2-pos3;
        myservo.write(pos1);
        myservo1.write(pos2);
        delay(500);

      }
      else{
        break;
      }
    }
    else{
      break;
    }
   }
 }
}
  
    past_value=command;
    Serial.print("hello");
    sensor_reading = analogRead(ADC_pin);
    Serial.println();
    float voltage = (((sensor_reading * 3.3) / 1024) * 2 ); 
    bat_percentage = mapfloat(voltage); 
    Serial.print("Analog Value = ");
    Serial.print(sensor_reading);
    Serial.print("\t Output Voltage = ");
    Serial.print(voltage);
    Serial.print("\t Battery Percentage = ");
    Serial.println(bat_percentage);
    delay(1000);
    newdata="battery "+String(bat_percentage);    
    client.publish(topic,newdata.c_str());
  
}
void loop()
{
  client.loop();
  
}
 
float mapfloat(float x)
{
  return round(2.1*sqrt(x*x+3200*x-11200));
}