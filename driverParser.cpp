//Giuseppe Celentano
//Programming Assignment 3
//November 22 2020
#include <iostream>  
#include <fstream>      
#include <cstdlib>  // exit prototype
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include "lex.h"
#include "parse.h"
#include "lex.cpp"
using namespace std;

bool progCheck = true;
bool endCheck = false;

bool checkLexError(LexItem checkTok, int& line)
{
	if(checkTok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized input pattern ");
		progCheck;
		cout << checkTok;
		return true;
	}
	else
		return false;
}

bool Prog(istream& in, int& line)
{
	//cout << "In Prog" << endl;
	bool check = true;
	bool statementList;
	LexItem tokObj = Parser::GetNextToken(in,line);
	
	if(!(tokObj.GetToken() == BEGIN))
	{
		ParseError(line, "Missing BEGIN");
		Parser::PushBackToken(tokObj);
		progCheck = false;
		return false;
	}
	tokObj = Parser::GetNextToken(in,line);
	if(tokObj.GetToken() == END)
	{
		ParseError(line, "No statements in program");
		progCheck = false;
		return false;
	}
	else
	{
		Parser::PushBackToken(tokObj);
		statementList = StmtList(in, line);
	}
		
	if(!statementList)
		progCheck = false;
	
	return progCheck;
}

bool Stmt(istream& in, int& line)
{
	bool status;
	LexItem t = Parser::GetNextToken(in, line);
	if(checkLexError(t, line)) { return false;}
	switch(t.GetToken())
	{
		case PRINT:
			status = PrintStmt(in, line);
			//cout << "status: " << (status? "true":"false") << endl;
			break;
				
		case IF:
			status = IfStmt(in, line);
			break;
			
		case IDENT:
			Parser::PushBackToken(t);
			status = AssignStmt(in, line);
			break;
		
		case END:
			Parser::PushBackToken(t);
			endCheck = true;
			break;
		
		case DONE:
			if(!endCheck)
			{
				ParseError(line, "Missing END");
				progCheck = false;
				return false;
				break;
			}
			else
				return true;
		
		default:
			ParseError(line, "Unrecognized Input");
			progCheck = false;
			return false;
			break;
	}
	
	return status;
}
	
bool PrintStmt(istream& in, int& line)
{
	bool ex = ExprList(in, line);
	if(!ex)
	{
		ParseError(line, "Missing expression after print");
		progCheck = false;
		return false;
	}
	return ex;
}
	
bool Var(istream& in, int& line)
{
	string identstr;
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		identstr = tok.GetLexeme();
		if(!(defVar.find(identstr)->second))
			defVar[identstr] = true;
		return true;
	}
	
	return false;
}

extern bool StmtList(istream& in, int& line)
{
	bool liststatus = true;
	bool status = true;
	do
	{
		status = Stmt(in, line);
		if (status)
		{
			LexItem scoma = Parser::GetNextToken(in, line);
			if(!(scoma.GetToken() == SCOMA) && scoma.GetToken() != END)
			{
				progCheck = false;
				ParseError(line, "Missing SCOMA (;)");
				return false;
			}
		}
		else
		{
			LexItem t = Parser::GetNextToken(in, line);
			if (t.GetToken() != END) 
			{
				progCheck = false;
				return false;
			}
			else
			{
				Parser::PushBackToken(t);
			}
		}
		
		if (!status)
		{
			LexItem t = Parser::GetNextToken(in, line);
			if(checkLexError(t, line)) { return false;}
			if (t.GetToken() != END) 
			{
				Parser::PushBackToken(t);
			}
			else
			{	
				liststatus = false;
			}
		}
	}
	while(liststatus);
	
	return true;
}

