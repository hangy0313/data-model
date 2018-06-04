#include "transformedERD.h"

using namespace std;

void transferToBinary(ERD* erd)
{
    Map* relationshipTable = erd->getRelationshipTable();
    
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        Attribute_List* roleList = relationshipPtr->getRoleList();
        
        if(roleList->size() > 2){
            //transfer Relationship to Entity
            string relationshipName = relationshipPtr->getRelationshipName();
            Entity* tmpEnity = new Entity(relationshipName);
            
            for(roleList->begin();!roleList->end();(*roleList)++){
                Role* rolePtr = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
                string roleName = rolePtr->getRoleName();
                string entityName = rolePtr->getEntityName();
                
                //add attribute to new Entity
                tmpEnity->addAttribute(entityName, "string");
                
                //create a Relationship to replace Role
                Relationship* tmpRelationship = new Relationship(roleName);
                
                //create new Role for the new Relationship
                Role* newRole1 = new Role(roleName+"_"+entityName);
                Role* newRole2 = new Role(roleName+"_"+relationshipName);
                
                //set new Role entity name
                newRole1->setEntityName(entityName);
                newRole2->setEntityName(relationshipName);
                
                //set new Role cardinality
                Attribute_List* attTmp = (Attribute_List*)(rolePtr->get_attribute_ref_al("Cardinality"));
                
                newRole1->add_attribute_al("Cardinality", *attTmp);
                newRole2->add_attribute_al("Cardinality", *attTmp);
                
                //set new Role navigation
                string navigation = *(((String*)(rolePtr->get_attribute_ref_al("Navigation")))->getptr());
                
                String toEntity;
                toEntity.set_value("to_entity");
                String toRelationship;
                toRelationship.set_value("to_relationship");
                String bidirectional;
                bidirectional.set_value("bidirectional");
                
                if(navigation == "to_entity"){
                    newRole1->add_attribute_al("Navigation", toEntity);
                    newRole2->add_attribute_al("Navigation", toRelationship);
                } else if(navigation == "to_relationship"){
                    newRole1->add_attribute_al("Navigation", toRelationship);
                    newRole2->add_attribute_al("Navigation", toEntity);
                } else {
                    newRole1->add_attribute_al("Navigation", bidirectional);
                    newRole2->add_attribute_al("Navigation", bidirectional);
                }
                
                //add new Role to new Relationship
                tmpRelationship->addRole(*newRole1);
                tmpRelationship->addRole(*newRole2);
                
                //add new Relationship to ERD
                erd->addRelationship(tmpRelationship);
            }
            //add new Entity to ERD
            erd->addEntity(tmpEnity);
            
            //remove original Relationship
            erd->removeRelationship(relationshipName);
        }
    }
}