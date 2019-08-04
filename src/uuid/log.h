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

#ifndef UUID_LOG_H_
#define UUID_LOG_H_

#include <Arduino.h>
#include <stdarg.h>

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <uuid/common.h>

namespace uuid {

namespace log {

enum class Level {
	OFF = -1,
	EMERG = 0,
	ALERT,
	CRIT,
	ERR,
	WARNING,
	NOTICE,
	INFO,
	DEBUG,
	TRACE,
	ALL,
};

std::string format_timestamp_ms(int days_width, uint64_t timestamp_ms);
char format_level_char(Level level);
const __FlashStringHelper *format_level_uppercase(Level level);
const __FlashStringHelper *format_level_lowercase(Level level);

class Message {
public:
	Message(uint64_t uptime_ms, Level level, const __FlashStringHelper *name, const std::string &&text);
	~Message() = default;

	const uint64_t uptime_ms_;
	const Level level_;
	const __FlashStringHelper *name_;
	const std::string text_;
};

class Receiver {
public:
	virtual ~Receiver() = default;

	virtual void add_log_message(std::shared_ptr<Message> message) = 0;

protected:
	Receiver() = default;
};

class Logger {
public:
	static constexpr size_t MAX_LOG_LENGTH = 255;

	Logger(const __FlashStringHelper *name);
	~Logger() = default;

	/**
	 * Register a log receiver. Call again to change the log level.
	 */
	static void register_receiver(Receiver *receiver, Level level);
	static void unregister_receiver(Receiver *receiver);

	static Level get_log_level(Receiver *receiver);
	/**
	 * Cleanup processed messages. Must be called regularly to release memory.
	 */
	static void cleanup_messages();

	static inline bool enabled(Level level);
	void emerg(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void emerg(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void alert(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void alert(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void crit(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void crit(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void err(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void err(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void warning(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void warning(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void notice(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void notice(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void info(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void info(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void debug(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void debug(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void trace(const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void trace(const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void log(Level level, const char *format, ...) /* __attribute__ ((format (printf, 2, 3))) */;
	void log(Level level, const __FlashStringHelper *format, ...) /* __attribute__ ((format (printf, 3, 4))) */;

private:
	static void refresh_log_level();
	void vlog(Level level, const char *format, va_list ap);
	void vlog(Level level, const __FlashStringHelper *format, va_list ap);
	void dispatch(Level level, std::vector<char> &text);

	static std::map<Receiver*,Level> receivers_;
	static Level level_;

	const __FlashStringHelper *name_;
};

} // namespace log

} // namespace uuid

#endif
