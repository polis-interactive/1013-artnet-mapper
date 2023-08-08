
#include "config.hpp"
#include "runtime.hpp"

#include "utility/clock.hpp"

#include "service/service.hpp"

int main(int argc, char* argv[]) {

    application::RemoveSuccessFile();
    auto json_config = application::get_config(application::AppType::APPLICATION, argc, argv);
    auto service_config = service::ServiceConfig{
        .art_net_config = infrastructure::ArtNetConfig::from_json(json_config),
        .asio_context_config = infrastructure::AsioContextConfig::from_json(json_config)
    };
    auto service = service::Service::Create(service_config);
    service->Start();
    application::WaitForShutdown();
    service->Stop();
    std::this_thread::sleep_for(250ms);
    service->Unset();
    std::this_thread::sleep_for(250ms);
    application::CreateSuccessFile();
    return 0;
}
