//
// Created by brucegoose on 7/5/23.
//

#include "pixel_buffer.hpp"

namespace infrastructure::graphics {

    PixelBuffers::PixelBuffers(const Config &config):
        _summary(config.graphics_summary), _pbo_count(config.graphics_pbo_count)
    {}

    void PixelBuffers::Setup() {
        std::generate_n(std::back_inserter(_ready_queue), _pbo_count, [&summary = _summary]() -> PixelBuffer* {
            return new PixelBuffer(summary);
        });
    }

    void PixelBuffers::Teardown() {
        std::unique_lock<std::mutex> lock(_spent_mutex);
        PixelBuffer *buffer = nullptr;
        while(!_spent_queue.empty()) {
            buffer = _spent_queue.front();
            _spent_queue.pop_front();
            delete buffer;
        }
        while (_ready_queue.empty()) {
            buffer = _ready_queue.front();
            _ready_queue.pop_front();
            delete buffer;
        }
        while (_ready_queue.empty()) {
            buffer = _ready_queue.front();
            _ready_queue.pop_front();
            delete buffer;
        }
    }

    PixelBuffer::PixelBuffer(const domain::InstallationSummary &summary):
        _width(summary.width),
        _height(summary.height),
        _format(summary.pixel_type == domain::PixelType::RGB ? GL_RGB : GL_RGBA)
    {
        glGenBuffers(1, &_pbo);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        const auto size_multiplier = _format == GL_RGB ? 3 : 4;
        glBufferData(GL_PIXEL_PACK_BUFFER, _width * _height * size_multiplier, 0, GL_STREAM_READ);
    }

    PixelBuffer::~PixelBuffer() {
        if (_mapped_ptr != nullptr) {
            UnmapBuffer();
        }
        glDeleteBuffers(1, &_pbo);
    }

    void PixelBuffer::RenderBuffer() {
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, 0);
    }

    void PixelBuffer::MapBuffer() {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        _mapped_ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    }

    void *PixelBuffer::GetMemory() {
        return _mapped_ptr;
    }

    void PixelBuffer::UnmapBuffer() {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        _mapped_ptr = nullptr;
    }

}