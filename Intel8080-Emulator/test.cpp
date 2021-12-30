#include "pch.h"
#include "CPU.h"
#include "gtest/gtest.h"

CPU cpu;

TEST(INTEL8080, LXI_B_D16) {
	
	cpu.C = 0x00;
	cpu.B = 0x01;
	
	EXPECT_EQ(cpu.C, 0x00);
	EXPECT_EQ(cpu.B, 0x01);
	
}

TEST(INTEL8080, Store_A_in_memory_address_in_register_pair_BC) {

	cpu.C = 0x73;
	cpu.B = 0x29;

	unsigned short pair_BC = (cpu.B << 8) | (cpu.C);

	cpu.Memory[pair_BC] = cpu.A;

	EXPECT_EQ(cpu.Memory[pair_BC], cpu.A);
	
	//EXPECT_TRUE(true);
}

TEST(INTEL8080, INCREMENT_register_pair_BC) {

	cpu.C = 0x73;
	cpu.B = 0x29;

	unsigned short pair_BC = (cpu.B << 8) | (cpu.C);

	pair_BC = pair_BC + 1;

	cpu.B = (pair_BC & 0xff00) >> 8;
	cpu.C = pair_BC & 0xff;

	EXPECT_EQ(cpu.B, (pair_BC & 0xff00) >> 8);
	EXPECT_EQ(cpu.C, pair_BC & 0xff);

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, INR_B) {

	cpu.B = 0x29;

	cpu.B += 1;

	//set flags

	if (((unsigned short)cpu.B & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.B & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.B, 8);

	cpu.A = (unsigned short)cpu.B & 0xff;



	EXPECT_EQ(cpu.B, 0x29 + 1);
	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.B) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.B & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.B & 0xf) == 0xf));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(cpu.B, 8));

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, DCR_B) {

	cpu.B = 0x29;

	cpu.B -= 1;

	//set flags

	if (((unsigned short)cpu.B & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.B & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.B, 8);

	cpu.A = (unsigned short)cpu.B & 0xff;

	EXPECT_EQ(cpu.B, 0x29 - 1);
	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.B) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.B & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.B & 0xf) == 0xf));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(cpu.B, 8));

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, MOV_B) {

	unsigned char tempVal = 0x57;

	cpu.B = tempVal;

	EXPECT_EQ(cpu.B, 0x57);

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, RLC) { 

	cpu.A = 0x70;

	unsigned char temp = cpu.A;

	cpu.A = ((temp << 1) | (temp & 0x80) >> 7);
	cpu.FLAGS.C = ((temp & 0x80) == 0x80);


	EXPECT_EQ(cpu.A, ((temp << 1) | (temp & 0x80) >> 7));
	EXPECT_EQ(cpu.FLAGS.C, ((temp & 0x80) == 0x80));
	//EXPECT_TRUE(true);
}

TEST(INTEL8080, DAD_B) {

	cpu.B = 0x60;
	cpu.C = 0x45;
	cpu.H = 0x11;
	cpu.L = 0x90;


	unsigned int pair_BC = (cpu.B << 8) | (cpu.C);
	unsigned int pair_HL = (cpu.H << 8) | (cpu.L);

	pair_HL = pair_BC + pair_HL;

	if (pair_HL > 0xff) cpu.FLAGS.C = 1;

	else cpu.FLAGS.C = 0;

	cpu.H = (pair_HL & 0xff00) >> 8; // H gets the most significant byte
	cpu.L = pair_HL & 0xff;          // L gets least significant byte

	EXPECT_EQ(cpu.FLAGS.C, (pair_HL > 0xff));

	EXPECT_EQ(cpu.H, ((pair_HL & 0xff00) >> 8));

	EXPECT_EQ(cpu.L, pair_HL & 0xff);
	//EXPECT_TRUE(true);
}

