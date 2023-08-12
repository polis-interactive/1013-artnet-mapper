//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_HPP

#include <memory>

#include "../common.hpp"

#include "../gl/pixel_buffer.hpp"

namespace infrastructure::graphics {

    class Renderer {
    public:
        // no copy assignment, no empty assignment
        Renderer() = delete;
        Renderer (const Renderer&) = delete;
        Renderer& operator= (const Renderer&) = delete;
    protected:
        friend class infrastructure::Graphics;
        static std::unique_ptr<Renderer> Create(
            const domain::RendererType &render_type,
            const domain::Dimensions &dimensions,
            const unsigned int &pixel_multiplier
        );
        virtual bool SetupContext() = 0;
        virtual void Setup(GraphicsPtr &graphics) = 0;
        virtual void Render(GraphicsPtr &graphics, PixelBuffer *pbo) = 0;
        virtual void Teardown() noexcept = 0;
    protected:
        explicit Renderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier);
        const domain::Dimensions &_dimensions;
        const unsigned int _multiplier;
    };
}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_HPP
