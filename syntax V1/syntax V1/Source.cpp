#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;
struct tok{
	string cp, vp, lineno;
}t[1000];
//==========Semantic==============
struct symbol{
	string name, dt;
	int scope;
}s[100];
bool lookup(string nm, int s);
bool insert(string nm, string t, int k);
string type, name;
int cn = 0;
int j = 0;
int sc = 1;
int stack[100];
int index = 0;
int k = 0;
string PL, AL, RT;
void token(string word[1000], int size);
int trans_ID(int ch, char word, int TT[3][4]);
int trans_char(int cs, char word, int TT[6][5]);
int trans_fl(int ch, char word, int TT[6][4]);
int trans_int(int ch, char word, int TT[4][3]);
int trans_st(int cs, char word, int TT[6][5]);
bool int_cons(string word);
bool ID(string word);
bool char_cons(string word);
bool flt_cons(string word);
bool st_cons(string word);
int line_index = 0;
int n = 0;
int arr[1000];
bool esC(char word);
bool spC(char word);
string arith[] = { "+", "-", "*", "/", "^" };
string keywords[] = { "then", "for", "send", "stop", "def", "rule", "end", "else_check", "let", "check", "true", "false", "until", "build", "state", "do", "namespace", "else", "default", "new", "main" };
string rel[] = { "<", ">", "<=", ">=", "==", "!=", "<>" };
string punc[] = { "(", ")", "[", "]", "{", "}", ",", ";", "\"", "\'", ":", "->", ".." };
string incdec[] = { "++", "--" };
string dt[] = { "int", "dec", "word", "alpha", "bool" };
string logical[] = { "&&", "||", "!" };
string assig[] = { "+=", "-=", "*=", "/=", "%=" };
fstream rfile("Write.txt");
fstream wfile("Tokens.txt");
int count1 = 1;
int fl = 1;
bool flo = false;
//============CFG================
bool for_st(); bool X(); bool Y(); bool body(); bool f_decl(); bool Y1(); bool Cond();
bool Exp(); bool Exp_(); bool A(); bool A_(); bool B(); bool B_(); bool E(); bool AND_OR(); bool T(); bool E_(); bool F(); bool T_(); bool G(); bool G1(); bool G2(); bool G3(); bool aconst(); bool args(); bool args1(); bool ADDMUL(); bool list(); bool Const();
bool check_st(); bool op_else(); bool op1();
bool until_st();
bool do_st();
bool LIST();
bool global();
bool func_def();
bool func_def1();
bool func_def2();
bool fbody();
bool m_st();
bool s_st();
bool send();
bool build();
bool build_body();
bool bm_st();
bool semi();
bool build_obj();
bool dec();
bool dec1();
bool list();
bool list1();
bool size();
bool init();
bool init1();
bool init2();
bool E1();
bool arrow();
bool ID_const();
bool pdec(); bool AND(); bool psize(); bool pinit();
bool ep = false;
int main()
{
	string temp, input;
	stringstream buff;
	buff << rfile.rdbuf();
	input = buff.str();
	cout << input << "\n";
	string word[1000];
	int i = 0, j = 0;
	while (i<input.length())
	{
		while (input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\\' && input[i] != '\'' && input[i] != '\"'  && input[i] != '+' && input[i] != '-' && input[i] != '^' && input[i] != '*' && input[i] != '/' && input[i] != '%' && input[i] != ';' && input[i] != '(' && input[i] != ')' && input[i] != '[' && input[i] != ']' && input[i] != '{' && input[i] != '}' && input[i] != '&' && input[i] != '|' && input[i] != '!' && input[i] != ':' && input[i] != ',' && input[i] != ';' && input[i] != '.' && input[i] != '<' && input[i] != '>' && input[i] != '=')
		{
			if (i == input.length())
			{
				break;
			}
			else
			{
				temp += input[i];
				i++;
				if (input[i] == '.' && !((input[i + 1] >= 'a' && input[i + 1] <= 'z') || (input[i + 1] >= 'A' && input[i + 1] <= 'Z')))//float
				{
					if (temp[0] >= '0' && temp[0] <= '9')
					{
						if (input[i + 1] >= '0' && input[i + 1] <= '9'){
							temp += input[i];
							i++;
						}
						if (input[i] >= '0' && input[i] <= '9')
						{
							while ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\\' && input[i] != '\'' && input[i] != '\"'  && input[i] != '+' && input[i] != '-' && input[i] != '^' && input[i] != '*' && input[i] != '/' && input[i] != '%' && input[i] != ';' && input[i] != '(' && input[i] != ')' && input[i] != '[' && input[i] != ']' && input[i] != '{' && input[i] != '}' && input[i] != '&' && input[i] != '|' && input[i] != '!' && input[i] != ':' && input[i] != ',' && input[i] != ';' && input[i] != '.' && input[i] != '<' && input[i] != '>' && input[i] != '=') && input[i] != '\0')
							{
								temp += input[i];
								i++;
							}
							if (input[i] == '\0'){
								flo = true;
							}
							word[j] = temp;
							j++; temp = "";
							arr[line_index] = n + 1;
							line_index++;
						}
					}
				}
			}
		}
		if (input[i] == '\n')
		{
			n++;
		}
		if (temp != "")
		{
			word[j] = temp;
			j++;
			temp = "";
			arr[line_index] = n + 1;
			line_index++;
		}
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\r' && input[i] != '\b')
		{
			if (input[i] == '/' && input[i + 1] == '/')//Single line Comments
			{
				i += 2;
				while (input[i] != '\n')
					i++;
			}
			else if (input[i] == '/' && input[i + 1] == '<')//Multi line
			{
				i += 2;
				while (!(input[i] == '>' && input[i + 1] == '/') && input[i] != EOF)
				{
					i++;
				}
				i += 2;
			}
			else if (input[i] == '.' && !((input[i + 1] >= 'a' && input[i + 1] <= 'z') || (input[i + 1] >= 'A' && input[i + 1] <= 'Z')))//float
			{
				temp = input[i];
				i++;
				while ((input[i] >= '0' && input[i] <= '9' || ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))) && input[i] != '\0')
				{
					temp += input[i];
					i++;
				}
				word[j] = temp;
				j++;
				temp = "";
				arr[line_index] = n + 1;
				line_index++;
			}
			else if (input[i] == '\"')//String
			{
				temp = input[i];
				i++;
				while ((input[i] != '\"' && input[i] != '\n') && input[i] != '\0')
				{
					temp += input[i];
					i++;
					if (input[i] == '\"' && input[i - 1] == '\\'){
						temp += input[i];
						i++;
					}
				}
				if (input[i] == '\"' && temp != ""){
					temp += input[i];
					word[j] = temp;
					j++; i++;
					temp = "";
					arr[line_index] = n + 1;
					line_index++;
				}
				else{
					word[j] = temp;
					j++; temp = "";
					arr[line_index] = n + 1;
					line_index++;
				}
				/*if(temp!="")
				{
				word[j] = temp;
				j++; i++;
				temp="";
				arr[line_index]=n+1;
				line_index++;
				}*/
			}
			else if (input[i] == '\'' && input[i + 1] != '\0')//Char
			{
				temp = input[i];
				i++;
				if (input[i] == '\\')
				{
					int c = 1;
					while (c <= 2)
					{
						temp += input[i];
						c++;
						i++;
					}
				}
				else
				{
					temp += input[i];
					i++;
				}
				temp += input[i];
				if (temp != "")
				{
					word[j] = temp;
					j++; i++;
					temp = "";
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '&')
			{
				if (input[i + 1] == '&'){
					word[j] = "&&";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else{
					word[j] = input[i];
					j++; i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '|' && input[i + 1] == '|')
			{
				word[j] = "||";
				j++;
				i += 2;
				arr[line_index] = n + 1;
				line_index++;
			}
			else if (input[i] == '*')
			{
				if (input[i + 1] == '=')
				{
					word[j] = "*=";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else
				{
					word[j] = input[i];
					j++;
					i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '%')
			{
				if (input[i + 1] == '=')
				{
					word[j] = "%=";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else
				{
					word[j] = input[i];
					j++;
					i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '/' && input[i + 1] == '=')
			{
				word[j] = "/=";
				j++;
				i += 2;
				arr[line_index] = n + 1;
				line_index++;
			}
			else if (input[i] == '+')
			{
				if (input[i + 1] == '+')
				{
					word[j] = "++";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else if (input[i + 1] == '=')
				{
					word[j] = "+=";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else
				{
					word[j] = input[i];
					j++;
					i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '-')
			{
				if (input[i + 1] == '>')
				{
					word[j] = "->";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else if (input[i + 1] == '-')
				{
					word[j] = "--";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else if (input[i + 1] == '=')
				{
					word[j] = "-=";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else
				{
					word[j] = input[i];
					j++;
					i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '>')
			{
				if (input[i + 1] == '=')
				{
					word[j] = ">=";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else if (input[i + 1] == '>')
				{
					word[j] = ">>";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else
				{
					word[j] = ">";
					j++;
					i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '<')
			{
				if (input[i + 1] == '=')
				{
					word[j] = "<=";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else if (input[i + 1] == '>')
				{
					word[j] = "<>";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else if (input[i + 1] == '<')
				{
					word[j] = "<<";
					j++;
					i += 2;
					arr[line_index] = n + 1;
					line_index++;
				}
				else
				{
					word[j] = "<";
					j++;
					i++;
					arr[line_index] = n + 1;
					line_index++;
				}
			}
			else if (input[i] == '=' && input[i + 1] == '=')
			{
				word[j] = "==";
				j++;
				i += 2;
				arr[line_index] = n + 1;
				line_index++;
			}
			else if (input[i] == '-' && input[i + 1] == '>')
			{
				word[j] = "->";
				j++;
				i += 2;
				arr[line_index] = n + 1;
				line_index++;
			}
			else
			{
				word[j] = input[i];
				j++;
				i++;
				arr[line_index] = n + 1;
				line_index++;
			}
		}
		else
		{
			i++;
		}
	}
	//cout << "Words are: \n";
	/*for (int k = 0; k < j; k++)
	{
		cout << word[k] << "\n";
	}*/
	if (flo == true){
		token(word, j - 1);
	}
	else{
		token(word, j);
	}
	system("Pause");

}
bool st_cons(string word)
{
	int TT[6][5] = { { 1, 5, 5, 5, 5 }, { 3, 2, 5, 4, 2 }, { 3, 2, 5, 4, 2 }, { 5, 5, 5, 5, 5 }, { 2, 2, 2, 2, 5 }, { 5, 5, 5, 5, 5 } };
	int initial = 0, Final = 3;
	int cs = initial;
	int i = 0;
	while (word[i] != NULL)
	{
		cs = trans_st(cs, word[i], TT);
		i++;
	}
	if (cs == Final)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool ID(string word)
{
	int TT[3][4] = { { 1, 1, 2, 2 }, { 1, 1, 1, 2 }, { 2, 2, 2, 2 } };
	int initial = 0, Final = 1;
	int cs = initial;
	int i = 0;
	while (word[i] != NULL)
	{
		cs = trans_ID(cs, word[i], TT);
		i++;
	}
	if (cs == Final)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool char_cons(string word)
{
	int TT[6][5] = { { 1, 5, 5, 5, 5 }, { 5, 2, 5, 4, 2 }, { 3, 5, 5, 5, 5 }, { 5, 5, 5, 5, 5 }, { 2, 2, 2, 2, 5 }, { 5, 5, 5, 5, 5 } };
	int initial = 0, Final = 3;
	int cs = initial;
	int i = 0;
	while (word[i] != NULL)
	{
		cs = trans_char(cs, word[i], TT);
		i++;
	}
	if (cs == Final)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool flt_cons(string word)
{
	int TT[6][4] = { { 1, 1, 3, 5 }, { 5, 2, 3, 5 }, { 5, 2, 3, 5 }, { 5, 4, 5, 5 }, { 5, 4, 5, 5 }, { 5, 5, 5, 5 } };
	int initial = 0, Final = 4;
	int cs = initial;
	int i = 0;
	while (word[i] != NULL)
	{
		cs = trans_fl(cs, word[i], TT);
		i++;
	}
	if (cs == Final)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool int_cons(string word)
{
	int TT[4][3] = { { 1, 2, 3 }, { 3, 2, 3 }, { 3, 2, 3 }, { 3, 3, 3 } };
	int initial = 0, Final = 2;
	int cs = initial;
	int i = 0;
	while (word[i] != NULL)
	{
		cs = trans_int(cs, word[i], TT);
		i++;
	}
	if (cs == Final)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void token(string word[50], int size)
{
	//cout << " CP\t\tVP\n";
	//wfile << "(CP,\t\tVP,\tline no.)\n";
	for (int i = 0; i < size; i++)
	{
		if (word[i] == dt[0] || word[i] == dt[1] || word[i] == dt[2] || word[i] == dt[3] || word[i] == dt[4])
		{
			//cout << "(DT\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(DT,\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "DT";t[cn].vp=word[i]; cn++;
		}
		else if (word[i] == keywords[0] || word[i] == keywords[1] || word[i] == keywords[2] || word[i] == keywords[3] || word[i] == keywords[4] || word[i] == keywords[5] || word[i] == keywords[6] || word[i] == keywords[7] || word[i] == keywords[8] || word[i] == keywords[9] || word[i] == keywords[10] || word[i] == keywords[11] || word[i] == keywords[12] || word[i] == keywords[13] || word[i] == keywords[14] || word[i] == keywords[15] || word[i] == keywords[16] || word[i] == keywords[17] || word[i] == keywords[18] || word[i] == keywords[19] || word[i] == keywords[20])
		{
			//cout << "(" << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = word[i]; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "+" || word[i] == "-")
		{
			//cout << "(AddSub" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(ADDSUB" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "ADDSUB"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "*" || word[i] == "/" || word[i] == "%")
		{
			//cout << "(DivMul" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(DIVMUL" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "DIVMUL"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "^")
		{
			//cout << "(" << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = word[i]; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == punc[0] || word[i] == punc[1] || word[i] == punc[2] || word[i] == punc[3] || word[i] == punc[4] || word[i] == punc[5] || word[i] == punc[6] || word[i] == punc[7] || word[i] == punc[8] || word[i] == punc[9] || word[i] == punc[10] || word[i] == punc[11] || word[i] == punc[12])
		{
			//cout << "(" << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = word[i]; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "void")
		{
			//cout << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "void"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "in")
		{
			//cout << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "in"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "=")
		{
			//cout << "(" << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "="; t[cn].vp = word[i]; cn++;
		}
		else if (ID(word[i]) == true)
		{
			//cout << "(ID" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(ID" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "ID"; t[cn].vp = word[i]; cn++;
		}
		else if (char_cons(word[i]) == true)
		{
			//cout << "(ch_cons" << "\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(char_const" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "char_const"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == ";")
		{
			//cout << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile <<"("<< word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = ";"; t[cn].vp = word[i]; cn++;
		}
		else if (flt_cons(word[i]) == true)
		{
			//cout << "(flt_cons" << "\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(flt_const" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "flt_const"; t[cn].vp = word[i]; cn++;
		}
		else if (st_cons(word[i]) == true)
		{
			//cout << "(st_cons" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(str_const" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "str_const"; t[cn].vp = word[i]; cn++;
		}
		else if (int_cons(word[i]) == true)
		{
			//cout << "(int_cons" << "\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(int_const" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "int_const"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "&&")
		{
			//cout << "(AND" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(&&" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "&&"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "||")
		{
			//cout << "(OR" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(||" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "||"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "!")
		{
			//cout << "(NOT" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(!" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "!"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == incdec[0] || word[i] == incdec[1])
		{
			//cout << "(IncDec" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(INCDEC" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "INCDEC"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == rel[0] || word[i] == rel[1] || word[i] == rel[2] || word[i] == rel[3] || word[i] == rel[4] || word[i] == rel[5] || word[i] == rel[6])
		{
			//cout << "(RO" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(RO" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "RO"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == assig[0] || word[i] == assig[1] || word[i] == assig[2] || word[i] == assig[3] || word[i] == assig[4])
		{
			//cout << "(assig_op" << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(ASSIGN_OP" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "ASSIGN_OP"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "->")
		{
			//cout << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" <<word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "->"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == "&")
		{
			//cout << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(" <<word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "&"; t[cn].vp = word[i]; cn++;
		}
		else if (word[i] == ".")
		{
			//cout << word[i] << "\t\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile <<"("<< word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "."; t[cn].vp = word[i]; cn++;
		}
		else if (word[i]=="<<" || word[i]==">>")
		{
			wfile << "(" << word[i] << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = word[i]; t[cn].vp = word[i]; cn++;
		}
		else
		{
			//cout << "(lexical error" << "\t" << word[i] << "\t" << arr[i] << ")" << "\n";
			wfile << "(lexical error" << ",\t" << word[i] << ",\t" << arr[i] << ")" << "\n";
			t[cn].cp = "lexical error"; t[cn].vp = word[i]; cn++;
		}
	}
	wfile << "($" << ",\t" << "$" << ",\t" << "$" << ")" << "\n";
	t[cn].cp = "$"; t[cn].vp = "$";
	/*for (int i = 0; i < 100; i++)
	{
		cout << t[i].cp << " ";
	}*/
	cn = 0;
	cout << "\n\t============Symbol Table===========\n\n";
	if (global() == true){
		cout << "\n\n\t========Mubarak Valid CFG======\n";
	}
	else{
		cout << "\n\n\t==========Sed lyf Invalid CFG :(( ===========\n";
	}
}
int trans_st(int S, char ch, int TT[6][5])
{
	if (ch == '\"'){
		return TT[S][0];
	}
	else if (ch == '\\'){
		return TT[S][3];
	}
	else if (spC(ch)){
		return TT[S][2];
	}
	else if (esC(ch)){
		return TT[S][1];
	}
	else{
		return TT[S][4];
	}
}
bool esC(char ch){
	char es[] = { 'a', 'b', 'f', 'n', 'r', 't', 'v' };
	for (int i = 0; i < 7; i++){
		if (es[i] == ch)
			return true;
	}
	return false;
}
bool spC(char ch){
	char sp[] = { '\?', '\\', '\'', '\"' };
	for (int i = 0; i <4; i++){
		if (sp[i] == ch)
			return true;
	}
	return false;
}
int trans_ID(int cs, char word, int TT[3][4])
{
	if (word == '_')
	{
		return TT[cs][0];
	}
	if ((word >= 'a' && word <= 'z') || (word >= 'A' && word <= 'Z'))
	{
		return TT[cs][1];
	}
	else if (word >= '0' && word <= '9')
	{
		return TT[cs][2];
	}
	else
	{
		return TT[cs][3];
	}
}
int trans_char(int cstate, char c, int TT[6][5])
{
	char sp[2] = { '\?', '\"' };
	char es[5] = { 'n', 't', 'r', 'b', 'f' };
	bool  e_s = false;

	if (c == '\'')
	{
		return TT[cstate][0];
	}

	else if (c == '\\')
	{
		return TT[cstate][3];
	}

	else if (c == sp[0] || c == sp[1])
	{
		return TT[cstate][2];
	}
	else
	{
		for (int i = 0; i<sizeof(es); i++)
		{
			if (c == es[i])
			{
				e_s = true;
				break;
			}
		}
	}

	if (e_s == true)
	{
		return TT[cstate][1];
	}
	else
	{
		return TT[cstate][4];
	}
}
int trans_fl(int S, char ch, int TT[6][4])
{
	if (ch == '+' || ch == '-')
		return TT[S][0];
	else if (ch == '.')
		return TT[S][2];
	else if (ch >= '0' && ch <= '9')
		return TT[S][1];
	else
		return TT[S][3];
}
int trans_int(int cs, char word, int TT[4][3])
{
	if (word == '+' || word == '-')
	{
		return TT[cs][0];
	}
	else if ((word >= '0' && word <= '9'))
	{
		return TT[cs][1];
	}
	else
	{
		return TT[cs][2];
	}
}
bool global()
{
	if (func_def() == true)
	{
		cn++;
		if (global() == true)
		{
			return true;
		}
	}
	else if (build() == true)
	{
		cn++;
		if (global() == true)
		{
			return true;
		}
	}
	else if (dec() == true)
	{
		//cn++;
		if (t[cn].cp == ";"){
			cn++;
		}
		if (global() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "$")
	{
		return true;
	}
	return false;
}

bool func_def()
{
	if (t[cn].cp == "def")
	{
		cn++;
		if (func_def1() == true)
		{
			return true;
		}
	}
	return false;
}

bool func_def1()
{
	if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		cn++;
		if (t[cn].cp == ":")
		{
			cn++;
			if (t[cn].cp == "DT")
			{
				RT = t[cn].vp;
				cn++;
				if (t[cn].cp == "(")
				{
					cn++;
					if (pdec() == true)
					{
						//cn++;
						if (t[cn].cp == ")")
						{
							if (AL == ""){
								PL = "void->" + RT;
								if (insert(name, PL, k)){
									return false;
								}
							}
							else{
								PL = AL + "->" + RT;
								AL = "";
								if (insert(name, PL, k)){
									return false;
								}
							}
							cn++;
							if (func_def2() == true)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	else if (t[cn].cp == "main")
	{
		cn++;
		if (t[cn].cp == ":")
		{
			cn++;
			if (t[cn].cp == "DT" || t[cn].cp=="void")
			{
				cn++;
				if (fbody() == true)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool func_def2()
{
	if (t[cn].cp == ";")
	{
		return true;
	}
	else if (fbody() == true)
	{
		return true;
	}
	return false;
}

bool fbody()
{
	if (t[cn].cp == "{")
	{
		cn++;
		stack[index] = k;
		k++; index++;
		if (m_st() == true)
		{
			//cn++;
			if (send() == true)
			{
				//idr cond aigi
				//cn++;
				if (t[cn].cp == "}" || t[cn+1].cp=="}")
				{
					if (t[cn + 1].cp == "}"){
						cn++;
					}
					k--;
					return true;
				}
			}
		}
	}
	return false;
}
bool m_st()
{
	if (s_st() == true)
	{
		cn++;
		if (t[cn].cp == ";"){
			cn--;
		}
		if (m_st() == true)
		{
			return true;
		}
	}
	//Follow
	else if (t[cn].cp == "send")
	{
		return true;
	}
	else if (t[cn].cp == "}")
	{
		return true;
	}
	else if (t[cn].cp == "end")
	{
		return true;
	}
	return false;
}

bool s_st()
{
	if (dec() == true)
	{
		return true;
	}
	else if (check_st() == true)
	{
		return true;
	}
	else if (until_st() == true)
	{
		return true;
	}
	else if (do_st() == true)
	{
		return true;
	}
	else if (for_st() == true)
	{
		return true;
	}
	else if (Exp() == true)
	{
		return true;
	}
	return false;
}

bool send()
{
	if (t[cn].cp == "send")
	{
		cn++;
		if (Exp() == true)
		{
			//cn++;
			if (t[cn].cp == "}"){
				cn--;
			}
			if (t[cn].cp == ";")
			{
				return true;
			}
		}
	}
	else if (t[cn].cp == "}")
	{
		return true;
	}
	return false;
}
//...GLOBAL END...

//...STRUCTURE...
bool build()
{
	if (t[cn].cp == "build")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			cn++;
			if (build_body() == true)
			{
				return true;
			}
		}
	}
	return false;
}

bool build_body()
{
	if (t[cn].cp == "{")
	{
		cn++;
		if (bm_st() == true)
		{
			//cn++;
			if (t[cn].cp == "}")
			{
				cn++;
				if (semi() == true)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool bm_st()
{
	if (dec() == true)
	{
		//cn++;
		if (bm_st() == true)
		{
			return true;
		}
	}
	else if (func_def() == true)
	{
		cn++;
		if (bm_st() == true)
		{
			return true;
		}
	}
	else if (build_obj() == true)
	{
		cn++;
		if (bm_st() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "}")
	{
		return true;
	}
	return false;
}

bool semi()
{
	if (t[cn].cp == ";")
	{
		return true;
	}
	else if (t[cn].cp == "ID")
	{
		cn++;
		if (t[cn].cp == ";")
		{
			return true;
		}
	}
	return false;
}

bool build_obj()
{
	if (t[cn].cp == "build")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			cn++;
			if (t[cn].cp == "ID")
			{
				cn++;
				if (t[cn].cp == ";")
				{
					return true;
				}
			}
		}
	}
	return false;
}
//...STRUCTURE END...

//...DECLARE FOR VARIABLE, ARRAY, POINTER...
bool dec()
{
	if (t[cn].cp == "let")
	{
		cn++;
		if (dec1() == true)
		{
			return true;
		}
	}
	//Follow for func
	else if (t[cn].cp == ")")
	{
		return true;
	}
	return false;
}

bool dec1()
{
	if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		cn++;
		if (list() == true)
		{
			return true;
		}
	}
	else if (arrow() == true)
	{
		//cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			cn++;
			if (list() == true)
			{
				return true;
			}
		}
	}
	return false;
}

bool list()
{
	if (t[cn].cp == ",")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
			cn++;
			if (list() == true)
			{
				return true;
			}
		}
	}
	else if (t[cn].cp == ":")
	{
		cn++;
		if (t[cn].cp == "DT")
		{
			type = t[cn].vp;
			if (insert(name, type, k)){
				return false;
			}
			cn++;
			if (list1() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == ")" || t[cn].cp == "def" || t[cn].cp == "build" || t[cn].cp == "let" || t[cn].cp == "$" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == "ID" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end")
	{
		if (t[cn].cp == "ID"){
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
		}
		return true;
	}
	return false;
}

bool list1()
{
	if (init() == true)
	{
		return true;
	}
	else if (size() == true)
	{
		cn++;
		if (init() == true)
		{
			return true;
		}
	}
	return false;
}

bool size()
{
	if (t[cn].cp == "[")
	{
		cn++;
		if (ID_const() == true)
		{
			cn++;
			if (t[cn].cp == "]")
			{
				cn++;
				if (size() == true)
				{
					//cn++;
					if (init() == true)
					{
						return true;
					}
				}
			}
		}
	}
	//Follow
	else if (t[cn].cp == "=" || t[cn].cp == "," || t[cn].cp == ";" || t[cn].cp == ")" || t[cn].cp == "def" || t[cn].cp == "build" || t[cn].cp == "let" || t[cn].cp == "$" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == "ID" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end")
	{
		return true;
	}
	return false;
}

bool init()
{
	if (t[cn].cp == "=")
	{
		cn++;
		if (init1() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == ",")
	{
		cn++;
		if (init1() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == ";")
	{
		return true;
	}
	//Follow
	else if (t[cn].cp == "let" || t[cn].cp == "def" || t[cn].cp == "build" || t[cn].cp == "}" || t[cn].cp == ")" || t[cn].cp == "until" || t[cn].cp == "$" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == "send" || t[cn].cp == "end" || Const()==true)
	{
		return true;
	}
	return false;
}

bool init1()
{
	if (E() == true)
	{
		cn++;
		if (init() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "&")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			cn++;
			if (init() == true)
			{
				return true;
			}
		}
	}
	else if (t[cn].cp == "{")
	{
		cn++;
		if (init2() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "[")
	{
		cn++;
		if (ID_const() == true)
		{
			cn++;
			if (t[cn].cp == "]")
			{
				cn++;
				if (size() == true)
				{
					return true;
				}
			}
		}
	}
	//Function parenthesis
	/*else if (t[cn].cp == "let")
	{
		return true;
	}*/
	return false;
}

bool init2()
{
	if (E() == true)
	{
		cn++;
		if (E1() == true)
		{
			//cn++;
			if (t[cn].cp == "}")
			{
				cn++;
				if (init() == true)
				{
					return true;
				}
			}
		}
	}
	else if (t[cn].cp == "{")
	{
		cn++;
		if (E() == true)
		{
			cn++;
			if (E1() == true)
			{
				if (t[cn].cp == "}")
				{
					cn++;
					if (init() == true)
					{
						cn++;
						if (t[cn].cp == "}")
						{
							cn++;
							if (init() == true)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool E1()
{
	if (t[cn].cp == ",")
	{
		cn++;
		if (E() == true)
		{
			cn++;
			if (E1() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == "}")
	{
		return true;
	}
	return false;
}

bool arrow()
{
	if (t[cn].cp == "->")
	{
		cn++;
		if (arrow() == true)
		{
			return true;
		}
	}
	//Follow
	else if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		return true;
	}
	return false;
}

bool ID_const()
{
	if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		if (lookup(name, sc)){
			return false;
		}
		return true;
	}
	else if (t[cn].cp == "int_const")
	{
		return true;
	}
	return false;
}
bool pdec()
{
	if (t[cn].cp == "let")
	{
		cn++;
		if (AND() == true)
		{
			if (t[cn].cp == "ID")
			{
				cn++;
				if (psize() == true)
				{
					if (t[cn].cp == ":")
					{
						cn++;
						if (t[cn].cp == "DT")
						{
							AL += t[cn].vp;
							cn++;
							if (pinit() == true)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	//Follow
	else if (t[cn].cp == ")")
	{
		return true;
	}
	return false;
}
bool AND()
{
	if (t[cn].cp == "&")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			return true;
		}
	}
	//Follow
	else if (t[cn].cp == "ID")
	{
		return true;
	}
	return false;
}
bool psize()
{
	if (t[cn].cp == "[")
	{
		cn++;
		if (ID_const() == true)
		{
			cn++;
			if (t[cn].cp == "]")
			{
				cn++;
				if (psize() == true)
				{
					return true;
				}
			}
		}
	}
	//Follow
	else if (t[cn].cp == ":")
	{
		return true;
	}
	return false;
}
bool pinit()
{
	if (t[cn].cp == ",")
	{
		AL += t[cn].vp;
		cn++;
		if (pdec() == true)
		{
			return true;
		}
	}
	//Follow
	else if (t[cn].cp == ")")
	{
		return true;
	}
	return false;
}
//...DECLARE FOR VARIABLE, ARRAY, POINTER END...
bool Exp()
{
	if (A() == true)
	{
		if (Exp_() == true)
		{
			return true;
		}
	}
	return false;
}
bool Exp_()
{
	if (t[cn].cp == "||")
	{
		cn++;
		if (A() == true)
		{
			if (Exp_() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == "]" || t[cn].cp == ")" || t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end" || t[cn].cp == "then" || t[cn].cp == ";" || t[cn].cp == ":" || t[cn].cp == ",")
	{
		return true;
	}
	return false;
}
bool A()
{
	if (B() == true)
	{
		if (A_() == true)
		{
			return true;
		}
	}
	return false;
}
bool A_()
{
	if (t[cn].cp == "&&")
	{
		cn++;
		if (B() == true)
		{
			if (A_() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == "]" || t[cn].cp == "||" || t[cn].cp == ")" || t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end" || t[cn].cp == "then" || t[cn].cp == ";" || t[cn].cp == ":" || t[cn].cp == ",")
	{
		return true;
	}
	return false;
}
bool B()
{
	if (E() == true)
	{
		if (B_() == true)
		{
			return true;
		}
	}
	return false;
}
bool B_()
{
	if (t[cn].cp == "RO")
	{
		cn++;
		if (E() == true)
		{
			if (B_() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == "]" || t[cn].cp == "&&" || t[cn].cp == "||" || t[cn].cp == ")" || t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end" || t[cn].cp == "then" || t[cn].cp == ";" || t[cn].cp == ":" || t[cn].cp == ",")
	{
		return true;
	}
	return false;
}
bool E()
{
	if (T() == true)
	{
		//cn++;
		if (E_() == true)
		{
			return true;
		}
	}
	return false;
}
bool E_()
{
	if (t[cn].cp == "ADDSUB")
	{
		cn++;
		if (T() == true)
		{
			//cn++;
			if (E_() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == "]" || t[cn].cp == "RO" || t[cn].cp == "=" || t[cn].cp == "def" || t[cn].cp == "build" || t[cn].cp == "&&" || t[cn].cp == "||" || t[cn].cp == ")" || t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end" || t[cn].cp == "then" || t[cn].cp == ";" || t[cn].cp == ":" || t[cn].cp == ",")
	{
		return true;
	}
	return false;
}
bool T()
{
	if (G() == true)
	{
		if (t[cn].cp == "," || t[cn].cp == ")"){
			cn--;
		}
		if (T_() == true)
		{
			return true;
		}
	}
	return false;
}
bool T_()
{
	if (t[cn].cp == "DIVMUL")
	{
		cn++;
		if (G() == true)
		{
			cn++;
			if (T_() == true)
			{
				return true;
			}
		}
	}
	//Follow
	else if (t[cn].cp == "]" || t[cn].cp == "ADDSUB" || t[cn].cp == "RO" || t[cn].cp == "=" || t[cn].cp == "def" || t[cn].cp == "build" || t[cn].cp == "&&" || t[cn].cp == "||" || t[cn].cp == ")" || t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end" || t[cn].cp == "then" || t[cn].cp == ";" || t[cn].cp == ":" || t[cn].cp == ",")
	{
		return true;
	}
	return false;
}
bool G()
{
	if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		if (lookup(name, sc)){
			return false;
		}
		cn++;
		if (G1() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "INCDEC")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
			return true;
		}
	}
	else if (t[cn].cp == "int_const" || t[cn].cp == "flt_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "ID")
	{
		return true;
	}
	else if (t[cn].cp == "(")
	{
		cn++;
		if (Exp() == true)
		{
			//cn++;
			if (t[cn].cp == ")")
			{
				return true;
			}
		}
	}
	else if (arrow() == true)
	{
		//cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
			return true;
		}
	}
	return false;
}
bool G1()
{
	if (t[cn].cp == ".")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			cn++;
			if (G1() == true)
			{
				return true;
			}
		}
	}
	else if (t[cn].cp == "[")
	{
		cn++;
		if (Exp() == true)
		{
			//cn++;
			/*if(ep==true){
			}
			else{
			cn++;
			}*/
			if (t[cn].cp == "]")
			{
				cn++;
				if (G1() == true)
				{
					return true;
				}
			}
		}
	}
	else if (t[cn].cp == "INCDEC")
	{
		return true;
	}
	else if (t[cn].cp == "(")
	{
		cn++;
		if (args() == true)
		{
			if (t[cn].cp == ";"){
				cn--;
			}
			if (t[cn].cp == ")")
			{
				return true;
			}
		}
	}
	else if (t[cn].cp == "<<" || t[cn].cp==">>")
	{
		cn++;
		if (Exp() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "ASSIGN_OP")
	{
		cn++;
	if (t[cn].cp == "int_const" || t[cn].cp == "flt_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "ID")
	{
		if (t[cn].cp=="ID"){
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
		}
		return true;
	}
	}
	//Follow
	else if (t[cn].cp == "]" || t[cn].cp == "DIVMUL" || t[cn].cp == "ADDSUB" || t[cn].cp == "RO" || t[cn].cp == "=" || t[cn].cp == "def" || t[cn].cp == "build" || t[cn].cp == "&&" || t[cn].cp == "||" || t[cn].cp == ")" || t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == ")" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end" || t[cn].cp == "then" || t[cn].cp == ";" || t[cn].cp == ":" || t[cn].cp == ",")
	{
		if (t[cn].cp == "ID"){
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
		}
		return true;
	}
	return false;
}
/*bool G2()
{
if (t[cn].cp == "[")
{
cn++;
if (aconst() == true)
{
cn++;
if (follow() == true){
cn--;
}
if (t[cn].cp == "]")
{
return true;
}
}
}
else if (t[cn].cp == ":")
{
cn++;
if (args() == true)
{
return true;
}
}
else if (follow() == true)
{
return true;
}
return false;
}
bool G3()
{
if (t[cn].cp == ".")
{
cn++;
if (t[cn].cp == "ID")
{
cn++;
if (G2() == true)
{
return true;
}
}
}
else if (follow() == true)
{
return true;
}
return false;
}*/
bool args()
{
	if (Exp() == true)
	{
		if (t[cn].cp == ";"){
			cn--;
		}
		else{
			cn++;
		}
		if (LIST() == true)
		{
			return true;
		}
	}
	return false;
}
bool LIST()
{
	if (t[cn].cp == ",")
	{
		cn++;
		if (args() == true){
			return true;
		}
	}
	//Follow
	else if (t[cn].cp == ")")
	{
		return true;
	}
	return false;
}
bool Const()
{
	if (t[cn].cp == "int_const")
	{
		return true;
	}
	else if (t[cn].cp == "flt_const")
	{
		return true;
	}
	else if (t[cn].cp == "ID")
	{
		return true;
	}
	return false;
}
bool for_st()
{
	if (t[cn].cp == "for")
	{
		cn++;
		if (X() == true)
		{
			cn++;
			if (Y() == true)
			{
				cn++;
				if (t[cn].cp == ":")
				{
					cn++;
					stack[index] = k;
					k++; index++;
					if (body() == true)
					{
						//cn++;
						if (t[cn].cp == "end")
						{
							cn++;
							if (t[cn].cp == ";")
							{
								k--;
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
bool X()
{
	if (f_decl() == true)
	{
		return true;
	}
	return false;
}
bool f_decl()
{
	if (t[cn].cp == "DT")
	{
		type = t[cn].vp;
		cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			if (insert(name, type, k)){
				return false;
			}
			cn++;
			if (t[cn].cp == "in")
			{
				cn++;
				if (Cond() == true)
				{
					return true;
				}
			}
		}
	}
	else if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		if (lookup(name, sc)){
			return false;
		}
		cn++;
		if (t[cn].cp == "in")
		{
			cn++;
			if (Cond() == true)
			{
				return true;
			}
		}
	}
	return false;
}
bool Y()
{
	if (t[cn].cp == "ID")
	{
		name = t[cn].vp;
		if (lookup(name, sc)){
			return false;
		}
		cn++;
		if (Y1() == true)
		{
			return true;
		}
	}
	else if (t[cn].cp == "INCDEC")
	{
		cn++;
		if (t[cn].cp == "ID")
		{
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
			return true;
		}
	}
	else if (ID_const() == true)
	{
		return true;
	}
	return false;
}
bool Y1()
{
	if (t[cn].cp == "INCDEC")
	{
		return true;
	}
	else if (t[cn].cp == "ASSIGN_OP")
	{
		cn++;
		if (ID_const() == true)
		{
			return true;
		}
	}
	return false;
}
bool body()
{
	if (m_st() == true)
	{
		return true;
	}
	else if (s_st() == true)
	{
		return true;
	}
	return false;
}
bool Cond()
{
	if (t[cn].cp == "int_const")
	{
		cn++;
		if (t[cn].cp == ".")
		{
			cn++;
			if (t[cn].cp == ".")
			{
				cn++;
				if (t[cn].cp == "int_const")
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool check_st()
{
	if (t[cn].cp == "check")
	{
		cn++;
		if (Exp() == true)
		{
			cn++;
			if (t[cn].cp == "then")
			{
				cn++;
				if (t[cn].cp == ":")
				{
					cn++;
					stack[index] = k;
					k++; index++;
					if (body() == true)
					{
						//cn++;
						if (t[cn].cp == "end")
						{
							cn++;
							if (t[cn].cp == ";")
							{
								cn++;
								k--;
								if (op_else() == true)
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}
bool op_else()
{
	if (t[cn].cp == "else")
	{
		cn++;
		if (op1() == true)
		{
			return true;
		}
	}
	//Follow
	else if (t[cn].cp == "let" || t[cn].cp == "check" || t[cn].cp == "until" || t[cn].cp == "do" || t[cn].cp == "for" || t[cn].cp == "ID" || t[cn].cp == "INCDEC" || t[cn].cp == "(" || t[cn].cp == "->" || t[cn].cp == "int_const" || t[cn].cp == "str_const" || t[cn].cp == "char_const" || t[cn].cp == "flt_const" || t[cn].cp == "send" || t[cn].cp == "}" || t[cn].cp == "end")
	{
		if (t[cn].cp == "ID"){
			name = t[cn].vp;
			if (lookup(name, sc)){
				return false;
			}
		}
		return true;
	}
	return false;
}
bool op1()
{
	if (check_st() == true)
	{
		return true;
	}
	else if (t[cn].cp == "then")
	{
		cn++;
		if (t[cn].cp == ":")
		{
			cn++;
			stack[index] = k;
			k++; index++;
			if (body() == true)
			{
				//cn++;
				if (t[cn].cp == "end")
				{
					cn++;
					if (t[cn].cp == ";")
					{
						k--;
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool until_st()
{
	if (t[cn].cp == "until")
	{
		cn++;
		if (Exp() == true)
		{
			cn++;
			if (t[cn].cp == ":")
			{
				cn++;
				stack[index] = k;
				k++; index++;
				if (body() == true)
				{
					if (t[cn].cp == "end")
					{
						cn++;
						if (t[cn].cp == ";")
						{
							k--;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
bool do_st()
{
	if (t[cn].cp == "do")
	{
		cn++;
		if (t[cn].cp == ":")
		{
			cn++;
			stack[index] = k;
			k++; index++;
			if (body() == true)
			{
				if (t[cn].cp == "end")
				{
					cn++;
					if (t[cn].cp == ";")
					{
						k--;
						cn++;
						if (t[cn].cp == "until")
						{
							cn++;
							if (Exp() == true)
							{
								if (t[cn].cp == ";")
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}
bool lookup(string nm, int sc)
{
	for (int i = 0; i <= j; i++)
	{
		if (nm == s[i].name){
			return false;
		}
	}
	return true;
}
bool insert(string nm, string t, int sc)
{
	if (lookup(nm, sc)){
		s[j].name = nm;
		s[j].dt = t;
		s[j].scope = sc;
		cout << "(" << s[j].name << " " << s[j].dt << " " << s[j].scope << ")\n";
		j++;
		return false;
	}
	else {
		cout << "\n\tVarible Already Declared (" << nm << " )\n";
		return true;
	}
}

