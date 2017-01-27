/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - 5DI / 1 AI Demultiplexer on a tension analog input - Stand-alone version                                     19/12/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Function libraries  - None (standalone version)                                                                                18/12/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Inclusion of librarise requested to execute the program ----------------------------------------------------------------------------------*
#include <Wire.h>					// Library  - Comm Ports
#include <Indio.h>					// Library  - Industruino input / output control
#include <UC1701.h>
static UC1701 lcd;

// Input and Output confuguration -----------------------------------------------------------------------------------------------------------*
float EA1_MII_MPLX;					// General  - Voltage Analog Input used to connect the input multiplexer
float VTE_MPLX[7] ;					// Multi_V  - Calulated voltages along the contact chain
int VNC_MPLX[6];					// Multi-V  - Filtered demultiplexed Input transition confirmation counters
boolean EM1_MII_MPLX[11];				// Multi-V  - Demultiplexed inputs

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[11] = {					// LCDScreen - Descriptions strings table
  "                     ", // 00
  "Haut, Enter : Accueil",
  "AI1                  ",
  "MI1                  ",
  "MI2                  ",
  "MI3                  ",
  "MI4                  ",
  "MI5                  ",
  "- Down:Zero Setting -",
  "-   Zero  Setting   -",
  " "};                   // 10

String TCM_UNIT[15] = {					// LCDScreen - Standard units strings table
  "        ",  // 00
  "oC      ",
  "%       ",
  "s       ",
  "V       ",
  "mV      ",  // 05
  "",
  "",
  "",
  "",
  "",          // 10
  "    Open ",
  "  Closed ",
  "     Set ",
  "   Learn "} ;

boolean FCO_PRUP[5];					// LCDScreen - "Up" button is pressed
boolean FCO_PREN[5];					// LCDScreen - "Enter" button is pressed
boolean FCO_PRDN[5];					// LCDScreen - "Down" button is pressed
int VNT_MENU[3];					// LCDScreen - Menus pointer table
int VNC_MENU[30];					// LCDScreen - Menus adress table
int VNC_PRUP[31];					// LCDScreen - Menus pointer table if "Up" is pressed
int VNC_PREN[30];					// LCDScreen - Menus pointer table if "Enter" is pressed
int VNC_PRDN[30];					// LCDScreen - Menus pointer table if "Down" is pressed
float VTT_SCRN[10];					// LCDScreen - Timer table

boolean FCM_MPX1[1];					// G_MODV1B - Learn command memory in LCD Screen menu

void setup() {						// Initialization ----------------------------------------------------------------------------------*
I_INPS();
I_DISP();
Serial.begin(9600);
}

void loop() {						// Execution main loop -----------------------------------------------------------------------------*
P_INPS();
P_DISP();
}

void I_INPS() {						// Input initialization ---------------------------------------------------------------------- INIT *
Indio.setADCResolution(14);				// System   - AI Resolution. POssible values : 12bit@240Lps, 14bit@60Lps, 16bit@15Lps and 18bit@3.75Lps.
Indio.analogReadMode(1, V10_p);			        // System   - AI1 is set to 0-10V / 0-100% measurement mode
VTE_MPLX[2] = 150;					// Voltage detection offset in mV
VTE_MPLX[3] = VTE_MPLX[1] * (1 - (10110 / 14715));
VTE_MPLX[4] = VTE_MPLX[3];
VNC_MPLX[0] = 3;					// Number of successive identical raw input values to confirm a transition to filtered value
return;
}

void P_INPS() {                                         // Reading inputs ---------------------------------------------------------------------------- EXEC *
  int RES[6] = {10110, 150, 301, 604, 1210, 2340};
  EA1_MII_MPLX = Indio.analogRead(1) ;
  T_DMULTV(EA1_MII_MPLX, RES, VTE_MPLX, VNC_MPLX, EM1_MII_MPLX, 1);
  Serial.print(VTE_MPLX[3]);
  Serial.print("\t");
  Serial.print(VTE_MPLX[4]);
  Serial.print("\t");
  Serial.print(EA1_MII_MPLX * 100);
  Serial.print("\t");
  Serial.print(VNC_MPLX[1]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[1]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[2]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[2]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[3]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[3]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[4]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[4]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[5]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[5]);
  Serial.println("\t");
  return; }

