#include "simulatorui.h"


void SimulatorUI::main_loop(){
    show_menu();
    
    int user_choice;
    cin>>user_choice;

    switch(user_choice){
        case 1:
            handle_load_file();
            break;
        case 2:
            handle_overwrite();
            break;
    }

}




void SimulatorUI::show_menu(){

    cout<<"---------- Vole Machine ----------\n";
    cout<<"Please enter a number for its respective option\n";
    cout<<"Enter 1 to load program from a file\n";
    cout<<"Enter 2 to overwrite the content of a register or memory cell\n";
}






void SimulatorUI::handle_load_file(){
    string filename;
    cin>>filename;
    machine.load_program(filename);
}





void SimulatorUI::handle_overwrite(){
    char c;
    cout<<"Enter M to modify a memory cell\n";
    cout<<"Enter R if you want to modify a Register\n";
    cin>>c;


    uint8_t index , value;
    string value_hex;

    switch(toupper(c)){
        
        
        case 'M':
            cout<<"Enter the slot to modify (0-255)\n";
            cin>>index;
            cout<<"Enter the value in hexadecimal (00-FF)\n";
            cin>>value_hex;
            value = stoi(value_hex,nullptr,16);
            machine.overwrite_memory(index,value);

        case 'R':
            cout<<"Enter the slot to modify (0-15)\n";
            cin>>index;
            cout<<"Enter the value in hexadecimal (00-FF)\n";
            cin>>value_hex;
            value = stoi(value_hex,nullptr,16);
            machine.overwrite_register(index,value);
    }

}