TEST(INTEL8080, LDAX_B) {

	cpu.B = 0x25;
	cpu.C = 0x82;

	unsigned short pair_BC = (cpu.B << 8) | (cpu.C);
	cpu.A = cpu.Memory[pair_BC];

	EXPECT_EQ(cpu.A, (cpu.Memory[(cpu.B << 8) | (cpu.C)]));

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, DCX_B) { // DCX B

	cpu.B = 0x15;
	cpu.C = 0x27;

	unsigned short pair_BC = (cpu.B << 8) | (cpu.C);

	pair_BC -= 1;

	cpu.B = (pair_BC & 0xff00) >> 8; // B (the leftmost register in the pair) gets the most significant byte
	cpu.C = (pair_BC & 0xff); // C (the rightmost register in the pair) gets the least significant byte

	EXPECT_EQ(cpu.B, (pair_BC & 0xff00) >> 8);
	EXPECT_EQ(cpu.C, pair_BC & 0xff);

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, INR_C) { // INR C

	cpu.C = 0x14;

	cpu.C += 1;

	//set flags

	if (((unsigned short)cpu.C & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.C & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	if ((unsigned short)cpu.C > 0xff) cpu.FLAGS.C = 1;

	else cpu.FLAGS.C = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.C, 8);

	cpu.A = (unsigned short)cpu.C & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.C) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.C & 0x80) == 0x80));

	EXPECT_EQ(cpu.FLAGS.A, ((cpu.C & 0xf) == 0xf));

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, DEC_C) { // DEC_C

	cpu.C = 0x09;

	cpu.C -= 1;

	//set flags

	if (((unsigned short)cpu.C & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.C & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	if ((unsigned short)cpu.C > 0xff) cpu.FLAGS.C = 1;

	else cpu.FLAGS.C = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.C, 8);

	cpu.A = (unsigned short)cpu.C & 0xff;


	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.C) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.C & 0x80) == 0x80));

	EXPECT_EQ(cpu.FLAGS.A, ((cpu.C & 0xf) == 0x0));

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, MVI_C_D8) { // MVI C D8
	
	cpu.C = 0x02;

	cpu.PC = 0x00;

	cpu.Memory[0] = 0x76;
	cpu.Memory[1] = 0x30;

	cpu.C = cpu.Memory[cpu.PC + 1];
	cpu.PC++;


	EXPECT_EQ(cpu.C, 0x30);

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, RRC) { // RRC

	cpu.A = 0x13;

	unsigned char temp = cpu.A;

	cpu.A = ((temp & 1) << 7 | (temp >> 1));
	cpu.FLAGS.C = ((temp & 1) == 1);



	EXPECT_EQ(cpu.A, ((temp & 1) << 7 | (temp >> 1)));
	EXPECT_EQ(cpu.FLAGS.C, ((temp & 1) == 1));

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, LXI_D_D16) { // LXI_D_D16

	cpu.D = 0x05;
	cpu.E = 0x83;

	cpu.PC = 0x00;

	cpu.Memory[0] = 0x23;
	cpu.Memory[1] = 0x74;
	cpu.Memory[2] = 0x45;

	cpu.E = cpu.Memory[cpu.PC + 1];
	cpu.D = cpu.Memory[cpu.PC + 2];

	cpu.PC += 2;

	EXPECT_EQ(cpu.E, 0x74);
	EXPECT_EQ(cpu.D, 0x45);

	//EXPECT_TRUE(true);
}

TEST(INTEL8080, STAX_D) { // STAX D

	cpu.D = 0x99;
	cpu.E = 0x01;

	unsigned short pair_DE = (cpu.D << 8) | (cpu.E);

	cpu.Memory[pair_DE] = cpu.A;

	EXPECT_EQ(cpu.Memory[(cpu.D << 8) | (cpu.E)], cpu.A);
	
	//EXPECT_TRUE(true);

}

TEST(INTEL8080, INX_D) { // INX D

	cpu.D = 0x09;
	cpu.E = 0x02;

	unsigned short pair_DE = (cpu.D << 8) | (cpu.E);

	pair_DE += 1;

	cpu.D = (pair_DE & 0xff00) >> 8;
	cpu.E = pair_DE & 0xff;

	EXPECT_EQ(cpu.D, (pair_DE & 0xff00) >> 8);
	EXPECT_EQ(cpu.E, pair_DE & 0xff);

}

TEST(INTEL8080, INR_D) { // INR D

	cpu.D = 0x87;
	
	cpu.D += 1;

	//set flags

	if (((unsigned short)cpu.D & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.D & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.D, 8);

	cpu.A = (unsigned short)cpu.D & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.D) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.D & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.D & 0xf) == 0xf));

}

TEST(INTEL8080, DCR_D) { // DCR D

	cpu.D = 0x54;

	cpu.D -= 1;

	//set flags

	if (((unsigned short)cpu.D & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.D & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.D, 8);

	cpu.A = (unsigned short)cpu.D & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.D) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.D & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.D & 0xf) == 0x0));

}

TEST(INTEL8080, MVI_D_D8) { // DCR D

	cpu.D = 0x98;

	cpu.PC = 0x00;

	cpu.Memory[1] = 0x55;

	cpu.D = cpu.Memory[cpu.PC + 1];
	cpu.PC++;

	EXPECT_EQ(cpu.D, 0x55);

}

TEST(INTEL8080, RAL) { // RAL // Fixed an error when cpu.A >= 0x80. 0x80 is not a char value, it is an integer.

	cpu.A = 0x20;

	unsigned char temp_register = cpu.A;
	unsigned char temp_flag = cpu.FLAGS.C;

	cpu.A = (unsigned char)((temp_register << 1) | temp_flag);
	cpu.FLAGS.C = ((temp_register & 0x80) == 0x80);


	EXPECT_EQ(cpu.A, (unsigned char)((temp_register << 1) | temp_flag));

	EXPECT_EQ(cpu.FLAGS.C, ((temp_register & 0x80) == 0x80));

}

TEST(INTEL8080, DAD_D) { // DAD D => HL = HL + DE

	cpu.H = 0x99;
	cpu.L = 0x80;

	cpu.D = 0x80;
	cpu.E = 0x80;


	unsigned int pair_HL = (cpu.H << 8) | (cpu.L);
	unsigned int pair_DE = (cpu.D << 8) | (cpu.E);

	unsigned int result = pair_HL + pair_DE;

	cpu.FLAGS.C = (result > 0xff);

	cpu.H = (result & 0xff00) >> 8;

	cpu.L = (result & 0xff);

	EXPECT_EQ(cpu.FLAGS.C, (result > 0xff));

	EXPECT_EQ(cpu.H, (result & 0xff00) >> 8);

	EXPECT_EQ(cpu.L, result & 0xff);

}

