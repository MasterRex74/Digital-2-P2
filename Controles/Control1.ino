#include <ps5.h>
#include <ps5Controller.h>
#include <ps5_int.h>
#include <Wire.h>
#include <HardwareSerial.h>

unsigned long lastTimeStamp = 0;



void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  Serial.println("Disconnected!.");    
}

void setup() 
{ 
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  //ps5.attach(notify);
  ps5.attachOnConnect(onConnect);
  ps5.attachOnDisconnect(onDisConnect);
  //ps5.begin("24:a6:fa:e4:09:ae");  //mac address Control B
  ps5.begin("88:03:4c:a5:3b:7a");  //mac address Control R
  while (ps5.isConnected() == false) 
  {
    Serial.println("PS5 controller not found");
    delay(300);
  }
  Serial.println("Ready.");

}

int deadzone = 50;

void loop() 
{
   // Supongamos que estos son los valores del joystick izquierdo
    int x = ps5.LStickX();
    int y = ps5.LStickY();
    int z = ps5.Cross();
    //int x = controller.axisX(); // Izquierda (-) a Derecha (+)
    //int y = controller.axisY(); // Arriba (-) a Abajo (+)

    // Dirección vertical
    if (y < -deadzone) {
        Serial.println("Abajo");
        Serial2.println("B\n");
    } else if (y > deadzone) {
        Serial.println("Arriba");
        Serial2.println("A\n");
    }

    // Dirección horizontal
    if (x < -deadzone) {
        Serial.println("Izquierda");
        Serial2.println("I\n");
    } else if (x > deadzone) {
        Serial.println("Derecha");
        Serial2.println("D\n");
    }

    if (z){
      Serial.println("ACCION");
      Serial2.println("N\n");
    }

   /* if ((x < -deadzone) | z){
      Serial.println("MOVIZQ Y ACCION");
      Serial2.println("L\n");
    }*/

    delay(100);
}