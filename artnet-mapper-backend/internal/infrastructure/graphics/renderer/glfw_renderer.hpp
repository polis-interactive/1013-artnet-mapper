//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HPP

#include "renderer.hpp"

namespace infrastructure::graphics {

    class GlfwRenderer: public Renderer {
    public:
        explicit GlfwRenderer(const domain::InstallationSummary &summary);
    protected:
        bool Setup() final;
        void Teardown() noexcept final;
    };

}


#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HPP
