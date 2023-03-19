import data
from flask import Flask, render_template, request, url_for, after_this_request
from multiprocessing import Process
import sys
import os
import random
import time
from json import dumps

from paho.mqtt import client as mqtt_client


broker = '192.168.1.28'
port = 1883
topic = "TEST"
client_id = '2'
array1=["bedroom1","bedroom2","livingroom","kitchen","office", "bathroom"]
array2=["bedroom1h","bedroom2h","livingroomh","kitchenh","officeh", "bathroomh"]
app = Flask(__name__)
newmessage=""
@app.route('/')
def index():
    return render_template("index.html", data = data.data)

@app.route('/configure', methods = ["POST"])
def configure():
  global newmessage
  global array
  @after_this_request
  def add_header(response):
      response.headers['Access-Control-Allow-Origin']='*'
      return response
  process=str(request.form)
  process=list(process)
  last_index=-1
  last_array=[]
  while True:
    try:
      last_index=process.index("'",last_index+1)
      last_array.append(last_index)
    except ValueError:
      break
  process=process[last_array[2]+1 : last_array[3]]
  newprocess="".join(process)
  process=newprocess.split()
  newmessage=newprocess
  with open('./static/js/messagelink.txt', 'w')as f:
      f.write(newmessage)
  f.close()
  if process[1] in array2:
    if process[0]=="on":
      a="1"
    else:
      a="0"
    position=array2.index(process[1])
    with open('static/js/heating.txt') as f:
      lines=f.readlines()
    list1=list(lines[0])
    list1[position]=a
    f.close()
      
    with open('static/js/heating.txt', 'w')as f:
      f.write("".join(list1))
    f.close()
  else:
    if process[0]=="on":
      a="1"
    else:
      a="0"
    position=array1.index(process[1])
    with open('static/js/lighting.txt') as f:
      lines=f.readlines()
    list1=list(lines[0])
    list1[position]=a
    f.close()
    with open('static/js/lighting.txt', 'w')as f:
      f.write("".join(list1))
    f.close()
    
  return "OK", 200

@app.route('/statistic', methods = ["GET"])
def statistic():
    return dumps(data.solar_panel())

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)

    client.on_connect = on_connect
    client.connect(broker, port)
    return client

payloads=[]
def subscribe(client: mqtt_client):
        
    def on_message(client, userdata, msg1):
        payloads.append(str(msg1.payload))
        print(str(msg1.payload.decode()))
        payld=str(msg1.payload.decode())
        payld=payld.split()
        if payld[0]=="battery":
            with open("data.py") as f:
                lines=readlines(f)
            lines[8]="'Energy used': "+payld[1]
        elif payld[0]=="off":
            place=array1.index(payld[1])
            with open('static/js/heating.txt') as f:
                lines1=f.readlines()
                list3=list(lines1[0])
                list3[place]="0"
            f.close()
                  
            with open('static/js/heating.txt', 'w')as f:
              f.write("".join(list3))
            f.close()
            with open('static/js/lighting.txt') as f:
                lines1=f.readlines()
                list3=list(lines1[0])
                list3[place]="0"
            f.close()
                  
            with open('static/js/lighting.txt', 'w')as f:
              f.write("".join(list3))
            f.close()   
    client.subscribe("new_data")
    client.on_message=on_message

def publish(client):

    with open('static/js/messagelink.txt') as f:
        lines=f.readlines()
    messages=str(lines[0])

    f.close()
    time.sleep(1)
    msg = messages
    result = client.publish(topic, msg)
    status = result[0]
    if status == 0:
        print(f"Send `{msg}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")
    


def run2():
    client = connect_mqtt()
    publish(client) 
    client.loop_start()
    while True:
        publish(client)
        subscribe(client)
        
def loop1():
    try:
        if __name__ == '__main__':
            run2()
    except KeyboardInterrupt:
        client.terminate()
def loop2():
    try:
        app.run(host='192.168.1.28', port=5555)
    except KeyboardInterrupt:
        app.terminate()
#try:
p1=Process(target=loop1)
p1.start()
p2=Process(target=loop2)
p2.start()

#except KeyboardInterrupt:
    


