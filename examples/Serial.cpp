#include <Arduino.h>
#include <uuid/common.h>
#include <uuid/log.h>

class SerialLogHandler: public uuid::log::Handler {
public:
	SerialLogHandler() = default;

	void start() {
		uuid::log::Logger::register_handler(this, uuid::log::Level::ALL);
	}

	/*
	 * It is not recommended to directly output from this function,
	 * this is only a simple example. Messages should normally be
	 * queued for later output when the application is less busy.
	 */
	void operator<<(std::shared_ptr<uuid::log::Message> message) {
		char temp[100] = { 0 };

		int ret = snprintf_P(temp, sizeof(temp), PSTR("%s %c [%S] %s\r\n"),
			uuid::log::format_timestamp_ms(message->uptime_ms).c_str(),
			uuid::log::format_level_char(message->level),
			message->name, message->text.c_str());

		if (ret > 0) {
			Serial.print(temp);
		}
	}
};

static SerialLogHandler log_handler;

void setup() {
	static uuid::log::Logger logger{F("setup")};

	Serial.begin(115200);

	log_handler.start();

	logger.info(F("Application started"));
}

void loop() {
	static uuid::log::Logger logger{F("loop")};
	static unsigned int i = 0;

	uuid::loop();

	logger.debug(F("Hello %u World!"), i++);

	delay(1000);
}
