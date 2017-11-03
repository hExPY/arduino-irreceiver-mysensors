#include <LowPower.h>
#include <SPI.h>
#include <MySensor.h>
#include <IRLib.h>


#define __product__		"Senso: RF24 Receiver, IR Sender"
#define __model__		"RF24IR"
#define __version__		"1508DEV"
#define __author__		"Leroy Foerster <gersilex@gmail.com>"

#define __copyright__	"(c) 2015 The Senso Team"
#define __license__		"All rights reserved."

MySensor gw;
IRsend irsend;

void interrupt(){} // dummy

void setup()
{
	Serial.begin(115200);
	Serial.println(__product__);
	Serial.print(__model__);
	Serial.println(__version__);
	Serial.println(__author__);
	Serial.println((String)__copyright__ + ", " + (String)__license__ + "\n");

	Serial.print("Running on ");
	#if defined(__AVR_ATmega2560__)
	Serial.print("AVR_ATmega2560");
	#elif defined(__AVR_ATmega328P__)
	Serial.print("AVR_ATmega328P");
	#else
	Serial.print("unknown");
	#endif
	Serial.println(" hardware.");
	
	pinMode(2,INPUT); // INTR pin

	analogWrite(3,128);
	delay(50);
	digitalWrite(3,LOW);

	gw.begin(incomingMessage);
	gw.sendSketchInfo(__model__, __version__);
	gw.present(1, S_IR, "IR Blaster");
}

void loop()
{
	attachInterrupt(digitalPinToInterrupt(2), interrupt, LOW);
	LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
	detachInterrupt(digitalPinToInterrupt(2));
	Serial.print('.');

	gw.process();
}

void incomingMessage(const MyMessage &message) {
	Serial.println(message.data);
	char* data = strdup(message.data);
	
	IRTYPES type = atoi(strtok(data, ","));
	unsigned long ir_data = strtoul(strtok(NULL, ","), NULL, 16);
	uint8_t len = atoi(strtok(NULL, ","));

	irsend.send(type, ir_data, len);
}
