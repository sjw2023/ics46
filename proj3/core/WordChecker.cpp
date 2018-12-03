// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    	//std::cout<<"here"<<std::endl;
	return words.contains(word);return false;
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    //std::cout<<words.size()<<std::endl;
	//std::cout<<"here"<<std::endl;
	std::vector<std::string> result;
	std::string temp;
	for(int i=0;i<word.size();i++)
	{
		temp = "";
		if(i+1 < word.size())
		{
			if(i==0)
			{
				temp += word[i+1];
				temp += word[i];
				temp += word.substr(i+2);
			}
			else if(i==word.size()-1)
			{
				temp += word.substr(0,i);
				temp += word[i+1];
				temp += word[i];
			}
			else
			{
				temp += word.substr(0,i);
				temp += word[i+1];
				temp += word[i];
				temp += word.substr(i+2);
			}
		}
		

		if(wordExists(temp) && std::find(result.begin(),result.end(),temp)==result.end())
			result.push_back(temp);
	}

	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for(int i=0;i<alphabet.size();i++)
	{
		for(int p=0;p<=word.size();p++)
		{
			temp = "";
			if(p==0)
			{
				temp += alphabet[i];
				temp += word;
			}
			else if(p==word.size())
			{
				temp += word;
				temp += alphabet[i];
			}
			else
			{
				temp += word.substr(0,p);
				temp += alphabet[i];
				temp += word.substr(p);
			}
			if(wordExists(temp) && std::find(result.begin(),result.end(),temp)==result.end())
				result.push_back(temp);
		}
	}

	int i = 0;
	while(i<word.size())
	{
		temp = "";
		if(i==0)
			temp += word.substr(i+1);
		else if(i==word.size()-1)
			temp += word.substr(0,word.size()-1);
		else
		{
			temp += word.substr(0,i);
			temp += word.substr(i+1);
		}
		if(wordExists(temp) && std::find(result.begin(),result.end(),temp)==result.end())
			result.push_back(temp);
		i++;
	}

	for(int i=0;i<alphabet.size();i++)
	{
		for(int p=0;p<word.size();p++)
		{
			temp = "";
			if(p==0)
			{
				temp += alphabet[i];
				temp += word.substr(p+1);
			}
			else if(p==word.size())
			{
				temp += word.substr(0,word.size()-1);
				temp += alphabet[i];
			}
			else
			{
				temp += word.substr(0,p);
				temp += alphabet[i];
				temp += word.substr(p+1);
			}
			if(wordExists(temp) && std::find(result.begin(),result.end(),temp)==result.end())
				result.push_back(temp);
		}
	}

	std::string string1;
	std::string string2;
	for(int i=1;i<word.size();i++)
	{
		string1 = word.substr(0,i);
		string2 = word.substr(i);
		if(wordExists(string1 + " " + string2) && std::find(result.begin(),result.end(),string1 + " " + string2)==result.end())
		{
			std::string final = string1 + " " + string2;
			result.push_back(final);
		}
	}
	//std::cout<<result.size()<<std::endl;
    return result;
}

