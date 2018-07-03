#include "accessModel.h"

using namespace std;

vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> result;
    str += pattern;
    int size = str.size();
    
    for(int i = 0;i < size;i++)
    {
        pos = str.find(pattern, i);
        if(pos < size)
        {
            string s = str.substr(i, pos-i);
            result.push_back(s);
            i = pos+pattern.size()-1;
        }
    }
    
    return result;
}

/*
 *  for source access model to specialized access model
 *  transfer from navigation erd & decision factor
 */
Attribute_List* createSpecializeAccessTable(ERD* erd)
{
    Attribute_List* accessTable = new Attribute_List();
    Map* embeddingInfo = importEmbedding();
    
    Map* entityTable = erd->getEntityTable();
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        Entity* entity = (Entity*)(entityTable->value());
        
        Attribute_List entityTmp;
        String tmpString;
        
        tmpString.set_value(entity->getEntityName());
        entityTmp.add_attribute_al("name", tmpString);
        
        Attribute_List* attListOrigin = entity->getAttributeList();
        Attribute_List attrListTmp;
        for(attListOrigin->begin();!attListOrigin->end();(*attListOrigin)++){
            Attribute_List attrTmp;
            
            universal_data name = attListOrigin->get_attribute_name_al();
            universal_data type = attListOrigin->get_attribute_value_al();
            
            attrTmp.add_attribute_al("name", name);
            attrTmp.add_attribute_al("type", type);
            
            attrListTmp.add_attribute_al(name, attrTmp);
        }
        entityTmp.add_attribute_al("attribute", attrListTmp);
        
        tmpString.set_value(entity->getEntityName());
        accessTable->add_attribute_al(tmpString, entityTmp);
    }
    
    Map* relationshipTable = erd->getRelationshipTable();
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        Relationship* relationship = (Relationship*)(relationshipTable->value());
        
        Attribute_List relationshipTmp;
        String tmpString;
        
        tmpString.set_value(relationship->getRelationshipName());
        relationshipTmp.add_attribute_al("name", tmpString);
        
        string decision;
        for(embeddingInfo->begin();!embeddingInfo->end();(*embeddingInfo)++){
            universal_data tmp = embeddingInfo->key();
            if(relationship->getRelationshipName() == *(((String*)(&tmp))->getptr())){
                decision = *(((String*)(embeddingInfo->value()))->getptr());
            }
        }
        tmpString.set_value(decision);
        relationshipTmp.add_attribute_al("embedding", tmpString);
        
        Attribute_List* roleList = relationship->getRoleList();
        Attribute_List roleListNew;
        for(roleList->begin();!roleList->end();(*roleList)++){
            Role* role = (Role*)(roleList->get_attribute_ref_al(roleList->get_attribute_name_al()));
            Attribute_List* car = (Attribute_List*)(role->get_attribute_ref_al("Cardinality"));
            String* navigation = (String*)(role->get_attribute_ref_al("Navigation"));
            
            Attribute_List roleTmp;
            
            roleTmp.add_attribute_al("navigation", *navigation);
            
            tmpString.set_value(role->getEntityName());
            roleTmp.add_attribute_al("entityName", tmpString);
            
            if(car != NULL){
                String* min = (String*)(car->get_attribute_ref_al("Minimum"));
                String* max = (String*)(car->get_attribute_ref_al("Maximum"));
                
                roleTmp.add_attribute_al("minCardi", *min);
                roleTmp.add_attribute_al("maxCardi", *max);
            }
            
            tmpString.set_value(role->getRoleName());
            roleListNew.add_attribute_al(tmpString, roleTmp);
        }
        relationshipTmp.add_attribute_al("role", roleListNew);
        
        tmpString.set_value(relationship->getRelationshipName());
        accessTable->add_attribute_al(tmpString, relationshipTmp);
    }
    
    
    return accessTable;
}

/*
 *  for specialized access model to library access model
 *  transfer from transformed erd
 */
Attribute_List* createLibraryAccessTable(TransformedERD* transERD)
{
    Attribute_List* accessTable = new Attribute_List();

    Map* entityTable = transERD->getEntityTable();
    for(entityTable->begin();!entityTable->end();(*entityTable)++){
        Entity* entity = (Entity*)(entityTable->value());
        
        Attribute_List entityTmp;
        String tmpString;
        
        tmpString.set_value(entity->getEntityName());
        entityTmp.add_attribute_al("name", tmpString);
        
        Attribute_List* attListOrigin = entity->getAttributeList();
        Attribute_List attrListTmp;
        for(attListOrigin->begin();!attListOrigin->end();(*attListOrigin)++){
            Attribute_List attrTmp;
            
            universal_data name = attListOrigin->get_attribute_name_al();
            universal_data type = attListOrigin->get_attribute_value_al();
            
            attrTmp.add_attribute_al("name", name);
            attrTmp.add_attribute_al("type", type);
            
            attrListTmp.add_attribute_al(name, attrTmp);
        }
        entityTmp.add_attribute_al("attribute", attrListTmp);
        
        Attribute_List* linkSet = (Attribute_List*)(entity->get_attribute_ref_al("Link_set"));
        Attribute_List linkSetNew;
        for(linkSet->begin();!linkSet->end();(*linkSet)++){
            Link* linkTmp = (Link*)(linkSet->get_attribute_ref_al(linkSet->get_attribute_name_al()));
            Attribute_List link;
            
            tmpString.set_value(linkTmp->getName());
            link.add_attribute_al("name", tmpString);

            tmpString.set_value(linkTmp->getTargetName());
            link.add_attribute_al("targetName", tmpString);
            
            tmpString.set_value(linkTmp->getName());
            linkSetNew.add_attribute_al(tmpString, link);
        }
        entityTmp.add_attribute_al("link", linkSetNew);
        
        tmpString.set_value(entity->getEntityName());
        accessTable->add_attribute_al(tmpString, entityTmp);
    }
    
    Map* relationshipTable = transERD->getRelationshipTable();
    for(relationshipTable->begin();!relationshipTable->end();(*relationshipTable)++){
        TransformedRelationship* relationship = (TransformedRelationship*)(relationshipTable->value());
        
        Attribute_List relationshipTmp;
        String tmpString;
        
        tmpString.set_value(relationship->getRelationshipName());
        relationshipTmp.add_attribute_al("name", tmpString);
        
        Attribute_List* linkSet = (Attribute_List*)(relationship->get_attribute_ref_al("Link_set"));
        Attribute_List linkSetNew;
        for(linkSet->begin();!linkSet->end();(*linkSet)++){
            Link* linkTmp = (Link*)(linkSet->get_attribute_ref_al(linkSet->get_attribute_name_al()));
            Attribute_List link;
            
            tmpString.set_value(linkTmp->getName());
            link.add_attribute_al("name", tmpString);
            
            tmpString.set_value(linkTmp->getTargetName());
            link.add_attribute_al("targetName", tmpString);
            
            tmpString.set_value(linkTmp->getName());
            linkSetNew.add_attribute_al(tmpString, link);
        }
        relationshipTmp.add_attribute_al("link", linkSetNew);
        
        tmpString.set_value(relationship->getRelationshipName());
        accessTable->add_attribute_al(tmpString, relationshipTmp);
    }
    
    return accessTable;
}

