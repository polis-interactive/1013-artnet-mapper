//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP

#include "renderer.hpp"

namespace infrastructure::graphics {

    class HeadlessRenderer: public Renderer {
    public:
        explicit HeadlessRenderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier);
    protected:
        bool SetupContext() final;
        void Setup(GraphicsPtr &graphics) final;
        void Render(GraphicsPtr &graphics, PixelBuffer *pbo) final;
        void Teardown() noexcept final;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP
