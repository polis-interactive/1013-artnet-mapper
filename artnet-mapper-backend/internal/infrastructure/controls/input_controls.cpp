//
// Created by broosegoose on 8/27/23.
//

#include <iostream>
#include <unistd.h>

#include "input_controls.hpp"



namespace infrastructure {

    InputControls::InputControls(
        const infrastructure::ControlsConfig &config,
        infrastructure::ControlsManagerPtr manager
    ):
        Controls(config, std::move(manager))
    {}

    void InputControls::doStart() {
        _polling_thread = std::make_unique<std::jthread>(std::bind_front(&InputControls::run, shared_from_this()));
    }

    void InputControls::run(std::stop_token st) {
        while(!st.stop_requested()) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);

            struct timeval timeout;
            timeout.tv_sec = 1;  // Wait up to 1 second
            timeout.tv_usec = 0;

            int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

            if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
                unsigned int value;
                std::cin >> value;

                if (std::cin.fail()) {
                    std::cin.clear(); // Clear the error flags
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
                    std::cout << "Invalid input. Please enter a number between " << _analog_min <<
                        " to " << _analog_max << std::endl;
                } else if (_analog_min <= value && value <= _analog_max) {
                    _manager->PostPotentiometerUpdate(getAnalogFloatFromRead(value));
                } else {
                    std::cout << "Invalid input. Please enter a number between " << _analog_min <<
                              " to " << _analog_max << std::endl;
                }
            } else if (ret == 0) {
                // Timeout, no input available
                continue;
            } else {
                // Error occurred
                break;
            }
        }
    }

}