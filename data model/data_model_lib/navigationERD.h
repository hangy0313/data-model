#ifndef NAVIGATION_ERD_H
#define NAVIGATION_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std ;

class navigationERD;
class navigationEntity;


struct keySchema{
    string type;
    string attributeName;
};

class navigationERD : public cardinalityERD
{
public:
    navigationERD();
    navigationERD(string erdName);
    ~navigationERD();
    
    list<navigationEntity> getnavigationEntityList();
    void addEntity(navigationEntity entity);
    void removeEntity(string entityName);
    navigationEntity findEntity(string entityName);
protected:
    list<navigationEntity> navigationEntityList;
};

class navigationEntity : public entitySchema
{
public:
    navigationEntity();
    navigationEntity(string entityName);
    ~navigationEntity();

    void addKey(keySchema key);
    void addKey(string type, string name);
    keySchema getKey(string name);
    list<keySchema> getKeyList();
protected:
    list<keySchema> keyList;
};

#endif