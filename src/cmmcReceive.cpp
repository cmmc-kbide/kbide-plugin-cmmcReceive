#ifndef _cmmcReceive_CPP_
#define _cmmcReceive_CPP_

#include "cmmcReceive.h"

cmmcReceive::cmmcReceive() {}
cmmcReceive::~cmmcReceive() {}

int8_t cmmcReceive::getRoll()
{
	return _roll;
}

int8_t cmmcReceive::getPitch()
{
	return _pitch;
}

int8_t cmmcReceive::getThrottle()
{
	return _throttle;
}

int8_t cmmcReceive::getYaw()
{
	return _yaw;
}

bool cmmcReceive::getState() {
	return _recieveState;
}

void cmmcReceive::addLine(String string)
{
	if (currentLine == numberOfLines - 1 && line[currentLine].length() > 0)
	{
		for (int i = 0; i < numberOfLines - 1; i++)
		{
			line[i] = line[i + 1];
		}
	}

	line[currentLine] = string;

	if (++currentLine >= numberOfLines)
	{
		currentLine = numberOfLines - 1;
	}

	updateOLED = true;
}

String cmmcReceive::floatToString(float value, int length, int decimalPalces)
{
	String stringValue = String(value, decimalPalces);
	String prefix = "";

	for (int i = 0; i < length - stringValue.length(); i++)
	{
		prefix += " ";
	}

	return prefix + stringValue;
}

String cmmcReceive::hexToString(byte value)
{
	int length = 2;
	String stringValue = String(value, HEX);
	String prefix = "";

	for (int i = 0; i < length - stringValue.length(); i++)
	{
		prefix += "0";
	}

	return "0x" + prefix + stringValue;
}

String cmmcReceive::intToString(int value, int length)
{
	String stringValue = String(value);
	String prefix = "";

	for (int i = 0; i < length - stringValue.length(); i++)
	{
		prefix += " ";
	}

	return prefix + stringValue;
}

String cmmcReceive::ipToString(IPAddress ip)
{
	return String(ip[0]) + "." + ip[1] + "." + ip[2] + "." + ip[3];
}

bool cmmcReceive::isSSID(char *ssid)
{
	return (strcmp(ssid, accessPointName) == 0 || strcmp(ssid, defaultESPWiFiName) == 0 || strlen(ssid) == 0);
}

void cmmcReceive::loadTuningData()
{
	for (int i = 0; i < 3; i++)
	{
		int address = i * 12;

		String str = readEEPROM(address, 4);
		tuningData[i].kp = str.toInt();
		address += 4;

		str = readEEPROM(address, 4);
		tuningData[i].ki = str.toInt();
		address += 4;

		str = readEEPROM(address, 4);
		tuningData[i].kd = str.toInt();
	}
}

void cmmcReceive::saveTuningData(int i)
{
	int address = i * 12;

	String str = String(tuningData[i].kp);
	writeEEPROM(address, str);
	address += 4;

	str = String(tuningData[i].ki);
	writeEEPROM(address, str);
	address += 4;

	str = String(tuningData[i].kd);
	writeEEPROM(address, str);
}

String cmmcReceive::readEEPROM(int index, int length)
{
	String text = "";
	char ch = 1;

	for (int i = index; (i < (index + length)) && ch; ++i)
	{
		if (ch = EEPROM.read(i))
		{
			text.concat(ch);
		}
	}

	return text;
}

int cmmcReceive::writeEEPROM(int index, String text)
{
	for (int i = index; i < text.length() + index; ++i)
	{
		EEPROM.write(i, text[i - index]);
	}

	EEPROM.write(index + text.length(), 0);
	EEPROM.commit();

	return text.length() + 1;
}

void cmmcReceive::shakeRemote(int16_t milliseconds)
{
	if (remotePort != -1)
	{
		FunctionData functionData = {0};
		functionData.startByte = 0xf9;
		functionData.parameter = milliseconds;
		functionData.checksum = functionData.parameter;
		memcpy(functionData.ssid, accessPointName, DEFAULT_SSID_LENGTH);
		memcpy(&data, &functionData, sizeof(FunctionData));

		String string1 = String("Send shake ") + functionData.parameter + " to ";
		String string2 = ipToString(remoteIP) + " port " + remotePort;
		Serial.println(string1 + string2);

		addLine(string1);
		addLine(string2);

		udp.beginPacket(remoteIP, remotePort);
		udp.write(data, sizeof(FunctionData));
		udp.endPacket();
		delay(200);
	}
}

void cmmcReceive::blink()
{
	static int8_t led = 0;
	led = 1 - led;
	digitalWrite(LED, led);
}

