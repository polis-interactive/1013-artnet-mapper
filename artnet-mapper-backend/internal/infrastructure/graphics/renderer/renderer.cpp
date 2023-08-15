//
// Created by brucegoose on 7/5/23.
//

#include "renderer.hpp"

#include "headless_renderer.hpp"

#ifdef _GLFW_RENDERER_
#include "glfw_renderer.hpp"
#include "dummy.hpp"
#endif


namespace infrastructure::graphics {
    std::unique_ptr<Renderer> Renderer::Create(
        const domain::RendererType &render_type,
        const domain::Dimensions &dimensions,
        const unsigned int &pixel_multiplier
    ) {
        switch(render_type) {
#ifdef _GLFW_RENDERER_
            case domain::RendererType::GLFW:
                return std::make_unique<GlfwRenderer>(dimensions, pixel_multiplier);
#endif
            case domain::RendererType::HEADLESS:
                return std::make_unique<HeadlessRenderer>(dimensions, pixel_multiplier);
            case domain::RendererType::DUMMY:
                return std::make_unique<DummyRenderer>(dimensions, pixel_multiplier);
            default:
                throw std::runtime_error("Selected graphics unavailable... ");
        }
    }

    Renderer::Renderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier):
        _dimensions(dimensions),
        _multiplier(pixel_multiplier)
    {}
}