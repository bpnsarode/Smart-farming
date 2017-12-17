#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int temp=1;
/**
 * g++ -L/usr/lib main.cc -I/usr/include -o main -lrrd
 **/

// CE Pin, CSN Pin, SPI Speed
RF24radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);

RF24Network network(radio);

// Constants that identifies this node
const uint16_t pi_node = 0;

// Time between checking for packets (in ms)
const unsigned long interval = 2000;

// Structure of our message
struct message_t {
  float temperature;
  float humidity;
  float soil_moisture;
} messagee;

int main(int argc, char** argv)
{
        // Initialize all radio related modules
        radio.begin();
        delay(5);
        network.begin(90, pi_node);

        // Print some radio details (for debug purposes)
       // radio.printDetails();
       // printf("Ready to receive...\n");

        // Now do this forever (until cancelled by user)
        while(temp)
{
              //get the latest network info
                network.update();
               // printf(".\n");
                // Enter this loop if there is data available to be read,
                // and continue it as long as there is more data to read
                while ( network.available() ) {
                        RF24NetworkHeader header;
                        message_t message;
                        // Have a peek at the data to see the header type
                        network.peek(header);
                        // We can only handle the Temperature type for now
                        if (header.type == 't') {
                                // Read the message
                                network.read(header, &message, sizeof(messagee));
                                messagee = message;
                                // Print it out

                                printf("Temperature received from node %i: %f \n", header.from_node, messagee.temperature);
                                printf("Humidity received from node %i: %f \n", header.from_node, messagee.humidity);
                                printf("soil moisture received from node %i: %f \n", header.from_node, messagee.soil_moisture);

                                 if(messagee.soil_moisture<300)
                                        printf("soil moisture : Water");
                                if(messagee.soil_moisture>300 && messagee.soil_moisture<700)
                                        printf("soil moisture : humid");
                                if(messagee.soil_moisture>700 )
                                        printf("soil moisture : dry");

                        } else {
                                // This is not a type we recognize
                                network.read(header, &message, sizeof(message));
                                printf("Unknown message received from node %i\n", header.from_node);
                        }
                }

                // Wait a bit before we start over again
                delay(2000);
        }

        // last thing we do before we end things
        return 0;
}

