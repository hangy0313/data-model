#include "dataAccessModel.h"

using namespace std;

map<string, UD_universal_data> importDataModelAccess(string dataModelInfo)
{
    map<string, UD_universal_data> inputted_attribute_table;
    ifstream dataModelTable(dataModelInfo.c_str());
    
    string name;
    string value;
    UD_String valueString;
    
    while(dataModelTable >> name){
        dataModelTable >> value;
        valueString.set_value(value);
        inputted_attribute_table.insert(pair<string, UD_universal_data>(name, valueString));
    }
    
    return inputted_attribute_table;
}

//exist(Ename, attr)
bool exist(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list)
{
    if(parameter_list.size() != 2)
        print_error_message("format error");
    
    list<UD_universal_data>::iterator iter;
    iter = parameter_list.begin();
    string first = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string second = *(((UD_String*)(&(*iter)))->getptr());
    
    map<string, UD_universal_data>::iterator attr_iter ;
    
    attr_iter = attribute_list.find(first) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    UD_universal_data enm = attr_iter->second ;
    string eName ;
    eName = *(((UD_String*)(&enm))->getptr());
    
    attr_iter = attribute_list.find(second) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    enm = attr_iter->second ;
    string attrName ;
    attrName = *(((UD_String*)(&enm))->getptr());
    
    string find = eName + "_" + attrName + "_name";
    
    attr_iter = attribute_list.find(find) ;
    if(attr_iter == attribute_list.end()){
        return false;
    }
    return true;
}

//isNavigat(Ename, Rname)
bool isNavigat(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list)
{
    if(parameter_list.size() != 2)
        print_error_message("format error");
    
    list<UD_universal_data>::iterator iter;
    iter = parameter_list.begin();
    string first = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string second = *(((UD_String*)(&(*iter)))->getptr());
    
    map<string, UD_universal_data>::iterator attr_iter ;
    
    //get entity name
    attr_iter = attribute_list.find(first) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    UD_universal_data enm = attr_iter->second ;
    string eName ;
    eName = *(((UD_String*)(&enm))->getptr());
    
    //get relationship name
    attr_iter = attribute_list.find(second) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    enm = attr_iter->second ;
    string rName ;
    rName = *(((UD_String*)(&enm))->getptr());
    
    for(attr_iter = attribute_list.begin();attr_iter != attribute_list.end();attr_iter++){
        string tmp = attr_iter->first;
        string roleName;
        map<string, UD_universal_data>::iterator iter;
        
        //find role name
        if(tmp.find(rName+"_") == 0
           && tmp.find("_name") != rName.size()
           && tmp.find("_name") != -1)
        {
            iter = attribute_list.find(rName + tmp.substr(rName.size()));
            enm = iter->second;
            roleName = *(((UD_String*)(&enm))->getptr());
            
            //find target entity name in role
            string targetName = rName+"_"+roleName+"_entityName";
            iter = attribute_list.find(targetName);
            enm = iter->second;
            targetName = *(((UD_String*)(&enm))->getptr());
            if(targetName == eName){
                string navigation = rName+"_"+roleName+"_navigation";
                iter = attribute_list.find(navigation);
                enm = iter->second;
                navigation = *(((UD_String*)(&enm))->getptr());
                if(navigation == "to_entity"){
                    return false;
                }else{
                    return true;
                }
            }
        }
    }
    
    return false;
}

