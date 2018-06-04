#ifndef CARDINALITY_ERD_H
#define CARDINALITY_ERD_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../generalized_parser_lib/generalized_parser_lib.h"
#include <iostream>
#include <fstream>

using namespace std;

struct cardinalitySchema{
    string roleName;
    string minNum;
    string maxNum;
};

/*
 *  import cardinality info
 */
list<cardinalitySchema> importCardinalityInfo();

/*
 *  Role add data member : Cardinality(Minimum, Maximum)
 */
void addCardinalityToERD(ERD* erd);

/*
 *  Dump ERD info after add cardinality
 */
void dumpCardinalityERD(ERD* erd);

#endif