extern bool IfStmt(istream& in, int& line)
{
	LexItem t = Parser::GetNextToken(in, line);
	if(!(t.GetToken() == LPAREN))
	{
		ParseError(line, "Expected left PAREN");
		progCheck = false;
		Parser::PushBackToken(t);
	}
	
	bool ex = Expr(in, line);
	if(!ex)
	{
		ParseError(line, "Missing statement for IF");
		progCheck = false;
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(!(t.GetToken() == RPAREN))
	{
		ParseError(line, "Expected right PAREN");
		progCheck = false;
		Parser::PushBackToken(t);
	}
	
	t = Parser::GetNextToken(in, line);
	if (! (t.GetToken() == THEN) ) {
		ParseError(line, "Missing THEN after IF");
		Parser::PushBackToken(t);
		progCheck = false;
		return false;
	}

	ex = Stmt(in, line);
	if (!ex)
	{
		ParseError(line, "Statement missing after THEN");
		progCheck = false;
	}
	
	return ex;
}
extern bool AssignStmt(istream& in, int& line)
{
	bool varCheck = Var(in, line);
	if(varCheck)
	{
		LexItem t = Parser::GetNextToken(in,line);
		if(!(t.GetToken() == EQ))
		{
			ParseError(line, "Missing Assignment Operator =");
			Parser::PushBackToken(t);
			progCheck = false;
			return false;
		}
		else
		{
			bool expCheck = Expr(in, line);
			if(!expCheck)
			{
				ParseError(line, "Missing Expression in Assignment Statment");
				//Parser::PushBackToken(t);
				progCheck = false;
				return false;
			}
			
		}
	}
	
	return true;
}
extern bool ExprList(istream& in, int& line)
{
	LexItem t;
	bool exprList;
	bool loopCheck = false;
	do
	{
		exprList = false;
		exprList = Expr(in, line);		
		if(!exprList)
		{
			ParseError(line, "Expected Expression");
			progCheck = false;
			//Parser::PushBackToken(t);
			return false;
		}
		
		t = Parser::GetNextToken(in, line);
		if(checkLexError(t, line)) { return false;}
		if(t.GetToken() != COMA && t.GetToken() != SCOMA)
		{
			Parser::PushBackToken(t);
			if(Expr(in, line))
			{
				ParseError(line, "Missing COMA (,)");
				return false;
			}
			
			return true;
		}
		else if(t.GetToken() == SCOMA)
		{
			Parser::PushBackToken(t);
			return true;
		}
		else
			exprList = true;
			
	}while(exprList);
	
	return true;
}
extern bool Expr(istream& in, int& line)
{
	bool exprCheck;
	do
	{
		exprCheck = false;
		if(! (Term(in, line)) )
		{
			ParseError(line, "Expecting term after EXPR");
			progCheck = false;
			return false;
		}
		
		LexItem plusMinus = Parser::GetNextToken(in, line);
		if(plusMinus.GetToken() == PLUS || plusMinus.GetToken() == MINUS)
		{
			if(Term(in, line))
			{
				plusMinus = Parser::GetNextToken(in, line);
				if(plusMinus.GetToken() == PLUS || plusMinus.GetToken() == MINUS)
				{
					exprCheck = true;
				}
				else
				{
					Parser::PushBackToken(plusMinus);
					if(checkLexError(plusMinus, line)) { return false;}
				}
			}
			
			else
			{
				ParseError(line, "Missing term after SIGN");
				progCheck = false;
			}
		}
		else
			Parser::PushBackToken(plusMinus);
	
	}while(exprCheck);
	
	return true;
}
extern bool Term(istream& in, int& line)
{
	bool termCheck;
	do
	{
		termCheck = false;
		if(! (Factor(in, line)) )
		{
			ParseError(line, "Expecting Factor after EXPR");
			progCheck = false;
			return false;
		}
		
		LexItem multDiv = Parser::GetNextToken(in, line);
		if(multDiv.GetToken() == MULT || multDiv.GetToken() == DIV)
		{
			if(Term(in, line))
			{
				multDiv = Parser::GetNextToken(in, line);
				if(multDiv.GetToken() == MULT || multDiv.GetToken() == DIV)
				{
					termCheck = true;
				}
				else
				{
					Parser::PushBackToken(multDiv);
					if(checkLexError(multDiv, line)) { return false;}
				}
			}
			
			else
			{
				if(checkLexError(multDiv, line)) { return false;}
				ParseError(line, "Missing term after SIGN");
				progCheck = false;
			}
		}
		else
			Parser::PushBackToken(multDiv);
	
	}while(termCheck);
	
	return true;
}
extern bool Factor(istream& in, int& line)
{
	string identstr;
	LexItem tok = Parser::GetNextToken(in, line);
	switch(tok.GetToken())
	{
		bool factorExpr;
		case IDENT:
			identstr = tok.GetLexeme();
			if(!(defVar.find(identstr)->second))
			{
				ParseError(line, "Undefined Variable");
				progCheck = false;
				return false;
			}
			return true;
			break;
			
		case ICONST:
			return true;
			break;
		
		case RCONST:
			return true;
			break;
			
		case SCONST:
			return true;
			break;
			
		case LPAREN:
			factorExpr = Expr(in, line);
			if(!factorExpr)
			{
				ParseError(line, "Missing Expr");
				progCheck = false;
				return false;
			}
			tok = Parser::GetNextToken(in, line);
			if(tok.GetToken() != RPAREN)
			{
				ParseError(line, "Missing right Parenthesis");
				Parser::PushBackToken(tok);
				progCheck = false;
				return false;
			}
			return true;
			break;
		
		default:
			Parser::PushBackToken(tok);
			if(checkLexError(tok, line)) {return false;}
			ParseError(line, "Unexpected FACTOR type");
			progCheck = false;
			return false;
	}
		
	return false;
}

int main(int argc, char* argv[])
{
	int linenumber = 1;
	string fName = argv[argc-1];
	ifstream in(fName.c_str());
	
	if(argc > 2)
	{
		cerr << "ONLY ONE FILE NAME ALLOWED ";
		exit(1);
	}
	
	if(!in)
	{
        cerr << "CANNOT OPEN THE FILE: " << fName << endl;
        exit(1);
    }
	
	bool parseProg = Prog(in, linenumber);
	if(parseProg)
	  cout << "Successful Parsing" << endl;
	else
	{
		cout << "Unsuccessful parsing" << endl;
		cout << "Error Count: " << error_count << endl;
	}
}
