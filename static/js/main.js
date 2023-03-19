console.log("hello");

const ctx = document.getElementById('chart');

var chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Energy',
            data: [],
            fill: {
                target: 'origin',
                above: '#00CCFF44'
            },
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    },
    options: {
        scales: {
            x: {
                title: {
                    display: true,
                    text: 'Time'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Energy / kWh'
                },
            }
        } 
    }

});

async function get_data() {
    return fetch("/statistic")
        .then((data) => data.json())
        .then((data) => {
            chart.data.labels = data["labels"];
            chart.data.datasets[0].data = data["data"];
            chart.update();
        })
}

get_data();

var heating;
var lighting;
var names=["bedroom1", "bedroom2", "livingroom", "kitchen", "office", "bathroom"];
var names1=["bedroom1h", "bedroom2h", "livingroomh", "kitchenh", "officeh", "bathroomh"];
var showTime = async () => { while (true) {
    get_data();
    
fetch('static/js/heating.txt').then(
x => x.text())
.then(response => {
  heating=response.toString().split("");
  console.log(response);
})
.then(() => {
    console.log(heating);
    fetch('static/js/lighting.txt').then(
    x => x.text())  
    .then(response => {
      lighting=response.toString().split("");
      console.log(response);
    
    })
    .then(() => {
        console.log(lighting);
        
        for (let i=0; i<heating.length; i++) {
          if (heating[i]==1) {
            var string=names1[i]+"on"
            document.getElementById(string).style.backgroundColor= "green"
            var string1=names1[i]+"off"
            document.getElementById(string1).style.backgroundColor= "Transparent"
          }
          else {
            var string=names1[i]+"off"
            document.getElementById(string).style.backgroundColor= "#ff3333"
            var string1=names1[i]+"on"
            document.getElementById(string1).style.backgroundColor= "Transparent"
          }
        }
        for (let i=0; i<lighting.length; i++) {
          if (lighting[i]==1) {
            var string=names[i]+"on"
            document.getElementById(string).style.backgroundColor= "green"
            var string1=names[i]+"off"
            document.getElementById(string1).style.backgroundColor= "Transparent"
          }
          else {
            var string=names[i]+"off"
            document.getElementById(string).style.backgroundColor= "#ff3333"
            var string1=names[i]+"on"
            document.getElementById(string1).style.backgroundColor= "Transparent"
          }
        }
        
       });
   });
await new Promise(resolve => setTimeout(resolve, 2000));
}};
showTime();












function changecolour(value){

    let text=value
    const myArray = text.split(" ");
    
    if (myArray[0]=="on") {
      var string=myArray[1];
      string+="on";
      document.getElementById(string).style.backgroundColor= "green"
      var string1=myArray[1];
      string1+="off";
      document.getElementById(string1).style.backgroundColor= "Transparent"
      
    
    }
    else {
      var string=myArray[1];
      string+="off";
      document.getElementById(string).style.backgroundColor= "#ff3333"
      var string1=myArray[1];
      string1+="on";
      document.getElementById(string1).style.backgroundColor= "Transparent"
      
    }
  

}
  function activate(tab) {
      var i, tabcontent, tablinks;
      tabcontent = document.getElementsByClassName("tabcontent");
      for (i = 0; i < tabcontent.length; i++) {
          tabcontent[i].style.display = "none";
      }
      tablinks = document.getElementsByClassName("tablinks");
      for (i = 0; i < tablinks.length; i++) {
          tablinks[i].className = tablinks[i].className.replace(" active", "");
      }
      document.getElementById(tab).style.display = "block";    
  }
  
  function change_tab(evt, cityName) {
      activate(cityName);
      evt.currentTarget.className += " active";
  }
  





activate('SOLAR PANEL');
