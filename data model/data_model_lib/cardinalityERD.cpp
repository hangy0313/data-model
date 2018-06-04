#include "cardinalityERD.h"

using namespace std;

/*
 *  import cardinality info
 */
list<cardinalitySchema> importCardinalityInfo()
{
    ifstream cardinalityInput("./erdScript/cardinality.txt");
    
    list<cardinalitySchema> tmpList;
    cardinalitySchema tmp;
    string roleName, minNumString, maxNumString;
    
    while(cardinalityInput >> roleName){
        cardinalityInput >> minNumString;
        cardinalityInput >> maxNumString;
        
        tmp.roleName = roleName;
        tmp.minNum = stoi(minNumString);
        tmp.maxNum = stoi(maxNumString);
        
        tmpList.push_back(tmp);
    }
    
    return tmpList;
}

/*
 *  Role add data member : Cardinality(Minimum, Maximum)
 */
void addCardinalityToERD(ERD erd)
{
    list<cardinalitySchema> cardinalityList = importCardinalityInfo();
    list<cardinalitySchema>::iterator cardinalityPtr;
    
    Map* relationshipTable = erd.getRelationshipTable();
    
    //foreach relationship
    for(relationshipTable->begin();relationshipTable->end();relationshipTable++) {
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        //foreach cardinality
        for(cardinalityPtr = cardinalityList.begin();cardinalityPtr != cardinalityList.end();cardinalityPtr++){
            Role* rolePtr = relationshipPtr->findRole(cardinalityPtr->roleName);
            if(rolePtr != NULL) {
                Attribute_List tmp;
                Int intTmp;

                intTmp.set_value(cardinalityPtr->minNum);
                tmp.add_attribute_al("Minimum", intTmp);

                intTmp.set_value(cardinalityPtr->maxNum);
                tmp.add_attribute_al("Maximum", intTmp);
                
                rolePtr->add_attribute_al("Cardinality", tmp);
            }
        }
    }
}