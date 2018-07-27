#include <iostream>
#include <fstream>
#include "generalized_parser.h"

using namespace std ;

// macro function 
#define initialization() \
	acc_vector.clear() ; \
	new_token_index = current_token_index ; \
	create_flag = compute_creation_shaping(construct_id) ; \
	set_nested_filtering(construct_id) ; \
	if(create_flag) \
		nodeptr = new node() ;

#define terminal_processing() \
	if(nodeptr != NULL) \
	{ \
		nodeptr->set_t_nt_flag('t') ; \
		nodeptr->set_construct_name(get_construct_name(construct_id) ) ; \
		string attr_name = node_creation_table.find(get_construct_name(construct_id))->second.attribute_name ; \
		nodeptr->add_node_attribute(attr_name, token_seq[new_token_index]->token_specifier) ; \
		acc_vector.push_back(pair<string, unsigned*>("node", (unsigned*)(nodeptr) ) ) ; \
	} \
	else \
	{ \
		string attr_name = node_creation_table.find(get_construct_name(construct_id))->second.attribute_name ; \
		pair<string, UD_universal_data>* attr_tmp = new pair<string, UD_universal_data>(attr_name, token_seq[new_token_index]->token_specifier) ; \
		acc_vector.push_back(pair<string, unsigned*>("attribute", (unsigned*)(attr_tmp) ) ) ; \
	} 

#define construct_preprocessed_match() \
	if( j == 0 && parse_tree_ptr != NULL) \
	{\
		nodeptr->push_node_branch(parse_tree_ptr) ; \
		parse_tree_ptr->push_parent_pointer(nodeptr) ; /*setup parent pointer */\
		continue ; \
	}

#define element_match_processing() \
	list<pair<string, unsigned*> >::iterator iter ; \
	if(create_flag) \
	{\
		nodeptr->set_t_nt_flag('n') ; \
		nodeptr->set_nt_index(construct_id) ; \
		nodeptr->set_construct_name(get_construct_name(construct_id)) ; \
		for( iter = tmp_acc_vector.begin() ; iter != tmp_acc_vector.end() ; iter++) \
		{\
			if(iter->first == "node") \
			{\
				nodeptr->push_node_branch( (node*)(iter->second)) ; \
				((node*)(iter->second))->push_parent_pointer(nodeptr) ; /* set up parent pointer*/ \
			}\
			else \
			{/*attribute*/ \
				pair<string, UD_universal_data> attr_tmp ;  \
				attr_tmp.first = ((pair<string, UD_universal_data>*)(iter->second))->first ; \
				attr_tmp.second = ((pair<string, UD_universal_data>*)(iter->second))->second ; \
				nodeptr->add_node_attribute( attr_tmp.first, attr_tmp.second) ; \
			}\
		}\
	}\
	else  \
	{ /*nodeptr is NULL*/ \
		for(iter = tmp_acc_vector.begin() ; iter != tmp_acc_vector.end(); iter++) \
			acc_vector_tmp.push_back(*iter) ; \
	} 

#define element_dismatch_processing() \
	if(create_flag) \
	{\
		nodeptr->deep_delete() ; \
		nodeptr->clear_attributes() ; \
	}\
	else \
		acc_vector_tmp.clear() ;

#define nonterminal_match_processing() \
	if(elements[j].add_flag == true) /*+case*/ \
	{\
		if(match_times >= 1) \
			match = true ; \
		else \
		{\
			match = false ; \
			element_dismatch_processing() ; \
			new_token_index = current_token_index ;\
			break ; /*break inner loop*/ \
		}\
	}\
	else/* *case */ \
	{\
		match = true ; \
		if(match_times == 0) \
		{\
			element_match_processing() ;\
		}\
	}

#define rule_match_processing() pair<bool, string> prec_data = compute_precedence_shaping(nodeptr, new_token_index) ; \
	if( !prec_data.first) \
	{	\
		if(nodeptr!= NULL) \
		{ \
			associativity_restructuring(nodeptr) ; \
			pair<string, unsigned*> record_elem("node", (unsigned*)nodeptr) ; \
			acc_vector_tmp.push_back(record_elem) ; \
		} \
	}	\
	else \
	{ \
		node* tmpptr = NULL ;\
		if(prec_data.second == "left_association") \
		{ \
			tmpptr = left_assoc_restructuring(nodeptr,new_token_index) ; \
		} \
		else \
		{ \
			tmpptr = right_assoc_restructuring(nodeptr, new_token_index) ;\
		} \
		if(tmpptr == NULL)/*match fail*/ \
		{\
			element_dismatch_processing() ;\
			new_token_index = current_token_index ;\
			continue ; /*continue outer loop*/\
		}\
		else \
		{\
			pair<string, unsigned*> record_elem("node", (unsigned*)tmpptr) ; \
			acc_vector_tmp.push_back(record_elem) ; \
		}\
	} \
	list<pair<string, unsigned*> >::iterator iter ; \
	for(iter = acc_vector_tmp.begin() ; iter != acc_vector_tmp.end(); iter++) \
			acc_vector.push_back(*iter) ; \
	break ; /*break outer loop*/

