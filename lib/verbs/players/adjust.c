/*
 * Part of Crafting Addition
 * adjust.c - a verb to cycle through the front and back descriptor
 * choices to personal its look.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("adjust");
    SetRules("OBJ STR");
    SetErrorMessage("Adjust what how?");
    SetHelp("Syntax: <adjust OBJECT <front/back>>\n"
            "Allows you to adjust which desciptors of a prototype are
showing.");
}

mixed can_adjust_obj_str() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_adjust_obj_str(object target, mixed str) {
        int front, back;
        front = target->GetCurrentFront();
        back = target->GetCurrentBack();
        if(str == "front")
        {
                front++;
                if(front >= sizeof(target->GetFrontDescriptors()) )
                        front = 0;
                target->SetCurrentFront(front);
        }
        else if(str == "back")
        {
                back++;
                if(back >= sizeof(target->GetBackDescriptors()) )
                        back = 0;
                target->SetCurrentBack(back);
        }
        else
        {
                write(str+" is not a valid choice. You must specify front or
back.");
                return 1;
        }
        target->SetShort(target->GetFrontDescriptors()[front]+"
"+target->GetMaterial()+"
"+target->GetProductName()+target->GetBackDescriptors()[back]);
        write("You adjust the "+target->GetProductName()+" slightly, leaving you with
" +target->GetShort()+".");
        return 1;
}
