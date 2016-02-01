//Bar instances
instance Bar_Health(GothicBar)
{
    x = 100;
    y = 30;
    barTex = "Bar_Health.tga";
};

instance Bar_Stamina(GothicBar)
{
    x = 100;
    y = 60;
    barTex = "Bar_Misc.tga";
};

instance Bar_Poison(GothicBar)
{
    x = 100;
    y = 90;
    barTex = "Bar_Poison.tga";
};

//Hook bar-updater to frame update.
func void InitBars()
{
    FF_ApplyOnce(UpdateBars);
};

var int _Bar_Health;
var int _Bar_Stamina;
var int _Bar_Poison;
var int _Bar_Poison_12;
var int _Bar_Poison_22;

func void UpdateBars()
{
    if (!Hlp_IsValidHandle(_Bar_Health)) {
        _Bar_Health = Bar_Create(Bar_Health);
    };

    Bar_SetMax(_Bar_Health, hero.attribute[ATR_HITPOINTS_MAX]);
    Bar_SetValue(_Bar_Health, hero.attribute[ATR_HITPOINTS]);


    if (!Hlp_IsValidHandle(_Bar_Stamina)) {
        _Bar_Stamina = Bar_Create(Bar_Stamina);
    };

    Bar_SetMax(_Bar_Stamina, hero.aivar[ATR_STAMINA_MAX]);
    Bar_SetValue(_Bar_Stamina, hero.aivar[ATR_STAMINA]);	
	
    if (!Hlp_IsValidHandle(_Bar_Poison)) {
        _Bar_Poison = Bar_Create(Bar_Poison);
    };

    Bar_SetMax(_Bar_Poison, 50);
    Bar_SetValue(_Bar_Poison, 25);
};
