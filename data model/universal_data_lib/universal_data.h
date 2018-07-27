/* --------------------------------------------------------------- */
/* (C) Copyright 2018	                                           */
/* Owner: Tsung Lee, Yao-Yi Huang							   	   */
/* All Rights Reserved.                                            */
/*																   */
/* universal data:												   */
/*	primitive  type: int, string, char, float, double, bool		   */
/*	structure type: pair, map, vector, set, list, multimap,		   */
/*											attribute list		   */
/*	iterator of structure type universal data					   */
/* --------------------------------------------------------------- */
#ifndef UNIVERSAL_DATA_H
#define UNIVERSAL_DATA_H

#include <string> 
#include <vector> 
#include <list> 
#include <set>
#include <map>
#include <iostream> 
#include <stdlib.h>				// exit 
#include "UD_predeclaration.h"

using namespace std ;

//###########################################
// define enumeration count of type tag
//###########################################
enum UD_Type_Tag { T_unknown, T_unlimited, T_symbolic_link, 
					T_int, T_string, T_char, T_float, T_double, T_bool,
					T_pair, T_map, T_vector, T_set, T_list, T_multimap, T_attribute_list,
					T_user_defined};

//###########################################
// free function 
//###########################################
// return the priority of type tag ; it is used in the less function  
int UD_get_type_tag_priority(UD_Type_Tag tt) ;

// return the iterator of universal data 
UD_Virtual_Iterator* UD_create_iterator(UD_universal_data* org, int aspect_id = 0) ;

// output message 
void UD_print_message(string msg) ;

// output message and exit program 
void UD_print_error_message(string msg) ; 

//###########################################
//abstract class 
//###########################################
class UD_universal_data
{
public: 
	UD_universal_data() ;
	~UD_universal_data() ;
	UD_universal_data(const UD_universal_data& ud) ;	// copy constructor 

	// access function of data member 
	void set_type_tag(UD_Type_Tag tt) ;
	UD_Type_Tag get_type_tag() ;
	void set_user_defined_type_tag(string tag) ;
	string get_user_defined_type_tag() ;
	void set_data(unsigned* d) ;									// note: user maintain the type of data 
	unsigned* get_data() ;

	void set_parent_pointer(UD_universal_data* ptr, int aspect_id = 0) ;
	void remove_parent_pointer(UD_universal_data* ptr, int aspect_id = 0) ;
	list<UD_universal_data*> find_parent_pointer(int aspect_id = 0) ;
	map<int, list<UD_universal_data*> > get_parent_pointer() ;
	list<int> get_joined_aspect() ;

	bool add_attribute(string nm, UD_universal_data ud) ;
	void set_attribute(string nm, UD_universal_data ud) ;
	void remove_attribute(string nm) ;
	UD_universal_data find_attribute(string nm) ;
	UD_universal_data* find_attribute_ref(string nm) ;
	// operation of iterator of attribute list 
	void attributes_begin() ; 
	bool attributes_end() ;
	void attributes_next() ;
	string get_attribute_name() ;									// return the name of iterated attribute 
	UD_universal_data get_attribute() ;								// return the value of iterated attribute 

	// operator 
	UD_universal_data& operator = (UD_universal_data other_data) ;	// deep copy for each kind of universal data 
	bool operator == (UD_universal_data other_data) ;
	bool operator != (UD_universal_data other_data) ;
	bool operator < (const UD_universal_data& rhs) const ;			// less function 

	// copy function 
	unsigned* copy_universal_data(UD_universal_data& source) ;

protected: 
	UD_Type_Tag type_tag ;											// the type of structure 
	string user_defined_type_tag ;									// this data will be used when type tag is user-defined 
	unsigned* data_ptr ;											// aggregate relational data of each kind of universal data 

	map<int, list<UD_universal_data*> > parent_ptr ;				// point to the parent object (aspect id, parent pointer list)

	list<pair<string, UD_universal_data> > attribute_list ;	// some attributes which specify the character of current universal data object 
	list<pair<string, UD_universal_data> >::iterator attr_iter ;
} ;

//###########################################
// symbolic link 
//###########################################
// lookup table 
// level 2: key to level 1 table's address			UD_L2_table 
// level 1: key to component's address				UD_L1_table

