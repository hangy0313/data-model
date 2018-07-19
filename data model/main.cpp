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
//test 1:1 binary
    Entity* e1 = new Entity("Customer");
    
    Entity* e2 = new Entity("Product");
    

    
    Relationship* r1 = new Relationship("Order");
    r1->addRole("Cus", "Customer");
    r1->addRole("Pro", "Product");

    
    tmpERD->addEntity(e1);
    tmpERD->addEntity(e2);
    tmpERD->addRelationship(r1);
    
    
//    tmpERD->dump();
    
    addCardinalityToERD(tmpERD);
//    dumpCardinalityERD(tmpERD);

    addNavigationToERD(tmpERD);
//    dumpNavigationERD(tmpERD);
    
    record = transferToBinary(tmpERD);

    directionDegeneration(tmpERD);
//    dumpNavigationERD(tmpERD);

    embedding(tmpERD, trnasERD);
//    trnasERD->dump();
//
    tansToPhysicalModel(trnasERD, physicalERDMap);
    
    node* ptree = transToParseTree(physicalERDMap);
    
    outputRealCode(ptree);
    
    return 0;
}