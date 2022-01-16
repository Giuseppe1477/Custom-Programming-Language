//Giuseppe Celentano
//Recitation Assignment 4
//October 11 2020
#include <iostream>       
#include <cstdlib>
#include <string>
#include <map>
#include "lex.h"
using namespace std;

static map <Token, string> tokenPrint = {
{PRINT, "PRINT"}, 
{IF, "IF"},
{THEN, "THEN"},
{BEGIN, "BEGIN"}, 
{END, "END"}, 
{IDENT, "IDENT"},
{ICONST, "ICONST"},
{SCONST, "SCONST"}, 
{RCONST, "RCONST"}, 
{PLUS, "PLUS"}, 
{MINUS, "MINUS"}, 
{MULT, "MULT"}, 
{DIV, "DIV"}, 
{EQ, "EQ"}, 
{LPAREN, "LPAREN"}, 
{RPAREN, "RPAREN"}, 
{SCOMA, "SCOMA"}, 
{COMA, "COMA"}, 
{ERR, "ERR"}, 
{DONE, "DONE"}};

ostream & operator << (ostream &out, const LexItem& tok) 
{ 
    Token t = tok.GetToken();
    out << tokenPrint[t];
    if(t == IDENT || t == ICONST || t == SCONST || t == RCONST || t == ERR || t == DONE)
	{
	 	out << " (" << tok.GetLexeme() << ") " << "Line #: " << tok.GetLinenum();
	}
	else 
		out << " Line #: " << tok.GetLinenum();
    return out; 
} 