void I_DISP() {						// Screen parameters initialization ---------------------------------------------------------- INIT *
VTT_SCRN[1] = 180;					// G_TOUCHx - Inactivity timeout to stop backlight
VTT_SCRN[2] = 240;					// G_TOUCHx - Inactivity timeout to get back to default screen
VTT_SCRN[3] = 20;					// G_TOUCHx - "Enter" button filtering time
VTT_SCRN[4] = 15;					// G_TOUCHx - "Up" and "Down" buttons filtering time
VNT_MENU[0] = 3;					// G_NAVIGM - Size of used adress tables
VNT_MENU[1] = 1;					// G_NAVIGM - Adress pointer initialization
VNC_MENU[0] = 1;					// G_NAVIGM - Menu adress table
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_MENU[3] = 3;
			  				// G_NAVIGM - Destination adress table for each button in each menu
VNC_PRUP[1] =   1; VNC_PREN[1] =   2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   1; VNC_PREN[2] =   1; VNC_PRDN[2] =   3;
VNC_PRUP[3] =   3; VNC_PREN[3] =   2; VNC_PRDN[3] =   3;
VNC_PRUP[4] =   2;
 
pinMode(23, INPUT);					// LCDScreen - 1286 only - Down button adress
pinMode(24, INPUT);					// LCDScreen - 1286 only - Enter button adress
pinMode(25, INPUT);					// LCDScreen - 1286 only - Up button adress
pinMode(26, OUTPUT);					// LCDScreen - 1286 only - Backlight output
/*
pinMode(13, OUTPUT);                    		// LCDScreen - 32U8 only - Backlight output
*/
lcd.begin();						// LCDScreen - Initialisation of the screen
lcd.clear();						// LCDScreen - Cleaning of the screen
return; }

void P_DISP() {						// Screen manus and displays ---------------------------------------------------------------- EXEC *
//G_TOUCH1(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);	// LCDScreen - 32U8 only - Button and activity monitoring
  G_TOUCH2(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);	// LCDScreen - 1286 only - Button and activity monitoring
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 0); lcd.clearLine(); }
  G_MESS1L(VNC_MENU, 1, 1, 0, "   www.econtrols.fr  ");
  G_MESS1L(VNC_MENU, 1, 2, 0, "Automation & Controls");
  G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  1286  ");
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 4); lcd.clearLine(); }
  G_MESS1L(VNC_MENU, 1, 5, 0, " 5DI/1AI Multiplexer ");
  G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
  G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.00 ESA 19/12/2016");
  
  G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
  G_VISV1F(VNC_MENU, 2, 1, 0, EA1_MII_MPLX * 100, TCM_PRES[2], TCM_UNIT[5]);
  G_VISV1B(VNC_MENU, 2, 2, 0, EM1_MII_MPLX[1], TCM_PRES[3], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 3, 0, EM1_MII_MPLX[2], TCM_PRES[4], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 4, 0, EM1_MII_MPLX[3], TCM_PRES[5], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 5, 0, EM1_MII_MPLX[4], TCM_PRES[6], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 6, 0, EM1_MII_MPLX[5], TCM_PRES[7], TCM_UNIT[11], TCM_UNIT[12]);
  G_MESS1L(VNC_MENU, 2, 7, 0, TCM_PRES[8]);

  G_MODV1B(VNC_MENU, 3, FCO_PRUP, FCO_PREN, FCO_PRDN, EM1_MII_MPLX, 0, FCM_MPX1, "- 5D/1A Multiplexer -", "EA1 - Zero Setting   ", TCM_UNIT[13], TCM_UNIT[14], true);

  G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
  return; }

// Functions used by the program -------------------------------------------------------------------------------------------------------------

