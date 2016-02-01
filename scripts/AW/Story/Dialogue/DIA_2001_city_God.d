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
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_01"); //������, ����� ����?
	AI_Output(other,self,"DIA_2001_God_firstmeet_15_02"); //��, ���, ������ ��...�
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_03"); //������� �����... ��� ���� �����?
	AI_Output(other,self,"DIA_2001_God_firstmeet_15_04"); //�... �� �����! �� ����� �...
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_05"); //(����������) ��, � ���� ����� �����.
	/*if(Npc_HasItems(other,ItMi_Gold) < 5) {
		AI_Output(self,other,"DIA_2001_God_firstmeet_05_06"); //(������� �����) ��� ��� ��������. ���� ����-�� ������?
		AI_Output(other,self,"DIA_2001_God_firstmeet_15_07"); //� ���� ����� ���... ��������� ����� � � �������� �����.
	} else {
		AI_Output(self,other,"DIA_2001_God_firstmeet_05_07"); //(������� �����) ��� ��� ��������.
		AI_Output(other,self,"DIA_2001_God_firstmeet_15_08"); //� ��������� ����� ��� ����� � ����������, �� ���� ���� ��� ������.
	};*/
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_09"); //� ������ �����, ������ ����� ��� ���� �������.
	Info_ClearChoices(DIA_2001_God_FirstMeet);
	Info_AddChoice(DIA_2001_God_FirstMeet,"� ���� ����?",DIA_2001_God_firstmeet_ask);
};

func void DIA_2001_God_firstmeet_ask()
{
	AI_Output(other,self,"DIA_2001_God_firstmeet_15_10"); //� ���� ����?
	AI_PointAt(self,"CITY_GODSSTUFF_INSIDE_BACKENTRANCE");
	AI_Output(self,other,"DIA_2001_God_firstmeet_05_11"); //(����������) ����� ������ ����� ����� � ����� ���.
	AI_StopPointAt(self);
	Info_ClearChoices(DIA_2001_God_FirstMeet);
};
