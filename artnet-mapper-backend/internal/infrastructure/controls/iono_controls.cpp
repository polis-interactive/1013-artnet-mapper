//
// Created by broosegoose on 8/27/23.
//

#include "iono_controls.hpp"

#include <iostream>

namespace infrastructure {

    IonoControls::IonoControls(
            const infrastructure::ControlsConfig &config,
            infrastructure::ControlsManagerPtr manager
    ):
        Controls(config, std::move(manager)),
        _polling_timeout(1 / config.controls_analog_polling_hz),
        _print_values(config.controls_print_values)
    {}

    void IonoControls::doStart() {
        _polling_thread = std::make_unique<std::jthread>(std::bind_front(&IonoControls::run, shared_from_this()));
    }

    void IonoControls::run(std::stop_token st) {
        setLed(true);
        while (!st.stop_requested()) {
            std::this_thread::sleep_for(_polling_timeout);
            if (!analogFileReady()) {
                _manager->PostPotentiometerUpdate(1.0);
                continue;
            }
            const auto current_analog_read = readAnalog();
            const auto analog_change = abs(static_cast<int>(current_analog_read) - static_cast<int>(last_analog_read));
            if (_print_values) {
                std::cout << "IonoControls::run: Last read: " << last_analog_read << std::endl;
                std::cout << "IonoControls::run: Current read: " << current_analog_read << std::endl;
                std::cout << "IonoControls::run: Current change: " << analog_change << std::endl;
                std::cout << "IonoControls::run: Hysteresis: " << _analog_hysteresis << std::endl;
            }
            if (analog_change < _analog_hysteresis) {
                continue;
            }
            last_analog_read = current_analog_read;
            const auto output_value = getAnalogFloatFromRead(current_analog_read);
            if (_print_values) {
                std::cout << "IonoControls::run: Output value: " << output_value << std::endl;
            }
            _manager->PostPotentiometerUpdate(output_value);
        }
        setLed(false);
    }

    bool IonoControls::analogFileReady() {
        if (analogFile.is_open()) {
            return true;
        }
        analogFile.open(ANALOG_PATH);
        const auto is_ready = analogFile.is_open();
        if (!is_ready) {
            std::cerr << "IonoControls::setLed Error opening " << ANALOG_PATH << std::endl;
        }
        return is_ready;
    }

    void IonoControls::setLed(const bool led_on) {
        digitalFile.open(OPEN_COLL_PATH);
        if (!digitalFile.is_open()) {
            std::cerr << "IonoControls::setLed Error opening " << OPEN_COLL_PATH << std::endl;
            return;
        }
        if (led_on) {
            digitalFile << "1";
        } else {
            digitalFile << "0";
        }
        digitalFile.flush();
        digitalFile.close();
    }

    unsigned int IonoControls::readAnalog() {
        unsigned int value;
        analogFile.seekg(0);
        analogFile >> value;
        return value;
    }

}