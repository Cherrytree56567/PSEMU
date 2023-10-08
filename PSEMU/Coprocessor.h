/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <fstream>
#include <bitset>

class Coprocessor0 {
public:
    uint regs[64];

		uint r0;
		uint r1;
		uint r2;
		uint Bpc;		/* Breakpoint Program Counter */
		uint r4;
		uint BDA;		/* Breakpoint Data Address */
		uint TAR;		/* Target Address */
		uint DCIC;		/* Debug and Cache Invalidate Control */
		uint BadA;		/* Bad Address */
		uint BDAM;		/* Breakpoint Data Address Mask */
		uint r10;
		uint BpcM;		/* Breakpoint Program Counter Mask */
		Cop0STAT sr;	    /* Status */
		Cop0CAUSE cause;	/* Cause */
		uint epc;		/* Exception Program Counter */
		uint PRId;		/* Processor Revision Identifier */
		uint reserved[32];
};
