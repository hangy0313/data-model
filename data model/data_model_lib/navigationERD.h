#ifndef NAVIGATION_ERD_H
#define NAVIGATION_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
 *  import cardinality info
 */
Attribute_List* importNavigationInfo();

/*
 *  Role add data member : Navigation => "to_entity", "to_relationship", "bidirectional"
 */
void addNavigationToERD(ERD erd);

//class navigationERD;
//class navigationEntity;
//
//
//struct keySchema{
//    string type;
//    list<string> attributeList;
//};
//
//class navigationERD : public cardinalityERD
//{
//public:
//    navigationERD();
//    navigationERD(string erdName);
//    ~navigationERD();
//    
//    list<navigationEntity> getnavigationEntityList();
//    void addEntity(navigationEntity entity);
//    void removeEntity(string entityName);
//    navigationEntity findEntity(string entityName);
//protected:
//    list<navigationEntity> navigationEntityList;
//};
//
//class navigationEntity : public entitySchema
//{
//public:
//    navigationEntity();
//    navigationEntity(string entityName);
//    ~navigationEntity();
//
//    void addKey(keySchema key);
//    void addPrimaryKey(string keyName);
//    string getPrimaryKey();
//    void addMultiKey(list<string> attList);
//    list<keySchema> getKeyList();
//protected:
//    list<keySchema> keyList;
//};
//
//list<keySchema> importNavigationInfo();
//navigationERD* transferCardinalityToNavigation(cardinalityERD* erd);
//
#endif