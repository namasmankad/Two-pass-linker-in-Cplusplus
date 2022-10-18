#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <algorithm>

using namespace std;


vector<string> all_tokens;
vector<int> y_vec;
vector<int> x_vec;
vector<int> base_add_list;

vector<string> s_list; //symbols in deflists
vector<int> mod_num_list; //modules numbers for symbols in s_list
vector<int> r_list; //relative word addresses in deflists
vector<int> s_occur_list; //number of occurances of a symbol in deflists

vector<string> use_s_list; //symbols in uselists
vector<int> use_mod_num_list; //module numbers for symbols in use_s_list

vector<string> type_list;
vector<int> instr_list;
vector<int> code_mod_num_list;

int main(int argc, char * argv[])
{
	int index = 0;
	int num_tokens;
	int mod_num = 1;

	if (argc < 2 || argc > 2)
	{
		cout<<"Unexpected input." << endl;
		exit(0);
	}

	ifstream input_file;
	string inputfile = argv[1];
	input_file.open(inputfile);
	int x_pos;
	int y_pos = 1; //line number where the token resides
	string line;
	while(getline(input_file, line))
	{
		stringstream word(line);
		string token;
		x_pos = 1; //position of the token in that line
		while(word >> token)
		{
			all_tokens.push_back(token);
			y_vec.push_back(y_pos);
			x_vec.push_back(x_pos);
			x_pos = x_pos + 1;
		}
		y_pos = y_pos + 1;
	}

	int last_row = y_pos-1;
	int last_offset = x_pos;
	y_vec.push_back(last_row);
	x_vec.push_back(last_offset);

	input_file.close();

	num_tokens = all_tokens.size();

	base_add_list.push_back(0);

	//===PASS 1===
	while(index < all_tokens.size())
	{
		//---PARSE DEF LIST---

		//check if the index is out of bounds of the all_token vector
		//i.e there is no token(number) in the input file when there should be one
		if(index >= num_tokens)
		{
			//error_flag = 1;
			int row_num = y_vec[index];
			int col_num = x_vec[index];
			int count = 0;
			int len = 0;
			for(int a=0;a<all_tokens.size();a++)
			{
				if (y_vec[a] == row_num and x_vec[a] <= col_num)
				{
					len += all_tokens[a].length();
					count += 1;
				}
			}
			int len1 = len + count;
			//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
			cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": NUM_EXPECTED";
			return 1; //or break or exit(0)
		}
		
		//now we take the token into a string since we know that its in the input file
		string temp_token = all_tokens[index];
		//check if defcount is a number
		for (int i=0; i<temp_token.length(); i++)
		{
			//if not a number set error_flag = 1, print the error and break
			if(!isdigit(temp_token[i]))
			{
				//error_flag = 1;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] < col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": NUM_EXPECTED";
				return 1; //or break or exit(0)
				//break;
			}
		}
		
		//now we know defcount is a number so we get defcount
		int defcount;
		defcount = stoi(all_tokens[index].c_str());

		//check if defcount is less than 16
		if (!(defcount <= 16))
		{
			//error_flag = 2;
			cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": TOO_MANY_DEF_IN_MODULE";
			return 1; //or break or exit(0)
		}

		// now parse the (s,r) pairs in the def list
		for (int i=0; i<defcount; i++)
		{
			index = index + 1;
			//check if the index is out of bounds of the all_token vector
			//i.e there is no token in the input file when there should be one
			if (index >= num_tokens)
			{
				//error_flag = 3;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] <= col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_EXPECTED";
				// cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": SYM_EXPECTED";
				return 1; //or break or exit(0)
			}
			
			//now we take the token in a string since there is a token present
			string sym = all_tokens[index];
			//check if sym is a valid symbol of type [a-z][0-9a-z]*
			for (int i = 0; i<sym.length(); i++)
			{
				if(i==0)
				{
					if(!isalpha(sym[i]))
					{
						//error_flag = 3;
						int row_num = y_vec[index];
						int col_num = x_vec[index];
						int count = 0;
						int len = 0;
						for(int a=0;a<all_tokens.size();a++)
						{
							if (y_vec[a] == row_num and x_vec[a] < col_num)
							{
								len += all_tokens[a].length();
								count += 1;
							}
						}
						int len1 = len + count;
						//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_EXPECTED";
						cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": SYM_EXPECTED";
						return 1; //or break or exit(0)
					}
				}
				else
				{
					if(!isalnum(sym[i]))
					{
						//error_flag = 3;
						int row_num = y_vec[index];
						int col_num = x_vec[index];
						int count = 0;
						int len = 0;
						for(int a=0;a<all_tokens.size();a++)
						{
							if (y_vec[a] == row_num and x_vec[a] < col_num)
							{
								len += all_tokens[a].length();
								count += 1;
							}
						}
						int len1 = len + count;
						//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_EXPECTED";
						cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": SYM_EXPECTED";
						return 1; //or break or exit(0)
					}
				}
			}
			//check if sym is longer than 16
			if (!(sym.length() <= 16))
			{
				//error_flag = 5;
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_TOO_LONG";
				return 1;
			}

			//check if sym is already in the s_list
			int occur_flag = 0;
			auto it = find(s_list.begin(), s_list.end(), sym);
			if (it != s_list.end())
			{
				//the sym is already in the s_list
				//we increase its counter of occurence in s_occur_list
				int temp_index = it - s_list.begin();
				s_occur_list[temp_index] += 1;
				occur_flag = 1;
			}
			else
			{
				//the sym is not in s_list
				//we append the sym in s_list
				//we append 1 to s_occur_list
				//we append mod_num to mod_num_list
				//occur_flag = 0;
				s_list.push_back(sym);
				s_occur_list.push_back(1);
				mod_num_list.push_back(mod_num);
			}

			//now we check for relative address value
			index = index + 1;

			if(index >= num_tokens)
			{
				//error_flag = 1;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] <= col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": NUM_EXPECTED";
				return 1; //or break or exit(0)
			}
			string radd = all_tokens[index];
			for (int i=0; i<radd.length(); i++)
			{
				//if not a number set error_flag = 1, print the error and break
				if(!isdigit(radd[i]))
				{
					//error_flag = 1;
					int row_num = y_vec[index];
					int col_num = x_vec[index];
					int count = 0;
					int len = 0;
					for(int a=0;a<all_tokens.size();a++)
					{
						if (y_vec[a] == row_num and x_vec[a] < col_num)
						{
							len += all_tokens[a].length();
							count += 1;
						}
					}
					int len1 = len + count;
					//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
					cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": NUM_EXPECTED";
					return 1; //or break or exit(0)
					//break;
				}
			}

			if(occur_flag == 0)
			{
				//the sym just before wasnt in the list, we append rel add to r_list
				int var1 = base_add_list.back() + stoi(radd);//check???
				r_list.push_back(var1);
			}
		}

		//---PARSE USE LIST---

		index = index + 1;

		if(index >= num_tokens)
		{
			//error_flag = 1;
			int row_num = y_vec[index];
			int col_num = x_vec[index];
			int count = 0;
			int len = 0;
			for(int a=0;a<all_tokens.size();a++)
			{
				if (y_vec[a] == row_num and x_vec[a] <= col_num)
				{
					len += all_tokens[a].length();
					count += 1;
				}
			}
			int len1 = len + count;
			//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
			cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": NUM_EXPECTED";
			return 1; //break or exit(0)
		}

		temp_token = all_tokens[index]; //use something other than temp_token??
		for (int i=0; i<temp_token.length(); i++)
		{
			//if not a number set error_flag = 1, print the error and break
			if(!isdigit(temp_token[i]))
			{
				//error_flag = 1;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] < col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": NUM_EXPECTED";
				return 1; //or break or exit(0)
				//break;
			}
		}

		//now we know usecount is a number so we get usecount
		int usecount;
		usecount = stoi(all_tokens[index].c_str());

		//check if usecount is less than 16
		if (!(usecount <= 16))
		{
			//error_flag = 4;
			cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": TOO_MANY_USE_IN_MODULE";
			return 1; //or break or exit(0)
		}

		for (int i=0; i<usecount; i++)
		{
			index = index + 1;
			if(index >= num_tokens)
			{
				//error_flag = 3;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] <= col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": SYM_EXPECTED";
				return 1; //break or exit(0)
			}

			string use_sym = all_tokens[index];

			//check if use_sym is a valid symbol of type [a-z][0-9a-z]*
			for (int i = 0; i<use_sym.length(); i++)
			{
				if(i==0)
				{
					if(!isalpha(use_sym[i]))
					{
						//error_flag = 3;
						int row_num = y_vec[index];
						int col_num = x_vec[index];
						int count = 0;
						int len = 0;
						for(int a=0;a<all_tokens.size();a++)
						{
							if (y_vec[a] == row_num and x_vec[a] < col_num)
							{
								len += all_tokens[a].length();
								count += 1;
							}
						}
						int len1 = len + count;
						//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_EXPECTED";
						cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": SYM_EXPECTED";
						return 1; //or break or exit(0)
					}
				}
				else
				{
					if(!isalnum(use_sym[i]))
					{
						//error_flag = 3;
						int row_num = y_vec[index];
						int col_num = x_vec[index];
						int count = 0;
						int len = 0;
						for(int a=0;a<all_tokens.size();a++)
						{
							if (y_vec[a] == row_num and x_vec[a] < col_num)
							{
								len += all_tokens[a].length();
								count += 1;
							}
						}
						int len1 = len + count;
						//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_EXPECTED";
						cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1+1<<": SYM_EXPECTED";
						return 1; //or break or exit(0)
					}
				}
			}

			//check if use_sym is longer than 16
			if(!(use_sym.length() <= 16))
			{
				//error_flag = 5;
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": SYM_TOO_LONG";
				return 1;
			}
			use_s_list.push_back(use_sym);
			use_mod_num_list.push_back(mod_num);
		}

		//---PARSE PROGRAM TEXT---

		index = index + 1;

		if(index >= num_tokens)
		{
			//error_flag = 1;
			int row_num = y_vec[index];
			int col_num = x_vec[index];
			int count = 0;
			int len = 0;
			for(int a=0;a<all_tokens.size();a++)
			{
				if (y_vec[a] == row_num and x_vec[a] <= col_num)
				{
					len += all_tokens[a].length();
					count += 1;
				}
			}
			int len1 = len + count;
			//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
			cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": NUM_EXPECTED";
			return 1;
		}
		temp_token = all_tokens[index]; //use something other than temp_token??
		for (int i=0; i<temp_token.length(); i++)
		{
			//if not a number set error_flag = 1, print the error and break
			if(!isdigit(temp_token[i]))
			{
				//error_flag = 1;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] < col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1+1<<": NUM_EXPECTED";
				return 1; //or break or exit(0)
				//break;
			}
		}

		int codecount;
		codecount = stoi(all_tokens[index].c_str());

		if (codecount > (512 - base_add_list.back()))
		{
			//error_flag = 6;
			cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": TOO_MANY_INSTR";
			return 1;
		}

		int last_modules_add;
		last_modules_add = base_add_list.back();
		base_add_list.push_back(last_modules_add + codecount);

		for (int i=0; i<codecount; i++)
		{
			index = index + 1;
			if(index >= num_tokens)
			{
				//error_flag = 7;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] <= col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": ADDR_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": ADDR_EXPECTED";
				return 1;
			}
			temp_token = all_tokens[index];
			if(temp_token != "A" && temp_token != "E" && temp_token != "I" && temp_token != "R")
			{
				//error_flag = 7;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] < col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": ADDR_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1 + 1<<": ADDR_EXPECTED";
				return 1;
			}

			type_list.push_back(temp_token);

			index = index + 1;

			if(index >= num_tokens)
			{
				//error_flag = 1;
				int row_num = y_vec[index];
				int col_num = x_vec[index];
				int count = 0;
				int len = 0;
				for(int a=0;a<all_tokens.size();a++)
				{
					if (y_vec[a] == row_num and x_vec[a] <= col_num)
					{
						len += all_tokens[a].length();
						count += 1;
					}
				}
				int len1 = len + count;
				//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
				cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1<<": NUM_EXPECTED";
				return 1;
			}
			temp_token = all_tokens[index]; //use something other than temp_token??
			for (int i=0; i<temp_token.length(); i++)
			{
				//if not a number set error_flag = 1, print the error and break
				if(!isdigit(temp_token[i]))
				{
					//error_flag = 1;
					int row_num = y_vec[index];
					int col_num = x_vec[index];
					int count = 0;
					int len = 0;
					for(int a=0;a<all_tokens.size();a++)
					{
						if (y_vec[a] == row_num and x_vec[a] < col_num)
						{
							len += all_tokens[a].length();
							count += 1;
						}
					}
					int len1 = len + count;
					//cout<<"Parse Error line "<<y_vec[index]<<" offset "<<x_vec[index]<<": NUM_EXPECTED";
					cout<<"Parse Error line "<<y_vec[index]<<" offset "<<len1+1<<": NUM_EXPECTED";
					return 1; //or break or exit(0)
					//break;
				}
			}
			int var2 = stoi(temp_token);

			instr_list.push_back(var2);

			code_mod_num_list.push_back(mod_num);
		}
		mod_num += 1;
		index = index + 1;
	}
	//===END OF PASS 1===

	int total_mods = mod_num - 1;

	//print warning if symbol's add is too large
	for(int k1=1; k1<=total_mods; k1++)
	{
		//calc how many instructioins in module k1 from code_mod_num_list
		int number_of_ins = 0;
		for(int k2 = 0; k2 < code_mod_num_list.size(); k2++)
		{
			if(code_mod_num_list[k2] == k1)
			{
				number_of_ins += 1;
			}
		}
		//check if all symbol values in def list of k1 module are less than number_of_ins
		for(int k2 = 0; k2 < mod_num_list.size(); k2++)
		{
			if(mod_num_list[k2] == k1)
			{
				int l1 = r_list[k2] - base_add_list[k1-1];
				int l2 = number_of_ins - 1;
				if(l1 > l2)
				{
					cout<<"Warning: Module "<<k1<<": "<<s_list[k2]<<" too big "<<l1<<" (max="<<l2<<") assume zero relative"<<endl;
					r_list[k2] = base_add_list[k1-1];
				}
			}
		}
	}


	//print symbol table
	cout<<"Symbol Table"<<endl;
	for(int m1 = 0; m1 < s_list.size(); m1++)
	{
		cout<<s_list[m1]<<"="<<r_list[m1];
		if (s_occur_list[m1] > 1)
		{
			cout<<" Error: This variable is multiple times defined; first value used"<<endl;
		}
		else
		{
			cout<<endl;
		}
	}

	mod_num = 1;

	//===PASS 2===
	cout<<"Memory Map"<<endl;
	while(mod_num <= total_mods)
	{

		//get all the use symbols in the current module from use_s_list
		vector<string> cur_use_list;
		for(int i = 0; i< use_s_list.size(); i++)
		{
			if(use_mod_num_list[i] == mod_num)
			{
				cur_use_list.push_back(use_s_list[i]);
			}
		}

		//make a count list of how many times a symbol in current module is used
		//to check later if all the currentr use symbols in the current module were used
		vector<int> cur_use_list_count;
		for(int i = 0; i<cur_use_list.size(); i++)
		{
			cur_use_list_count.push_back(0);
		}

		for(int i = 0; i<type_list.size(); i++)
		{
			if(code_mod_num_list[i] == mod_num)
			{
				//int instruction = instr_list[i]; //here instead??
				if(type_list[i] == "E")
				{
					string err = "";
					int instruction = instr_list[i]; //here instead??
					if(instruction >= 10000)
					{
						instruction = 9999;
						err = " Error: Illegal opcode; treated as 9999";
					}
					else
					{
						int opcode = instruction / 1000;
						int operand = instruction % 1000;
						if(operand > cur_use_list.size() - 1)
						{
							err = " Error: External address exceeds length of uselist; treated as immediate";
						}
						else
						{
							string symbol = cur_use_list[operand];
							//find the symbol in s_list
							auto it = find(s_list.begin(), s_list.end(), symbol);
							//int pos = it - s_list.begin();
							if(it == s_list.end())
							{
								//symbol was not found i.e it is not defined
								instruction = opcode * 1000;
								err = " Error: " + symbol + " is not defined; zero used";
								cur_use_list_count[operand] += 1;
							}
							else
							{
								//symbol was found i.e it is defined
								cur_use_list_count[operand] += 1;
								int pos = it - s_list.begin();
								instruction = (opcode * 1000) + r_list[pos];
							}
						}
					}
					//print the order and instruction
					//convert i and instruction to strings
					string order = to_string(i);
					string check_str = "";
					if(order.length() == 1)
					{
						check_str += "00" + order;
					}
					if(order.length() == 2)
					{
						check_str += "0" + order;
					}
					string ins = to_string(instruction);
					if(ins.length() == 1)
					{
						ins = "000" + ins;
					}
					else if(ins.length() == 2)
					{
						ins = "00" + ins;
					}
					else if(ins.length() == 3)
					{
						ins = "0" + ins;
					}
					cout<<check_str<<": "<<ins<<err<<endl;
				}
				else if(type_list[i] == "I")
				{
					string err = "";
					int instruction = instr_list[i]; //here instead??
					if(instruction >= 10000)
					{
						instruction = 9999;
						err = " Error: Illegal immediate value; treated as 9999";
					}
					//print the order and instruction
					//convert i and instruction to strings
					string order = to_string(i);
					string check_str = "";
					if(order.length() == 1)
					{
						check_str += "00" + order;
					}
					if(order.length() == 2)
					{
						check_str += "0" + order;
					}
					string ins = to_string(instruction);
					if(ins.length() == 1)
					{
						ins = "000" + ins;
					}
					else if(ins.length() == 2)
					{
						ins = "00" + ins;
					}
					else if(ins.length() == 3)
					{
						ins = "0" + ins;
					}
					cout<<check_str<<": "<<ins<<err<<endl;
				}
				else if(type_list[i] == "A")
				{
					string err = "";
					int instruction = instr_list[i]; //here instead??
					if(instruction >= 10000)
					{
						instruction = 9999;
						err = " Error: Illegal immediate value; treated as 9999";
					}
					else
					{
						int operand = instruction % 1000;
						int opcode = instruction / 1000;
						if(!(operand <= 512))
						{
							instruction = opcode * 1000;
							err = " Error: Absolute address exceeds machine size; zero used";
						}
					}
					//print the order and instruction
					//convert i and instruction to strings
					string order = to_string(i);
					string check_str = "";
					if(order.length() == 1)
					{
						check_str += "00" + order;
					}
					if(order.length() == 2)
					{
						check_str += "0" + order;
					}
					string ins = to_string(instruction);
					if(ins.length() == 1)
					{
						ins = "000" + ins;
					}
					else if(ins.length() == 2)
					{
						ins = "00" + ins;
					}
					else if(ins.length() == 3)
					{
						ins = "0" + ins;
					}
					cout<<check_str<<": "<<ins<<err<<endl;
				}
				else if(type_list[i] == "R")
				{
					string err = "";
					int instruction = instr_list[i]; //here instead??
					if(instruction >= 10000)
					{
						instruction = 9999;
						//err = " Error: Illegal immediate value; treated as 9999";
						err = " Error: Illegal opcode; treated as 9999";
					}
					else
					{
						int operand = instruction % 1000;
						int opcode = instruction / 1000;
						//find number of instructions in this module
						int module_size = 0;
						for(int j = 0 ; j<code_mod_num_list.size();j++)
						{
							if(code_mod_num_list[j] == mod_num)
							{
								module_size += 1;
							}
						}
						if(operand > module_size)
						{
							instruction = opcode * 1000 + base_add_list[mod_num-1];
							err = " Error: Relative address exceeds module size; zero used";
						}
						else
						{
							instruction = instruction + base_add_list[mod_num-1];
						}
					}
					//print the order and instruction
					//convert i and instruction to strings
					string order = to_string(i);
					string check_str = "";
					if(order.length() == 1)
					{
						check_str += "00" + order;
					}
					if(order.length() == 2)
					{
						check_str += "0" + order;
					}
					string ins = to_string(instruction);
					if(ins.length() == 1)
					{
						ins = "000" + ins;
					}
					else if(ins.length() == 2)
					{
						ins = "00" + ins;
					}
					else if(ins.length() == 3)
					{
						ins = "0" + ins;
					}
					cout<<check_str<<": "<<ins<<err<<endl;
				}
			}
		}
		//check if the symbols in the current uselist were actually used
		for(int i=0; i<cur_use_list.size();i++) //or used cur_use_list_count.size()??
		{
			string not_used;
			if(cur_use_list_count[i] == 0)
			{
				//the symbol wasnt used but was present in the use list
				not_used = cur_use_list[i];
				cout<<"Warning: Module "<<mod_num<<": "<<not_used<<" appeared in the uselist but was not actually used"<<endl;
			}
		}

		mod_num += 1;
	}
	//===END OF PASS 2===


	for(int p = 0; p < s_list.size(); p++)
	{
		string unused_sym = s_list[p];
		auto it1 = find(use_s_list.begin(), use_s_list.end(), unused_sym);
		if(it1 == use_s_list.end())
		{
			//not found in uselist hence it was defined but never used
			int module_number = mod_num_list[p];
			cout<<"Warning: Module "<<module_number<<": "<<unused_sym<<" was defined but never used"<<endl;
		}
	}
	return 0;
}