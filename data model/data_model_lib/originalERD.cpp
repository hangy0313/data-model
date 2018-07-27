#include "originalERD.h"

using namespace std;

ERD::ERD(string erdName)
{
    setERDName(erdName);
    
    UD_Map mapTmp;
    add_attribute_al("Entity_table", mapTmp);
    add_attribute_al("Relationship_table", mapTmp);
}

ERD::~ERD()
{
}

void ERD::setERDName(string erdName)
{
    UD_String tmp;
    tmp.set_value(erdName);
    
    UD_universal_data* ptr = get_attribute_ref_al("ERD_name");
    if(ptr != NULL) {
        set_attribute_al("ERD_name", tmp);
    } else {
        add_attribute_al("ERD_name", tmp);
    }
}

string ERD::getERDName()
{
    UD_universal_data *tmp = get_attribute_ref_al("ERD_name");
    
    UD_String* stringTmp = (UD_String*)tmp;
    
    return *(stringTmp->getptr());
}

UD_Map* ERD::getEntityTable()
{
    return (UD_Map*)(get_attribute_ref_al("Entity_table"));
}

UD_Map* ERD::getRelationshipTable()
{
    return (UD_Map*)(get_attribute_ref_al("Relationship_table"));
}

void ERD::addEntity(Entity* en)
{
    UD_String stringTmp;
    stringTmp.set_value(en->getEntityName());
    UD_Map* mapTmp = getEntityTable();
    
    mapTmp->insert(stringTmp, en);
}

void ERD::removeEntity(string entityName)
{
    UD_String stringTmp;
    stringTmp.set_value(entityName);
    UD_Map* mapTmp = getEntityTable();
    
    mapTmp->erase(stringTmp);
}

void ERD::addRelationship(Relationship* relationship)
{
    UD_String stringTmp;
    stringTmp.set_value(relationship->getRelationshipName());
    UD_Map* mapTmp = getRelationshipTable();
    
    mapTmp->insert(stringTmp, relationship);
}

void ERD::removeRelationship(string relationshipName)
{
    UD_String stringTmp;
    stringTmp.set_value(relationshipName);
    UD_Map* mapTmp = getRelationshipTable();
    
    mapTmp->erase(stringTmp);
}

void ERD::dump()
{
    cout << "ERD Name : " << getERDName() << endl;
    cout << "==Entity Table==" << endl;
    
    UD_Map* entityTable = getEntityTable();
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        Entity* entity = (Entity*)(entityTable->value());
        entity->dump();
    }
    
    UD_Map* relationshipTable = getRelationshipTable();
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationship = (Relationship*)(relationshipTable->value());
        relationship->dump();
    }
}

Entity* ERD::findEntity(string entityName)
{
    UD_String stringTmp;
    stringTmp.set_value(entityName);
    UD_Map* mapTmp = getEntityTable();
    
    mapTmp->find(stringTmp);
    
    return (Entity*)(mapTmp);
}

Relationship* ERD::findRelationship(string relationshipName)
{
    UD_String stringTmp;
    stringTmp.set_value(relationshipName);
    UD_Map* mapTmp = getRelationshipTable();
    
    mapTmp->find(stringTmp);
    
    return (Relationship*)(mapTmp);
}

Entity::Entity()
{
    UD_Attribute_List attributeListTmp;
    
    setEntityName("");
    add_attribute_al("Attribute_list", attributeListTmp);
}

Entity::Entity(string entityName)
{
    UD_Attribute_List attributeListTmp;
    
    setEntityName(entityName);
    add_attribute_al("Attribute_list", attributeListTmp);
}

Entity::~Entity()
{
    
}

void Entity::setEntityName(string entityName)
{
    UD_String tmp;
    tmp.set_value(entityName);
    
    UD_universal_data* ptr = get_attribute_ref_al("Entity_name");
    if(ptr != NULL) {
        set_attribute_al("Entity_name", tmp);
    } else {
        add_attribute_al("Entity_name", tmp);
    }
}

string Entity::getEntityName()
{
    return *((UD_String*)(get_attribute_ref_al("Entity_name")))->getptr();
}

void Entity::addAttribute(string attributeName,string attributeType)
{
    UD_String stringTmp;
    stringTmp.set_value(attributeType);
    UD_Attribute_List* tmp = (UD_Attribute_List*)(get_attribute_ref_al("Attribute_list"));
    
    tmp->add_attribute_al(attributeName, stringTmp);
}

void Entity::addAttribute(UD_Attribute_List tmpAttribute)
{
    UD_universal_data tmp = tmpAttribute.get_attribute();
    for(tmpAttribute.begin();tmpAttribute.end();tmpAttribute++){
        addAttribute(tmpAttribute.get_attribute_name(), *(((UD_String*)(&tmp))->getptr()));
    }
}

void Entity::removeAttribute(string attributeName)
{
    UD_Attribute_List* tmp = (UD_Attribute_List*)(get_attribute_ref_al("Attribute_list"));
    
    tmp->remove_attribute_al(attributeName);
}

UD_Attribute_List* Entity::getAttributeList()
{
    return (UD_Attribute_List*)(get_attribute_ref_al("Attribute_list"));
}

