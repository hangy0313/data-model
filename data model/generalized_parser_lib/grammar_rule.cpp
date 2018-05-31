#include <vector>
#include "grammar_rule.h"

using namespace std ;

subrule::subrule()
{
	this->function_pointer = NULL ;
	this->delemiter = 0 ;
}

subrule::~subrule()
{
}

//
rule::rule() 
{
}

rule::~rule()
{
}

void rule::push_subrule(subrule *r)
{
	this->sub_rules.push_back(r) ;
}

void rule::pop_subrule()
{
	this->sub_rules.pop_back() ;
}

subrule* rule::operator [](unsigned int index)
{
	return this->sub_rules[index] ;
}

int rule::size()
{
	return this->sub_rules.size() ;
}
//
