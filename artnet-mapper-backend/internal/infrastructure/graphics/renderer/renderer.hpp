//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_HPP

#include <memory>

#include "../common.hpp"

namespace infrastructure::graphics {

    class Renderer {
    public:
        // no copy assignment, no empty assignment
        Renderer() = delete;
        Renderer (const Renderer&) = delete;
        Renderer& operator= (const Renderer&) = delete;
        virtual ~Renderer() = default;
    protected:
        friend class Graphics;
        static std::unique_ptr<Renderer> Create(
            const RendererType render_type, const domain::InstallationSummary &summary
        );
        virtual bool Setup() = 0;
        virtual void Teardown() noexcept = 0;
    private:
        friend class GlfwRenderer;
        friend class HeadlessRenderer;
        explicit Renderer(const domain::InstallationSummary &summary);
        const domain::InstallationSummary &_summary;
    };
}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_HPP