void T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - 5DI / 1 AI Demultiplexer on a tension analog input - Stand-alone version                                     07/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DMULTV(float MES, float TNS[], float VNC[], boolean RES[], int CFG)                                                   07/09/2016 *
 *                MES : float, 0-10V reading to percentage (V10_p) of the analog input                                                       *
 *                OHM : [00] Common load resistance value in Ohms                                                                            *
 *                      [01] Load resistance value of multiplexers dry contact input 1 in Ohms                                               *
 *                      [02] Load resistance value of multiplexers dry contact input 2 in Ohms                                               *
 *                      [03] Load resistance value of multiplexers dry contact input 3 in Ohms                                               *
 *                      [04] Load resistance value of multiplexers dry contact input 4 in Ohms                                               *
 *                      [05] Load resistance value of multiplexers dry contact input 5 in Ohms                                               *
 *                      [06] Instant value of unshunted load resistances [00] to [05]                                                        *
 *                TNS : [00] Calculated tension on OHM[0] depending on the shunting configuration of OHM[1] to [5]                           *
 *                      [01] Reference power supply voltage : 24000 mV                                                                       *
 *                      [02] Voltage differential to compensate resistance deviations : 150 to 200 mV                                        *
 *                      [03] Theoretical voltage on RES[0], with no OHM[x] shunted, and for a 24000 mVdc power supply                        *
 *                      [04] Measured voltage on RES[0], with no OHM[x] shunted, and for actual power supply                                 *
 *                      [05] Actual power supply voltage according to TNS[3] and TNS[2]                                                      *
 *                VNC : [00] Number of controller scan cycles to confirm RES[x] transitions (rebound and interference filtering)             *
 *                      [01] Scan cycles counter of different RES[1] and RES[6] to confirm RES[1] transition                                 *
 *                      [02] Scan cycles counter of different RES[2] and RES[7] to confirm RES[2] transition                                 *
 *                      [03] Scan cycles counter of different RES[3] and RES[8] to confirm RES[3] transition                                 *
 *                      [04] Scan cycles counter of different RES[4] and RES[9] to confirm RES[4] transition                                 *
 *                      [05] Scan cycles counter of different RES[6] and RES[10] to confirm RES[5] transition                                *
 *                RES : [00] Operator validation to set TNS[4] to current input value (Voltage initialization)                               *
 *                      [01] Filtered demultiplexed digital input 1                                                                          *
 *                      ...                                                                                                                  *
 *                      [05] Filtered demultiplexed digital input 5                                                                          *
 *                      [06] Raw demultiplexed digital input 1                                                                               *
 *                      ...                                                                                                                  *
 *                      [10] Raw demultiplexed digital input 1                                                                               *
 *                CFG : Standard Override Command                                                                                            *
 *                      <0  : RES[1] to [5] stay false whatever the value of MES is                                                          *
 *                      0   : RES[1] to [5] keep their current value, thant can be changed from outside the function                         *
 *                      1   : RES[1] to [5] are actuated according to the values of MES                                                      *
 *                      2   : RES[1] to [5] stay true whatever the value of MES is                                                           *
 *                      3   : RES[1] to [5] stay true whatever the value of MES is                                                           *
 *                      >3  : RES[1] to [5] stay false whatever the value of MES is                                                          *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/09/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

 TNS[1] = 24000;
TNS[6] = float(OHM[5] + OHM[4] + OHM[3] + OHM[2]  + OHM[1] + OHM[0]);
TNS[3] = TNS[1] * (1 - (OHM[0] / TNS[6]));
if (RES[0] == true) {TNS[4] = MES * 100; RES[0] = false; }
TNS[5] = TNS[1] * TNS[4] / TNS[3];
switch (CFG) {
	case 0:
		break;
    case 1:
		TNS[6] = float(OHM[4] + OHM[3] + OHM[2]  + OHM[1] + OHM[0]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[6] = true; } else { RES[6] = false ; }
		TNS[6] = TNS[6] + (float(OHM[5]) * (1 - float(RES[6] == true))) - float(OHM[4]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[7] = true;  } else { RES[7] = false ; }
		TNS[6] = TNS[6] + (float(OHM[4]) * (1 - float(RES[7] == true))) - float(OHM[3]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[8] = true;  } else { RES[8] = false ; }
		TNS[6] = TNS[6] + (float(OHM[3]) * (1 - float(RES[8] == true))) - float(OHM[2]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[9] = true;  } else { RES[9] = false ; }
		TNS[6] = TNS[6] + (float(OHM[2]) * (1 - float(RES[9] == true))) - float(OHM[1]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[10] = true;  } else { RES[10] = false ; }
		for (int I = 1; I < 6; I++) {
			if (RES[I] < RES[I + 5]) { VNC[I] = min(VNC[I] + 1, VNC[0]); }
			if (RES[I] > RES[I + 5]) { VNC[I] = max(VNC[I] - 1, 0); }
			if ((RES[I] == RES[I + 5]) && (VNC[I] != 0) && (VNC[I] != VNC[0])) {VNC[I] = VNC[0] * float(RES[I]); }
			if (VNC[I] == 0) { RES[I] = false; }
			if (VNC[I] == VNC[0]) { RES[I] = true; }
		}
		break;
    case 2:
		for (int i = 1; i < 6; i++) { RES[i] = true; }
		break;
    case 3:
		for (int i = 1; i < 6; i++) { RES[i] = true; }
		break;
    default:
		for (int i = 1; i < 6; i++) { RES[i] = false; }
		break;
	}
