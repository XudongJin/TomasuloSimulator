/*
 * Instruction.h
 *
 *  Created on: Apr 21, 2015
 *         Author: Xudong Jin
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "Configure.h"
#include <string.h>
#include "Functionunit.h"
using namespace std;

class Instruction;




class Instruction {

public:
	int cycle;
	int state;
	int function_type;
	int dest;
	int src1;
	float src2;
	bool imm;
	bool memaddr;
	bool taken;
	float result;
	string mnemonics;
	string submit_type;
	int address;
	virtual bool Execution(float operand1,float operand2){}
	virtual ~Instruction(){}

};

class  FPADD :public Instruction
{
public:

	//string mnemonics;

	FPADD(int dest,int operand1,float operand2,bool im,int addr,int cycle=FPADD_CYCLE,int type=FP_ADD)
	{
		state=result=INVALID;
		mnemonics="FPADD";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=operand1;
		this->src2=operand2;
		submit_type="Register";
		imm=im;
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{
		#ifdef DEGUG
		cout<<address<<":FPADD is executing! cycle:  "<<cycle<<endl;
		//pinfo("FPADD is executing! cycle:  %d\n",cycle);
		#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=operand1+operand2;
			return true;
		}
	}

	~FPADD(){}
};

class  FPSUB :public Instruction
{
public:

	//string mnemonics;

	FPSUB(int dest,int operand1,float operand2,bool im,int addr,int cycle=FPSUB_CYCLE,int type=FP_ADD)
	{
		state=result=INVALID;
		mnemonics="FPSUB";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{
		#ifdef DEGUG
		cout<<address<<":FPSUB is executing! cycle:  "<<cycle<<endl;
		#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=operand1-operand2;
			return true;
		}
	}

	~FPSUB(){}
};

class  FPMULT :public Instruction
{
public:
	int totalcycle;

	FPMULT(int dest,int operand1,float operand2,bool im,int addr,int cycle=FPMULT_CYCLE,int type=FP_MUL)
	{
		state=result=INVALID;
		mnemonics="FPMULT";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
		totalcycle=cycle;
	}

	bool Execution(float operand1,float operand2)
	{
		#ifdef DEGUG
		cout<<address<<":FPMULT is executing! cycle:  "<<cycle<<endl;
		#endif
		int dcycle;
		--cycle;
		if(operand1==0||operand1==1||operand1==-1||operand2==0 ||operand2==1||operand2==-1)
			dcycle=totalcycle-1;
		else if (operand1==4||operand2==4)
			dcycle=totalcycle-2;
		else
			dcycle=0;
		 if(cycle!=dcycle){
			return false;
			}
		else{
			result=operand1*operand2;
			return true;
		}
	}
};
class  FPDIV :public Instruction
{
public:

	//string mnemonics;

	FPDIV(int dest,int operand1,float operand2,bool im,int addr,int cycle=FPDIV_CYCLE,int type=FP_MUL)
	{
		state=result=INVALID;
		mnemonics="FPDIV";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{
		#ifdef DEGUG
		cout<<address<<":FPDIV is executing! cycle:  "<<cycle<<endl;
		#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=operand1/operand2;
			return true;
		}
	}
};

class  ADD :public Instruction
{
public:

	//string mnemonics;

	ADD(int dest,int operand1,float operand2,bool im,int addr,int cycle=ADD_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="ADD";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{


#ifdef DEGUG
		cout<<address<<":ADD is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=operand1+operand2;
			return true;
		}
	}
};

class  SUB :public Instruction
{
public:

	//string mnemonics;

	SUB(int dest,int operand1,float operand2,bool im,int addr,int cycle=SUB_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="SUB";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{
#ifdef DEGUG
		cout<<address<<":SUB is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=operand1-operand2;
			return true;
		}
	}
};

class  STR :public Instruction
{
public:

	//string mnemonics;

	STR(int operand1,float operand2,bool im,bool maddr,int addr,int cycle=STR_CYCLE,int type=MEM)
	{
		state=result=INVALID;
		mnemonics="STR";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Memory";
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":STR is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=operand1;
		//	datamemory.writedata(src2,src1);
			return true;
		}
	}
};


class LOAD :public Instruction
{
public:

	//string mnemonics;
	LOAD (int dest,float operand2,bool im,bool maddr,int addr,int cycle=LOAD_CYCLE,int type=MEM)
	{
		state=result=INVALID;
		mnemonics="LOAD";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=INVALID;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
		memaddr=maddr;
	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":LOAD is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{

			result=operand2;
		//	result=datamemory.readdata(src2);

			return true;
		}
	}
};

class MOVE :public Instruction
{
public:

	//string mnemonics;
	MOVE (int dest,float operand2,bool im,int addr,int cycle=MOV_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="MOV";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=dest;
		this->src1=INVALID;
		this->src2=operand2;
		imm=im;
		submit_type="Register";
		address=addr;
	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":MOVE is executing! cycle:  "<<cycle<<endl;
#endif


		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
		//	result=datamemory.readdata(src2);
			result=operand2;
			return true;
		}
	}
};

class  BR :public Instruction
{
public:

	//string mnemonics;
	bool taken;

	BR(float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BR";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=INVALID;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;

	}

	bool Execution(float operand1,float operand2)
	{


#ifdef DEGUG
		cout<<address<<":BR is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			result=address+(int)operand2;
			if(result==address+1)taken=false;
			if(result!=address+1)taken=true;
			return true;
		}
	}
};


class  BGT :public Instruction
{
public:

	//string mnemonics;

	BGT(int operand1,float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BGT";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;

	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":BGT is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			if(operand1>0){
				result=address+(int)operand2;
				taken=true;
			}
			else {
				result=address+1;
				taken=false;

			}
			return true;
		}
	}
};

class  BLT :public Instruction
{
public:

	//string mnemonics;
	BLT(int operand1,float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BLT";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;

	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":BLT is executing! cycle:  "<<cycle<<endl;
#endif


		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			if(operand1<0){
				result=address+(int)operand2;
				taken=true;
			}
			else {
				result=address+1;
				taken=false;
			}
			return true;
		}
	}
};

class  BGE :public Instruction
{
public:

	BGE(int operand1,float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BGE";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;

	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":BGE is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			if(operand1>=0){
				result=address+(int)operand2;
				taken=true;
			}
			else {
				result=address+1;
				taken=false;
			}
			return true;
		}
	}
};

class  BLE :public Instruction
{
public:


	BLE(int operand1,float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BLE";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;

	}

	bool Execution(float operand1,float operand2)
	{
#ifdef DEGUG
		cout<<address<<":BLE is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			if(operand1<=0) {
				result=address+(int)operand2;
				taken=true;
			}
			else{
				result=address+1;
				taken=false;
			}
			return true;
		}
	}
};

class  BZ :public Instruction
{
public:

	BZ(int operand1,float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BZ";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;
	}

	bool Execution(float operand1,float operand2)
	{
#ifdef DEGUG
		cout<<address<<":BZ is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			if(operand1==0) {
				result=address+(int)operand2;
				taken=true;
			}
			else {
				result=address+1;
				taken=false;
			}
			return true;
		}
	}
};

class  BNEZ :public Instruction
{
public:

	BNEZ(int operand1,float operand2,bool im,int addr,int cycle=BRANCH_CYCLE,int type=INT_UNIT)
	{
		state=result=INVALID;
		mnemonics="BNEZ";
		this->cycle=cycle;
		this->function_type=type;
		this->dest=INVALID;
		this->src1=operand1;
		this->src2=operand2;
		imm=im;
		submit_type="Branch";
		address=addr;
		taken=false;
	}

	bool Execution(float operand1,float operand2)
	{

#ifdef DEGUG
		cout<<address<<":BNEZ is executing! cycle:  "<<cycle<<endl;
#endif
		--cycle;
		if(cycle!=0){
			return false;
			}
		else{
			if(operand1!=0){
				result=address+(int)operand2;
				taken=true;
			}
			else {
				result=address+1;
				taken=false;
			}
			return true;
		}
	}
};
class  HALT :public Instruction
{
public:
	HALT()
	{
		state=result=INVALID;
		mnemonics="HALT";
		this->cycle=INVALID;
		this->function_type=INVALID;
		this->dest=INVALID;
		this->src1=INVALID;
		this->src2=INVALID;
		imm=false;
	}

	bool Execution(float operand1,float operand2){ return true;}
};

#endif /* INSTRUCTION_H_ */