void cmmcReceive::begin(String _name)
{
	delay(3000);
	Serial.begin(115200);
	EEPROM.begin(512);
	
	// loadTuningData();
	
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	byte mac[6] = {0};
	WiFi.macAddress(mac);

	sprintf(defaultESPWiFiName, "ESP_%#02X%#02X%#02X", mac[3], mac[4], mac[5]);
	defaultESPWiFiName[DEFAULT_SSID_LENGTH - 1] = '\0';

	// sprintf(accessPointName, "KBCar%lu", ESP.getEfuseMac());
	sprintf(accessPointName, "KBCar-%s", _name.c_str());
	accessPointName[DEFAULT_SSID_LENGTH - 1] = '\0';

	sprintf(accessPointPassword, "%lu", _name.c_str());
	accessPointPassword[DEFAULT_SSID_LENGTH - 1] = '\0';

	WiFi.softAP(accessPointName, accessPointPassword);
	WiFi.mode(WIFI_AP_STA);
	udp.begin(localPort);

	Serial.println();
	Serial.println(String("Name: ") + accessPointName);
	Serial.println(String("Pass: ") + accessPointPassword);
	Serial.println(String("Port: ") + localPort);
	Serial.println();

	pinMode(LED, OUTPUT);
	// ticker.attach_ms(1000, blink);
}

