//#################################################################
//
//  32 bit IEEE 754 floats (kurz: Gleitkommazahlen) f�r Daedalus:
//      Script vom 26.11.2008, erstellt von Sektenspinner
//
//  Es wird NICHT der vollst�ndige IEEE 754 Standard unterst�tzt
//  Es gibt Unterschiede in Spezialf�llen und Rundungsfehler.
//  Das grunds�tzliche Format wird aber eingehalten, sodass
//  auch mit Engine-Floats gerechnet werden kann.
//
//  Bugfix am 19.03.2010:
//      Grober Fehler: Exponent war stets um eins falsch.
//      Das fiel auf, da durch die neue "Forschung" auch
//      "externe floats" aus der Engine aus den Scripten
//      erreichbar sind.
//  Bugfix am 1.07.2010:
//      Grober Fehler: mulf lieferte stets positive Ergebnisse
//      Danke an orcwarriorPL!
//  Bugfix am 23.09.2010:
//      Behoben sind:
//          -Problem in truncf bei sehr kleinen Zahlen
//          -Problem in addf falls Exponenten sich um
//           mehr als 31 unterscheiden.
//          -Subtraktion von zwei gleichen Zahlen ergibt nun
//           null (anstatt ~2^(-23) mal diese Zahl)
//          -Die Funktionen sollten nun beliebig schachtelbar sein.
//      Danke an mud-freak!
//  Erweiterung am 5.2.2011:
//      -fracf hinzugef�gt, danke an orcwarriorPL!
//  Erweiterung am 7.4.2011:
//      -sqrtf_approx hinzugef�gt, geschrieben von Lehona
//  Performanceverbesserung 8.4.2011:
//      -sqrtf beschleunigt, Dank geb�hrt Lehona
//
//#################################################################

