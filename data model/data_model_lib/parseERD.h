#ifndef PARSE_ERD_H
#define PARSE_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

node* transToParseTree(UD_Map* physicalERDMap);

#endif