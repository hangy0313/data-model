#include "output_generation.h"

general_output_generation::general_output_generation(
     std::string node_print_rule_file,
     std::string global_print_table_file,
     std::string interleave_vector_file,
     std::string enclose_vector_file,
     std::string file_root_file,
     std::string indent_control_table_file,
     std::string condition_function_table_file)
{
	this->initialized_node_print_rule(node_print_rule_file) ;
	this->initialized_global_print_table(global_print_table_file) ;
	this->initialized_interleave_vector(interleave_vector_file) ;
	this->initialized_enclose_vector(enclose_vector_file) ;
	this->initialized_file_root(file_root_file) ;
	this->initialized_indent_control_table(indent_control_table_file) ;
	this->initialized_condition_function_table(condition_function_table_file) ;
	this->global_nest_print_flag.push(true) ;
	this->indent_counter = 0 ;
}

general_output_generation::~general_output_generation()
{
}

void general_output_generation::initialized_node_print_rule(std::string filename)
{
	// format: 4 kind 
	// 1. construct function_name func_name 
	// 2. construct attribute_name print_attr_name_flag attr_name
	// 3. construct string_value	"string"
	// 4. construct all_attribute print_attr_name_flag 

	ifstream infile ; 
	infile.open(filename.c_str() ) ;
	if(!infile)
	{
		cout<<"open "<<filename<<" fail"<<endl ;
		exit(-1) ;
	}

	string construct_name, kind, flag, name ;

	while(infile>>construct_name) 
	{
		infile>>kind ;

		if(kind == "function_name")
		{
			print_rule_elem tmp ;
			tmp.kind = Func_Name ;
			infile>>name ;
			tmp.name = name ;
			this->node_print_rule.insert(pair<string, print_rule_elem>(construct_name, tmp)) ;
		}
		else if(kind == "attribute_name")
		{
			infile>>flag ;
			infile>>name ;
			print_rule_elem tmp ;
			tmp.kind = Attr_Name ;
			if(flag == "T")
				tmp.attr_name_flag = true ;
			else if(flag == "F")
				tmp.attr_name_flag = false ;
			else
			{
				cout<<"unknown word in "<<filename<<endl ;
				exit(-1) ;
			}
			tmp.name = name ; 
			this->node_print_rule.insert(pair<string, print_rule_elem>(construct_name, tmp)) ;
		}
		else if(kind == "string_value")
		{
			string stmp ;
			infile>>stmp ; 
			if(stmp[0] !='"')
			{
				cout<<"initialized node print rule error"<<endl ;
				exit(-1) ;
			}
			stmp.erase(stmp.begin()) ;
			name = stmp ;
			if(stmp[stmp.size()-1] == '"') // empty string and one string case 
			{
				stmp.erase(stmp.size()-1) ;
				if(stmp.size() == 0)
					name = "" ;
				else
					name = stmp ;
			}
			else // for multiple string case 
			{
				infile>>stmp ; 
				while( stmp[stmp.size()-1]!='"')
				{
					name = name + " " + stmp ;
					infile>>stmp ;
				}
				stmp.erase(stmp.size()-1) ;
				name = name + " " + stmp ;
			}
			print_rule_elem tmp ;
			tmp.kind = String_Value; 
			tmp.name = name ;
			this->node_print_rule.insert(pair<string, print_rule_elem>(construct_name, tmp)) ;
		}
		else if (kind == "all_attribute")
		{
			infile>>flag; 
			print_rule_elem tmp ;
			tmp.kind = All_Attribute ;
			if(flag == "T")
				tmp.attr_name_flag = true ;
			else if(flag == "F")
				tmp.attr_name_flag = false ;
			this->node_print_rule.insert(pair<string, print_rule_elem>(construct_name, tmp)) ;
		}
		else 
		{
			cout<<"unknown word in "<<filename<<endl ;
			exit(-1) ;
		}
	}
}

void general_output_generation::initialized_global_print_table(std::string filename)
{
	// format: construct	nest_print_flag		traversal kind 
	ifstream infile ;
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file "<<filename<<" fail"<<endl ;
		exit(-1) ;
	}
	string name ;
	char print_flag;
	string kind ;

	while(infile>>name)
	{
		infile>>print_flag; 
		infile>>kind ;

		if(print_flag!='T'&&print_flag!='F'&&print_flag!='R'&&print_flag!='O'&&print_flag!='P'){
			cout<<"unknow word in "<<filename<<endl ;
			exit(-1) ;
		}
		info_elem e ;
		e.nest_print_flag = print_flag ; 
		if(kind == "pre")
			e.kind = tk_preorder; 
		else if(kind == "in")
			e.kind = tk_inorder ;
		else if(kind == "post")
			e.kind = tk_postorder ;
		else
		{
			cout<<"unknow word in "<<filename<<endl ;
			exit(-1) ;
		}
		global_print_table.insert(pair<string, info_elem>(name, e) ) ;
	}
}

