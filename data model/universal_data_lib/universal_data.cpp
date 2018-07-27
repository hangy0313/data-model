/* --------------------------------------------------------------- */
/* (C) Copyright 2018	                                           */
/* Owner: Tsung Lee, Yao-Yi Huang							   	   */
/* All Rights Reserved.                                            */
/* --------------------------------------------------------------- */
#include "universal_data.h"

//###########################################
// free function 
//###########################################
int UD_get_type_tag_priority(UD_Type_Tag tt) 
{
	switch(tt)
	{
		case T_char: return 1 ;
		case T_int: return 2 ;
		case T_float: return 3 ;
		case T_double: return 4 ;
		case T_string: return 5 ;
		case T_pair: return 6 ;
		case T_set: return 7 ;
		case T_list: return 8 ;
		case T_vector: return 9 ;
		case T_map: return 10 ;
		case T_multimap: return 11 ;
		case T_attribute_list: return 12 ;
		default: return -1 ; // user defined type 
	}
}

UD_Virtual_Iterator* UD_create_iterator(UD_universal_data* org, int aspect_id)
{
	switch(org->get_type_tag())
	{
	case T_pair:
		return new UD_Pair_Iterator(org, aspect_id) ;
		break ;
	case T_map:
		return new UD_Map_Iterator(org, aspect_id) ;
		break; 
	case T_vector: 
		return new UD_Vector_Iterator(org, aspect_id) ;
		break ;
	case T_set: 
		return new UD_Set_Iterator(org, aspect_id) ;
		break ;
	case T_list:
		return new UD_List_Iterator(org, aspect_id) ;
		break ;
	case T_multimap:
		return new UD_Multimap_Iterator(org, aspect_id) ;
		break ;
	case T_attribute_list: 
		return new UD_Attribute_List_Iterator(org, aspect_id) ;
		break ;
	default:
		cout<<"not support this kind of iterator "<<endl ;
	}
	return NULL ;
}

void UD_print_message(string msg) 
{
	cout<<"Message:: "<<msg<<endl ;
}

void UD_print_error_message(string msg) 
{
	cout<<"Error:: "<<msg<<endl ;
	exit(-1) ;
}

//###########################################
// abstract class  
//###########################################
UD_universal_data::UD_universal_data(){
	this->type_tag = T_unknown ;
	this->user_defined_type_tag = "" ;
	this->data_ptr = NULL ;
	list<UD_universal_data*> newone ;
	parent_ptr.insert(pair<int, list<UD_universal_data*> >(0, newone) ) ;
}

UD_universal_data::~UD_universal_data(){
}

UD_universal_data::UD_universal_data(const UD_universal_data &ud){
	this->type_tag = ((UD_universal_data*)(&ud))->get_type_tag() ;
	this->user_defined_type_tag = ((UD_universal_data*)(&ud))->get_user_defined_type_tag() ;
	this->parent_ptr = ((UD_universal_data*)(&ud))->get_parent_pointer() ;
	this->attribute_list = ((UD_universal_data*)(&ud))->attribute_list ;
	if(  ((UD_universal_data*)(&ud))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_universal_data*)(&ud))) ;
}

// type_tag
void UD_universal_data::set_type_tag(UD_Type_Tag tt){
	type_tag = tt;
}

UD_Type_Tag UD_universal_data::get_type_tag(){
	return type_tag ;
}
// user defind type tag 
void UD_universal_data::set_user_defined_type_tag(std::string tag) {
	user_defined_type_tag = tag ;
}

string UD_universal_data::get_user_defined_type_tag(){
	return user_defined_type_tag ;
}
//parent pointer
void UD_universal_data::set_parent_pointer(UD_universal_data *ptr, int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = parent_ptr.find(aspect_id) ;
	if(iter == parent_ptr.end() )
	{ // aspect is exist 
		list<UD_universal_data*> newone ;
		newone.push_back(ptr) ;
		parent_ptr.insert(pair<int, list<UD_universal_data*> >(aspect_id, newone) ) ;
	}
	else 
	{
		iter->second.push_back(ptr) ;
	}
}

void UD_universal_data::remove_parent_pointer(UD_universal_data *ptr, int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = parent_ptr.find(aspect_id) ;
	if(iter != parent_ptr.end() )
		iter->second.remove(ptr) ;
}

list<UD_universal_data*> UD_universal_data::find_parent_pointer(int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	list<UD_universal_data*> rdata ;
	
	iter = parent_ptr.find(aspect_id) ;
	if(iter != parent_ptr.end())
		rdata = iter->second ;
	return rdata ;
}

map<int, list<UD_universal_data*> > UD_universal_data::get_parent_pointer(){
	return parent_ptr ;
}

