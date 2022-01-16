//Giuseppe Celentano
//Programming Assignment 4
//December 9 2020
#include "parseRun.h"
#include "val.h"
#include <iostream>  
#include <fstream>      
#include <cstdlib>  // exit prototype
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include "lex.h"
#include "lex.cpp"
using namespace std;

//Program is: Prog := begin StmtList end
bool Prog(istream& in, int& line)
{
	bool sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Prog" << endl;
	
	if (tok.GetToken() == BEGIN) 
	{
		sl = StmtList(in, line);
		if( !sl  )
			ParseError(line, "No statements in program");
		if( error_count > 0 )
			return false;
	}
	else if(tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == END)
		return true;
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
		return false;
}

// StmtList is a Stmt followed by semicolon followed by a StmtList
 bool StmtList(istream& in, int& line) 
 {
 	//cout << "in StmtList" << endl;
	bool status = Stmt(in, line);
	
	if( !status )
		return false;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if( tok == SCOMA ) 
	{
		status = StmtList(in, line);
	}
	else if (tok == ERR) 
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if (tok == END)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else 
	{
		ParseError(line, "Missing semicolon");
		return false;
	}
	
	return status;
}

//Stmt is either a PrintStmt, IfStmt, or an AssigStmt
bool Stmt(istream& in, int& line) 
{
	bool status;
	//cout << "in Stmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) 
	{

	case PRINT:
		status = PrintStmt(in, line);
		//cout << "status: " << (status? true:false) <<endl;
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
		return true;
	case ERR:
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
		
	case DONE:
		return false;

	default:
		ParseError(line, "Invalid statement");
		return false;
	}

	return status;
}

//PrintStmt:= print ExpreList 
bool PrintStmt(istream& in, int& line) 
{
	//cout << "in print" << endl;
	/*create an empty queue of Value objects.*/
	ValQue = new queue<Value>;
	bool ex = ExprList(in, line);
	if( !ex ) 
	{
		ParseError(line, "Missing expression after print");
		//Empty the queue and delete.
		while(!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}
	
	//Evaluate: print out the list of expressions' values
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() == SCOMA)
	{
		//Execute the statement after making sure
		//the semicolon is seen.
		//cout << "before print val" << endl;
		while (!(*ValQue).empty())
		{
			Value nextVal = (*ValQue).front();
			//cout << "about to print val" << endl;
			cout << nextVal;
			ValQue->pop();
		}
		cout << endl;
	}
	Parser::PushBackToken(t);
	return ex;
}