class UD_sym_condition
{// use to find the next component via condition 
public :
	UD_sym_condition() {}
	~UD_sym_condition() {}

	// give a unviersal data, find the universal data that satisfy the condition and it is the child component of given universal data 
	virtual UD_universal_data* find_target(UD_universal_data* current) = 0 ; 
protected: 
} ;

struct UD_symbolic_link_data
{
	UD_symbolic_link_data() ; // constructor 
	UD_universal_data L1_key ;// used to find top level component 
	unsigned* table_pointer;
	UD_universal_data L1_name ; // used to find L1 table 
	bool L1_flag;// true means table pointer point to a level 1 table; otherwise, table pointer point to a level 2 table 
	bool Absolute_flag ; // true means this is absolute path, else it is a relative path 
	list<UD_sym_condition*> path ;
} ;

class UD_symbolic_link: public UD_universal_data
{
public: 
	UD_symbolic_link() ;
	~UD_symbolic_link() ;

	// copy constructor 
	UD_symbolic_link(const UD_symbolic_link& sym) ;

	// following three parameter are recorded in attribute list 
	// key
	void set_L1_key(UD_universal_data key) ;
	UD_universal_data get_L1_key() ;
	// table pointer
	void set_table_pointer(unsigned* tp) ;
	unsigned* get_table_pointer() ;
	// L1 name (level 1 table name)
	void set_L1_name(UD_universal_data nm) ;
	UD_universal_data get_L1_name() ;
	// L1 flag 
	void set_L1_flag(bool f) ;
	bool get_L1_flag() ;
	// absolute flag 
	void set_Absolute_flag(bool f) ;
	bool get_Absolute_flag() ;
	// path 
	void path_push_back(UD_sym_condition* c) ;
	void path_pop_back() ;

	// link to target
	UD_universal_data* linkto_target() ;

protected: 
} ;

//###########################################
// primitive type 
//###########################################
class UD_Int: public UD_universal_data
{
public: 
	UD_Int();
	~UD_Int();

	// copy constructor 
	UD_Int(const UD_Int& i) ;

