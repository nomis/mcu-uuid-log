/*
 * uuid-log - Microcontroller logging framework
 * Copyright 2019,2022  Simon Arlott
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

#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#define PROGMEM
#define PGM_P const char *
#define PSTR(s) (__extension__({static const char __c[] = (s); &__c[0];}))

class __FlashStringHelper;
#define FPSTR(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))
#define F(string_literal) (FPSTR(PSTR(string_literal)))

#define strcmp_P strcmp
#define snprintf_P snprintf
#define vsnprintf_P vsnprintf

class Print;

class Printable {
public:
    virtual size_t printTo(Print &print) const = 0;
};

class Print {
public:
    virtual size_t write(uint8_t c) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    size_t print(char c) { return write((uint8_t)c); }
    size_t print(const char *data) { return write(reinterpret_cast<const uint8_t *>(data), strlen(data)); }
    size_t print(const __FlashStringHelper *data) { return print(reinterpret_cast<const char *>(data)); }
    size_t print(const Printable &printable) { return printable.printTo(*this); }
    size_t print(int value) { return print(std::to_string(value).c_str()); }
    size_t print(unsigned int value) { return print(std::to_string(value).c_str()); }
    size_t print(long value) { return print(std::to_string(value).c_str()); }
    size_t print(unsigned long value) { return print(std::to_string(value).c_str()); }
    size_t println() { return print("\r\n"); }
    size_t println(const char *data) { return print(data) + println(); }
    size_t println(const __FlashStringHelper *data) { return print(reinterpret_cast<const char *>(data)) + println(); }
    size_t println(const Printable &printable) { return printable.printTo(*this) + println(); }
    size_t println(int value) { return print(std::to_string(value).c_str()) + println(); }
    size_t println(unsigned int value) { return print(std::to_string(value).c_str()) + println(); }
    size_t println(long value) { return print(std::to_string(value).c_str()) + println(); }
    size_t println(unsigned long value) { return print(std::to_string(value).c_str()) + println(); }
    virtual void flush() { };
};

static __attribute__((unused)) void yield(void) { }

#endif
