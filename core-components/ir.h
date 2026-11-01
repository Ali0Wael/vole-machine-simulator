#ifndef IR_H
#define IR_H
#include <cstdint>
#include <string>
using namespace std;

uint8_t hexToUint(uint8_t obj){
    uint8_t temp = obj & 0b00001111;   
    return temp;
}




class IR{

    private:
        uint16_t instruction; // store 4 char, they are opcode, destination, operand 1 and operand 2 respectivly
        uint8_t decoded[4];

    public:
        void load(uint16_t ins){
            instruction = ins; // set instruction
            
            // convert hex values to int and store its ASCII in string (to reduce consuming of memory)

            for (int i = 0; i < 4; i++){
                decoded[i] = (instruction >> i * 4) & 0b00001111;
            }
        }

        const uint16_t& get_instruction(){
            return instruction;
        }

        const uint8_t& get_opcode(){
            return decoded[3];
        }

        const uint8_t& get_dest(){
            return decoded[2];
        }
        
        const uint8_t& get_operand1(){
            return decoded[1];
        }

        const uint8_t& get_operand2(){
            return decoded[0];
        }     

};

#endif