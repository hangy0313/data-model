#include "cardinalityERD.h"

using namespace std;

cardinalityERD::cardinalityERD():originalERDSchema()
{
}

cardinalityERD::cardinalityERD(string erdName):originalERDSchema(erdName)
{
}

cardinalityERD::~cardinalityERD()
{
}

list<cardinalityRelationship> cardinalityERD::getCardinalityRelationList()
{
    return cardinalityRelationList;
}

void cardinalityERD::addRelationship(cardinalityRelationship relationship)
{
    cardinalityRelationList.push_back(relationship);
}

void cardinalityERD::removeCardinalityRelation(string relationshipName)
{
    list<cardinalityRelationship>::iterator relationIter;
    
    for(relationIter = cardinalityRelationList.begin();relationIter != cardinalityRelationList.end();relationIter++){
        if((*relationIter).getRelationshipName() == relationshipName){
            cardinalityRelationList.erase(relationIter);
        }
    }
}

cardinalityRelationship cardinalityERD::findCardinalityRelation(string relationshipName)
{
    list<cardinalityRelationship>::iterator relationIter;
    
    for(relationIter = cardinalityRelationList.begin();relationIter != cardinalityRelationList.end();relationIter++){
        if((*relationIter).getRelationshipName() == relationshipName){
            return (*relationIter);
        }
    }
    return (*relationIter);
}


cardinalityRelationship::cardinalityRelationship() : relationshipSchema()
{
}

cardinalityRelationship::cardinalityRelationship(string relationshipName) : relationshipSchema(relationshipName)
{
}

cardinalityRelationship::~cardinalityRelationship()
{
}



void cardinalityRelationship::addRoleSchema(cardinalityRole role)
{
    cardinalityRoleList.push_back(role);
}

void cardinalityRelationship::addRoleSchema(string roleName,string entityName, int minNum, int maxNum)
{
    cardinalityRole tmp;
    
    tmp.setRoleName(roleName);
    tmp.setEntityName(entityName);
    tmp.setCardinality(minNum, maxNum);
    
    cardinalityRoleList.push_back(tmp);
}

void cardinalityRelationship::removeRole(string roleName)
{
    list<cardinalityRole>::iterator roleIter;
    
    for(roleIter = cardinalityRoleList.begin();roleIter != cardinalityRoleList.end();roleIter++){
        if((*roleIter).getRoleName() == roleName){
            cardinalityRoleList.erase(roleIter);
        }
    }
}

list<cardinalityRole> cardinalityRelationship::getcardinalityRoleList()
{
    return cardinalityRoleList;
}

string cardinalityRelationship::findEntityByRole(string roleName)
{
    list<cardinalityRole>::iterator roleIter;
    
    for(roleIter = cardinalityRoleList.begin();roleIter != cardinalityRoleList.end();roleIter++){
        if((*roleIter).getRoleName() == roleName){
            return (*roleIter).getEntityName();
        }
    }
    
    return "";
}

cardinalityRole cardinalityRelationship::findRole(string roleName)
{
    list<cardinalityRole>::iterator roleIter;
    
    for(roleIter = cardinalityRoleList.begin();roleIter != cardinalityRoleList.end();roleIter++){
        if((*roleIter).getRoleName() == roleName){
            return (*roleIter);
        }
    }
    return (*roleIter);
}

cardinalityRole::cardinalityRole() : roleSchema()
{
}

cardinalityRole::cardinalityRole(string roleName) : roleSchema(roleName)
{
}

cardinalityRole::~cardinalityRole()
{
}

void cardinalityRole::setCardinality(int minNum, int maxNum)
{
    cardinality.minNum = minNum;
    cardinality.maxNum = maxNum;
}
struct cardinalitySchema cardinalityRole::getCardinality()
{
    return cardinality;
}