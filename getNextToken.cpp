//Giuseppe Celentano
//Programming Assignment 2
//November 8 2020
#include <iostream>  
#include <fstream>      
#include <cstdlib>  // exit prototype
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include "lex.h"
#include "io.cpp"
using namespace std;

LexItem getNextToken(istream& in, int& linenum)
{
	enum TokState {START,INID,INSTRING,ININT,INREAL,INCOMMENT,SIGN} lexstate = START;
	string lexeme = "";
	char ch;
	bool lastToken = false;
	while(in.get(ch))
	{	
		switch(lexstate)
		{
			case START:
				if(ch == '\n')
				{
					linenum++;
					break;
				}
				
				else if(isspace(ch))
					break;
				
				lexeme = ch;
				if(isalpha(ch))
				{
					in.putback(ch);
					lexstate = INID;
					break;
				}
				
				else if(ch == '/' && in.peek() == '/')
                {
                    lexstate = INCOMMENT;
                }
                
                else if( ch == '/' && in.peek() != '/')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == '*')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == '+')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == '-')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == '=')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == '(')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == ')')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == ';')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
                else if(ch == ',')
                {
                    lexstate = SIGN;
                    in.putback(ch);
                }
                
				else if(ch == '"')
				{
					lexstate = INSTRING;
					in.putback(ch);
					lexeme = "";
				}
				
				else if(isdigit(ch))
				{	
					lexstate = ININT;
					break;
				}
				else if(ch == '.')
				{
					lexstate = INREAL;
					break;
				}
				else
				{
	                    return LexItem(ERR, lexeme, linenum);
				}
					
				break;
					
			case SIGN:
				if(ch == '+')
					return LexItem(PLUS,lexeme, linenum);
				if(ch == '-')
					return LexItem(MINUS,lexeme, linenum);
				if(ch == '/')
					return LexItem(DIV,lexeme, linenum);
				if(ch == '*')
					return LexItem(MULT,lexeme, linenum);
				if(ch == '=')
					return LexItem(EQ,lexeme, linenum);
				if(ch == '(')
					return LexItem(LPAREN,lexeme, linenum);
				if(ch == ')')
					return LexItem(RPAREN,lexeme, linenum);
				if(ch == ';')
					return LexItem(SCOMA,lexeme, linenum);
				if(ch == ',')
					return LexItem(COMA,lexeme, linenum);
				
				break;
					
			case INSTRING:
				while(in.get(ch))
	                {
	                    if(ch == '"')
	                    	return LexItem(SCONST, lexeme, linenum);
	                    else if(ch == '\n')
	                    {
	                    	return LexItem(ERR, lexeme, linenum);
						}
						else
	                    	lexeme += ch;
	        		}
	        	break;
			
			case ININT:
				if(isdigit (ch))
					lexeme += ch;
				
				else if(ch == '.')
				{
					if(!(isdigit(in.peek())))
					{
						lexeme += ch;
						return LexItem(ERR, lexeme, linenum);
					}
					else
						lexeme += ch;
						lexstate = INREAL;
						break;
				}
				else
				{
					in.putback(ch);
					return LexItem(ICONST, lexeme, linenum);
				}
				
				break;
				
			case INREAL:
				if(isdigit(ch))
					lexeme += ch;
				else
				{
					in.putback(ch);
					return LexItem(RCONST, lexeme, linenum);
				}
				
				break;
				
			case INCOMMENT:
                while(in.get(ch))
                {
                    if(ch == '\n')
                    {
                    	linenum++;
                    	lexstate = START;
                    	lexeme = "";
                    	break;
					}
        		}
        		
        		break;
        		
			case INID:
				while(in.get(ch))
				{
					if(lexeme == "PRINT" || lexeme == "print")
					{
						in.putback(ch);
	                    return LexItem(PRINT,lexeme, linenum);
	                    break;
	                }
	                else if(lexeme == "END" || lexeme == "end")
					{
						in.putback(ch);
	                    return LexItem(END,lexeme, linenum);
	                    break;
	                }
	                else if(lexeme == "IF" || lexeme == "if")
					{
						in.putback(ch);
	                    return LexItem(IF,lexeme, linenum);
	                    break;
	                }
	                else if(lexeme == "THEN" || lexeme == "then")
					{
						in.putback(ch);
	                    return LexItem(THEN,lexeme, linenum);
	                    break;
	                }
	                else if(lexeme == "BEGIN" || lexeme == "begin")
					{
						in.putback(ch);
						return LexItem(BEGIN,lexeme, linenum);
						break;
					}
					else if(isalnum(ch))
					{
						lexeme += ch;
					}
					else// if(ch == '\n' || ch == ' ' || (!(isalnum(ch))))
					{
						in.putback(ch);
						return LexItem(IDENT, lexeme, linenum);
					}
					
				}
				
			break;
        }
        if(in.peek() == EOF)
        {
        	return LexItem(DONE, lexeme, linenum);
		}
	}
}
