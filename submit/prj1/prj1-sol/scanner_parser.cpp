#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <regex>
#include <algorithm>

using namespace std;


class Ast
{
	public:
		pair<string, string> key;
		pair<string, string> value;
		vector<Ast> xkids;
};



/*@@@@@@@@@@@@@@@@@@@@@@@@@@ JSON_STRINGIFIERS @@@@@@@@@@@@@@@@@@@@@@@@@@*/
void pretty_print_tokens(vector< pair<string, string> > tokens)
{
	cout << "[" << endl;

	for(auto t : tokens)
	{
		string temp = "";
		for(auto c : t.second)
			if(c == '\n')
				temp += "\\n";
			else if(c == '\t')
				temp += "\\t";
			else
				temp += c;
		t.second = temp;
		
		cout << "\t[" << endl;
		cout << "\t\t\"" << t.first << "\"," << endl;
		cout << "\t\t\"" << t.second << "\"" << endl;
		cout << "\t]," << endl;
	}

	cout << "]" << endl;
}
string JSON_stringify_tokens(vector< pair<string, string> > tokens)	//stringify tokens into JSON format
{
	string s = "";
	s += "[";

	for(auto it = tokens.begin(); it != tokens.end(); ++it)
	{
		string temp = "";
		for(auto c : it->second)
			if(c == '\n')
				temp += "\\n";
			else if(c == '\t')
				temp += "\\t";
			else
				temp += c;
		it->second = temp;

		s += "[\"" + it->first + "\",";
		s += "\"" + it->second + "\"";
		if(it == --tokens.end())
			s += "]";
		else
			s += "],";
	}

	s += "]";
	return s;
}


void print_ast(Ast ast);
void pretty_print_asts(vector<Ast> asts)
{
	for(auto a : asts)
	{
		print_ast(a);
	}
}
void print_ast(Ast ast)
{
	cout << "KEYS  :\"" << ast.key.first << "\":\"" << ast.key.second << "\"\n";
	cout << "VALUES:\"" << ast.value.first << "\":\"" << ast.value.second << "\"\n";
	if(ast.xkids.empty())
		return;
	else
		pretty_print_asts(ast.xkids);
}

