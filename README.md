# log_lib

`log_lib` is a small C++ logging library built around an asynchronous logging pipeline.
It accepts log events, queues them, formats them, and writes them to both the console and a file sink.

## Features

- asynchronous logging through a background worker thread
- singleton logger API for simple integration
- console sink and file sink enabled by default
- JSON-based logger initialization
- unit tests with GoogleTest

Default file output path:
- `/tmp/log_daemon.log`

## Log Format

Each message is formatted like this:

```text
YYYY-MM-DD HH:MM:SS [app_name] [level] payload
```

Example:

```text
2026-03-22 12:00:00 [my-app] [INFO] service started
```

## Requirements

- Linux
- CMake 3.10+
- C++ compiler with C++17 support
- `nlohmann_json`
- GoogleTest for unit tests

Ubuntu or Debian:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake nlohmann-json3-dev libgtest-dev
```

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build --parallel
```

## Run Tests

```bash
ctest --test-dir build --output-on-failure
```

## Quick Start

### Required configuration

`logger::SLogger::init()` expects a JSON string with these keys:

- `app_name`
- `level`
- `pid`

Example:

```json
{
  "app_name": "demo-app",
  "level": "INFO",
  "pid": "12345"
}
```

### Severity Suggestions for your project
in this project you can adjust your severity levels however you want but our suggestions are:
  1- INFO: for normal info logs
  2- WARNING: for non-critical warning situations
  3- ERROR: for critical error situations
  4- CRITICAL: for most critical errors have a chance to be happened somehow.

### Minimal example

```cpp
#include "src/Logger/include/logger.h"

#include <string>

int main() {
  auto &log = logger::SLogger::getInstance();

  log.init(R"({"app_name":"demo-app","level":"INFO","pid":"12345"})");
  log.start();

  std::string payload = "service started";
  log.log(payload); //if using with default log level coming from your json input in init state or:
  log.log("ERROR", payload); // if wanna use the logger with custom level.

  if (!log.shutdown()) {
    return 1;
  }

  return 0;
}
```

### Expected result

The example above writes the same formatted log line to:

- standard output
- `/tmp/log_daemon.log`

## Public API

The main user-facing type is `logger::SLogger`.

```cpp
static SLogger &getInstance();
void init(std::string config_input);
void start();
bool shutdown();
void log(std::string &log_level, std::string &payload);
```

Lifecycle:

1. Call `init()` once.
2. Call `start()` once.
3. Send log messages through `log()`.
4. Call `shutdown()` before process exit.

Behavior notes:

- `init()` creates the queue, formatter, sinks, and worker.
- `start()` launches the worker thread.
- `log()` pushes a `LogEvent` into the queue.
- `shutdown()` stops the queue, joins the worker thread, and flushes the sinks.
- `shutdown()` returns `false` if the worker thread was never started.

## Usage Example

```cpp
#include "src/Logger/include/logger.h"

#include <string>

int main() {
  auto &log = logger::SLogger::getInstance();
  log.init(R"({"app_name":"payment-service","level":"INFO","pid":"2026"})");
  log.start();

  log.log("INFO", "service started");
  log.log("WARN", "downstream timeout, retry scheduled");

  return log.shutdown() ? 0 : 1;
}
```

## CMake Targets

This repository currently builds these internal targets:

- `logger`
- `log_event`
- `queue`
- `formatter`
- `worker_thread`
- `sink`
- `sink_interface`
- `sink_manager`
- `utils`
- `log_daemon`

Build entry point:

```bash
cmake -S . -B build
cmake --build build --parallel
```

## Configuration Reference

| Key | Type | Description |
| --- | --- | --- |
| `app_name` | string | Application name shown in each log line |
| `level` | string | Configured log level value |
| `pid` | string | Process identifier parsed from config |

Example:

```cpp
log.init(R"({"app_name":"billing","level":"DEBUG","pid":"9001"})");
```

## Output Sinks

The current implementation enables both sinks automatically during `init()`:

- `ConsoleSink`: writes to standard output
- `FileSink`: appends to `/tmp/log_daemon.log`

## Project Structure

```text
.
├── CMakeLists.txt
├── README.md
├── constants.h
├── main.cpp
├── src
│   ├── Logger
│   │   ├── include
│   │   │   ├── log_event.h
│   │   │   └── logger.h
│   │   └── src
│   │       ├── log_event.cpp
│   │       └── logger.cpp
│   ├── Queue
│   │   ├── include
│   │   │   ├── blocking_queue.h
│   │   │   ├── formatter.h
│   │   │   └── worker_thread.h
│   │   └── src
│   │       ├── blocking_queue.cpp
│   │       ├── formatter.cpp
│   │       └── worker_thread.cpp
│   ├── Sink
│   │   ├── include
│   │   │   ├── console_sink.h
│   │   │   ├── constants.h
│   │   │   ├── file_sink.h
│   │   │   ├── sink_factory.h
│   │   │   ├── sink_interface.h
│   │   │   └── sink_manager.h
│   │   └── src
│   │       ├── console_sink.cpp
│   │       ├── file_sink.cpp
│   │       ├── sink_interface.cpp
│   │       └── sink_manager.cpp
│   └── utils
│       └── include
│           └── json_parser.h
└── tests
    ├── formatter_test.cpp
    ├── log_event_test.cpp
    ├── logger_test.cpp
    ├── parser_test.cpp
    ├── queue_test.cpp
    ├── sink_factory_test.cpp
    └── sink_test.cpp
```

## Current Limitations

Current gaps to be aware of before integrating:

- the logger is exposed as a singleton only
- sink selection is not configurable through the public API yet
- the configured `level` value is stored, but log-level filtering is not enforced yet
- the configured `pid` value is parsed, but not included in the formatted output yet
- file output path is fixed to the default `FileSink` path unless the implementation is extended

## Development Commands

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

CI currently checks:

- project build
- unit tests
- coverage artifact generation
- static analysis artifact generation
