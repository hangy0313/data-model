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

void navigationEntity::addPrimaryKey(string keyName)
{
    struct keySchema tmpKey;
    list<keySchema>::iterator iter;
    
    for(iter = keyList.begin();iter != keyList.end();iter++){
        if((*iter).type == "primary_key"){
            cout << "primary key is existed" << endl;
            return;
        }
    }
    
    tmpKey.type = "primary_key";
    tmpKey.attributeList.push_back(keyName);
    
    keyList.push_back(tmpKey);
}

string navigationEntity::getPrimaryKey()
{
    list<keySchema>::iterator iter;
    
    for(iter = keyList.begin();iter != keyList.end();iter++){
        if((*iter).type == "primary_key"){
            return (*(*iter).attributeList.begin());
        }
    }
    
    cout << "There is no primary key" << endl;
    exit(1);
}

void navigationEntity::addMultiKey(list<string> attList)
{
    struct keySchema tmpKey;
    
    tmpKey.type = "multi_key";
    tmpKey.attributeList = attList;
    
    keyList.push_back(tmpKey);
}

list<keySchema> navigationEntity::getKeyList()
{
    return keyList;
}