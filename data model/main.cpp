//
//  main.cpp
//  data model
//
//  Created by hangy on 2018/5/31.
//  Copyright (c) 2018年 hangy. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "./data_model_lib/dataModelLib.h"
#include "./output_generation_lib/output_generation_lib.h"

using namespace std;

void show_universal_data(universal_data ud)
{
    if(ud.get_type_tag() == T_int)
        cout<< *(((Int*)(&ud))->getptr()) ;
    else if (ud.get_type_tag() == T_char)
        cout<< *(((Char*)(&ud))->getptr()) ;
    else if (ud.get_type_tag() == T_string)
        cout<< *(((String*)(&ud))->getptr()) ;
    else if (ud.get_type_tag() == T_float)
        cout<< *(((Float*)(&ud))->getptr()) ;
    else if (ud.get_type_tag() == T_double)
        cout<< *(((Double*)(&ud))->getptr()) ;
    else 
        cout<<"unknown" ;
}

void print_parse_tree_mini(node* nodeptr)
{
    if( nodeptr == NULL)
    {
        cout<<"node:: empty subrule"<<endl ;
        return ;
    }
    cout<<"node construct_name: "<< nodeptr->get_construct_name()<<endl ;
    cout<<"attribute: "<<endl ;
    Associate_List_Iterator* aiter = (Associate_List_Iterator*)(nodeptr->get_node_attribute_iter()) ;
    for(aiter->begin() ; !aiter->end(); aiter->advance())
    {
        if(aiter->first() != "NoName")
        {
            cout<<"attr: "<<aiter->first()<<endl ;
            cout<<"value: "; show_universal_data(aiter->second()) ;
            cout<<endl ;
        }
    }
    cout<<endl ;
    
    cout<<"binding: "<<endl ;
    for(nodeptr->binding_info_begin() ; !nodeptr->binding_info_end() ; nodeptr->binding_info_advance())
        cout<<"kind: "<<nodeptr->binding_info_kind()<<"name: "<<nodeptr->binding_info_name()<<endl ;
    
    cout<<"child: {// "<<endl ;
    
    Virtual_Iterator* biter = nodeptr->get_node_branch_iter() ;
    for(biter->begin() ; !biter->end() ; biter->advance())
    {
        node* tmp = (node*)(*(*biter)) ;
        print_parse_tree_mini( tmp ) ;
    }
    
    cout<<" //}"<<endl ;
}

