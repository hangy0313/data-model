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

TransformedERD::TransformedERD(string erdName)
{
    setERDName(erdName);
    
    Map mapTmp;
    add_attribute_al("Entity_table", mapTmp);
    add_attribute_al("Relationship_table", mapTmp);
}

TransformedERD::~TransformedERD()
{
}

void TransformedERD::setERDName(string erdName)
{
    String tmp;
    tmp.set_value(erdName);
    
    universal_data* ptr = get_attribute_ref_al("ERD_name");
    if(ptr != NULL) {
        set_attribute_al("ERD_name", tmp);
    } else {
        add_attribute_al("ERD_name", tmp);
    }
}

string TransformedERD::getERDName()
{
    universal_data *tmp = get_attribute_ref_al("ERD_name");
    
    String* stringTmp = (String*)tmp;
    
    return *(stringTmp->getptr());
}

Map* TransformedERD::getEntityTable()
{
    return (Map*)(get_attribute_ref_al("Entity_table"));
}

Map* TransformedERD::getRelationshipTable()
{
    return (Map*)(get_attribute_ref_al("Relationship_table"));
}

void TransformedERD::addEntity(TransformedEntity* en)
{
    String stringTmp;
    stringTmp.set_value(en->getEntityName());
    Map* mapTmp = getEntityTable();
    
    mapTmp->insert(stringTmp, en);
}

void TransformedERD::removeEntity(string entityName)
{
    String stringTmp;
    stringTmp.set_value(entityName);
    Map* mapTmp = getEntityTable();
    
    mapTmp->erase(stringTmp);
}

void TransformedERD::addRelationship(TransformedRelationship* relationship)
{
    String stringTmp;
    stringTmp.set_value(relationship->getRelationshipName());
    Map* mapTmp = getRelationshipTable();
    
    mapTmp->insert(stringTmp, relationship);
}

void TransformedERD::removeRelationship(string relationshipName)
{
    String stringTmp;
    stringTmp.set_value(relationshipName);
    Map* mapTmp = getRelationshipTable();
    
    mapTmp->erase(stringTmp);
}


TransformedEntity* TransformedERD::findEntity(string entityName)
{
    String stringTmp;
    stringTmp.set_value(entityName);
    Map* mapTmp = getEntityTable();
    
    mapTmp->find(stringTmp);
    
    return (TransformedEntity*)(mapTmp);
}

TransformedRelationship* TransformedERD::findRelationship(string relationshipName)
{
    String stringTmp;
    stringTmp.set_value(relationshipName);
    Map* mapTmp = getRelationshipTable();
    
    mapTmp->find(stringTmp);
    
    return (TransformedRelationship*)(mapTmp);
}

void TransformedERD::dump()
{
    cout << "Transformed ERD Name : " << getERDName() << endl;
    cout << "==Entity Table==" << endl;
    
    Map* entityTable = getEntityTable();
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        TransformedEntity* entity = (TransformedEntity*)(entityTable->value());
        entity->dump();
    }
    
    Map* relationshipTable = getRelationshipTable();
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        TransformedRelationship* relationship = (TransformedRelationship*)(relationshipTable->value());
        relationship->dump();
    }
}

TransformedEntity::TransformedEntity()
{
    Attribute_List attributeListTmp;
    
    setEntityName("");
    add_attribute_al("Attribute_list", attributeListTmp);
}

TransformedEntity::TransformedEntity(string entityName)
{
    Attribute_List attributeListTmp;
    
    setEntityName(entityName);
    add_attribute_al("Attribute_list", attributeListTmp);
    add_attribute_al("Link_set", attributeListTmp);
}

TransformedEntity::~TransformedEntity()
{
    
}

void TransformedEntity::setEntityName(string entityName)
{
    String tmp;
    tmp.set_value(entityName);
    
    universal_data* ptr = get_attribute_ref_al("Entity_name");
    if(ptr != NULL) {
        set_attribute_al("Entity_name", tmp);
    } else {
        add_attribute_al("Entity_name", tmp);
    }
}

string TransformedEntity::getEntityName()
{
    return *((String*)(get_attribute_ref_al("Entity_name")))->getptr();
}

void TransformedEntity::addAttribute(string attributeName,string attributeType)
{
    String stringTmp;
    stringTmp.set_value(attributeType);
    Attribute_List* tmp = (Attribute_List*)(get_attribute_ref_al("Attribute_list"));
    
    tmp->add_attribute_al(attributeName, stringTmp);
}

void TransformedEntity::addAttribute(Attribute_List tmpAttribute)
{
    universal_data tmp = tmpAttribute.get_attribute();
    for(tmpAttribute.begin();tmpAttribute.end();tmpAttribute++){
        addAttribute(tmpAttribute.get_attribute_name(), *(((String*)(&tmp))->getptr()));
    }
}

