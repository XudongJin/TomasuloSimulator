/*
 * Configure.h
 *
 *  Created on: Apr 21, 2015
 *      Author: Xudong Jin
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_


#include <queue>
#include <vector>
#include <map>
#include <list>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <stdio.h>

using namespace std;

#define REGISTER int32
#define TAG int32
#define R_INSTRUCTION 8
#define J_INSTRUCTION 6
#define I_INSTRUCTION 9

#define REGISTER_NUM 16
#define int32 int
#define INVALID -999
#define ISSUED 0
#define STRUCT_STALL -1
#define DATA_STALL -2
#define READY 1
#define EXECUTING 2
#define EXECUTED 3
#define WRITEBACK 4
#define END 5


#define FP_MUL  0
#define FP_ADD 1
#define INT_UNIT 2
#define MEM 3

//#define LOAD 2
#define STORE 3

#define FPADD_CYCLE  3
#define FPSUB_CYCLE  3
#define FPMULT_CYCLE 5
#define FPDIV_CYCLE  8
#define ADD_CYCLE    1
#define SUB_CYCLE    1
#define LOAD_CYCLE   1
#define MOV_CYCLE    1
#define STR_CYCLE    3
#define BRANCH_CYCLE 1

//#define FPADD_CYCLE  3
//#define FPSUB_CYCLE  3
//#define FPMULT_CYCLE 5
//#define FPDIV_CYCLE  8
//#define ADD_CYCLE    1
//#define SUB_CYCLE    1
//#define LOAD_CYCLE   1
//#define MOV_CYCLE    1
//#define STR_CYCLE    3
//#define BRANCH_CYCLE 1

#define SIZE_INSMEM 256
#define SIZE_DATAMEM 1024
#define NUM_FPADDER 2
#define NUM_FPMUL 2
#define NUM_INTER 2
#define NUM_LDBUFFER 2
#define NUM_ROB 20



#define DEGUG
#define FORMAT setiosflags(ios::fixed)<<setiosflags(ios::right)<<setprecision(3)<<setw(15)

#define fatal(fmt, ...) do {fprintf(stderr, "[FATAL] (%s:%d) " fmt, __FILE__, __LINE__, ##__VA_ARGS__); exit(-1); } while (0)
#define perr(fmt, ...)  do {fprintf(stderr, "[ERROR] (%s:%d) " fmt, __FILE__, __LINE__, ##__VA_ARGS__); } while (0)
#define pwarn(fmt, ...) do {fprintf(stderr,  "[WARN] (%s:%d) " fmt, __FILE__, __LINE__, ##__VA_ARGS__); } while (0)
#define pinfo(fmt, ...) do {fprintf(stderr,  "[INFO]  " ##__VA_ARGS__); } while (0)

typedef struct{
	unsigned op:6;
	unsigned rs:5;
	unsigned rt:5;
	unsigned immediate:16;
}I_Instruction;

typedef struct {
	unsigned op:6;
	unsigned rs:5;
	unsigned rt:5;
	unsigned rd:5;
	unsigned shamt:5;
	unsigned funct:6;
}R_Instruction;

typedef struct{
	unsigned op:6;
	unsigned offset:26;
}J_Instruction;

typedef union {

	I_Instruction type_i;
	R_Instruction type_r;
	J_Instruction type_j;
}Instruction_32;






#endif /* CONFIGURE_H_ */
