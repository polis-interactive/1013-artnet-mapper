//
// Created by broosegoose on 7/23/23.
//

#include "service.hpp"

namespace service {

    ServicePtr Service::Create(const service::ServiceConfig &config) {
        auto embedded_service = std::make_shared<Service>();
        embedded_service->initialize(config);
        return embedded_service;
    }

    Service::Service(): _is_started(false) {}

    void Service::initialize(const service::ServiceConfig &config) {
        _asio_context = infrastructure::AsioContext::Create(config.asio_context_config);
        _art_net = infrastructure::ArtNet::Create(config.art_net_config, _asio_context->GetContext());
        _graphics = infrastructure::Graphics::Create(config.graphics_config, shared_from_this());
    }

    void Service::Start() {
        if (_is_started) {
            return;
        }
        _asio_context->Start();
        _art_net->Start();
        _graphics->Start();
        _is_started = true;
    }

    void Service::Stop() {
        if (!_is_started) {
            return;
        }
        _art_net->Stop();
        _asio_context->Stop();
        _graphics->Stop();
        _is_started = false;
    }

    void Service::Unset() {
        _art_net.reset();
        _asio_context.reset();
    }

    void Service::PostGraphicsUpdate(utility::SizedBufferPtr &&pixels) {
        // _art_net->Post(std::move(pixels));
    }

}