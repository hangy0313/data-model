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
    add_attribute_al("Link_set", attributeListTmp);
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

//for 1:1 and default use
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

//for 1:N, M:N use
void TransformedEntity::addListLink(Link* linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    List* listLinkPtr = (List*)(linkSet->get_attribute_ref_al("List_"+linkName->getName()));
    
    if(listLinkPtr == NULL){
        List listmp;
        listmp.push_back(linkName);
        
        linkSet->add_attribute_al("List_"+linkName->getName(), listmp);
    }else{
        listLinkPtr->push_back(linkName);
    }
}

void TransformedEntity::addHeadLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->add_attribute_al("Head_"+linkName.getName(), linkName);
}

void TransformedEntity::addNextLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->add_attribute_al("Next_"+linkName.getName(), linkName);
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
    
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    for(linkSet->begin();!linkSet->end();(*linkSet)++){
//        Link* linkTmp = (Link*)(linkSet->get_attribute_ref_al(linkSet->get_attribute_name_al()));
//        cout << "link name : " << linkSet->get_attribute_name_al() << endl;
//        cout << "Target name : " << linkTmp->getTargetName() << endl;
        universal_data tmp = linkSet->get_attribute_name_al();
        string linkName = *(((String*)(&tmp))->getptr());
        cout << "link name : " << linkName << endl;
    }
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

//for 1:N, M:N use
void TransformedRelationship::addListLink(Link* linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    List* listLinkPtr = (List*)(linkSet->get_attribute_ref_al("List_"+linkName->getName()));
    
    if(listLinkPtr == NULL){
        List listmp;
        listmp.push_back(linkName);
        
        linkSet->add_attribute_al("List_"+linkName->getName(), listmp);
    }else{
        listLinkPtr->push_back(linkName);
    }
}

void TransformedRelationship::addHeadLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->add_attribute_al("Head_"+linkName.getName(), linkName);
}

void TransformedRelationship::addNextLink(Link linkName)
{
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    
    linkSet->add_attribute_al("Next_"+linkName.getName(), linkName);
}

