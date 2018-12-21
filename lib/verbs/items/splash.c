/* /verbs/items/splash.c
 * Part of Alchemy Addition
 * Verb to splash alchemical potions at other objects, living and
 * non-living, within the same room, for that potion's affects to
 * activate on the splash target.
 * Author: Tsarenzi Updated: 08/18/2009
 */
#include <lib.h>
#include <position.h>
#include "damage_types.h"
#include ROOMS_H

inherit LIB_VERB;

int eventSplash(object splasher, object target, object potion);

static void create() {
    verb::create();
    SetVerb("splash");
    SetRules("OBJ at STR", "OBJ on STR");
    SetErrorMessage("Splash what at who?");
    SetHelp("Syntax: splash OBJECT at TARGET\n\n"
            "This allows you to splash someone or something with a potion.");
}

mixed can_splash_obj_on_str(mixed splasher, mixed target) {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }

    return this_player()->CanManipulate();
}

//Target must be present in the room with the splasher or in their inventory
mixed do_splash_obj_on_str(mixed obj, mixed target) {
    object targ;
   
    targ = present(target, environment(this_player()));
    if(!targ) targ = present(target, this_player());
    if(!targ) return capitalize(target)+" isn't here.";

    return eventSplash(this_player(), targ, obj);
}

mixed can_splash_obj_at_str(mixed splasher, mixed target) {
    if( this_player()->GetParalized() > 0 ) {
        return "You cannot do anything.";
    }

    return this_player()->CanManipulate();
}

//Target must be present in the room with the splasher or in their inventory
mixed do_splash_obj_at_str(mixed obj, mixed target) {
    object targ;
   
    targ = present(target, environment(this_player()));
    if(!targ) targ = present(target, this_player());
    if(!targ) return capitalize(target)+" isn't here.";

    return eventSplash(this_player(), targ, obj);
}


/* The splasher splashes a target with the given potion.  The splasher
 * must have the potion and a skill check is done to see whether or not
 * it hits, based on projectile attack versus projectile defense, and
 * coordination, agility, luck and mobility.  The messaging is printed
 * and the potion is consumed whether or not the splash is successful. 
 * The potion's affects are put onto the target in the event of success.
 */
int eventSplash(object splasher, object target, object potion) {
    object env;
    float tmp;
    int value;


    if(!splasher || !(env = environment(splasher))) return 0;
    if(!target || (!present(target, env) && !present(target, splasher)))  {
        splasher->eventPrint("Your target is no longer present.");
        return 1;
    }

    //Calculate likelihood of successful splash
    tmp = splasher->GetSkillLevel("projectile attack")*2.5;
    tmp += splasher->GetStatLevel("coordination")/2.0;
    tmp += splasher->GetStatLevel("luck")/3.0;
    tmp += splasher->GetMobility()/3.0;
    tmp -= target->GetSKillLevel("projectile defense");
    tmp -= target->GetStatLevel("luck")/4.0;
    tmp -= target->GetStatLevel("agility")/4.0;
    tmp -= target->GetMobility()/5.0;
    tmp *= 1.0;
    if(tmp < 1.0) tmp = 1.0;
    value = to_int(tmp);

    splasher->AddSkillPoints("projectile attack", to_int(value * 3.0));
    splasher->AddStaminaPoints(-(1 + random(3)));

    splasher->eventPrint(" You splash "+potion->GetShort()+" towards
"+target->GetName()+ ".");
    target->eventPrint(""+splasher->GetName()+" splashes
"+potion->GetShort()+" towards you.");
    environment(splasher)->eventPrint(""+splasher->GetName()+" splashes
"+potion->GetShort()+" towards "+ target->GetName()+ ".",  ({splasher,
target}));
    //Only living targets can evade.  No longer being present is considered
    //successful evasion.
    if(living(target) && (value < random(100) || !present(target,
environment(splasher)))) {
        splasher->eventPrint("" + target->GetName()+ " evades your splash.");
        target->eventPrint("You easily dodge " +possessive_noun(splasher)+ "
attempted splash.");
        environment(splasher)->eventPrint("" + target->GetName()+ " dodges
"+possessive_noun(splasher)+ " attempted splash.", ({splasher, target}));
        potion->eventMove(ROOM_FURNACE);
        return 1;
    }  

    //Successful hit
    splasher->eventPrint(""+capitalize(potion->GetShort())+" covers
"+target->GetName()+ ", quickly soaking in.");
    target->eventPrint(""+capitalize(potion->GetShort())+" covers you, quickly
soaking in.");
    environment(splasher)->eventPrint(""+capitalize(potion->GetShort())+"
covers "+ target->GetName()+", quickly soaking in.",  ({splasher, target}));


    //Apply potion's affects to the target
    potion->splash(target, "");

    
    return 1;
}