/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>

//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "CLARO1_1CFE9F";  // Enter your SSID here
const char* password = "485f7wvKFq";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

bool parqueo1 = HIGH;
bool parqueo2 = HIGH;
bool parqueo3 = HIGH;
bool parqueo4 = HIGH;

uint8_t LED1pin = 2;
bool LED1status = LOW;
int cont = 0;
int entrada = 0;

#define RXD2 16
#define TXD2 17

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  
  Serial.begin(115200); //baudrate
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(LED1pin, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();

 // if (Serial2.read()> -1){
    entrada = Serial2.read();
    Serial.println(entrada);
  //}
  
  
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status));
}
//************************************************************************************************
// Handler de led1on
//************************************************************************************************
void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status));
}
//************************************************************************************************
// Handler de led1off
//************************************************************************************************
void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t led1stat) {
 String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";

ptr += "table {";
ptr += "  font-family: arial, sans-serif;";
ptr += "  border-collapse: collapse;";
ptr += "  width: 100%;";
ptr += "}";

ptr += "td, th {";
ptr += "  border: 1px solid #dddddd;";
ptr += "  text-align: left;";
ptr += "  padding: 8px;";
ptr += "}";

ptr += "tr:nth-child(even) {";
ptr += "  background-color: #dddddd;";
ptr += "}";
  
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server &#128664</h1>\n";
  ptr += "<h3>Ejemplo de Web Server</h3>\n";

  if (led1stat)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

ptr += "<h2>HTML Table</h2>";

ptr += "<table>";
ptr += "  <tr>";
ptr += "    <th>Nombre del parqueo</th>";
ptr += "    <th>Estado</th>";
ptr += "  </tr>";

//***********************************************************************
if (entrada == 65){ //Parqueo 1 Ocupado
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 1</td>";
  ptr += "    <td>Ocupado</td>";
  ptr += "  </tr>";
  }
else if (entrada == 66){
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 1</td>";
  ptr += "    <td>Disponible</td>";
  ptr += "  </tr>";
}

//**********************************************************************
else if (entrada == 67){//Parqueo 2 Ocupado
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 2</td>";
  ptr += "    <td>Ocupado</td>";
  ptr += "  </tr>";
}
else if (entrada == 68){
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 2</td>";
  ptr += "    <td>Disponible</td>";
  ptr += "  </tr>";
}

//*********************************************************************
else if(entrada == 69){ //Parqueo 3 ocupado
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 3</td>";
  ptr += "    <td>Ocupado</td>";
  ptr += "  </tr>";
}
else if (entrada == 70){
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 3</td>";
  ptr += "    <td>Disponible</td>";
  ptr += "  </tr>";
}

//********************************************************************

else if(entrada == 71){ //Parqueo 4 ocupado
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 4</td>";
  ptr += "    <td>Ocupado</td>";
  ptr += "  </tr>";
}
else if (entrada == 72){
  ptr += "  <tr>";
  ptr += "    <td>Parqueo 4</td>";
  ptr += "    <td>Disponible</td>";
  ptr += "  </tr>";
}

//********************************************************************

ptr += "</table>";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}



//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
