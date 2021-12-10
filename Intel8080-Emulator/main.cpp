 //Intel 8080 CPU emulator written in C++.
 //Nate Hargrove
 //12/1/21

    #include <iostream>
    #include <fstream>
    #include "CPU.h"
    #include <vector>
    
  // ============
    int main() {
    
        CPU cpu;

         //Open binary file, read each byte and store each byte in a vector

        std::fstream file("invaders.e", std::ios::in | std::ios::out | std::ios::binary);

        file.seekg(0);

        std::vector<unsigned short> program;

        while (!file.eof()) {

            unsigned char x;

            file.read(reinterpret_cast<char*>(&x), 1);

            program.push_back(x);

        }

        // Load program into memory

        cpu.LoadProgram(program);

       // cpu.GetInstructionSize();

        
        cpu.Step();
        
        cpu.Dump_Registers();
        //cpu.Step();
        //cpu.Step();
        //cpu.Step();

    	return 0;

    } // Function main()
 // ====================