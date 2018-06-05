#include "transformedERD.h"

using namespace std;

RelationshipRecord::RelationshipRecord(string name)
{
    Relationship tmpRelationship;
    Attribute_List transformedInfo;
    Map tmpMap;
    
    setName(name);
    
    add_attribute_al("Relationship", tmpRelationship);
    
    transformedInfo.add_attribute_al("Entity_set", tmpMap);
    transformedInfo.add_attribute_al("Relationship_set", tmpMap);
    add_attribute_al("Transformed_info", transformedInfo);
}

RelationshipRecord::~RelationshipRecord()
{
    
}

void RelationshipRecord::setName(string name)
{
    String tmp;
    tmp.set_value(name);
    
    universal_data* ptr = get_attribute_ref_al("Name");
    if(ptr != NULL) {
        set_attribute_al("Name", tmp);
    } else {
        add_attribute_al("Name", tmp);
    }
}

string RelationshipRecord::getName()
{
    return *(((String*)(get_attribute_ref_al("Name")))->getptr());
}

void RelationshipRecord::setSourceRelationship(Relationship* relationship)
{
    set_attribute_al("Relationship", *relationship);
}

Relationship* RelationshipRecord::getSourceRelationship()
{
    return (Relationship*)(get_attribute_ref_al("Relationship"));
}

void RelationshipRecord::addTransformedEntity(Entity* entity)
{
    String stringTmp;
    Attribute_List* tmp =  (Attribute_List*)(get_attribute_ref_al("Transformed_info"));
    
    Map* transformedEntitySet = (Map*)(tmp->get_attribute_ref_al("Entity_set"));
    stringTmp.set_value(entity->getEntityName());
    
    transformedEntitySet->insert(stringTmp, entity);
}

void RelationshipRecord::addTransformedRelationship(Relationship* relationship)
{
    String stringTmp;
    Attribute_List* tmp =  (Attribute_List*)(get_attribute_ref_al("Transformed_info"));
    
    Map* transformedRelationshipSet = (Map*)(tmp->get_attribute_ref_al("Relationship_set"));
    stringTmp.set_value(relationship->getRelationshipName());
    
    transformedRelationshipSet->insert(stringTmp, relationship);
}

Map* RelationshipRecord::getTransformedEntitySet()
{
    Attribute_List* tmp =  (Attribute_List*)(get_attribute_ref_al("Transformed_info"));
    
    return (Map*)(tmp->get_attribute_ref_al("Entity_set"));
}

Map* RelationshipRecord::getTransformedRelationshipSet()
{
    Attribute_List* tmp =  (Attribute_List*)(get_attribute_ref_al("Transformed_info"));
    
    return (Map*)(tmp->get_attribute_ref_al("Relationship_set"));
}

void RelationshipRecord::dump()
{
    cout << "Record Table Name : " << getName() << endl;
    
    Relationship* tmpRelationship = getSourceRelationship();
    cout << "==========" << endl;
    cout << "Relationship Name : " << tmpRelationship->getRelationshipName() << endl;
    Attribute_List* roleList = tmpRelationship->getRoleList();
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
    
    Map* entitySet = getTransformedEntitySet();
    for(entitySet->begin();!entitySet->end();(*entitySet)++){
        Entity* tmpEntity = (Entity*)(entitySet->value());
        
        tmpEntity->dump();
    }
    
    Map* relationshipSet = getTransformedRelationshipSet();
    for(relationshipSet->begin();!relationshipSet->end();(*relationshipSet)++){
        Relationship* tmpRelationship = (Relationship*)(relationshipSet->value());
        
        tmpRelationship->dump();
    }
}

Map* transferToBinary(ERD* erd)
{
    Map* relationshipTable = erd->getRelationshipTable();
    Map* record = new Map();

    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        Attribute_List* roleList = relationshipPtr->getRoleList();
        
        if(roleList->size() > 2){
            String tmpStrnig;
            //transfer Relationship to Entity
            string relationshipName = relationshipPtr->getRelationshipName();
            Entity* tmpEnity = new Entity(relationshipName);
            
            //relationshipRecord for record relationship and entity info before flat
            RelationshipRecord* tmpRecord = new RelationshipRecord(relationshipName);
            tmpStrnig.set_value(relationshipName);
            record->insert(tmpStrnig, tmpRecord);
            tmpRecord->setSourceRelationship(relationshipPtr);
            tmpRecord->addTransformedEntity(tmpEnity);
            
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
                
                //relationshipRecord for record relationship and entity info before flat
                tmpRecord->addTransformedRelationship(tmpRelationship);
            }
            //add new Entity to ERD
            erd->addEntity(tmpEnity);
            
//            //remove original Relationship
//            erd->removeRelationship(relationshipName);
        }
    }
    //remove original Relationship
    for(record->begin();!record->end();(*record)++){
        universal_data tmp = record->key();
//        cout << *(((String*)&(tmp))->getptr()) << endl;
        erd->removeRelationship(*(((String*)&(tmp))->getptr()));
    }
    
    return record;
}