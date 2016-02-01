instance CITY_2001_GOD(Npc_Default)
{
	name[0] = "Год";
	//name[0] = "Цуко в джинсе";
	npcType = npctype_main;
	guild = GIL_CITY;
	id = 2;
	voice = 5;
	level = 50;
	aivar[AIV_SystemFlags] = SF_Invincible;
	attribute[ATR_STRENGTH] = 100;
	attribute[ATR_DEXTERITY] = 100;
	attribute[ATR_MANA_MAX] = 500;
	attribute[ATR_MANA] = 500;
	attribute[ATR_HITPOINTS_MAX] = 600;
	attribute[ATR_HITPOINTS] = 600;
	B_CreateAmbientInv(self);
	Mdl_SetVisual(self,"HUMANS.MDS");
	Mdl_SetVisualBody(self,"hum_body_Naked0",1,0,"Hum_Head_Bald",26,0,ItAr_Clothes_02);
	B_SetFightSkills(self,100);
	fight_tactic = FAI_HUMAN_MASTER;
	daily_routine = Rtn_Start_2001;
};

func void Rtn_Start_2001()
{
	TA_Sit_Throne(8,0,23,0,"CITY_GODSSTUFF_INSIDE_03");
	TA_Sleep(23,0,8,0,"CITY_GODSSTUFF_INSIDE_03");
};

