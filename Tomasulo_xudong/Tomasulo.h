/*
 * Tomasulo.h
 *
 *  Created on: Apr 18, 2015
 *  Author: Xudong Jin
 */

#ifndef TOMASULO_H_
#define TOMASULO_H_


#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Configure.h"
#include "Functionunit.h"
#include "Instruction.h"
#include "Predictor.h"


using namespace std;


class entry;
class LB_entry;


typedef int  Registerstat[REGISTER_NUM];
typedef vector<entry> Reservation_station;
typedef vector<entry> Load_buffer;

typedef struct {
	int reorder;
	bool busy;
}Registerstat_rob;




class entry {
public:
	string op;
	float Vj,Vk;
	int Qj,Qk;
	int type;
	Instruction * ins;
	FunctionUnit * funit;
	bool busy;
	int dest;
	entry(FunctionUnit * fu=NULL):funit(fu){
		dest=Qj=Qk=INVALID;
		Vj=Vk=INVALID;
		busy=false;op='\0';ins=NULL;
	    type=fu->type;
	}
};



struct ROB_entry{
	string op;
	string type;
	int dest;
	float value;
	bool ready;
	bool busy;
	int tag;
	int address;
	bool taken;
};


typedef vector<struct ROB_entry > ROB;
//struct ROB_entry ROB[5];
//ROB rob;



class CDB
{

public:
	bool busy;



	void BroadcastingRS_withROB(int index, float result, int index_rob,
			Reservation_station & rs_station, ROB & rob) {
		unsigned int i;
		int tag = rs_station[index].dest;
		rs_station[index].busy = false;
		for (i = 0; i < rs_station.size(); ++i) {
			if (rs_station[i].ins != NULL) {
				if (rs_station[i].Qj == tag) {
					rs_station[i].Vj = result;
					rs_station[i].Qj = INVALID;
					if (rs_station[i].Qk == INVALID){
						rs_station[i].ins->state = EXECUTING;
					}

				}
				if (rs_station[i].Qk == tag) {
					rs_station[i].Vk = result;
					rs_station[i].Qk = INVALID;
					if (rs_station[i].Qj == INVALID){
						rs_station[i].ins->state = EXECUTING;
					}
				}
			}
		}
		rob[index_rob].value = result;
		rob[index_rob].ready = true;
		if (rs_station[index].ins->mnemonics == "STR")
			rob[index_rob].dest = (int) rs_station[index].Vk;
		if (rs_station[index].ins->submit_type == "Branch")
			rob[index_rob].taken = rs_station[index].ins->taken;
	}
};


class Tomasulo_Simulator{



public:

	Reservation_station rs_station;
	CDB cdb;
	//Load_buffer ld_buffer;
	Registerstat registerstat;
	ROB rob;
	int tag;
	bool stopflag;
	Predictor * predictor;
	int cycle;




	Registerstat_rob registerstat_rob[REGISTER_NUM];

	bool Init(string predictor_type,int num_adder=NUM_FPADDER,int num_mul=NUM_FPMUL,int num_inter=NUM_INTER,int num_ld=NUM_LDBUFFER){


		//predictor=m_predictor;
		for(int i=0;i<num_mul;++i){
			FunctionUnit * fpmul=new FP_Multipler();
			entry newentry(fpmul);
			rs_station.push_back(newentry);

		}
		for(int i=0;i<num_adder;++i){
			FunctionUnit * fpadd=new FP_Adder();
			entry newentry(fpadd);
			rs_station.push_back(newentry);
		}
		for(int i=0;i<num_inter;++i){
			FunctionUnit * inter=new Interger_unit();
			entry newentry(inter);
			rs_station.push_back(newentry);
		}
		for(int i=0;i<num_ld;++i){
			FunctionUnit * dataMemory=new DataMemory();
			entry newentry(dataMemory);
			rs_station.push_back(newentry);
		}

        for (int i = 0; i < REGISTER_NUM; i++)
        {
        	registerstat[i] = INVALID;
        	registerstat_rob[i].busy=false;
        	registerstat_rob[i].reorder=INVALID;
        }
        tag=0;stopflag=false;
        cycle=0;


        ResultRecord(1,true,true);

        //    BTFNT * predictor= new BTFNT;

    //  string  predictor_type="-2bit";
       	// Predictor * predictor=NULL;
          if (predictor_type = "-2bit")
       	  predictor = new _2bitPredictor;
        //  _2bitPredictor * predictor= new _2bitPredictor();
       	else if (predictor_type = "-1bit")
       	predictor =new _1bitPredictor;
          //    _1bitPredictor * predictor= new _1bitPredictor();
       	else if (predictor_type  = "-BTFNT")
       		predictor =new BTFNT;
           //    BTFNT * predictor= new BTFNT();
       	else
       	{   predictor=NULL;
       		cout<<"No definition for such type of predictor is found ,please choose one from -2bit,-1bit,-BTFNT"<<endl;
       		return false;
       	}

          return false;


	}