void Entity::dump()
{
    cout << "==========" << endl;
    cout << "Entity Name : " << getEntityName() << endl;
    
    UD_Attribute_List* attList = getAttributeList();
    for(attList->begin();!attList->end();(*attList)++){
        UD_universal_data name = attList->get_attribute_name_al();
        UD_universal_data type = attList->get_attribute_value_al();
        cout << "name : " << *(((UD_String*)(&name))->getptr())
             << ", type : " << *(((UD_String*)(&type))->getptr()) << endl;
    }
    cout << "==========" << endl;
}

Relationship::Relationship()
{
    setRelationshipName("");
    
    UD_Attribute_List tmp;
    
    add_attribute_al("Role_list", tmp);
}

Relationship::Relationship(string relationshipName)
{
    setRelationshipName(relationshipName);
    
    UD_Attribute_List tmp;
    
    add_attribute_al("Role_list", tmp);
}

Relationship::~Relationship()
{
    
}

void Relationship::setRelationshipName(string relationshipName)
{
    UD_String stringTmp;
    stringTmp.set_value(relationshipName);
    
    UD_universal_data* ptr = get_attribute_ref_al("Relationship_name");
    
    if(ptr != NULL) {
        set_attribute_al("Relationship_name", stringTmp);
    } else {
        add_attribute_al("Relationship_name", stringTmp);
    }
}

string Relationship::getRelationshipName()
{
    return *(((UD_String*)(get_attribute_ref_al("Relationship_name")))->getptr());
}

void Relationship::addRole(Role role)
{
    string roleName = role.getRoleName();
    
    UD_Attribute_List* attPtr = getRoleList();
    
    attPtr->add_attribute_al(roleName, role);
}

void Relationship::addRole(string roleName, string entityName)
{
    UD_Attribute_List* attPtr = getRoleList();
    
    Role* roleTmp = new Role(roleName);
    roleTmp->setEntityName(entityName);
    
    attPtr->add_attribute_al(roleName, *roleTmp);
}

void Relationship::removeRole(string roleName)
{
    UD_Attribute_List* attPtr = getRoleList();
    
    attPtr->remove_attribute_al(roleName);
}

Role* Relationship::findRole(string roleName)
{
    UD_Attribute_List* attPtr = getRoleList();
    
    return (Role*)(attPtr->get_attribute_ref_al(roleName));
}

UD_Attribute_List* Relationship::getRoleList()
{
    return (UD_Attribute_List*)(get_attribute_ref_al("Role_list"));
}

void Relationship::dump()
{
    cout << "==========" << endl;
    cout << "Relationship Name : " << getRelationshipName() << endl;
    
    UD_Attribute_List* roleList = getRoleList();
    for(roleList->begin();!roleList->end();(*roleList)++){
        Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
        cout << "Role name : " << role->getRoleName()
             << ", Entity name : " << role->getEntityName() << endl;
    }
    cout << "==========" << endl;
}

Role::Role()
{
    UD_String stringTmp;
    
    setRoleName("");
    add_attribute_al("Entity_name", stringTmp);
}

Role::Role(string roleName)
{
    UD_String stringTmp;
    
    setRoleName(roleName);
    add_attribute_al("Entity_name", stringTmp);
}

Role::~Role()
{
    
}

void Role::setRoleName(string roleName)
{
    UD_String stringTmp;
    stringTmp.set_value(roleName);
    
    UD_universal_data* ptr = get_attribute_ref_al("Role_name");
    
    if(ptr != NULL) {
        set_attribute_al("Role_name", stringTmp);
    } else {
        add_attribute_al("Role_name", stringTmp);
    }
}
string Role::getRoleName()
{
    return *(((UD_String*)(get_attribute_ref_al("Role_name")))->getptr());
}

void Role::setEntityName(string entityName)
{
    UD_String stringTmp;
    stringTmp.set_value(entityName);
    
    set_attribute_al("Entity_name", stringTmp);
}

string Role::getEntityName()
{
    return *(((UD_String*)(get_attribute_ref_al("Entity_name")))->getptr());
}

ERD* import(string erdScript)
{
    string label, tmp;
    ERD* erd = new ERD("tmp");
    ifstream erdInput(erdScript);
    
    erdInput >> label;
    while(label != "End_ERD"){
        if(label == "ERD"){
            erdInput >> tmp;
            erd->setERDName(tmp);
        }
        if(label == "Entity"){
            string entityLabel;
            
            erdInput >> tmp;
            Entity* entity = new Entity(tmp);
            
            erdInput >> entityLabel;
            while(entityLabel != "End_Entity"){
                if(entityLabel == "Attribute"){
                    string type, name;
                    
                    erdInput >> type;
                    erdInput >> name;
                    entity->addAttribute(name, type);
                    
                    erdInput >> entityLabel;
                }
            }
            erd->addEntity(entity);
        }
        if(label == "Relationship"){
            string relationLabel;
            
            erdInput >> tmp;
            Relationship* relationship = new Relationship(tmp);
            
            erdInput >> relationLabel;
            while(relationLabel != "End_Relationship"){
                if(relationLabel == "Role"){
                    string roleName, entityName;
                    
                    erdInput >> roleName;
                    erdInput >> entityName;
                    relationship->addRole(roleName, entityName);
                    
                    erdInput >> relationLabel;
                }
            }
            erd->addRelationship(relationship);
        }
        erdInput >> label;
    }
    
    return erd;
}
