
--{stype,ctype,utag,body}
function on_auth_recv_cmd(s,msg)
end

function on_auth_session_disconnect(s)
    local ip,port = Session.get_address(s)
    Logger.debug(port,ip)
end

local auth_server = 
{
    on_session_recv_cmd = on_auth_recv_cmd,
    on_session_disconnect = on_auth_session_disconnect,
}

return auth_server