//isEqual(Rname, "embedding", "N/A")
bool isEqual(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list)
{
    list<UD_universal_data>::iterator iter;
    iter = parameter_list.begin();
    string first = *(((UD_String*)(&(*iter)))->getptr());
    iter++ ;
    string postfix = *(((UD_String*)(&(*iter)))->getptr());
    iter++ ;
    string value = *(((UD_String*)(&(*iter)))->getptr());
    
    map<string, UD_universal_data>::iterator attr_iter ;
    //get entity or relationship name
    attr_iter = attribute_list.find(first) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    UD_universal_data enm = attr_iter->second ;
    string name ;
    name = *(((UD_String*)(&enm))->getptr());
    
    //get compare string
    string findString = name+"_"+postfix;
    attr_iter = attribute_list.find(findString) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    enm = attr_iter->second ;
    string compareString ;
    compareString = *(((UD_String*)(&enm))->getptr());
    
    if(compareString == value){cout << compareString << " " << value << endl;
        return true;
    }else{
        return false;
    }
}
//determineLink(Eename, Rname, prefix)
bool determineLinkOneStep(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list)
{
    map<string, UD_universal_data>::iterator attr_iter ;
    
    list<UD_universal_data>::iterator iter;
    iter = parameter_list.begin();
    string first = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string second = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string prefix = *(((UD_String*)(&(*iter)))->getptr());
    
    //get entity name
    attr_iter = attribute_list.find(first) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    UD_universal_data enm = attr_iter->second ;
    string eName ;
    eName = *(((UD_String*)(&enm))->getptr());
    
    //get relationship name
    attr_iter = attribute_list.find(second) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    enm = attr_iter->second ;
    string rName ;
    rName = *(((UD_String*)(&enm))->getptr());
    
    string linkName = eName+"_"+prefix; cout << linkName << endl;
    
    map<string, UD_universal_data>::iterator tmpIter;
    for(tmpIter = attribute_list.begin();tmpIter != attribute_list.end();tmpIter++){
        cout << tmpIter->first << endl;
        string key = tmpIter->first;
        if(key.find(linkName) != -1) return true;
    }
    
    return false;
}
//determineLink(Eename, Rname, prefix)
bool determineLink(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list)
{
    map<string, UD_universal_data>::iterator attr_iter ;
    
    list<UD_universal_data>::iterator iter;
    iter = parameter_list.begin();
    string first = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string second = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string prefix = *(((UD_String*)(&(*iter)))->getptr());
    
    //get entity name
    attr_iter = attribute_list.find(first) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    UD_universal_data enm = attr_iter->second ;
    string eName ;
    eName = *(((UD_String*)(&enm))->getptr());
    
    //get relationship name
    attr_iter = attribute_list.find(second) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    enm = attr_iter->second ;
    string rName ;
    rName = *(((UD_String*)(&enm))->getptr());
    
    string linkName = eName+"_"+prefix+eName+"_"+rName+"_name";
    
    attr_iter = attribute_list.find(linkName) ;
    if(attr_iter == attribute_list.end()) {
        return false;
    }
    
    return true;
}
//determineLinkFromR(Ename, Rname, prefix)
bool determineLinkFromR(map<string, UD_universal_data> attribute_list, list<UD_universal_data> parameter_list)
{
    map<string, UD_universal_data>::iterator attr_iter ;
    
    list<UD_universal_data>::iterator iter;
    iter = parameter_list.begin();
    string first = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string second = *(((UD_String*)(&(*iter)))->getptr());
    iter++;
    string prefix = *(((UD_String*)(&(*iter)))->getptr());
    
    //get entity name
    attr_iter = attribute_list.find(first) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    UD_universal_data enm = attr_iter->second ;
    string eName ;
    eName = *(((UD_String*)(&enm))->getptr());
    
    //get relationship name
    attr_iter = attribute_list.find(second) ;
    if(attr_iter == attribute_list.end())
        return false ;
    
    enm = attr_iter->second ;
    string rName ;
    rName = *(((UD_String*)(&enm))->getptr());
    
    string findString = rName+"_"+prefix+rName;
    for(attr_iter = attribute_list.begin();attr_iter != attribute_list.end();attr_iter++){
        string tmp = attr_iter->first;
        if(tmp.find(findString) != -1 && tmp.find(eName) == -1){
            return true;
        }
    }
    
    return false;
}

