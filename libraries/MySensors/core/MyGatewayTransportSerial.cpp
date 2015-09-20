/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */


#include "MyConfig.h"
#include "MyProtocolMySensors.h"
#include "MyGatewayTransport.h"
#include "MyMessage.h"
#include "MyProtocol.h"

#define MAX_RECEIVE_LENGTH 100 // Maximum message length for messages coming from controller

char _serialInputString[MAX_RECEIVE_LENGTH];    // A buffer for incoming commands from serial interface
int _serialInputPos;
MyMessage _serialMsg;

bool gatewayTransportInit() {
	return true;
}

bool gatewayTransportSend(MyMessage &message) {
	Serial.print(protocolFormat(message));
	// Serial print is always successful
	return true;
}

bool gatewayTransportAvailable() {
	bool available = false;
	while (Serial.available()) {
		// get the new byte:
		char inChar = (char) Serial.read();
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (_serialInputPos < MAX_RECEIVE_LENGTH - 1 && !available) {
			if (inChar == '\n') {
				_serialInputString[_serialInputPos] = 0;
				available = true;
			} else {
				// add it to the inputString:
				_serialInputString[_serialInputPos] = inChar;
				_serialInputPos++;
			}
		} else {
			// Incoming message too long. Throw away
			_serialInputPos = 0;
		}
	}
	if (available) {
		// Parse message and return parse result
		available = protocolParse(_serialMsg, _serialInputString);
		_serialInputPos = 0;
	}
	return available;
}

MyMessage & gatewayTransportReceive() {
	// Return the last parsed message
	return _serialMsg;
}
