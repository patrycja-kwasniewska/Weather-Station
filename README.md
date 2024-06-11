# Weather-Station - joint project
# ESP32, C++, JavaScript

<img src="images/IMG_20240610_123827.png" alt="neutral" width="700">

The program was written in C++ in the Arduino execution environment. ESP32 microcontroller, BME humidity, pressure and temperature sensor, TFT display, photoresistor, resistors and diodes were used.

The display shows measurements of light intensity (%), temperature (*C), humidity (%) and air pressure (hPa), as well as a moving GIF file using the AnimatedGIF library. 

When the temperature reading is over 31*C, a red LED lights up. When the humidity is more than 55%, the green LED lights up. When the light intensity drops below 5%, the white LED lights up.

<img src="images/IMG_20240610_123910.jpg" alt="light" width="700">
<img src="images/IMG_20240610_123937.jpg" alt="tempnadhumidity" width="700">

A Wi-Fi connection was used, and a website was prepared to present scalable graphs using the charts.js library presenting temperature, light intensity and humidity data. Measurements are taken every second.


