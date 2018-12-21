/*
 * Part of Crafting Addition
 * embellish.c - a verb to add an embellishment to a crafted item
 * prototype, adding its description choices to the item, and tacking
 * an addition onto the long description.  This also increases the
 * value of the item and uses up the embellishment, placing it in
 * the prototype object so it can possibly be salvaged later if the 
 * prototype is destroyed instead of finished.
 * If done with the string argument, the embellishment is personalized
 * before it is used (any instance of %s is replaced with the string).
 * This is permanent and will be retained if the embellishment is
 * salvaged and will not overwrite personalizations already done.
 * The %s will only appear in unused embellishments.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>
#include <rooms.h>
//#include ROOMS_H
inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("embellish");
    SetRules("OBJ with OBJ","OBJ with OBJ STR");
    SetErrorMessage("Embellish what with what?");
    SetHelp("Syntax: <embellish OBJECT with OBJECT>\n"
        "<embellish OBJECT with OBJECT STRING>\n"
            "Allows you to embellish a crafted object with an embellishment.");
}

mixed can_embellish_obj_with_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_embellish_obj_with_obj(mixed item, mixed emb) {
        //Check if the embellishment type is allowed on this prototype
        if(member_array(emb->GetEmbellishmentType(),
item->GetAllowedEmbellishments()) == -1)
        {
                write("That's not a sort of embellishment you can do to
"+item->GetMaterial()+".");
                return 1;
        }
        //personalize the embellishment with a default string to prevent
        //stray ugly %s in the description.
        emb->personalize("simple designs");
        write("You add "+emb->GetShort()+" to "+item->GetShort()+".");
        item->SetLong(item->GetLong()+"  "+emb->GetLongAddition());
        item->SetFrontDescriptors(item->GetFrontDescriptors()+emb->GetFrontDescriptors());
        item->SetBackDescriptors(item->GetBackDescriptors()+emb->GetBackDescriptors());
        //item value and mass is increased by the value and mass of the embellishment
        item->SetMass(item->GetMass()+emb->GetMass());
        item->SetBaseCost("silver", item->GetBaseCost()+emb->GetBaseCost());
        //Store the embellishment in the prototype or else destroy it if
        // not possible.
        emb->eventMove(item);
        if(environment(emb) == this_player())
                emb->eventMove(LIB_FURNACE);
        return 1;
}
mixed can_embellish_obj_with_obj_str() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_embellish_obj_with_obj_str(mixed item, mixed emb, mixed str) {
        //Check if the embellishment type is allowed
       
if(member_array(emb->GetEmbellishmentType(),item->GetAllowedEmbellishments())
== -1)
        {
                write("That's not a sort of embellishment you can do to
"+item->GetMaterial()+".");
                return 1;
        }
        //personalize the embellishment with the given string
        emb->personalize(str);
        write("You add "+emb->GetShort()+" to "+item->GetShort()+".");
        item->SetLong(item->GetLong()+"  "+emb->GetLongAddition());

item->SetFrontDescriptors(item->GetFrontDescriptors()+emb->GetFrontDescriptors
());
item->SetBackDescriptors(item->GetBackDescriptors()+emb->GetBackDescriptors())
;
        //item value and mass is increased by the value and mass of the embellishment
        item->SetMass(item->GetMass()+emb->GetMass());
        item->SetBaseCost("silver", item->GetBaseCost()+emb->GetBaseCost());
        //Store the embellishment in the prototype or else destroy it if
        // not possible.
        emb->eventMove(item);
        if(environment(emb) == this_player())
                emb->eventMove(LIB_FURNACE);
        return 1;
}
