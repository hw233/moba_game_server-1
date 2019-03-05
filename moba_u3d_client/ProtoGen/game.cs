//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: game.proto
namespace gprotocol
{
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"LoginRes")]
  public partial class LoginRes : global::ProtoBuf.IExtensible
  {
    public LoginRes() {}
    
    private int _status;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"status", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int status
    {
      get { return _status; }
      set { _status = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ExitRes")]
  public partial class ExitRes : global::ProtoBuf.IExtensible
  {
    public ExitRes() {}
    
    private int _status;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"status", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int status
    {
      get { return _status; }
      set { _status = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"SendMsgReq")]
  public partial class SendMsgReq : global::ProtoBuf.IExtensible
  {
    public SendMsgReq() {}
    
    private string _content;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"content", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string content
    {
      get { return _content; }
      set { _content = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"SendMsgRes")]
  public partial class SendMsgRes : global::ProtoBuf.IExtensible
  {
    public SendMsgRes() {}
    
    private string _status;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"status", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string status
    {
      get { return _status; }
      set { _status = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"OnUserLogin")]
  public partial class OnUserLogin : global::ProtoBuf.IExtensible
  {
    public OnUserLogin() {}
    
    private string _ip;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"ip", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string ip
    {
      get { return _ip; }
      set { _ip = value; }
    }
    private int _port;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"port", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int port
    {
      get { return _port; }
      set { _port = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"OnSendMsg")]
  public partial class OnSendMsg : global::ProtoBuf.IExtensible
  {
    public OnSendMsg() {}
    
    private string _ip;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"ip", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string ip
    {
      get { return _ip; }
      set { _ip = value; }
    }
    private int _port;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"port", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int port
    {
      get { return _port; }
      set { _port = value; }
    }
    private string _content;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"content", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string content
    {
      get { return _content; }
      set { _content = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
    [global::ProtoBuf.ProtoContract(Name=@"Cmd")]
    public enum Cmd
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"INVALID_CMD", Value=0)]
      INVALID_CMD = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eLoginReq", Value=1)]
      eLoginReq = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eLoginRes", Value=2)]
      eLoginRes = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eExitReq", Value=3)]
      eExitReq = 3,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eExitRes", Value=4)]
      eExitRes = 4,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eSendMsgReq", Value=5)]
      eSendMsgReq = 5,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eSendMsgRes", Value=6)]
      eSendMsgRes = 6,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eOnUserLogin", Value=7)]
      eOnUserLogin = 7,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eOnUserExit", Value=8)]
      eOnUserExit = 8,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eOnSendMsg", Value=9)]
      eOnSendMsg = 9
    }
  
}