#include <RF24Network.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <RF24.h>
#include <SPI.h>


// The DHT data line is connected to pin 2 on the Arduino
#define DHTPIN 3

// Leave as is if you're using the DHT22. Change if not.
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Radio with CE & CSN connected to 7 & 8
RF24 radio(7, 8);
RF24Network network(radio);

// Constants that identify this node and the node to send data to
const uint16_t this_node = 1;
const uint16_t parent_node = 0;

// Time between packets (in ms)
const unsigned long interval = 2000;

// Structure of our message
struct message_t {
  float temperature;
  float humidity;
  float soil_moisture;
};
message_t message;

// The network header initialized for this node
RF24NetworkHeader header(parent_node);

void setup(void)
{
  // Initialize all radio related modules
  SPI.begin();
  radio.begin();
  delay(5);
  network.begin(90, this_node);

  // Initialize the DHT library
  dht.begin();

  // Set up the Serial Monitor
  Serial.begin(9600);
}

void loop() {

  // Update network data
  network.update();

  // Read humidity (percent)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
Serial.print("test line 1");
  
float soil_most=analogRead(A0);  // read from analog pin A0
/*int soil_moisture=analogRead(A0);  // read from analog pin A3

Serial.print("analog value ");

if(soil_moisture<300) {

Serial.println("Water:");
Serial.println(analogRead(A0));
}

if((soil_moisture>300)&&(soil_moisture<700)) {

Serial.println("Humid soil:");
Serial.println(analogRead(A0));

}

if((soil_moisture>700)&&(soil_moisture<950)){
  

Serial.println("dry soil:");
Serial.println(analogRead(A0));


}

*/
  
  // Construct the message we'll send (replace t with f if you're sending Fahrenheit)
  message = (message_t){ t, h, soil_most};
  Serial.print("Humidity = ");
  Serial.println(message.humidity);
   Serial.print("moisture = ");
  Serial.println(message.soil_moisture);
  // Headers will always be type t for this node
  // We set it again each loop iteration because fragmentation of the messages might change this between loops
  header.type = 't';
  

  // Writing the message to the network means sending it
  if (network.write(header, &message, sizeof(message))) {
    Serial.print("Message sent\n"); 
  } else {
    Serial.print("Could not send message\n"); 
  }

  // Wait a bit before we start over again
  delay(interval);
}
