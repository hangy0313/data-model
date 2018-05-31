//###################################################
// define universal data 
// some structures and primitive type that unviersal data support 
//###################################################
#ifndef UNIVERSAL_DATA_H
#define UNIVERSAL_DATA_H
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include "enumerate_constant.h"  // elem_type_tag is declared in here
#include "predeclare.h" // predeclare some class 

using namespace std; 

/* move to unviersal_data .cpp
// lookup table 
// level 2: key to level 1 table's address 
map<universal_data, map<universal_data, universal_data*>*> L2_table ;
// level 1: key to component's address 
map<universal_data, universal_data*> L1_table ;
*/

// priority of type tag ; used in the less function 
int get_type_tag_priority(Type_Tag tt) ;

//#################################
// abstract class 
//#################################
class universal_data 
{
public: 
	universal_data() ;
	~universal_data() ;

	// copy constructor 
	universal_data(const universal_data& ud) ;

	// type_tag
	void set_type_tag(Type_Tag tt) ;
	Type_Tag get_type_tag() ;
	// user-defined type tag 
	void set_user_defined_type_tag(string tag) ;
	string get_user_defined_type_tag() ;
	//parent pointer
	void set_parent_point(universal_data* ptr) ;
	universal_data* get_parent_point() ;
	// data 
	void set_data( unsigned* d) ; // user need to maintain the type of data 
	unsigned* get_data() ;
	// attribute list
	bool add_attribute( string nm, universal_data ud) ;
	void set_attribute(string nm, universal_data ud) ;
	void remove_attribute(string nm) ;
	universal_data find_attribute(string nm) ; // if name is not existed in attribute list, reture a invalid universal_data( tag is T_unknown)
	universal_data* find_attribute_ref(string nm) ;
	void attribute_begin() ;
	bool attribute_end() ;
	void attribute_next() ;
	string get_attribute_name() ; // retrun the name of attribute that iterator iterate
	universal_data get_attribute() ;// return the attribute that iterator iterate 

	// operator 
	universal_data& operator = (universal_data other_data);// deep copy for each kind of universal data 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function

	//copy function 
	unsigned* copy_universal_data(universal_data& source) ;
	
//	friend class universal_data ;
protected: 
	Type_Tag type_tag ; //the type of structure 	
	string user_defined_type_tag ; // the type tag is used when type is user-defined
	universal_data* parent_ptr ; // point to the parent universal_data object 
	unsigned* data_ptr ; // aggregate relational data for each kind of universal_data 
	list<pair<universal_data, universal_data> > attribute_list ;//attribute list ; key must be string
	list<pair<universal_data, universal_data> >::iterator attr_iter ;
} ;

//#################################
// symbolic link 
//#################################
struct symbolic_link_data
{
	symbolic_link_data() ; // constructor 
	universal_data L1_key ;// used to find top level component 
	unsigned* table_pointer;
	universal_data L1_name ; // used to find L1 table 
	bool L1_flag;// true means table pointer point to a level 1 table; otherwise, table pointer point to a level 2 table 
	bool Absolute_flag ; // true means this is absolute path, else it is a relative path 
	list<condition*> path ;
} ;

class symbolic_link: public universal_data
{
public: 
	symbolic_link() ;
	~symbolic_link() ;

	// copy constructor 
	symbolic_link(const symbolic_link& sym) ;

	// following three parameter are recorded in attribute list 
	// key
	void set_L1_key(universal_data key) ;
	universal_data get_L1_key() ;
	// table pointer
	void set_table_pointer(unsigned* tp) ;
	unsigned* get_table_pointer() ;
	// L1 name (level 1 table name)
	void set_L1_name(universal_data nm) ;
	universal_data get_L1_name() ;
	// L1 flag 
	void set_L1_flag(bool f) ;
	bool get_L1_flag() ;
	// absolute flag 
	void set_Absolute_flag(bool f) ;
	bool get_Absolute_flag() ;
	// path 
	void path_push_back(condition* c) ;
	void path_pop_back() ;

