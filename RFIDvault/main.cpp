#include <unistd.h>
#include <signal.h>
#include "MFRC522.h"
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "rpiServo.h"

void delay(int ms) {
	usleep(ms * 1000);
}

class Servo {
private:
	rpiServo servo;
	unsigned int valueOpen;
	unsigned int valueClose;
public:
	Servo(int vOpen, int vClose) {
		this->valueOpen = vOpen;
		this->valueClose = vClose;
	}

	virtual ~Servo() {
		close();
	}

	void open() {
		servo.setAngle(this->valueOpen);
	}

	void close() {
		servo.setAngle(this->valueClose);
	}
};

Servo* servo;

void serviceMode() {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << " - Service Mode!" << std::endl;
	delay(1000);
}

void servoCloseMode() {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << " - Servo Close Mode!" << std::endl;

	servo->close();
}
void terminateProgram(int sig) {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << " - End of program!" << std::endl;

	delete servo;
	exit(1);
}

int main(int argc, char** argv) {
	signal(SIGINT, terminateProgram);
	MFRC522 mfrc;
	mfrc.PCD_Init();

	if (wiringPiSetupGpio() == -1) {
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
		std::cout << std::put_time(std::localtime(&now_c), "%F %T") << " - Cannot start WiringPi GPIO!" << std::endl;
		exit(1);
	}

	servo = new Servo(atoi(argv[1]), atoi(argv[2]));

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
		// Print UID
		for (byte i = 0; i < mfrc.uid.size; ++i) {
			if (mfrc.uid.uidByte[i] < 0x10) {
				printf(" 0");
				printf("%X", mfrc.uid.uidByte[i]);
			}
			else {
				printf(" ");
				printf("%X", mfrc.uid.uidByte[i]);
			}

		}
		printf("\n");
		delay(1000);
	}

	delete servo;
	return 0;
}