	void Clear(){
		for(unsigned i=0;i<rs_station.size();++i){
			delete rs_station[i].funit;
		}
		if(predictor!=NULL)
			delete predictor;
	}

	void Instruction_Fetch(){
		if(pc<insqueue.size()&& ireg==NULL){
			ireg=Instruction_Decoding(insqueue[pc],pc);
			++pc;
		}
		++cycle;
	}

	Instruction * Instruction_Decoding(ParseCode code,int currentpc){
		return decoder.decoding(code,currentpc);
	}



	void Instruction_Issue_withROB(){
		cout<<"Cycle:"<<cycle<<"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
		if(ireg!=NULL){
		if(Issue_withROB(ireg)==true){

			#ifdef DEGUG
			cout<<ireg->address<<":"<<ireg->mnemonics<<" is issued"<<endl;
			#endif
			ireg=NULL;
		}
		else{
			#ifdef DEGUG
				cout<<"[Structure hazard]:"<<ireg->address<<":"<<ireg->mnemonics<<" is stalled"<<endl;
			#endif
			ireg->state=STRUCT_STALL;
			}
		}

	}


	bool Issue_withROB(Instruction * ins) {

		int index=0;
		int tag=0;
		if((index=rs_Isavailable(ins->function_type))>=0 && (tag=rob_Isavailable())>=0)
			return rs_Bookkeeping_withROB(index,tag,ins);
		else
			return false;
	}




	   void Executing(){
		   unsigned int index;
		   bool finish;
		   float loadValue=INVALID;
		   for(index=0;index<rs_station.size();++index){
			   finish=false;
			 if(rs_station[index].ins!=NULL && rs_station[index].ins->state==READY){
				 	    rs_station[index].ins->state=EXECUTING;
			 } else if(rs_station[index].ins!=NULL && rs_station[index].ins->state==EXECUTING){
				if (rs_station[index].ins->mnemonics=="LOAD"){
					if(rs_station[index].ins->memaddr==true){
					int memaddress=(int)rs_station[index].Vk;
				    loadValue=datamemory.readMem(memaddress);
					for(int i=0;i<rob.size();++i){
						if(rob[i].op=="STR" && rob[i].dest==memaddress && rob[i].tag < rs_station[index].dest)
							loadValue=rob[i].value;
					}
					}else
						loadValue=rs_station[index].Vk;
					if(loadValue!=INVALID)
					finish = rs_station[index].ins->Execution(rs_station[index].Vj, loadValue);
				}
				else
					finish = rs_station[index].ins->Execution(
							rs_station[index].Vj, rs_station[index].Vk);
				 if(finish==true){
					    rs_station[index].ins->state=EXECUTED;
				 }
			 } else if (rs_station[index].ins!=NULL && rs_station[index].ins->state==DATA_STALL){

				 cout<<"[Data hazard]:"<<rs_station[index].ins->address<<":"<<rs_station[index].ins->mnemonics<<" is stalled"<<endl;
			 } else if (rs_station[index].ins!=NULL && rs_station[index].ins->state==ISSUED){
				         rs_station[index].ins->state=DATA_STALL;
			 }
		   }
	   }





		 void WriteBack_withROB(){
			 unsigned int index;
			 for(index=0;index<rs_station.size();++index){
				 if(rs_station[index].ins!=NULL && rs_station[index].ins->state==EXECUTED){
					 rs_station[index].ins->state=WRITEBACK;
				 } else if(rs_station[index].ins!=NULL && rs_station[index].ins->state==WRITEBACK)
				 {
				   cout<<rs_station[index].ins->address<<":"<<rs_station[index].ins->mnemonics<<" is writing back !  "<<endl;
				   cdb.BroadcastingRS_withROB(index,rs_station[index].ins->result,findIndex(rs_station[index].dest),rs_station,rob);
				   rs_station[index].ins->state=END;
				   ClearRSEntry(index);
				 }
			 }
		 }