void cmmcReceive::loop()
{
	int numberOfBytes = udp.parsePacket();

	if (numberOfBytes > 0)
	{
		udp.read(data, numberOfBytes);
		remoteIP = udp.remoteIP();
		remotePort = udp.remotePort();

		if (data[0] >= 0xf1 && data[0] <= 0xf9)
		{ // functions
			FunctionData functionData = {0};
			memcpy(&functionData, data, sizeof(FunctionData));
			int16_t checksum = functionData.parameter;
			functionData.ssid[DEFAULT_SSID_LENGTH - 1] = '\0';

			if (functionData.checksum == checksum && isSSID(functionData.ssid))
			{
				shakeRemote((data[0] == 0xf2) ? 1000 : 40); // test shake
				String string = "Function " + hexToString(functionData.startByte);
				Serial.println(string);
				addLine(string);
			}
		}
		else if (data[0] == 0xf0 && data[1] == 0xf0)
		{ // tuning data
			// TuningData tuningDataBuffer = {0};
			// memcpy(&tuningDataBuffer, data, sizeof(TuningData));
			// int16_t checksum = tuningDataBuffer.yawPitchRoll + tuningDataBuffer.kp + tuningDataBuffer.ki + tuningDataBuffer.kd;
			// tuningDataBuffer.ssid[DEFAULT_SSID_LENGTH - 1] = '\0';

			// if (tuningDataBuffer.yawPitchRoll >= 0x01 && tuningDataBuffer.yawPitchRoll <= 0x03 && tuningDataBuffer.checksum == checksum && isSSID(tuningDataBuffer.ssid))
			// {
			// 	int i = tuningDataBuffer.yawPitchRoll - 1;
			// 	tuningData[i] = tuningDataBuffer;
			// 	saveTuningData(i);

			// 	float kp = tuningDataBuffer.kp / 10.0f;
			// 	float ki = tuningDataBuffer.ki / 10.0f;
			// 	float kd = tuningDataBuffer.kd / 10.0f;

			// 	String string = "";

			// 	if (i == 0)
			// 	{
			// 		string += String("Receive tuning data from ") + ipToString(udp.remoteIP()) + " port " + udp.remotePort() + ":";
			// 		Serial.println(string);
			// 		addLine("Receive tuning data from");
			// 		addLine(ipToString(udp.remoteIP()) + " Port " + udp.remotePort());
			// 	}

			// 	String kpString = floatToString(kp, 5, 1);
			// 	String kiString = floatToString(ki, 5, 1);
			// 	String kdString = floatToString(kd, 5, 1);
			// 	string = String(yawPitchRollText[i]) + " KP " + kpString + ", KI " + kiString + ", KD " + kdString;
			// 	Serial.println(string);
			// 	addLine(String(yawPitchRollText[i]) + kpString + "," + kiString + "," + kdString);
			// }
		}
		else if (data[0] == 0xfc && data[1] == 0xfc)
		{ // get tuning data
			TuningData tuningDataBuffer = {0};
			memcpy(&tuningDataBuffer, data, sizeof(TuningData));
			int16_t checksum = tuningDataBuffer.yawPitchRoll + tuningDataBuffer.kp + tuningDataBuffer.ki + tuningDataBuffer.kd;
			tuningDataBuffer.ssid[DEFAULT_SSID_LENGTH - 1] = '\0';

			if (strlen(tuningDataBuffer.ssid) == 0)
			{
				memcpy(tuningDataBuffer.ssid, accessPointName, DEFAULT_SSID_LENGTH);
			}

			if (tuningDataBuffer.checksum == checksum && isSSID(tuningDataBuffer.ssid))
			{

			// 	// send tuning data to remote control
			// 	for (tuningDataBuffer.yawPitchRoll = 0x01; tuningDataBuffer.yawPitchRoll <= 0x03; tuningDataBuffer.yawPitchRoll++)
			// 	{
			// 		int i = tuningDataBuffer.yawPitchRoll - 1;
			// 		tuningDataBuffer.kp = tuningData[i].kp;
			// 		tuningDataBuffer.ki = tuningData[i].ki;
			// 		tuningDataBuffer.kd = tuningData[i].kd;
			// 		tuningDataBuffer.checksum = tuningDataBuffer.yawPitchRoll + tuningDataBuffer.kp + tuningDataBuffer.ki + tuningDataBuffer.kd;
			// 		memcpy(&data, &tuningDataBuffer, sizeof(TuningData));

			// 		String string = "";

			// 		if (i == 0)
			// 		{
			// 			string += String("Send tuning data to ") + ipToString(udp.remoteIP()) + " port " + udp.remotePort() + ":";
			// 			Serial.println(string);
			// 			addLine("Send tuning data to");
			// 			addLine(ipToString(udp.remoteIP()) + " Port " + udp.remotePort());
			// 		}

			// 		string = hexToString(tuningDataBuffer.startByte) + ", ";
			// 		string += hexToString(tuningDataBuffer.startByte2) + ", ";
			// 		string += hexToString(tuningDataBuffer.yawPitchRoll) + ", ";
			// 		string += intToString(tuningDataBuffer.kp, 4) + ", " + intToString(tuningDataBuffer.ki, 4) + ", " + intToString(tuningDataBuffer.kd, 4) + ", " + intToString(tuningDataBuffer.checksum, 4);
			// 		Serial.println(string);

			// 		String kpString = floatToString(tuningDataBuffer.kp / 10.0f, 5, 1);
			// 		String kiString = floatToString(tuningDataBuffer.ki / 10.0f, 5, 1);
			// 		String kdString = floatToString(tuningDataBuffer.kd / 10.0f, 5, 1);
			// 		addLine(String(yawPitchRollText[i]) + kpString + "," + kiString + "," + kdString);

			// 		udp.beginPacket(udp.remoteIP(), udp.remotePort());
			// 		udp.write(data, sizeof(TuningData));
			// 		udp.endPacket();
			// 		delay(200); // don't forget to delay
			// 	}
			}
		}
		else if (data[0] == 0xfe || data[0] == 0x01)
		{ // control or idle
			_recieveState = true;
			ControlData controlData = {0};
			memcpy(&controlData, data, sizeof(ControlData));
			int8_t checksum = controlData.roll + controlData.pitch + controlData.throttle + controlData.yaw;
			controlData.ssid[DEFAULT_SSID_LENGTH - 1] = '\0';

			if (controlData.checksum == checksum && isSSID(controlData.ssid))
			{
				int8_t trimCommand = 0xff;
				int8_t controlCommand = 0xfe;

				if (controlData.roll == trimCommand && controlData.pitch == trimCommand && controlData.throttle == trimCommand && controlData.yaw == trimCommand)
				{
					Serial.println("Trim");
					addLine("Trim");
					shakeRemote(200); // test shake
				}
				else
				{
					if (controlData.startByte == controlCommand)
					{
						// blink();

						if (!isPowerOn)
						{
							isPowerOn = true;
							Serial.println("Power On");
							addLine("Power On");
						}
					}
					else if (isPowerOn)
					{
						isPowerOn = false;
						Serial.println("Power Off");
						addLine("Power Off");
					}

					_roll = controlData.roll;
					_pitch = controlData.pitch;
					_throttle = controlData.throttle;
					_yaw = controlData.yaw;

					String rollString = intToString(controlData.roll, 4);
					String pitchString = intToString(controlData.pitch, 4);
					String throttleString = intToString(controlData.throttle, 3);
					String yawString = intToString(controlData.yaw, 4);
					output = String("Roll ") + rollString + ", Pitch " + pitchString + ", Throttle " + throttleString + ", Yaw " + yawString;

					if (output != lastOutput)
					{
						lastOutput = output;
						Serial.println(output);
						addLine(String("|") + rollString + "|" + pitchString + "|" + throttleString + "|" + yawString + "|");
					}
				}
			}
		} else {
			_recieveState = false;
		}

	}
}

#endif
