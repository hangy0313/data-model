//###################################################
// define enumeration count of type tag 
//###################################################
#ifndef ENUMERATE_CONSTANT_H
#define ENUMERATE_CONSTANT_H

// elem_type_tag 
enum Type_Tag {T_unknown, T_unlimited, T_symbolic_link, 
							T_int, T_string, T_char, T_float, T_double, T_bool, T_incidence,
							T_pair, T_map, T_vector, T_set, T_list, T_multimap, T_p_pair, T_p_map, T_p_vector, T_p_set, T_p_list, 
							T_attribute_list, T_ud_hypergraph, T_user_defined} ;
/* move to universal_data.cpp
int structure_head = T_pair ;
int structure_tail = T_p_list ;
int primitive_head = T_int ;
int primitive_tail = T_incidence;
*/

#endif 