#define postprocessing() restore_nested_filtering(construct_id) ; \
	if(nodeptr != NULL && return_val == true) \
	{\
		/*action*/ \
		action_processing() ;\
		/*binding*/ \
		binding_processing() ;\
	}

#define action_processing() \
	/*action*/ \
	map<string, list<string> >::iterator action_iter  ;\
	action_iter = action_table.find(get_construct_name(construct_id)) ; \
	if(action_iter != action_table.end()) \
	{ \
		list<string>::iterator aliter; \
		for(aliter = action_iter->second.begin() ; aliter != action_iter->second.end() ; aliter++)\
		{\
			map<string, void(*)(node*)>::iterator global_action_iter ;\
			global_action_iter = global_action_table.find(*aliter) ;\
			if(global_action_iter == global_action_table.end()) \
			{\
				cout<<"error: can't find action function "<<*aliter<<endl ;\
				exit(-1) ;\
			}\
			global_action_iter->second( (node*)(nodeptr)) ; \
		}\
	}

#define binding_processing() \
	/*binding*/\
	map<string, list<pair<string, string> > >::iterator binding_iter ;\
	binding_iter = binding_table.find(get_construct_name(construct_id)) ;\
	if(binding_iter != binding_table.end()) \
	{\
		list<pair<string, string> >::iterator binding_liter ; \
		for(binding_liter = binding_iter->second.begin() ; binding_liter != binding_iter->second.end() ; binding_liter++)\
		{\
			nodeptr->push_binding_info( binding_liter->first, binding_liter->second) ; \
		}\
	}
// --- --- -- -- - 

generalized_parser::generalized_parser(std::string grammar_filename, std::string node_creation_rule_filename, std::string associativity_filename, std::string precedence_filename, std::string general_construct_filename,string action_filename, string binding_filename)
{
	this->alloc_num = 0 ;
	this->nt_num = 0 ;
	this->keyword_bound = 0 ;
	this->initialize_grammar_rule(grammar_filename) ;
	this->initialize_node_creation_rule(node_creation_rule_filename) ;
	this->initialize_associativity(associativity_filename) ;
	this->initialize_precedence(precedence_filename) ;
	this->initialize_general_constuct_name(general_construct_filename) ;
	this->initialize_action_table(action_filename) ;
	this->initialize_binding_table(binding_filename) ;

	global_nest_creation_stack.push(true) ;
}

generalized_parser::~generalized_parser()
{
}

