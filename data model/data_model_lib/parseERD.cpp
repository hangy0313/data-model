#include "parseERD.h"

using namespace std;

node* transToParseTree(UD_Map* physicalERDMap)
{
    list<node*> ptree;
    
    //class declaraction
    for(physicalERDMap->begin();!physicalERDMap->end();(*physicalERDMap)++){
        physicalERD* classPtr = (physicalERD*)(physicalERDMap->value());
        
        node* root = new node();
        root->set_t_nt_flag('n');
        root->set_construct_name("class_declaration");

        node* classContent = new node();
        classContent->set_t_nt_flag('n');
        classContent->set_construct_name("class_content");
        
        UD_String stmp ;
        stmp.set_value(classPtr->getClassName()) ;
        classContent->add_node_attribute("class_name", stmp) ;

        root->push_node_branch(classContent);
        classContent->push_parent_pointer(root);
        
        ptree.push_back(root);
    }
    
    //class definition
    for(physicalERDMap->begin();!physicalERDMap->end();(*physicalERDMap)++){
        physicalERD* classPtr = (physicalERD*)(physicalERDMap->value());
        
        node* root = new node();
        root->set_t_nt_flag('n');
        root->set_construct_name("class_declaration");
        
        node* classContent = new node();
        classContent->set_t_nt_flag('n');
        classContent->set_construct_name("class_content");

        UD_String stmp;
        stmp.set_value(classPtr->getClassName());
        classContent->add_node_attribute("class_name", stmp);
        
        // data member
        node* dataMemberSequence = new node();
        dataMemberSequence->set_t_nt_flag('n');
        dataMemberSequence->set_construct_name("data_member_sequence");

        list<dataMemberSchema> dataList = classPtr->getAllDataMember();
        list<dataMemberSchema>::iterator dataListPtr;
        for(dataListPtr = dataList.begin();dataListPtr != dataList.end();dataListPtr++){
            node* dataMember = new node();
            dataMember->set_t_nt_flag('n');
            dataMember->set_construct_name("data_member");
            
            node* type = new node();
            type->set_t_nt_flag('t');
            type->set_construct_name("type");
            UD_String stmp;
            stmp.set_value((*dataListPtr).type);
            type->add_node_attribute("type", stmp);
            
            node* name = new node();
            name->set_t_nt_flag('t');
            name->set_construct_name("var");
            stmp.set_value((*dataListPtr).name);
            name->add_node_attribute("var", stmp);
            
            dataMember->push_node_branch(type);
            type->push_parent_pointer(dataMember);
            
            dataMember->push_node_branch(name);
            name->push_parent_pointer(dataMember);
            
            dataMemberSequence->push_node_branch(dataMember);
            dataMember->push_parent_pointer(dataMember);
        }
        
        // member funciton
        node* memberFunctionSeq = new node();
        memberFunctionSeq->set_t_nt_flag('n');
        memberFunctionSeq->set_construct_name("member_function_sequence");
        
        list<memberFunctionSchema> functionList = classPtr->getAllMemberFunction();
        list<memberFunctionSchema>::iterator functionListPtr;
        for(functionListPtr = functionList.begin();functionListPtr != functionList.end();functionListPtr++){
            node* memberFunction = new node();
            memberFunction->set_t_nt_flag('n');
            memberFunction->set_construct_name("member_function");
            
            node* rtype = new node();
            rtype->set_t_nt_flag('t');
            rtype->set_construct_name("type");
            UD_String stmp;
            stmp.set_value((*functionListPtr).returnType);
            rtype->add_node_attribute("type", stmp);
            
            node* funcName = new node();
            funcName->set_t_nt_flag('t');
            funcName->set_construct_name("function_name");
            stmp.set_value((*functionListPtr).functionName);
            funcName->add_node_attribute("function_name", stmp);
            
            node* pars = new node();
            pars->set_t_nt_flag('t');
            pars->set_construct_name("parameter_list");
            
            list<pair<string, string> >::iterator pIter;
            for(pIter = (*functionListPtr).parameters.begin();pIter != (*functionListPtr).parameters.end();pIter++){
                node* par = new node();
                par->set_t_nt_flag('n');
                par->set_construct_name("par");
                
                node* t = new node();
                t->set_t_nt_flag('t');
                t->set_construct_name("type");
                stmp.set_value((*pIter).first);
                t->add_node_attribute("type", stmp);
                
                node* v = new node();
                v->set_t_nt_flag('t');
                v->set_construct_name("var");
                stmp.set_value((*pIter).second);
                v->add_node_attribute("var", stmp);
                
                par->push_node_branch(t);
                t->push_parent_pointer(par);
                
                par->push_node_branch(v);
                v->push_parent_pointer(par);
                
                pars->push_node_branch(par);
                par->push_parent_pointer(pars);
            }
            
            node* block = new node();
            block->set_t_nt_flag('n');
            block->set_construct_name("block");
            
            if((*functionListPtr).functionBody != NULL){
                node* body = (*functionListPtr).functionBody;
                
                block->push_node_branch(body);
                body->push_parent_pointer(block);
            }
            
            memberFunction->push_node_branch(rtype);
            rtype->push_parent_pointer(memberFunction);
            
            memberFunction->push_node_branch(funcName);
            funcName->push_parent_pointer(memberFunction);
            
            memberFunction->push_node_branch(pars);
            pars->push_parent_pointer(memberFunction);
            
            memberFunction->push_node_branch(block);
            block->push_parent_pointer(memberFunction);
            
            memberFunctionSeq->push_node_branch(memberFunction);
            memberFunction->push_parent_pointer(memberFunctionSeq);
        }
        classContent->push_node_branch(dataMemberSequence);
        dataMemberSequence->push_parent_pointer(classContent);
        
        classContent->push_node_branch(memberFunctionSeq);
        memberFunctionSeq->push_parent_pointer(classContent);

        root->push_node_branch(classContent);
        classContent->push_parent_pointer(root);
        
        ptree.push_back(root);
    }
    
    node* nt = new node();
    nt->set_t_nt_flag('n');
    nt->set_construct_name("C++");
    
    list<node*>::iterator iter ;
    for(iter = ptree.begin();iter != ptree.end();iter++){
        nt->push_node_branch(*iter);
        (*iter)->push_parent_pointer(nt);
    }
    
    return nt;
}