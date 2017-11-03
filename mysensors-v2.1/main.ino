#define __product__    "Senso: RF24 Receiver, IR Sender"
#define __model__   "RF24IR"
#define __version__   "1509DEV"
#define __author__    "Leroy Foerster <gersilex@gmail.com>"
#define __contributors__ "Marvyn Zalewski <mszalewski@ownpixel.com>"

#define __copyright__ "(c) 2015 The Senso Team"
#define __license__   "All rights reserved."
#define MY_DEBUG
#define MY_RADIO_NRF24
#define MY_NODE_ID     <ADD NODE ID HERE> 
#define MY_CHILD_ID    <ADD CHILD ID HERE>
#include <SPI.h>
#include <IRLib.h>
#include <MySensors.h>

IRsend irsend;

void interrupt(){} // dummy

void setup()
{
  Serial.begin(115200);
  Serial.println("Channel: " + (String)MY_RF24_CHANNEL);
  Serial.println("ChildID: " + (String)MY_CHILD_ID);
  Serial.println(__product__);
  Serial.print(__model__);
  Serial.println(__version__);
  Serial.println(__author__);
  Serial.println(__contributors__);
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
}

void presentation()  {
  sendSketchInfo(__model__, __version__);
  present(MY_CHILD_ID, S_IR, "IR Blaster");
}

void loop()
{
}

void receive(const MyMessage &message) {
  Serial.println(message.data);
  char* data = strdup(message.data);
  
  IRTYPES type = atoi(strtok(data, ","));
  unsigned long ir_data = strtoul(strtok(NULL, ","), NULL, 16);
  uint8_t len = atoi(strtok(NULL, ","));

  irsend.send(type, ir_data, len);
}
