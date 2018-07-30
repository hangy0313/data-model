#ifndef DATA_ACCESS_MODEL_H
#define DATA_ACCESS_MODEL_H

#include "system_lib.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

list<node*> funcCallList;
list<string> varList;

map<string, UD_universal_data> importDataModelAccess(string dataModelInfo);

//exist(Ename, attr)
bool exist(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list);
//isNavigat(Ename, Rname)
bool isNavigat(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list);
//isEqual(Rname, "embedding", "N/A")
bool isEqual(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list);
//determineLink(Eename, Rname, prefix)
bool determineLinkOneStep(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list);
//determineLink(Eename, Rname, prefix)
bool determineLink(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list);
//determineLinkFromR(Ename, Rname, prefix)
bool determineLinkFromR(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list);

//transfer data access pattern to parse tree
node* createParseTree(node* nodePtr, string funcList);

void findFunctionCallNode(node* nodePtr);
void findVar(node* nodePtr);

//find the real function name in data model
bool checkLibraryFunction(string funcName);
node* transToRealCode(string funcName, string funcList);

//library function list
node* pointer();
node* list_taget_type(string funcList);
node* link_one_step(string funcList);
node* link_step_one(string funcList);
node* list_link_step_two(string funcList);

//parse tree composition
node* CLM_C_code(list<node*> content) ;

node* CLM_data_declaration(string type, string name) ;

node* CLM_assignment_statement(string var, node* exp);
node* CLM_assignment_statement(string var, int val);
node* CLM_assignment_statement(node* var, node* exp);
node* CLM_assignment_statement(string var, string val);
node* CLM_binary_expression(string lhs, string op, string rhs);
node* CLM_binary_expression(node* lhs, string op, node* rhs);

node* CLM_function_call(string fname, list<string> par_list) ;
node* CLM_statement_sequence(list<node*> stmt) ;
node* CLM_member_function_call(string obj, string fname, list<string> par_list);

#endif