//
// Created by brucegoose on 7/5/23.
//

#ifndef ARTNETMAPPER_UNTYPED_BUFFER_HPP
#define ARTNETMAPPER_UNTYPED_BUFFER_HPP

namespace utility {
    struct UnsizedBuffer {
        [[nodiscard]] virtual void *GetMemory() = 0;
    };
}

#endif //ARTNETMAPPER_UNTYPED_BUFFER_HPP
