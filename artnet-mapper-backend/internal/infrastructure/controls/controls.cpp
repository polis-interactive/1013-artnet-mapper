//
// Created by broosegoose on 8/27/23.
//

#include "controls.hpp"

#include "iono_controls.hpp"
#include "input_controls.hpp"

namespace infrastructure {
    ControlsPtr Controls::Create(const ControlsConfig &config, ControlsManagerPtr manager) {
        switch (config.controls_type) {
            case domain::ControlsType::IONO:
                return std::make_unique<IonoControls>(config, std::move(manager));
            case domain::ControlsType::INPUT:
                return std::make_unique<InputControls>(config, std::move(manager));
            default:
                throw std::runtime_error("Selected controls unavailable... ");

        }
    }

    Controls::Controls(const infrastructure::ControlsConfig &config, ControlsManagerPtr manager):
        _analog_min(config.controls_analog_min),
        _analog_max(config.controls_analog_max),
        _analog_resolution(static_cast<float>(config.controls_analog_resolution)),
        _analog_step_size(static_cast<float>(_analog_max - _analog_min) / _analog_resolution),
        _analog_hysteresis(_analog_step_size / static_cast<float>(config.controls_analog_hysteresis_divider)),
        _manager(std::move(manager))
    {}

    void Controls::Start() {
        if (_polling_thread != nullptr) {
            return;
        }
        doStart();
    }

    void Controls::Stop() {
        if (_polling_thread == nullptr) {
            return;
        }
        if (_polling_thread->joinable()) {
            _polling_thread->request_stop();
            _polling_thread->join();
        }
        _polling_thread = nullptr;
    }

    Controls::~Controls() {
        Stop();
    }

    float Controls::getAnalogFloatFromRead(unsigned int raw_read) {
        if (raw_read <= _analog_min) return 0.0;
        if (raw_read >= _analog_max) return 1.0;

        const float scaled = static_cast<float>(raw_read - _analog_min) / _analog_step_size;
        // floor
        unsigned int subdivision = static_cast<unsigned int>(scaled);
        // put it back to a float
        const float result = static_cast<float>(subdivision) / _analog_resolution;

        return (result <= 0.0) ? 0.0 : (result >= 1.0) ? 1.0 : result;
    }
}