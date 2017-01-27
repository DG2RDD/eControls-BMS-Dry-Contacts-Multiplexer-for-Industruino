/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - Fonction de démultiplexage 5DI / 1 AI sur une entrée en tension - Version autonome                           19/12/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Bibliothèques utilisées  - Aucune (version autonome)                                                                           18/12/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Inclusion des bibliothèques nécessaires au fonctionnement du programme -------------------------------------------------------------------*
#include <Wire.h>					// Library  - Comm Ports
#include <Indio.h>					// Library  - Contrôle des entrées-sorties Industruino
#include <UC1701.h>
static UC1701 lcd;

// Configuration des entrées et sorties -----------------------------------------------------------------------------------------------------*
float EA1_MII_MPLX;					// General  - Entrée analogique en tension pour connecter l'entrée multiplexée
float VTE_MPLX[7] ;					// Multi_V  - Tensions calculées le long de la chaine de contacts
int VNC_MPLX[6];					// Multi-V  - Compteurs confirmation de transition des contacts
boolean EM1_MII_MPLX[11];				// Multi-V  - Entrées démultiplexées

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[11] = {					// LCDScreen - Table des chaînes de description
  "                     ", // 00
  "Haut, Enter : Accueil",
  "AI1 Value            ",
  "MI1 Contact          ",
  "MI2 Contact          ",
  "MI3 Contact          ",
  "MI4 Contact          ",
  "MI5 Contact          ",
  "- Down:Zero Setting -",
  "-   Zero  Setting   -",
  " "};                   // 10

String TCM_UNIT[15] = {					// LCDScreen - Table des chaînes d'unités standard
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
  " Running ",
  "   Learn "} ;

boolean FCO_PRUP[5];					// LCDScreen - Appui sur le bouton "Up"
boolean FCO_PREN[5];					// LCDScreen - Appui sur le bouton "Enter"
boolean FCO_PRDN[5];					// LCDScreen - Appui sur le bouton "Down"
int VNT_MENU[3];					// LCDScreen - Table des pointeurs de menus
int VNC_MENU[30];					// LCDScreen - Table des adresses de menus
int VNC_PRUP[31];					// LCDScreen - Table des pointeurs de menus si la touche "Up" est pressée
int VNC_PREN[30];					// LCDScreen - Table des pointeurs de menus si la touche "Enter" est pressée
int VNC_PRDN[30];					// LCDScreen - Table des pointeurs de menus si la touche "Down" est pressée
float VTT_SCRN[10];					// LCDScreen - Table des temporisations

boolean FCM_MPX1[1];					// G_MODV1B - Commande de mémorisation de tension de référence par l'écran LCD

void setup() {						// Initialisation ----------------------------------------------------------------------------------*
I_INPS();
I_DISP();
Serial.begin(9600);
}

void loop() {						// Execution boucle principale ---------------------------------------------------------------------*
P_INPS();
P_DISP();
}

void I_INPS() {						// Initialisation des entrées ---------------------------------------------------------------- INIT *
Indio.setADCResolution(14);				// System   - Résolution des EA. Valeurs possibles : 12bit@240Lps, 14bit@60Lps, 16bit@15Lps and 18bit@3.75Lps.
Indio.analogReadMode(1, V10_p);			        // System   - EA1 paramétrée en mode de mesure 0-10V / 0-100%
VTE_MPLX[2] = 150;					// Offset de détection des transitions en mV
VTE_MPLX[3] = VTE_MPLX[1] * (1 - (10110 / 14715));
VTE_MPLX[4] = VTE_MPLX[3];
VNC_MPLX[0] = 3;					// Nombre d'états successifs de même valeur pour confirmer une transition dans la valeur d'entrée filtrée
return;
}

