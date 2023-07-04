//
// Created by brucegoose on 7/5/23.
//

#include "glfw_renderer.hpp"

namespace infrastructure::graphics {
    GlfwRenderer::GlfwRenderer(const domain::InstallationSummary &summary): Renderer(summary) {}
    bool GlfwRenderer::Setup() {
        return false;
    }
    void GlfwRenderer::Teardown() noexcept {}
}