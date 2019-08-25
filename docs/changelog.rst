Change log
==========

Unreleased_
-----------

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

.. _Unreleased: https://github.com/nomis/mcu-uuid-log/compare/2.0.3...HEAD
.. _2.0.3: https://github.com/nomis/mcu-uuid-log/compare/2.0.2...2.0.3
.. _2.0.2: https://github.com/nomis/mcu-uuid-log/compare/2.0.1...2.0.2
.. _2.0.1: https://github.com/nomis/mcu-uuid-log/compare/2.0.0...2.0.1
.. _2.0.0: https://github.com/nomis/mcu-uuid-log/compare/1.0.1...2.0.0
.. _1.0.1: https://github.com/nomis/mcu-uuid-log/compare/1.0.0...1.0.1
.. _1.0.0: https://github.com/nomis/mcu-uuid-log/commits/1.0.0
