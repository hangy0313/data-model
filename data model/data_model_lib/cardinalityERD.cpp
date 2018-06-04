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
        //foreach cardinality
        for(cardinalityPtr = cardinalityList.begin();cardinalityPtr != cardinalityList.end();cardinalityPtr++){
            Role* rolePtr = relationshipPtr->findRole(cardinalityPtr->roleName);
            if(rolePtr != NULL) {
                Attribute_List tmp;
                String stringTmp;

                stringTmp.set_value(cardinalityPtr->minNum);
                tmp.add_attribute_al("Minimum", stringTmp);

                stringTmp.set_value(cardinalityPtr->maxNum);
                tmp.add_attribute_al("Maximum", stringTmp);
                
                rolePtr->add_attribute_al("Cardinality", tmp);
            }
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
            universal_data tmp = roleList->get_attribute_value_al();
            Role* role = (Role*)(&tmp);
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