#include "navigationERD.h"

using namespace std;

navigationERD::navigationERD() : cardinalityERD()
{
}

navigationERD::navigationERD(string erdName): cardinalityERD(erdName)
{
}

navigationERD::~navigationERD()
{
}

list<navigationEntity> navigationERD::getnavigationEntityList()
{
    return navigationEntityList;
}

void navigationERD::addEntity(navigationEntity entity)
{
    navigationEntityList.push_back(entity);
}

void navigationERD::removeEntity(string entityName)
{
    list<navigationEntity>::iterator iter;
    
    for(iter = navigationEntityList.begin();iter != navigationEntityList.end();iter++){
        if((*iter).getEntityName() == entityName){
            navigationEntityList.erase(iter);
        }
    }
}
navigationEntity navigationERD::findEntity(string entityName)
{
    list<navigationEntity>::iterator iter;
    
    for(iter = navigationEntityList.begin();iter != navigationEntityList.end();iter++){
        if((*iter).getEntityName() == entityName){
            return (*iter);
        }
    }
    return (*iter);
}

navigationEntity::navigationEntity() : entitySchema()
{
}

navigationEntity::navigationEntity(string entityName) : entitySchema(entityName)
{
}

navigationEntity::~navigationEntity()
{
}

void navigationEntity::addKey(keySchema key)
{
    keyList.push_back(key);
}

void navigationEntity::addKey(string type, string name)
{
    struct keySchema tmpKey;
    
    tmpKey.type = type;
    tmpKey.attributeName = name;
    
    keyList.push_back(tmpKey);
}

keySchema navigationEntity::getKey(string name)
{
    list<keySchema>::iterator iter;
    
    for(iter = keyList.begin();iter != keyList.end();iter++){
        if((*iter).attributeName == name){
            return (*iter);
        }
    }
    return (*iter);
}
list<keySchema> navigationEntity::getKeyList()
{
    return keyList;
}