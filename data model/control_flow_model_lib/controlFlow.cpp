#include "controlFlow.h"

using namespace std;

control_flow_model::control_flow_model()
{
    UD_Map controlBlockList;
    controlNum = 1;
    
    this->add_attribute_al("control_block_list", controlBlockList);
}

control_flow_model::~control_flow_model()
{
    
}

void control_flow_model::addControlBlock(control_block* controlBlock)
{
    UD_Map* controlBlockTable = (UD_Map*)(get_attribute_ref_al("control_block_list"));

    UD_Int intTmp;
    intTmp.set_value(controlNum);
    
    controlBlockTable->insert(intTmp, controlBlock);
    
    controlNum++;
}

control_block* control_flow_model::getControlBlock(string name)
{
    UD_String stringTmp;
    stringTmp.set_value(name);
    
    UD_Map* controlBlockTable = (UD_Map*)(get_attribute_ref_al("control_block_list"));
    
    return (control_block*)(controlBlockTable->find(stringTmp));
}

control_block::control_block()
{
    UD_String tmp;
    
    this->add_attribute_al("label", tmp);
}

control_block::~control_block()
{
    
}

void control_block::setLabel(string label)
{
    UD_String tmp;
    tmp.set_value(label);
    this->set_attribute_al("label", tmp);
}

string control_block::getLabel()
{
    UD_universal_data* tmp = this->get_attribute_ref_al("label");
    
    return *((UD_String*)(tmp))->getptr();
}

for_loop_block::for_loop_block()
{
    node tmp;
    
    setLabel("for_loop_block");
    
    this->add_attribute_al("initial", tmp);
    this->add_attribute_al("condition", tmp);
    this->add_attribute_al("iteration", tmp);
    this->add_attribute_al("body", tmp);
}

for_loop_block::~for_loop_block()
{
    
}

void for_loop_block::setInitial(node* initial)
{
    this->set_attribute_al("initial", *initial);
}

node* for_loop_block::getInitial()
{
    node* tmp = (node*)(this->get_attribute_ref_al("initial"));
    
    return tmp;
}

void for_loop_block::setCondition(node* condition)
{
    this->set_attribute_al("condition", *condition);
}

node* for_loop_block::getCondition()
{
    node* tmp = (node*)(this->get_attribute_ref_al("condition"));
    
    return tmp;
}

void for_loop_block::setIteration(node* iteration)
{
    this->set_attribute_al("iteration", *iteration);
}

node* for_loop_block::getIteration()
{
    node* tmp = (node*)(this->get_attribute_ref_al("iteration"));
    
    return tmp;
}

void for_loop_block::setBody(node* body)
{
    this->set_attribute_al("body", *body);
}

node* for_loop_block::getBody()
{
    node* tmp = (node*)(this->get_attribute_ref_al("body"));
    
    return tmp;
}

while_loop_block::while_loop_block()
{
    node tmp;
    
    setLabel("while_loop_block");
    
    this->add_attribute_al("condition", tmp);
    this->add_attribute_al("body", tmp);
}

while_loop_block::~while_loop_block()
{
    
}

void while_loop_block::setCondition(node* condition)
{
    this->set_attribute_al("condition", *condition);
}

node* while_loop_block::getCondition()
{
    node* tmp = (node*)(this->get_attribute_ref_al("condition"));
    
    return tmp;
}

void while_loop_block::setBody(node* body)
{
    this->set_attribute_al("body", *body);
}

node* while_loop_block::getBody()
{
    node* tmp = (node*)(this->get_attribute_ref_al("body"));
    
    return tmp;
}

if_else_block::if_else_block()
{
    node tmp;
    
    setLabel("if_else_block");
    
    this->add_attribute_al("condition", tmp);
    this->add_attribute_al("if_body", tmp);
    this->add_attribute_al("else_body", tmp);
}

if_else_block::~if_else_block()
{
    
}

