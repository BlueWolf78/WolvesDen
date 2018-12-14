/*
 * Part of Crafting Addition
 * prototype.c - an object to represent an unfinished crafted item
 * to allow the crafter to embellish it and adjust its design.  The
 * product cannot be used until the prototype is finished, setting
 * the short and long description into stone.  The prototype keeps
 * track of the final product and applies the descriptor choices.
 * The prototype can be adjusted to show different front or back
 * descriptors.  The prototype is technically a container which holds
 * the materials used in crafting the object so they can be salvaged
 * later, but crafted cannot treat it like a container.  The craft verb 
 * creates this object and it need not be inherited by coders.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>

inherit LIB_STORAGE;


string Product = "";
string ProductName = "";
string Material = "";
string array FrontDescriptors = ({});
string array BackDescriptors = ({});
int currentFront = 0;
int currentBack = 0;
string array AllowedEmbellishments = ({});
mapping Specials = ([ ]);


void SetProduct(string prod);
void SetProductName(string name);
void SetMaterial(string mat);
varargs string array SetFrontDescriptors(mixed descs...);
varargs string array SetBackDescriptors(mixed descs...);
string GetProduct();
string GetProductName();
string GetMaterial();
string array GetFrontDescriptors();
string array GetBackDescriptors();
varargs string array SetAllowedEmbellishments(mixed embs...);
string array GetAllowedEmbellishments();
void SetCurrentFront(int n);
void SetCurrentBack(int n);
int GetCurrentFront();
int GetCurrentBack();
void SetSpecials(mapping specials);
mapping GetSpecials();



static void create() {
        ::create();
        SetKeyName("prototype");
        SetId( ({ "prototype", "object" }) );
        SetShort("a prototype object");
        SetLong("A prototype object for designing a crafted item");
        AddSave( ({ "Product", "ProductName", "Material", "FrontDescriptors",
"BackDescriptors", "currentFront", "currentBack", "AllowedEmbellishments",
"Short", "ExternalDesc","Specials","Id" }) );
        //Permanently closed so contents cannot be manipulated
        SetCanClose(0);
        SetClosed(1);
}

void init(){
        ::init();
}

void SetProduct(string prod) { Product = prod; }
void SetProductName(string name) { ProductName = name; }
void SetMaterial(string mat) { Material = mat; }
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

string array GetFrontDescriptors() { return FrontDescriptors; }
string array GetBackDescriptors() { return BackDescriptors; }
string GetProduct() { return Product; }
string GetProductName() { return ProductName; }
string GetMaterial() { return Material; }
string array GetAllowedEmbellishments(){ return AllowedEmbellishments; }
void SetCurrentFront(int n) { currentFront = n; }
void SetCurrentBack(int n) { currentBack = n; }
int GetCurrentFront() { return currentFront; }
int GetCurrentBack() { return currentBack; }

mixed direct_adjust_obj_str(object target) {
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
mixed direct_finish_obj(object target) {
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}

//Replace all instances of %s in the prototype with a personalized string
void personalize(string str)
{
        string tmp;
        int empty = 0;
        if(member_array("",GetBackDescriptors()) != -1) empty = 1;
        tmp = implode(GetFrontDescriptors(),"@");
        tmp = replace_string(tmp,"%s",str);
        SetFrontDescriptors(explode(tmp,"@"));
        tmp = implode(GetBackDescriptors(),"@");
        tmp = replace_string(tmp,"%s",str);
        SetBackDescriptors(explode(tmp,"@"));
        if(empty == 1) SetBackDescriptors(({""})+GetBackDescriptors());
        tmp = implode(GetAdjectives(),"@");
        tmp = replace_string(tmp,"%s",str);
        SetAdjectives(explode(tmp,"@"));
        SetShort(replace_string(GetShort(),"%s",str));
        SetLong(replace_string(GetLong(),"%s",str));
}

mixed direct_embellish_obj_with_obj(mixed item, mixed emb) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
mixed direct_embellish_obj_with_obj_str(mixed item, mixed emb, mixed str) 
{
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
varargs mixed CanShowInterior(object who, object target)
{
        return "You can't look in that.";
}
mixed direct_salvage_obj(object target) {
        if(environment() != this_player())
                return "You are not holding "+GetShort();
        return 1;
}
void SetSpecials(mapping specials) { Specials = specials; }
mapping GetSpecials() { return Specials; }
//Tack the reminder that this is a prototype not a finished product on
//when the object is looked at.
varargs mixed eventShow(object who, string component){
        ::eventShow(who, component);
        who->eventPrint("It seems to be in the process of being crafted, and
will need to be finished before it is ready.  However, embellishments could
still be added to it in the meantime.");
}