void generalized_parser::initialize_grammar_rule(std::string filename)
{
	ifstream infile ; 
	string tmp ; 
	map<string, int>::iterator construct_iter ;
	// open file 
	infile.open(filename.c_str()) ;
	if(!infile){
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}
	// read file 
	// read nonterminal declaration 
	// read a string 
	infile>>tmp ;
	if(tmp!= "nonterminal")
	{
		cout<<"read error string"<<endl ;
		exit(-1) ;
	}
	// read a string 
	infile>>tmp ;
	while( (!infile.eof()) && tmp != ";")
	{
		// lookup string in the construct table 
		construct_iter = construct_table.find(tmp) ;
		if(construct_iter != construct_table.end())
		{
			cout<<"error: duplicate "<<tmp<<endl ;
			exit(-1) ;
		}
		// insert string in teh construct table, alloc_num++
		construct_table.insert(pair<string, int>(tmp, alloc_num++) ) ;
		// nt_num++
		nt_num++ ;
		// add a empty rule in the grammar rules 
		grammar_rules.push_back(new rule()) ;
		// read a string 
		infile >>tmp ;
	}
	//read keyword declaration 
	// read a string 
	infile>>tmp ;
	if(tmp!= "keyword")
	{
		cout<<"read error string in keyword declaration"<<endl ;
		exit(-1) ;
	}
	// read a string 
	keyword_bound = nt_num ;
	infile >>tmp ; 
	while( (!infile.eof()) && tmp != ";")
	{
		// lookup string in the construct table 
		construct_iter = construct_table.find(tmp) ;
		if(construct_iter != construct_table.end())
		{
			cout<<"error: duplicate "<<tmp<<endl ;
			exit(-1) ;
		}
		// insert (string, alloc_num++) in construct table 
		construct_table.insert(pair<string, int>(tmp, alloc_num++)) ;
		// keyword bound ++ 
		keyword_bound++ ;
		// read a string 
		infile>>tmp ;
	}
	// read terminal declaration 
	// read a string 
	infile >> tmp ;
	if(tmp!="terminal")
	{
		cout<<"read error string in terminal declaration"<<endl ;
		exit(-1) ;
	}
	// read a string 
	infile >> tmp ;
	while ( (!infile.eof()) && tmp!=";")
	{
		// look up string in the construct table 
		construct_iter = construct_table.find(tmp) ;
		if(construct_iter != construct_table.end())
		{
			cout<<"error: duplicate"<<tmp<<endl ;
			exit(-1) ;
		}
		// insert (string, alloc_num++) in teh construct table 
		construct_table.insert(pair<string, int>(tmp, alloc_num++)) ; 
		// read a string 
		infile >>tmp ;
	}
	// read grammar rules 
	int index, subrule_index ; 
	// read a string 
	infile>>tmp ;
	while(!infile.eof()) 
	{
		construct_iter = construct_table.find(tmp) ;
		if( (construct_iter == construct_table.end()) || construct_iter->second>= nt_num)
		{
			cout<<"not a nonterminal "<<tmp<<endl ;
			exit(-1) ;
		}
		index = construct_iter->second ;
		subrule* srtmp = new subrule ;
		grammar_rules[index]->push_subrule(srtmp) ;
		// subrule_index = grammar_rules[index].size-1
		subrule_index = grammar_rules[index]->size() -1 ;
		// read a string 
		infile>> tmp ;
		if(tmp == "==")
		{// set up delimiter 
			// read a string 
			infile>>tmp ;
			construct_iter = construct_table.find(tmp) ;
			if( (construct_iter == construct_table.end()) || construct_iter->second < nt_num)
			{
				cout<<"error: it is not a terminal "<<endl ;
				exit(-1) ;
			}

			if(construct_iter->second < keyword_bound)
			{
				// set up delimiter 
				(*(grammar_rules[index]))[subrule_index]->delemiter = construct_iter->second ;
				//read a string 
				infile>>tmp ;
				// continue
				continue ;
			}
			else // error
			{
				cout<<"it is not a keyword"<<endl ;
				exit(-1) ;
			}
		}
		else if (tmp != "=") //error
		{
			cout<<"error: != '=' "<<tmp<<endl ;
			exit(-1) ;
		}
		// tmp == "="
		//read a string 
		infile >>tmp ;
		while( (!infile.eof()) && tmp != ";")
		{
			string tmp_name = tmp ; 
			bool tmp_aflag = false ;
			bool tmp_sflag = false ;
			if(tmp_name[tmp_name.size() -1] == '+')
			{
				tmp_aflag = true ;
				tmp.erase(tmp.end()-1) ;
			}
			else if (tmp_name[tmp_name.size()-1] == '*')
			{
				tmp_sflag = true ;
				tmp.erase(tmp.end()-1) ;
			}

			construct_iter = construct_table.find(tmp) ;
			if(construct_iter != construct_table.end() && construct_iter->second >= nt_num) 
			{// terminal 
				element etmp ;
				etmp.t_nt_flag = 't' ;
				etmp.t_nt_index = construct_iter->second ;
				etmp.add_flag = tmp_aflag ;
				etmp.star_flag = tmp_sflag ;
				// insert element in the subrule 
				(*(grammar_rules[index]))[subrule_index]->element_vector.push_back(etmp) ;
				// read a string 
				infile>>tmp ;
				continue ;
			}
			else if(construct_iter != construct_table.end() && construct_iter->second < nt_num)
			{// nonterminal 
				element etmp ;
				etmp.t_nt_flag = 'n' ;
				etmp.t_nt_index = construct_iter->second ;
				etmp.add_flag = tmp_aflag; 
				etmp.star_flag = tmp_sflag ;
				// insert element in teh subrule 
				(*(grammar_rules[index]))[subrule_index]->element_vector.push_back(etmp) ;
				//read a string 
				infile>>tmp ;
				continue ;
			}
			else
			{//error
				cout<<"error: not valid string"<<tmp<<endl ;
				exit(-1) ;
			}
		}
		//read a string 
		infile>>tmp ;
	}
}

