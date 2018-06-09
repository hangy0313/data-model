#ifndef LOGICAL_ERD_H
#define LOGICAL_ERD_H
#include "../universal_data_lib/universal_data_lib.h"
#include "sourceERD.h"
#include "RFlattenedERD.h"
#include <iostream>
#include <string>
using namespace std ;

// -- predeclaration
class Logical_ERD_schema;
class Logical_Entity_schema;
class Relationship_Entity;
class Link;

// -- global table
map<string,Logical_ERD_schema*> Logical_ERD_table;


// --------------------------------------------
// --  logical ERD schema
// --------------------------------------------
class Logical_ERD_schema : public Attribute_List
{
public:
    Logical_ERD_schema(string erd_name);
    ~Logical_ERD_schema();
    
    string get_ERD_name();
    
    Map* get_Entity_table();
    Map* get_Relationship_entity_table();
    
    void add_an_Entity(Logical_Entity_schema* entity);
    void remove_an_Entity(string E_name);
    
    void add_a_Relationship_entity(Relationship_Entity* R_entity);
    void remove_a_Relationship_entity(string R_name);
    
    Logical_Entity_schema* find_Entity(string E_name);
    Relationship_Entity* find_Relationship_entity(string R_name);
    
    void binding(string E_name, string R_name,string Role_name);
    void unbinding(string E_name, string R_name,string Role_name);
    
    // only for debug
    void cout_entity_link_container();//print function
protected:
    void set_ERD_name(string erd_name);
};

// --------------------------------------------
// --  logical entity schema
// --------------------------------------------
class Logical_Entity_schema : public Unit_schema
{
public:
    Logical_Entity_schema();
    Logical_Entity_schema(string E_name);
    ~Logical_Entity_schema();
    void set_E_name(string E_name);
    string get_E_name();
    void add_attribute_and_type(string attribute_name,string att_type);
    void add_attribute_and_type(Attribute_List att_temp);
    void remove_attribute_and_type(string attribute_name) ;
    Attribute_List* get_attribute_and_type();
    // the following is container's design
    void set_set_flag(bool set_flag);
    bool get_set_flag();
    void set_multi_key_flag(bool multi_flag);
    bool get_multi_key_flag();
    Map* get_key_unique_map();
    void add_key_unique(string key_name,bool unique_flag);
    void set_key_unique(string key_name,bool unique_flag);
    bool get_key_unique(string key_name);
    void remove_key_unique(string key_name);
    
    void remove_container(string R_name,string target_role_name);
    
    void remove_link(string R_name,Link linktemp,string target_role_name);//C
    
    void add_linkinlinkset(string R_name,string target_role_name,Link linktemp);//C
    void add_a_link(string R_name,string target_role_name,Link linktemp);//C
    void add_head_link(string R_name,string target_role_name,Link linktemp);//0106 OK
    void add_linkinlist(string R_name,string target_role_name,Relationship_Entity* REptr);
    
    void add_a_next_link(string R_name,string target_role_name,Link linktemp);//
    void add_decision_set(string R_name,string target_role_name,Attribute_List RE);
    void remove_all_next_link(string R_name,string target_role_name);
    void remove_a_next_link(string R_name,string target_role_name);
    
    universal_data* get_link(string R_name,string target_role_name);
    
    Link* get_nextlink(string R_name,string target_role_name);
    
    string get_link_type(string R_name,string target_role_name);
    
    Attribute_List get_all_link_info() ; // add at (2015/6/10)  : link, headlink, nextlink
    Attribute_List get_all_link_in_list_info() ; // add at (2015/6/11) : list, linkset
    
    // only for debug
    void cout_link_container();//print function
protected:
};

// --------------------------------------------
// --  logical relationship entity schema
// --------------------------------------------
class Relationship_Entity : public Unit_schema
{
public:
    Relationship_Entity();
    Relationship_Entity(string R_name);
    
    ~Relationship_Entity();
    void set_R_name(string R_name);
    string get_R_name();
    void set_degree(int degree);
    int get_degree();
    void set_recursive_flag(bool re_flag);
    bool get_recursive_flag();
    
    string find_another_role_name(string source_role);
    
    void add_role_to_cardinality(string this_role,string target_cardi);
    string get_role_to_cardinality(string this_role);
    void remove_link(string role_name);
    
    void add_a_link(string role_name,Link linktemp);
    
    void add_a_next_link(string role_name,Link linktemp);//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    Link* get_link(string role_name);
    
    Link* get_next_link(string role_name);//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void remove_next_link(string role_name);
    
    void set_degeneration(string role_name, string thedirection, bool degre);
    bool get_degeneration(string role_name, string thedirection);
    
    void set_embedding(string role_name, string thedirection, string embedding);
    string get_embedding(string role_name, string thedirection);
    
    
    void set_link_decision(string role_name, string link_dec);
    string get_link_decision(string role_name);
    
    list<string> get_role_list();
    
    Attribute_List get_all_link_info() ; // add at (2015/6/10) : link, nextlink
protected:
};

// --------------------------------------------
// --  link
// --------------------------------------------
class Link : public Attribute_List
{
public:
    Link() ;
    Link(string link_decision) ;
    ~Link() ;
    void set_linktag(string link_decision);
    string get_linktag();
    void set_link(universal_data key);
    void set_link(universal_data* link);
    universal_data* get_link();
    universal_data get_key();
    
protected:
} ;

// --------------------------------------------
// --  transfer function
// --------------------------------------------
Logical_ERD_schema* RFtoL_ERD(RF_ERD_schema* RFerd);//transfer RF_ERD to Logical ERD

// --------------------------------------------
// --  decision procedure
// --------------------------------------------
void decision_procedure(Logical_ERD_schema* LERDptr);
void direction_degrenerations(Relationship_Entity* REptr,string rolename,Logical_Entity_schema* srcEptr);
void embedding(Relationship_Entity* REptr, Logical_Entity_schema* Entity1,string roleA, Logical_Entity_schema* Entity2,string roleB);

#endif 

