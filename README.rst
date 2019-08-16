Logging framework |Build Status|
================================

Provides a framework for handling log messages. This library is for
single threaded applications and cannot be used from an interrupt
context.

There is no option to exclude compilation of debug-level messages, the
expectation is that these will be disabled at runtime. If your program
cannot fit in flash with the debug messages enabled or run with
sufficient performance then there's no benefit in having them.

The performance impact of disabled debug logging can be limited by using
the ``Logger::enabled(level)`` function (but messages will not be
formatted if the level isn't enabled).

* `Change log <CHANGELOG.rst>`_

Usage
-----

Create a ``uuid::log::Logger`` and call its functions for logging
messages.

Create a class that implements ``uuid::log::Handler`` and registers
to handle log messages. Output those messages by whatever means is
appropriate for your application (e.g. serial console, over the WiFi
network, by email).

.. |Build Status| image:: https://travis-ci.org/nomis/mcu-uuid-log.svg?branch=master
   :target: https://travis-ci.org/nomis/mcu-uuid-log
