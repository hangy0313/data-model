#include "physicalERD.h"

using namespace std;

void physicalERD::setClassName(string name)
{
    className = name;
}

string physicalERD::getClassName()
{
    return className;
}

void physicalERD::addDataMember(string type, string name, string accessSpecifier = "public")
{
    struct dataMemberSchema tmp;

    tmp.type = type;
    tmp.name = name;
    tmp.accessSpecifier = accessSpecifier;
    
    dataMemberList.push_back(tmp);
}

void physicalERD::removeDataMember(string name)
{
    list<dataMemberSchema>::iterator tmpIter;
    
    for(tmpIter = dataMemberList.begin();tmpIter != dataMemberList.end();tmpIter++){
        if((*tmpIter).name == name) {
            dataMemberList.erase(tmpIter);
        }
    }
}

list<dataMemberSchema> physicalERD::getAllDataMember()
{
    return dataMemberList;
}

//for specific function
void physicalERD::addMemberFunction(string rType,
                                    string functionName,
                                    list<pair<string, string>> plist,
                                    node* content)
{
    struct memberFunctionSchema tmp;
    
    tmp.returnType = rType;
    tmp.functionName = functionName;
    tmp.parameters = plist;
    tmp.functionBody = content;
    
    memberFunctionList.push_back(tmp);
}

void physicalERD::addMemberFunction(string prefix,
                                    string rtype,
                                    string suffix,
                                    list<pair<string, string>> plist)
{
    struct memberFunctionSchema tmpFunction;
    
    String stmp;
    string functionName = prefix + "_"+suffix;
    
    tmpFunction.returnType = rtype;
    tmpFunction.functionName = functionName;
    tmpFunction.parameters = plist;
    
    if(prefix == "set")
    {
        list<pair<string, string> >::iterator tmpIter;

        tmpIter = plist.begin();
        
        node* leftSide;
        leftSide->set_t_nt_flag('t');
        leftSide->set_construct_name("var");
        stmp.set_value(suffix);
        leftSide->add_node_attribute("var", stmp);
        
        node* rightSide;
        rightSide->set_t_nt_flag('t');
        rightSide->set_construct_name("var");
        stmp.set_value(tmpIter->second);
        rightSide->add_node_attribute("var", stmp);
        
        node* assignStatement;
        assignStatement->set_t_nt_flag('n');
        assignStatement->set_construct_name("assignment_statement");
        stmp.set_value("=");
        assignStatement->set_attribute("assign_op", stmp);
        assignStatement->push_node_branch(leftSide);
        leftSide->push_parent_pointer(assignStatement);
        assignStatement->push_node_branch(rightSide);
        rightSide->push_parent_pointer(assignStatement);
        
        node* content;
        content->set_t_nt_flag('n');
        content->set_construct_name("statement_sequence");
        content->push_node_branch(assignStatement);
        assignStatement->push_parent_pointer(content);
        
        tmpFunction.functionBody = content;
    }else if(prefix == "get") {
        node* returnValue;
        returnValue->set_t_nt_flag('t');
        returnValue->set_construct_name("var");
        stmp.set_value(suffix);
        returnValue->add_node_attribute("var", stmp);
        
        node* reCall;
        reCall->set_t_nt_flag('n');
        reCall->set_construct_name("return_process");
        reCall->push_node_branch(returnValue);
        returnValue->push_parent_pointer(reCall);
        
        node* content;
        content->set_t_nt_flag('n');
        content->set_construct_name("statement_sequence");
        content->push_node_branch(reCall);
        reCall->push_parent_pointer(content);
        
        tmpFunction.functionBody = content;
    }
    
    memberFunctionList.push_back(tmpFunction);
}

void physicalERD::removeMemberFunction(string functionName)
{
    list<memberFunctionSchema>::iterator tmpIter;
    
    for(tmpIter = memberFunctionList.begin();tmpIter != memberFunctionList.end();tmpIter++){
        if((*tmpIter).functionName == functionName){
            memberFunctionList.erase(tmpIter);
        }
    }
}

list<memberFunctionSchema> physicalERD::getAllMemberFunction()
{
    return memberFunctionList;
}