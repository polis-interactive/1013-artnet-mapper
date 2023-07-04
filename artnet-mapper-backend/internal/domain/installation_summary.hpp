//
// Created by brucegoose on 7/5/23.
//

#ifndef DOMAIN_INSTALLATION_SUMMARY_HPP
#define DOMAIN_INSTALLATION_SUMMARY_HPP

namespace domain {

    enum class PixelType {
        RGB,
        RGBA
    };

    // should be directly created from a json object
    struct InstallationSummary {
        const unsigned int width;
        const unsigned int height;
        const PixelType pixel_type;
    };
}

#endif //ARTNETMAPPER_GRAPHICS_SUMMARY_HPP
