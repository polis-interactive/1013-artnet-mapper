//
// Created by brucegoose on 7/5/23.
//

#include "headless_renderer.hpp"

namespace infrastructure::graphics {
    HeadlessRenderer::HeadlessRenderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier):
        Renderer(dimensions, pixel_multiplier)
    {}

    bool HeadlessRenderer::SetupContext() {
        return false;
    }

    void HeadlessRenderer::Setup(GraphicsPtr &graphics) {
    }

    void HeadlessRenderer::Render(infrastructure::GraphicsPtr &graphics, PixelBuffer *pbo) {

    }
    void HeadlessRenderer::Teardown() noexcept {}
}