		 void Submit() {
		if (rob.empty() != true) {
			struct ROB_entry first_entry = rob.front();
			if (first_entry.ready == true) {
				int dest = first_entry.dest;
				if (dest != INVALID
						&& registerstat_rob[dest].reorder == first_entry.tag) {
					registerstat_rob[dest].busy = false;
					registerstat_rob[dest].reorder = INVALID;
				}
				if (first_entry.type == "Branch") {
					bool predict_correct = predictor->update(first_entry.taken,
							first_entry.address);
					ResultRecord(first_entry.address + 1, predict_correct,
							false);
					if (first_entry.taken == true) {
						pc = (int) (first_entry.value) + 1;
						cout << first_entry.address << ":" << first_entry.op
								<< " sumbitted ! " << endl;
						Back_out();
					} else {
						cout << first_entry.address << ":" << first_entry.op
								<< " sumbitted ! " << endl;
						rob.erase(rob.begin());
					}
				} else if (first_entry.type == "Memory" && dest != INVALID) {
					datamemory.writeMem(dest, first_entry.value);
					cout << first_entry.address << ":" << first_entry.op
							<< " sumbitted ! " << endl;
					rob.erase(rob.begin());
				} else {
					if (first_entry.type == "Register" && dest != INVALID) {
						registerfile.writeReg(dest, first_entry.value);
						cout << first_entry.address << ":" << first_entry.op
								<< " sumbitted ! " << endl;
						rob.erase(rob.begin());
					}
				}
			}
		}
	}



		 void display_withROB(){
			 unsigned int index;
			 cout<<FORMAT<<"Index"<<FORMAT<<"Type"<<FORMAT<<"Busy"<<FORMAT<<"Operation"<<FORMAT<<"Src1"<<FORMAT<<"Src2(Addr)"<<FORMAT<<"Qj"<<FORMAT<<"Qk"<<FORMAT<<"Tag"<<endl;
			 for(index=0;index<rs_station.size();++index){
			  cout<<FORMAT<<index<<FORMAT<<rs_station[index].type<<FORMAT<<rs_station[index].busy<<FORMAT<<rs_station[index].op<<FORMAT<<rs_station[index].Vj<<FORMAT<<rs_station[index].Vk<<FORMAT<<rs_station[index].Qj<<FORMAT<<rs_station[index].Qk<<FORMAT<<rs_station[index].dest;
			  cout<<endl;
			 }
			  cout<<endl;
			 for(index=0;index<REGISTER_NUM;++index){
			  cout<<FORMAT<<setw(14)<<"R"<<index;
			 }
			  cout<<endl;
			  cout<<"Register status:"<<endl;
			  for(index=0;index<REGISTER_NUM;++index){
			  cout<<FORMAT<<registerstat_rob[index].reorder;
			 }
			  cout<<endl;
			  registerfile.display();
			  cout<<endl;
			  cout<<"Reorder Buffer"<<endl;
				 cout<<FORMAT<<"Index"<<FORMAT<<"Busy"<<FORMAT<<"Type"<<FORMAT<<"Operation"<<FORMAT<<"Destination"<<FORMAT<<"Value"<<FORMAT<<"Ready"<<FORMAT<<"Tag"<<endl;
				 for(index=0;index<rob.size();++index){
				  cout<<FORMAT<<index<<FORMAT<<rob[index].busy<<FORMAT<<rob[index].type<<FORMAT<<rob[index].op<<FORMAT<<rob[index].dest<<FORMAT<<rob[index].value<<FORMAT<<rob[index].ready<<FORMAT<<rob[index].tag;
				  cout<<endl;
				 }

		 }

	bool EndofSimulation() {
		for (unsigned int index = 0; index < rs_station.size(); ++index) {
			if (rs_station[index].busy == true)
				return false;
		}
		if(pc>=insqueue.size() && rob.size() == 0)
			return true;
		else
			return false;
	}


private:

	int rs_Isavailable(int type) {
		unsigned int index=0;
		for(index=0;index<rs_station.size();++index)
		    if(rs_station[index].type==type && rs_station[index].busy==false)
		    	break;
		if (index==rs_station.size()){
			return INVALID;
		}else return index;
	}

