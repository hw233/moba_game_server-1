local game_config = require("game_config")
local mysql_conn = nil
local moba_ugame_config = require("moba_game_config")
local utils = require("utils")

local function is_connected()
    if not mysql_conn then
        return false
    end
    return true
end

local function mysql_connect_to_system_center()
    if is_connected() then return end
    local conf = game_config.game_mysql
    Mysql.connect(conf.host,conf.port,conf.db_name,conf.uname,conf.upwd,
    function(err,conn)
        if err then
            Logger.error(err) 
            Scheduler.once(mysql_connect_to_ugame_system_center,5000)
            return
        end
        Logger.debug("connect to ugame_system database success!!!")
        mysql_conn = conn
    end)
end
mysql_connect_to_system_center()

local function _check_is_connected_sql(ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!!!",nil)
        end
        return nil
    end
    return true
end
--get info by uid
local function get_ugame_system_uinfo(uid, ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    local sql = "select uid, uchip, uchip1, uchip2, uvip, uvip_endtime, udata1,udata2,udata3,uexp,ustatus from ugame where uid = %d limit 1"
	local cmd_sql = string.format(sql, uid)	
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then
            if ret_handler ~= nil then
                ret_handler(err,nil)
            end
            return
        end
        --没有这条数据
        if ret == nil or #ret <= 0 then
            if ret_handler ~= nil then
                ret_handler(nil,nil)
            end
            return
        end
        --end
        local result = ret[1]
        local uinfo = {}
        uinfo.uid = tonumber(result[1])
        uinfo.uchip = tonumber(result[2])
        uinfo.uchip1 = tonumber(result[3])
        uinfo.uchip2 = tonumber(result[4])
        uinfo.uvip = tonumber(result[5])
        uinfo.uvip_endtime = tonumber(result[6])
        uinfo.udata1 = tonumber(result[7])
        uinfo.udata2 = tonumber(result[8])
        uinfo.udata3 = tonumber(result[9])
        uinfo.uexp = tonumber(result[10])
        uinfo.ustatus = tonumber(result[11])
        ret_handler(nil,uinfo)
    end)

end


local function insert_world_rank_info(uid,uchip,uvip)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    local sql = "insert into urank(`uid`, `uchip`, `uvip`)values(%d, %d, %d)"
	local cmd_sql = string.format(sql, uid, uchip, uvip)
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            Logger.debug(err)
        end
    end)
end
local function update_world_rank_info(uid,chip)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    local sql = "update urank set uchip = uchip + %d where uid = %d"
	local cmd_sql = string.format(sql,chip,uid)
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            Logger.debug(err)
        end
    end)
end


--insert info
local function insert_ugame_system_user(uid,uchip,uvip,uexp,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    
    local sql = "insert into ugame(`uid`, `uchip`, `uvip`, `uexp`)values(%d, %d, %d, %d)"
	local cmd_sql = string.format(sql, uid, uchip, uvip, uexp)

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            ret_handler(nil,nil)
            insert_world_rank_info(uid,uchip,uvip)
        end
    end)
end

local function get_bonues_info(uid,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    local sql = "select uid, bonues, status, bonues_time, days from login_bonues where uid = %d limit 1"
	local cmd_sql = string.format(sql, uid)	
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then
            if ret_handler ~= nil then
                ret_handler(err,nil)
            end
            return
        end
        --没有这条数据
        if ret == nil or #ret <= 0 then
            if ret_handler ~= nil then
                ret_handler(nil,nil)
            end
            return
        end
        --end
        local result = ret[1]
        local uinfo = {}
        uinfo.uid = tonumber(result[1])
        uinfo.bonues = tonumber(result[2])
        uinfo.status = tonumber(result[3])
        uinfo.bonues_time = tonumber(result[4])
        uinfo.days = tonumber(result[5])
        ret_handler(nil,uinfo)
    end)
end

local function insert_bonues_info(uid,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    
    local sql = "insert into login_bonues(`uid`, `bonues_time`,`status`)values( %d, %d,1)"
	local cmd_sql = string.format(sql, uid, TimeStamp.timestamp())

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            ret_handler(nil,nil)
        end
    end)
end