void generalized_parser::initialize_node_creation_rule(std::string filename)
{
	ifstream infile ; 
	char npf, ncf ;
	string name, attr_name ;
	map<string, node_creation_rule_elem>::iterator iter ;
	// open file 
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}
	infile>>name ;
	while(!infile.eof())
	{
		iter = node_creation_table.find(name) ;
		if(iter != node_creation_table.end())
		{
			cout<<"error, duplicate name "<<name<<endl ;
			exit(-1) ;
		}
		// set flag 
		infile>>npf ;
		infile>>ncf ;
		if(npf != 'T' && npf != 'F' && npf != 'R' && npf != 'O')
		{
			cout<<"error, unknown word "<<npf<<endl ;
			exit(-1) ;
		}
		if(ncf != 'T' && ncf != 'F')
		{
			cout<<"error, unknown word "<<ncf<<endl ;
			exit(-1) ;
		}
		infile>>attr_name ;
		node_creation_rule_elem tmp ;
		if(attr_name == ";")
		{
			tmp.nest_print_flag  = npf ;
			tmp.node_creation_flag = ncf ;
			tmp.attribute_name = "NoName" ;
			node_creation_table.insert(pair<string, node_creation_rule_elem>(name, tmp) ) ;
		}
		else
		{
			tmp.nest_print_flag  = npf ;
			tmp.node_creation_flag = ncf ;
			tmp.attribute_name = attr_name ;
			node_creation_table.insert(pair<string, node_creation_rule_elem>(name, tmp) ) ;
			infile>>name ;
			if(name != ";")
			{
				cout<<"error, not ';' "<<name<<endl ;
				exit(-1) ;
			}
		}
		infile>>name ;
	}
}

void generalized_parser::initialize_associativity(std::string filename)
{
	ifstream infile ;
	string name, attr_name, next_name, next_attr_name ;
	map<string, list<sensitive_elem> >::iterator iter ;
	// open file 
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}
	infile>>name ;
	while(!infile.eof())
	{
		infile>>attr_name ;
		infile>>next_name ;
		infile>>next_attr_name ;

		sensitive_elem tmp ;
		tmp.attr_name = attr_name ;
		tmp.next_level_construct_name = next_name ;
		tmp.next_level_attr_name = next_attr_name ;

		iter = associativity_table.find(name) ;
		if(iter == associativity_table.end())
		{
			list<sensitive_elem> sltmp ;
			sltmp.push_back(tmp) ;
			associativity_table.insert(pair<string, list<sensitive_elem> >(name, sltmp)) ;
		}
		else
		{
			iter->second.push_back(tmp) ;
		}
		//read a string 
		infile>>name ;
		if(name != ";")
		{
			cout<<"error: format is error"<<endl ;
			exit(-1) ;
		}
		//read next line 
		infile>>name ;
	}
}

void generalized_parser::initialize_precedence(std::string filename)
{
	ifstream infile ;
	int level ;
	string type, elem ;
	map<int, list<precedence_elem> >::iterator iter ;
	
	// open file 
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}

	//read a string 
	infile>>level ;
	while(!infile.eof())
	{
		infile>>type ;
		while( type != ";" && !infile.eof())
		{
			infile>>elem ;
			if(type != "binary_op" && type!="unary_op")
			{
				cout<<"error: unknown type "<<type<<endl; 
				exit(-1) ;
			}
			precedence_elem tmp ;
			tmp.type = type ; 
			tmp.elem = elem ;

			iter = precedence_table.find(level) ;
			if (iter == precedence_table.end())
			{
				list<precedence_elem> ltmp ;
				ltmp.push_back(tmp) ;
				precedence_table.insert(pair<int, list<precedence_elem> >(level, ltmp)) ;
			}
			else
			{
				iter->second.push_back(tmp) ;
			}
			infile >> type ;
		}
		// read next line
		infile>>level ;
	}
}

void generalized_parser::initialize_general_constuct_name(std::string filename)
{
	ifstream infile ;
	string name, left, right ;
	map<string, general_construct_name>::iterator iter ;
	//open file 
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}

	infile>>name ;
	while(!infile.eof())
	{
		infile>>left ;
		infile>>right ;
		general_construct_name tmp ;
		tmp.upper_level_construct_name = left ;
		tmp.lower_level_construct_name = right ;

		iter = general_construct_table.find(name) ;
		if(iter != general_construct_table.end())
		{
			cout<<"error, duplicate"<<name<<endl ;
			exit(-1) ;
		}
		general_construct_table.insert(pair<string, general_construct_name>(name, tmp) ) ;
		infile>>name ;
		if(name!=";")
		{
			cout<<"format error: not ; "<<endl ;
			exit(-1) ;
		}
		// read a string 
		infile>>name ;
	}
}

