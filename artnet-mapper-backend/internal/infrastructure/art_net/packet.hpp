//
// Created by broosegoose on 7/18/23.
//

#ifndef INFRASTRUCTURE_ARTNET_PACKET_HPP
#define INFRASTRUCTURE_ARTNET_PACKET_HPP

#include <cstring>

#include "utility/buffers.hpp"

#define ARTNET_PORT 6454

namespace infrastructure {
    /* I hearby declare that I will eventually implement the full artnet standard */

    class ArtNedDmxPacket: public utility::SizedBuffer {
    public:
        ArtNedDmxPacket(unsigned int universe, unsigned int length):
            _universe_high((universe >> 8) & 0xFF),
            _universe_low(universe & 0xFF),
            _length_high((length >> 8) & 0xFF),
            _length_low(length & 0xFF),
            _length(length),
            _data(std::vector<uint8_t>(length + 18))
        {
            std::memcpy(_data.data(), &_header, 18);
        }
        void UpdateSequence(const uint8_t sequence) {
            _data.at(13) = sequence;
        }
        // MAN THIS IS UNSAFE :D
        void CopyBody(uint8_t *body) {
            std::memcpy(_data.data() + 18, body, _length);
        }
        void GenerateBody(unsigned int universe) {
            uint8_t color[3] = { 0, 0, 0 };
            switch (universe) {
                case 0:
                    color[0] = 30;
                    break;
                case 1:
                    color[1] = 30;
                    break;
                case 2:
                    color[2] = 30;
                    break;
                case 3:
                    color[0] = 255;
                    break;
                case 4:
                    color[1] = 255;
                    break;
                case 5:
                    color[2] = 255;
                    break;
                case 6:
                    color[0] = 255;
                    color[1] = 127;
                    break;
                case 7:
                    color[0] = 255;
                    color[2] = 127;
                    break;
                case 8:
                    color[1] = 255;
                    color[2] = 127;
                    break;
                case 9:
                    color[1] = 255;
                    color[0] = 127;
                    break;
                case 10:
                    color[2] = 255;
                    color[0] = 127;
                    break;
                case 11:
                    color[2] = 255;
                    color[1] = 127;
                    break;
                case 12:
                default:
                    color[0] = 127;
                    color[1] = 127;
                    color[2] = 127;
                    break;
            }
            const unsigned int times = (_length / 3.0);
            for (int i = 0; i < times; i++) {
                const auto base = i * 3 + 18;
                _data[base] = color[0];
                _data[base + 1] = color[1];
                _data[base + 2] = color[2];
            }
        }
        [[nodiscard]] uint8_t *GetMemory() final {
            return _data.data();
        }
        [[nodiscard]] std::size_t GetSize() final {
            return _data.size();
        };
    private:
        union {
            struct {
                uint8_t _id[8] = "Art-Net";
                uint16_t _op_code = 0x5000;
                uint16_t _version = 0x0e00;
                uint8_t _sequence = 0;
                uint8_t _physical = 0;
                uint8_t _universe_low;
                uint8_t _universe_high;
                uint8_t _length_high;
                uint8_t _length_low;
            };
            uint8_t _header[18];
        };
        const unsigned int _length;
        std::vector<uint8_t> _data;
    };
}

#endif //INFRASTRUCTURE_ARTNET_PACKET_HPP