TEST(INTEL8080, LDAX_D) { // LDAX D: A <- (DE)

	cpu.D = 0x01;
	cpu.E = 0x99;

	unsigned short pair_DE = (cpu.D << 8) | (cpu.E);

	cpu.A = cpu.Memory[pair_DE];

	EXPECT_EQ(cpu.A, cpu.Memory[(cpu.D << 8) | (cpu.E)]);

}

TEST(INTEL8080, DCX_D) { // DCX D

	cpu.D = 0x23;
	cpu.E = 0x80;

	// pair both of the registers
	unsigned short pair_DE = (cpu.D << 8) | (cpu.E);

	pair_DE -= 1;

	cpu.D = (pair_DE & 0xff00) >> 8;
	cpu.E = (pair_DE & 0xff);

	EXPECT_EQ(cpu.D, (pair_DE & 0xff00) >> 8);
	EXPECT_EQ(cpu.E, (pair_DE & 0xff));

}

TEST(INTEL8080, INR_E) { // INR E

	cpu.E = 0x68;

	cpu.E += 1;

	if (((unsigned short)cpu.E & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.E & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.E, 8);

	cpu.A = (unsigned short)cpu.E & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.E) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.E & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.E & 0xf) == 0xf));

}

TEST(INTEL8080, DEC_E) { // DEC E

	cpu.E = 0x80;

	cpu.E -= 1;

	if (((unsigned short)cpu.E & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.E & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.E, 8);

	cpu.A = (unsigned short)cpu.E & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.E) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.E & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.E & 0xf) == 0x0));

}

TEST(INTEL8080, MVI_E) { // MVI E D8

	cpu.PC = 0;

	cpu.Memory[1] = 0x90;

	cpu.E = cpu.Memory[cpu.PC + 1];

	cpu.PC++;

	EXPECT_EQ(cpu.E, 0x90);
	
}

TEST(INTEL8080, RAR) { // RAR

	cpu.A = 0x80;

	unsigned char temp = cpu.A;

	unsigned char temp_flag = cpu.FLAGS.C;

	cpu.A = ((temp_flag << 7) | (temp >> 1));
	cpu.FLAGS.C = ((temp & 1) == 1);

	EXPECT_EQ(cpu.A, ((temp_flag << 7) | (temp >> 1)));
	EXPECT_EQ(cpu.FLAGS.C, ((temp & 1) == 1));

}

TEST(INTEL8080, CPI) { // CPI

	cpu.A = 0x10;

	cpu.PC = 0x00;

	cpu.Memory[cpu.A] = 0x80;
	
	cpu.Memory[cpu.PC + 1] = 0x30;

	unsigned short x = (unsigned short)cpu.A - (unsigned short)cpu.Memory[cpu.PC + 1];

	cpu.FLAGS.Z = ((x & 0xff) == 0);
	cpu.FLAGS.S = (0x80 == (x & 0x80));

	cpu.FLAGS.C = (x > 0xff);

	cpu.FLAGS.P = cpu.Parity(x & 0xff, 8);

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.C) & 0xf)) > 0xf);

	cpu.PC++;

	EXPECT_EQ(cpu.FLAGS.Z, ((x & 0xff) == 0));
	EXPECT_EQ(cpu.FLAGS.S, (0x80 == (x & 0x80)));
	EXPECT_EQ(cpu.FLAGS.C, (x > 0xff));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(x & 0xff, 8));
	EXPECT_EQ(cpu.FLAGS.A, (((cpu.A & 0xf) + (-(cpu.C) & 0xf)) > 0xf));

}

TEST(INTEL8080, LXI_H_D16) { // LXI_H_D16

	cpu.PC = 0x00;


	cpu.Memory[cpu.PC + 1] = 0x20;
	cpu.Memory[cpu.PC + 2] = 0x50;


	cpu.L = cpu.Memory[cpu.PC + 1];
	cpu.H = cpu.Memory[cpu.PC + 2];

	cpu.PC += 2;

	EXPECT_EQ(cpu.L, 0x20);
	EXPECT_EQ(cpu.H, 0x50);

}

TEST(INTEL8080, SHLD) { // SHLD

	cpu.L = 0x57;
	cpu.H = 0x03;

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 1] = 0x51;
	cpu.Memory[cpu.PC + 2] = 0x11;

	unsigned short address = (cpu.Memory[cpu.PC + 2] << 8) | (cpu.Memory[cpu.PC + 1]);

	cpu.Memory[address] = cpu.L;
	cpu.Memory[address + 1] = cpu.H;

	cpu.PC += 2;

	EXPECT_EQ(cpu.Memory[address], cpu.L);

	EXPECT_EQ(cpu.Memory[address + 1], cpu.H);

}

TEST(INTEL8080, INX_H) { // INX H

	cpu.H = 0x20;
	cpu.L = 0x45;

	unsigned short pair_HL = (cpu.H << 8) | (cpu.L);

	pair_HL += 1;

	cpu.H = (pair_HL & 0xff00) >> 8;
	cpu.L = pair_HL & 0xff;

	EXPECT_EQ(cpu.H, (pair_HL & 0xff00) >> 8);

	EXPECT_EQ(cpu.L, pair_HL & 0xff);

}