	void set_value(int d) ;
	int* getptr();

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class UD_Char: public UD_universal_data
{
public: 
	UD_Char();
	~UD_Char() ;

	// copy constructor 
	UD_Char(const UD_Char& c) ;

	void set_value(char d) ;
	char* getptr();

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class UD_Float: public UD_universal_data
{
public: 
	UD_Float();
	~UD_Float();

	// copy constructor 
	UD_Float(const UD_Float& f) ;

	void set_value(float d);
	float* getptr();

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class UD_String: public UD_universal_data
{
public: 
	UD_String();
	~UD_String() ;

	// copy constructor 
	UD_String(const UD_String& s) ;

	void set_value(string d) ;
	string* getptr();

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class UD_Double: public UD_universal_data
{
public: 
	UD_Double();
	~UD_Double() ;

	// copy constructor 
	UD_Double(const UD_Double& d) ;

	void set_value(double d);
	double* getptr();

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class UD_Bool: public UD_universal_data
{
public: 
	UD_Bool();
	~UD_Bool() ;

	// copy constructor 
	UD_Bool(const UD_Bool& d) ;

	void set_value(bool d);
	bool* getptr();

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

//###########################################
// structure type  
//###########################################
struct UD_Vector_data 
{
	UD_Vector_data() ;// constructor 
	vector<UD_universal_data*>::iterator iter ;
	map<int, vector<UD_universal_data*> > vec ;	// (aspect id, container) 
	UD_Type_Tag elem_type_tag ; // used to limit the type of element
	UD_universal_data* owner ; // point to the owner (UD_Vector)
} ;

class UD_Vector: public UD_universal_data
{
public: 
	UD_Vector();
	~UD_Vector() ;

	// copy constructor 
	UD_Vector(const UD_Vector& v) ;

	void set_elem_type_tag(UD_Type_Tag tt) ;
	UD_Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void push_back(UD_universal_data* d, int aspect_id = 0);
	void pop_back(int aspect_id = 0) ;
	UD_universal_data* operator[] (unsigned int n) ;	// for default aspect 
	UD_universal_data* get_structure_data(unsigned int n, int aspect_id = 0) ; // general access function 
	unsigned int size(int aspect_id = 0) ;								// vector size 
	list<int> get_aspect_in_structure() ;

	// iterator
	void begin(int aspect_id = 0) ;
	bool end(int aspect_id = 0) ;
	void operator ++(int) ;
	UD_universal_data* operator*() ;
	
	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct UD_Pair_data
{
	UD_Pair_data() ; // constructor 
	map< int, pair< UD_universal_data*, UD_universal_data*> > elem;	// (aspect id, container)  
	UD_Type_Tag type_tag_first ; // used to limit the type of first element of pair 
	UD_Type_Tag type_tag_second ;// used to limit the type of second element of pair
	UD_universal_data* owner ; // point to owner (UD_Pair) 
} ;

class UD_Pair: public UD_universal_data
{
public: 
	UD_Pair();
	~UD_Pair() ;

	// copy constructor 
	UD_Pair(const UD_Pair& p) ;

	void set_type_tag_first(UD_Type_Tag tt) ;
	UD_Type_Tag get_type_tag_first() ;
	void set_type_tag_second(UD_Type_Tag tt) ;
	UD_Type_Tag get_type_tag_second() ;
	// insert, delete, get
	void set_first(UD_universal_data* ud, int aspect_id = 0) ;
	void set_second(UD_universal_data* ud, int aspect_id = 0) ;
	list<int> get_aspect_in_structure() ;

	UD_universal_data* first(int aspect_id = 0) ;
	UD_universal_data* second(int aspect_id = 0) ;

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct UD_Map_data
{
	UD_Map_data() ;
	map<UD_universal_data, UD_universal_data*>::iterator iter ;
	map<int, map<UD_universal_data, UD_universal_data*> > elem ; // (aspect_id, container)
	UD_Type_Tag type_tag_key ; // used to limit the type of key 
	UD_Type_Tag type_tag_value ; // used to limit the type of value 
	UD_universal_data* owner ; // point to the owner (UD_Map)
} ;

class UD_Map: public UD_universal_data
{
public: 
	UD_Map();
	~UD_Map() ;

	// copy constructor 
	UD_Map(const UD_Map& m) ;

	void set_type_tag_key(UD_Type_Tag tt) ;
	UD_Type_Tag get_type_tag_key() ;
	void set_type_tag_value(UD_Type_Tag tt) ;
	UD_Type_Tag get_type_tag_value() ;
	// insert, delete, get
	void insert(UD_universal_data key, UD_universal_data* val, int aspect_id = 0) ;
	UD_universal_data* find(UD_universal_data key, int aspect_id = 0) ; // find the value that key map to 
	void erase( UD_universal_data key, int aspect_id = 0) ;
	unsigned int size(int aspect_id = 0) ;
	list<int> get_aspect_in_structrue() ;

	// inner iterator 
	void begin(int aspect_id = 0);
	bool end(int aspect_id = 0) ;
	void operator++(int) ;
	UD_universal_data key() ; // get the key that iterator iterate 
	UD_universal_data* value() ; // get the value that iterator iterate

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct UD_Set_data
{
	UD_Set_data() ;
	set<UD_universal_data*>::iterator iter ;
	map<int, set<UD_universal_data*> > elem ;  // (aspect id, container)
	UD_Type_Tag elem_type_tag ; // used to limit the type of element 
	UD_universal_data* owner ; // point to the owner (UD_Set) 
} ;

class UD_Set: public UD_universal_data
{
public: 
	UD_Set();
	~UD_Set() ;

	// copy constructor 
	UD_Set(const UD_Set& s) ;

	void set_elem_type_tag(UD_Type_Tag tt) ;
	UD_Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void insert(UD_universal_data* d, int aspect_id = 0) ;
	void erase(UD_universal_data* d, int aspect_id = 0) ;
	unsigned int size(int aspect_id = 0) ;
	list<int> get_aspect_in_structure() ;

	//iterator 
	void begin(int aspect_id = 0) ;
	bool end(int aspect_id = 0) ; 
	void operator++(int) ;
	UD_universal_data* operator*() ;

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct UD_List_data
{
	UD_List_data() ;
	list<UD_universal_data*>::iterator iter ;
	map<int, list<UD_universal_data*> > elem ; // (aspect id, container)
	UD_Type_Tag elem_type_tag ; // used to limit the type of element 
	UD_universal_data* owner ; // point to owner (UD_List)
} ;

class UD_List: public UD_universal_data
{
public: 
	UD_List();
	~UD_List() ;

	// copy constructor 
	UD_List(const UD_List& l) ;

	void set_elem_type_tag(UD_Type_Tag tt) ;
	UD_Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void push_back(UD_universal_data* data, int aspect_id = 0) ;
	void pop_back (int aspect_id = 0) ;
	unsigned int size(int aspect_id = 0) ;
	list<int> get_aspect_in_structrue() ;

	//iterator 
	void begin(int aspect_id = 0) ;
	bool end(int aspect_id = 0) ;
	void operator++(int) ;
	UD_universal_data* operator*() ;
	void erase(int aspect_id = 0) ; // erase the element that iter iterate; use it with iterator 

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator< (const UD_universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct UD_Multimap_data
{
	UD_Multimap_data() ; // constructor 
	multimap<UD_universal_data, UD_universal_data*>::iterator iter ;
	map<int, multimap<UD_universal_data, UD_universal_data*> > elem ; // (aspect id, container)
	UD_Type_Tag type_tag_key ;// used to limit the type of key ; default is T_unlimited
	UD_Type_Tag type_tag_value ; // used to limit the type of value ; default is T_unlimited
	UD_universal_data* owner ; // point to owner (UD_Multimap) 
} ;

class UD_Multimap : public UD_universal_data
{
public: 
	UD_Multimap() ; 
	~UD_Multimap() ;
	// copy constructor 
	UD_Multimap(const UD_Multimap& m) ; 

	// set & get type tag of key and value 
	void set_type_tag_key(UD_Type_Tag tt) ;
	void set_type_tag_value(UD_Type_Tag tt) ;
	UD_Type_Tag get_type_tag_key() ;
	UD_Type_Tag get_type_tag_value() ;

	// insert
	void insert(UD_universal_data key, UD_universal_data* value, int aspect_id = 0) ;
	// size 
	unsigned int size(int aspect_id = 0) ;
	list<int> get_aspect_in_structure() ;

	// operator 
	bool operator == (UD_universal_data other_data) ;
	bool operator != (UD_universal_data other_data) ;
	bool operator < (const UD_universal_data& rhs) const ;

	// iterator 
	void begin(int aspect_id = 0) ;
	bool end(int aspect_id = 0) ;
	void operator++(int) ;
	void erase(int aspect_id = 0) ; // erase iterated element 
	UD_universal_data key() ; // return key of iterated element 
	UD_universal_data* value() ; // return value of iterated element 

protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct UD_Attribute_List_data
{
	UD_Attribute_List_data() ;
	map<int, map<UD_universal_data, UD_universal_data> > attributes ; // (aspect id, container) 
	map<UD_universal_data, UD_universal_data>::iterator attrs_iter ;
	UD_Type_Tag type_tag_name ;// used to limit the type of name ; default is T_unlimited
	UD_Type_Tag type_tag_value ; // used to limit the type of value ; default is T_unlimited
	UD_universal_data* owner ; // point to owner (UD_Attribute_List)
} ;

class UD_Attribute_List : public UD_universal_data
{
public: 
	UD_Attribute_List() ; 
	~UD_Attribute_List() ;
	UD_Attribute_List(const UD_Attribute_List &al) ;

	void add_attribute_al(string key, UD_universal_data val, int aspect_id = 0) ;
	void add_attribute_al(UD_universal_data key, UD_universal_data val, int aspect_id = 0) ;

	UD_universal_data get_attribute_al(string key, int aspect_id = 0) ;
	UD_universal_data get_attribute_al(UD_universal_data key, int aspect_id = 0) ;
	UD_universal_data* get_attribute_ref_al(string key, int aspect_id = 0) ;
	UD_universal_data* get_attribute_ref_al(UD_universal_data key, int aspect_id = 0) ;
	void set_attribute_al(string key, UD_universal_data val, int aspect_id = 0) ; // if not found the attribute, set fail
	void set_attribute_al(UD_universal_data key, UD_universal_data val, int aspect_id = 0) ; // if not found the attribute, set fail
	void remove_attribute_al(string key, int aspect_id = 0) ;
	void remove_attribute_al(UD_universal_data key, int aspect_id = 0) ;
	// 
	unsigned int size(int aspect_id = 0) ;
	list<int> get_aspect_in_structure() ;

	// operator 
	bool operator==(UD_universal_data other_data) ;
	bool operator!=(UD_universal_data other_data) ;
	bool operator <(const UD_universal_data& rhs) const ;

	// iterator 
	void begin(int aspect_id = 0) ;
	bool end(int aspect_id = 0) ;
	void operator++(int) ;
	UD_universal_data get_attribute_name_al() ;
	UD_universal_data get_attribute_value_al() ;
protected: 
} ;

//###########################################
// iterator 
//###########################################
class UD_Virtual_Iterator 
{
public: 
	UD_Virtual_Iterator() {tag = T_unknown ; aspect = -1 ;} 
	~UD_Virtual_Iterator() {} 

	UD_Type_Tag get_tag() {return tag ;}
	int get_aspect() {return aspect ;}

	// access function 
	virtual void begin() =0;
	virtual bool end() =0 ;
	virtual void advance() =0  ;
	virtual UD_universal_data* operator*() = 0;
protected: 
	void set_tag(UD_Type_Tag t) {tag = t ;}
	void set_aspect(int id) {aspect = id ;}
	UD_Type_Tag tag ;
	int aspect ;
} ;

class UD_Vector_Iterator: public UD_Virtual_Iterator 
{
public: 
	UD_Vector_Iterator(UD_universal_data* org, int apect_id = 0) ;
	~UD_Vector_Iterator() ;

	virtual void begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	vector<UD_universal_data*>::iterator iter ;
} ;

class UD_Pair_Iterator: public UD_Virtual_Iterator
{
public: 
	UD_Pair_Iterator(UD_universal_data* org, int aspect_id = 0) ; 
	~UD_Pair_Iterator() ;

	virtual void begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	int count; // 1 is first element, 2 is second element, 3 is end 
} ;

class UD_List_Iterator: public UD_Virtual_Iterator 
{
public: 
	UD_List_Iterator(UD_universal_data* org, int aspect_id = 0) ;
	~UD_List_Iterator() ;

	virtual void begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	list<UD_universal_data*>::iterator iter ;
} ;

class UD_Set_Iterator: public UD_Virtual_Iterator
{
public: 
	UD_Set_Iterator(UD_universal_data* org, int aspect_id = 0) ;
	~UD_Set_Iterator() ;

	virtual void begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	set<UD_universal_data*>::iterator iter ;
} ;

class UD_Map_Iterator: public UD_Virtual_Iterator 
{
public: 
	UD_Map_Iterator(UD_universal_data* org, int aspect_id = 0) ;
	~UD_Map_Iterator() ;

	virtual void begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	map<UD_universal_data, UD_universal_data*>::iterator iter ;
} ;

class UD_Multimap_Iterator: public UD_Virtual_Iterator 
{
public: 
	UD_Multimap_Iterator(UD_universal_data* org, int aspect_id = 0) ;
	~UD_Multimap_Iterator() ;

	virtual void begin() ;
	virtual bool end() ;
	virtual void advance() ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	multimap<UD_universal_data, UD_universal_data*>::iterator iter ;
} ;

class UD_Attribute_List_Iterator: public UD_Virtual_Iterator
{
public: 
	UD_Attribute_List_Iterator(UD_universal_data* org, int aspect_id = 0) ;
	~UD_Attribute_List_Iterator() ;

	virtual void begin() ;
	virtual bool end() ;
	virtual void advance() ;
	virtual UD_universal_data* operator*() ;
protected: 
	UD_universal_data* organization ;
	map<UD_universal_data, UD_universal_data>::iterator iter ;
};


#endif 