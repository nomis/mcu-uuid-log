Usage
=====

.. code:: c++

   #include <uuid/log.h>

Create a ``uuid::log::Logger`` and call its functions for logging
messages.

Create a class that implements ``uuid::log::Handler`` and registers
to handle log messages. Output those messages by whatever means is
appropriate for your application (e.g. serial console, over the WiFi
network, by email).

There is no option to exclude compilation of debug-level messages, the
expectation is that these will be disabled at runtime. If your program
cannot fit in flash with the debug messages enabled or run with
sufficient performance then there's no benefit in having them.

The performance impact of disabled debug logging can be limited by using
the ``Logger::enabled(level)`` function (but messages will not be
formatted if the level isn't enabled).

Example
-------

.. literalinclude:: ../examples/Serial.cpp
