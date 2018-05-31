#include "create_iterator_function.h"

using namespace std ;

// create iterator function 
Virtual_Iterator* create_iterator(universal_data* org)
{
	switch(org->get_type_tag())
	{
	case T_pair:
		return new Pair_Iterator(org) ;
		break ;
	case T_map:
		return new Map_Iterator(org) ;
		break; 
	case T_vector: 
		return new Vector_Iterator(org) ;
		break ;
	case T_set: 
		return new Set_Iterator(org) ;
		break ;
	case T_list:
		return new List_Iterator(org) ;
		break ;
	case T_multimap:
		return new Multimap_Iterator(org) ;
		break ;
	case T_attribute_list: 
		return new Attribute_List_Iterator(org) ;
		break ;
	case T_p_pair:
		return new p_Pair_Iterator(org) ;
		break ;
	case T_p_map:
		return new p_Map_Iterator(org) ;
		break ;
	case T_p_vector: 
		return new p_Vector_Iterator(org) ;
		break ;
	case T_p_set:
		return new p_Set_Iterator(org) ;
		break ;
	case T_p_list: 
		return new p_List_Iterator(org) ;
		break ;
	case T_user_defined: 
		if(org->get_user_defined_type_tag() == "associate_list")
			return new Associate_List_Iterator(org) ;
		else if (org->get_user_defined_type_tag() == "pointer_list")
			return new Pointer_List_Iterator(org) ;
		else 
			cout<<"not support this kind of iterator "<<endl ;
		break ;
	default:
		cout<<"not support this kind of iterator "<<endl ;
	}
	return NULL ;
}

