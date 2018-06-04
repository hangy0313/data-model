#include <iostream>
#include "universal_data.h"
//#include "incidence.h"
//#include "UD_hypergraph.h"

using namespace std; 

// from enumerate_constant.h
int structure_head = T_pair ;
int structure_tail = T_p_list ;
int primitive_head = T_int ;
int primitive_tail = T_incidence;

// lookup table 
// level 2: key to level 1 table's address 
map<universal_data, map<universal_data, universal_data*>*> L2_table ;
// level 1: key to component's address 
map<universal_data, universal_data*> L1_table ;

// get priority of type tag
int get_type_tag_priority(Type_Tag tt) 
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

//#################################
// abstract class 
//#################################
universal_data::universal_data(){
	this->type_tag = T_unknown ;
	this->parent_ptr = NULL ;
	this->data_ptr = NULL ;
}

universal_data::~universal_data(){
}

universal_data::universal_data(const universal_data &ud){
	this->type_tag = ((universal_data*)(&ud))->get_type_tag() ;
	this->user_defined_type_tag = ((universal_data*)(&ud))->get_user_defined_type_tag() ;
	this->parent_ptr = ((universal_data*)(&ud))->get_parent_point() ;
	this->attribute_list = ((universal_data*)(&ud))->attribute_list ;
	if(  ((universal_data*)(&ud))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((universal_data*)(&ud))) ;
}

// type_tag
void universal_data::set_type_tag(Type_Tag tt){
	type_tag = tt;
}

Type_Tag universal_data::get_type_tag(){
	return type_tag ;
}
// user defind type tag 
void universal_data::set_user_defined_type_tag(std::string tag) {
	user_defined_type_tag = tag ;
}

string universal_data::get_user_defined_type_tag(){
	return user_defined_type_tag ;
}
//parent pointer
void universal_data::set_parent_point(universal_data *ptr){
	parent_ptr = ptr ;
}

universal_data* universal_data::get_parent_point(){
	return parent_ptr ;
}

//data 
void universal_data::set_data(unsigned* d) {
	if(this->data_ptr == NULL)
		this->data_ptr = d ;
	else
	{
		delete this->data_ptr ;
		this->data_ptr = d ;
	}
}

unsigned* universal_data::get_data(){
	return this->data_ptr ;
}

// attribute list 
bool universal_data::add_attribute(std::string nm, universal_data ud){
	String tmp ;
	tmp.set_value(nm) ;
	// search name in attribute list 
	list<pair<universal_data, universal_data> >::iterator iter ;
	for(iter = attribute_list.begin() ; iter != attribute_list.end() ; iter ++){
		if( (*iter).first == tmp)
			return false ; // the tag is used 
	}
	attribute_list.push_back( pair<universal_data, universal_data>(tmp, ud) ) ;
	return true; 
}

void universal_data::set_attribute(string nm, universal_data ud) {
	String tmp ;
	tmp.set_value(nm) ;
	list<pair<universal_data, universal_data> >::iterator iter ;
	for(iter = attribute_list.begin() ; iter != attribute_list.end() ; iter ++){
		if( (*iter).first == tmp)
			iter->second = ud ;
	}
}

void universal_data::remove_attribute(std::string nm){
	String tmp ;
	tmp.set_value(nm) ;
	// search nm in attribute list 
	list<pair<universal_data, universal_data> >::iterator iter ;
	for(iter = attribute_list.begin() ; iter != attribute_list.end() ; iter ++){
		if( (*iter).first == tmp)
		{
			// remove from attribute list
			attribute_list.erase(iter) ;
			return ;
		}
	}
}

universal_data universal_data::find_attribute(std::string nm) {
	String tmp ;
	tmp.set_value(nm) ;
	list< pair<universal_data, universal_data> >::iterator titer ;
	for(titer = attribute_list.begin() ; titer != attribute_list.end() ; titer++)
		if(titer->first == tmp)
			break ;

	if(titer == attribute_list.end() ){
		universal_data empty_ud ;
		return empty_ud;
	}
	else
		return titer->second ;
}

universal_data* universal_data::find_attribute_ref(string nm){
	String tmp ;
	tmp.set_value(nm) ;
	list< pair<universal_data, universal_data> >::iterator titer ;
	for(titer = attribute_list.begin() ; titer != attribute_list.end() ; titer++)
		if(titer->first == tmp)
			break ;

	if(titer == attribute_list.end() )
		return NULL ;
	else
		return &(titer->second) ;
}

void universal_data::attribute_begin() {
	attr_iter = attribute_list.begin() ;
}

bool universal_data::attribute_end() {
	return (attr_iter == attribute_list.end() ) ;
}

void universal_data::attribute_next(){
	attr_iter++ ;
}

string universal_data::get_attribute_name() {
	String* ud = (String*)(&(attr_iter->first)) ;
	return *(ud->getptr()) ;
}

universal_data universal_data::get_attribute() {
	return attr_iter->second ;
}