void generalized_parser::initialize_action_table(string filename) 
{
	// format: construct_name action_name [action_name] ;
	ifstream infile ;
	string construct_name, action_name ;
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}
	while(infile>>construct_name) 
	{
		infile>>action_name ;
		list<string> action_list ;
		action_list.clear() ;
		while( !infile.eof() && action_name != ";")
		{
			action_list.push_back(action_name) ;
			infile>>action_name ;
		}
		action_table.insert(pair<string, list<string> >(construct_name, action_list)) ;
	}
}

void generalized_parser::initialize_binding_table(std::string filename)
{
	// format : construct_name kind name [kind name] ;
	ifstream infile ;
	string construct_name, kind, name ;
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}
	while(infile>>construct_name) 
	{
		infile>>kind ;
		list<pair<string, string> > binding_list ; // pair<kind, name>
		binding_list.clear() ;
		while(!infile.eof() && kind != ";")
		{
			infile>>name ;
			binding_list.push_back(pair<string, string>(kind, name) ) ;
			infile>>kind ;
		}
		binding_table.insert(pair<string, list<pair<string, string> > >(construct_name, binding_list) ) ;
	}
}

bool generalized_parser::shaped_unified_parse(int construct_id, int current_token_index, int &new_token_index, std::list<pair<string,unsigned*> > &acc_vector, node *&parse_tree_ptr)
{
	if(current_token_index >= (int)token_seq.size())
		return false ;
	node* nodeptr = NULL ;
	bool return_val = false  ;
	bool match  ;
	bool create_flag ;
	initialization() ;
	// terminal and non-termian processing 
	if( construct_id >= nt_num) // terminal processing 
	{
		if(construct_id == token_seq[new_token_index]->token_id) // terminal match 
		{
			terminal_processing() ;	// perform terminal processing 
			new_token_index++ ; 
			return_val = true ;
		}
		else 
		{
			new_token_index = current_token_index ;
			return_val = false ;
		}
	}
	else // nonterminal processing 
	{
		list<pair<string, unsigned*> > acc_vector_tmp ;
		rule* r = grammar_rules[construct_id] ; // fetch the grammar rule 
		for(int i = 0 ; i < r->size() ; i++) // for each subrule in rule 
		{// match the grammr rule 
			subrule* sr = (*r)[i] ;
			if(sr->element_vector.size() == 0)
			{// match the delimiter of an empty subrule 
				if(sr->delemiter != token_seq[new_token_index]->token_id )
				{
					cout<<"parse error"<<endl ;
					exit(-1) ;
				}
				if(nodeptr != NULL)
				{
					nodeptr->deep_delete() ;
					delete nodeptr ;
				}
				nodeptr = NULL ;
				return_val = true ;
			}
			vector<element> elements = sr->element_vector ;
			for( unsigned j = 0 ; j < elements.size() ; j++) // for each element in subrule 
			{ // match a sub-rule of the grammar rule 
				construct_preprocessed_match() ;
				list<pair<string, unsigned*> > tmp_acc_vector ;
				int tmp_token_index ;
				node* ntmp = NULL ;
				if( elements[j].add_flag == true || elements[j].star_flag == true) 
				{// *, + case 
					int match_times = 0 ;
					while(1)
					{
						// element (terminal, non-terminal) match
						match = shaped_unified_parse(elements[j].t_nt_index, new_token_index, tmp_token_index, tmp_acc_vector, ntmp) ;

						if(match) // continue to match next element in teh subrule 
						{
							element_match_processing() ;
							match_times++ ;
							new_token_index = tmp_token_index ;
						}
						else		//skip to next subrule's match 
						{
							break ; // break while loop; 
						}
					}
					nonterminal_match_processing() ;
				}
				else 
				{// no *, + case 
					// element (terminal, non-terminal) match
					match = shaped_unified_parse(elements[j].t_nt_index, new_token_index, tmp_token_index, tmp_acc_vector, ntmp) ;

					if(match) // continue to match next element in teh subrule 
					{
						element_match_processing() ;
						new_token_index = tmp_token_index ;
					}
					else		//skip to next subrule's match 
					{
						element_dismatch_processing() ;
						new_token_index = current_token_index ;
						break ; // break inner loop; 
					}
				}
			}
			if(match) // grammar rule match succeeds 
			{
				rule_match_processing() ;
			}
			// continue to next subrule's match 
		}
		return_val = match ; // return grammr rule match status
	}
	postprocessing() ;	// postprocessing 
	return return_val ;
}