list<int> UD_universal_data::get_joined_aspect(){
	map<int, list<UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = parent_ptr.begin() ; iter != parent_ptr.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

//data 
void UD_universal_data::set_data(unsigned* d) {
	if(this->data_ptr == NULL)
		this->data_ptr = d ;
	else
	{
		delete this->data_ptr ;
		this->data_ptr = d ;
	}
}

unsigned* UD_universal_data::get_data(){
	return this->data_ptr ;
}

// attribute list 
bool UD_universal_data::add_attribute(std::string nm, UD_universal_data ud){
	// search name in attribute list 
	list<pair<string, UD_universal_data> >::iterator iter ;
	for(iter = attribute_list.begin() ; iter != attribute_list.end() ; iter ++){
		if( (*iter).first == nm)
			return false ; // the tag is used 
	}
	attribute_list.push_back( pair<string, UD_universal_data>(nm, ud) ) ;
	return true; 
}

void UD_universal_data::set_attribute(string nm, UD_universal_data ud) {
	list<pair<string, UD_universal_data> >::iterator iter ;
	for(iter = attribute_list.begin() ; iter != attribute_list.end() ; iter ++){
		if( (*iter).first == nm)
			iter->second = ud ;
	}
}

void UD_universal_data::remove_attribute(std::string nm){
	// search nm in attribute list 
	list<pair<string, UD_universal_data> >::iterator iter ;
	for(iter = attribute_list.begin() ; iter != attribute_list.end() ; iter ++){
		if( (*iter).first == nm)
		{
			// remove from attribute list
			attribute_list.erase(iter) ;
			return ;
		}
	}
}

UD_universal_data UD_universal_data::find_attribute(std::string nm) {
	list< pair<string, UD_universal_data> >::iterator titer ;
	for(titer = attribute_list.begin() ; titer != attribute_list.end() ; titer++)
		if(titer->first == nm)
			break ;

	if(titer == attribute_list.end() ){
		UD_universal_data empty_ud ;
		return empty_ud;
	}
	else
		return titer->second ;
}

UD_universal_data* UD_universal_data::find_attribute_ref(string nm){
	list< pair<string, UD_universal_data> >::iterator titer ;
	for(titer = attribute_list.begin() ; titer != attribute_list.end() ; titer++)
		if(titer->first == nm)
			break ;

	if(titer == attribute_list.end() )
		return NULL ;
	else
		return &(titer->second) ;
}

void UD_universal_data::attributes_begin() {
	attr_iter = attribute_list.begin() ;
}

bool UD_universal_data::attributes_end() {
	return (attr_iter == attribute_list.end() ) ;
}

void UD_universal_data::attributes_next(){
	attr_iter++ ;
}

string UD_universal_data::get_attribute_name() {
	return attr_iter->first ;
}

UD_universal_data UD_universal_data::get_attribute() {
	return attr_iter->second ;
}

UD_universal_data& UD_universal_data::operator =(UD_universal_data other_data){

	this->type_tag = other_data.get_type_tag() ;
	this->user_defined_type_tag = other_data.get_user_defined_type_tag() ;
	this->parent_ptr = other_data.get_parent_pointer() ;
	this->attribute_list = other_data.attribute_list ;
	if(this->data_ptr == NULL)
		this->data_ptr = copy_universal_data(other_data) ;
	else{
		if(this->type_tag != T_user_defined)
			delete this->data_ptr ;
		this->data_ptr = copy_universal_data(other_data) ;
	}
	return *this ;
}

bool UD_universal_data::operator ==(UD_universal_data other_data){
	// detail design in corresponding class 
	switch(this->get_type_tag() ){
		case T_int: 
			return *((UD_Int*)this) == other_data ;
			break ;
		case T_string: 
			return *((UD_String*)this) == other_data ;
			break ;
		case T_char: 
			return *((UD_Char*)this) == other_data ;
			break ;
		case T_float: 
			return *((UD_Float*)this) == other_data ;
			break ;
		case T_double: 
			return *((UD_Double*)this) == other_data ;
			break ;
		case T_bool: 
			return *((UD_Bool*)this) == other_data ;
			break ;
		case T_pair: 
			return *((UD_Pair*)this) == other_data ;
		case T_set: 
			return *((UD_Set*)this) == other_data ;
		case T_list: 
			return *((UD_List*)this) == other_data ;
		case T_vector: 
			return *((UD_Vector*)this) == other_data ;
		case T_map: 
			return *((UD_Map*)this) == other_data ;
		case T_multimap: 
			return *((UD_Multimap*)this) == other_data ;
		case T_attribute_list: 
			return *((UD_Attribute_List*)this) == other_data ;
		case T_user_defined:
			return this->data_ptr == other_data.get_data() ;
		default:
			UD_print_error_message("not support this operator ==") ;
			return false ;
	}
}

bool UD_universal_data::operator !=(UD_universal_data other_data){
	// detail design in corresponding class 
	switch(this->get_type_tag() ){
		case T_int: 
			return *((UD_Int*)this) != other_data ;
			break ;
		case T_string: 
			return *((UD_String*)this) != other_data ;
			break ;
		case T_char: 
			return *((UD_Char*)this) != other_data ;
			break ;
		case T_float: 
			return *((UD_Float*)this) != other_data ;
			break ;
		case T_double: 
			return *((UD_Double*)this) != other_data ;
			break ;
		case T_bool:
			return *((UD_Bool*)this) != other_data ;
			break ;
		case T_pair: 
			return *((UD_Pair*)this) != other_data ;
		case T_set: 
			return *((UD_Set*)this) != other_data ;
		case T_list: 
			return *((UD_List*)this) != other_data ;
		case T_vector: 
			return *((UD_Vector*)this) != other_data ;
		case T_map: 
			return *((UD_Map*)this) != other_data ;
		case T_multimap: 
			return *((UD_Multimap*)this) != other_data ;
		case T_attribute_list: 
			return *((UD_Attribute_List*)this) != other_data ;
		default:
			UD_print_error_message("not support this operator !=") ;
			return false ;
	}
}

bool UD_universal_data::operator <(const UD_universal_data &rhs) const{
	// detail design in corresponding class 
	switch(((UD_universal_data*)this)->get_type_tag() ){
		case T_int: 
			return *((UD_Int*)this) < rhs ;
			break ;
		case T_string: 
			return *((UD_String*)this) < rhs ;
			break ;
		case T_char: 
			return *((UD_Char*)this) < rhs ;
			break ;
		case T_float: 
			return *((UD_Float*)this) < rhs ;
			break ;
		case T_double: 
			return *((UD_Double*)this) < rhs ;
			break ;
		case T_bool: 
			return *((UD_Bool*)this) < rhs ;
			break ;
		case T_pair: 
			return *((UD_Pair*)this) < rhs ;
		case T_set: 
			return *((UD_Set*)this) < rhs ;
		case T_list: 
			return *((UD_List*)this) < rhs ;
		case T_vector: 
			return *((UD_Vector*)this) < rhs ;
		case T_map: 
			return *((UD_Map*)this) < rhs ;
		case T_multimap: 
			return *((UD_Multimap*)this) < rhs ;
		case T_attribute_list: 
			return *((UD_Attribute_List*)this) < rhs ;
		default:
			UD_print_error_message("not support this operator <") ;
			return false ;
	}
}

unsigned* UD_universal_data::copy_universal_data(UD_universal_data &source){
	// get type_tag
	unsigned* tmp ;
	if(source.get_data() == NULL)
		return NULL ;

	switch (source.get_type_tag() ){
		case T_unknown: 
			// do nothing 
			tmp = NULL ;
			break ;
		case T_symbolic_link:
			tmp = (unsigned*)(new UD_symbolic_link_data); 
			((UD_symbolic_link_data*)tmp)->L1_flag = ((UD_symbolic_link_data*)(source.get_data()))->L1_flag ;
			((UD_symbolic_link_data*)tmp)->L1_key = ((UD_symbolic_link_data*)(source.get_data()))->L1_key ;
			((UD_symbolic_link_data*)tmp)->L1_name = ((UD_symbolic_link_data*)(source.get_data()))->L1_name ;
			((UD_symbolic_link_data*)tmp)->table_pointer = ((UD_symbolic_link_data*)(source.get_data()))->table_pointer ;
			((UD_symbolic_link_data*)tmp)->Absolute_flag = ((UD_symbolic_link_data*)(source.get_data()))->Absolute_flag ;
			((UD_symbolic_link_data*)tmp)->path = ((UD_symbolic_link_data*)(source.get_data()))->path ;
			break ;
		case T_int: 
			tmp = (unsigned*)(new int) ;
			(*((int*)tmp)) = ( *((int*)(source.get_data())) ) ;
			break ;
		case T_string: 
			tmp = (unsigned*)(new string) ;
			(*((string*)tmp)) = ( *((string*)(source.get_data())) ) ;
			break;
		case T_char: 
			tmp = (unsigned*)(new char) ;
			(*((char*)tmp)) = ( *((char*)(source.get_data())) ) ;
			break;
		case T_float: 
			tmp = (unsigned*)(new float) ;
			(*((float*)tmp)) = ( *((float*)(source.get_data())) ) ;
			break ;
		case T_double: 
			tmp = (unsigned*)(new double) ;
			(*((double*)tmp)) = ( *((double*)(source.get_data())) ) ;
			break ;
		case T_bool:
			tmp = (unsigned*)(new bool) ;
			(*((bool*)tmp) ) = (* ((bool*)(source.get_data()))) ;
			break ;
		case T_pair: 
			tmp = (unsigned*)(new UD_Pair_data) ;
			((UD_Pair_data*)tmp)->elem = ((UD_Pair_data*)(source.get_data()))->elem ;
			((UD_Pair_data*)tmp)->type_tag_first = ((UD_Pair_data*)(source.get_data()))->type_tag_first ;
			((UD_Pair_data*)tmp)->type_tag_second = ((UD_Pair_data*)(source.get_data()))->type_tag_second ;
			break ;
		case T_map: 
			tmp = (unsigned*)(new UD_Map_data) ;
			((UD_Map_data*)tmp)->elem = ((UD_Map_data*)(source.get_data()))->elem ;
			((UD_Map_data*)tmp)->type_tag_key = ((UD_Map_data*)(source.get_data()))->type_tag_key ;
			((UD_Map_data*)tmp)->type_tag_value = ((UD_Map_data*)(source.get_data()))->type_tag_value ;
			break ;
		case T_vector: 
			tmp = (unsigned*)(new UD_Vector_data) ;
			((UD_Vector_data*)tmp)->elem_type_tag = ((UD_Vector_data*)(source.get_data()))->elem_type_tag ;
			((UD_Vector_data*)tmp)->vec = ((UD_Vector_data*)(source.get_data()))->vec ;
			break ;
		case T_set: 
			tmp = (unsigned*)(new UD_Set_data) ;
			((UD_Set_data*)tmp)->elem = ((UD_Set_data*)(source.get_data()))->elem ;
			((UD_Set_data*)tmp)->elem_type_tag = ((UD_Set_data*)(source.get_data()))->elem_type_tag ;
			break ;
		case T_list: 
			tmp = (unsigned*)(new UD_List_data) ;
			((UD_List_data*)tmp)->elem = ((UD_List_data*)(source.get_data()))->elem ;
			((UD_List_data*)tmp)->elem_type_tag = ((UD_List_data*)(source.get_data()))->elem_type_tag ;
			break ;
		case T_multimap: 
			tmp = (unsigned*)(new UD_Multimap_data) ;
			((UD_Multimap_data*)tmp)->elem = ((UD_Multimap_data*)(source.get_data()))->elem ;
			((UD_Multimap_data*)tmp)->type_tag_key = ((UD_Multimap_data*)(source.get_data()))->type_tag_key ;
			((UD_Multimap_data*)tmp)->type_tag_value = ((UD_Multimap_data*)(source.get_data()))->type_tag_value ;
			break ;
		case T_attribute_list: 
			tmp = (unsigned*)(new UD_Attribute_List_data) ;
			((UD_Attribute_List_data*)tmp)->attributes = ((UD_Attribute_List_data*)(source.get_data()))->attributes ;
			break ;
		case T_user_defined: 
			tmp = source.get_data() ;
			break ;
		default: 
			UD_print_error_message("Not set up this type "+ source.get_type_tag() ) ;
	}
	return tmp ;
}

//###########################################
// symbolic link 
//###########################################
// lookup table 
// level 2: key to level 1 table's address 
map<UD_universal_data, map<UD_universal_data, UD_universal_data*>*> UD_L2_table ;
// level 1: key to component's address 
map<UD_universal_data, UD_universal_data*> UD_L1_table ;

UD_symbolic_link_data::UD_symbolic_link_data(){
	this->table_pointer = NULL ;
	this->L1_flag = true; 
	this->Absolute_flag = true ; 
}

UD_symbolic_link::UD_symbolic_link(){
	set_type_tag(T_symbolic_link) ;
	this->data_ptr = (unsigned*)(new UD_symbolic_link_data) ;
}

UD_symbolic_link::~UD_symbolic_link(){
	delete this->data_ptr ;
}

UD_symbolic_link::UD_symbolic_link(const UD_symbolic_link& sym) {
	this->type_tag = ((UD_symbolic_link*)(&sym))->get_type_tag() ;
	this->parent_ptr = ((UD_symbolic_link*)(&sym))->get_parent_pointer() ;
	this->attribute_list = ((UD_symbolic_link*)(&sym))->attribute_list ;
	if(  ((UD_symbolic_link*)(&sym))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_symbolic_link*)(&sym))) ;
}

void UD_symbolic_link::set_L1_key(UD_universal_data key){
	((UD_symbolic_link_data*)(this->data_ptr))->L1_key = key ;
}

UD_universal_data UD_symbolic_link::get_L1_key(){
	return ((UD_symbolic_link_data*)(this->data_ptr))->L1_key ;
}

void UD_symbolic_link::set_table_pointer(unsigned* tp){
	((UD_symbolic_link_data*)(this->data_ptr))->table_pointer = tp ;
}

unsigned* UD_symbolic_link::get_table_pointer(){
	return ((UD_symbolic_link_data*)(this->data_ptr))->table_pointer ;
}

void UD_symbolic_link::set_L1_name(UD_universal_data nm){
	((UD_symbolic_link_data*)(this->data_ptr))->L1_name = nm ;
}

UD_universal_data UD_symbolic_link::get_L1_name(){
	return ((UD_symbolic_link_data*)(this->data_ptr))->L1_name ;
}

void UD_symbolic_link::set_L1_flag(bool f) {
	((UD_symbolic_link_data*)(this->data_ptr))->L1_flag = f ;
}

bool UD_symbolic_link::get_L1_flag(){
	return ((UD_symbolic_link_data*)(this->data_ptr))->L1_flag ;
}

void UD_symbolic_link::set_Absolute_flag(bool f) {
	((UD_symbolic_link_data*)(this->data_ptr))->Absolute_flag = f ;
}

bool UD_symbolic_link::get_Absolute_flag() {
	return ((UD_symbolic_link_data*)(this->data_ptr))->Absolute_flag ;
}

void UD_symbolic_link::path_push_back(UD_sym_condition *c) {
	((UD_symbolic_link_data*)(this->data_ptr))->path.push_back(c) ;
}

void UD_symbolic_link::path_pop_back() {
	((UD_symbolic_link_data*)(this->data_ptr))->path.pop_back() ;
}