TEST(INTEL8080, INR_H) { // INR H

	cpu.H = 0x90;

	cpu.H += 1;

	//set flags

	if (((unsigned short)cpu.H & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.H & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.H, 8);

	cpu.A = (unsigned short)cpu.H & 0xff;

	EXPECT_EQ(cpu.H, 0x90 + 1);

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.H) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.H & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(cpu.H, 8));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.H & 0xf) == 0xf));

}

TEST(INTEL8080, DCR_H) { // DCR H

	cpu.H = 0x90;

	cpu.H -= 1;

	//set flags

	if (((unsigned short)cpu.H & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.H & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.H, 8);

	cpu.A = (unsigned short)cpu.H & 0xff;

	EXPECT_EQ(cpu.H, 0x90 - 1);

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.H) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.H & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(cpu.H, 8));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.H & 0xf) == 0x0));

}

TEST(INTEL8080, MVI_H_D8) { // MVI H, byte

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 1] = 0x99;

	cpu.H = cpu.Memory[cpu.PC + 1];

	cpu.PC++;

	EXPECT_EQ(cpu.H, 0x99);

}

TEST(INTEL8080, DAD_H) { // DAD H => HL = HL + HL

	cpu.H = 0x90;
	cpu.L = 0x03;

	unsigned int pair_HL = (cpu.H << 8) | (cpu.L);

	pair_HL = pair_HL + pair_HL;

	cpu.C = (pair_HL > 0xff);

	cpu.H = (pair_HL & 0xff00) >> 8;

	cpu.L = pair_HL & 0xff;

	EXPECT_EQ(cpu.C, (pair_HL > 0xff));
	EXPECT_EQ(cpu.H, (pair_HL & 0xff00) >> 8);
	EXPECT_EQ(cpu.L, (pair_HL & 0xff));

}

TEST(INTEL8080, LHLD_adr) { // LHLD adr

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 1] = 0x35;
	cpu.Memory[cpu.PC + 2] = 0x99;

	unsigned short address = (cpu.Memory[cpu.PC + 2] << 8) | (cpu.Memory[cpu.PC + 1]);

	cpu.L = cpu.Memory[address];
	cpu.H = cpu.Memory[address + 1];

	cpu.PC += 2;

	EXPECT_EQ(cpu.L, cpu.Memory[address]);
	EXPECT_EQ(cpu.H, cpu.Memory[address + 1]);

}

TEST(INTEL8080, DCX_H) { // DCX H

	cpu.H = 0x80;
	cpu.L = 0x79;

	unsigned short pair_HL = (cpu.H << 8) | (cpu.L);

	pair_HL -= 1;

	cpu.H = (pair_HL & 0xff00) >> 8;
	cpu.L = (pair_HL & 0xff);

	EXPECT_EQ(cpu.H, (pair_HL & 0xff00) >> 8);
	EXPECT_EQ(cpu.L, (pair_HL & 0xff));

}

TEST(INTEL8080, INR_L) { // INR L

	cpu.L = 0x50;

	cpu.L += 1;

	if (((unsigned short)cpu.L & 0xff) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((unsigned short)cpu.L & 0x80)) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.L, 8);

	cpu.A = (unsigned short)cpu.L & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.L) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.L & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(cpu.L, 8));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.L & 0xf) == 0xf));

}

TEST(INTEL8080, DCR_L) { // DCR L

	cpu.L = 0x80;

	cpu.L = cpu.L - 1;
	cpu.FLAGS.Z = ((cpu.L) == 0);
	cpu.FLAGS.S = ((cpu.L & 0x80) == 0x80);
	cpu.FLAGS.P = cpu.Parity(cpu.L, 8);
	cpu.FLAGS.A = ((cpu.L & 0xf) == 0x0);

	EXPECT_EQ(cpu.FLAGS.Z, ((cpu.L) == 0));
	EXPECT_EQ(cpu.FLAGS.S, ((cpu.L & 0x80) == 0x80));
	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(cpu.L, 8));
	EXPECT_EQ(cpu.FLAGS.A, ((cpu.L & 0xf) == 0x0));

}

TEST(INTEL8080, MVI_L_D8) { // MVI L, D8, Instruction length: 2 bytes

	cpu.PC = 0x0000;

	cpu.Memory[cpu.PC + 1] = 0x99;

	cpu.L = cpu.Memory[cpu.PC + 1];

	cpu.PC++;

	EXPECT_EQ(cpu.L, 0x99);
	
}

TEST(INTEL8080, CMA_NOT) { // CMA NOT

	cpu.A = 0x80;

	cpu.A = (unsigned char)(~cpu.A);

	EXPECT_EQ(cpu.A, (unsigned char)(~(0x80)));

}

TEST(INTEL8080, ANI) { // ANI
	
	cpu.PC = 0x0000;
	cpu.Memory[cpu.PC + 1] = 0x10;

	cpu.A = 0x1F;

	unsigned char x = cpu.A & cpu.Memory[cpu.PC + 1];

	cpu.FLAGS.Z = (x == 0);
	cpu.FLAGS.S = (0x80 == (x & 0x80));

	cpu.FLAGS.P = cpu.Parity(x, 8);

	cpu.FLAGS.C = 0;
	cpu.FLAGS.A = 0;

	cpu.A = x;

	cpu.PC++;

	EXPECT_EQ(x, 0x10 & 0x1F); // 00011100 & 00011101
	                           // = 000111000

	EXPECT_EQ(cpu.FLAGS.Z, (x == 0));

	EXPECT_EQ(cpu.FLAGS.S, (0x80 == (x & 0x80)));

	EXPECT_EQ(cpu.FLAGS.P, cpu.Parity(x, 8));

	EXPECT_EQ(cpu.FLAGS.A, 0);

	EXPECT_EQ(cpu.A, x);

}       

