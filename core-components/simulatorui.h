#include "volemachine.h"


class SimulatorUI{
    private:
        VoleMachine machine;
    public:
        void main_loop();
        void show_menu();
        void handle_load_file();
        void handle_overwrite();
        void handle_run();
        void display_state();
        void wait_for_enter();
};