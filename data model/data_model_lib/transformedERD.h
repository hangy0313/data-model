#ifndef TRANSFORMED_ERD_H
#define TRANSFORMED_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>
#include <fstream>

using namespace std;

class RelationshipRecord;
class TransformedERD;
class TransformedEntity;
class TransformedRelationship;
class Link;

/*
 *  data member : Relationship, Transformed Entity set and Relationship set
 */
class RelationshipRecord : public UD_Attribute_List
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
    
    UD_Map* getTransformedEntitySet();
    UD_Map* getTransformedRelationshipSet();
    
    void dump();
};

/*
 *  member data : name, tansformed entity table, tansformed relationship table
 */
class TransformedERD : public UD_Attribute_List
{
public:
    TransformedERD(string erdName);
    ~TransformedERD();
    
    void setERDName(string erdName);
    string getERDName();
    UD_Map* getEntityTable();
    UD_Map* getRelationshipTable();
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
class TransformedEntity : public UD_Attribute_List
{
public:
    TransformedEntity();
    TransformedEntity(string entityName);
    ~TransformedEntity();
    
    void setEntityName(string entityName);
    string getEntityName();
    
    void addAttribute(string attributeName, string attributeType);
    void addAttribute(UD_Attribute_List tmpAttribute);
    void removeAttribute(string attributeName);
    UD_Attribute_List* getAttributeList();
    
    void addLink(Link linkName);
    void removeLink(Link linkName);
    void addListLink(Link* linkName);
    void addHeadLink(Link linkName);
    void addNextLink(Link linkName);

    
    void dump();
};

/*
 *  member data : name, link set
 */
class TransformedRelationship : public UD_Attribute_List
{
public:
    TransformedRelationship();
    TransformedRelationship(string relationshipName);
    ~TransformedRelationship();
    
    void setRelationshipName(string relationshipName);
    string getRelationshipName();
    
    void addLink(Link linkName);
    void removeLink(Link linkName);
    void addListLink(Link* linkName);
    void addHeadLink(Link linkName);
    void addNextLink(Link linkName);
    
    void dump();
};

/*
 *  member data : link name, link type, 
 *                link direction(to entity or to relationship),
 *                target name,
 *                symbolic link,
 *                true link
 */
class Link : public UD_Attribute_List
{
public:
public:
    Link();
    ~Link();
    
    void setName(string name);
    string getName();
    void setType(string type);
    string getType();
    void setDirection(string direction);
    string getDirection();
    void setTargetName(string name);
    string getTargetName();
    void setTrueLink(UD_universal_data utmp);
    UD_universal_data* getTrueLink();
};

UD_Map* transferToBinary(ERD* erd);

UD_Map* importDirectionDegeneration(string dirDegenerationScript);
void directionDegeneration(ERD* erd, string dirDegenerationScript);

UD_Map* importEmbedding(string embeddingScript);
void duplicate(ERD* erd, TransformedERD* transERD);
TransformedERD* embedding(ERD* erd, string embeddingScript);

#endif