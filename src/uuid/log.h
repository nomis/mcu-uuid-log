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

#include <cstdarg>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <uuid/common.h>

namespace uuid {

namespace log {

/**
 * Severity level of log messages.
 */
enum Level : int8_t {
	OFF = -1, /*!< Meta level representing no log messages. */
	EMERG = 0, /*!< System is unusable. */
	ALERT, /*!< Action must be taken immediately. */
	CRIT, /*!< Critical conditions. */
	ERR, /*!< Error conditions. */
	WARNING, /*!< Warning conditions. */
	NOTICE, /*!< Normal but significant conditions. */
	INFO, /*!< Informational messages. */
	DEBUG, /*!< Debug-level messages. */
	TRACE, /*!< Trace messages. */
	ALL, /*!< Meta level representing all log messages. */
};

/**
 * Facility type of the process logging a message.
 */
enum Facility : uint8_t {
	KERN = 0, /*!< Kernel messages. */
	USER, /*!< User-level messages. */
	MAIL, /*!< Mail system. */
	DAEMON, /*!< System daemons. */
	AUTH, /*!< Security/authentication messages. */
	SYSLOG, /*!< Messages generated internally by logger. */
	LPR, /*!< Line printer subsystem. */
	NEWS, /*!< Network news subsystem. */
	UUCP, /*!< UUCP subsystem. */
	CRON, /*!< Clock daemon. */
	AUTHPRIV, /*!< Security/authentication messages (private). */
	FTP, /*!< FTP daemon. */
	NTP, /*!< NTP subsystem. */
	SECURITY, /*!< Log audit. */
	CONSOLE, /*!< Log alert. */
	CRON2, /*!< Scheduling daemon. */
	LOCAL0, /*!< Locally used facility 0. */
	LOCAL1, /*!< Locally used facility 1. */
	LOCAL2, /*!< Locally used facility 2. */
	LOCAL3, /*!< Locally used facility 3. */
	LOCAL4, /*!< Locally used facility 4. */
	LOCAL5, /*!< Locally used facility 5. */
	LOCAL6, /*!< Locally used facility 6. */
	LOCAL7, /*!< Locally used facility 7. */
};

/**
 * Format a system uptime timestamp as a string.
 *
 * Using the format "d+HH:mm:ss.SSS" with leading zeros for the days.
 *
 * @param[in] System uptime in milliseconds, see uuid::get_uptime_ms().
 * @param[in] Leading zeros for the days part of the output.
 * @return String with the formatted system uptime.
 */
std::string format_timestamp_ms(uint64_t timestamp_ms, unsigned int days_width = 1);

/**
 * Format a log level as a single character.
 *
 * Level::EMERG is represented as 'P' because it conflicts with
 * Level::ERR and it used to be the "panic" level.
 *
 * @param[in] Log level.
 * @return Single character uppercase representation of the log level.
 */
char format_level_char(Level level);

/**
 * Format a log level as an uppercase string.
 *
 * @param[in] Log level.
 * @return Uppercase name of the log level (flash string).
 */
const __FlashStringHelper *format_level_uppercase(Level level);

/**
 * Format a log level as a lowercase string.
 *
 * @param[in] Log level.
 * @return Lowercase name of the log level (flash string).
 */
const __FlashStringHelper *format_level_lowercase(Level level);

/**
 * Log message text with timestamp and logger attributes.
 *
 * These will be created when a message is logged and then passed to
 * all registered handlers.
 */
struct Message {
	/**
	 * Create a new log message (not directly useful).
	 *
	 * @param[in] uptime_ms System uptime, see uuid::get_uptime_ms().
	 * @param[in] level Severity level of the message.
	 * @param[in] facility Facility type of the process logging the message.
	 * @param[in] name Logger name (flash string).
	 * @param[in] text Log message text.
	 */
	Message(uint64_t uptime_ms, Level level, Facility facility, const __FlashStringHelper *name, const std::string &&text);
	~Message() = default;

	/**
	 * System uptime at the time the message was logged.
	 *
	 * @see uuid::get_uptime_ms()
	 */
	const uint64_t uptime_ms;

	/**
	 * Severity level of the message.
	 */
	const Level level;

	/**
	 * Facility type of the process that logged the message.
	 */
	const Facility facility;

	/**
	 * Name of the logger used (flash string).
	 */
	const __FlashStringHelper *name;

	/**
	 * Formatted log message text.
	 *
	 * Does not include any of the other message attributes, those must
	 * be added by the handler when outputting messages.
	 */
	const std::string text;
};

/**
 * Logger handler used to process log messages.
 */
class Handler {
public:
	virtual ~Handler() = default;

	/**
	 * Add a new log message.
	 *
	 * This should normally be put in a queue instead of being
	 * processed immediately so that log messages have minimal impact
	 * at the time of use.
	 *
	 * Queues should have a maximum size and discard the oldest message
	 * when full.
	 *
	 * @param[in] message New log message, shared by all handlers.
	 */
	virtual void operator<<(std::shared_ptr<Message> message) = 0;

protected:
	Handler() = default;
};

/**
 * Logger instance used to make log messages.
 */
class Logger {
public:
	/**
	 * This is the maximum length of any log message.
	 *
	 * Determines the size of the buffer used for format string
	 * printing.
	 */
	static constexpr size_t MAX_LOG_LENGTH = 255;

