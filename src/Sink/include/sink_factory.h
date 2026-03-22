#pragma once
#include "sink_interface.h"
#include "console_sink.h"
#include "file_sink.h"
#include "constants.h"
#include <memory>

namespace sink {
    class SinkFactory {
    public:
        std::unique_ptr<ISink> Create(SinkFactoryEnum attr) {
            std::unique_ptr<ISink> sink;
            if (attr == SinkFactoryEnum::kConsole) {
                sink = std::make_unique<ConsoleSink>();
            } else {
                sink = std::make_unique<FileSink>();
            }
            return sink;
        }
    };
}