//IfStmt:= if (Expr) then Stmt
bool IfStmt(istream& in, int& line) 
{
	bool ex = false;
	Value newval;
	LexItem t = Parser::GetNextToken(in, line);
	if( t != LPAREN )
	{
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	ex = Expr(in, line, newval);
	//check the remaining syntax of IfStmt
	if( !ex ) 
	{
		ParseError(line, "Missing if statement expression");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != RPAREN ) 
	{
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	if((t=Parser::GetNextToken(in, line)) != THEN ) {
		
		ParseError(line, "Missing THEN");
		return false;
	}

	//Evaluate: execute the if statement
	//Check type of the expr
	bool st = true;
	if(newval.IsInt())
	{
		//check syntax of stmt and interpret the stmt
		if(newval.GetInt() != 0)
		{
			st = Stmt(in, line);
			//cout << "val of st: " << st << endl;
			if( !st ) 
			{
				ParseError(line, "Missing statement for if");
				return false;
			}
		}
		else
		{
			while(t.GetToken() != SCOMA)
			{
				t = Parser::GetNextToken(in, line);
				if(t.GetToken() == SCOMA)
				{
					Parser::PushBackToken(t);
					break;
				}
			}
			return st;
		}
	}
	else 
	{
		ParseError(line, "Runtime-Error Statement not Int");
		return false;
	}
	
	return st;
}

//Var:= ident
bool Var(istream& in, int& line, LexItem & tok)
{
	//called only from the AssignStmt function
	string identstr;
	//cout << "in Var" << endl;
	tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT)
	{
		identstr = tok.GetLexeme();
		if(!(defVar[identstr]))
            defVar[identstr] = true;
		return true;
	}
	else if(tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) 
{
	//cout << "in AssignStmt" << endl;
	bool varstatus = false, status = false;
	LexItem t;
	string identStr;
	Value val1;
	varstatus = Var(in, line, t);
	//cout << "varstatus:" << varstatus << endl;
	
	if (varstatus)
	{
		identStr = t.GetLexeme();
		if ((t=Parser::GetNextToken(in, line)) == EQ)
		{
			status = Expr(in, line, val1);
			if(!status)
			{
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator =");
			return false;
		}
	}
	else 
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	
	if(symbolTable.find(identStr) != symbolTable.end())
	{
		Value foundVal = symbolTable.find(identStr)->second;
		if(foundVal.GetType() == val1.GetType())
		{
			//cout << "re-assigned val: " << identStr << " - " << val1 << endl;
			symbolTable[identStr] = val1;
		}
		
		else if((foundVal.IsInt() && val1.IsReal()) || (foundVal.IsReal() && val1.IsInt()))
		{
			//cout << "re-assigned val: " << identStr << " - " << val1 << endl;
			symbolTable[identStr] = val1;
		}
		
		else
		{
			ParseError(line, "Runtime-Error Re-assignment type does not match");
			return false;
		}
	}
	else
	{
		//cout << "assigned val: " << identStr << " - " << val1 << endl;
		symbolTable.insert({identStr, val1});
	}
	
	return status;	
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) 
{
	bool status = false;
	//cout << "in ExprList" << endl;
	Value val2;
	status = Expr(in, line, val2);
	if(!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}
	else
		ValQue->push(val2);
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMA) 
	{
		//cout << "calling exprlist again" << endl;
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) 
{
	//cout << "in Expr" << endl;
	bool t1 = Term(in, line, retVal);
	LexItem tok;
	if( !t1 ) 
	{
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		LexItem tempTok = tok;
		Value tempVal;
		t1 = Term(in, line, tempVal);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		//Evaluate: evaluate the expression for addition or subtraction
		if(tempTok.GetToken() == PLUS)
		{
			if(tempVal.IsStr() || retVal.IsStr())
			{
				ParseError(line, "Runtime-Error Cannot add strings");
				return false;
			}
			//cout << "adding vals " << endl;
			retVal = retVal + tempVal;
		}
		if(tempTok.GetToken() == MINUS)
		{
			if(tempVal.IsStr() || retVal.IsStr())
			{
				ParseError(line, "Runtime-Error Cannot subtract strings");
				return false;
			}
			//cout << "subtracting vals " << endl;
			retVal = retVal - tempVal;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

//Term:= Factor {(*|/) Factor}
bool Term(istream& in, int& line, Value & retVal) 
{
	//cout << "in Term" << endl;
	bool t1 = Factor(in, line, retVal);
	//cout << "retVal " << retVal << endl;
	LexItem tok;
	//cout << "status of factor1: " << t1 << endl;
	if( !t1 ) 
	{
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	
	if(tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	while ( tok.GetToken() == MULT || tok.GetToken() == DIV  )
	{
		Value tempVal;
		LexItem tempTok = tok;
		t1 = Factor(in, line, tempVal);
		//cout << "status of factor2: " << t1 << endl;
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after operator");
			return false;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		//Evaluate: evaluate the expression for multiplication or division
		if(tempTok.GetToken() == MULT)
		{
			if(tempVal.IsStr() || retVal.IsStr())
			{
				ParseError(line, "Runtime-Error Cannot multiply strings");
				return false;
			}
			//cout << "multiplying vals " << endl;
			retVal = retVal * tempVal;
		}
		if(tempTok.GetToken() == DIV)
		{
			if(tempVal.IsStr() || retVal.IsStr())
			{
				ParseError(line, "Runtime-Error Cannot divide strings");
				return false;
			}
			//cout << "dividing vals " << endl;
			retVal = retVal / tempVal;
		}
	}
	
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, Value & retVal) 
{
	//cout << "in factor" << endl;
	LexItem tok = Parser::GetNextToken(in, line);
	if( tok == IDENT ) 
	{
		string identstr = tok.GetLexeme();
		//check if the var is defined
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undefined Variable");
			return false;
		}
		else
		{
			retVal = symbolTable.find(identstr)->second;
			return true;
		}
	}
	else if( tok == ICONST ) 
	{
		//convert the string of digits to an integer number
		//create a Value object for ICONST
		int val;
		string lexeme = tok.GetLexeme();
		val = stoi(tok.GetLexeme());
		//cout << "integer const: " << val << endl;
		Value newVal(val);
		retVal = newVal;
		return true;
	}
	else if(tok == SCONST)
	{
		//create a Val object for SICONST and enter into sysmol table
		string lexeme = tok.GetLexeme();
		//cout << "String const: " << lexeme << endl;
		Value newVal(lexeme);
		retVal = newVal;
		return true;
	}
	else if( tok == RCONST ) 
	{
		//convert the string of digits to real number
		//create a Val object for RCONST and enter into sysmol table
		float val;
		string lexeme = tok.GetLexeme();
		val = stof(tok.GetLexeme());
		//cout << "Real const: " << val << endl;
		Value newVal(val);
		retVal = newVal;
		return true;
	}
	else if( tok == LPAREN ) 
	{
		bool ex = Expr(in, line, retVal);
		if( !ex ) 
		{
			ParseError(line, "Missing expression after (");
			return false;
		}
		
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;

		ParseError(line, "Missing ) after expression");
		return false;
	}
	else if(tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	ParseError(line, "Unrecognized input");
	return 0;
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
	  cout << "Successful Execution" << endl;
	  
	else
	{
		cout << "Unsuccessful interpretation" << endl;
		cout << "Error Count: " << error_count << endl;
	}
}