universal_data& universal_data::operator =(universal_data other_data){

	this->type_tag = other_data.get_type_tag() ;
	this->user_defined_type_tag = other_data.get_user_defined_type_tag() ;
	this->parent_ptr = other_data.get_parent_point() ;
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

bool universal_data::operator ==(universal_data other_data){
	// detail design in corresponding class 
	switch(this->get_type_tag() ){
		case T_int: 
			return *((Int*)this) == other_data ;
			break ;
		case T_string: 
			return *((String*)this) == other_data ;
			break ;
		case T_char: 
			return *((Char*)this) == other_data ;
			break ;
		case T_float: 
			return *((Float*)this) == other_data ;
			break ;
		case T_double: 
			return *((Double*)this) == other_data ;
			break ;
		case T_bool: 
			return *((Bool*)this) == other_data ;
			break ;
//		case T_incidence: 
//			return *((Incidence*)this) == other_data ;
		case T_pair: 
			return *((Pair*)this) == other_data ;
		case T_set: 
			return *((Set*)this) == other_data ;
		case T_list: 
			return *((List*)this) == other_data ;
		case T_vector: 
			return *((Vector*)this) == other_data ;
		case T_map: 
			return *((Map*)this) == other_data ;
		case T_multimap: 
			return *((Multimap*)this) == other_data ;
		case T_attribute_list: 
			return *((Attribute_List*)this) == other_data ;
		case T_user_defined:
			return this->data_ptr == other_data.get_data() ;
		default:
			cout<<"ERROR: not support this operation == "<<endl ;
			exit(-1) ;
	}
}

bool universal_data::operator !=(universal_data other_data){
	// detail design in corresponding class 
	switch(this->get_type_tag() ){
		case T_int: 
			return *((Int*)this) != other_data ;
			break ;
		case T_string: 
			return *((String*)this) != other_data ;
			break ;
		case T_char: 
			return *((Char*)this) != other_data ;
			break ;
		case T_float: 
			return *((Float*)this) != other_data ;
			break ;
		case T_double: 
			return *((Double*)this) != other_data ;
			break ;
		case T_bool:
			return *((Bool*)this) != other_data ;
			break ;
//		case T_incidence: 
//			return *((Incidence*)this) != other_data ;
		case T_pair: 
			return *((Pair*)this) != other_data ;
		case T_set: 
			return *((Set*)this) != other_data ;
		case T_list: 
			return *((List*)this) != other_data ;
		case T_vector: 
			return *((Vector*)this) != other_data ;
		case T_map: 
			return *((Map*)this) != other_data ;
		case T_multimap: 
			return *((Multimap*)this) != other_data ;
		case T_attribute_list: 
			return *((Attribute_List*)this) != other_data ;
		default:
			cout<<"ERROR: not support this operation != "<<endl ;
			exit(-1) ;
	}
}

bool universal_data::operator <(const universal_data &rhs) const{
	// detail design in corresponding class 
	switch(((universal_data*)this)->get_type_tag() ){
		case T_int: 
			return *((Int*)this) < rhs ;
			break ;
		case T_string: 
			return *((String*)this) < rhs ;
			break ;
		case T_char: 
			return *((Char*)this) < rhs ;
			break ;
		case T_float: 
			return *((Float*)this) < rhs ;
			break ;
		case T_double: 
			return *((Double*)this) < rhs ;
			break ;
		case T_bool: 
			return *((Bool*)this) < rhs ;
			break ;
//		case T_incidence: 
//			return *((Incidence*)this) < rhs ;
		case T_pair: 
			return *((Pair*)this) < rhs ;
		case T_set: 
			return *((Set*)this) < rhs ;
		case T_list: 
			return *((List*)this) < rhs ;
		case T_vector: 
			return *((Vector*)this) < rhs ;
		case T_map: 
			return *((Map*)this) < rhs ;
		case T_multimap: 
			return *((Multimap*)this) < rhs ;
		case T_attribute_list: 
			return *((Attribute_List*)this) < rhs ;
		default:
			cout<<"ERROR: not support this operation < "<<endl ;
			exit(-1) ;
	}
}

unsigned* universal_data::copy_universal_data(universal_data &source){
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
			tmp = (unsigned*)(new symbolic_link_data); 
			((symbolic_link_data*)tmp)->L1_flag = ((symbolic_link_data*)(source.get_data()))->L1_flag ;
			((symbolic_link_data*)tmp)->L1_key = ((symbolic_link_data*)(source.get_data()))->L1_key ;
			((symbolic_link_data*)tmp)->L1_name = ((symbolic_link_data*)(source.get_data()))->L1_name ;
			((symbolic_link_data*)tmp)->table_pointer = ((symbolic_link_data*)(source.get_data()))->table_pointer ;
			((symbolic_link_data*)tmp)->Absolute_flag = ((symbolic_link_data*)(source.get_data()))->Absolute_flag ;
			((symbolic_link_data*)tmp)->path = ((symbolic_link_data*)(source.get_data()))->path ;
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
			tmp = (unsigned*)(new Pair_data) ;
			((Pair_data*)tmp)->elem = ((Pair_data*)(source.get_data()))->elem ;
			((Pair_data*)tmp)->type_tag_first = ((Pair_data*)(source.get_data()))->type_tag_first ;
			((Pair_data*)tmp)->type_tag_second = ((Pair_data*)(source.get_data()))->type_tag_second ;
			break ;
		case T_map: 
			tmp = (unsigned*)(new Map_data) ;
			((Map_data*)tmp)->elem = ((Map_data*)(source.get_data()))->elem ;
			((Map_data*)tmp)->type_tag_key = ((Map_data*)(source.get_data()))->type_tag_key ;
			((Map_data*)tmp)->type_tag_value = ((Map_data*)(source.get_data()))->type_tag_value ;
			break ;
		case T_vector: 
			tmp = (unsigned*)(new Vector_data) ;
			((Vector_data*)tmp)->elem_type_tag = ((Vector_data*)(source.get_data()))->elem_type_tag ;
			((Vector_data*)tmp)->vec = ((Vector_data*)(source.get_data()))->vec ;
			break ;
		case T_set: 
			tmp = (unsigned*)(new Set_data) ;
			((Set_data*)tmp)->elem = ((Set_data*)(source.get_data()))->elem ;
			((Set_data*)tmp)->elem_type_tag = ((Set_data*)(source.get_data()))->elem_type_tag ;
			break ;
		case T_list: 
			tmp = (unsigned*)(new List_data) ;
			((List_data*)tmp)->elem = ((List_data*)(source.get_data()))->elem ;
			((List_data*)tmp)->elem_type_tag = ((List_data*)(source.get_data()))->elem_type_tag ;
			break ;
		case T_multimap: 
			tmp = (unsigned*)(new Multimap_data) ;
			((Multimap_data*)tmp)->elem = ((Multimap_data*)(source.get_data()))->elem ;
			((Multimap_data*)tmp)->type_tag_key = ((Multimap_data*)(source.get_data()))->type_tag_key ;
			((Multimap_data*)tmp)->type_tag_value = ((Multimap_data*)(source.get_data()))->type_tag_value ;
			break ;
		case T_attribute_list: 
			tmp = (unsigned*)(new Attribute_List_data) ;
			((Attribute_List_data*)tmp)->attributes = ((Attribute_List_data*)(source.get_data()))->attributes ;
			break ;
//		case T_incidence: // not set up parent pointer
//			tmp = (unsigned*)(new Incidence_data) ;
//			((Incidence_data*)tmp)->locate_ptr = ((Incidence_data*)(source.get_data()))->locate_ptr ;
//			((Incidence_data*)tmp)->flag= ((Incidence_data*)(source.get_data()))->flag ;
//			((Incidence_data*)tmp)->terminal_ptr = ((Incidence_data*)(source.get_data()))->terminal_ptr ;
//			break ;
		case T_p_pair:  // not set up parent pointer
			tmp = (unsigned*)(new Pair_data) ;
			((p_Pair_data*)tmp)->elem = ((p_Pair_data*)(source.get_data()))->elem ;
			((p_Pair_data*)tmp)->type_tag_first = ((p_Pair_data*)(source.get_data()))->type_tag_first ;
			((p_Pair_data*)tmp)->type_tag_second = ((p_Pair_data*)(source.get_data()))->type_tag_second ;
			break ;
		case T_p_map:   // not set up parent pointer
			tmp = (unsigned*)(new p_Map_data) ;
			((p_Map_data*)tmp)->elem = ((p_Map_data*)(source.get_data()))->elem ;
			((p_Map_data*)tmp)->type_tag_key = ((p_Map_data*)(source.get_data()))->type_tag_key ;
			((p_Map_data*)tmp)->type_tag_value = ((p_Map_data*)(source.get_data()))->type_tag_value ;
			break ;
		case T_p_vector:   // not set up parent pointer
			tmp = (unsigned*)(new p_Vector_data) ;
			((p_Vector_data*)tmp)->elem_type_tag = ((p_Vector_data*)(source.get_data()))->elem_type_tag ;
			((p_Vector_data*)tmp)->vec = ((p_Vector_data*)(source.get_data()))->vec ;
			break ;
		case T_p_set:   // not set up parent pointer
			tmp = (unsigned*)(new p_Set_data) ;
			((p_Set_data*)tmp)->elem = ((p_Set_data*)(source.get_data()))->elem ;
			((p_Set_data*)tmp)->elem_type_tag = ((p_Set_data*)(source.get_data()))->elem_type_tag ;
			break ;
		case T_p_list:   // not set up parent pointer
			tmp = (unsigned*)(new p_List_data) ;
			((p_List_data*)tmp)->elem = ((p_List_data*)(source.get_data()))->elem ;
			((p_List_data*)tmp)->elem_type_tag = ((p_List_data*)(source.get_data()))->elem_type_tag ;
			break ;
//		case T_ud_hypergraph: // not set up parent pointer
			// use the same hypergraph, because graph id is unique 
//			tmp = (unsigned*)(new UD_hypergraph_data) ;
//			((UD_hypergraph_data*)tmp)->vertex_type_tag = ((UD_hypergraph_data*)(source.get_data()))->vertex_type_tag ;
//			((UD_hypergraph_data*)tmp)->edge_type_tag = ((UD_hypergraph_data*)(source.get_data()))->edge_type_tag ;
//			((UD_hypergraph_data*)tmp)->HG = ((UD_hypergraph_data*)(source.get_data()))->HG ;
//			break ;
		case T_user_defined: 
			tmp = source.get_data() ;
			break ;
		default: 
			cout<<"Not set up this type "<<source.get_type_tag()<<endl ;
			exit(-1) ;
	}
	return tmp ;
}

//#################################
// symbolic link 
//#################################
symbolic_link_data::symbolic_link_data(){
	this->table_pointer = NULL ;
	this->L1_flag = true; 
	this->Absolute_flag = true ; 
}

symbolic_link::symbolic_link(){
	set_type_tag(T_symbolic_link) ;
	this->data_ptr = (unsigned*)(new symbolic_link_data) ;
}

symbolic_link::~symbolic_link(){
	delete this->data_ptr ;
}

symbolic_link::symbolic_link(const symbolic_link& sym) {
	this->type_tag = ((symbolic_link*)(&sym))->get_type_tag() ;
	this->parent_ptr = ((symbolic_link*)(&sym))->get_parent_point() ;
	this->attribute_list = ((symbolic_link*)(&sym))->attribute_list ;
	if(  ((symbolic_link*)(&sym))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((symbolic_link*)(&sym))) ;
}

void symbolic_link::set_L1_key(universal_data key){
	((symbolic_link_data*)(this->data_ptr))->L1_key = key ;
}

universal_data symbolic_link::get_L1_key(){
	return ((symbolic_link_data*)(this->data_ptr))->L1_key ;
}

void symbolic_link::set_table_pointer(unsigned* tp){
	((symbolic_link_data*)(this->data_ptr))->table_pointer = tp ;
}

unsigned* symbolic_link::get_table_pointer(){
	return ((symbolic_link_data*)(this->data_ptr))->table_pointer ;
}

void symbolic_link::set_L1_name(universal_data nm){
	((symbolic_link_data*)(this->data_ptr))->L1_name = nm ;
}

universal_data symbolic_link::get_L1_name(){
	return ((symbolic_link_data*)(this->data_ptr))->L1_name ;
}

void symbolic_link::set_L1_flag(bool f) {
	((symbolic_link_data*)(this->data_ptr))->L1_flag = f ;
}

bool symbolic_link::get_L1_flag(){
	return ((symbolic_link_data*)(this->data_ptr))->L1_flag ;
}

void symbolic_link::set_Absolute_flag(bool f) {
	((symbolic_link_data*)(this->data_ptr))->Absolute_flag = f ;
}

bool symbolic_link::get_Absolute_flag() {
	return ((symbolic_link_data*)(this->data_ptr))->Absolute_flag ;
}

void symbolic_link::path_push_back(condition *c) {
	((symbolic_link_data*)(this->data_ptr))->path.push_back(c) ;
}

void symbolic_link::path_pop_back() {
	((symbolic_link_data*)(this->data_ptr))->path.pop_back() ;
}

universal_data* symbolic_link::linkto_target() {
	if( ((symbolic_link_data*)(this->data_ptr))->path.size() == 0) // empty
		return NULL ;

	list<condition*>::iterator iter ;
	map<universal_data, universal_data*>::iterator L1_iter ;
	map<universal_data, map<universal_data, universal_data*>*>::iterator L2_iter ;
	universal_data* tmp ;
	if( ((symbolic_link_data*)(this->data_ptr))->Absolute_flag ) // absolute path
	{
		// find the top level component 
		if( ((symbolic_link_data*)(this->data_ptr))->L1_flag) // L1 table
		{
			L1_iter = L1_table.find( ((symbolic_link_data*)(this->data_ptr))->L1_key) ;
			if( L1_iter == L1_table.end() ) // not found
			{
				cout<<"can't find top level component "<<endl ;
				return NULL ;
			}
			tmp = L1_iter->second ;
		}
		else // L2 table 
		{
			L2_iter = L2_table.find(((symbolic_link_data*)(this->data_ptr))->L1_name) ;
			if(L2_iter == L2_table.end() )
			{
				cout<<"can't find L1 table"<<endl ;
				return NULL ;
			}

			L1_iter = L2_iter->second->find( ((symbolic_link_data*)(this->data_ptr))->L1_key) ;
			if( L1_iter == L2_iter->second->end() ) // not found
			{
				cout<<"can't find top level component "<<endl ;
				return NULL ;
			}
			tmp = L1_iter->second ;
		}

		// use top level component to access 
		for( iter = ((symbolic_link_data*)(this->data_ptr))->path.begin() ; iter != ((symbolic_link_data*)(this->data_ptr))->path.end() ; iter++)
		{
			tmp = (*iter)->find_target(tmp) ;
			if(tmp == NULL){
				cout<<"path is error"<<endl ;
				return NULL ;
			}
		}
		if(tmp->get_type_tag() == T_symbolic_link) // cascade path 
			return ((symbolic_link*)tmp)->linkto_target() ;
		else
			return tmp ;
	}
	else // relative path
	{
		tmp = this ;
		 // use current compoennt to access 
		for( iter = ((symbolic_link_data*)(this->data_ptr))->path.begin() ; iter != ((symbolic_link_data*)(this->data_ptr))->path.end() ; iter++)
		{
			tmp = (*iter)->find_target(tmp) ;
			if(tmp == NULL){
				cout<<"path is error"<<endl ;
				return NULL ;
			}
		}
		if(tmp->get_type_tag() == T_symbolic_link)  // cascade path
			return ((symbolic_link*)tmp)->linkto_target() ;
		else
			return tmp ;
	}
}

