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
UD_Attribute_List* importNavigationInfo(string naviScript);

/*
 *  Role add data member : Navigation => "to_entity", "to_relationship", "bidirectional"
 */
void addNavigationToERD(ERD* erd, string naviScript);

/*
 *  Dump ERD info after add navigation
 */
void dumpNavigationERD(ERD* erd);

#endif