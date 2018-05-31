#include "sourceERD.h"
using namespace std ;

// --------------------------------------------
// --  Source ERD schema
// --------------------------------------------
Source_ERD_schema::Source_ERD_schema(string erd_name){
	set_ERD_name(erd_name) ;
	Map mtmp;
	add_attribute_al("Entity_table", mtmp) ;
	add_attribute_al("Relationship_table", mtmp) ;

	Source_ERD_table.insert(pair<string,Source_ERD_schema*>(erd_name,this));
}

Source_ERD_schema::~Source_ERD_schema(){
}

void Source_ERD_schema::set_ERD_name(string erd_name){
	String stmp ;
	stmp.set_value(erd_name) ;
	universal_data* udptr=get_attribute_ref_al("ERD_name");
	if(udptr!=NULL)
		set_attribute_al("ERD_name", stmp) ;
	else
		add_attribute_al("ERD_name", stmp) ;
}

string Source_ERD_schema::get_ERD_name(){
	universal_data* udptr=get_attribute_ref_al("ERD_name");
	if(udptr!=NULL)
		return *(((String*)udptr)->getptr()) ;
	else
		return "";
}


Map* Source_ERD_schema::get_Entity_table(){
	universal_data* udptr=get_attribute_ref_al("Entity_table");
	if(udptr!=NULL)
		return (Map*)udptr;
	else
		return NULL;
}

Map* Source_ERD_schema::get_Relationship_table(){
	universal_data* udptr=get_attribute_ref_al("Relationship_table");
	if(udptr!=NULL)
		return (Map*)udptr;
	else
		return NULL;
}

void Source_ERD_schema::add_an_Entity(Entity_schema* entity){	
	String stmp;
	string ename=entity->get_E_name();
	stmp.set_value(ename);
	
	Map* Mapptr=get_Entity_table();

	if(Mapptr!=NULL)
	{	
		universal_data* udptr=find_Entity(ename);
		if(udptr==NULL)
			Mapptr->insert(stmp,entity);
		else
		{
			cout<<"entity"<<ename<<"already exist"<<endl;
			exit(-1);
		}
	}
	else
	{
		Map mtmp;
		mtmp.insert(stmp,entity);
		add_attribute_al("Entity_table", mtmp) ;
	}
}

void Source_ERD_schema::remove_an_Entity(string E_name){	
	String stmp;
	stmp.set_value(E_name) ;

	Map* Mapptr=get_Entity_table();
	if(Mapptr!=NULL)
	{	
		universal_data* udptr=find_Entity(E_name);
		if(udptr==NULL)
		{
			cout<<"entity"<<E_name<<"not exist"<<endl;
		 	exit(-1);
		}
		else
			Mapptr->erase(stmp);
	}
	else
		cout<<"table not exist"<<endl;
}

void Source_ERD_schema::add_a_Relationship(Relationship_schema* relationship){
	String stmp;
	string rname=relationship->get_R_name();
	stmp.set_value(rname);
	
	Map* Mapptr=get_Relationship_table();
	
	if(Mapptr!=NULL)
	{
		universal_data* udptr=find_Relationship(rname);
		if(udptr==NULL)
			Mapptr->insert(stmp, relationship);
		else
		{
			cout<<"relationship"<<rname<<"already exist"<<endl;
			exit(-1);
		}
	}
	else
	{
		Map mtmp;
		mtmp.insert(stmp,relationship);
		add_attribute_al("Relationship_table", mtmp) ;
	}
}

void Source_ERD_schema::remove_a_Relationship(string R_name){
	String stmp;
	stmp.set_value(R_name) ;
	Map* Mapptr=get_Relationship_table();

	if(Mapptr!=NULL)
	{	
		universal_data* udptr=find_Relationship(R_name);
		if(udptr==NULL)
		{
			cout<<"Relationship"<<R_name<<"not exist"<<endl;
			exit(-1);
		}
		else
			Mapptr->erase(stmp);
	}
	else
		cout<<"table not exist"<<endl;		
}

Entity_schema* Source_ERD_schema::find_Entity(string E_name){
	String Ename;
	Ename.set_value(E_name);
	Map* Mapptr=get_Entity_table();
	Entity_schema* Etemp;
	if(Mapptr!=NULL)
	{
		Etemp=(Entity_schema*)(Mapptr->find(Ename));
		if (Etemp!=NULL)
			return Etemp;
		else
			return NULL; 
	}
	else
		return NULL; 
}

Relationship_schema* Source_ERD_schema::find_Relationship(string R_name){
	String Rname;
	Rname.set_value(R_name);
	Map* Mapptr=get_Relationship_table();
	Relationship_schema* Rtemp;
	if(Mapptr!=NULL)
	{
		Rtemp=(Relationship_schema*)(Mapptr->find(Rname));
		if (Rtemp!=NULL)
			return Rtemp;
		else
			return NULL; 
	}
	else
		return NULL; 
}

