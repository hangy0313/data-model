#ifndef GRAMMAR_RULE_H
#define GRAMMAR_RULE_H
#include <string>
#include <map>
#include <list> 

using namespace std ;

struct element 
{
	element(){t_nt_flag = ' '; t_nt_index = -1; add_flag = false; star_flag = false;}
	char t_nt_flag; 
	int t_nt_index;
	bool add_flag; 
	bool star_flag;
} ;

class subrule
{
public: 
	subrule() ;
	~subrule() ;

	vector<element> element_vector ;
	int delemiter ; 
	unsigned* function_pointer ; 

protected: 
} ;

class rule 
{
public: 
	rule() ;
	~rule() ;

	void push_subrule(subrule* r) ;
	void pop_subrule() ;
	subrule* operator[] (unsigned index) ;
	int size() ;
protected: 
	vector<subrule*> sub_rules ;
} ;

#endif 
