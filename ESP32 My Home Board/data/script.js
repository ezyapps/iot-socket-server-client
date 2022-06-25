// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);
var manual_ip_mode = false;
var blueTooth_ip_mode = false;
var var1 = 0;
var var2 = 0;
var var3 = 0;
var var4 = 0;
var var5 = 0;
var var6 = 0;
var var7 = 0;
var var8 = 0;
var var9 = 0;
var var10 = 0;

const rl1Ct = document.querySelector('#var1');
const rl2Ct = document.querySelector('#var2');
const rl3Ct = document.querySelector('#var3');
const rl4Ct = document.querySelector('#var4');

const rl5Ct = document.querySelector('#var5');
const rl6Ct = document.querySelector('#var6');
const rl7Ct = document.querySelector('#var7');
const rl8Ct = document.querySelector('#var8');

const rl9Ct = document.querySelector('#var9');
const rl10Ct = document.querySelector('#var10');

const manualIPCt = document.querySelector('#MANUAL_SWITCH');
const blueToothIPCt = document.querySelector('#BLUETOOTH_MODE');

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
function changeInputMode (element) {

  var xhr = new XMLHttpRequest();
  if(element.checked)
    xhr.open("GET", "/change_mode?variable="+element.id+"&value=1", true);
  else
    xhr.open("GET", "/change_mode?variable="+element.id+"&value=0", true);
  xhr.send();
}
function allSwitch(inp) {
  updateRelay('ALL', inp);
}
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
  // xhr.onreadystatechange = function() {
  //   if (this.readyState == 4 && this.status == 200) {
  //     var myObj = JSON.parse(this.responseText);      
  //   }
  // }; 
  xhr.open("GET", "/update?variable="+relay+"&value="+value, true);
  xhr.send();
}
function syncUI(myObj){
  var temp = myObj.temperature;
  var hum = myObj.humidity;
  manual_ip_mode = myObj.manual_sw_mode;
  blueTooth_ip_mode = myObj.bt_sw_mode;

  var1 = myObj.var1;
  var2 = myObj.var2;
  var3 = myObj.var3;
  var4 = myObj.var4;
  var5 = myObj.var5;
  var6 = myObj.var6;
  var7 = myObj.var7;
  var8 = myObj.var8;
  var9 = myObj.var9;
  var10 = myObj.var10;

  gaugeTemp.value = temp;
  gaugeHum.value = hum;
  console.log(var1, var2, var3, var4, var5, var6, var7, var8);
  rl1Ct.checked = (var1 == "1")? true:false;
  rl2Ct.checked = (var2 == "1")? true:false;
  rl3Ct.checked = (var3 == "1")? true:false;
  rl4Ct.checked = (var4 == "1")? true:false;
  rl5Ct.checked = (var5 == "1")? true:false;
  rl6Ct.checked = (var6 == "1")? true:false;
  rl7Ct.checked = (var7 == "1")? true:false;
  rl8Ct.checked = (var8 == "1")? true:false;

  rl9Ct.value = var9; // (var8 == "1")? true:false;
  rl10Ct.value = var10; //(var8 == "1")? true:false;

  manualIPCt.checked = (manual_ip_mode == "1")? true:false;
  blueToothIPCt.checked = (blueTooth_ip_mode == "1")? true:false; 
}
// Function to get current readings on the webpage when it loads for the first time
function getReadings() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      syncUI(myObj);      
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
    syncUI(myObj);
  }, false);
}