	int rob_Isavailable() {
		if(rob.size()<NUM_ROB)
			{++tag;
			return tag;}
		else
		    return INVALID;
	}



void ResultRecord(int address, bool result,bool recording){
	  string CSVfileName="predictResult.csv";
	  std::ofstream out (CSVfileName.c_str (),std::ios::app);
	  if(recording==true)
	  out <<"Address," << "Result," << std::endl;
	  if(recording==false)
      out <<address << ","<<result << std::endl;
	  out.close ();
}



	int findIndex(int tag){
		for(unsigned int i=0;i<rob.size();++i){
			if(rob[i].tag==tag)
				return i;
		}

		cout<<"findIndex error"<<endl;
		return INVALID;
	}


	bool rs_Bookkeeping_withROB(int index, int newtag, Instruction * ins) {
		int state = INVALID;
		int h,tag;

		if (ins->src1 != INVALID) {
			if (registerstat_rob[ins->src1].busy == true) {
				tag = registerstat_rob[ins->src1].reorder;
				h=findIndex(tag);
				if (rob[h].ready == true) {
					rs_station[index].Vj = rob[h].value;
					rs_station[index].Qj = INVALID;
				} else {
					rs_station[index].Vj = INVALID;
					rs_station[index].Qj = tag;
				}
			} else {
					rs_station[index].Vj = registerfile.readReg(ins->src1);
					rs_station[index].Qj = INVALID;
			}
		}

		if (ins->src2 != INVALID) {
			if (ins->imm == true) {
				rs_station[index].Vk = ins->src2;
				rs_station[index].Qk = INVALID;
			} else if (ins->imm == false
					&& registerstat_rob[(int) (ins->src2)].busy == true) {
					tag = registerstat_rob[(int) ins->src2].reorder;
				h=findIndex(tag);
				if (rob[h].ready == true) {
					rs_station[index].Vk = rob[h].value;
					rs_station[index].Qk = INVALID;
				} else {
					rs_station[index].Vk = INVALID;
					rs_station[index].Qk = tag;
				}
			} else {
				if (ins->imm == false
						&& registerstat_rob[(int) (ins->src2)].busy == false) {
					rs_station[index].Vk = registerfile.readReg(ins->src2);
					rs_station[index].Qk = INVALID;
				}
			}
		}

		if (rs_station[index].Qk == INVALID && rs_station[index].Qj == INVALID) {
			state = READY;
		} else
			state = ISSUED;

		rs_station[index].busy = true;
		rs_station[index].dest = newtag;/////add dest field in rs station
		rs_station[index].op = ins->mnemonics;
		rs_station[index].ins = ins;
		rs_station[index].ins->state = state;

		struct ROB_entry newentry;

		newentry.op = ins->mnemonics;
		newentry.busy = true;

		newentry.ready = false;
		newentry.value = INVALID;
		newentry.type= ins->submit_type;
		newentry.tag= newtag;
		newentry.dest = INVALID;
		newentry.address=ins->address;
		newentry.taken=false;

        if(ins->dest!=INVALID){
		registerstat_rob[ins->dest].busy =true;////DIDN'T CONSIDR THE CASE
		registerstat_rob[ins->dest].reorder =newtag;
		newentry.dest = ins->dest;
        }
        if(ins->mnemonics=="STR")
        	newentry.dest = rs_station[index].Vk;
        rob.push_back(newentry);
        return true;
	}

		 void ClearRSEntry(int index){
			 rs_station[index].Qj=rs_station[index].Qk=INVALID;
			 rs_station[index].Vj=rs_station[index].Vk=INVALID;
			 rs_station[index].busy=false;
			 rs_station[index].op='\0';
			 rs_station[index].dest=INVALID;
			 if(rs_station[index].ins!=NULL)
			 delete rs_station[index].ins;
			 rs_station[index].ins=NULL;
         }

		 void Back_out() {
			 for(int i = 0;i<rs_station.size(); ++i)
				 if (rs_station[i].ins != NULL)
					 ClearRSEntry(i);
			 for (int i = 0; i < REGISTER_NUM; i++) {
				 registerstat_rob[i].busy = false;
				 registerstat_rob[i].reorder = INVALID;
			 }
			 rob.clear();
		 }
};




#endif /* TOMASULO_H_ */
