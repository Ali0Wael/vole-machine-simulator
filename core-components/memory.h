#pragma once

#include <cstdint>
#include <array>

class Memory {
    private:
        std::array <uint8_t, 256> cells_{0};
    public:
        uint8_t read(uint8_t address) {
            return cells_[address];
        }
        void write(uint8_t address, uint8_t value) {
            cells_[address] = value;
        }
        void clear() {
            cells_.fill(0);
        }
};
