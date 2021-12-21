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