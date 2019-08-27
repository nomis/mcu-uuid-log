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
#ifdef ENV_NATIVE

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>

#include <string>

NativeConsole Serial;

static unsigned long __millis = 0;

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
	setup();
	while (millis() <= 10 * 1000) {
		loop();
	}
	return 0;
}

unsigned long millis() {
	return __millis;
}

void delay(unsigned long millis) {
	__millis += millis;
}

void yield(void) {

}

int snprintf_P(char *str, size_t size, const char *format, ...) {
	va_list ap;

	va_start(ap, format);
	int ret = vsnprintf_P(str, size, format, ap);
	va_end(ap);

	return ret;
}

int vsnprintf_P(char *str, size_t size, const char *format, va_list ap) {
	std::string native_format;

	char previous = 0;
	for (size_t i = 0; i < strlen(format); i++) {
		char c = format[i];

		// This would be a lot easier if the ESP8266 platform
		// simply read all strings with 32-bit accesses instead
		// of repurposing %S (wchar_t).
		if (previous == '%' && c == 'S') {
			c = 's';
		}

		native_format += c;
		previous = c;
	}

	return vsnprintf(str, size, native_format.c_str(), ap);
}

#endif