return;}


void G_TOUCH1(int VNC[], float CPT[], boolean TUP[],boolean TEN[],boolean TDN[],int VNT[]) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_TOUCH1 - Industruino 32U8 display buttons and activity monitoring                                                     26/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_TOUCH1(int VNC[], float CPT[], boolean TUP[],boolean TEN[],boolean TDN[],int VNT[])                                   26/09/2016 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                CPT : [00] Time counter                                                                                                    *
 *                      [01] Backlight inactivity timeout in seconds                                                                         *
 *                      [02] Return to default menu Inactivity timeout in seconds                                                            *
 *                      [03] Enter button confirmation time (ms)                                                                             *
 *                      [04] Up and Down buttons confirmation time (ms)                                                                      *
 *                      [05] Controller calculated scan time                                                                                 *
 *                      [06] Millis system variable memory to calculate controller scan time                                                 *
 *                      [07] Current state of buttons analog input                                                                           *
 *                      [08] State of buttons analog input during last scan cycle                                                            *
 *                      [09] Millis system variable memory when a button was last pressed                                                    *
 *                TUP : [00] Filtered "Up" button state                                                                                      *
 *                      [01] Raw "Up" button state                                                                                           *
 *                      [02] Memory of "Up" button state during last cycle for G_TOUCH1 function                                             *
 *                      [03] Memory of "Up" button state during last cycle for G_NAVIGM function                                             *
 *                      [04] Memory of "Up" button state during last cycle for G_MENU8L or G_CHOIX8 functions                                *
 *                TEN : [00] Filtered "Enter" button state                                                                                   *
 *                      [01] Raw "Enter" button state                                                                                        *
 *                      [02] Memory of "Enter" button state during last cycle for G_TOUCH1 function                                          *
 *                      [03] Memory of "Enter" button state during last cycle for G_NAVIGM function                                          *
 *                      [04] Memory of "Enter" button state during last cycle for G_MENU8L or G_CHOIX8 functions                             *
 *                TDN : [00] Filtered "Down" button state                                                                                    *
 *                      [01] Raw "Down" button state                                                                                         *
 *                      [02] Memory of "Down" button state during last cycle for G_TOUCH1 function                                           *
 *                      [03] Memory of "Down" button state during last cycle for G_NAVIGM function                                           *
 *                      [04] Memory of "Down" button state during last cycle for G_MENU8L or G_CHOIX8 functions                              *
 *                VNT : [00] Size of used menu adress tables                                                                                 *
 *                      [01] Current screen line number in the menu adress tables                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8                                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/09/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
  CPT[5] =  max(0, (millis() - CPT[6])) / 1000;
  CPT[6] =  millis();
  CPT[7] = analogRead(A5);
  if ((CPT[7] > 100) && (CPT[7] < 400)) { TUP[1] = true; } else  { TUP[1] = false; }
  if ((CPT[7] > 400) && (CPT[7] < 700)) { TEN[1] = true; } else  { TEN[1] = false; }
  if ((CPT[7] > 700) && (CPT[7] < 1000)) { TDN[1] = true; } else  { TDN[1] = false; }
  if ((TUP[1] > TUP[2]) || (TEN[1] > TEN[2]) || (TDN[1] > TDN[2])) { CPT[9] =  millis(); }
  if (((millis() - CPT[9]) > CPT[4]) && TUP[1] && TUP[2]) { TUP[0] = true; } else  { TUP[0] = false; }
  if (((millis() - CPT[9]) > CPT[3]) && TEN[1] && TEN[2]) { TEN[0] = true; } else  { TEN[0] = false; }
  if (((millis() - CPT[9]) > CPT[4]) && TDN[1] && TDN[2]) { TDN[0] = true; } else  { TDN[0] = false; }
  if (TUP[0] || TEN[0] || TDN[0]) { digitalWrite(13, LOW); }
  TUP[2] = TUP[1];
  TEN[2] = TEN[1];
  TDN[2] = TDN[1];
  if (abs(CPT[7] - CPT[8]) < 50) { CPT[0] = CPT[0] + CPT[5]; } else  { CPT[0] = 0; }
  if (CPT[0] > CPT[1]) { digitalWrite(13, HIGH); } else  { digitalWrite(13, LOW); }
  if ((CPT[0] > CPT[2]) && (CPT[0] < (CPT[2] + 2))) { VNC[0] = VNC[1]; VNT[1] = 1; }
  CPT[0] = min(CPT[0], CPT[2] + 42);
  CPT[8] = CPT[7];
  return;
}