void Source_ERD_schema::add_binding(string E_name, string R_name,string role_name){
	Entity_schema* Etemp= find_Entity (E_name);
	Relationship_schema* Rtemp= find_Relationship(R_name);

	if ((Etemp==NULL)||(Rtemp==NULL))
	{
		cout<<"entity or relationship  not exist"<<endl;
		exit(-1);
	}

	if(Rtemp->get_form()=="integral")
	{
		Role_nary Roletemp;
		Roletemp.set_Role_name(role_name);

		Etemp->add_binding(role_name,Rtemp);
		Rtemp->add_binding(Roletemp,Etemp);
	}
	else if(Rtemp->get_form()=="decoupled")
	{	
		Role Roletemp;
		Roletemp.set_Role_name(role_name);

		Etemp->add_binding(role_name,Rtemp);
		Rtemp->add_binding(Roletemp,Etemp);
	}
	else
	{
		cout<<"form error"<<Rtemp->get_form()<<endl;
		exit(-1);
	}
}
void Source_ERD_schema::remove_binding(string E_name, string R_name,string Role_name){
	Entity_schema* Etemp= find_Entity (E_name);
	Relationship_schema* Rtemp= find_Relationship(R_name);

	if ((Etemp!=NULL)&&(Rtemp!=NULL))
	{	
		Relationship_schema* Rtemp= find_Relationship(R_name);
		Etemp->remove_binding(R_name,Role_name);
		Rtemp->remove_binding(Role_name);
	}	
	else
		cout<<"entity not exist"<<endl;
}

void Source_ERD_schema::cout_entity_data(){
	Map* entitymap=this->get_Entity_table();
	if(entitymap==NULL)
	{	
		cout<<"no table"<<endl;
		exit(-1);
	}
	string entityname;
	for( entitymap->begin();!entitymap->end();(*entitymap)++)
	{
		entityname=*(((String*)&(entitymap->key()))->getptr());
		cout<<"entity name:"<<entityname<<endl;
	}
}
void Source_ERD_schema::cout_relationship_data(){
	Map* relationshipmap=this->get_Relationship_table();
	if(relationshipmap==NULL)
	{	
		cout<<"no table"<<endl;
		exit(-1);
	}
	string relationshipname;
	for( relationshipmap->begin();!relationshipmap->end();(*relationshipmap)++)
	{
		relationshipname=*(((String*)&(relationshipmap->key()))->getptr());
		
		cout<<"relationship name:"<<relationshipname<<endl;
		this->find_Relationship(relationshipname)->cout_binding_data();

	}
}

// --------------------------------------------
// --  Unit schema
// --------------------------------------------
Unit_schema::Unit_schema(){
	String stmp ;
	stmp.set_value("") ;
	add_attribute_al("Unit_name", stmp) ;
	add_attribute_al("Unit_type", stmp) ;
}

void Unit_schema::set_Unit_name(string Unit_name)
{	
	String stmp;
	stmp.set_value(Unit_name);
	universal_data* udptr=get_attribute_ref_al("Unit_name");
	if(udptr !=NULL)
		set_attribute_al ("Unit_name", stmp) ;
	else
		add_attribute_al ("Unit_name", stmp) ;
}

string Unit_schema::get_Unit_name(){
	universal_data* udptr=get_attribute_ref_al("Unit_name");
	if(udptr !=NULL)
	{
		String* stmp=(String*)udptr;
		return *(stmp->getptr());
	}
	else
		return "";
}

void Unit_schema::set_Unit_type(string Unit_type){	
	String stmp;
	stmp.set_value(Unit_type);
	universal_data* udptr=get_attribute_ref_al("Unit_type");
	if(udptr !=NULL)
		set_attribute_al ("Unit_type", stmp) ;
	else
		add_attribute_al ("Unit_type", stmp) ;
}

string Unit_schema::get_Unit_type(){
	universal_data* udptr=get_attribute_ref_al("Unit_type");
	if(udptr !=NULL)
	{
		String* stmp=(String*)udptr;
		return *(stmp->getptr());
	}
	else
		return "";
}

// --------------------------------------------
// --  Entity schema 
// --------------------------------------------
Entity_schema::Entity_schema(){
	List Listtemp;
	add_attribute_al("binding_list", Listtemp) ;//list of (Relationship_schema pointer,role_name)
	set_E_name("");
	set_Unit_type("S_entity");
}

Entity_schema::Entity_schema(string E_name){
	List Listtemp;
	add_attribute_al("binding_list", Listtemp) ;//list of (Relationship_schema pointer,role_name)

	set_E_name(E_name);
	set_Unit_type("S_entity");
}

Entity_schema::~Entity_schema() {
}

void Entity_schema::set_E_name(string Unit_name){
	set_Unit_name(Unit_name);
}

string Entity_schema::get_E_name(){
	return get_Unit_name();
}

void Entity_schema::add_attribute_and_type(string attribute_name,string att_type){
	universal_data* udptr=get_attribute_ref_al("attributes");

	String thetype;
	thetype.set_value(att_type);

	if(udptr!=NULL)
	{
		Attribute_List*att= (Attribute_List*)udptr;
		universal_data* typeudptr=att->get_attribute_ref_al(attribute_name);
		
		if(typeudptr!=NULL)
			att->set_attribute_al(attribute_name, thetype) ;
		else
			att->add_attribute_al(attribute_name, thetype) ;
	}
	else
	{
		Attribute_List att ;
		att.add_attribute_al(attribute_name, thetype) ;
		add_attribute_al("attributes", att) ;
	}
}

void Entity_schema::add_attribute_and_type(Attribute_List att_temp){
	for(att_temp.begin() ; !att_temp.end() ; att_temp++)
	{
		universal_data type_ud = att_temp.get_attribute() ;
		add_attribute_and_type(att_temp.get_attribute_name(), *(((String*)(&type_ud))->getptr())) ;
	}
}

void Entity_schema::remove_attribute_and_type(string attribute_name){
	Attribute_List* attrs = (Attribute_List*)(get_attribute_ref_al("attributes")) ;

	if( attrs != NULL)
		attrs->remove_attribute_al(attribute_name) ;
}