void TransformedRelationship::dump()
{
    cout << "==========" << endl;
    cout << "Relationship Name : " << getRelationshipName() << endl;
    
    Attribute_List* linkSet = (Attribute_List*)(get_attribute_ref_al("Link_set"));
    for(linkSet->begin();!linkSet->end();(*linkSet)++){
//        Link* linkTmp = (Link*)(linkSet->get_attribute_ref_al(linkSet->get_attribute_name_al()));
//        cout << "link name : " << linkTmp->getName() << endl;
//        cout << "Target name : " << linkTmp->getTargetName() << endl;
        universal_data tmp = linkSet->get_attribute_name_al();
        string linkName = *(((String*)(&tmp))->getptr());
        cout << "link name : " << linkName << endl;
    }
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
//                tmpEnity->addAttribute(entityName, "string");
                
                //create a Relationship to replace Role
                Relationship* tmpRelationship = new Relationship("RELATIONSHIP_"+roleName);
                
                //create new Role for the new Relationship
                Role* newRole1 = new Role("ROLE_"+entityName);
                Role* newRole2 = new Role("ROLE_"+roleName);
                
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

Map* importDirectionDegeneration()
{
    ifstream dirDegenerationInput("./decision/direction_degeneration.txt");
    Map* mapTmp = new Map();
    
    string roleName, to_entity, to_relationship, directionName;
    
    while(dirDegenerationInput >> roleName){
        Attribute_List* attTmp = new Attribute_List();
        String stringTmp;

        dirDegenerationInput >> directionName;
        dirDegenerationInput >> to_entity;
        stringTmp.set_value(to_entity);
        attTmp->add_attribute_al(directionName, stringTmp);
        
        dirDegenerationInput >> directionName;
        dirDegenerationInput >> to_relationship;
        stringTmp.set_value(to_relationship);
        attTmp->add_attribute_al(directionName, stringTmp);

        stringTmp.set_value(roleName);
        mapTmp->insert(stringTmp, attTmp);
    }
    
    return mapTmp;
}

void directionDegeneration(ERD* erd)
{
    Map* dirDegeneration = importDirectionDegeneration();
    Map* relationshipTable = (Map*)(erd->get_attribute_ref_al("Relationship_table"));
    
    //foreach direction degeneration
    for(dirDegeneration->begin();!dirDegeneration->end();(*dirDegeneration)++){
        universal_data tmp = dirDegeneration->key();
        string roleName = *(((String*)(&tmp))->getptr());
        Attribute_List* attTmp = (Attribute_List*)(dirDegeneration->value());
        //foreach relationship
        for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
            Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
            Attribute_List* roleList = relationshipPtr->getRoleList();
            for(roleList->begin();!roleList->end();(*roleList)++){
                Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
                if(roleName == role->getRoleName()){
                    String* navigationPtr = (String*)(role->get_attribute_ref_al("Navigation"));
                    string navigation = *(navigationPtr->getptr());
                    String stringTmp;
                    
                    string toEntity = *(((String*)(attTmp->get_attribute_ref_al("to_entity")))->getptr());
                    string toRelationship = *(((String*)(attTmp->get_attribute_ref_al("to_relationship")))->getptr());
                    
                    if(navigation == "to_entity"){
                        if(toEntity == "1"){
                            stringTmp.set_value("NULL");
                            role->set_attribute_al("Navigation", stringTmp);
                        }
                    }else if(navigation == "to_relationship"){
                        if(toRelationship == "1"){
                            stringTmp.set_value("NULL");
                            role->set_attribute_al("Navigation", stringTmp);
                        }
                    }else{ //bidirectional
                        if(toEntity == "1"){
                            if(toRelationship == "1"){
                                stringTmp.set_value("NULL");
                                role->set_attribute_al("Navigation", stringTmp);
                            }
                            if(toRelationship == "0"){
                                stringTmp.set_value("to_relationship");
                                role->set_attribute_al("Navigation", stringTmp);
                            }
                        }
                        if(toEntity == "0"){
                            if(toRelationship == "1"){
                                stringTmp.set_value("to_entity");
                                role->set_attribute_al("Navigation", stringTmp);
                            }
                        }
                    }
                }
            }
        }
    }
}

Map* importEmbedding()
{
    ifstream embeddingInput("./decision/embedding.txt");
    Map* mapTmp = new Map();
    
    string relationshipName, embed;
    
    while(embeddingInput >> relationshipName){
        String stringTmp;
        String* stringPtr = new String();
        
        stringTmp.set_value(relationshipName);
        
        embeddingInput >> embed;
        stringPtr->set_value(embed);
        
        mapTmp->insert(stringTmp, stringPtr);
    }
    
    return mapTmp;
}

void duplicate(ERD* erd, TransformedERD* transERD)
{
    Map* entityTable = erd->getEntityTable();
    Map* relationshipTable = erd->getRelationshipTable();
    
    Map* transEntityTable = transERD->getEntityTable();
    Map* transRelationshipTable = transERD->getRelationshipTable();
    
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        Entity* entityPtr = (Entity*)(entityTable->value());
        string entityName = entityPtr->getEntityName();
        TransformedEntity* transEntity = new TransformedEntity(entityName);
        
        Attribute_List* attList = entityPtr->getAttributeList();
        for(attList->begin();!attList->end();(*attList)++){
            universal_data name = attList->get_attribute_name_al();
            universal_data type = attList->get_attribute_value_al();
            
            transEntity->addAttribute(*(((String*)(&name))->getptr()), *(((String*)(&type))->getptr()));
        }
        transEntityTable->insert(entityTable->key(), transEntity);
    }
    
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        string relationshipName = relationshipPtr->getRelationshipName();
        TransformedRelationship* transRelationship = new TransformedRelationship(relationshipName);
        
        transRelationshipTable->insert(relationshipTable->key(), transRelationship);
    }
}

