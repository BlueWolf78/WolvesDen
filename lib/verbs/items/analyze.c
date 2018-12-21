/*    /verbs/items/analyze.c
 *    Part of Alchemy Addition
 *    Created by Tsarenzi
 *    Version: 1.0
 *    Last modified: 08/09/2009
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("analyze");
    SetRules("OBJ");
    SetErrorMessage("Analyze what?");
    SetHelp("Syntax: <analyze OBJECT>\n"
            "Allows you to analyze alchemical ingredients.");
}

mixed can_analyze_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_analyze_obj(object target) {
    return target->eventAnalyze(this_player());
}