UD_universal_data* UD_symbolic_link::linkto_target() {
	if( ((UD_symbolic_link_data*)(this->data_ptr))->path.size() == 0) // empty
		return NULL ;

	list<UD_sym_condition*>::iterator iter ;
	map<UD_universal_data, UD_universal_data*>::iterator L1_iter ;
	map<UD_universal_data, map<UD_universal_data, UD_universal_data*>*>::iterator L2_iter ;
	UD_universal_data* tmp ;
	if( ((UD_symbolic_link_data*)(this->data_ptr))->Absolute_flag ) // absolute path
	{
		// find the top level component 
		if( ((UD_symbolic_link_data*)(this->data_ptr))->L1_flag) // L1 table
		{
			L1_iter = UD_L1_table.find( ((UD_symbolic_link_data*)(this->data_ptr))->L1_key) ;
			if( L1_iter == UD_L1_table.end() ) // not found
			{
				UD_print_message("can't find top level component") ;
				return NULL ;
			}
			tmp = L1_iter->second ;
		}
		else // L2 table 
		{
			L2_iter = UD_L2_table.find(((UD_symbolic_link_data*)(this->data_ptr))->L1_name) ;
			if(L2_iter == UD_L2_table.end() )
			{
				UD_print_message("can't find L1 table") ;
				return NULL ;
			}

			L1_iter = L2_iter->second->find( ((UD_symbolic_link_data*)(this->data_ptr))->L1_key) ;
			if( L1_iter == L2_iter->second->end() ) // not found
			{
				UD_print_message("can't find top level component") ;
				return NULL ;
			}
			tmp = L1_iter->second ;
		}

		// use top level component to access 
		for( iter = ((UD_symbolic_link_data*)(this->data_ptr))->path.begin() ; iter != ((UD_symbolic_link_data*)(this->data_ptr))->path.end() ; iter++)
		{
			tmp = (*iter)->find_target(tmp) ;
			if(tmp == NULL){
				UD_print_message("path is error") ;
				return NULL ;
			}
		}
		if(tmp->get_type_tag() == T_symbolic_link) // cascade path 
			return ((UD_symbolic_link*)tmp)->linkto_target() ;
		else
			return tmp ;
	}
	else // relative path
	{
		tmp = this ;
		 // use current compoennt to access 
		for( iter = ((UD_symbolic_link_data*)(this->data_ptr))->path.begin() ; iter != ((UD_symbolic_link_data*)(this->data_ptr))->path.end() ; iter++)
		{
			tmp = (*iter)->find_target(tmp) ;
			if(tmp == NULL){
				UD_print_message("path is error") ;
				return NULL ;
			}
		}
		if(tmp->get_type_tag() == T_symbolic_link)  // cascade path
			return ((UD_symbolic_link*)tmp)->linkto_target() ;
		else
			return tmp ;
	}
}

//###########################################
// primitive type 
//###########################################
UD_Int::UD_Int(){
	set_type_tag(T_int) ;
	this->data_ptr = (unsigned*)(new int) ;
}

UD_Int::~UD_Int(){
	delete this->data_ptr ;
}

UD_Int::UD_Int(const UD_Int& i) {
	this->type_tag = ((UD_Int*)(&i))->get_type_tag() ;
	this->parent_ptr = ((UD_Int*)(&i))->get_parent_pointer() ;
	this->attribute_list = ((UD_Int*)(&i))->attribute_list ;
	if(  ((UD_Int*)(&i))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Int*)(&i))) ;
}

void UD_Int::set_value(int d){
	*((int*)(this->data_ptr)) = d;
}

int* UD_Int::getptr(){
	return (int*)(this->data_ptr) ;
}

bool UD_Int::operator ==(UD_universal_data other_data){
	// check whether it's type tag is T_int
	if(other_data.get_type_tag() != T_int)
		return false ;
	else 
		return ( *((int*)(this->data_ptr)) == *( ((UD_Int*)(&other_data))->getptr() ) ) ;
}

bool UD_Int::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Int::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if(((UD_universal_data*)(&rhs))->get_type_tag() != T_int)
	{
		int priority1 = UD_get_type_tag_priority(T_int) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else 
		return ( *((int*)(this->data_ptr)) < *( ((UD_Int*)(&rhs))->getptr() ) ) ;
}

UD_Char::UD_Char(){
	set_type_tag(T_char) ;
	this->data_ptr = (unsigned*)(new char) ;
}

UD_Char::~UD_Char(){
	delete this->data_ptr ;
}

UD_Char::UD_Char(const UD_Char& c) {
	this->type_tag = ((UD_Char*)(&c))->get_type_tag() ;
	this->parent_ptr = ((UD_Char*)(&c))->get_parent_pointer() ;
	this->attribute_list = ((UD_Char*)(&c))->attribute_list ;
	if(  ((UD_Char*)(&c))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Char*)(&c))) ;
}

void UD_Char::set_value(char d){
	*((char*)(this->data_ptr)) = d ;
}

char* UD_Char::getptr(){
	return (char*)(this->data_ptr);
}

bool UD_Char::operator ==(UD_universal_data other_data){
	// check whether it's type tag is T_char
	if(other_data.get_type_tag() != T_char)
		return false ;
	else 
		return ( *((char*)(this->data_ptr)) == *( ((UD_Char*)(&other_data))->getptr() ) ) ;
}

bool UD_Char::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Char::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if(((UD_universal_data*)(&rhs))->get_type_tag() != T_char)
	{
		int priority1 = UD_get_type_tag_priority(T_char) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
		return ( *((char*)(this->data_ptr)) < *( ((UD_Char*)(&rhs))->getptr() ) ) ;
}

UD_Float::UD_Float(){
	set_type_tag(T_float) ;
	this->data_ptr = (unsigned*)(new float) ;
}

UD_Float::~UD_Float(){
	delete this->data_ptr ;
}

UD_Float::UD_Float(const UD_Float& f) {
	this->type_tag = ((UD_Float*)(&f))->get_type_tag() ;
	this->parent_ptr = ((UD_Float*)(&f))->get_parent_pointer() ;
	this->attribute_list = ((UD_Float*)(&f))->attribute_list ;
	if(  ((UD_Float*)(&f))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Float*)(&f))) ;
}

void UD_Float::set_value(float d){
	*((float*)(this->data_ptr)) = d; 
}

float* UD_Float::getptr(){
	return (float*)(this->data_ptr) ;
}

bool UD_Float::operator ==(UD_universal_data other_data){
	// check whether it's type tag is T_float
	if(other_data.get_type_tag() != T_float)
		return false ;
	else 
		return ( *((float*)(this->data_ptr)) == *( ((UD_Float*)(&other_data))->getptr() ) ) ;
}

bool UD_Float::operator !=(UD_universal_data other_data){
	return ! ( (*this) == (other_data)) ;
}

bool UD_Float::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if(((UD_universal_data*)(&rhs))->get_type_tag() != T_float)
	{
		int priority1 = UD_get_type_tag_priority(T_float) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else 
		return ( *((float*)(this->data_ptr)) < *( ((UD_Float*)(&rhs))->getptr() ) ) ;
}

UD_String::UD_String(){
	set_type_tag(T_string) ;
	this->data_ptr = (unsigned*)(new string) ;
}

UD_String::~UD_String(){
	delete this->data_ptr ;
}

UD_String::UD_String(const UD_String& s) {
	this->type_tag = ((UD_String*)(&s))->get_type_tag() ;
	this->parent_ptr = ((UD_String*)(&s))->get_parent_pointer() ;
	this->attribute_list = ((UD_String*)(&s))->attribute_list ;
	if(  ((UD_String*)(&s))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_String*)(&s))) ;
}

void UD_String::set_value(string d) {
	*((string*)(this->data_ptr)) = d; 
}

string* UD_String::getptr(){
	return (string*)(this->data_ptr) ;
}

bool UD_String::operator ==(UD_universal_data other_data){
	// check whether it's type tag is T_string
	if(other_data.get_type_tag() != T_string)
		return false ;
	else 
		return ( *((string*)(this->data_ptr)) == *( ((UD_String*)(&other_data))->getptr() ) ) ;
}

bool UD_String::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_String::operator <(const UD_universal_data &rhs) const{
	// check whether it's type tag is T_string
	if(((UD_universal_data*)(&rhs))->get_type_tag() != T_string)
	{
		int priority1 = UD_get_type_tag_priority(T_string) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
		return ( *((string*)(this->data_ptr)) < *( ((UD_String*)(&rhs))->getptr() ) ) ;
}

UD_Double::UD_Double(){
	set_type_tag(T_double) ;
	this->data_ptr = (unsigned*)(new double) ;
}

UD_Double::~UD_Double(){
	delete this->data_ptr ;
}

UD_Double::UD_Double(const UD_Double &d) {
	this->type_tag = ((UD_Double*)(&d))->get_type_tag() ;
	this->parent_ptr = ((UD_Double*)(&d))->get_parent_pointer() ;
	this->attribute_list = ((UD_Double*)(&d))->attribute_list ;
	if(  ((UD_Double*)(&d))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Double*)(&d))) ;
}

void UD_Double::set_value(double d){
	*((double*)(this->data_ptr)) = d ;
}

double* UD_Double::getptr(){
	return (double*)(this->data_ptr); 
}

bool UD_Double::operator ==(UD_universal_data other_data){
	// check whether it's type tag is T_double
	if(other_data.get_type_tag() != T_double)
		return false ;
	else 
		return ( *((double*)(this->data_ptr)) == *( ((UD_Double*)(&other_data))->getptr() ) ) ;
}

bool UD_Double::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Double::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if(((UD_universal_data*)(&rhs))->get_type_tag() != T_double)
	{
		int priority1 = UD_get_type_tag_priority(T_double) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else 
		return ( *((double*)(this->data_ptr)) < *( ((UD_Double*)(&rhs))->getptr() ) ) ;
}

UD_Bool::UD_Bool()
{
	set_type_tag(T_bool) ;
	this->data_ptr = (unsigned*)(new bool) ;
}

UD_Bool::~UD_Bool()
{
	delete this->data_ptr ;
}

UD_Bool::UD_Bool(const UD_Bool& b)
{
	this->type_tag = ((UD_Bool*)(&b))->get_type_tag() ;
	this->parent_ptr = ((UD_Bool*)(&b))->get_parent_pointer() ;
	this->attribute_list = ((UD_Bool*)(&b))->attribute_list ;
	if(  ((UD_Bool*)(&b))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Bool*)(&b))) ;
}

void UD_Bool::set_value(bool d) 
{
	*((bool*)(this->data_ptr)) = d ;
}