/*####################################
//      Kurzdokumentation
//####################################

//*********************
//  Grunds�tzliches
//*********************

32-bit Floats der Genauigkeit "single" werden mit den in diesem Script enthaltenen Funktionen auf einem gew�hnlichen integer simuliert. Dabei ist darauf zu achten floats und integer trotz des formal gleichen Datentyps penibel zu unterscheiden. Wenn eine Zahl in Floatdarstellung mit einer Zahl in Integerdarstellung verrechnet wird oder Funktionen auf integer angewendet werden, die eigentlich f�r floats gedacht sind oder umgekehrt kommt im besten Fall einfach nur Bl�dsinn heraus. Normalerweise meckert in so einem Fall der Compiler, Gothic kennt aber den Unterschied nicht!

//************************************
//  Das Instrumentarium
//************************************

Um mit floats rechnen zu k�nnen, m�ssen diese zun�chst erzeugt werden. Dazu gibt es eine Abbildung mkf, die eine integer Ganzzahl in einen float umwandelt.
floats k�nnen untereinander addiert, subtrahiert, multipliziert und dividiert werden. Eine Wurzelfunktion ist ebenfalls definiert.
Um den Wert der floats intepretieren zu k�nnen, sind die Ordnungsrelationen >=, >, <, <= mit den Funktionen ge, g, l, le gegeben.
Ferner k�nnen floats mit der truncf und roundf Funktion zur�ck in einen integer konvertiert werden.

//************************************
//  Die Funktionen im �berblick:
//************************************

"func float" und "var float" wie hier angegeben gibt es nicht wirklich (f�r Gothic sind alles integer). Diese Notation soll hier nur zur Veranschaulichung dienen.
Um sich die Funktionsnamen merken zu k�nnen hilft vielleicht folgende Liste, die die Abk�rzungen erkl�rt:

mkf    = make float
truncf = truncate float
roundf = round float
addf   = add floats
subf   = subtract floats
negf   = negate float
mulf   = multiply floats
divf   = divide floats
invf   = inverse float
gf     = greater
gef    = greater or equal
lf     = lower
lef    = lower or equal
sqrtf  = square root of float
printf = print float

**************** Umwandlung *******************
func float mkf (var int x) //Erzeugt die Floatdarstellung aus einer Ganzzahl.
func int truncf (var float x) //reduziert einen float auf seinen Ganzzahlanteil, wobei alle Nachkommastellen verworfen werden. Der Ergebnistyp ist Integer. (-1.5 w�rde zu -1.0, nicht zu -2)
func int roundf (var float x) //reduziert einen float auf eine Ganzzahl, die durch Runden ermittelt wird. Wie truncf, nur das zuvor 0.5 addiert wird. Der Ergebnistyp ist Integer.

**************** Addition *********************
func float addf (var float x, var float y) //addiert x und y und gibt das Ergebnis zur�ck.
func float subf (var float x, var float y) //subtrahiert y von x und gibt das Ergebnis zur�ck.
func float negf (var float x) //Gibt das additive Inverse von x (also -x) zur�ck.

**************** Multiplikation ***************
func float mulf (var float x, var float y) //multipliziert x und y miteinander und gibt das Ergebnis zur�ck.
func float divf (var float x, var float y) //dividiert x durch y und gibt das Ergebnis zur�ck.
func float invf (var float x) //Gibt das multiplikative Inverse des floats x, also 1/x zur�ck. Gibt f�r x = 0 einen Fehler aus.
func float fracf (var int p, var int p) //Gibt den Bruch p/q als float zur�ck. �quivalent zu divf(mkf(p), mkf(q))

**************** Ordnungsrelationen ***********
func int gf (var float x, var float y) //gibt f�r x > y TRUE zur�ck, sonst FALSE
func int gef (var float x, var float y) //gibt f�r x >= y TRUE zur�ck, sonst FALSE
func int lef (var float x, var float y) //gibt f�r x <= y TRUE zur�ck, sonst FALSE
func int lf (var float x, var float y) //gibt f�r x < y TRUE zur�ck, sonst FALSE

**************** Verschiedene *****************
func float sqrtf (var float x) //gibt die Wurzel des floats x zur�ck. Gibt f�r negative x einen Fehler aus.
func float sqrtf_approx (var float x) //berechnet die Wurzel erheblich performanter als sqrtf aber m�glicherweise ungenauer
func void printf (var float x) {}; //gibt einen float als Kommazahl auf dem Bildschirmaus. Diese Funktion funktioniert f�r sehr gro�e und sehr kleine x nicht richtig.

//************************************
//  Sonstiges
//************************************

Es sind f�nf float Konstanten definiert, die genutzt werden k�nnen, ohne dass sie erst berechnet/erzeugt werden m�ssen:

FLOATNULL = 0
FLOATEINS = 1
FLOATHALB = 0.5
PI = 3.1415...
E = 2.7182...

//************************************
//  Beispiele
//************************************

Folgende Funktion soll das Volumen eines Zylinders in cm� berechnen und gerundet zur�ckgeben:

func int Zylindervolumen (var int radius, var int hoehe) {
    var int radiusf, var int hoehef;
    radiusf = mkf (radius);
    hoehef = mkf (hoehe);

    //Volumen = r� * PI * h

    var int ergebnisf;
    ergebnisf = mulf (radiusf, radiusf);
    ergebnisf = mulf (ergebnisf, PI);
    ergebnisf = mulf (ergebnisf, h);

    return roundf (ergebnisf);
};

Folgende Funktion berechnet eine Zahl und gibt sie auf dem Bildschirm aus. Sie schachtelt dabei manche Funktionen

func void antwort () {
    var int foo;

    foo = mulf (mkf (1337), PI);
    printf (divf (foo, mkf (100)));

    //(1337*PI)/100 ist verbl�ffend genau 42. ;-)
};

Folgende Funktion macht ein paar Vergleiche. Es wird in keinem Fall "FEHLER" ausgegeben.

func void floattest()
{
    var int halb; var int eins; var int zwei;
    var int null;
    var int minuseins; var int minuszwei;

    halb = invf (mkf (2));
    eins = mkf (1);
    zwei = mkf (2);
    null = mkf (0);
    minuseins = mkf (-1);
    minuszwei = mkf (-2);

    if (gf (zwei,eins))           {} else { print ("FEHLER!"); };
    if (gf (eins,null))           {} else { print ("FEHLER!"); };
    if (lf (minuseins,null))      {} else { print ("FEHLER!"); };
    if (lf (minuszwei,minuseins)) {} else { print ("FEHLER!"); };
    if (gf (halb,minuseins))      {} else { print ("FEHLER!"); };
    if (lf (halb,zwei))           {} else { print ("FEHLER!"); };
    if (lef (null,null))          {} else { print ("FEHLER!"); };
    if (gef (null,null))          {} else { print ("FEHLER!"); };
};

//************************************
//  Beschr�nkungen und Fallen
//************************************

***********  Nutzerfehler ************
Es sollten unter keinen Umst�nden die Operatoren +, -, * oder / auf floats angewendet werden. Sie haben dort keine sinnvolle Anwendung. Bestensfalls kommt einfach nur Bl�dsinn heraus. Wie oben beschrieben sind addf, subf, mulf und divf anzuwenden.
Wer versteht wie ein float intern aufgebaut ist, kann zum Beispiel innerhalb der positiven Zahlen die Ordnungsrealtionen (>, <, <= >=) benutzen. Wer sich nicht sicher ist, sollte auf die gegebenen Funktionen zur�ckgreifen.
Nat�rlich sind umgekehrt die Floatfunktionen f�r Integer unbrauchbar. Der Ausdruck sqrtf (4) ist nicht 2 sondern einfach nur falsch, da 81 ein Integer ist!

*********** Float-Fehler *************
Die Genauigkeit der floats ist sehr begrenzt, ab etwa 4 Dezimalstellen ist mit Rundungsfehlern zu rechnen. Daf�r k�nnen sehr gro�e und sehr kleine Zahlen dargestellt werden.
Es gelten die gew�hnlichen Einschr�nkungen und Empfehlungen f�r floats. Zum Beispiel ist es selten sinnvoll floats auf Gleichheit zu �berpr�fen, da es durch Rundungsfehler wahrscheinlich ist, dass auch zwei floats, die eigentlich gleich sein m�ssten kleine Abweichungen zueinander aufweisen. Es sollte in einem solchen Fall besser gepr�ft werden, ob die Differenz der beiden floats einen (im Verh�ltnis zur Problemstellung) kleinen Wert unterschreitet.

//************************************
//  Vielen Dank...
//************************************

...dass du dich f�r dieses Script interessierst und es gelesen, oder zumindest �berflogen hast. Dann war meine Arbeit nicht ganz umsonst. ;-)
Mir ist allerdings bewusst, dass dies wohl eher ein Randgebiet des Gothicmoddings ist.

Edit: M�rz 2010: Haha! Mit direktem Zugriff auf zEngine Objekte ist dies mitnicht ein Randgebiet! Es lassen sich einige hochinteressante Floatwerte in Gothic finden!

*/