void G_TOUCH2(int VNC[], float CPT[], boolean TUP[],boolean TEN[],boolean TDN[],int VNT[]) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_TOUCH2 - Industruino 1286 display buttons and activity monitoring                                                     26/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_TOUCH2(int VNC[], float CPT[], boolean TUP[],boolean TEN[],boolean TDN[],int VNT[])                                   26/09/2016 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                CPT : [00] Time counter                                                                                                    *
 *                      [01] Backlight inactivity timeout in seconds                                                                         *
 *                      [02] Return to default menu Inactivity timeout in seconds                                                            *
 *                      [03] Enter button confirmation time (ms)                                                                             *
 *                      [04] Up and Down buttons confirmation time (ms)                                                                      *
 *                      [05] Controller calculated scan time                                                                                 *
 *                      [06] Millis system variable memory to calculate controller scan time                                                 *
 *                      [07] Current state of buttons analog input                                                                           *
 *                      [08] State of buttons analog input during last scan cycle                                                            *
 *                      [09] Millis system variable memory when a button was last pressed                                                    *
 *                TUP : [00] Filtered "Up" button state                                                                                      *
 *                      [01] Raw "Up" button state                                                                                           *
 *                      [02] Memory of "Up" button state during last cycle for G_TOUCH1 function                                             *
 *                      [03] Memory of "Up" button state during last cycle for G_NAVIGM function                                             *
 *                      [04] Memory of "Up" button state during last cycle for G_MENU8L or G_CHOIX8 functions                                *
 *                TEN : [00] Filtered "Enter" button state                                                                                   *
 *                      [01] Raw "Enter" button state                                                                                        *
 *                      [02] Memory of "Enter" button state during last cycle for G_TOUCH1 function                                          *
 *                      [03] Memory of "Enter" button state during last cycle for G_NAVIGM function                                          *
 *                      [04] Memory of "Enter" button state during last cycle for G_MENU8L or G_CHOIX8 functions                             *
 *                TDN : [00] Filtered "Down" button state                                                                                    *
 *                      [01] Raw "Down" button state                                                                                         *
 *                      [02] Memory of "Down" button state during last cycle for G_TOUCH1 function                                           *
 *                      [03] Memory of "Down" button state during last cycle for G_NAVIGM function                                           *
 *                      [04] Memory of "Down" button state during last cycle for G_MENU8L or G_CHOIX8 functions                              *
 *                VNT : [00] Size of used menu adress tables                                                                                 *
 *                      [01] Current screen line number in the menu adress tables                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 1286                                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/09/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
  CPT[5] =  max(0, (millis() - CPT[6])) / 1000;
  CPT[6] =  millis();
  CPT[7] = analogRead(A5);
  if (digitalRead(25) == HIGH) { TUP[1] = false; } else  { TUP[1] = true; }
  if (digitalRead(24) == HIGH) { TEN[1] = false; } else  { TEN[1] = true; }
  if (digitalRead(23) == HIGH) { TDN[1] = false; } else  { TDN[1] = true; }
  if ((TUP[1] > TUP[2]) || (TEN[1] > TEN[2]) || (TDN[1] > TDN[2])) { CPT[9] =  millis(); }
  if (((millis() - CPT[9]) > CPT[4]) && TUP[1] && TUP[2]) { TUP[0] = true; } else  { TUP[0] = false; }
  if (((millis() - CPT[9]) > CPT[3]) && TEN[1] && TEN[2]) { TEN[0] = true; } else  { TEN[0] = false; }
  if (((millis() - CPT[9]) > CPT[4]) && TDN[1] && TDN[2]) { TDN[0] = true; } else  { TDN[0] = false; }
  if (TUP[0] || TEN[0] || TDN[0]) { digitalWrite(26, LOW); }
  TUP[2] = TUP[1];
  TEN[2] = TEN[1];
  TDN[2] = TDN[1];
  if (TUP[1] || TEN[1] || TDN[1]) { CPT[0] = 0; } else  { CPT[0] = CPT[0] + CPT[5]; }
  if (CPT[0] > CPT[1]) { digitalWrite(26, HIGH); } else  { digitalWrite(26, LOW); }
  if ((CPT[0] > CPT[2]) && (CPT[0] < (CPT[2] + 2))) { VNC[0] = VNC[1]; VNT[1] = 1; }
  CPT[0] = min(CPT[0], CPT[2] + 42);
  CPT[8] = CPT[7];
  return; }

