#ifndef ORIGINAL_ERD_H
#define ORIGINAL_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std ;

class originalERDSchema;
class entitySchema;
class relationshipSchema;
class roleSchema;

struct attributeSchema{
    string type;
    string name;
};

// -- global table
//map<string,Source_ERD_schema*> Source_ERD_table;

class originalERDSchema
{
public:
    originalERDSchema();
    originalERDSchema(string erdName);
    ~originalERDSchema();

    string get_ERD_name();
    list<entitySchema> getEntityList();
    list<relationshipSchema> getRelationshipList();
    void addEntity(entitySchema entity);
    void removeEntity(string entityName);
    void addRelationship(relationshipSchema relationship);
    void removeRelationship(string relationshipName);
    entitySchema find_Entity(string entityName);
    relationshipSchema find_Relationship(string relationshipName);
protected:
    string erdName;
    list<entitySchema> entityList;
    list<relationshipSchema> relationshipList;
};

class entitySchema
{
public:
    entitySchema();
    entitySchema(string entityName);
    ~entitySchema();
    
    void setEntityName(string entityName);
    string getEntityName();
    
    void addAttribute(string attributeName,string attributeType);
    void addAttribute(attributeSchema tmpAttribute);
    void removeAttribute(string attributeName);
    list<attributeSchema> getAttributeList();
protected:
    string name;
    list<attributeSchema> attributeList;
};

class relationshipSchema
{
public:
    relationshipSchema();
    relationshipSchema(string relationshipName);
    ~relationshipSchema();
    
    void setRelationshipName(string relationshipName);
    string getRelationshipName();
    
    void setDegree(int degree);
    int getDegree();
    
    void addRoleSchema(roleSchema role);
    void addRoleSchema(string role,string entityName);
    void removeRole(string roleName);
    list<roleSchema> getRoleList();
    
    string findEntityByRole(string roleName);
    roleSchema findRole(string roleName);
protected:
    string name;
    int degree;
    list<roleSchema> roleList;
};

class roleSchema
{
public:
    roleSchema();
    roleSchema(string roleName);
    ~roleSchema();
    
    void setRoleName(string roleName);
    string getRoleName();
    
    void setEntityName(string entityName);
    string getEntityName();
protected:
    string name;
    string entityName;
};

#endif