#pragma once
#include <iostream>

class TuringMachine 
{
public:
	TuringMachine(std::string file_name);
	~TuringMachine();
	void Start();
private:
	bool Input_Correct(std::string &tape);
	int Find_Alphabet_index(char tape_value);
	bool Next_Instruction(std::string &Q,int index_coll,std::string &next_position,std::string& value);
private:
	std::string alphabet_A_B;
	std::string *alphabet_Q;
	std::string** instruction_table;
	int m_row;   //  matrix line size
	int m_coll;  // marix coll size

};