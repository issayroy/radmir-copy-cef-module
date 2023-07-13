@___If_u_can_read_this_u_r_nerd();		// 10 different ways to crash DeAMX
@___If_u_can_read_this_u_r_nerd()		// and also a nice tag for exported functions table in the AMX file
{
    #emit	stack	0x7FFFFFFF	// wtf (1) (stack over... overf*ck!?)
    #emit	inc.s	cellmax		// wtf (2) (this one should probably make DeAMX allocate all available memory and lag forever)
    static const ___[][] = {"pro-pawn", ".ru"};	// pretty old anti-deamx trick
    #emit	retn
    #emit	load.s.pri	___		// wtf (3) (opcode outside of function?)
    #emit	proc				// wtf (4) (if DeAMX hasn't crashed already, it would think it is a new function)
    #emit	proc				// wtf (5) (a function inside of another function!?)
    #emit	fill		cellmax	// wtf (6) (fill random memory block with 0xFFFFFFFF)
    #emit	proc
    #emit	stack		1		// wtf (7) (compiler usually allocates 4 bytes or 4*N for arrays of N elements)
    #emit	stor.alt	___		// wtf (8) (...)
    #emit	strb.i		2    	// wtf (9)
    #emit	switch		4
    #emit	retn    			// wtf (10) (no "casetbl" opcodes before retn - invalid switch statement?)
L1:
    #emit	jump	L1			// avoid compiler crash from "#emit switch"
    #emit	zero	cellmin		// wtf (11) (nonexistent address)
}

#define FILTERSCRIPT
#include <a_samp>
#include <foreach>
#include <pawncmd>
#include <streamer>
#include <sscanf2>
#include <Pawn.RakNet>
#include "../gamemodes/modules/cef_data.inc"

public OnFilterScriptInit()
{
	printf("cuatex security was loaded. Author: Fernando_Black (vk.com/issayroy)");

	return 1;
}


public OnFilterScriptExit()
{
	printf("cuatex security unload");
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

forward FS_OnPlayerKey(playerid, key);
public FS_OnPlayerKey(playerid, key)
{
	SCMF(playerid, -1, "[debug] FS_OnPlayerKey -> key: %d", key);

	return 1;
}
forward fs_OnInterfaceEvent(playerid, const event[], BitStream:bs);
public fs_OnInterfaceEvent(playerid, const event[], BitStream:bs)
{
	if(strcmp(event, "OnPlayerClientSideKey") == 0)
	{
		new key;
		BS_IgnoreBits(bs, 40);
		BS_ReadUint32(bs, key);

		#if defined FS_OnPlayerKey
			FS_OnPlayerKey(playerid, key);
		#endif
	}
	return 1;
}

