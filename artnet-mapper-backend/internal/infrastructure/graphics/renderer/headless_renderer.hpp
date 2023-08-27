//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP

#include "renderer.hpp"
#include "../gl/full_vao.hpp"
#include "../gl/fbo.hpp"

namespace infrastructure::graphics {

    class HeadlessRenderer: public Renderer {
    public:
        explicit HeadlessRenderer(
                const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
        );
    protected:
        bool SetupContext() final;
        void Setup(GraphicsPtr &graphics) final;
        void Render(GraphicsPtr &graphics, CpuPixelBuffer *buffer) final;
        void Teardown() noexcept final;
    private:
        EGLDisplay _display = 0;
        EGLContext _context = 0;
        graphics::FullVao _full_vao;
        graphics::Fbo _offscreen_buffer;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP
