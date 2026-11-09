#include "volemachine.h"
#include<string>
#include <iostream>
#include<fstream>

using namespace std;

void VoleMachine::load_program(string filename) {

    ifstream file(filename);
    if (!file.is_open()) { return; }

    uint8_t c_address = 0x00;
    string name;

    while (getline(file, name)) {

        string ins1 = name.substr(0, 2);
        string ins2 = name.substr(2, 2);
        
        uint8_t instruction =stoi(ins1, nullptr, 16);
        memory.write(c_address, instruction);
        c_address += 1;

        instruction = stoi(ins2, nullptr, 16);
        memory.write(c_address, instruction);
        c_address += 1;
    }
}

//string ins = to_string(memory.read(pc)) + to_string(memory.read(pc+1));
//uint8_t instruction = stoi(ins, nullptr, 16);





void VoleMachine::overwrite_register(uint8_t index, uint8_t value) {
    if (index < 16)
        registers[index].set_value(value);
}
void VoleMachine::overwrite_memory(uint8_t index, uint8_t value) {
    if (index < 255)
        memory.write(index,value);
}


uint8_t VoleMachine::get_memory(uint8_t address){
    return memory.read(address);
}
uint8_t VoleMachine::get_register(uint8_t index){
    return registers[index].get_value();
}
uint8_t VoleMachine::get_pc(){
    return pc.get_value();
}
uint16_t VoleMachine::get_ir(){
    return ir.get_instruction();
}





void VoleMachine::step(){
    if(!halted && !error) cu.execute_step();
}

void VoleMachine::run_all() {
    while (halted && !error) {
        cu.execute_step();
    }
};




bool VoleMachine::get_halted() {
    return halted;
}
bool VoleMachine::get_error(){
    return error;
}
string VoleMachine::get_screen_buffer() {
    return screen_buffer_;
}




/*
void VoleMachine::Step() {

    if (get_halted()) { return; }
// The Fetch case  (counter -> pc)  (copy ins to ir)
    
    uint8_t left = memory.read(pc);
    uint8_t right = memory.read(pc + 1);

    uint16_t ins = (static_cast<uint16_t>(left) << 8) | right;
    
    ir.load(ins);



 // The Decode (Is valid or not) (The OP & Operand code)
    if (!cu.validate_instruction()) { return; }
    uint8_t op_c = ir.get_opcode();
    uint8_t r = ir.get_dest();
    uint8_t s = ir.get_operand1();
    uint8_t t = ir.get_operand2();


 // Execute program

    bool jump = false;

    switch (op_c) {
        
    case 1: {
        uint8_t exe = (static_cast<uint8_t>(s) << 4) | t;
        registers[r].set_value(memory.read(exe));
        break;
    }

    case 2: {
        uint8_t exe = (static_cast<uint8_t>(s) << 4) | t;
        registers[r].set_value(exe);
        break;
    }

    case 3: {
        uint8_t exe = (static_cast<uint8_t>(s) << 4) | t;
        memory.write(exe, registers[r].get_value());
        if (exe == 0xff) { memory.write(0xff, registers[r].get_value()); }

        break;
    }

    case 4: {
        registers[t].set_value(registers[s].get_value());
        break;
    }

    case 5: {
        registers[r].set_value(alu.add(registers[s].get_value(), registers[t].get_value()));
        break;
    }

    case 6: {
        registers[r].set_value(alu.add(registers[s].get_value(), registers[t].get_value()));
        break;
    }

    case 7: {
        registers[r].set_value(alu.OR(registers[s].get_value(), registers[t].get_value()));
        break;
    }

    case 8: {
        registers[r].set_value(alu.AND(registers[s].get_value(), registers[t].get_value()));
        break;
    }

    case 9: {
        registers[r].set_value(alu.XOR(registers[s].get_value(), registers[t].get_value()));
        break;
    }

    case 'A': {
        registers[r].set_value(alu.rotate(registers[r].get_value(), t));
        break;
    }

    case 'B': {
        if (registers[r].get_value() == registers[0].get_value()) {
            uint8_t exe = (static_cast<uint8_t>(s) << 4) | t;
            pc = exe;
            jump = true;
        }
            break;
    }

    case 'C': {
        halted = true;
        break;
    }
        
}
    if (!jump) {
        pc += 2;
    }
};
*/