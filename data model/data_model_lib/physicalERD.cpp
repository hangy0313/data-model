#include "physicalERD.h"

using namespace std;

void tansToPhysicalModel(TransformedERD* transERD, Map* physicalERDMap)
{
    Map* entityTable = transERD->getEntityTable();
    list<pair<string, string> > paraList;
    
    Map* relationshipTable = transERD->getRelationshipTable();
    
    //foreach entity transfer to class
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        universal_data nameTmp = entityTable->key();
        
        TransformedEntity* entityPtr = (TransformedEntity*)(entityTable->value());
        physicalERD* classPtr = new physicalERD(*(((String*)(&nameTmp))->getptr()));
        
        //add data member and basic function for attribute list
        Attribute_List* attList = entityPtr->getAttributeList();
        for(attList->begin();!attList->end();(*attList)++){
            //add data member
            universal_data name = attList->get_attribute_name_al();
            universal_data type = attList->get_attribute_value_al();
            
            string nameString = *(((String*)(&name))->getptr());
            string typeString = *(((String*)(&type))->getptr());
            
            classPtr->addDataMember(typeString, nameString, "public");
            
            //add basic member function
            paraList.clear();
            paraList.push_back(pair<string, string>(typeString, "var"));
            classPtr->addMemberFunction("set", "void", nameString, paraList);
            
            paraList.clear();
            classPtr->addMemberFunction("get", typeString, nameString, paraList);
        }
        
        Attribute_List* linkSet = (Attribute_List*)(entityPtr->get_attribute_ref_al("Link_set"));
        for(linkSet->begin();!linkSet->end();(*linkSet)++){
            universal_data tmp = linkSet->get_attribute_name_al();
            string linkName = *(((String*)(&tmp))->getptr());
            
            //basic link, head link, next link
            int pos = linkName.find("_Link");
            if(linkName.find("_Link") == -1 ||
               linkName.find("Head_") == 0  ||
               linkName.find("Next_") == 0)
            {
                Link* linkPtr = (Link*)(linkSet->get_attribute_ref_al(linkName));
                string type = linkPtr->getTargetName();
                
                //add data member
                classPtr->addDataMember(type+"*", linkName, "public");
                
                //add basic member function
                paraList.clear();
                paraList.push_back(pair<string, string>(type+"*", "var"));
                classPtr->addMemberFunction("set", "void", linkName, paraList);
                
                paraList.clear();
                classPtr->addMemberFunction("get", type+"*", linkName, paraList);
            }
            
            //list link
            pos = linkName.find("List_");
            if(pos == 0){
                Link* linkPtr = (Link*)(linkSet->get_attribute_ref_al(linkName));
                string type = linkPtr->getTargetName();
                
                //add data member
                classPtr->addDataMember("list<"+type+">", linkName, "public");
                
                //add basic member function
                paraList.clear();
                paraList.push_back(pair<string, string>(type, "var"));
                classPtr->addMemberFunction("insert", "void", linkName, paraList);
                classPtr->addMemberFunction("remove", "void", linkName, paraList);
                
                paraList.clear();
                classPtr->addMemberFunction("get", "list<"+type+">", linkName, paraList);
            }
        }
        physicalERDMap->insert(nameTmp, classPtr);
    }
    
    //foreach relationship transfer to class
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        universal_data nameTmp = relationshipTable->key();
        
        TransformedRelationship* relationshipPtr = (TransformedRelationship*)(relationshipTable->value());
        physicalERD* classPtr = new physicalERD(*(((String*)(&nameTmp))->getptr()));
        
        Attribute_List* linkSet = (Attribute_List*)(relationshipPtr->get_attribute_ref_al("Link_set"));
        
        //there is no link in this relationship, so do not transfer to class
        if(linkSet->size() == 0){
            continue;
        }
        
        for(linkSet->begin();!linkSet->end();(*linkSet)++){
            universal_data tmp = linkSet->get_attribute_name_al();
            string linkName = *(((String*)(&tmp))->getptr());
            
            //basic link, head link, next link
            int pos = linkName.find("_Link");
            if(linkName.find("_Link") == -1 ||
               linkName.find("Head_") == 0  ||
               linkName.find("Next_") == 0)
            {
                Link* linkPtr = (Link*)(linkSet->get_attribute_ref_al(linkName));
                string type = linkPtr->getTargetName();
                
                //add data member
                classPtr->addDataMember(type+"*", linkName, "public");
                
                //add basic member function
                paraList.clear();
                paraList.push_back(pair<string, string>(type+"*", "var"));
                classPtr->addMemberFunction("set", "void", linkName, paraList);
                
                paraList.clear();
                classPtr->addMemberFunction("get", type+"*", linkName, paraList);
            }
            
            //list link
            pos = linkName.find("List_");
            if(pos == 0){
                Link* linkPtr = (Link*)(linkSet->get_attribute_ref_al(linkName));
                string type = linkPtr->getTargetName();
                
                //add data member
                classPtr->addDataMember("list<"+type+">", linkName, "public");
                
                //add basic member function
                paraList.clear();
                paraList.push_back(pair<string, string>(type, "var"));
                classPtr->addMemberFunction("insert", "void", linkName, paraList);
                classPtr->addMemberFunction("remove", "void", linkName, paraList);
                
                paraList.clear();
                classPtr->addMemberFunction("get", "list<"+type+">", linkName, paraList);
            }
        }
        physicalERDMap->insert(nameTmp, classPtr);
    }
}

physicalERD::physicalERD(string name)
{
    setClassName(name);
}