void if_else_block::setCondition(node* condition)
{
    this->set_attribute_al("condition", *condition);
}

node* if_else_block::getCondition()
{
    node* tmp = (node*)(this->get_attribute_ref_al("condition"));
    
    return tmp;
}

void if_else_block::setIfBody(node* ifBody)
{
    this->set_attribute_al("if_body", *ifBody);
}

node* if_else_block::getIfBody()
{
    node* tmp = (node*)(this->get_attribute_ref_al("if_body"));
    
    return tmp;
}

void if_else_block::setElseBody(node* elseBody)
{
    this->set_attribute_al("else_body", *elseBody);
}

node* if_else_block::getElseBody()
{
    node* tmp = (node*)(this->get_attribute_ref_al("else_body"));
    
    return tmp;
}



node* findPattern(string pName, map<string, node*> pattern_table)
{
    return pattern_table.find(pName)->second;
}

control_flow_model* importControlModel(string controlFlowScript, map<string, node*> pattern_table)
{
    string label;
    control_flow_model* cfm = new control_flow_model();
    ifstream cmInput(controlFlowScript.c_str());
    list<node*> cTree;
    
    while(cmInput >> label){cout << label << endl;
        if(label == "for_loop"){
            string pattern;
            node* init = new node();
            node* condition = new node();
            node* iteration = new node();
            node* body = new node();
            list<node*> bodyList;
            string lop, op, rop;
            
            for_loop_block* flb = new for_loop_block();
            
            cmInput >> pattern;
            while(pattern != "end_for_loop"){
                if(pattern == "initial"){
                    cmInput >> lop;
                    cmInput >> op;
                    cmInput >> rop;
                    init = CLM_binary_expression(lop, op, rop);
                    
                    flb->setInitial(init);
                }
                if(pattern == "condition"){
                    cmInput >> lop;
                    cmInput >> op;
                    cmInput >> rop;
                    condition = CLM_binary_expression(lop, op, rop);
                    
                    flb->setCondition(condition);
                }
                if(pattern == "iteration"){
                    cmInput >> lop;
                    cmInput >> op;
                    cmInput >> rop;
                    iteration = CLM_binary_expression(lop, op, rop);
                    
                    flb->setIteration(iteration);
                }
                if(pattern == "body") {
                    string tmp;
                    cmInput >> tmp;
                    if(tmp == "pattern"){
                        string dataAccess;
                        cmInput >> dataAccess;
                        
                        node* tmp = findPattern(dataAccess, pattern_table);
                        bodyList.push_back(tmp);
                    }else{
                        lop = tmp;
                        cmInput >> op;
                        cmInput >> rop;
                        bodyList.push_back(CLM_binary_expression(lop, op, rop));
                    }
                    body = CLM_statement_sequence(bodyList);
                    
                    flb->setBody(body);
                }
                cmInput >> pattern;
            }
            cfm->addControlBlock(flb);
        }
        
        if(label == "while_loop"){
            string pattern;
            node* condition = new node();
            node* body = new node();
            list<node*> bodyList;
            string lop, op, rop;
            
            while_loop_block* wlb = new while_loop_block();
            
            cmInput >> pattern;
            while(pattern != "end_while_loop"){
                if(pattern == "condition"){
                    cmInput >> lop;
                    cmInput >> op;
                    cmInput >> rop;
                    condition = CLM_binary_expression(lop, op, rop);
                    
                    wlb->setCondition(condition);
                }
                if(pattern == "body") {
                    string tmp;
                    cmInput >> tmp;
                    if(tmp == "pattern"){
                        string dataAccess;
                        cmInput >> dataAccess;
                        
                        node* tmp = findPattern(dataAccess, pattern_table);
                        bodyList.push_back(tmp);
                    }else{
                        lop = tmp;
                        cmInput >> op;
                        cmInput >> rop;
                        bodyList.push_back(CLM_binary_expression(lop, op, rop));
                    }
                    body = CLM_statement_sequence(bodyList);
                    
                    wlb->setBody(body);
                }
                cmInput >> pattern;
            }
            cfm->addControlBlock(wlb);
        }
        
        if(label == "if_else_block"){
            string pattern;
            node* condition = new node();
            node* ifBody = new node();
            node* elseBody = new node();
            list<node*> ifBodyList;
            list<node*> elseBodyList;
            string lop, op, rop;
            
            if_else_block* ieb = new if_else_block();
            
            cmInput >> pattern;
            while(pattern != "end_if_else_block"){
                if(pattern == "condition"){
                    cmInput >> lop;
                    cmInput >> op;
                    cmInput >> rop;
                    condition = CLM_binary_expression(lop, op, rop);
                    
                    ieb->setCondition(condition);
                }
                if(pattern == "if_body") {
                    string tmp;
                    cmInput >> tmp;
                    if(tmp == "pattern"){
                        string dataAccess;
                        cmInput >> dataAccess;
                        
                        node* tmp = findPattern(dataAccess, pattern_table);
                        ifBodyList.push_back(tmp);
                    }else{
                        lop = tmp;
                        cmInput >> op;
                        cmInput >> rop;
                        ifBodyList.push_back(CLM_binary_expression(lop, op, rop));
                    }
                    ifBody = CLM_statement_sequence(ifBodyList);
                    
                    ieb->setIfBody(ifBody);
                }
                if(pattern == "else_body") {
                    string tmp;
                    cmInput >> tmp;
                    if(tmp == "pattern"){
                        string dataAccess;
                        cmInput >> dataAccess;
                        
                        node* tmp = findPattern(dataAccess, pattern_table);
                        elseBodyList.push_back(tmp);
                    }else{
                        lop = tmp;
                        cmInput >> op;
                        cmInput >> rop;
                        elseBodyList.push_back(CLM_binary_expression(lop, op, rop));
                    }
                    elseBody = CLM_statement_sequence(elseBodyList);
                    
                    ieb->setElseBody(ifBody);
                }
                cmInput >> pattern;
            }
            cfm->addControlBlock(ieb);
        }
    }
    
    return cfm;
}

