# log_lib

`log_lib` is a small C++17 logging library with an asynchronous worker. Your application initializes the logger once, pushes log messages into a queue, and the library formats and writes each entry to both stdout and a file.

The default file sink writes to the current working directory, under:

```text
./logs/log_daemon.log
```

The `logs/` directory is created automatically if it does not already exist.

## What It Does

- Accepts log messages through a singleton logger API.
- Buffers messages in an in-memory queue and processes them on a worker thread.
- Formats each record as a single line with timestamp, app name, level, and payload.
- Writes every formatted line to both the console and a log file.
- Builds with CMake and includes a GoogleTest-based test suite.

## Requirements

- Linux
- CMake 3.10 or newer
- A C++17 compiler
- `nlohmann_json`
- GoogleTest, if you want to build and run the unit tests

On Ubuntu or Debian:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake nlohmann-json3-dev libgtest-dev
```

## Build

Configure and build from the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build --parallel
```

The top-level CMake build defines these main targets:

- `logger`
- `log_daemon`
- `unit_test` when `BUILD_TESTING=ON` and GTest is available

## Test

Run the test suite with CTest:

```bash
ctest --test-dir build --output-on-failure
```

If GTest is not available, CMake emits a warning and skips the `unit_test` target.

## Public API

The main entry point is `logger::SLogger` in [`/workspace/src/Logger/include/logger.h`](/workspace/src/Logger/include/logger.h).

```cpp
static SLogger &getInstance();
void init(std::string config_input);
void start();
bool shutdown();
void log(std::string_view log_level, std::string_view payload);
void log(std::string_view payload);
```

Exact behavior:

- `getInstance()` returns the singleton logger.
- `init()` parses a JSON configuration string and creates the queue, formatter, sink manager, and worker.
- `start()` launches the background worker thread.
- `log(level, payload)` logs with an explicit level and generate a log_event and push it into the queue.
- `log(payload)` same. only difference isuses the level from the configuration passed to `init()`.
- `shutdown()` stops the queue, flushes the sinks, and joins the worker thread. It returns `false` if the worker was never started.

## Configuration

`init()` expects a JSON string with these required string keys:

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

If the JSON is malformed or a required field is missing, the parser logs an error to `stderr` and returns empty strings.

## Quick Start

```cpp
#include "logger.h"

int main() {
  auto &log = logger::SLogger::getInstance();

  log.init(R"({"app_name":"demo-app","level":"INFO","pid":"12345"})");
  log.start();

  log.log("service started");
  log.log("ERROR", "database unavailable");

  return log.shutdown() ? 0 : 1;
}
```

## Runtime Behavior

When you call the API in that order:

1. `init()` reads the JSON config and sets the app name and default log level.
2. `init()` creates two sinks: console and file.
3. `init()` prepares the file sink at `./logs/log_daemon.log` relative to the process current working directory.
4. `start()` starts a worker thread that waits for queued log events.
5. `log()` creates a `LogEvent` and pushes it into the queue.
6. The worker formats each event as:

```text
YYYY-MM-DD HH:MM:SS [app_name] [level] payload
```

7. The formatted line is written to stdout and appended to the log file.
8. `shutdown()` stops the queue, drains remaining events, flushes the sinks, and joins the worker thread.

## Important Details

- The file sink appends to the log file rather than truncating it.
- The queue capacity is `200` messages. When it overflows, the oldest queued message is dropped.
- The logger is a singleton, so the same instance should be reused for the whole process.
- There is no install/export package yet; the project is meant to be consumed from the source tree.

## Using It From Another CMake Project

The simplest integration is to add this repository as a subdirectory and link the `logger` target.

```cmake
add_subdirectory(path/to/log_lib)
target_link_libraries(your_app PRIVATE logger)
```

Then include the public header from your application code:

```cpp
#include "logger.h"
```

This works because the `logger` target publishes the header directory through CMake. If you want a packaged install flow, the CMake export/install setup still needs to be added.

## Project Layout

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

CI pipeline currently checks:

- project build
- unit tests
- coverage artifact generation
- static analysis artifact generation
=======
log-lib
