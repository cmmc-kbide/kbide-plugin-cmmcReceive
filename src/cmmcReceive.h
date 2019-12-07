// Chiang Mai Maker Club Drone Simulator for CMMC Remote
// CMMCDroneSimulator.ino
//
// Created by Pheeraphat Sawangphian on Thu Aug 11 2016.
//
// Remote Protocol
// 0x01 = idle (Power Off)
// 0xf0 = send Tuning Data to drone (KP, KI, KD)
// 0xfc = receive Tuning Data from drone (KP, KI, KD)
// 0xfe = send Trim or Control Data to drone (yaw, pitch, roll, throttle)
// 0xf1-0xf9 = send Function Data (F1-F9) to drone
// 0xf9 = receive Function Data (Shake) from drone
//
// Drone Protocol
// 0xf0 = receive Tuning Data from remote (KP, KI, KD)
// 0xfC = send Tuning Data to remote (KP, KI, KD)
// 0xfe = receive Trim or Control Data from remote (yaw, pitch, roll, throttle)
// 0xf1-0xf9 = receive Function Data (F1-F9) from remote
// 0xf9 = send Function Data (Shake) to remote

#ifndef _cmmcReceive_H_
#define _cmmcReceive_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include <EEPROM.h>
#include <Ticker.h>

#define LED 2
#define DEFAULT_SSID_LENGTH 16

class cmmcReceive
{

public:
	cmmcReceive();
	~cmmcReceive();

	// method
	void begin(String _name);
	void loop();
	int8_t getRoll();
	int8_t getPitch();
	int8_t getThrottle();
	int8_t getYaw();
	bool getState();

	WiFiUDP udp;
	Ticker ticker;
	IPAddress remoteIP;

	typedef struct
	{
		int8_t startByte;
		int16_t parameter;
		int16_t checksum;
		char ssid[DEFAULT_SSID_LENGTH];
	} FunctionData;

	typedef struct
	{
		int8_t startByte;
		int8_t roll;
		int8_t pitch;
		int8_t throttle;
		int8_t yaw;
		int8_t checksum;
		char ssid[DEFAULT_SSID_LENGTH];
	} ControlData;

	typedef struct
	{
		int8_t startByte;
		int8_t startByte2;
		int8_t yawPitchRoll;
		int16_t kp;
		int16_t ki;
		int16_t kd;
		int16_t checksum;
		char ssid[DEFAULT_SSID_LENGTH];
	} TuningData;

	TuningData tuningData[3];

	char accessPointName[DEFAULT_SSID_LENGTH] = {'\0'};
	char defaultESPWiFiName[DEFAULT_SSID_LENGTH] = {'\0'};
	char accessPointPassword[DEFAULT_SSID_LENGTH] = {'\0'};
	bool isPowerOn = false;
	unsigned int remotePort = -1;
	static const int numberOfLines = 5;
	String line[numberOfLines] = {""};
	int currentLine = 0;
	bool updateOLED = true;
	byte data[512] = {0};
	unsigned int localPort = 12345;
	

	String output = "";
	String lastOutput = "";
	String yawPitchRollText[3] = {"Yaw..:", "Pitch:", "Roll.:"};

private:
	int8_t _roll = 0;
	int8_t _pitch = 0;
	int8_t _throttle = 0;
	int8_t _yaw = 0;
	bool _recieveState = false;

	void addLine(String string);
	String floatToString(float value, int length, int decimalPalces);
	String hexToString(byte value);
	String intToString(int value, int length);
	String ipToString(IPAddress ip);
	bool isSSID(char *ssid);
	void loadTuningData();
	void saveTuningData(int i);
	String readEEPROM(int index, int length);
	int writeEEPROM(int index, String text);
	void shakeRemote(int16_t milliseconds);
	void blink();
};
#endif /*__cmmc_remote_H__*/