void general_output_generation::initialized_interleave_vector(std::string filename)
{
	// format: construct [element, [element] ] ;
	// element: "string"
	// "" means empty string 
	ifstream infile ;
	infile.open(filename.c_str() ) ;
	if(!infile)
	{
		cout<<"open file "<<filename<<" fail"<<endl ;
		exit(-1) ;
	}
	string name, tmp ;
	map<string, vector<string> >::iterator iter ;
	while(infile>>name)
	{
		iter = NT_interleave_vector.find(name) ;
		if (iter != NT_interleave_vector.end() )
		{
			cout<<"the word is used "<<endl ;
			exit(-1) ;
		}
		vector<string> empty ;
		NT_interleave_vector.insert( pair<string, vector<string> >(name, empty) ) ;
		iter = NT_interleave_vector.find(name) ;

		infile>>tmp ;
		while(!infile.eof() && tmp != ";")
		{
			if(tmp[0] != '"')
			{
				cout<<"initialize interleave error: format error"<<endl;
				exit(-1) ;
			}
			else
			{
				string tmp1 ;
				tmp.erase(tmp.begin()) ;
				tmp1 = tmp ;
				if(tmp[tmp.size()-1] == '"') // empty or single string 
				{
					tmp.erase(tmp.size()-1) ;
					if(tmp.size() == 0 )
						tmp1 = "" ;
					else 
						tmp1 = tmp ;
				}
				else // multiple string 
				{
					infile>>tmp ; 
					while( tmp[tmp.size()-1]!='"')
					{
						tmp1 = tmp1 + " " + tmp ;
						infile>>tmp ;
					}
					tmp.erase(tmp.size()-1) ;
					tmp1 = tmp1 + " " + tmp ;
				}
			}

			iter->second.push_back(tmp) ;
			infile>>tmp ;
		}
	}
}

void general_output_generation::initialized_enclose_vector(std::string filename)
{
	// format: construct enclose enclose 
	// enclose: $: empty, ( ), [ ] , { } 
	ifstream infile ;
	infile.open(filename.c_str() ) ;
	if(!infile)
	{
		cout<<"open file "<<filename<<" fail"<<endl ;
		exit(-1) ;
	}
	map<string, pair<string, string> >::iterator iter ;
	string name, tmp1, tmp2 ;

	while(infile>>name)
	{
		iter = enclose_vector.find(name) ;
		if(iter != enclose_vector.end()){
			cout<<"element is existed"<<endl ;
			exit(-1) ;
		}
		infile>>tmp1 ;
		if(tmp1 == "$")
			tmp1 = "" ;
		infile>>tmp2 ;
		if(tmp2 == "$")
			tmp2 = "" ;
		pair<string, string> tmp(tmp1, tmp2) ;
		enclose_vector.insert(pair<string, pair<string, string> >(name, tmp) ) ;
	}
}

void general_output_generation::initialized_file_root(std::string filename)
{
	// format: construct 
	ifstream infile ;
	infile.open(filename.c_str() ) ;
	if(!infile)
	{
		cout<<"open file "<<filename<<" fail"<<endl ;
		exit(-1) ;
	}
	string name; 
	while(infile>>name)
	{
		list<string>::iterator iter ;
		for(iter = file_root_list.begin(); iter!= file_root_list.end() ;iter++)
			if(*iter == name)
				break ;
		if(iter != file_root_list.end())
		{
			cout<<"file root duplicate"<<endl ;
			exit(-1) ;
		}
		file_root_list.push_back(name) ;
	}
}

void general_output_generation::initialized_indent_control_table(string filename) 
{
	// format: construct_name indent_num newline
	ifstream infile ;
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<< filename<<endl ;
		exit(-1) ;
	}
	string construct_name, newline ;
	int indent ;

	while(infile>>construct_name) 
	{
		infile>>indent ;
		infile>>newline ;

		control_info tmp ;
		tmp.indent_num = indent ;
		if(newline == "true")
			tmp.newline = true ;
		else if (newline == "false")
			tmp.newline = false;
		else 
		{
			cout<<"unknown word"<<endl ;
			exit(-1) ;
		}
		indent_control_table.insert(pair<string, control_info>(construct_name, tmp) ) ;
	}
}

void general_output_generation::initialized_condition_function_table(string filename) 
{
	// format: construct_name function_name 
	ifstream infile ;
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail: "<<filename<<endl ;
		exit(-1) ;
	}
	string construct_name, function_name ;
	while(infile>>construct_name)
	{
		infile>>function_name ;
		cfptr_mapping_table.insert(pair<string, string>(construct_name, function_name) ) ;
	}
}

void general_output_generation::set_nested_printing(node *nodeptr)
{
	map<string, info_elem>::iterator iter ;
	iter = global_print_table.find(nodeptr->get_construct_name()) ;
	if(iter == global_print_table.end()){
		cout<<"set nested printing error"<<endl ;
		exit(-1) ;
	}
	char print_flag = iter->second.nest_print_flag ;
	if(print_flag == 'F')
		global_nest_print_flag.push(false) ;
	else if(print_flag == 'R')
		global_nest_print_flag.push(true) ;
}

