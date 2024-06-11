// Declare variables
var targetUrl = `ws://${window.location.hostname}:81/`;
var websocket;
var ctxTemp, ctxHum, ctxLum;
var myChartTemp, myChartHum, myChartLum;

const MAX_DATA_COUNT = 20;

// Add function for body load
window.addEventListener("load", onLoad);

// Function to run during on load of the HTML Page
function onLoad() {
  initializeSocket();
  initializeChart();
}

// Initialize the chart
function initializeChart(){
  ctxTemp = document.getElementById("myChartTemp").getContext("2d");
  myChartTemp = new Chart(ctxTemp, {
      type: "line",
      data: {
          datasets: [{ label: "Temperatura" }],
      },
      options: {
          borderWidth: 3,
          borderColor: ["rgba(199, 0, 57, 1)"],
      },
  });

  ctxHum = document.getElementById("myChartHum").getContext("2d");
  myChartHum = new Chart(ctxHum, {
      type: "line",
      data: {
          datasets: [{ label: "Wilgotnosc" }],
      },
      options: {
          borderWidth: 3,
          borderColor: ["rgba(65, 105, 225, 1)"],
      },
  });

  ctxLum = document.getElementById("myChartLum").getContext("2d");
  myChartLum = new Chart(ctxLum, {
      type: "line",
      data: {
          datasets: [{ label: "Natezenie swiatla" }],
      },
      options: {
          borderWidth: 3,
          borderColor: ["rgba(253, 218, 13, 1)"],
      },
  });
}

// Initialize the Websockets
function initializeSocket() {
  console.log(
    `Opening WebSocket connection to Microcontroller :: ${targetUrl}`
  );
  websocket = new WebSocket(targetUrl);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}
// Websocket Callback Client function 
function onOpen(event) {
  console.log("Starting connection to server..");
}
function onClose(event) {
  console.log("Closing connection to server..");
  setTimeout(initializeSocket, 2000);
}
function onMessage(event) {
  console.log("WebSocket message received:", event);

  var data = event.data.split(",");
  var foto_res = data[0];
  var temp = parseFloat(data[1]);
  var hum = data[2];

  console.log("Photo data:", foto_res);
  console.log("Temperature:", temp);
  console.log("Humidity:", hum);

  if (myChartTemp.data.labels.length > MAX_DATA_COUNT) {
      removeFirstData(myChartTemp);
  }
  addData(getCurrentDateTime(), temp, myChartTemp);

  if (myChartHum.data.labels.length > MAX_DATA_COUNT) {
      removeFirstData(myChartHum);
  }
  addData(getCurrentDateTime(), hum, myChartHum);

  if (myChartLum.data.labels.length > MAX_DATA_COUNT) {
    removeFirstData(myChartLum);
  }
  addData(getCurrentDateTime(), foto_res, myChartLum);
}

function getCurrentDateTime() {
  var today = new Date();
  var date = today.getFullYear() + "-" + (today.getMonth() + 1) + "-" + today.getDate();
  var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
  var dateTime = date + " " + time;
  return dateTime;
}

function addData(label, data, myChart) {
  myChart.data.labels.push(label);
  myChart.data.datasets.forEach((dataset) => {
      dataset.data.push(data);
  });
  myChart.update();
}

function removeFirstData(myChart) {
  myChart.data.labels.splice(0, 1);
  myChart.data.datasets.forEach((dataset) => {
      dataset.data.shift();
  });
}