node* createParseTree(control_flow_model* cfm)
{
    list<node*> cTree;
    node* root;
    
    UD_Map* controlBlockTable = (UD_Map*)(cfm->get_attribute_ref_al("control_block_list"));
    for(controlBlockTable->begin();!controlBlockTable->end();(*controlBlockTable)++){
        UD_universal_data* udTmp = controlBlockTable->value();
        control_block* cb = (control_block*)(udTmp);
        
        if(cb->getLabel() == "for_loop_block"){
            for_loop_block* flb = (for_loop_block*)(udTmp);
            
            node* forLoop = CLM_for_loop(flb->getInitial(),
                                         flb->getCondition(),
                                         flb->getIteration(),
                                         flb->getBody());
            
            cTree.push_back(forLoop);
        }
        if(cb->getLabel() == "while_loop_block"){
            while_loop_block* wlb = (while_loop_block*)(udTmp);
            
            node* whileLoop = CLM_while_loop(wlb->getCondition(),
                                             wlb->getBody());
            
            cTree.push_back(whileLoop);
        }
        
        if(cb->getLabel() == "if_else_block"){
            if_else_block* ieb = (if_else_block*)(udTmp);
            
            node* ifElseBlock = CLM_if_else_statement(ieb->getCondition(),
                                                    ieb->getIfBody(),
                                                    ieb->getElseBody());
            
            cTree.push_back(ifElseBlock);
        }
    }
    
    root = CLM_C_code(cTree);
    
    return root;
}

