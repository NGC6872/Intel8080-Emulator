    #pragma once

    #include "pch.h"
    #include <vector>
    #include <string>
   
//  ===========
    class CPU {

        public:

            CPU(); // contructor
            ~CPU(); // decs.

            unsigned short SP = 0; // stack pointer
            unsigned short PC = 0; // program counter

            unsigned char Memory[1024 * 64];

            unsigned char opCode;

            int instructionSize = 1;

//          Registers below

//          ====================
            unsigned char A = 0;
            unsigned char B = 0;
            unsigned char C = 0;
            unsigned char D = 0;
            unsigned char E = 0;
            unsigned char H = 0;
            unsigned char L = 0;
//          ====================

//          Flags below

//          ====================
            struct StatusFlags {

                unsigned char S : 1;      // Sign flag
                unsigned char Z : 1;      // Zero flag - Set if the result is zero
                unsigned char P : 1;      // Parity flag - Set if the number of 1 bits in the result is even
                unsigned char C : 1;      // Carry flag - Set if the last addition operation resulted in a carry or if the last subtraction operation required a borrow
                unsigned char A : 1;     // Auxiliary carry flag - used for binary-coded decimal arithmetic

            } FLAGS;
//          =======

            void Step();  // Execute a single instruction
            void Reset(); // Reset the CPU to a known state
            void LoadProgramIntoMemoryAt(std::vector<unsigned char>, unsigned int); // load program into memory, starting at 0x00
            void GetInstructionSize();
            void SetArithmeticFlags(unsigned short);
            void SetCarryFlag(unsigned short);
            void SetZeroFlag(unsigned short);
            void Dump_Registers();
            int  Parity(int, int);

    }; // Class CPU()
//  =================