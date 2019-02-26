#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string>
#include <map>
#include "../../../netbus/proto_man.h"

std::map<int,std::string> cmd_map;/* =
{
	{0,"LoginReq"},
	{1,"LoginRes"},
};
*/


void init_pf_cmd_map() 
{
	cmd_map[0] = "LoginReq";
	cmd_map[1] = "LoginRes";
	proto_man::register_protobuf_cmd_map(cmd_map);
}
