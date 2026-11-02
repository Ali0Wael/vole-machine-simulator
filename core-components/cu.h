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
    public:
        CU(Memory& memory , Register (&registers)[16] , ALU& alu , Register& pc , IR& ir , string& screen_buffer_string);
        void fetch();
        bool validate_instruction();
        void execute();
        void load();
        void load(uint8_t value);
        void load(float value);
        void load_immediate();
        void store();
        void move();
        void jump();
        void halt();
        void update_pc();
        void execute_step();
        
};




#endif