#include "DHT.h"
#include <WiFi.h>

//#define DHTPIN 2  //Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11
//DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid     = "66";
const char* password = "11111111";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String statusPin18 = "off";
String statusPin19 = "off";
String statusPin21 = "off";

// Assign output variables to GPIO pins
const int outputPin18 = 18;
const int outputPin19 = 19;
const int outputPin21 = 21;

void setup() {
//  Serial.begin(115200);
//  Serial.println("DHT11 test!");
//  dht.begin();
  
  // Initialize the output variables as outputs
  pinMode(outputPin18, OUTPUT);
  pinMode(outputPin19, OUTPUT);
  pinMode(outputPin21, OUTPUT);
            
  digitalWrite(outputPin18, HIGH);
  delay(2000);
  Serial.println("LED 18 0N");
  digitalWrite(outputPin19, HIGH);
  delay(2000);
  Serial.println("LED 19 0N");
  digitalWrite(outputPin21, HIGH);
  delay(2000);
  Serial.println("LED 21 0N");

  // Set outputs to LOW
  digitalWrite(outputPin18, LOW);
  digitalWrite(outputPin19, LOW);
  digitalWrite(outputPin21, LOW);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){  
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /18/on") >= 0) {
              Serial.println("PIN 18 on");
              statusPin18 = "on";
              digitalWrite(outputPin18, HIGH);
            }else if (header.indexOf("GET /18/off") >= 0) {
              Serial.println("PIN 18 off");
              statusPin18 = "off";
              digitalWrite(outputPin18, LOW);
            }else if (header.indexOf("GET /19/on") >= 0) {
              Serial.println("PIN 19 on");
              statusPin19 = "on";
              digitalWrite(outputPin19, HIGH);
            }else if (header.indexOf("GET /19/off") >= 0) {
              Serial.println("PIN 19 off");
              statusPin19 = "off";
              digitalWrite(outputPin19, LOW);
            }else if (header.indexOf("GET /21/on") >= 0) {
              Serial.println("PIN 21 on");
              statusPin21 = "on";
              digitalWrite(outputPin21, HIGH);
            }else if (header.indexOf("GET /21/off") >= 0) {
              Serial.println("PIN 21 off");
              statusPin21 = "off";
              digitalWrite(outputPin21, LOW);
            }
            
            // HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
             client.print("<meta http-equiv=\"refresh\" content=\"5\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #5bc0de; border: none; border-radius:10%; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 5px 5px 5px #aaaaaa;}");
            client.println("h3{background-size: auto;background-color:#E0F2F1;padding:10 0;}");
            client.println(".button2 {background-color: #0f887d;}</style></head>");
            
            // JUDUL
            client.println("<body><h1>Monitoring Suhu dan Control LED</h1>");
       //SUHU DAN KELEMBABAN
//            float suhu = dht.readTemperature();
//            float kelembaban = dht.readHumidity();
//            if (isnan(suhu) || isnan(kelembaban)) {
//              Serial.println("GAGAL MEMBACA SENSOR SUHU DAN KELEMBABAN");
//              return;
//            }
//            Serial.println(suhu);
//            Serial.println(kelembaban);
//
//            
//            client.println("<h3>Suhu dan Kelembaban</h3>\n");
//            client.println("<h5>Suhu : \n");
//            client.println(suhu);
//            client.println("*C</h5>\n");
//            client.println("<h5>Kelembaban : \n");
//            client.println(kelembaban);
//            client.println("%</h5>\n");
//            client.println(suhu);
//            client.print(" || ");
//            client.print(kelembaban);
//            client.println("\n");
            client.println("<h3>Control LED</h3>\n");
            // PIN 18  
            client.println("<p>PIN 18 - Status " + statusPin18 + "</p>");
            // If the statusPin18 is off, it displays the ON button       
            if (statusPin18=="off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // PIN 19  
            client.println("<p>PIN 19 - Status " + statusPin19 + "</p>");
            if (statusPin19=="off") {
              client.println("<p><a href=\"/19/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/19/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            // PIN 21
            client.println("<p>PIN 21 - Status " + statusPin21 + "</p>");
            if (statusPin21=="off") {
              client.println("<p><a href=\"/21/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/21/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }
    
}
