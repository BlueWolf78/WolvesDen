#include <lib.h>

inherit LIB_DAEMON;

mapping kills = ([]);

mixed cmd(string args) {
    string *stats;
    string killfile, ret, tmp, name;
    int i, x, y;

    name = lower_case(this_player()->GetKeyName());
    killfile = "/save/kills/"+name[0..0]+"/"+name;
    if(!file_exists(killfile)) {
        write("%^YELLOW%^You have no kills to your name.%^RESET%^");
        return 1;
    }
    kills = restore_variable(read_file(killfile));
    if(!sizeof(kills)){
        write("%^YELLOW%^You have never harmed a living thing.%^RESET%^");
        return 1;
    }
    ret = "%^GREEN%^You are " +this_player()->GetShort() + "%^GREEN%^, level " +
        this_player()->GetLevel();
    if( (tmp = this_player()->GetClass()) ) 
        ret += " " + capitalize(tmp);
    else ret += " Drifter";
    ret += " (" + this_player()->GetRace() + ")%^RESET%^\n";
    ret+= "%^YELLOW%^Your list of victories comprises the following:%^RESET%^\n\n";
    stats = map(keys(kills),
            (: sprintf("%:-20s: %:-1i", $1, 
                       kills[$1]) :));
    i = sizeof(stats);
    while(i--) if( (y = strlen(stats[i])) > x ) x = y;
    x = ((int *)this_player()->GetScreen())[0]/(x+2);
    ret += format_page(stats, x);
    message("system", ret, this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: kills\n\n"
            "A list of your victories.");
}
