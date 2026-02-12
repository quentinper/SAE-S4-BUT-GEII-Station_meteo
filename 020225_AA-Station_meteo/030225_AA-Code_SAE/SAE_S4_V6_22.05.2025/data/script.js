const button_on_ONo = document.getElementById("ON_LED");
const button_on_OFF = document.getElementById("OFF_LED");
const temp_city = document.getElementById("temp_city");
const hum_city = document.getElementById("hum_city");
const lum_city = document.getElementById("lum_city");
const meteo_city = document.getElementById("meteo_city");
const slider = document.getElementById('ledSlider');

let temperatureChart = null;      // Graphique
const temperatures = [];          // Liste des températures
const labels = [];                // Liste des labels (ex: numéro de mesure)
let intervalId = null;
let humidityChart = null;      // Graphique
const humidity = [];          // Liste d'humidité
const labels_2 = [];                // Liste des labels (ex: numéro de mesure)
let intervalId_2 = null;
let luminosityChart = null;      // Graphique
const luminosity = [];          // Liste des luminosité
const labels_3 = [];                // Liste des labels (ex: numéro de mesure)
let intervalId_3 = null;
const url = "http://openweathermap.org/img/wn/";
var Boolean, flag = false;
var Boolean, flag_1 = false;
var Boolean, flag_2 = false;
var Boolean, flag_3 = false;
var Boolean, flag_4 = false;
var Boolean, flag_5 = false;
var Boolean, flag_6 = false;
const temperatureOK = true;
const luminositeOK = false;
const humiditeOK = true;
const datetimeOK = true;
const carteSDOK = false;
const wifiOK = true;

button_on_ONo.addEventListener("click", button_on_handler);
button_on_OFF.addEventListener("click", button_off_handler);

setInterval(getTemperature, 2000);
setInterval(getHumidity, 2000);
setInterval(getIlluminance, 2000);
setInterval(getMinutes, 2000);
setInterval(getHeures, 2000);
setInterval(getstateSDcarte, 2000);
setInterval(getstateWifi, 2000);
setInterval(getstatedatetime, 2000);
setInterval(getstatehumidity, 2000);
setInterval(getstatelight, 2000);
setInterval(getstatetemp, 2000);

async function button_on_handler() {
  try {
    const res = await fetch("/action?led=1");
    flag = true;
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
    flag = false;
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
      gaugeTemperature.value = tempValue;
      console.log(tempValue);
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};

async function getNewTemperature() {
  try {
    const res = await fetch("/courbetemp");
    if (!res.ok) {
      throw new Error("Erreur de réponse");
    }
    const tempValue = await res.text();
    console.log("Température reçue :", tempValue);
    const newTemp = parseFloat(tempValue.trim());
    if (isNaN(newTemp)) {
      throw new Error("Température invalide reçue");
    }
    temperatures.push(newTemp);
    labels.push(temperatures.length);
    if (temperatureChart) {
      temperatureChart.data.labels = labels;
      temperatureChart.data.datasets[0].data = temperatures;
      temperatureChart.update();
    }

  } catch (error) {
    console.log("Erreur :", error);
  }
}

function getCourbeTemperature() {
  if (!temperatureChart) {
    const ctx = document.getElementById('temperatureGraph').getContext('2d');
    temperatureChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [{
          label: 'Température (°C)',
          data: temperatures,
          borderColor: 'red',
          borderWidth: 2,
          fill: false,
          tension: 0.3
        }]
      },
      options: {
        scales: {
          x: { title: { display: true, text: 'Mesure n°' } },
          y: { title: { display: true, text: 'Température (°C)' } }
        }
      }
    });
  }
  if (!intervalId) {
    intervalId = setInterval(getNewTemperature, 2000);
  }
}

