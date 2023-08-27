//
// Created by broosegoose on 8/27/23.
//

#ifndef INFRASTRUCTURE_CONTROLS_HPP
#define INFRASTRUCTURE_CONTROLS_HPP

#include <thread>

#include <nlohmann/json.hpp>

#include "domain/controls.hpp"

namespace infrastructure {

    struct ControlsManager {
        virtual void PostPotentiometerUpdate(const float new_pot_read) = 0;
    };
    typedef std::shared_ptr<ControlsManager> ControlsManagerPtr;

    struct ControlsConfig {
        unsigned int controls_analog_min;
        unsigned int controls_analog_max;
        unsigned int controls_analog_resolution;
        unsigned int controls_analog_hysteresis_divider;
        unsigned int controls_analog_polling_hz;
        bool controls_print_values;
        domain::ControlsType controls_type;

        static ControlsConfig from_json(const nlohmann::json& j) {
            ControlsConfig conf{};
            conf.controls_analog_min = j.at("controls_analog_min").get<unsigned int>();
            conf.controls_analog_max = j.at("controls_analog_max").get<unsigned int>();
            conf.controls_analog_resolution = j.at("controls_analog_resolution").get<unsigned int>();
            conf.controls_analog_hysteresis_divider = j.at("controls_analog_hysteresis_divider").get<unsigned int>();
            conf.controls_analog_polling_hz = j.at("controls_analog_polling_hz").get<unsigned int>();
            conf.controls_print_values = j.at("controls_print_values").get<bool>();
            conf.controls_type = domain::ControlsTypeFromString(j.at("controls_type").get<std::string>());
            return conf;
        }
    };



    class Controls;
    typedef std::shared_ptr<Controls> ControlsPtr;

    class Controls {
    public:
        static ControlsPtr Create(const ControlsConfig &config, ControlsManagerPtr manager);
        Controls(const ControlsConfig &config, ControlsManagerPtr manager);
        void Start();
        void Stop();
        ~Controls();
        Controls() = delete;
        Controls (const Controls&) = delete;
        Controls& operator= (const Controls&) = delete;
    protected:
        virtual void doStart() = 0;
        virtual void run(std::stop_token st) = 0;
        std::unique_ptr<std::jthread> _polling_thread = { nullptr };

        float getAnalogFloatFromRead(unsigned int raw_read);

        const unsigned int _analog_min;
        const unsigned int _analog_max;
        const float _analog_resolution;
        const float _analog_step_size;
        const unsigned int _analog_hysteresis;

        ControlsManagerPtr _manager;
    };

}

#endif //INFRASTRUCTURE_CONTROLS_HPP
