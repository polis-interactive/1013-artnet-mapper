//
// Created by brucegoose on 7/5/23.
//

#include "pixel_buffer.hpp"

namespace infrastructure::graphics {

    PixelBuffers::PixelBuffers(const domain::installation::Config &installation_config):
        _installation_config(installation_config)
    {}

    void PixelBuffers::Setup() {
        std::generate_n(
            std::back_inserter(_ready_queue),
            _installation_config.buffer_count,
            [&installation_config = _installation_config]() -> PixelBuffer* {
                return new PixelBuffer(installation_config);
            }
        );
    }


    void PixelBuffers::Teardown() {
        std::unique_lock<std::mutex> lock(_spent_mutex);
        PixelBuffer *buffer = nullptr;
        while(!_spent_queue.empty()) {
            buffer = _spent_queue.front();
            _spent_queue.pop_front();
            delete buffer;
        }
        while (_render_queue.empty()) {
            buffer = _render_queue.front();
            _render_queue.pop_front();
            delete buffer;
        }
        while (_ready_queue.empty()) {
            buffer = _ready_queue.front();
            _ready_queue.pop_front();
            delete buffer;
        }
    }

    PixelBuffer::PixelBuffer(const domain::installation::Config &installation_config):
        _width((int) installation_config.dimensions.width),
        _height((int )installation_config.dimensions.height),
        _format(installation_config.rgbw_pixels ? GL_RGBA : GL_RGB)
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

    void PixelBuffer::RenderBuffer() const {
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, 0);
    }

    bool PixelBuffer::MapBuffer() {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        _mapped_ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        return _mapped_ptr != nullptr;
    }

    uint8_t *PixelBuffer::GetMemory() {
        return (uint8_t *) _mapped_ptr;
    }

    std::size_t PixelBuffer::GetSize() {
        const auto size_multiplier = _format == GL_RGB ? 3 : 4;
        return _width * _height * size_multiplier;
    }

    void PixelBuffer::UnmapBuffer() {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        _mapped_ptr = nullptr;
    }

}