async function getHumidity() {
  try {
    const res = await fetch("/humidity");
    if (res.ok == false) {
      throw new Error("Error response");
    }
    else {
      const HumValue = await res.text();
      gaugeHumidity.value = HumValue;
      console.log(HumValue);
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};

async function getNewHumidity() {
  try {
    const res = await fetch("/courbehum");
    if (!res.ok) {
      throw new Error("Erreur de réponse");
    }
    const humValue = await res.text();
    console.log("Humidité reçue :", humValue);
    const newhum = parseFloat(humValue.trim());
    if (isNaN(newhum)) {
      throw new Error("Température invalide reçue");
    }
    humidity.push(newhum);
    labels_2.push(humidity.length);
    if (humidityChart) {
      humidityChart.data.labels = labels_2;
      humidityChart.data.datasets[0].data = humidity;
      humidityChart.update();
    }
  } catch (error) {
    console.log("Erreur :", error);
  }
}

function getCourbeHumidity() {
  if (!humidityChart) {
    const ctx = document.getElementById('HumidityGraph').getContext('2d');
    humidityChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels_2,
        datasets: [{
          label: 'Humiditée (%)',
          data: humidity,
          borderColor: 'red',
          borderWidth: 2,
          fill: false,
          tension: 0.3
        }]
      },
      options: {
        scales: {
          x: { title: { display: true, text: 'Mesure n°' } },
          y: { title: { display: true, text: 'Humidity (%)' } }
        }
      }
    });
  }
  if (!intervalId_2) {
    intervalId_2 = setInterval(getNewHumidity, 2000);
  }
}