void G_MESS1L(int VNC[], int VNM, int VNL, int VDC, String TXT) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_MESS1L - Positions and prints a string on the screen within a specific menu                                           07/08/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_MESS1L(int VNC[], int VNM, int VNL, int VDC, string TXT)                                                              07/08/2015 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                VNM : Number of the screen calling the function                                                                            *
 *                VNL : Number of the line on the screen where the function will write (0 to 7)                                              *
 *                VDC : First character position in the line (0 : extreme left to 128 pixels : extreme right. 1 letter = 6 pixels)           *
 *                TXT : Text string to be written on screen in the above position (21 letters maximum)                                       *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/08/2015 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 if (VNC[0] == VNM) {
    lcd.setCursor(VDC, VNL);
    lcd.print(TXT);
	}
return; }
  

void G_VISV1B(int VNC[], int VNM, int VNL, int VDC, boolean VAL, String DSC, String TLO, String THI) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_VISV1B - Prints an animated boolean, its comment and state on one screen line                                         30/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_VISV1B(int VNC[], int VNM, int VNL, int VDC, boolean VAL, string DSC, string TLO, string THI)                         30/05/2016 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                VNM : Number of the screen calling the function                                                                            *
 *                VNL : Number of the line on the screen where the function will write (0 to 7)                                              *
 *                VDC : First character position in the line (0 : extreme left to 128 pixels : extreme right. 1 letter = 6 pixels)           *
 *                VAL : Boolean variable to display on screen                                                                                *
 *                DSC : Comment string (12 characters maximum)                                                                               *
 *                TLO : "False" State String (8 characters masimum) : describes the "false" state of the displayed bolean                    *
 *                THI : "True" State String (8 characters masimum) : describes the "true" state of the displayed bolean                      *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 05/07/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 if (VNC[0] == VNM) {
    lcd.setCursor(VDC, VNL);
    lcd.print(DSC);
    lcd.setCursor(79, VNL);
    if (VAL == true) { lcd.print(THI); } else { lcd.print(TLO); }
   }
 return; }