/*
 *  for library access model to language access model
 *  transfer from physical data model
 */
Attribute_List* createLanguageAccessTable(Map* physicalERDMap)
{
    Attribute_List* accessTable = new Attribute_List();
    
    for(physicalERDMap->begin();!physicalERDMap->end();(*physicalERDMap)++){
        physicalERD* classPtr = (physicalERD*)(physicalERDMap->value());
        
        Attribute_List classTmp;
        String tmpString;
        
        tmpString.set_value(classPtr->getClassName());
        classTmp.set_attribute_al("name", tmpString);
        
        list<dataMemberSchema> dataList = classPtr->getAllDataMember();
        list<dataMemberSchema>::iterator dataListPtr;
        Attribute_List dataMemberList;
        for(dataListPtr = dataList.begin();dataListPtr != dataList.end();dataListPtr++){
            Attribute_List dataMember;
            
            tmpString.set_value((*dataListPtr).type);
            dataMember.add_attribute_al("type", tmpString);
            
            tmpString.set_value((*dataListPtr).name);
            dataMember.add_attribute_al("name", tmpString);
            
            dataMemberList.add_attribute_al(tmpString, dataMember);
        }
        classTmp.set_attribute_al("dataMember", dataMemberList);
        
        list<memberFunctionSchema> functionList = classPtr->getAllMemberFunction();
        list<memberFunctionSchema>::iterator functionListPtr;
        Attribute_List memberFunctionList;
        for(functionListPtr = functionList.begin();functionListPtr != functionList.end();functionListPtr++){
            Attribute_List memberFunction;
            
            tmpString.set_value((*functionListPtr).functionName);
            memberFunction.add_attribute_al("functionName", tmpString);
            
            memberFunctionList.add_attribute_al(tmpString, memberFunction);
        }
        classTmp.set_attribute_al("memberFunction", memberFunctionList);
        
        tmpString.set_value(classPtr->getClassName());
        accessTable->add_attribute_al(tmpString, classTmp);
    }
    
    return accessTable;
}

string findFunctionName(string Ename, string Rname, string prefix)
{
    string functionName;
    
    for(languageAccessTable->begin();!languageAccessTable->end();(*languageAccessTable)++){
        if(Ename == languageAccessTable->get_attribute_name()){
            Attribute_List* functionList = (Attribute_List*)(languageAccessTable->get_attribute_ref_al("memberFunction"));
            for(functionList->begin();!functionList->end();(*functionList)++){
                string fName = functionList->get_attribute_name();
                if(functionList->get_attribute_name().find(Rname) != -1 &&
                   functionList->get_attribute_name().find(prefix) != -1){
                    return functionList->get_attribute_name();
                }
            }
        }
    }
    
    return functionName;
}

string findFunctionName(string Rname, string prefix)
{
    string functionName;
    
    for(languageAccessTable->begin();!languageAccessTable->end();(*languageAccessTable)++){
        if(Rname == languageAccessTable->get_attribute_name()){
            Attribute_List* functionList = (Attribute_List*)(languageAccessTable->get_attribute_ref_al("memberFunction"));
            for(functionList->begin();!functionList->end();(*functionList)++){
                string fName = functionList->get_attribute_name();
                if(functionList->get_attribute_name().find(prefix) != -1){
                    return functionList->get_attribute_name();
                }
            }
        }
    }
    
    return functionName;
}

string findTagetType(string Ename, string Rname)
{
    string functionName;
    
    for(languageAccessTable->begin();!languageAccessTable->end();(*languageAccessTable)++){
        if(Ename == languageAccessTable->get_attribute_name()){
            Attribute_List* functionList = (Attribute_List*)(languageAccessTable->get_attribute_ref_al("memberFunction"));
            for(functionList->begin();!functionList->end();(*functionList)++){
                string fName = functionList->get_attribute_name();
                if(functionList->get_attribute_name().find(Ename) != -1 &&
                   functionList->get_attribute_name().find(Rname) != -1){
                    string fName = functionList->get_attribute_name();
                    vector<string> result = split(fName, "_");
                    
                    return result[result.size()-1];
                }
            }
        }
    }
    
    return functionName;
}