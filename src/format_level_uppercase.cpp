/*
 * uuid-log - Microcontroller logging framework
 * Copyright 2019,2021-2022  Simon Arlott
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

#include <string>

#ifndef PSTR_ALIGN
# define PSTR_ALIGN 4
#endif

namespace uuid {

namespace log {

//! @cond false
static constexpr const char *pstr_level_uppercase_off __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "OFF";
static constexpr const char *pstr_level_uppercase_emerg __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "EMERG";
static constexpr const char *pstr_level_uppercase_crit __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "CRIT";
static constexpr const char *pstr_level_uppercase_alert __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "ALERT";
static constexpr const char *pstr_level_uppercase_err __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "ERR";
static constexpr const char *pstr_level_uppercase_warning __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "WARNING";
static constexpr const char *pstr_level_uppercase_notice __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "NOTICE";
static constexpr const char *pstr_level_uppercase_info __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "INFO";
static constexpr const char *pstr_level_uppercase_debug __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "DEBUG";
static constexpr const char *pstr_level_uppercase_trace __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "TRACE";
static constexpr const char *pstr_level_uppercase_all __attribute__((__aligned__(PSTR_ALIGN))) PROGMEM = "ALL";

static const __FlashStringHelper *log_level_uppercase[(int)Level::ALL - (int)Level::OFF + 1] = {
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_off),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_emerg),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_crit),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_alert),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_err),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_warning),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_notice),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_info),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_debug),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_trace),
	reinterpret_cast<const __FlashStringHelper *>(pstr_level_uppercase_all)
};
//! @endcond

const __FlashStringHelper *format_level_uppercase(Level level) {
	return log_level_uppercase[(int)level + 1];
}

} // namespace log

} // namespace uuid
