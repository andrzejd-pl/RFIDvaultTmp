#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <signal.h>
#include "MFRC522.h"
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Servo.h"
#include "CodeContainer.h"

void delay(int ms) {
	usleep(ms * 1000);
}

void log(const std::string& message) {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << " - " << message << std::endl;
}

std::string toHex(const int myInt) {
	std::stringstream stream;
	stream << std::hex << myInt;
	return stream.str();
}

Servo* servo;

CodeContainer cards;
MFRC522 mfrc;

void serviceMode() {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << " - Service Mode!" << std::endl;
	delay(1000);

	while (!mfrc.PICC_IsNewCardPresent());
	Card cardNew;
	cardNew.uid = mfrc.uid;
	cardNew.name = "";
	cardNew.validated = false;
	cards.WriteToContainer(cardNew));
}

void servoCloseMode() {
	log("Servo Close Mode!");

	servo->close();
}

void terminateProgram(int sig) {
	log("End of program!");

	servo->close();
	delete servo;
	exit(1);
}

int main(int argc, char** argv) {
	signal(SIGINT, terminateProgram);
	mfrc.PCD_Init();

	if (wiringPiSetupGpio() == -1) {
		log("Cannot start WiringPi GPIO!");
		exit(1);
	}

	servo = new Servo(atoi(argv[1]), atoi(argv[2]));

	servo->close();

	pinMode(atoi(argv[3]), INPUT);
	pullUpDnControl(atoi(argv[3]), PUD_UP);
	pinMode(atoi(argv[4]), INPUT);
	pullUpDnControl(atoi(argv[4]), PUD_UP);
	
	int initServiceMode = wiringPiISR(atoi(argv[3]), INT_EDGE_FALLING, serviceMode);
	int initCloseDoorMode = wiringPiISR(atoi(argv[4]), INT_EDGE_FALLING, servoCloseMode);

	while (1) {
		// Look for a card
		if (!mfrc.PICC_IsNewCardPresent())
			continue;

		if (!mfrc.PICC_ReadCardSerial())
			continue;


		servo->open();

		std::string cardTmp = "Card: ";
		// Print UID
		for (byte i = 0; i < mfrc.uid.size; ++i) {
			if (mfrc.uid.uidByte[i] < 0x10) {
				cardTmp += " 0" + std::to_string(mfrc.uid.uidByte[i]);
			}
			else {
				cardTmp += " " + std::to_string(mfrc.uid.uidByte[i]);
			}

		}
		printf("\n");
		delay(1000);
	}

	delete servo;
	return 0;
}