Attribute_List* Entity_schema::get_attribute_and_type(){
	universal_data* udptr=get_attribute_ref_al("attributes");

	if(udptr!=NULL)
		return (Attribute_List*)udptr;
	else
		return NULL;
}

void Entity_schema::add_binding(string Role_name,Relationship_schema* relationship){
	Pair* ptmp=new Pair();
	ptmp->set_first(relationship);
	String* stmp=new String();
	stmp->set_value(Role_name);
	ptmp->set_second(stmp);

	List* binding_list=(List*)(get_attribute_ref_al("binding_list"));
	
	if(binding_list!=NULL)
		binding_list->push_back(ptmp);
	else
	{
		List Listtemp;
		Listtemp.push_back(ptmp);
		add_attribute_al("binding_list", Listtemp) ;
	}
}

Relationship_schema* Entity_schema::find_a_binding (string R_name,string role_name){
	universal_data Ebindingud= get_binding_list();
	if(Ebindingud.get_type_tag()==T_unknown)
	{
		cout<<"there is no biding list"<<endl;
		exit(-1);
	}

	Pair* ptmp;

	Relationship_schema* Rtmp;
	List* Ebindinglist=(List*)(&Ebindingud) ;
	for(Ebindinglist->begin();!Ebindinglist->end();(*Ebindinglist)++)
	{
		ptmp=(Pair*)(*(*Ebindinglist));
		Rtmp=(Relationship_schema*)ptmp->first();
		string rname=Rtmp->get_R_name();
		if (rname==R_name)
		{
			String* stmp=(String*)(ptmp->second());
			string rolename=*(stmp->getptr());
			if (rolename==role_name)
			{
				return Rtmp;
			}
		}
	}
	return NULL;
}

void Entity_schema::remove_binding(string R_name,string role_name){
	List* Ebindinglist=(List*)(get_attribute_ref_al("binding_list"));
	if(Ebindinglist==NULL)
	{
		cout<<"there is no biding list"<<endl;
		exit(-1);
	}

	Pair* ptmp;
	Relationship_schema* Rtmp;
	for(Ebindinglist->begin();!Ebindinglist->end();(*Ebindinglist)++)
	{
		ptmp=(Pair*)(*(*Ebindinglist));
		Rtmp=(Relationship_schema*)ptmp->first();
		string rname=Rtmp->get_R_name();
		if (rname==R_name)
		{
			String* stmp=(String*)(ptmp->second());
			string rolename=*(stmp->getptr());
			if (rolename==role_name)
			{
				Ebindinglist->erase();
				return;
			}
		}
	}
}

universal_data Entity_schema::get_binding_list(){
	return get_attribute_al("binding_list");
}

void Entity_schema::set_set_flag(bool set_flag){
	Bool btmp;
	btmp.set_value(set_flag);
	universal_data* udptr=get_attribute_ref_al("set_flag");
	if(udptr!=NULL)
		set_attribute_al("set_flag",btmp);
	else
		add_attribute_al("set_flag",btmp);
	
	if(set_flag==true)
	{
		Map mtmp;
		universal_data* udptr01=get_attribute_ref_al("multi_key_flag");
		if(udptr01==NULL)
			add_attribute_al("multi_key_flag",btmp);
		
		universal_data* udptr02=get_attribute_ref_al("key_unique_map");
		if(udptr02==NULL)
			add_attribute_al("key_unique_map",mtmp);
	}
}

bool Entity_schema::get_set_flag(){
	universal_data* udptr=get_attribute_ref_al("set_flag");
	if(udptr!=NULL)
	{
		Bool* btmp=(Bool*)udptr;
		return *(btmp->getptr());
	}
	else
		return NULL; ;
}

void Entity_schema::set_multi_key_flag(bool multi_flag){
	Bool btmp;
	btmp.set_value(multi_flag);
	universal_data* udptr=get_attribute_ref_al("multi_key_flag");

	if(udptr!=NULL)
		set_attribute_al("multi_key_flag",btmp);
	else
		add_attribute_al("multi_key_flag",btmp);
}

bool Entity_schema::get_multi_key_flag(){
	universal_data* udptr=get_attribute_ref_al("multi_key_flag");
	if(udptr!=NULL)
	{
		Bool* btmp=(Bool*)udptr;
		return *(btmp->getptr());
	}
	else
		return false; ;
}

Map* Entity_schema::get_key_unique_map(){
	universal_data* udptr=get_attribute_ref_al("key_unique_map");
	if(udptr!=NULL)
		return(Map*)udptr;
	else
		return NULL; ;
}

void Entity_schema::add_key_unique(string key_name,bool unique_flag){
	String stmp;
	stmp.set_value(key_name);
	
	Map* Mapptr=get_key_unique_map();

	if(Mapptr!=NULL)
	{	
		Bool* btmp2=(Bool*)(Mapptr->find(stmp));
		if(btmp2==NULL)
		{	
			Bool* btmp=new Bool;
			btmp->set_value(unique_flag);
			Mapptr->insert(stmp,btmp);
		}
		else
		{
			cout<<"this key already exit"<<endl;
			exit(-1);
		}
	}
	else
		cout<<"key_unique_map no exit yet"<<endl;
}
void Entity_schema::set_key_unique(string key_name,bool unique_flag){
	String stmp;
	stmp.set_value(key_name);
	
	Map* Mapptr=get_key_unique_map();
	if(Mapptr!=NULL)
	{
		Bool* btmp=new Bool();
		btmp->set_value(unique_flag);

		Bool* btmp2=(Bool*)(Mapptr->find(stmp));
		
		if(btmp2!=NULL)
		{
			Mapptr->erase(stmp);
			Mapptr->insert(stmp,btmp);
		}
		else
			Mapptr->insert(stmp,btmp);
	}
	else
		cout<<"key_unique_map no exit yet"<<endl;
}

