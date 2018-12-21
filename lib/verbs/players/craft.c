/*
 * Part of Crafting Addition
 * craft.c - A verb to create a crafted item according to some pattern
 * using some base material.  The pattern can be either memorized or
 * in hand.  It checks if all the requirements listed in the recipe
 * object are met, takes the material and other ingredients and creates
 * the object.  If the object is personalizable, a prototype object is
 * created which can be adjusted.  Otherwise, the final product is
 * created directly.  A string argument can further be given to replace
 * any instances of %s in the description of the crafted item.  If the
 * object is a final product, %s are replaced only if the argument is
 * provided to allow for crafting of embellishments with the %s intact.
 * For prototype objects, the %s is replaced with a default string.
 * Once crafting begins, the materials are deducted immediately and the
 * sequence of messages dictated by the recipe is given.  When these
 * messages are finished, the item is created.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>
#include <rooms.h>
//#include ROOMS_H
inherit LIB_VERB;

mixed craft(mixed recipe, mixed material, mixed str);
mixed craftItem(mixed prototype,mixed recipe, mixed material, mixed str);
int printMessage(string message);
static void create() {
    verb::create();
    SetVerb("craft");
    SetRules("OBJ from OBJ STR", "OBJ from OBJ","STR from OBJ","STR from OBJ STR");
    SetErrorMessage("Craft what from what?");
    SetHelp("Syntax: <craft OBJECT from OBJECT [STRING]>\n"
            "Allows you to craft an object from a material if you have the recipe.");
}

mixed can_craft_obj_from_obj_str() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_craft_obj_from_obj_str(mixed recipe, mixed material, mixed str) {
        return craft(recipe,material,str);
}
mixed can_craft_obj_from_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_craft_obj_from_obj(mixed recipe, mixed material) {
        return craft(recipe,material,"");
}
mixed craft(mixed recipe, mixed material, mixed str)
{
        int amt,time;
        object prototype;
        object *contents;
        object thing;
        int rank, neededSkill;
        //Check the player's skill to see if they can complete the recipe
        rank = this_player()->GetSkillLevel(recipe->GetSkill());
        neededSkill = recipe->GetSkillLevel() + material->GetSkillLevel();
        if(rank < recipe->GetSkillLevel())
        {
                write("You can't make heads or tails of
"+recipe->GetShort()+".");
                return 1;
        }
        else if(rank < neededSkill)
        {
                //When they could do the recipe if the material was easy enough
                write("You think you may be able to follow " +
	recipe->GetShort() + " but you are fairly sure you can't pull it off yet using "
	+ material->GetKeyName() + ".");
                return 1;
        }
        prototype = new("/lib/crafting/prototype");
        contents = all_inventory(this_object());
        //Check for correct base material type
        if(material->GetMaterialType() != recipe->GetBaseMaterial())
        {
                write(capitalize(material->GetShort())+ " can not be used for " +
		recipe->GetShort()+".  You need a " + recipe->GetBaseMaterial() + ".");
                return 1;
        }
        //Check for enough of base material
        amt = recipe->GetBaseUnits();
        if(amt > material->GetNumUnits())
        {
                write("You do not have enough "+material->GetKeyName()+" in "+
material->GetShort()+" to complete "+recipe->GetShort()+".");
                return 1;
        }
        //Check for player holding all supplies in recipe.  An item is a 
        //match if it has an ID of the string listed in the recipe
        foreach(string supply in recipe->GetSupplies())
        {
                if(!present(supply,this_player()))
                {
                        write("You really need "+add_article(supply)+" on
hand.");
                        return 1;
                }               
        }
        //Check room player is in for all necessary supplies.  An item is a 
        //match if it has an ID of the string listed in the recipe
        foreach(string supply in recipe->GetRoomSupplies())
        {
                if(!present(supply,environment(this_player())))
                {
                        write("You really need "+add_article(supply)+" in the room.");
                        return 1;
                }               
        }
        //Check for player holding necessary ingredients.  An item is a 
        //match if it has an ID of the string listed in the recipe
        foreach(string ingred in recipe->GetIngredients())
        {
                if(!present(ingred,this_player()))
                {
                        write("You really need "+add_article(ingred)+".");
                        return 1;
                }               
        }
        //Deduct necessary ingredients from player.  They are stored in the
        //prototype object for potential salvage if the prototype is not used.
        foreach(string ingred in recipe->GetIngredients())
        {
                thing = present(ingred,this_player());
                if(!thing)
                {
                        write("You find yourself out of "+add_article(ingred)+" and halt your progress in confusion.");
                        return 1;
                }
                thing->eventMove(prototype);
        }
        write("You begin work on crafting "+add_article(recipe->GetProductName())+
	" out of "+material->GetKeyName()+".");
        say(this_player()->GetName()+" begins work on crafting something out of "+material->GetKeyName()+".");

        
        //Create another material object to hold the number of units that are
        //removed from the piece used to store in the prototype object
        thing = new(base_name(material));
        thing->eventMove(prototype);
        material->SubtractUnits(amt);
        thing->SetNumUnits(amt);
        //If the material is gone destroy the empty object
        if(material->GetNumUnits() == 0)
        {
                write("You use up the last of "+material->GetShort()+".");
                material->eventMove(LIB_FURNACE);
        }
        //Cycle through each of the messages listed in the recipe at 10
        //second intervals, and create the item when all these are done.
        time = 5;
        foreach(string msg in recipe->GetMessages())
        {
                call_out("printMessage",time,msg);
                time += 10;
        }
        call_out("craftItem",time,prototype,recipe,material,str);
        return 1;
}



//Actually create the item
mixed craftItem(mixed prototype,mixed recipe, mixed material, mixed str){
        int rank, neededSkill, diff,qualityValue;
        int amt;
        string quality;
        amt = recipe->GetBaseUnits();
        //Calculate the skill with which the item is crafted at a random
        //chance based on how far over the basic requirement the crafter is
        rank = this_player()->GetSkillLevel(recipe->GetSkill());
        neededSkill = recipe->GetSkillLevel() + material->GetSkillLevel();
        diff = rank - neededSkill;
        qualityValue = random(100);
        if(qualityValue < (diff/2) - 10)
        {
                //Perfect item
                quality = "perfect";
        }
        else if(qualityValue <= (diff*5) + 10)
        {
                //Quality item
                quality = "quality";
        }
        else
        {
                //Shoddy item
                quality = "shoddy";
        }

        //If the item cannot be personalized, just create the final product
        if(!recipe->GetPersonalizable())
        {
                mapping funcs;
                prototype = new(recipe->GetProduct());
                if(!(prototype->eventMove(this_player()) ))
                {
                        return "Error creating the prototype.";
                }
                
                //Non-personalizable descriptors only apply to embellishments
                prototype->SetFrontDescriptors(
recipe->GetFrontDescriptors());
                prototype->SetBackDescriptors( recipe->GetBackDescriptors());
                prototype->SetMass(material->GetMass(amt));
                prototype->SetBaseCost("silver",material->GetCost(amt));
                prototype->SetId(recipe->GetProductName());
                prototype->SetAdjectives( ({ recipe->GetMaterial() }) );
                //If a string argument is given, add it to the item
                if(sizeof(str))
                {
                        prototype->SetLong(prototype->GetLong()+"  It features
"+str+".");
                        prototype->personalize(str);
                        prototype->SetAdjectives(prototype->GetAdjectives()+
explode(str, " "));
                }
                //Call the functions specified for the product based on its
                //quality.  Perfect and quality products both get good functions
                //while the others call the shoddy ones
                if(quality == "perfect" || quality == "quality")
                        funcs = (recipe->GetGoodSpecials() +
material->GetGoodSpecials());
                else
                        funcs = (recipe->GetShoddySpecials() +
material->GetShoddySpecials());
                foreach(string func, string arg in funcs)
                {
                        if(arrayp(arg))
                                call_other(prototype, func, arg...);
                        else
                                call_other(prototype, func, arg);
                }
                write("You complete creation of "+prototype->GetShort()+".");
                return 1;
        }
        //create the manipulable prototype object and set it up on the player
        if(!(prototype->eventMove(this_player()) ))
        {
                return "Error creating the prototype.";
        }
        prototype->SetProduct(recipe->GetProduct());
        prototype->SetProductName(recipe->GetProductName());
        prototype->SetMaterial(material->GetKeyName());
        prototype->SetFrontDescriptors(material->GetFrontDescriptors()
+recipe->GetFrontDescriptors());
        prototype->SetBackDescriptors(material->GetBackDescriptors()+
recipe->GetBackDescriptors());
       
prototype->SetAllowedEmbellishments(material->GetAllowedEmbellishments());
        prototype->SetId(prototype->GetProductName());
        prototype->SetAdjectives( ({ prototype->GetMaterial() }) );
        prototype->SetBaseCost("silver",material->GetCost(amt));
        //Different long descriptions, descriptors and special function
        //calls are assigned depending on the quality of the crafting
        if(quality == "perfect")
        {
       
       
prototype->SetLong(capitalize(add_article(prototype->GetProductName()))+" made
of "+prototype->GetMaterial()+", a prime example of the craft.");
                prototype->SetFrontDescriptors( ({ "a perfect","a superb" }) +
prototype->GetFrontDescriptors() );
                prototype->SetSpecials(recipe->GetGoodSpecials() +
material->GetGoodSpecials());
                //Perfect items get an additional value bonus for workmanship
                prototype->SetBaseCost("silver",prototype->GetBaseCost()*1.5);
        }
        else if(quality == "quality")
        {
                prototype->SetLong(capitalize(add_article(prototype->GetProductName()) )+"
made of "+prototype->GetMaterial()+", a quality example of the craft.");
                prototype->SetFrontDescriptors(({ "a quality","an average"
})+prototype->GetFrontDescriptors() );
                prototype->SetSpecials(recipe->GetGoodSpecials()
+material->GetGoodSpecials());
        }
        else
        {
       
                prototype->SetLong(capitalize( add_article(prototype->GetProductName()))+"
made of "+prototype->GetMaterial()+", a crude example of the craft.");
                prototype->SetFrontDescriptors(({ "a shoddy","a crude" }) +
prototype->GetFrontDescriptors() );
                prototype->SetSpecials(recipe->GetShoddySpecials() +
material->GetShoddySpecials());
        }
        prototype->SetShort(prototype->GetFrontDescriptors()[0]+" "+
prototype->GetMaterial()+" "
+prototype->GetProductName()+prototype->GetBackDescriptors()[0]);
        prototype->SetMass(material->GetMass(amt));
        //If a string is provided, use it to personalize the object, otherwise
        //use a default string.
        if(sizeof(str))
        {
                prototype->SetLong(prototype->GetLong()+"  It features
"+str+".");
                prototype->personalize(str);
               
prototype->SetAdjectives(prototype->GetAdjectives()+explode(str, " "));
        }
        else {
                prototype->personalize("simple designs");
        }
        write("You have created "+prototype->GetShort()+" that seems ready for
embellishment, though you have left a few elements which will need to be
finished after that before you are done.");

        return 1;
}
int printMessage(string message)
{
        write(message);
}
mixed can_craft_str_from_obj() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

//If a string is given instead of a recipe, check for the corresponding recipe
//in the recipe book.
mixed do_craft_str_from_obj(mixed recipe, mixed material) {
        object recipeObj,recipebook;
        recipebook = present("recipebook", this_player());
        if(!recipebook)
        {
                write("Considering the items you remember how to make, you don't think you
know how to make "+add_article(recipe)+".");
                return 1;
        }
        recipeObj = present(recipe, recipebook);
        if(!recipeObj)
        {
                write("Considering the items you remember how to make, you don't think you
know how to make "+add_article(recipe)+".");
                return 1;
        }
        write("You consider the items you remember how to make and recall
"+recipeObj->GetShort()+".");
        return craft(recipeObj,material,"");
}

mixed can_craft_str_from_obj_str() {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

//If a string is given instead of a recipe, check for the corresponding recipe
//in the recipe book.
mixed do_craft_str_from_obj_str(mixed recipe, mixed material, mixed str) {
        object recipeObj,recipebook;
        recipebook = present("recipebook", this_player());
        if(!recipebook)
        {
                write("Considering the items you remember how to make, you don't think you
know how to make "+add_article(recipe)+".");
                return 1;
        }
        recipeObj = present(recipe, recipebook);
        if(!recipeObj)
        {
                write("Considering the items you remember how to make, you don't think you
know how to make "+add_article(recipe)+".");
                return 1;
        }
        write("You consider the items you remember how to make and recall
"+recipeObj->GetShort()+".");
        return craft(recipeObj,material,str);
}