void generalized_parser::add_token(int token_id, UD_universal_data token_specifier)
{
	token* tmp = new token ;
	tmp->token_id = token_id ;
	tmp->token_specifier = token_specifier ;
	this->token_seq.push_back(tmp) ;
}

void generalized_parser::remove_token()
{
	this->token_seq.pop_back() ;
}

int generalized_parser::token_size()
{
	return this->token_seq.size() ;
}

token* generalized_parser::get_token(int index)
{
	if( index > (int)(token_seq.size() -1))
		return NULL ;
	else 
		return token_seq[index] ;
}

int generalized_parser::get_symbol_index(std::string s)
{
	map<string, int>::iterator iter ;
	iter = construct_table.find(s) ;
	if(iter == construct_table.end())
		return -1 ;
	else 
		return iter->second ;
}

string generalized_parser::get_construct_name(int index)
{
	map<string, int>::iterator iter ;
	for(iter = this->construct_table.begin() ; iter != construct_table.end(); iter++)
		if(iter->second == index)
			break ;
	if(iter == construct_table.end())
	{
		cout<<"index error: not found "<<endl ;
		exit(-1) ;
	}
	return iter->first ;
}

bool generalized_parser::compute_creation_shaping(int construct_id)
{
	map<string, node_creation_rule_elem>::iterator iter ;
	string construct_name = get_construct_name(construct_id) ;
	iter = node_creation_table.find(construct_name) ;
	if(iter == node_creation_table.end())
	{
		cout<<"error: can't find node creation rule with id "<<construct_id<<endl ;
		exit(-1) ;
	}
	// make reduction decision for this construct 
	bool reduction_decision ;
	if(iter->second.node_creation_flag == 'T')
		reduction_decision = false ;
	else 
		reduction_decision = true ;

	// make filter decision for this construct 
	bool filter_decision ;
	if( global_nest_creation_stack.top() == true || iter->second.nest_print_flag == 'O')
		filter_decision = false ;  // filter out the node's creation 
	else 
		filter_decision = true ;

	// combine node creation decision 
	bool create_flag ; 
	if(!filter_decision) // not fileter 
		create_flag = !reduction_decision ;
	else 
		create_flag = !filter_decision ;
	//bool create_flag = (!filter_decision) | (!reduction_decision) ;
	return create_flag ;
}

bool generalized_parser::compute_associativity_shaping(node *nodeptr, node *childptr)
{
	map<string, list<sensitive_elem> >::iterator iter ;
	list<sensitive_elem>::iterator liter ;
	iter = associativity_table.find( nodeptr->get_construct_name()) ;
	if(iter != associativity_table.end() )
	{
		// make associativity shaping decision 
		for(liter = iter->second.begin() ; liter != iter->second.end(); liter++)
			if( liter->next_level_construct_name == childptr->get_construct_name())
			{
				UD_universal_data elem1 = nodeptr->get_node_attribute(liter->attr_name) ; // parent 
				UD_universal_data elem2 = childptr->get_node_attribute(liter->next_level_attr_name) ; // child 
		
				if( (elem1.get_type_tag() != T_unknown) && (elem2.get_type_tag() != T_unknown)){
					string value1 = *(((UD_String*)(&elem1))->getptr()) ;
					string value2 = *(((UD_String*)(&elem2))->getptr()) ;
					if(value1 == value2)
						return true ;
				}
			}
	}
	return false ;
}

