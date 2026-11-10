#include "volemachine.h"


using namespace std;



bool VoleMachine::is_valid_instruction(string instruction) {

    return ('1' <= instruction[0] && instruction[0] <= '9') || ('A' <= instruction[0] && instruction[0] <= 'C') && // range of op-code from 0 to C
           ('0' <= instruction[1] && instruction[1] <= '9') || ('A' <= instruction[1] && instruction[1] <= 'F') &&  // range of destination registers from 0 to F
           ('0' <= instruction[2] && instruction[2] <= '9') || ('A' <= instruction[2] && instruction[2] <= 'F') && // range of operand 1 which is from 0 to F
           ('0' <= instruction[3] && instruction[3] <= '9') || ('A' <= instruction[3] && instruction[3] <= 'F'); // reange of operand 2 which is from 0 to F

}



bool VoleMachine::is_even_memory_loc(uint8_t loc){
    return loc%2==0;
}


bool VoleMachine::load_program(string filename,uint8_t loc) {

    ifstream file(filename);
    if (!file.is_open()) { return false; }

    uint8_t c_address = loc;
    string name;
    vector <string> v_ins;
    int size_v;

    while (getline(file, name)) {

        if (name.empty()) { continue; } // if empty

        int start_idx = 0;
        while (start_idx < name.size() && (name[start_idx] == ' ' || name[start_idx] == '\t')) {
            start_idx++;
        } // all typs of spaces

        if (start_idx >= name.size() || name[start_idx] == ';') {
            continue;
        }

        for (int i = 0; i < name.size(); i++) {
            if (name[i] == ';') {
                name = name.substr(0, i);
                break;
            }
        }

            string new_copy = "";
            for (int y = 0; y < name.size(); y++) {
                if (name[y] != ' ' && name[y] != '\t' && name[y] != '\r' && name[y] != '\n') {
                    new_copy += name[y];
                }
            }

            if (new_copy != "") {
                v_ins.push_back(new_copy);
            }
    }


    bool valid = true;
    if(!is_even_memory_loc(c_address)) valid = false;
    for (int i = 0; i < v_ins.size(); i++) {
            string name = v_ins[i];
            if (!is_valid_instruction(name)) valid = false;
    }


    if(valid){
        
        pc.set_value(c_address);

        for (int i = 0; i < v_ins.size(); i++) {
            string name = v_ins[i];
        

            string ins1 = name.substr(0, 2);
            string ins2 = name.substr(2, 2);

            uint8_t instruction_1 = stoi(ins1, nullptr, 16);
            uint8_t instruction_2 = stoi(ins2, nullptr, 16);

            memory.write(c_address, instruction_1);
            c_address += 1;

            memory.write(c_address, instruction_2);
            c_address += 1;
            
        }
    }
    return valid;
}













void VoleMachine::overwrite_register(uint8_t index, uint8_t value) {
    if (index < 16)
        registers[index].set_value(value);
}
void VoleMachine::overwrite_memory(uint8_t index, uint8_t value) {
    if (index < 256){
        memory.write(index,value);
        
        if(index==255){
            cu.write_to_screen(value);
        }
    }

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
    while (!halted && !error) {
        cu.execute_step();
    }
};



void VoleMachine::reset() {
    memory.clear();
    
    pc.set_value(0);
    ir.load(0);

    for (uint8_t i = 0; i < 16; ++i) {
        registers[i].set_value(0);
    }
    
    screen_buffer_ = "";
    halted = false;
    error = false;
}



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