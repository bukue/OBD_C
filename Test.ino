#include <Arduino.h>
#include <Wire.h>
#include <OBD.h>

COBD obd;
byte byteRead;
int pid;

int value;

String content = "";
char character;
char *command;

void setup()
{
	Serial.begin(9600);

	// we'll use the debug LED as output
	pinMode(13, OUTPUT);
	// start communication with OBD-II UART adapter
	obd.begin();
	// initiate OBD-II connection until success
	while (!obd.init());
}

void loop()
{
	if (Serial.available()){
		character = Serial.read();
		
		if (character == '~'){
			command = (char*)content.c_str();

			pid = (int)strtol(command, NULL, 10);
			content = "";

			if(pid == 0){
				Serial.println("READY");
			// if pid is 0x666 then we are asking for the VIN
			}else if (pid == 1638){
				char* vin = obd.readVin();
				Serial.println(vin);
			}else if(obd.read(pid, value)) {
				Serial.println(value);
			}
		}
		else{
			content.concat(character);
		}
	}
 
	 Serial.flush();
}
