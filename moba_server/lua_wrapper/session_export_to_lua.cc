﻿#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "session_export_to_lua.h"
#include "../netbus/session.h"
#include "../netbus/proto_man.h"
#include "google/protobuf/message.h"
#include "../utils/logger.h"
#include "../netbus/netbus.h"
  
using namespace google::protobuf;

static int session_close_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,0);
	if(s == NULL)
	{
		goto lua_failed;
	}
	s->close();
lua_failed:
	return 0;
}

static int get_address_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s== NULL)
	{
		goto lua_failed;
	}
	int client_port;
	const char* ip = s->get_address(&client_port);
	lua_pushstring(tolua_S,ip);
	lua_pushinteger(tolua_S,client_port);
	return 2;
lua_failed:
	return 1;
}

static google::protobuf::Message* lua_table_to_protobuf(lua_State* L, int stack_index, const char* msg_name) {
	if (!lua_istable(L, stack_index)) {
		return NULL;
	}

	Message* message = proto_man::create_message(msg_name);
	if (!message) {
		log_error("cant find message  %s from compiled poll \n", msg_name);
		return NULL;
	}

	const Reflection* reflection = message->GetReflection();
	const Descriptor* descriptor = message->GetDescriptor();

	for (int32_t index = 0; index < descriptor->field_count(); ++index) {
		const FieldDescriptor* fd = descriptor->field(index);
		const string& name = fd->name();

		bool isRequired = fd->is_required();
		bool bReapeted = fd->is_repeated();
		lua_pushstring(L, name.c_str());
		lua_rawget(L, stack_index);

		bool isNil = lua_isnil(L, -1);

		if (bReapeted) {
			if (isNil) {
				lua_pop(L, 1);
				continue;
			}
			else {
				bool isTable = lua_istable(L, -1);
				if (!isTable) {
					log_error("cant find required repeated field %s\n", name.c_str());
					proto_man::release_message(message);
					return NULL;
				}
			}

			lua_pushnil(L);
			for (; lua_next(L, -2) != 0;) {
				switch (fd->cpp_type()) {

				case FieldDescriptor::CPPTYPE_DOUBLE:
				{
					double value = luaL_checknumber(L, -1);
					reflection->AddDouble(message, fd, value);
				}
					break;
				case FieldDescriptor::CPPTYPE_FLOAT:
				{
					float value = luaL_checknumber(L, -1);
					reflection->AddFloat(message, fd, value);
				}
					break;
				case FieldDescriptor::CPPTYPE_INT64:
				{
					int64_t value = luaL_checknumber(L, -1);
					reflection->AddInt64(message, fd, value);
				}
					break;
				case FieldDescriptor::CPPTYPE_UINT64:
				{
					uint64_t value = luaL_checknumber(L, -1);
					reflection->AddUInt64(message, fd, value);
				}
					break;
				case FieldDescriptor::CPPTYPE_ENUM: 
				{
					int32_t value = luaL_checknumber(L, -1);
					const EnumDescriptor* enumDescriptor = fd->enum_type();
					const EnumValueDescriptor* valueDescriptor = enumDescriptor->FindValueByNumber(value);
					reflection->AddEnum(message, fd, valueDescriptor);
				}
				break;
				case FieldDescriptor::CPPTYPE_INT32:
				{
					int32_t value = luaL_checknumber(L, -1);
					reflection->AddInt32(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_UINT32:
				{
					uint32_t value = luaL_checknumber(L, -1);
					reflection->AddUInt32(message, fd, value);
				}
					break;
				case FieldDescriptor::CPPTYPE_STRING:
				{
					size_t size = 0;
					const char* value = luaL_checklstring(L, -1, &size);
					reflection->AddString(message, fd, std::string(value, size));
				}
				break;
				case FieldDescriptor::CPPTYPE_BOOL:
				{
					bool value = lua_toboolean(L, -1);
					reflection->AddBool(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_MESSAGE:
				{
					Message* value = lua_table_to_protobuf(L, lua_gettop(L), fd->message_type()->name().c_str());
					if (!value) {
						log_error("convert to message %s failed whith value %s\n", fd->message_type()->name().c_str(), name.c_str());
						proto_man::release_message(value);
						return NULL;
					}
					Message* msg = reflection->AddMessage(message, fd);
					msg->CopyFrom(*value);
					proto_man::release_message(value);
				}
				break;
				default:
					break;
				}

				// remove value£¬ keep the key
				lua_pop(L, 1);
			}
		}
		else {

			if (isRequired) {
				if (isNil) {
					log_error("cant find required field %s\n", name.c_str());
					proto_man::release_message(message);
					return NULL;
				}
			}
			else {
				if (isNil) {
					lua_pop(L, 1);
					continue;
				}
			}

			switch (fd->cpp_type()) {
			case FieldDescriptor::CPPTYPE_DOUBLE:
			{
				double value = luaL_checknumber(L, -1);
				reflection->SetDouble(message, fd, value);
			}
				break;
			case FieldDescriptor::CPPTYPE_FLOAT:
			{
				float value = luaL_checknumber(L, -1);
				reflection->SetFloat(message, fd, value);
			}
				break;
			case FieldDescriptor::CPPTYPE_INT64:
			{
				int64_t value = luaL_checknumber(L, -1);
				reflection->SetInt64(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_UINT64:
			{
				uint64_t value = luaL_checknumber(L, -1);
				reflection->SetUInt64(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_ENUM: 
			{
				int32_t value = luaL_checknumber(L, -1);
				const EnumDescriptor* enumDescriptor = fd->enum_type();
				const EnumValueDescriptor* valueDescriptor = enumDescriptor->FindValueByNumber(value);
				reflection->SetEnum(message, fd, valueDescriptor);
			}
			break;
			case FieldDescriptor::CPPTYPE_INT32:
			{
				int32_t value = luaL_checknumber(L, -1);
				reflection->SetInt32(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_UINT32:
			{
				uint32_t value = luaL_checknumber(L, -1);
				reflection->SetUInt32(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_STRING:
			{
				size_t size = 0;
				const char* value = luaL_checklstring(L, -1, &size);
				reflection->SetString(message, fd, std::string(value, size));
			}
			break;
			case FieldDescriptor::CPPTYPE_BOOL:
			{
				bool value = lua_toboolean(L, -1);
				reflection->SetBool(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
			{
				Message* value = lua_table_to_protobuf(L, lua_gettop(L), fd->message_type()->name().c_str());
				if (!value) {
					log_error("convert to message %s failed whith value %s \n", fd->message_type()->name().c_str(), name.c_str());
					proto_man::release_message(message);
					return NULL;
				}
				Message* msg = reflection->MutableMessage(message, fd);
				msg->CopyFrom(*value);
				proto_man::release_message(value);
			}
				break;
			default:
				break;
			}
		}
		// pop value
		lua_pop(L, 1);
	}
	return message;
}

// {1: stype, 2: ctype, 3: utag, 4 body}
static int send_msg_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	if(!lua_istable(tolua_S,2))
	{
		goto lua_failed;
	}
	//read table key-value and push in lua stack
	struct cmd_msg msg;
	int n = luaL_len(tolua_S,2);
	if(n != 4 && n !=3 )
	{
		goto lua_failed;
	}
	lua_pushnumber(tolua_S,1);
	lua_gettable(tolua_S,2);
	msg.stype = luaL_checkinteger(tolua_S,-1);

	lua_pushnumber(tolua_S, 2);
	lua_gettable(tolua_S, 2);
	msg.ctype = luaL_checkinteger(tolua_S, -1);

	lua_pushnumber(tolua_S, 3);
	lua_gettable(tolua_S, 2);
	msg.utag = luaL_checkinteger(tolua_S, -1);

	lua_pushnumber(tolua_S, 4);
	lua_gettable(tolua_S, 2);

	if (n == 3)
	{
		msg.body = NULL;
		s->send_msg(&msg);
		return 0;
	}

	if(proto_man::proto_type() == PROTO_JSON)
	{
		msg.body = (char*)lua_tostring(tolua_S,6);
		s->send_msg(&msg);
	}
	else
	{
		if(!lua_istable(tolua_S,6))
		{
			msg.body = NULL;
			s->send_msg(&msg);
		}
		else
		{
			if(!lua_istable(tolua_S, -1))
			{
				msg.body = NULL;
				s->send_msg(&msg);
			}
			else
			{
				const char* msg_name = proto_man::protobuf_cmd_name(msg.ctype);
				msg.body = lua_table_to_protobuf(tolua_S,6,msg_name);
				s->send_msg(&msg);
				proto_man::release_message((google::protobuf::Message*)(msg.body));
			}
		}
	}
lua_failed:
	return 0;
}

static int set_utag_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	unsigned int utag = lua_tointeger(tolua_S,2);
	s->utag = utag;

lua_failed:
	return 0;
}

static int get_utag_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	lua_pushinteger(tolua_S,s->utag);
	return 1;

lua_failed:
	return 0;
}


static int set_uid_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	unsigned int uid = lua_tointeger(tolua_S,2);
	s->uid = uid;

lua_failed:
	return 0;
}

static int get_uid_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	lua_pushinteger(tolua_S,s->uid);
	return 1;

lua_failed:
	return 0;
}


static int as_client_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	lua_pushinteger(tolua_S,s->as_client);
	return 1;
lua_failed:
	return 0;
}

static int send_raw_cmd_tolua(lua_State* tolua_S)
{
	session* s = (session*)tolua_touserdata(tolua_S,1,NULL);
	if(s == NULL)
	{
		goto lua_failed;
	}
	struct raw_cmd* raw = (struct raw_cmd*)tolua_touserdata(tolua_S, 2, NULL);
	if(raw == NULL)
	{
		goto lua_failed;
	}
	s->send_raw_cmd(raw);
lua_failed:

	return 0;
}

static void udp_send_msg(char* ip, int port,struct cmd_msg* msg)
{
	unsigned char* encode_pkg = NULL;
	int encode_len = 0;
	encode_pkg = proto_man::encode_msg_to_raw(msg,&encode_len);
	if(encode_pkg)
	{
		netbus::instance()->udp_send_to(ip,port,encode_pkg,encode_len);
		proto_man::msg_raw_free(encode_pkg);
	}
}

// {1: stype, 2: ctype, 3: utag, 4 body}
static int udp_send_msg_tolua(lua_State* tolua_S)
{
	char* ip = (char*)tolua_tostring(tolua_S,1,NULL);
	if(ip == NULL)
	{
		goto lua_failed;
	}
	int port = (int)tolua_tonumber(tolua_S,2,NULL);
	if(port == NULL)
	{
		goto lua_failed;
	}
	if(!lua_istable(tolua_S,3))
	{
		goto lua_failed;
	}
	//read table key-value and push in lua stack
	struct cmd_msg msg;
	int n = luaL_len(tolua_S,3);
	if(n != 4 && n !=3 )
	{
		goto lua_failed;
	}
	lua_pushnumber(tolua_S,1);
	lua_gettable(tolua_S,3);
	msg.stype = luaL_checkinteger(tolua_S,-1);

	lua_pushnumber(tolua_S, 2);
	lua_gettable(tolua_S, 3);
	msg.ctype = luaL_checkinteger(tolua_S, -1);

	lua_pushnumber(tolua_S, 3);
	lua_gettable(tolua_S, 3);
	msg.utag = luaL_checkinteger(tolua_S, -1);

	lua_pushnumber(tolua_S, 4);
	lua_gettable(tolua_S, 3);

	if (n == 3)
	{
		msg.body = NULL;
		udp_send_msg(ip,port,&msg);
		return 0;
	}

	if(proto_man::proto_type() == PROTO_JSON)
	{
		msg.body = (char*)lua_tostring(tolua_S,6);
		udp_send_msg(ip,port,&msg);
	}
	else
	{
		if(!lua_istable(tolua_S, -1))
		{
			msg.body = NULL;
			udp_send_msg(ip,port,&msg);
		}
		else
		{
			const char* msg_name = proto_man::protobuf_cmd_name(msg.ctype);
			msg.body = lua_table_to_protobuf(tolua_S,lua_gettop(tolua_S),msg_name);
			udp_send_msg(ip,port,&msg);
			proto_man::release_message((google::protobuf::Message*)(msg.body));
		}
	}
lua_failed:
	return 0;
}

int register_session_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"Session",0);
		tolua_beginmodule(tolua_S,"Session");

		tolua_function(tolua_S,"close",session_close_tolua);
		tolua_function(tolua_S,"get_address",get_address_tolua);
		tolua_function(tolua_S,"send_msg",send_msg_tolua);
		tolua_function(tolua_S,"set_utag",set_utag_tolua);
		tolua_function(tolua_S,"get_utag",get_utag_tolua);
		tolua_function(tolua_S,"set_uid",set_uid_tolua);
		tolua_function(tolua_S,"get_uid",get_uid_tolua);
		tolua_function(tolua_S,"as_client",as_client_tolua);
		tolua_function(tolua_S, "send_raw_cmd", send_raw_cmd_tolua);
		tolua_function(tolua_S, "udp_send_msg",udp_send_msg_tolua);

		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}