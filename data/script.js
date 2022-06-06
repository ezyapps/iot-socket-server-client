// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);
var relay1 = 0;
var relay2 = 0;
var relay3 = 0;
var relay4 = 0;
var relay5 = 0;
var relay6 = 0;
var relay7 = 0;
var relay8 = 0;
const rl1Ct = document.querySelector('#relay1');
const rl2Ct = document.querySelector('#relay2');
const rl3Ct = document.querySelector('#relay3');
const rl4Ct = document.querySelector('#relay4');

const rl5Ct = document.querySelector('#relay5');
const rl6Ct = document.querySelector('#relay6');
const rl7Ct = document.querySelector('#relay7');
const rl8Ct = document.querySelector('#relay8');

// Create Temperature Gauge
var gaugeTemp = new LinearGauge({
  renderTo: 'gauge-temperature',
  width: 120,
  height: 400,
  units: "Temperature C",
  minValue: 0,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 40,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 2,
  valueInt: 2,
  majorTicks: [
      "0",
      "5",
      "10",
      "15",
      "20",
      "25",
      "30",
      "35",
      "40"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 30,
          "to": 40,
          "color": "rgba(200, 50, 50, .75)"
      }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 10,
}).draw();
  
// Create Humidity Gauge
var gaugeHum = new RadialGauge({
  renderTo: 'gauge-humidity',
  width: 300,
  height: 300,
  units: "Humidity (%)",
  minValue: 0,
  maxValue: 100,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 2,
  majorTicks: [
      "0",
      "20",
      "40",
      "60",
      "80",
      "100"

  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 80,
          "to": 100,
          "color": "#03C0C1"
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();
function toggleSwitch(element)
 {
   if(element.checked){ 
    updateRelay(element.id, 1);
    }
  else { 
    updateRelay(element.id, 0);
  }  
}
function updateSpeed(element) {
  updateRelay(element.id, element.value);
}
function updateRelay(relay, value) {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);      
    }
  }; 
  xhr.open("GET", "/update?relay="+relay+"&state="+value, true);
  xhr.send();
}
// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var temp = myObj.temperature;
      var hum = myObj.humidity;
      relay1 = myObj.relay1;
      relay2 = myObj.relay2;
      relay3 = myObj.relay3;
      relay4 = myObj.relay4;
      relay5 = myObj.relay5;
      relay6 = myObj.relay6;
      relay7 = myObj.relay7;
      relay8 = myObj.relay8;

      gaugeTemp.value = temp;
      gaugeHum.value = hum;
      console.log(relay1, relay2, relay3, relay4);
      rl1Ct.checked = (relay1 !== "0")? true:false;
      rl2Ct.checked = (relay2 !== "0")? true:false;
      rl3Ct.checked = (relay3 !== "0")? true:false;
      rl4Ct.value = relay4 // (relay4 !== "0")? true:false;
      rl5Ct.checked = (relay5 !== "0")? true:false;
      rl6Ct.checked = (relay6 !== "0")? true:false;
      rl7Ct.checked = (relay7 !== "0")? true:false;
      rl8Ct.value = relay8 // (relay4 !== "0")? true:false;
    }
  }; 
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');
  
  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);
  
  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);
  
  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    gaugeTemp.value = myObj.temperature;
    gaugeHum.value = myObj.humidity;

    relay1 = myObj.relay1;
    relay2 = myObj.relay2;
    relay3 = myObj.relay3;
    relay4 = myObj.relay4;
    relay5 = myObj.relay5;
    relay6 = myObj.relay6;
    relay7 = myObj.relay7;
    relay8 = myObj.relay8;
    console.log(relay1, relay2, relay3, relay4);
    rl1Ct.checked = (relay1 !== "0")? true:false;
    rl2Ct.checked = (relay2 !== "0")? true:false;
    rl3Ct.checked = (relay3 !== "0")? true:false;
    rl4Ct.value = relay4 // (relay4 !== "0")? true:false;
    rl5Ct.checked = (relay5 !== "0")? true:false;
    rl6Ct.checked = (relay6 !== "0")? true:false;
    rl7Ct.checked = (relay7 !== "0")? true:false;
    rl8Ct.value = relay8 // (relay4 !== "0")? true:false;
  }, false);
}