//Giuseppe Celentano
//Programming Assignment 1
//October 11 2020
#include <iostream>  
#include <fstream>      
#include <cstdlib>  // exit prototype
#include <string>
#include <algorithm>
#include <bits/stdc++.h> 
using namespace std;

void indentFile(int indentSpace)//function to indent the file certain amount
{
	for(int i = 0; i < indentSpace; i++)
		cout << " ";
}

void addLineSpace(int spacingAmt)//function to end line the file a specified amount
{
	for(int i = 0; i < spacingAmt; i++)
		cout << endl;
}

int main(int argc, char* argv[])
{
	string arguments[4] = {"-LL", "-IN", "-WC", "-SP"};
	int numWords = 0;
	int lineLength = 20;
	int indent = 5;
	int wordCount = 50;
	int lineSpace = 1;
	int tempInt = 0;
	int currentLength = 0;
	bool tooLong = false;
	string tempString = "";
    string fName = "";
    
	fName = argv[1];
	for(int i = 2; i < argc; i++)
	{
		tempString = argv[i];
		if(tempString.find(arguments[0]) != string::npos)
		{
			if(tempString.find("=") != string::npos)//check for = sign
			{
				tempString.erase(0,4);//erase all characters except for the numbers
				tempInt = stoi(tempString);// c++11 string to int function
				if(tempInt > 0)
					lineLength = tempInt;
				else
				{
					cout << "VALUE NOT INTEGER greater than 0";
					exit(1);
				}
			}
			else
			{
				cout << "MISSING = SIGN flag";
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[1]) != string::npos)
   		{
			if(tempString.find("=") != string::npos)
			{
				tempString.erase(0,4);
				tempInt = stoi(tempString);
				if(tempInt > 0)
					indent = tempInt;
				else
				{
					cout << "VALUE NOT INTEGER greater than 0";
					exit(1);
				}
			}
			else
			{
				cout << "MISSING = SIGN flag";
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[2]) != string::npos)
    	{
			if(tempString.find("=") != string::npos)
			{
				tempString.erase(0,4);
				tempInt = stoi(tempString);
				if(tempInt > 0)
					wordCount = tempInt;
				else
				{
					cout << "VALUE NOT INTEGER greater than 0";
					exit(1);
				}
			}
			else
			{
				cout << "MISSING = SIGN flag";
				exit(1);
			}
		}
		
		else if(tempString.find(arguments[3]) != string::npos)
    	{
			if(tempString.find("=") != string::npos)
			{
				tempString.erase(0,4);
				tempInt = stoi(tempString);
				if(tempInt > 0)
					lineSpace = tempInt;
				else
				{
					cout << "VALUE NOT INTEGER greater than 0";
					exit(1);
				}
			}
			else
			{
				cout << "MISSING = SIGN flag";
				exit(1);
			}
		}
		
		else
		{
			cout << "UNRECOGNIZED FLAG flag";
			exit(1);
		}
	}
    
    ifstream myFile(fName.c_str());
	if(!myFile)
	{
        cerr << "UNABLE TO OPEN FILE " << fName << endl;
        exit(1);
    } 
	
	tempString = "";
	indentFile(indent);
	currentLength = indent;
	bool firstWord = true;
	bool beginParagraph = true;
	while (getline(myFile,tempString))
    {
    	if(tempString.empty() && beginParagraph == true)//checks for new paragraph
    		{
	    		addLineSpace(lineSpace);
	    		cout << endl;
	    		indentFile(indent);
	    		beginParagraph = false;
	    		firstWord == true;
	    		currentLength = 0;
			}
	    stringstream tempStream(tempString.c_str());
	    string tempWord;
	    while (tempStream >> tempWord)// checks to make sure its not more than the wordcount
	    {
	    	if(numWords < wordCount)
	    	{
	    		if(firstWord == true)//for single word lines
	    		{
	    			cout << tempWord;
	    			currentLength += 1 + tempWord.length();
			    	cout << " ";
			    	numWords++;
			    	beginParagraph = true;
				}
	    		else if ((currentLength + tempWord.length()) <= lineLength)//check for line length
	    		{
		    		cout << tempWord;
		    		currentLength += 1 + tempWord.length();
			    	cout << " ";
			    	numWords++;
			    	beginParagraph = true;
				}
				else//ends line and starts new one
				{
					currentLength = 0;
					addLineSpace(lineSpace);
					cout << tempWord;
					numWords++;
					cout << " ";
					currentLength += 1 + tempWord.length();
					beginParagraph = true;
				}
				firstWord = false;
			}
			else
				tooLong = true;
		}
	}
	
	cout << endl << endl << endl << "The maximum allowed number of words is " << wordCount << endl;
	if(tooLong == true)
		cout << "The read file contains number of words exceeding the allowed maximum. It has been truncated accordingly." << endl << endl;
	else
		cout << "The read file contains " << numWords << " words." << endl << endl;
	
    return 0;
}