//node* CLM_C_code(list<node*> content)
//{
//    node* c = new node() ;
//    c->set_construct_name("C++") ;
//    c->set_t_nt_flag('n') ;
//    
//    list<node*>::iterator iter ;
//    for(iter = content.begin() ; iter != content.end() ; iter++)
//        c->push_node_branch(*iter) ;
//    
//    return c ;
//}
//
//node* CLM_data_declaration(string type, string name)
//{
//    node* dd = new node() ;
//    dd->set_construct_name("variable_declaration") ;
//    dd->set_t_nt_flag('n') ;
//    
//    node* t = new node() ;
//    t->set_construct_name("type") ;
//    t->set_t_nt_flag('t') ;
//    UD_String type_ud ;
//    type_ud.set_value(type) ;
//    t->add_node_attribute("type", type_ud) ;
//    
//    node* var = new node() ;
//    var->set_construct_name("var") ;
//    var->set_t_nt_flag('t') ;
//    UD_String var_ud ;
//    var_ud.set_value(name) ;
//    var->add_node_attribute("var", var_ud) ;
//    
//    dd->push_node_branch(t) ;
//    dd->push_node_branch(var) ;
//    
//    return dd ;
//}
//
//node* CLM_assignment_statement(string var, int val)
//{
//    node* as = new node() ;
//    as->set_construct_name("assignment_statement") ;
//    as->set_t_nt_flag('n') ;
//    String op_ud ;
//    op_ud.set_value("=") ;
//    as->add_node_attribute("assign_op", op_ud) ;
//    
//    node* v = new node() ;
//    v->set_construct_name("var") ;
//    v->set_t_nt_flag('t') ;
//    String v_ud ;
//    v_ud.set_value(var) ;
//    v->add_node_attribute("var", v_ud) ;
//    
//    node* v2 = new node() ;
//    v2->set_construct_name("value") ;
//    v2->set_t_nt_flag('t') ;
//    Int v2_ud;
//    v2_ud.set_value(val) ;
//    v2->add_node_attribute("value", v2_ud) ;
//    
//    as->push_node_branch(v) ;
//    as->push_node_branch(v2) ;
//    
//    return as ;
//}
//
//node* CLM_assignment_statement(node* var, node* exp)
//{
//    node* as = new node() ;
//    as->set_construct_name("assignment_statement") ;
//    as->set_t_nt_flag('n') ;
//    UD_String op_ud ;
//    op_ud.set_value("=") ;
//    as->add_node_attribute("assign_op", op_ud) ;
//    
//    as->push_node_branch(var) ;
//    as->push_node_branch(exp) ;
//    
//    return as ;
//}
//
//node* CLM_assignment_statement(string var, string val)
//{
//    node* as = new node() ;
//    as->set_construct_name("assignment_statement") ;
//    as->set_t_nt_flag('n') ;
//    UD_String op_ud ;
//    op_ud.set_value("=") ;
//    as->add_node_attribute("assign_op", op_ud) ;
//    
//    node* v = new node() ;
//    v->set_construct_name("var") ;
//    v->set_t_nt_flag('t') ;
//    String v_ud ;
//    v_ud.set_value(var) ;
//    v->add_node_attribute("var", v_ud) ;
//    
//    node* v2 = new node() ;
//    v2->set_construct_name("var") ;
//    v2->set_t_nt_flag('t') ;
//    UD_String v2_ud;
//    v2_ud.set_value(val) ;
//    v2->add_node_attribute("var", v2_ud) ;
//    
//    as->push_node_branch(v) ;
//    as->push_node_branch(v2) ;
//    
//    return as ;
//}
//
//node* CLM_binary_expression(string lhs, string op, string rhs)
//{
//    node* be = new node() ;
//    be->set_construct_name("binary_expression") ;
//    be->set_t_nt_flag('n') ;
//    UD_String op_ud ;
//    op_ud.set_value(op) ;
//    be->add_node_attribute("binary_op", op_ud) ;
//    
//    node* l = new node() ;
//    l->set_construct_name("var") ;
//    l->set_t_nt_flag('t') ;
//    UD_String l_ud ;
//    l_ud.set_value(lhs) ;
//    l->add_node_attribute("var", l_ud) ;
//    
//    node* r = new node() ;
//    r->set_construct_name("var") ;
//    r->set_t_nt_flag('t') ;
//    String r_ud ;
//    r_ud.set_value(rhs) ;
//    r->add_node_attribute("var", r_ud) ;
//    
//    be->push_node_branch(l) ;
//    be->push_node_branch(r) ;
//    
//    return be ;
//}
//
node* CLM_for_loop(node* init, node* cond, node* next, node* body)
{
    node* fl = new node() ;
    fl->set_construct_name("for_loop") ;
    fl->set_t_nt_flag('n') ;
    
    fl->push_node_branch(init) ;
    fl->push_node_branch(cond) ;
    fl->push_node_branch(next) ;
    
    node* b = new node() ;
    b->set_construct_name("block") ;
    b->set_t_nt_flag('n') ;
    
    b->push_node_branch(body) ;
    
    fl->push_node_branch(b) ;
    
    return fl ;
}

