#include <string> 
#include <sstream>
#include <map>
#include <iostream>
#include <cstring>
#include <vector>


using namespace std;

class Message
{
	
	private:
	std::string stringifiedJson;
	public:
	//constructor/destructor
	Message();
	~Message();

	//socket opening/binding	
	std::string serialize(map<string,string> monDictJson);
    	std::map<string,string>deserialize(string json);
	

	private:
	int checkComplete(string &frame);
	void findAndReplaceAll(string & data, string toSearch, string replaceStr);
	int getEntities(std::vector<size_t> * vect,std::string & str,std::string toSearch);
	int extractCouple(std::vector<size_t> vectPos,std::vector<string> * vectCouple,string str);
	void degraissage(string & instring,string graisse);
	void storeInMap(std::map<string,string> & map,std::vector<string> vectPos);

};
