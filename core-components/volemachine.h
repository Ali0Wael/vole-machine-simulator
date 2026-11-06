#pragma once

#include "memory.h"
#include "alu.h"
#include "cu.h"
#include"register.h"
#include "ir.h"

#include <cstdint>
#include <array>
#include <string>

class VoleMachine {

private:

    Register registers[16];
    Memory memory;
    ALU alu;
    CU cu;
    uint8_t pc = 0x00;
    IR ir;
    bool halted;
    string screen_buffer_;

public:

    void LoadProgram();
    void OverwriteRegister(uint8_t index, uint8_t value);
    void Step();
    void RunAll();
    string get_screen_buffer();
    bool get_halted();
};