void P_INPS() {                                         // Lecture des entrées ----------------------------------------------------------------------- EXEC *
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

void I_DISP() {						// Initialisation des paramètres de l'écran -------------------------------------------------- INIT *
VTT_SCRN[1] = 180;					// G_TOUCHx - Durée d'inactivité pour arrêter le rétroéclairage
VTT_SCRN[2] = 240;					// G_TOUCHx - Durée d'inactivité pour retourner à l'écran par défaut
VTT_SCRN[3] = 20;					// G_TOUCHx - Temporisation de filtre sur le bouton "Enter"
VTT_SCRN[4] = 15;					// G_TOUCHx - Temporisation de filtre sur les boutons "Up" et "Down"
VNT_MENU[0] = 3;					// G_NAVIGM - Taille de la table d'adresses utilisées
VNT_MENU[1] = 1;					// G_NAVIGM - Initialisation du pointeur d'adresses
VNC_MENU[0] = 1;					// G_NAVIGM - Table des adresses de menus
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_MENU[3] = 3;
			  				// G_NAVIGM - Table des adresses de destination de chaque bouton dans chaque menu
VNC_PRUP[1] =   1; VNC_PREN[1] =   2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   1; VNC_PREN[2] =   1; VNC_PRDN[2] =   3;
VNC_PRUP[3] =   3; VNC_PREN[3] =   2; VNC_PRDN[3] =   3;
VNC_PRUP[4] =   2;
 
pinMode(23, INPUT);					// LCDScreen - 1286 seulement - Adresse du bouton Down
pinMode(24, INPUT);					// LCDScreen - 1286 seulement - Adresse du bouton Enter
pinMode(25, INPUT);					// LCDScreen - 1286 seulement - Adresse du bouton Up
pinMode(26, OUTPUT);					// LCDScreen - 1286 seulement - Adresse de la sortie de rétroéclairage
/*
pinMode(13, OUTPUT);                    		// LCDScreen - 32U8 seulement - Adresse de la sortie de rétroéclairage
*/
lcd.begin();						// LCDScreen - Initialisation de l'écran
lcd.clear();						// LCDScreen - Effacement de l'écran
return; }

void P_DISP() {						// Screen manus and displays ---------------------------------------------------------------- EXEC *
//G_TOUCH1(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);	// LCDScreen - 32U8 only - Suivi des boutons de façade
  G_TOUCH2(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);	// LCDScreen - 1286 only - Suivi des boutons de façade
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 0); lcd.clearLine(); }
  G_MESS1L(VNC_MENU, 1, 1, 0, "   www.econtrols.fr  ");
  G_MESS1L(VNC_MENU, 1, 2, 0, "Automation & Controls");
  G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  1286  ");
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 4); lcd.clearLine(); }
  G_MESS1L(VNC_MENU, 1, 5, 0, "Multiplexeur  5DI/1AI");
  G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
  G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.00 FSA 19/12/2016");
  
  G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
  G_VISV1F(VNC_MENU, 2, 1, 0, EA1_MII_MPLX * 100, TCM_PRES[2], TCM_UNIT[5]);
  G_VISV1B(VNC_MENU, 2, 2, 0, EM1_MII_MPLX[1], TCM_PRES[3], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 3, 0, EM1_MII_MPLX[2], TCM_PRES[4], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 4, 0, EM1_MII_MPLX[3], TCM_PRES[5], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 5, 0, EM1_MII_MPLX[4], TCM_PRES[6], TCM_UNIT[11], TCM_UNIT[12]);
  G_VISV1B(VNC_MENU, 2, 6, 0, EM1_MII_MPLX[5], TCM_PRES[7], TCM_UNIT[11], TCM_UNIT[12]);
  G_MESS1L(VNC_MENU, 2, 7, 0, TCM_PRES[8]);

  G_MODV1B(VNC_MENU, 3, FCO_PRUP, FCO_PREN, FCO_PRDN, EM1_MII_MPLX, 0, FCM_MPX1, "-Multiplexeur  5D/1A-", "EA1 - reglage du Zero", TCM_UNIT[13], TCM_UNIT[14], true);
  G_MODV1B(VNC_MENU, 3, FCO_PRUP, FCO_PREN, FCO_PRDN, EM1_MII_MPLX, 0, FCM_MPX1, "- 5D/1A Multiplexer -", "EA1 - Zero Setting   ", TCM_UNIT[13], TCM_UNIT[14], true);

  G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
  return; }

// Functions used by the program -------------------------------------------------------------------------------------------------------------

