# DO-AN-DIEN-TU-Y-SINH
>> The project "Circuit for Measuring Heart Rate and Blood Oxygen Level" was carried out by the student group: 22200019 - Le Phong Canh, 22200041 - Nguyen Bao Duy, 22200063 - Pham Trong Hoang, 22200084 - Dang Dinh Khoi, under the guidance of Associate Professor Dr. Le Duc Hung, University of Science, VNU-HCM.

>> Project structure
- Code_ESP8266_MAX30100_OLED_Blynk.ino: Main Arduino code file for ESP8266. Reads data from MAX30100 sensor, displays results on OLED, and sends data to Blynk platform.
- health_data.csv: Raw sensor data file recorded from MAX30100 sensor.
- health_data.ipynb: Jupyter Notebook file for processing raw data and saving it to health_data.csv.
- health_data_labeled.csv: Labeled health status dataset file, used as input for model training.
- health_data_classification.ipynb: Jupyter Notebook file for AI model training. Uses labeled data from health_data_labeled.csv and implements Random Forest algorithm for health status classification.
- health_data_predicted_rf.csv: Output file containing health status predictions made by the Random Forest model.

>> Objectives
Develop a basic health monitoring system to measure and display:
+ Heart Rate
+ Blood Oxygen Level (SpO2)
+ Data will be displayed in real time on an OLED screen, sent to the Blynk IoT and transmitted via Serial (COM port) for Python + AI processing
  
>> Hardware Used
+ ESP8266 NodeMCU:	Main microcontroller for data processing and WiFi connectivity
+ MAX30100: Sensor	Measures heart rate and SpO2 via infrared
+ OLED SSD1306 (0.91 inch 128x32 pixel):	Real-time display
+ Breadboard Jumper Wires: Component Connections
+ 5V/USB Power: Supply	Powers the NodeMCU
(All devices communicate via the I2C protocol)

>> Software/Tools Used
+ Arduino IDE:	Code writing and uploading for ESP8266
+ Blynk IoT:	Displays data on smartphones via the Internet
+ Python + pandas/sklearn:	Data storage and AI processing (health classification)
+ GitHub:	Manages source code and project documentation

>> Hardware Connections
MAX30100       -> ESP8266 NodeMCU
VIN            -> 3V3
GND            -> GND
SCL            -> D1 (GPIO5)
SDA            -> D2 (GPIO4)

OLED SSD1306   -> ESP8266 NodeMCU
VCC            -> 3V3
GND            -> GND
SCL            -> D1 (GPIO5)
SDA            -> D2 (GPIO4)
(The OLED and MAX30100 share the same I2C bus, so both are connected to D1 (SCL) and D2 (SDA))

>> Important Notes When Running
- Ensure stable WiFi for the ESP8266 to connect to Blynk
- Disable #define BLYNK_PRINT Serial if sending data to Python via COM port
- Serial baud rate: 115200
- Verify the I2C address of the OLED is 0x3C, modify the code if different
