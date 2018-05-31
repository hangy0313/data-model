#ifndef TRANSFORMED_ERD_H
#define TRANSFORMED_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std;

class specializeERD;
class specializeEntity;
class specializeRelationship;

struct linkShema{
    string type;
    string linkTarget;
};

class specializeERD : public navigationERD
{
public:
    specializeERD();
    specializeERD(string erdName);
    ~specializeERD();
    
    list<specializeEntity> getSpecializeEntityList();
    list<specializeRelationship> getSpecializeRelationList();
    
    void addEntity(specializeEntity entity);
    void removeEntity(string entityName);
    
    void addRelationship(specializeRelationship relationship);
    void removeRelationship(string relationshipName);
    
    specializeEntity findEntity(string entityName);
    specializeRelationship find_Relationship(string relationshipName);
protected:
    list<specializeEntity> specializeEntityList;
    list<specializeRelationship> specializeRelationList;
};

class specializeEntity : public navigationEntity
{
public:
    specializeEntity();
    specializeEntity(string entityName);
    ~specializeEntity();
    
    void addLinkSchema(linkShema link);
    void addLinkSchema(string linkTarget, string type);
    void removeLinkSchema(string linkTarget);
protected:
    list<linkShema> linkShemaList;
};

class specializeRelationship : public cardinalityRelationship
{
public:
    specializeRelationship();
    specializeRelationship(string relationshipName);
    ~specializeRelationship();
    
    void addLinkSchema(linkShema link);
    void addLinkSchema(string linkTarget, string type);
    void removeLinkSchema(string linkTarget);
protected:
    list<linkShema> linkShemaList;
};

#endif