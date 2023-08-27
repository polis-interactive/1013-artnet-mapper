//
// Created by broosegoose on 8/27/23.
//

#ifndef INFRASTRUCTURE_CONTROLS_INPUT_HPP
#define INFRASTRUCTURE_CONTROLS_INPUT_HPP

#include "controls.hpp"

namespace infrastructure {

    class InputControls: public Controls, public std::enable_shared_from_this<InputControls> {
    public:
        InputControls(const ControlsConfig &config, ControlsManagerPtr manager);
        InputControls() = delete;
        InputControls (const InputControls&) = delete;
        InputControls& operator= (const InputControls&) = delete;
    private:
        void doStart() final;
        void run(std::stop_token st) final;
    };

}

#endif //INFRASTRUCTURE_CONTROLS_INPUT_HPP
