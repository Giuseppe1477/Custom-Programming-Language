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
#include "getNextToken.cpp"
using namespace std;

int main(int argc, char* argv[])
{
	string arguments[5] = {"-v", "-iconsts", "-rconsts", "-sconsts", "-ids"};
	int lineNum = 1;
	int tokens = 0;
	bool v = false;
	bool iconst = false;
	bool rconst = false;
	bool sconst = false;
	bool ids = false;
	LexItem tok;
	string fName = argv[argc-1];
	ifstream myFile(fName.c_str());
    set <string> idSet;
    set <float> realSet;
    set <int> intSet;
    set <string> strSet;
	
	if(!myFile)
	{
        cout << "CANNOT OPEN THE FILE: " << fName << endl;
        exit(1);
    }
	
	for(int i = 1; i < argc-1; i++)
	{
		string tempString = argv[i];
		if(tempString.find(arguments[0]) != string::npos)
		{
			if(tempString.find("-") != string::npos)
			{
				v = true;
			}
			else
			{
				cerr << "UNRECOGNIZED FLAG: " << argv[i];
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[1]) != string::npos)
   		{
			if(tempString.find("-") != string::npos)
			{
				iconst = true;
			}
			else
			{
				cerr << "UNRECOGNIZED FLAG: " << argv[i];
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[2]) != string::npos)
    	{
			if(tempString.find("-") != string::npos)
			{
				rconst = true;
			}
			else
			{
				cerr << "UNRECOGNIZED FLAG: " << argv[i];
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[3]) != string::npos)
    	{
			if(tempString.find("-") != string::npos)
			{
				sconst = true;
			}
			else
			{
				cerr << "UNRECOGNIZED FLAG: " << argv[i];
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[4]) != string::npos)
    	{
			if(tempString.find("-") != string::npos)
			{
				ids = true;
			}
			else
			{
				cerr << "UNRECOGNIZED FLAG: " << argv[i];
				exit(1);
			}
		}
		
		else if(!(tempString.find("-") != string::npos))
		{
			cerr << "ONLY ONE FILE NAME ALLOWED ";
			exit(1);
		}
		
		else
		{
			cerr << "UNRECOGNIZED FLAG: " << argv[i];
			exit(1);
		}
	}
	
	while((tok = getNextToken(myFile,lineNum)) != DONE && tok != ERR)
   	{
       	tokens++;  	
       	if(v)
		{
			cout << tok << endl;
		}
		if(tok.GetToken() == IDENT)
		{
			idSet.insert(tok.GetLexeme());
		}
		if(tok.GetToken() == ICONST)
		{
			intSet.insert(stoi(tok.GetLexeme()));
		}
		if(tok.GetToken() == RCONST)
		{
			realSet.insert(stof(tok.GetLexeme()));
		}
		if(tok.GetToken() == SCONST)
		{
			strSet.insert(tok.GetLexeme());
		}
   	}
   	
   	if(sconst)
	{
	    cout << "\nSTRINGS:\n";
	    for (auto it = strSet.begin(); it != strSet.end(); ++it)
        	cout << *it << " ";
	}
		
	if(iconst)
	{
	    cout << "\nINTEGERS:\n";
	    for (auto it = intSet.begin(); it != intSet.end(); ++it)
        	cout << *it << " ";	
	}
		
	if(rconst)
	{
	    cout << "\nREALS:\n";
	    for (auto it = realSet.begin(); it != realSet.end(); ++it)
        	cout << *it << " ";	
	}
		
	if(ids)
	{
	    cout << "\nIDENTIFIERS:\n";
	    for (auto it = idSet.begin(); it != idSet.end(); ++it)
        	cout << *it << " ";		
	}
   	
   	if(tok.GetToken() == ERR)
   	{
   		cout << "Error on Line: " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")" << endl;
	}
	else
	{
		cout << endl << endl << "Tokens: " << tokens << endl;
	   	cout << "Line Amount: " << lineNum << endl;
	}
}
