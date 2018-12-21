/*
 * Part of Crafting Addition
 * memorize.c - A verb to memorize recipes for the crafter to use when
 * ever they like without having a copy of the recipe.  In actuality, 
 * the recipe is moved into an invisible recipebook which the player
 * retains on death and cannot manipulate. The physical recipe is
 * considered discarded after memorization.  
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("memorize");
    SetRules("OBJ");
    SetErrorMessage("Memorize what?");
    SetHelp("Syntax: <memorize OBJECT>\n"
            "Allows you to commit a recipe to memory.");
}

mixed can_memorize_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_memorize_obj(object recipe) {
        object recipebook;
        object memRecipe;
        int rank;
        //you may not memorize recipes you do not have the skill for
        rank = this_player()->GetSkillLevel(recipe->GetSkill());
        if(rank < recipe->GetSkillLevel())
        {
                write("You can't make heads or tails of
"+recipe->GetShort()+".");
                return 1;
        }
        //If the player does not already have a recipebook, create a
        //new one.
        recipebook = present("recipebook",this_player());
        if(!recipebook)
        {
                recipebook = new("/lib/crafting/recipebook.c");
                recipebook->eventMove(this_player());
        }
        //Since memorized recipes are referenced by their shorts, it is not
        //possible to memorize more than one recipe with the same short
        memRecipe = present(recipe->GetProductName(), recipebook);
        if(memRecipe)
        {
                if(memRecipe->GetShort() == recipe->GetShort())
                {
                        write("You have already memorized "+memRecipe->GetShort()+", and see no reason to commit this one to memory as well.");
                        return 1;
                }       
        }
        recipe->eventMove(recipebook);
        write("You commit "+recipe->GetShort()+" to memory and then discard it.");
        say(this_player()->GetName()+" reads through "+recipe->GetShort()+" before
discarding it.");
        return 1;
}
