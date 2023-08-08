//
// Created by broosegoose on 7/23/23.
//

#ifndef APPLICATION_CONFIG_HPP
#define APPLICATION_CONFIG_HPP

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace application {

    enum class AppType {
        APPLICATION
    };

    nlohmann::json get_config(const AppType &app_type, int argc, char * argv[]) {
        std::string json_name = "";
        switch (app_type) {
            case AppType::APPLICATION:
                json_name = "application.";
                break;
        }
        if (argc > 1) {
            json_name += argv[1];
        } else {
            json_name += "default";
        }
        json_name += ".json";
        std::filesystem::path root_dir = ROOT_DIR;
        auto conf_dir = root_dir / "config";
        auto json_file_path = conf_dir / json_name;
        if (!std::filesystem::exists(json_file_path)) {
            throw std::runtime_error("Couldn't find json file at path: " + json_file_path.string());
        }

        /* load json */
        std::ifstream config_file(json_file_path);
        nlohmann::json config;
        config_file >> config;

        return std::move(config);
    }
}

#endif //APPLICATION_CONFIG_HPP