void general_output_generation::restore_nested_printing(node *nodeptr)
{
	map<string, info_elem>::iterator iter ;
	iter = global_print_table.find(nodeptr->get_construct_name()) ;
	if(iter == global_print_table.end()){
		cout<<"restore nested printing error"<<endl ;
		exit(-1) ;
	}
	char print_flag = iter->second.nest_print_flag ;
	if(print_flag == 'F' || print_flag == 'R')
		global_nest_print_flag.pop() ;
}

traversal_kind general_output_generation::find_traversal_kind(node *nodeptr)
{
	map<string, info_elem>::iterator iter ;
	iter = global_print_table.find(nodeptr->get_construct_name()) ;
	if(iter == global_print_table.end()){
		cout<<"find traverse kind error"<<nodeptr->get_construct_name()<<endl ;
		exit(-1) ;
	}
	return iter->second.kind ;
}

print_rule_elem general_output_generation::get_print_rule_elem(node *nodeptr)
{
	map<string, print_rule_elem>::iterator iter ;
	iter = node_print_rule.find(nodeptr->get_construct_name()) ;
	if(iter == node_print_rule.end())
	{
		cout<<"get print rule elem error"<<nodeptr->get_construct_name()<<endl ;
		exit(-1) ;
	}
	return iter->second ;
}

pair<string, string> general_output_generation::get_enclose(node *nodeptr)
{
	map<string, pair<string, string> >::iterator iter ;
	iter = enclose_vector.find(nodeptr->get_construct_name() ) ;
	if(iter == enclose_vector.end())
	{
		cout<<"get enclose error"<<nodeptr->get_construct_name()<<endl ;
		exit(-1) ;
	}
	return iter->second ;
}

void general_output_generation::output_generation(node *nodeptr)
{
	traversal_kind kind = find_traversal_kind(nodeptr) ;
	if(kind == tk_preorder) 
		return traverse_preorder(nodeptr) ;
	else if (kind == tk_postorder)
		return traverse_postorder(nodeptr) ;
	else // inorder
		return traverse_inorder(nodeptr) ;
}

