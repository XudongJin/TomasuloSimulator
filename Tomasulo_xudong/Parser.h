/*
 * Parser.h
 *
 *  Created on: Apr 23, 2015
 *  Author: Xudong Jin
 */

#ifndef PARSER_H_
#define PARSER_H_


#include "Configure.h"
//#include "Tomasulo.h"

using namespace std;

vector<vector<string> >   parsedfile(80);//use this to save all lines loaded from file

class Parser {

private:
	bool codeOrmemory;
	int code_line_num;
	int memory_line_num;


	bool IsNum(string str) {
		for (unsigned int i = 0; i < str.size(); i++) {
			if ((str.at(i) > '9') || (str.at(i) < '0')) {
				return false;
			}
		}
		return true;
	}


	float str2int(string s){

		istringstream s2int(s);
		float num;
		s2int>>num;
		return num;
	}

	void parse_src2(string word, ParseCode & par) {
		string codeline = word;
		int pos = 0;
		if ((pos = codeline.find(",")) != -1) {
			codeline = codeline.erase(pos, pos);
		}

		if ((pos = codeline.find("R")) != -1) {
			codeline = codeline.erase(pos, pos + 1);
			par.rt_imm = false;
			par.rt_memaddr= true;
		} else if ((pos = codeline.find("#")) != -1) {
			codeline = codeline.erase(pos, pos + 1);
			par.rt_imm = true;
			par.rt_memaddr= false;
		} else{
			par.rt_imm = true;
			par.rt_memaddr= true;
		}
		par.rt = str2int(codeline);

	}


	void parse_src_dest(string word, ParseCode & par, string srcOrdest) {
		string codeline = word;
		int pos = 0;
		if ((pos = codeline.find(",")) != -1) {
			codeline = codeline.erase(pos, pos);
		}
		if ((pos = codeline.find('R')) != -1) {
			codeline = codeline.erase(pos, pos+1);
		}
			if (srcOrdest == "src1")
				par.rs = str2int(codeline);
			else if (srcOrdest == "dest")
				par.rd = str2int(codeline);
			else
			    par.rs = par.rd = INVALID;
	#ifdef DEBUG
			cout<<"Parse src and dest error"<<endl;
	#endif
	}

	float parse_memory(string word) {
		string codeline = word;
		int pos = 0;
		if ((pos = codeline.find("<")) != -1) {
			codeline = codeline.erase(pos, pos+1);
		}
		if ((pos = codeline.find('>')) != -1) {
			codeline = codeline.erase(pos, pos+1);
		}
		if (codeline[0] >= '0' && codeline[0] <= '9')
			return str2int(codeline);
		else {
		     #ifdef DEBUG
			cout<<"parse_memory"<<endl;
			  #endif
			return INVALID;
		}
	}


public:

	Parser(){
		codeOrmemory = false;
		code_line_num = 0;
		memory_line_num = 0;
	}

	void parse_file(char * filename) {

		ifstream instruction_file(filename);
		int pos = 0;
		int currentline = 0;

		ofstream afterparse;
		afterparse.open("parse_file.txt");
		if (!instruction_file.is_open()) {
			fatal("Can't open test file : %s, please make sure name and path of file is correct! \n", filename);
		}

		for (string s; getline(instruction_file, s);)
		{
			if (s[0] == '-' && s[1] == '-')
				continue;
		  if (s[0] == '\r' || s[0] == '\0')
				continue;
			if (s[0] >= '0' && s[0] <= '9') {
				if (codeOrmemory == false) {
					code_line_num = str2int(s);
					codeOrmemory = true;
				} else
					memory_line_num = str2int(s);
				continue;
			}
			if ((pos = s.find("--")) != -1)
				s = s.erase(pos, s.size());
			pos=0;
			while((pos = s.find(",",pos+1)) != -1){
				s.insert(pos+1,"  ");
			}
			istringstream ss(s);
			for (string word; ss >> word;) {
				if (word != " ")
					parsedfile[currentline].push_back(word);
			}
			++currentline;

		}

		for (currentline = 0; currentline < code_line_num; ++currentline){
			for (unsigned int i = 0; i < parsedfile[currentline].size(); ++i)
				afterparse << parsedfile[currentline][i] <<" ";
			    afterparse << endl;
		}
		instruction_file.close();
		afterparse.close();
	}


	void parse_line(Instruction_Queue & insqueue, DataMemory & datamemory){

		ParseCode parsecode;
		string codeline;
		int currentline;
		for (currentline = 0; currentline < code_line_num; ++currentline) {


			if (parsedfile[currentline].size() >= 1)
				parsecode.name = parsedfile[currentline][0];
			//////////////////////////////////////
			if (parsedfile[currentline].size() == 1) {
				parsecode.rs = INVALID;
				parsecode.rd = INVALID;
				parsecode.rt = INVALID;
				parsecode.rt_imm = false;
			}
			/////////////////////////////////////////////////////
			if (parsedfile[currentline].size() == 2) {
				parsecode.rs = INVALID;
				parsecode.rd = INVALID;
				parse_src2(parsedfile[currentline][1], parsecode);
			}
			/////////////////////////////
			if (parsedfile[currentline].size() == 3) {
				parse_src2(parsedfile[currentline][2], parsecode);
				if (parsecode.name == "LOAD" || parsecode.name == "MOVE") {
					parsecode.rs = INVALID;
					//   cout<<"v2[line][1]"<<v2[line][1]<<endl;
					parse_src_dest(parsedfile[currentline][1], parsecode, "dest");

				} else {
					parsecode.rd = INVALID;
					parse_src_dest(parsedfile[currentline][1], parsecode, "src1");
				}
			}

			////////////////////////////////////////////////////////
			if (parsedfile[currentline].size() == 4) {
				parse_src_dest(parsedfile[currentline][1], parsecode, "dest");
				parse_src_dest(parsedfile[currentline][2], parsecode, "src1");
				parse_src2(parsedfile[currentline][3], parsecode);
			}
			//////////////////////////////////////////////
			if(parsecode.name !="HALT"){
			insqueue.push_back(parsecode);
			cout << parsecode.name << "  rd " << parsecode.rd << "  rs "
					<< parsecode.rs << "  rt " << parsecode.rt << endl;
			}
		}

        int offset=currentline;
		for (;currentline < memory_line_num+offset; ++currentline){
			int index =(int)parse_memory(parsedfile[currentline][0]);
			float data=		parse_memory(parsedfile[currentline][1]);
			datamemory.writeMem(index,data);
		}
	}

};


#endif /* PARSER_H_ */
