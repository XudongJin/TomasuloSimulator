/*
 * Predictor.h
 *
 *  Created on: Apr 27, 2015
 *        Author: Xudong Jin
 */

#ifndef PREDICTOR_H_
#define PREDICTOR_H_

#include "Configure.h"
#include "Instruction.h"


class Predictor {
public:
	map<int ,int> predictResult;
	virtual void predict(Instruction * ins ,int address){}
	virtual bool update(bool predict_correct,int address){return false;}
	int getpredictResult(int address){
		map<int ,int>::iterator it=predictResult.find(address);
		  if(it==predictResult.end()){
		     cout<<"Can't find matching entry"<<endl;
		     return INVALID;
		   }else
			 return it->second;
	}
	virtual ~Predictor(){}
};



class BTFNT: public Predictor {
public:
	void predict(Instruction * ins, int address) {
		map<int, int>::iterator it = predictResult.find(address);
		if (it == predictResult.end()) {
			cout << "Create new entry " << endl;
			predictResult.insert(pair<int, int>(address, false));
			it = predictResult.find(address);
		}
		if (ins->src2 > 0) {
			it->second = false;
		} else if (ins->src2 < 0) {
			it->second = true;
		}
	}
	bool update(bool taken, int address) {
		if (taken == getpredictResult(address)) {
			return true;
		} else {
			return false;
		}
	}
};


class _1bitPredictor :public Predictor{
	map<int ,int> m_1bit_bht;
public:
	void predict(Instruction * ins ,int address){
		map<int ,int>::iterator it=m_1bit_bht.find(address);
		map<int, int>::iterator pt = predictResult.find(address);
		   if(it==m_1bit_bht.end() || pt==predictResult.end()){
		     m_1bit_bht.insert ( pair <int, int>  ( address, 0) );
		     predictResult.insert( pair <int, int>  ( address, false) );
		     it=m_1bit_bht.find(address); pt = predictResult.find(address);
		   }
		   if(it->second==0)
		   {
			   pt->second =false;
		   } else {
			   pt->second =true;
		   }
		  //return predict_address;
	}

	bool update(bool taken, int address) {
		map<int, int>::iterator it = m_1bit_bht.find(address);
		if (taken == getpredictResult(address)) {
			return true;
		} else {
			it->second = 1 - it->second;
			return false;
		}
	}
};





class _2bitPredictor: public Predictor {
	map<int, int> m_2bits_bht;
public:
	void predict(Instruction * ins, int address) {
		map<int, int>::iterator it = m_2bits_bht.find(address);
		map<int, int>::iterator pt = predictResult.find(address);
		if (it == m_2bits_bht.end() || pt == predictResult.end()) {
			m_2bits_bht.insert(pair<int, int>(address, 0));
			predictResult.insert(pair<int, int>(address, false));
			it = m_2bits_bht.find(address);
			pt = predictResult.find(address);
		}
		if (it->second == 0 || it->second == 1) {
			pt->second = false;
		} else if (it->second == 2 || it->second == 3) {
			pt->second = true;
		}
	}

	bool update(bool taken, int address) {
		map<int, int>::iterator it = m_2bits_bht.find(address);
		if (taken == true) {
			if (it->second == 2)
				it->second = 3;
			if (it->second == 1)
				it->second = 2;
			if (it->second == 0)
				it->second = 1;
		} else {
			if (it->second == 3)
				it->second = 2;
			if (it->second == 2)
				it->second = 1;
			if (it->second == 1)
				it->second = 0;
		}
		if (taken == getpredictResult(address)) {
			return true;
		} else {
			return false;
		}
	}
};

#endif /* PREDICTOR_H_ */
