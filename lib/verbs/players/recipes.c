/*
 * Part of Crafting Addition
 * recipes.c - a verb to examine the list of memorized recipes when used
 * without an argument, or to look at the details of a specific recipe
 * which has been memorized as if the crafter were reading the physical
 * recipe.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("recipes");
    SetRules("", "STR");
    SetErrorMessage("Recipes which?");
    SetHelp("Syntax: <recipes>\n       <recipes STR>"
            "Allows you to look at the list of recipes which you know or
examine a specific recipe.");
}

mixed can_recipes() {
    return 1;
}

mixed do_recipes() {
        //List out all recipes
        object *contents;
        object recipebook;
        //If no recipebook, no recipes are memorized
        recipebook = present("recipebook", this_player());
        if(!recipebook)
        {
                write("You can't remember how to craft anything.");
                return 1;
        }
        contents = all_inventory(recipebook);
        if(!sizeof(contents))
        {
                write("You can't remember how to craft anything.");
        }
        write("You consider the different recipes you have memorized and
recall:");
        foreach(object thing in contents)
        {
                write(thing->GetShort());
        }
        return 1;
}

mixed can_recipes_str() {
    return 1;
}

mixed do_recipes_str(mixed str) {
        //describe one recipe
        object recipebook, recipe;
        //If no recipebook, no recipes are memorized
        recipebook = present("recipebook", this_player());
        if(!recipebook)
        {
                write("You can't remember how to craft that.");
                return 1;
        }
        recipe = present(str,recipebook);
        if(!recipe)
        {
                write("You can't remember how to craft that.");
                return 1;
        }
        write("You try to recall "+recipe->GetShort()+":");
        recipe->ReadFunction();
        return 1;
}
