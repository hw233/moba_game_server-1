﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class ugames : Singletom<ugames>
{
    public string unick = "";
    public int sex = 0;
    public int usex = 0;
    public int uface = 1;
    public int face = 1;
    public int vip = 0;
    public bool is_guest = false;
    public int zid = -1;
    public int matchid = -1;
    public int self_seatid = -1;
    public int self_side = -1;
    public UserGameInfo ugameInfo;
    public List<UserArrived> other_users = new List<UserArrived>();
    public List<PlayerMatchInfo> player_math_info = null;//当前正在比赛的玩家信息

    public void save_uinfo(UserCenterInfo info, bool is_guest)
    {

        this.unick = info.unick;
        this.sex = info.sex;
        this.usex = info.sex;
        this.uface = info.face;
        this.face = info.face;
        this.vip = info.uvip;
        this.is_guest = is_guest;
        event_manager.Instance.dispatch_event(event_manager.EVT_SET_MAIN_USER_INFO, info);
    }
    public void save_ugame_info(UserGameInfo info)
    {
        this.ugameInfo = info;
        event_manager.Instance.dispatch_event(event_manager.EVT_UPDATE_UGAME_INFO, info);
    }
    public void set_zid(int zid)
    {
        this.zid = zid;
    }
    // Update is called once per frame
    void Update()
    {
        
    }
}
