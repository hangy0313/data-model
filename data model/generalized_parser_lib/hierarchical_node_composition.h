#ifndef HIERARCHICAL_NODE_COMPOSITION_H
#define HIERARCHICAL_NODE_COMPOSITION_H

#include <iostream>
#include "../universal_data_lib/universal_data.h"
#include "../universal_data_lib/universal_data_extend.h"
#include "../universal_data_lib/create_iterator_function.h"

using namespace std ;

class node : public universal_data 
{
public:
	node() ;
	~node() ;
	void deep_delete() ; // deep delete
	void set_construct_name(string nm) ;
	string get_construct_name() ;
	void set_nt_index(int id) ;
	int get_nt_index() ;
	void set_t_nt_flag(char f) ;
	char get_t_nt_flag() ;
	void push_parent_pointer(universal_data* ptr) ;
	universal_data* get_node_parent_pointer() ;
	void pop_parent_pointer() ;
	// attribute list 
	void add_node_attribute(string nm, universal_data ud) ;
	void set_node_attribute(string nm, universal_data ud) ;
	universal_data get_node_attribute(string nm) ;
	void remove_node_attribute(string nm) ;
	Virtual_Iterator* get_node_attribute_iter() ;
	void clear_attributes() ;
	// branch vector 
	void push_node_branch(universal_data* bptr) ;
	universal_data* get_node_branch(int index) ;
	unsigned int node_branch_size() ;
	void pop_node_branch() ; // pop the last element of branch vector
	Virtual_Iterator* get_node_branch_iter() ;
	void clear_branches() ; 
	// auxiliary data 
	void set_auxiliary_data(universal_data ud) ;
	universal_data get_auxiliary_data() ;
	// binding info 
	void push_binding_info(string kind, string nm) ;
	void pop_binding_info() ;
	// binding info iterator 
	void binding_info_begin() ;
	bool binding_info_end() ;
	void binding_info_advance() ;
	string binding_info_kind() ;
	string binding_info_name() ;
protected:
} ;


#endif 
