//
// Created by broosegoose on 7/23/23.
//

#ifndef APPLICATION_CONFIG_HPP
#define APPLICATION_CONFIG_HPP

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <iostream>

#include "utility/files.hpp"

namespace application {

    nlohmann::json get_config(int argc, char * argv[]) {
        std::string project, configuration;

        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];

            if (arg == "-p" && i + 1 < argc) {
                project = argv[++i];
            } else if (arg == "-c" && i + 1 < argc) {
                configuration = argv[++i];
            } else {
                throw std::runtime_error("application::get_config - Unknown or incomplete argument: " + arg);
            }
        }

        if (project.empty()) {
            throw std::runtime_error("application::get_config - -p flag not set");
        }
        if (configuration.empty()) {
            std::cout << "application::get_config - -c not set, using default config" << std::endl;
            configuration = "default";
        }

        auto config_file = project + "." + configuration + ".json";
        std::filesystem::path config_dir = CONFIG_DIR;
        auto config_file_path = config_dir / config_file;
        if (utility::FileExists(config_file_path) != utility::Ternary::TRUE) {
            throw std::runtime_error("application::get_config - config file not found: " + config_file_path.string());
        }

        /* load json */
        std::ifstream config_file_content(config_file_path);
        nlohmann::json config;
        config_file_content >> config;

        return std::move(config);
    }
}

#endif //APPLICATION_CONFIG_HPP
