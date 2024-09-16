// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: network_connection.proto

#include "network_connection.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

static const ::_pb::EnumDescriptor* file_level_enum_descriptors_network_5fconnection_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_network_5fconnection_2eproto = nullptr;
const uint32_t TableStruct_network_5fconnection_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_network_5fconnection_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\030network_connection.proto\032 google/proto"
  "buf/descriptor.proto*\340K\n\033ENetworkDisconn"
  "ectionReason\022\036\n\032NETWORK_DISCONNECT_INVAL"
  "ID\020\000\022\037\n\033NETWORK_DISCONNECT_SHUTDOWN\020\001\022F\n"
  "%NETWORK_DISCONNECT_DISCONNECT_BY_USER\020\002"
  "\032\033\242\324\030\027#GameUI_Disconnect_User\022J\n\'NETWORK"
  "_DISCONNECT_DISCONNECT_BY_SERVER\020\003\032\035\242\324\030\031"
  "#GameUI_Disconnect_Server\022B\n\027NETWORK_DIS"
  "CONNECT_LOST\020\004\032%\242\324\030!#GameUI_Disconnect_C"
  "onnectionLost\022J\n\033NETWORK_DISCONNECT_OVER"
  "FLOW\020\005\032)\242\324\030%#GameUI_Disconnect_Connectio"
  "nOverflow\022I\n\037NETWORK_DISCONNECT_STEAM_BA"
  "NNED\020\006\032$\242\324\030 #GameUI_Disconnect_SteamIDBa"
  "nned\022G\n\036NETWORK_DISCONNECT_STEAM_INUSE\020\007"
  "\032#\242\324\030\037#GameUI_Disconnect_SteamIDInUse\022G\n"
  "\037NETWORK_DISCONNECT_STEAM_TICKET\020\010\032\"\242\324\030\036"
  "#GameUI_Disconnect_SteamTicket\022E\n\036NETWOR"
  "K_DISCONNECT_STEAM_LOGON\020\t\032!\242\324\030\035#GameUI_"
  "Disconnect_SteamLogon\022M\n&NETWORK_DISCONN"
  "ECT_STEAM_AUTHCANCELLED\020\n\032!\242\324\030\035#GameUI_D"
  "isconnect_SteamLogon\022O\n(NETWORK_DISCONNE"
  "CT_STEAM_AUTHALREADYUSED\020\013\032!\242\324\030\035#GameUI_"
  "Disconnect_SteamLogon\022K\n$NETWORK_DISCONN"
  "ECT_STEAM_AUTHINVALID\020\014\032!\242\324\030\035#GameUI_Dis"
  "connect_SteamLogon\022I\n$NETWORK_DISCONNECT"
  "_STEAM_VACBANSTATE\020\r\032\037\242\324\030\033#GameUI_Discon"
  "nect_SteamVAC\022S\n,NETWORK_DISCONNECT_STEA"
  "M_LOGGED_IN_ELSEWHERE\020\016\032!\242\324\030\035#GameUI_Dis"
  "connect_SteamInUse\022T\n+NETWORK_DISCONNECT"
  "_STEAM_VAC_CHECK_TIMEDOUT\020\017\032#\242\324\030\037#GameUI"
  "_Disconnect_SteamTimeOut\022I\n NETWORK_DISC"
  "ONNECT_STEAM_DROPPED\020\020\032#\242\324\030\037#GameUI_Disc"
  "onnect_SteamDropped\022M\n\"NETWORK_DISCONNEC"
  "T_STEAM_OWNERSHIP\020\021\032%\242\324\030!#GameUI_Disconn"
  "ect_SteamOwnership\022U\n&NETWORK_DISCONNECT"
  "_SERVERINFO_OVERFLOW\020\022\032)\242\324\030%#GameUI_Disc"
  "onnect_ServerInfoOverflow\022K\n#NETWORK_DIS"
  "CONNECT_TICKMSG_OVERFLOW\020\023\032\"\242\324\030\036#GameUI_"
  "Disconnect_TickMessage\022Y\n*NETWORK_DISCON"
  "NECT_STRINGTABLEMSG_OVERFLOW\020\024\032)\242\324\030%#Gam"
  "eUI_Disconnect_StringTableMessage\022S\n\'NET"
  "WORK_DISCONNECT_DELTAENTMSG_OVERFLOW\020\025\032&"
  "\242\324\030\"#GameUI_Disconnect_DeltaEntMessage\022Q"
  "\n&NETWORK_DISCONNECT_TEMPENTMSG_OVERFLOW"
  "\020\026\032%\242\324\030!#GameUI_Disconnect_TempEntMessag"
  "e\022O\n%NETWORK_DISCONNECT_SOUNDSMSG_OVERFL"
  "OW\020\027\032$\242\324\030 #GameUI_Disconnect_SoundsMessa"
  "ge\022P\n#NETWORK_DISCONNECT_SNAPSHOTOVERFLO"
  "W\020\030\032\'\242\324\030##GameUI_Disconnect_SnapshotOver"
  "flow\022J\n NETWORK_DISCONNECT_SNAPSHOTERROR"
  "\020\031\032$\242\324\030 #GameUI_Disconnect_SnapshotError"
  "\022P\n#NETWORK_DISCONNECT_RELIABLEOVERFLOW\020"
  "\032\032\'\242\324\030##GameUI_Disconnect_ReliableOverfl"
  "ow\022N\n\037NETWORK_DISCONNECT_BADDELTATICK\020\033\032"
  ")\242\324\030%#GameUI_Disconnect_BadClientDeltaTi"
  "ck\022H\n\037NETWORK_DISCONNECT_NOMORESPLITS\020\034\032"
  "#\242\324\030\037#GameUI_Disconnect_NoMoreSplits\022@\n\033"
  "NETWORK_DISCONNECT_TIMEDOUT\020\035\032\037\242\324\030\033#Game"
  "UI_Disconnect_TimedOut\022H\n\037NETWORK_DISCON"
  "NECT_DISCONNECTED\020\036\032#\242\324\030\037#GameUI_Disconn"
  "ect_Disconnected\022H\n\037NETWORK_DISCONNECT_L"
  "EAVINGSPLIT\020\037\032#\242\324\030\037#GameUI_Disconnect_Le"
  "avingSplit\022X\n\'NETWORK_DISCONNECT_DIFFERE"
  "NTCLASSTABLES\020 \032+\242\324\030\'#GameUI_Disconnect_"
  "DifferentClassTables\022P\n#NETWORK_DISCONNE"
  "CT_BADRELAYPASSWORD\020!\032\'\242\324\030##GameUI_Disco"
  "nnect_BadRelayPassword\022X\n\'NETWORK_DISCON"
  "NECT_BADSPECTATORPASSWORD\020\"\032+\242\324\030\'#GameUI"
  "_Disconnect_BadSpectatorPassword\022L\n!NETW"
  "ORK_DISCONNECT_HLTVRESTRICTED\020#\032%\242\324\030!#Ga"
  "meUI_Disconnect_HLTVRestricted\022H\n\037NETWOR"
  "K_DISCONNECT_NOSPECTATORS\020$\032#\242\324\030\037#GameUI"
  "_Disconnect_NoSpectators\022N\n\"NETWORK_DISC"
  "ONNECT_HLTVUNAVAILABLE\020%\032&\242\324\030\"#GameUI_Di"
  "sconnect_HLTVUnavailable\022@\n\033NETWORK_DISC"
  "ONNECT_HLTVSTOP\020&\032\037\242\324\030\033#GameUI_Disconnec"
  "t_HLTVStop\022<\n\031NETWORK_DISCONNECT_KICKED\020"
  "\'\032\035\242\324\030\031#GameUI_Disconnect_Kicked\022@\n\033NETW"
  "ORK_DISCONNECT_BANADDED\020(\032\037\242\324\030\033#GameUI_D"
  "isconnect_BanAdded\022H\n\037NETWORK_DISCONNECT"
  "_KICKBANADDED\020)\032#\242\324\030\037#GameUI_Disconnect_"
  "KickBanAdded\022D\n\035NETWORK_DISCONNECT_HLTVD"
  "IRECT\020*\032!\242\324\030\035#GameUI_Disconnect_HLTVDire"
  "ct\022\\\n)NETWORK_DISCONNECT_PURESERVER_CLIE"
  "NTEXTRA\020+\032-\242\324\030)#GameUI_Disconnect_PureSe"
  "rver_ClientExtra\022V\n&NETWORK_DISCONNECT_P"
  "URESERVER_MISMATCH\020,\032*\242\324\030&#GameUI_Discon"
  "nect_PureServer_Mismatch\022>\n\032NETWORK_DISC"
  "ONNECT_USERCMD\020-\032\036\242\324\030\032#GameUI_Disconnect"
  "_UserCmd\022N\n#NETWORK_DISCONNECT_REJECTED_"
  "BY_GAME\020.\032%\242\324\030!#GameUI_Disconnect_Reject"
  "edByGame\022T\n&NETWORK_DISCONNECT_MESSAGE_P"
  "ARSE_ERROR\020/\032(\242\324\030$#GameUI_Disconnect_Mes"
  "sageParseError\022X\n(NETWORK_DISCONNECT_INV"
  "ALID_MESSAGE_ERROR\0200\032*\242\324\030&#GameUI_Discon"
  "nect_InvalidMessageError\022T\n&NETWORK_DISC"
  "ONNECT_BAD_SERVER_PASSWORD\0201\032(\242\324\030$#GameU"
  "I_Disconnect_BadServerPassword\0221\n-NETWOR"
  "K_DISCONNECT_DIRECT_CONNECT_RESERVATION\020"
  "2\022S\n%NETWORK_DISCONNECT_CONNECTION_FAILU"
  "RE\0203\032(\242\324\030$#GameUI_Disconnect_ConnectionF"
  "ailure\022Y\n)NETWORK_DISCONNECT_NO_PEER_GRO"
  "UP_HANDLERS\0204\032*\242\324\030&#GameUI_Disconnect_No"
  "PeerGroupHandlers\022#\n\037NETWORK_DISCONNECT_"
  "RECONNECTION\0205\022H\n\037NETWORK_DISCONNECT_LOO"
  "PSHUTDOWN\0206\032#\242\324\030\037#GameUI_Disconnect_Loop"
  "Shutdown\022L\n!NETWORK_DISCONNECT_LOOPDEACT"
  "IVATE\0207\032%\242\324\030!#GameUI_Disconnect_LoopDeac"
  "tivate\022H\n\037NETWORK_DISCONNECT_HOST_ENDGAM"
  "E\0208\032#\242\324\030\037#GameUI_Disconnect_Host_EndGame"
  "\022\\\n*NETWORK_DISCONNECT_LOOP_LEVELLOAD_AC"
  "TIVATE\0209\032,\242\324\030(#GameUI_Disconnect_LoopLev"
  "elLoadActivate\022V\n\'NETWORK_DISCONNECT_CRE"
  "ATE_SERVER_FAILED\020:\032)\242\324\030%#GameUI_Disconn"
  "ect_CreateServerFailed\022D\n\032NETWORK_DISCON"
  "NECT_EXITING\020;\032$\242\324\030 #GameUI_Disconnect_E"
  "xitingEngine\022T\n)NETWORK_DISCONNECT_REQUE"
  "ST_HOSTSTATE_IDLE\020<\032%\242\324\030!#GameUI_Disconn"
  "ect_Request_HSIdle\022\\\n.NETWORK_DISCONNECT"
  "_REQUEST_HOSTSTATE_HLTVRELAY\020=\032(\242\324\030$#Gam"
  "eUI_Disconnect_Request_HLTVRelay\022Q\n*NETW"
  "ORK_DISCONNECT_CLIENT_CONSISTENCY_FAIL\020>"
  "\032!\242\324\030\035#GameUI_ClientConsistencyFail\022Q\n+N"
  "ETWORK_DISCONNECT_CLIENT_UNABLE_TO_CRC_M"
  "AP\020\?\032 \242\324\030\034#GameUI_ClientUnableToCRCMap\022="
  "\n NETWORK_DISCONNECT_CLIENT_NO_MAP\020@\032\027\242\324"
  "\030\023#GameUI_ClientNoMap\022K\n\'NETWORK_DISCONN"
  "ECT_CLIENT_DIFFERENT_MAP\020A\032\036\242\324\030\032#GameUI_"
  "ClientDifferentMap\022M\n(NETWORK_DISCONNECT"
  "_SERVER_REQUIRES_STEAM\020B\032\037\242\324\030\033#GameUI_Se"
  "rverRequireSteams\022M\n\"NETWORK_DISCONNECT_"
  "STEAM_DENY_MISC\020C\032%\242\324\030!#GameUI_Disconnec"
  "t_SteamDeny_Misc\022_\n,NETWORK_DISCONNECT_S"
  "TEAM_DENY_BAD_ANTI_CHEAT\020D\032-\242\324\030)#GameUI_"
  "Disconnect_SteamDeny_BadAntiCheat\022M\n\"NET"
  "WORK_DISCONNECT_SERVER_SHUTDOWN\020E\032%\242\324\030!#"
  "GameUI_Disconnect_ServerShutdown\022U\n&NETW"
  "ORK_DISCONNECT_REPLAY_INCOMPATIBLE\020G\032)\242\324"
  "\030%#GameUI_Disconnect_ReplayIncompatible\022"
  "Z\n+NETWORK_DISCONNECT_CONNECT_REQUEST_TI"
  "MEDOUT\020H\032)\242\324\030%#GameUI_Disconnect_Connect"
  "ionTimedout\022U\n&NETWORK_DISCONNECT_SERVER"
  "_INCOMPATIBLE\020I\032)\242\324\030%#GameUI_Disconnect_"
  "ServerIncompatible\022^\n*NETWORK_DISCONNECT"
  "_LOCALPROBLEM_MANYRELAYS\020J\032.\242\324\030*#GameUI_"
  "Disconnect_LocalProblem_ManyRelays\022z\n8NE"
  "TWORK_DISCONNECT_LOCALPROBLEM_HOSTEDSERV"
  "ERPRIMARYRELAY\020K\032<\242\324\0308#GameUI_Disconnect"
  "_LocalProblem_HostedServerPrimaryRelay\022d"
  "\n-NETWORK_DISCONNECT_LOCALPROBLEM_NETWOR"
  "KCONFIG\020L\0321\242\324\030-#GameUI_Disconnect_LocalP"
  "roblem_NetworkConfig\022T\n%NETWORK_DISCONNE"
  "CT_LOCALPROBLEM_OTHER\020M\032)\242\324\030%#GameUI_Dis"
  "connect_LocalProblem_Other\022S\n!NETWORK_DI"
  "SCONNECT_REMOTE_TIMEOUT\020O\032,\242\324\030(#GameUI_D"
  "isconnect_RemoteProblem_Timeout\022h\n,NETWO"
  "RK_DISCONNECT_REMOTE_TIMEOUT_CONNECTING\020"
  "P\0326\242\324\0302#GameUI_Disconnect_RemoteProblem_"
  "TimeoutConnecting\022O\n\037NETWORK_DISCONNECT_"
  "REMOTE_OTHER\020Q\032*\242\324\030&#GameUI_Disconnect_R"
  "emoteProblem_Other\022U\n\"NETWORK_DISCONNECT"
  "_REMOTE_BADCRYPT\020R\032-\242\324\030)#GameUI_Disconne"
  "ct_RemoteProblem_BadCrypt\022Z\n(NETWORK_DIS"
  "CONNECT_REMOTE_CERTNOTTRUSTED\020S\032,\242\324\030(#Ga"
  "meUI_Disconnect_RemoteProblem_BadCert\022>\n"
  "\032NETWORK_DISCONNECT_UNUSUAL\020T\032\036\242\324\030\032#Game"
  "UI_Disconnect_Unusual\022K\n!NETWORK_DISCONN"
  "ECT_INTERNAL_ERROR\020U\032$\242\324\030 #GameUI_Discon"
  "nect_InternalError\022Q\n&NETWORK_DISCONNECT"
  "_REJECT_BADCHALLENGE\020\200\001\032$\242\324\030 #GameUI_Ser"
  "verRejectBadChallenge\022A\n!NETWORK_DISCONN"
  "ECT_REJECT_NOLOBBY\020\201\001\032\031\242\324\030\025#GameUI_Serve"
  "rNoLobby\022O\n(NETWORK_DISCONNECT_REJECT_BA"
  "CKGROUND_MAP\020\202\001\032 \242\324\030\034#Valve_Reject_Backg"
  "round_Map\022M\n\'NETWORK_DISCONNECT_REJECT_S"
  "INGLE_PLAYER\020\203\001\032\037\242\324\030\033#Valve_Reject_Singl"
  "e_Player\022I\n%NETWORK_DISCONNECT_REJECT_HI"
  "DDEN_GAME\020\204\001\032\035\242\324\030\031#Valve_Reject_Hidden_G"
  "ame\022O\n%NETWORK_DISCONNECT_REJECT_LANREST"
  "RICT\020\205\001\032#\242\324\030\037#GameUI_ServerRejectLANRest"
  "rict\022O\n%NETWORK_DISCONNECT_REJECT_BADPAS"
  "SWORD\020\206\001\032#\242\324\030\037#GameUI_ServerRejectBadPas"
  "sword\022M\n$NETWORK_DISCONNECT_REJECT_SERVE"
  "RFULL\020\207\001\032\"\242\324\030\036#GameUI_ServerRejectServer"
  "Full\022]\n,NETWORK_DISCONNECT_REJECT_INVALI"
  "DRESERVATION\020\210\001\032*\242\324\030&#GameUI_ServerRejec"
  "tInvalidReservation\022S\n\'NETWORK_DISCONNEC"
  "T_REJECT_FAILEDCHANNEL\020\211\001\032%\242\324\030!#GameUI_S"
  "erverRejectFailedChannel\022W\n,NETWORK_DISC"
  "ONNECT_REJECT_CONNECT_FROM_LOBBY\020\212\001\032$\242\324\030"
  " #Valve_Reject_Connect_From_Lobby\022W\n,NET"
  "WORK_DISCONNECT_REJECT_RESERVED_FOR_LOBB"
  "Y\020\213\001\032$\242\324\030 #Valve_Reject_Reserved_For_Lob"
  "by\022Z\n*NETWORK_DISCONNECT_REJECT_INVALIDK"
  "EYLENGTH\020\214\001\032)\242\324\030%#GameUI_ServerReject_In"
  "validKeyLength\022O\n%NETWORK_DISCONNECT_REJ"
  "ECT_OLDPROTOCOL\020\215\001\032#\242\324\030\037#GameUI_ServerRe"
  "jectOldProtocol\022O\n%NETWORK_DISCONNECT_RE"
  "JECT_NEWPROTOCOL\020\216\001\032#\242\324\030\037#GameUI_ServerR"
  "ejectNewProtocol\022[\n+NETWORK_DISCONNECT_R"
  "EJECT_INVALIDCONNECTION\020\217\001\032)\242\324\030%#GameUI_"
  "ServerRejectInvalidConnection\022U\n(NETWORK"
  "_DISCONNECT_REJECT_INVALIDCERTLEN\020\220\001\032&\242\324"
  "\030\"#GameUI_ServerRejectInvalidCertLen\022_\n-"
  "NETWORK_DISCONNECT_REJECT_INVALIDSTEAMCE"
  "RTLEN\020\221\001\032+\242\324\030\'#GameUI_ServerRejectInvali"
  "dSteamCertLen\022C\n\037NETWORK_DISCONNECT_REJE"
  "CT_STEAM\020\222\001\032\035\242\324\030\031#GameUI_ServerRejectSte"
  "am\022Q\n,NETWORK_DISCONNECT_REJECT_SERVERAU"
  "THDISABLED\020\223\001\032\036\242\324\030\032#GameUI_ServerAuthDis"
  "abled\022Y\n0NETWORK_DISCONNECT_REJECT_SERVE"
  "RCDKEYAUTHINVALID\020\224\001\032\"\242\324\030\036#GameUI_Server"
  "CDKeyAuthInvalid\022E\n NETWORK_DISCONNECT_R"
  "EJECT_BANNED\020\225\001\032\036\242\324\030\032#GameUI_ServerRejec"
  "tBanned\022T\n%NETWORK_DISCONNECT_KICKED_TEA"
  "MKILLING\020\226\001\032(\242\324\030$#Player_DisconnectReaso"
  "n_TeamKilling\022N\n\"NETWORK_DISCONNECT_KICK"
  "ED_TK_START\020\227\001\032%\242\324\030!#Player_DisconnectRe"
  "ason_TK_Start\022^\n*NETWORK_DISCONNECT_KICK"
  "ED_UNTRUSTEDACCOUNT\020\230\001\032-\242\324\030)#Player_Disc"
  "onnectReason_UntrustedAccount\022^\n*NETWORK"
  "_DISCONNECT_KICKED_CONVICTEDACCOUNT\020\231\001\032-"
  "\242\324\030)#Player_DisconnectReason_ConvictedAc"
  "count\022d\n-NETWORK_DISCONNECT_KICKED_COMPE"
  "TITIVECOOLDOWN\020\232\001\0320\242\324\030,#Player_Disconnec"
  "tReason_CompetitiveCooldown\022T\n%NETWORK_D"
  "ISCONNECT_KICKED_TEAMHURTING\020\233\001\032(\242\324\030$#Pl"
  "ayer_DisconnectReason_TeamHurting\022Z\n(NET"
  "WORK_DISCONNECT_KICKED_HOSTAGEKILLING\020\234\001"
  "\032+\242\324\030\'#Player_DisconnectReason_HostageKi"
  "lling\022N\n\"NETWORK_DISCONNECT_KICKED_VOTED"
  "OFF\020\235\001\032%\242\324\030!#Player_DisconnectReason_Vot"
  "edOff\022F\n\036NETWORK_DISCONNECT_KICKED_IDLE\020"
  "\236\001\032!\242\324\030\035#Player_DisconnectReason_Idle\022L\n"
  "!NETWORK_DISCONNECT_KICKED_SUICIDE\020\237\001\032$\242"
  "\324\030 #Player_DisconnectReason_Suicide\022V\n&N"
  "ETWORK_DISCONNECT_KICKED_NOSTEAMLOGIN\020\240\001"
  "\032)\242\324\030%#Player_DisconnectReason_NoSteamLo"
  "gin\022X\n\'NETWORK_DISCONNECT_KICKED_NOSTEAM"
  "TICKET\020\241\001\032*\242\324\030&#Player_DisconnectReason_"
  "NoSteamTicket\022\217\001\n)NETWORK_DISCONNECT_KIC"
  "KED_INPUTAUTOMATION\020\242\001\032_\242\324\030(#Player_Disc"
  "onnectReason_InputAutomation\252\324\030/#Player_"
  "DisconnectReason_InputAutomation_Detail:"
  "E\n\030network_connection_token\022!.google.pro"
  "tobuf.EnumValueOptions\030\304\212\003 \001(\t:L\n\037networ"
  "k_connection_detail_token\022!.google.proto"
  "buf.EnumValueOptions\030\305\212\003 \001(\t"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_network_5fconnection_2eproto_deps[1] = {
  &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
};
static ::_pbi::once_flag descriptor_table_network_5fconnection_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_network_5fconnection_2eproto = {
    false, false, 9908, descriptor_table_protodef_network_5fconnection_2eproto,
    "network_connection.proto",
    &descriptor_table_network_5fconnection_2eproto_once, descriptor_table_network_5fconnection_2eproto_deps, 1, 0,
    schemas, file_default_instances, TableStruct_network_5fconnection_2eproto::offsets,
    nullptr, file_level_enum_descriptors_network_5fconnection_2eproto,
    file_level_service_descriptors_network_5fconnection_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_network_5fconnection_2eproto_getter() {
  return &descriptor_table_network_5fconnection_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_network_5fconnection_2eproto(&descriptor_table_network_5fconnection_2eproto);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ENetworkDisconnectionReason_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_network_5fconnection_2eproto);
  return file_level_enum_descriptors_network_5fconnection_2eproto[0];
}
bool ENetworkDisconnectionReason_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
    case 69:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 85:
    case 128:
    case 129:
    case 130:
    case 131:
    case 132:
    case 133:
    case 134:
    case 135:
    case 136:
    case 137:
    case 138:
    case 139:
    case 140:
    case 141:
    case 142:
    case 143:
    case 144:
    case 145:
    case 146:
    case 147:
    case 148:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
    case 159:
    case 160:
    case 161:
    case 162:
      return true;
    default:
      return false;
  }
}

const std::string network_connection_token_default("");
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::EnumValueOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::StringTypeTraits, 9, false>
  network_connection_token(kNetworkConnectionTokenFieldNumber, network_connection_token_default, nullptr);
const std::string network_connection_detail_token_default("");
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::EnumValueOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::StringTypeTraits, 9, false>
  network_connection_detail_token(kNetworkConnectionDetailTokenFieldNumber, network_connection_detail_token_default, nullptr);

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