bool* UD_Bool::getptr()
{
	return (bool*)(this->data_ptr); 
}

bool UD_Bool::operator ==(UD_universal_data other_data){
	// check whether it's type tag is T_bool
	if(other_data.get_type_tag() != T_bool)
		return true ; 
	else 
		return ( *((bool*)(this->data_ptr)) == *( ((UD_Bool*)(&other_data))->getptr() ) ) ;
}

bool UD_Bool::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Bool::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if(((UD_universal_data*)(&rhs))->get_type_tag() != T_bool)
	{
		int priority1 = UD_get_type_tag_priority(T_bool) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
		return ( *((bool*)(this->data_ptr)) < *( ((UD_Bool*)(&rhs))->getptr() ) ) ;
}

//###########################################
// structure type
//###########################################
UD_Vector_data::UD_Vector_data() {
	this->elem_type_tag = T_unlimited  ;
}

UD_Vector::UD_Vector(){
	set_type_tag(T_vector) ;
	this->data_ptr = (unsigned*)(new UD_Vector_data) ;
	((UD_Vector_data*)data_ptr)->owner = this ;
	vector<UD_universal_data*> newone ;
	((UD_Vector_data*)(this->data_ptr))->vec.insert(pair<int, vector<UD_universal_data*> >(0, newone) ) ;
}

UD_Vector::~UD_Vector(){
	delete this->data_ptr ;
}

UD_Vector::UD_Vector(const UD_Vector& v) {
	this->type_tag = ((UD_Vector*)(&v))->get_type_tag() ;
	this->parent_ptr = ((UD_Vector*)(&v))->get_parent_pointer() ;
	this->attribute_list = ((UD_Vector*)(&v))->attribute_list ;
	if(  ((UD_Vector*)(&v))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Vector*)(&v))) ;
}

void UD_Vector::set_elem_type_tag(UD_Type_Tag tt) {
	((UD_Vector_data*)(this->data_ptr))->elem_type_tag = tt ;
}

UD_Type_Tag UD_Vector::get_elem_type_tag(){
	return ((UD_Vector_data*)(this->data_ptr))->elem_type_tag ;
}

void UD_Vector::push_back(UD_universal_data* d, int aspect_id){
	// check elem type tag 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()))
		UD_print_error_message("the type tag is not the same") ;

	d->set_parent_pointer(this) ;

	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(aspect_id) ;
	if(iter == ((UD_Vector_data*)(this->data_ptr))->vec.end())
	{
		vector<UD_universal_data*> newone ;
		newone.push_back(d) ;
		((UD_Vector_data*)(this->data_ptr))->vec.insert(pair<int, vector<UD_universal_data*> >(aspect_id, newone) ) ;
	}
	else 
		iter->second.push_back(d) ;
}

void UD_Vector::pop_back(int aspect_id){
	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(aspect_id) ;
	if( iter != ((UD_Vector_data*)(this->data_ptr))->vec.end())
	{
		vector<UD_universal_data*> tmp = iter->second ;
		unsigned size = tmp.size() ;
		tmp[size-1]->remove_parent_pointer(this) ;
		iter->second.pop_back() ;
	}
}

UD_universal_data* UD_Vector::operator [](unsigned int n){
	// default aspect 
	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(0) ;

	if(n>= iter->second.size())
		return NULL ;
	else 
		return iter->second[n] ;
}

UD_universal_data* UD_Vector::get_structure_data(unsigned int n, int aspect_id){
	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(aspect_id) ;
	if(iter == ((UD_Vector_data*)(this->data_ptr))->vec.end())
		return NULL ;

	if ( n>= iter->second.size())
		return NULL ; 
	else 
		return iter->second[n] ;
}

unsigned int UD_Vector::size(int aspect_id){
	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(aspect_id) ;
	if (iter == ((UD_Vector_data*)(this->data_ptr))->vec.end())
		return 0 ;
	else 
		return iter->second.size() ;
}

list<int> UD_Vector::get_aspect_in_structure(){
	map<int, vector<UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = ((UD_Vector_data*)(this->data_ptr))->vec.begin() ; iter != ((UD_Vector_data*)(this->data_ptr))->vec.end() ;iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

void UD_Vector::begin(int aspect_id){
	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(aspect_id) ;
	if(iter == ((UD_Vector_data*)(this->data_ptr))->vec.end())
		UD_print_error_message("in Vector, can't find aspect "+aspect_id) ;

	((UD_Vector_data*)(this->data_ptr))->iter = iter->second.begin() ;
}

bool UD_Vector::end(int aspect_id){
	map<int, vector<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Vector_data*)(this->data_ptr))->vec.find(aspect_id) ;
	if (iter == ((UD_Vector_data*)(this->data_ptr))->vec.end())
		return true ;
	else 
		return (((UD_Vector_data*)(this->data_ptr))->iter == iter->second.end()) ;
}

void UD_Vector::operator ++(int){
	((UD_Vector_data*)(this->data_ptr))->iter++ ;
}

UD_universal_data* UD_Vector::operator *(){
	return *(((UD_Vector_data*)(this->data_ptr))->iter) ;
}

bool UD_Vector::operator ==(UD_universal_data other_data){
	if (this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		if( this->size() != ((UD_Vector*)(&other_data))->size())
			return false ;
		UD_Virtual_Iterator* iter1 = UD_create_iterator(this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator(&other_data) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) &&(!iter2->end()))
		{
			if(  *(*(*iter1)) != *(*(*iter2)) )
				return false ;
			iter1->advance() ; 
			iter2->advance() ;
		}
		return true ;
	}
}

bool UD_Vector::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Vector::operator <(const UD_universal_data& rhs) const{
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_vector)
	{
		int priority1 = UD_get_type_tag_priority(T_vector) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator((UD_universal_data*)this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator((UD_universal_data*)(&rhs)) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) && (!iter2->end())) 
		{
			if( *(*(*iter1)) == *(*(*iter2)) )
			{
				iter1->advance() ; 
				iter2->advance() ;
			}
			else 
				return *(*(*iter1)) < *(*(*iter2)) ;
		}
		if( iter2->end())
			return false ;
		else 
			return true ;
	}
}

//
UD_Pair_data::UD_Pair_data(){
	type_tag_first = T_unlimited ;
	type_tag_second = T_unlimited ;
}

UD_Pair::UD_Pair(){
	set_type_tag(T_pair) ;
	this->data_ptr = (unsigned*)(new UD_Pair_data) ;
	((UD_Pair_data*)data_ptr)->owner = this ;
	pair<UD_universal_data*, UD_universal_data*> newone ;
	newone.first = NULL ;
	newone.second = NULL ;
	((UD_Pair_data*)data_ptr)->elem.insert(pair<int, pair<UD_universal_data*, UD_universal_data*> >(0, newone) ) ;
}

UD_Pair::~UD_Pair(){
	delete this->data_ptr ;
}

UD_Pair::UD_Pair(const UD_Pair& p) {
	this->type_tag = ((UD_Pair*)(&p))->get_type_tag() ;
	this->parent_ptr = ((UD_Pair*)(&p))->get_parent_pointer() ;
	this->attribute_list = ((UD_Pair*)(&p))->attribute_list ;
	if(  ((UD_Pair*)(&p))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Pair*)(&p))) ;
}

void UD_Pair::set_type_tag_first(UD_Type_Tag tt) {
	((UD_Pair_data*)(this->data_ptr))->type_tag_first = tt ;
}

UD_Type_Tag UD_Pair::get_type_tag_first() {
	return ((UD_Pair_data*)(this->data_ptr))->type_tag_first ;
}

void UD_Pair::set_type_tag_second(UD_Type_Tag tt) {
	((UD_Pair_data*)(this->data_ptr))->type_tag_second = tt ;
}

UD_Type_Tag UD_Pair::get_type_tag_second() {
	return ((UD_Pair_data*)(this->data_ptr))->type_tag_second ;
}

void UD_Pair::set_first(UD_universal_data* ud, int aspect_id){
	// check the type tag of first element 
	if ( (this->get_type_tag_first() != T_unlimited)&&(this->get_type_tag_first() != ud->get_type_tag()) )
		UD_print_error_message("The type tag is not the same") ;

	map<int, pair<UD_universal_data*, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Pair_data*)data_ptr)->elem.find(aspect_id) ;
	if( iter == ((UD_Pair_data*)data_ptr)->elem.end())
		return ;

	if(iter->second.first == NULL)
	{
		ud->set_parent_pointer(this) ;
		iter->second.first = ud ;
	}
	else
	{
		iter->second.first->remove_parent_pointer(this) ;
		ud->set_parent_pointer(this) ;
		iter->second.first = ud ;
	}
}

void UD_Pair::set_second(UD_universal_data* ud, int aspect_id){
	// check the type tag of second element 
	if ( (this->get_type_tag_second() != T_unlimited)&&(this->get_type_tag_second() != ud->get_type_tag()) )
		UD_print_error_message("The type tag is not the same") ;

	map<int, pair<UD_universal_data*, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Pair_data*)data_ptr)->elem.find(aspect_id) ;
	if(iter == ((UD_Pair_data*)data_ptr)->elem.end())
		return ;

	if(iter->second.second == NULL)
	{
		ud->set_parent_pointer(this) ;
		iter->second.second = ud ;
	}
	else
	{
		iter->second.second->remove_parent_pointer(this) ;
		ud->set_parent_pointer(this) ;
		iter->second.second = ud ;
	}
}

list<int> UD_Pair::get_aspect_in_structure(){
	map<int, pair<UD_universal_data*, UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = ((UD_Pair_data*)data_ptr)->elem.begin() ; iter != ((UD_Pair_data*)data_ptr)->elem.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

UD_universal_data* UD_Pair::first(int aspect_id){
	map<int, pair<UD_universal_data*, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Pair_data*)data_ptr)->elem.find(aspect_id) ;
	if(iter == ((UD_Pair_data*)data_ptr)->elem.end())
		return NULL ;
	else 
		return iter->second.first ;
}

UD_universal_data* UD_Pair::second(int aspect_id){
	map<int, pair<UD_universal_data*, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Pair_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Pair_data*)data_ptr)->elem.end())
		return NULL ;
	else 
		return iter->second.second ;
}

