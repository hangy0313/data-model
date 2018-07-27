/* --------------------------------------------------------------- */
/* (C) Copyright 2018	                                           */
/* Owner: Tsung Lee, Yao-Yi Huang							   	   */
/* All Rights Reserved.                                            */
/*																   */
/* --------------------------------------------------------------- */
#include <iostream>
#include "hierarchical_node_composition.h"

using namespace std ;

node::node(){
	// set up type tag 
	set_type_tag(T_user_defined) ;
	// set up user defined type tag
	set_user_defined_type_tag("node") ;
	
	UD_universal_data empty ;
	// create attribute construct name 
	add_attribute("construct_name", empty) ;
	// create attribute t_nt_index
	add_attribute("nt_index", empty); 
	// create attribute t_nt_flag
	add_attribute("t_nt_flag", empty) ;
	// create attribute parent_pointer 
	UD_List ptmp ; // list of parent pointer 
	add_attribute("parent_pointer", ptmp) ;
	// create attribute attribute_list 
	UD_Attribute_List atmp ;
	add_attribute("attribute_list", atmp) ;
	// create attribute auxiliary data 
	add_attribute("auxiliary_data", empty) ;
	// create attribute binding_info 
	UD_List pstmp ; // list of pair (string, string) 
	add_attribute("binding_info", pstmp) ;
	// create architectrue 
	this->data_ptr = (unsigned*)(new UD_Vector()) ;
}

node::~node(){
}

void node::deep_delete(){
	UD_Vector* tmp = (UD_Vector*)(this->data_ptr) ;
	for(tmp->begin() ; !tmp->end() ; (*tmp)++)
		((node*)(*(*tmp)))->deep_delete() ;
	while(tmp->size() != 0)
		tmp->pop_back() ;
}

void node::set_construct_name(std::string nm){
	UD_String tmp ;
	tmp.set_value(nm) ;
	set_attribute("construct_name", tmp) ;
}

string node::get_construct_name(){
	UD_universal_data tmp = find_attribute("construct_name") ;
	return *( ((UD_String*)(&tmp))->getptr() ) ;
}

void node::set_nt_index(int id){
	UD_Int tmp ;
	tmp.set_value(id) ;
	set_attribute("nt_index", tmp) ;
}

int node::get_nt_index(){
	UD_universal_data tmp = find_attribute("nt_index") ;
	return *(((UD_Int*)(&tmp))->getptr()) ;
}

void node::set_t_nt_flag(char f){
	UD_Char tmp ;
	tmp.set_value(f) ;
	set_attribute("t_nt_flag", tmp) ;
}

char node::get_t_nt_flag(){
	UD_universal_data tmp = find_attribute("t_nt_flag") ;
	return *( ((UD_Char*)(&tmp))->getptr() ) ;
}

void node::push_parent_pointer(UD_universal_data* ptr){
	UD_List* tmp = (UD_List*)(find_attribute_ref("parent_pointer")) ;
	if (tmp->size() == 0)
		tmp->push_back(ptr) ;
	else 
	{
		tmp->pop_back() ;
		tmp->push_back(ptr) ;
	}
}

UD_universal_data* node::get_node_parent_pointer(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("parent_pointer")) ;
	if(tmp->size() == 0)
		return NULL ;
	else 
	{
		tmp->begin() ;
		UD_universal_data* rptr = *(*tmp) ;
		return rptr ;
	}
}

void node::pop_parent_pointer(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("parent_pointer")) ;
	if(tmp->size() != 0)
		tmp->pop_back() ;
}

void node::add_node_attribute(std::string nm, UD_universal_data ud){
	UD_Attribute_List* tmp = (UD_Attribute_List*)(find_attribute_ref("attribute_list")) ;

	if(tmp->get_attribute_ref_al(nm) == NULL) 
		tmp->add_attribute_al(nm, ud) ;
}

void node::set_node_attribute(std::string nm, UD_universal_data ud){
	UD_Attribute_List* tmp = (UD_Attribute_List*)(find_attribute_ref("attribute_list")) ;
	if(tmp->get_attribute_ref_al(nm) != NULL)
		tmp->set_attribute_al(nm, ud) ;
}

