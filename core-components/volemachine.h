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
        Register pc;
        IR ir;
        bool halted;
        bool error;
        string screen_buffer_;

       
    public:
        VoleMachine(): cu(memory,registers,alu,pc,ir,screen_buffer_,halted,error), halted(false),error(false){}
        void load_program(string filename);
        void overwrite_register(uint8_t index, uint8_t value);
        void overwrite_memory(uint8_t index, uint8_t value);
        uint8_t get_memory(uint8_t address);
        uint8_t get_register(uint8_t index);
        uint8_t get_pc();
        uint16_t get_ir();
        void step();
        void run_all();
        string get_screen_buffer();
        bool get_halted();
        bool get_error();
};