node* createParseTree(node* nodePtr, string funcList)
{
    list<node*> content;
    
    list<node*>::iterator iterFuncCall;
    for(iterFuncCall = funcCallList.begin();iterFuncCall != funcCallList.end();iterFuncCall++){
        node* functionCall = (node*)(*iterFuncCall);
        string funcName;
        
        UD_Virtual_Iterator* biter = functionCall->get_node_branch_iter() ;
        for(biter->begin() ; !biter->end() ; biter->advance())
        {
            node* tmp = (node*)(*(*biter)) ;
            
            if(tmp->get_construct_name() == "function_name"){
                UD_Attribute_List node_attr = tmp->get_all_node_attribute() ;
                node_attr.begin();
                UD_universal_data val = node_attr.get_attribute_value_al() ;
                funcName = *(((UD_String*)(&val))->getptr()) ;
            }
            
            if(tmp->get_construct_name() == "data_list"){
                varList.clear();
                findVar(tmp);
            }
        }
        node* stmt = new node();
        if(checkLibraryFunction(funcName)) {
            stmt = transToRealCode(funcName, funcList);
        }else {
            stmt = CLM_function_call(funcName, varList);
        }
        content.push_back(stmt);
    }
    
    node* ss = CLM_statement_sequence(content);
    list<node*> tmp;
    tmp.push_back(ss);
    
    return CLM_C_code(tmp);
}

void findFunctionCallNode(node* nodePtr)
{
    UD_Virtual_Iterator* biter = nodePtr->get_node_branch_iter() ;
    for(biter->begin() ; !biter->end() ; biter->advance())
    {
        node* tmp = (node*)(*(*biter)) ;
        if(tmp->get_construct_name() == "function_call"){
            funcCallList.push_back(tmp);
        }
        findFunctionCallNode(tmp);
    }
}

void findVar(node* nodePtr)
{
    UD_Virtual_Iterator* biter = nodePtr->get_node_branch_iter() ;
    for(biter->begin() ; !biter->end() ; biter->advance())
    {
        node* tmp = (node*)(*(*biter)) ;
        if(tmp->get_construct_name() == "variable"){
            UD_Attribute_List node_attr = tmp->get_all_node_attribute() ;
            node_attr.begin();
            UD_universal_data val = node_attr.get_attribute_value_al() ;
            string data = *(((UD_String*)(&val))->getptr()) ;
            
            varList.push_back(data);
        }
        findVar(tmp);
    }
}

bool checkLibraryFunction(string funcName)
{
    if(funcName == "pointer") return true;
    if(funcName == "list_taget_type") return true;
    if(funcName == "link_one_step") return true;
    if(funcName == "link_step_one") return true;
    if(funcName == "list_link_step_two") return true;
        
    return false;
}

node* transToRealCode(string funcName, string funcList)
{
    node* stmt;
    
    if(funcName == "pointer") stmt = pointer();
    if(funcName == "list_taget_type") stmt = list_taget_type(funcList);
    if(funcName == "link_one_step") stmt = link_one_step(funcList);
    if(funcName == "link_step_one") stmt = link_step_one(funcList);
    if(funcName == "list_link_step_two") stmt = list_link_step_two(funcList);
    
    return stmt;
}

node* pointer()
{
    list<string>::iterator iter;
    iter = varList.begin();
    
    return CLM_data_declaration((*iter)+"*", (*iter));
}

node* list_taget_type(string funcList)
{
    ifstream funcTableInput(funcList.c_str());
    string entiyName, relationshipName;
    string targetName;
    
    map<string, string> funcTable;
    map<string, string>::iterator funcTableIter;
    
    string input1, input2;
    while(funcTableInput >> input1){
        funcTableInput >> input2;
        funcTable.insert(pair<string, string>(input1, input2));
    }
    
    list<string>::iterator dataIter;
    dataIter = varList.begin();
    entiyName = *dataIter;
    dataIter++;
    relationshipName = *dataIter;
    for(funcTableIter = funcTable.begin();funcTableIter != funcTable.end();funcTableIter++){
        string funcTitle = funcTableIter->first;
        string funcNameInTable = funcTableIter->second;
        if(funcTitle.find(entiyName) == -1 &&
           funcTitle.find(relationshipName) == -1){
            targetName = targetName.assign(funcTitle, 0, funcTitle.find("_function"));
            
            break;
        }
    }
    
    return CLM_data_declaration("list<"+targetName+">", "Var2");
}

