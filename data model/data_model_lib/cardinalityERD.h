#ifndef CARDINALITY_ERD_H
#define CARDINALITY_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std;

class cardinalityERD;
class cardinalityRelationship;
class cardinalityRole;

struct cardinalitySchema{
    int minNum;
    int maxNum;
};

class cardinalityERD : public originalERDSchema
{
public:
    cardinalityERD();
    cardinalityERD(string erdName);
    ~cardinalityERD();
    
    list<cardinalityRelationship> getCardinalityRelationList();
    void addRelationship(cardinalityRelationship relationship);
    void removeCardinalityRelation(string relationshipName);
    cardinalityRelationship findCardinalityRelation(string relationshipName);
protected:
    list<cardinalityRelationship> cardinalityRelationList;
};

class cardinalityRelationship : public relationshipSchema
{
public:
    cardinalityRelationship();
    cardinalityRelationship(string relationshipName);
    ~cardinalityRelationship();
    
    void addRoleSchema(cardinalityRole role);
    void addRoleSchema(string roleName,string entityName, int minNum, int maxNum);
    void removeRole(string roleName);
    list<cardinalityRole> getcardinalityRoleList();
    string findEntityByRole(string roleName);
    cardinalityRole findRole(string roleName);
protected:
    list<cardinalityRole> cardinalityRoleList;
};

class cardinalityRole : public roleSchema
{
public:
    cardinalityRole();
    cardinalityRole(string roleName);
    ~cardinalityRole();
    
    void setCardinality(int minNum, int maxNum);
    struct cardinalitySchema getCardinality();
protected:
    struct cardinalitySchema cardinality;
};

#endif