void G_VISV1F(int VNC[], int VNM, int VNL, int VDC, float VAL, String DSC, String UNT) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_VISV1F - Prints an animated float value, its comment and units on one screen line                                     30/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_VISV1F(int VNC[], int VNM, int VNL, int VDC, float VAL, string DSC, string UNT)                                       30/05/2016 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                VNM : Number of the screen calling the function                                                                            *
 *                VNL : Number of the line on the screen where the function will write (0 to 7)                                              *
 *                VDC : First character position in the line (0 : extreme left to 128 pixels : extreme right. 1 letter = 6 pixels)           *
 *                VAL : Float variable to display on screen                                                                                  *
 *                DSC : Comment string (10 characters maximum, depending on VDC value)                                                       *
 *                UNT : Units string (6 characters maximum)                                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 05/07/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 if (VNC[0] == VNM) {
    lcd.setCursor(VDC, VNL);
    lcd.print(DSC);
    lcd.setCursor(60, VNL);
    lcd.print(VAL);
    lcd.setCursor(102, VNL);
    lcd.print(UNT);
   }
  return; }
  
  void G_MODV1B(int VNC[], int VNM, boolean TUP[],boolean TEN[],boolean TDN[], boolean FCO[], int ARG, boolean FCM[], String TIT, String DSC, String TLO, String THI, boolean LOC) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_MODV1B - Modifies a boolean value from screen. Up turns On, Down turns Off, Enter validates                           23/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_MODV1B(i VNC[],i VNM,b TUP[],b TEN[],b TDN[],b FCO[],i ARG,S TIT,S DSC,S TLO,S THI,b LOC)                             23/06/2016 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                VNM : Number of the screen calling the function                                                                            *
 *                TUP : [00] Filtered "Up" button state                                                                                      *
 *                      [01] Raw "Up" button state                                                                                           *
 *                      [02] Memory of "Up" button state during last cycle for G_TOUCH1 function                                             *
 *                      [03] Memory of "Up" button state during last cycle for G_NAVIGM function                                             *
 *                      [04] Memory of "Up" button state during last cycle for G_MENU8L or G_CHOIX8 functions                                *
 *                TEN : [00] Filtered "Enter" button state                                                                                   *
 *                      [01] Raw "Enter" button state                                                                                        *
 *                      [02] Memory of "Enter" button state during last cycle for G_TOUCH1 function                                          *
 *                      [03] Memory of "Enter" button state during last cycle for G_NAVIGM function                                          *
 *                      [04] Memory of "Enter" button state during last cycle for G_MENU8L or G_CHOIX8 functions                             *
 *                TDN : [00] Filtered "Down" button state                                                                                    *
 *                      [01] Raw "Down" button state                                                                                         *
 *                      [02] Memory of "Down" button state during last cycle for G_TOUCH1 function                                           *
 *                      [03] Memory of "Down" button state during last cycle for G_NAVIGM function                                           *
 *                      [04] Memory of "Down" button state during last cycle for G_MENU8L or G_CHOIX8 functions                              *
 *                VAL : [00] Array in which the value to modify is included (Has to be an array of at least 1 in size)                       *
 *                      [...] Other boolean values                                                                                           *
 *                ARG : Integer - line of VAL[] array to modify                                                                              *
 *                VAM : [00] Temporary memory value (Has to be an array of at least 1 in size)                                               *
 *                TIT : 21 characters long string ending plus [space] at the end - Description line 1 of the modified value                  *
 *                DSC : 21 characters long string ending plus [space] at the end - Description line 1 of the modified value                  *
 *                TLO : "False" State String (8 characters masimum) : describes the "false" state of the modified bolean                     *
 *                THI : "True" State String (8 characters masimum)  : describes the "true"  state of the modified bolean                     *
 *                LOC : Boolean - Value change lock : change is possble if True, locked if False                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 23/06/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 if (VNC[0] == VNM) {
	byte FVH[5]= { B00001000, B00001100, B00001110, B00001100, B00001000};
	lcd.createChar(0, FVH);
	byte FEN[5]= { B00010000, B00111000, B01111100, B00010000, B00011110};
	lcd.createChar(1, FEN);
	byte FVB[5]= { B00000100, B00001100, B00011100, B00001100, B00000100};
	lcd.createChar(2, FVB);
	if ((LOC == true) && (TUP[0] > TUP[4])) { FCM[0] = true; }
	if ((LOC == true) && (TDN[0] > TDN[4])) { FCM[0] = false; }
	if ((LOC == true) && (TEN[0] > TEN[4])) { FCO[ARG] = FCM[0]; }
	lcd.setCursor(0, 0);	lcd.clearLine();	lcd.print(TIT);
	lcd.setCursor(0, 1);	lcd.clearLine();	lcd.print(DSC);
	lcd.setCursor(0, 2);	lcd.clearLine();	lcd.print("Mode:");
	lcd.setCursor(30, 2);	if (LOC == true) {lcd.print(" Unlocked    ");} else {lcd.print(" Locked      ");} 
	lcd.setCursor(0, 3);	lcd.clearLine();
	lcd.setCursor(10, 3);	lcd.write(byte(0));
	lcd.setCursor(24, 3);	lcd.print(": Activate  ");
	lcd.setCursor(0, 4);	lcd.clearLine();	lcd.print("Last:");
	lcd.setCursor(50, 4);	if (FCO[ARG] == true) {lcd.print(THI);} else {lcd.print(TLO);} 
	lcd.setCursor(0, 5);	lcd.clearLine();	lcd.print("New :");
	lcd.setCursor(50, 5);	if (FCM[0] == true) {lcd.print(THI);} else {lcd.print(TLO);} 
	lcd.setCursor(0, 6);	lcd.clearLine();
	lcd.setCursor(10, 6);	lcd.write(byte(2));
	lcd.setCursor(24, 6);	lcd.print(": Deactivate     ");
	lcd.setCursor(0, 7);	lcd.clearLine();
	lcd.setCursor(10, 7);	lcd.write(byte(1));
	lcd.setCursor(24, 7);	lcd.print(": Confirm+Return ");
	TUP[4] = TUP[0];
	TEN[4] = TEN[0];
	TDN[4] = TDN[0];
	}
  return; }

