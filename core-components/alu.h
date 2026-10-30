#ifndef ALU_H
#define ALU_H

#include <cstdint>


class ALU{
    public:
        uint8_t add(uint8_t a, uint8_t b){
            return a+b;
        }

        float add(float a, float b){
            return a+b;
        }

        uint8_t AND(uint8_t a, uint8_t b){
            return a & b;
        }

        uint8_t OR(uint8_t a, uint8_t b){
            return a | b;
        }

        uint8_t XOR(uint8_t a, uint8_t b){
            return a ^ b;
        }

        uint8_t rotate(uint8_t value, uint8_t count){
            return value >> count;
        }


};


#endif