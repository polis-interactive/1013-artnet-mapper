//
// Created by broosegoose on 7/23/23.
//

#ifndef SERVICE_EMBEDDED_HPP
#define SERVICE_EMBEDDED_HPP

#include "infrastructure/art_net/art_net.hpp"
#include "infrastructure/asio/context.hpp"
#include "infrastructure/graphics/graphics.hpp"
#include "infrastructure/controls/controls.hpp"

namespace service {

    struct ServiceConfig {
        infrastructure::ArtNetConfig art_net_config;
        infrastructure::AsioContextConfig asio_context_config;
        infrastructure::GraphicsConfig graphics_config;
        infrastructure::ControlsConfig controls_config;
        bool service_run_pipeline;
        bool service_allow_reset;
    };

    class Service;
    typedef std::shared_ptr<Service> ServicePtr;

    class Service:
        public infrastructure::GraphicsManager,
        public infrastructure::ControlsManager,
        public std::enable_shared_from_this<Service>
    {
    public:
        [[nodiscard]] static ServicePtr Create(const ServiceConfig &config);
        Service();
        void Start();
        void Stop();
        void Unset();

        /* Manager Members */
        // graphics
        void PostGraphicsUpdate(utility::SizedBufferPtr &&pixels) final;
        void RequestReboot() final;
        // controls
        void PostPotentiometerUpdate(const float new_pot_read) final;


        // no copy assignment
        Service (const Service&) = delete;
        Service& operator= (const Service&) = delete;
    private:
        void initialize(const ServiceConfig &config);
        bool _run_pipeline = true;
        bool _allow_reset = true;
        std::atomic_bool _is_started = false;
        infrastructure::ArtNetPtr _art_net = nullptr;
        infrastructure::AsioContextPtr _asio_context = nullptr;
        infrastructure::GraphicsPtr _graphics = nullptr;
        infrastructure::ControlsPtr _controls = nullptr;
    };
}

#endif //SERVICE_EMBEDDED_HPP
