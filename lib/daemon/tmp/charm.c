#include <bboard.h>
#include <cfg.h>
#include <cgi.h>
#include <clean_up.h>
#include <clock.h>
#include <commands.h>
#include <comp.h>
#include <compat.h>
#include <config.h>
#include <council.h>
#include <daemons.h>
#include <deputies.h>
#include <dirs.h>
#include <domains.h>
#include <events.h>
#include <files.h>
#include <flags.h>
#include <function.h>
#include <global.h>
#include <iips.h>
#include <lib.h>
#include <link.h>
#include <localtime.h>
#include <logs.h>
#include <lvs.h>
#include <modules.h>
#include <mssp.h>
#include <mudlib.h>
#include <network.h>
#include <news.h>
#include <objects.h>
#include <origin.h>
#include <parse_com.h>
#include <parser_error.h>
#include <post.h>
#include <pov.h>
#include <privs.h>
#include <props.h>
#include <rcp.h>
#include <rooms.h>
#include <runtime_config.h>
#include <save.h>
#include <secrets.h>
#include <socket.h>
#include <socket_err.h>
#include <sockets.h>
#include <std.h>
#include <type.h>
#include <user.h>
#include <virtual.h>
#include <voting.h>
#include <armor_types.h>
#include <assessment.h>
#include <astar.h>
#include <body_types.h>
#include <boobytraps.h>
#include <build_types.h>
#include <climb.h>
#include <combat_messages.h>
#include <damage_types.h>
#include <gossip.h>
#include <graphics.h>
#include <jump.h>
#include <magic.h>
#include <magic_protection.h>
#include <marriage.h>
#include <materials.h>
#include <meal_types.h>
#include <medium.h>
#include <message_class.h>
#include <motion.h>
#include <mouth_types.h>
#include <position.h>
#include <race.h>
#include <respiration_types.h>
#include <rounds.h>
#include <size_types.h>
#include <soul.h>
#include <stargate.h>
#include <talk_type.h>
#include <terrain_types.h>
#include <vendor_types.h>
#include <vision.h>
#ifndef charm
#define charm "DEFINED_D: undefined"
#endif

mixed defcheck(){
int err;
mixed ret;
err = catch( ret = charm );
if(err) ret = "DEFINED_D: error";
else if(undefinedp(ret)) ret = "DEFINED_D: undefined";
return ret;
}