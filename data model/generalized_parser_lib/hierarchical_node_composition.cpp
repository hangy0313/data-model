#include <iostream>
#include "hierarchical_node_composition.h"

using namespace std ;

node::node(){
	// set up type tag 
	set_type_tag(T_user_defined) ;
	// set up user defined type tag
	set_user_defined_type_tag("node") ;
	
	universal_data empty ;
	// create attribute construct name 
	add_attribute("construct_name", empty) ;
	// create attribute t_nt_index
	add_attribute("nt_index", empty); 
	// create attribute t_nt_flag
	add_attribute("t_nt_flag", empty) ;
	// create attribute parent_pointer 
	pointer_list ptmp ;
	add_attribute("parent_pointer", ptmp) ;
	// create attribute attribute_list 
	associate_list atmp ;
	add_attribute("attribute_list", atmp) ;
	// create attribute auxiliary data 
	add_attribute("auxiliary_data", empty) ;
	// create attribute binding_info 
	pair_string_list pstmp ;
	add_attribute("binding_info", pstmp) ;
	// create architectrue 
	this->data_ptr = (unsigned*)(new Vector()) ;
}

node::~node(){
}

void node::deep_delete(){
	Vector* tmp = (Vector*)(this->data_ptr) ;
	for(tmp->begin() ; !tmp->end() ; (*tmp)++)
		((node*)(*(*tmp)))->deep_delete() ;
	while(tmp->size() != 0)
		tmp->pop_back() ;
}

void node::set_construct_name(std::string nm){
	String tmp ;
	tmp.set_value(nm) ;
	set_attribute("construct_name", tmp) ;
}

string node::get_construct_name(){
	universal_data tmp = find_attribute("construct_name") ;
	return *( ((String*)(&tmp))->getptr() ) ;
}

void node::set_nt_index(int id){
	Int tmp ;
	tmp.set_value(id) ;
	set_attribute("nt_index", tmp) ;
}

int node::get_nt_index(){
	universal_data tmp = find_attribute("nt_index") ;
	return *(((Int*)(&tmp))->getptr()) ;
}

void node::set_t_nt_flag(char f){
	Char tmp ;
	tmp.set_value(f) ;
	set_attribute("t_nt_flag", tmp) ;
}

char node::get_t_nt_flag(){
	universal_data tmp = find_attribute("t_nt_flag") ;
	return *( ((Char*)(&tmp))->getptr() ) ;
}

void node::push_parent_pointer(universal_data* ptr){
	pointer_list* tmp = (pointer_list*)(find_attribute_ref("parent_pointer")) ;
	tmp->begin() ;
	if (tmp->end())
		tmp->add(ptr) ;
	else{
		tmp->remove(*(*tmp)) ;
		tmp->add(ptr) ;
	}
}

universal_data* node::get_node_parent_pointer(){
	pointer_list* tmp = (pointer_list*)(find_attribute_ref("parent_pointer")) ;
	tmp->begin() ;
	if(tmp->end())
		return NULL ;
	else 
		return *(*tmp) ;
}

void node::pop_parent_pointer(){
	pointer_list* tmp = (pointer_list*)(find_attribute_ref("parent_pointer")) ;
	tmp->begin() ;
	if(! tmp->end())
		tmp->remove(*(*tmp)) ;
}

void node::add_node_attribute(std::string nm, universal_data ud){
	associate_list* tmp = (associate_list*)(find_attribute_ref("attribute_list")) ;
	for(tmp->begin(); !tmp->end() ; (*tmp)++)
		if(tmp->get_name() == nm) 
			break ;
	if(tmp->end())
		tmp->add(nm, ud) ;
}

void node::set_node_attribute(std::string nm, universal_data ud){
	associate_list* tmp = (associate_list*)(find_attribute_ref("attribute_list")) ;
	for(tmp->begin() ; !tmp->end(); (*tmp)++)
		if(tmp->get_name() == nm)
			tmp->set(nm, ud) ;
}

universal_data node::get_node_attribute(std::string nm){
	associate_list* tmp = (associate_list*)(find_attribute_ref("attribute_list")) ;
	for(tmp->begin(); !tmp->end() ; (*tmp)++)
		if(tmp->get_name() == nm)
			return tmp->get_value() ;

	universal_data empty ;
	return empty ;
}

void node::remove_node_attribute(std::string nm){
	associate_list* tmp = (associate_list*)(find_attribute_ref("attribute_list")) ;
	for(tmp->begin() ; !tmp->end() ; (*tmp)++)
		if(tmp->get_name() == nm)
			break ;
	if(!tmp->end())
		tmp->remove(nm) ;
}

Virtual_Iterator* node::get_node_attribute_iter(){
	return create_iterator(find_attribute_ref("attribute_list")) ;
}

void node::clear_attributes(){
	associate_list* tmp = (associate_list*)(find_attribute_ref("attribute_list")) ;
	tmp->begin() ;
	while (!tmp->end()){
		tmp->remove( tmp->get_name()) ;
		tmp->begin() ;
	}
}

void node::push_node_branch(universal_data *bptr){
	Vector* tmp = (Vector*)(this->data_ptr) ;
	tmp->push_back(bptr) ;
}

universal_data* node::get_node_branch(int index){
	Vector* tmp = (Vector*)(this->data_ptr) ;
	return (*tmp)[index] ;
}

unsigned int node::node_branch_size(){
	Vector* tmp = (Vector*)(this->data_ptr) ;
	return tmp->size() ;
}

void node::pop_node_branch(){
	Vector* tmp = (Vector*)(this->data_ptr) ;
	tmp->pop_back() ;
}

Virtual_Iterator* node::get_node_branch_iter(){
	return create_iterator((universal_data*)(this->data_ptr)) ;
}

void node::clear_branches(){
	Vector* tmp = (Vector*)(this->data_ptr) ;
	tmp->begin() ;
	while(!tmp->end()){
		tmp->pop_back() ;
		tmp->begin() ;
	}
}

void node::set_auxiliary_data(universal_data ud){
	set_attribute("auxiliary_data", ud) ;
}

universal_data node::get_auxiliary_data(){
	return find_attribute("auxiliary_data") ;
}

void node::push_binding_info(std::string kind, std::string nm){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	tmp->push(kind, nm) ;
}

void node::pop_binding_info(){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	tmp->pop() ;
}

void node::binding_info_begin(){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	tmp->begin() ;
}

bool node::binding_info_end(){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	return tmp->end() ;
}

void node::binding_info_advance(){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	(*tmp)++ ;
}

string node::binding_info_kind(){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	return tmp->first() ;
}

string node::binding_info_name(){
	pair_string_list* tmp = (pair_string_list*)(find_attribute_ref("binding_info")) ;
	return tmp->second() ;
}