//#################################
// primitive data 
//#################################
Int::Int(){
	set_type_tag(T_int) ;
	this->data_ptr = (unsigned*)(new int) ;
}

Int::~Int(){
	delete this->data_ptr ;
}

Int::Int(const Int& i) {
	this->type_tag = ((Int*)(&i))->get_type_tag() ;
	this->parent_ptr = ((Int*)(&i))->get_parent_point() ;
	this->attribute_list = ((Int*)(&i))->attribute_list ;
	if(  ((Int*)(&i))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Int*)(&i))) ;
}

void Int::set_value(int d){
	*((int*)(this->data_ptr)) = d;
}

int* Int::getptr(){
	return (int*)(this->data_ptr) ;
}

bool Int::operator ==(universal_data other_data){
	// check whether it's type tag is T_int
	if(other_data.get_type_tag() != T_int)
		return false ;
	else 
		return ( *((int*)(this->data_ptr)) == *( ((Int*)(&other_data))->getptr() ) ) ;
}

bool Int::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Int::operator <(const universal_data &rhs) const{
	// general version less function 
	if(((universal_data*)(&rhs))->get_type_tag() != T_int)
	{
		int priority1 = get_type_tag_priority(T_int) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else 
		return ( *((int*)(this->data_ptr)) < *( ((Int*)(&rhs))->getptr() ) ) ;
}

Char::Char(){
	set_type_tag(T_char) ;
	this->data_ptr = (unsigned*)(new char) ;
}

Char::~Char(){
	delete this->data_ptr ;
}

Char::Char(const Char& c) {
	this->type_tag = ((Char*)(&c))->get_type_tag() ;
	this->parent_ptr = ((Char*)(&c))->get_parent_point() ;
	this->attribute_list = ((Char*)(&c))->attribute_list ;
	if(  ((Char*)(&c))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Char*)(&c))) ;
}

void Char::set_value(char d){
	*((char*)(this->data_ptr)) = d ;
}

char* Char::getptr(){
	return (char*)(this->data_ptr);
}

bool Char::operator ==(universal_data other_data){
	// check whether it's type tag is T_char
	if(other_data.get_type_tag() != T_char)
		return false ;
	else 
		return ( *((char*)(this->data_ptr)) == *( ((Char*)(&other_data))->getptr() ) ) ;
}

bool Char::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Char::operator <(const universal_data &rhs) const{
	// general version less function 
	if(((universal_data*)(&rhs))->get_type_tag() != T_char)
	{
		int priority1 = get_type_tag_priority(T_char) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
		return ( *((char*)(this->data_ptr)) < *( ((Char*)(&rhs))->getptr() ) ) ;
}

Float::Float(){
	set_type_tag(T_float) ;
	this->data_ptr = (unsigned*)(new float) ;
}

Float::~Float(){
	delete this->data_ptr ;
}

Float::Float(const Float& f) {
	this->type_tag = ((Float*)(&f))->get_type_tag() ;
	this->parent_ptr = ((Float*)(&f))->get_parent_point() ;
	this->attribute_list = ((Float*)(&f))->attribute_list ;
	if(  ((Float*)(&f))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Float*)(&f))) ;
}

void Float::set_value(float d){
	*((float*)(this->data_ptr)) = d; 
}

float* Float::getptr(){
	return (float*)(this->data_ptr) ;
}

bool Float::operator ==(universal_data other_data){
	// check whether it's type tag is T_float
	if(other_data.get_type_tag() != T_float)
		return false ;
	else 
		return ( *((float*)(this->data_ptr)) == *( ((Float*)(&other_data))->getptr() ) ) ;
}

bool Float::operator !=(universal_data other_data){
	return ! ( (*this) == (other_data)) ;
}

bool Float::operator <(const universal_data &rhs) const{
	// general version less function 
	if(((universal_data*)(&rhs))->get_type_tag() != T_float)
	{
		int priority1 = get_type_tag_priority(T_float) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else 
		return ( *((float*)(this->data_ptr)) < *( ((Float*)(&rhs))->getptr() ) ) ;
}

String::String(){
	set_type_tag(T_string) ;
	this->data_ptr = (unsigned*)(new string) ;
}

String::~String(){
	delete this->data_ptr ;
}

String::String(const String& s) {
	this->type_tag = ((String*)(&s))->get_type_tag() ;
	this->parent_ptr = ((String*)(&s))->get_parent_point() ;
	this->attribute_list = ((String*)(&s))->attribute_list ;
	if(  ((String*)(&s))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((String*)(&s))) ;
}

void String::set_value(string d) {
	*((string*)(this->data_ptr)) = d; 
}

string* String::getptr(){
	return (string*)(this->data_ptr) ;
}

bool String::operator ==(universal_data other_data){
	// check whether it's type tag is T_string
	if(other_data.get_type_tag() != T_string)
		return false ;
	else 
		return ( *((string*)(this->data_ptr)) == *( ((String*)(&other_data))->getptr() ) ) ;
}

bool String::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool String::operator <(const universal_data &rhs) const{
	// check whether it's type tag is T_string
	if(((universal_data*)(&rhs))->get_type_tag() != T_string)
	{
		int priority1 = get_type_tag_priority(T_string) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
		return ( *((string*)(this->data_ptr)) < *( ((String*)(&rhs))->getptr() ) ) ;
}

Double::Double(){
	set_type_tag(T_double) ;
	this->data_ptr = (unsigned*)(new double) ;
}

Double::~Double(){
	delete this->data_ptr ;
}

Double::Double(const Double &d) {
	this->type_tag = ((Double*)(&d))->get_type_tag() ;
	this->parent_ptr = ((Double*)(&d))->get_parent_point() ;
	this->attribute_list = ((Double*)(&d))->attribute_list ;
	if(  ((Double*)(&d))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Double*)(&d))) ;
}

void Double::set_value(double d){
	*((double*)(this->data_ptr)) = d ;
}

double* Double::getptr(){
	return (double*)(this->data_ptr); 
}

bool Double::operator ==(universal_data other_data){
	// check whether it's type tag is T_double
	if(other_data.get_type_tag() != T_double)
		return false ;
	else 
		return ( *((double*)(this->data_ptr)) == *( ((Double*)(&other_data))->getptr() ) ) ;
}

bool Double::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Double::operator <(const universal_data &rhs) const{
	// general version less function 
	if(((universal_data*)(&rhs))->get_type_tag() != T_double)
	{
		int priority1 = get_type_tag_priority(T_double) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else 
		return ( *((double*)(this->data_ptr)) < *( ((Double*)(&rhs))->getptr() ) ) ;
}

Bool::Bool()
{
	set_type_tag(T_bool) ;
	this->data_ptr = (unsigned*)(new bool) ;
}

Bool::~Bool()
{
	delete this->data_ptr ;
}

Bool::Bool(const Bool& b)
{
	this->type_tag = ((Bool*)(&b))->get_type_tag() ;
	this->parent_ptr = ((Bool*)(&b))->get_parent_point() ;
	this->attribute_list = ((Bool*)(&b))->attribute_list ;
	if(  ((Bool*)(&b))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Bool*)(&b))) ;
}

void Bool::set_value(bool d) 
{
	*((bool*)(this->data_ptr)) = d ;
}

bool* Bool::getptr()
{
	return (bool*)(this->data_ptr); 
}

bool Bool::operator ==(universal_data other_data){
	// check whether it's type tag is T_bool
	if(other_data.get_type_tag() != T_bool)
		return true ; 
	else 
		return ( *((bool*)(this->data_ptr)) == *( ((Bool*)(&other_data))->getptr() ) ) ;
}

bool Bool::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Bool::operator <(const universal_data &rhs) const{
	// general version less function 
	if(((universal_data*)(&rhs))->get_type_tag() != T_bool)
	{
		int priority1 = get_type_tag_priority(T_bool) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
		return ( *((bool*)(this->data_ptr)) < *( ((Bool*)(&rhs))->getptr() ) ) ;
}

//#################################
// structure 
//#################################
Vector_data::Vector_data() {
	this->elem_type_tag = T_unlimited  ;
}

Vector::Vector(){
	set_type_tag(T_vector) ;
	this->data_ptr = (unsigned*)(new Vector_data) ;
}

Vector::~Vector(){
	delete this->data_ptr ;
}

Vector::Vector(const Vector& v) {
	this->type_tag = ((Vector*)(&v))->get_type_tag() ;
	this->parent_ptr = ((Vector*)(&v))->get_parent_point() ;
	this->attribute_list = ((Vector*)(&v))->attribute_list ;
	if(  ((Vector*)(&v))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Vector*)(&v))) ;
}

void Vector::set_elem_type_tag(Type_Tag tt) {
	((Vector_data*)(this->data_ptr))->elem_type_tag = tt ;
}

Type_Tag Vector::get_elem_type_tag(){
	return ((Vector_data*)(this->data_ptr))->elem_type_tag ;
}

void Vector::push_back(universal_data* d){
	// check elem type tag 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()))
	{
		cout<<"the type tag is not the same "<<endl ;
		exit(-1) ;
	}

	d->set_parent_point(this) ;
	((Vector_data*)(this->data_ptr))->vec.push_back(d) ;
}

void Vector::pop_back(){
	unsigned size = ((Vector_data*)(this->data_ptr))->vec.size() ;
	(((Vector_data*)(this->data_ptr))->vec)[size -1]->set_parent_point(NULL) ;
	((Vector_data*)(this->data_ptr))->vec.pop_back() ;
}

