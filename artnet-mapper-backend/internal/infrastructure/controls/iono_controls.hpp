//
// Created by broosegoose on 8/27/23.
//

#ifndef INFRASTRUCTURE_CONTROLS_IONO_HPP
#define INFRASTRUCTURE_CONTROLS_IONO_HPP

#include "controls.hpp"

#include <fstream>

namespace infrastructure {

    class IonoControls: public Controls, public std::enable_shared_from_this<IonoControls> {
    public:
        IonoControls(const ControlsConfig &config, ControlsManagerPtr manager);
        IonoControls() = delete;
        IonoControls (const IonoControls&) = delete;
        IonoControls& operator= (const IonoControls&) = delete;
    protected:
        void doStart() final;
        void run(std::stop_token st) final;
    private:
        bool analogFileReady();
        unsigned int readAnalog();
        void setLed(const bool led_on);

        const bool _print_values;

        std::ifstream analogFile;
        const std::string ANALOG_PATH = "/sys/class/ionopi/analog_in/ai1_mv";
        std::ofstream digitalFile;
        const std::string OPEN_COLL_PATH = "/sys/class/ionopi/open_coll/oc1";

        const std::chrono::duration<double> _polling_timeout;
        unsigned int last_analog_read = 0.0;
    };
}

#endif //INFRASTRUCTURE_CONTROLS_IONO_HPP
