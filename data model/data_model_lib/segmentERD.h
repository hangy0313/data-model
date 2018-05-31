#ifndef SEGMENT_ERD_H
#define SEGMENT_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

using namespace std;

class dataModel;
class tableSchema;

struct foreignKeyShema{
    string fieldName;
    string correspondingTable;
    string correspondingField;
    string type;
};

struct fieldSchema{
    string type;
    string name;
};

class dataModel
{
public:

protected:
    list<tableSchema> tableList;
};

class tableSchema
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