## Development Log

### Initial ESP8266 Experience and Basic Project

Today, I gained hands-on experience with the ESP8266 platform and its development environment.  
I set up the ESP8266 board using the Arduino IDE and verified the required toolchain, board configuration, and serial communication. During this process, I resolved initial upload and connection issues and confirmed stable firmware flashing and runtime behavior.

As a first basic project, I implemented the Blink example to verify correct GPIO operation and overall board functionality. The onboard LED blinked as expected, confirming that the ESP8266 was operating correctly and that the development environment was properly configured.

After confirming basic functionality, I extended the Blink project by integrating Wi-Fi connectivity. The ESP8266 was configured to connect to a local 2.4 GHz Wi-Fi network, and the connection status was monitored through serial output. This step highlighted practical considerations such as Wi-Fi frequency compatibility and correct serial baud rate configuration.

Building upon this, I implemented a simple web server running directly on the ESP8266. Using a web browser on a mobile device or computer, I was able to remotely adjust the LED blinking interval over Wi-Fi. This project demonstrated fundamental IoT concepts, including wireless communication, remote parameter control, and non-blocking firmware design based on `millis()` rather than delay-based blocking.

Overall, this initial work provided a solid foundation in ESP8266 development, covering hardware setup, Wi-Fi communication, web-based device control, and basic embedded system debugging. These experiments form the basis for the next development phase, which will focus on sensor integration, low-power operation using Deep Sleep mode, and reliable sensor data transmission to a central server.