void general_output_generation::traverse_preorder(node *nodeptr)
{
	map<string, vector<string> >::iterator interleave_iter ;
	interleave_iter = NT_interleave_vector.find(nodeptr->get_construct_name()) ;
	map<string, pair<string, string> >::iterator enclose_iter ;
	enclose_iter = enclose_vector.find(nodeptr->get_construct_name()) ;
	// find the stream 
	list<pair<string, string> >::iterator stream_iter ;
	list<string>::iterator file_root_iter ;
	for(file_root_iter = file_root_list.begin() ; file_root_iter != file_root_list.end(); file_root_iter++)
		if( (*file_root_iter) == nodeptr->get_construct_name())
			break ;
	if(file_root_iter != file_root_list.end())
	{
		string tmp_stream ;
		string filename ;
		stringstream ss ;
		ss<<nodeptr->get_construct_name() ;
		ss<<((void*)nodeptr) ;
		ss>>filename ;
//		char* tmp = new char[20] ;
//		sprintf(tmp, "%d", nodeptr) ;
//		filename = nodeptr->get_construct_name() + tmp ;
		stream_table.push_back(pair<string, string>(filename, tmp_stream)) ;
		if(!stream_stack.empty()) // not the first root string
		{// set up symbol output 
			list< pair<string,string> >::iterator tmp_iter ;
			for(tmp_iter = stream_table.begin(); tmp_iter != stream_table.end(); tmp_iter++)
				if(tmp_iter->first == stream_stack.top()) 
					break ;
			tmp_iter->second = tmp_iter->second + " sym_link " + filename+" " ;
		}
		stream_stack.push(filename) ;
	}
	for(stream_iter = stream_table.begin(); stream_iter != stream_table.end(); stream_iter++)
		if(stream_iter->first == stream_stack.top()) 
			break ;
	int count ;
	// find print flag 
	char nest_print_flag = global_print_table.find(nodeptr->get_construct_name())->second.nest_print_flag ;
	bool print_flag = (global_nest_print_flag.top() == true || nest_print_flag == 'O') ;
	// set nest printing
	set_nested_printing(nodeptr) ;

	// set up indent 
	map<string, control_info>::iterator indent_iter ;
	indent_iter = indent_control_table.find(nodeptr->get_construct_name()) ;
	if(indent_iter != indent_control_table.end())
	{
		indent_counter = indent_counter + indent_iter->second.indent_num ;
	}

	// print current node 
	if( print_flag)
	{
		// set up count 
		map<string, int>::iterator acc_iter ;
		acc_iter = accumulated_map.find(nodeptr->get_construct_name()) ;
		if(acc_iter == accumulated_map.end())
		{
			count = 0 ;
			accumulated_map.insert(pair<string, int>(nodeptr->get_construct_name(), count)) ;
		}
		else
		{
			acc_iter->second++ ;
			count = acc_iter->second ;
		}
		print_node_info(nodeptr, count) ;
	}
	// branch
	if(nest_print_flag != 'P')
	{
//		if(nodeptr->get_branches_size() != 0)
        if(print_flag)
            if(enclose_iter != enclose_vector.end())
                if(enclose_iter->second.first!="")
                    stream_iter->second = stream_iter->second + enclose_iter->second.first +"" ;

		if(indent_iter != indent_control_table.end()
           && nodeptr->get_construct_name() != "class_content")
		{
			if(indent_iter->second.newline) // newline 
				stream_iter->second = stream_iter->second + "\n" ;
			int i  = 0; 
			while(i<indent_counter)
			{
				stream_iter->second = stream_iter->second + " " ;
				i++ ;
			}
		}

		for(int i = 0 ; i < (int)(nodeptr->node_branch_size()) ; i++)
        {
			if (i >=(int)interleave_iter->second.size()-1 && i != (int)(nodeptr->node_branch_size())-1)
			{
				if(interleave_iter->second[interleave_iter->second.size()-2] == "")
					stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-2] ;
				else
					stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-2] + " " ;
			}
			else if (i >=(int)interleave_iter->second.size())
			{
				if(interleave_iter->second[interleave_iter->second.size()-2] == "")
					stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-2] ;
				else
					stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-2] + " " ;
			}
			else 
			{
				if(interleave_iter->second[i] == "")
					stream_iter->second = stream_iter->second + interleave_iter->second[i] ;
				else
					stream_iter->second = stream_iter->second + interleave_iter->second[i] + " " ;
			}
            //for public in class_content
            if(i == 0 && nodeptr->get_construct_name() == "class_content"){
                stream_iter->second = stream_iter->second + "\n" + "public:";
            }
            
			if(i>0)
			{
				if(indent_iter != indent_control_table.end())
				{
					if(indent_iter->second.newline) // newline 
						stream_iter->second = stream_iter->second + "\n" ;
					int i  = 0; 
					while(i<indent_counter)
					{
						stream_iter->second = stream_iter->second + " " ;
						i++ ;
					}
				}
			}
			output_generation( (node*)(nodeptr->get_node_branch(i))) ;
		}
		if( nodeptr->node_branch_size() != 0) 
		{
			if(interleave_iter->second[interleave_iter->second.size()-1] == "")
				stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-1] ;
			else
				stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-1] + " " ;
			if(indent_iter != indent_control_table.end())
			{
				if(indent_iter->second.newline) // newline 
					stream_iter->second = stream_iter->second + "\n" ;

				// reset indent counter
				indent_counter = indent_counter - indent_iter->second.indent_num ;

				int i  = 0; 
				while(i<indent_counter)
				{
					stream_iter->second = stream_iter->second + " " ;
					i++ ;
				}
			}
		}
		else 
		{
			// reset indent counter
			if(indent_iter != indent_control_table.end())
				indent_counter = indent_counter - indent_iter->second.indent_num ;
		}

//		if(nodeptr->get_branches_size() != 0)
			if(print_flag)
				if(enclose_iter != enclose_vector.end())
					if(enclose_iter->second.first!="")
						stream_iter->second = stream_iter->second + enclose_iter->second.second +" " ;
	}
	else 
	{
		// reset indent counter
		if(indent_iter != indent_control_table.end())
		{
	//		if(indent_iter->second.newline) // newline 
	//			stream_iter->second = stream_iter->second + "\n" ;
			indent_counter = indent_counter - indent_iter->second.indent_num ;
		}
	}
	
	// pop stream stack, if push it 
	if(file_root_iter != file_root_list.end())
		stream_stack.pop() ;
	//restore nest printing
	restore_nested_printing(nodeptr) ;
}