universal_data* Vector::operator [](unsigned int n){
	universal_data* tmp = (((Vector_data*)(this->data_ptr))->vec)[n] ;
	return tmp; 
}

unsigned int Vector::size(){
	return ((Vector_data*)(this->data_ptr))->vec.size() ;
}

void Vector::begin(){
	((Vector_data*)(this->data_ptr))->iter = ((Vector_data*)(this->data_ptr))->vec.begin() ;
}

bool Vector::end(){
	return (((Vector_data*)(this->data_ptr))->iter == ((Vector_data*)(this->data_ptr))->vec.end()) ;
}

void Vector::operator ++(int){
	((Vector_data*)(this->data_ptr))->iter++ ;
}

universal_data* Vector::operator *(){
	return *(((Vector_data*)(this->data_ptr))->iter) ;
}

bool Vector::operator ==(universal_data other_data){
	if (this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		if( this->size() != ((Vector*)(&other_data))->size())
			return false ;
		Virtual_Iterator* iter1 = create_iterator(this) ;
		Virtual_Iterator* iter2 = create_iterator(&other_data) ;
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

bool Vector::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Vector::operator <(const universal_data& rhs) const{
	if( ((universal_data*)(&rhs))->get_type_tag() != T_vector)
	{
		int priority1 = get_type_tag_priority(T_vector) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Virtual_Iterator* iter1 = create_iterator((universal_data*)this) ;
		Virtual_Iterator* iter2 = create_iterator( (universal_data*)(&rhs)) ;
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
Pair_data::Pair_data(){
	elem.first = NULL ;
	elem.second = NULL ;
	type_tag_first = T_unlimited ;
	type_tag_second = T_unlimited ;
}

Pair::Pair(){
	set_type_tag(T_pair) ;
	this->data_ptr = (unsigned*)(new Pair_data) ;
}

Pair::~Pair(){
	delete this->data_ptr ;
}

Pair::Pair(const Pair& p) {
	this->type_tag = ((Pair*)(&p))->get_type_tag() ;
	this->parent_ptr = ((Pair*)(&p))->get_parent_point() ;
	this->attribute_list = ((Pair*)(&p))->attribute_list ;
	if(  ((Pair*)(&p))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Pair*)(&p))) ;
}

void Pair::set_type_tag_first(Type_Tag tt) {
	((Pair_data*)(this->data_ptr))->type_tag_first = tt ;
}

Type_Tag Pair::get_type_tag_first() {
	return ((Pair_data*)(this->data_ptr))->type_tag_first ;
}

void Pair::set_type_tag_second(Type_Tag tt) {
	((Pair_data*)(this->data_ptr))->type_tag_second = tt ;
}

Type_Tag Pair::get_type_tag_second() {
	return ((Pair_data*)(this->data_ptr))->type_tag_second ;
}

void Pair::set_first(universal_data* ud){
	// check the type tag of first element 
	if ( (this->get_type_tag_first() != T_unlimited)&&(this->get_type_tag_first() != ud->get_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	if(((Pair_data*)(this->data_ptr))->elem.first == NULL)
	{
		ud->set_parent_point(this) ;
		((Pair_data*)(this->data_ptr))->elem.first = ud ;
	}
	else
	{
		((Pair_data*)(this->data_ptr))->elem.first->set_parent_point(NULL) ;
		ud->set_parent_point(this) ;
		((Pair_data*)(this->data_ptr))->elem.first = ud ;
	}
}

void Pair::set_second(universal_data* ud){
	// check the type tag of second element 
	if ( (this->get_type_tag_second() != T_unlimited)&&(this->get_type_tag_second() != ud->get_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	if(((Pair_data*)(this->data_ptr))->elem.second == NULL)
	{
		ud->set_parent_point(this) ;
		((Pair_data*)(this->data_ptr))->elem.second = ud ;
	}
	else
	{
		((Pair_data*)(this->data_ptr))->elem.second->set_parent_point(NULL) ;
		ud->set_parent_point(this) ;
		((Pair_data*)(this->data_ptr))->elem.second = ud ;
	}
}

universal_data* Pair::first(){
	return ((Pair_data*)(this->data_ptr))->elem.first ;
}

universal_data* Pair::second(){
	return ((Pair_data*)(this->data_ptr))->elem.second ;
}

bool Pair::operator ==(universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		Virtual_Iterator* iter1 = create_iterator(this) ;
		Virtual_Iterator* iter2 = create_iterator(&other_data) ;
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

bool Pair::operator !=( universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Pair::operator <(const universal_data& rhs) const {
	if( ((universal_data*)(&rhs))->get_type_tag() != T_pair)
	{
		int priority1 = get_type_tag_priority(T_pair) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Virtual_Iterator* iter1 = create_iterator((universal_data*)this) ;
		Virtual_Iterator* iter2 = create_iterator( (universal_data*)(&rhs)) ;
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
Map_data::Map_data(){
	type_tag_key = T_unlimited ;
	type_tag_value = T_unlimited ;
}

Map::Map(){
	set_type_tag(T_map) ;
	this->data_ptr = (unsigned*)(new Map_data) ;
}

Map::~Map(){
	delete this->data_ptr ;
}

Map::Map(const Map& m) {
	this->type_tag = ((Map*)(&m))->get_type_tag() ;
	this->parent_ptr = ((Map*)(&m))->get_parent_point() ;
	this->attribute_list = ((Map*)(&m))->attribute_list ;
	if(  ((Map*)(&m))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Map*)(&m))) ;
}

void Map::set_type_tag_key(Type_Tag tt) {
	((Map_data*)(this->data_ptr))->type_tag_key = tt ;
}

Type_Tag Map::get_type_tag_key() {
	return ((Map_data*)(this->data_ptr))->type_tag_key ;
}

void Map::set_type_tag_value(Type_Tag tt){
	((Map_data*)(this->data_ptr))->type_tag_value = tt ;
}

Type_Tag Map::get_type_tag_value(){
	return ((Map_data*)(this->data_ptr))->type_tag_value ;
}

void Map::insert(universal_data key, universal_data *val){
	// check the type tag of key and value (need the same type for key comparison)
	if ( ((this->get_type_tag_key() != T_unlimited))&&(key.get_type_tag() != this->get_type_tag_key()) ){
		cout<<"The type tag of key is not the same"<<endl ;
		exit(-1) ;
	}

	if ( (this->get_type_tag_value() != T_unlimited)&&(val->get_type_tag() != this->get_type_tag_value()) ){
		cout<<"The type tag of value is not the same"<<endl ;
		exit(-1) ;
	}

	val->set_parent_point(this) ;
	
	map<universal_data, universal_data*>::iterator miter ;

	miter = ((Map_data*)(this->data_ptr))->elem.find(key) ;
	if( miter == ((Map_data*)(this->data_ptr))->elem.end()) 
		((Map_data*)(this->data_ptr))->elem.insert(pair<universal_data, universal_data*>(key, val) ) ;
	else
		cout<<"insert fail: already exist" ;
}

universal_data* Map::find(universal_data key){
	// check the type tag of key (need the same type for key comparison)
	if ( ((this->get_type_tag_key() != T_unlimited))&&(key.get_type_tag() != this->get_type_tag_key()) ){
		cout<<"the type tag of key is not the same"<<endl ;
		exit(-1) ;
	}
	map<universal_data, universal_data*>::iterator miter ;

	miter = ((Map_data*)(this->data_ptr))->elem.find(key) ;
	if(miter != ((Map_data*)(this->data_ptr))->elem.end())
		return miter->second ;
	else 
		return NULL ;
}

void Map::erase(universal_data key){
	// check the type tag of key (need the same type for key comparison)
	if ( ((this->get_type_tag_key() != T_unlimited))&&(key.get_type_tag() != this->get_type_tag_key()) ){
		cout<<"the type tag of key is not the same"<<endl ;
		exit(-1) ;
	}

	((Map_data*)(this->data_ptr))->elem.erase(key) ;
}

unsigned int Map::size(){
	return ((Map_data*)(this->data_ptr))->elem.size() ;
}

void Map::begin(){
	((Map_data*)(this->data_ptr))->iter = ((Map_data*)(this->data_ptr))->elem.begin() ;
}

bool Map::end(){
	return (((Map_data*)(this->data_ptr))->iter == ((Map_data*)(this->data_ptr))->elem.end()) ;
}

void Map::operator ++(int){
	((Map_data*)(this->data_ptr))->iter++ ;
}

universal_data Map::key(){
	return ((Map_data*)(this->data_ptr))->iter->first ;
}

universal_data* Map::value(){
	return ((Map_data*)(this->data_ptr))->iter->second ;
}

bool Map::operator ==(universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		Map* tmp = (Map*)(&other_data) ;
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

bool Map::operator !=(universal_data other_data){
	return !( (*this)  == other_data) ;
}

bool Map::operator <(const universal_data &rhs) const{
	// general version less function 
	if( ((universal_data*)(&rhs))->get_type_tag() != T_map)
	{
		int priority1 = get_type_tag_priority(T_map) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Map* tmp = (Map*)(&rhs) ;
		// compare key 
		((Map*)this)->begin() ; 
		tmp->begin() ; 
		while( !((Map*)this)->end() && !tmp->end())
		{
			if(((Map*)this)->key()  == tmp->key())
			{
				if (((Map*)this)->value() == tmp->value())
				{
					(*((Map*)this))++ ;
					(*tmp)++ ;
				}
				else
					return ((Map*)this)->value() < tmp->value() ;
			}
			else 
				return ((Map*)this)->key() < tmp->key() ;
		}
		if(tmp->end())
			return false ;
		else 
			return true ;
	}
}

//
Set_data::Set_data(){
	elem_type_tag = T_unlimited ;
}

Set::Set(){
	set_type_tag(T_set) ;
	this->data_ptr = (unsigned*)(new Set_data) ;
}

Set::~Set(){
	delete this->data_ptr ;
}

Set::Set(const Set& s) {
	this->type_tag = ((Set*)(&s))->get_type_tag() ;
	this->parent_ptr = ((Set*)(&s))->get_parent_point() ;
	this->attribute_list = ((Set*)(&s))->attribute_list ;
	if(  ((Set*)(&s))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((Set*)(&s))) ;
}

void Set::set_elem_type_tag(Type_Tag tt) {
	((Set_data*)(this->data_ptr))->elem_type_tag = tt ;
}

Type_Tag Set::get_elem_type_tag() {
	return ((Set_data*)(this->data_ptr))->elem_type_tag ;
}

void Set::insert(universal_data *d){
	// check type tag of key 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	d->set_parent_point(this) ;
	((Set_data*)(this->data_ptr))->elem.insert(d) ;
}

void Set::erase(universal_data *d){
	// check type tag of key 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	set<universal_data*>::iterator siter ;
	siter = ((Set_data*)(this->data_ptr))->elem.find (d) ;
	if (siter != ((Set_data*)(this->data_ptr))->elem.end() )
		(*siter)->set_parent_point(NULL) ;
	((Set_data*)(this->data_ptr))->elem.erase(d) ;
}

unsigned int Set::size(){
	return ((Set_data*)(this->data_ptr))->elem.size() ;
}

void Set::begin(){
	((Set_data*)(this->data_ptr))->iter = ((Set_data*)(this->data_ptr))->elem.begin() ;
}

bool Set::end(){
	return (((Set_data*)(this->data_ptr))->iter ==((Set_data*)(this->data_ptr))->elem.end()) ;
}

void Set::operator ++(int){
	((Set_data*)(this->data_ptr))->iter++ ;
}

universal_data* Set::operator *(){
	return *(((Set_data*)(this->data_ptr))->iter) ;
}

bool Set::operator ==(universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		Virtual_Iterator* iter1 = create_iterator(this) ;
		Virtual_Iterator* iter2 = create_iterator(&other_data) ;
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

bool Set::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Set::operator <(const universal_data& rhs) const {
	if( ((universal_data*)(&rhs))->get_type_tag() != T_set)
	{
		int priority1 = get_type_tag_priority(T_set) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Virtual_Iterator* iter1 = create_iterator((universal_data*)this) ;
		Virtual_Iterator* iter2 = create_iterator( (universal_data*)(&rhs)) ;
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
List_data::List_data() {
	elem_type_tag = T_unlimited ;
}

List::List(){
	set_type_tag(T_list) ;
	this->data_ptr = (unsigned*)(new List_data) ;
}

List::~List(){
	delete this->data_ptr ;
}

List::List(const List& l) {
	this->type_tag = ((List*)(&l))->get_type_tag() ;
	this->parent_ptr = ((List*)(&l))->get_parent_point() ;
	this->attribute_list = ((List*)(&l))->attribute_list ;
	if(  ((List*)(&l))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
		this->data_ptr = copy_universal_data(*((List*)(&l))) ;
}

void List::set_elem_type_tag(Type_Tag tt) {
	((List_data*)(this->data_ptr))->elem_type_tag = tt ;
}

Type_Tag List::get_elem_type_tag() {
	return ((List_data*)(this->data_ptr))->elem_type_tag ;
}

void List::push_back(universal_data *data){
	//check the type tag of data 
	if ((this->get_elem_type_tag() != T_unlimited)&&(data->get_type_tag() != this->get_elem_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	data->set_parent_point(this) ;
	((List_data*)(this->data_ptr))->elem.push_back(data) ;
}

void List::pop_back(){
	if (((List_data*)(this->data_ptr))->elem.size() != 0){
		((List_data*)(this->data_ptr))->elem.back()->set_parent_point(NULL) ;
		((List_data*)(this->data_ptr))->elem.pop_back() ;
	}
}

unsigned int List::size(){
	return ((List_data*)(this->data_ptr))->elem.size() ;
}

void List::begin(){
	((List_data*)(this->data_ptr))->iter = ((List_data*)(this->data_ptr))->elem.begin() ;
}

bool List::end(){
	return (((List_data*)(this->data_ptr))->iter == ((List_data*)(this->data_ptr))->elem.end()) ;
}

void List::operator ++(int){
	((List_data*)(this->data_ptr))->iter++ ;
}

universal_data* List::operator *(){
	return *(((List_data*)(this->data_ptr))->iter);
}

void List::erase(){
	((List_data*)(this->data_ptr))->elem.erase(((List_data*)(this->data_ptr))->iter) ;
}

bool List::operator ==(universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		Virtual_Iterator* iter1 = create_iterator(this) ;
		Virtual_Iterator* iter2 = create_iterator(&other_data) ;
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

bool List::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool List::operator <(const universal_data& rhs) const {
	if( ((universal_data*)(&rhs))->get_type_tag() != T_list)
	{
		int priority1 = get_type_tag_priority(T_list) ;
		int priority2 = get_type_tag_priority(((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Virtual_Iterator* iter1 = create_iterator((universal_data*)this) ;
		Virtual_Iterator* iter2 = create_iterator( (universal_data*)(&rhs)) ;
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
Multimap_data::Multimap_data(){
	type_tag_key = T_unlimited ;
	type_tag_value = T_unlimited ;
}

Multimap::Multimap(){
	set_type_tag(T_multimap) ;
	this->data_ptr = (unsigned*)(new Multimap_data()) ;
}

Multimap::~Multimap(){
	delete data_ptr ;
}

Multimap::Multimap(const Multimap &m){
	this->type_tag = ((Multimap*)(&m))->get_type_tag() ;
	this->parent_ptr = ((Multimap*)(&m))->get_parent_point() ;
	this->attribute_list = ((Multimap*)(&m))->attribute_list ;
	if( ((Multimap*)(&m))->get_data() == NULL)
		this->data_ptr = NULL ;
	else 
		this->data_ptr = copy_universal_data( *((Multimap*)(&m))) ;
}

void Multimap::set_type_tag_key(Type_Tag tt){
	((Multimap_data*)(this->data_ptr))->type_tag_key = tt ;
}

void Multimap::set_type_tag_value(Type_Tag tt){
	((Multimap_data*)(this->data_ptr))->type_tag_value = tt ;
}

Type_Tag Multimap::get_type_tag_key(){
	return ((Multimap_data*)(this->data_ptr))->type_tag_key ;
}

Type_Tag Multimap::get_type_tag_value(){
	return ((Multimap_data*)(this->data_ptr))->type_tag_value ;
}

void Multimap::insert(universal_data key, universal_data *value){
	// check the type tag of key and value 
	if( ((this->get_type_tag_key() != T_unlimited)) && (key.get_type_tag())) {
		cout<<"The type tag of key is not the same"<<endl; 
		exit(-1) ;
	}

	if( (this->get_type_tag_value() != T_unlimited) && (value->get_type_tag())){
		cout<<"The type tag of value is not the same"<<endl ;
		exit(-1) ;
	}

	value->set_parent_point(this) ;
	((Multimap_data*)(this->data_ptr))->elem.insert(pair<universal_data, universal_data*>(key, value)) ;
}

unsigned int Multimap::size(){
	return ((Multimap_data*)(this->data_ptr))->elem.size() ;
}

bool Multimap::operator ==(universal_data other_data) {
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		Multimap* tmp = (Multimap*)(&other_data) ;
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

bool Multimap::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Multimap::operator <(const universal_data &rhs) const{
	// general version less function 
	if( ((universal_data*)(&rhs))->get_type_tag() != T_multimap)
	{
		int priority1 = get_type_tag_priority(T_multimap) ;
		int priority2 = get_type_tag_priority( ((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Multimap* tmp = (Multimap*)(&rhs) ;
		// compare key 
		((Multimap*)this)->begin() ; 
		tmp->begin() ; 
		while( !((Multimap*)this)->end() && !tmp->end())
		{
			if( ((Multimap*)this)->key() == tmp->key() )
			{
				if ( ((Multimap*)this)->value() == tmp->value())
				{
					(*(Multimap*)this)++ ;
					(*tmp)++ ;
				}
				else 
					return ((Multimap*)this)->value() < tmp->value() ;
			}
			else 
				return ((Multimap*)this)->key() < tmp->key() ;
		}
		if( tmp->end())
			return false ;
		else 
			return true ;
	}
}

void Multimap::begin(){
	((Multimap_data*)(this->data_ptr))->iter = ((Multimap_data*)(this->data_ptr))->elem.begin() ;
}

bool Multimap::end(){
	return ((Multimap_data*)(this->data_ptr))->iter == ((Multimap_data*)(this->data_ptr))->elem.end() ;
}

void Multimap::operator ++(int){
	((Multimap_data*)data_ptr)->iter++ ;
}

void Multimap::erase(){
	((Multimap_data*)data_ptr)->elem.erase( ((Multimap_data*)data_ptr)->iter) ;
}

universal_data Multimap::key(){
	return ((Multimap_data*)data_ptr)->iter->first ;
}

universal_data* Multimap::value(){
	return ((Multimap_data*)data_ptr)->iter->second ;
}

//#################################
// structure (extension)
//#################################
Attribute_List::Attribute_List(){
	set_type_tag(T_attribute_list) ;
	this->data_ptr = (unsigned*)(new Attribute_List_data()) ;
}

Attribute_List::~Attribute_List(){
	delete data_ptr ;
}

Attribute_List::Attribute_List(const Attribute_List &al){
	this->type_tag = ((Attribute_List*)(&al))->get_type_tag() ;
	this->parent_ptr = ((Attribute_List*)(&al))->get_parent_point() ;
	this->attribute_list = ((Attribute_List*)(&al))->attribute_list ;
	if( ((Attribute_List*)(&al))->get_data() == NULL)
		this->data_ptr = NULL ;
	else 
		this->data_ptr = copy_universal_data( *((Attribute_List*)(&al))) ;
}

void Attribute_List::add_attribute_al(std::string key, universal_data val){
	String k ;
	k.set_value(key) ;
    map<universal_data, universal_data>::iterator iter ;
    iter = ((Attribute_List_data*)data_ptr)->attributes.find(k) ; 
	if(iter == ((Attribute_List_data*)data_ptr)->attributes.end())
		((Attribute_List_data*)data_ptr)->attributes.insert(pair<universal_data, universal_data>(k, val) ) ;
	else
		cout<<"insert fail: already exist"<<endl ;
}

void Attribute_List::add_attribute_al(universal_data key, universal_data val){
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(key) ;
	if(iter == ((Attribute_List_data*)data_ptr)->attributes.end())
		((Attribute_List_data*)data_ptr)->attributes.insert(pair<universal_data, universal_data>(key, val) ) ;
	else 
		cout<<"insert fail: already exist"<<endl ;
}

universal_data Attribute_List::get_attribute_al(std::string key){
	universal_data empty ;
	String k ;
	k.set_value(key) ;
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(k) ;
	if(iter == ((Attribute_List_data*)data_ptr)->attributes.end())
		return empty ;
	else
		return iter->second ;
}

universal_data Attribute_List::get_attribute_al(universal_data key) {
	universal_data empty ;
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(key) ;
	if(iter == ((Attribute_List_data*)data_ptr)->attributes.end())
		return empty ;
	else
		return iter->second ;
}

universal_data* Attribute_List::get_attribute_ref_al(std::string key){
	String k ;
	k.set_value(key) ;
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(k) ;
    if(iter == ((Attribute_List_data*)data_ptr)->attributes.end())
        return NULL ;
	else
		return &(iter->second) ;
}

universal_data* Attribute_List::get_attribute_ref_al(universal_data key){
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(key) ;
	if(iter == ((Attribute_List_data*)data_ptr)->attributes.end() )
		return NULL ;
	else 
		return &(iter->second) ;
}

void Attribute_List::set_attribute_al(std::string key, universal_data val){
	String k ;
	k.set_value(key) ;
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(k) ;
	if (iter != ((Attribute_List_data*)data_ptr)->attributes.end())
		iter->second = val ;
}

void Attribute_List::set_attribute_al(universal_data key, universal_data val) {
	map<universal_data, universal_data>::iterator iter ;
	iter = ((Attribute_List_data*)data_ptr)->attributes.find(key) ;
	if (iter != ((Attribute_List_data*)data_ptr)->attributes.end())
		iter->second = val ;
}

void Attribute_List::remove_attribute_al(std::string key) {
	String k ;
	k.set_value(key) ;

	((Attribute_List_data*)data_ptr)->attributes.erase(k) ;
}

void Attribute_List::remove_attribute_al(universal_data key) {
	((Attribute_List_data*)data_ptr)->attributes.erase(key) ;
}

unsigned int Attribute_List::size(){
	return ((Attribute_List_data*)data_ptr)->attributes.size() ;
}

bool Attribute_List::operator ==(universal_data other_data){
	if( this->get_type_tag() != other_data.get_type_tag())
		return false ;
	else 
	{
		Attribute_List* tmp = (Attribute_List*)(&other_data) ;
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

bool Attribute_List::operator !=(universal_data other_data){
	return !( (*this) == other_data) ;
}

bool Attribute_List::operator <(const universal_data &rhs) const{
	// general version less function 
	if( ((universal_data*)(&rhs))->get_type_tag() != T_attribute_list)
	{
		int priority1 = get_type_tag_priority(T_attribute_list) ;
		int priority2 = get_type_tag_priority( ((universal_data*)(&rhs))->get_type_tag()) ;
		return priority1 < priority2 ;
	}
	else
	{
		Attribute_List* tmp = (Attribute_List*)(&rhs) ;
		// compare key 
		((Attribute_List*)this)->begin() ; 
		tmp->begin() ; 
		while( !((Attribute_List*)this)->end() && !tmp->end())
		{
			if( ((Attribute_List*)this)->get_attribute_name_al() == tmp->get_attribute_name_al() )
			{
				if(((Attribute_List*)this)->get_attribute_value_al() == tmp->get_attribute_value_al())
				{
					(*(Attribute_List*)this)++ ;
					(*tmp)++ ;
				}
				else 
					return ((Attribute_List*)this)->get_attribute_value_al() < tmp->get_attribute_value_al() ;
			}
			else 
				return ((Attribute_List*)this)->get_attribute_name_al() < tmp->get_attribute_name_al() ;
		}
		if(tmp->end())
			return false ;
		else 
			return true ;
	}
}

void Attribute_List::begin(){
	((Attribute_List_data*)data_ptr)->attrs_iter = ((Attribute_List_data*)data_ptr)->attributes.begin() ;
}

bool Attribute_List::end(){
	return ((Attribute_List_data*)data_ptr)->attrs_iter == ((Attribute_List_data*)data_ptr)->attributes.end() ;
}

void Attribute_List::operator ++(int){
	((Attribute_List_data*)data_ptr)->attrs_iter++ ;
}

universal_data Attribute_List::get_attribute_name_al(){
	return ((Attribute_List_data*)data_ptr)->attrs_iter->first ;
}

universal_data Attribute_List::get_attribute_value_al(){
	return ((Attribute_List_data*)data_ptr)->attrs_iter->second ;
}

//#################################
// parent pointer version  
//#################################
// parent class 
parent_class::parent_class(){
	this->parent_ptr = NULL ;
}

parent_class::~parent_class(){
}

void parent_class::set_parent_pointer(universal_data *ptr){
	this->parent_ptr = ptr ;
}

universal_data* parent_class::get_parent_pointer(){
	return this->parent_ptr ;
}

// p_Vector 
p_Vector_data::p_Vector_data(){
	this->elem_type_tag = T_unlimited ;
	this->set_parent_pointer(NULL) ;
}

p_Vector::p_Vector(){
	set_type_tag(T_p_vector) ;
	this->data_ptr = (unsigned*)(new p_Vector_data) ;
	((p_Vector_data*)(this->data_ptr))->set_parent_pointer(this) ;
}

p_Vector::~p_Vector(){
	delete this->data_ptr ;
}

p_Vector::p_Vector(const p_Vector &v){
	this->type_tag = ((p_Vector*)(&v))->get_type_tag() ;
	this->parent_ptr = ((p_Vector*)(&v))->get_parent_point() ;
	this->attribute_list = ((p_Vector*)(&v))->attribute_list ;
	if(  ((p_Vector*)(&v))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
	{
		this->data_ptr = copy_universal_data(*((p_Vector*)(&v))) ;
		((p_Vector_data*)(this->data_ptr))->set_parent_pointer(this) ;
	}
}

void p_Vector::set_elem_type_tag(Type_Tag tt) {
	((p_Vector_data*)(this->data_ptr))->elem_type_tag = tt ;
}

Type_Tag p_Vector::get_elem_type_tag(){
	return ((p_Vector_data*)(this->data_ptr))->elem_type_tag ;
}

void p_Vector::push_back(universal_data* d){
	// check elem type tag 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()))
	{
		cout<<"the type tag is not the same "<<endl ;
		exit(-1) ;
	}

	d->set_parent_point(this) ;
	((p_Vector_data*)(this->data_ptr))->vec.push_back(d) ;
}

void p_Vector::pop_back(){
	unsigned size = ((p_Vector_data*)(this->data_ptr))->vec.size() ;
	(((p_Vector_data*)(this->data_ptr))->vec)[size -1]->set_parent_point(NULL) ;
	((p_Vector_data*)(this->data_ptr))->vec.pop_back() ;
}

universal_data* p_Vector::operator [](unsigned int n){
	universal_data* tmp = (((p_Vector_data*)(this->data_ptr))->vec)[n] ;
	return tmp; 
}

unsigned int p_Vector::size(){
	return ((p_Vector_data*)(this->data_ptr))->vec.size() ;
}

void p_Vector::begin(){
	((p_Vector_data*)(this->data_ptr))->iter = ((p_Vector_data*)(this->data_ptr))->vec.begin() ;
}

bool p_Vector::end(){
	return (((p_Vector_data*)(this->data_ptr))->iter == ((p_Vector_data*)(this->data_ptr))->vec.end()) ;
}

void p_Vector::operator ++(int){
	((p_Vector_data*)(this->data_ptr))->iter++ ;
}

universal_data* p_Vector::operator *(){
	return *(((p_Vector_data*)(this->data_ptr))->iter) ;
}

//p_Pair
p_Pair_data::p_Pair_data(){
	elem.first = NULL ;
	elem.second = NULL ;
	type_tag_first = T_unlimited ;
	type_tag_second = T_unlimited ;
	this->set_parent_pointer(NULL) ;
}

p_Pair::p_Pair(){
	set_type_tag(T_p_pair) ;
	this->data_ptr = (unsigned*)(new p_Pair_data) ;
	((p_Pair_data*)(this->data_ptr))->set_parent_pointer(this) ;
}

p_Pair::~p_Pair(){
	delete this->data_ptr ;
}

p_Pair::p_Pair(const p_Pair& p) {
	this->type_tag = ((p_Pair*)(&p))->get_type_tag() ;
	this->parent_ptr = ((p_Pair*)(&p))->get_parent_point() ;
	this->attribute_list = ((p_Pair*)(&p))->attribute_list ;
	if(  ((p_Pair*)(&p))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
	{
		this->data_ptr = copy_universal_data(*((p_Pair*)(&p))) ;
		((p_Pair_data*)(this->data_ptr))->set_parent_pointer(this) ;
	}
}

void p_Pair::set_type_tag_first(Type_Tag tt) {
	((p_Pair_data*)(this->data_ptr))->type_tag_first = tt ;
}

Type_Tag p_Pair::get_type_tag_first() {
	return ((p_Pair_data*)(this->data_ptr))->type_tag_first ;
}

void p_Pair::set_type_tag_second(Type_Tag tt) {
	((p_Pair_data*)(this->data_ptr))->type_tag_second = tt ;
}

Type_Tag p_Pair::get_type_tag_second() {
	return ((p_Pair_data*)(this->data_ptr))->type_tag_second ;
}

void p_Pair::set_first(universal_data* ud){
	// check the type tag of first element 
	if ( (this->get_type_tag_first() != T_unlimited)&&(this->get_type_tag_first() != ud->get_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	if(((p_Pair_data*)(this->data_ptr))->elem.first == NULL)
	{
		ud->set_parent_point(this) ;
		((p_Pair_data*)(this->data_ptr))->elem.first = ud ;
	}
	else
	{
		((p_Pair_data*)(this->data_ptr))->elem.first->set_parent_point(NULL) ;
		ud->set_parent_point(this) ;
		((p_Pair_data*)(this->data_ptr))->elem.first = ud ;
	}
}

void p_Pair::set_second(universal_data* ud){
	// check the type tag of second element 
	if ( (this->get_type_tag_second() != T_unlimited)&&(this->get_type_tag_second() != ud->get_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	if(((p_Pair_data*)(this->data_ptr))->elem.second == NULL)
	{
		ud->set_parent_point(this) ;
		((p_Pair_data*)(this->data_ptr))->elem.second = ud ;
	}
	else
	{
		((p_Pair_data*)(this->data_ptr))->elem.second->set_parent_point(NULL) ;
		ud->set_parent_point(this) ;
		((p_Pair_data*)(this->data_ptr))->elem.second = ud ;
	}
}

universal_data* p_Pair::first(){
	return ((p_Pair_data*)(this->data_ptr))->elem.first ;
}

universal_data* p_Pair::second(){
	return ((p_Pair_data*)(this->data_ptr))->elem.second ;
}

//p_Map
p_Map_data::p_Map_data(){
	type_tag_key = T_unlimited ;
	type_tag_value = T_unlimited ;
	this->set_parent_pointer(NULL) ;
}

p_Map::p_Map(){
	set_type_tag(T_p_map) ;
	this->data_ptr = (unsigned*)(new p_Map_data) ;
	((p_Map_data*)(this->data_ptr))->set_parent_pointer(this) ;
}

p_Map::~p_Map(){
	delete this->data_ptr ;
}

p_Map::p_Map(const p_Map& m) {
	this->type_tag = ((p_Map*)(&m))->get_type_tag() ;
	this->parent_ptr = ((p_Map*)(&m))->get_parent_point() ;
	this->attribute_list = ((p_Map*)(&m))->attribute_list ;
	if(  ((p_Map*)(&m))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
	{
		this->data_ptr = copy_universal_data(*((p_Map*)(&m))) ;
		((p_Map_data*)(this->data_ptr))->set_parent_pointer(this) ;
	}
}

void p_Map::set_type_tag_key(Type_Tag tt) {
	((p_Map_data*)(this->data_ptr))->type_tag_key = tt ;
}

Type_Tag p_Map::get_type_tag_key() {
	return ((p_Map_data*)(this->data_ptr))->type_tag_key ;
}

void p_Map::set_type_tag_value(Type_Tag tt){
	((p_Map_data*)(this->data_ptr))->type_tag_value = tt ;
}

Type_Tag p_Map::get_type_tag_value(){
	return ((p_Map_data*)(this->data_ptr))->type_tag_value ;
}

void p_Map::insert(universal_data key, universal_data *val){
	// check the type tag of key and value (need the same type for key comparison)
	if ( (this->get_type_tag_key() != T_unlimited)&&(key.get_type_tag() != this->get_type_tag_key()) ){
		cout<<"The type tag of key is not the same"<<endl ;
		exit(-1) ;
	}

	if ( (this->get_type_tag_value() != T_unlimited)&&(val->get_type_tag() != this->get_type_tag_value()) ){
		cout<<"The type tag of value is not the same"<<endl ;
		exit(-1) ;
	}

	val->set_parent_point(this) ;
	((p_Map_data*)(this->data_ptr))->elem.insert(pair<universal_data, universal_data*>(key, val) ) ;
}

universal_data* p_Map::find(universal_data key){
	// check the type tag of key (need the same type for key comparison)
	if ( key.get_type_tag() != this->get_type_tag_key() ) {
		cout<<"the type tag of key is not the same"<<endl ;
		exit(-1) ;
	}

	map<universal_data, universal_data*>::iterator miter ;
	miter = ((p_Map_data*)(this->data_ptr))->elem.find(key) ;
	if(miter == ((p_Map_data*)(this->data_ptr))->elem.end())
		return NULL ; 
	else
		return miter->second ;
}

void p_Map::erase(universal_data key){
	// check the type tag of key (need the same type for key comparison)
	if (key.get_type_tag() != this->get_type_tag_key() ){
		cout<<"the type tag of key is not the same"<<endl ;
		exit(-1) ;
	}

	map<universal_data, universal_data*>::iterator miter ;
	miter = ((p_Map_data*)(this->data_ptr))->elem.find(key) ;
	if (miter != ((p_Map_data*)(this->data_ptr))->elem.end() )
		miter->second->set_parent_point(NULL) ;

	((p_Map_data*)(this->data_ptr))->elem.erase(key) ;
}

unsigned int p_Map::size(){
	return ((p_Map_data*)(this->data_ptr))->elem.size() ;
}

void p_Map::begin(){
	((p_Map_data*)(this->data_ptr))->iter = ((p_Map_data*)(this->data_ptr))->elem.begin() ;
}

bool p_Map::end(){
	return (((p_Map_data*)(this->data_ptr))->iter == ((p_Map_data*)(this->data_ptr))->elem.end()) ;
}

void p_Map::operator ++(int){
	((p_Map_data*)(this->data_ptr))->iter++ ;
}

universal_data p_Map::key(){
	return ((p_Map_data*)(this->data_ptr))->iter->first ;
}

universal_data* p_Map::value(){
	return ((p_Map_data*)(this->data_ptr))->iter->second ;
}

//p_Set 
p_Set_data::p_Set_data(){
	elem_type_tag = T_unlimited ;
	this->set_parent_pointer(NULL) ;
}

p_Set::p_Set(){
	set_type_tag(T_p_set) ;
	this->data_ptr = (unsigned*)(new p_Set_data) ;
	((p_Set_data*)(this->data_ptr) )->set_parent_pointer(this) ;
}

p_Set::~p_Set(){
	delete this->data_ptr ;
}

p_Set::p_Set(const p_Set& s) {
	this->type_tag = ((p_Set*)(&s))->get_type_tag() ;
	this->parent_ptr = ((p_Set*)(&s))->get_parent_point() ;
	this->attribute_list = ((p_Set*)(&s))->attribute_list ;
	if(  ((p_Set*)(&s))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
	{
		this->data_ptr = copy_universal_data(*((p_Set*)(&s))) ;
		((p_Set_data*)(this->data_ptr) )->set_parent_pointer(this) ;
	}
}

void p_Set::set_elem_type_tag(Type_Tag tt) {
	((p_Set_data*)(this->data_ptr))->elem_type_tag = tt ;
}

Type_Tag p_Set::get_elem_type_tag() {
	return ((p_Set_data*)(this->data_ptr))->elem_type_tag ;
}

void p_Set::insert(universal_data *d){
	// check type tag of key 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	d->set_parent_point(this) ;
	((p_Set_data*)(this->data_ptr))->elem.insert(d) ;
}

void p_Set::erase(universal_data *d){
	// check type tag of key 
	if ( (this->get_elem_type_tag() != T_unlimited)&&(d->get_type_tag() != this->get_elem_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	set<universal_data*>::iterator siter ;
	siter = ((p_Set_data*)(this->data_ptr))->elem.find (d) ;
	if (siter != ((p_Set_data*)(this->data_ptr))->elem.end() )
		(*siter)->set_parent_point(NULL) ;
	((p_Set_data*)(this->data_ptr))->elem.erase(d) ;
}

unsigned int p_Set::size(){
	return ((p_Set_data*)(this->data_ptr))->elem.size() ;
}

void p_Set::begin(){
	((p_Set_data*)(this->data_ptr))->iter = ((p_Set_data*)(this->data_ptr))->elem.begin() ;
}

bool p_Set::end(){
	return (((p_Set_data*)(this->data_ptr))->iter ==((p_Set_data*)(this->data_ptr))->elem.end()) ;
}

void p_Set::operator ++(int){
	((p_Set_data*)(this->data_ptr))->iter++ ;
}

universal_data* p_Set::operator *(){
	return *(((p_Set_data*)(this->data_ptr))->iter) ;
}

//p_List
p_List_data::p_List_data() {
	elem_type_tag = T_unlimited ;
	this->set_parent_pointer(NULL) ;
}

p_List::p_List(){
	set_type_tag(T_p_list) ;
	this->data_ptr = (unsigned*)(new p_List_data) ;
	((p_List_data*)(this->data_ptr))->set_parent_pointer(this) ;
}

p_List::~p_List(){
	delete this->data_ptr ;
}

p_List::p_List(const p_List& l) {
	this->type_tag = ((p_List*)(&l))->get_type_tag() ;
	this->parent_ptr = ((p_List*)(&l))->get_parent_point() ;
	this->attribute_list = ((p_List*)(&l))->attribute_list ;
	if(  ((p_List*)(&l))->get_data() == NULL)
		this->data_ptr = NULL ;
	else
	{
		this->data_ptr = copy_universal_data(*((p_List*)(&l))) ;
		((p_List_data*)(this->data_ptr))->set_parent_pointer(this) ;
	}
}

void p_List::set_elem_type_tag(Type_Tag tt) {
	((p_List_data*)(this->data_ptr))->elem_type_tag = tt ;
}

Type_Tag p_List::get_elem_type_tag() {
	return ((p_List_data*)(this->data_ptr))->elem_type_tag ;
}

void p_List::push_back(universal_data *data){
	//check the type tag of data 
	if ((this->get_elem_type_tag() != T_unlimited)&&(data->get_type_tag() != this->get_elem_type_tag()) ){
		cout<<"The type tag is not the same"<<endl ;
		exit(-1) ;
	}

	data->set_parent_point(this) ;
	((p_List_data*)(this->data_ptr))->elem.push_back(data) ;
}

void p_List::pop_back(){
	if (((p_List_data*)(this->data_ptr))->elem.size() != 0){
		((p_List_data*)(this->data_ptr))->elem.back()->set_parent_point(NULL) ;
		((p_List_data*)(this->data_ptr))->elem.pop_back() ;
	}
}

unsigned int p_List::size(){
	return ((p_List_data*)(this->data_ptr))->elem.size() ;
}

void p_List::begin(){
	((p_List_data*)(this->data_ptr))->iter = ((p_List_data*)(this->data_ptr))->elem.begin() ;
}

bool p_List::end(){
	return (((p_List_data*)(this->data_ptr))->iter == ((p_List_data*)(this->data_ptr))->elem.end()) ;
}

void p_List::operator ++(int){
	((p_List_data*)(this->data_ptr))->iter++ ;
}

universal_data* p_List::operator *(){
	return *(((p_List_data*)(this->data_ptr))->iter);
}

void p_List::erase(){
	((p_List_data*)(this->data_ptr))->elem.erase(((p_List_data*)(this->data_ptr))->iter) ;
}

//#################################
// Iterator 
//#################################
Vector_Iterator::Vector_Iterator(universal_data *org)
{
	// check whether it is vector 
	if ( org->get_type_tag() != T_vector )
	{
		cout<<"The organization is not vector"<<endl ;
		exit(-1) ;
	}
	this->organization = org ;
}

Vector_Iterator::~Vector_Iterator()
{
}

void Vector_Iterator::begin()
{
	iter = ((Vector_data*)(this->organization->get_data()))->vec.begin() ;
}

bool Vector_Iterator::end()
{
	return iter == ((Vector_data*)(this->organization->get_data()))->vec.end() ;
}

void Vector_Iterator::advance()
{
	iter++ ;
}

universal_data* Vector_Iterator::operator *()
{
	return *iter ;
}
//
Pair_Iterator::Pair_Iterator(universal_data *org)
{
	// check whether it is pair 
	if ( org->get_type_tag() != T_pair )
	{
		cout<<"The organization is not pair"<<endl ;
		exit(-1) ;
	}

	this->organization = org ;
	this->count = 0;
}

Pair_Iterator::~Pair_Iterator()
{
}

void Pair_Iterator::begin()
{
	this->count = 1 ;
}

bool Pair_Iterator::end()
{
	return this->count >2;
}

void Pair_Iterator::advance() 
{
	this->count++ ;
}

universal_data* Pair_Iterator::operator *()
{
	if(this->count == 1)
		return ((Pair_data*)(this->organization->get_data()))->elem.first ;
	else if(this->count == 2)
		return ((Pair_data*)(this->organization->get_data()))->elem.second ;
	else
		return NULL ;
}
//
List_Iterator::List_Iterator(universal_data *org)
{
	// check whether it is list
	if ( org->get_type_tag() != T_list )
	{
		cout<<"The organization is not list"<<endl ;
		exit(-1) ;
	}

	this->organization = org ;
}

List_Iterator::~List_Iterator()
{
}

void List_Iterator::begin()
{
	iter = ((List_data*)(this->organization->get_data()))->elem.begin() ;
}

bool List_Iterator::end()
{
	return iter == ((List_data*)(this->organization->get_data()))->elem.end() ;
}

void List_Iterator::advance()
{
	iter++ ;
}

universal_data* List_Iterator::operator *()
{
	return *iter ;
}
//
Set_Iterator::Set_Iterator(universal_data *org)
{
	// check whether it is set 
	if ( org->get_type_tag() != T_set )
	{
		cout<<"The organization is not set"<<endl ;
		exit(-1) ;
	}
	this->organization = org;
}

Set_Iterator::~Set_Iterator()
{
}

void Set_Iterator::begin()
{
	iter = ((Set_data*)(this->organization->get_data()))->elem.begin() ;
}

bool Set_Iterator::end()
{
	return iter == ((Set_data*)(this->organization->get_data()))->elem.end() ;
}

void Set_Iterator::advance()
{
	iter++ ;
}

universal_data* Set_Iterator::operator *()
{
	return *iter ;
}
//
Map_Iterator::Map_Iterator(universal_data *org)
{
	// check whether it is map
	if ( org->get_type_tag() != T_map )
	{
		cout<<"The organization is not map"<<endl ;
		exit(-1) ;
	}

	this->organization = org ;
}

Map_Iterator::~Map_Iterator()
{
}

void Map_Iterator::begin()
{
	iter = ((Map_data*)(this->organization->get_data()))->elem.begin() ;
}

bool Map_Iterator::end()
{
	return iter == ((Map_data*)(this->organization->get_data()))->elem.end() ;
}

void Map_Iterator::advance()
{
	iter++ ;
}

universal_data* Map_Iterator::operator *()
{
	return iter->second ;
}

//
Multimap_Iterator::Multimap_Iterator(universal_data *org){
	// check whether it is map
	if ( org->get_type_tag() != T_multimap )
	{
		cout<<"The organization is not multimap"<<endl ;
		exit(-1) ;
	}
	this->organization = org ;
}

Multimap_Iterator::~Multimap_Iterator(){
}

void Multimap_Iterator::begin(){
	iter = ((Multimap_data*)(this->organization->get_data()))->elem.begin() ;
}

bool Multimap_Iterator::end(){
	return iter == ((Multimap_data*)(this->organization->get_data()))->elem.end() ;
}

void Multimap_Iterator::advance(){
	iter++ ;
}

universal_data* Multimap_Iterator::operator *(){
	return iter->second ;
}

// extend at 20140718
Attribute_List_Iterator::Attribute_List_Iterator(universal_data *org){
	if(org->get_type_tag() != T_attribute_list)
	{
		cout<<"The organization is not attribute_list"<<endl ;
		exit(-1) ;
	}
	this->organization = org ;
}

Attribute_List_Iterator::~Attribute_List_Iterator(){
}

void Attribute_List_Iterator::begin(){
	iter = ((Attribute_List_data*)(this->organization->get_data()))->attributes.begin() ;
}

bool Attribute_List_Iterator::end(){
	return iter == ((Attribute_List_data*)(this->organization->get_data()))->attributes.end() ;
}

void Attribute_List_Iterator::advance(){
	iter++ ;
}

universal_data* Attribute_List_Iterator::operator *(){
	return &(iter->second) ;
}

//
p_Vector_Iterator::p_Vector_Iterator(universal_data *org)
{
	// check whether it is p_vector 
	if ( org->get_type_tag() != T_p_vector )
	{
		cout<<"The organization is not p_vector"<<endl ;
		exit(-1) ;
	}
	this->organization = org ;
}

p_Vector_Iterator::~p_Vector_Iterator()
{
}

void p_Vector_Iterator::begin()
{
	iter = ((p_Vector_data*)(this->organization->get_data()))->vec.begin() ;
}

bool p_Vector_Iterator::end()
{
	return iter == ((p_Vector_data*)(this->organization->get_data()))->vec.end() ;
}

void p_Vector_Iterator::advance()
{
	iter++ ;
}

universal_data* p_Vector_Iterator::operator *()
{
	return *iter ;
}
//
p_Pair_Iterator::p_Pair_Iterator(universal_data *org)
{
	// check whether it is pair 
	if (org->get_type_tag() != T_p_pair )
	{
		cout<<"The organization is not p_pair"<<endl ;
		exit(-1) ;
	}

	this->organization = org ;
	this->count = 0;
}

p_Pair_Iterator::~p_Pair_Iterator()
{
}

void p_Pair_Iterator::begin()
{
	this->count = 1 ;
}

bool p_Pair_Iterator::end()
{
	return this->count >2;
}

void p_Pair_Iterator::advance() 
{
	this->count++ ;
}

universal_data* p_Pair_Iterator::operator *()
{
	if(this->count == 1)
		return ((p_Pair_data*)(this->organization->get_data()))->elem.first ;
	else if(this->count == 2)
		return ((p_Pair_data*)(this->organization->get_data()))->elem.second ;
	else
		return NULL ;
}
//
p_List_Iterator::p_List_Iterator(universal_data *org)
{
	// check whether it is list
	if ( org->get_type_tag() != T_p_list )
	{
		cout<<"The organization is not p_list"<<endl ;
		exit(-1) ;
	}

	this->organization = org ;
}

p_List_Iterator::~p_List_Iterator()
{
}

void p_List_Iterator::begin()
{
	iter = ((p_List_data*)(this->organization->get_data()))->elem.begin() ;
}

bool p_List_Iterator::end()
{
	return iter == ((p_List_data*)(this->organization->get_data()))->elem.end() ;
}

void p_List_Iterator::advance()
{
	iter++ ;
}

universal_data* p_List_Iterator::operator *()
{
	return *iter ;
}
//
p_Set_Iterator::p_Set_Iterator(universal_data *org)
{
	// check whether it is set 
	if ( org->get_type_tag() != T_p_set )
	{
		cout<<"The organization is not p_set"<<endl ;
		exit(-1) ;
	}
	this->organization = org;
}

p_Set_Iterator::~p_Set_Iterator()
{
}

void p_Set_Iterator::begin()
{
	iter = ((p_Set_data*)(this->organization->get_data()))->elem.begin() ;
}

bool p_Set_Iterator::end()
{
	return iter == ((p_Set_data*)(this->organization->get_data()))->elem.end() ;
}

void p_Set_Iterator::advance()
{
	iter++ ;
}

universal_data* p_Set_Iterator::operator *()
{
	return *iter ;
}
//
p_Map_Iterator::p_Map_Iterator(universal_data *org)
{
	// check whether it is map
	if ( org->get_type_tag() != T_p_map )
	{
		cout<<"The organization is not p_map"<<endl ;
		exit(-1) ;
	}

	this->organization = org ;
}

p_Map_Iterator::~p_Map_Iterator()
{
}

void p_Map_Iterator::begin()
{
	iter = ((p_Map_data*)(this->organization->get_data()))->elem.begin() ;
}

bool p_Map_Iterator::end()
{
	return iter == ((p_Map_data*)(this->organization->get_data()))->elem.end() ;
}

void p_Map_Iterator::advance()
{
	iter++ ;
}

universal_data* p_Map_Iterator::operator *()
{
	return iter->second ;
}
