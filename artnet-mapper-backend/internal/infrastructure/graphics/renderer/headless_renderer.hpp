//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP

#include "renderer.hpp"

namespace infrastructure::graphics {

    class HeadlessRenderer: public Renderer {
    public:
        explicit HeadlessRenderer(const domain::InstallationSummary &summary);
    protected:
        bool Setup() final;
        void Teardown() noexcept final;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_HEADLESS_HPP