local function update_login_bonues(uid,bonues_info,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    
    local sql = "update login_bonues set status = 0,bonues = %d,bonues_time = %d,days = %d where uid = %d"
	local cmd_sql = string.format(sql, bonues_info.bonues,bonues_info.bonues_time,bonues_info.days,uid)

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            ret_handler(nil,nil)
        end
    end)
end

local function update_login_bonues_status(uid,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    
    local sql = "update login_bonues set status = 1 where uid = %d"
	local cmd_sql = string.format(sql,uid)

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            ret_handler(nil,nil)
        end
    end)
end

local function add_chip(uid,chip,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    
    local sql = "update ugame set uchip = uchip + %d where uid = %d"
	local cmd_sql = string.format(sql,chip,uid)
    update_world_rank_info(uid,chip)
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            if ret_handler then
                ret_handler(nil,nil)
            end
        end
    end)
end
--获取金币排行前rank_num的数据
local function get_world_rank_with_uchip(rank_num,ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    --DESC降序 ASC升序
    local sql = "select * from urank order by uchip DESC limit %d"
	local cmd_sql = string.format(sql,rank_num)

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
                return
            end
        end
        
        --end
        local i = 0
        local rank_data = {}
        for i = 1,#ret do 
            local result = ret[i]
            local rank_info = {}
            rank_info.uid = tonumber(result[2])
            rank_info.unick = tostring(result[3])
            rank_info.usex = tonumber(result[4])
            rank_info.uvip = tonumber(result[5])
            rank_info.uchip= tonumber(result[6])
            rank_info.uface= tonumber(result[7])
            table.insert(rank_data,rank_info)
        end
        ret_handler(nil,rank_data)
    end)
end

local function get_sys_msg_info(ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    local sql = "select * from sys_msg"
	local cmd_sql = sql

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
                return
            end
        end
        
        --end
        local k,v
        local sys_data = {}
        for k,v in ipairs(ret) do 
           sys_data[k] = v[2]
        end
        ret_handler(nil,sys_data)
    end)
end

local function get_robots_ugame_info(ret_handler)
    if _check_is_connected_sql(ret_handler) == nil then 
        return 
    end
    local sql = "select uid, uchip, uchip1, uchip2, uvip, uvip_endtime, udata1,udata2,udata3,uexp,ustatus from ugame where is_robot =1"
	local cmd_sql = sql
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then
            if ret_handler ~= nil then
                ret_handler(err,nil)
            end
            return
        end
        --没有这条数据
        if ret == nil or #ret <= 0 then
            if ret_handler ~= nil then
                ret_handler(nil,nil)
            end
            return
        end
        --end

        local k,v 
        local robots = {}
        for k,v in pairs(ret) do
            local result = v
            local one_robot = {}
            one_robot.uid = tonumber(result[1])
            one_robot.uchip = tonumber(result[2])
            one_robot.uchip1 = tonumber(result[3])
            one_robot.uchip2 = tonumber(result[4])
            one_robot.uvip = tonumber(result[5])
            one_robot.uvip_endtime = tonumber(result[6])
            one_robot.udata1 = tonumber(result[7])
            one_robot.udata2 = tonumber(result[8])
            one_robot.udata3 = tonumber(result[9])
            one_robot.uexp = tonumber(result[10])
            one_robot.ustatus = tonumber(result[11])
            table.insert(robots,one_robot)
        end
        ret_handler(nil,robots)
    end)
end

local mysql_game_tb = 
{
    is_connected = is_connected,
    get_ugame_system_uinfo = get_ugame_system_uinfo,
    insert_ugame_system_user = insert_ugame_system_user,
    get_bonues_info = get_bonues_info,
    insert_bonues_info = insert_bonues_info,
    update_login_bonues = update_login_bonues,
    update_login_bonues_status = update_login_bonues_status,
    add_chip = add_chip,
    --排行
    get_world_rank_with_uchip = get_world_rank_with_uchip,
    insert_world_rank_info = insert_world_rank_info,
    update_world_rank_info = update_world_rank_info,
    --系统消息
    get_sys_msg_info = get_sys_msg_info,
    --机器人
    get_robots_ugame_info = get_robots_ugame_info,
}

return mysql_game_tb