//#################################################################
//
//  DIE FUNKTIONEN
//
//#################################################################

const int BIT_VZ = 1; //Vorzeichen hat 1 Bit (was auch sonst?!)
const int BIT_EXP = 8; //nach IEEE 754 ist 8 die Norm
const int BIT_MT = 23; //bleiben 23 bit f�r die Mantisse
const int EXP_OFFSET = 127; //exp = characteristic - EXP_OFFSET

const int EXP_PATTERN = ((1 << BIT_EXP) - 1) << BIT_MT;
const int MT_PATTERN = ((1 << BIT_MT) - 1);

const int MINUS = 1 << 31;
const int MININT = 1 << 31;
const int MAXINT = MININT - 1;

const int FLOATNULL = 0; //vz 0, exp -128, mt 1.0 //nicht echt 0! Aber so ziemlich. Damit die Vergleiche gut funktionieren. Letztendlich ist der Wert aber egal. FLOATNULL ist ein Symbol mit dem nicht gerechnet wird.
const int FLOATEINS = 1065353216; //vz 0, exp 0 (also 127), mt 1.0
const int FLOATHALB = 1056964608; //vz 0, exp -1 (also 126), mt 1.0

const int PI = 1078530011;
const int E =  1076754516;

//************************************
//  Interne Hilfsfunktionen
//************************************

func int HighestBitPos (var int x) {
    if (x == 0) {
        return 0;
    }
    else {
        return 1 + HighestBitPos (x >> 1);
    };
};