TEST(INTEL8080, LXI_SP_D16) { // LXI SP D16

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 1] = 0x31;
	cpu.Memory[cpu.PC + 2] = 0x1E;

	cpu.SP = (cpu.Memory[cpu.PC + 2] << 8) | (cpu.Memory[cpu.PC + 1]);
	        
	cpu.PC += 2;

	EXPECT_EQ(cpu.SP, (0x1E << 8) | (0x31));

}

TEST(INTEL8080, STA_adr) { // STA

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 1] = 0x1F;
	cpu.Memory[cpu.PC + 2] = 0x23;

	unsigned short adr = (cpu.Memory[cpu.PC + 2] << 8) | (cpu.Memory[cpu.PC + 1]);

	cpu.Memory[adr] = cpu.A;

	cpu.PC += 2;

	EXPECT_EQ(cpu.Memory[adr], cpu.A);

}

TEST(INTEL8080, INX_SP) { // INX SP

	cpu.SP = 0x0000;

	cpu.SP++;

	EXPECT_EQ(cpu.SP, 0x0001);

}

TEST(INTEL8080, INR_M) { // INR M

	cpu.H = 0x20;
	cpu.L = 0x15;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = (unsigned char)(cpu.Memory[offset] + 1);

	if ((cpu.Memory[offset]) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((cpu.Memory[offset]) & 0x80) == 0x80) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.Memory[offset], 8);

	cpu.A = ((cpu.Memory[offset] & 0xf) == 0xf);

	EXPECT_EQ(cpu.Memory[offset], 0x01);
	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.P, 0x00);
	EXPECT_EQ(cpu.FLAGS.A, 0x00);
	
}

TEST(INTEL8080, DCR_M) { // DCR M

	cpu.H = 0x22;
	cpu.L = 0x37;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = 0x04;

	cpu.Memory[offset] = (unsigned char)(cpu.Memory[offset] - 1);

	if ((cpu.Memory[offset]) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if (((cpu.Memory[offset]) & 0x80) == 0x80) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.Memory[offset], 8);

	cpu.A = ((cpu.Memory[offset] & 0xf) == 0xf);

	EXPECT_EQ(cpu.Memory[offset], 0x03);
	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.P, 0x01);
	EXPECT_EQ(cpu.FLAGS.A, 0x00);

}

TEST(INTEL8080, MVI_M_D8) { // MVI M D8, instruction length: 2 bytes


	cpu.H = 0x08;
	cpu.L = 0x35;

	cpu.PC = 0x00;
	cpu.Memory[cpu.PC + 1] = 0x11;

	unsigned char offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.Memory[cpu.PC + 1];

	cpu.PC++;

	EXPECT_EQ(cpu.Memory[offset], 0x11);

}

TEST(INTEL8080, STC) { // STC, Set carry flag = 1

	cpu.FLAGS.C = 1;

	EXPECT_EQ(cpu.FLAGS.C, 1);

}

TEST(INTEL8080, DAD_SP) { // DAD SP HL = HL + SP, set carry flag

	cpu.H = 0x03;
	cpu.L = 0x43;


	unsigned int pair_HL = (cpu.H << 8) | (cpu.L);

	pair_HL = pair_HL + (unsigned int)(cpu.SP);

	cpu.FLAGS.C = (pair_HL > 0xff);

	cpu.H = (pair_HL & 0xff00) >> 8;

	cpu.L = pair_HL & 0xff;

	EXPECT_EQ(cpu.FLAGS.C, 1);
	EXPECT_EQ(cpu.H, 0x03);
	EXPECT_EQ(cpu.L, 0x44);



}

TEST(INTEL8080, LDA_adr) { // LDA adr, instruction length: 3 bytes

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 2] = 0x25;
	cpu.Memory[cpu.PC + 1] = 0x04;

	unsigned short adr = (cpu.Memory[cpu.PC + 2] << 8) | (cpu.Memory[cpu.PC + 1]);

	cpu.A = cpu.Memory[adr];

	cpu.PC += 2;

	EXPECT_EQ(cpu.A, cpu.Memory[adr]);

}

TEST(INTEL8080, DCX_SP) { // DCX SP
	
	cpu.SP = 0x0004;

	cpu.SP--;

	cpu.B = (cpu.SP & 0xff00) >> 8;
	cpu.L = cpu.SP & 0xff;

	EXPECT_EQ(cpu.SP, 0x0003);
	EXPECT_EQ(cpu.B, (cpu.SP & 0xff00) >> 8);
	EXPECT_EQ(cpu.L, cpu.SP & 0xff);

}

TEST(INTEL8080, INR_A) { // INR A, set flags

	cpu.A = 0x04;

	cpu.A += 1;

	if ((cpu.A) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if ((cpu.A & 0x80) == 0x80) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.A, 8);

	cpu.FLAGS.A = ((cpu.A & 0xf) == 0xf);

	EXPECT_EQ(cpu.A, 0x04 + 1);
	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.A, 0);

}

