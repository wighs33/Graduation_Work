#pragma once
#ifndef __ENUM_H__
#define __ENUM_H__

#pragma warning( disable : 26812 ) 

enum EVENT_TYPE { CL_BONEFIRE, CL_BONEOUT, CL_MATCH, CL_END_MATCH, SP_DROP, GAME_OVER };
enum COMMAND {
	OP_RECV, OP_SEND, OP_ACCEPT, OP_NPC_MOVE, OP_NPC_ATTACK, OP_PLAYER_MOVE,
	OP_PLAYER_ATTACK, OP_PLAYER_RE, OP_PLAYER_HEAL, OP_PLAYER_DAMAGE, OP_OBJ_SPAWN, OP_SERVER_RECV, OP_SERVER_SEND, OP_SERVER_RECYCLE
};
enum CL_STATE { ST_FREE, ST_ACCEPT, ST_INGAME, ST_SERVER }; //  접속 상태
enum SERVER_STATE { SERVER_FREE, SERVER_MATHCING, SERVER_USING }; //서버 상태
enum STATE { ST_SNOWMAN, ST_INBURN, ST_OUTBURN, ST_ANIMAL, ST_TORNADO }; //인게임 상태

enum SNOW { SNOW_CREATE, SNOW_DESTORY };
enum ATTACK { HAND, GUN };
enum HEAL { BONFIRE, MATCH };

enum LEVEL { LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_END };
enum TYPE { TYPE_SWORD, TYPE_ARMOR, TYPE_END };
enum ITEM_STATE { STATE_EQUIP, STATE_UNEQUIP, STATE_END };

enum ITEM { ITEM_MAT, ITEM_UMB, ITEM_BAG, ITEM_SNOW, ITEM_JET, ITEM_ICE, ITEM_SPBOX };
enum OBJ_Type { PLAYER, ITEM_BOX, TONARDO, SUPPLYBOX };
enum Login_fail_Type { OVERLAP_ID, WORNG_ID, WORNG_PW, OVERLAP_AC, CREATE_AC };

enum BULLET_Type
{
	BULLET_SNOWBALL = 0,
	BULLET_ICEBALL,
	BULLET_SNOWBOMB
};



enum TELEPORT_Type
{
	TEL_FIRE = 1,
	TEL_BRIDGE,
	TEL_TOWER,
	TEL_ICE
};

enum CHEAT_Type
{
	CHEAT_HP_UP = 1,
	CHEAT_HP_DOWN,
	CHEAT_SNOW_PLUS,
	CHEAT_ICE_PLUS
};

enum CauseOfDeath {
	DeathBySnowball, DeathBySnowballBomb, DeathByCold, DeathBySnowman
};

enum KillLogType {
	None, Attacker, Victim
};

enum BODDYPARTS_Type
{
	BODDY_HEAD = 0,
	BODDY_LEFTHAND,
	BODDY_RIGHTHAND,
	BODDY_LEFTLEG,
	BODDY_RIGHTLEG,
	BODDY_CENTER
};

#endif // !__ENUM_H__
