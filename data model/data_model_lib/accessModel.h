#ifndef ACCESS_MODEL_H
#define ACCESS_MODEL_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, string pattern);

/*
 *  access table for data access model transfer
 */
Attribute_List* createSpecializeAccessTable(ERD* erd);
Attribute_List* createLibraryAccessTable(TransformedERD* transERD);
Attribute_List* createLanguageAccessTable(Map* physicalERDMap);

/*
 *  for macro
 */
Attribute_List* languageAccessTable = new Attribute_List();
string findFunctionName(string Ename, string Rname, string prefix);
string findFunctionName(string Rname, string prefix);
string findTagetType(string Ename, string Rname);

#endif