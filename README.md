# Weather-Station - joint project
<p align='justify'><i>Please note that the code commits in this repository attributed to a single individual are related to the fact that we had only one development kit with an ESP microcontroller available. Consequently, the code was written and tested on this single device. The entire project was a collaborative effort. All team members co-wrote the code, shared responsibilities, and actively participated in the development process. This unified approach ensured the project's success, leveraging everyone's skills and knowledge.</i></p>

# ESP32, C, JavaScript

<img src="images/IMG_20240610_123827.png" alt="neutral" width="700">

The program was written in C in the Arduino execution environment. ESP32 microcontroller, BME humidity, pressure and temperature sensor, TFT display, photoresistor, resistors and diodes were used.

The display shows measurements of light intensity (%), temperature (°C), humidity (%) and air pressure (hPa), as well as a moving GIF file using the AnimatedGIF library. 

When the temperature reading is over 29°C, a red LED lights up. When the humidity is more than 60%, the green LED lights up. When the light intensity drops below 5%, the white LED lights up.

## Light intensity less than 5%.

<img src="images/IMG_20240610_123910.jpg" alt="light" width="700">

## Temperature higher than 29°C. Humidity higher than 60%.

<img src="images/IMG_20240610_123937.jpg" alt="tempnadhumidity" width="700">

A Wi-Fi connection was used and the website was prepared to present scalable graphs using the charts.js library presenting temperature, light intensity and humidity data. Measurements are taken every second.


## Used libraries
<p align='justify'>
  Libraries were used to utilize the necessary components, which streamlined the work and allowed for more efficient work.
</p>
<ul>
  <li>TFT_eSPI for screen handling via <a href="https://github.com/Bodmer/TFT_eSPI">github.com/Bodmer/TFT_eSPI<a></li>
  <li>SPIFFS filesystem uploader via <a href="https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/latest">github.com/me-no-dev/arduino-esp32fs-plugin<a></li>
  <li>AnimatedGIF via <a href="https://github.com/bitbank2/AnimatedGIF">github.com/bitbank2/AnimatedGIF<a></li>
  <li>WebSockets via <a href="https://github.com/Links2004/arduinoWebSockets">github.com/Links2004/arduinoWebSockets<a></li>
  <li>Adafruit for BME280 sensor via <a href="https://github.com/adafruit/Adafruit_BME280_Library">github.com/adafruit/Adafruit_BME280_Library<a></li>
  <li>WiFi via <a href="https://www.arduino.cc/reference/en/libraries/wifi/">Arduino Official Page - WiFi<a></li>
</ul>

## Views from web application with statistics of measurments

<p align="justify">
  The charts on the page are updated on the client side every second.
  The&nbsp;Y-axis is scaled automatically based on the spread of the values. The&nbsp;X-axis is also automatically scaled based on the amount of data.
</p>

<div align="center">
  
  ![s1](https://github.com/patrycja-kwasniewska/WeatherStation/assets/84547266/28f2b97a-bd51-41c7-8472-88d504c696f2)
  <i>Chart presenting temperature measurements from BME280 sensor readings</i>
  
  ![s3](https://github.com/patrycja-kwasniewska/WeatherStation/assets/84547266/d2300a32-aa72-4762-a689-4133c7fa5e89)
  <i>Chart presenting humidity measurements from BME280 sensor readings</i>
  
  ![s4](https://github.com/patrycja-kwasniewska/WeatherStation/assets/84547266/719d5972-71dc-4820-96dd-669bc94c9363)
  <i>Chart presenting light intesivity measurements from photoresistor sensor readings</i>
  
  ![s5](https://github.com/patrycja-kwasniewska/WeatherStation/assets/84547266/6a1e85b7-3acb-4c7b-b279-28b2977e1238)
  
  <i>View of the charts presenting all the measurements presented above in a form scaled to a smaller browser window</i>
  
</div>

<hr>
<p><i>The project was developed as part of course credit on Lublin University of Technology</i></p>
