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

int main()
{
    ERD* tmpERD = import("./experiment_2/erdScript/erd.txt");
//    tmpERD->dump();

    addCardinalityToERD(tmpERD, "./experiment_2/erdScript/cardinality.txt");
//    dumpCardinalityERD(tmpERD);

    addNavigationToERD(tmpERD, "./experiment_2/erdScript/navigation.txt");
//    dumpNavigationERD(tmpERD);

    UD_Map* record = transferToBinary(tmpERD);

    directionDegeneration(tmpERD, "./experiment_2/decision/direction_degeneration.txt");
//    dumpNavigationERD(tmpERD);

    speicalizedERD(tmpERD, "./experiment_2/decision/embedding.txt", "./experiment_2/data_access_trans1.txt");
    TransformedERD* transERD = embedding(tmpERD, "./experiment_2/decision/embedding.txt");
    logicalERD(transERD, "./experiment_2/data_access_trans2.txt");
    
//    transERD->dump();

    UD_Map* physicalERDMap = tansToPhysicalModel(transERD);
    outputFunctionList(physicalERDMap, "./experiment_2/function_list.txt");

    node* ptree = transToParseTree(physicalERDMap);
    
    //output real code
    general_output_generation gog("data_model_lib/physical_model_cpp_language/node_print_rule_file.txt",
                                  "data_model_lib/physical_model_cpp_language/global_print_table_file.txt",
                                  "data_model_lib/physical_model_cpp_language/interleave_vector_file.txt",
                                  "data_model_lib/physical_model_cpp_language/enclose_vector_file.txt",
                                  "data_model_lib/physical_model_cpp_language/file_root_file.txt",
                                  "data_model_lib/physical_model_cpp_language/indent_control_table_file.txt",
                                  "data_model_lib/physical_model_cpp_language/condition_function_table_file.txt") ;
    
    initialized_precedence_table("data_model_lib/physical_model_cpp_language/precedence_file.txt") ;
    
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
    gog.set_condition_fptr("less_than", less_than);
    
    gog.output_generation(ptree);
    
    gog.output_file(true);
    
    return 0;
}