/*
 * Part of Crafting Addition
 * salvage.c - A verb to destroy an unfinished crafted object prototype
 * and salvage some of the ingredients and material put into it.  There
 * is a chance of returning each of the ingredients and a random portion
 * of the base material is saved.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>
#include <rooms.h>
//#include ROOMS_H

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("salvage");
    SetRules("OBJ");
    SetErrorMessage("Salvage what?");
    SetHelp("Syntax: <salvage OBJECT>\n"
            "Allows you to reclaim materials from unfinished crafted goods.");
}

mixed can_salvage_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_salvage_obj(object target) {
        object *contents;
        string resultsString;
        string removed;
        string *salvaged = ({});
        contents = all_inventory(target);
        foreach(string item in contents)
        {
                if(item->GetNumUnits() > 0)
                {
                        item->SetNumUnits(random(item->GetNumUnits()+1));
                        item->eventMove(this_player());
                        salvaged = salvaged + ({item->GetShort()});
                }
                else
                {
                        if(random(100) < 50)
                        {
                                item->eventMove(this_player());
                                salvaged = salvaged + ({item->GetShort()});
                        }
                }
        }
        if(!sizeof(salvaged))
        {
                resultsString = "You are unable to salvage anything.";
        } else if(sizeof(salvaged) == 1) {
                resultsString = "You manage to salvage usable pieces of
"+salvaged[0]+".";
        } else {
                resultsString = "You manage to salvage usable pieces of
"+(implode(salvaged-({salvaged[0]}),", "))+" and "+salvaged[0]+".";
        }
        write("You work on picking apart "+target->GetShort()+".");
        say(this_player()->GetName()+" works on picking apart
"+target->GetShort()+".");
        write(resultsString); 
        target->eventMove(LIB_FURNACE);
        return 1;
}
