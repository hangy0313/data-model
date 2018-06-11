#include "parseERD.h"

using namespace std;

node* transToParseTree(Map* physicalERDMap)
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
        
        String stmp ;
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

        String stmp;
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
            String stmp;
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
            String stmp;
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

//void setup_token_macro_def(string filename, generalized_parser& parser)
//{
//    ifstream infile ;
//    infile.open(filename.c_str()) ;
//    if(!infile)
//    {
//        cout<<"open file fail"<<filename<<endl ;
//        exit(-1) ;
//    }
//    
//    string type, word, word_tmp ;
//    int id, value ;
//    while(infile>>type)
//    {
//        if(type == "keyword")
//        {
//            infile>>word ;
//            id = parser.get_symbol_index(word) ;
//            universal_data empty ;
//            parser.add_token(id, empty) ;
//        }
//        else if(type == "type")
//        {
//            infile>>word ;
//            id = parser.get_symbol_index(type) ;
//            String tmp ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//        }
//        else if(type == "string_value")
//        {
//            infile>>word ;
//            
//            id = parser.get_symbol_index("value") ;
//            String tmp ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//        }
//        else if (type == "value")
//        {
//            infile>>value ;
//            id = parser.get_symbol_index(type) ;
//            Int tmp ;
//            tmp.set_value(value) ;
//            parser.add_token(id, tmp) ;
//        }
//        else if (type == "binary_op")
//        {
//            infile>>word ;
//            id = parser.get_symbol_index(type) ;
//            String tmp ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//        }
//        else if (type == "assign_op")
//        {
//            infile>>word ;
//            id = parser.get_symbol_index(type) ;
//            String tmp ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//        }
//        else if(type == "function_name")
//        {
//            infile>>word ;
//            word.erase(word.end()-1) ;
//            id = parser.get_symbol_index(type) ;
//            String tmp ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//            
//            id = parser.get_symbol_index("(") ;
//            universal_data empty ;
//            parser.add_token(id, empty) ;
//        }
//        else if (type == "unary_op_f")
//        {
//            infile>>word ;
//            word.erase(word.begin()) ;
//            word.erase(word.begin()) ;
//            
//            String tmp ;
//            id = parser.get_symbol_index("unary_op") ;
//            tmp.set_value("++") ;
//            parser.add_token(id, tmp) ;
//            
//            id = parser.get_symbol_index("var") ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//        }
//        else if (type == "unary_op_b")
//        {
//            infile>>word ;
//            word.erase(word.end()-1) ;
//            word.erase(word.end()-1) ;
//            
//            String tmp ;
//            id = parser.get_symbol_index("var") ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//            
//            id = parser.get_symbol_index("unary_op") ;
//            tmp.set_value("++") ;
//            parser.add_token(id, tmp) ;
//        }
//        else if (type == "unary_op_f2")
//        {
//            infile>>word ;
//            word.erase(word.begin()) ;
//            
//            String tmp ;
//            id = parser.get_symbol_index("unary_op") ;
//            tmp.set_value("*") ;
//            parser.add_token(id, tmp) ;
//            
//            id = parser.get_symbol_index("var") ;
//            tmp.set_value(word) ;
//            parser.add_token(id, tmp) ;
//        }
//        else
//        {
//            cout<<"unknown type: "<<type<<endl ;
//            exit(-1) ;
//        }
//    }
//}
//
//void macro_definition_input_procedure(string macro_name, string filename)
//{
//    map<string, node*>::iterator iter ;
//    iter = macro_definition_table.find(macro_name) ;
//    if(iter != macro_definition_table.end())
//    {
//        cout<<"macro deinition is existed: "<<macro_name<<endl ;
//        exit(-1) ;
//    }
//    
//    generalized_parser parser("data_model_lib/physical_model_macro_definition/grammar_rule_file.txt",
//                              "data_model_lib/physical_model_macro_definition/node_creation_rule_file.txt",
//                              "data_model_lib/physical_model_macro_definition/associativity_file.txt",
//                              "data_model_lib/physical_model_macro_definition/precedence_file.txt",
//                              "data_model_lib/physical_model_macro_definition/general_construct_file.txt",
//                              "data_model_lib/physical_model_macro_definition/action_file.txt",
//                              "data_model_lib/physical_model_macro_definition/binding_file.txt") ;
//    
//    
//    setup_token_macro_def(filename, parser) ;
//    
//    // show token
//    /*
//     cout<<"======show token=========="<<endl;
//     for(int i = 0 ; i < parser.token_size() ; i++)
//     {
//     token* tmp = parser.get_token(i) ;
//     cout<<"i = "<<i ;
//     cout<<"token id: "<<tmp->token_id ; 
//     cout<<"token specifier: "  ;
//     if(tmp->token_specifier.get_type_tag() != T_unknown)
//     show_universal_data(tmp->token_specifier) ;
//     cout<<endl ;
//     }
//     cout<<"========================="<<endl ;
//     */
//    bool finish ; 
//    int new_index = 0 ;
//    int current_index = 0 ;
//    int construct_id = parser.get_symbol_index("function_definition") ;
//    
//    list<pair<string, unsigned*> > acc_vector;
//    acc_vector.clear() ;
//    
//    node* ptptr = NULL ;
//    
//    finish = parser.shaped_unified_parse(construct_id, current_index, new_index, acc_vector, ptptr) ;
//    
//    if(finish)
//        cout<<"match succeess "<<new_index<<"	/	"<<parser.token_size()<<endl ;
//    else 
//        cout<<"match fail"<<endl ;
//    
//    if(finish)
//    {
//        node* nodeptr = NULL ;
//        if(acc_vector.begin()->first == "node")
//            nodeptr = (node*)(acc_vector.begin()->second) ;
//        
//        if(nodeptr == NULL)
//            exit(-1) ;
//        
//        // add definition in macro definition table 
//        macro_definition_table.insert(pair<string, node*>( macro_name, nodeptr) ) ;
//    }
//    else
//        exit(-1) ;
//}