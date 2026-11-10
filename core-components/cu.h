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
        bool& halted;
        bool& error;
        
        void load();
        void load(uint8_t value);
        void load_immediate();
        void store();
        void move();
        void jump();
        void halt();
        void error_exist();
        uint8_t calculate_memory_address();
        void update_pc();
        void fetch();
        bool validate_instruction();
        void execute();
    public:
        CU(Memory& memory , Register (&registers)[16] , ALU& alu , Register& pc , IR& ir , string& screen_buffer_string, bool& halted , bool& error);
        void execute_step(); 
        void write_to_screen(uint8_t value);     
};



#endif