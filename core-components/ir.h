#ifndef IR_H
#define IR_H
#include <cstdint>
#include <string>
using namespace std;

uint8_t hexToUint(char obj){
    if ('0' <= obj && obj <= '9')
        obj -= '0';
    else
        obj += -'A' + 10;
    
    return obj;
}

class IR{

    private:
        string instruction = ""; // store 4 char, they are opcode, destination, operand 1 and operand 2 respectivly
        uint8_t decoded[4];

    public:
        void load(string i){
            instruction = i; // set instruction
            
            // convert hex values to int and store its ASCII in string (to reduce consuming of memory)

            for (int i = 0; i < 4; i++){
                decoded[i] = hexToUint(instruction[i]);
            }
        }

        const string& get_instruction(){
            return instruction;
        }

        const uint8_t& get_opcode(){
            return decoded[0];
        }

        const uint8_t& get_dest(){
            return decoded[1];
        }

        const uint8_t& get_operand1(){
            return decoded[2];
        }

        const uint8_t& get_operand2(){
            return decoded[3];
        }     

};

#endif