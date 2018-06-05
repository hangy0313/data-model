#ifndef TRANSFORMED_ERD_H
#define TRANSFORMED_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std;

class RelationshipRecord ;

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

Map* transferToBinary(ERD* erd);


#endif