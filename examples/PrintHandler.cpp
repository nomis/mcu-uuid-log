#include <Arduino.h>
#include <uuid/common.h>
#include <uuid/log.h>

static uuid::log::PrintHandler log_handler{Serial};

void setup() {
	static uuid::log::Logger logger{F("setup")};

	Serial.begin(115200);

	uuid::log::Logger::register_handler(&log_handler, uuid::log::Level::ALL);

	logger.info(F("Application started"));
}

void loop() {
	static uuid::log::Logger logger{F("loop")};
	static unsigned int i = 0;

	uuid::loop();

	logger.debug(F("Hello %u World!"), i++);

	log_handler.loop();

	delay(1000);
}