int main()
{
    //    /*
    //     *  test importCardinalityInfo()
    //     */
    //    list<cardinalityRole> tmpRoleList = importCardinalityInfo();
    //    list<cardinalityRole>::iterator iter;
    //
    //    for(iter = tmpRoleList.begin();iter != tmpRoleList.end();iter++){
    //        cout << (*iter).getRoleName() << endl;
    //        cout << (*iter).getCardinality().minNum << endl;
    //        cout << (*iter).getCardinality().maxNum << endl;
    //    }
    ERD* tmpERD = new ERD("test");
    TransformedERD* trnasERD = new TransformedERD(tmpERD->getERDName());
    Map* record = new Map();
    Map* physicalERDMap = new Map();
    
    Entity* e1 = new Entity("E1");
    e1->addAttribute("test1", "int");
    Entity* e2 = new Entity("E2");
    e2->addAttribute("test2", "string");
    Entity* e3 = new Entity("E3");
    e3->addAttribute("test3", "char");
    //
    Relationship* r1 = new Relationship("R1");
    //
    r1->addRole("role1", "E1");
    r1->addRole("role2", "E2");
    r1->addRole("role3", "E3");
    
    tmpERD->addEntity(e1);
    tmpERD->addEntity(e2);
    tmpERD->addEntity(e3);
    tmpERD->addRelationship(r1);
    
//    tmpERD->dump();
    
    addCardinalityToERD(tmpERD);
//    dumpCardinalityERD(tmpERD);

    addNavigationToERD(tmpERD);
//    dumpNavigationERD(tmpERD);
    
    record = transferToBinary(tmpERD);
//    dumpNavigationERD(tmpERD);
    
    //print record table which record mapping of n-ary relationship and binary relationship
//    cout << endl << "===============" << endl;
//    for(record->begin();!record->end();(*record)++){
//        RelationshipRecord* tmpRecord = (RelationshipRecord*)(record->value());
//        tmpRecord->dump();
//    }
    
    directionDegeneration(tmpERD);
    dumpNavigationERD(tmpERD);
    
    embedding(tmpERD, trnasERD);
    trnasERD->dump();
    
//    string test = "Head_Link_E2_RELATIONSHIP_role2";
//    int pos = test.find("Head_");
//    cout << pos << endl;
    tansToPhysicalModel(trnasERD, physicalERDMap);
    
//    for(physicalERDMap->begin();!physicalERDMap->end();(*physicalERDMap)++){
//        physicalERD* classPtr = (physicalERD*)(physicalERDMap->value());
//        cout << "------->" << classPtr->getClassName() << endl;
//        
//        list<memberFunctionSchema> functionList = classPtr->getAllMemberFunction();
//        list<memberFunctionSchema>::iterator functionPtr;
//        for(functionPtr = functionList.begin();functionPtr != functionList.end();functionPtr++){
//            node* functionBody = (*functionPtr).functionBody;
//            string returnType = (*functionPtr).returnType;
//            string functionName = (*functionPtr).functionName;
//            list<pair<string, string> > parameters = (*functionPtr).parameters;
//            list<pair<string, string> >::iterator paramPtr;
//            
////            member_function_info mfi = mfs_iter->second ;
//            cout << "==>" << returnType << " "<< functionName << " (";
////            list<pair<string, string> >::iterator piter ;
//            for(paramPtr = parameters.begin();paramPtr != parameters.end();paramPtr++)
//                cout << (*paramPtr).first << " " << (*paramPtr).second << " ,";
//            cout << ") {" << endl;
//            print_parse_tree_mini(functionBody);
//            cout << "    }"<<endl;
//        }
//    }
//    macro_definition_input_procedure("print_func", "./result.txt") ;
    
    node* ptree = transToParseTree(physicalERDMap);
    
    general_output_generation gog("data_model_lib/physical_model_cpp_language/node_print_rule_file.txt",
                                  "data_model_lib/physical_model_cpp_language/global_print_table_file.txt",
                                  "data_model_lib/physical_model_cpp_language/interleave_vector_file.txt",
                                  "data_model_lib/physical_model_cpp_language/enclose_vector_file.txt",
                                  "data_model_lib/physical_model_cpp_language/file_root_file.txt",
                                  "data_model_lib/physical_model_cpp_language/indent_control_table_file.txt",
                                  "data_model_lib/physical_model_cpp_language/condition_function_table_file.txt") ;
    // set precedence
    initialized_precedence_table("data_model_lib/physical_model_cpp_language/precedence_file.txt") ;
    // set up node print function
    string filename = "data_model_lib/physical_model_cpp_language/node_print_function_file.txt" ;
    ifstream infile(filename.c_str());
    if(!infile)
    {
        cout<<"open file fail "<<filename<<endl;
        exit(-1) ;
    }
    
    string construct_name;
    string tmp1, tmp2;
    while(infile>>construct_name )
    {
        infile>>tmp1 ;
        infile>>tmp2 ;
        if (tmp1 != "default" || tmp2 != "default")
        {
            cout<<"different than default"<<endl ;
            exit(-1) ;
        }
        gog.set_node_print_function(construct_name, print_node_info_func, print_node_info_func);
    }
    //set condition function
    gog.set_condition_fptr("less_than", less_than);
    
    // invoke output shaping
    gog.output_generation(ptree);
    cout<<"output generation	=> finish"<<endl;
    
    gog.output_file(true);
//    cout<<"output file finish	=> finish"<<endl;
    
    return 0;
}