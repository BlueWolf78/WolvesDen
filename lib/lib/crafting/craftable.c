/*
 * Part of Crafting Addition
 * craftable.c - A file to inherit on an object to make it save
 * correctly as the target product of a recipe.  While this is
 * not required, it is designed to make it so you do not need
 * to set the necessary saves by hand or have crafted objects
 * revert to the unpersonalized pattern on reset.
 * To make an item craftable, the easiest way is to inherit this
 * file and then call SetCraftSaves with the appropriate type in
 * the create function.
 * Author: Tsarenzi Updated: 09/04/2009
 */
#include <lib.h>
#include "/lib/crafting/include/crafting.h"

string array GetCraftSaves(int type);
void SetCraftSaves(int type)
{
        this_object()->AddSave(GetCraftSaves(type));
}

string array GetCraftSaves(int type)
{
        string array saves =
({"Adjectives","KeyName","Short","ExternalDesc","Id","Mass","Cost"});
        switch(type)
        {
                case C_ARMOR:
                        saves = saves + ({"Wear","Protection","DamagePoints"});
                        break;
                case C_FOOD:
                        saves = saves +
({"MyMessage","MealStrength","OtherMessage"});
                        break;
                case C_TREASURE:
                        saves = saves + ({ });
                        break;
                case C_WEAPON:
                        saves = saves + ({ "WeaponType", "Class", "Wield",
"DamageType", "DamagePoints"});
                        break;
                case C_DRINK:
                        saves = saves +
({"MyMessage","MealStrength","OtherMessage"});
                        break;
                case C_CONTAINER:
                        saves = saves + ({ "MaxCarry" });
                        break;
                default:
                        break;
        }
        return saves;
}
