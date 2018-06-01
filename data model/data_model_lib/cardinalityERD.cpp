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

list<cardinalityRole> importCardinalityInfo()
{
    ifstream cardinalityInput("./erdScript/cardinality.txt");
    
    list<cardinalityRole> tmpRoleList;
    cardinalityRole tmpRole;
    string roleName, minNumString, maxNumString;
    
    while(cardinalityInput >> roleName){
        cardinalityInput >> minNumString;
        cardinalityInput >> maxNumString;

        tmpRole.setRoleName(roleName);
        tmpRole.setCardinality(stoi(minNumString), stoi(maxNumString));
        
        tmpRoleList.push_back(tmpRole);
    }
    
    return tmpRoleList;
}

cardinalityERD* transferToCardinalityERD(originalERDSchema *erd)
{
    cardinalityERD* newERD = new cardinalityERD(erd->get_ERD_name());
    
    //import cardinality info
    list<cardinalityRole> carRoleList = importCardinalityInfo();
    
    //duplicate entity list
    list<entitySchema>::iterator entityIter;
    list<entitySchema>::iterator entityBegin = erd->getEntityList().begin();
    list<entitySchema>::iterator entityEnd = erd->getEntityList().end();
    for(entityIter = entityBegin;entityIter != entityEnd;entityIter++){
        newERD->addEntity(*entityIter);
    }
    
    //relationshipSchema to cardinalityRelationship
    list<relationshipSchema>::iterator relationIter;
    list<relationshipSchema>::iterator relationBegin = erd->getRelationshipList().begin();
    list<relationshipSchema>::iterator relationEnd = erd->getRelationshipList().end();
    
    list<cardinalityRole>::iterator carIter;
    
    for(relationIter = relationBegin;relationIter != relationEnd;relationIter++){
        cardinalityRelationship* tmpRelation = new cardinalityRelationship((*relationIter).getRelationshipName());
        
        //foreach role in the relationship
        list<roleSchema> roleList = (*relationIter).getRoleList();
        list<roleSchema>::iterator roleIter;
        for(roleIter = roleList.begin();roleIter != roleList.end();roleIter++){
            //foreach role from cardinality info
            bool update = false;
            for(carIter = carRoleList.begin();carIter != carRoleList.end();carIter++){
                if((*carIter).getRoleName() == (*roleIter).getRoleName()){
                    tmpRelation->addRoleSchema((*carIter));
                    update = true;
                }
            }
            if(!update){
                cardinalityRole tmpCarRole;
                tmpCarRole.setRoleName((*roleIter).getRoleName());
                tmpRelation->addRoleSchema(tmpCarRole);
            }
        }
        newERD->addRelationship(*tmpRelation);
    }
    
    return newERD;
}