TEST(INTEL8080, DCR_A) { // DCR A, set flags

	cpu.A = 0x04;

	cpu.A -= 1;

	if ((cpu.A) == 0) cpu.FLAGS.Z = 1;

	else cpu.FLAGS.Z = 0;

	if ((cpu.A & 0x80) == 0x80) cpu.FLAGS.S = 1;

	else cpu.FLAGS.S = 0;

	cpu.FLAGS.P = cpu.Parity(cpu.A, 8);

	cpu.FLAGS.A = ((cpu.A & 0xf) == 0xf);

	EXPECT_EQ(cpu.A, 0x04 - 1);
	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.A, 0);

}

TEST(INTEL8080, MVI_A_D8) { // MVI_A_D8, instruction length: 2 bytes

	cpu.PC = 0x00;

	cpu.Memory[cpu.PC + 1] = 0x56;

	cpu.A = cpu.Memory[cpu.PC + 1];

	cpu.PC++;

	EXPECT_EQ(cpu.A, 0x56);

}

TEST(INTEL8080, CMC) { // CMC, set carry flag

	cpu.FLAGS.C = 0x05;

	unsigned char temp = !(cpu.FLAGS.C);


	cpu.FLAGS.C = temp;


	EXPECT_EQ(cpu.FLAGS.C, temp);

}

TEST(INTEL8080, MOV_B_B) { // MOV B, B

	unsigned char temp = cpu.B;
	cpu.B = temp;


	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_B_C) { // MOV B, C

	unsigned char temp = cpu.C;
	cpu.B = temp;


	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_B_D) { // MOV B, D

	unsigned char temp = cpu.D;
	cpu.B = temp;

	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_B_E) { // MOV B, E

	unsigned char temp = cpu.E;
	cpu.B = temp;

	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_B_H) { // MOV B, H

	unsigned char temp = cpu.H;
	cpu.B = temp;


	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_B_L) { // MOV B, L

	unsigned char temp = cpu.L;
	cpu.B = temp;


	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_B_HL) { // MOV B, HL - Store memory at location pair HL into register B

	unsigned short pair_HL = (cpu.H >> 8) | (cpu.L);

	cpu.B = cpu.Memory[pair_HL];

	EXPECT_EQ(cpu.B, cpu.Memory[pair_HL]);

}

TEST(INTEL8080, MOV_B_A) { // MOV B, A

	unsigned char temp = cpu.A;
	cpu.B = temp;


	EXPECT_EQ(cpu.B, temp);

}

TEST(INTEL8080, MOV_C_B) { // MOV C, B

	unsigned char temp = cpu.B;
	cpu.C = temp;


	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_C_C) { // MOV C, C

	unsigned char temp = cpu.C;
	cpu.C = temp;


	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_C_D) { // MOV C, D

	unsigned char temp = cpu.D;
	cpu.C = temp;


	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_C_E) { // MOV C, E

	unsigned char temp = cpu.E;
	cpu.C = temp;

	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_C_H) { // MOV C, H

	unsigned char temp = cpu.H;
	cpu.C = temp;

	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_C_L) { // MOV C, L

	unsigned char temp = cpu.L;
	cpu.C = temp;

	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_C_HL) { // MOV C, HL - Store memory at location pair HL into register C

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.C = cpu.Memory[offset];

	EXPECT_EQ(cpu.C, cpu.Memory[offset]);

}

TEST(INTEL8080, MOV_C_A) { // MOV C, A

	unsigned char temp = cpu.A;
	cpu.C = temp;

	EXPECT_EQ(cpu.C, temp);

}

TEST(INTEL8080, MOV_D_B) { // MOV D, B

	unsigned char temp = cpu.B;
	cpu.D = temp;

	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_D_C) { // MOV D, C

	unsigned char temp = cpu.C;
	cpu.D = temp;


	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_D_D) { // MOV D, D

	unsigned char temp = cpu.D;
	cpu.D = temp;


	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_D_E) { // MOV D, E

	unsigned char temp = cpu.E;
	cpu.D = temp;

	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_D_H) { // MOV D, H

	unsigned char temp = cpu.H;
	cpu.D = temp;

	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_D_L) { // MOV D, L

	unsigned char temp = cpu.L;
	cpu.D = temp;

	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_D_HL) { // MOV D, HL - Store memory at location pair HL into register D

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.D = cpu.Memory[offset];

	EXPECT_EQ(cpu.D, cpu.Memory[offset]);

}

TEST(INTEL8080, MOV_D_A) { // MOV D, A

	unsigned char temp = cpu.A;
	cpu.D = temp;

	EXPECT_EQ(cpu.D, temp);

}

TEST(INTEL8080, MOV_E_B) { // MOV E, B

	unsigned char temp = cpu.B;
	cpu.E = temp;

	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_E_C) { // MOV E, C

	unsigned char temp = cpu.C;
	cpu.E = temp;


	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_E_D) { // MOV E, D

	unsigned char temp = cpu.D;
	cpu.E = temp;


	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_E_E) { // MOV E, E

	unsigned char temp = cpu.E;
	cpu.E = temp;

	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_E_H) { // MOV E, H

	unsigned char temp = cpu.H;
	cpu.E = temp;

	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_E_L) { // MOV E, L

	unsigned char temp = cpu.L;
	cpu.E = temp;

	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_E_HL) { // MOV E, HL - Store memory at location pair HL into register E

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.E = cpu.Memory[offset];

	EXPECT_EQ(cpu.E, cpu.Memory[offset]);

}

