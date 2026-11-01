#include <iostream>
#include <cstdint>

using namespace std;

class Register {

private:
	uint8_t value;
	
public:

	Register(uint8_t val = 0)
	{
			value = val;
	}



	void set_value(uint8_t val) {
		value = val;
	}
	uint8_t get_value(){ return value; }


};