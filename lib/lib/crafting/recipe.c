/*
 * Part of the Crafting Addition
 * recipe.c - a recipe for how to make a crafted item.  This defines
 * the item which is created, some descriptions available,  the necessary
 * ingredients (used up), supplies (not used up, must be held), room supplies
 * (not used up, must be in room), special properties and the messaging for
 * making the prototype object.  Defines the skill used and the necessary
 * skill level.  Can be personalizable or not.  Personalizable items create
 * prototypes, with dynamic short and long descriptions.  Non-personalizable
 * items merely make up the final product with its descriptors as is, except
 * for replacing %s with the string provided by craft.
 * Inherit from this file to create a new recipe, and sets its values for
 * that crafted item.
 * Author: Tsarenzi Updated: 09/01/2009
 */
#include <lib.h>

inherit LIB_ITEM;
inherit LIB_READ;

string ProductName = "";
string BaseMaterial = "";
int BaseUnits = 0;
string array Ingredients = ({});
string array Supplies = ({});
string Product = "";
int Personalizable = 0;
string array FrontDescriptors = ({});
string array BackDescriptors = ({});
string array RoomSupplies = ({});
mapping GoodSpecials = ([ ]);
mapping ShoddySpecials = ([ ]);
string Skill = "crafting";
int SkillLevel = 1;
string array Messages = ({"You start in on the necessary preparation.","You
carefully put the pieces together.","You look over the finished work."});

void SetProductName(string name);
void SetBaseMaterial(string material);
varargs string array SetIngredients(mixed ingreds...);
varargs string array SetSupplies(mixed supps...);
void SetProduct(string prod);
void SetPersonalizable(int pers);
varargs string array SetFrontDescriptors(mixed descs...);
varargs string array SetBackDescriptors(mixed descs...);
string GetProductName();
string GetBaseMaterial();
string array GetIngredients();
string array GetSupplies();
string GetProduct();
int GetPersonalizable();
string array GetFrontDescriptors();
string array GetBackDescriptors();
void SetBaseUnits(int units);
int GetBaseUnits();
void SetGoodSpecials(mapping specials);
void SetShoddySpecials(mapping specials);
mapping GetGoodSpecials();
mapping GetShoddySpecials();
varargs string array SetRoomSupplies(mixed supps...);
string array GetRoomSupplies();
void SetSkill(string skillName);
void SetSkillLevel(int level);
string GetSkill();
int GetSkillLevel();
varargs string array SetMessages(mixed messages...);
string array GetMessages();


int ReadFunction() {
        string ingredientList, supplyList;
        ingredientList = "\n";
        foreach(string str in Ingredients)
        {
                ingredientList += "      "+add_article(str)+"\n";
        }
        supplyList = "\n";
        foreach(string str in Supplies)
        {
                supplyList += "      "+add_article(str)+"\n";
        }
        foreach(string str in RoomSupplies)
        {
                supplyList += "      "+add_article(str)+"\n";
        }
        this_player()->eventPrint(capitalize(GetKeyName())+" for
"+add_article(ProductName)+"\n"+
                "    Base Material: "+BaseMaterial+", "+BaseUnits+" units\n"+
                "    Other Ingredients: "+ingredientList+"\n"+
                "    Supplies Needed: "+supplyList);
        return 1;

}

static void create() {
        item::create();
        SetKeyName("recipe");
        SetId( ({ "recipe", "paper","sheet" }) );
        SetShort("a recipe");
        SetLong("A sheet of paper on which is written a recipe.");
        SetRead((: ReadFunction :));
}

void init(){
        ::init();
}

void SetProductName(string name){ ProductName = name; }
void SetBaseMaterial(string material) { BaseMaterial = material; }
varargs string array SetIngredients(mixed ingreds...)
{
        if(stringp(ingreds) ) {
                ingreds = ({ ingreds });
        }
        else if ( !arrayp(ingreds) ) {
                error("Bad argument to SetIngredients().\n");
        }
        Ingredients = ({});
        foreach(mixed val in ingreds){
                if( arrayp(val) ){
                    Ingredients += val;
                }
                else {
                        Ingredients += ({ val });
                }
        }
        return Ingredients;
}

varargs string array SetSupplies(mixed supps...)
{
        if(stringp(supps) ) {
                supps = ({ supps });
        }
        else if ( !arrayp(supps) ) {
                error("Bad argument to SetSupplies().\n");
        }
        Supplies = ({});
        foreach(mixed val in supps){
                if( arrayp(val) ){
                    Supplies += val;
                }
                else {
                        Supplies += ({ val });
                }
        }
        return Supplies;
}

void SetProduct(string prod) { Product = prod; }
void SetPersonalizable(int pers) { Personalizable = pers; }
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

string GetProductName() { return ProductName; }
string GetBaseMaterial() { return BaseMaterial; }
string array GetIngredients() { return Ingredients; }
string array GetSupplies() { return Supplies; }
string GetProduct() { return Product; }
int GetPersonalizable() { return Personalizable; }
string array GetFrontDescriptors() { return FrontDescriptors; }
string array GetBackDescriptors() { return BackDescriptors; }


void SetBaseUnits(int units) { BaseUnits = units; }
int GetBaseUnits() { return BaseUnits; }
mixed direct_craft_obj_from_obj(mixed recipe, mixed mat) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
mixed direct_craft_obj_from_obj_str(mixed recipe, mixed mat, mixed str) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
void SetGoodSpecials(mapping specials) { GoodSpecials = specials; }
void SetShoddySpecials(mapping specials) { ShoddySpecials = specials; }
mapping GetGoodSpecials() { return GoodSpecials; }
mapping GetShoddySpecials() { return ShoddySpecials; }
varargs string array SetRoomSupplies(mixed supps...)
{
        if(stringp(supps) ) {
                supps = ({ supps });
        }
        else if ( !arrayp(supps) ) {
                error("Bad argument to SetRoomSupplies().\n");
        }
        RoomSupplies = ({});
        foreach(mixed val in supps){
                if( arrayp(val) ){
                    RoomSupplies += val;
                }
                else {
                        RoomSupplies += ({ val });
                }
        }
        return RoomSupplies;
}


string array GetRoomSupplies() { return RoomSupplies; }
void SetSkill(string skillName) { Skill = skillName; }
void SetSkillLevel(int level) { SkillLevel = level; }
string GetSkill() { return Skill; }
int GetSkillLevel() { return SkillLevel; }
varargs string array SetMessages(mixed messages...)
{
        if(stringp(messages) ) {
                messages = ({ messages });
        }
        else if ( !arrayp(messages) ) {
                error("Bad argument to SetMessages().\n");
        }
        Messages = ({});
        foreach(mixed val in messages){
                if( arrayp(val) ){
                        Messages += val;
                }
                else {
                        Messages += ({ val });
                }
        }
        return Messages;
}

string array GetMessages() { return Messages; }
mixed direct_memorize_obj(mixed recipe) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