async function getIlluminance() {
  try {
    const res = await fetch("/luminosite");
    if (res.ok == false) {
      throw new Error("Error response");
    }
    else {
      const LumValue = await res.text();
      gaugeIlluminance.value = LumValue;
      console.log(LumValue);
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};

async function getMinutes() {
  try {
    const res = await fetch("/minute");
    if (res.ok == false) {
      throw new Error("Error response");
    }
    else {
      const minute = await res.text();
      minuteGauge.value = minute * 6;
      console.log("minute reçu:", minute);
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};

async function getHeures() {
  try {
    const res = await fetch("/heures");
    if (res.ok == false) {
      throw new Error("Error response");
    }
    else {
      const heure = await res.text();
      heureGauge.value = heure * 30;
      console.log("heure reçu:", heure);
    }
  }
  catch (error) {
    console.log("Error:", error);
  }
};

async function getNewluminosity() {
  try {
    const res = await fetch("/courbelum");
    if (!res.ok) {
      throw new Error("Erreur de réponse");
    }

    const lumValue = await res.text();
    console.log("luminosité reçue :", lumValue);

    const newlum = parseFloat(lumValue.trim());
    if (isNaN(newlum)) {
      throw new Error("Luminosité invalide reçue");
    }
    luminosity.push(newlum);
    labels_3.push(luminosity.length);
    if (luminosityChart) {
      luminosityChart.data.labels = labels_3;
      luminosityChart.data.datasets[0].data = luminosity;
      luminosityChart.update();
    }
  } catch (error) {
    console.log("Erreur :", error);
  }
}

function getCourbeIlluminance() {
  if (!luminosityChart) {
    const ctx = document.getElementById('illuminanceGraph').getContext('2d');
    luminosityChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels_3,
        datasets: [{
          label: 'Luminosité (lux)',
          data: luminosity,
          borderColor: 'red',
          borderWidth: 2,
          fill: false,
          tension: 0.3
        }]
      },
      options: {
        scales: {
          x: { title: { display: true, text: 'Mesure n°' } },
          y: { title: { display: true, text: 'Luminosité (lux)' } }
        }
      }
    });
  }
  if (!intervalId_3) {
    intervalId_3 = setInterval(getNewluminosity, 2000);
  }
}

slider.addEventListener('input', async () => {
  const value = slider.value;
  console.log('Valeur du slider :', value);
  try {
    if (!flag) {
      const res = await fetch(`/set_led_intensity?intensity=${value}`);
      const data = await res.text();
      console.log('Réponse ESP32:', data);
    }
  } catch (error) {
    console.log('Erreur:', error);
  }
});

function showCard(id) {
  const action = document.getElementById(id);
  if (action)
    action.style.display = 'block';

}

document.addEventListener('DOMContentLoaded', () => {
  document.querySelectorAll('.red-square').forEach(btn => {
    btn.addEventListener('click', (input) => {
      const article = input.target.closest('article');
      if (article) {
        article.style.display = 'none';
      }
    });
  });
});

async function submit_datetime_handler(event) {
  event.preventDefault(); // Empêche le rechargement de la page au clic sur le bouton
  try {
    // Récupération des éléments du DOM
    const daySelect = document.querySelector("select.w3-select");
    const timeInput = document.getElementById("thetime");
    const dateInput = document.getElementById("thedate");

    // Extraction des valeurs
    const day = daySelect.value;
    const time = timeInput.value;
    const date = dateInput.value;

    // Construction des paramètres GET
    const queryParams = new URLSearchParams({
      day: day,
      time: time,
      date: date
    });

    // Requête HTTP
    const res = await fetch(`/set_datetime?${queryParams.toString()}`);

    if (!res.ok) {
      throw new Error("Erreur de réponse du serveur");
    }

    console.log("Date et heure envoyées avec succès");
  } catch (error) {
    console.error("Erreur :", error);
  }
}

async function submit_city_handler(event) {
  event.preventDefault(); // Empêche le rechargement de la page au clic sur le bouton
  try {
    const cityInput = document.getElementById("thecity");
    const city = cityInput.value;
    console.log(city);
    const queryParams = new URLSearchParams({
      city: city
    });
    const res = await fetch(`/set_city?${queryParams.toString()}`);
    if (!res.ok) {
      throw new Error("Erreur de réponse du serveur");
    }
    else {
      const thejson = await res.json();
      const temp = thejson.main.temp;
      const hum = thejson.main.humidity;
      const lum = thejson.weather[0].description;
      const meteo = thejson.weather[0].icon;
      const icon = url + meteo + "@2x.png";
      temp_city.textContent = temp;
      hum_city.textContent = hum;
      lum_city.textContent = lum;
      console.log(icon);
      meteo_city.src = icon;
      console.log(thejson);
    }
  } catch (error) {
    console.error("Erreur :", error);
  }
}

async function getstatehumidity() {
  if (!flag_1) {
    try {
      const res = await fetch("/getstatehumidity");
      if (!res.ok) {
        throw new Error("Error response");
      }
      const HumState = await res.text();
      const action = document.getElementById("sensor_humidity");
      flag_1 = true;
      action.classList.remove("rouge", "vert");
      if (HumState.trim() == "0") {
        action.classList.add("rouge");
      } else {
        action.classList.add("vert");
      }
      console.log("HumState:", HumState);
    } catch (error) {
      console.log("Error:", error);
    }
  }
}
async function getstatetemp() {
  if (!flag_2) {
    try {
      const res = await fetch("/getstatetemp");
      if (!res.ok) {
        throw new Error("Error response");
      }
      const tempState = await res.text();
      const action = document.getElementById("sensor_temp");
      flag_2 = true;
      action.classList.remove("rouge", "vert");
      if (tempState.trim() == "0") {
        action.classList.add("rouge");
      } else {
        action.classList.add("vert");
      }
      console.log("tempState:", tempState);
    } catch (error) {
      console.log("Error:", error);
    }
  }
}
async function getstatelight() {
  if (!flag_3) {
    try {
      const res = await fetch("/getstatelight");
      if (!res.ok) {
        throw new Error("Error response");
      }
      const lightState = await res.text();
      const action = document.getElementById("sensor_light");
      flag_3 = true;
      action.classList.remove("rouge", "vert");
      if (lightState.trim() == "0") {
        action.classList.add("rouge");
      } else {
        action.classList.add("vert");
      }
      console.log("lightState:", lightState);
    } catch (error) {
      console.log("Error:", error);
    }
  }
}
async function getstatedatetime() {
  if (!flag_4) {
    try {
      const res = await fetch("/getstatedatetime");
      if (!res.ok) {
        throw new Error("Error response");
      }
      const datetimeState = await res.text();
      const action = document.getElementById("sensor_datetime");
      flag_4 = true;
      action.classList.remove("rouge", "vert");
      if (datetimeState.trim() == "0") {
        action.classList.add("rouge");
      } else {
        action.classList.add("vert");
      }
      console.log("datetimeState:", datetimeState);
    } catch (error) {
      console.log("Error:", error);
    }
  }
}
async function getstateSDcarte() {
  if (!flag_5) {
    try {
      const res = await fetch("/getstateSDcarte");
      if (!res.ok) {
        throw new Error("Error response");
      }
      const SDcarteState = await res.text();
      const action = document.getElementById("sensor_SDcarte");
      flag_5 = true;
      action.classList.remove("rouge", "vert");
      if (SDcarteState.trim() == "0") {
        action.classList.add("rouge");
      } else {
        action.classList.add("vert");
      }
      console.log("SDcarteState:", SDcarteState);
    } catch (error) {
      console.log("Error:", error);
    }
  }
}
async function getstateWifi() {
  if (!flag_6) {
    try {
      const res = await fetch("/getstateWifi");
      if (!res.ok) {
        throw new Error("Error response");
      }
      const WifiState = await res.text();
      const action = document.getElementById("sensor_Wifi");
      flag_6 = true;
      action.classList.remove("rouge", "vert");
      if (WifiState.trim() == "0") {
        action.classList.add("rouge");
      } else {
        action.classList.add("vert");
      }
      console.log("WifiState:", WifiState);
    } catch (error) {
      console.log("Error:", error);
    }
  }
}
const gaugeTemperature = new RadialGauge({
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
      "to": -40,
      "color": "rgba(0, 0, 255, 0.7)"
    },
    {
      "from": -40,
      "to": -20,
      "color": "rgba(0, 0, 255, 0.6)"
    },
    {
      "from": -20,
      "to": 0,
      "color": "rgba(0, 0, 255, 0.5)"
    },
    {
      "from": 0,
      "to": 20,
      "color": "rgba(255, 0, 0, 0.2)"
    },
    {
      "from": 20,
      "to": 40,
      "color": "rgba(255, 0, 0, 0.283)"
    },
    {
      "from": 40,
      "to": 60,
      "color": "rgba(255, 0, 0, 0.367)"
    },
    {
      "from": 60,
      "to": 80,
      "color": "rgba(255, 0, 0, 0.45)"
    },
    {
      "from": 80,
      "to": 100,
      "color": "rgba(255, 0, 0, 0.533)"
    },
    {
      "from": 100,
      "to": 120,
      "color": "rgba(255, 0, 0, 0.617)"
    },
    {
      "from": 120,
      "to": 140,
      "color": "rgba(255, 0, 0, 0.7)"
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
      "to": 10,
      "color": "rgba(53, 158, 255, 0.1)"
    },
    {
      "from": 10,
      "to": 20,
      "color": "rgba(53, 158, 255, 0.2)"
    },
    {
      "from": 20,
      "to": 30,
      "color": "rgba(53, 158, 255, 0.3)"
    },
    {
      "from": 30,
      "to": 40,
      "color": "rgba(53, 158, 255, 0.4)"
    },
    {
      "from": 40,
      "to": 50,
      "color": "rgba(53, 158, 255, 0.5)"
    },
    {
      "from": 50,
      "to": 60,
      "color": "rgba(53, 158, 255, 0.6)"
    },
    {
      "from": 60,
      "to": 70,
      "color": "rgba(53, 158, 255, 0.7)"
    },
    {
      "from": 70,
      "to": 80,
      "color": "rgba(53, 158, 255, 0.8)"
    },
    {
      "from": 80,
      "to": 90,
      "color": "rgba(53, 158, 255, 0.9)"
    },
    {
      "from": 90,
      "to": 100,
      "color": "rgba(53, 158, 255, 1)"
    }
    ,
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
  units: "Lux",
  minValue: 0,
  maxValue: 2000,
  majorTicks: [
    0,
    200,
    400,
    600,
    800,
    1000,
    1200,
    1400,
    1600,
    1800,
    2000    
  ],
  minorTicks: 10,
  strokeTicks: true,
  highlights: [
    {
      "from": 0,
      "to": 200,
      "color": "rgba(255, 255, 0, 0.1)"
    },
    {
      "from": 200,
      "to": 400,
      "color": "rgba(255, 255, 0, 0.2)"
    },
    {
      "from": 400,
      "to": 600,
      "color": "rgba(255, 255, 0, 0.3)"
    },
    {
      "from": 600,
      "to": 800,
      "color": "rgba(255, 255, 0, 0.4)"
    },
    {
      "from": 800,
      "to": 1000,
      "color": "rgba(255, 255, 0, 0.5)"
    },
    {
      "from": 1000,
      "to": 1200,
      "color": "rgba(255, 255, 0, 0.6)"
    },
    {
      "from": 1200,
      "to": 1400,
      "color": "rgba(255, 255, 0, 0.7)"
    },
    {
      "from": 1400,
      "to": 1600,
      "color": "rgba(255, 255, 0, 0.8)"
    },
    {
      "from": 1600,
      "to": 1800,
      "color": "rgba(255, 255, 0, 0.9)"
    },
    {
      "from": 1800,
      "to": 2000,
      "color": "rgba(255, 255, 0, 1.0)"
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

// Horloge des minutes (cadran + aiguille des minutes)
var minuteGauge = new RadialGauge({
  renderTo: 'horloge-minutes',
  width: 300,
  height: 300,
  minValue: 0,
  maxValue: 360,
  majorTicks: ["12", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"],
  minorTicks: 5,
  startAngle: 180,
  ticksAngle: 360,
  strokeTicks: false,
  highlights: false,
  colorPlate: "rgb(253, 253, 253)",
  colorMajorTicks: "rgb(0, 0, 0)",
  colorMinorTicks: "rgb(68, 66, 66)",
  colorNumbers: "rgb(0, 0, 0)",
  colorNeedle: "blue",
  colorNeedleEnd: "blue",
  needleType: "line",
  needleStart: 0,
  needleEnd: 80,
  needleWidth: 3,
  borders: true,
  borderInnerWidth: 0,
  borderMiddleWidth: 0,
  borderOuterWidth: 10,
  colorBorderOuter: "rgb(122, 198, 227)",
  colorBorderOuterEnd: "rgb(122, 198, 227)",
  animationDuration: 500,
  animationRule: "linear",
  valueBox: false,
  value: 0
}).draw();

// Horloge des heures (aiguille rouge au-dessus)
var heureGauge = new RadialGauge({
  renderTo: 'horloge-heures',
  width: 300,
  height: 300,
  minValue: 0,
  maxValue: 360,
  majorTicks: [],
  minorTicks: 0,
  startAngle: 180,
  ticksAngle: 360,
  strokeTicks: false,
  highlights: false,
  colorPlate: "transparent",
  colorMajorTicks: "transparent",
  colorMinorTicks: "transparent",
  colorNumbers: "transparent",
  colorNeedle: "red",
  colorNeedleEnd: "red",
  needleType: "line",
  needleStart: 0,
  needleEnd: 50,  // plus courte
  needleWidth: 5, // bien visible
  borders: false,
  animationDuration: 500,
  animationRule: "linear",
  valueBox: false,
  value: 0
}).draw();

// Fonction mise à jour de l'horloge

document.addEventListener("DOMContentLoaded", () => {
  // Attache l'écouteur au formulaire après que la page soit chargée
  const form = document.getElementById("datetime-form");
  if (form) {
    form.addEventListener("submit", submit_datetime_handler);
  }
});

document.addEventListener("DOMContentLoaded", () => {
  const form = document.getElementById("city-form");
  if (form) {
    form.addEventListener("submit", submit_city_handler);
  }
});



