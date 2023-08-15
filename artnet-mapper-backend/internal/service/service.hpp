//
// Created by broosegoose on 7/23/23.
//

#ifndef SERVICE_EMBEDDED_HPP
#define SERVICE_EMBEDDED_HPP

#include "infrastructure/art_net/art_net.hpp"
#include "infrastructure/asio/context.hpp"
#include "infrastructure/graphics/graphics.hpp"

namespace service {

    struct ServiceConfig {
        infrastructure::ArtNetConfig art_net_config;
        infrastructure::AsioContextConfig asio_context_config;
        infrastructure::GraphicsConfig graphics_config;
        bool run_pipeline;
    };

    class Service;
    typedef std::shared_ptr<Service> ServicePtr;

    class Service:
        public infrastructure::GraphicsManager,
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


        // no copy assignment
        Service (const Service&) = delete;
        Service& operator= (const Service&) = delete;
    private:
        void initialize(const ServiceConfig &config);
        bool _run_pipeline = true;
        std::atomic_bool _is_started = false;
        infrastructure::ArtNetPtr _art_net;
        infrastructure::AsioContextPtr _asio_context;
        infrastructure::GraphicsPtr _graphics;
    };
}

#endif //SERVICE_EMBEDDED_HPP
