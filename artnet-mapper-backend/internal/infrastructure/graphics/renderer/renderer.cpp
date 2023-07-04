//
// Created by brucegoose on 7/5/23.
//

#include "renderer.hpp"

#include "headless_renderer.hpp"

#ifdef _GLFW_RENDERER_
#include "glfw_renderer.hpp"
#endif

namespace infrastructure::graphics {
    std::unique_ptr<Renderer> Renderer::Create(
        const infrastructure::graphics::RendererType render_type, const domain::InstallationSummary &summary
    ) {
        switch(render_type) {
#ifdef _GLFW_RENDERER_
            case RendererType::GLFW:
                return std::make_unique<GlfwRenderer>(summary);
#endif
            case RendererType::HEADLESS:
                return std::make_unique<HeadlessRenderer>(summary);
            default:
                throw std::runtime_error("Selected graphics unavailable... ");
        }
    }

    Renderer::Renderer(const domain::InstallationSummary &summary): _summary(summary) {}
}