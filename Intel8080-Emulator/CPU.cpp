#include "pch.h"

//  ================
#include "CPU.h"
#include <iostream>
//  ================

//  ============
CPU::CPU() {





} // Constructor CPU()
//  ======================

//  =============
CPU::~CPU() {



} // Deconstructor CPU()
//  ========================

//  ==================
void CPU::Step() {

    opCode = ((Memory[PC]));

    // Instruction set

    switch (opCode) {

    case 0x00: {


        // Does nothing


    }

             break;

    case 0x01: { // LXI B,D16 => Load 16-bit immediate data into BC Register pair

        // Instruction length: 3 bytes

        C = Memory[PC + 1];
        B = Memory[PC + 2];

        PC += 2;
    }

             break;

    case 0x02: { // Store accumulator indirect => A stored into  BC Register pair

        unsigned short pair = (B << 8) | (C);
        Memory[pair] = A;

    }

             break;

    case 0x03: { // INX B => Add 1 to register pair BC

        unsigned short pair = (B << 8) | (C);
        pair += 1;

        // the left register gets the Most Significant byte and the right register gets the Least Significant byte

        B = (pair & 0xff00) >> 8;
        C = pair & 0xff;

    }

             break;

    case 0x04: { // INR B => Add 1 to register B and set flags

        B += 1;

        //set flags

        if (((unsigned short)B & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)B & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        if ((unsigned short)B > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        FLAGS.P = Parity(B, 8);

        A = (unsigned short)B & 0xff;

    }

             break;

    case 0x05: { // DCR B => Subtract 1 from register B and set flags

        B -= 1;

        //set flags

        if (((unsigned short)B & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)B & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        if ((unsigned short)B > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        FLAGS.P = Parity(B, 8);

        A = (unsigned short)B & 0xff;

    }

             break;

    case 0x06: { // MVI B, D8 => Move immediate data into register B, Instruction length: 2 bytes

        B = Memory[PC + 1];
        PC++;

    }

             break;

    case 0x07: { // Rotate accumulator left (RLC) =>  Each binary bit of the accumulator is rotated left by one position. Bit D7 is placed in the position of D0 as well as in the Carry flag.

        unsigned char temp = A;

        A = ((temp << 1) | (temp & 0x80) >> 7);
        FLAGS.C = ((temp & 0x80) == 0x80);

    }

             break;

    case 0x08: {

        // Does nothing

    }

             break;

    case 0x09: { // DAD B => The 16-bit contents of the specified register pair are added to the contents of the HL register and the sum is stored in the HL register. If the result is > 16 bits, set carry flag.

        unsigned int pair_BC = (B << 8) | (C);
        unsigned int pair_HL = (H << 8) | (L);

        pair_HL = pair_BC + pair_HL;

        if (pair_HL > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        H = (pair_HL & 0xff00) >> 8; // H gets the most significant byte
        L = pair_HL & 0xff;          // L gets least significant byte

    }

             break;


    case 0x0A: { // LDAX B => The contents of the designated register pair (in this case, BC) point to a memory location. This instruction copies the contents of that memory location into the accumulator. The contents of either the register pair or the memory location are not altered.

        unsigned short pair_BC = (B << 8) | (C);
        A = Memory[pair_BC];

    }

             break;

    case 0x0B: { // DCX B => The contents of the designated register pair are decremented by 1 and the result is stored in the same place.

        unsigned short pair_BC = (B << 8) | (C);
        pair_BC -= 1;

        B = (pair_BC & 0xff00) >> 8; // B (the leftmost register in the pair) gets the most significant byte
        C = (pair_BC & 0xff); // C (the rightmost register in the pair) gets the least significant byte

    }

             break;

    case 0x0C: { // INR C => Add 1 to register C and set flags

        C += 1;

        //set flags

        if (((unsigned short)C & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)C & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        if ((unsigned short)C > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        FLAGS.P = Parity(C, 8);

        A = (unsigned short)C & 0xff;

    }

             break;

    case 0x0D: { // Dec C => Subtract 1 from register C and set flags

        C -= 1;

        //set flags

        if (((unsigned short)C & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)C & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        if ((unsigned short)C > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        //P = Parity(C & 0xff);

        A = (unsigned short)C & 0xff;

    }

             break;

    case 0x0e: { // MVI C, D8

        C = Memory[PC + 1];
        PC++;

    }

             break;

    case 0x0f: { // RRC => Each binary bit of the accumulator is rotated right by one position. Bit D0 is placed in the position of D7 as well as in the Carry flag. CY is modified according to bit D0. S, Z, P, AC are not affected.

        unsigned char temp = A;

        A = ((temp & 1) << 7 | (temp >> 1));
        FLAGS.C = ((temp & 1) == 1);

    }

             break;

    case 0x11: { // LXI D, D16, Instruction length: 3 bytes

        //pair = DE

        E = Memory[PC + 1];
        D = Memory[PC + 2];
        PC += 2;

    }

             break;

    case 0x12: { // STAX D

        unsigned short pair_DE = (D << 8) | (E);

        Memory[pair_DE] = A;

    }

             break;

    case 0x13: { // INX D

        unsigned short pair_DE = (D << 8) | (E);
        pair_DE += 1;

        D = (pair_DE & 0xff00) >> 8;
        E = pair_DE & 0xff;

    }

             break;

    case 0x14: { // INR D

        D += 1;

        //set flags

        if (((unsigned short)D & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)D & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        FLAGS.P = Parity(D, 8);

        A = (unsigned short)D & 0xff;

    }

             break;

    case 0x15: { // DCR D

        D -= 1;

        //set flags

        if (((unsigned short)D & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)D & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        FLAGS.P = Parity(D, 8);

        A = (unsigned short)D & 0xff;

    }

             break;

    case 0x16: { // MVI D, D8, Instruction length: 2 bytes

        D = Memory[PC + 1];
        PC++;

    }

             break;

    case 0x17: { // RAL

        unsigned char temp_register = A;
        unsigned char temp_flag = FLAGS.C;

        A = (unsigned char)((temp_register << 1) | temp_flag);
        FLAGS.C = ((temp_register & 0x80) == 0x80);
    }

             break;

    case 0x19: { // DAD D => HL = HL + DE

        unsigned int pair_HL = (H << 8) | (L);
        unsigned int pair_DE = (D << 8) | (E);

        unsigned int result = pair_HL + pair_DE;

        FLAGS.C = (result > 0xff);

        H = (result & 0xff00) >> 8;

        L = (result & 0xff);

    }

             break;

    case 0x1A: { // LDAX D: A <- (DE)

        unsigned short pair_DE = (D << 8) | (E);

        A = Memory[pair_DE];

    }

             break;

    case 0x1B: { // DE = DE-1

        // pair both of the registers
        unsigned short pair_DE = (D << 8) | (E);

        pair_DE -= 1;

        D = (pair_DE & 0xff00) >> 8;
        E = (pair_DE & 0xff);

    }

             break;

    case 0x1C: { // INR E => E += 1, set flags

        E += 1;

        if (((unsigned short)E & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)E & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        FLAGS.P = Parity(E, 8);

        A = (unsigned short)E & 0xff;

    }

             break;

    case 0x1D: { // DEC E => E -= 1, set flags

        E -= 1;

        if (((unsigned short)E & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)E & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        FLAGS.P = Parity(E, 8);

        A = (unsigned short)E & 0xff;

    }

             break;

    case 0x1E: { // MVI E, D8, Instruction length: 2 bytes

        E = Memory[PC + 1];
        PC++;

    }

             break;

    case 0x1F: { // RAR

        A = ((FLAGS.C << 7) | (A >> 1));
        FLAGS.C = ((A & 1) == 1);

    }

             break;

    case 0xfe: { // CPI

        unsigned short x = (unsigned short)A - (unsigned short)Memory[PC + 1];

        FLAGS.Z = ((x & 0xff) == 0);
        FLAGS.S = (0x80 == (x & 0x80));

        FLAGS.C = (x > 0xff);

        FLAGS.P = Parity(x & 0xff, 8);

        FLAGS.A = (((A & 0xf) + (-(C) & 0xf)) > 0xf);

        PC++;

    }

             break;

    case 0x20: { // Does nothing



    }

             break;

    case 0x21: { // LXI H, D16, Instruction length: 3 bytes

        L = Memory[PC + 1];
        H = Memory[PC + 2];

        PC += 2;

    }

             break;

    case 0x22: { //SHLD adr, Instruction length: 3 bytes

        unsigned short address = (Memory[PC + 2] << 8) | (Memory[PC + 1]);

        Memory[address] = L;
        Memory[address + 1] = H;

        PC += 2;

    }

             break;

    case 0x23: { // INX H => HL = HL + 1

        unsigned short pair_HL = (H << 8) | (L);

        pair_HL += 1;

        H = (pair_HL & 0xff00) >> 8;
        L = pair_HL & 0xff;

    }

             break;

    case 0x24: { // INR H => H += 1, set flags

        H += 1;

        //set flags

        if (((unsigned short)H & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)H & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        FLAGS.P = Parity(H, 8);

        A = (unsigned short)H & 0xff;

    }

             break;

    case 0x25: { // DCR H => H -= 1, set flags

        H -= 1;

        //set flags

        if (((unsigned short)H & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)H & 0x80)) FLAGS.S = 1;

        else FLAGS.S = 0;

        if ((unsigned short)H > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        //P = Parity(H & 0xff);

        A = (unsigned short)H & 0xff;

    }

             break;

    case 0x26: { // MVI H, D8, instruction length: 2 bytes

        H = Memory[PC + 1];

        PC++;

    }

             break;

    case 0x27: { // Special instruction



    }

             break;

    case 0x28: { // Does nothing



    }

             break;

    case 0x29: { // DAD H => HL = HL + HL

        unsigned int pair_HL = (H << 8) | (L);

        pair_HL = pair_HL + pair_HL;

        C = (pair_HL > 0xff);

        H = (pair_HL & 0xff00) >> 8;

        L = pair_HL & 0xff;

    }

             break;

    case 0x2A: { // LHLD adr, Instruction length: 3 bytes

        unsigned short address = (Memory[PC + 2] << 8) | (Memory[PC + 1]);

        L = Memory[address];
        H = Memory[address + 1];

        PC += 2;

    }

             break;

    case 0x2B: { // DCX H, HL = HL - 1

        unsigned short pair_HL = (H << 8) | (L);

        pair_HL -= 1;

        H = (pair_HL & 0xff00) >> 8;
        L = (pair_HL & 0xff);

    }

             break;

    case 0x2C: { // INR L

        L += 1;

        if (((unsigned short)L & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((unsigned short)L & 0x80))FLAGS.S = 1;

        else FLAGS.S = 0;

        FLAGS.P = Parity(L, 8);

        A = (unsigned short)L & 0xff;

    }

             break;

    case 0x2D: { // DCR L

        L = L - 1;

        FLAGS.Z = ((L) == 0);

        FLAGS.S = ((L & 0x80) == 0x80);

        FLAGS.P = Parity(L, 8);

        FLAGS.A = ((L & 0xf) == 0x0);

    }

             break;

    case 0x2E: { // MVI L, D8, Instruction length: 2 bytes

        L = Memory[PC + 1];

        PC++;

    }

             break;

    case 0x2F: { // CMA

        A = (unsigned char)(~A);

    }

             break;

    case 0xe6: { // ANI

        unsigned char x = A & Memory[PC + 1];

        FLAGS.Z = (x == 0);
        FLAGS.S = (0x80 == (x & 0x80));

        FLAGS.P = Parity(x, 8);

        FLAGS.C = 0;
        FLAGS.A = x;

        PC++;

    }

             break;

    case 0x30: { // Does nothing



    }

             break;

    case 0x31: {  // LXI SP, D16, Instruction length: 3 bytes

        SP = (Memory[PC + 2] << 8) | (Memory[PC + 1]);

        PC += 2;

    }

             break;

    case 0x32: {  // STA adr, Instruction length: 3 bytes => Accumulator 8-bit content will be stored to a memory location whose 16-bit address is indicated in the instruction as a16

        unsigned short adr = (Memory[PC + 2] << 8) | (Memory[PC + 1]);

        Memory[adr] = A;

        PC += 2;

    }

             break;

    case 0x33: { // INX SP

        SP++;

    }

             break;

    case 0x34: { // INR M => (HL) = (HL) + 1, set flags

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = Memory[offset] + 1;

        if ((Memory[offset]) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((Memory[offset]) & 0x80) == 0x80) FLAGS.S = 1;

        else FLAGS.S = 0;


        //P = Parity(Memory[offset] & 0xff);

        A = ((Memory[offset] & 0xf) == 0xf);

    }

             break;

    case 0x35: { // DCR M => (HL) = (HL) - 1, set flags

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = Memory[offset] - 1;

        if ((Memory[offset]) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (((Memory[offset]) & 0x80) == 0x80) FLAGS.S = 1;

        else FLAGS.S = 0;


        //P = Parity(Memory[offset] & 0xff);

        A = ((Memory[offset] & 0xf) == 0xf);

    }

             break;

    case 0x36: { // MVI M, D8, instruction length: 2 bytes

        unsigned char offset = (H << 8) | (L);

        Memory[offset] = Memory[PC + 1];

        PC++;

    }

             break;

    case 0x37: { // STC Set the carry flag = 1

        C = 1;

    }

             break;

    case 0x38: { // Does nothing



    }

             break;

    case 0x39: { // DAD SP HL = HL + SP, set carry flag

        unsigned int pair_HL = (H << 8) | (L);

        pair_HL = pair_HL + (unsigned int)SP;

        C = (pair_HL > 0xff);

        H = (pair_HL & 0xff00) >> 8;

        L = pair_HL & 0xff;

    }

             break;

    case 0x3A: { // LDA adr, instruction length: 3 bytes

        unsigned short adr = (Memory[PC + 2] << 8) | (Memory[PC + 1]);

        A = Memory[adr];

        PC += 2;

    }

             break;

    case 0x3B: { // DCX SP

        SP--;

        B = (SP & 0xff00) >> 8;
        L = SP & 0xff;

    }

             break;

    case 0x3C: { // INR A, set flags

        A += 1;

        if ((A) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if ((A & 0x80) == 0x80) FLAGS.S = 1;

        else FLAGS.S = 0;

        //P = Parity(A & 0xff);

        A = ((A & 0xf) == 0xf);

    }

             break;

    case 0x3D: { // DCR A, set flags

        A -= 1;

        if ((A) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if ((A & 0x80) == 0x80) FLAGS.S = 1;

        else FLAGS.S = 0;

        //P = Parity(A & 0xff);

        A = ((A & 0xf) == 0xf);

    }

             break;

    case 0x3E: { // MVI A, D8, instruction length: 2 bytes

        A = Memory[PC + 1];

        PC++;

    }

             break;

    case 0x3F: { // CMC, set carry flag

        C = !C;

    }

             break;

    case 0x40: {

        B = B;

    }

             break;

    case 0x41: {

        B = C;

    }

             break;

    case 0x42: {

        B = D;

    }

             break;

    case 0x43: {

        B = E;

    }

             break;

    case 0x44: {

        B = H;

    }

             break;

    case 0x45: {

        B = L;

    }

             break;

    case 0x46: { // B <- (HL) 

        unsigned short offset = (H << 8) | (L);

        B = Memory[offset];

    }

             break;

    case 0x47: {

        B = A;

    }

             break;

    case 0x48: {

        B = C;

    }

             break;

    case 0x49: {

        C = C;

    }

             break;

    case 0x4a: {

        C = D;

    }

             break;

    case 0x4b: {

        C = E;

    }

             break;

    case 0x4c: {

        C = H;

    }

             break;

    case 0x4d: {

        C = L;

    }

             break;

    case 0x4e: {

        unsigned short offset = (H << 8) | (L);

        C = Memory[offset];

    }

             break;

    case 0x4f: {

        C = A;
    }

             break;

    case 0x50: {

        D = B;
    }

             break;

    case 0x51: {

        D = C;
    }

             break;

    case 0x52: {

        D = D;
    }

             break;

    case 0x53: {

        D = E;
    }

             break;

    case 0x54: {

        D = H;
    }

             break;

    case 0x55: {

        D = L;
    }

             break;

    case 0x56: {

        unsigned short offset = (H << 8) | (L);

        D = Memory[offset];

    }

             break;

    case 0x57: {

        D = A;

    }

             break;

    case 0x58: {

        E = B;

    }

             break;

    case 0x59: {

        E = C;

    }

             break;

    case 0x5A: {

        E = D;

    }

             break;

    case 0x5B: {

        E = E;

    }

             break;

    case 0x5C: {

        E = H;

    }

             break;

    case 0x5D: {

        E = L;

    }

             break;

    case 0x5E: {

        unsigned short offset = (H << 8) | (L);

        E = Memory[offset];

    }

             break;

    case 0x5F: {

        E = A;

    }

             break;

    case 0x60: {

        H = B;

    }

             break;

    case 0x61: {

        H = C;

    }

             break;

    case 0x62: {

        H = D;

    }

             break;

    case 0x63: {

        H = E;

    }

             break;

    case 0x64: {

        H = H;

    }

             break;

    case 0x65: {

        H = L;

    }

             break;

    case 0x66: {

        unsigned short offset = (H << 8) | (L);

        H = Memory[offset];

    }

             break;

    case 0x67: {

        H = A;

    }

             break;

    case 0x68: {

        L = B;

    }

             break;

    case 0x69: {

        L = C;

    }

             break;

    case 0x6A: {

        L = D;

    }

             break;

    case 0x6B: {

        L = E;

    }

             break;

    case 0x6C: {

        L = H;

    }

             break;

    case 0x6D: {

        L = L;

    }

             break;

    case 0x6E: {

        unsigned short offset = (H << 8) | (L);

        L = Memory[offset];

    }

             break;

    case 0x6F: {

        L = A;

    }

             break;

    case 0x70: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = B;

    }

             break;

    case 0x71: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = C;

    }

             break;

    case 0x72: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = D;

    }

             break;

    case 0x73: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = E;

    }

             break;

    case 0x74: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = H;

    }

             break;

    case 0x75: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = L;

    }

             break;

    case 0x76: { // Special instruction



    }

             break;

    case 0x77: {

        unsigned short offset = (H << 8) | (L);

        Memory[offset] = A;

    }

             break;

    case 0x78: {

        A = B;

    }

             break;

    case 0x79: {

        A = B;

    }

             break;

    case 0x7A: {

        A = D;

    }

             break;

    case 0x7B: {

        A = E;

    }

             break;

    case 0x7C: {

        A = H;

    }

             break;

    case 0x7D: {

        A = L;

    }

             break;

    case 0x7E: {

        unsigned short offset = (H << 8) | (L);

        A = Memory[offset];

    }

             break;

    case 0x7F: {

        A = A;

    }

             break;

    case 0x80: {

        unsigned short result = (unsigned short)A + (unsigned short)B;

        if ((result & 0xff) == 0) FLAGS.Z = 1;

        else FLAGS.Z = 0;

        if (result & 0x80) FLAGS.S = 1;

        else FLAGS.S = 0;

        if (result > 0xff) FLAGS.C = 1;

        else FLAGS.C = 0;

        //FLAGS.P = Parity(result & 0xff); Implement parity function later

        A = result & 0xff;

    }

             break;

    case 0x81: {

        unsigned short result = (unsigned short)A + (unsigned short)C;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x82: {

        unsigned short result = (unsigned short)A + (unsigned short)D;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x83: {

        unsigned short result = (unsigned short)A + (unsigned short)E;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x84: {

        unsigned short result = (unsigned short)A + (unsigned short)H;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x85: {

        unsigned short result = (unsigned short)A + (unsigned short)L;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0xC6: {

        unsigned short result = (unsigned short)A + (unsigned short)(Memory[PC + 1]);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0xC1: {

        C = Memory[SP];

        B = Memory[SP + 1];

        SP += 2;

    }

             break;

    case 0xC5: {

        Memory[SP - 1] = B;
        Memory[SP - 2] = C;

        SP -= 2;

    }

             break;

    case 0xf1: {

        A = Memory[SP + 1];

        unsigned char psw = Memory[SP];

        FLAGS.Z = (0x01 == (psw & 0x01));
        FLAGS.S = (0x02 == (psw & 0x02));
        FLAGS.P = (0x04 == (psw & 0x04));
        FLAGS.C = (0x05 == (psw & 0x08));
        FLAGS.A = (0x10 == (psw & 0x10));

        SP += 2;

    }

             break;

    case 0xf5: {

        A = Memory[SP - 1];

        unsigned char psw = (FLAGS.Z | FLAGS.S << 1 | FLAGS.P << 2 | FLAGS.C << 3 | FLAGS.A << 4);

        Memory[SP - 2] = psw;

        SP -= SP;

    }

             break;


    case 0x86: {

        unsigned short offset = (H << 8) | (L);

        unsigned short result = (unsigned short)A + (Memory[offset]);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x87: {

        unsigned short result = (unsigned short)A + (unsigned short)A;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x88: {

        unsigned short result = (unsigned short)A + (unsigned short)B + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x89: {

        unsigned short result = (unsigned short)A + (unsigned short)C + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x8a: {

        unsigned short result = (unsigned short)A + (unsigned short)D + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x8b: {

        unsigned short result = (unsigned short)A + (unsigned short)E + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x8c: {

        unsigned short result = (unsigned short)A + (unsigned short)H + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x8d: {

        unsigned short result = (unsigned short)A + (unsigned short)L + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x8e: {


        unsigned short offset = (H << 8) | (L);
        unsigned short result = (unsigned short)A + Memory[offset] + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x8f: {

        unsigned short result = (unsigned short)A + (unsigned short)A + (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x90: {

        unsigned short result = (unsigned short)A - (unsigned short)B;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x91: {

        unsigned short result = (unsigned short)A - (unsigned short)C;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x92: {

        unsigned short result = (unsigned short)A + (unsigned short)D;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x93: {

        unsigned short result = (unsigned short)A - (unsigned short)E;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x94: {

        unsigned short result = (unsigned short)A + (unsigned short)H;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x95: {

        unsigned short result = (unsigned short)A - (unsigned short)L;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x96: {

        unsigned short offset = (H << 8) | (L);

        unsigned short result = (unsigned short)A - (Memory[offset]);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x97: {

        unsigned short result = (unsigned short)A - (unsigned short)A;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x98: {

        unsigned short result = (unsigned short)A - (unsigned short)B - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x99: {

        unsigned short result = (unsigned short)A - (unsigned short)C - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x9a: {

        unsigned short result = (unsigned short)A - (unsigned short)D - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x9b: {

        unsigned short result = (unsigned short)A - (unsigned short)D - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x9c: {

        unsigned short result = (unsigned short)A - (unsigned short)H - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x9d: {

        unsigned short result = (unsigned short)A - (unsigned short)L - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x9e: {

        unsigned short offset = (H << 8) | (L);

        unsigned short result = (unsigned short)A - Memory[offset] - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0x9f: {

        unsigned short offset = (H << 8) | (L);

        unsigned short result = (unsigned short)A - (unsigned short)A - (unsigned short)(FLAGS.C);

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0xa0: {

        unsigned short result = A & B;

        FLAGS.Z = ((result & 0xff) == 0);
        FLAGS.S = ((result & 0x80) != 0);
        FLAGS.C = (result > 0xff);
        //P = Parity(answer & 0xff);
        A = result & 0xff;

    }

             break;

    case 0xc2: { // JNZ

        if (FLAGS.Z == 0)
            PC = (Memory[PC + 2] << 8) | Memory[PC + 1];

        else PC += 2;

    }

             break;

    case 0xc3: { // JMP

        PC = (Memory[PC + 2] << 8) | Memory[PC + 1];

    }

             break;

    case 0xcd: { // JMP

        if (((Memory[PC + 2] << 8) | Memory[PC + 1]) == 5) {

            if (C == 9) {

                unsigned short offset = (D << 8) | (E);

                unsigned char* str = &Memory[offset + 3];

                while (*str != '$')
                    printf("%c", *str++);
                printf("\n");
            }



            else if (C == 2) {

                printf("print char routine called\n");

            }
        }

        else if ((Memory[PC + 2] << 8) | Memory[PC + 1]) {
            std::cout << "exiting...";
            exit(0);
        }

        else {
            unsigned short result = PC + 2;

            Memory[SP - 1] = (result >> 8) & 0xff;
            Memory[SP - 2] = (result & 0xff);

            SP -= 2;

            PC = (Memory[PC + 2] << 8) | Memory[PC + 1];

        }

    }

             break;

    case 0xc9: {

        PC = Memory[SP] | (Memory[SP + 1] << 8);
        SP += 2;

    }

             break;

    default: std::cout << "UNSUPPORTED OPCODE: " << std::hex << opCode << std::endl;

    }

    PC++;

} // Function Step()
//  ====================

//  ===================
void CPU::Reset() {

    memset(Memory, 0, sizeof(Memory));

    PC = 0;
    SP = 0;

} // Function Reset()
//  =====================

//  =================================================================================================================
void CPU::LoadProgramIntoMemoryAt(std::vector<unsigned char> program_to_load_into_memory, unsigned int offset) {

    // Reset CPU to known state

    Reset();

    // load the program into memory

    for (int ii = 0; ii < program_to_load_into_memory.size(); ii++) {

        Memory[ii + offset] = program_to_load_into_memory[ii];

    }

    PC = 0;

} // Function loadProgramIntoMemoryAt()
//  =======================================

//  ============================
void CPU::Dump_Registers() {

    std::cout << "================";

    std::cout << std::endl << "STACK POINTER: " << SP << std::endl << std::endl;

    std::cout << "PROGRAM COUNTER: " << PC << std::endl << std::endl;

    std::cout << "Dumping registers..." << std::endl << std::endl;

    std::cout << "A: " << std::hex << A << std::endl << std::endl;

    std::cout << "B: " << std::hex << B << std::endl << std::endl;

    std::cout << "C: " << std::hex << C << std::endl << std::endl;

    std::cout << "D: " << std::hex << D << std::endl << std::endl;

    std::cout << "E: " << std::hex << E << std::endl << std::endl;

    std::cout << "H: " << std::hex << H << std::endl << std::endl;

    std::cout << "L: " << std::hex << L << std::endl;

    std::cout << "===";

} // Function Dump_registers()
//   ==============================

//  ==================================
int CPU::Parity(int x, int size) {

    int i;

    int p = 0;

    x = (x & ((1 << size) - 1));

    for (i = 0; i < size; i++) {

        if (x & 0x1) p++;
        x = x >> 1;

    }

    return (0 == (p & 0x1));

    return 0;

} // Function Parity()
//  ======================

//  ================================
void CPU::GetInstructionSize() {

    opCode = ((Memory[PC]));

    // only change value of numBytes when instruction size > 1.

    switch (opCode) {

    case 0x01: {

        instructionSize = 3;

    }

             break;

    case 0x06: {

        instructionSize = 2;

    }

             break;

    case 0x11: {

        instructionSize = 3;

    }

             break;

    case 0x16: {

        instructionSize = 2;

    }

             break;

    case 0x1e: {

        instructionSize = 2;

    }

             break;

    case 0x21: {

        instructionSize = 3;

    }

             break;

    case 0x22: {

        instructionSize = 3;

    }

             break;

    case 0x26: {

        instructionSize = 2;

    }

             break;

    case 0x2a: {

        instructionSize = 3;

    }

             break;

    case 0x2e: {

        instructionSize = 2;

    }

             break;

    case 0x31: {

        instructionSize = 3;

    }

             break;

    case 0x32: {

        instructionSize = 3;

    }

             break;

    case 0x36: {

        instructionSize = 2;

    }

             break;

    case 0x3a: {

        instructionSize = 3;

    }

             break;

    case 0x3e: {

        instructionSize = 2;

    }

             break;

    case 0xc2: {

        instructionSize = 3;

    }

             break;

    case 0xc3: {

        instructionSize = 3;

    }

             break;

    case 0xc4: {

        instructionSize = 3;

    }

             break;

    case 0xc6: {

        instructionSize = 2;

    }

             break;

    case 0xca: {

        instructionSize = 3;

    }

             break;

    case 0xcc: {

        instructionSize = 3;

    }

             break;

    case 0xcd: {

        instructionSize = 3;

    }

             break;

    case 0xce: {

        instructionSize = 2;

    }

             break;

    case 0xd2: {

        instructionSize = 3;

    }

             break;

    case 0xd3: {

        instructionSize = 2;

    }

             break;

    case 0xd4: {

        instructionSize = 3;

    }

             break;

    case 0xd6: {

        instructionSize = 2;

    }

             break;

    case 0xda: {

        instructionSize = 3;

    }

             break;

    case 0xdb: {

        instructionSize = 2;

    }

             break;

    case 0xdc: {

        instructionSize = 3;

    }

             break;

    case 0xde: {

        instructionSize = 2;

    }

             break;

    case 0xe2: {

        instructionSize = 3;

    }

             break;

    case 0xe4: {

        instructionSize = 3;

    }

             break;

    case 0xe6: {

        instructionSize = 2;

    }

             break;

    case 0xea: {

        instructionSize = 3;

    }

             break;

    case 0xec: {

        instructionSize = 3;

    }

             break;

    case 0xee: {

        instructionSize = 2;

    }

             break;

    case 0xf2: {

        instructionSize = 3;

    }

             break;

    case 0xf4: {

        instructionSize = 3;

    }

             break;

    case 0xf6: {

        instructionSize = 2;

    }

             break;

    case 0xfa: {

        instructionSize = 3;

    }

             break;

    case 0xfc: {

        instructionSize = 3;

    }

             break;

    case 0xfe: {

        instructionSize = 2;

    }

             break;

    }

} // GetInstructionSize()
//  =========================

//  ==================================================
void CPU::Set_Z_S_C_Flags(unsigned short result) {

    if ((result & 0xff) == 0) FLAGS.Z = 1;

    else FLAGS.Z = 0;

    if (result & 0x80) FLAGS.S = 1;

    else FLAGS.S = 0;

    if (result > 0xff) FLAGS.C = 1;

    else FLAGS.C = 0;

} // Function Set_Z_S_C_Flags()
//  ===============================
