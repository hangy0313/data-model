#ifndef OUTPUT_GENERATION_H
#define OUTPUT_GENERATION_H

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <list>
#include "../generalized_parser_lib/hierarchical_node_composition.h"
#include "../generalized_parser_lib/generalized_parser.h" // use precedence_elem struct
#include <sstream> 

using namespace std ;

enum traversal_kind {tk_preorder, tk_inorder, tk_postorder} ;
enum output_kind { Func_Name, Attr_Name, String_Value, All_Attribute} ;

struct print_rule_elem
{
	output_kind kind ; 
	bool attr_name_flag ;
	string name ; // name or string 
} ;

struct info_elem
{
	char nest_print_flag  ; 
	traversal_kind kind ;
} ;

class general_output_generation ;

void outputRealCode(node* ptree);

// free function 
void print_node_info_func(string& stream, node* nodeptr, general_output_generation* gog) ; 

struct control_info
{
	int indent_num ;
	bool newline ;
};

class general_output_generation
{
public:
	general_output_generation(string node_print_rule_file, string global_print_table_file, string interleave_vector_file, string enclose_vector_file,string file_root_file, string indent_control_table_file, string condition_function_table_file) ; 
	~general_output_generation() ;

	// initialized
	void initialized_node_print_rule(string filename) ; // specify how to show the node info 
	void initialized_global_print_table(string filename) ; // include nest_print_flag and traversal kind 
	void initialized_interleave_vector(string filename) ;// specify interleave 
	void initialized_enclose_vector(string filename) ; // specify the enclose 
	void initialized_file_root(string filename) ; // specify the root in the file 
	void initialized_indent_control_table(string filename) ; // specify indent and newline
	void initialized_condition_function_table(string filename) ; // specify use what condition function 
	void set_nested_printing(node* nodeptr) ; // set up nest print flag
	void restore_nested_printing(node* nodeptr) ; // restore nest print flag 
	traversal_kind find_traversal_kind(node* nodeptr) ; // get the traverse kind with construct name 
	print_rule_elem get_print_rule_elem(node* nodeptr) ; // get the print rule which specify how to show the node info 
	pair<string, string> get_enclose(node* nodeptr) ; // get the enclose 

	void output_generation( node* nodeptr) ;
	void traverse_preorder( node* nodeptr) ;
	void traverse_postorder(node* nodeptr) ;
	void traverse_inorder( node* nodeptr) ;
	void print_node_info( node* nodeptr, int count) ; 

	// set up function pointer
	void set_fptr(string func_name, void(*fptr)(string&, node*) ) ; // when show node info is a function name , find the function pointer by this function 
	void (*get_fptr(string func_name))(string&, node*) ; // get the function pointer
	void remove_fptr(string func_name) ;

	// set up node print function 
	// set up two funciton for print node info; one for count is 0, one for another 
	void set_node_print_function(string construct, void(*fptr1)(string&, node*, general_output_generation*), void (*fptr2)(string&, node*, general_output_generation*)) ; 
	void remove_node_print_funciton(string construct) ;

	void reset_stream_table() ; // reset stream table 
	void output_file(bool single_file) ; // output stream to file 
	void output_file(char* filename) ; // output stream to given filename 

	// setup condition function 
	void set_condition_fptr(string func_name, bool(*fptr)(node*) ) ; 
	bool (*get_condition_fptr(string func_name))(node*) ; // get condition function pointer 

protected: 
	list<string> file_root_list; // list what construct will be a root 
	stack<string> stream_stack ; // record the construct name of current stream  
	list<pair<string, string> > stream_table; // record the file name (construct name + address of current node) and output stream
	map<string, int> accumulated_map; // record the construct name and count 
	map<string, pair<void(*)(string&, node*, general_output_generation*), void(*)(string&, node*, general_output_generation*)> > node_print_function_table ;

	map<string, void (*)(string&, node*) > fptr_table ;  // for show node info with function name 
	map<string, print_rule_elem> node_print_rule; 
	map<string, info_elem> global_print_table ;
	map<string, vector<string> > NT_interleave_vector ;
	map<string, pair<string, string> > enclose_vector ;
//	vector<string > attr_enclose_vector ;
	stack<bool> global_nest_print_flag ;
	// indent control table 
	map<string, control_info> indent_control_table ; // if not in the table; use current indent 
	int indent_counter ;
	// condition function 
	map<string, bool(*)(node*)> condition_function ;
	map<string, string> cfptr_mapping_table ; // construct name -> condition function name ;; if not found , default condition function result is false
} ;

// precedence 
map<int, list<precedence_elem> > precedence_table ; // ( level, list of element) ; high integer means high level 

void initialized_precedence_table(string filename) ;// specify precedence

bool less_than(node* nodeptr) ;

#endif 
