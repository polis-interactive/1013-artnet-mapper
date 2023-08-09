//
// Created by broosegoose on 7/22/23.
//

#ifndef DOMAIN_DISPLAY_HPP
#define DOMAIN_DISPLAY_HPP

#include <nlohmann/json.hpp>
#include <filesystem>

#include "color.hpp"

namespace domain {

    enum class RendererType {
        GLFW,
        HEADLESS
    };

    struct Display {
        RendererType render_type;
        std::string shader;
        std::string pixel_texture;
        std::string artnet_texture;
        unsigned int buffer_count;
        unsigned int pixel_multiplier = 1;

        [[nodiscard]] std::string RendererTypeToString() const {
            if (render_type == RendererType::GLFW) {
                return "GLFW";
            } else if (render_type == RendererType::HEADLESS) {
                return "HEADLESS";
            }
        }

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["render_type"] = RendererTypeToString();
            j["shader"] = shader;
            j["pixel_texture"] = pixel_texture;
            j["artnet_texture"] = artnet_texture;
            j["buffer_count"] = buffer_count;
            j["pixel_multiplier"] = pixel_multiplier;
            return j;
        }

        static RendererType RendererTypeFromString(const std::string &render_type) {
            if (render_type == "GLFW") {
                return RendererType::GLFW;
            } else if (render_type == "HEADLESS") {
                return RendererType::HEADLESS;
            } else {
                throw std::domain_error("Unknown domain::RenderType: " + render_type);
            }
        }

        static Display from_json(const nlohmann::json& j) {
            Display d;
            d.render_type = RendererTypeFromString(j.at("render_type").get<std::string>());
            d.shader = j.at("shader").get<std::string>();
            d.pixel_texture = j.at("pixel_texture").get<std::string>();
            d.artnet_texture = j.at("artnet_texture").get<std::string>();
            d.buffer_count = j.at("buffer_count").get<unsigned int>();
            d.pixel_multiplier = j.at("pixel_multiplier").get<unsigned int>();
            return d;
        }
    };

}

#endif //DOMAIN_DISPLAY_HPP
