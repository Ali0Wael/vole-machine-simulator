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

        bool get_halted();
        bool get_error();

    public:
        VoleMachine(): cu(memory,registers,alu,pc,ir,screen_buffer_,halted,error), halted(false),error(false){}
        void load_program(string filename);
        void overwrite_register(uint8_t index, uint8_t value);
        void step();
        void run_all();
        string get_screen_buffer();
    

};