pair<bool, string> generalized_parser::compute_precedence_shaping(node *nodeptr, int next_token_index)
{
	map<int, list<precedence_elem> >::iterator iter ;
	list<precedence_elem>::iterator liter ;
	bool precedence_flag ;
	string assoc_decision = "" ;
	int current_node_level, next_node_level ;
	// precedence shaping decision 
	// look ahead next token 
	if(next_token_index < (int)token_seq.size() && nodeptr!=NULL)
	{
		int next_token = token_seq[next_token_index]->token_id ;
		
		string kindtmp ;
		UD_universal_data tmp = nodeptr->get_node_attribute("binary_op") ;
		kindtmp = "binary_op" ;
		if(tmp.get_type_tag() == T_unknown){
			tmp = nodeptr->get_node_attribute("unary_op") ;
			kindtmp = "unary_op" ;
		}

		if(tmp.get_type_tag() != T_unknown) // node's operation is a unary or binary operation 
		{
			if(next_token == get_symbol_index("binary_op") || next_token == get_symbol_index("unary_op"))
			{
				precedence_flag = true ;
				precedence_elem current_tmp, next_tmp ;
				current_tmp.type = kindtmp ;
				current_tmp.elem = *(((UD_String*)(&tmp))->getptr()) ;
				if(next_token == get_symbol_index("binary_op"))
					next_tmp.type = "binary_op" ;
				else 
					next_tmp.type = "unary_op" ;
				next_tmp.elem = *( ((UD_String*)(&(token_seq[next_token_index]->token_specifier)))->getptr() ) ;

				for(iter = precedence_table.begin(); iter != precedence_table.end() ; iter++)
				{
					for(liter = iter->second.begin(); liter != iter->second.end() ; liter ++)
					{
						if( liter->type == current_tmp.type && liter->elem == current_tmp.elem)
							current_node_level = iter->first ;
						if( liter->type == next_tmp.type && liter->elem == next_tmp.elem) 
							next_node_level = iter->first ;
					}
				}

				if( (next_node_level > current_node_level) || (next_node_level== current_node_level))
					assoc_decision = "right_association" ;	// right association
				else
					assoc_decision = "left_association" ;		// left association
			}
			else
				precedence_flag = false  ; // next token is not operation 
		}
		else
			precedence_flag = false ; // no association
	}
	else// out of range 
		precedence_flag = false ;
	pair<bool, string> return_val(precedence_flag, assoc_decision) ;
	return return_val ; // return precedence data 
}

void generalized_parser::associativity_restructuring(node *nodeptr)
{
	// form a empty reshape vector 
	vector<node*> reshape_vector ;
	vector<node*> reclaim_vector ;

	UD_Virtual_Iterator* biter = nodeptr->get_node_branch_iter() ;
	for(biter->begin() ; !biter->end() ; biter->advance()){
		node* tmp = (node*)(*(*biter)) ;
		if(tmp->get_t_nt_flag() == 't') // the subtree is a leaf
		{
			// same structuring for a leaf subtree
			// add the root of the subtree into the reshpae vector 
			reshape_vector.push_back(tmp) ;
		}
		else if (!compute_associativity_shaping(nodeptr, tmp))
		{
			//same structuring for a subtree without associativity 
			// add the root of the subtree into the reshpe vector 
			reshape_vector.push_back(tmp) ;
		}
		else // flatten the subtree with associativity 
		{
			UD_Virtual_Iterator* biter2 = tmp->get_node_branch_iter() ;
			for(biter2->begin() ; !biter2->end() ; biter2->advance())
				reshape_vector.push_back( (node*)(*(*biter2))) ;
			reclaim_vector.push_back(tmp) ;
		}
	}
	
	// replace the reshape vector into the branch vector of the node pointed by nodeptr
	nodeptr->clear_branches() ;
	// reclaim 
	for(unsigned i = 0 ; i < reclaim_vector.size() ;i++)
		delete reclaim_vector[i] ;

	for(unsigned i = 0 ; i < reshape_vector.size() ; i++)
	{
		nodeptr->push_node_branch(reshape_vector[i]) ;
		reshape_vector[i]->push_parent_pointer(nodeptr) ; // setup parent pointer 
	}
}

node* generalized_parser::left_assoc_restructuring(node *nodeptr, int& next_token_index)
{
	// conditionally perform associativity 
	if (nodeptr != NULL)
		associativity_restructuring(nodeptr) ;

	// let current associated expression pointed by nodeptr as the 1st child of the new 
	//		parsed parent expression 
	map<string, general_construct_name>::iterator general_construct_iter ;
	general_construct_iter = general_construct_table.find(nodeptr->get_construct_name()) ;
	if(general_construct_iter == general_construct_table.end())
	{
		cout<<"error, can't find in the general construct table"<<endl ;
		exit(-1) ;
	}
	int construct_id = get_symbol_index( general_construct_iter->second.upper_level_construct_name) ;
	list<pair<string, unsigned*> > tmp_acc_vector ;
	int tmp_token_index ;
	bool match = shaped_unified_parse(construct_id, next_token_index, tmp_token_index, tmp_acc_vector, nodeptr) ;
	if(!match) // not matched
		return NULL ; // match fails in further expression parsing
	next_token_index = tmp_token_index ;
	return (node*)(tmp_acc_vector.begin()->second) ;
}

