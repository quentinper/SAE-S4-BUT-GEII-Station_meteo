const button_on_ONo = document.getElementById("ON_LED");
const button_on_OFF = document.getElementById("OFF_LED");
button_on_ONo.addEventListener("click", button_on_handler);
button_on_OFF.addEventListener("click", button_off_handler);
let temperatureValue = 0;
setInterval(getTemperature, 5000);

async function button_on_handler() {
  try {
    const res = await fetch("/action?led=1");
    if (res.ok == false) {
      throw new Error("Error response");
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};
async function button_off_handler() {
  try {
    const res = await fetch("/action?led=0");
    if (res.ok == false) {
      throw new Error("Error response");
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};
async function getTemperature() {
  try {
    const res = await fetch("/temperature");
    if (res.ok == false) {
      throw new Error("Error response");
    }
    else {
      const tempValue = await res.text();
      console.log(tempValue);
      //mise a jour interface
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};

const gaugeemperature = new RadialGauge({
  renderTo: 'canvas_temperature_id',
  width: 300,
  height: 300,
  units: "°C",
  minValue: -60,
  maxValue: 140,
  majorTicks: [
    -60,
    -40,
    -20,
    0,
    20,
    40,
    60,
    80,
    100,
    120,
    140
  ],
  minorTicks: 10,
  strokeTicks: true,
  highlights: [
    {
      "from": -60,
      "to": 0,
      "color": "rgba(0,0, 255, .3)"
    },
    {
      "from": 0,
      "to": 140,
      "color": "rgba(255, 0, 0, .3)"
    }
  ],
  ticksAngle: 225,
  startAngle: 67.5,
  colorMajorTicks: "rgb(0,0,0)",
  colorMinorTicks: "rgb(0,0,0)",
  colorTitle: "rgb(0,0,0)",
  colorUnits: "rgb(0,0,0)",
  colorNumbers: "rgb(0,0,0)",
  colorPlate: "rgb(250, 248, 243)",
  borderShadowWidth: 0,
  borders: true,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

const gaugeHumidity = new RadialGauge({
  renderTo: 'canvas_humidity_id',
  width: 300,
  height: 300,
  units: "%",
  minValue: 0,
  maxValue: 100,
  majorTicks: [
    0,
    10,
    20,
    30,
    40,
    50,
    60,
    70,
    80,
    90,
    100
  ],
  minorTicks: 10,
  strokeTicks: true,
  highlights: [
    {
      "from": 0,
      "to": 100,
      "color": "rgba(53, 158, 255, 0.3)"
    },
  ],
  ticksAngle: 225,
  startAngle: 67.5,
  colorMajorTicks: "rgb(0,0,0)",
  colorMinorTicks: "rgb(0,0,0)",
  colorTitle: "rgb(0,0,0)",
  colorUnits: "rgb(0,0,0)",
  colorNumbers: "rgb(0,0,0)",
  colorPlate: "rgb(250, 248, 243)",
  borderShadowWidth: 0,
  borders: true,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

const gaugeIlluminance = new RadialGauge({
  renderTo: 'canvas_illuminance_id',
  width: 300,
  height: 300,
  units: "Lux * 1000",
  minValue: 0,
  maxValue: 10,
  majorTicks: [
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10
  ],
  minorTicks: 10,
  strokeTicks: true,
  highlights: [
    {
      "from": 0,
      "to": 1,
      "color": "rgba(255, 255, 204, 0.3)"
    },
    {
      "from": 1,
      "to": 2,
      "color": "rgba(255, 255, 153, 0.3)"
    },
    {
      "from": 2,
      "to": 3,
      "color": "rgba(255, 255, 102, 0.3)"
    },
    {
      "from": 3,
      "to": 4,
      "color": "rgba(255, 255, 51, 0.3)"
    },
    {
      "from": 4,
      "to": 5,
      "color": "rgba(255, 255, 0, 0.3)"
    },
    {
      "from": 5,
      "to": 6,
      "color": "rgba(255, 255, 0, 0.4)"
    },
    {
      "from": 6,
      "to": 7,
      "color": "rgba(255, 255, 0, 0.5)"
    },
    {
      "from": 7,
      "to": 8,
      "color": "rgba(255, 255, 0, 0.6)"
    },
    {
      "from": 8,
      "to": 9,
      "color": "rgba(255, 255, 0, 0.7)"
    },
    {
      "from": 9,
      "to": 10,
      "color": "rgba(255, 255, 0, 0.8)"
    }
  ],
  ticksAngle: 225,
  startAngle: 67.5,
  colorMajorTicks: "rgb(0,0,0)",
  colorMinorTicks: "rgb(0,0,0)",
  colorTitle: "rgb(0,0,0)",
  colorUnits: "rgb(0,0,0)",
  colorNumbers: "rgb(0,0,0)",
  colorPlate: "rgb(250, 248, 243)",
  borderShadowWidth: 0,
  borders: true,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