bool Entity_schema::get_key_unique(string key_name){
	String stmp;
	stmp.set_value(key_name);
	Bool* btmp;
	
	Map* Mapptr=get_key_unique_map();
	if(Mapptr!=NULL)
	{
		btmp=(Bool*)(Mapptr->find(stmp));
		if(btmp!=NULL)
			return *(btmp->getptr());
		else
			return false;
	}
	else
	{
		cout<<"key_unique_map no exit yet"<<endl;
		return NULL;
	}
}

void Entity_schema::remove_key_unique(string key_name){
	String stmp;
	stmp.set_value(key_name);
	Bool* btmp;
	Map* Mapptr=get_key_unique_map();
	if(Mapptr!=NULL)
	{
		btmp=(Bool*)(Mapptr->find(stmp));
		if(btmp!=NULL)
			Mapptr->erase(stmp);
		else
			cout<<"not exit "<<endl;
	}
	else
		cout<<"key_unique_map no exit yet"<<endl;
}

// --------------------------------------------
// --  Relationship schema 
// --------------------------------------------
Relationship_schema::Relationship_schema(){
	List Listtemp;
	String stmp;
	Bool btmp;
	btmp.set_value(false);
	Map mtmp;
	add_attribute_al("binding_list", mtmp) ;//Map of(Role object,Entity_schema pointer)
	add_attribute_al("recursive_flag", btmp) ;	
	Int itmp;
	itmp.set_value(2);
	add_attribute_al("degree", itmp) ;	
	set_R_name("");
	set_Unit_type("S_relationship");
	add_attribute_al("form", stmp) ;	
}

Relationship_schema::Relationship_schema(string R_name){
	String stmp;
	Bool btmp;
	btmp.set_value(false);
	Map mtmp;
	add_attribute_al("binding_list", mtmp) ;//Map of(Role object,Entity_schema pointer)
	add_attribute_al("recursive_flag", btmp) ;	
	Int itmp;
	itmp.set_value(2);
	add_attribute_al("degree", itmp) ;	

	set_R_name(R_name);
	set_Unit_type("S_relationship");
	add_attribute_al("form", stmp) ;
}

Relationship_schema::~Relationship_schema() {
}

void Relationship_schema::set_R_name(string R_name){
	set_Unit_name(R_name);
}

string Relationship_schema::get_R_name(){
	return get_Unit_name();
}	

void Relationship_schema::set_degree(int degree){
	Int itmp;
	itmp.set_value(degree);
	universal_data* udptr=get_attribute_ref_al("degree");
	if(udptr!=NULL)
		set_attribute_al("degree", itmp) ;
	else
		add_attribute_al("degree", itmp) ;
}

int Relationship_schema::get_degree(){
	universal_data* udptr=get_attribute_ref_al("degree");
	if(udptr!=NULL)
	{
		Int* degree=(Int*)udptr;
		return *(degree->getptr());
	}
	else
		return -1;
}

void Relationship_schema::set_recursive_flag(bool re_flag){
	Bool btmp;
	btmp.set_value(re_flag);
	universal_data* udptr=get_attribute_ref_al("recursive_flag");
	if(udptr!=NULL)
		set_attribute_al("recursive_flag", btmp) ;
	else
		add_attribute_al("recursive_flag", btmp) ;
}

bool Relationship_schema::get_recursive_flag(){
	universal_data* udptr=get_attribute_ref_al("recursive_flag");
	if(udptr!=NULL)
	{
		Bool* btmp=(Bool*)udptr;
		return *(btmp->getptr());
	}
	else
		return false;
}

void Relationship_schema::set_form(string formtpye){
	String stmp;
	stmp.set_value(formtpye);
	universal_data* udptr=get_attribute_ref_al("form");
	if(udptr!=NULL)
		set_attribute_al("form", stmp) ;
	else
		add_attribute_al("form", stmp) ;
}

string Relationship_schema::get_form(){
	universal_data* udptr=get_attribute_ref_al("form");
	if(udptr!=NULL)
	{
		String* formtye=(String*)udptr;
		return *(formtye->getptr());
	}
	else
		return "";
}

void Relationship_schema::add_binding(universal_data role,Entity_schema* Entity){
	string rolename;
	Map* Mapptr=(Map*)(get_attribute_ref_al("binding_list"));
	if(get_form()=="integral")
	{
		Role_nary* rolenary=(Role_nary*)&role;
		rolename=rolenary->get_Role_name();
	}
	else if(get_form()=="decoupled")
	{
			
		Role* roleptr=(Role*)&role;
		rolename=roleptr->get_Role_name();
	}
	else
	{
		cout<<"form error"<<get_form()<<endl;
		exit(-1);
	}

	if(Mapptr!=NULL)
	{
		Entity_schema* Etemp=find_a_binding(rolename);
		if(Etemp==NULL)
			Mapptr->insert(role, Entity);
		else
			cout<<"this role already exit"<<endl;
	}
	else
	{	
		Map mtmp;
		mtmp.insert(role, Entity);;
		add_attribute_al("binding_list",mtmp);
	}

	set_defaultdecision(rolename);
}

