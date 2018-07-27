#ifndef ORIGINAL_ERD_H
#define ORIGINAL_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std;

class ERD;
class Entity;
class Relationship;
class Role;

/*
 *  member data : name, entity table, relationship table
 */
class ERD : public UD_Attribute_List
{
public:
    ERD(string erdName);
    ~ERD();

    void setERDName(string erdName);
    string getERDName();
    UD_Map* getEntityTable();
    UD_Map* getRelationshipTable();
    void addEntity(Entity* en);
    void removeEntity(string entityName);
    void addRelationship(Relationship* relationship);
    void removeRelationship(string relationshipName);
    Entity* findEntity(string entityName);
    Relationship* findRelationship(string relationshipName);
    
    void dump();
};

/*
 *  member data : name, attribute set
 */
class Entity : public UD_Attribute_List
{
public:
    Entity();
    Entity(string entityName);
    ~Entity();
    
    void setEntityName(string entityName);
    string getEntityName();
    
    void addAttribute(string attributeName,string attributeType);
    void addAttribute(UD_Attribute_List tmpAttribute);
    void removeAttribute(string attributeName);
    UD_Attribute_List* getAttributeList();
    
    void dump();
};

/*
 *  member data : name, role set
 */
class Relationship : public UD_Attribute_List
{
public:
    Relationship();
    Relationship(string relationshipName);
    ~Relationship();
    
    void setRelationshipName(string relationshipName);
    string getRelationshipName();
    
    void addRole(Role role);
    void addRole(string roleName,string entityName);
    void removeRole(string roleName);
    Role* findRole(string roleName);
    UD_Attribute_List* getRoleList();
    
    void dump();
    
//    string findEntityByRole(string roleName);
};

/*
 *  member data : name, entity name
 */
class Role : public UD_Attribute_List
{
public:
    Role();
    Role(string roleName);
    ~Role();
    
    void setRoleName(string roleName);
    string getRoleName();
    
    void setEntityName(string entityName);
    string getEntityName();
};

ERD* import(string erdScript);

#endif