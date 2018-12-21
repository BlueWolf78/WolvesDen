/*
 * Part of Crafting Addition
 * finish.c - A verb to turn a prototype object into a finished usable
 * product with the properties set in the prototype.  The prototype
 * object is destroyed, replaced with the real product.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>
#include <rooms.h>
//#include ROOMS_H

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("finish");
    SetRules("OBJ");
    SetErrorMessage("Finish what?");
    SetHelp("Syntax: <finish OBJECT>\n"
            "Allows you to complete a crafted item prototype.");
}

mixed can_finish_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_finish_obj(object prototype) {
        object prod;
        prod = new(prototype->GetProduct());

        prod->SetKeyName(prototype->GetProductName());
        prod->SetId(prototype->GetId());
        prod->SetAdjectives(prototype->GetAdjectives()+explode(prototype->GetShort(), " "));
        prod->SetShort(prototype->GetShort());
        prod->SetLong(prototype->GetLong());
        prod->SetMass(prototype->GetMass());
        prod->SetBaseCost("silver",prototype->GetCost());
        foreach(string func, mixed arg in prototype->GetSpecials())
        {
                if(arrayp(arg))
                        call_other(prod, func, arg...);
                else
                        call_other(prod, func, arg);
        }
	prod->eventMove(this_player());
        prototype->eventMove(LIB_FURNACE);
        write("You put the finishing touches on "+prod->GetShort()+".");
        say(this_player()->GetName()+" puts the finishing touches on
"+prod->GetShort()+".");
        return 1;
}