string stringify_ast(Ast ast);
string stringify_ast_array(vector<Ast> asts);
string JSON_stringify_asts(vector<Ast> asts)	//wrapper function
{
	string s = "";
	s += "[";
	s += stringify_ast_array(asts);
	s += "]";
	return s;
}
string stringify_ast_array(vector<Ast> asts)
{
	string s = "";
	for(auto at = asts.begin(); at != asts.end(); ++at)
	{
		if(at == --asts.end())
			s += "{" + stringify_ast(*at) + "}";
		else
			s += "{" + stringify_ast(*at) + "},";
	}
	return s;
}
string stringify_ast(Ast ast)
{
	string temp = "";
	for(auto c : ast.value.second)		//explicit "\n"
		if(c == '\n')
			temp += "\\n";
		else if(c == '\t')
			temp += "\\t";
		else
			temp += c;
	ast.value.second = temp;

	string s = "";
	if(ast.value.first != "")			//to deal with ELSE objs
	{
		s += "\"" + ast.key.first + "\":\"" + ast.key.second + "\",";
		s += "\"" + ast.value.first + "\":\"" + ast.value.second + "\"";
	}
	else
		s += "\"" + ast.key.first + "\":\"" + ast.key.second + "\"";

	if(!ast.xkids.empty())
		s += ",\"xkids\":[" + stringify_ast_array(ast.xkids) + "]";
	return s;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



/*@@@@@@@@@@@@@@@@@@@@@@@@@@ SCANNER @@@@@@@@@@@@@@@@@@@@@@@@@@*/
vector< pair<string, string> > tokens;

void scan(string s)
{
	tokens.clear();
	string prev = "";

	while(s.length() > 0)
	{
		smatch m;
		
		if(regex_search(s, m, regex("^\\s*#(\\w+)[^\\S\\n]*")))	//if the current line is a directive line
		{
			if(	(m.str(1) == "ifdef") | 		//can have SYM
				(m.str(1) == "ifndef") | 
				(m.str(1) == "elif") )
			{
				if(!prev.empty())				//push previous lexemes
				{
					tokens.push_back( pair<string, string>(string("TEXT"), prev) );
					prev.clear();
				}
				string kind = m.str(1);
				transform(kind.begin(), kind.end(), kind.begin(), ::toupper);
				tokens.push_back( pair<string, string>(kind, string("#" + m.str(1))) );

				s = s.substr(m.length());
				if(regex_search(s, m, regex("^[a-zA-Z_]\\w*")))		//if valid SYM
					tokens.push_back( pair<string, string>(string("SYM"), m.str()) );

				s = s.substr(m.length());
				regex_search(s, m, regex("^.*\\n"));
			}
			else if( (m.str(1) == "else") | (m.str(1) == "endif") )	//cannot have SYM
			{
				if(!prev.empty())
				{
					tokens.push_back( pair<string, string>(string("TEXT"), prev) );
					prev.clear();
				}
				string kind = m.str(1);
				transform(kind.begin(), kind.end(), kind.begin(), ::toupper);
				tokens.push_back( pair<string, string>(kind, string("#" + m.str(1))) );

				regex_search(s, m, regex("^.*\\n"));
			}
			else	//not a valid directive
			{
				regex_search(s, m, regex("^.*\\n"));
				prev += m.str();
			}
		}
		else		//else current line is not a directive
		{
			regex_search(s, m, regex("^.*\\n"));
			prev += m.str();
		}

		s = s.substr(m.length());

	}

	if(!prev.empty())
	{
		tokens.push_back( pair<string, string>(string("TEXT"), prev) );
		prev.clear();
	}
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



/*@@@@@@@@@@@@@@@@@@@@@@@@@@ PARSER @@@@@@@@@@@@@@@@@@@@@@@@@@*/
unsigned int tokensIndex = 0;
pair<string, string> lookahead;

bool check(string kind)
{
	return lookahead.first == kind;
}
pair<string, string> next_token()
{
	if(tokensIndex >= tokens.size())
		return pair<string, string>("EOF", "<EOF>");
	else
		return tokens[tokensIndex++];
};
void match(string kind)
{
	if(check(kind))
		lookahead = next_token();
	else
	{
		cerr << "Expecting '" << kind << "' at " << lookahead.first << endl;
		exit(EXIT_FAILURE);
	}
}

/*
source
  : (TEXT | ifdef)*         
  ;
ifdef
  : (IFDEF | IFNDEF) SYM
    source
    (ELIF SYM source)*
    (ELSE source)?
    ENDIF
  ;
*/

Ast ifdef();
vector<Ast> source();

vector<Ast> parse(string s)
{
	tokens.clear();
	scan(s);
	lookahead = next_token();
	vector<Ast> Asts;
	if(!check("EOF"))
		Asts = source();
	if(!check("EOF"))		//if there are more tokens, it violates the EBNF grammar
	{
		cerr << "Could not consume remaining tokens" << endl;
		exit(EXIT_FAILURE);
	}
	return Asts;
}

vector<Ast> source()
{
	vector<Ast> Asts;
	while(check("TEXT") | check("IFDEF") | check("IFNDEF"))		//(TEXT | ifdef)*
	{
		if(check("TEXT"))
		{
			auto lexeme = lookahead.second;
			match("TEXT");
			Ast ast;
			ast.key = pair<string, string>(string("tag"), string("TEXT"));
			ast.value = pair<string, string>(string("text"), lexeme);
			Asts.push_back(ast);
		}
		else if(check("IFDEF") | check("IFNDEF"))
		{
			Asts.push_back(ifdef());
		}
	}
	return Asts;
}

Ast ifdef()
{
	Ast ast;

	auto kind = lookahead.first;			//(IFDEF | IFNDEF) SYM
	match(kind);

	auto lexeme = lookahead.second;
	match("SYM");
	ast.key = pair<string, string>(string("tag"), kind);
	ast.value = pair<string, string>(string("sym"), lexeme);

	ast.xkids = source();					//source ***returns kids***

	while(check("ELIF"))					//(ELIF SYM source)*
	{
		match("ELIF");
		auto lexeme = lookahead.second;
		match("SYM");

		Ast ast2;
		ast2.key = pair<string, string>(string("tag"), string("ELIF"));
		ast2.value = pair<string, string>(string("sym"), lexeme);

		ast2.xkids = source();
		ast.xkids.push_back(ast2);
	}

	if(check("ELSE"))						//(ELSE source)?
	{
		match("ELSE");

		Ast ast2;
		ast2.key = pair<string, string>(string("tag"), string("ELSE"));

		ast2.xkids = source();
		ast.xkids.push_back(ast2);
	}

	match("ENDIF");							//ENDIF

	return ast;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cerr << "script file usage:\n\t" << argv[0] << " < filename scan|parse" << endl;
		exit(EXIT_FAILURE);
	}

	string temp, fileContents;
	while(getline(cin, temp))
		fileContents += temp + "\n";

	if(string(argv[1]) == "scan")
	{
		scan(fileContents);
		cout << JSON_stringify_tokens(tokens) << endl;
	}
	else if(string(argv[1]) == "parse")
	{
		vector<Ast> asts = parse(fileContents);
		cout << JSON_stringify_asts(asts) << endl;
	}
	else
	{
		cerr << "script file usage:\n\t" << argv[0] << " < filename scan|parse" << endl;
		exit(EXIT_FAILURE);
	}
}