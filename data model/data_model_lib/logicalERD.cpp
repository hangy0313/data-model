#include "LogicalERD.h"
#include <string>

using namespace std ;

// --------------------------------------------
// --  logical ERD schema
// --------------------------------------------
Logical_ERD_schema::Logical_ERD_schema(string erd_name){
    String stmp ;
    stmp.set_value(erd_name) ;
    add_attribute_al("ERD_name", stmp) ;
    Map mtmp;
    add_attribute_al("Entity_table", mtmp) ;
    add_attribute_al("Relationship_Entity_table", mtmp) ;
    
    Logical_ERD_table.insert(pair<string,Logical_ERD_schema*>(erd_name,this));
}

Logical_ERD_schema::~Logical_ERD_schema(){
}

void Logical_ERD_schema::set_ERD_name(string erd_name){
    String stmp ;
    stmp.set_value(erd_name) ;
    universal_data* udptr=get_attribute_ref_al("ERD_name");
    if(udptr!=NULL)
        set_attribute_al("ERD_name", stmp) ;
    else
        add_attribute_al("ERD_name", stmp) ;
}

string Logical_ERD_schema::get_ERD_name(){
    universal_data* udptr=get_attribute_ref_al("ERD_name");
    if(udptr!=NULL)
    {
        String* stmp=(String*)udptr;
        return *(stmp->getptr());
    }
    else
        return "";
}

Map* Logical_ERD_schema::get_Entity_table(){
    universal_data* udptr=get_attribute_ref_al("Entity_table");
    if(udptr!=NULL)
        return (Map*)udptr;
    else
        return NULL;
}

Map* Logical_ERD_schema::get_Relationship_entity_table(){
    universal_data* udptr=get_attribute_ref_al("Relationship_Entity_table");
    if(udptr!=NULL)
        return (Map*)udptr;
    else
        return NULL;
}

void Logical_ERD_schema::add_an_Entity(Logical_Entity_schema* entity){
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

void Logical_ERD_schema::remove_an_Entity(string E_name){
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

void Logical_ERD_schema::add_a_Relationship_entity(Relationship_Entity* R_entity){
    String stmp;
    string rname=R_entity->get_R_name();
    stmp.set_value(rname);
    
    Map* Mapptr=get_Relationship_entity_table();
    
    if(Mapptr!=NULL)
    {
        universal_data* udptr=find_Relationship_entity(rname);
        if(udptr==NULL)
            Mapptr->insert(stmp, R_entity);
        else
        {
            cout<<"relationship"<<rname<<"already exist"<<endl;
            exit(-1);
        }
    }
    else
    {
        Map mtmp;
        mtmp.insert(stmp,R_entity);
        add_attribute_al("Relationship_Entity_table", mtmp) ;
    }
}

void Logical_ERD_schema::remove_a_Relationship_entity(string R_name){
    String stmp;
    stmp.set_value(R_name) ;
    Map* Mapptr=get_Relationship_entity_table();
    
    if(Mapptr!=NULL)
    {
        universal_data* udptr=find_Relationship_entity(R_name);
        if(udptr==NULL)
        {
            cout<<"Relationship_entity"<<R_name<<"not exist"<<endl;
            exit(-1);
        }
        else
            Mapptr->erase(stmp);
    }
    else
    {
        cout<<"table not exist"<<endl;
        exit(-1);
    }
}


Logical_Entity_schema* Logical_ERD_schema::find_Entity(string E_name){
    String Ename;
    Ename.set_value(E_name);
    Map* Mapptr=get_Entity_table();
    Logical_Entity_schema* Etemp;
    if(Mapptr!=NULL)
    {
        Etemp=(Logical_Entity_schema*)(Mapptr->find(Ename));
        if (Etemp!=NULL)
            return Etemp;
        else
            return NULL;
    }
    else
        return NULL;
}

Relationship_Entity* Logical_ERD_schema::find_Relationship_entity(string R_name){
    String Rname;
    Rname.set_value(R_name);
    Map* Mapptr=get_Relationship_entity_table();
    Relationship_Entity* Rtemp;
    if(Mapptr!=NULL)
    {
        Rtemp=(Relationship_Entity*)(Mapptr->find(Rname));
        if (Rtemp!=NULL)
            return Rtemp;
        else
            return NULL;
    }
    else
        return NULL;
}

void Logical_ERD_schema::binding(string E_name, string R_name,string Role_name){
    Logical_Entity_schema* Etemp= find_Entity (E_name);
    
    if (Etemp==NULL)
    {
        cout<<"no entity"<<E_name<<endl;
        return ;
    }
    Relationship_Entity* REtemp= find_Relationship_entity(R_name);
    if (REtemp==NULL)
    {
        cout<<"no Relationship_Entity"<<R_name<<endl;
        return ;
    }
    string targetrole ,erd_name;
    erd_name=this->get_ERD_name();
    map<string,RF_ERD_schema*>::iterator RF_ERD_iter;
    RF_ERD_iter=RF_ERD_table.find(erd_name);
    if(RF_ERD_iter==RF_ERD_table.end())
    {
        cout<<"there is no this ERD"<<erd_name<<endl;
        exit(-1);
    }
    
    RF_ERD_schema* RF_ERD=(RF_ERD_schema*)(RF_ERD_iter->second);
    
    RF_Relationship_schema* relationship=RF_ERD->find_Relationship(R_name);
    if(relationship==NULL)
    {
        cout<<"there is no this relationship"<<R_name<<endl;
        exit(-1);
    }
    
    targetrole=relationship->find_another_role_name(Role_name);
    
    Link linktoE;
    linktoE.set_link(Etemp);
    
    REtemp->add_a_link(Role_name,linktoE);
    
    Link linktoRE;
    linktoRE.set_link(REtemp);
    
    Etemp->add_a_link(R_name,targetrole,linktoRE);
}

void Logical_ERD_schema::unbinding(string E_name, string R_name,string Role_name){
    Logical_Entity_schema* Etemp= find_Entity (E_name);
    Relationship_Entity* REtemp= find_Relationship_entity(R_name);
    string otherrolename=REtemp->find_another_role_name(Role_name);
    if ((Etemp!=NULL)&&(REtemp!=NULL))
    {
        Etemp->remove_container(R_name,Role_name);
        REtemp->remove_link(R_name);
    }
    else
        cout<<"entity or relationship is not exist"<<E_name<<""<<R_name<<endl;
}

void Logical_ERD_schema::cout_entity_link_container(){
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
        this->find_Entity(entityname)->cout_link_container();
    }
    cout<<endl;
}

// --------------------------------------------
// --  logical entity schema
// --------------------------------------------
Logical_Entity_schema::Logical_Entity_schema(){
    // the original bindinglist is flatterned
    List Listtemp;
    set_E_name("");
    set_Unit_type("L_entity");
}

Logical_Entity_schema::Logical_Entity_schema(string E_name){
    // the original bindinglist is flatterned
    List Listtemp;
    set_E_name(E_name);
    set_Unit_type("L_entity");
}

Logical_Entity_schema::~Logical_Entity_schema() {
}

void Logical_Entity_schema::set_E_name(string Unit_name){
    set_Unit_name(Unit_name);
}
string Logical_Entity_schema::get_E_name(){
    return get_Unit_name();
}
void Logical_Entity_schema::add_attribute_and_type(string attribute_name,string att_type){
    universal_data* udptr=get_attribute_ref_al("attributes");
    Attribute_List* att;
    String thetype;
    thetype.set_value(att_type);
    if(udptr!=NULL)
    {
        att= (Attribute_List*)udptr;
        universal_data* typeudptr=att->get_attribute_ref_al(attribute_name);
        if(typeudptr!=NULL)
            att->set_attribute_al(attribute_name, thetype) ;
        else
            att->add_attribute_al(attribute_name, thetype) ;
    }
    else
    {
        att=new Attribute_List;
        att->add_attribute_al(attribute_name, thetype) ;
        add_attribute_al("attributes", *att) ;
    }
}

void Logical_Entity_schema::add_attribute_and_type(Attribute_List att_temp){
    for(att_temp.begin() ; !att_temp.end() ; att_temp++)
    {
        universal_data name_ud = att_temp.get_attribute_name_al() ;
        universal_data type_ud = att_temp.get_attribute_value_al() ;
        add_attribute_and_type(*(((String*)(&name_ud))->getptr()), *(((String*)(&type_ud))->getptr()) ) ;
    }
}

void Logical_Entity_schema::remove_attribute_and_type(string attribute_name){
    Attribute_List* attrs = (Attribute_List*)(get_attribute_ref_al("attributes")) ;
    
    if( attrs != NULL)
        attrs->remove_attribute_al(attribute_name) ;
}

Attribute_List* Logical_Entity_schema::get_attribute_and_type(){
    universal_data* udptr=get_attribute_ref_al("attributes");
    
    if(udptr!=NULL)
        return (Attribute_List*)udptr;
    else
        return NULL;
}

void Logical_Entity_schema::set_set_flag(bool set_flag){
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
        {
            btmp.set_value(false);
            add_attribute_al("multi_key_flag",btmp);
        }
        universal_data* udptr02=get_attribute_ref_al("key_unique_map");
        if(udptr02==NULL)
            add_attribute_al("key_unique_map",mtmp);
    }
}

