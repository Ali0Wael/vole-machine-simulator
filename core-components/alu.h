#ifndef ALU_H
#define ALU_H

#include <cstdint>
#include <bit>
using namespace std;

class ALU{
    private:
        float uint8_tToFloat(uint8_t n){
            uint32_t sign = (n >> 7) & 1;
            uint32_t exp = ((n >> 5) & 0x03) + 126;
            uint32_t mantissa = n & 0x1F;

            uint32_t i = sign << 31;
            i = i | (exp << 23);
            i = i | (mantissa << 18);

            
            return bit_cast<float>(i);
        }

        uint8_t floatToUint8_t(float f){
            uint32_t i = std::bit_cast<uint32_t>(f);
            uint8_t sign = i >> 31,
                    exp = ((i >> 23) & 0x000000FF) - 126,
                    mantissa = (i >> 18) & 0x1F;

            uint8_t u = sign << 7;
            u = u | ((exp & 0x03) << 5);
            u = u | mantissa;
            
            return u;
        }
        
    public:
        uint8_t add(uint8_t a, uint8_t b){
            int8_t i_a = bit_cast<int8_t>(a); // to store bit sequence in the another data type
            int8_t i_b = bit_cast<int8_t>(b);

            int8_t sum = i_a + i_b;
            
            return  bit_cast<uint8_t>(sum);
        }

        uint8_t fp_add(uint8_t a, uint8_t b){
            float f_a = uint8_tToFloat(a);
            float f_b = uint8_tToFloat(b);
            float sum = f_a + f_b;
            
            return uint8_tToFloat(sum);
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
            return (value >> count) | (value << (8 - count));
        }


};


#endif