void T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - Fonction de démultiplexage 5DI / 1 AI sur une entrée en tension                             (Compatible IFS) 07/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG)                                                     *
 *                MES : float, lecture en tension 0-10V vers un pourcentage (V10_p) de l'entrée analogique                                   *
 *                OHM : [00] Valeur de la résistance de charge de base                                                                       *
 *                      [01] Valeur de la résistance de charge de l'entrée digitale 1 du multiplexeur                                        *
 *                      [02] Valeur de la résistance de charge de l'entrée digitale 2 du multiplexeur                                        *
 *                      [03] Valeur de la résistance de charge de l'entrée digitale 3 du multiplexeur                                        *
 *                      [04] Valeur de la résistance de charge de l'entrée digitale 4 du multiplexeur                                        *
 *                      [05] Valeur de la résistance de charge de l'entrée digitale 5 du multiplexeur                                        *
 *                TNS : [00] Calcul de la tension en aval de la résistance de charge RES[0] selon la configuration des RES[1] à [5]          *
 *                      [01] Tension d'alimentation de référence : 24000 mV                                                                  *
 *                      [02] Différentiel de tension de compensation des imprécisions de résistances : 150 à 200 mV                          *
 *                      [03] Tension en aval de la résistance de charge RES[0] mesurée à vide pour une alimentation de référence 24Vdc       *
 *                      [04] Tension en aval de la résistance de charge RES[0] mesurée à vide en conditions réelles                          *
 *                      [05] Tension d'alimentation réelle recalculée à partir de TNS[3] et TNS[4]                                           *
 *                      [06] Somme des résistances de charge en ligne à chaque instant de [00] à [05] au format float                        *
 *                VNC : [00] Nombre de cycles de confirmation antirebond d'une transition d'une des entrées RES                              *
 *                      [01] Compteur de nombre de cycles de différence continue entre RES[1] et RES[6]                                      *
 *                      [02] Compteur de nombre de cycles de différence continue entre RES[2] et RES[7]                                      *
 *                      [03] Compteur de nombre de cycles de différence continue entre RES[3] et RES[8]                                      *
 *                      [04] Compteur de nombre de cycles de différence continue entre RES[4] et RES[9]                                      *
 *                      [05] Compteur de nombre de cycles de différence continue entre RES[5] et RES[10]                                     *
 *                RES : [00] Commande de mémorisation de la tension à vide du multiplexeur à la calibration                                  *
 *                      [01] Etat calculé de l'entrée digitale 1 du multiplexeur après traitement antirebond                                 *
 *                      ...                                                                                                                  *
 *                      [05] Etat calculé de l'entrée digitale 5 du multiplexeur après traitement antirebond                                 *
 *                      [06] Etat calculé de l'entrée digitale 1 du multiplexeur avant traitement antirebond                                 *
 *                      ...                                                                                                                  *
 *                      [10] Etat calculé de l'entrée digitale 5 du multiplexeur avant traitement antirebond                                 *
 *                CFG : Commande de forçage                                                                                                  *
 *                      <0  : les booléens RES[1] à [5] restent false quelle que soit la valeur de MPX[RNG]                                  *
 *                      0   : les booléens RES[1] à [5] restent figés à leur valeur courante                                                 *
 *                      1   : les booléens RES[1] à [5] suivent les valeurs de MES décodé suivant les valeurs de tensions                    *
 *                      2   : les booléens RES[1] à [5] restent true quelle que soit la valeur de MPX[RNG]                                   *
 *                      3   : les booléens RES[1] à [5] restent true quelle que soit la valeur de MPX[RNG]                                   *
 *                      >3  : les booléens RES[1] à [5] restent false quelle que soit la valeur de MPX[RNG]                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/09/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
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
 * SE2R : G_TOUCH1 - Gestion des touches de l'écran Industruino 32U8                                                              26/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_TOUCH1(int VNC[], float CPT[], boolean TUP[],boolean TEN[],boolean TDN[],int VNT[])                                              *
 *                VNC : [00] Numéro du menu en cours d'affichage                                                                             *
 *                      [01] Numéro du menu d'accueil                                                                                        *
 *                      [...] Menus suivants                                                                                                 *
 *                CPT : [00] Compteur de temporisation                                                                                       *
 *                      [01] Tempo de maintien allumé de l'écran après l'action sur une touche                                               *
 *                      [02] Tempo d'inactivité de retour à l'écran d'accueil (s)                                                            *
 *                      [03] Tempo de confirmation de la touche Enter (ms)                                                                   *
 *                      [04] Tempo de confirmation des touches de flêches (ms)                                                               *
 *                      [05] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [06] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                      [07] Lecture de l'état de l'entrée analogique de synthèse des touches                                                *
 *                      [08] Mémorisation de l'état de l'entrée analogique de synthèse des touches du cycle n-1                              *
 *                      [09] Mémorisation millis() lors au début de la dernière action sur une touche                                        *
 *                TUP : [00] Etat de la touche "Up" après traitement antirebond                                                              *
 *                      [01] Etat brut de la touche "Up"                                                                                     *
 *                      [02] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_TOUCH1                                   *
 *                      [03] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_NAVIGM                                   *
 *                      [04] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_MENU8L ou G_CHOIX8                       *
 *                TEN : [00] Etat de la touche "Enter" après traitement antirebond                                                           *
 *                      [01] Etat brut de la touche "Enter"                                                                                  *
 *                      [02] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_TOUCH1                                *
 *                      [03] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_NAVIGM                                *
 *                      [04] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_MENU8L ou G_CHOIX8                    *
 *                TDN : [00] Etat de la touche "Down" après traitement antirebond                                                            *
 *                      [01] Etat brut de la touche "Down"                                                                                   *
 *                      [02] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_TOUCH1                                 *
 *                      [03] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_NAVIGM                                 *
 *                      [04] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_MENU8L ou G_CHOIX8                     *
 *                VNT : [0] Taille des tables de navigation                                                                                  *
 *                      [1] Numéro de la ligne des tables correspondant à l'écran en cours                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U6                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/09/2016 : SE2R, DD : Testée et validée sur banc                                                                                        *
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
 * SE2R : G_TOUCH2 - Gestion des touches de l'écran Industruino 1286                                                              26/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_TOUCH2(int VNC[], float CPT[], boolean TUP[],boolean TEN[],boolean TDN[],int VNT[])                                              *
 *                VNC : [00] Numéro du menu en cours d'affichage                                                                             *
 *                      [01] Numéro du menu d'accueil                                                                                        *
 *                      [...] Menus suivants                                                                                                 *
 *                CPT : [00] Compteur de temporisation                                                                                       *
 *                      [01] Tempo de maintien allumé de l'écran après l'action sur une touche                                               *
 *                      [02] Tempo d'inactivité de retour à l'écran d'accueil (s)                                                            *
 *                      [03] Tempo de confirmation de la touche Enter (ms)                                                                   *
 *                      [04] Tempo de confirmation des touches de flêches (ms)                                                               *
 *                      [05] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [06] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                      [07] Lecture de l'état de l'entrée analogique de synthèse des touches                                                *
 *                      [08] Mémorisation de l'état de l'entrée analogique de synthèse des touches du cycle n-1                              *
 *                      [09] Mémorisation millis() lors au début de la dernière action sur une touche                                        *
 *                TUP : [00] Etat de la touche "Up" après traitement antirebond                                                              *
 *                      [01] Etat brut de la touche "Up"                                                                                     *
 *                      [02] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_TOUCH2                                   *
 *                      [03] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_NAVIGM                                   *
 *                      [04] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_MENU8L ou G_CHOIX8                       *
 *                TEN : [00] Etat de la touche "Enter" après traitement antirebond                                                           *
 *                      [01] Etat brut de la touche "Enter"                                                                                  *
 *                      [02] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_TOUCH2                                *
 *                      [03] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_NAVIGM                                *
 *                      [04] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_MENU8L ou G_CHOIX8                    *
 *                TDN : [00] Etat de la touche "Down" après traitement antirebond                                                            *
 *                      [01] Etat brut de la touche "Down"                                                                                   *
 *                      [02] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_TOUCH2                                 *
 *                      [03] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_NAVIGM                                 *
 *                      [04] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_MENU8L ou G_CHOIX8                     *
 *                VNT : [0] Taille des tables de navigation                                                                                  *
 *                      [1] Numéro de la ligne des tables correspondant à l'écran en cours                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 1286                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/09/2016 : SE2R, DD : Testée et validée sur banc                                                                                        *
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
 * SE2R : G_MESS1L - Fonction d'affichage à l'écran d'une ligne de caractères au choix                                            07/08/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_MESS1L(int VNC[], int VNM, int VNL,  int VDC, string TXT)                                                                        *
 *                VNC : [00] Numéro du menu en cours d'affichage                                                                             *
 *                      [01] Numéro du menu d'accueil                                                                                        *
 *                      [...] Menus suivants                                                                                                 *
 *                VNM : Numéro du menu auquel appartient l'appel de fonction                                                                 *
 *                VNL : Numéro de la ligne d'écran où la fonction doit écrire                                                                *
 *                VDC : Position de début d'écriture dans la ligne (de 0 : gauche à 128 : droite. 1 caractère = 6 pixels)                    *
 *                TXT : Chaîne de caractères de description de la valeur représentée (21 caractères maximum)                                 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U6, 1286                                                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/08/2015 : SE2R, DD : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 if (VNC[0] == VNM) {
    lcd.setCursor(VDC, VNL);
    lcd.print(TXT);
	}
