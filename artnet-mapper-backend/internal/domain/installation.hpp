//
// Created by broosegoose on 7/18/23.
//

#ifndef DOMAIN_INSTALLATION_HPP
#define DOMAIN_INSTALLATION_HPP

#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "color.hpp"

namespace domain {

    struct Universe {
        unsigned int pixel_count;
        unsigned int pixel_width;
        unsigned int start_index;

        [[nodiscard]] inline unsigned int GetLastLength() const {
            return start_index + pixel_width * pixel_count;
        }

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["pixel_count"] = pixel_count;
            j["pixel_width"] = pixel_width;
            j["start_index"] = start_index;
            return j;
        }

        static Universe from_json(const nlohmann::json& j) {
            Universe universe = { 0 };
            universe.pixel_count = j.at("pixel_count");
            universe.pixel_width = j.at("pixel_width");
            universe.start_index = j.at("start_index");
            return universe;
        }
    };

    typedef std::map<unsigned int, Universe> UniverseMap;

    struct Dimensions {
        unsigned int width;
        unsigned int height;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["width"] = width;
            j["height"] = height;
            return j;
        }

        static Dimensions from_json(const nlohmann::json& j) {
            Dimensions dimensions = { 0 };
            dimensions.width = j.at("width");
            dimensions.height = j.at("height");
            return dimensions;
        }
    };

    namespace installation {

        struct Layout {
            UniverseMap universes;
            std::map<std::string, std::vector<unsigned int>> controllers;

            [[nodiscard]] nlohmann::json to_json() const {
                nlohmann::json j;
                j["controllers"] = controllers;
                for (const auto &[key, value]: universes) {
                    j["universes"][std::to_string(key)] = value.to_json();
                }
                return j;
            }

            static Layout from_json(const nlohmann::json& j) {
                Layout l;
                l.controllers = j.at("controllers").get<std::map<std::string, std::vector<unsigned int>>>();
                for (const auto& [key, value]: j["universes"].items()) {
                    l.universes[std::stoi(key)] = Universe::from_json(value);
                }
                return l;
            }

        };

        struct Config {

            unsigned int buffer_count;

            /* these should really be at the universe level / pixel level but w.e */
            bool rgbw_pixels = false;
            unsigned int pixel_types = 1;

            Dimensions dimensions;

            double fps = 30.0;
            double threshold_fps = 24.0;
            double threshold_dropped_frames = 0.1;
            unsigned int rolling_frames = 100;

            std::optional<CRGB> color_correction;
            std::optional<float> gamma;

            // only relevant to rgbw pixels
            std::optional<unsigned int> color_temperature;
            std::optional<CRGB> white_color;


            [[nodiscard]] nlohmann::json to_json() const {
                nlohmann::json j;
                j["buffer_count"] = buffer_count;
                j["rgbw_pixels"] = rgbw_pixels;
                j["pixel_types"] = pixel_types;
                j["dimensions"] = dimensions.to_json();
                j["fps"] = fps;
                j["threshold_fps"] = threshold_fps;
                j["threshold_dropped_frames"] = threshold_dropped_frames;
                j["rolling_frames"] = rolling_frames;
                if (color_correction.has_value()) {
                    j["color_correction"] = color_correction.value().to_json();
                }
                if (gamma.has_value()) {
                    j["gamma"] = gamma.value();
                }
                if (color_temperature.has_value()) {
                    j["color_temperature"] = color_temperature.value();
                }
                if (white_color.has_value()) {
                    j["white_color"] = white_color.value().to_json();
                }
                return j;
            }

            static Config from_json(const nlohmann::json& j) {
                Config c;
                c.buffer_count = j.at("buffer_count").get<unsigned int>();
                c.rgbw_pixels = j.at("rgbw_pixels").get<bool>();
                c.pixel_types = j.at("pixel_types").get<unsigned int>();
                c.dimensions = { Dimensions::from_json(j.at("dimensions")) };
                c.fps = j.at("fps").get<double>();
                c.threshold_fps = j.at("threshold_fps").get<double>();
                c.threshold_dropped_frames = j.at("threshold_dropped_frames").get<double>();
                c.rolling_frames = j.at("rolling_frames").get<unsigned int>();
                if (j.contains("color_correction")) {
                    c.color_correction = { CRGB::from_json(j.at("color_correction")) };
                }
                if (j.contains("gamma")) {
                    c.gamma = { j.at("gamma").get<float>() };
                }
                if (j.contains("color_temperature")) {
                    c.color_temperature = { j.at("color_temperature").get<unsigned int>() };
                }
                if (j.contains("white_color")) {
                    c.white_color = { CRGB::from_json(j.at("white_color")) };
                }
                return c;
            }
        };

    }

}

#endif //DOMAIN_INSTALLATION_HPP
