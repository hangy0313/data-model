#ifndef CONTROL_FLOW_H
#define CONTROL_FLOW_H

#include "../universal_data_lib/universal_data_lib.h"
#include "../output_generation_lib/output_generation_lib.h"
#include <iostream>
#include <fstream>

using namespace std;

class control_flow_model;
class control_block;
class for_loop_block;
class while_loop_block;
class switch_block;
class if_then_else_block;
class function_call_block;

class control_flow_model:public Attribute_List
{
protected:
    int controlNum;
public:
    control_flow_model();
    ~control_flow_model();
    
    void addControlBlock(control_block* controlBlock);
    control_block* getControlBlock(string name);
};

class control_block:public Attribute_List
{
public:
    control_block();
    ~control_block();
    
    void setLabel(string label);
    string getLabel();
};

class for_loop_block:public control_block
{
public:
    for_loop_block();
    ~for_loop_block();
    
    void setInitial(node* initial);
    node* getInitial();
    void setCondition(node* condition);
    node* getCondition();
    void setIteration(node* iteration);
    node* getIteration();
    void setBody(node* body);
    node* getBody();
};

class while_loop_block:public control_block
{
public:
    
    while_loop_block();
    ~while_loop_block();
    
    void setCondition(node* condition);
    node* getCondition();
    void setBody(node* body);
    node* getBody();
};

class if_else_block:public control_block
{
public:
    if_else_block();
    ~if_else_block();
    
    void setCondition(node* condition);
    node* getCondition();
    void setIfBody(node* ifBody);
    node* getIfBody();
    void setElseBody(node* elseBody);
    node* getElseBody();
};


Attribute_List* pattern_table = new Attribute_List();

void initialPatternTable();
Map* findPattern(string pName);

void importControlModel(control_flow_model* cfm);
node* createParseTree(control_flow_model* cfm);

//parse tree composition
node* CLM_C_code(list<node*> content) ;

node* CLM_data_declaration(string type, string name) ;

node* CLM_assignment_statement(string var, int val);
node* CLM_assignment_statement(node* var, node* exp);
node* CLM_assignment_statement(string var, string val);
node* CLM_binary_expression(string lhs, string op, string rhs);

node* CLM_for_loop(node* init, node* cond, node* next, node* body) ;
node* CLM_if_else_statement(node* cond, node* body1, node* body2) ;
node* CLM_while_loop(node* cond, node* body) ;
node* CLM_function_call(string fname, list<string> par_list) ;
node* CLM_statement_sequence(list<node*> stmt) ;

#endif