void TransformedEntity::removeAttribute(string attributeName)
{
    Attribute_List* tmp = (Attribute_List*)(get_attribute_ref_al("Attribute_list"));
    
    tmp->remove_attribute_al(attributeName);
}

Attribute_List* TransformedEntity::getAttributeList()
{
    return (Attribute_List*)(get_attribute_ref_al("Attribute_list"));
}

void TransformedEntity::addLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->add_attribute_al(linkName.getName(), linkName);
}

void TransformedEntity::removeLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->remove_attribute_al(linkName);
}

void TransformedEntity::dump()
{
    cout << "==========" << endl;
    cout << "Entity Name : " << getEntityName() << endl;
    
    Attribute_List* attList = getAttributeList();
    for(attList->begin();!attList->end();(*attList)++){
        universal_data name = attList->get_attribute_name_al();
        universal_data type = attList->get_attribute_value_al();
        cout << "name : " << *(((String*)(&name))->getptr())
        << ", type : " << *(((String*)(&type))->getptr()) << endl;
    }
    cout << "==========" << endl;
}

TransformedRelationship::TransformedRelationship(string relationshipName)
{
    Attribute_List attributeListTmp;
    
    setRelationshipName(relationshipName);
    add_attribute_al("Link_set", attributeListTmp);
}

TransformedRelationship::~TransformedRelationship()
{
    
}

void TransformedRelationship::setRelationshipName(string relationshipName)
{
    String stringTmp;
    stringTmp.set_value(relationshipName);
    
    universal_data* ptr = get_attribute_ref_al("Relationship_name");
    
    if(ptr != NULL) {
        set_attribute_al("Relationship_name", stringTmp);
    } else {
        add_attribute_al("Relationship_name", stringTmp);
    }
}

string TransformedRelationship::getRelationshipName()
{
    return *(((String*)(get_attribute_ref_al("Relationship_name")))->getptr());
}

void TransformedRelationship::addLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->add_attribute_al(linkName.getName(), linkName);
}

void TransformedRelationship::removeLink(Link linkName)
{
     Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->remove_attribute_al(linkName);
}

void TransformedRelationship::dump()
{
    
}

Link::Link(){
    String stringTmp;
    
    stringTmp.set_value("");
    add_attribute_al("Name", stringTmp);
    
    stringTmp.set_value("True_link");
    add_attribute_al("Type", stringTmp);
    
    stringTmp.set_value("");
    add_attribute_al("Direction", stringTmp);
    
    stringTmp.set_value("");
    add_attribute_al("Taget_name", stringTmp);
    
    Map mapTmp;
    add_attribute_al("Symbolic_link", mapTmp);
    
    universal_data tmp;
    add_attribute_al("True_link", tmp);
}

Link::~Link()
{
    
}

void Link::setName(string name)
{
    String tmp;
    
    tmp.set_value(name);
    set_attribute_al("Name", tmp);
}

string Link::getName()
{
    return *(((String*)get_attribute_ref_al("Name"))->getptr());
}

void Link::setType(string type)
{
    String tmp;
    
    tmp.set_value(type);
    set_attribute_al("Type", tmp);
}

string Link::getType()
{
    return *(((String*)get_attribute_ref_al("Type"))->getptr());
}

void Link::setDirection(string direction)
{
    String tmp;
    
    tmp.set_value(direction);
    set_attribute_al("Direction", tmp);
}

string Link::getDirection()
{
    return *(((String*)get_attribute_ref_al("Direction"))->getptr());
}

void Link::setTargetName(string name)
{
    String tmp;
    
    tmp.set_value(name);
    set_attribute_al("Taget_name", tmp);
}

string Link::getTargetName()
{
    return *(((String*)get_attribute_ref_al("Taget_name"))->getptr());
}

void Link::setTrueLink(universal_data utmp)
{
    set_attribute_al("True_link", utmp);
}

universal_data* Link::getTrueLink()
{
    return get_attribute_ref_al("True_link");
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
            Entity* tmpEnity = new Entity("ENTITY_"+relationshipName);
            
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
                Relationship* tmpRelationship = new Relationship("RELATIONSHIP_"+roleName);
                
                //create new Role for the new Relationship
                Role* newRole1 = new Role("ROLE_"+entityName);
                Role* newRole2 = new Role("ROLE_"+tmpEnity->getEntityName());
                
                //set new Role entity name
                newRole1->setEntityName(entityName);
                newRole2->setEntityName(tmpEnity->getEntityName());
                
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