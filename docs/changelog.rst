Change log
==========

Unreleased_
-----------

2.3.0_ |--| 2022-10-25
----------------------

Be thread-safe where possible.

Added
~~~~~

* Function to get a ``Logger``'s default facility for new messages.
* Indicate whether this version of the library is thread-safe or not
  (``UUID_LOG_THREAD_SAFE`` and ``uuid::log::thread_safe``).

Changed
~~~~~~~

* Make the library thread-safe when supported by the platform.

2.2.0_ |--| 2022-01-29
----------------------

Add a basic log handler for the Print interface.

Added
~~~~~

* Basic log handler for writing messages to any object supporting the
  ``Print`` interface (``PrintHandler``).

2.1.4_ |--| 2021-06-02
----------------------

Fix for compile failure with newer GCC in espressif8266 3.0.0.

Fixed
~~~~~

* Compile failure with newer GCC in espressif8266 3.0.0 using
  ``reinterpret_cast`` for a ``constexpr`` value.

2.1.3_ |--| 2021-04-18
----------------------

Upgrade to PlatformIO 5.

Changed
~~~~~~~

* Use PlatformIO 5 dependency specification.

2.1.2_ |--| 2020-01-17
----------------------

Fixes for uncontrolled ordering of static object lifetimes.

Changed
~~~~~~~

* Automatically unregister handlers when they are destroyed.

Fixed
~~~~~

* Make registration of log handlers safe during static initialization
  and unregistration safe during static deinitialization.

2.1.1_ |--| 2019-09-15
----------------------

Fix to use less memory.

Fixed
~~~~~

* Use a flash string for the format string in ``format_timestamp_ms()``.

2.1.0_ |--| 2019-09-07
----------------------

Add functions to get levels, format them as strings and parse them

Added
~~~~~

* Functions to get a list of all levels, format them as uppercase or
  lowercase strings and then parse them back again.

2.0.4_ |--| 2019-08-26
----------------------

Fix uptime format string.

Changed
~~~~~~~

* Put formatting functions in a separate file to improve linker
  behaviour.

Fixed
~~~~~

* Uptime format string should use the unsigned conversion specifier for
  all values.

2.0.3_ |--| 2019-08-17
----------------------

Make ``get_log_level()`` accept a ``const Handler``.

Fixed
~~~~~

* Make ``get_log_level()`` accept a ``const Handler``.

2.0.2_ |--| 2019-08-17
----------------------

Make logging functions ``const`` so that a ``const Logger`` can be used.

Changed
~~~~~~~

* Make all of the logging functions ``const``.

2.0.1_ |--| 2019-08-16
----------------------

Fix ESP32 builds and the example.

Fixed
~~~~~

* Workaround incorrect definition of ``FPSTR()`` on ESP32
  (`#1371 <https://github.com/espressif/arduino-esp32/issues/1371>`_).
* Remove use of ``Serial.printf_P()`` from the example (which does not
  exist in the standard Arduino library).
* Add missing ``Serial.begin()`` to the example.

2.0.0_ |--| 2019-08-12
----------------------

Improve names of types, data members and interfaces.

Changed
~~~~~~~

* The names of ``Message`` data members no longer have trailing
  underscores.
* Rename log ``Receiver`` type to ``Handler``.
* Use operator ``<<`` instead of ``add_log_message()`` as the interface
  for log handlers.

1.0.1_ |--| 2019-08-12
----------------------

Fix uptime text formatting.

Changed
~~~~~~~

* The size of the ``Level`` and ``Facility`` enums is now 1 byte.

Fixed
~~~~~

* The width of the days part when formatting uptimes now has an upper
  bound of 10 instead of a lower bound of 10.

1.0.0_ |--| 2019-08-11
----------------------

First stable release.

Added
~~~~~

* Support for logging messages at all syslog levels and facilities.
* Support for registering receivers to handle log messages.

.. |--| unicode:: U+2013 .. EN DASH

.. _Unreleased: https://github.com/nomis/mcu-uuid-log/compare/2.3.0...HEAD
.. _2.3.0: https://github.com/nomis/mcu-uuid-log/compare/2.2.0...2.3.0
.. _2.2.0: https://github.com/nomis/mcu-uuid-log/compare/2.1.4...2.2.0
.. _2.1.4: https://github.com/nomis/mcu-uuid-log/compare/2.1.3...2.1.4
.. _2.1.3: https://github.com/nomis/mcu-uuid-log/compare/2.1.2...2.1.3
.. _2.1.2: https://github.com/nomis/mcu-uuid-log/compare/2.1.1...2.1.2
.. _2.1.1: https://github.com/nomis/mcu-uuid-log/compare/2.1.0...2.1.1
.. _2.1.0: https://github.com/nomis/mcu-uuid-log/compare/2.0.4...2.1.0
.. _2.0.4: https://github.com/nomis/mcu-uuid-log/compare/2.0.3...2.0.4
.. _2.0.3: https://github.com/nomis/mcu-uuid-log/compare/2.0.2...2.0.3
.. _2.0.2: https://github.com/nomis/mcu-uuid-log/compare/2.0.1...2.0.2
.. _2.0.1: https://github.com/nomis/mcu-uuid-log/compare/2.0.0...2.0.1
.. _2.0.0: https://github.com/nomis/mcu-uuid-log/compare/1.0.1...2.0.0
.. _1.0.1: https://github.com/nomis/mcu-uuid-log/compare/1.0.0...1.0.1
.. _1.0.0: https://github.com/nomis/mcu-uuid-log/commits/1.0.0
