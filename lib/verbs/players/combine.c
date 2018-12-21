/*
 * Part of Crafting Addition
 * combine.c - A verb to allow crafters to combine multiple pieces of
 * the same material into a single piece.  One piece can be combined
 * with another, or a full set can be using plurals.  The end material
 * object will have the number of units as the sum of all the parts.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>
#include <rooms.h>
//#include ROOMS_H
inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("combine");
    SetRules("OBJ with OBJ","OBS");
    SetErrorMessage("Combine what with what?");
    SetHelp("Syntax: <combine OBJECT with OBJECT>\n"
            "Allows you to combine two pieces of the same type of material to
use them together.");
}

mixed can_combine_obj_with_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_combine_obj_with_obj(mixed item, mixed item2) {
        if(item == item2)
        {
                write("You cannot combine a material with itself.");
                return 1;
        }
        if(!item || !item2)
        {
                write("Problem.");
                return 1;
        }
        if(item->GetKeyName() != item2->GetKeyName()
                || item->GetMaterialType() != item2->GetMaterialType())
        {
                write("You must combine materials of the same type.");
                return 1;
        }
        item->SetNumUnits(item->GetNumUnits()+item2->GetNumUnits());
        write("You combine "+item->GetShort()+" with "+item2->GetShort()+",
leaving you with "+item->GetNumUnits()+" units of the stuff.");
        say(this_player()->GetName()+" combines "+item->GetShort()+" with
"+item2->GetShort()+".");
        item2->eventMove(LIB_FURNACE);
        return 1;
}
mixed do_combine_obs(object array items) {
        object ob;
        if(sizeof(items) < 2)
                write("You need more than one item to combine them.");
        ob = items[0];
        items = items - ({items[0]});
        foreach(object comb in items)
        {
                do_combine_obj_with_obj(ob,comb);
        }
        return 1;
}

mixed can_combine_obj(mixed items) {
    return can_combine_obj_with_obj();
}
