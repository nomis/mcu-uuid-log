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

#include <uuid/log.h>

#include <Arduino.h>
#include <stdarg.h>

#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace uuid {

namespace log {

std::map<Receiver*,Level> Logger::receivers_;
Level Logger::level_ = Level::OFF;

static const char *pstr_level_uppercase_off __attribute__((__aligned__(sizeof(int)))) PROGMEM = "OFF";
static const char *pstr_level_uppercase_emerg __attribute__((__aligned__(sizeof(int)))) PROGMEM = "EMERG";
static const char *pstr_level_uppercase_crit __attribute__((__aligned__(sizeof(int)))) PROGMEM = "CRIT";
static const char *pstr_level_uppercase_alert __attribute__((__aligned__(sizeof(int)))) PROGMEM = "ALERT";
static const char *pstr_level_uppercase_err __attribute__((__aligned__(sizeof(int)))) PROGMEM = "ERR";
static const char *pstr_level_uppercase_warning __attribute__((__aligned__(sizeof(int)))) PROGMEM = "WARNING";
static const char *pstr_level_uppercase_notice __attribute__((__aligned__(sizeof(int)))) PROGMEM = "NOTICE";
static const char *pstr_level_uppercase_info __attribute__((__aligned__(sizeof(int)))) PROGMEM = "INFO";
static const char *pstr_level_uppercase_debug __attribute__((__aligned__(sizeof(int)))) PROGMEM = "DEBUG";
static const char *pstr_level_uppercase_trace __attribute__((__aligned__(sizeof(int)))) PROGMEM = "TRACE";
static const char *pstr_level_uppercase_all __attribute__((__aligned__(sizeof(int)))) PROGMEM = "ALL";

static const char *pstr_level_lowercase_off __attribute__((__aligned__(sizeof(int)))) PROGMEM = "off";
static const char *pstr_level_lowercase_emerg __attribute__((__aligned__(sizeof(int)))) PROGMEM = "emerg";
static const char *pstr_level_lowercase_crit __attribute__((__aligned__(sizeof(int)))) PROGMEM = "crit";
static const char *pstr_level_lowercase_alert __attribute__((__aligned__(sizeof(int)))) PROGMEM = "alert";
static const char *pstr_level_lowercase_err __attribute__((__aligned__(sizeof(int)))) PROGMEM = "err";
static const char *pstr_level_lowercase_warning __attribute__((__aligned__(sizeof(int)))) PROGMEM = "warning";
static const char *pstr_level_lowercase_notice __attribute__((__aligned__(sizeof(int)))) PROGMEM = "notice";
static const char *pstr_level_lowercase_info __attribute__((__aligned__(sizeof(int)))) PROGMEM = "info";
static const char *pstr_level_lowercase_debug __attribute__((__aligned__(sizeof(int)))) PROGMEM = "debug";
static const char *pstr_level_lowercase_trace __attribute__((__aligned__(sizeof(int)))) PROGMEM = "trace";
static const char *pstr_level_lowercase_all __attribute__((__aligned__(sizeof(int)))) PROGMEM = "all";

std::string format_timestamp_ms(int days_width, uint64_t timestamp_ms) {
	unsigned long days;
	unsigned int hours, minutes, seconds, milliseconds;

	days = timestamp_ms / 86400000UL;
	timestamp_ms %= 86400000UL;

	hours = timestamp_ms / 3600000UL;
	timestamp_ms %= 3600000UL;

	minutes = timestamp_ms / 60000UL;
	timestamp_ms %= 60000UL;

	seconds = timestamp_ms / 1000UL;
	timestamp_ms %= 1000UL;

	milliseconds = timestamp_ms;

	std::vector<char> text(10 + 1 /* days */ + 2 + 1 /* hours */ + 2 + 1 /* minutes */ + 2 + 1 /* seconds */ + 3 /* milliseconds */ + 1);

	snprintf(text.data(), text.size(), "%0*ld+%02d:%02d:%02d.%03d", days_width, days, hours, minutes, seconds, milliseconds);

	return text.data();
}

char format_level_char(Level level) {
	constexpr char log_level_chars[(int)Level::ALL - (int)Level::OFF + 1] = { ' ', 'P', 'A', 'C', 'E', 'W', 'N', 'I', 'D', 'T', ' ' };
	return log_level_chars[(int)level + 1];
}

const __FlashStringHelper *format_level_uppercase(Level level) {
	const __FlashStringHelper *log_level_uppercase[(int)Level::ALL - (int)Level::OFF + 1] = {
			FPSTR(pstr_level_uppercase_off),
			FPSTR(pstr_level_uppercase_emerg),
			FPSTR(pstr_level_uppercase_crit),
			FPSTR(pstr_level_uppercase_alert),
			FPSTR(pstr_level_uppercase_err),
			FPSTR(pstr_level_uppercase_warning),
			FPSTR(pstr_level_uppercase_notice),
			FPSTR(pstr_level_uppercase_info),
			FPSTR(pstr_level_uppercase_debug),
			FPSTR(pstr_level_uppercase_trace),
			FPSTR(pstr_level_uppercase_all)
	};
	return log_level_uppercase[(int)level + 1];
}

const __FlashStringHelper *format_level_lowercase(Level level) {
	const __FlashStringHelper *log_level_lowercase[(int)Level::ALL - (int)Level::OFF + 1] = {
			FPSTR(pstr_level_lowercase_off),
			FPSTR(pstr_level_lowercase_emerg),
			FPSTR(pstr_level_lowercase_crit),
			FPSTR(pstr_level_lowercase_alert),
			FPSTR(pstr_level_lowercase_err),
			FPSTR(pstr_level_lowercase_warning),
			FPSTR(pstr_level_lowercase_notice),
			FPSTR(pstr_level_lowercase_info),
			FPSTR(pstr_level_lowercase_debug),
			FPSTR(pstr_level_lowercase_trace),
			FPSTR(pstr_level_lowercase_all)
	};
	return log_level_lowercase[(int)level + 1];
}

Message::Message(uint64_t uptime_ms, Level level, Facility facility, const __FlashStringHelper *name, const std::string &&text)
		: uptime_ms_(uptime_ms), level_(level), facility_(facility), name_(name), text_(std::move(text)) {
}

Logger::Logger(const __FlashStringHelper *name, Facility facility)
		: name_(name), facility_(facility) {

};

void Logger::register_receiver(Receiver *receiver, Level level) {
	receivers_[receiver] = level;
	refresh_log_level();
};

void Logger::unregister_receiver(Receiver *receiver) {
	receivers_.erase(receiver);
	refresh_log_level();
};

Level Logger::get_log_level(Receiver *receiver) {
	const auto level = receivers_.find(receiver);

	if (level != receivers_.end()) {
		return level->second;
	}

	return Level::OFF;
}

void Logger::emerg(const char *format, ...) {
	if (enabled(Level::EMERG)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::EMERG, format, ap);
		va_end(ap);
	}
};

