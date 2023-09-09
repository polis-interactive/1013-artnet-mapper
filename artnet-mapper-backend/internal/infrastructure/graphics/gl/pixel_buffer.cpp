//
// Created by brucegoose on 7/5/23.
//

#include <thread>
#include "pixel_buffer.hpp"

#include "utility/clock.hpp"

namespace infrastructure::graphics {

    PixelBuffers::PixelBuffers(const domain::installation::Config &installation_config):
        _installation_config(installation_config)
    {}

    void PixelBuffers::Setup() {
        for (int i = 0; i < _installation_config.buffer_count; i++) {
            auto pbo = new PixelBuffer(_installation_config);
            _ready_queue.push_back(pbo);
        }
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
        const auto pixel_size = _format == GL_RGB ? 3 : 4;
        glBufferData(GL_PIXEL_PACK_BUFFER, _width * _height * pixel_size * 2, 0, GL_STREAM_READ);
        ThrowOnGlError("PixelBuffer::PixelBuffer: Failed to initialize pbo");
    }

    PixelBuffer::~PixelBuffer() {
        if (_mapped_ptr != nullptr) {
            UnmapBuffer();
        }
        glDeleteBuffers(1, &_pbo);
    }

    void PixelBuffer::RenderBuffer() const {
        std::cout << _pbo << std::endl;
        std::cout << _width << std::endl;
        std::cout << _height << std::endl;
        std::cout << _format << std::endl;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(0, 0, _width, _height, _format, GL_UNSIGNED_BYTE, 0);
    }

    bool PixelBuffer::MapBuffer() {
        GLint is_mapped = 0;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            // Handle or log the error
            printf("OpenGL Error before mapping: %d\n", error);
        }
        glGetIntegerv(GL_BUFFER_MAPPED, &is_mapped);
        printf("buffer is%s mapped\n", is_mapped ? "" : " not");
        std::this_thread::sleep_for(100ms);
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