node* link_one_step(string funcList)
{
    ifstream funcTableInput(funcList.c_str());
    string entiyName, relationshipName;
    string realFuncName;
    
    map<string, string> funcTable;
    map<string, string>::iterator funcTableIter;
    
    string input1, input2;
    while(funcTableInput >> input1){
        funcTableInput >> input2;
        funcTable.insert(pair<string, string>(input1, input2));
    }
    
    list<string>::iterator dataIter;
    dataIter = varList.begin();
    entiyName = *dataIter;
    dataIter++;
    relationshipName = *dataIter;
    for(funcTableIter = funcTable.begin();funcTableIter != funcTable.end();funcTableIter++){
        string funcTitle = funcTableIter->first;
        string funcNameInTable = funcTableIter->second;
        
        if(funcTitle.find(entiyName) != -1 &&
           funcNameInTable.find(entiyName) != -1 &&
           funcNameInTable.find("List") == -1){
            realFuncName = funcNameInTable;
        }
    }
    
    list<string> tmp;
    tmp.push_back(" ");
    node* fc = CLM_member_function_call(entiyName, realFuncName, tmp);
    
    int start = realFuncName.find(entiyName) + entiyName.size() + 1;
    int size = realFuncName.size()-(realFuncName.find(entiyName)+entiyName.size())-1;
    string type;
    type = type.assign(realFuncName, start, size);
    
    node* dd = CLM_data_declaration(type+"*", type);
    
    return CLM_assignment_statement(dd, fc);
}

node* link_step_one(string funcList)
{
    ifstream funcTableInput(funcList.c_str());
    string entiyName, relationshipName;
    string realFuncName;
    
    map<string, string> funcTable;
    map<string, string>::iterator funcTableIter;

    string input1, input2;
    while(funcTableInput >> input1){
        funcTableInput >> input2;
        funcTable.insert(pair<string, string>(input1, input2));
    }
    
    list<string>::iterator dataIter;
    dataIter = varList.begin();
    entiyName = *dataIter;
    dataIter++;
    relationshipName = *dataIter;
    for(funcTableIter = funcTable.begin();funcTableIter != funcTable.end();funcTableIter++){
        string funcTitle = funcTableIter->first;
        string funcNameInTable = funcTableIter->second;
        
        if(funcTitle.find(entiyName) != -1 &&
           funcNameInTable.find(entiyName) != -1 &&
           funcNameInTable.find(relationshipName) != -1 &&
           funcNameInTable.find("List") == -1){
            realFuncName = funcNameInTable;
        }
    }

    list<string> tmp;
    tmp.push_back(" ");
    node* fc = CLM_member_function_call(entiyName, realFuncName, tmp);
    
    return CLM_assignment_statement(relationshipName, fc);
}

node* list_link_step_two(string funcList)
{
    ifstream funcTableInput(funcList.c_str());
    string entiyName, relationshipName;
    string realFuncName;
    
    map<string, string> funcTable;
    map<string, string>::iterator funcTableIter;
    
    string input1, input2;
    while(funcTableInput >> input1){
        funcTableInput >> input2;
        funcTable.insert(pair<string, string>(input1, input2));
    }
    
    list<string>::iterator dataIter;
    dataIter = varList.begin();
    entiyName = *dataIter;
    dataIter++;
    relationshipName = *dataIter;
    for(funcTableIter = funcTable.begin();funcTableIter != funcTable.end();funcTableIter++){
        string funcTitle = funcTableIter->first;
        string funcNameInTable = funcTableIter->second;
        
        if(funcTitle.find(relationshipName) != -1 &&
           funcNameInTable.find(entiyName) == -1 &&
           funcNameInTable.find(relationshipName) != -1 &&
           funcNameInTable.find("List") != -1){
            realFuncName = funcNameInTable;
        }
    }
    
    list<string> tmp;
    tmp.push_back(" ");
    node* fc = CLM_member_function_call(relationshipName, realFuncName, tmp);
    
    return CLM_assignment_statement("Var2", fc);
}

