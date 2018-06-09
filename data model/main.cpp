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

using namespace std;

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
    Map *record = new Map();
    
    Entity* e1 = new Entity("E1");
    e1->addAttribute("test1", "int");
    Entity* e2 = new Entity("E2");
    e2->addAttribute("test2", "string");
    Entity* e3 = new Entity("E3");
    e2->addAttribute("test3", "char");
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
    dumpNavigationERD(tmpERD);
    
    //print record table which record mapping of n-ary relationship and binary relationship
//    cout << endl << "===============" << endl;
//    for(record->begin();!record->end();(*record)++){
//        RelationshipRecord* tmpRecord = (RelationshipRecord*)(record->value());
//        tmpRecord->dump();
//    }
    
    directionDegeneration(tmpERD);
    dumpNavigationERD(tmpERD);
    
    return 0;
}