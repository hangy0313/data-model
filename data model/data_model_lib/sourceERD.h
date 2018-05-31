#ifndef SOURCE_ERD_H
#define SOURCE_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include <iostream>
#include <sstream>
using namespace std ;

// -- predecare
class Source_ERD_schema;
class Unit_schema;	
class Entity_schema;
class Relationship_schema;
class Role;
class Cardinality;

// -- global table 
map<string,Source_ERD_schema*> Source_ERD_table;

// --------------------------------------------
// --  Source ERD schema 
// --------------------------------------------
class Source_ERD_schema : public Attribute_List
{
public:
	Source_ERD_schema(string erd_name);
	~Source_ERD_schema();
	
	string get_ERD_name();
	Map* get_Entity_table();
	Map* get_Relationship_table();
	void add_an_Entity(Entity_schema* entity);
	void remove_an_Entity(string E_name);
	void add_a_Relationship(Relationship_schema* relationship);
	void remove_a_Relationship(string R_name);
	Entity_schema* find_Entity(string E_name);
	Relationship_schema* find_Relationship(string R_name);

	void add_binding(string E_name, string R_name,string role_name);
	void remove_binding(string E_name, string R_name,string Role_name);

	// only for testing 
	void cout_entity_data();//print function
	void cout_relationship_data();//print function
protected:
	void set_ERD_name(string erd_name);
};

// --------------------------------------------
// --  Unit schema 
// --------------------------------------------
class Unit_schema : public Attribute_List
{
public:
	Unit_schema();
	void set_Unit_name(string Unit_name);
	string get_Unit_name();
	void set_Unit_type(string Unit_type);
	string get_Unit_type();
protected:
};

// --------------------------------------------
// --  Entity schema 
// --------------------------------------------
class Entity_schema : public Unit_schema
{
public:
	Entity_schema();
	Entity_schema(string E_name);
	~Entity_schema();
	void set_E_name(string E_name);
	string get_E_name();
	
	void add_attribute_and_type(string attribute_name,string att_type); // {(name, type)'s}
	void add_attribute_and_type(Attribute_List att_temp);
	void remove_attribute_and_type(string attribute_name) ; // (add at 2015/5/29)
	Attribute_List* get_attribute_and_type();

	void add_binding(string Role_name,Relationship_schema* relationship);
	Relationship_schema* find_a_binding(string R_name,string role_name);
	void remove_binding(string R_name, string role_name);
	universal_data get_binding_list();
	
	//the following belongs to container's design 
	void set_set_flag(bool set_flag);
	bool get_set_flag();
	void set_multi_key_flag(bool multi_flag);
	bool get_multi_key_flag();
	Map* get_key_unique_map();
	void add_key_unique(string key_name,bool unique_flag);
	void set_key_unique(string key_name,bool unique_flag);
	bool get_key_unique(string key_name);
	void remove_key_unique(string key_name);
protected:
};

// --------------------------------------------
// --  Relaitonship schema 
// --------------------------------------------
class Relationship_schema : public Unit_schema
{
public:
	Relationship_schema();
	Relationship_schema(string R_name);
	~Relationship_schema();
	void set_R_name(string R_name);
	string get_R_name();
	void set_degree(int degree);
	int get_degree();
	void set_form(string formtpye);
	string get_form();
	void set_recursive_flag(bool re_flag);
	bool get_recursive_flag();

	//--------------------------------------------------------------
	
	void add_binding(universal_data role,Entity_schema* Entity);
	Entity_schema* find_a_binding(string role_name);
	void remove_binding(string role_name);
	universal_data get_binding_list();
	universal_data find_role(string Role_name);



	void Role_set_cardi_Rside(string role_name,string Minimum,string Maxmum);
	void Role_set_cardi_Rside(string role_name,Cardinality* carditemp);
	Cardinality* Role_get_cardi_Rside(string role_name);

	void Role_set_R_cardi(string role_name,string Minimum,string Maxmum);
	void Role_set_R_cardi(string role_name,Cardinality* carditemp);
	Cardinality* Role_get_R_cardi(string role_name);
	void Role_set_cardi_Eside(string role_name,string Minimum,string Maxmum);
	void Role_set_cardi_Eside(string role_name,Cardinality* carditemp);
	Cardinality* Role_get_cardi_Eside(string role_name);

	void set_degeneration(string role_name, string thedirection, bool degre);
	bool get_degeneration(string role_name, string thedirection);

	void set_embedding(string role_name, string thedirection, string embedding);
	string get_embedding(string role_name, string thedirection);

	void set_link_decision(string role_name, string link_dec);
	string get_link_decision(string role_name);

	void set_defaultdecision(string role_name);


	void cout_binding_data();//print function
protected:
};

// --------------------------------------------
// --  Role
// --------------------------------------------
class Role : public Attribute_List
{
public:
	Role();
	Role(string role_name);
	~Role(){}
	void set_R_cardi(string Minimum,string Maxmum);
	void set_R_cardi(Cardinality* carditemp);
	Cardinality* get_R_cardi();

	void set_Role_name(string Role_name);
	string get_Role_name();
protected:
};

// --------------------------------------------
// --  Role _ nary
// --------------------------------------------
class Role_nary : public Attribute_List
{
public:
	Role_nary();
	Role_nary(string role_name);
	~Role_nary(){}
	void set_cardi_Rside(string Minimum,string Maxmum);
	void set_cardi_Rside(Cardinality* carditemp);
	Cardinality* get_cardi_Rside();
	void set_cardi_Eside(string Minimum,string Maxmum);
	void set_cardi_Eside(Cardinality* carditemp);
	Cardinality* get_cardi_Eside();
	void set_Role_name(string Role_name);
	string get_Role_name();
protected:
};

// --------------------------------------------
// --  cardinality 
// --------------------------------------------
class Cardinality : public Attribute_List
{
public:
	Cardinality();
	void set_cardinality(string Minimum,string Maxmum);
	void set_Mini(string Minimum);
	string get_Mini();
	void set_Max (string Maxmum);
	string get_Max();
protected:
};
#endif 