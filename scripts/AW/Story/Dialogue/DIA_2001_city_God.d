instance DIA_2001_God_EXIT(C_Info)
{
	npc = city_2001_God;
	nr = 999;
	condition = DIA_Always;
	information = DIA_End;
	permanent = TRUE;
	description = Dialog_Ende;
};

func int DIA_2001_God_exit_condition()
{
	return TRUE;
};

instance DIA_2001_God_FirstMeet(C_Info)
{
	npc = city_2001_God;
	nr = 999;
	condition = C_Return_True;
	information = DIA_2001_God_firstmeet_info;
	important = TRUE;
	permanent = FALSE;
};

func void DIA_2001_God_firstmeet_info()
{
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_01"); //Привет, ищешь чего?
	AI_Output(other,self,"DIA_2001_God_firstmeet_15_02"); //Да, вот, попить бы...ы
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_03"); //Выбирай тогда... Как тебя звать?
	AI_Output(other,self,"DIA_2001_God_firstmeet_15_04"); //Я... Не помню! Не помню я...
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_05"); //(усмехается) Ну, а меня Годом звать.
	/*if(Npc_HasItems(other,ItMi_Gold) < 5) {
		AI_Output(self,other,"DIA_2001_God_firstmeet_05_06"); //(понизив голос) Вот так проблема. Чего пить-то будешь?
		AI_Output(other,self,"DIA_2001_God_firstmeet_15_07"); //У меня денег нет... Проснулся утром — в карманах пусто.
	} else {
		AI_Output(self,other,"DIA_2001_God_firstmeet_05_07"); //(понизив голос) Вот так проблема.
		AI_Output(other,self,"DIA_2001_God_firstmeet_15_08"); //Я проснулся утром без денег и документов, не знаю даже что делать.
	};*/
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_09"); //К ментам сходи, скорее всего они тебе помогут.
	Info_ClearChoices(DIA_2001_God_FirstMeet);
	Info_AddChoice(DIA_2001_God_FirstMeet,"А куда идти?",DIA_2001_God_firstmeet_ask);
};

func void DIA_2001_God_firstmeet_ask()
{
	AI_Output(other,self,"DIA_2001_God_firstmeet_15_10"); //А куда идти?
	AI_PointAt(self,"CITY_GODSSTUFF_INSIDE_BACKENTRANCE");
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_11"); //(Усмехается) Через заднюю дверь выйди и прямо иди.
	AI_StopPointAt(self);
	Info_ClearChoices(DIA_2001_God_FirstMeet);
};