node* generalized_parser::right_assoc_restructuring(node *nodeptr, int& next_token_index)
{
	// detach the right child for right association 
	node* tmpptr = (node*)(nodeptr->get_node_branch(nodeptr->node_branch_size()-1)) ;
	nodeptr->pop_node_branch() ;
	// let the right subexpression pointed by nodeptr as the 1st child to the new parsed child
	//		expression
	map<string, general_construct_name>::iterator general_construct_iter ;
	general_construct_iter = general_construct_table.find(nodeptr->get_construct_name()) ;
	if(general_construct_iter == general_construct_table.end())
	{
		cout<<"error, can't find in the general construct table "<<nodeptr->get_construct_name()<<endl ;
		exit(-1) ;
	}
	int construct_id = get_symbol_index(general_construct_iter->second.lower_level_construct_name) ;
	list<pair<string, unsigned*> > tmp_acc_vector ;
	int tmp_token_index ;
	bool match = shaped_unified_parse(construct_id, next_token_index, tmp_token_index, tmp_acc_vector, tmpptr) ;
	if(!match) // not match 
		return NULL ; // match fails in further scanning 
	next_token_index = tmp_token_index ;
	// hook the right-associated expression in current node 
	node* child_ptr = (node*)(tmp_acc_vector.begin()->second) ;
	nodeptr->push_node_branch(child_ptr) ; // add child_ptr as the right child in teh node pointed by nodeptr l
	child_ptr->push_parent_pointer(nodeptr) ; // set up parent pointer 
	// conditionally perform associativity 
	if (nodeptr != NULL)
		associativity_restructuring(nodeptr) ;
	return nodeptr ;	// return current node 
}

void generalized_parser::set_nested_filtering(int construct_id)
{
	map<string, node_creation_rule_elem>::iterator iter ;
	string construct_name = get_construct_name(construct_id) ;
	iter = node_creation_table.find( construct_name) ;
	if(iter == node_creation_table.end())
	{
		cout<<"error, can't find node creation rule with "<< construct_name<<endl ;
		exit(-1) ;
	}
	// set nested filtering for lower levels
	if(iter->second.nest_print_flag == 'F')
		global_nest_creation_stack.push(false) ;
	else if (iter->second.nest_print_flag == 'R')
		global_nest_creation_stack.push(true) ;
}

void generalized_parser::restore_nested_filtering(int construct_id)
{
	map<string, node_creation_rule_elem>::iterator iter ;
	string construct_name = get_construct_name(construct_id) ;
	iter = node_creation_table.find( construct_name) ;
	if(iter == node_creation_table.end())
	{
		cout<<"error, can't find node creation rule with "<< construct_name<<endl ;
		exit(-1) ;
	}
	// restore upper construct's global_nest_creation 
	if( iter->second.nest_print_flag == 'F' || iter->second.nest_print_flag == 'R')
		global_nest_creation_stack.pop() ;
}

void generalized_parser::print()
{
	map<string, int>::iterator construct_iter ;
	vector<rule*>::iterator rule_iter ;
	vector<subrule*>::iterator srule_iter ;
	vector<element>::iterator eiter ;

	for(construct_iter = construct_table.begin() ; construct_iter!= construct_table.end() ; construct_iter++)
	{
		if(construct_iter->second < nt_num)
			cout<<"nonterminal: ( "<<construct_iter->first<<", "<<construct_iter->second<<") "<<endl ;
		else if(construct_iter->second < keyword_bound)
			cout<<"keyword: ( "<<construct_iter->first<<", "<<construct_iter->second<<") "<<endl ;
		else 
			cout<<"terminal: ( "<<construct_iter->first<<", "<<construct_iter->second<<") "<<endl ;
	}

	int j = 0 ;
	cout<<"grammar rule: "<<endl ;
	for(rule_iter = grammar_rules.begin() ; rule_iter != grammar_rules.end() ; rule_iter++)
	{
		cout<<"rule number "<<j<<" : "<<get_construct_name(j)<<endl ;
		for( int i = 0 ; i < (*rule_iter)->size() ; i++)
		{
			cout<<"subrule number: "<<i<<endl ;
			cout<<"element: "<<endl ;
			for(eiter = ((*(*rule_iter))[i])->element_vector.begin() ; eiter != ((*(*rule_iter))[i])->element_vector.end() ; eiter++)
			{
				cout<<" construct_name: "<<get_construct_name(eiter->t_nt_index) ;
				cout<<"	t_nt_flag: "<< eiter->t_nt_flag;
				cout<<"	t_nt_index: "<<eiter->t_nt_index ;
				cout<<" add_flag: "<<eiter->add_flag ;
				cout<<" star_flag: "<<eiter->star_flag<<endl ;	
			}
			cout<<"delimiter: "<< ((*(*rule_iter))[i])->delemiter<<endl ;
		}
		j++ ;
		cout<<"===\n"<<endl ;
	}
}
