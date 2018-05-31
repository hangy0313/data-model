#include "transformedERD.h"

using namespace std;

specializeERD::specializeERD() : navigationERD()
{
}

specializeERD::specializeERD(string erdName) : navigationERD(erdName)
{
}

specializeERD::~specializeERD()
{
}

list<specializeEntity> specializeERD::getSpecializeEntityList()
{
    return specializeEntityList;
}

list<specializeRelationship> specializeERD::getSpecializeRelationList()
{
    return specializeRelationList;
}

void specializeERD::addEntity(specializeEntity entity)
{
    specializeEntityList.push_back(entity);
}

void specializeERD::removeEntity(string entityName)
{
    list<specializeEntity>::iterator iter;
    
    for(iter = specializeEntityList.begin();iter != specializeEntityList.end();iter++){
        if((*iter).getEntityName() == entityName){
            specializeEntityList.erase(iter);
        }
    }
}

void specializeERD::addRelationship(specializeRelationship relationship)
{
    specializeRelationList.push_back(relationship);
}

void specializeERD::removeRelationship(string relationshipName)
{
    list<specializeRelationship>::iterator iter;
    
    for(iter = specializeRelationList.begin();iter != specializeRelationList.end();iter++){
        if((*iter).getRelationshipName() == relationshipName){
            specializeRelationList.erase(iter);
        }
    }
}

specializeEntity specializeERD::findEntity(string entityName)
{
    list<specializeEntity>::iterator iter;
    
    for(iter = specializeEntityList.begin();iter != specializeEntityList.end();iter++){
        if((*iter).getEntityName() == entityName){
            return (*iter);
        }
    }
    return (*iter);
}

specializeRelationship specializeERD::find_Relationship(string relationshipName)
{
    list<specializeRelationship>::iterator iter;
    
    for(iter = specializeRelationList.begin();iter != specializeRelationList.end();iter++){
        if((*iter).getRelationshipName() == relationshipName){
            return (*iter);
        }
    }
    return (*iter);
}

specializeEntity::specializeEntity() : navigationEntity()
{
}

specializeEntity::specializeEntity(string entityName) : navigationEntity(entityName)
{
}

specializeEntity::~specializeEntity()
{
}

void specializeEntity::addLinkSchema(linkShema link)
{
    linkShemaList.push_back(link);
}

void specializeEntity::addLinkSchema(string linkTarget, string type = "true_link")
{
    struct linkShema tmpLink;
    
    tmpLink.type = type;
    tmpLink.linkTarget = linkTarget;
    
    linkShemaList.push_back(tmpLink);
}

void specializeEntity::removeLinkSchema(string linkTarget)
{
    list<linkShema>::iterator iter;
    
    for(iter = linkShemaList.begin();iter != linkShemaList.end();iter++){
        if((*iter).linkTarget == linkTarget){
            linkShemaList.erase(iter);
        }
    }
}

specializeRelationship::specializeRelationship() : cardinalityRelationship()
{
}

specializeRelationship::specializeRelationship(string entityName) : cardinalityRelationship(entityName)
{
}

specializeRelationship::~specializeRelationship()
{
}

void specializeRelationship::addLinkSchema(linkShema link)
{
    linkShemaList.push_back(link);
}

void specializeRelationship::addLinkSchema(string linkTarget, string type = "true_link")
{
    struct linkShema tmpLink;
    
    tmpLink.type = type;
    tmpLink.linkTarget = linkTarget;
    
    linkShemaList.push_back(tmpLink);
}

void specializeRelationship::removeLinkSchema(string linkTarget)
{
    list<linkShema>::iterator iter;
    
    for(iter = linkShemaList.begin();iter != linkShemaList.end();iter++){
        if((*iter).linkTarget == linkTarget){
            linkShemaList.erase(iter);
        }
    }
}