	/**
	 * Create a new logger with the given name and logging facility.
	 *
	 * @param[in] name Logger name (flash string).
	 * @param[in] facility Default logging facility for messages.
	 */
	Logger(const __FlashStringHelper *name, Facility facility = Facility::LOCAL0);
	~Logger() = default;

	/**
	 * Register a log handler.
	 *
	 * Call again to change the log level.
	 *
	 * Do not call this function from a static initializer.
	 *
	 * @param[in] handler Handler object that will handle log
	 *                    messages.
	 * @param[in] level Minimum log level that the handler is
	 *                  interested in.
	 */
	static void register_handler(Handler *handler, Level level);

	/**
	 * Unregister a log handler.
	 *
	 * It is safe to call this with a handler that is not registered.
	 *
	 * Do not call this function from a static initializer.
	 *
	 * @param[in] handler Handler object that will no longer handle
	 *                    log messages.
	 */
	static void unregister_handler(Handler *handler);

	/**
	 * Get the current log level of a handler.
	 *
	 * It is safe to call this with a handler that is not registered.
	 *
	 * Do not call this function from a static initializer.
	 *
	 * @param[in] handler Handler object that may handle log
	 *                    messages.
	 * @return The current log level of the specified handler.
	 */
	static Level get_log_level(Handler *handler);

	/**
	 * Determine if the current log level is enabled by any registered
	 * handlers.
	 *
	 * @return The current minimum global log level across all
	 *         handlers.
	 */
	static inline bool enabled(Level level) { return level <= level_; }

	/**
	 * Log a message at level Level::EMERG.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void emerg(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::EMERG.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void emerg(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::ALERT.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void alert(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::ALERT.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void alert(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::CRIT.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void crit(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::CRIT.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void crit(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::ERR.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void err(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::ERR.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void err(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::WARNING.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void warning(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::WARNING.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void warning(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::NOTICE.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void notice(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::NOTICE.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void notice(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::INFO.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void info(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::INFO.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void info(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::DEBUG.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void debug(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::DEBUG.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void debug(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message at level Level::TRACE.
	 *
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void trace(const char *format, ...) const /* __attribute__((format (printf, 2, 3))) */;
	/**
	 * Log a message at level Level::TRACE.
	 *
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void trace(const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 2, 3))) */;

	/**
	 * Log a message with a custom facility.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] facility Facility type of the process logging the message.
	 * @param[in] format Format string.
	 * @param[in] ... Format string arguments.
	 */
	void log(Level level, Facility facility, const char *format, ...) const /* __attribute__((format (printf, 3, 4))) */;
	/**
	 * Log a message with a custom facility.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] facility Facility type of the process logging the message.
	 * @param[in] format Format string (flash string).
	 * @param[in] ... Format string arguments.
	 */
	void log(Level level, Facility facility, const __FlashStringHelper *format, ...) const /* __attribute__((format (printf, 4, 5))) */;

private:
	/**
	 * Refresh the minimum global log level across all handlers.
	 */
	static void refresh_log_level();

	/**
	 * Log a message at the specified level.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] format Format string.
	 * @param[in] ap Variable arguments pointer for format string.
	 */
	void vlog(Level level, const char *format, va_list ap) const;
	/**
	 * Log a message at the specified level.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] format Format string (flash string).
	 * @param[in] ap Variable arguments pointer for format string.
	 */
	void vlog(Level level, const __FlashStringHelper *format, va_list ap) const;

	/**
	 * Log a message at the specified level and facility.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] facility Facility type of the process logging the message.
	 * @param[in] format Format string.
	 * @param[in] ap Variable arguments pointer for format string.
	 */
	void vlog(Level level, Facility facility, const char *format, va_list ap) const;
	/**
	 * Log a message at the specified level and facility.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] facility Facility type of the process logging the message.
	 * @param[in] format Format string (flash string).
	 * @param[in] ap Variable arguments pointer for format string.
	 */
	void vlog(Level level, Facility facility, const __FlashStringHelper *format, va_list ap) const;

	/**
	 * Dispatch a log message to all handlers that are registered to
	 * handle messages of the specified level.
	 *
	 * Automatically sets the timestamp of the message to the current
	 * system uptime.
	 *
	 * @param[in] level Severity level of the message.
	 * @param[in] facility Facility type of the process logging the message.
	 * @param[in] text Log message text.
	 */
	void dispatch(Level level, Facility facility, std::vector<char> &text) const;

	static std::map<Handler*,Level> handlers_; /*<! Registered log handlers. */
	static Level level_; /*<! Minimum global log level across all handlers. */

	const __FlashStringHelper *name_; /*!< Logger name (flash string). */
	const Facility facility_; /*!< Default logging facility for messages. */
};

} // namespace log

} // namespace uuid

#endif
