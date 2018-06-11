#ifndef PARSE_ERD_H
#define PARSE_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

//map<string, node*> macro_definition_table;

node* transToParseTree(Map* physicalERDMap);
//void setup_token_macro_def(string filename, generalized_parser& parser);
//void macro_definition_input_procedure(string macro_name, string filename);


#endif