func int extractExp (var int x) {
    var int exp;
    exp = x & EXP_PATTERN;
    exp = exp >> BIT_MT;
    exp = exp - EXP_OFFSET; //wegen Vergleichen ist der exponent verschoben

    return exp;
};

func int extractMt (var int x) {
    var int mt;
    mt = x & MT_PATTERN;
    //das erste bit, was gespart wurde wieder hin:
    mt = mt | (1 << BIT_MT);

    return mt;
};

func int packExp (var int exp) {
    //exponent -> Charakteristik -> und schieben
    return (exp + EXP_OFFSET) << BIT_MT;
};

//************************************
//      float bauen:
//************************************

func int mkf (var int x) {
    var int result; result = 0;
    //das Vorzeichen bit
    if (x < 0) {
        result = MINUS;
        x = -x;
    };

    var int exp;
    exp = HighestBitPos (x) - 1;

    if (exp < 0) { //kann nur hei�en, dass die Zahl null ist
        return FLOATNULL;
    };

    //Dass die erste Zahl eine 1 ist, ist ja wohl klar, also wird sie abgeschnitten:
    x = x & (~(1 << exp));

    //Und jetzt packe ich das ganze in einen float:
    result = result | packExp(exp); //den Exponenten neben die Mantisse schieben.

    if (BIT_MT > exp) {
        return result | (x << (BIT_MT - exp)); //Die Mantisse wird nach vorne geschoben, aber eben nur soweit Platz ist
    }
    else {
        return result | (x >> (exp - BIT_MT));
    };
};

//************************************
//  R�ckumwandlung zu integer
//************************************

func int truncf (var int x) {
    //Sonderbehandlung f�r das Symbol NULL
    if (x == FLOATNULL) {
        return 0;
    };

    var int exp; exp = extractExp (x); //Exponenten holen
    var int mt; mt = extractMt (x); //Mantisse holen

    var int result;

    //�berl�ufe:
    if (exp >= 31) { //2^31 * 1.0 l�uft ins Vorzeichenbit rein!
        if (x > 0) {
            return MAXINT;
        } else {
            return MININT;
        };
    };

    //schneidet
    if (exp > BIT_MT) {
        result = mt << (exp - BIT_MT); //Mantisse zurechtschieben
    }
    else {
        //32 bit oder mehr schieben geht schief.
        if (BIT_MT - exp > 31) {
            return 0;
        };

        result = mt >> (BIT_MT - exp);
    };

    if (x < 0) {
        return - result;
    }
    else {
        return result + 0;
    };
};

//************************************
//  Addition
//************************************

func int addf (var int x, var int y) {
    var int expX; expX = extractExp (x);
    var int expY; expY = extractExp (y);
    var int mtX; mtX = extractMt (x);
    var int mtY; mtY = extractMt (y);
    var int isnegX; isnegX = x & MINUS;
    var int isnegY; isnegY = y & MINUS;

    //Sonderbehandlung f�r das Symbol NULL
    if (x == FLOATNULL) {
        return y + 0;
    }
    else if (y == FLOATNULL) {
        return x + 0;
    };

    //Die Betragsm��ig kleinere Zahl an die gr��ere anpassen
    if (expX > expY)
    {
        if (expX - expY > 31) {
            //dann schlagen die folgenden shiftoperationen fehl.
            //Aber x ist soviel gr��er als y, einfach x zur�ckgeben.
            return x + 0;
        };

        mtY = mtY >> (expX - expY);
        expY = expX;
    }
    else
    {
        if (expY - expX > 31) {
            //dann schlagen die folgenden shiftoperationen fehl.
            //Aber y ist soviel gr��er als x, einfach y zur�ckgeben.
            return y + 0;
        };

        mtX = mtX >> (expY - expX);
        expX = expY;
    };

    //Das Ergebnis berechnen:
    var int mtRes;
    if (isnegX) {
        mtRes = -mtX;
    }
    else {
        mtRes = mtX;
    };

    if (isnegY) {
        mtRes -= mtY;
    }
    else {
        mtRes += mtY;
    };

    var int isnegRes;
    if (mtRes < 0) {
        isnegRes = MINUS;
        mtRes = -mtRes;
    }
    else {
        isnegRes = 0;
    };

    //Pr�zisierung:
    if (!mtRes) { //damit abziehen eines Wertes von sich selbst pr�zise ist.
        return FLOATNULL;
    };

    var int shift;
    shift = HighestBitPos (mtRes) - (BIT_MT + 1);

    if  (shift > 0) {
        mtRes = mtRes >> shift;
    }
    else {
        mtRes = mtRes << (-shift);
    };

    //Noch die erste Zahl abschneiden (also zuschneiden):
    mtRes = mtRes & ((1 << BIT_MT) - 1);

    var int expRes;
    expRes = expX + shift;

    return isnegRes | packExp(expRes) | mtRes;
};

