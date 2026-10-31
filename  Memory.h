#pragma once

#include <cstdint>
#include <array>

class Memory {
    std::array <uint8_t, 256> cells;
    uint8_t read(uint8_t address) {
        return cells[address];
    }
    void write(uint8_t address, uint8_t value) {
        cells[address] = value;
    }
    const std::array <uint8_t, 256>& get_cells() const {
        return cells;
    }
};
