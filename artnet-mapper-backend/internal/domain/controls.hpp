//
// Created by broosegoose on 8/27/23.
//

#ifndef DOMAIN_CONTROLS_HPP
#define DOMAIN_CONTROLS_HPP

#include <string>

namespace domain {

    enum class ControlsType {
        IONO,
        INPUT
    };

    static ControlsType ControlsTypeFromString(const std::string &control_type) {
        if (control_type == "IONO") {
            return ControlsType::IONO;
        } else if (control_type == "INPUT") {
            return ControlsType::INPUT;
        } else {
            throw std::domain_error("Unknown domain::ControlsType: " + control_type);
        }
    }


}

#endif //DOMAIN_CONTROLS_HPP
