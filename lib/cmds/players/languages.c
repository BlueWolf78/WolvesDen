#include <lib.h>
#include <talk_type.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    mapping FluencyMap = ([]);
    string *langs = this_player()->GetLanguages();

    if(str){
        mixed ret = this_player()->CanSpeak(0, TALK_LOCAL, "foo", str);
        if(intp(ret)) {
            this_player()->SetDefaultLanguage(str);
            write("%^GREEN%^You are now speaking in "+capitalize(lower_case(str))+".%^RESET%^");
            return 1;
        }
        else return ret;
    }

    foreach(string lang in langs){
        FluencyMap[lang] = this_player()->GetLanguageLevel(lang);
    }

    if(this_player()->GetPolyglot()){
        write("%^GREEN%^You understand all languages with 100% proficiency.%^RESET%^");
    }

    write("%^YELLOW%^You speak: %^RESET%^");
    foreach(string key, int val in FluencyMap){
        write(capitalize(key)+" with "+val+"% proficiency.");
    }
    write("%^GREEN%^Your current default language is: %^BOLD%^"+
            this_player()->GetDefaultLanguage()+".%^GREEN%^");

    return 1;
}

string GetHelp() {
    return ("Syntax: language [LANGUAGE]\n\n"
            "This command reports which languages you speak and understand.\n"
            "If a language is specified and you know the language, it is "
            "set as your default speaking language.\n"
            "See also: languages, speak");
}