UD_universal_data node::get_node_attribute(std::string nm){
	UD_Attribute_List* tmp = (UD_Attribute_List*)(find_attribute_ref("attribute_list")) ;
	if(tmp->get_attribute_ref_al(nm)!= NULL)
		return tmp->get_attribute_al(nm) ;
	else 
	{
		UD_universal_data empty ;
		return empty ;
	}
}

void node::remove_node_attribute(std::string nm){
	UD_Attribute_List* tmp = (UD_Attribute_List*)(find_attribute_ref("attribute_list")) ;
	tmp->remove_attribute_al(nm) ;
}

UD_Virtual_Iterator* node::get_node_attribute_iter(){
	return UD_create_iterator(find_attribute_ref("attribute_list")) ;
}

void node::clear_attributes(){
	UD_Attribute_List* tmp = (UD_Attribute_List*)(find_attribute_ref("attribute_list")) ;
	while(tmp->size() != 0)
	{
		tmp->begin() ;
		tmp->remove_attribute_al(tmp->get_attribute_name_al()) ;
	}
}

UD_Attribute_List node::get_all_node_attribute(){
	UD_Attribute_List* tmp = (UD_Attribute_List*)(find_attribute_ref("attribute_list")) ;
	return *tmp ;
}

void node::push_node_branch(UD_universal_data *bptr){
	UD_Vector* tmp = (UD_Vector*)(this->data_ptr) ;
	tmp->push_back(bptr) ;
}

UD_universal_data* node::get_node_branch(int index){
	UD_Vector* tmp = (UD_Vector*)(this->data_ptr) ;
	return (*tmp)[index] ;
}

unsigned int node::node_branch_size(){
	UD_Vector* tmp = (UD_Vector*)(this->data_ptr) ;
	return tmp->size() ;
}

void node::pop_node_branch(){
	UD_Vector* tmp = (UD_Vector*)(this->data_ptr) ;
	tmp->pop_back() ;
}

UD_Virtual_Iterator* node::get_node_branch_iter(){
	return UD_create_iterator((UD_universal_data*)(this->data_ptr)) ;
}

void node::clear_branches(){
	UD_Vector* tmp = (UD_Vector*)(this->data_ptr) ;
	tmp->begin() ;
	while(!tmp->end()){
		tmp->pop_back() ;
		tmp->begin() ;
	}
}

void node::set_auxiliary_data(UD_universal_data ud){
	set_attribute("auxiliary_data", ud) ;
}

UD_universal_data node::get_auxiliary_data(){
	return find_attribute("auxiliary_data") ;
}

void node::push_binding_info(std::string kind, std::string nm){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;

	UD_String* udf = new UD_String() ;
	udf->set_value(kind) ;
	UD_String* uds = new UD_String() ;
	uds->set_value(nm) ;
	
	UD_Pair* udp = new UD_Pair() ;
	udp->set_first(udf) ;
	udp->set_second(uds) ;

	tmp->push_back(udp) ;
}

void node::pop_binding_info(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;

	int count = 1 ;
	
	for(tmp->begin() ; !tmp->end() ; tmp++)
	{
		if(count == tmp->size()) 
		{
			UD_Pair* ptmp = (UD_Pair*)(*(*tmp)) ;
			UD_String* f = (UD_String*)(ptmp->first()) ;
			UD_String* s = (UD_String*)(ptmp->second()) ;
			delete f ;
			delete s ;
			delete ptmp ;
			return ;
		}
		else 
			count++ ;
	}
}

void node::binding_info_begin(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;
	tmp->begin() ;
}

bool node::binding_info_end(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;
	return tmp->end() ;
}

void node::binding_info_advance(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;
	(*tmp)++ ;
}

string node::binding_info_kind(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;
	if(binding_info_end())
		return "" ;
	
	UD_Pair* ptmp = (UD_Pair*)(*(*tmp)) ;
	UD_String* udk = (UD_String*)(ptmp->first()) ;
	return *(udk->getptr()) ;
}

string node::binding_info_name(){
	UD_List* tmp = (UD_List*)(find_attribute_ref("binding_info")) ;
	if(binding_info_end())
		return "" ;
	
	UD_Pair* ptmp = (UD_Pair*)(*(*tmp)) ;
	UD_String* udn = (UD_String*)(ptmp->second()) ;
	return *(udn->getptr()) ;
}