node* CLM_C_code(list<node*> content)
{
    node* c = new node() ;
    c->set_construct_name("C++") ;
    c->set_t_nt_flag('n') ;
    
    list<node*>::iterator iter ;
    for(iter = content.begin() ; iter != content.end() ; iter++)
        c->push_node_branch(*iter) ;
    
    return c ;
}

node* CLM_data_declaration(string type, string name)
{
    node* dd = new node() ;
    dd->set_construct_name("variable_declaration") ;
    dd->set_t_nt_flag('n') ;
    
    node* t = new node() ;
    t->set_construct_name("type") ;
    t->set_t_nt_flag('t') ;
    UD_String type_ud ;
    type_ud.set_value(type) ;
    t->add_node_attribute("type", type_ud) ;
    
    node* var = new node() ;
    var->set_construct_name("var") ;
    var->set_t_nt_flag('t') ;
    UD_String var_ud ;
    var_ud.set_value(name) ;
    var->add_node_attribute("var", var_ud) ;
    
    dd->push_node_branch(t) ;
    dd->push_node_branch(var) ;
    
    return dd ;
}

node* CLM_assignment_statement(string var, node* exp)
{
    node* as = new node() ;
    as->set_construct_name("assignment_statement") ;
    as->set_t_nt_flag('n') ;
    UD_String op_ud ;
    op_ud.set_value("=") ;
    as->add_node_attribute("assign_op", op_ud) ;
    
    node* v = new node() ;
    v->set_construct_name("var") ;
    v->set_t_nt_flag('t') ;
    UD_String v_ud ;
    v_ud.set_value(var) ;
    v->add_node_attribute("var", v_ud) ;
    
    as->push_node_branch(v) ;
    as->push_node_branch(exp) ;
    
    return as ;
}

node* CLM_assignment_statement(string var, int val)
{
    node* as = new node() ;
    as->set_construct_name("assignment_statement") ;
    as->set_t_nt_flag('n') ;
    UD_String op_ud ;
    op_ud.set_value("=") ;
    as->add_node_attribute("assign_op", op_ud) ;
    
    node* v = new node() ;
    v->set_construct_name("var") ;
    v->set_t_nt_flag('t') ;
    UD_String v_ud ;
    v_ud.set_value(var) ;
    v->add_node_attribute("var", v_ud) ;
    
    node* v2 = new node() ;
    v2->set_construct_name("value") ;
    v2->set_t_nt_flag('t') ;
    UD_Int v2_ud;
    v2_ud.set_value(val) ;
    v2->add_node_attribute("value", v2_ud) ;
    
    as->push_node_branch(v) ;
    as->push_node_branch(v2) ;
    
    return as ;
}

node* CLM_assignment_statement(node* var, node* exp)
{
    node* as = new node() ;
    as->set_construct_name("assignment_statement") ;
    as->set_t_nt_flag('n') ;
    UD_String op_ud ;
    op_ud.set_value("=") ;
    as->add_node_attribute("assign_op", op_ud) ;
    
    as->push_node_branch(var) ;
    as->push_node_branch(exp) ;
    
    return as ;
}

node* CLM_assignment_statement(string var, string val)
{
    node* as = new node() ;
    as->set_construct_name("assignment_statement") ;
    as->set_t_nt_flag('n') ;
    UD_String op_ud ;
    op_ud.set_value("=") ;
    as->add_node_attribute("assign_op", op_ud) ;
    
    node* v = new node() ;
    v->set_construct_name("var") ;
    v->set_t_nt_flag('t') ;
    UD_String v_ud ;
    v_ud.set_value(var) ;
    v->add_node_attribute("var", v_ud) ;
    
    node* v2 = new node() ;
    v2->set_construct_name("var") ;
    v2->set_t_nt_flag('t') ;
    UD_String v2_ud;
    v2_ud.set_value(val) ;
    v2->add_node_attribute("var", v2_ud) ;
    
    as->push_node_branch(v) ;
    as->push_node_branch(v2) ;
    
    return as ;
}

