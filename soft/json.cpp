
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <map>
//#define DEBUG
using namespace std;
int checkComplete(string &frame);
void findAndReplaceAll(string & data, string toSearch, string replaceStr);
int getEntities(std::vector<size_t> * vect,std::string & str,std::string toSearch);
int extractCouple(std::vector<size_t> vectPos,std::vector<string> * vectCouple,string str);
void degraissage(string & instring,string graisse);
void storeInMap(std::map<string,string> & map,std::vector<string> vectPos);
std::map<string,string>deserialize(string json);
int main() 
{
	string line = "{\"lastName\": \"Gnakouri\",\"firstName\": \"Armand\",\"age\": \"39\",\"gender\": \"h\",\"ssn\": \"cpam\",\"phone\": \"065324562\"}";
	

	if(checkComplete(line) !=0)
	{
		printf("Frame ERROR \n");
	}
	else
	{
		printf("Frame OK \n");
	}


std::map<string,string> theMap = deserialize(line);

cout << theMap["lastName"] << endl ; 
cout << theMap["firstName"] << endl ; 
cout << theMap["age"] << endl ; 
cout << theMap["gender"] << endl ; 
cout << theMap["ssn"] << endl ; 
cout << theMap["phone"] << endl ; 

  return 0;

}



std::map<string,string>deserialize(string json)
{

std::vector<size_t> VectorPos; // a vector which contains x couple ( key : value )
getEntities(&VectorPos,json,",");	
std::vector<string> StrVector; // a vector which contains x couple ( key : value )
extractCouple(VectorPos,&StrVector,json);
std::map<string,string> theMap;
storeInMap(theMap,StrVector);
return theMap;
}



int checkComplete(string &frame)
{
#ifdef DEBUG
	printf("find { %d \n",frame.find("{"));
	printf("find } %d \n",frame.find("}"));
#endif
	if(frame.find("{") == -1)
	{
		return -1;
	}
	if(frame.find("}") == -1)
	{
		return -1;
	}
return 0;

}

int getEntities(std::vector<size_t> * vect,std::string & str,std::string toSearch)
{
	
	string data = str;
	int occ = 0;
	size_t pos = data.find(toSearch); // find the first entity
	if(pos == -1)
	{
		printf("bye !! \n");
		return -1; // if not found
		
	}
	else
	{
		vect->push_back(pos); //store the position of the entity
		
		data.erase(pos,1); // erase the entity 
	}
 
	
	while( pos != std::string::npos) // go trough the string 
	{
		
		// Get the next occurrence 
		pos = data.find(toSearch); // look for the next one 
		vect->push_back(pos+occ); //store the position of the entity
		occ++;
		if(pos != std::string::npos )
		{
			data.erase(pos,1); // erase the entity
		} 
		
	}
return 0;
}


int extractCouple(std::vector<size_t> vectPos,std::vector<string> * vectCouple,string str)
{
	int debut = 0;
	string data = str;
	for(int i =0;i<vectPos.size();i++)
	{
		
		string temp =  data.substr (debut,vectPos[i]-debut);
		degraissage(temp,",");
		degraissage(temp,"\"");
		degraissage(temp,"{");
		degraissage(temp,"}");
		degraissage(temp," ");
		vectCouple->push_back(temp);
		debut = vectPos[i];
	}

}

void degraissage(string & instring,string graisse)
{
	size_t pos = instring.find(graisse); //
 
	// Repeat till end is reached
	while( pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		instring.replace(pos, graisse.size(), "");
		// Get the next occurrence from the current position
		pos = instring.find(graisse,pos);
	}
 
}

void storeInMap(std::map<string,string> & map,std::vector<string> vectPos)
{
	string tempstr;
	string key;
	string value;
	for(int i =0;i<vectPos.size();i++)
	{
		tempstr = vectPos[i];
		key = tempstr.substr(0,tempstr.find(":"));		
		value = tempstr.substr(tempstr.find(":")+1);
		map[key] = value;
		/// map->insert (it, std::pair<char,int>('c',400));  // no max efficiency inserting
		//cout << key << endl;
		//cout << value << endl;	
	}

}





