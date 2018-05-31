#include "originalERD.h"

using namespace std;

originalERDSchema::originalERDSchema()
{
    erdName = "";
}

originalERDSchema::originalERDSchema(string erdName)
{
    erdName = erdName;
}

originalERDSchema::~originalERDSchema()
{
}

string originalERDSchema::get_ERD_name()
{
    return erdName;
}

list<entitySchema> originalERDSchema::getEntityList()
{
    return entityList;
}
list<relationshipSchema> originalERDSchema::getRelationshipList()
{
    return relationshipList;
}

void originalERDSchema::addEntity(entitySchema entity)
{
    entityList.push_back(entity);
}

void originalERDSchema::removeEntity(string entityName)
{
    list<entitySchema>::iterator entityIter;
    
    for(entityIter = entityList.begin();entityIter != entityList.end();entityIter++){
        if((*entityIter).getEntityName() == entityName){
            entityList.erase(entityIter);
        }
    }
}

void originalERDSchema::addRelationship(relationshipSchema relationship)
{
    relationshipList.push_back(relationship);
}

void originalERDSchema::removeRelationship(string relationshipName)
{
    list<relationshipSchema>::iterator relationIter;
    
    for(relationIter = relationshipList.begin();relationIter != relationshipList.end();relationIter++){
        if((*relationIter).getRelationshipName() == relationshipName){
            relationshipList.erase(relationIter);
        }
    }
}

entitySchema originalERDSchema::find_Entity(string entityName)
{
    list<entitySchema>::iterator entityIter;
    
    for(entityIter = entityList.begin();entityIter != entityList.end();entityIter++){
        if((*entityIter).getEntityName() == entityName){
            return (*entityIter);
        }
    }
    return (*entityIter);
}

relationshipSchema originalERDSchema::find_Relationship(string relationshipName)
{
    list<relationshipSchema>::iterator relationIter;
    
    for(relationIter = relationshipList.begin();relationIter != relationshipList.end();relationIter++){
        if((*relationIter).getRelationshipName() == relationshipName){
            return (*relationIter);
        }
    }
    return (*relationIter);
}

entitySchema::entitySchema()
{
    name = "";
}

entitySchema::entitySchema(string entityName)
{
    name = entityName;
}

entitySchema::~entitySchema()
{
}

void entitySchema::setEntityName(string entityName)
{
    name = entityName;
}

string entitySchema::getEntityName()
{
    return name;
}

void entitySchema::addAttribute(string attributeName,string attributeType)
{
    struct attributeSchema tmp;
    
    tmp.name = attributeName;
    tmp.type = attributeType;
    
    attributeList.push_back(tmp);
}

void entitySchema::addAttribute(attributeSchema tmpAttribute)
{
    attributeList.push_back(tmpAttribute);
}

void entitySchema::removeAttribute(string attributeName)
{
    list<attributeSchema>::iterator attributeIter;
    
    for(attributeIter = attributeList.begin();attributeIter != attributeList.end();attributeIter++){
        if((*attributeIter).name == attributeName){
            attributeList.erase(attributeIter);
        }
    }
}

list<attributeSchema> entitySchema::getAttributeList()
{
    return attributeList;
}

relationshipSchema::relationshipSchema()
{
    name = "";
}

relationshipSchema::relationshipSchema(string relationshipName)
{
    name = relationshipName;
}

relationshipSchema::~relationshipSchema()
{
    
}

void relationshipSchema::setRelationshipName(string relationshipName)
{
    name = relationshipName;
}

string relationshipSchema::getRelationshipName()
{
    return name;
}

void relationshipSchema::setDegree(int degree)
{
    this->degree = degree;
}

int relationshipSchema::getDegree()
{
    return degree;
}

void relationshipSchema::addRoleSchema(roleSchema role)
{
    roleList.push_back(role);
}

void relationshipSchema::addRoleSchema(string roleName,string entityName)
{
    roleSchema tmp;
    
    tmp.setRoleName(roleName);
    tmp.setEntityName(entityName);
    
    roleList.push_back(tmp);
}

void relationshipSchema::removeRole(string roleName)
{
    list<roleSchema>::iterator roleIter;
    
    for(roleIter = roleList.begin();roleIter != roleList.end();roleIter++){
        if((*roleIter).getRoleName() == roleName){
            roleList.erase(roleIter);
        }
    }
}

list<roleSchema> relationshipSchema::getRoleList()
{
    return roleList;
}

string relationshipSchema::findEntityByRole(string roleName)
{
    list<roleSchema>::iterator roleIter;
    
    for(roleIter = roleList.begin();roleIter != roleList.end();roleIter++){
        if((*roleIter).getRoleName() == roleName){
            return (*roleIter).getEntityName();
        }
    }
    
    return "";
}

roleSchema relationshipSchema::findRole(string roleName)
{
    list<roleSchema>::iterator roleIter;
    
    for(roleIter = roleList.begin();roleIter != roleList.end();roleIter++){
        if((*roleIter).getRoleName() == roleName){
            return (*roleIter);
        }
    }
    return (*roleIter);
}

roleSchema::roleSchema()
{
    name = "";
}
roleSchema::roleSchema(string roleName)
{
    name = roleName;
}
roleSchema::~roleSchema()
{
    
}

void roleSchema::setRoleName(string roleName)
{
    name = roleName;
}

string roleSchema::getRoleName()
{
    return name;
}

void roleSchema::setEntityName(string entityName)
{
    this->entityName = entityName;
}

string roleSchema::getEntityName()
{
    return entityName;
}