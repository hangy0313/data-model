#ifndef PREDECLARE_H
#define PREDECLARE_H

// predeclaration 
class universal_data; 
// symbolic link
struct symbolic_link_data ;
class symbolic_link; 
// primitive data 
class Int; 
class Char; 
class String; 
class Double; 
class Float; 
class Bool ;
// structure 
struct Vector_data; 
class Vector; 
struct Pair_data ;
class Pair; 
struct List_data ;
class List; 
struct Set_data; 
class Set; 
struct Map_data; 
class Map; 
struct Multimap_data ;
class Multimap ;
// attribute list structure 
struct Attribute_List_data ;
class Attribute_List ;

// condition 
class condition ; //abstract class 

//incidence 
struct Incidence_data ;
class Incidence ;

//parent class 
class parent_class  ;
// has a parent point (backward pointer) to find upper level universal_data for discrete structure
class p_Vector_data; 
class p_Vector; 
class p_Pair_data ;
class p_Pair; 
class p_List_data ;
class p_List; 
class p_Set_data; 
class p_Set; 
class p_Map_data; 
class p_Map; 

// iterator 
class Virtual_Iterator ;
class Vector_Iterator; 
class Pair_Iterator;
class List_Iterator; 
class Set_Iterator ;
class Map_Iterator ;
class Multimap_Iterator ;
class Attribute_List_Iterator ;
class p_Vector_Iterator; 
class p_Pair_Iterator;
class p_List_Iterator; 
class p_Set_Iterator ;
class p_Map_Iterator ;

#endif 