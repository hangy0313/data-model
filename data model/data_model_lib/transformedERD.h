#ifndef TRANSFORMED_ERD_H
#define TRANSFORMED_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std;

class RelationshipRecord;
class TransformedERD;
class TransformedEntity;
class TransformedRelationship;

/*
 *  data member : Relationship, Transformed Entity set and Relationship set
 */
class RelationshipRecord : public Attribute_List
{
public:
    RelationshipRecord(string name);
    ~RelationshipRecord();
    
    void setName(string name);
    string getName();
    
    void setSourceRelationship(Relationship* relationship);
    Relationship* getSourceRelationship();
    
    void addTransformedEntity(Entity* entiyty);
    void addTransformedRelationship(Relationship* relationship);
    
    Map* getTransformedEntitySet();
    Map* getTransformedRelationshipSet();
    
    void dump();
};

/*
 *  member data : name, tansformed entity table, tansformed relationship table
 */
class TransformedERD : public Attribute_List
{
public:
    TransformedERD(string erdName);
    ~TransformedERD();
    
    void setERDName(string erdName);
    string getERDName();
    Map* getEntityTable();
    Map* getRelationshipTable();
    void addEntity(TransformedEntity* en);
    void removeEntity(string entityName);
    void addRelationship(TransformedRelationship* relationship);
    void removeRelationship(string relationshipName);
    TransformedEntity* findEntity(string entityName);
    TransformedRelationship* findRelationship(string relationshipName);
    
    void dump();
};

/*
 *  member data : name, attribute set, link set
 */
class TransformedEntity : public Attribute_List
{
public:
    TransformedEntity();
    TransformedEntity(string entityName);
    ~TransformedEntity();
    
    void setEntityName(string entityName);
    string getEntityName();
    
    void addAttribute(string attributeName, string attributeType);
    void addAttribute(Attribute_List tmpAttribute);
    void removeAttribute(string attributeName);
    Attribute_List* getAttributeList();
    
    void dump();
};

/*
 *  member data : name, link set
 */
class TransformedRelationship : public Attribute_List
{
public:
    TransformedRelationship();
    TransformedRelationship(string relationshipName);
    ~TransformedRelationship();
    
    void setRelationshipName(string relationshipName);
    string getRelationshipName();
    
    void dump();
};

/*
 *  member data : link tag(to entity or to relationship)
 *                target name, symbolic link, true link
 */
class Link : public Attribute_List
{
    
};

Map* transferToBinary(ERD* erd);

#endif