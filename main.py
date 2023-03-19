import data
from flask import Flask, render_template, request, url_for, after_this_request

import os
array1=["bedroom1","bedroom2","livingroom","kitchen","office", "bathroom"]
array2=["bedroom1h","bedroom2h","livingroomh","kitchenh","officeh", "bathroomh"]
app = Flask(__name__)

@app.route('/')
def index():
    return render_template("index.html", data = data.data)

@app.route('/configure', methods = ["POST"])

def configure():
  global array
  @after_this_request
  def add_header(response):
      response.headers['Access-Control-Allow-Origin']='*'
      return response
  #print(request.form) # do smthn with this wilson
  #print(url_for('static', filename='js/heating.txt'))

  #os.system(process)
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
  process="".join(process)
  process=process.split()
  
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
    return data.solar_panel()

app.run(host='0.0.0.0', port=81)
print(url_for('static', filename='js/heating.txt'))