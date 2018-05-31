#ifndef UNIVERSAL_DATA_EXTEND_H
#define UNIVERSAL_DATA_EXTEND_H

#include <list>
#include <iostream>
#include "universal_data.h"

using namespace std ;

struct plist_data
{
	list<universal_data*> plist ;
	list<universal_data*>::iterator plist_iter ;
} ;

class pointer_list: public universal_data
{ // no set up parent pointer, only used to store data 
public: 
	pointer_list() ;
	~pointer_list() ;

	void add(universal_data* ptr) ;
	void remove(universal_data* ptr) ;

	void begin() ; 
	bool end() ;
	void operator++(int) ;
	universal_data* operator*() ;
protected:
} ;

class Pointer_List_Iterator: public Virtual_Iterator
{
public: 
	Pointer_List_Iterator(universal_data* org) ;
	~Pointer_List_Iterator() ;

	virtual void begin() ;
	virtual bool end() ;
	virtual void advance() ;
	virtual universal_data* operator*() ;
protected:
	universal_data* organization ;
	list<universal_data*>::iterator iter ;
} ;

struct alist_data
{
	list<pair<string, universal_data> > alist ;
	list<pair<string, universal_data> >::iterator alist_iter ;
} ;

class associate_list: public universal_data
{ // no set up parent pointer, only used to store data 
public: 
	associate_list() ;
	~associate_list() ;

	void add(string nm, universal_data ud) ;
	void set(string nm, universal_data ud) ;
	void remove(string nm) ;

	void begin() ; 
	bool end() ;
	void operator++(int) ;
	string get_name() ; // get name of iterated element 
	universal_data get_value() ; // get value of iterated element 
protected:
} ;

class Associate_List_Iterator: public Virtual_Iterator
{
public: 
	Associate_List_Iterator(universal_data* org) ;
	~Associate_List_Iterator() ;

	virtual void  begin();
	virtual bool end() ;
	virtual void advance()  ;
	virtual universal_data* operator*() ;
	string first() ;
	universal_data second() ;
protected: 
	universal_data* organization ;
	list<pair<string, universal_data> >::iterator iter ;
} ;

struct pslist_data
{
	list<pair<string, string> > pslist ;
	list<pair<string, string> >::iterator pslist_iter ;
} ;

class pair_string_list: public universal_data 
{
public:
	pair_string_list() ;
	~pair_string_list() ;

	void push(string first, string second) ;
	void pop() ;

	// iterator 
	void begin() ;
	bool end() ;
	void operator++(int) ;
	string first() ;
	string second() ;
	
protected: 
} ;

#endif 
