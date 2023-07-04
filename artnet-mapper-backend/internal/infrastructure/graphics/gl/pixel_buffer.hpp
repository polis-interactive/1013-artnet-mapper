//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_PIXEL_BUFFERS_HPP
#define INFRASTRUCTURE_GRAPHICS_PIXEL_BUFFERS_HPP

#include <deque>
#include <memory>
#include <mutex>

#include "../common.hpp"

#include "utility/untyped_buffer.hpp"

namespace infrastructure::graphics {

    class PixelBuffer: public utility::UnsizedBuffer {
    public:
        [[nodiscard]] void *GetMemory() final;
        ~PixelBuffer();
        // no copy assignment, empty assignment
        PixelBuffer() = delete;
        PixelBuffer (const PixelBuffer&) = delete;
        PixelBuffer& operator= (const PixelBuffer&) = delete;
    protected:
        friend class PixelBuffers;
        explicit PixelBuffer(const domain::InstallationSummary &summary);
        void RenderBuffer();
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
        friend class Graphics;
        explicit PixelBuffers(const Config &summary);
        void Setup();
        void Teardown();
        std::deque<PixelBuffer *> _ready_queue;
        std::deque<PixelBuffer *> _render_queue;
        // can't unmap the buffer from non render thread, methinks?
        std::deque<PixelBuffer *> _spent_queue;
        std::mutex _spent_mutex;
    private:
        const domain::InstallationSummary &_summary;
        const unsigned int _pbo_count;
    };
}


#endif //INFRASTRUCTURE_GRAPHICS_PIXEL_BUFFERS_HPP
