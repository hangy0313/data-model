#include "universal_data_extend.h"

pointer_list::pointer_list(){
	set_type_tag(T_user_defined) ;
	set_user_defined_type_tag("pointer_list") ;
	data_ptr = (unsigned*)(new plist_data()) ;
}

pointer_list::~pointer_list(){
}

void pointer_list::add(universal_data* ptr){
	list<universal_data*>::iterator iter ;
	for(iter = ((plist_data*)data_ptr)->plist.begin() ; iter != ((plist_data*)data_ptr)->plist.end() ; iter++){
		if(*iter == ptr){
			cout<<"already exist this pointer"<<endl ;
			return ;
		}
	}
	((plist_data*)data_ptr)->plist.push_back(ptr) ;
}

void pointer_list::remove(universal_data* ptr) {
	list<universal_data*>::iterator iter ;
	for(iter = ((plist_data*)data_ptr)->plist.begin() ; iter != ((plist_data*)data_ptr)->plist.end() ; iter++){
		if( *iter == ptr) {
			((plist_data*)data_ptr)->plist.erase(iter) ;
			return ;
		}
	}
}

void pointer_list::begin(){
	((plist_data*)data_ptr)->plist_iter = ((plist_data*)data_ptr)->plist.begin() ;
}

bool pointer_list::end(){
	return ((plist_data*)data_ptr)->plist_iter == ((plist_data*)data_ptr)->plist.end() ;
}

void pointer_list::operator ++(int){
	((plist_data*)data_ptr)->plist_iter++ ;
}

universal_data* pointer_list::operator *(){
	return *( ((plist_data*)data_ptr)->plist_iter) ;
}

///
Pointer_List_Iterator::Pointer_List_Iterator(universal_data *org){
	// check type tag 
	if( (org->get_type_tag() != T_user_defined ) || (org->get_user_defined_type_tag() != "pointer_list")){
		cout<<"Type tag is not valid: "<<org->get_type_tag()<<" : "<<org->get_user_defined_type_tag() <<endl ;
		exit(-1) ;
	}
	organization = org ;
}

Pointer_List_Iterator::~Pointer_List_Iterator(){
}

void Pointer_List_Iterator::begin(){
	iter = ((plist_data*)(organization->get_data()))->plist.begin() ;
}

bool Pointer_List_Iterator::end(){
	return iter == ((plist_data*)(organization->get_data()))->plist.end() ;
}

void Pointer_List_Iterator::advance(){
	iter++ ;
}

universal_data* Pointer_List_Iterator::operator *(){
	return *iter ;
}

///
associate_list::associate_list(){
	set_type_tag(T_user_defined) ;
	set_user_defined_type_tag("associate_list") ;
	data_ptr = (unsigned*)(new alist_data()) ;
}

associate_list::~associate_list(){
}

void associate_list::add(string nm, universal_data ud) {
	list<pair<string, universal_data> >::iterator iter ;
	for(iter = ((alist_data*)data_ptr)->alist.begin() ; iter != ((alist_data*)data_ptr)->alist.end() ; iter++){
		if(iter->first == nm){
			cout<<"the element is existed"<<endl ;
			exit(-1) ;
		}
	}
	((alist_data*)data_ptr)->alist.push_back(pair<string, universal_data>(nm, ud) ) ;
}

void associate_list::set(string nm, universal_data ud) {
		list<pair<string, universal_data> >::iterator iter ;
	for(iter = ((alist_data*)data_ptr)->alist.begin() ; iter != ((alist_data*)data_ptr)->alist.end() ; iter++){
		if(iter->first == nm){
			iter->second = ud ;
			return ;
		}
	}

}

void associate_list::remove(string nm) {
	list<pair<string, universal_data> >::iterator iter ;
	for(iter = ((alist_data*)data_ptr)->alist.begin() ; iter != ((alist_data*)data_ptr)->alist.end() ; iter++){
		if(iter->first == nm){
			((alist_data*)data_ptr)->alist.erase(iter) ;
			return ;
		}
	}
}

void associate_list::begin() {
	((alist_data*)data_ptr)->alist_iter = ((alist_data*)data_ptr)->alist.begin() ;
}

bool associate_list::end(){
	return ((alist_data*)data_ptr)->alist_iter == ((alist_data*)data_ptr)->alist.end() ;
}

void associate_list::operator ++(int){
	((alist_data*)data_ptr)->alist_iter++ ;
}

string associate_list::get_name(){
	return ((alist_data*)data_ptr)->alist_iter->first ;
}

universal_data associate_list::get_value(){
	return ((alist_data*)data_ptr)->alist_iter->second ;
}

////

Associate_List_Iterator::Associate_List_Iterator(universal_data *org){
	// check type tag 
	if( (org->get_type_tag() != T_user_defined) || (org->get_user_defined_type_tag() != "associate_list")){
		cout<<"Type tag is not valid: "<<org->get_type_tag()<<" : "<<org->get_user_defined_type_tag() <<endl ;
		exit(-1) ;
	}
	organization = org ;
}

Associate_List_Iterator::~Associate_List_Iterator(){
}

void Associate_List_Iterator::begin() {
	iter = ((alist_data*)(organization->get_data()))->alist.begin() ;
}

bool Associate_List_Iterator::end(){
	return iter == ((alist_data*)(organization->get_data()))->alist.end() ;
}

void Associate_List_Iterator::advance(){
	iter++ ;
}

universal_data* Associate_List_Iterator::operator *(){
	return &(iter->second) ;
}

string Associate_List_Iterator::first(){
	return iter->first ;
}

universal_data Associate_List_Iterator::second(){
	return iter->second; 
}
//

pair_string_list::pair_string_list(){
	set_type_tag(T_user_defined) ;
	set_user_defined_type_tag("pair_string_list") ;
	data_ptr = (unsigned*)(new pslist_data()) ;
}

pair_string_list::~pair_string_list(){
}

void pair_string_list::push(std::string first, std::string second){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	tmp->pslist.push_back(pair<string, string>(first, second)) ;
}

void pair_string_list::pop(){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	tmp->pslist.pop_back() ;
}

void pair_string_list::begin(){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	tmp->pslist_iter = tmp->pslist.begin() ;
}

bool pair_string_list::end(){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	return tmp->pslist_iter == tmp->pslist.end() ;
}

void pair_string_list::operator ++(int){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	tmp->pslist_iter++ ;
}

string pair_string_list::first(){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	return tmp->pslist_iter->first ;
}

string pair_string_list::second(){
	pslist_data* tmp = (pslist_data*)(this->data_ptr) ;
	return tmp->pslist_iter->second ;
}