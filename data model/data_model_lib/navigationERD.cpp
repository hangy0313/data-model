#include "navigationERD.h"

using namespace std;

/*
 *  import navifation info
 */
UD_Attribute_List* importNavigationInfo(string naviScript)
{
    UD_Attribute_List* attTmp = new UD_Attribute_List;
    
    UD_String navigationString;
    string roleName, navigation;
    
    ifstream navigationInput(naviScript);
    
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
void addNavigationToERD(ERD* erd, string naviScript)
{
    UD_Attribute_List* navigationList = importNavigationInfo(naviScript);
    UD_Map* relationshipTable = erd->getRelationshipTable();
    
    //foreach relationship
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        UD_Attribute_List* roleList = relationshipPtr->getRoleList();
        //foreach role
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            
            UD_String tmpString;
            tmpString.set_value("bidirecctional");
            
            role->add_attribute_al("Navigation", tmpString);
            //foreach navigation
            for(navigationList->begin();!navigationList->end();(*navigationList)++){
                UD_universal_data tmpName = navigationList->get_attribute_name_al();
                if(role->getRoleName() == *((UD_String*)(&tmpName))->getptr()){
                    UD_universal_data tmpNavigation = navigationList->get_attribute_value_al();
                    UD_String* navigation = (UD_String*)(&tmpNavigation);
                    
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
            UD_String* navigation = (UD_String*)(role->get_attribute_ref_al("Navigation"));
            
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
            //show navigation info
            cout << "Navigation : " << *(navigation->getptr()) << endl;
        }
    }
}