void G_NAVIGM(int VNC[], int VNT[], boolean TUP[], boolean TEN[], boolean TDN[], int VNU[], int VNE[], int VND[]) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_NAVIGM - Navigation générale entre les écrans                                                                         26/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_NAVIGM(int VNC[], int VNT[], boolean TUP[], boolean TEN[], boolean TDN[], int VNU[], int VNE[], int VND[])            26/09/2016 *
 *                VNC : [00] Adress of current menu displayed on screen                                                                      *
 *                      [01] Adress of default menu page                                                                                     *
 *                      [...] Other menu adresses                                                                                            *
 *                VNT : [0] Size of the destinations tables                                                                                  *
 *                      [1] Destinations table line number corresponding to the current menu page on screen                                  *
 *                TUP : [00] Filtered "Up" button state                                                                                      *
 *                      [01] Raw "Up" button state                                                                                           *
 *                      [02] Memory of "Up" button state during last cycle for G_TOUCH1 function                                             *
 *                      [03] Memory of "Up" button state during last cycle for G_NAVIGM function                                             *
 *                      [04] Memory of "Up" button state during last cycle for G_MENU8L or G_CHOIX8 functions                                *
 *                TEN : [00] Filtered "Enter" button state                                                                                   *
 *                      [01] Raw "Enter" button state                                                                                        *
 *                      [02] Memory of "Enter" button state during last cycle for G_TOUCH1 function                                          *
 *                      [03] Memory of "Enter" button state during last cycle for G_NAVIGM function                                          *
 *                      [04] Memory of "Enter" button state during last cycle for G_MENU8L or G_CHOIX8 functions                             *
 *                TDN : [00] Filtered "Down" button state                                                                                    *
 *                      [01] Raw "Down" button state                                                                                         *
 *                      [02] Memory of "Down" button state during last cycle for G_TOUCH1 function                                           *
 *                      [03] Memory of "Down" button state during last cycle for G_NAVIGM function                                           *
 *                      [04] Memory of "Down" button state during last cycle for G_MENU8L or G_CHOIX8 functions                              *
 *                VNU : [00] Menu adress of destination if button "Up" is pressed in the current menu                                        *
 *                      [01] Menu adress of destination if button "Up" is pressed on home page                                               *
 *                      [xx] Menu adress of destination if button "Up" is pressed in the menu with VNC[xx] as an adress                      *
 *                VNE : [00] Menu adress of destination if button "Enter" is pressed in the current menu                                     *
 *                      [01] Menu adress of destination if button "Enter" is pressed on home page                                            *
 *                      [xx] Menu adress of destination if button "Enter" is pressed in the menu with VNC[xx] as an adress                   *
 *                VND : [00] Menu adress of destination if button "Down" is pressed in the current menu                                      *
 *                      [01] Menu adress of destination if button "Down" is pressed on home page                                             *
 *                      [xx] Menu adress of destination if button "Down" is pressed in the menu with VNC[xx] as an adress                    *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/09/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
if ((VNU[VNT[1]] != VNE[VNT[1]]) || (VND[VNT[1]] != VNE[VNT[1]]) || (VNU[VNT[1]] != VND[VNT[1]])) {
    if ((TUP[0] > TUP[3]) && (VNC[VNT[1]] != VNU[VNT[1]])) {VNC[0] = VNU[VNT[1]]; }
    if ((TEN[0] > TEN[3]) && (VNC[VNT[1]] != VNE[VNT[1]])) {VNC[0] = VNE[VNT[1]]; }
    if ((TDN[0] > TDN[3]) && (VNC[VNT[1]] != VND[VNT[1]])) {VNC[0] = VND[VNT[1]]; }
    if (VNC[0] != VNC[VNT[1]]) {
		for (int i = 0; i <= VNT[0]; i++) {
			if  (VNC[0] == VNC[i]) { VNT[1] = i; }
		}
	}
    TUP[3] = TUP[0];
    TEN[3] = TEN[0];
    TDN[3] = TDN[0];
  }
  return; }