	// link to target
	universal_data* linkto_target() ;

protected: 
} ;

//#################################
// primitive data 
//#################################
class Int: public universal_data
{
public: 
	Int();
	~Int();

	// copy constructor 
	Int(const Int& i) ;

	void set_value(int d) ;
	int* getptr();

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class Char: public universal_data
{
public: 
	Char();
	~Char() ;

	// copy constructor 
	Char(const Char& c) ;

	void set_value(char d) ;
	char* getptr();

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class Float: public universal_data
{
public: 
	Float();
	~Float();

	// copy constructor 
	Float(const Float& f) ;

	void set_value(float d);
	float* getptr();

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class String: public universal_data
{
public: 
	String();
	~String() ;

	// copy constructor 
	String(const String& s) ;

	void set_value(string d) ;
	string* getptr();

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class Double: public universal_data
{
public: 
	Double();
	~Double() ;

	// copy constructor 
	Double(const Double& d) ;

	void set_value(double d);
	double* getptr();

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

class Bool: public universal_data
{
public: 
	Bool();
	~Bool() ;

	// copy constructor 
	Bool(const Bool& d) ;

	void set_value(bool d);
	bool* getptr();

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected:
	// value is in the data of universal_data, need to cast before using
}; 

//#################################
// structure 
//#################################
struct Vector_data 
{
	Vector_data() ;// constructor 
	vector<universal_data*>::iterator iter ;
	vector<universal_data*> vec ;
	Type_Tag elem_type_tag ; // used to limit the type of element
} ;

class Vector: public universal_data
{
public: 
	Vector();
	~Vector() ;

	// copy constructor 
	Vector(const Vector& v) ;

	void set_elem_type_tag(Type_Tag tt) ;
	Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void push_back(universal_data* d);
	void pop_back() ;
	universal_data* operator[] (unsigned int n) ;
	unsigned int size() ;

	// iterator
	void begin() ;
	bool end() ;
	void operator ++(int) ;
	universal_data* operator*() ;
	
	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct Pair_data
{
	Pair_data() ; // constructor 
	pair< universal_data*, universal_data*> elem; 
	Type_Tag type_tag_first ; // used to limit the type of first element of pair 
	Type_Tag type_tag_second ;// used to limit the type of second element of pair
} ;

class Pair: public universal_data
{
public: 
	Pair();
	~Pair() ;

	// copy constructor 
	Pair(const Pair& p) ;

	void set_type_tag_first(Type_Tag tt) ;
	Type_Tag get_type_tag_first() ;
	void set_type_tag_second(Type_Tag tt) ;
	Type_Tag get_type_tag_second() ;
	// insert, delete, get
	void set_first(universal_data* ud) ;
	void set_second(universal_data* ud) ;

	universal_data* first() ;
	universal_data* second() ;

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct Map_data
{
	Map_data() ;
	map<universal_data, universal_data*>::iterator iter ;
	map<universal_data, universal_data*> elem ;
	Type_Tag type_tag_key ; // used to limit the type of key 
	Type_Tag type_tag_value ; // used to limit the type of value 
} ;

class Map: public universal_data
{
public: 
	Map();
	~Map() ;

	// copy constructor 
	Map(const Map& m) ;

	void set_type_tag_key(Type_Tag tt) ;
	Type_Tag get_type_tag_key() ;
	void set_type_tag_value(Type_Tag tt) ;
	Type_Tag get_type_tag_value() ;
	// insert, delete, get
	void insert(universal_data key, universal_data* val) ;
	universal_data* find(universal_data key) ; // find the value that key map to 
	void erase( universal_data key) ;
	unsigned int size() ;

	// iterator 
	void begin();
	bool end() ;
	void operator++(int) ;
	universal_data key() ; // get the key that iterator iterate 
	universal_data* value() ; // get the value that iterator iterate

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct Set_data
{
	Set_data() ;
	set<universal_data*>::iterator iter ;
	set<universal_data*> elem ; 
	Type_Tag elem_type_tag ; // used to limit the type of element 
} ;

class Set: public universal_data
{
public: 
	Set();
	~Set() ;

	// copy constructor 
	Set(const Set& s) ;

	void set_elem_type_tag(Type_Tag tt) ;
	Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void insert(universal_data* d) ;
	void erase(universal_data* d) ;
	unsigned int size() ;

	//iterator 
	void begin() ;
	bool end() ; 
	void operator++(int) ;
	universal_data* operator*() ;

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct List_data
{
	List_data() ;
	list<universal_data*>::iterator iter ;
	list<universal_data*> elem ;
	Type_Tag elem_type_tag ; // used to limit the type of element 
} ;

class List: public universal_data
{
public: 
	List();
	~List() ;

	// copy constructor 
	List(const List& l) ;

	void set_elem_type_tag(Type_Tag tt) ;
	Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void push_back(universal_data* data) ;
	void pop_back () ;
	unsigned int size() ;

	//iterator 
	void begin() ;
	bool end() ;
	void operator++(int) ;
	universal_data* operator*() ;
	void erase() ; // erase the element that iter iterate; use it with iterator 

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator< (const universal_data& rhs) const ; // less function
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

struct Multimap_data
{
	Multimap_data() ; // constructor 
	multimap<universal_data, universal_data*>::iterator iter ;
	multimap<universal_data, universal_data*> elem ;
	Type_Tag type_tag_key ;// used to limit the type of key ; default is T_unlimited
	Type_Tag type_tag_value ; // used to limit the type of value ; default is T_unlimited
} ;

class Multimap : public universal_data
{
public: 
	Multimap() ; 
	~Multimap() ;
	// copy constructor 
	Multimap(const Multimap& m) ; 

	// set & get type tag of key and value 
	void set_type_tag_key(Type_Tag tt) ;
	void set_type_tag_value(Type_Tag tt) ;
	Type_Tag get_type_tag_key() ;
	Type_Tag get_type_tag_value() ;

	// insert
	void insert(universal_data key, universal_data* value) ;
	// size 
	unsigned int size() ;

	// operator 
	bool operator == (universal_data other_data) ;
	bool operator != (universal_data other_data) ;
	bool operator < (const universal_data& rhs) const ;

	// iterator 
	void begin() ;
	bool end() ;
	void operator++(int) ;
	void erase() ; // erase iterated element 
	universal_data key() ; // return key of iterated element 
	universal_data* value() ; // return value of iterated element 

protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

// condition 
class condition
{// use to find the next component via condition 
public :
	condition() {}
	~condition() {}

	// give a unviersal data, find the universal data that satisfy the condition and it is the child component of given universal data 
	virtual universal_data* find_target(universal_data* current) = 0 ; 
protected: 
} ;

//#################################
// structure (extension)
//#################################
struct Attribute_List_data
{
	map<universal_data, universal_data> attributes ;
	map<universal_data, universal_data>::iterator attrs_iter ;
} ;

class Attribute_List : public universal_data
{
public: 
	Attribute_List() ; 
	~Attribute_List() ;
	Attribute_List(const Attribute_List &al) ;

	void add_attribute_al(string key, universal_data val) ;
	void add_attribute_al(universal_data key, universal_data val) ;

	universal_data get_attribute_al(string key) ;
	universal_data get_attribute_al(universal_data key) ;
	universal_data* get_attribute_ref_al(string key) ;
	universal_data* get_attribute_ref_al(universal_data key) ;
	void set_attribute_al(string key, universal_data val) ; // if not found the attribute, set fail
	void set_attribute_al(universal_data key, universal_data val) ; // if not found the attribute, set fail
	
	void remove_attribute_al(string key) ;
	void remove_attribute_al(universal_data key) ;

	// 
	unsigned int size() ;

	// operator 
	bool operator==(universal_data other_data) ;
	bool operator!=(universal_data other_data) ;
	bool operator <(const universal_data& rhs) const ;

	// iterator 
	void begin() ;
	bool end() ;
	void operator++(int) ;
	universal_data get_attribute_name_al() ;
	universal_data get_attribute_value_al() ;
protected: 
} ;

//#################################
// parent point  version  
//#################################
// parent class 
class parent_class 
{
public: 
	parent_class() ;
	~parent_class() ;
	void set_parent_pointer(universal_data* ptr) ;
	universal_data* get_parent_pointer() ;
protected: 
	universal_data* parent_ptr ;
} ;
// p vector 
class p_Vector_data:public parent_class
{
public: 
	p_Vector_data() ;// constructor 
	vector<universal_data*>::iterator iter ;
	vector<universal_data*> vec ;
	Type_Tag elem_type_tag ; // used to limit the type of element
} ;

class p_Vector: public universal_data
{
public: 
	p_Vector();
	~p_Vector() ;

	// copy constructor 
	p_Vector(const p_Vector& v) ;

	void set_elem_type_tag(Type_Tag tt) ;
	Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void push_back(universal_data* d);
	void pop_back() ;
	universal_data* operator[] (unsigned int n) ;
	unsigned int size() ;

	// iterator
	void begin() ;
	bool end() ;
	void operator ++(int) ;
	universal_data* operator*() ;
	
protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;
//BP_Pair
class p_Pair_data: public parent_class 
{
public: 
	p_Pair_data() ; // constructor 
	pair< universal_data*, universal_data*> elem; 
	Type_Tag type_tag_first ; // used to limit the type of first element of pair 
	Type_Tag type_tag_second ;// used to limit the type of second element of pair
} ;

class p_Pair: public universal_data
{
public: 
	p_Pair();
	~p_Pair() ;

	// copy constructor 
	p_Pair(const p_Pair& p) ;

	void set_type_tag_first(Type_Tag tt) ;
	Type_Tag get_type_tag_first() ;
	void set_type_tag_second(Type_Tag tt) ;
	Type_Tag get_type_tag_second() ;
	// insert, delete, get
	void set_first(universal_data* ud) ;
	void set_second(universal_data* ud) ;

	universal_data* first() ;
	universal_data* second() ;

protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;
//p_Map
class p_Map_data: public parent_class 
{
public: 
	p_Map_data() ;
	map<universal_data, universal_data*>::iterator iter ;
	map<universal_data, universal_data*> elem ;
	Type_Tag type_tag_key ; // used to limit the type of key 
	Type_Tag type_tag_value ; // used to limit the type of value 
} ;

class p_Map: public universal_data
{
public: 
	p_Map();
	~p_Map() ;

	// copy constructor 
	p_Map(const p_Map& m) ;

	void set_type_tag_key(Type_Tag tt) ;
	Type_Tag get_type_tag_key() ;
	void set_type_tag_value(Type_Tag tt) ;
	Type_Tag get_type_tag_value() ;
	// insert, delete, get
	void insert(universal_data key, universal_data* val) ;
	universal_data* find(universal_data key) ; // find the value that key map to 
	void erase( universal_data key) ;
	unsigned int size() ;

	// iterator 
	void begin();
	bool end() ;
	void operator++(int) ;
	universal_data key() ; // get the key that iterator iterate 
	universal_data* value() ; // get the value that iterator iterate

protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;
// p_Set
class p_Set_data: public parent_class
{
public: 
	p_Set_data() ;
	set<universal_data*>::iterator iter ;
	set<universal_data*> elem ; 
	Type_Tag elem_type_tag ; // used to limit the type of element 
} ;

class p_Set: public universal_data
{
public: 
	p_Set();
	~p_Set() ;

	// copy constructor 
	p_Set(const p_Set& s) ;

	void set_elem_type_tag(Type_Tag tt) ;
	Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void insert(universal_data* d) ;
	void erase(universal_data* d) ;
	unsigned int size() ;

	//iterator 
	void begin() ;
	bool end() ; 
	void operator++(int) ;
	universal_data* operator*() ;

protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;
// BP_List
class p_List_data: public parent_class
{
public: 
	p_List_data() ;
	list<universal_data*>::iterator iter ;
	list<universal_data*> elem ;
	Type_Tag elem_type_tag ; // used to limit the type of element 
} ;

class p_List: public universal_data
{
public: 
	p_List();
	~p_List() ;

	// copy constructor 
	p_List(const p_List& l) ;

	void set_elem_type_tag(Type_Tag tt) ;
	Type_Tag get_elem_type_tag() ;
	// insert, delete, get
	void push_back(universal_data* data) ;
	void pop_back () ;
	unsigned int size() ;

	//iterator 
	void begin() ;
	bool end() ;
	void operator++(int) ;
	universal_data* operator*() ;
	void erase() ; // erase the element that iter iterate; use it with iterator 

protected: 
	// aggregated structure is in the data of universal_data, need to cast before using
} ;

//#################################
// Iterator 
//#################################
class Virtual_Iterator 
{
public: 
	Virtual_Iterator() {} 
	~Virtual_Iterator() {} 

	// access function 
	virtual void  begin() =0;
	virtual bool end() =0 ;
	virtual void advance() =0  ;
	virtual universal_data* operator*() = 0;
protected: 
} ;

class Vector_Iterator: public Virtual_Iterator 
{
public: 
	Vector_Iterator(universal_data* org) ;
	~Vector_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	vector<universal_data*>::iterator iter ;
} ;

class Pair_Iterator: public Virtual_Iterator
{
public: 
	Pair_Iterator(universal_data* org) ; 
	~Pair_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	int count; // 1 is first element, 2 is second element, 3 is end 
} ;

class List_Iterator: public Virtual_Iterator 
{
public: 
	List_Iterator(universal_data* org) ;
	~List_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	list<universal_data*>::iterator iter ;
} ;

class Set_Iterator: public Virtual_Iterator
{
public: 
	Set_Iterator(universal_data* org) ;
	~Set_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	set<universal_data*>::iterator iter ;
} ;

class Map_Iterator: public Virtual_Iterator 
{
public: 
	Map_Iterator(universal_data* org) ;
	~Map_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	map<universal_data, universal_data*>::iterator iter ;
} ;

class Multimap_Iterator: public Virtual_Iterator 
{
public: 
	Multimap_Iterator(universal_data* org) ;
	~Multimap_Iterator() ;

	virtual void begin() ;
	virtual bool end() ;
	virtual void advance() ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	multimap<universal_data, universal_data*>::iterator iter ;
} ;

// ----------------------------------
// extend at 20140718
// ----------------------------------
class Attribute_List_Iterator: public Virtual_Iterator
{
public: 
	Attribute_List_Iterator(universal_data* org) ;
	~Attribute_List_Iterator() ;

	virtual void begin() ;
	virtual bool end() ;
	virtual void advance() ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	map<universal_data, universal_data>::iterator iter ;
};
//-----------------------------------

class p_Vector_Iterator: public Virtual_Iterator 
{
public: 
	p_Vector_Iterator(universal_data* org) ;
	~p_Vector_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	vector<universal_data*>::iterator iter ;
} ;

class p_Pair_Iterator: public Virtual_Iterator
{
public: 
	p_Pair_Iterator(universal_data* org) ; 
	~p_Pair_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	int count; // 1 is first element, 2 is second element, 3 is end 
} ;

class p_List_Iterator: public Virtual_Iterator 
{
public: 
	p_List_Iterator(universal_data* org) ;
	~p_List_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	list<universal_data*>::iterator iter ;
} ;

class p_Set_Iterator: public Virtual_Iterator
{
public: 
	p_Set_Iterator(universal_data* org) ;
	~p_Set_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	set<universal_data*>::iterator iter ;
} ;

class p_Map_Iterator: public Virtual_Iterator 
{
public: 
	p_Map_Iterator(universal_data* org) ;
	~p_Map_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
protected: 
	universal_data* organization ;
	map<universal_data, universal_data*>::iterator iter ;
} ;

#endif