//************************************
//  Es lassen sich vier kleine
//  n�tzliche Hilfsfunktionen
//  definieren:
//************************************

func int negf (var int x) {
    if (x < 0) { return x & (~MINUS); }
    else { return x | MINUS; };
};

func int absf (var int x) {
    if (x < 0) { return negf (x); }
    else       { return x + 0; };
};

func int subf (var int x, var int y) {
    return addf (x, negf (y));
};

func int roundf (var int x) {
    if (x < 0) {
        return truncf (subf (x, FLOATHALB));
    } else {
        return truncf (addf (x, FLOATHALB));
    };
};

//************************************
//  Multiplikation
//************************************

func int mulf (var int x, var int y) {
    var int expX; expX = extractExp (x);
    var int expY; expY = extractExp (y);
    var int mtX; mtX = extractMt (x);
    var int mtY; mtY = extractMt (y);
    var int isnegX; isnegX = x & MINUS;
    var int isnegY; isnegY = y & MINUS;

    //Sonderbehandlung f�r das Symbol NULL
    if (x == FLOATNULL)
    || (y == FLOATNULL) {
        return FLOATNULL;
    };

    //Die Exponenten werden addiert
    var int expRes;
    expRes = expX + expY;

    //Die Mantissen multipliziert (wobei auf die 32 bit Grenze geachtet werden muss)
    var int mtRes;
    mtRes = (mtX >> (BIT_MT - 14)) * (mtY >> (BIT_MT - 14));
    mtRes = mtRes >> (28 - BIT_MT);

    if (mtRes >= (1 << (BIT_MT + 1)))
    {
        mtRes = mtRes >> 1;
        expRes += 1;
    };

    //Noch die erste Zahl abschneiden (also die Mantisse zuschneiden):
    mtRes = mtRes & ((1 << BIT_MT) - 1);

    var int isNegRes;
    if (isnegX == isnegY) {
        isNegRes = 0;
    }
    else {
        isNegRes = MINUS;
    };

    //noch Erkenntnisse zusammenf�gen
    return isnegRes | packExp(expRes) | mtRes;
};

//************************************
//  Die Division l�sst sich
//  nicht auf die Multiplikation
//  zur�ckf�hren. Das multiplikative
//  Inverse ist schlie�lich schwerer
//  zu finden, als das additive
//  Inverse. Also, gesonderte Funktion:
//************************************

func int divf (var int x, var int y) {
    var int expX; expX = extractExp (x);
    var int expY; expY = extractExp (y);
    var int mtX; mtX = extractMt (x);
    var int mtY; mtY = extractMt (y);
    var int isnegX; isnegX = x & MINUS;
    var int isnegY; isnegY = y & MINUS;

    //Sonderbehandlung f�r das Symbol NULL
    if (y == FLOATNULL) {
        Print ("### ERROR: DIVISION BY ZERO ###");
        return FLOATNULL;
    }
    else if (x == FLOATNULL) {
        return FLOATNULL;
    };

    //Exponent subtrahieren
    var int expRes;
    expRes = expX - expY;

    //Die Mantissen dividieren, davor Divident und Divisor passend hinschieben
    var int mtRes;
    mtRes = (mtX << (7)) / (mtY >> 9); //X soweit es geht nach links, Y auf die Mitte
    mtRes = mtRes << (BIT_MT - 7 - 9);

    //Und das Ergebnis wieder zur�ckschieben
    if (mtRes < (1 << (BIT_MT))) {
        mtRes = mtRes << 1;
        expRes -= 1;
    };

    //Noch die erste Zahl abschneiden (also die Mantisse zuschneiden):
    mtRes = mtRes & ((1 << BIT_MT) - 1);

    var int isNegRes;
    if (isnegX == isnegY) {
        isNegRes = 0;
    }
    else {
        isNegRes = MINUS;
    };

    //noch Erkenntnisse zusammenf�gen
    return isnegRes | packExp(expRes) | mtRes;
};

