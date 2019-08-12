Change log
==========

Unreleased_
-----------

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

.. _Unreleased: https://github.com/nomis/mcu-uuid-log/compare/1.0.1...HEAD
.. _1.0.1: https://github.com/nomis/mcu-uuid-log/compare/1.0.0...1.0.1
.. _1.0.0: https://github.com/nomis/mcu-uuid-log/commits/1.0.0