node* CLM_binary_expression(string lhs, string op, string rhs)
{
    node* be = new node() ;
    be->set_construct_name("binary_expression") ;
    be->set_t_nt_flag('n') ;
    UD_String op_ud ;
    op_ud.set_value(op) ;
    be->add_node_attribute("binary_op", op_ud) ;
    
    node* l = new node() ;
    l->set_construct_name("var") ;
    l->set_t_nt_flag('t') ;
    UD_String l_ud ;
    l_ud.set_value(lhs) ;
    l->add_node_attribute("var", l_ud) ;
    
    node* r = new node() ;
    r->set_construct_name("var") ;
    r->set_t_nt_flag('t') ;
    UD_String r_ud ;
    r_ud.set_value(rhs) ;
    r->add_node_attribute("var", r_ud) ;
    
    be->push_node_branch(l) ;
    be->push_node_branch(r) ;
    
    return be ;
}

node* CLM_binary_expression(node* lhs, string op, node* rhs)
{
    node* be = new node() ;
    be->set_construct_name("binary_expression") ;
    be->set_t_nt_flag('n') ;
    UD_String op_ud ;
    op_ud.set_value(op) ;
    be->add_node_attribute("binary_op", op_ud) ;
    
    be->push_node_branch(lhs) ;
    be->push_node_branch(rhs) ;
    
    return be ;
}

node* CLM_function_call(string fname, list<string> par_list)
{
    node* fc = new node() ;
    fc->set_construct_name("function_call") ;
    fc->set_t_nt_flag('n') ;
    
    node* fnm = new node() ;
    fnm->set_construct_name("function_name") ;
    fnm->set_t_nt_flag('t') ;
    UD_String fnm_ud ;
    fnm_ud.set_value(fname) ;
    fnm->add_node_attribute("function_name", fnm_ud) ;
    
    node* vl = new node() ;
    vl->set_construct_name("parameter_list") ;
    vl->set_t_nt_flag('n') ;
    list<string>::iterator iter ;
    for(iter = par_list.begin() ; iter != par_list.end() ; iter++)
    {
        node* v = new node() ;
        v->set_construct_name("var") ;
        v->set_t_nt_flag('t') ;
        UD_String v_ud ;
        v_ud.set_value(*iter) ;
        v->add_node_attribute("var", v_ud) ;
        
        vl->push_node_branch(v) ;
    }
    
    fc->push_node_branch(fnm) ;
    fc->push_node_branch(vl) ;
    
    return fc ;
}

node* CLM_statement_sequence(list<node*> stmt)
{
    node* ss = new node() ;
    ss->set_construct_name("statement_sequence") ;
    ss->set_t_nt_flag('n') ;
    
    list<node*>::iterator iter ;
    for(iter = stmt.begin() ; iter != stmt.end() ;iter++)
        ss->push_node_branch(*iter) ;
    
    return ss ;
}

node* CLM_member_function_call(string obj, string fname, list<string> par_list)
{
    node* mfc = new node() ;
    mfc->set_construct_name("object_call_function") ;
    mfc->set_t_nt_flag('n') ;
    
    node* o = new node() ;
    o->set_construct_name("var") ;
    o->set_t_nt_flag('t') ;
    UD_String o_ud ;
    o_ud.set_value(obj) ;
    o->add_node_attribute("var", o_ud) ;
    
    node* fnm = new node() ;
    fnm->set_construct_name("function_name") ;
    fnm->set_t_nt_flag('t') ;
    UD_String fnm_ud ;
    fnm_ud.set_value(fname) ;
    fnm->add_node_attribute("function_name", fnm_ud) ;
    
    node* vl = new node() ;
    vl->set_construct_name("parameter_list") ;
    vl->set_t_nt_flag('n') ;
    list<string>::iterator iter ;
    for(iter = par_list.begin() ; iter != par_list.end() ; iter++)
    {
        node* v = new node() ;
        v->set_construct_name("var") ;
        v->set_t_nt_flag('t') ;
        UD_String v_ud ;
        v_ud.set_value(*iter) ;
        v->add_node_attribute("var", v_ud) ;
        
        vl->push_node_branch(v) ;
    }
    
    mfc->push_node_branch(o) ;
    mfc->push_node_branch(fnm) ;
    mfc->push_node_branch(vl) ;
    
    return mfc ;
}