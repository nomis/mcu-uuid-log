#include <Arduino.h>
#include <uuid/common.h>
#include <uuid/log.h>

class SerialLogHandler: public uuid::log::Handler {
public:
	SerialLogHandler() = default;

	~SerialLogHandler() {
		uuid::log::Logger::unregister_handler(this);
	};

	void start() {
		uuid::log::Logger::register_handler(this, uuid::log::Level::ALL);
	}

	/*
	 * It is not recommended to directly output from this function,
	 * this is only a simple example. Messages should normally be
	 * queued for later output when the application is less busy.
	 */
	void add_log_message(std::shared_ptr<uuid::log::Message> message) {
		Serial.printf_P(PSTR("%s %c [%S] %s\r\n"),
				uuid::log::format_timestamp_ms(message->uptime_ms, 3).c_str(),
				uuid::log::format_level_char(message->level),
				message->name, message->text.c_str());
	}
};

static SerialLogHandler log_handler;

void setup() {
	static uuid::log::Logger logger{F("setup")};

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
