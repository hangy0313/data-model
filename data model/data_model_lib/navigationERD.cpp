#include "navigationERD.h"

using namespace std;

/*
 *  import navifation info
 */
Attribute_List* importNavigationInfo()
{
    Attribute_List* attTmp = new Attribute_List;
    
    String navigationString;
    string roleName, navigation;
    
    ifstream navigationInput("./erdScript/navigation.txt");
    
    while(navigationInput >> roleName){
        navigationInput >> navigation;
        
        navigationString.set_value(navigation);
        
        attTmp->add_attribute_al(roleName, navigationString);
    }

    return attTmp;
}

/*
 *  Role add data member : Navigation => "to_entity", "to_relationship", "bidirectional"
 */
void addNavigationToERD(ERD* erd)
{
    Attribute_List* navigationList = importNavigationInfo();
    Map* relationshipTable = erd->getRelationshipTable();
    
    //foreach relationship
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        Attribute_List* roleList = relationshipPtr->getRoleList();
        //foreach role
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            
            String tmpString;
            tmpString.set_value("bidirecctional");
            
            role->add_attribute_al("Navigation", tmpString);
            //foreach navigation
            for(navigationList->begin();!navigationList->end();(*navigationList)++){
                universal_data tmpName = navigationList->get_attribute_name_al();
                if(role->getRoleName() == *((String*)(&tmpName))->getptr()){
                    universal_data tmpNavigation = navigationList->get_attribute_value_al();
                    String* navigation = (String*)(&tmpNavigation);
                    
                    role->set_attribute_al("Navigation", *navigation);
                }
            }
        }
    }
}



/*
 *  Dump ERD info after add navigation
 */
void dumpNavigationERD(ERD* erd)
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
            String* navigation = (String*)(role->get_attribute_ref_al("Navigation"));
            
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
            //show navigation info
            cout << "Navigation : " << *(navigation->getptr()) << endl;
        }
    }
}