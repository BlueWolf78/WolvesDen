/*
 * Part of Crafting addition
 * material.c - Represents a crafting material of some type.
 * A material has a certain number of untis which can be used
 * up in crafting, and has certain embellishments, descriptors
 * and special properties associated with it.  A material also
 * has a skill level which it adds onto the base skill level of
 * the recipe.
 * Inherit from this file to create a new material, and sets its values for
 * that material.
 * Author: Tsarenzi Updated: 09/01/2009
 */
#include <lib.h>

inherit LIB_ITEM;

int NumUnits = 0;
int CostPerUnit = 0;
int MassPerUnit = 0;
private static string array AllowedEmbellishments = ({});
string MaterialType = "";
string array FrontDescriptors = ({});
string array BackDescriptors = ({});
mapping GoodSpecials = ([ ]);
mapping ShoddySpecials = ([ ]);
int SkillLevel = 0;

void SetNumUnits(int num);
void SetCostPerUnit(int cost);
void SetMassPerUnit(int mass);
int GetNumUnits();
int GetCostPerUnit();
int GetMassPerUnit();
int GetCost(int units);
int GetMass(int units);
void SubtractUnits(int sub);
varargs string array SetAllowedEmbellishments(mixed embs...);
string array GetAllowedEmbellishments();
void SetMaterialType(string type);
string GetMaterialType();
varargs string array SetFrontDescriptors(mixed descs...);
varargs string array SetBackDescriptors(mixed descs...);
void SetSkillLevel(int level);
int GetSkillLevel();

string array GetFrontDescriptors();
string array GetBackDescriptors();
void SetGoodSpecials(mapping specials);
void SetShoddySpecials(mapping specials);
mapping GetGoodSpecials();
mapping GetShoddySpecials();

static void create() {
        item::create();
        AddSave( ({ "NumUnits" }) );
}

void init(){
        ::init();
}

void SetNumUnits(int num){  NumUnits = num;  }
void SetCostPerUnit(int cost){  CostPerUnit = cost; }
void SetMassPerUnit(int mass){  MassPerUnit = mass; }
int GetNumUnits(){  return NumUnits;  }
int GetCostPerUnit(){ return CostPerUnit; }
int GetMassPerUnit(){ return MassPerUnit; }
int GetCost(int units){ return units*CostPerUnit; }
int GetMass(int units){ return units*MassPerUnit; }
void SubtractUnits(int sub){ NumUnits = NumUnits - sub; 
}

//Allowed embellishments may be specified as an array or strings
varargs string array SetAllowedEmbellishments(mixed embs...)
{
        if(stringp(embs) ) {
                embs = ({ embs });
        }
        else if ( !arrayp(embs) ) {
                error("Bad argument to SetAllowedEmbellishments().\n");
        }
        AllowedEmbellishments = ({});
        foreach(mixed val in embs){
                if( arrayp(val) ){
                    AllowedEmbellishments += val;
                }
                else {
                        AllowedEmbellishments += ({ val });
                }
        }
        return AllowedEmbellishments;
}

string array GetAllowedEmbellishments()
{
        return AllowedEmbellishments;
}
void SetMaterialType(string type)
{
        MaterialType = type;
}

string GetMaterialType()
{
        return MaterialType;
}
//A material should be the second obj in the craft syntax
mixed indirect_craft_obj_from_obj(mixed recipe, mixed mat) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
//A material should be the second obj in the craft syntax
mixed indirect_craft_obj_from_obj_str(mixed recipe, mixed mat, mixed str) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
//A material should be the second obj in the craft syntax
mixed direct_craft_str_from_obj(mixed recipe, mixed mat)
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
//A material should be the second obj in the craft syntax
mixed direct_craft_str_from_obj_str(mixed recipe, mixed mat, mixed str)
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
string array GetFrontDescriptors() { return FrontDescriptors; }
string array GetBackDescriptors() { return BackDescriptors; }
//Front descriptors can be specified in array or strings format
varargs string array SetFrontDescriptors(mixed descs...)
{
        if(stringp(descs) ) {
                descs = ({ descs });
        }
        else if ( !arrayp(descs) ) {
                error("Bad argument to SetFrontDescriptors().\n");
        }
        FrontDescriptors = ({});
        foreach(mixed val in descs){
                if( arrayp(val) ){
                        FrontDescriptors += val;
                }
                else {
                        FrontDescriptors += ({ val });
                }
        }
        return FrontDescriptors;
}

//Back descriptors can be specified in array or strings format
varargs string array SetBackDescriptors(mixed descs...)
{
        if(stringp(descs) ) {
                descs = ({ descs });
        }
        else if ( !arrayp(descs) ) {
                error("Bad argument to SetBackDescriptors().\n");
        }
        BackDescriptors = ({});
        foreach(mixed val in descs){
                if( arrayp(val) ){
                        BackDescriptors += val;
                }
                else {
                        BackDescriptors += ({ val });
                }
        }
        return BackDescriptors;
}
//Both objects in combine should be materials
mixed direct_combine_obj_with_obj(mixed item, mixed item2) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}

//Both objects in combine should be materials
mixed indirect_combine_obj_with_obj(mixed item, mixed item2) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
//Tack the number of units remaining onto look/show.
varargs mixed eventShow(object who, string component){
        ::eventShow(who, component);
        who->eventPrint("There seems to be "+GetNumUnits()+" units
remaining.");
}
void SetGoodSpecials(mapping specials) { GoodSpecials = specials; }
void SetShoddySpecials(mapping specials) { ShoddySpecials = specials; }
mapping GetGoodSpecials() { return GoodSpecials; }
mapping GetShoddySpecials() { return ShoddySpecials; }
//Used for combining multiple of the same material
mixed direct_combine_obj(mixed items) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
void SetSkillLevel(int level) { SkillLevel = level; }
int GetSkillLevel() { return SkillLevel; }