physicalERD::~physicalERD()
{
    
}

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
                                    list<pair<string, string> > plist,
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
                                    list<pair<string, string> > plist)
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
        
        node* leftSide = new node();
        leftSide->set_t_nt_flag('t');
        leftSide->set_construct_name("var");
        stmp.set_value(suffix);
        leftSide->add_node_attribute("var", stmp);
        
        node* rightSide = new node();
        rightSide->set_t_nt_flag('t');
        rightSide->set_construct_name("var");
        stmp.set_value(tmpIter->second);
        rightSide->add_node_attribute("var", stmp);
        
        node* assignStatement = new node();
        assignStatement->set_t_nt_flag('n');
        assignStatement->set_construct_name("assignment_statement");
        stmp.set_value("=");
        assignStatement->add_node_attribute("assign_op", stmp);
        assignStatement->push_node_branch(leftSide);
        leftSide->push_parent_pointer(assignStatement);
        assignStatement->push_node_branch(rightSide);
        rightSide->push_parent_pointer(assignStatement);
        
        node* content = new node();
        content->set_t_nt_flag('n');
        content->set_construct_name("statement_sequence");
        content->push_node_branch(assignStatement);
        assignStatement->push_parent_pointer(content);
        
        tmpFunction.functionBody = content;
    }else if(prefix == "get") {
        node* returnValue = new node();
        returnValue->set_t_nt_flag('t');
        returnValue->set_construct_name("var");
        stmp.set_value(suffix);
        returnValue->add_node_attribute("var", stmp);
        
        node* reCall = new node();
        reCall->set_t_nt_flag('n');
        reCall->set_construct_name("return_process");
        reCall->push_node_branch(returnValue);
        returnValue->push_parent_pointer(reCall);
        
        node* content = new node();
        content->set_t_nt_flag('n');
        content->set_construct_name("statement_sequence");
        content->push_node_branch(reCall);
        reCall->push_parent_pointer(content);
        
        tmpFunction.functionBody = content;
    }else if(prefix == "insert"){
        list<pair<string, string> >::iterator tmpIter;
        
        tmpIter = plist.begin();
        
        node* obj = new node();
        obj->set_t_nt_flag('t');
        obj->set_construct_name("var");
        stmp.set_value(suffix);
        obj->add_node_attribute("var", stmp);
        
        node* objectCallFunc = new node();
        objectCallFunc->set_t_nt_flag('n');
        objectCallFunc->set_construct_name("object_call_function");
        
        node* functionName = new node();
        functionName->set_t_nt_flag('t');
        functionName->set_construct_name("function_name");
        stmp.set_value("push_back");
        functionName->add_node_attribute("function_name", stmp);
        
        node* plist = new node();
        plist->set_t_nt_flag('n');
        plist->set_construct_name("parameter_list");
        
        list<string> pars;
        pars.push_back(tmpIter->second);
        list<string>::iterator iter;
        for(iter = pars.begin();iter != pars.end();iter++){
            node* p = new node();
            p->set_t_nt_flag('t');
            p->set_construct_name("var");
            stmp.set_value(*iter);
            obj->add_node_attribute("var", stmp);
            
            plist->push_node_branch(p);
            p->push_parent_pointer(plist);
        }
        objectCallFunc->push_node_branch(obj);
        obj->push_parent_pointer(objectCallFunc);

        objectCallFunc->push_node_branch(functionName);
        functionName->push_parent_pointer(objectCallFunc);

        objectCallFunc->push_node_branch(plist);
        plist->push_parent_pointer(objectCallFunc);
        
        node* content = new node();
        content->set_t_nt_flag('n');
        content->set_construct_name("statement_sequence");
        
        content->push_node_branch(objectCallFunc);
        objectCallFunc->push_parent_pointer(content);
        
        tmpFunction.functionBody = content;
    }else if(prefix == "remove"){
        list<pair<string, string> >::iterator tmpIter;
        
        tmpIter = plist.begin();
        
//        node* obj = ptree_create_t_node("var") ;
//        ptree_add_attribute(obj, "var", elem) ;
        node* obj = new node();
        obj->set_t_nt_flag('t');
        obj->set_construct_name("var");
        stmp.set_value(suffix);
        obj->add_node_attribute("var", stmp);
        
        node* objectCallFunc = new node();
        objectCallFunc->set_t_nt_flag('n');
        objectCallFunc->set_construct_name("object_call_function");
        
        node* functionName = new node();
        functionName->set_t_nt_flag('t');
        functionName->set_construct_name("function_name");
        stmp.set_value("remove");
        functionName->add_node_attribute("function_name", stmp);
        
        node* plist = new node();
        plist->set_t_nt_flag('n');
        plist->set_construct_name("parameter_list");
        
        list<string> pars;
        pars.push_back(tmpIter->second);
        list<string>::iterator iter;
        for(iter = pars.begin();iter != pars.end();iter++){
            node* p = new node();
            p->set_t_nt_flag('t');
            p->set_construct_name("var");
            stmp.set_value(*iter);
            obj->add_node_attribute("var", stmp);
            
            plist->push_node_branch(p);
            p->push_parent_pointer(plist);
        }
        
        objectCallFunc->push_node_branch(obj);
        obj->push_parent_pointer(objectCallFunc);
        
        objectCallFunc->push_node_branch(functionName);
        functionName->push_parent_pointer(objectCallFunc);
        
        objectCallFunc->push_node_branch(plist);
        plist->push_parent_pointer(objectCallFunc);
        
        node* content = new node();
        content->set_t_nt_flag('n');
        content->set_construct_name("statement_sequence");
        
        content->push_node_branch(objectCallFunc);
        objectCallFunc->push_parent_pointer(content);
        
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