bool UD_Pair::operator ==(UD_universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator(this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator(&other_data) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) &&(!iter2->end()))
		{
			if(  *(*(*iter1)) != *(*(*iter2)) )
				return false ;
			iter1->advance() ; 
			iter2->advance() ;
		}
		return true ;
	}
}

bool UD_Pair::operator !=( UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Pair::operator <(const UD_universal_data& rhs) const {
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_pair)
	{
		int priority1 = UD_get_type_tag_priority(T_pair) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator((UD_universal_data*)this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator( (UD_universal_data*)(&rhs)) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) && (!iter2->end())) 
		{
			if( *(*(*iter1)) == *(*(*iter2)) )
			{
				iter1->advance() ; 
				iter2->advance() ;
			}
			else 
				return *(*(*iter1)) < *(*(*iter2)) ;
		}
		if( iter2->end())
			return false ;
		else 
			return true ;
	}
}

//
UD_Map_data::UD_Map_data(){
	type_tag_key = T_unlimited ;
	type_tag_value = T_unlimited ;
}

UD_Map::UD_Map(){
	set_type_tag(T_map) ;
	this->data_ptr = (unsigned*)(new UD_Map_data) ;
	((UD_Map_data*)data_ptr)->owner = this ;
	map<UD_universal_data, UD_universal_data*> newone ;
	((UD_Map_data*)data_ptr)->elem.insert(pair<int, map<UD_universal_data, UD_universal_data*> >(0, newone) ) ;
}

UD_Map::~UD_Map(){
	delete this->data_ptr ;
}

UD_Map::UD_Map(const UD_Map& m) {
	this->type_tag = ((UD_Map*)(&m))->get_type_tag() ;
	this->parent_ptr = ((UD_Map*)(&m))->get_parent_pointer() ;
	this->attribute_list = ((UD_Map*)(&m))->attribute_list ;
	if(  ((UD_Map*)(&m))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Map*)(&m))) ;
}

void UD_Map::set_type_tag_key(UD_Type_Tag tt) {
	((UD_Map_data*)(this->data_ptr))->type_tag_key = tt ;
}

UD_Type_Tag UD_Map::get_type_tag_key() {
	return ((UD_Map_data*)(this->data_ptr))->type_tag_key ;
}

void UD_Map::set_type_tag_value(UD_Type_Tag tt){
	((UD_Map_data*)(this->data_ptr))->type_tag_value = tt ;
}

UD_Type_Tag UD_Map::get_type_tag_value(){
	return ((UD_Map_data*)(this->data_ptr))->type_tag_value ;
}

void UD_Map::insert(UD_universal_data key, UD_universal_data *val, int aspect_id){
	// check the type tag of key and value (need the same type for key comparison)
	if ( ((this->get_type_tag_key() != T_unlimited))&&(key.get_type_tag() != this->get_type_tag_key()) )
		UD_print_error_message("The type tag of key is not the same") ;

	if ( (this->get_type_tag_value() != T_unlimited)&&(val->get_type_tag() != this->get_type_tag_value()) )
		UD_print_error_message("The type tag of value is not the same") ;

	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Map_data*)data_ptr)->elem.find(aspect_id) ;

	val->set_parent_pointer(this) ;
	
	if (iter == ((UD_Map_data*)data_ptr)->elem.end())
	{
		map<UD_universal_data, UD_universal_data*> newone ;
		newone.insert(pair<UD_universal_data, UD_universal_data*>(key, val)) ;
		((UD_Map_data*)data_ptr)->elem.insert(pair<int, map<UD_universal_data, UD_universal_data*> >(aspect_id, newone) ) ;
	}
	else 
	{
		map<UD_universal_data, UD_universal_data*>::iterator miter ;

		miter = iter->second.find(key) ;
		if( miter == iter->second.end()) 
			iter->second.insert(pair<UD_universal_data, UD_universal_data*>(key, val) ) ;
		else
			UD_print_message("insert fail - already existed") ;
	}
}

UD_universal_data* UD_Map::find(UD_universal_data key, int aspect_id){
	// check the type tag of key (need the same type for key comparison)
	if ( ((this->get_type_tag_key() != T_unlimited))&&(key.get_type_tag() != this->get_type_tag_key()) )
		UD_print_error_message("the type tag of key is not the same") ;

	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Map_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Map_data*)data_ptr)->elem.end())
		return NULL ;

	map<UD_universal_data, UD_universal_data*>::iterator miter ;

	miter = iter->second.find(key) ;
	if(miter != iter->second.end())
		return miter->second ;
	else 
		return NULL ;
}

void UD_Map::erase(UD_universal_data key, int aspect_id){
	// check the type tag of key (need the same type for key comparison)
	if ( ((this->get_type_tag_key() != T_unlimited))&&(key.get_type_tag() != this->get_type_tag_key()) )
		UD_print_error_message("the type tag of key is not the same") ;	

	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Map_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Map_data*)data_ptr)->elem.end())
		return ;

	map<UD_universal_data, UD_universal_data*>::iterator miter ;
	miter = iter->second.find(key) ;
	if (miter != iter->second.end())
	{
		UD_universal_data* tmp = miter->second ;
		tmp->remove_parent_pointer(this) ;
		iter->second.erase(key) ;
	}
}

unsigned int UD_Map::size(int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Map_data*)data_ptr)->elem.find(aspect_id) ; 
	if (iter == ((UD_Map_data*)data_ptr)->elem.end())
		return 0 ; 
	else 
		return iter->second.size() ;
}

list<int> UD_Map::get_aspect_in_structrue(){
	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for( iter = ((UD_Map_data*)data_ptr)->elem.begin() ; iter != ((UD_Map_data*)data_ptr)->elem.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

void UD_Map::begin(int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Map_data*)data_ptr)->elem.find(aspect_id) ;

	if (iter == ((UD_Map_data*)data_ptr)->elem.end())
		UD_print_error_message("in Map, can't find aspect "+aspect_id) ;

	((UD_Map_data*)(this->data_ptr))->iter = iter->second.begin() ;
}

bool UD_Map::end(int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Map_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Map_data*)data_ptr)->elem.end() )
		return true ;
	else 
		return (((UD_Map_data*)(this->data_ptr))->iter == iter->second.end()) ;
}

void UD_Map::operator ++(int){
	((UD_Map_data*)(this->data_ptr))->iter++ ;
}

UD_universal_data UD_Map::key(){
	return ((UD_Map_data*)(this->data_ptr))->iter->first ;
}

UD_universal_data* UD_Map::value(){
	return ((UD_Map_data*)(this->data_ptr))->iter->second ;
}

bool UD_Map::operator ==(UD_universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		UD_Map* tmp = (UD_Map*)(&other_data) ;
		if( this->size() != tmp->size())
			return false ;
		
		this->begin() ; 
		tmp->begin() ; 
		while( !this->end())
		{
			// compare key 
			if(this->key()  != tmp->key())
				return false ;
			// compare value 
			if( *(this->value()) != *(tmp->value()))
				return false ;
			(*this)++ ;
			(*tmp)++ ;
		}
		return true ;
	}
}

bool UD_Map::operator !=(UD_universal_data other_data){
	return !( (*this)  == other_data) ;
}

bool UD_Map::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_map)
	{
		int priority1 = UD_get_type_tag_priority(T_map) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Map* tmp = (UD_Map*)(&rhs) ;
		// compare key 
		((UD_Map*)this)->begin() ; 
		tmp->begin() ; 
		while( !((UD_Map*)this)->end() && !tmp->end())
		{
			if(((UD_Map*)this)->key()  == tmp->key())
			{
				if (((UD_Map*)this)->value() == tmp->value())
				{
					(*((UD_Map*)this))++ ;
					(*tmp)++ ;
				}
				else
					return ((UD_Map*)this)->value() < tmp->value() ;
			}
			else 
				return ((UD_Map*)this)->key() < tmp->key() ;
		}
		if(tmp->end())
			return false ;
		else 
			return true ;
	}
}

//
UD_Set_data::UD_Set_data(){
	elem_type_tag = T_unlimited ;
}

UD_Set::UD_Set(){
	set_type_tag(T_set) ;
	this->data_ptr = (unsigned*)(new UD_Set_data) ;
	((UD_Set_data*)data_ptr)->owner = this ;
	set<UD_universal_data*> newone ;
	((UD_Set_data*)data_ptr)->elem.insert(pair<int, set<UD_universal_data*> >(0, newone) ) ;
}

UD_Set::~UD_Set(){
	delete this->data_ptr ;
}

UD_Set::UD_Set(const UD_Set& s) {
	this->type_tag = ((UD_Set*)(&s))->get_type_tag() ;
	this->parent_ptr = ((UD_Set*)(&s))->get_parent_pointer() ;
	this->attribute_list = ((UD_Set*)(&s))->attribute_list ;
	if(  ((UD_Set*)(&s))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_Set*)(&s))) ;
}

void UD_Set::set_elem_type_tag(UD_Type_Tag tt) {
	((UD_Set_data*)(this->data_ptr))->elem_type_tag = tt ;
}

UD_Type_Tag UD_Set::get_elem_type_tag() {
	return ((UD_Set_data*)(this->data_ptr))->elem_type_tag ;
}

void UD_Set::insert(UD_universal_data *d, int aspect_id){
	// check type tag of key 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()) )
		UD_print_error_message("The type tag is not the same") ;

	d->set_parent_pointer(this) ;

	map<int, set<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Set_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter== ((UD_Set_data*)data_ptr)->elem.end())
	{
		set<UD_universal_data*> newone ;
		newone.insert(d) ;
		((UD_Set_data*)data_ptr)->elem.insert(pair<int, set<UD_universal_data*> >(aspect_id, newone) ) ;
	}
	else 
		iter->second.insert(d) ;
}

void UD_Set::erase(UD_universal_data *d, int aspect_id){
	// check type tag of key 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()) )
		UD_print_error_message("The type tag is not the same") ;

	map<int, set<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Set_data*)data_ptr)->elem.find(aspect_id) ;

	if (iter == ((UD_Set_data*)data_ptr)->elem.end())
		return ;

	set<UD_universal_data*>::iterator siter ;
	siter = iter->second.find (d) ;
	if (siter != iter->second.end() )
		(*siter)->remove_parent_pointer(this) ;
	iter->second.erase(d) ;
}

