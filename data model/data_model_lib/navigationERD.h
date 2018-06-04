#ifndef NAVIGATION_ERD_H
#define NAVIGATION_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
 *  import navigation info
 */
Attribute_List* importNavigationInfo();

/*
 *  Role add data member : Navigation => "to_entity", "to_relationship", "bidirectional"
 */
void addNavigationToERD(ERD* erd);

/*
 *  Dump ERD info after add navigation
 */
void dumpNavigationERD(ERD* erd);

#endif