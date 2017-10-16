#include <iostream>
#include "string_map.h"
#include <string>

int main(){

string_map<int> m;

const string g = "gato";
std::pair<const string,int> par(g,1);
m.insert(par);

const string g2 = "gapo";
std::pair<const string,int> par2(g2,1);
m.insert(par2);


const string g3 = "gapora";
std::pair<const string,int> par3(g3,1);
m.insert(par3);

string_map<int>::const_iterator it = m.begin();
//++it;
//std::cout<<it->first<<std::endl;

//std::cout<<it->second<<std::endl;






return 0;
}
