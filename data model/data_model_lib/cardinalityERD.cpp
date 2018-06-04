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
        tmp.minNum = minNumString;
        tmp.maxNum = maxNumString;
        
        tmpList.push_back(tmp);
    }

    return tmpList;
}

/*
 *  Role add data member : Cardinality(Minimum, Maximum)
 */
void addCardinalityToERD(ERD* erd)
{
    list<cardinalitySchema> cardinalityList = importCardinalityInfo();
    list<cardinalitySchema>::iterator cardinalityPtr;

    Map* relationshipTable = erd->getRelationshipTable();
    
    //foreach relationship
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++) {
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        Attribute_List* roleList = relationshipPtr->getRoleList();
        //foreach role
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* rolePtr = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            
            Attribute_List tmpList;
            String minTmp, maxTmp;
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
    
    Map* entityTable = erd->getEntityTable();
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        Entity* entity = (Entity*)(entityTable->value());
        entity->dump();
    }
    
    cout << "==Relationship Table==" << endl;
    Map* relationshipTable = erd->getRelationshipTable();
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationship = (Relationship*)(relationshipTable->value());
        cout << "==========" << endl;
        cout << "Relationship Name : " << relationship->getRelationshipName() << endl;
        
        Attribute_List* roleList = relationship->getRoleList();
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            Attribute_List* car = (Attribute_List*)(role->get_attribute_ref_al("Cardinality"));
            
            cout << "******" << endl;
            cout << "Role name : " << role->getRoleName()
            << ", Entity name : " << role->getEntityName() << endl;
            
            if(car != NULL){
                String* min = (String*)(car->get_attribute_ref_al("Minimum"));
                String* max = (String*)(car->get_attribute_ref_al("Maximum"));
                
                cout << "Cardinality : ";
                cout << " min : " << *(min->getptr());
                cout << " max : " << *(max->getptr()) << endl;
            }
        }
    }
}