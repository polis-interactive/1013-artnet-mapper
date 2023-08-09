//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_PIXEL_BUFFERS_HPP
#define INFRASTRUCTURE_GRAPHICS_PIXEL_BUFFERS_HPP

#include <deque>
#include <memory>
#include <mutex>

#include "../common.hpp"

#include "utility/buffers.hpp"

namespace infrastructure::graphics {

    class PixelBuffer: public utility::SizedBuffer {
    public:
        [[nodiscard]] uint8_t *GetMemory() final;
        [[nodiscard]] std::size_t GetSize() final;
        ~PixelBuffer();
        // no copy assignment, empty assignment
        PixelBuffer() = delete;
        PixelBuffer (const PixelBuffer&) = delete;
        PixelBuffer& operator= (const PixelBuffer&) = delete;
    protected:
        friend class PixelBuffers;
        explicit PixelBuffer(const domain::installation::Config &installation_config);
        void RenderBuffer() const;
        void MapBuffer();
        void UnmapBuffer();
    private:
        const GLsizei _width;
        const GLsizei _height;
        const GLenum _format;
        GLuint _pbo = 0;
        void *_mapped_ptr = nullptr;
    };

    class PixelBuffers {
    public:
        // no copy assignment, empty assignment
        PixelBuffers() = delete;
        PixelBuffers (const PixelBuffers&) = delete;
        PixelBuffers& operator= (const PixelBuffers&) = delete;
    protected:
        friend class infrastructure::Graphics;
        explicit PixelBuffers(const domain::installation::Config &installation_config);
        void Setup();
        void Teardown();
        std::deque<PixelBuffer *> _ready_queue;
        std::deque<PixelBuffer *> _render_queue;
        // can't unmap the buffer from non render thread, methinks?
        std::deque<PixelBuffer *> _spent_queue;
        std::mutex _spent_mutex;
    private:
        const domain::installation::Config &_installation_config;
    };
}


#endif //INFRASTRUCTURE_GRAPHICS_PIXEL_BUFFERS_HPP
