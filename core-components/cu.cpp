#include "cu.h"



CU::CU(Memory& memory , Register (&registers)[16] , ALU& alu , Register& pc , IR& ir , string& screen_buffer_string):
    memory(memory), registers(registers), alu(alu), pc(pc), ir(ir), screen_buffer_string(screen_buffer_string){}


void CU::update_pc(){
    pc.set_value(pc.get_value()+2);
}


void CU::fetch(){
    uint16_t first_part = memory.read(pc.get_value());
    uint8_t second_part = memory.read(pc.get_value()+1);
    first_part <<= 8;
    first_part += second_part;
    ir.load(first_part);
    update_pc();
}

bool CU::validate_instruction(){

    return (1 <= ir.get_opcode()) && (ir.get_opcode() <= 12) && // range of op-code
           (0 <= ir.get_dest()) && (ir.get_dest() <= 15) &&  // range of destination registers
           (0 <= ir.get_operand1()) && (ir.get_operand1() <= 255) && // range of operand 1 which is from 0 to 2^8 -1
           (0 <= ir.get_operand2()) && (ir.get_operand2() <= 255); // reange of operand 2 which is from 0 to 2^8 -1

}

void CU::execute(){
    switch(ir.get_opcode()){
        case 1:
            load();
            break;

        case 2:
            load_immediate();
            break;
        
        case 3:
            store();
            break;
        
        case 4:
            move();
            break;
        
        case 5:
        case 6:
            load(alu.add(ir.get_operand1(), ir.get_operand2()));
            break;

        case 7:
            load(alu.OR(ir.get_operand1(), ir.get_operand2()));
            break;

        case 8:
            load(alu.AND(ir.get_operand1(), ir.get_operand2()));
            break;

        case 9:
            load(alu.XOR(ir.get_operand1(), ir.get_operand2()));
            break;
        
        case 10:
            load(alu.rotate(ir.get_operand1(), ir.get_operand2()));
            break;
        
        case 11:
            jump();
            break;
        
        case 12:
            halt();
            break;
    }
}

uint8_t CU::calculate_memory_address(uint8_t op1 , uint8_t op2){    // get the index in memory
        return 16 * op1 + op2;
}

void CU::load(){
    uint8_t i = calculate_memory_address(ir.get_operand1(),ir.get_operand2());    // get the index in memory
    registers[ir.get_dest()].set_value(memory.read(i));
}

void CU::load(uint8_t value){
    registers[ir.get_dest()].set_value(value);
}
void CU::load(float value){
    registers[ir.get_dest()].set_value(value);
}

void CU::load_immediate(){
    uint8_t i = calculate_memory_address(ir.get_operand1(),ir.get_operand2()); 
    registers[ir.get_dest()].set_value(i);
}

void CU::store(){
    uint8_t i = calculate_memory_address(ir.get_operand1(),ir.get_operand2()); 
    memory.write(i, registers[ir.get_dest()].get_value());
}


void CU::move(){
    uint8_t value = registers[ir.get_operand1()].get_value();
    registers[ir.get_operand2()].set_value(value);
}

void CU::jump(){
    if(registers[ir.get_dest()].get_value() == registers[0].get_value()){
        uint8_t i = calculate_memory_address(ir.get_operand1(),ir.get_operand2());
        pc.set_value(i);
    }
}


void CU::halt(){

}