void general_output_generation::traverse_postorder(node *nodeptr)
{
	map<string, vector<string> >::iterator interleave_iter ;
	interleave_iter = NT_interleave_vector.find(nodeptr->get_construct_name()) ;
	// find the stream 
	list<pair<string, string> >::iterator stream_iter ;
	list<string>::iterator file_root_iter ;
	for(file_root_iter = file_root_list.begin() ; file_root_iter != file_root_list.end(); file_root_iter++)
		if( (*file_root_iter) == nodeptr->get_construct_name())
			break ;
	if(file_root_iter != file_root_list.end())
	{
		string tmp_stream ;
		string filename ;
		stringstream ss ;
		ss<<nodeptr->get_construct_name() ;
		ss<<((void*)nodeptr) ;
		ss>>filename ;
//		char* tmp = new char[20] ;
//		sprintf(tmp, "%d", nodeptr) ;
//		filename = nodeptr->get_construct_name() + tmp ;
		stream_table.push_back(pair<string, string>(filename, tmp_stream)) ;
		if(!stream_stack.empty()) // not the first root string
		{// set up symbol output 
			list< pair<string,string> >::iterator tmp_iter ;
			for(tmp_iter = stream_table.begin(); tmp_iter != stream_table.end(); tmp_iter++)
				if(tmp_iter->first == stream_stack.top()) 
					break ;
			tmp_iter->second = tmp_iter->second + " sym_link " + filename+" " ;
		}
		stream_stack.push(filename) ;
	}
	for(stream_iter = stream_table.begin(); stream_iter != stream_table.end(); stream_iter++)
		if(stream_iter->first == stream_stack.top()) 
			break ;
	int count ;
	// find print flag 
	char nest_print_flag = global_print_table.find(nodeptr->get_construct_name())->second.nest_print_flag ;
	bool print_flag = (global_nest_print_flag.top() == true || nest_print_flag == 'O') ;
	// set nest printing
	set_nested_printing(nodeptr) ;

	// set up indent 
	map<string, control_info>::iterator indent_iter ;
	indent_iter = indent_control_table.find(nodeptr->get_construct_name()) ;
	if(indent_iter != indent_control_table.end())
	{
		indent_counter = indent_counter + indent_iter->second.indent_num ;
		int i  = 0; 
		while(i<indent_counter)
		{
			stream_iter->second = stream_iter->second + " " ;
			i++ ;
		}
	}

	// branch
	if(nest_print_flag != 'P')
	{
		for(int i = 0 ; i < (int)( nodeptr->node_branch_size()) ; i++)
		{
			if(interleave_iter->second[i] == "")
				stream_iter->second = stream_iter->second + interleave_iter->second[i] ;
			else
				stream_iter->second = stream_iter->second + interleave_iter->second[i] + " " ;
			output_generation( (node*)(nodeptr->get_node_branch(i))) ;
		}
		if( nodeptr->node_branch_size() != 0) 
		{
			if(interleave_iter->second[interleave_iter->second.size()-1] == "")
				stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-1] ;
			else
				stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-1] + " " ;
		}
	}
	// print current node 
	if( print_flag)
	{
		// set up count 
		map<string, int>::iterator acc_iter ;
		acc_iter = accumulated_map.find(nodeptr->get_construct_name()) ;
		if(acc_iter == accumulated_map.end())
		{
			count = 0 ;
			accumulated_map.insert(pair<string, int>(nodeptr->get_construct_name(), count)) ;
		}
		else
		{
			acc_iter->second++ ;
			count = acc_iter->second ;
		}
		print_node_info(nodeptr, count) ;
	}

	// reset indent counter
	if(indent_iter != indent_control_table.end())
	{
		if(indent_iter->second.newline) // newline 
			stream_iter->second = stream_iter->second + "\n" ;
		indent_counter = indent_counter - indent_iter->second.indent_num ;
	}
	// pop stream stack, if push it 
	if(file_root_iter != file_root_list.end())
		stream_stack.pop() ;
	//restore nest printing
	restore_nested_printing(nodeptr) ;
}