Entity_schema* Relationship_schema::find_a_binding(string Role_name){
	universal_data Mapud=get_binding_list();
	if(Mapud.get_type_tag()!=T_unknown)
	{
		Map* Mapptr = (Map*)(&Mapud) ;
		string rolename;
		for(Mapptr->begin();!Mapptr->end();(*Mapptr)++)
		{
			if(get_form()=="integral")
			{
				Role_nary* roletemp=((Role_nary*)&(Mapptr->key()));
				rolename=roletemp->get_Role_name();
			}
			else if(get_form()=="decoupled")
			{	
				Role* roletemp=((Role*)&(Mapptr->key()));
				rolename=roletemp->get_Role_name();
			}
			else
			{
				cout<<"form error"<<get_form()<<endl;
				exit(-1);
			}
			
			if(rolename==Role_name)
				return (Entity_schema*)(Mapptr->value());
		}
		return NULL;
	}
	else
	{
		cout<<"binding_list not exist  yet"<<endl;
		return NULL;
	}
}


void Relationship_schema::remove_binding(string Role_name){	
	string rolename;
	Map* Mapptr=(Map*)(get_attribute_ref_al("binding_list"));
	if(Mapptr!=NULL)
	{
		for(Mapptr->begin();!Mapptr->end();(*Mapptr)++)
		{
			if(get_form()=="integral")
			{
				Role_nary* roletemp=((Role_nary*)&(Mapptr->key()));
				rolename=roletemp->get_Role_name();
			}
			else if(get_form()=="decoupled")
			{	
				Role* roletemp=((Role*)&(Mapptr->key()));
				rolename=roletemp->get_Role_name();
			}
			else
			{
				cout<<"form error"<<get_form()<<endl;
				exit(-1);
			}

			if(rolename==Role_name)
			{
				universal_data ud=Mapptr->key();
				Mapptr->erase(ud);
				return ;
			}
		}
	}
	else
		cout<<"binding_list not exist  yet"<<endl;
}

universal_data Relationship_schema::get_binding_list(){
	return get_attribute_al("binding_list");
}

universal_data Relationship_schema::find_role(string Role_name){	
	string rolename;
	universal_data Mapud=get_binding_list();
	if(Mapud.get_type_tag()!=T_unknown)
	{
		Map* Mapptr = (Map*)(&Mapud) ;
		for(Mapptr->begin();!Mapptr->end();(*Mapptr)++)
		{
			universal_data udtemp=Mapptr->key();
			if(get_form()=="integral")
			{
				Role_nary* roletemp=(Role_nary*)&udtemp;
				rolename=roletemp->get_Role_name();
			}
			else if(get_form()=="decoupled")
			{	
				Role* roletemp=(Role*)&udtemp;
				rolename=roletemp->get_Role_name();
			}
			else
			{
				cout<<"form error"<<get_form()<<endl;
				exit(-1);
			}
			
			if(rolename==Role_name)
				return udtemp;
		}
		// do not have this role
		universal_data udtep;
		return udtep;
	}
	else
	{
		universal_data udtep;
		return udtep;
	}
}


