#include "cardinalityERD.h"

using namespace std;

/*
 *  import cardinality info
 */
list<cardinalitySchema> importCardinalityInfo(string cardiScript)
{
    ifstream cardinalityInput(cardiScript);
    
    list<cardinalitySchema> tmpList;
    cardinalitySchema tmp;
    string roleName, minNumString, maxNumString;
    
    while(cardinalityInput >> roleName){
        cardinalityInput >> minNumString;
        cardinalityInput >> maxNumString;
        
        tmp.roleName = roleName;
        tmp.minNum = minNumString;
        tmp.maxNum = maxNumString;
        
        tmpList.push_back(tmp);
    }

    return tmpList;
}

/*
 *  Role add data member : Cardinality(Minimum, Maximum)
 */
void addCardinalityToERD(ERD* erd, string cardiScript)
{
    list<cardinalitySchema> cardinalityList = importCardinalityInfo(cardiScript);
    list<cardinalitySchema>::iterator cardinalityPtr;

    UD_Map* relationshipTable = erd->getRelationshipTable();
    
    //foreach relationship
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++) {
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        UD_Attribute_List* roleList = relationshipPtr->getRoleList();
        //foreach role
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* rolePtr = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            
            UD_Attribute_List tmpList;
            UD_String minTmp, maxTmp;
            minTmp.set_value("-1");
            maxTmp.set_value("-1");
            //foreach cardinality
            for(cardinalityPtr = cardinalityList.begin();cardinalityPtr != cardinalityList.end();cardinalityPtr++){
                if(rolePtr->getRoleName() == cardinalityPtr->roleName) {
                    minTmp.set_value(cardinalityPtr->minNum);
                    maxTmp.set_value(cardinalityPtr->maxNum);
                }
            }
            tmpList.add_attribute_al("Minimum", minTmp);
            tmpList.add_attribute_al("Maximum", maxTmp);
            rolePtr->add_attribute_al("Cardinality", tmpList);
        }
    }
}

void dumpCardinalityERD(ERD* erd)
{
    cout << "ERD Name : " << erd->getERDName() << endl;
    
    cout << "==Entity Table==" << endl;
    
    UD_Map* entityTable = erd->getEntityTable();
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        Entity* entity = (Entity*)(entityTable->value());
        entity->dump();
    }
    
    cout << "==Relationship Table==" << endl;
    UD_Map* relationshipTable = erd->getRelationshipTable();
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationship = (Relationship*)(relationshipTable->value());
        cout << "==========" << endl;
        cout << "Relationship Name : " << relationship->getRelationshipName() << endl;
        
        UD_Attribute_List* roleList = relationship->getRoleList();
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            UD_Attribute_List* car = (UD_Attribute_List*)(role->get_attribute_ref_al("Cardinality"));
            
            cout << "******" << endl;
            cout << "Role name : " << role->getRoleName()
            << ", Entity name : " << role->getEntityName() << endl;
            
            if(car != NULL){
                UD_String* min = (UD_String*)(car->get_attribute_ref_al("Minimum"));
                UD_String* max = (UD_String*)(car->get_attribute_ref_al("Maximum"));
                
                cout << "Cardinality : ";
                cout << " min : " << *(min->getptr());
                cout << " max : " << *(max->getptr()) << endl;
            }
        }
    }
}