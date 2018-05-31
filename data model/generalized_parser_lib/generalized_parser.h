#ifndef GENERALIZED_PARSER_H
#define GENERALIZED_PARSER_H

#include <vector>
#include <map>
#include <string>
#include <list>
#include <stack>
#include <fstream>
#include "token.h"
#include "grammar_rule.h"
#include "../universal_data_lib/universal_data.h"
#include "hierarchical_node_composition.h"

using namespace std ;

// global 
map<string, void(*)(node*)> global_action_table ;
//-----------------------------

struct sensitive_elem
{
	string attr_name; 
	string next_level_construct_name ;
	string next_level_attr_name ;
} ;

struct precedence_elem
{
	string type ; // binary_op, unary_op 
	string elem ; 
};

struct node_creation_rule_elem 
{
	node_creation_rule_elem(){nest_print_flag = 'T', node_creation_flag = 'T', attribute_name = "NoName";}
	char nest_print_flag ; // T, F, O, R
	char node_creation_flag ;// T, F
	string attribute_name ; // use when propogate this node 
} ;

struct general_construct_name
{
	string upper_level_construct_name ; // used for left-association
	string lower_level_construct_name ;// used for right-association
} ;

class generalized_parser
{// T is the type of auxiliary data of node 
public : 
	generalized_parser(string grammar_filename, string node_creation_rule_filename, string associativity_filename, string precedence_filename, string general_construct_filename, string action_filename, string binding_filename) ;
	~generalized_parser() ;

	// initialize grammar rule 
	void initialize_grammar_rule( string filename) ; // read file and setup terminal symbol table nonterminal symbol table grammar rule
	// initialize node creation rule 
	void initialize_node_creation_rule(string filename) ; // read file and set up node_creation_rule table 
	// initialize associative 
	void initialize_associativity(string filename) ; // read file and set up associative table 
	// initialize precedence 
	void initialize_precedence(string filename) ; // read file and set up precedence table 
	void initialize_general_constuct_name(string filename) ; // read file and set up general construct table 
	// initialized action 
	void initialize_action_table(string filename) ; // read file and set up action table 
	// initialized binding 
	void initialize_binding_table(string filename) ; // read file and set up binding table 
	
	// parse 
	bool shaped_unified_parse(int construct_id, int current_token_index, int& new_token_index, list<pair<string, unsigned*> >& acc_vector, node* &parse_tree_ptr) ;

	// set up token seq
	void add_token(int token_id, universal_data token_specifier) ; // push back in token seq
	void remove_token() ; // pop back from token seq
	int token_size() ;
	token* get_token(int index) ;

	// get index 
	int get_symbol_index(string s) ; // get the index of construct name 
	// get name
	string get_construct_name(int index) ; // get the construct name with index 

	bool compute_creation_shaping(int construct_id) ;
	bool compute_associativity_shaping(node* nodeptr, node* childptr) ;
	pair<bool, string> compute_precedence_shaping(node* nodeptr, int next_token_index) ;
	// associativity 
	void associativity_restructuring (node* nodeptr) ;
	// precedence 
	node* left_assoc_restructuring(node* nodeptr, int& next_token_index) ;
	node* right_assoc_restructuring(node* nodeptr, int& next_token_index) ;
	// filter 
	void set_nested_filtering(int construct_id) ;
	void restore_nested_filtering(int construct_id) ;

	void print() ; // print all grammars
protected: 
	// grammar rule 
	map<string, int> construct_table; // include terminal, nonterminal, keyword
	int nt_num ;  // if construct id < nt_num, than it is a nonterminal 
	int keyword_bound ; // if  nt_num < construct_id < keyword_bound, than it is a keyword
	int alloc_num ; // after allocation, alloc_num + 1 ; record the allocation number (id)
	vector<rule*> grammar_rules ;
	// node creation table 
	map<string, node_creation_rule_elem> node_creation_table ; // (construct name, node creation rule)
	// associativity rule 
	map<string, list<sensitive_elem> > associativity_table ; // (construct name, sensitive set)
	// precedence rule 
	map<int, list<precedence_elem> > precedence_table ; // ( level, list of element) ; high integer means high level 
	map<string, general_construct_name> general_construct_table ; // (construct name, pair<upper level construct id, lower level construct id> )
	// filter rule 
	stack<bool> global_nest_creation_stack ;
	// token sequence 
	vector<token*> token_seq ;
	// action table 
	map<string, list<string> > action_table ; // construct_name, list of action_name
	// binding table 
	map<string, list< pair<string, string> > > binding_table ; // construct_name, list of pair<kind, name> 
} ;

#endif 
