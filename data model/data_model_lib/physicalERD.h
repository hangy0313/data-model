#ifndef PHYSICAL_ERD_H
#define PHYSICAL_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>

UD_Map* tansToPhysicalModel(TransformedERD* transERD);

struct dataMemberSchema
{
    string accessSpecifier;
    string type;
    string name;
};

struct memberFunctionSchema
{
    string returnType;
    string functionName;
    list<pair<string, string> > parameters;
    node* functionBody;
};

class physicalERD : public UD_universal_data
{
public:
    physicalERD(string name);
    ~physicalERD();
    
    void setClassName(string name);
    string getClassName();
    
    void addDataMember(string type, string name, string accessSpecifier);
    void removeDataMember(string name);
    list<dataMemberSchema> getAllDataMember();

    //for specific function
    void addMemberFunction(string rType, string functionName, list<pair<string, string> > plist, node* content);
    //for basic function
    void addMemberFunction(string prefix, string rtype, string suffix, list<pair<string, string> > plist);
    void removeMemberFunction(string functionName);
    list<memberFunctionSchema> getAllMemberFunction();
protected:
    string className;
    list<dataMemberSchema> dataMemberList;
    list<memberFunctionSchema> memberFunctionList;
};

#endif