TEST(INTEL8080, MOV_E_A) { // MOV E, A

	unsigned char temp = cpu.A;
	cpu.E = temp;

	EXPECT_EQ(cpu.E, temp);

}

TEST(INTEL8080, MOV_H_B) { // MOV H, B

	unsigned char temp = cpu.B;
	cpu.H = temp;

	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_H_C) { // MOV H, C

	unsigned char temp = cpu.C;
	cpu.H = temp;


	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_H_D) { // MOV H, D

	unsigned char temp = cpu.D;
	cpu.H = temp;


	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_H_E) { // MOV H, E

	unsigned char temp = cpu.E;
	cpu.H = temp;

	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_H_H) { // MOV H, H

	unsigned char temp = cpu.H;
	cpu.H = temp;

	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_H_L) { // MOV H, L

	unsigned char temp = cpu.L;
	cpu.H = temp;

	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_H_HL) { // MOV H, HL - Store memory at location pair HL into register H

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.H = cpu.Memory[offset];

	EXPECT_EQ(cpu.H, cpu.Memory[offset]);

}

TEST(INTEL8080, MOV_H_A) { // MOV H, A

	unsigned char temp = cpu.A;
	cpu.H = temp;

	EXPECT_EQ(cpu.H, temp);

}

TEST(INTEL8080, MOV_L_B) { // MOV L, B

	unsigned char temp = cpu.B;
	cpu.L = temp;

	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_L_C) { // MOV L, C

	unsigned char temp = cpu.C;
	cpu.L = temp;


	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_L_D) { // MOV L, D

	unsigned char temp = cpu.D;
	cpu.L = temp;


	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_L_E) { // MOV L, E

	unsigned char temp = cpu.E;
	cpu.L = temp;

	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_L_H) { // MOV L, H

	unsigned char temp = cpu.H;
	cpu.L = temp;

	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_L_L) { // MOV L, L

	unsigned char temp = cpu.L;
	cpu.L = temp;

	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_L_HL) { // MOV L, HL - Store memory at location pair HL into register L

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.L = cpu.Memory[offset];

	EXPECT_EQ(cpu.L, cpu.Memory[offset]);

}

TEST(INTEL8080, MOV_L_A) { // MOV L, A

	unsigned char temp = cpu.A;
	cpu.L = temp;

	EXPECT_EQ(cpu.L, temp);

}

TEST(INTEL8080, MOV_M_B) { // MOV M, B - Set Memory at register pair HL equal to register B

	cpu.H = 0x05;
	cpu.L = 0x34;
	cpu.B = 0x40;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.B;

	EXPECT_EQ(cpu.Memory[offset], cpu.B);

}

TEST(INTEL8080, MOV_M_C) { // MOV M, C - Set Memory at register pair HL equal to register C

	cpu.H = 0x15;
	cpu.L = 0x20;
	cpu.C = 0x11;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.C;

	EXPECT_EQ(cpu.Memory[offset], cpu.C);

}

TEST(INTEL8080, MOV_M_D) { // MOV M, C - Set Memory at register pair HL equal to register D

	cpu.H = 0x23;
	cpu.L = 0x11;
	cpu.D = 0x42;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.D;

	EXPECT_EQ(cpu.Memory[offset], cpu.D);

}

TEST(INTEL8080, MOV_M_E) { // MOV M, E - Set Memory at register pair HL equal to register E

	cpu.H = 0x50;
	cpu.L = 0x14;
	cpu.E = 0x00;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.E;

	EXPECT_EQ(cpu.Memory[offset], cpu.E);

}

TEST(INTEL8080, MOV_M_H) { // MOV M, H - Set Memory at register pair HL equal to register H

	cpu.H = 0x02;
	cpu.L = 0x05;
	cpu.H = 0x12;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.H;

	EXPECT_EQ(cpu.Memory[offset], cpu.H);

}

TEST(INTEL8080, MOV_M_L) { // MOV M, L - Set Memory at register pair HL equal to register L

	cpu.H = 0x33;
	cpu.L = 0x15;
	cpu.L = 0x01;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.L;

	EXPECT_EQ(cpu.Memory[offset], cpu.L);

}

TEST(INTEL8080, MOV_M_A) { // MOV M, A - Set Memory at register pair HL equal to register A

	cpu.H = 0x33;
	cpu.L = 0x01;
	cpu.A = 0x15;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.Memory[offset] = cpu.A;

	EXPECT_EQ(cpu.Memory[offset], cpu.A);

}

TEST(INTEL8080, MOV_A_B) { // MOV A, B

	cpu.B = 0x15;

	unsigned char temp = cpu.B;
	cpu.A = temp;

	EXPECT_EQ(cpu.A, temp);

}

TEST(INTEL8080, MOV_A_C) { // MOV A, C

	cpu.C = 0x14;

	unsigned char temp = cpu.C;
	cpu.A = temp;


	EXPECT_EQ(cpu.A, temp);

}

TEST(INTEL8080, MOV_A_D) { // MOV A, D

	cpu.D = 0x13;

	unsigned char temp = cpu.D;
	cpu.A = temp;


	EXPECT_EQ(cpu.A, temp);

}