//************************************
//  Kleine Hilfsfunktion
//************************************

func int invf (var int x) {
    return divf (FLOATEINS, x);
};

/* thanks to orcwarriorPL for the idea! */
func int fracf (var int p, var int q) {
    return divf(mkf(p), mkf(q));
};

//************************************
//  Wurzelziehen
//************************************

func int sqrtf_hlp (var int target, var int guess, var int steps) {
    //babylonisches Wurzelziehen / Heron
    guess = addf (guess, divf (target, guess));
    guess = mulf (FLOATHALB, guess);

    if (steps == 0) {
        return guess;
    } else {
        return sqrtf_hlp (target, guess, steps - 1);
    };
};

func int sqrtf (var int x) {
    if (x < FLOATNULL) {
        Print ("ERROR: sqrtf: x must be nonnegative.");

        return FLOATNULL;
    };
    
    //guess wird der Startwert des Heronverfahrens
    //der Exponent von guess soll der halbe Exponent von x sein.
    var int e;
	e = ExtractExp(x);
	e = e/2;
    var int guess;
    guess = packExp(e); //Mantisse ist egal.
    
    //4 ist schon eher viel. Man kann hier auch auf 3 runtergehen.
    //ab 4 d�rfte sich das Ergebnis sp�testens stabilisiert haben.
    return sqrtf_hlp (x, guess, 4) + 0;
};

//Schnelles Wurzelziehen von Lehona getreu dem Wikipedia Artikel:
// http://en.wikipedia.org/wiki/Fast_inverse_square_root

func int sqrtf_approx(var int f) {
    var int x2f;
    var int threehalfs;
    if (!threehalfs) {
        threehalfs = addf(FLOATEINS, FLOATHALB);
    };

    x2f = mulf(f, FLOATHALB);

    f = 1597463007 /* 5F3759DFh */ - (f >> 1);
    return invf(mulf(f, subf(threehalfs, mulf(x2f, mulf(f,f)))));
};

//************************************
//  Ordnungsrelationen
//************************************

func int gf (var int x, var int y) {
    var int isnegX; isnegX = x & MINUS;
    var int isnegY; isnegY = y & MINUS;

    if (isNegX && isNegY) { //beide negativ
        if (x < y) {
            return TRUE;
        };
    }
    else {
        if (x > y) {
            return TRUE;
        };
    };

    return FALSE;
};

func int gef (var int x, var int y) {
    if (gf (x,y)) || (x == y) {
        return TRUE;
    };
    return FALSE;
};

func int lef (var int x, var int y) {
    if (!gf (x,y)) {
        return TRUE;
    };
    return FALSE;
};

func int lf (var int x, var int y) {
    if (!gef (x,y)) {
        return TRUE;
    };
    return FALSE;
};

//************************************
//  Ausgabefunktionen
//************************************

func string BuildNumberHlp (var string res, var int x, var int kommapos) {
    if (kommapos == 0) {
        res = ConcatStrings (",", res);
        res = ConcatStrings (IntToString (x), res);

        return res;
    };

    res = ConcatStrings (IntToString (x % 10), res);

    return BuildNumberHlp (res, x / 10, kommapos - 1);
};

func string BuildNumber (var string res, var int x, var int kommapos) {
    if (x < 0) {
        return ConcatStrings ("-", BuildNumberHlp (res, -x, kommapos));
    }
    else {
        return BuildNumberHlp (res, x, kommapos);
    };
};

func void printf (var int x) { //Ok, ok sorry c-ler. Aber ich will konsistente Namen haben.
    //Ich bekomme nur eine primitive Darstellung als Kommazahl hin.
    //f�r die Darstellung als X * 10^EXP fehlen mir Ideen oder Logarithmusfunktionen

    x = mulf (x, mkf (10000));
    x = truncf (x);

    Print (BuildNumber ("", x, 4));
};

