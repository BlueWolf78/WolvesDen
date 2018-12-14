/*
 * Part of Crafting Addition
 * embellishment.c - an item which is used to add extra description pieces
 * to a crafted item's prototype using the embellish verb.  This adds all
 * of the embellishment's front and back descriptors to the item's short
 * choices and the specified addition to the item's long description.
 * An embellishment has a type and prototypes will only accept certain types
 * of embellishments, defined by the material used.
 * %s can be used as a placeholder for a user defined string with craft or
 * embellish, set by the personalize function.
 * Inherit from this file to create a new embellishment and set its values
 * for that embellishment.
 * Author: Tsarenzi Updated: 09/03/2009
 */
#include <lib.h>

inherit LIB_ITEM;


string EmbellishmentType = "";
string LongAddition = "";
string array FrontDescriptors = ({});
string array BackDescriptors = ({});

void SetEmbellishmentType(string type);
void SetLongAddition(string add);
varargs string array SetFrontDescriptors(mixed descs...);
varargs string array SetBackDescriptors(mixed descs...);
string GetEmbellishmentType();
string GetLongAddition();
string array GetFrontDescriptors();
string array GetBackDescriptors();



static void create() {
        item::create();
        SetKeyName("embellishment");
        SetId( ({ "embellishment", "accent" }) );
        SetShort("an embellishment");
        SetLong("An embellishment to accent a crafted item.");
        AddSave( ({ "FrontDescriptors","BackDescriptors", "Short",
"ExternalDesc", "LongAddition" }) );
}

void init(){
        ::init();
}

void SetEmbellishmentType(string type) { EmbellishmentType = type; }
void SetLongAddition(string add) { LongAddition = add; }
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
string array GetFrontDescriptors() { return FrontDescriptors; }
string array GetBackDescriptors() { return BackDescriptors; }
string GetEmbellishmentType() { return EmbellishmentType; }
string GetLongAddition() { return LongAddition; }
//Replace all instances of %s with the personalized string
void personalize(string str)
{
        string tmp;
        tmp = implode(GetFrontDescriptors(),"@");
        tmp = replace_string(tmp,"%s",str);
        SetFrontDescriptors(explode(tmp,"@"));
        tmp = implode(GetBackDescriptors(),"@");
        tmp = replace_string(tmp,"%s",str);
        SetBackDescriptors(explode(tmp,"@"));
        tmp = implode(GetAdjectives(),"@");
        tmp = replace_string(tmp,"%s",str);
        SetAdjectives(explode(tmp,"@"));
        SetLongAddition(replace_string(GetLongAddition(),"%s",str));
        SetShort(replace_string(GetShort(),"%s",str));
        SetLong(replace_string(GetLong(),"%s",str));
}
mixed indirect_embellish_obj_with_obj(mixed item, mixed emb) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
mixed indirect_embellish_obj_with_obj_str(mixed item, mixed emb,mixed str) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
