/*
 * Part of the Crafting Addition
 * recipebook.c - an invisible recipe book to simulate a
 * crafter's memory for recipes which they have memorized.
 * When recipes are memorized, they are simply transferred
 * into this container object, and the craft and recipes 
 * verbs consult it when used.  It should not be manipulable
 * or observable by crafters.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>

inherit LIB_STORAGE;

static void create() {
        ::create();
        SetKeyName("recipebook");
        SetId( ({ "recipebook" }) );
        SetCanClose(0);
        SetClosed(1);
        SetInvis(1);
        SetRetainOnDeath(1);
        SetPreventPut("You cannot manipulate that.");
        SetPreventDrop("You cannot manipulate that.");
        SetPreventGet("You cannot manipulate that.");
}
varargs mixed CanShowInterior(object who, object target)
{
        return "Look in what?";
}
varargs mixed eventShow(object who, string component)
{
        return "Look at what?";
}
void init(){
::init();
}
