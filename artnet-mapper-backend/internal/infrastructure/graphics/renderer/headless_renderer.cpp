//
// Created by brucegoose on 7/5/23.
//

#include "headless_renderer.hpp"

namespace infrastructure::graphics {
    HeadlessRenderer::HeadlessRenderer(const domain::InstallationSummary &summary): Renderer(summary) {}
    bool HeadlessRenderer::Setup() {
        return false;
    }
    void HeadlessRenderer::Teardown() noexcept {}
}