bool Logical_Entity_schema::get_set_flag(){
    universal_data* udptr=get_attribute_ref_al("set_flag");
    if(udptr!=NULL)
    {
        Bool* btmp=(Bool*)udptr;
        return *(btmp->getptr());
    }
    else
        return false;
}

void Logical_Entity_schema::set_multi_key_flag(bool multi_flag){
    Bool btmp;
    btmp.set_value(multi_flag);
    universal_data* udptr=get_attribute_ref_al("multi_key_flag");
    
    if(udptr!=NULL)
        set_attribute_al("multi_key_flag",btmp);
    else
        add_attribute_al("multi_key_flag",btmp);
}

bool Logical_Entity_schema::get_multi_key_flag(){
    universal_data* udptr=get_attribute_ref_al("multi_key_flag");
    if(udptr!=NULL)
    {
        Bool* btmp=(Bool*)udptr;
        return *(btmp->getptr());
    }
    else
        return false ;
}

Map* Logical_Entity_schema::get_key_unique_map(){
    universal_data* udptr=get_attribute_ref_al("key_unique_map");
    if(udptr!=NULL)
        return(Map*)udptr;
    else
        return NULL; ;
}

void Logical_Entity_schema::add_key_unique(string key_name,bool unique_flag){
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

void Logical_Entity_schema::set_key_unique(string key_name,bool unique_flag){
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

bool Logical_Entity_schema::get_key_unique(string key_name){
    String stmp;
    Bool* btmp;
    
    stmp.set_value(key_name);
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
        return false;
    }
}

void Logical_Entity_schema::remove_key_unique(string key_name){
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

void Logical_Entity_schema::remove_container(string R_name,string target_role_name){
    string linktype=get_link_type(R_name,target_role_name);
    string attributename=R_name+target_role_name+linktype;
    universal_data* udptr=get_attribute_ref_al(attributename);
    if(udptr!=NULL)
        remove_attribute_al(attributename);
    else
        cout<<"there is no "<<linktype<<" container to remove"<<endl;
}


void Logical_Entity_schema::remove_link(string R_name,Link linktemp,string target_role_name){
    string linktype=get_link_type(R_name,target_role_name);
    
    string attributename=R_name+target_role_name+linktype;
    if ((linktype=="_link")||(linktype=="_headlink"))
    {
        universal_data* udptr=get_attribute_ref_al(attributename);
        if(udptr!=NULL)
        {
            if(*((Link*)(udptr))==linktemp)
            {
                Link NULLlink;
                NULLlink.set_link(NULL);
                set_attribute_al(attributename,NULLlink);
            }
            else
                cout<<"there is no this link"<<endl;
        }
        else
            cout<<"nothing to remove"<<endl;
    }
    else if(linktype=="_linkset")
    {
        universal_data* udptr=get_attribute_ref_al(attributename);
        List* ltemp;
        if(udptr!=NULL)
        {
            ltemp=(List*)udptr;
            Link oldlink;
            for(ltemp->begin();!ltemp->end();(*ltemp)++)
            {
                oldlink=*((Link*)(*(*ltemp)));
                if (oldlink==linktemp)
                {
                    cout<<"this link  exit in linkset"<<endl;
                    ltemp->erase();
                    return;
                }
            }
        }
    }
    else
    {
        cout<<"can't find this link type"<<endl;
        exit(-1);
    }
}

void Logical_Entity_schema::add_linkinlinkset(string R_name,string target_role_name,Link linktemp){
    if((get_link_type(R_name,target_role_name)=="")||(get_link_type(R_name,target_role_name)=="_linkset"))
    {
        universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_linkset");
        List* ltemp;
        Link* linkptr=new Link();
        List listtemp;
        
        (*linkptr)=linktemp;
        if(udptr!=NULL)
        {
            ltemp=(List*)udptr;
            Link oldlink,newlink;
            for(ltemp->begin();!ltemp->end();(*ltemp)++)
            {
                oldlink=*((Link*)(*(*ltemp)));
                newlink=*linkptr;
                if (oldlink==newlink) // already exist
                {
                    cout<<"link is already existed"<<endl ;
                    return ;
                }
            }
            listtemp.push_back(linkptr);
            set_attribute_al(R_name+target_role_name+"_linkset", listtemp);
        }
        else
        {
            listtemp.push_back(linkptr);
            add_attribute_al(R_name+target_role_name+"_linkset", listtemp);
        }
    }
}

void Logical_Entity_schema::add_a_link(string R_name,string target_role_name,Link linktemp){
    if((get_link_type(R_name,target_role_name)=="")||(get_link_type(R_name,target_role_name)=="_link"))
    {
        universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_link");
        
        if(udptr!=NULL)
            set_attribute_al(R_name+target_role_name+"_link", linktemp);
        else
            add_attribute_al(R_name+target_role_name+"_link", linktemp);
    }
}

void Logical_Entity_schema::add_head_link(string R_name,string target_role_name,Link linktemp){
    if((get_link_type(R_name,target_role_name)=="")||(get_link_type(R_name,target_role_name)=="_headlink"))
    {
        universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_headlink");
        if(udptr!=NULL)
            set_attribute_al(R_name+target_role_name+"_headlink", linktemp);
        else
            add_attribute_al(R_name+target_role_name+"_headlink", linktemp);
    }
}


void Logical_Entity_schema::add_linkinlist(string R_name,string target_role_name,Relationship_Entity* REptr){
    if((get_link_type(R_name,target_role_name)=="")||(get_link_type(R_name,target_role_name)=="_list"))
    {
        universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_list");
        
        if(udptr!=NULL)
        {
            List* listtemp=(List*) udptr;
            listtemp->push_back(REptr);
        }
        else
        {
            List listobj;
            listobj.push_back(REptr);
            add_attribute_al(R_name+target_role_name+"_list", listobj);
        }
    }
}

void Logical_Entity_schema::add_a_next_link(string R_name,string target_role_name,Link linktemp){
    universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_nextlink");
    if(udptr!=NULL)
        set_attribute_al(R_name+target_role_name+"_nextlink", linktemp);
    else
        add_attribute_al(R_name+target_role_name+"_nextlink", linktemp);
}

void Logical_Entity_schema::remove_all_next_link(string R_name,string target_role_name){
    universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_nextlink");
    universal_data* udptrforE;
    Logical_Entity_schema* Etemp;
    Link* linktemp;
    Link linkobj;
    linkobj.set_link(NULL);
    if(udptr!=NULL)
    {
        linktemp=(Link*)udptr;
        udptrforE=(linktemp->get_link());
        
        if(udptrforE!=NULL)
        {
            Etemp=(Logical_Entity_schema*)udptrforE;
            Etemp->remove_all_next_link(R_name,target_role_name); // recursive remove (deep)
        }
        set_attribute_al(R_name+target_role_name+"_nextlink",linkobj);
    }
    else
        cout<<"nothing to remove"<<endl;
}

void Logical_Entity_schema::remove_a_next_link(string R_name,string traget_role_name){
    string attributename=R_name+traget_role_name+"_nextlink";
    universal_data* udptr=get_attribute_ref_al(attributename);
    if(udptr!=NULL)
    {
        Link nothinglink;
        nothinglink.set_link(NULL);
        set_attribute_al(attributename,nothinglink)	;
    }
    else
        cout<<"nothing to remove"<<endl;
}

universal_data* Logical_Entity_schema::get_link(string R_name,string target_role_name){
    string linktype=get_link_type(R_name,target_role_name);
    string attributename=R_name+target_role_name+linktype;
    
    universal_data* udptr=get_attribute_ref_al(attributename);
    if(udptr!=NULL)
        return udptr;
    else
        return NULL;
}

Link* Logical_Entity_schema::get_nextlink(string R_name,string target_role_name){
    universal_data* udptr=get_attribute_ref_al(R_name+target_role_name+"_nextlink");
    if(udptr!=NULL)
        return (Link*)udptr;
    else
        return NULL;
}

string Logical_Entity_schema::get_link_type(string R_name,string target_role_name){
    //this not include _nextlink
    string attributename;
    for(this->begin();!this->end();(*this)++)
    {
        attributename=*(((String*)&(this->get_attribute_name_al()))->getptr());
        size_t found=attributename.find(R_name+target_role_name);
        if (found!=string::npos)
        {
            found=attributename.find("_linkset");
            if (found!=string::npos)
            {
                return "_linkset";
            }
            found=attributename.find("_link");
            if (found!=string::npos)
            {
                return "_link";
            }
            found=attributename.find("_headlink");
            if (found!=string::npos)
            {
                
                return "_headlink";
            }
            found=attributename.find("_list");
            if (found!=string::npos)
            {
                
                return "_list";
            }
        }
    }
    return ""; // not found case
}

Attribute_List Logical_Entity_schema::get_all_link_info(){
    Attribute_List result ;
    
    for(this->begin() ; !this->end() ; (*this)++)
    {
        string attr_nm = *(((String*)(&(this->get_attribute_name_al())))->getptr()) ;
        int found = attr_nm.find("_linkset") ;
        if(found != string::npos)
            continue ;
        found = attr_nm.find("_link") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
        found = attr_nm.find("_headlink") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
        found = attr_nm.find("_nextlink") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
    }
    
    return result ;
}

Attribute_List Logical_Entity_schema::get_all_link_in_list_info(){
    Attribute_List result ;
    
    for(this->begin() ; !this->end() ; (*this)++)
    {
        string attr_nm = *(((String*)(&(this->get_attribute_name_al())))->getptr()) ;
        int found = attr_nm.find("_linkset") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
        found = attr_nm.find("_list") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
    }
    
    return result ;
}

void Logical_Entity_schema::cout_link_container(){
    string attributename;
    
    for(this->begin();!this->end();(*this)++)
    {
        attributename=*(((String*)&(this->get_attribute_name_al()))->getptr());
        size_t found=attributename.find("_link");
        if (found!=string::npos)
            cout<<"  "<<attributename<<endl;
        found=attributename.find("_nextlink");
        if (found!=string::npos)
            cout<<"  "<<attributename<<endl;
        found=attributename.find("_headlink");
        if (found!=string::npos)
            cout<<"  "<<attributename<<endl;
        found=attributename.find("_list");
        if (found!=string::npos)
            cout<<"  "<<attributename<<endl;
    }
    cout<<endl;
}

// --------------------------------------------
// --  logical relationship entity  schema
// --------------------------------------------
Relationship_Entity::Relationship_Entity(){
    List Listtemp;
    String stmp;
    Bool btmp;
    btmp.set_value(false);
    Map mtmp;
    add_attribute_al("recursive_flag", btmp) ;
    Int itmp;
    itmp.set_value(2);
    add_attribute_al("degree", itmp) ;
    set_R_name("");
    set_Unit_type("L_relationship_entity");
}

Relationship_Entity::Relationship_Entity(string R_name){
    String stmp;
    Bool btmp;
    btmp.set_value(false);
    Map mtmp;
    
    add_attribute_al("recursive_flag", btmp) ;
    Int itmp;
    itmp.set_value(2);
    add_attribute_al("degree", itmp) ;
    set_R_name(R_name);
    set_Unit_type("L_relationship_entity");
}

Relationship_Entity::~Relationship_Entity() {
}

void Relationship_Entity::set_R_name(string R_name){
    set_Unit_name(R_name);
}

string Relationship_Entity::get_R_name(){
    return get_Unit_name();
}

void Relationship_Entity::set_degree(int degree){
    Int itmp;
    itmp.set_value(degree);
    universal_data* udptr=get_attribute_ref_al("degree");
    if(udptr!=NULL)
        set_attribute_al("degree", itmp) ;
    else
        add_attribute_al("degree", itmp) ;
}

int Relationship_Entity::get_degree(){
    universal_data* udptr=get_attribute_ref_al("degree");
    if(udptr!=NULL)
    {
        Int* degree=(Int*)udptr;
        return *(degree->getptr());
    }
    else
        return -1;
}

void Relationship_Entity::set_recursive_flag(bool re_flag){
    Bool btmp;
    btmp.set_value(re_flag);
    universal_data* udptr=get_attribute_ref_al("recursive_flag");
    if(udptr!=NULL)
        set_attribute_al("recursive_flag", btmp) ;
    else
        add_attribute_al("recursive_flag", btmp) ;
}

bool Relationship_Entity::get_recursive_flag(){
    universal_data* udptr=get_attribute_ref_al("recursive_flag");
    if(udptr!=NULL)
    {
        Bool* btmp=(Bool*)udptr;
        return *(btmp->getptr());
    }
    else
        return false;
}

string Relationship_Entity::find_another_role_name(string source_role){
    list<string> rlist=get_role_list();
    list<string>::iterator rlist_iter;
    if(rlist.size()==0)
    {
        cout<<"role list error"<<endl;
        return "";
    }
    
    string roleinL;
    
    for (rlist_iter=rlist.begin();rlist_iter!=rlist.end();rlist_iter++)
    {
        roleinL=*rlist_iter;
        
        if(roleinL!=source_role)
            return roleinL;
    }
    cout<<"find_another_role_nameerror"<<endl;
    return "";
}

void Relationship_Entity::add_role_to_cardinality(string this_role,string target_cardi){
    String stemp;
    stemp.set_value(target_cardi);
    Attribute_List* attemp;
    universal_data* at_udptr=get_attribute_ref_al("rolelist");
    if(at_udptr!=NULL)
    {
        attemp=(Attribute_List*)at_udptr;
        universal_data* udptr=attemp->get_attribute_ref_al(this_role);
        
        if(udptr!=NULL)
            attemp->set_attribute_al(this_role, stemp);
        else
            attemp->add_attribute_al(this_role, stemp);
    }
    else
    {
        attemp=new Attribute_List();
        attemp->add_attribute_al(this_role, stemp);
        add_attribute_al("rolelist",*attemp);
    }
}

string Relationship_Entity::get_role_to_cardinality(string this_role){
    universal_data* at_udptr=get_attribute_ref_al("rolelist");
    if(at_udptr!=NULL)
    {
        Attribute_List* attemp=(Attribute_List*)at_udptr;
        universal_data* udptr=attemp->get_attribute_ref_al(this_role);
        
        if(udptr!=NULL)
        {
            String * cardinality=(String*)udptr;
            return *(cardinality->getptr());
        }
        else
            return "";
    }
    else
        return "";
}

void Relationship_Entity::set_degeneration(string role_name, string thedirection, bool degre){
    string direction;
    string rolename;
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

bool Relationship_Entity::get_degeneration(string role_name, string thedirection){
    if (thedirection=="RtoL")
    {
        if(role_name=="")
        {
            cout<<"direction  role has not been set yet"<<endl;
            exit(-1);
        }
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
        if(role_name=="")
        {
            cout<<"direction  role has not been set yet"<<endl;
            exit(-1);
        }
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

void Relationship_Entity::set_embedding(string role_name, string thedirection, string embedding){
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

string Relationship_Entity::get_embedding(string role_name, string thedirection){
    if (thedirection=="RtoL")
    {
        if(role_name=="")
        {
            cout<<"direction  role has not been set yet"<<endl;
            exit(-1);
        }
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
        if(role_name=="")
        {
            cout<<"direction  role has not been set yet"<<endl;
            exit(-1);
        }
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



void Relationship_Entity::set_link_decision(string role_name, string link_dec){
    String stemp;
    stemp.set_value(link_dec);
    if(role_name=="")
    {
        cout<<"direction  role has not been set yet"<<endl;
        exit(-1);
    }
    universal_data* udptr=get_attribute_ref_al(role_name+"link_decision");
    if(udptr!=NULL)
        set_attribute_al(role_name+"link_decision", stemp);
    else
        add_attribute_al(role_name+"link_decision", stemp);
}

string Relationship_Entity::get_link_decision(string role_name)
{
    if(role_name=="")
    {
        cout<<"direction  role has not been set yet"<<endl;
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

void Relationship_Entity::remove_link(string role_name){
    universal_data* udptr=get_attribute_ref_al(role_name+"_link");
    Link linktemp;
    if(udptr!=NULL)
        remove_attribute_al(role_name+"_link");
    else
        cout<<"nothing to remove"<<endl;
}

Link* Relationship_Entity::get_link(string role_name){
    universal_data* udptr=get_attribute_ref_al(role_name+"_link");
    Link* linktmp;
    if(udptr!=NULL)
    {
        linktmp=(Link*)udptr;
        return (Link*)udptr;
    }
    else
        return NULL;
}

Link* Relationship_Entity::get_next_link(string role_name){
    universal_data* udptr=get_attribute_ref_al(role_name+"_nextlink");
    Link* linktmp;
    if(udptr!=NULL)
    {
        linktmp=(Link*)udptr;
        return (Link*)udptr;
    }
    else
        return NULL;
}

void Relationship_Entity::remove_next_link(string role_name){
    universal_data* udptr=get_attribute_ref_al(role_name+"_nextlink");
    Link linktemp;
    if(udptr!=NULL)
        remove_attribute_al(role_name+"_nextlink");
    else
        cout<<"nothing to remove"<<endl;
}

void Relationship_Entity::add_a_link(string role_name,Link linktemp){
    universal_data* udptr=get_attribute_ref_al(role_name+"_link");
    if(udptr!=NULL)
        set_attribute_al(role_name+"_link", linktemp);
    else
        add_attribute_al(role_name+"_link", linktemp);
}

void Relationship_Entity::add_a_next_link(string role_name,Link linktemp){
    universal_data* udptr=get_attribute_ref_al(role_name+"_nextlink");
    if(udptr!=NULL)
        set_attribute_al(role_name+"_nextlink", linktemp);
    else
        add_attribute_al(role_name+"_nextlink", linktemp);
}

list<string> Relationship_Entity::get_role_list(){
    list<string> rolelist;
    string rolename;
    
    universal_data* at_udptr=get_attribute_ref_al("rolelist");
    if(at_udptr!=NULL)
    {
        Attribute_List* attemp=(Attribute_List*)at_udptr;
        for(attemp->begin();!attemp->end();(*attemp)++)
        {
            rolename=*(((String*)&(attemp->get_attribute_name_al()))->getptr());
            rolelist.push_back(rolename);
        }
        return rolelist;
    }
    else
        return rolelist ; // empty list
}

Attribute_List Relationship_Entity::get_all_link_info(){
    Attribute_List result ;
    
    for(this->begin() ; !this->end() ; (*this)++)
    {
        string attr_nm = *(((String*)(&(this->get_attribute_name_al())))->getptr()) ;
        int found = attr_nm.find("_link") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
        found = attr_nm.find("_nextlink") ;
        if( found != string::npos)
            result.add_attribute_al(this->get_attribute_name_al(), this->get_attribute_value_al()) ;
    }
    
    return result ;
}

// --------------------------------------------
// --  link
// --------------------------------------------
Link::Link(){
    set_user_defined_type_tag("LINK") ;
    
    set_linktag("true_link") ;
    
    universal_data udtemp;
    add_attribute_al("symbolic_link", udtemp) ;
    
    List ltemp;
    add_attribute_al("true_link", ltemp) ;
}

Link::Link(string link_decision){
    set_user_defined_type_tag("LINK") ;
    
    set_linktag(link_decision) ;
    
    universal_data udtemp;
    add_attribute_al("symbolic_link", udtemp) ;
    
    List ltemp;
    add_attribute_al("true_link", ltemp) ;
}

Link::~Link(){
}

void Link::set_linktag(string link_decision){
    if( (link_decision != "symbolic_link") && (link_decision != "true_link"))
    {
        cout<<"invlaid link decision: "<<link_decision<<endl ;
        exit(-1) ;
    }
    String stmp ;
    universal_data* udptr=get_attribute_ref_al("link_tag");
    stmp.set_value(link_decision) ;
    if(udptr!=NULL)
        set_attribute_al("link_tag", stmp);
    else
        add_attribute_al("link_tag", stmp);
}

string Link::get_linktag(){
    universal_data* udptr=get_attribute_ref_al("link_tag");
    
    if(udptr!=NULL)
    {
        String* stmp=(String*)udptr;
        return *(stmp->getptr());
    }
    else
        return "";
}

void Link::set_link(universal_data key){
    if(get_linktag()=="symbolic_link")
    {
        universal_data* udptr=get_attribute_ref_al("symbolic_link");
        
        if (udptr!=NULL)
            set_attribute_al("symbolic_link", key) ;
        else
            add_attribute_al("symbolic_link", key) ;
    }
    else
    {
        cout<<"this is not symbolic link"<<endl;
        exit(-1);
    }
}

void Link::set_link(universal_data* link){
    List ltemp;
    if(get_linktag()=="true_link")
    {
        universal_data* udptr=get_attribute_ref_al("true_link");
        
        if (udptr!=NULL)
        {
            List* listptr=(List*)udptr;
            while((listptr->size())!=0)
                listptr->pop_back();
            if(link!=NULL)
                listptr->push_back(link);
        }
        else
        {
            ltemp.push_back(link);
            add_attribute_al("true_link", ltemp) ;
        }
    }
    else
    {
        cout<<"this is not true link"<<endl;
        exit(-1);
    }
}


universal_data* Link::get_link(){
    List* ltemp;
    if(get_linktag()=="true_link")
    {
        universal_data* udptr=get_attribute_ref_al("true_link");
        if (udptr!=NULL)
        {
            ltemp=(List*)udptr;
            if((ltemp->size())==0)
                return NULL;
            else
            {
                ltemp->begin();
                return *(*ltemp);
            }
        }
        else
            return NULL;
    }
    else
    {
        cout<<"this is not true link"<<endl;
        exit(-1);
    }
}

universal_data Link::get_key(){
    if(get_linktag()=="symbolic_link")
    {
        universal_data* udptr=get_attribute_ref_al("symbolic_link");
        if (udptr!=NULL)
            return *udptr;
        else
        {
            universal_data udtemp;
            return udtemp;
        }
    }
    else
    {
        cout<<"this is not symbolic link"<<endl;
        universal_data udtemp;
        return udtemp;
    }
}

// --------------------------------------------
// --  transfer function
// --------------------------------------------
Logical_ERD_schema* RFtoL_ERD(RF_ERD_schema* RFerd)
{
    //1.create logical_ERD
    Logical_ERD_schema * Lerd_ptr=new Logical_ERD_schema(RFerd->get_ERD_name());
    
    //2.entity transformation
    Map* oldEtable=RFerd->get_Entity_table();//®˙•X©“¶≥¬¬™∫entity table
    RF_Entity_schema* oldE;
    Logical_Entity_schema* newE;
    for( oldEtable->begin();!oldEtable->end();(*oldEtable)++)//Ω∆ªs©“¶≥¬¬entity
    {
        oldE=(RF_Entity_schema*)oldEtable->value();
        newE=new Logical_Entity_schema(oldE->get_E_name());
        Attribute_List* attrptr = oldE->get_attribute_and_type() ;
        if(attrptr != NULL)
            newE->add_attribute_and_type(*attrptr) ;
        
        newE->set_set_flag(oldE->get_set_flag());
        
        if (newE->get_set_flag()==true)
        {
            newE->set_multi_key_flag(oldE->get_multi_key_flag ());
            Map* old_keyMap= oldE->get_key_unique_map();
            //≥v§@ß‚old_keyMap§§™∫®C§@ƒÊ®œ•ŒnewE -> add_key_unique(key_name, unique_flag)•[®Ï∑s™∫entity§§
            String* Keyname;
            string keyname;
            Bool* unique_flag;
            for( old_keyMap->begin();!old_keyMap->end();(*old_keyMap)++)
            {
                Keyname=(String*)(&old_keyMap->key());
                keyname=*(Keyname->getptr());
                unique_flag=(Bool*)(old_keyMap->value());
                newE->add_key_unique(keyname,*(unique_flag->getptr()));
            }
        }
        Lerd_ptr-> add_an_Entity(newE);
    }
    
    //3.relationship¬‡¥´
    Map* oldRtable=RFerd-> get_Relationship_table();//®˙•X©“¶≥¬¬™∫Relationship table
    RF_Relationship_schema* oldR;
    string new_Rname;
    string oldrolename,oldtargetrole, Ename;
    Role* oldRole;
    for( oldRtable->begin();!oldRtable->end();(*oldRtable)++)//πÔ©“¶≥Rß@¬‡¥´
    {
        oldR=(RF_Relationship_schema*) (oldRtable->value());
        string oldRname= oldR->get_R_name();
        Map* bindingmap= (Map*)(&(oldR->get_binding_list()));
        if((bindingmap->size())>2)
        {
            cout<<"wrong degree"<<endl;
            exit(-1);
        }
        Relationship_Entity* newRasE=new Relationship_Entity(oldRname);
        newRasE->set_recursive_flag(oldR->get_recursive_flag());
        
        Lerd_ptr->add_a_Relationship_entity(newRasE);
        for(bindingmap->begin();!bindingmap->end();(*bindingmap)++)//
        {
            
            oldRole=(Role*)(&(bindingmap->key()));
            oldrolename= oldRole->get_Role_name();
            
            oldE=oldR->find_a_binding(oldrolename);
            if(oldE!=NULL)
                Ename=oldE->get_E_name();
            else
            {
                cout<<"do not find E"<<endl;
                exit(-1);
            }
            
            Lerd_ptr->binding(Ename, oldRname,oldrolename);
            //copy degenaration
            newRasE->set_degeneration(oldrolename,"LtoR",oldR->get_degeneration(oldrolename,"LtoR"));
            newRasE->set_degeneration(oldrolename,"RtoL", oldR->get_degeneration(oldrolename,"RtoL"));
            //copy embedding
            newRasE->set_embedding(oldrolename,"LtoR",oldR->get_embedding(oldrolename,"LtoR"));
            newRasE->set_embedding(oldrolename,"RtoL",oldR->get_embedding(oldrolename,"RtoL"));
            //copy link_decision
            newRasE->set_link_decision(oldrolename, oldR->get_link_decision(oldrolename));
            
            //set cardinality from this role to target role
            string targetrolename=oldR ->find_another_role_name(oldrolename);
            Cardinality*  target_cardi=oldR ->Role_get_R_cardi(targetrolename);
            string cardi_max=target_cardi->get_Max();
            newRasE->add_role_to_cardinality(oldrolename,cardi_max);
        }
    }
    return Lerd_ptr;
}

// --------------------------------------------
// --  decision procedure
// --------------------------------------------
void decision_procedure(Logical_ERD_schema* LERDptr)
{
    //get the relationship entity table from the LERDptr
    Map* REmap=LERDptr->get_Relationship_entity_table();
    
    List* tuple_list=new List;
    //for each element REptr of relationship entity table
    
    //ß‰•X©“¶≥™∫≥sµ≤∏Í∞T
    for(REmap->begin();!REmap->end();(*REmap)++)
    {
        //´≈ßirole_list •Œlist´¨∫A  ™´•ÛÆ@!!
        List* tuple=new List();
        Relationship_Entity* REtemp=(Relationship_Entity*)(REmap->value());
        
        list<string> role_list=REtemp->get_role_list();
        
        String* tmpRname =new String;
        tmpRname->set_value(REtemp->get_R_name());
        tuple->push_back(tmpRname);
        int count=0;
        
        list<string>::iterator rlist_iter;
        if(role_list.size()!=2)
        {
            cout<<"role list error"<<role_list.size()<<endl;
            return ;
        }
        
        string roleinL;
        
        for (rlist_iter=role_list.begin();rlist_iter!=role_list.end();rlist_iter++)
        {//ß‚®‚≠”role¨€πÔ™∫entity©Ò®Ïtuple™∫≤ƒ§GƒÊ∏Ú≤ƒ§TƒÊ
            string therole=*rlist_iter;
            Link* source_link=REtemp->get_link(therole);
            if(source_link==NULL)
            {
                cout<<"error;ß‰§£®Ï≥o≠”®§¶‚™∫link"<<endl;
                exit(-1);
            }
            Logical_Entity_schema* srcEptr=(Logical_Entity_schema*)(source_link->get_link());
            
            if (count<2)//ß‚count=0©Mµ•©Û§@¶X®÷¨∞count<2
            {
                String* tmpEname01 =new String;
                tmpEname01->set_value( srcEptr->get_E_name());
                String* Rolename=new String();
                Rolename->set_value(therole);
                tuple->push_back(tmpEname01);
                tuple->push_back(Rolename);
            }
            else
            {
                cout<<"error"<<endl;
                exit(-1);
            }
            count++;
        }
        
        tuple_list->push_back(tuple);
        
        // degeneration
        for (rlist_iter=role_list.begin();rlist_iter!=role_list.end();rlist_iter++)
        {
            string therole=*rlist_iter;
            Link* source_link=REtemp->get_link(therole);
            if(source_link==NULL)
            {
                cout<<"error;ß‰§£®Ï≥o≠”®§¶‚™∫link"<<endl;
                exit(-1);
            }
            Logical_Entity_schema* srcEptr=(Logical_Entity_schema*)(source_link->get_link());
            
            direction_degrenerations(REtemp,therole,srcEptr);//•˝∞wπÔ¶π®§¶‚∞µ∞h§∆
        }
    }
    
    Relationship_Entity* REptr;
    Logical_Entity_schema* entity01,* entity02;
    
    //•H§Uß@§∫¥O
    for(tuple_list->begin();!tuple_list->end();(*tuple_list)++)
    {
        List* tuple=( List*)*(*tuple_list);
        
        tuple->begin();
        String* stmp =( String*)*(*tuple);
        string thename= *(stmp->getptr());
        string rolename01,rolename02;
        
        REptr= LERDptr->find_Relationship_entity(thename);
        
        (*tuple)++;
        stmp =( String*)*(*tuple);
        thename= *(stmp->getptr());
        
        entity01= LERDptr->find_Entity(thename);
        
        (*tuple)++;
        stmp =( String*)*(*tuple);
        rolename01= *(stmp->getptr());
        
        (*tuple)++;
        stmp =( String*)*(*tuple);
        thename= *(stmp->getptr());
        entity02= LERDptr->find_Entity(thename);
        
        (*tuple)++;
        stmp =( String*)*(*tuple);
        rolename02= *(stmp->getptr());
        
        //¶A∞wπÔ•H¶πrelationship∞µ§∫¥O®Mµ¶
        
        embedding(REptr,entity01,rolename01,entity02,rolename02);//
    }
}

void direction_degrenerations(Relationship_Entity* REptr,string rolename,Logical_Entity_schema* srcEptr)
{
    string  R_name=REptr->get_R_name();
    bool degeneration=REptr->get_degeneration(rolename,"RtoL");
    if(degeneration==true)//≤æ∞£±qrelationship ®Ïrole∫›™∫≥sµ≤
        REptr->remove_link(rolename);
    
    //get the decision of left-to-right direction degeneration from REptr
    degeneration=REptr->get_degeneration(rolename ,"LtoR");
    if(degeneration==true) //≤æ∞£±qrole∫›entity®Ïrelationship entity™∫≥sµ≤
    {
        string targetrolename=REptr->find_another_role_name(rolename);
        srcEptr->remove_container(R_name, targetrolename);
    }
}


void embedding(Relationship_Entity* REptr, Logical_Entity_schema* Entity1,string roleA, Logical_Entity_schema* Entity2,string roleB)
{
    string REname=REptr->get_R_name();
    //•˝ΩTª{¶π§Ë¶V¶≥µL∞h§∆
    //1.entity 1 -> entity 2
    bool dege01,dege02;
    dege01=REptr->get_degeneration(roleA,"LtoR");
    dege02=REptr->get_degeneration(roleB,"RtoL");
    string embedding01=REptr->get_embedding(roleA,"LtoR");
    string embedding02=REptr->get_embedding(roleB,"RtoL");
    
    if(dege01||dege02)
    {// already degenerated, only can no embedding
        if(embedding01==embedding02)
        {
            if(embedding01!="NA")
            {
                cout<<"§w∞h§∆µL™k§∫¥O:"<<embedding01<<endl;
                exit(-1);
            }
        }
        else
        {
            cout<<"§£§@≠P"<<endl;
            exit(-1);
        }	
    }
    
    if(embedding01!=embedding02)
    {
        cout<<embedding01<<" "<<roleA<<"embedding error"<<embedding02<<" "<<roleB<<endl;
        exit(-1);
    }
    
    if(embedding01=="source_embedding")
    {	
        //get the  left-to-right link from the REptr
        Link* e1_to_e2_link=REptr->get_link(roleB);
        if(e1_to_e2_link==NULL)
        {
            cout<<"can find the E2 link"<<endl;
            exit(-1);
        }
        
        //replace the link pointer of link slot with E1-to-E2 link
        string cardi=REptr->get_role_to_cardinality(roleA);
        
        Link* linkptr=(Link*)(Entity1->get_link(REname,roleB));
        
        if((linkptr->get_link())==(REptr))
        {
            Entity1->remove_container(REname,roleB);
            if(cardi=="1")
                Entity1->add_a_link(REname,roleB, *e1_to_e2_link);
            else
                Entity1->add_linkinlinkset(REname,roleB,*e1_to_e2_link);
            REptr->remove_link(roleB);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if(embedding01=="list_embedding")
    {	
        //get the  left-to-right link from the REptr
        string cardi01=REptr->get_role_to_cardinality(roleA);
        string cardi02=REptr->get_role_to_cardinality(roleB);
        
        if((cardi01!="1")&&(cardi02!="1"))
        {
            cout<<"§£§‰¥©¶hπÔ¶h"<<endl;
            exit(-1);
        }
        Link* linkptr=(Link*)(Entity1->get_link(REname,roleB));
        
        if((linkptr->get_link())==(REptr))
        {
            Entity1->remove_container(REname,roleB);
            Entity1->add_linkinlist(REname,roleB,REptr);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if (embedding01=="intermediate_embedding") 
    {
        Link* linkptr=(Link*)(Entity1->get_link(REname,roleB));
        Link NULLlink,linktemp;
        NULLlink.set_link(NULL);
        linktemp=*linkptr;
        if((linkptr->get_link())==(REptr))
        {
            REptr->add_a_next_link(roleB,NULLlink);
            Entity1->remove_container(REname,roleB);
            Entity1->add_head_link(REname,roleB,linktemp);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if(embedding01=="target_embedding")
    {	
        //(E)-(RE)-(E)
        //target embedding procedure
        
        //get the  left-to-right link from the REptr
        Link* e1_to_e2_link=REptr->get_link(roleB);
        if(e1_to_e2_link==NULL)
        {
            cout<<"can find the E2 link"<<endl;
            exit(-1);
        }
        
        //replace the link pointer of link slot with E1-to-E2 link
        
        Link* linkptr=(Link*)(Entity1->get_link(REname,roleB));
        if((linkptr->get_link())==(REptr))
        {
            Entity1->remove_container(REname,roleB);
            Entity1->add_head_link(REname,roleB,*e1_to_e2_link);
            
            REptr->remove_link(roleB);
            Link nextlink;
            nextlink.set_link(NULL);
            Entity2->add_a_next_link(REname,roleB,nextlink);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }	
    else if(embedding01=="NA")
    {	
        if(!(dege02||dege01))
        { // no degeneration 
            string cardi=REptr->get_role_to_cardinality(roleA);
            Link* linkptr=(Link*)(Entity1->get_link(REname,roleB));
            Link linktemp=*linkptr;
            if((linkptr->get_link())==(REptr))
            {
                if(cardi!="1")
                {// for n-ary case 			
                    Entity1->remove_container(REname,roleB);//Æeæπ§§™∫linkptr§]≥Q≤æ∞£§F
                    Entity1->add_linkinlinkset(REname,roleB,linktemp);
                }
            }
        }
    }
    else
    {
        cout<<"embedding error"<<embedding01<<endl;
        exit(-1);
    }
    
    //2.entity 2 -> entity 1
    embedding01=REptr->get_embedding(roleB,"LtoR");
    embedding02=REptr->get_embedding(roleA,"RtoL");
    dege01=REptr->get_degeneration(roleB,"LtoR");
    dege02=REptr->get_degeneration(roleA,"RtoL");
    if(dege01||dege02)
    {	
        if(embedding01==embedding02)
        {
            if(embedding01!="NA")
            {
                cout<<"§w∞h§∆µL™k§∫¥O:"<<embedding01<<endl;
                exit(-1);
            }
        }
        else
        {
            cout<<"§£§@≠P"<<endl;
            exit(-1);
        }
        
    }
    
    if(embedding01!=embedding02)
    {
        cout<<embedding01<<"embedding error"<<embedding02<<endl;
        exit(-1);
    }
    
    if(embedding02=="source_embedding")
    {	
        //get the  left-to-right link from the REptr
        Link* e2_to_e1_link=REptr->get_link(roleA);
        if(e2_to_e1_link==NULL)
        {
            cout<<"can find the E2 link"<<endl;
            exit(-1);
        }
        
        //replace the link pointer of link slot with E1-to-E2 link
        string cardi=REptr->get_role_to_cardinality(roleB);
        
        Link* linkptr=(Link*)(Entity2->get_link(REname,roleA));
        
        if((linkptr->get_link())==(REptr))
        {
            Entity2->remove_container(REname,roleA);
            if(cardi=="1")
                Entity2->add_a_link(REname,roleA,*e2_to_e1_link);
            else
                Entity2->add_linkinlinkset(REname,roleA,*e2_to_e1_link);
            REptr->remove_link(roleA);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if(embedding02=="list_embedding")
    {
        string cardi01=REptr->get_role_to_cardinality(roleB);
        string cardi02=REptr->get_role_to_cardinality(roleA);
        
        if((cardi01!="1")&&(cardi02!="1"))
        {
            cout<<"§£§‰¥©¶hπÔ¶h"<<endl;
            exit(-1);
        }
        Link* linkptr=(Link*)(Entity2->get_link(REname,roleA));
        if((linkptr->get_link())==(REptr))
        {
            Entity2->remove_container(REname,roleA);
            Entity2->add_linkinlist(REname,roleA,REptr);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if (embedding02=="intermediate_embedding") 
    {
        Link* linkptr=(Link*)(Entity2->get_link(REname,roleA));
        Link NULLlink,linktemp;
        NULLlink.set_link(NULL);
        linktemp=*linkptr;
        
        if((linkptr->get_link())==(REptr))
        {
            REptr->add_a_next_link(roleA,NULLlink);
            Entity2->remove_container(REname,roleA);
            Entity2->add_head_link(REname,roleA,linktemp);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if(embedding02=="target_embedding")
    {
        //(E)-(RE)-(E)
        //target embedding procedure
        //get the  left-to-right link from the REptr
        Link* e2_to_e1_link=REptr->get_link(roleA);
        if(e2_to_e1_link==NULL)
        {
            cout<<"can find the E2 link"<<endl;
            exit(-1);
        }
        
        //replace the link pointer of link slot with E1-to-E2 link
        Link* linkptr=(Link*)(Entity2->get_link(REname,roleA));
        
        if(linkptr->get_link()==(REptr))
        {
            Entity2->remove_container(REname,roleA);
            Entity2->add_head_link(REname,roleA,*e2_to_e1_link);
            REptr->remove_link(roleA);
            Link nextlink;
            nextlink.set_link(NULL);
            Entity1->add_a_next_link(REname,roleA,nextlink);
        }
        else
        {
            cout<<"the link no pointer to the relationship entity"<<endl;
            exit(-1);
        }
    }
    else if(embedding02=="NA")
    {
        if(!(dege02||dege01))
        {
            string cardi=REptr->get_role_to_cardinality(roleB);
            
            Link* linkptr=(Link*)(Entity2->get_link(REname,roleA));
            
            Link linktemp=*linkptr;
            if((linktemp.get_link())==(REptr))
            {
                if(cardi!="1")
                {// for n-ary case 		
                    Entity2->remove_container(REname,roleA);
                    Entity2->add_linkinlinkset(REname,roleA,linktemp);
                }
            }
        }
    }
    else
    {
        cout<<"embedding setting error"<<endl;
        exit(-1);
    }
}
