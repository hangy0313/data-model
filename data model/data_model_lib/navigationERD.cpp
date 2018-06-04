#include "navigationERD.h"

using namespace std;

/*
 *  import cardinality info
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
            universal_data tmp = roleList->get_attribute_value_al();
            Role* role = (Role*)(&tmp);
            
            String tmpString;
            
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