unsigned int UD_Set::size(int aspect_id){
	map<int, set<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Set_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Set_data*)data_ptr)->elem.end()) 
		return 0 ;
	else 
		return iter->second.size() ;
}

list<int> UD_Set::get_aspect_in_structure(){
	map<int, set<UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = ((UD_Set_data*)data_ptr)->elem.begin() ; iter != ((UD_Set_data*)data_ptr)->elem.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

void UD_Set::begin(int aspect_id){
	map<int, set<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Set_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Set_data*)data_ptr)->elem.end())
		UD_print_error_message("in Set, can't find aspect "+aspect_id) ;

	((UD_Set_data*)(this->data_ptr))->iter = iter->second.begin() ;
}

bool UD_Set::end(int aspect_id){
	map<int, set<UD_universal_data*> >::iterator iter ;
	iter = ((UD_Set_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Set_data*)data_ptr)->elem.end() )
		return true ;
	else 
		return (((UD_Set_data*)(this->data_ptr))->iter ==iter->second.end()) ;
}

void UD_Set::operator ++(int){
	((UD_Set_data*)(this->data_ptr))->iter++ ;
}

UD_universal_data* UD_Set::operator *(){
	return *(((UD_Set_data*)(this->data_ptr))->iter) ;
}

bool UD_Set::operator ==(UD_universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator(this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator(&other_data) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) &&(!iter2->end()))
		{
			if(  *(*(*iter1)) != *(*(*iter2)) )
				return false ;
			iter1->advance() ; 
			iter2->advance() ;
		}
		return true ;
	}
}

bool UD_Set::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Set::operator <(const UD_universal_data& rhs) const {
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_set)
	{
		int priority1 = UD_get_type_tag_priority(T_set) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator((UD_universal_data*)this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator( (UD_universal_data*)(&rhs)) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) && (!iter2->end())) 
		{
			if( *(*(*iter1)) == *(*(*iter2)) )
			{
				iter1->advance() ; 
				iter2->advance() ;
			}
			else 
				return *(*(*iter1)) < *(*(*iter2)) ;
		}
		if( iter2->end())
			return false ;
		else 
			return true ;
	}
}

//
UD_List_data::UD_List_data() {
	elem_type_tag = T_unlimited ;
}

UD_List::UD_List(){
	set_type_tag(T_list) ;
	this->data_ptr = (unsigned*)(new UD_List_data) ;
	((UD_List_data*)data_ptr)->owner = this ;
	list<UD_universal_data*> newone ;
	((UD_List_data*)data_ptr)->elem.insert(pair<int, list<UD_universal_data*> >(0, newone) ) ;
}

UD_List::~UD_List(){
	delete this->data_ptr ;
}

UD_List::UD_List(const UD_List& l) {
	this->type_tag = ((UD_List*)(&l))->get_type_tag() ;
	this->parent_ptr = ((UD_List*)(&l))->get_parent_pointer() ;
	this->attribute_list = ((UD_List*)(&l))->attribute_list ;
	if(  ((UD_List*)(&l))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((UD_List*)(&l))) ;
}

void UD_List::set_elem_type_tag(UD_Type_Tag tt) {
	((UD_List_data*)(this->data_ptr))->elem_type_tag = tt ;
}

UD_Type_Tag UD_List::get_elem_type_tag() {
	return ((UD_List_data*)(this->data_ptr))->elem_type_tag ;
}

void UD_List::push_back(UD_universal_data *data, int aspect_id){
	//check the type tag of data 
	if ((this->get_elem_type_tag() != T_unlimited)&&(data->get_type_tag() != this->get_elem_type_tag()) )
		UD_print_error_message("The type tag is not the same") ;

	data->set_parent_pointer(this) ;
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = ((UD_List_data*)data_ptr)->elem.find(aspect_id) ;
	if(iter == ((UD_List_data*)data_ptr)->elem.end())
	{
		list<UD_universal_data*> newone ;
		newone.push_back(data) ;
		((UD_List_data*)data_ptr)->elem.insert(pair<int, list<UD_universal_data*> >(aspect_id, newone) ) ;
	}
	else 
		iter->second.push_back(data) ;
}

void UD_List::pop_back(int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = ((UD_List_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_List_data*)data_ptr)->elem.end())
		return ;

	if (iter->second.size() != 0){
		iter->second.back()->remove_parent_pointer(this) ;
		iter->second.pop_back() ;
	}
}

unsigned int UD_List::size(int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = ((UD_List_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_List_data*)data_ptr)->elem.end())
		return 0 ;
	else 
		return iter->second.size() ;
}

list<int> UD_List::get_aspect_in_structrue(){
	map<int, list<UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = ((UD_List_data*)data_ptr)->elem.begin() ; iter != ((UD_List_data*)data_ptr)->elem.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

void UD_List::begin(int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = ((UD_List_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_List_data*)data_ptr)->elem.end())
		UD_print_error_message("in List, can't find aspect "+aspect_id) ;

	((UD_List_data*)(this->data_ptr))->iter = iter->second.begin() ;
}

bool UD_List::end(int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = ((UD_List_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_List_data*)data_ptr)->elem.end()) 
		return true ;
	else 
		return (((UD_List_data*)(this->data_ptr))->iter == iter->second.end()) ;
}

void UD_List::operator ++(int){
	((UD_List_data*)(this->data_ptr))->iter++ ;
}

UD_universal_data* UD_List::operator *(){
	return *(((UD_List_data*)(this->data_ptr))->iter);
}

void UD_List::erase(int aspect_id){
	map<int, list<UD_universal_data*> >::iterator iter ;
	iter = ((UD_List_data*)data_ptr)->elem.find(aspect_id) ;

	if (iter == ((UD_List_data*)data_ptr)->elem.end())
		return ;

	UD_universal_data* tmp = *(((UD_List_data*)(this->data_ptr))->iter) ;
	tmp->remove_parent_pointer(this) ;
	iter->second.erase(((UD_List_data*)(this->data_ptr))->iter) ;
}

bool UD_List::operator ==(UD_universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator(this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator(&other_data) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) &&(!iter2->end()))
		{
			if(  *(*(*iter1)) != *(*(*iter2)) )
				return false ;
			iter1->advance() ; 
			iter2->advance() ;
		}
		return true ;
	}
}

bool UD_List::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_List::operator <(const UD_universal_data& rhs) const {
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_list)
	{
		int priority1 = UD_get_type_tag_priority(T_list) ;
		int priority2 = UD_get_type_tag_priority(((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Virtual_Iterator* iter1 = UD_create_iterator((UD_universal_data*)this) ;
		UD_Virtual_Iterator* iter2 = UD_create_iterator( (UD_universal_data*)(&rhs)) ;
		iter1->begin() ; 
		iter2->begin() ;
		while( (!iter1->end()) && (!iter2->end())) 
		{
			if( *(*(*iter1)) == *(*(*iter2)) )
			{
				iter1->advance() ; 
				iter2->advance() ;
			}
			else 
				return *(*(*iter1)) < *(*(*iter2)) ;
		}
		if( iter2->end())
			return false ;
		else 
			return true ;
	}
}

// 
UD_Multimap_data::UD_Multimap_data(){
	type_tag_key = T_unlimited ;
	type_tag_value = T_unlimited ;
}

UD_Multimap::UD_Multimap(){
	set_type_tag(T_multimap) ;
	this->data_ptr = (unsigned*)(new UD_Multimap_data()) ;
	((UD_Multimap_data*)data_ptr)->owner = this ;
	multimap<UD_universal_data, UD_universal_data*> newone ;
	((UD_Multimap_data*)data_ptr)->elem.insert(pair<int, multimap<UD_universal_data, UD_universal_data*> > (0, newone) ) ;
}

UD_Multimap::~UD_Multimap(){
	delete data_ptr ;
}

UD_Multimap::UD_Multimap(const UD_Multimap &m){
	this->type_tag = ((UD_Multimap*)(&m))->get_type_tag() ;
	this->parent_ptr = ((UD_Multimap*)(&m))->get_parent_pointer() ;
	this->attribute_list = ((UD_Multimap*)(&m))->attribute_list ;
	if( ((UD_Multimap*)(&m))->get_data() == NULL)
		this->data_ptr = NULL ;
	else 
		this->data_ptr = copy_universal_data( *((UD_Multimap*)(&m))) ;
}

void UD_Multimap::set_type_tag_key(UD_Type_Tag tt){
	((UD_Multimap_data*)(this->data_ptr))->type_tag_key = tt ;
}

void UD_Multimap::set_type_tag_value(UD_Type_Tag tt){
	((UD_Multimap_data*)(this->data_ptr))->type_tag_value = tt ;
}

UD_Type_Tag UD_Multimap::get_type_tag_key(){
	return ((UD_Multimap_data*)(this->data_ptr))->type_tag_key ;
}

UD_Type_Tag UD_Multimap::get_type_tag_value(){
	return ((UD_Multimap_data*)(this->data_ptr))->type_tag_value ;
}

void UD_Multimap::insert(UD_universal_data key, UD_universal_data *value, int aspect_id){
	// check the type tag of key and value 
	if( ((this->get_type_tag_key() != T_unlimited)) && (key.get_type_tag())) 
		UD_print_error_message("The type tag of key is not the same") ;

	if( (this->get_type_tag_value() != T_unlimited) && (value->get_type_tag()))
		UD_print_error_message("The type tag of value is not the same") ;

	value->set_parent_pointer(this) ;

	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Multimap_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter== ((UD_Multimap_data*)data_ptr)->elem.end())
	{
		multimap<UD_universal_data, UD_universal_data*> newone ;
		newone.insert(pair<UD_universal_data, UD_universal_data*>(key, value) ) ;
		((UD_Multimap_data*)data_ptr)->elem.insert(pair<int, multimap<UD_universal_data, UD_universal_data*> >(aspect_id, newone) ) ;
	}
	else 
		iter->second.insert(pair<UD_universal_data, UD_universal_data*>(key, value)) ;
}

unsigned int UD_Multimap::size(int aspect_id){
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Multimap_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Multimap_data*)data_ptr)->elem.end()) 
		return 0 ;
	else 
		return iter->second.size() ;
}

