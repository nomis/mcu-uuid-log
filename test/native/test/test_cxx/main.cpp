/*
 * uuid-log - Microcontroller logging framework
 * Copyright 2022  Simon Arlott
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

#include <list>
#include <memory>

#include <uuid/log.h>

using uuid::log::Message;

namespace uuid {

uint64_t get_uptime_ms() {
	static uint64_t millis = 0;
	return ++millis;
}

} // namespace uuid

/*
 * Verify the process used by SyslogService::loop() to check for changes
 * in the messages after processing the front() message.
 */
void test() {
	std::list<std::shared_ptr<Message>> messages;
	std::shared_ptr<Message> message1 = std::make_shared<Message>((uint64_t)1,
		uuid::log::Level::INFO, uuid::log::Facility::LOCAL0,
		reinterpret_cast<const __FlashStringHelper *>("logger"),
		"text");
	std::shared_ptr<Message> message2 = std::make_shared<Message>((uint64_t)1,
		uuid::log::Level::INFO, uuid::log::Facility::LOCAL0,
		reinterpret_cast<const __FlashStringHelper *>("logger"),
		"text");

	TEST_ASSERT_FALSE_MESSAGE(message1 == message2, "Message must not be equal to the other message");

	messages.emplace_back(std::move(message1));
	messages.emplace_back(std::move(message2));

	auto front = messages.front();

	TEST_ASSERT_TRUE_MESSAGE(messages.front() == front, "Retrieved message must be equal to itself");
	TEST_ASSERT_FALSE_MESSAGE(messages.back() == front, "Retrieved message must not be equal to any other message");

	messages.pop_front();
	TEST_ASSERT_FALSE_MESSAGE(messages.front() == front, "Retrieved message must not be equal to any other message");
}

int main(int argc, char *argv[]) {
	UNITY_BEGIN();
	RUN_TEST(test);
	return UNITY_END();
}