node* CLM_if_else_statement(node* cond, node* body1, node* body2)
{
    node* ie = new node() ;
    ie->set_construct_name("if_else") ;
    ie->set_t_nt_flag('n') ;
    
    node* ic = new node() ;
    ic->set_construct_name("if_cond") ;
    ic->set_t_nt_flag('n') ;
    ic->push_node_branch(cond) ;
    
    node* b1 = new node() ;
    b1->set_construct_name("block") ;
    b1->set_t_nt_flag('n') ;
    
    b1->push_node_branch(body1) ;
    
    node* b2 = new node() ;
    b2->set_construct_name("block") ;
    b2->set_t_nt_flag('n') ;
    
    b2->push_node_branch(body2) ;
    
    ie->push_node_branch(cond) ;
    ie->push_node_branch(b1) ;
    ie->push_node_branch(b2) ;
    
    return ie ;
}

node* CLM_while_loop(node* cond, node* body)
{
    node* wl = new node() ;
    wl->set_construct_name("while_loop") ;
    wl->set_t_nt_flag('n') ;
    
    node* b = new node() ;
    b->set_construct_name("block") ;
    b->set_t_nt_flag('n') ;
    
    b->push_node_branch(body) ;
    
    wl->push_node_branch(cond) ;
    wl->push_node_branch(b) ;
    
    return wl ;
}
//
//node* CLM_function_call(string fname, list<string> par_list)
//{
//    node* fc = new node() ;
//    fc->set_construct_name("function_call") ;
//    fc->set_t_nt_flag('n') ;
//    
//    node* fnm = new node() ;
//    fnm->set_construct_name("function_name") ;
//    fnm->set_t_nt_flag('t') ;
//    UD_String fnm_ud ;
//    fnm_ud.set_value(fname) ;
//    fnm->add_node_attribute("function_name", fnm_ud) ;
//    
//    node* vl = new node() ;
//    vl->set_construct_name("parameter_list") ;
//    vl->set_t_nt_flag('n') ;
//    list<string>::iterator iter ;
//    for(iter = par_list.begin() ; iter != par_list.end() ; iter++)
//    {
//        node* v = new node() ;
//        v->set_construct_name("var") ;
//        v->set_t_nt_flag('t') ;
//        String v_ud ;
//        v_ud.set_value(*iter) ;
//        v->add_node_attribute("var", v_ud) ;
//        
//        vl->push_node_branch(v) ;
//    }
//    
//    fc->push_node_branch(fnm) ;
//    fc->push_node_branch(vl) ;
//    
//    return fc ; 
//}
//
//node* CLM_statement_sequence(list<node*> stmt)
//{
//    node* ss = new node() ;
//    ss->set_construct_name("statement_sequence") ;
//    ss->set_t_nt_flag('n') ;
//    
//    list<node*>::iterator iter ;
//    for(iter = stmt.begin() ; iter != stmt.end() ;iter++)
//        ss->push_node_branch(*iter) ;
//    
//    return ss ;
//}