void Relationship_schema::Role_set_R_cardi(string role_name,string Minimum,string Maxmum){
	universal_data udtemp=find_role(role_name);
	Role* Roletemp;
	if(udtemp.get_type_tag()!=T_unknown)
	{
		Roletemp=(Role*)(&udtemp);
		Roletemp->set_Role_name(role_name);
		Roletemp->set_R_cardi(Minimum,Maxmum);
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	Entity_schema* Etmp=this->find_a_binding(role_name);
	if(Etmp==NULL)
	{
		cout<<"the role_name not exit"<<endl;
		exit(-1);
	}
	remove_binding(role_name);

	add_binding(*Roletemp,Etmp);
}

void Relationship_schema::Role_set_R_cardi(string role_name,Cardinality* carditemp){	
	universal_data udtemp=find_role(role_name);
	
	Role* Roletemp=NULL;
	if(udtemp.get_type_tag()!=T_unknown)
	{
		Roletemp=(Role*)(&udtemp);
		Roletemp->set_Role_name(role_name);
		Roletemp->set_R_cardi(carditemp);
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}
	Entity_schema* Etmp=this->find_a_binding(role_name);
	if(Etmp==NULL)
	{
		cout<<"the role_name not exit"<<endl;
		exit(-1);
	}
	remove_binding(role_name);

	this->add_binding(*Roletemp,Etmp);
}

Cardinality* Relationship_schema::Role_get_R_cardi(string role_name){
	universal_data udtemp=find_role(role_name);

	if(udtemp.get_type_tag()!=T_unknown)
	{	
		Role* Roletemp=(Role*)(&udtemp);
		return Roletemp->get_R_cardi();
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		return NULL;
	}
}

void Relationship_schema::Role_set_cardi_Eside(string role_name,string Minimum,string Maxmum){
	universal_data udtemp=find_role(role_name);
	Role_nary* Roletemp=NULL;
	if(udtemp.get_type_tag()!=T_unknown)
	{
		Roletemp=(Role_nary*)(&udtemp);
		Cardinality* cartemp=Roletemp->get_cardi_Rside();
		Roletemp->set_cardi_Rside(cartemp);
		Roletemp->set_Role_name(role_name);
		Roletemp->set_cardi_Eside(Minimum,Maxmum);
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	Entity_schema* Etmp=this->find_a_binding(role_name);
	if(Etmp==NULL)
	{
		cout<<"the role_name not exit"<<endl;
		exit(-1);
	}
	remove_binding(role_name);

	add_binding(*Roletemp,Etmp);
}

void Relationship_schema::Role_set_cardi_Eside(string role_name,Cardinality* carditemp){	
	universal_data udtemp=find_role(role_name);
	
	Role_nary* Roletemp=NULL;
	if(udtemp.get_type_tag()!=T_unknown)
	{
		Roletemp=(Role_nary*)(&udtemp);
		Cardinality* cartemp=Roletemp->get_cardi_Rside();
		Roletemp->set_cardi_Rside(cartemp);
		Roletemp->set_Role_name(role_name);
		Roletemp->set_cardi_Eside(carditemp);
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}
	Entity_schema* Etmp=this->find_a_binding(role_name);
	if(Etmp==NULL)
	{
		cout<<"the role_name not exit"<<endl;
		exit(-1);
	}
	remove_binding(role_name);

	this->add_binding(*Roletemp,Etmp);
}

Cardinality* Relationship_schema::Role_get_cardi_Eside(string role_name){
	universal_data udtemp=find_role(role_name);

	if(udtemp.get_type_tag()!=T_unknown)
	{	
		Role_nary* Roletemp=(Role_nary*)(&udtemp);
		return Roletemp->get_cardi_Eside();
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		return NULL;
	}
}

void Relationship_schema::Role_set_cardi_Rside(string role_name,string Minimum,string Maxmum){
	universal_data udtemp=find_role(role_name);
	Role_nary* Roletemp=NULL;
	if(udtemp.get_type_tag()!=T_unknown)
	{
		Roletemp=(Role_nary*)(&udtemp);
		Cardinality* cartemp=Roletemp->get_cardi_Rside();
		Roletemp->set_cardi_Rside(cartemp);
		Roletemp->set_Role_name(role_name);
		Roletemp->set_cardi_Rside(Minimum,Maxmum);
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	Entity_schema* Etmp=this->find_a_binding(role_name);
	if(Etmp==NULL)
	{
		cout<<"the role_name not exit"<<endl;
		exit(-1);
	}
	remove_binding(role_name);

	add_binding(*Roletemp,Etmp);
}

void Relationship_schema::Role_set_cardi_Rside(string role_name,Cardinality* carditemp){	
	universal_data udtemp=find_role(role_name);
	
	Role_nary* Roletemp=NULL;
	if(udtemp.get_type_tag()!=T_unknown)
	{
		Roletemp=(Role_nary*)(&udtemp);
		Cardinality* cartemp=Roletemp->get_cardi_Rside();
		Roletemp->set_cardi_Rside(cartemp);
		Roletemp->set_Role_name(role_name);
		Roletemp->set_cardi_Rside(carditemp);
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}
	Entity_schema* Etmp=this->find_a_binding(role_name);
	if(Etmp==NULL)
	{
		cout<<"the role_name not exit"<<endl;
		exit(-1);
	}
	remove_binding(role_name);

	this->add_binding(*Roletemp,Etmp);
}

Cardinality* Relationship_schema::Role_get_cardi_Rside(string role_name){
	universal_data udtemp=find_role(role_name);

	if(udtemp.get_type_tag()!=T_unknown)
	{	
		Role_nary* Roletemp=(Role_nary*)(&udtemp);
		return Roletemp->get_cardi_Rside();
	}
	else
	{
		cout<<"Role not exist  yet"<<endl;
		return NULL;
	}
}

void Relationship_schema::set_degeneration(string role_name, string thedirection, bool degre){
	universal_data udtemp=find_role(role_name);
	Role* Roletemp=(Role*)(&udtemp);

	if(udtemp.get_type_tag()==T_unknown)
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	Bool bemp;
	bemp.set_value(degre);

	if (thedirection=="RtoL")
	{
		universal_data* udptr=get_attribute_ref_al(role_name+"RtoLdegeneration");
		if(udptr!=NULL)
			set_attribute_al(role_name+"RtoLdegeneration", bemp);
		else
			add_attribute_al(role_name+"RtoLdegeneration", bemp);
	}
	else if(thedirection=="LtoR")
	{
		universal_data* udptr=get_attribute_ref_al(role_name+"LtoRdegeneration");
		if(udptr!=NULL)
			set_attribute_al(role_name+"LtoRdegeneration", bemp);
		else
			add_attribute_al(role_name+"LtoRdegeneration", bemp);
	}
	else
	{
		cout<<"wrong direction"<<endl;
		exit(-1);
	}
}
	
bool Relationship_schema::get_degeneration(string role_name, string thedirection){
	universal_data udtemp=find_role(role_name);
	Role* Roletemp=(Role*)(&udtemp);

	if(udtemp.get_type_tag()==T_unknown)
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	if (thedirection=="RtoL")
	{
		
		universal_data* udptr=get_attribute_ref_al(role_name+"RtoLdegeneration");
		if(udptr!=NULL)
		{
			Bool * degeneration=(Bool*)udptr;
			return *(degeneration->getptr());
		}
		else
			return false;
	}
	else if(thedirection=="LtoR")
	{
	
		universal_data* udptr=get_attribute_ref_al(role_name+"LtoRdegeneration");
		if(udptr!=NULL)
		{
			Bool * degeneration=(Bool*)udptr;
			return *(degeneration->getptr());
		}
		else
			return false;
	}
	else
	{
		cout<<"wrong direction"<<endl;
		exit(-1);
	}
}

void Relationship_schema::set_embedding(string role_name, string thedirection, string embedding){
	universal_data udtemp=find_role(role_name);
	Role* Roletemp=(Role*)(&udtemp);

	if(udtemp.get_type_tag()==T_unknown)
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	String stemp;
	stemp.set_value(embedding);

	if (thedirection=="RtoL")
	{
		universal_data* udptr=get_attribute_ref_al(role_name+"RtoLembedding");
		if(udptr!=NULL)
			set_attribute_al(role_name+"RtoLembedding", stemp);
		else
			add_attribute_al(role_name+"RtoLembedding", stemp);
	}
	else if(thedirection=="LtoR")
	{
		universal_data* udptr=get_attribute_ref_al(role_name+"LtoRembedding");
		if(udptr!=NULL)
			set_attribute_al(role_name+"LtoRembedding", stemp);
		else
			add_attribute_al(role_name+"LtoRembedding", stemp);
	}
	else
	{
		cout<<"wrong direction"<<endl;
		exit(-1);
	}
}

string Relationship_schema::get_embedding(string role_name, string thedirection){
	universal_data udtemp=find_role(role_name);
	Role* Roletemp=(Role*)(&udtemp);
	
	if(udtemp.get_type_tag()==T_unknown)
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	if (thedirection=="RtoL")
	{
	
		universal_data* udptr=get_attribute_ref_al(role_name+"RtoLembedding");
		if(udptr!=NULL)
		{
			String * embedding=(String*)udptr;
			return *(embedding->getptr());
		}
		else
			return "";
	}
	else if(thedirection=="LtoR")
	{

		universal_data* udptr=get_attribute_ref_al(role_name+"LtoRembedding");
		if(udptr!=NULL)
		{
			String * embedding=(String*)udptr;
			return *(embedding->getptr());
		}
		else
			return "";
	}
	else
	{
		cout<<"wrong direction"<<endl;
		exit(-1);
	}
}

void Relationship_schema::set_link_decision(string role_name, string link_dec){
	universal_data udtemp=find_role(role_name);
	Role* Roletemp=(Role*)(&udtemp);

	if(udtemp.get_type_tag()==T_unknown)
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}
	
	String stemp;
	stemp.set_value(link_dec);

 	universal_data* udptr=get_attribute_ref_al(role_name+"link_decision");
	if(udptr!=NULL)
		set_attribute_al(role_name+"link_decision", stemp);
	else
		add_attribute_al(role_name+"link_decision", stemp);
}

string Relationship_schema::get_link_decision(string role_name){
	universal_data udtemp=find_role(role_name);

	if(udtemp.get_type_tag()==T_unknown)
	{
		cout<<"Role not exist  yet"<<endl;
		exit(-1);
	}

	universal_data* udptr=get_attribute_ref_al(role_name+"link_decision");
	if(udptr!=NULL)
	{
		String *embedding =(String*)udptr;
		return *(embedding->getptr());
	}
	else
		return "";
}

void Relationship_schema::set_defaultdecision(string role_name){
	set_degeneration(role_name,"RtoL",false);//relationship to entity
	set_degeneration(role_name,"LtoR",false);// entity to relationship 
	set_embedding(role_name,"RtoL","NA");//relationship to entity
	set_embedding(role_name,"LtoR","NA");// entity to relationship 
	set_link_decision(role_name, "true_link");
}

void Relationship_schema::cout_binding_data(){
	universal_data Mapud=get_binding_list();
	if(Mapud.get_type_tag()!=T_unknown)
	{
		Map* binding_list = (Map*)(&Mapud) ;

		for( binding_list->begin();!binding_list->end();(*binding_list)++)
		{			
			string rolename=((Role*)(&(binding_list->key())))->get_Role_name();
			cout<<"   Entity: "<<((Entity_schema*)(binding_list->value()))->get_E_name()<<" as role: "<<rolename<<endl;
		}
	}
	else
		cout<<"no binding_list"<<endl;
}

// --------------------------------------------
// --  Role
// --------------------------------------------
Role::Role(){
	String stmp;
	stmp.set_value("");
	add_attribute_al("Role_name",stmp) ;	
	Cardinality cardi_temp;
	add_attribute_al("cardi_at_R", cardi_temp) ;	
}

Role::Role(string role_name){
	String stmp;
	stmp.set_value(role_name);
	add_attribute_al("Role_name",stmp) ;	
	Cardinality cardi_temp;
	add_attribute_al("cardi_at_R", cardi_temp) ;	
}

void Role::set_R_cardi(string Minimum,string Maxmum){
	Cardinality* carptr=get_R_cardi();
	if(carptr!=NULL)
		carptr->set_cardinality(Minimum, Maxmum);
	else
	{
		Cardinality cardi_temp;
		cardi_temp.set_cardinality(Minimum, Maxmum);
		add_attribute_al("cardi_at_R", cardi_temp) ;	
	}
}

void Role::set_R_cardi(Cardinality* carditemp){
	if(carditemp==NULL)
	{
		cout<<"this is NULL in set_R_cardi(Cardinality* carditemp)"<<endl;
		exit(-1);
	}

	string Minimum=carditemp->get_Mini();
	string Maxmum=carditemp->get_Max();
	set_R_cardi(Minimum,Maxmum);
}

Cardinality* Role::get_R_cardi(){
	universal_data* udptr=get_attribute_ref_al("cardi_at_R");

	if(udptr!=NULL)
		return(Cardinality*)udptr;	
	else
		return NULL;
}

void Role::set_Role_name(string Role_name){
	String stmp;
	stmp.set_value(Role_name);
	universal_data* udptr=get_attribute_ref_al("Role_name");
	if(udptr!=NULL)
		set_attribute_al("Role_name",stmp) ;	
	else
		add_attribute_al("Role_name",stmp) ;	
}

string Role::get_Role_name(){	
	universal_data* udptr=get_attribute_ref_al("Role_name");

	if(udptr!=NULL)
	{ 
		String* stmp=(String*)udptr;
		return *(stmp->getptr());
	}	
	else
		return "";
}

// --------------------------------------------
// --  Role _ nary
// --------------------------------------------
Role_nary::Role_nary(){
	String stmp;
	stmp.set_value("");
	add_attribute_al("Role_name",stmp) ;	
	Cardinality cardi_temp;
	add_attribute_al("cardi_at_R", cardi_temp) ;	
	add_attribute_al("cardi_at_Entity", cardi_temp) ;
}

Role_nary::Role_nary(string role_name){
	String stmp;
	stmp.set_value(role_name);
	add_attribute_al("Role_name",stmp) ;	
	Cardinality cardi_temp;
	add_attribute_al("cardi_at_R", cardi_temp) ;	
	add_attribute_al("cardi_at_Entity", cardi_temp) ;
}

void Role_nary::set_cardi_Rside(string Minimum,string Maxmum){
	Cardinality* carptr=get_cardi_Rside();
	if(carptr!=NULL)
		carptr->set_cardinality(Minimum, Maxmum);
	else
	{
		Cardinality cardi_temp;
		cardi_temp.set_cardinality(Minimum, Maxmum);
		add_attribute_al("cardi_at_R", cardi_temp) ;	
	}
}

void Role_nary::set_cardi_Rside(Cardinality* carditemp){
	string Minimum=carditemp->get_Mini();
	string Maxmum=carditemp->get_Max();
	set_cardi_Rside(Minimum,Maxmum);
}

Cardinality* Role_nary::get_cardi_Rside(){
	universal_data* udptr=get_attribute_ref_al("cardi_at_R");

	if(udptr!=NULL)
		return(Cardinality*)udptr;	
	else
		return NULL;
}

void Role_nary::set_cardi_Eside(string Minimum,string Maxmum){
	Cardinality* carptr=get_cardi_Eside();
	if(carptr!=NULL)
		carptr->set_cardinality(Minimum, Maxmum);
	else
	{
		Cardinality cardi_temp;
		cardi_temp.set_cardinality(Minimum, Maxmum);
		add_attribute_al("cardi_at_Entity", cardi_temp) ;	
	}
}

void Role_nary::set_cardi_Eside(Cardinality* carditemp){	
	string Minimum=carditemp->get_Mini();
	string Maxmum=carditemp->get_Max();
	set_cardi_Eside(Minimum,Maxmum);
}

Cardinality* Role_nary::get_cardi_Eside(){
	universal_data* udptr=get_attribute_ref_al("cardi_at_Entity");
	if(udptr!=NULL)
		return(Cardinality*)udptr;	
	else
	{
		cout<<" this is NULL"<<endl;
		return NULL;
	}
}

void Role_nary::set_Role_name(string Role_name){
	String stmp;
	stmp.set_value(Role_name);
	universal_data* udptr=get_attribute_ref_al("Role_name");
	if(udptr!=NULL)
		set_attribute_al("Role_name",stmp) ;	
	else
		add_attribute_al("Role_name",stmp) ;	
}

string Role_nary::get_Role_name(){	
	universal_data* udptr=get_attribute_ref_al("Role_name");
	if(udptr!=NULL)
	{ 
		String* stmp=(String*)udptr;
		return *(stmp->getptr());
	}	
	else
		return "";
}

// --------------------------------------------
// --  Cardinality
// --------------------------------------------
Cardinality::Cardinality(){
	String stmp;
	stmp.set_value("1");
	add_attribute_al("Minimum",stmp) ;	
	add_attribute_al("Maxmum",stmp) ;	
}

void Cardinality::set_cardinality(string Minimum,string Maxmum){
	set_Mini(Minimum);
	set_Max(Maxmum);
}

void Cardinality::set_Mini(string Minimum){
	String stmp;
	stmp.set_value(Minimum);
	universal_data* udptr=get_attribute_ref_al("Minimum");
	if(udptr!=NULL)
		set_attribute_al("Minimum",stmp);
	else
		add_attribute_al("Minimum",stmp);
}

string Cardinality::get_Mini(){
	universal_data* udptr=get_attribute_ref_al("Minimum");
	if(udptr!=NULL)
	{
		String* stmp= (String*)udptr;
		return *(stmp->getptr());
	}	
	else
		return NULL;
}

void Cardinality::set_Max (string Maxmum){
	String stmp;
	stmp.set_value(Maxmum);	
	universal_data* udptr=get_attribute_ref_al("Maxmum");
	if(udptr!=NULL)
		set_attribute_al("Maxmum",stmp);
	else
		add_attribute_al("Maxmum",stmp);
}

string Cardinality::get_Max()
{
	universal_data* udptr=get_attribute_ref_al("Maxmum");
	if(udptr!=NULL)
	{
		String* stmp= (String*)udptr;
		return *(stmp->getptr());
	}	
	else
		return NULL;
}
