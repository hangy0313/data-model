#ifndef TOKEN_H
#define TOKEN_H
#include <vector> 
#include "../universal_data_lib/universal_data_lib.h"

using namespace std; 

struct token 
{
	int token_id ;
	UD_universal_data token_specifier ; // can be cast to corresponding type and pointer 
} ;

#endif 
