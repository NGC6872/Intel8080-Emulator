 //Intel 8080 CPU emulator written in C++.
 //Nate Hargrove
 //12/1/21

    #include <iostream>
    #include <fstream>
    #include "CPU.h"
    #include <vector>
    #include "../packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.3/build/native/include/gtest/internal/gtest-port.h"
#include "../packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.3/build/native/include/gtest/gtest.h"
       


 // // ============
 //   int main() {
 //   
 //       CPU cpu;

 //        //Open binary file, read each byte and store each byte in a vector

 //       std::fstream file("invaders.concatenated", std::ios::in | std::ios::out | std::ios::binary);

 //       file.seekg(0);

 //       std::vector<unsigned char> program;

 //       while (!file.eof()) {

 //           unsigned char x;

 //           file.read(reinterpret_cast<char*>(&x), 1);

 //           program.push_back(x);

 //       }

 //       // Load program into memory

 //       cpu.LoadProgramIntoMemoryAt(program, 0x00);

 //       cpu.PC = 3;
 //      
 //       while (true) {

 //           
 //           cpu.Step();
 //       }
 //       


 //    
 //      
 //   	return 0;

 //   } // Function main()
 //// ====================

GTEST_API_ int main(int argc, char** argv) {

    printf("Running main() google test", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}