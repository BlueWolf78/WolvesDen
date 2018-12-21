/*    /verbs/items/stir.c
 *    Part of Alchemy Addition
 *    Created by Tsarenzi
 *    Version: 1.0
 *    Last modified: 08/09/2009
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("stir");
    SetSynonyms("mix");
    SetRules("OBJ");
    SetErrorMessage("Stir what?");
    SetHelp("Syntax: <stir OBJECT>\n"
            "Allows you to mix up a cauldron of alchemical goodies.");
}

mixed can_stir_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_stir_obj(object target) {
    return target->eventStir(this_player());
}