void general_output_generation::traverse_inorder(node *nodeptr)
{
	map<string, vector<string> >::iterator interleave_iter ;
	interleave_iter = NT_interleave_vector.find(nodeptr->get_construct_name()) ;
	// find the stream 
	list<pair<string, string> >::iterator stream_iter ;
	list<string>::iterator file_root_iter ;
	for(file_root_iter = file_root_list.begin() ; file_root_iter != file_root_list.end(); file_root_iter++)
		if( (*file_root_iter) == nodeptr->get_construct_name())
			break ;
	if(file_root_iter != file_root_list.end())
	{
		string tmp_stream ;
		string filename ;
		stringstream ss ;
		ss<<nodeptr->get_construct_name() ;
		ss<<((void*)nodeptr) ;
		ss>>filename ;
//		char* tmp = new char[20] ;
//		sprintf(tmp, "%d", nodeptr) ;
//		filename = nodeptr->get_construct_name() + tmp ;
		stream_table.push_back(pair<string, string>(filename, tmp_stream)) ;
		if(!stream_stack.empty()) // not the first root string
		{// set up symbol output 
			list< pair<string,string> >::iterator tmp_iter ;
			for(tmp_iter = stream_table.begin(); tmp_iter != stream_table.end(); tmp_iter++)
				if(tmp_iter->first == stream_stack.top()) 
					break ;
			tmp_iter->second = tmp_iter->second + " sym_link " + filename+" " ;
		}
		stream_stack.push(filename) ;
	}
	for(stream_iter = stream_table.begin(); stream_iter != stream_table.end(); stream_iter++)
		if(stream_iter->first == stream_stack.top()) 
			break ;
	int count ;
	// find print flag 
	char nest_print_flag = global_print_table.find(nodeptr->get_construct_name())->second.nest_print_flag ;
	bool print_flag = (global_nest_print_flag.top() == true || nest_print_flag == 'O') ;
	// set nest printing
	set_nested_printing(nodeptr) ;

	// set up indent 
	map<string, control_info>::iterator indent_iter ;
	indent_iter = indent_control_table.find(nodeptr->get_construct_name()) ;
	if(indent_iter != indent_control_table.end())
	{
		indent_counter = indent_counter + indent_iter->second.indent_num ;
		int i  = 0; 
		while(i<indent_counter)
		{
			stream_iter->second = stream_iter->second + " " ;
			i++ ;
		}
	}

	// condition check whether output ( or )
	bool flag = false ;
	map<string, string>::iterator iter ;
	iter = cfptr_mapping_table.find(nodeptr->get_construct_name()) ;
	if (iter != cfptr_mapping_table.end() )
	{
		map<string, bool(*)(node*)>::iterator fptr_iter ;
		fptr_iter = condition_function.find(iter->second) ;
		if(fptr_iter == condition_function.end())
		{
			cout<<"condition function is not exist"<<endl ;
			exit(-1) ;
		}
		flag = fptr_iter->second(nodeptr) ;
	}

	if(flag)
		stream_iter->second = stream_iter->second + "( " ;

	// branch
	if(nest_print_flag != 'P')
	{
		for(int i = 0 ; i < (int)(nodeptr->node_branch_size()) ; i++)
		{
			if(interleave_iter->second[i] == "")
				stream_iter->second = stream_iter->second + interleave_iter->second[i] ;
			else
				stream_iter->second = stream_iter->second + interleave_iter->second[i] + " " ;
			output_generation( (node*)(nodeptr->get_node_branch(i))) ;
			if(i < (int)(nodeptr->node_branch_size()-1))
			{
				// print current node 
				if( print_flag)
				{
					// set up count 
					map<string, int>::iterator acc_iter ;
					acc_iter = accumulated_map.find(nodeptr->get_construct_name()) ;
					if(acc_iter == accumulated_map.end())
					{
						count = 0 ;
						accumulated_map.insert(pair<string, int>(nodeptr->get_construct_name(), count)) ;
					}
					else
					{
						acc_iter->second++ ;
						count = acc_iter->second ;
					}
					print_node_info(nodeptr, count) ;
				}
			}
		}
		if( nodeptr->node_branch_size() != 0) 
		{
			if(interleave_iter->second[interleave_iter->second.size()-1] == "")
				stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-1] ;
			else
				stream_iter->second = stream_iter->second + interleave_iter->second[interleave_iter->second.size()-1] + " " ;
		}
	}

	if(flag)
		stream_iter->second = stream_iter->second + ") " ;

	// reset indent counter
	if(indent_iter != indent_control_table.end())
	{
		if(indent_iter->second.newline) // newline 
			stream_iter->second = stream_iter->second + "\n" ;
		indent_counter = indent_counter - indent_iter->second.indent_num ;
	}
	// pop stream stack, if push it 
	if(file_root_iter != file_root_list.end())
		stream_stack.pop() ;
	//restore nest printing
	restore_nested_printing(nodeptr) ;
}

void general_output_generation::print_node_info(node *nodeptr, int count)
{
	map<string, pair<void(*)(string&, node*, general_output_generation*), void(*)(string&, node*, general_output_generation*)> >::iterator func_iter ;
	func_iter = this->node_print_function_table.find(nodeptr->get_construct_name()) ;
	if(func_iter == node_print_function_table.end())
	{
		cout<<"print node info error "<<nodeptr->get_construct_name()<<endl ;
		exit(-1) ;
	}
	list<pair<string, string> >::iterator stream_iter ;
	for(stream_iter = stream_table.begin(); stream_iter != stream_table.end(); stream_iter++)
		if(stream_iter->first == stream_stack.top())
			break ;
	if(stream_iter == stream_table.end())
	{
		cout<<"print node info error: stream error"<<endl ;
		exit(-1) ;
	}
	if(count == 0)
		func_iter->second.first(stream_iter->second, nodeptr,this) ;
	else 
		func_iter->second.second(stream_iter->second, nodeptr,this) ;
}

void general_output_generation::set_fptr(std::string func_name, void (*fptr)(std::string &, node *))
{
	map<string, void(*)(string&, node*)>::iterator iter ;
	iter = this->fptr_table.find(func_name) ;
	if(iter != this->fptr_table.end())
	{
		cout<<"set fptr error"<<endl ;
		exit(-1) ;
	}
	fptr_table.insert(pair<string, void(*)(string&, node*)>(func_name, fptr)) ;
}

void (*general_output_generation::get_fptr(string func_name))(string&, node*)  // get the function pointer
{
	map<string, void(*)(string&, node*)>::iterator iter ;
	iter = this->fptr_table.find(func_name) ;
	if(iter != this->fptr_table.end())
		return iter->second ;
	else 
	{
		cout<<"not found"<<endl ;
		exit(-1) ;
	}
}