list<int> UD_Multimap::get_aspect_in_structure() {
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = ((UD_Multimap_data*)data_ptr)->elem.begin() ; iter != ((UD_Multimap_data*)data_ptr)->elem.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list;
}

bool UD_Multimap::operator ==(UD_universal_data other_data) {
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		UD_Multimap* tmp = (UD_Multimap*)(&other_data) ;
		if(this->size() != tmp->size())
			return false ;

		this->begin() ;
		tmp->begin() ;
		while( !this->end())
		{
			// compare key
			if( this->key() != tmp->key())
				return false ;
			// compare value 
			if( *(this->value()) != *(tmp->value()))
				return false ;
			(*this)++ ;
			(*tmp)++ ;
		}
		return true ;
	}
}

bool UD_Multimap::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Multimap::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_multimap)
	{
		int priority1 = UD_get_type_tag_priority(T_multimap) ;
		int priority2 = UD_get_type_tag_priority( ((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Multimap* tmp = (UD_Multimap*)(&rhs) ;
		// compare key 
		((UD_Multimap*)this)->begin() ; 
		tmp->begin() ; 
		while( !((UD_Multimap*)this)->end() && !tmp->end())
		{
			if( ((UD_Multimap*)this)->key() == tmp->key() )
			{
				if ( ((UD_Multimap*)this)->value() == tmp->value())
				{
					(*(UD_Multimap*)this)++ ;
					(*tmp)++ ;
				}
				else 
					return ((UD_Multimap*)this)->value() < tmp->value() ;
			}
			else 
				return ((UD_Multimap*)this)->key() < tmp->key() ;
		}
		if( tmp->end())
			return false ;
		else 
			return true ;
	}
}

void UD_Multimap::begin(int aspect_id){
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Multimap_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Multimap_data*)data_ptr)->elem.end() )
		UD_print_error_message("in Multimap, can't find aspect "+aspect_id) ;

	((UD_Multimap_data*)(this->data_ptr))->iter = iter->second.begin() ;
}

bool UD_Multimap::end(int aspect_id){
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Multimap_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Multimap_data*)data_ptr)->elem.end()) 
		return true ;
	else 
		return ((UD_Multimap_data*)(this->data_ptr))->iter == iter->second.end() ;
}

void UD_Multimap::operator ++(int){
	((UD_Multimap_data*)data_ptr)->iter++ ;
}

void UD_Multimap::erase(int aspect_id){
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator iter ;
	iter = ((UD_Multimap_data*)data_ptr)->elem.find(aspect_id) ;
	if (iter == ((UD_Multimap_data*)data_ptr)->elem.end())
		return ;
	UD_universal_data* tmp = ((UD_Multimap_data*)data_ptr)->iter->second ;
	tmp->remove_parent_pointer(this) ;
	iter->second.erase( ((UD_Multimap_data*)data_ptr)->iter) ;
}

UD_universal_data UD_Multimap::key(){
	return ((UD_Multimap_data*)data_ptr)->iter->first ;
}

UD_universal_data* UD_Multimap::value(){
	return ((UD_Multimap_data*)data_ptr)->iter->second ;
}

UD_Attribute_List_data::UD_Attribute_List_data() {
	type_tag_name = T_unlimited ;
	type_tag_value = T_unlimited ;
}

UD_Attribute_List::UD_Attribute_List(){
	set_type_tag(T_attribute_list) ;
	this->data_ptr = (unsigned*)(new UD_Attribute_List_data()) ;
	((UD_Attribute_List_data*)data_ptr)->owner = this ;
	map<UD_universal_data, UD_universal_data> newone ;
	((UD_Attribute_List_data*)data_ptr)->attributes.insert(pair<int, map<UD_universal_data, UD_universal_data> >(0, newone) ) ;
}

UD_Attribute_List::~UD_Attribute_List(){
	delete data_ptr ;
}

UD_Attribute_List::UD_Attribute_List(const UD_Attribute_List &al){
	this->type_tag = ((UD_Attribute_List*)(&al))->get_type_tag() ;
	this->parent_ptr = ((UD_Attribute_List*)(&al))->get_parent_pointer() ;
	this->attribute_list = ((UD_Attribute_List*)(&al))->attribute_list ;
	if( ((UD_Attribute_List*)(&al))->get_data() == NULL)
		this->data_ptr = NULL ;
	else 
		this->data_ptr = copy_universal_data( *((UD_Attribute_List*)(&al))) ;
}

void UD_Attribute_List::add_attribute_al(std::string key, UD_universal_data val, int aspect_id){
	UD_String k ;
	k.set_value(key) ;

	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ; 
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end()) 
	{
		map<UD_universal_data, UD_universal_data> newone ;
		newone.insert(pair<UD_universal_data, UD_universal_data>(k, val) ) ;
		((UD_Attribute_List_data*)data_ptr)->attributes.insert(pair<int, map<UD_universal_data, UD_universal_data> >(aspect_id, newone) ) ;
	}
	else 
	{
		map<UD_universal_data, UD_universal_data>::iterator aiter ;
		aiter = iter->second.find(k) ;
		if(aiter == iter->second.end())
			iter->second.insert(pair<UD_universal_data, UD_universal_data>(k, val) ) ;
		else
			UD_print_message("insert fail - already exist") ;
	}
}

void UD_Attribute_List::add_attribute_al(UD_universal_data key, UD_universal_data val, int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
	{
		map<UD_universal_data, UD_universal_data> newone ;
		newone.insert(pair<UD_universal_data, UD_universal_data>(key, val) ) ;
		((UD_Attribute_List_data*)data_ptr)->attributes.insert(pair<int, map<UD_universal_data, UD_universal_data> >(aspect_id, newone) ) ;
	}
	else 
	{
		map<UD_universal_data, UD_universal_data>::iterator aiter ;
		aiter = iter->second.find(key) ;
		if(aiter == iter->second.end())
			iter->second.insert(pair<UD_universal_data, UD_universal_data>(key, val) ) ;
		else 
			UD_print_message("insert fail - already exist") ;
	}
}

UD_universal_data UD_Attribute_List::get_attribute_al(std::string key, int aspect_id){
	UD_universal_data empty ;
	UD_String k ;
	k.set_value(key) ;

	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end()) 
		return empty ;

	map<UD_universal_data, UD_universal_data>::iterator aiter ;
	aiter = iter->second.find(k) ;
	if(aiter == iter->second.end())
		return empty ;
	else
		return aiter->second ;
}

UD_universal_data UD_Attribute_List::get_attribute_al(UD_universal_data key, int aspect_id) {
	UD_universal_data empty ;

	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return empty ;

	map<UD_universal_data, UD_universal_data>::iterator aiter ;
	aiter = iter->second.find(key) ;
	if(aiter == iter->second.end())
		return empty ;
	else
		return aiter->second ;
}

UD_universal_data* UD_Attribute_List::get_attribute_ref_al(std::string key, int aspect_id){
	UD_String k ;
	k.set_value(key) ;

	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return NULL ;

	map<UD_universal_data, UD_universal_data>::iterator aiter ;
	aiter = iter->second.find(k) ;
	if(aiter == iter->second.end()) 
		return NULL ;
	else
		return &(aiter->second) ;
}

UD_universal_data* UD_Attribute_List::get_attribute_ref_al(UD_universal_data key, int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end()) 
		return NULL ;

	map<UD_universal_data, UD_universal_data>::iterator aiter ;
	aiter = iter->second.find(key) ;
	if(aiter == iter->second.end() )
		return NULL ;
	else 
		return &(aiter->second) ;
}

void UD_Attribute_List::set_attribute_al(std::string key, UD_universal_data val, int aspect_id){
	UD_String k ;
	k.set_value(key) ;
	
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return ;

	map<UD_universal_data, UD_universal_data>::iterator aiter ;
	aiter = iter->second.find(k) ;
	if (aiter != iter->second.end())
		aiter->second = val ;
}

void UD_Attribute_List::set_attribute_al(UD_universal_data key, UD_universal_data val, int aspect_id) {
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return ;

	map<UD_universal_data, UD_universal_data>::iterator aiter ;
	aiter = iter->second.find(key) ;
	if (aiter != iter->second.end())
		aiter->second = val ;
}

void UD_Attribute_List::remove_attribute_al(std::string key, int aspect_id) {
	UD_String k ;
	k.set_value(key) ;

	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return ;

	iter->second.erase(k) ;
}

void UD_Attribute_List::remove_attribute_al(UD_universal_data key, int aspect_id) {
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return ;

	iter->second.erase(key) ;
}

unsigned int UD_Attribute_List::size(int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end() )
		return 0 ;
	else 
		return iter->second.size() ;
}

list<int> UD_Attribute_List::get_aspect_in_structure(){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	list<int> aspect_list ;
	for(iter = ((UD_Attribute_List_data*)data_ptr)->attributes.begin() ; iter != ((UD_Attribute_List_data*)data_ptr)->attributes.end() ; iter++)
		aspect_list.push_back(iter->first) ;
	return aspect_list ;
}

bool UD_Attribute_List::operator ==(UD_universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		UD_Attribute_List* tmp = (UD_Attribute_List*)(&other_data) ;
		if(this->size() != tmp->size())
			return false ;

		this->begin() ;
		tmp->begin() ;
		while( !this->end())
		{
			// compare key
			if( this->get_attribute_name_al() != tmp->get_attribute_name_al())
				return false ;
			// compare value 
			if( this->get_attribute_value_al() != tmp->get_attribute_value_al())
				return false ;
			(*this)++ ;
			(*tmp)++ ;
		}
		return true ;
	}
}

bool UD_Attribute_List::operator !=(UD_universal_data other_data){
	return !( (*this) == other_data) ;
}

