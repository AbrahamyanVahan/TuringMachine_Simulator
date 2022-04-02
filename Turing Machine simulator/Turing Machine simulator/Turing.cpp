#include "Turing.h"
#include <string>
#include <fstream>



TuringMachine::TuringMachine(std::string file_name)
{
	// Create fstream type object for reading file
	std::fstream read;
	read.open(file_name, std::fstream:: in);
	//  Check if file has been openned or not
	if (!read.is_open())
	{
		std::cerr << "File isnt Open!" << std::endl;
	}
	else 
	{
		int count_A_B=0;   //       Quantity of input output alphabet elements
		int count_Q = 1;   //       Quantity of Q alphabet elements (Quantity , +1)
		int lineNumber = 1;   //    Line number
		int table_line_index=0;   
		int table_col_index=0;    
		bool matrix_create = false;
		// Read file
		while (!read.eof()) 
		{
			std::string line;
			std::getline(read, line);
			//  Read input and output alphabets
			if (lineNumber==1)  
			{
				++lineNumber;
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] != ',' && line[i] != ' ') 
					{
						this->alphabet_A_B += line[i];
					}
				}
				count_A_B = this->alphabet_A_B.size();
				this->m_coll = count_A_B;     //   matrix table colls
			}
			//   Read Q
			else if(lineNumber==2)
			{
				++lineNumber;
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i]==',')
					{
						++count_Q;
					}
				}
				std::string read_Q;
				this->alphabet_Q = new std::string[count_Q];
				int index = 0;
				// matix line size
				this->m_row = count_Q;
				int size12 = line.size();
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] != ',' && line[i] != ' '&& i != line.size() - 1)
					{
						read_Q += line[i];
					}
					else if (line[i]==','||  i == line.size() - 1)
					{
						if (i== i != line.size() - 1&& line[i] != ',')
						{
							read_Q += line[i];
						}
						this->alphabet_Q[index] = read_Q;
						read_Q = "";
						++index;
					}

				}
			}
			//    Read function table 
			else if (lineNumber > 2 )
			{
				if ((line == " " || line == "  " || line == "  "))
				{
					break;
				}
				++lineNumber;
				std::string temp_instr;
				if (matrix_create==false)
				{
					//    Creat matrix for saveing function table
					this->instruction_table = new std::string * [count_Q];
					for (int i = 0; i < count_Q; i++)
					{
						this->instruction_table[i] = new std::string[count_A_B];
					}
					matrix_create = true;
				}
				
				int a=line.size();
				//Write function tabel in matrix
				for (int i = 0; i < line.size(); i++)
				{
					if (i == line.size() - 1)
					{
						if (line[i]!=' ')
						{
							temp_instr += line[i];
						}
						
						this->instruction_table[table_line_index][table_col_index] = temp_instr;
					}
					else if (line[i] != '|'&& line[i]!=' ') 
					{
						temp_instr += line[i];
					}
					else if (line[i]=='|' )
					{
						this->instruction_table[table_line_index][table_col_index] = temp_instr;
						++table_col_index;
						temp_instr = "";
					}
				}
				++table_line_index;
				table_col_index = 0;
			}
		}
	}
	read.close();

}
bool TuringMachine::Input_Correct(std::string &tape)
{
	//   If there are empty symbols on input tape ignore them
	int k = tape.size();
	std::string temp;
	int star = 0;
	for (int i = 0; i < k; i++)
	{
		if (tape[i]!=' ' && tape[i] != '_')
		{
			if (tape[i] == '*')
			{
				++star;
				temp += tape[i];
			}
			else 
			{
				temp += tape[i];
			}
		}
	}
	// If there is more than one * in input print error
	if (star>1)
	{
		std::cerr << "THERE SHOULD BE ONE * ON THE TAPE" << std::endl;
		return false;
	}
	// If there is 0 * in input print error
	else if(star==0)
	{
		std::cerr << "AT LEAST THERE MUST BE ONE * ON THE TAPE" << std::endl;
		return false;
	}
	tape = temp;
	int alphabet_size = this->alphabet_A_B.size();
	int index_while = 0;
	// Check that there is no * at the beginning or end of the input 
	if (tape[0] == '*' || tape[tape.size()-1]=='*') 
	{
		std::cerr << "* MUST SEPARATE TWO NUMBERS FROM EACH OTHER" << std::endl;
		return false;
	}
	else 
	{
		// If all elements belog to alphabet
		for (int i = 0; i < tape.size(); i++)
		{
			while (index_while != alphabet_size)
			{
				if (tape[i] == alphabet_A_B[index_while])
				{
					break;
				}
				else
				{
					if (alphabet_size == index_while + 1)
					{
						std::cerr << "incorenct input" << std::endl;
						return false;
					}
					else 
					{
						++index_while;
					}
				}
			}
			index_while = 0;
		}
		return true;
	}
	
}
int TuringMachine::Find_Alphabet_index(char tape_value) 
{
	// Find index of input value
	int size = this->alphabet_A_B.size();
	for (int i = 0; i < size; i++)
	{
		if (tape_value==this->alphabet_A_B[i])
		{
			return i;
		}
	}

	return -1;
}
bool TuringMachine::Next_Instruction(std::string &Q,int index_coll,std::string& next_position,std::string& value) 
{
	int size = this->m_row;
	bool ERROR_Q = true;
	for (int i = 0; i < size; i++)
	{
		//  Find the index corresponding to the current state which is the index
		//     of matrix row
		if (Q == this->alphabet_Q[i]) 
		{
			std::string temp;  // save next value, next Q,and next position
			temp = this->instruction_table[i][index_coll];
			if (temp=="X")
			{
				std::cerr << "SOMETHING IS WRONG IN THE INSTRUCTION TABLE!!!" << std::endl;
				return false;
			}
			//  If Allright iis ok
			else 
			{
				Q = "";
				value = "";
				next_position = "";
				int count = 0;
				int size_temp = temp.size();
				for (int i = 0; i < size_temp; i++)
				{
					if (temp[i] != ',' && count == 0) 
					{
						value += temp[i];
					}
					else if (temp[i] == ',') 
					{
						++count;
					}
					else if (temp[i]!=','&&count==1)
					{
						Q += temp[i];
					}
					else if (temp[i]!=','&&count==2)
					{
						next_position += temp[i];
					}

				}
				ERROR_Q = false;
				return true;
			}
		}
		
	}
	if (ERROR_Q == true)
	{
		std::cout << "THERE IS NO SUCH A Q\nINSTRUCTION TABLE ERROR!!" << std::endl;
		return false;
	}
}
void TuringMachine::Start() 
{
	std::cout << "ENTER VALUE OF INPUT TAPE" << std::endl;
	std::cout << "THAT SHOULD BE "<<this->alphabet_A_B << std::endl;
	std::string input_tape;
	std::cin >> input_tape;
	if (!Input_Correct(input_tape)) 
	{
		std::cerr << "INPUT ERROR! " << std::endl;
	}
	else
	{
		//  ADD Empty cells from beginning and from ending for the infinity of animation
		std::string temp = "___";
		temp += input_tape;
		input_tape = temp;
		for (int i = 0; i < 90; i++)
		{
			input_tape += '_';
		}
		int input_size = input_tape.size();
		
		std::cout << "TURING MACHINE INPUT TAPE IS CORRECT AND READY FOR WORK  " << std::endl;
		std::cout << input_tape << std::endl;
		std::string first_q = "q0"; ///  begiinning situatinon
		std::string Q = first_q;   //  current situation
		std::string head = "^";    // reading head of machine
		//
		int index_coll;
		std::string next_position;   // next position
		std::string value;
		int current_position;
		//  Find  index of first not empty box of tape 
		for (int i = 0; i < input_tape.size(); i++)
		{
			if (input_tape[i] != '_')
			{
				current_position = i;
				break;
			}
		}
		std::string key;
		bool DEBUG_MODE = false;
		std::cout << "IF YOU WANT TO CONTINUE WORK IN A DEBUG MODE CLICK 1,IF NOT CLICK ANY BUTTON FROM KEYBOARD  " << std::endl;
		std::cin >> key;
		if (key=="1")
		{
			DEBUG_MODE = true;
			std::cout << "TO TAKE THE NEXT STEP IN DEBUG MODE,PRESS ANY BUTTON FROM KEYBOARD AND ENTER" << std::endl;
			std::cout << "IF YOU WANT EXIT IN DEBUG MODE PLSEASE ENTER 000 " << std::endl;
		}
		else if (key!="1")
		{
			DEBUG_MODE = false;
		}
		std::string pos;
		bool k;
		for (int i = 1; i < INT_MAX; i++)
		{
			if (DEBUG_MODE == false)
			{
				//   Find tapes corresponding elements index in the alphabet
				index_coll = Find_Alphabet_index(input_tape[current_position]);
				if (index_coll > -1)
				{
					
					 k=Next_Instruction(Q, index_coll, next_position, value);
					if (Q == "H")
					{
						break;
					}
					else if (k==false)
					{
						break;
					}
					else
					{
						input_tape[current_position] = value[0];
						if (next_position == ">")
						{
							++current_position;
						}
						else if (next_position == "<")
						{
							--current_position;
						}
					}
				}
				else
				{
					std::cerr << "An incorrect value is written on the tape" << std::endl;
				}
			}
			else 
			{
				//   Find tapes corresponding elements index in the alphabet
				index_coll = Find_Alphabet_index(input_tape[current_position]);
				std::string deb;
				
				if (index_coll > -1)
				{
					// head position
					for (int i = 0; i < current_position; i++)
					{
						pos += " ";
					}
					//
					 k=Next_Instruction(Q, index_coll, next_position, value);
					 std::cout << "VALUE= " << value << " Q= " << Q << ", NEXT_POSITION= " << next_position << std::endl;
					std::cout << input_tape << std::endl;
					std::cout << pos << head << std::endl;
					pos = ""; 
					
					std::cin >> deb;
					//
					if (deb == "000") 
					{
						DEBUG_MODE = false;
					}
					//
					if (Q == "H")
					{
						break;
					}
					else if (k==false)
					{
						break;
					}
					else
					{
						input_tape[current_position] = value[0];
						if (next_position == ">")
						{
							++current_position;
						}
						else if (next_position == "<")
						{
							--current_position;
						}
					}
				}
				else
				{
					std::cerr << "An incorrect value is written on the tape" << std::endl;
				}
				
			}
			
		}
		//  head position
		for (int i = 0; i < current_position; i++)
		{
			pos += " ";
		}
		if (k!=false)
		{
			std::cout << "OUTPUT" << std::endl;
			std::cout << input_tape << std::endl;
			std::cout << pos << head << std::endl;
		}
		else 
		{
			std::cerr << "OUTPUT ERROR!" << std::endl;
		}

		
	}
}
TuringMachine::~TuringMachine() 
{
	delete[] this->alphabet_Q;
	for (int i = 0; i <m_row; i++)
	{
		delete[] this->instruction_table[i];
	}
	delete[] this->instruction_table;
}