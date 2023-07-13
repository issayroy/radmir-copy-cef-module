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

CMD:test_cef(playerid, params[])
{
    extract params -> new string:param[20];

    //SendClientEventWithArgumentsF(playerid, Interface, CefShow, "Auth", "[\"auth\", \"%s\", 1, 1, 0, 0, 1]", param);
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