bool UD_Attribute_List::operator <(const UD_universal_data &rhs) const{
	// general version less function 
	if( ((UD_universal_data*)(&rhs))->get_type_tag() != T_attribute_list)
	{
		int priority1 = UD_get_type_tag_priority(T_attribute_list) ;
		int priority2 = UD_get_type_tag_priority( ((UD_universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		UD_Attribute_List* tmp = (UD_Attribute_List*)(&rhs) ;
		// compare key 
		((UD_Attribute_List*)this)->begin() ; 
		tmp->begin() ; 
		while( !((UD_Attribute_List*)this)->end() && !tmp->end())
		{
			if( ((UD_Attribute_List*)this)->get_attribute_name_al() == tmp->get_attribute_name_al() )
			{
				if(((UD_Attribute_List*)this)->get_attribute_value_al() == tmp->get_attribute_value_al())
				{
					(*(UD_Attribute_List*)this)++ ;
					(*tmp)++ ;
				}
				else 
					return ((UD_Attribute_List*)this)->get_attribute_value_al() < tmp->get_attribute_value_al() ;
			}
			else 
				return ((UD_Attribute_List*)this)->get_attribute_name_al() < tmp->get_attribute_name_al() ;
		}
		if(tmp->end())
			return false ;
		else 
			return true ;
	}
}

void UD_Attribute_List::begin(int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		UD_print_error_message("in Attribute List, can't find aspect "+aspect_id) ;

	((UD_Attribute_List_data*)data_ptr)->attrs_iter = iter->second.begin() ;
}

bool UD_Attribute_List::end(int aspect_id){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator iter ;
	iter = ((UD_Attribute_List_data*)data_ptr)->attributes.find(aspect_id) ;
	if (iter == ((UD_Attribute_List_data*)data_ptr)->attributes.end())
		return true ;
	else 
		return ((UD_Attribute_List_data*)data_ptr)->attrs_iter == iter->second.end() ;
}

void UD_Attribute_List::operator ++(int){
	((UD_Attribute_List_data*)data_ptr)->attrs_iter++ ;
}

UD_universal_data UD_Attribute_List::get_attribute_name_al(){
	return ((UD_Attribute_List_data*)data_ptr)->attrs_iter->first ;
}

UD_universal_data UD_Attribute_List::get_attribute_value_al(){
	return ((UD_Attribute_List_data*)data_ptr)->attrs_iter->second ;
}

//###########################################
// iterator 
//###########################################
UD_Vector_Iterator::UD_Vector_Iterator(UD_universal_data *org, int aspect_id)
{
	// check whether it is vector 
	if ( org->get_type_tag() != T_vector )
		UD_print_error_message("The organziation is not vector") ;
	this->organization = org ;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_Vector_Iterator::~UD_Vector_Iterator()
{
}

void UD_Vector_Iterator::begin()
{
	map<int, vector<UD_universal_data*> >::iterator citer ;
	citer = ((UD_Vector_data*)(this->organization->get_data()))->vec.find(aspect) ;
	if (citer == ((UD_Vector_data*)(this->organization->get_data()))->vec.end())
		UD_print_error_message("In Vector Iterator: can't find aspect "+aspect) ;

	iter = citer->second.begin() ;
}

bool UD_Vector_Iterator::end()
{
	map<int, vector<UD_universal_data*> >::iterator citer ;
	citer = ((UD_Vector_data*)(this->organization->get_data()))->vec.find(aspect) ;
	if (citer == ((UD_Vector_data*)(this->organization->get_data()))->vec.end())
		return true ;
	else 
		return iter == citer->second.end() ;
}

void UD_Vector_Iterator::advance()
{
	iter++ ;
}

UD_universal_data* UD_Vector_Iterator::operator *()
{
	if (end())
		return NULL ;
	else
		return *iter ;
}
//
UD_Pair_Iterator::UD_Pair_Iterator(UD_universal_data *org, int aspect_id)
{
	// check whether it is pair 
	if ( org->get_type_tag() != T_pair )
		UD_print_error_message("The organization is not pair") ;

	this->organization = org ;
	this->count = 0;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_Pair_Iterator::~UD_Pair_Iterator()
{
}

void UD_Pair_Iterator::begin()
{
	this->count = 1 ;
}

bool UD_Pair_Iterator::end()
{
	return this->count >2;
}

void UD_Pair_Iterator::advance() 
{
	this->count++ ;
}

UD_universal_data* UD_Pair_Iterator::operator *()
{
	map<int, pair<UD_universal_data*, UD_universal_data*> >::iterator citer ;
	citer = ((UD_Pair_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Pair_data*)(this->organization->get_data()))->elem.end())
		return NULL ;

	if(this->count == 1)
		return citer->second.first ;
	else if(this->count == 2)
		return citer->second.second ;
	else
		return NULL ;
}
//
UD_List_Iterator::UD_List_Iterator(UD_universal_data *org, int aspect_id)
{
	// check whether it is list
	if ( org->get_type_tag() != T_list )
		UD_print_error_message("The organization is not list") ;

	this->organization = org ;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_List_Iterator::~UD_List_Iterator()
{
}

void UD_List_Iterator::begin()
{
	map<int, list<UD_universal_data*> >::iterator citer ;
	citer = ((UD_List_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_List_data*)(this->organization->get_data()))->elem.end())
		UD_print_error_message("in List iterator, can't find aspect "+aspect) ;

	iter = citer->second.begin() ;
}

bool UD_List_Iterator::end()
{
	map<int, list<UD_universal_data*> >::iterator citer ;
	citer = ((UD_List_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_List_data*)(this->organization->get_data()))->elem.end())
		return true ;
	else 
		return iter == citer->second.end() ;
}

void UD_List_Iterator::advance()
{
	iter++ ;
}

UD_universal_data* UD_List_Iterator::operator *()
{
	if (end())
		return NULL ;
	else 
		return *iter ;
}
//
UD_Set_Iterator::UD_Set_Iterator(UD_universal_data *org, int aspect_id)
{
	// check whether it is set 
	if ( org->get_type_tag() != T_set )
		UD_print_error_message("The organization is not set") ;
	this->organization = org;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_Set_Iterator::~UD_Set_Iterator()
{
}

void UD_Set_Iterator::begin()
{
	map<int, set<UD_universal_data*> >::iterator citer ;
	citer = ((UD_Set_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Set_data*)(this->organization->get_data()))->elem.end()) 
		UD_print_error_message("in Set Iterator, can't find aspect "+aspect) ;
	iter = citer->second.begin() ;
}

bool UD_Set_Iterator::end()
{
	map<int, set<UD_universal_data*> >::iterator citer ;
	citer = ((UD_Set_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Set_data*)(this->organization->get_data()))->elem.end()) 
		return true; 
	else 
		return iter == citer->second.end() ;
}

void UD_Set_Iterator::advance()
{
	iter++ ;
}

UD_universal_data* UD_Set_Iterator::operator *()
{
	if (end())
		return NULL ;
	else 
		return *iter ;
}
//
UD_Map_Iterator::UD_Map_Iterator(UD_universal_data *org, int aspect_id)
{
	// check whether it is map
	if ( org->get_type_tag() != T_map )
		UD_print_error_message("The organization is not map") ;

	this->organization = org ;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_Map_Iterator::~UD_Map_Iterator()
{
}

void UD_Map_Iterator::begin()
{
	map<int, map<UD_universal_data, UD_universal_data*> >::iterator citer ;
	citer = ((UD_Map_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Map_data*)(this->organization->get_data()))->elem.end())
		UD_print_error_message("in Map Iterator, can't find aspect "+aspect) ;

	iter = citer->second.begin() ;
}

bool UD_Map_Iterator::end()
{
	map<int, map<UD_universal_data, UD_universal_data*> >::iterator citer ;
	citer = ((UD_Map_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Map_data*)(this->organization->get_data()))->elem.end())
		return true ;
	else 
		return iter == citer->second.end() ;
}

void UD_Map_Iterator::advance()
{
	iter++ ;
}

UD_universal_data* UD_Map_Iterator::operator *()
{
	if (end())
		return NULL ;
	else 
		return iter->second ;
}

//
UD_Multimap_Iterator::UD_Multimap_Iterator(UD_universal_data *org, int aspect_id){
	// check whether it is map
	if ( org->get_type_tag() != T_multimap )
		UD_print_error_message("The organization is not multimap") ;
	this->organization = org ;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_Multimap_Iterator::~UD_Multimap_Iterator(){
}

void UD_Multimap_Iterator::begin(){
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator citer ;
	citer = ((UD_Multimap_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Multimap_data*)(this->organization->get_data()))->elem.end()) 
		UD_print_error_message("in Multimap Iterator, can't find aspect "+aspect) ;
	iter = citer->second.begin() ;
}

bool UD_Multimap_Iterator::end(){
	map<int, multimap<UD_universal_data, UD_universal_data*> >::iterator citer ;
	citer = ((UD_Multimap_data*)(this->organization->get_data()))->elem.find(aspect) ;
	if (citer == ((UD_Multimap_data*)(this->organization->get_data()))->elem.end()) 
		return true ;
	else 
		return iter == citer->second.end() ;
}

void UD_Multimap_Iterator::advance(){
	iter++ ;
}

UD_universal_data* UD_Multimap_Iterator::operator *(){
	if (end())
		return NULL ;
	else 
		return iter->second ;
}

UD_Attribute_List_Iterator::UD_Attribute_List_Iterator(UD_universal_data *org, int aspect_id){
	if(org->get_type_tag() != T_attribute_list)
		UD_print_error_message("The organziation is not attribute list") ;
	this->organization = org ;
	this->set_tag(org->get_type_tag()) ;
	this->set_aspect(aspect_id) ;
}

UD_Attribute_List_Iterator::~UD_Attribute_List_Iterator(){
}

void UD_Attribute_List_Iterator::begin(){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator citer ;
	citer= ((UD_Attribute_List_data*)(this->organization->get_data()))->attributes.find(aspect) ;
	if (citer== ((UD_Attribute_List_data*)(this->organization->get_data()))->attributes.end())
		UD_print_error_message("in Attribute List, can't find aspect "+aspect) ;

	iter = citer->second.begin() ;
}

bool UD_Attribute_List_Iterator::end(){
	map<int, map<UD_universal_data, UD_universal_data> >::iterator citer ;
	citer= ((UD_Attribute_List_data*)(this->organization->get_data()))->attributes.find(aspect) ;
	if (citer== ((UD_Attribute_List_data*)(this->organization->get_data()))->attributes.end())
		return true ;
	else 
		return iter == citer->second.end() ;
}

void UD_Attribute_List_Iterator::advance(){
	iter++ ;
}

UD_universal_data* UD_Attribute_List_Iterator::operator *(){
	if (end())
		return NULL ;
	else 
		return &(iter->second) ;
}