void general_output_generation::remove_fptr(std::string func_name)
{
	map<string, void(*)(string&, node*)>::iterator iter ;
	iter = this->fptr_table.find(func_name) ;
	if(iter != this->fptr_table.end())
		fptr_table.erase(func_name) ;
}

void general_output_generation::set_node_print_function(std::string construct, void (*fptr1)(std::string &, node*, general_output_generation*), void (*fptr2)(std::string &, node*, general_output_generation*))
{
	map<string, pair<void(*)(string&, node*, general_output_generation*), void(*)(string&, node*, general_output_generation*)> >::iterator iter ;
	iter = this->node_print_function_table.find(construct) ;
	if(iter != node_print_function_table.end())
	{
		cout<<"duplicate set up"<<endl ;
		exit(-1) ;
	}
	pair<void(*)(string&, node*, general_output_generation*), void(*)(string&, node*, general_output_generation*)> fpair(fptr1,fptr2) ;
	node_print_function_table.insert( pair<string, pair<void(*)(string&, node*, general_output_generation*), void(*)(string&, node*, general_output_generation*)> >(construct, fpair) ) ;
}

void general_output_generation::remove_node_print_funciton(std::string construct)
{
	map<string, pair<void(*)(string&, node*, general_output_generation*), void(*)(string&, node*, general_output_generation*)> >::iterator iter ;
	iter = this->node_print_function_table.find(construct) ;
	if(iter != node_print_function_table.end())
		node_print_function_table.erase(construct) ;
}

void general_output_generation::reset_stream_table()
{
	this->stream_table.clear() ;
}

void general_output_generation::output_file(bool single_file)
{
	list<pair<string, string> >::iterator iter ;
	ofstream outfile ;
	string filename ;
	
	if(single_file) 
	{
		iter = stream_table.begin() ;
		if(iter == stream_table.end())
			return ;
		filename = "output.cpp" ;
		outfile.open( filename.c_str()) ;
		if(!outfile)
		{
			cout<<"open file fail"<<endl ;
			exit(-1) ;
		}
		for(iter = stream_table.begin() ; iter != stream_table.end(); iter++)
		{
	//		outfile<<iter->first<<" \n" ;
			outfile<<iter->second<<" \n" ;
			outfile<<"\n" ;
		}
		outfile.close() ;
	}
	else
	{
		for(iter = stream_table.begin() ; iter != stream_table.end(); iter++)
		{
			filename = iter->first+".txt" ;
			outfile.open( filename.c_str()) ;
			if(!outfile){
				cout<<"open file fail"<<endl ;
				exit(-1) ;
			}
	//		outfile<<iter->first<<" \n" ;
			outfile<<iter->second<<" \n" ;
			outfile.close() ;
		}
	}
}

void general_output_generation::output_file(char* filename)
{
	list<pair<string, string> >::iterator iter ;
	ofstream outfile ;
	outfile.open(filename) ;
	if(!outfile)
	{
		cout<<"open file fail "<<filename<<endl ;
		exit(-1) ;
	}
	
	for(iter = stream_table.begin() ; iter != stream_table.end(); iter++)
	{
		outfile<<iter->second<<" \n" ;
		outfile<<"\n" ;
	}
	outfile.close() ;
	
}

// free function 
void print_universal_data(string& stream, universal_data ud)
{
	if(ud.get_type_tag() == T_int)
	{
		stringstream ss ;
		ss<<*(((Int*)(&ud))->getptr()) ;
		string tmp ;
		ss>>tmp  ;
//		char* tmp = new char[20] ;
//		sprintf(tmp, "%d", *(((Int*)(&ud))->getptr())) ;
		stream = stream+tmp+" " ;
	}
	else if (ud.get_type_tag() == T_char)
		stream = stream +  *(((Char*)(&ud))->getptr()) + " " ;
	else if (ud.get_type_tag() == T_string)
		stream = stream + *(((String*)(&ud))->getptr()) + " " ;
	else if (ud.get_type_tag() == T_float)
	{
		stringstream ss ;
		ss<<*(((Float*)(&ud))->getptr()) ;
		string tmp ;
		ss>>tmp  ;
//		char* tmp = new char[20] ;
//		sprintf(tmp, "%f", *(((Float*)(&ud))->getptr())) ;
		stream = stream+tmp+" " ;
	}
	else if (ud.get_type_tag() == T_double)
	{
		stringstream ss ;
		ss<<*(((Double*)(&ud))->getptr()) ;
		string tmp ;
		ss>>tmp  ;
//		char* tmp = new char[20] ;
//		sprintf(tmp, "%d", *(((Double*)(&ud))->getptr())) ;
		stream = stream+tmp+" " ;
	}
	else 
		cout<<"unknown" ;
}

