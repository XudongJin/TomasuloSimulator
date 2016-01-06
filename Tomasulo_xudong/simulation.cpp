/*
 * simulation.cpp
 *
 *  Created on: Apr 19, 2015
 *         Author: Xudong Jin
 *
 */



#include "Configure.h"
#include "Functionunit.h"
//#include "Memory.h"
#include "Tomasulo.h"
#include "Instruction.h"
#include "Parser.h"


using namespace std;







int main(int argc,char *argv[]){


	char  *file_name= NULL;
    string predictor_type;

	if(argc==3){
	 file_name= argv[2];
	 predictor_type= argv[1];
	} else{
		cout<<"Need two parameter"<<endl;
		cout<<"First one is predictor_type(-BTFNT,-1bit or -2bit) ,second one is path of test file."<<endl;
	    cout<<"For example: ./tomasulo.out -BTFNT t2.txt"<<endl;
	    exit(-1);

	}

    Tomasulo_Simulator tomasulo;
    Parser parser;

	int i=0;

	 if (tomasulo.Init(predictor_type)==false)
		 return -1;


    parser.parse_file(file_name);
    parser.parse_line(insqueue,datamemory);

    datamemory.display(0,30);

	do {
		tomasulo.Instruction_Fetch();
		tomasulo.Instruction_Issue_withROB();
		tomasulo.Executing();
		tomasulo.WriteBack_withROB();
		tomasulo.Submit();
		tomasulo.display_withROB();
		++i;
	}//while(i<50);
	while(tomasulo.EndofSimulation()==false);
    	tomasulo.Clear();
	return 0;

}