TEST(INTEL8080, MOV_A_E) { // MOV A, E

	cpu.E = 0x12;

	unsigned char temp = cpu.E;
	cpu.A = temp;

	EXPECT_EQ(cpu.A, temp);

}

TEST(INTEL8080, MOV_A_H) { // MOV A, H

	cpu.H = 0x11;

	unsigned char temp = cpu.H;
	cpu.A = temp;

	EXPECT_EQ(cpu.A, temp);

}

TEST(INTEL8080, MOV_A_L) { // MOV A, L

	cpu.L = 0x06;

	unsigned char temp = cpu.L;
	cpu.A = temp;

	EXPECT_EQ(cpu.A, 0x06);

}

TEST(INTEL8080, MOV_A_HL) { // MOV A, HL - Store memory at location pair HL into register A

	cpu.H = 0x09;
	cpu.L = 0x10;

	unsigned short offset = (cpu.H << 8) | (cpu.L);

	cpu.A = cpu.Memory[offset];

	EXPECT_EQ(cpu.A, cpu.Memory[offset]);

}

TEST(INTEL8080, MOV_A_A) { // MOV A, A

	cpu.A = 0x08;

	unsigned char temp = cpu.A;
	cpu.A = temp;

	EXPECT_EQ(cpu.A, 0x08);

}

TEST(INTEL8080, ADD_B) { // ADD B

	cpu.A = 0x09;
	cpu.B = 0x34;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.B;
	
	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);
	EXPECT_EQ(cpu.A, 0x0000003d);

}

TEST(INTEL8080, ADD_C) { // ADD C

	cpu.A = 0x08;
	cpu.C = 0x15;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.C;

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xff);

}

TEST(INTEL8080, ADD_D) { // ADD D

	cpu.A = 0x23;
	cpu.D = 0x11;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.D;

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xff);

}

TEST(INTEL8080, ADD_E) { // ADD E

	cpu.A = 0x13;
	cpu.E = 0x57;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.E;

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);
	EXPECT_EQ(cpu.A, result & 0xff);

}

TEST(INTEL8080, ADD_H) { // ADD H

	cpu.A = 0x40;
	cpu.H = 0x09;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.H;

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xff);

}

TEST(INTEL8080, ADD_L) { // ADD L

	cpu.A = 0x32;
	cpu.L = 0x62;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.L;

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xff);

}

TEST(INTEL8080, ADD_M) { // ADD M

	cpu.A = 0x32;
	
	cpu.H = 0x50;
	cpu.L = 0x32;

	unsigned short pair_HL = (cpu.H << 8) | (cpu.L);

	unsigned short result = ((unsigned short)cpu.A + cpu.Memory[pair_HL]);

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, ((unsigned short)cpu.A + cpu.Memory[pair_HL]) & 0xFF);

}

TEST(INTEL8080, ADD_A) { // ADD A

	cpu.A = 0x32;

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.A;

	cpu.SetArithmeticFlags(result);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_B) { // ADC B - 0x88

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.B + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.B & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_C) { // ADC C

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.C + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.C & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_D) { // ADC D

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.D + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.D & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_E) { // ADC E

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.E + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.E & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 1);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_H) { // ADC H

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.H + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.H & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_L) { // ADC L

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.L + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.L & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_M) { // ADC M

	unsigned short pair_HL = (cpu.H << 8) | (cpu.L);

	unsigned short result = (unsigned short)cpu.A + cpu.Memory[pair_HL] + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.Memory[pair_HL] & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, ADC_A) { // ADC A

	unsigned short result = (unsigned short)cpu.A + (unsigned short)cpu.A + cpu.FLAGS.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (cpu.A & 0xf) + cpu.FLAGS.C) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 1);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_B) { // SUB B

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.B;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.B) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 1);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_C) { // SUB C

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.C;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.C) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_D) { // SUB D

	cpu.A = 0x23;
	cpu.D = 0x11;

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.D;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.D) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_E) { // SUB E

	cpu.A = 0xff;
	cpu.E = 0x04;

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.E;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.E) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 1);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_H) { // SUB H

	cpu.A = 0x40;
	cpu.H = 0x09;

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.H;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.H) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_L) { // SUB L

	cpu.A = 0x20;
	cpu.L = 0x01;

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.L;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.L) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_M) { // SUB M

	cpu.H = 0x02;
	cpu.L = 0x30;
	cpu.A = 0x12;

	unsigned short pair_HL = (cpu.H << 8) | (cpu.L);

	unsigned short result = (unsigned short)cpu.A - cpu.Memory[pair_HL];

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.Memory[result]) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 0);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}

TEST(INTEL8080, SUB_A) { // SUB A

	cpu.A = 0x50;

	unsigned short result = (unsigned short)cpu.A - (unsigned short)cpu.A;

	cpu.SetArithmeticFlags(result);

	// Set aux carry flag

	cpu.FLAGS.A = (((cpu.A & 0xf) + (-(cpu.A) & 0xf)) > 0xf);

	cpu.A = result & 0xff;

	EXPECT_EQ(cpu.FLAGS.Z, 1);
	EXPECT_EQ(cpu.FLAGS.S, 0);
	EXPECT_EQ(cpu.FLAGS.C, 0);

	EXPECT_EQ(cpu.A, result & 0xFF);

}
