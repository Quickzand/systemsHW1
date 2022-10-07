// Matthew Sand
// Seth Lenhof
// Iain Crowe

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

int base(int *stack, int BP, int L);
void print_instruction(int PC, instruction IR);
void print_stack(int PC, int BP, int SP, int *stack);
void execute(int trace_flag, instruction *code);
/*
typedef struct instruction {
	int op;
	int l;
	int m;
} instruction;
*/

void execute(int trace_flag, instruction *code)
{
	// CPU Registers initialization
	int BP = 0;
	int SP = -1;
	int PC = 0;
	int halt_flag = 0;
	instruction IR;

	// Stack initialization
	int *stack = malloc(sizeof(int) * ARRAY_SIZE);

	if (trace_flag)
	{
		printf("VM Execution:\n");
		printf("\t\t\t\tPC\tBP\tSP\tstack\n");
		printf("Initial Values:\t\t\t0\t0\t-1\n");
	}

	while (!halt_flag)
	{

		// Fetch Cycle 🐶 woof!
		IR = code[PC++];
		// Print Cycle 🖨

		// Execute Cycle 😵🔫
		// In the Execute cycle, the instruction placed in IR is executed by the VM-CPU. The op-code (OP) component that is stored in the IR register (IR.op) indicates the operation to be executed. For example, if IR.OP is the instruction JPC (IR.op = 8), then the machine will check the value at the top of the stack. If it’s equal to 0, then PC will be set to the M value of the instruction (PC !IR.m). This uses up the value at the top of the stack, so SP must be decremented (SP!SP - 1).
		switch (IR.op)
		{
		case 1: // LIT
			stack[++SP] = IR.m;
			break;
		case 2: // OPR
			switch (IR.m)
			{
			case 1: // ADD
				SP--;
				stack[SP] += stack[SP + 1];
				break;
			case 2: // SUB
				SP--;
				stack[SP] -= stack[SP + 1];
				break;
			case 3: // MUL
				SP--;
				stack[SP] *= stack[SP + 1];
				break;
			case 4: // DIV
				SP--;
				stack[SP] /= stack[SP + 1];
				break;
			case 5: // EQL
				SP--;
				stack[SP] = stack[SP] == stack[SP + 1];
				break;
			case 6: // NEQ
				SP--;
				stack[SP] = stack[SP] != stack[SP + 1];
				break;
			case 7: // LSS
				SP--;
				stack[SP] = stack[SP] < stack[SP + 1];
				break;
			case 8: // LEQ
				SP--;
				stack[SP] = stack[SP] <= stack[SP + 1];
				break;
			case 9: // GTR
				SP--;
				stack[SP] = stack[SP] > stack[SP + 1];
				break;
			case 10: // GEQ
				SP--;
				stack[SP] = stack[SP] >= stack[SP + 1];
				break;
			}
			break;
		case 3: // LOD
			SP++;
			stack[SP] = stack[base(stack, BP, IR.l) + IR.m];
			break;
		case 4: // STO
			stack[base(stack, BP, IR.l) + IR.m] = stack[SP];
			SP--;
			break;
		case 5:									   // CAL
			stack[SP + 1] = base(stack, BP, IR.l); // static link (SL)
			stack[SP + 2] = BP;					   // dynamic link (DL)
			stack[SP + 3] = PC;					   // return address (RA)
			BP = SP + 1;
			PC = IR.m;
			break;
		case 6: // RTN
			SP = BP - 1;
			BP = stack[SP + 2];
			PC = stack[SP + 3];
			break;
		case 7: // INC
			SP += IR.m;
			break;
		case 8: // JMP
			PC = IR.m;
			break;
		case 9: // JPC
			if (stack[SP] == 0)
				PC = IR.m;
			SP--;
			break;
		case 10: // SYS
			switch (IR.m)
			{
			case 1: // WRT
				printf("\nOutput : %d", stack[SP]);
				printf("\n\t\t\t\t");
				SP--;
				break;
			case 2: // RED
				SP++;
				printf("\nInput : ");
				scanf("%d", &stack[SP]);
				break;
			case 3: // HLT
				halt_flag = 1;
				break;
			}
			break;
		}
		if (trace_flag)
		{
			print_instruction(PC, IR);
			print_stack(PC, BP, SP, stack);
		}
	}
}

int base(int *stack, int BP, int L)
{
	while (L > 0)
	{
		BP = stack[BP];
		L--;
	}
	return BP;
}

void print_stack(int PC, int BP, int SP, int *stack)
{
	int i;
	printf("%d\t%d\t%d\t", PC, BP, SP);
	for (i = 0; i <= SP; i++)
		printf("%d ", stack[i]);
	printf("\n");
}

void print_instruction(int PC, instruction IR)
{
	char opname[4];

	switch (IR.op)
	{
	case LIT:
		strcpy(opname, "LIT");
		break;
	case OPR:
		switch (IR.m)
		{
		case ADD:
			strcpy(opname, "ADD");
			break;
		case SUB:
			strcpy(opname, "SUB");
			break;
		case MUL:
			strcpy(opname, "MUL");
			break;
		case DIV:
			strcpy(opname, "DIV");
			break;
		case EQL:
			strcpy(opname, "EQL");
			break;
		case NEQ:
			strcpy(opname, "NEQ");
			break;
		case LSS:
			strcpy(opname, "LSS");
			break;
		case LEQ:
			strcpy(opname, "LEQ");
			break;
		case GTR:
			strcpy(opname, "GTR");
			break;
		case GEQ:
			strcpy(opname, "GEQ");
			break;
		default:
			strcpy(opname, "err");
			break;
		}
		break;
	case LOD:
		strcpy(opname, "LOD");
		break;
	case STO:
		strcpy(opname, "STO");
		break;
	case CAL:
		strcpy(opname, "CAL");
		break;
	case RTN:
		strcpy(opname, "RTN");
		break;
	case INC:
		strcpy(opname, "INC");
		break;
	case JMP:
		strcpy(opname, "JMP");
		break;
	case JPC:
		strcpy(opname, "JPC");
		break;
	case SYS:
		switch (IR.m)
		{
		case WRT:
			strcpy(opname, "WRT");
			break;
		case RED:
			strcpy(opname, "RED");
			break;
		case HLT:
			strcpy(opname, "HLT");
			break;
		default:
			strcpy(opname, "err");
			break;
		}
		break;
	default:
		strcpy(opname, "err");
		break;
	}

	printf("%d\t%s\t%d\t%d\t", PC - 1, opname, IR.l, IR.m);
}