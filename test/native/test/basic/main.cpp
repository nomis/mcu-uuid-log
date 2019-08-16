/*
 * uuid-log - Microcontroller logging framework
 * Copyright 2019  Simon Arlott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <unity.h>

#include <memory>

#include <uuid/log.h>

class Test: public uuid::log::Handler {
public:
	Test() = default;
	~Test() override {
		uuid::log::Logger::unregister_handler(this);
	}

	void operator<<(std::shared_ptr<uuid::log::Message> message) override {
		message_ = message;
	}

	std::shared_ptr<uuid::log::Message> message_;
};

namespace uuid {

uint64_t get_uptime_ms() {
	static uint64_t millis = 0;
	return ++millis;
}

} // namespace uuid

void test() {
	Test test1;
	Test test2;
	uuid::log::Logger logger{F("test"), uuid::log::Facility::LOCAL0};

	uuid::log::Logger::register_handler(&test1, uuid::log::Level::INFO);
	logger.info("Hello, %u World!", 42);

	TEST_ASSERT_TRUE_MESSAGE(test1.message_, "Handler 1 must have the message");
	TEST_ASSERT_EQUAL_UINT64((uint64_t)1, test1.message_->uptime_ms);
	TEST_ASSERT_EQUAL_INT(uuid::log::Level::INFO, test1.message_->level);
	TEST_ASSERT_EQUAL_INT(uuid::log::Facility::LOCAL0, test1.message_->facility);
	TEST_ASSERT_EQUAL_STRING("test", reinterpret_cast<const char *>(test1.message_->name));
	TEST_ASSERT_EQUAL_STRING("Hello, 42 World!", test1.message_->text.c_str());
	TEST_ASSERT_EQUAL_INT(1, test1.message_.use_count());

	TEST_ASSERT_FALSE_MESSAGE(test2.message_, "Handler 2 must not have the message");

	test1.message_.reset();

	uuid::log::Logger::register_handler(&test1, uuid::log::Level::NOTICE);
	logger.info("Hello, %u World!", 42);

	TEST_ASSERT_FALSE_MESSAGE(test1.message_, "Handler 1 must not have the message");
	TEST_ASSERT_FALSE_MESSAGE(test2.message_, "Handler 2 must not have the message");

	uuid::log::Logger::register_handler(&test1, uuid::log::Level::ALL);
	uuid::log::Logger::register_handler(&test2, uuid::log::Level::DEBUG);
	logger.info("Hello, %u World!", 42);

	TEST_ASSERT_TRUE_MESSAGE(test1.message_, "Handler 1 must have the message");
	TEST_ASSERT_EQUAL_UINT64((uint64_t)2, test1.message_->uptime_ms);
	TEST_ASSERT_EQUAL_INT(uuid::log::Level::INFO, test1.message_->level);
	TEST_ASSERT_EQUAL_INT(uuid::log::Facility::LOCAL0, test1.message_->facility);
	TEST_ASSERT_EQUAL_STRING("test", reinterpret_cast<const char *>(test1.message_->name));
	TEST_ASSERT_EQUAL_STRING("Hello, 42 World!", test1.message_->text.c_str());
	TEST_ASSERT_EQUAL_INT(2, test1.message_.use_count());

	TEST_ASSERT_TRUE_MESSAGE(test2.message_, "Handler 2 must have the message");
	TEST_ASSERT_EQUAL_UINT64((uint64_t)2, test2.message_->uptime_ms);
	TEST_ASSERT_EQUAL_INT(uuid::log::Level::INFO, test2.message_->level);
	TEST_ASSERT_EQUAL_INT(uuid::log::Facility::LOCAL0, test2.message_->facility);
	TEST_ASSERT_EQUAL_STRING("test", reinterpret_cast<const char *>(test2.message_->name));
	TEST_ASSERT_EQUAL_STRING("Hello, 42 World!", test2.message_->text.c_str());
	TEST_ASSERT_EQUAL_INT(2, test2.message_.use_count());

	TEST_ASSERT_TRUE_MESSAGE(test1.message_.get() == test2.message_.get(), "Message must be shared between handlers");
}

int main(int argc, char *argv[]) {
	UNITY_BEGIN();
	RUN_TEST(test);
	return UNITY_END();
}
