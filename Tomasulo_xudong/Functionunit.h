/*
 * Functionunit.h
 *
 *  Created on: Apr 17, 2015
 *        Author: Xudong Jin
 */

#ifndef FUNCTIONUNIT_H_
#define FUNCTIONUNIT_H_

#include "Configure.h"
#include "Instruction.h"



using namespace std;




typedef struct {
	string name;
	int rs;
	float rt;
	int rd;
	bool rt_imm;
	bool rt_memaddr;

}ParseCode;




typedef Instruction * InstructionRegister;
typedef  vector<ParseCode> Instruction_Queue;
typedef int Instruction_Pointer;


class FunctionUnit
{
public:

	int rt;
	int type;
    virtual int execute(int operand_1, int  operand_2, int operation){
    	cout<<"this is FUnctionUNit"<<endl;
    	return 0;
    }
    virtual ~FunctionUnit (){}
};

/*
FunctionUnit::~FunctionUnit(){
	cout<<"bye"<<endl;
}*/
class FP_Adder :public FunctionUnit
{
public:

	FP_Adder(){
		type=FP_ADD;
	}
	int execute(int operand_1, int  operand_2, int operation){
    	cout<<"this is FP_Adder"<<endl;
    	return  0.0;
	}

	~FP_Adder(){}
};

class FP_Multipler :public FunctionUnit
{
public:
	FP_Multipler(){
		type=FP_MUL;
	}
	int execute(int operand_1, int  operand_2, int operation){cout<<"this is fpmultipler"<<endl;}
};


class Interger_unit :public FunctionUnit
{
public:
	Interger_unit(){
		type=INT_UNIT;
	}
	int execute(int operand_1, int  operand_2, int operation){cout<<"this is Interger_unit"<<endl;}
};

class DataMemory: public FunctionUnit
{
	private:
	float unit[SIZE_DATAMEM];
	public:
	DataMemory(){
		type=MEM;
		for(int i=0;i<SIZE_DATAMEM;++i){
			unit[i]=0.0;
		}
	}
    float readMem(int r_addr){
    	return unit[r_addr];
    }
    void writeMem(int w_addr, float data){
    	unit[w_addr]=data;
    }
    int execute(int operand_1, int  operand_2, int operation){}

    void display(int startindex,int endindex){

      if(startindex<=endindex && startindex>=0 && endindex<=SIZE_DATAMEM){
    	  cout<<"Memory value:"<<endl;
	  for(int index=startindex;index<endindex;++index){
	      cout<<FORMAT<<index<<":"<<readMem(index);
	      if((index-startindex)!=0 && (index-startindex)%16==0)
		   cout<<endl;
	  }
	  cout<<endl;
      }else
    	  cout<<"Memory display error"<<endl;
    }
};



class Registersfile
{
	private:
	float registers[16];
	public:
	Registersfile(){

        for (int i = 0; i < 16; i++)
        {
            registers[i] =0.0;
        }

	}
    float readReg(int r_addr){
    	return registers[r_addr];
    }
    void writeReg(int w_addr, float value){
    	registers[w_addr] = value;
    }

    void display(){
      cout<<"Register value:"<<endl;
	  for(int index=0;index<REGISTER_NUM;++index){
	  cout<<FORMAT<<readReg(index);
	 }
	  cout<<endl;
    }
};

class InstructionMemory
{
	private:
	int unit[SIZE_INSMEM];
	public:
	InstructionMemory(){
		for(int i=0;i<SIZE_INSMEM;++i){
			unit[i]=0;
		}
	}
    int readinstruction(int r_addr);
    void storeinstruction(int w_addr, int instruction);


};





class InstructionDecoder {

public:
	//Instruction_32 decoding(Machinecode machinecode);
	Instruction * decoding(ParseCode tscode,int pc){

			Instruction * inst=NULL;
			if(tscode.name=="FPADD")
			{
				inst=new FPADD(tscode.rd,tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="FPSUB")
			{
				inst=new FPSUB(tscode.rd,tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="FPMULT")
			{
				inst=new FPMULT(tscode.rd,tscode.rs,tscode.rt,tscode.rt_imm,pc);

			}
			if(tscode.name=="FPDIV")
			{
				inst=new FPDIV(tscode.rd,tscode.rs,tscode.rt,tscode.rt_imm,pc);

			}
			if(tscode.name=="ADD")
			{
				inst=new ADD(tscode.rd,tscode.rs,tscode.rt,tscode.rt_imm,pc);

			}
			if(tscode.name=="SUB")
			{
				inst=new SUB(tscode.rd,tscode.rs,tscode.rt,tscode.rt_imm,pc);

			}

			if(tscode.name=="LOAD")
			{
				inst=new LOAD(tscode.rd,tscode.rt,tscode.rt_imm,tscode.rt_memaddr,pc);
			}

			if(tscode.name=="MOV")
			{
				inst=new MOVE(tscode.rd,tscode.rt,tscode.rt_imm,pc);
			}

			if(tscode.name=="STR")
			{
				inst=new STR(tscode.rs,tscode.rt,tscode.rt_imm,tscode.rt_memaddr,pc);
			}
			if(tscode.name=="BR")
			{
				inst=new BR(tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="BGT")
			{
				inst=new BGT(tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="BLT")
			{
				inst=new BLT(tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="BGE")
			{
				inst=new BGE(tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="BLE")
			{
				inst=new BLE(tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}

			if(tscode.name=="BZ")
			{
				inst=new BZ(tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}
			if(tscode.name=="BNEZ")
			{
				inst=new BNEZ(tscode.rs,tscode.rt,tscode.rt_imm,pc);
			}

			if(tscode.name=="HALT")
			{
				inst=new HALT();
			}

			return inst;
		}



};




 extern DataMemory datamemory;
 extern Registersfile registerfile;
 extern Instruction_Queue insqueue;
 extern InstructionDecoder decoder;
 extern InstructionRegister ireg;
 extern Instruction_Pointer pc;





#endif /* FUNCTIONUNIT_H_ */
