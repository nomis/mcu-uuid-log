Logging framework
=================

Provides a framework for handling log messages. This library is
for single threaded applications and cannot be used from an interrupt
context (unless you disable interrupts every time you use any of the
functions in ``uuid::log::Logger``, which would be quite inconvenient).

There is no option to exclude compilation of debug-level messages, the
expectation is that these will be disabled at runtime. If your program
cannot fit in flash with the debug messages enabled then there's no
benefit in having them.

The performance impact of disabled debug logging can be limited by using
the ``uuid::log::Logger::enabled(uuid::log::Level level)`` function (but
messages will not be formatted if the level isn't enabled).

* `Change log <CHANGELOG.rst>`_

Usage
-----

Create a ``uuid::log::Logger`` and call its functions for logging
messages.

Create a class that implements ``uuid::log::Receiver`` and registers
to receive log messages. Output those messages by whatever means is
appropriate for your application (e.g. serial console, over the WiFi
network, by email).
