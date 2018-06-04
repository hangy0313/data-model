#include "originalERD.h"

using namespace std;

ERD::ERD(string erdName)
{
    setERDName(erdName);
    
    Map mapTmp;
    add_attribute_al("Entity_table", mapTmp);
    add_attribute_al("Relationship_table", mapTmp);
}

ERD::~ERD()
{
}

void ERD::setERDName(string erdName)
{
    String tmp;
    tmp.set_value(erdName);
    
    universal_data* ptr = get_attribute_ref_al("ERD_name");
    if(ptr != NULL) {
        set_attribute_al("ERD_name", tmp);
    } else {
        add_attribute_al("ERD_name", tmp);
    }
}

string ERD::getERDName()
{
    universal_data *tmp = get_attribute_ref_al("ERD_name");
    
    String* stringTmp = (String*)tmp;
    
    return *(stringTmp->getptr());
}

Map* ERD::getEntityTable()
{
    return (Map*)(get_attribute_ref_al("Entity_table"));
}

Map* ERD::getRelationshipTable()
{
    return (Map*)(get_attribute_ref_al("Relationship_table"));
}

void ERD::addEntity(Entity* en)
{
    String stringTmp;
    stringTmp.set_value(en->getEntityName());
    Map* mapTmp = getEntityTable();
    
    mapTmp->insert(stringTmp, en);
}

void ERD::removeEntity(string entityName)
{
    String stringTmp;
    stringTmp.set_value(entityName);
    Map* mapTmp = getEntityTable();
    
    mapTmp->erase(stringTmp);
}

void ERD::addRelationship(Relationship* relationship)
{
    String stringTmp;
    stringTmp.set_value(relationship->getRelationshipName());
    Map* mapTmp = getRelationshipTable();
    
    mapTmp->insert(stringTmp, relationship);
}

void ERD::removeRelationship(string relationshipName)
{
    String stringTmp;
    stringTmp.set_value(relationshipName);
    Map* mapTmp = getRelationshipTable();
    
    mapTmp->erase(stringTmp);
}

Entity* ERD::findEntity(string entityName)
{
    String stringTmp;
    stringTmp.set_value(entityName);
    Map* mapTmp = getEntityTable();
    
    mapTmp->find(stringTmp);
    
    return (Entity*)(mapTmp);
}

Relationship* ERD::findRelationship(string relationshipName)
{
    String stringTmp;
    stringTmp.set_value(relationshipName);
    Map* mapTmp = getRelationshipTable();
    
    mapTmp->find(stringTmp);
    
    return (Relationship*)(mapTmp);
}

Entity::Entity()
{
    Attribute_List attributeListTmp;
    
    setEntityName("");
    add_attribute_al("Attribute_list", attributeListTmp);
}

Entity::Entity(string entityName)
{
    Attribute_List attributeListTmp;
    
    setEntityName("entityName");
    add_attribute_al("Attribute_list", attributeListTmp);
}

Entity::~Entity()
{
    
}

void Entity::setEntityName(string entityName)
{
    String tmp;
    tmp.set_value(entityName);
    
    universal_data* ptr = get_attribute_ref_al("Entity_name");
    if(ptr != NULL) {
        set_attribute_al("Entity_name", tmp);
    } else {
        add_attribute_al("Entity_name", tmp);
    }
}

string Entity::getEntityName()
{
    return *((String*)(get_attribute_ref_al("Entity_name")))->getptr();
}

void Entity::addAttribute(string attributeName,string attributeType)
{
    String stringTmp;
    stringTmp.set_value(attributeType);
    Attribute_List* tmp = (Attribute_List*)(get_attribute_ref_al("Attribute_list"));
    
    tmp->set_attribute_al(attributeName, stringTmp);
}

void Entity::addAttribute(Attribute_List tmpAttribute)
{
    universal_data tmp = tmpAttribute.get_attribute();
    for(tmpAttribute.begin();tmpAttribute.end();tmpAttribute++){
        addAttribute(tmpAttribute.get_attribute_name(), *(((String*)(&tmp))->getptr()));
    }
}

void Entity::removeAttribute(string attributeName)
{
    Attribute_List* tmp = (Attribute_List*)(get_attribute_ref_al("Attribute_list"));
    
    tmp->remove_attribute_al(attributeName);
}

Attribute_List* Entity::getAttributeList()
{
    return (Attribute_List*)(get_attribute_ref_al("Attribute_list"));
}

Relationship::Relationship()
{
    setRelationshipName("");
    
    Attribute_List tmp;
    
    add_attribute_al("Role_list", tmp);
}

Relationship::Relationship(string relationshipName)
{
    setRelationshipName(relationshipName);
    
    Attribute_List tmp;
    
    add_attribute_al("Role_list", tmp);
}

Relationship::~Relationship()
{
    
}

void Relationship::setRelationshipName(string relationshipName)
{
    String stringTmp;
    stringTmp.set_value(relationshipName);
    
    universal_data* ptr = get_attribute_ref_al("Relationship_name");
    
    if(ptr != NULL) {
        set_attribute_al("Relationship_name", stringTmp);
    } else {
        add_attribute_al("Relationship_name", stringTmp);
    }
}

string Relationship::getRelationshipName()
{
    return *(((String*)(get_attribute_ref_al("Relationship_name")))->getptr());
}

void Relationship::addRole(Role role)
{
    string roleName = role.getRoleName();
    
    Attribute_List* attPtr = getRoleList();
    
    attPtr->add_attribute_al(roleName, role);
}

void Relationship::addRole(string roleName, string entityName)
{
    Attribute_List* attPtr = getRoleList();
    
    Role* roleTmp = new Role(roleName);
    roleTmp->setEntityName(entityName);
    
    attPtr->add_attribute_al(roleName, *roleTmp);
}

void Relationship::removeRole(string roleName)
{
    Attribute_List* attPtr = getRoleList();
    
    attPtr->remove_attribute_al(roleName);
}

Role* Relationship::findRole(string roleName)
{
    Attribute_List* attPtr = getRoleList();
    
    return (Role*)(attPtr->get_attribute_ref_al(roleName));
}

Attribute_List* Relationship::getRoleList()
{
    return (Attribute_List*)(get_attribute_ref_al("Role_list"));
}

Role::Role()
{
    String stringTmp;
    
    setRoleName("");
    add_attribute_al("Entity_name", stringTmp);
}

Role::Role(string roleName)
{
    String stringTmp;
    
    setRoleName(roleName);
    add_attribute_al("Entity_name", stringTmp);
}

Role::~Role()
{
    
}

void Role::setRoleName(string roleName)
{
    String stringTmp;
    stringTmp.set_value(roleName);
    
    universal_data* ptr = get_attribute_ref_al("Role_name");
    
    if(ptr != NULL) {
        set_attribute_al("Role_name", stringTmp);
    } else {
        add_attribute_al("Role_name", stringTmp);
    }
}
string Role::getRoleName()
{
    return *(((String*)(get_attribute_ref_al("Role_name")))->getptr());
}

void Role::setEntityName(string entityName)
{
    String stringTmp;
    stringTmp.set_value(entityName);
    
    set_attribute_al("Entity_name", stringTmp);
}

string Role::getEntityName()
{
    return *(((String*)(get_attribute_ref_al("Entity_name")))->getptr());
}