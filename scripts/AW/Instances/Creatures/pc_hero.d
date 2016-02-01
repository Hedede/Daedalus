	//(day - 1) % 7 + 1

instance ItSe_LockpickFisch(C_Item)
{
	name = "Легкая рыба";
	mainflag = ITEM_KAT_NONE;
	flags = ITEM_MISSION | ITEM_MULTI;
	value = 25;
	visual = "ItFo_Fish.3DS";
	material = MAT_LEATHER;
	scemeName = "MAPSEALED";
	on_state[0] = Use_LockpickFisch;
	description = name;
	text[2] = "В этой рыбе что-то спрятано.";
};


func void Use_LockpickFisch()
{
	//B_PlayerFindItem(ItRw_Bolt,100);
};

instance PC_Hero(Npc_Default)
{
	name[0] = "ГГ";
	npcType = npctype_main;
	id = 0;
	guild = 0;
	voice = 15;
	level = 0;
	bodyStateInterruptableOverride = TRUE;
	exp = 0;
	exp_next = 500;
	lp = 0;
	attribute[ATR_STRENGTH] = 40;
	attribute[ATR_DEXTERITY] = 40;
	attribute[ATR_MANA_MAX] = 0;
	attribute[ATR_MANA] = 0;
	attribute[ATR_HITPOINTS_MAX] = 100;
	attribute[ATR_HITPOINTS] = 100;
	aivar[ATR_STAMINA] = 50;
	aivar[ATR_STAMINA_MAX] = 100;
	//attribute[ATR_REGENERATEHP] = 1;
	Npc_SetTalentSkill(self,NPC_TALENT_PICKLOCK,1);
	Npc_SetTalentSkill(self,NPC_TALENT_RIFLES,7);
	Npc_SetTalentSkill(self,17,7);
	Npc_SetTalentSkill(self,19,7);
	B_SetFightSkills(self,0);		
	Mdl_SetVisual(self,"HUMANS.MDS");
	Mdl_SetVisualBody(self,"HUM_BODY_HERO",0,0,"Hum_Head_AW_BP_HD",Face_P_Hero,Teeth_Normal,NO_ARMOR);
	/*CreateInvItems(self,ItPo_Health_Addon_04,1000);
	CreateInvItems(self,ItPo_Mana_Addon_04,1000);
	CreateInvItems(self,ItPo_Addon_Geist_01,1000);
	CreateInvItems(self,ItPo_Addon_Geist_02,1000);
	CreateInvItems(self,MIS_Doc_Pass,1);
	CreateInvItems(self,ItSe_LockpickFisch,100);
	CreateInvItems(self,Heal_Aspirin,1000);*/
	CreateInvItems(self,ItRw_Bolt,1000);
};
/*
instance PC_Test(Npc_Default)
{
	name[0] = "ГГ";
	npcType = npctype_main;
	id = 0;
	guild = 0;
	voice = 15;
	level = 0;
	bodyStateInterruptableOverride = TRUE;
	exp = 0;
	exp_next = 500;
	lp = 0;
	attribute[ATR_STRENGTH] = 30;
	attribute[ATR_DEXTERITY] = 30;
	attribute[ATR_MANA_MAX] = 0;
	attribute[ATR_MANA] = 0;
	attribute[ATR_HITPOINTS_MAX] = 100;
	attribute[ATR_HITPOINTS] = 100;
	aivar[ATR_Stamina] = 50;
	aivar[ATR_Stamina_MAX] = 100;
	//attribute[ATR_REGENERATEHP] = 1;
	Npc_SetTalentSkill(self,NPC_TALENT_PICKLOCK,1);
	Npc_SetTalentSkill(self,NPC_TALENT_RIFLES,7);
	Npc_SetTalentSkill(self,17,7);
	Npc_SetTalentSkill(self,19,7);
	B_SetFightSkills(self,0);		
	Mdl_SetVisual(self,"HUMANS.MDS");
	Mdl_SetVisualBody(self,"HUM_ARMOR_CAMOUFLAGE01",1,0,"Hum_Head_AW_BuxProf",Face_P_Hero,Teeth_Normal,NO_ARMOR);
	CreateInvItems(self,ItPo_Health_Addon_04,1000);
	CreateInvItems(self,ItPo_Mana_Addon_04,1000);
	CreateInvItems(self,ItPo_Addon_Geist_01,1000);
	CreateInvItems(self,ItPo_Addon_Geist_02,1000);
	CreateInvItems(self,MIS_Doc_Pass,1);
	CreateInvItems(self,ItSe_LockpickFisch,100);
	CreateInvItems(self,Heal_Aspirin,1000);
	CreateInvItems(self,Ammo_7_62mm,1000);
};*/