void print_node_info_func(string& stream, node* nodeptr, general_output_generation* gog)
{
	print_rule_elem elem = gog->get_print_rule_elem(nodeptr) ;
	pair<string, string> enclose = gog->get_enclose(nodeptr) ;

	if(elem.kind == Func_Name)
	{
		void (*fptr)(string&, node*) = NULL ;
		fptr = gog->get_fptr(elem.name) ;
		fptr(stream, nodeptr) ;
	}
	else if (elem.kind == Attr_Name)
	{
		universal_data tmp = nodeptr->get_node_attribute( elem.name) ;
		if(tmp.get_type_tag() == T_unknown) 
		{
			cout<<"this attribute "<<elem.name<<" is not existed"<<endl ;
			exit(-1) ;
		}
		if(enclose.first!="")
			stream = stream + enclose.first + " " ;
		if(elem.attr_name_flag)
			stream = stream + elem.name +" " ;
		print_universal_data(stream, tmp) ;
		if(enclose.second!="")
			stream = stream + enclose.second + " " ;
	}
	else if (elem.kind == String_Value)
	{
		if(elem.name != "")
			stream = stream + elem.name + " " ;
	}
	else // all attribute 
	{
		Associate_List_Iterator* aiter = (Associate_List_Iterator*)(nodeptr->get_node_attribute_iter()) ;
		aiter->begin() ;
		while(!aiter->end())
		{
			if(enclose.first!="")
				stream = stream + enclose.first + " " ;
			if(elem.attr_name_flag)
				stream = stream + aiter->first() + " " ;
			print_universal_data(stream, aiter->second()) ;
			if(enclose.second!="")
				stream = stream + enclose.second + " " ;
			aiter->advance() ;
			if(!aiter->end())
				stream = stream + ", " ;
		}
	}
}

void general_output_generation::set_condition_fptr(string func_name, bool (*fptr)(node*)) 
{
	map<string, bool(*)(node*)>::iterator iter ;
	iter = this->condition_function.find(func_name) ;
	if(iter!= this->condition_function.end())
	{
		cout<<"error, function name is exist"<<endl ;
		exit(-1) ;
	}
	condition_function.insert(pair<string, bool(*)(node*)>(func_name, fptr) );
}

bool (*general_output_generation::get_condition_fptr(std::string func_name))(node*)
{
	map<string, bool(*)(node*)>::iterator iter ;
	iter = this->condition_function.find(func_name) ;
	if(iter != this->condition_function.end())
		return iter->second ;
	else 
	{
		cout<<"not found"<<endl ;
		exit(-1) ;
	}
}

//////////////////////////////////////////////////////////////////////////////////
void initialized_precedence_table(std::string filename)
{
	// format: level elem1 [ elem2 ... ] ;
	ifstream infile ;
	int level ;
	string type, elem ;
	map<int, list<precedence_elem> >::iterator iter ;
	
	// open file 
	infile.open(filename.c_str()) ;
	if(!infile)
	{
		cout<<"open file fail"<<endl ;
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
/////////////////
bool less_than(node*nodeptr)
{
	node* parent_ptr = (node*)(nodeptr->get_node_parent_pointer()) ;
	if(parent_ptr == NULL) 
		return false ;
	// check whether parent ptr is binary operator or unary operator 
	string parent_kind, child_kind ;
	universal_data elem1 = parent_ptr->get_node_attribute("binary_op") ;
	parent_kind = "binary_op" ;
	if(elem1.get_type_tag() == T_unknown){
		elem1 = parent_ptr->get_node_attribute("unary_op") ;
		parent_kind = "unary_op" ;
	}
	if(elem1.get_type_tag() == T_unknown)
		return false ;
	if(parent_kind == "unary_op")
		return true ;

	universal_data elem2 = nodeptr->get_node_attribute("binary_op") ;
	child_kind = "binary_op" ;
	if(elem2.get_type_tag() == T_unknown){
		elem2 = nodeptr->get_node_attribute("unary_op") ;
		child_kind = "unary_op" ;
	}
	if(elem2.get_type_tag() == T_unknown)
		return false ;



	precedence_elem parent_tmp, child_tmp ;
	int level_parent, level_child ;
	map<int, list<precedence_elem> >::iterator iter ;
	list<precedence_elem>::iterator liter ;

	parent_tmp.type = parent_kind ;
	parent_tmp.elem = *(((String*)(&elem1))->getptr()) ;
	child_tmp.type = child_kind ;
	child_tmp.elem = *(((String*)(&elem2))->getptr()) ;

	for(iter = precedence_table.begin(); iter != precedence_table.end() ; iter++)
	{
		for(liter = iter->second.begin(); liter != iter->second.end() ; liter ++)
		{
			if( liter->type == parent_tmp.type && liter->elem == parent_tmp.elem)
				level_parent = iter->first ;
			if( liter->type == child_tmp.type && liter->elem == child_tmp.elem) 
				level_child = iter->first ;
		}
	}

	if(level_parent<level_child)
		return true ;
	else 
		return false ;
	
}
