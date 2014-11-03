#include <RHReliableDatagram.h>
#include "MyDriver.h"
#include "MyDriverRF69.h"

MyDriverRF69::MyDriverRF69() : MyDriver() {
	radio = new RFM69();
}

void MyDriverRF69::init() {
	// Start up the radio library
	radio->initialize(FREQUENCY,_address,NETWORKID);
#ifdef IS_RFM69HW
    radio->setHighPower(); //uncomment only for RFM69HW!
#endif
    // radio->encrypt(ENCRYPTKEY);
	
}

void MyDriverRF69::setAddress(uint8_t address) {
	_address = address;
	radio->setAddress(address);
}

uint8_t MyDriverRF69::getAddress() {
	return _address;
}

bool MyDriverRF69::send(uint8_t to, const void* data, uint8_t len) {
	// Make sure radio has powered up
	return radio->sendWithRetry(to,data,len);
}

bool MyDriverRF69::available(uint8_t *to) {
	return radio->receiveDone();
}

uint8_t MyDriverRF69::receive(void* data) {
	// for (byte i = 0; i < radio->DATALEN; i++){
		// data[i]= (void)radio->DATA[i];
	// }
	memcpy(data,(const void *)radio->DATA, radio->DATALEN);
	if (radio->ACKRequested())
    {
      radio->sendACK();
    }
}	

void MyDriverRF69::powerDown() {
	radio->sleep();
}