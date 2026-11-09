#include "cu.h"

#include <sstream>
#include <iomanip>


CU::CU(Memory& memory , Register (&registers)[16] , ALU& alu , Register& pc , IR& ir , string& screen_buffer_string, bool& halted, bool& error):
    memory(memory), registers(registers), alu(alu), pc(pc), ir(ir), screen_buffer_string(screen_buffer_string), halted(halted) , error(error){}


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
    uint8_t i1, i2;
    if (5 <= ir.get_opcode() && ir.get_opcode() <= 10){
        uint8_t i1 = ir.get_operand1();
        uint8_t i2 = ir.get_operand1();
    }

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
            alu.add(registers[i1].get_value(), registers[i2].get_value());
            break;
        case 6:
            load(alu.add(registers[i1].get_value(), registers[i2].get_value()));
            break;

        case 7:
            load(alu.OR(registers[i1].get_value(), registers[i2].get_value()));
            break;

        case 8:
            load(alu.AND(registers[i1].get_value(), registers[i2].get_value()));
            break;

        case 9:
            load(alu.XOR(registers[i1].get_value(), registers[i2].get_value()));
            break;
        
        case 10:
            load(alu.rotate(registers[i1].get_value(), registers[i2].get_value()));
            break;
        
        case 11:
            jump();
            break;
        
        case 12:
            halt();
            break;
    }
}

void CU::execute_step(){
    fetch();
    if(validate_instruction()){
        execute();
    }
    else{
        error_exist();
    }
}


uint8_t CU::calculate_memory_address(){    // get the index in memory
        return 16 * ir.get_operand1() + ir.get_operand2();
}

void CU::load(){
    uint8_t i = calculate_memory_address();    // get the index in memory
    registers[ir.get_dest()].set_value(memory.read(i));
}

void CU::load(uint8_t value){
    registers[ir.get_dest()].set_value(value);
}
void CU::load(float value){
    registers[ir.get_dest()].set_value(value);
}

void CU::load_immediate(){
    uint8_t i = calculate_memory_address(); 
    registers[ir.get_dest()].set_value(i);
}

void CU::store(){
    uint8_t i = calculate_memory_address(); 
    uint8_t value = registers[ir.get_dest()].get_value();
    memory.write(i, value);

    if(i==255){
        stringstream ss;
        ss << hex << uppercase << setw(2) << setfill('0') << static_cast<int> (value);
        screen_buffer_string+=ss.str();
    }
}


void CU::move(){
    uint8_t value = registers[ir.get_operand1()].get_value();
    registers[ir.get_operand2()].set_value(value);
}

void CU::jump(){
    if(registers[ir.get_dest()].get_value() == registers[0].get_value()){
        uint8_t i = calculate_memory_address();
        pc.set_value(i);
    }
}


void CU::halt(){
    halted = true;
}

void CU::error_exist(){
    error = true;
}