void embedding(ERD* erd, TransformedERD* transERD)
{
    Map* embeddingInfo = importEmbedding();
    Map* relationshipTable = erd->getRelationshipTable();
    
    //copy original ERD to transformed ERD
    duplicate(erd, transERD);
    Map* transEntityTable = transERD->getEntityTable();
    Map* transRelationshipTable = transERD->getRelationshipTable();
    
    //foreach relationship
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationshipPtr = (Relationship*)(relationshipTable->value());
        string relationshipName = relationshipPtr->getRelationshipName();
        Attribute_List* roleList = relationshipPtr->getRoleList();
        
        string navigation[2], min[2], max[2], entityName[2];
        int i = 0;
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            Attribute_List* car = (Attribute_List*)(role->get_attribute_ref_al("Cardinality"));
            String* navigationTmp = (String*)(role->get_attribute_ref_al("Navigation"));
            
            entityName[i] = role->getEntityName();
            
            navigation[i] = *(navigationTmp->getptr());
            
            min[i] = *(((String*)(car->get_attribute_ref_al("Minimum")))->getptr());
            max[i] = *(((String*)(car->get_attribute_ref_al("Maximum")))->getptr());
            
            i++;
        }
        //foreach embedding information
        string decision;
        for(embeddingInfo->begin();!embeddingInfo->end();(*embeddingInfo)++){
            universal_data tmp = embeddingInfo->key();
            if(relationshipName == *(((String*)(&tmp))->getptr())){
                decision = *(((String*)(embeddingInfo->value()))->getptr());
            }
        
        
            String relationshipNameTmp;
            relationshipNameTmp.set_value(relationshipName);
            TransformedRelationship* transRelationshipPtr
                = (TransformedRelationship*)(transRelationshipTable->find(relationshipNameTmp));
            
            // N/A
            if(decision == "N/A"){
                for(int index = 0;index < 2;index++){
                    String entityNameTmp;
                    entityNameTmp.set_value(entityName[index]);
                    
                    TransformedEntity* transEntityPtr
                        = (TransformedEntity*)(transEntityTable->find(entityNameTmp));
                    
                    if(navigation[index] == "to_entity"){
                        //add link pointer to entity in relationship side
                        Link* linkTmp = new Link();
                        linkTmp->setName("Link_"+relationshipName+"_"+entityName[index]);
                        linkTmp->setDirection("to_entity");
                        linkTmp->setTargetName(entityName[index]);
                        linkTmp->setTrueLink(*transEntityPtr);
                        
                        transRelationshipPtr->addLink(*linkTmp);
                    }
                    if(navigation[index] == "to_relationship"){
                        //add link pointer to relationship in entity side
                        Link* linkTmp = new Link();
                        linkTmp->setName("Link_"+entityName[index]+"_"+relationshipName);
                        linkTmp->setDirection("to_relationship");
                        linkTmp->setTargetName(relationshipName);
                        linkTmp->setTrueLink(*transRelationshipPtr);
                        
                        transEntityPtr->addLink(*linkTmp);
                    }
                    if(navigation[index] == "bidirectional"){
                        //add link pointer to relationship in entity side
                        Link* linkTmp = new Link();
                        linkTmp->setName("Link_"+entityName[index]+"_"+relationshipName);
                        linkTmp->setDirection("to_relationship");
                        linkTmp->setTargetName(relationshipName);
                        linkTmp->setTrueLink(*transRelationshipPtr);
                        
                        transEntityPtr->addLink(*linkTmp);
                        
                        //add link pointer to entity in relationship side
                        linkTmp->setName("Link_"+relationshipName+"_"+entityName[index]);
                        linkTmp->setDirection("to_entity");
                        linkTmp->setTargetName(entityName[index]);
                        linkTmp->setTrueLink(*transEntityPtr);
                        
                        transRelationshipPtr->addLink(*linkTmp);
                    }
                }
            }
            
            String entityNameTmp;
            //E1
            entityNameTmp.set_value(entityName[0]);
            TransformedEntity* transEntityPtr1
                = (TransformedEntity*)(transEntityTable->find(entityNameTmp));
            //E2
            entityNameTmp.set_value(entityName[1]);
            TransformedEntity* transEntityPtr2
                = (TransformedEntity*)(transEntityTable->find(entityNameTmp));
            
            //source embedding : target entity is 1 side or N side
            if(decision == "source_embedding"){
                //E1<-R<-E2
                if(navigation[0] == "to_entity" && navigation[1] == "to_relationship"){
                    Link* linkTmp = new Link();
                    linkTmp->setName("Link_"+entityName[1]+"_"+entityName[0]);
                    linkTmp->setDirection("to_entity");
                    linkTmp->setTargetName(entityName[0]);
                    linkTmp->setTrueLink(*transEntityPtr1);
                    
                    //cardinality max = 1 => E1 is 1 side
                    //else                => E1 is N side
                    if(max[0] == "1"){
                        transEntityPtr2->addLink(*linkTmp);
                    }else{
                        transEntityPtr2->addListLink(linkTmp);
                    }
                }
                //E1->R->E2
                if(navigation[0] == "to_relationship" && navigation[1] == "to_entity"){
                    Link* linkTmp = new Link();
                    linkTmp->setName("Link_"+entityName[0]+"_"+entityName[1]);
                    linkTmp->setDirection("to_entity");
                    linkTmp->setTargetName(entityName[1]);
                    linkTmp->setTrueLink(*transEntityPtr2);
                    
                    //cardinality max = 1 => E2 is 1 side
                    //else                => E2 is N side
                    if(max[1] == "1"){
                        transEntityPtr1->addLink(*linkTmp);
                    }else{
                        transEntityPtr1->addListLink(linkTmp);
                    }
                }
                //E1<->R<->E2
                if(navigation[0] == "bidirectional" && navigation[1] == "bidirectional"){
                    Link* linkTmp1 = new Link();
                    linkTmp1->setName("Link_"+entityName[1]+"_"+entityName[0]);
                    linkTmp1->setDirection("to_entity");
                    linkTmp1->setTargetName(entityName[0]);
                    linkTmp1->setTrueLink(*transEntityPtr1);
                    
                    //cardinality max = 1 => E1 is 1 side
                    //else                => E1 is N side
                    if(max[0] == "1"){
                        transEntityPtr2->addLink(*linkTmp1);
                    }else{
                        transEntityPtr2->addListLink(linkTmp1);
                    }
                    
                    Link* linkTmp2 = new Link();
                    linkTmp2->setName("Link_"+entityName[0]+"_"+entityName[1]);
                    linkTmp2->setDirection("to_entity");
                    linkTmp2->setTargetName(entityName[1]);
                    linkTmp2->setTrueLink(*transEntityPtr2);
                    
                    //cardinality max = 1 => E2 is 1 side
                    //else                => E2 is N side
                    if(max[1] == "1"){
                        transEntityPtr1->addLink(*linkTmp2);
                    }else{
                        transEntityPtr1->addListLink(linkTmp2);
                    }
                }
            }
            
            //target embedding
            if(decision == "target_embedding"){
                //E1<-R<-E2
                if(navigation[0] == "to_entity" && navigation[1] == "to_relationship"){
                    Link* linkTmp = new Link();
                    linkTmp->setName("Link_"+entityName[1]+"_"+entityName[0]);
                    linkTmp->setDirection("to_entity");
                    linkTmp->setTargetName(entityName[0]);
                    linkTmp->setTrueLink(*transEntityPtr1);
                    
                    Link nextLink;
                    nextLink.setName("Link"+entityName[0]+"_"+entityName[0]);
                    nextLink.setTargetName(entityName[0]);
                    
                    transEntityPtr2->addHeadLink(*linkTmp);
                    transEntityPtr1->addNextLink(nextLink);
                }
                //E1->R->E2
                if(navigation[0] == "to_relationship" && navigation[1] == "to_entity"){
                    Link* linkTmp = new Link();
                    linkTmp->setName("Link_"+entityName[0]+"_"+entityName[1]);
                    linkTmp->setDirection("to_entity");
                    linkTmp->setTargetName(entityName[1]);
                    linkTmp->setTrueLink(*transEntityPtr2);
                    
                    Link nextLink;
                    nextLink.setName("Link"+entityName[1]+"_"+entityName[1]);
                    nextLink.setTargetName(entityName[1]);
                    
                    transEntityPtr1->addHeadLink(*linkTmp);
                    transEntityPtr2->addNextLink(nextLink);
                }
                //E1<->R<->E2
                if(navigation[0] == "bidirectional" && navigation[1] == "bidirectional"){
                    Link* linkTmp1 = new Link();
                    linkTmp1->setName("Link_"+entityName[1]+"_"+entityName[0]);
                    linkTmp1->setDirection("to_entity");
                    linkTmp1->setTargetName(entityName[0]);
                    linkTmp1->setTrueLink(*transEntityPtr1);
                    
                    Link nextLink1;
                    nextLink1.setName("Link"+entityName[0]+"_"+entityName[0]);
                    nextLink1.setTargetName(entityName[0]);
                    
                    transEntityPtr2->addHeadLink(*linkTmp1);
                    transEntityPtr1->addNextLink(nextLink1);
                    
                    Link* linkTmp2 = new Link();
                    linkTmp2->setName("Link_"+entityName[0]+"_"+entityName[1]);
                    linkTmp2->setDirection("to_entity");
                    linkTmp2->setTargetName(entityName[1]);
                    linkTmp2->setTrueLink(*transEntityPtr2);
                    
                    Link nextLink2;
                    nextLink2.setName("Link"+entityName[1]+"_"+entityName[1]);
                    nextLink2.setTargetName(entityName[1]);
                    
                    transEntityPtr1->addHeadLink(*linkTmp2);
                    transEntityPtr2->addNextLink(nextLink2);
                }
            }
            
            //intermediate embedding
            if(decision == "intermediate_embedding"){
                //E1<-R<-E2
                if(navigation[0] == "to_entity" && navigation[1] == "to_relationship"){
                    Link* linkTmp = new Link();
                    linkTmp->setName("Link_"+entityName[1]+"_"+relationshipName);
                    linkTmp->setDirection("to_relationship");
                    linkTmp->setTargetName(relationshipName);
                    linkTmp->setTrueLink(*transRelationshipPtr);
                    
                    Link nextLink;
                    nextLink.setName("Link_"+relationshipName+"_"+entityName[0]);
                    nextLink.setTargetName(entityName[0]);
                    
                    transEntityPtr2->addHeadLink(*linkTmp);
                    transRelationshipPtr->addNextLink(nextLink);
                }
                //E1->R->E2
                if(navigation[0] == "to_relationship" && navigation[1] == "to_entity"){
                    Link* linkTmp = new Link();
                    linkTmp->setName("Link_"+entityName[0]+"_"+relationshipName);
                    linkTmp->setDirection("to_relationship");
                    linkTmp->setTargetName(relationshipName);
                    linkTmp->setTrueLink(*transRelationshipPtr);
                    
                    Link nextLink;
                    nextLink.setName("Link_"+relationshipName+"_"+entityName[1]);
                    nextLink.setTargetName(entityName[1]);
                    
                    transEntityPtr1->addHeadLink(*linkTmp);
                    transRelationshipPtr->addNextLink(nextLink);
                }
                //E1<->R<->E2
                if(navigation[0] == "bidirectional" && navigation[1] == "bidirectional"){
                    Link* linkTmp1 = new Link();
                    linkTmp1->setName("Link_"+entityName[1]+"_"+relationshipName);
                    linkTmp1->setDirection("to_relationship");
                    linkTmp1->setTargetName(relationshipName);
                    linkTmp1->setTrueLink(*transRelationshipPtr);
                    
                    Link nextLink1;
                    nextLink1.setName("Link_"+relationshipName+"_"+entityName[0]);
                    nextLink1.setTargetName(entityName[0]);
                    
                    transEntityPtr2->addHeadLink(*linkTmp1);
                    transRelationshipPtr->addNextLink(nextLink1);
                    
                    Link* linkTmp2 = new Link();
                    linkTmp2->setName("Link_"+entityName[0]+"_"+relationshipName);
                    linkTmp2->setDirection("to_relationship");
                    linkTmp2->setTargetName(relationshipName);
                    linkTmp2->setTrueLink(*transRelationshipPtr);
                    
                    Link nextLink2;
                    nextLink2.setName("Link_"+relationshipName+"_"+entityName[1]);
                    nextLink2.setTargetName(entityName[1]);
                    
                    transEntityPtr1->addHeadLink(*linkTmp2);
                    transRelationshipPtr->addNextLink(nextLink2);
                }
            }
        
        
            //list embedding
            if(decision == "list_embedding"){
                //E1<-R<-E2
                if(navigation[0] == "to_entity" && navigation[1] == "to_relationship"){
                    Link* linkTmp1 = new Link();
                    linkTmp1->setName("Link_"+entityName[1]+"_"+relationshipName);
                    linkTmp1->setDirection("to_relationship");
                    linkTmp1->setTargetName(relationshipName);
                    linkTmp1->setTrueLink(*transRelationshipPtr);
                    
                    transEntityPtr2->addHeadLink(*linkTmp1);
                    
                    Link* linkTmp2 = new Link();
                    linkTmp2->setName("Link_"+relationshipName+"_"+entityName[0]);
                    linkTmp2->setDirection("to_entity");
                    linkTmp2->setTargetName(entityName[0]);
                    linkTmp2->setTrueLink(*transEntityPtr1);
                    
                    transEntityPtr2->addHeadLink(*linkTmp1);
                    transRelationshipPtr->addListLink(linkTmp2);
                }
                //E1->R->E2
                if(navigation[0] == "to_relationship" && navigation[1] == "to_entity"){
                    Link* linkTmp1 = new Link();
                    linkTmp1->setName("Link_"+entityName[0]+"_"+relationshipName);
                    linkTmp1->setDirection("to_relationship");
                    linkTmp1->setTargetName(relationshipName);
                    linkTmp1->setTrueLink(*transRelationshipPtr);
                    
                    transEntityPtr1->addHeadLink(*linkTmp1);
                    
                    Link* linkTmp2 = new Link();
                    linkTmp2->setName("Link_"+relationshipName+"_"+entityName[1]);
                    linkTmp2->setDirection("to_entity");
                    linkTmp2->setTargetName(entityName[1]);
                    linkTmp2->setTrueLink(*transEntityPtr2);
                    
                    transEntityPtr2->addHeadLink(*linkTmp1);
                    transRelationshipPtr->addListLink(linkTmp2);
                }
                //E1<->R<->E2
                if(navigation[0] == "bidirectional" && navigation[1] == "bidirectional"){
                    Link* linkTmp1 = new Link();
                    linkTmp1->setName("Link_"+entityName[0]+"_"+relationshipName);
                    linkTmp1->setDirection("to_relationship");
                    linkTmp1->setTargetName(relationshipName);
                    linkTmp1->setTrueLink(*transRelationshipPtr);
                    
                    transEntityPtr1->addHeadLink(*linkTmp1);
                    
                    Link* linkTmp2 = new Link();
                    linkTmp2->setName("Link_"+relationshipName+"_"+entityName[1]);
                    linkTmp2->setDirection("to_entity");
                    linkTmp2->setTargetName(entityName[1]);
                    linkTmp2->setTrueLink(*transEntityPtr2);
                    
                    transEntityPtr2->addHeadLink(*linkTmp1);
                    transRelationshipPtr->addListLink(linkTmp2);
                }
            }
        }
    }
}