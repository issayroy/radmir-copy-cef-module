/*
	* Module: CefData
	* Author: Fernando_Black (vk.com/issayroy)
	*
	* CuatexDev (c) vk.com/cuatex_dev 2023
*/

#if !defined ID_CEF_PACKET
    #define ID_CEF_PACKET   215
#endif


#if !defined SendPacket_1
    #define SendPacket_1    SendClientEvent
#endif

#if !defined SendPacket_2
    #define SendPacket_2    SendClientEventWithArguments
#endif

#if !defined Dialog
    #define Dialog          ShowPlayerDialog
#endif

#if !defined SendClientEventF
    #define SendClientEventF(%1,%2,%3,%4) 		str_2[0] = EOS, format(str_2,sizeof str_2,%4), SendClientEvent(%1,%2,%3,str_2)
#endif

#if !defined SendClientEventWithArgumentsF
    #define SendClientEventWithArgumentsF(%1,%2,%3,%4,%5) 		str_2[0] = EOS, format(str_2,sizeof str_2,%5), SendClientEventWithArguments(%1,%2,%3,%4,str_2)
#endif

enum CefDataStyle 
{
	CefNone,
	Interface = 2,
 	Number = 32,
 	Tinting = 12,
 	TurnLights = 15,
 	Tuning = 17,
};

enum CefDataType 
{
	CefNones,
	CefUpdate = 2,
	CefShow = 4,
	CefHide,
	CefNumber = 6,
};

#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif

#define ShowPlayerDialog( client_ShowPlayerDialog(

stock client_ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[])
{
    static buffer[4096 + 1];

    for (new i = sizeof(buffer) - 1; i != -1; i--)
        buffer[i] = '\0';

    for (new i, j; info[i] != '\0'; i++) {
        if (info[i] == '\n')
            strcat(buffer, "<n>"), j += 3;
        else if (info[i] == '\t')
            strcat(buffer, "<t>"), j += 3;
        else if (info[i] == '\"')
            buffer[j++] = '\'';
        else
            buffer[j++] = info[i];
    }

    SendClientEventF(playerid, Interface, CefUpdate, "window.addDialogInQueue('[%i,%i,\"%s\",\"\",\"%s\",\"%s\",0,0]', \"%s\", 0)", dialogid, style, caption, button1, button2, buffer);

    return 1;
}


#if defined _ALS_GameTextForPlayer
    #undef GameTextForPlayer
#else
    #define _ALS_GameTextForPlayer
#endif

#define GameTextForPlayer( client_GameTextForPlayer(

stock client_GameTextForPlayer(playerid, const string[], time, style)
{
	SendClientEventF(playerid, Interface, CefUpdate, "interface('GameText').add('[%d,\"%s\",%d,0,-1,1]')", style, string, time);
 	SendClientEvent(playerid, Interface, CefShow, "GameText");

    return 1;
}


stock SendClientEvent(playerid, CefDataStyle:style, CefDataType:type, string[])
{
    new
        BitStream:bsn = BS_New();

    BS_WriteValue(bsn, PR_UINT8, ID_CEF_PACKET);
    BS_WriteValue(bsn, PR_UINT16, _:style);
	BS_WriteValue(bsn, PR_UINT32, _:type);
	BS_WriteValue(bsn, PR_UINT8, 1);
	BS_WriteValue(
		bsn,
		PR_UINT32, strlen(string),
		PR_STRING, string, strlen(string)
	);

    PR_SendPacket(bsn, playerid);
    BS_Delete(bsn);
	return true;
}

stock SendClientEventWithArguments(playerid, CefDataStyle:style, CefDataType:type, string[], string1[])
{
    new
        BitStream:bsn = BS_New();

    BS_WriteValue(bsn, PR_UINT8, ID_CEF_PACKET);
    BS_WriteValue(bsn, PR_UINT16, _:style);
	BS_WriteValue(bsn, PR_UINT32, _:type);
	BS_WriteValue(bsn, PR_UINT8, 2);
	BS_WriteValue(
		bsn,
		PR_UINT32, strlen(string),
		PR_STRING, string, strlen(string)
	);
	BS_WriteValue(
		bsn,
		PR_UINT32, strlen(string1),
		PR_STRING, string1, strlen(string1)
	);

    PR_SendPacket(bsn, playerid);
    BS_Delete(bsn);
}
forward OnInterfaceEvent(playerid, const event[], bs);

public OnInterfaceEvent(playerid, const event[], bs)
{
    #if defined FILTERSCRIPT
        CallRemoteFunction("fs_OnInterfaceEvent", "isd", playerid, event, bs);
    #else
        // код для реализации евента
    #endif
	return 1;
}

IPacket:215(playerid, BitStream:bs)
{
    new bytes;

    BS_GetNumberOfBytesUsed(bs, bytes);

    if (bytes < 6) {
    return 0;
    }

    new
    style, types;

    BS_IgnoreBits(bs, 8);
    BS_ReadUint16(bs, style);
    BS_ReadUint32(bs, types);

    if (style == _:Interface)
    {
        new
            call[64 + 1];

        BS_ReadString32(bs, call);

        OnInterfaceEvent(playerid, call, _:bs);
    }

    return 1;
}