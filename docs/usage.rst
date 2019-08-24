Usage
=====

.. code:: c++

   #include <uuid/log.h>

Create a |uuid::log::Logger|_ and call its functions for logging
messages.

Create a class that implements |uuid::log::Handler|_ and registers
to handle log messages. Output those messages by whatever means is
appropriate for your application (e.g. serial console, over the WiFi
network, by email).

There is no option to exclude compilation of debug-level messages, the
expectation is that these will be disabled at runtime. If your program
cannot fit in flash with the debug messages enabled or run with
sufficient performance then there's no benefit in having them.

The performance impact of disabled debug logging can be limited by using
the |Logger::enabled(LogLevel)|_ function (but messages will not be
formatted if the level isn't enabled).

Example
-------

.. literalinclude:: ../examples/Serial.cpp

.. |uuid::log::Logger| replace:: ``uuid::log:Logger``
.. _uuid::log::Logger: https://mcu-doxygen.uuid.uk/classuuid_1_1log_1_1Logger.html

.. |uuid::log::Handler| replace:: ``uuid::log:Handler``
.. _uuid::log::Handler: https://mcu-doxygen.uuid.uk/classuuid_1_1log_1_1Handler.html

.. |Logger::enabled(LogLevel)| replace:: ``Logger::enabled(LogLevel)``
.. _Logger::enabled(LogLevel): https://mcu-doxygen.uuid.uk/classuuid_1_1log_1_1Logger.html#af16363367efa67c17c368b9c2bacf0dd