return; }
  

void G_VISV1B(int VNC[], int VNM, int VNL, int VDC, boolean VAL, String DSC, String TLO, String THI) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : G_VISV1B - Fonction d'affichage à l'écran de l'état des E/S digitales, texte d'état paramétrable                        30/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_VISV1B(int VNC[], int VNM, int VNL, int VDC, boolean VAL, string DSC, string TLO, string THI)                                    *
 *                VNC : [00] Numéro du menu en cours d'affichage                                                                             *
 *                VNM : Numéro du menu auquel appartient l'appel de fonction                                                                 *
 *                VNL : Numéro de la ligne d'écran où la fonction doit écrire                                                                *
 *                VDC : Position de début d'écriture dans la ligne (de 0 : gauche à 128 : droite. 1 caractère = 6 pixels)                    *
 *                VAL : Variable digitale dont on souhaite afficher l'état                                                                   *
 *                DSC : Chaîne de caractères de description de la valeur représentée (12 caractères masimum)                                 *
 *                TLO : Chaîne de caractères représentant l'état "Low" de la voie digitale  (8 caractères masimum)                           *
 *                THI : Chaîne de caractères représentant l'état "High" de la voie digitale (8 caractères masimum)                           *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U6, 1286                                                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 05/07/2016 : SE2R, DD : Testée et validée sur banc                                                                                        *
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
 * SE2R : G_VISV1F - Fonction d'affichage à l'écran d'une valeur analogique sur 1 ligne                                           30/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_VISV1F(int VNC[], int VNM, int VNL, int VDC, float VAL, string DSC, string UNT)                                                  *
 *                VNC : [0] Numéro du menu en cours d'affichage                                                                              *
 *                VNM : Numéro du menu auquel appartient l'appel de fonction                                                                 *
 *                VNL : Numéro de la ligne d'écran où la fonction doit écrire                                                                *
 *                VDC : Position de début d'écriture dans la ligne (de 0 : gauche à 128 : droite. 1 caractère = 6 pixels)                    *
 *                VAL : Nom de la valeur à afficher                                                                                          *
 *                DSC : Chaîne de caractères de description de la valeur (10 caractères maximum, 9 si décalage)                              *
 *                UNT : Chaîne de caractères des unités de la valeur (6 caractères maximum)                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U6, 1286                                                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 05/07/2016 : SE2R, DD : Testée et validée sur banc                                                                                        *
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
 * SE2R : G_MODV1B - Fonction de consignation à l'écran d'une valeur de type Booléen                                              23/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        G_MODV1B(i VNC[],i VNM,b TUP[],b TEN[],b TDN[],b FCO[],i ARG,S TIT,S DSC,S TLO,S THI,b LOC)                                        *
 *                VNC : [00] Numéro du menu en cours d'affichage                                                                             *
 *                VNM : Numéro du menu parmi les menus                                                                                       *
 *                TUP : [00] Etat de la touche "Up" après traitement antirebond                                                              *
 *                      [01] Etat brut de la touche "Up"                                                                                     *
 *                      [02] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_TOUCHD                                   *
 *                      [03] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_NAVIGM                                   *
 *                      [04] Mémorisation de l'état de la touche "Up" du cycle n-1 des fonctions G_MENU8L et G_MODV1x                        *
 *                TEN : [00] Etat de la touche "Enter" après traitement antirebond                                                           *
 *                      [01] Etat brut de la touche "Enter"                                                                                  *
 *                      [02] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_TOUCHD                                *
 *                      [03] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_NAVIGM                                *
 *                      [04] Mémorisation de l'état de la touche "Enter" du cycle n-1 des fonctions G_MENU8L et G_MODV1x                     *
 *                TDN : [00] Etat de la touche "Down" après traitement antirebond                                                            *
 *                      [01] Etat brut de la touche "Down"                                                                                   *
 *                      [02] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_TOUCHD                                 *
 *                      [03] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_NAVIGM                                 *
 *                      [04] Mémorisation de l'état de la touche "Down" du cycle n-1 des fonctions G_MENU8L et G_MODV1x                      *
 *                VAL : [00] Valeur à modifier (Nécessairement un tableau de taille au moins 1)                                              *
 *                ARG : Entier - Rang de la valeur à mettre à jour dans le tableau VAL[]                                                     *
 *                VAM : [00] Valeur mémorisée entrée par l'utilisateur (Nécessairement un tableau de taille au moins 1)                      *
 *                TIT : Chaîne de 21 caractères se terminant par un espace - Description de la valeur paramétrable ligne 1                   *
 *                DSC : Chaîne de 21 caractères se terminant par un espace - Description de la valeur paramétrable ligne 2                   *
 *                TLO : Chaîne de caractères représentant l'état "False" de l'information (8 caractères maximum justifiés à droite)          *
 *                THI : Chaîne de caractères représentant l'état "True"  de l'information (8 caractères maximum justifiés à droite)          *
 *                LOC : Booléen - Changement de consigne autorisé par la fonction si On, bloqué si Off                                       *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U8 et Industruino 1286                                                                               *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 23/06/2016 : SE2R, DD : Testée et validée sur banc                                                                                        *
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
 *        G_NAVIGM(int VNC[], int VNT[], boolean TUP[], boolean TEN[], boolean TDN[], int VNU[], int VNE[], int VND[])                       *
 *                VNC : [0] Numéro du menu en cours d'affichage                                                                              *
 *                      [1] Numéro du menu d'accueil                                                                                         *
 *                      [x] Numéro des autres écrans de l'afficheur (dizaines : niveau / unités : n° du menu dans le niveau)                 *
 *                VNT : [0] Taille des tables de navigation                                                                                  *
 *                      [1] Numéro de la ligne des tables correspondant à l'écran en cours                                                   *
 *                TUP : [0] Etat de la touche "Up" après traitement antirebond                                                               *
 *                      [1] Etat brut de la touche "Up"                                                                                      *
 *                      [2] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_TOUCHD                                    *
 *                      [3] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_NAVIGM                                    *
 *                      [4] Mémorisation de l'état de la touche "Up" du cycle n-1 de la fonction G_MENU8L ou G_ASCG1x                        *
 *                TEN : [0] Etat de la touche "Enter" après traitement antirebond                                                            *
 *                      [1] Etat brut de la touche "Enter"                                                                                   *
 *                      [2] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_TOUCHD                                 *
 *                      [3] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_NAVIGM                                 *
 *                      [4] Mémorisation de l'état de la touche "Enter" du cycle n-1 de la fonction G_CHOIX8 ou G_ASCG1x                     *
 *                TDN : [0] Etat de la touche "Down" après traitement antirebond                                                             *
 *                      [1] Etat brut de la touche "Down"                                                                                    *
 *                      [2] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_TOUCHD                                  *
 *                      [3] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_NAVIGM                                  *
 *                      [4] Mémorisation de l'état de la touche "Down" du cycle n-1 de la fonction G_MENU8L ou G_ASCG1x                      *
 *                VNU : [0] Numéro du menu de destination pour la touche "Up" pressée dans le menu courant                                   *
 *                      [1] Numéro du menu de destination pour la touche "Up" pressée dans l'écran d'accueil                                 *
 *                      [x] Numéro du menu de destination pour la touche "Up" pressée dans l'écran correspondant de la table VNC             *
 *                VNE : [0] Numéro du menu de destination pour la touche "Enter" pressée dans le menu courant                                *
 *                      [1] Numéro du menu de destination pour la touche "Enter" pressée dans l'écran d'accueil                              *
 *                      [x] Numéro du menu de destination pour la touche "Enter" pressée dans l'écran correspondant de la table VNC          *
 *                VND : [0] Numéro du menu de destination pour la touche "Down" pressée dans le menu courant                                 *
 *                      [1] Numéro du menu de destination pour la touche "Down" pressée dans l'écran d'accueil                               *
 *                      [x] Numéro du menu de destination pour la touche "Down" pressée dans l'écran correspondant de la table VNC           *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/09/2016 : SE2R, DD : Testée et validée sur banc                                                                                        *
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