void Logger::emerg(const __FlashStringHelper *format, ...) {
	if (enabled(Level::EMERG)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::EMERG, format, ap);
		va_end(ap);
	}
};
void Logger::crit(const char *format, ...) {
	if (enabled(Level::CRIT)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::CRIT, format, ap);
		va_end(ap);
	}
};

void Logger::crit(const __FlashStringHelper *format, ...) {
	if (enabled(Level::CRIT)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::CRIT, format, ap);
		va_end(ap);
	}
};
void Logger::alert(const char *format, ...) {
	if (enabled(Level::ALERT)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::ALERT, format, ap);
		va_end(ap);
	}
};

void Logger::alert(const __FlashStringHelper *format, ...) {
	if (enabled(Level::ALERT)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::ALERT, format, ap);
		va_end(ap);
	}
};
void Logger::err(const char *format, ...) {
	if (enabled(Level::ERR)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::ERR, format, ap);
		va_end(ap);
	}
};

void Logger::err(const __FlashStringHelper *format, ...) {
	if (enabled(Level::ERR)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::ERR, format, ap);
		va_end(ap);
	}
};
void Logger::warning(const char *format, ...) {
	if (enabled(Level::WARNING)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::WARNING, format, ap);
		va_end(ap);
	}
};

void Logger::warning(const __FlashStringHelper *format, ...) {
	if (enabled(Level::WARNING)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::WARNING, format, ap);
		va_end(ap);
	}
};
void Logger::notice(const char *format, ...) {
	if (enabled(Level::NOTICE)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::NOTICE, format, ap);
		va_end(ap);
	}
};

