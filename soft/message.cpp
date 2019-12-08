#include <string.h> 
#include <map>
#include <iostream>
#include <sstream>
#include <message.h>

using namespace std;

Message::Message()
{

}

Message::~Message()
{

}

//////////////////////////////////////////////////////////////////////////////////PARTIE SERIALIZATION////////////////////////////////////////////////////////////


std::string Message::serialize(map<string,string> monDictJson)
{
	string stringifiedJson;
	stringstream ss;

	for (auto const& x : monDictJson)
	{
		// ------ Concaténation dans un string des key et des values --------
		ss << "\""
				<< x.first  // string (key)
				<< ':' 
				<< x.second // string's value 
				<< "\",";
		stringifiedJson = ss.str();

		//----------------- Ajout des {} ------------------------------------
		stringifiedJson.insert(0,1,'{');
		stringifiedJson.replace(stringifiedJson.size()-1,1,"}");

	}
	return stringifiedJson;
}



//////////////////////////////////////////////////////////////////////////////////PARTIE DESERIALIZATION////////////////////////////////////////////////////////////


std::map<string,string> Message::deserialize(string json)
{

std::vector<size_t> VectorPos; // a vector which contains x couple ( key : value )
getEntities(&VectorPos,json,",");	
std::vector<string> StrVector; // a vector which contains x couple ( key : value )
extractCouple(VectorPos,&StrVector,json);
std::map<string,string> theMap;
storeInMap(theMap,StrVector);
return theMap;
}



int Message::checkComplete(string &frame)
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

int Message::getEntities(std::vector<size_t> * vect,std::string & str,std::string toSearch)
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


int Message::extractCouple(std::vector<size_t> vectPos,std::vector<string> * vectCouple,string str)
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

void Message::degraissage(string & instring,string graisse)
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

void Message::storeInMap(std::map<string,string> & map,std::vector<string> vectPos)
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




