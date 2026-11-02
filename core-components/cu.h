#ifndef CU_H
#define CU_H

#include "ir.h"
#include "alu.h"
#include "memory.h"
#include "register.h"

#include <cstdint>

class CU{
    private:
        Memory& memory;
        Register (&registers)[16];
        ALU& alu;
        Register& pc;
        IR& ir;
        string& screen_buffer_string;
        void fetch();
        bool validate_instruction();
        void execute();
        void load();
        void load_immediate();
        void store();
        void move();
        void jump();
        void halt();
        void update_pc();
    public:
        CU(Memory memory , Register registers[16] , ALU alu , Register pc , IR ir , string screen_buffer_string);
        void execute_step();
        
};




#endif