void Logger::notice(const __FlashStringHelper *format, ...) {
	if (enabled(Level::NOTICE)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::NOTICE, format, ap);
		va_end(ap);
	}
};
void Logger::info(const char *format, ...) {
	if (enabled(Level::INFO)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::INFO, format, ap);
		va_end(ap);
	}
};

void Logger::info(const __FlashStringHelper *format, ...) {
	if (enabled(Level::INFO)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::INFO, format, ap);
		va_end(ap);
	}
};
void Logger::debug(const char *format, ...) {
	if (enabled(Level::DEBUG)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::DEBUG, format, ap);
		va_end(ap);
	}
};

void Logger::debug(const __FlashStringHelper *format, ...) {
	if (enabled(Level::DEBUG)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::DEBUG, format, ap);
		va_end(ap);
	}
};
void Logger::trace(const char *format, ...) {
	if (enabled(Level::TRACE)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::TRACE, format, ap);
		va_end(ap);
	}
};

void Logger::trace(const __FlashStringHelper *format, ...) {
	if (enabled(Level::TRACE)) {
		va_list ap;

		va_start(ap, format);
		vlog(Level::TRACE, format, ap);
		va_end(ap);
	}
};

void Logger::log(Level level, Facility facility, const char *format, ...) {
	if (level < Level::EMERG) {
		level = Level::EMERG;
	} else if (level > Level::TRACE) {
		level = Level::TRACE;
	}

	if (enabled(level)) {
		va_list ap;

		va_start(ap, format);
		vlog(level, facility, format, ap);
		va_end(ap);
	}
};

void Logger::log(Level level, Facility facility, const __FlashStringHelper *format, ...) {
	if (level < Level::EMERG) {
		level = Level::EMERG;
	} else if (level > Level::TRACE) {
		level = Level::TRACE;
	}

	if (enabled(level)) {
		va_list ap;

		va_start(ap, format);
		vlog(level, facility, format, ap);
		va_end(ap);
	}
};

void Logger::vlog(Level level, const char *format, va_list ap) {
	vlog(level, facility_, format, ap);
}

void Logger::vlog(Level level, Facility facility, const char *format, va_list ap) {
	std::vector<char> text(MAX_LOG_LENGTH + 1);

	if (vsnprintf(text.data(), text.size(), format, ap) <= 0) {
		return;
	}

	dispatch(level, facility, text);
}

void Logger::vlog(Level level, const __FlashStringHelper *format, va_list ap) {
	vlog(level, facility_, format, ap);
}

void Logger::vlog(Level level, Facility facility, const __FlashStringHelper *format, va_list ap) {
	std::vector<char> text(MAX_LOG_LENGTH + 1);

	if (vsnprintf_P(text.data(), text.size(), reinterpret_cast<PGM_P>(format), ap) <= 0) {
		return;
	}

	dispatch(level, facility, text);
}

void Logger::dispatch(Level level, Facility facility, std::vector<char> &text) {
	std::shared_ptr<Message> message = std::make_shared<Message>(get_uptime_ms(), level, facility, name_, text.data());
	text.resize(0);

	for (auto &receiver : receivers_) {
		if (level <= receiver.second) {
			receiver.first->add_log_message(message);
		}
	}
}

inline bool Logger::enabled(Level level) {
	return level <= level_;
};

void Logger::refresh_log_level() {
	level_ = Level::OFF;

	for (auto &receiver : receivers_) {
		if (level_ < receiver.second) {
			level_ = receiver.second;
		}
	}
}

} // namespace log

} // namespace uuid
