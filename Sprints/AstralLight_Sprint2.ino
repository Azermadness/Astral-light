/* ------------------------------- */
/*    Imports des bibliothèques    */
/* ------------------------------- */
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "ILI9341_due_config.h"
#include "ILI9341_due.h"
#include "SystemFont5x7.h"
#include "SPI.h"
#include "math.h"

/* ------------------------------- */
/* Ports de l'afficheurs */
/* ------------------------------- */
#define TFT_DC 9
#define TFT_CS 10

/* ------------------------------- */
/* Different type d'orientation de l'écran */
/* ------------------------------- */
#define PORTRAIT 0
#define LANDSCAPE 1

/* ------------------------------- */
/* Constante contenant le code héxadécimal des couleurs */
/* Convertion ds couleurs en code RGB565 sur: http://www.barth-dev.de/online/rgb565-color-picker/ */
/* ------------------------------- */
#define BLACK         0x0000
#define BLUE          0x001F
#define RED           0xF800
#define GREEN         0x7700
#define CYAN          0x07FF
#define MAGENTA       0xF81F
#define YELLOW        0xFFE0
#define WHITE         0xFFFF
#define BROWN         0x8200
#define GRAY          0x31A6
#define LIGHTGRAY     0xB596
#define VERYLIGHTGRAY 0xAD75
#define LIME          0x5FA0
#define LIGHTRED      0xFA49
#define ORANGE        0xFC08
#define REDF          0xF800

#define SCORING1      0xFCC0
#define SCORING2      0xFE60
#define HIGHLIGHT     0xE71C

#define PORTALA       0x07DF
#define PORTALB       0x2D1F
#define PORTALC       0x52DF
#define PORTALD       0xA93F
#define PORTALE       0xF81F


/* ------------------------------- */
/* Longueur en pixel de l'écran */
#define SCREENWIDTH 320 // Equivalent à tft.width()
/* Largeur en pixel de l'écran */
#define SCREENHEIGHT 240 // Equivalent à tft.height()
// Taille d'une casse
#define BOX_SIZE 20
#define PLAYER_HEIGHT 40
#define MAP_COLS 16
#define MAP_ROWS 12

#define PSTARTX 30 // position de départ du joueur
#define PSTARTY 150

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
ILI9341_due tft2 = ILI9341_due(TFT_CS, TFT_DC);

//=========================================================================================

//setup variables

int screenSave[200];
int incr;
int tabx;
int tabx2;
int taby;
int taby2;
int taby3;
int nextRightWall;
int nextLeftWall;
int nextCeilling;
int nextFloor;
int lifeCount;
bool ground[MAP_ROWS][MAP_COLS] = {
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false},
  {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
  {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
  {true , false, false, false, false, false, false, false, false, false, false, false, true , false, false, true },
  {true , false, false, false, false, false, false, false, false, false, false, true , true , false, false, true },
  {true , false, false, false, false, false, false, false, false, false, true , true , true , false, false, true },
  {true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true },
};
int ground_Salle1[MAP_ROWS][MAP_COLS] = {
  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
  {15, 13, 14, 12, 10, 15, 10, 13, 10, 14, 10, 13, 11, 15, 10, 13},
  {10, 12, 11, 10, 14, 10, 12, 15, 11, 13, 12, 15, 14, 10, 12, 14},
  {5 , 10, 10, 13, 11, 14, 10, 10, 10, 12, 10, 14, 12, 10, 11, 13},
  {2 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 14, 12, 15, 11},
  {1 , 13, 14, 11, 12, 10, 15, 10, 11, 10, 13, 12, 11, 10, 14, 10},
  {1 , 10, 15, 10, 13, 10, 11, 10, 13, 11, 14, 15, 10, 12, 11, 2 },
  {1 , 14, 12, 11, 10, 13, 15, 14, 12, 10, 13, 10, 14, 15, 13, 1 },
  {1 , 11, 13, 10, 15, 12, 10, 12, 10, 15, 11, 10, 2 , 10, 11, 1 },
  {1 , 12, 10, 11, 14, 10, 15, 13, 11, 12, 14, 2 , 1 , 12, 11, 1 },
  {1 , 15, 14, 15, 13, 11, 10, 15, 10, 10, 2 , 1 , 1 , 4 , 4 , 1 },
  {1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 1 , 1 , 2 , 2 , 1 },
};

//controles

bool leftPressed;
bool rightPressed;
bool upPressed;
bool downPressed;

bool redPressed;
bool greenPressed;

//autres

unsigned long timebuffer;
unsigned long refresh = 100;
unsigned long timePassed;
bool gameSetup;
bool homeMenu;
bool multiMenu;
bool winScreen;
bool gOverScreen;
bool finished;
bool physicsUpdated;
bool airborn;
bool done;
bool slimeCollide;

//==========================================================================================

//setup structures

struct Entity {
  	int speedx;      // vitesse horizontale
    int speedy;      // vitesse verticale
    int x;           // Coordonnée X actuelles
  	int y;           // Coordonnée Y actuelles
    int prevx;       // previous X
    int prevy;       // previous Y
    int ax;             // accel horizontale
    int ay;             // accel verticale
    char type;          // definit le type d'entité pour des tests
    bool state;         // etat joueur : (vie/mort)   slime : (idle/attack)
    int ulimit;         //
    int dlimit;         //
    int llimit;         // limites utilisées pour les collisions
    int rlimit;         //
    bool direction;     // true = droite / false = gauche
    bool prevDirection; // orientation du joueur 1 frame en arrière
};
struct Party {
  int partyTimeMil;
  int partyTimeSec;
  int partyTimeMin;
};

//init structures

struct Entity player;
struct Entity slime;
struct Party party;

//==========================================================================================

// fonctions graphiques

void art_heart(int x, int y, int color) {
  tft.fillCircle(x+25, y-25, 30, color);
  tft.fillCircle(x-25, y-25, 30, color);
  tft.fillTriangle(x, y+50, x-51, y-10, x+51, y-10, color);
  tft.fillCircle(x+14, y-9, 8, BLACK);
  tft.fillCircle(x-14, y-9, 8, BLACK);
}
void art_starbg(int color) {
    for(int x=0; x < SCREENWIDTH; x=x+100) {
        for(int y=0; y < SCREENHEIGHT; y=y+100) {
            //TODO placer etoiles dans tableau
            tft.fillRect(x, y, 2, 2, color);  //Carré d'un pixel
            tft.fillRect(x+20, y+14, 2, 2, color);
            tft.fillRect(x+44, y+78, 2, 2, color);
            tft.fillRect(x+70, y+100, 2, 2, color);
            tft.fillRect(x+16, y+40, 2, 2, color);
            tft.fillRect(x+88, y+80, 2, 2, color);
            tft.fillRect(x+30, y+30, 2, 2, color);
            tft.fillRect(x+5, y+60, 2, 2, color);
            tft.fillRect(x+12, y+88, 2, 2, color);
            tft.fillRect(x+60, y+2, 2, 2, color);
            tft.fillRect(x+76, y+55, 2, 2, color);
            tft.fillRect(x+95, y+15, 2, 2, color);

            tft.fillRect(x+12, y+8, 3, 3, color);  //Carré de deux pixel
            tft.fillRect(x+20, y+40, 3, 3, color);
            tft.fillRect(x+50, y+10, 3, 3, color);
            tft.fillRect(x+75, y+80, 3, 3, color);
            tft.fillRect(x+15, y+50, 3, 3, color);
            tft.fillRect(x+95, y+75, 3, 3, color);
            tft.fillRect(x+66, y+98, 3, 3, color);
            tft.fillRect(x+33, y+55, 3, 3, color);
            tft.fillRect(x+41, y, 3, 3, color);

            tft.fillRect(x+5, y+21, 6, 2, color);   //Croix 
            tft.fillRect(x+7, y+18, 2, 8, color);
            tft.fillRect(x+45, y+41, 6, 2, color);   //Croix 
            tft.fillRect(x+47, y+38, 2, 8, color);
            tft.fillRect(x+39, y+80, 6, 2, color);   //Croix 
            tft.fillRect(x+41, y+77, 2, 8, color);
            tft.fillRect(x+85, y+90, 6, 2, color);   //Croix 
            tft.fillRect(x+87, y+87, 2, 8, color);
            tft.fillRect(x+98, y+65, 6, 2, color);   //Croix 
            tft.fillRect(x+100, y+62, 2, 8, color);
            tft.fillRect(x+20, y+50, 6, 2, color);   //Croix 
            tft.fillRect(x+22, y+47, 2, 8, color);
        }
    }
}
void art_star(int x,int y) {
  if (ground_Salle1[y][x] == 10) {
    tft.fillRect(x*BOX_SIZE+12, y*BOX_SIZE+8, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+4, y*BOX_SIZE+15, 6, 2, VERYLIGHTGRAY); 
    tft.fillRect(x*BOX_SIZE+6, y*BOX_SIZE+12, 2, 8, VERYLIGHTGRAY);
  }
  else if (ground_Salle1[y][x] == 11) {
    tft.fillRect(x*BOX_SIZE+15, y*BOX_SIZE, 2, 2, VERYLIGHTGRAY);
  }
  else if (ground_Salle1[y][x] == 12) {
    tft.fillRect(x*BOX_SIZE+13, y*BOX_SIZE+1, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE, y*BOX_SIZE+4, 3, 3, VERYLIGHTGRAY);
  }
  else if (ground_Salle1[y][x] == 13) {
    tft.fillRect(x*BOX_SIZE+5, y*BOX_SIZE+3, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+4, y*BOX_SIZE+14, 2, 2, VERYLIGHTGRAY);
  }
  else if (ground_Salle1[y][x] == 14) {
    tft.fillRect(x*BOX_SIZE+12, y*BOX_SIZE+5, 6, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+14, y*BOX_SIZE+2, 2, 8, VERYLIGHTGRAY);
  }
  else if (ground_Salle1[y][x] == 15) {
    tft.fillRect(x*BOX_SIZE+13, y*BOX_SIZE+2, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+7, y*BOX_SIZE+7, 3, 3, VERYLIGHTGRAY);
  }
}
void art_blockterre(int x, int y){
    tft.fillRect(x, y, BOX_SIZE, BOX_SIZE, MAGENTA);
}
void art_blocksurface(int x, int y){
    tft.fillRect(x, y, 20, 4, GREEN);
    tft.fillRect(x, y+4, 20, 16, MAGENTA);
    tft.fillRect(x+1, y-3, 1, 9, GREEN);
    tft.fillRect(x+4, y-1, 1, 7, GREEN);
    tft.fillRect(x+5, y-3, 1, 10, GREEN);
    tft.fillRect(x+7, y-2, 1, 7, GREEN);
    tft.fillRect(x+10, y-1, 1, 6, GREEN);
    tft.fillRect(x+11, y-3, 1, 9, GREEN);
    tft.fillRect(x+14, y-1, 1, 7, GREEN);
    tft.fillRect(x+15, y-2, 2, 7, GREEN);
    tft.fillRect(x+19, y-2, 1, 7, GREEN);
}
void art_blockplateforme(int x, int y){
    tft.fillRect(x, y, 20, 4, GREEN);
    tft.fillRect(x, y+4, 20, 16, MAGENTA);
    
    tft.fillRect(x+1, y-3, 1, 9, GREEN);
    tft.fillRect(x+4, y-1, 1, 7, GREEN);
    tft.fillRect(x+5, y-3, 1, 10, GREEN);
    tft.fillRect(x+7, y-2, 1, 7, GREEN);
    tft.fillRect(x+10, y-1, 1, 6, GREEN);
    tft.fillRect(x+11, y-3, 1, 9, GREEN);
    tft.fillRect(x+14, y-1, 1, 7, GREEN);
    tft.fillRect(x+15, y-2, 2, 7, GREEN);
    tft.fillRect(x+19, y-2, 1, 7, GREEN);

    tft.fillTriangle(x, y+18, x, y+19, x+2, y+19, BLACK);
    tft.fillTriangle(x+5, y+19, x+7, y+16, x+9, y+19, BLACK);
    tft.fillTriangle(x+12, y+19, x+13, y+16, x+14, y+19, BLACK);
    tft.fillTriangle(x+17, y+19, x+19, y+17, x+19, y+19, BLACK);
}
void art_fire(int x, int y) {
    tft.fillTriangle(x+2, y+5, x+2, y-4, x+8, y+1, RED); // point blanc
    tft.fillTriangle(x+3, y-1, x+12, y+2, x+9, y-8, RED); // point jaune
    tft.fillTriangle(x+10, y+2, x+14, y+1, x+14, y-12, RED); // point bleu f
    tft.fillTriangle(x+14, y+10, x+18, y-3, x+18, y-4, RED); // point bleu c
    tft.fillCircle(x+10, y+10, 9, RED);
    tft.fillCircle(x+10, y+13, 8, ORANGE);
    tft.fillCircle(x+10, y+14, 5, YELLOW);
}
void art_map(int x,int y) {
  if (ground_Salle1[y][x] == 1) art_blockterre(x*BOX_SIZE, y*BOX_SIZE); 
  else if (ground_Salle1[y][x] == 2) art_blocksurface(x*BOX_SIZE, y*BOX_SIZE);
  else if (ground_Salle1[y][x] == 3) art_blockplateforme(x*BOX_SIZE, y*BOX_SIZE);
  else if (ground_Salle1[y][x] == 4) art_fire(x*BOX_SIZE, y*BOX_SIZE);
  else if (ground_Salle1[y][x] == 5) art_portal(x*BOX_SIZE,y*BOX_SIZE);
  else art_star(x, y);
}
void art_portal (int x, int y){
  tft.fillCircle(x+10, y+9, 9, PORTALE);
  tft.fillCircle(x+10, y+7, 7, PORTALD);
  tft.fillCircle(x+10, y+5, 5, PORTALC);
  tft.fillCircle(x+10, y+3, 3, PORTALB);
  tft.fillCircle(x+10, y+1, 1, PORTALA);
  tft.drawCircle(x+10, y+10, 9.9, WHITE);
  tft.drawCircle(x+10, y+10, 9, WHITE);
} 
void art_skull(int x, int y, int color) {
  tft.fillCircle(x, y+5, 45, color);
  tft.fillCircle(x, y+35, 30, color);
  
  tft.fillCircle(x+15, y+7, 9, BLACK);
  tft.fillTriangle(x, y+25, x-7, y+32, x+7, y+32, BLACK);
  tft.fillRect(x-7, y+33, 15, 4, BLACK);
  tft.fillCircle(x-15, y+7, 9, BLACK);
}
void art_button(int x, int y, int color, int color2) {
  tft.fillCircle(x, y, 24, color);
  tft.fillCircle(x, y, 18, WHITE);
  tft.fillCircle(x-1, y-1, 18, BLACK);
  tft.fillCircle(x-1, y-1, 17, color2);
}
void art_planteg(int x, int y) {

  //Feuille gauche
  tft.fillTriangle(x+1, y+39, x+4, y+34, x+4, y+38, LIME);
  tft.fillTriangle(x+9, y+39, x+4, y+34, x+4, y+38, LIME);
  tft.fillTriangle(x+10, y+36, x+4, y+34, x+10, y+39, LIME);
  tft.fillRect(x+10, y+36, 4, 4, GREEN);
  tft.fillCircle(x+10, y+38, 2, LIME);

  //Feuille droite
  tft.fillTriangle(x+20, y+34, x+19, y+38, x+14, y+36, LIME);
  tft.fillTriangle(x+14, y+36, x+19, y+38, x+15, y+39, LIME);
  tft.fillCircle(x+14, y+38, 2, LIME);

  //Tige
  tft.fillRect(x+12, y+34, 2, 2, GREEN);
  tft.fillCircle(x+13, y+34, 1, GREEN);
  tft.fillCircle(x+12, y+33, 1, GREEN);
  tft.fillRect(x+11, y+31, 2, 2, GREEN);
  tft.fillCircle(x+12, y+30, 1, GREEN);
  tft.fillCircle(x+13, y+29, 1, GREEN);
  tft.fillRect(x+12, y+26, 2, 3, GREEN);

  //Antennes
  tft.fillTriangle(x+8, y+8, x+10, y+14, x+12, y+8, GREEN);
  tft.fillCircle(x+10, y+8, 2, LIME);
  tft.fillTriangle(x+14, y+8, x+14, y+14, x+18, y+8, GREEN);
  tft.fillCircle(x+16, y+8, 2, LIME);

  //Tête
  tft.fillCircle(x+13, y+20, 7, LIME);
  tft.fillCircle(x+4, y+20, 4, LIME);
  tft.fillCircle(x+4, y+20, 2, BLACK);

  tft.fillCircle(x+11, y+24, 1, GREEN);
  tft.fillCircle(x+17, y+20, 1, GREEN);
  tft.fillCircle(x+11, y+17, 1, GREEN);

  //Yeux
  tft.fillRect(x+9, y+18, 2, 1, LIGHTRED);
  tft.fillRect(x+10, y+17, 1, 2, LIGHTRED);
  
}
void art_planted(int x, int y) {

  //Feuille gauche
  tft.fillTriangle(x+19, y+39, x+16, y+34, x+16, y+38, LIME);
  tft.fillTriangle(x+11, y+39, x+16, y+34, x+16, y+38, LIME);
  tft.fillTriangle(x+10, y+36, x+16, y+34, x+10, y+39, LIME);
  tft.fillRect(x+8, y+36, 4, 4, GREEN);
  tft.fillCircle(x+10, y+38, 2, LIME);

  //Feuille droite
  tft.fillTriangle(x, y+34, x+1, y+38, x+6, y+36, LIME);
  tft.fillTriangle(x+6, y+36, x+1, y+38, x+5, y+39, LIME);
  tft.fillCircle(x+6, y+38, 2, LIME);

  //Tige
  tft.fillRect(x+8, y+34, 2, 2, GREEN);
  tft.fillCircle(x+7, y+34, 1, GREEN);
  tft.fillCircle(x+8, y+33, 1, GREEN);
  tft.fillRect(x+9, y+31, 2, 2, GREEN);
  tft.fillCircle(x+8, y+30, 1, GREEN);
  tft.fillCircle(x+7, y+29, 1, GREEN);
  tft.fillRect(x+8, y+26, 2, 3, GREEN);

  //Antennes
  tft.fillTriangle(x+12, y+8, x+10, y+14, x+8, y+8, GREEN);
  tft.fillCircle(x+10, y+8, 2, LIME);
  tft.fillTriangle(x+6, y+8, x+6, y+14, x+2, y+8, GREEN);
  tft.fillCircle(x+4, y+8, 2, LIME);

  //Tête
  tft.fillCircle(x+7, y+20, 7, LIME);
  tft.fillCircle(x+16, y+20, 4, LIME);
  tft.fillCircle(x+16, y+20, 2, BLACK);

  tft.fillCircle(x+9, y+24, 1, GREEN);
  tft.fillCircle(x+3, y+20, 1, GREEN);
  tft.fillCircle(x+9, y+17, 1, GREEN);

  //Yeux
  tft.fillRect(x+9, y+18, 2, 1, LIGHTRED);
  tft.fillRect(x+9, y+17, 1, 2, LIGHTRED);
  
}
void art_shieldg(int x, int y) {

  //Torse
  tft.fillCircle(x+14, y+22, 4, LIGHTGRAY);
  tft.fillCircle(x+14, y+22, 3, GRAY);
  tft.fillRect(x+11, y+22, 6, 10, GRAY);
  tft.fillTriangle(x+17, y+22, x+16, y+26, x+8, y+28, LIME);

  //Jambes
  tft.fillTriangle(x+4, y+32, x+16, y+36, x+17, y+32, GRAY);
  tft.fillTriangle(x+13, y+34, x+17, y+32, x+20, y+38, LIME);
  tft.fillCircle(x+19, y+38, 1, LIGHTGRAY);
  tft.fillRect(x+16, y+38, 4, 2, LIGHTGRAY);
  tft.fillCircle(x+16, y+39, 1, LIGHTGRAY);

  //Tête
  tft.fillCircle(x+8, y+11, 2, RED);
  tft.fillCircle(x+9, y+9, 1, WHITE);
  tft.fillRect(x+8, y+8, 4, 2, LIME);
  tft.fillCircle(x+13, y+13, 5, RED);
  tft.fillCircle(x+13, y+16, 6, LIME);

  //Yeux
  tft.fillRect(x+10, y+14, 2, 1, LIGHTRED);
  tft.fillRect(x+11, y+13, 1, 2, LIGHTRED);

  //Bouclier
  tft.fillCircle(x+4, y+35, 4, LIGHTGRAY);
  tft.fillCircle(x+8, y+35, 4, LIGHTGRAY);
  tft.fillRect(x, y+20, 13, 16, LIGHTGRAY);
  tft.fillCircle(x+4, y+20, 4, LIGHTGRAY);
  tft.fillCircle(x+8, y+20, 4, LIGHTGRAY);
  tft.fillRect(x+4, y+16, 5, 24, BLACK);
  tft.fillRect(x, y+24, 13, 5, BLACK);
  
}
void art_shieldd(int x, int y) {

  //Torse
  tft.fillCircle(x+6, y+22, 4, LIGHTGRAY);
  tft.fillCircle(x+6, y+22, 3, GRAY);
  tft.fillRect(x+3, y+22, 6, 10, GRAY);
  tft.fillTriangle(x+3, y+22, x+4, y+26, x+12, y+28, LIME);

  //Jambes
  tft.fillTriangle(x+16, y+32, x+4, y+36, x+3, y+32, GRAY);
  tft.fillTriangle(x+7, y+34, x+3, y+32, x, y+38, LIME);
  tft.fillCircle(x+1, y+38, 1, LIGHTGRAY);
  tft.fillRect(x, y+38, 4, 2, LIGHTGRAY);
  tft.fillCircle(x+4, y+39, 1, LIGHTGRAY);

  //Tête
  tft.fillCircle(x+12, y+11, 2, RED);
  tft.fillCircle(x+11, y+9, 1, WHITE);
  tft.fillRect(x+8, y+8, 4, 2, LIME);
  tft.fillCircle(x+7, y+13, 5, RED);
  tft.fillCircle(x+7, y+16, 6, LIME);

  //Yeux
  tft.fillRect(x+8, y+14, 2, 1, LIGHTRED);
  tft.fillRect(x+8, y+13, 1, 2, LIGHTRED);

  //Bouclier
  tft.fillCircle(x+15, y+35, 4, LIGHTGRAY);
  tft.fillCircle(x+11, y+35, 4, LIGHTGRAY);
  tft.fillRect(x+7, y+20, 13, 16, LIGHTGRAY);
  tft.fillCircle(x+15, y+20, 4, LIGHTGRAY);
  tft.fillCircle(x+11, y+20, 4, LIGHTGRAY);
  tft.fillRect(x+11, y+16, 5, 24, BLACK);
  tft.fillRect(x+7, y+24, 13, 5, BLACK);
  
}
void art_persog(int x, int y) {

  //Robe partie gauche
  tft.fillTriangle(x+19, y+36, x+15, y+30, x+17, y+38, BROWN);
  tft.fillTriangle(x+13, y+30, x+15, y+30, x+17, y+38, BROWN);
  tft.fillRect(x+13, y+20, 3, 10, BROWN);

  //Robe partie droite
  tft.fillTriangle(x+16, y+39, x+11, y+34, x+10, y+39, BROWN);
  tft.fillTriangle(x+6, y+34, x+11, y+34, x+10, y+39, BROWN);
  tft.fillTriangle(x+10, y+25, x+8, y+28, x+11, y+34, BROWN);
  tft.fillTriangle(x+4, y+22, x+6, y+34, x+11, y+34, BROWN);
  tft.fillTriangle(x+4, y+22, x+12, y+22, x+8, y+28, BROWN);
  tft.fillTriangle(x+6, y+30, x+4, y+39, x+2, y+39, BROWN);
  tft.fillTriangle(x+6, y+30, x+4, y+22, x+2, y+39, BROWN);

  //Robe haut
  tft.fillRect(x+5, y+16, 8, 6, BROWN);
  tft.fillTriangle(x+6, y+30, x+4, y+22, x+2, y+39, BROWN);
  tft.fillCircle(x+6, y+16, 1, BROWN);

  tft.fillCircle(x+11, y+17, 5, BROWN);
  tft.fillCircle(x+11, y+17, 4, YELLOW);
  tft.fillCircle(x+11, y+17, 3, CYAN);

  //Tête
  tft.fillCircle(x+12, y+8, 4, BROWN);
  tft.fillRect(x+9, y+4, 3, 4, BROWN);
  tft.fillCircle(x+9, y+8, 4, YELLOW);
  tft.fillCircle(x+8, y+8, 3, VERYLIGHTGRAY);

  //Yeux
  tft.fillRect(x+7, y+6, 2, 1, BLACK);
  tft.fillRect(x+6, y+7, 1, 2, BLACK);

  //Points cyan
  tft.fillRect(x+12, y+23, 1, 1, CYAN);
  tft.fillRect(x+11, y+25, 1, 1, CYAN);
  tft.fillRect(x+11, y+28, 1, 1, CYAN);
}
void art_persod(int x, int y) {

  //Robe partie gauche
  tft.fillTriangle(x, y+36, x+5, y+30, x+3, y+38, BROWN);
  tft.fillTriangle(x+7, y+30, x+5, y+30, x+3, y+38, BROWN);
  tft.fillRect(x+5, y+20, 3, 10, BROWN);

  //Robe partie droite
  tft.fillTriangle(x+4, y+39, x+9, y+34, x+10, y+39, BROWN);
  tft.fillTriangle(x+14, y+34, x+9, y+34, x+10, y+39, BROWN);
  tft.fillTriangle(x+10, y+25, x+12, y+28, x+9, y+34, BROWN);
  tft.fillTriangle(x+16, y+22, x+14, y+34, x+9, y+34, BROWN);
  tft.fillTriangle(x+16, y+22, x+8, y+22, x+12, y+28, BROWN);
  tft.fillTriangle(x+14, y+30, x+16, y+39, x+18, y+39, BROWN);
  tft.fillTriangle(x+14, y+30, x+16, y+22, x+18, y+39, BROWN);

  //Robe haut
  tft.fillRect(x+8, y+16, 8, 6, BROWN);
  tft.fillTriangle(x+14, y+30, x+16, y+22, x+18, y+39, BROWN);
  tft.fillCircle(x+14, y+16, 1, BROWN);

  tft.fillCircle(x+9, y+17, 5, BROWN);
  tft.fillCircle(x+9, y+17, 4, YELLOW);
  tft.fillCircle(x+9, y+17, 3, CYAN);

  //Tête
  tft.fillCircle(x+8, y+8, 4, BROWN);
  tft.fillRect(x+8, y+4, 3, 4, BROWN);
  tft.fillCircle(x+11, y+8, 4, YELLOW);
  tft.fillCircle(x+12, y+8, 3, VERYLIGHTGRAY);

  //Yeux
  tft.fillRect(x+12, y+6, 2, 1, BLACK);
  tft.fillRect(x+14, y+7, 1, 2, BLACK);

  //Points cyan
  tft.fillRect(x+7, y+23, 1, 1, CYAN);
  tft.fillRect(x+8, y+25, 1, 1, CYAN);
  tft.fillRect(x+8, y+28, 1, 1, CYAN);
  
}
void art_slimeg(int x, int y, int color, int color2) {

  //Base du corps
  tft.fillCircle(x+9, y+9, 9, color);
  tft.fillCircle(x+8, y+9, 6, color2);
  tft.fillCircle(x+10, y+9, 6, color2);

  //Yeux
  tft.fillRect(x+5, y+5, 2, 5, BLACK);

  //Pics en bas
  tft.fillTriangle(x+6, y+15, x+2, y+19, x+14, y+18, LIGHTGRAY);
  tft.fillTriangle(x+10, y+14, x+14, y+19, x+6, y+19, GRAY);
  tft.fillTriangle(x+17, y+19, x+14, y+15, x+6, y+16, LIGHTGRAY);

  //Pics en haut
  tft.fillTriangle(x+18, y+2, x+17, y+6, x+16, y, LIGHTGRAY);
  tft.fillTriangle(x+17, y-3, x+17, y+4, x+11, y+2, GRAY);
  tft.fillTriangle(x+13, y-4, x+15, y+2, x+10, y+1, LIGHTGRAY);
  tft.fillTriangle(x+7, y-5, x+9, y+1, x+2, y+2, LIGHTGRAY);
  tft.fillTriangle(x+12, y+1, x+10, y-4, x+6, y+1, GRAY);
  tft.fillTriangle(x+1, y-2, x+1, y+5, x+5, y+3, GRAY);
  tft.fillTriangle(x+3, y+6, x+2, y+8, x, y+4, LIGHTGRAY);
  
}
void art_slimed(int x, int y, int color, int color2) {

  //Base du corps
  tft.fillCircle(x+10, y+9, 9.9, color);
  tft.fillCircle(x+11, y+9, 6, color2);
  tft.fillCircle(x+9, y+9, 6, color2);

  //Yeux
  tft.fillRect(x+14, y+5, 2, 5, BLACK);

  //Pics en bas
  tft.fillTriangle(x+14, y+15, x+18, y+19, x+6, y+18, LIGHTGRAY);
  tft.fillTriangle(x+10, y+14, x+6, y+19, x+14, y+19, GRAY);
  tft.fillTriangle(x+3, y+19, x+6, y+15, x+14, y+16, LIGHTGRAY);

  //Pics en haut
  tft.fillTriangle(x, y+2, x+3, y+6, x+4, y, LIGHTGRAY);
  tft.fillTriangle(x+3, y-3, x+3, y+4, x+9, y+2, GRAY);
  tft.fillTriangle(x+7, y-4, x+5, y+2, x+10, y+1, LIGHTGRAY);
  tft.fillTriangle(x+13, y-5, x+11, y+1, x+18, y+2, LIGHTGRAY);
  tft.fillTriangle(x+8, y+1, x+10, y-4, x+14, y+1, GRAY);
  tft.fillTriangle(x+19, y-2, x+19, y+5, x+15, y+3, GRAY);
  tft.fillTriangle(x+17, y+6, x+18, y+8, x+19, y+4, LIGHTGRAY);
  
}
void art_persog_shadow(int x, int y) {

  //Robe partie gauche
  tft.fillTriangle(x+19, y+36, x+15, y+30, x+17, y+38, BLACK);

  //Corps
  tft.fillRect(x+2, y+15, 16, 25, BLACK);

  //Tête
  tft.fillCircle(x+10.5, y+10, 7, BLACK);
}
void art_persod_shadow(int x, int y) {

  //Robe partie gauche
  tft.fillTriangle(x, y+36, x+5, y+30, x+3, y+38, BLACK);

  //Corps
  tft.fillRect(x+2, y+15, 17, 25, BLACK);

  //Tête
  tft.fillCircle(x+10.5, y+10, 7, BLACK);
  
}
void art_slimeg_shadow(int x, int y) {

  tft.fillRect(x, y-5, 20, 25, BLACK);
  
}
void art_slimed_shadow(int x, int y) {

  tft.fillRect(x, y-5, 20, 25, BLACK);

}
//fonctions affichage entités

void last_square() {

  if(player.prevDirection) {
    art_persod_shadow(player.prevx,player.prevy);
  }
  else {
    art_persog_shadow(player.prevx,player.prevy);
  }

  if (slime.prevDirection) {
    art_slimed_shadow(slime.prevx,slime.prevy);
  }
  else {
    art_slimeg_shadow(slime.prevx,slime.prevy);
  }
  

  // refresh graphique perso
  int x = player.prevx/BOX_SIZE;
  int x2 = (player.prevx + (BOX_SIZE - 1)) / BOX_SIZE;
  int y = player.prevy/BOX_SIZE;
  int y2 = player.prevy/BOX_SIZE + 1;
  int y3 = (player.prevy + 2*BOX_SIZE - 1)/BOX_SIZE;
  art_map(x,y);
  art_map(x2,y);
  art_map(x,y2);
  art_map(x2,y2);
  art_map(x,y3);
  art_map(x2,y3);

  // refresh graphique slime
  art_map(slime.prevx/BOX_SIZE,slime.prevy/BOX_SIZE);
  art_map((slime.prevx + BOX_SIZE-1)/BOX_SIZE,slime.prevy/BOX_SIZE);
  art_map(slime.prevx/BOX_SIZE,(slime.prevy - BOX_SIZE+10)/BOX_SIZE);
  art_map((slime.prevx + BOX_SIZE-1)/BOX_SIZE,(slime.prevy-BOX_SIZE +10)/BOX_SIZE);
}
void display_player() {
  if(player.speedx > 0) {
    player.direction = true;
  }
  if(player.speedx < 0) {
    player.direction = false;
  }
  if(player.direction == true) {
    art_persod(player.x,player.y);
  }
  else {
    art_persog(player.x,player.y);
  }
}
void display_slime() {

  int color;
  int color2;

  if(slime.state == true) {
    color = RED;
    color2 = LIGHTRED;
  }
  else {
    color = BLUE;
    color2 = CYAN;
  }

  if(slime.speedx > 0) {
    slime.direction = true;
  }
  if(slime.speedx < 0) {
    slime.direction = false;
  }
  if(slime.direction == true) {
    art_slimed(slime.x,slime.y,color,color2);
  }
  else {
    art_slimeg(slime.x,slime.y,color,color2);
  }
}

// fonction affichage overlay

void display_winMenu() {
  tft.fillScreen(BLACK);
  art_starbg(GRAY);
  tft.setCursor(SCREENWIDTH/4, SCREENHEIGHT/8);
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.print("VICTORY");

  art_heart(SCREENWIDTH/2, SCREENHEIGHT/2, ORANGE);
  
  tft.setTextSize(2);
  tft.setCursor((SCREENWIDTH*0.3)/5, (SCREENHEIGHT*3)/5);
  tft.print("REPLAY ?");

  tft.setCursor((SCREENWIDTH*4.9)/7, (SCREENHEIGHT*3)/5);
  tft.print("QUIT ?");

  tft.setCursor((SCREENWIDTH*2.1)/5, (SCREENHEIGHT*3.1)/4);
  tft.print("PRESS");

  art_button(SCREENWIDTH/5, (SCREENHEIGHT*4)/5, GREEN, LIME);
  art_button((SCREENWIDTH*4)/5, (SCREENHEIGHT*4)/5, RED, LIGHTRED);
}
void display_loseMenu() {
  tft.fillScreen(BLACK);
  art_starbg(GRAY);
  tft.setCursor(SCREENWIDTH/5, SCREENHEIGHT/8);
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.print("GAMEOVER");
  
  art_skull(SCREENWIDTH/2, ((SCREENHEIGHT*3)/7)+5, LIGHTGRAY);
  
  tft.setTextSize(2);
  tft.setCursor((SCREENWIDTH*0.3)/5, (SCREENHEIGHT*3)/5);
  tft.print("REPLAY ?");

  tft.setCursor((SCREENWIDTH*4.9)/7, (SCREENHEIGHT*3)/5);
  tft.print("QUIT ?");

  tft.setCursor((SCREENWIDTH*2.1)/5, (SCREENHEIGHT*3.1)/4);
  tft.print("PRESS");

  art_button(SCREENWIDTH/5, (SCREENHEIGHT*4)/5, GREEN, LIME);
  art_button((SCREENWIDTH*4)/5, (SCREENHEIGHT*4)/5, RED, LIGHTRED);
}
void display_topMenu() {
  display_life();
  timeCalculation(party);
  display_score();
}
void display_multiMenu() {
  tft.fillScreen(BLACK);
  art_starbg(GRAY);
  tft.setCursor(SCREENWIDTH/4, SCREENHEIGHT/8);//Peut-être à modifié
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.print("SETTING");
  
  tft.setTextSize(2);
  tft.setCursor((SCREENWIDTH*0.65)/5, (SCREENHEIGHT*3)/5);
  tft.print("SOLO");

  tft.setCursor((SCREENWIDTH*4.9)/7, (SCREENHEIGHT*3)/5);
  tft.print("VERSUS");

  tft.setCursor((SCREENWIDTH*2.1)/5, (SCREENHEIGHT*3.1)/4);
  tft.print("PRESS");

  art_button(SCREENWIDTH/5  , (SCREENHEIGHT*4)/5, GREEN, LIME);
  art_button((SCREENWIDTH*4)/5, (SCREENHEIGHT*4)/5, RED, LIGHTRED);
}
void display_homeMenu() {
  tft.fillScreen(BLACK);
  art_starbg(GRAY);
  tft.setCursor((SCREENWIDTH/9)-15, SCREENHEIGHT/8);//Peut-être à modifier
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.print("ASTRAL LIGHT");
  
  art_planted((SCREENWIDTH/2)-10, (SCREENHEIGHT/2)-20);

  tft.setTextSize(2);
  tft.setCursor((SCREENWIDTH*0.4)/7, (SCREENHEIGHT*3.4)/4);//A centrer
  tft.print("PRESS THE");

  tft.setCursor((SCREENWIDTH*4.3)/7, (SCREENHEIGHT*3.4)/4);//A centrer
  tft.print("TO START");

  art_button(SCREENWIDTH/2, (SCREENHEIGHT*4.4)/5, GREEN, LIME);//A centrer
}
void display_topBar() {
  tft.fillRect(0, 0, SCREENWIDTH, 20, BLACK);
}
void display_score() {
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  
  tft.setCursor((SCREENHEIGHT*7.2)/8, (SCREENHEIGHT*0.2)/8);
  tft.print("SCORE:");
  tft.print("??");
}
void display_life() {
  art_hp((SCREENWIDTH*3.3)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, RED);
}
void display_menu() {
  display_topBar();
  display_life();
  display_score();
}
void display_map() {
  for(int i = 0;i < MAP_COLS;i++) {
    for(int j = 0;j < MAP_ROWS;j++) {
      art_map(i,j);
    }
  }
}
void art_hp(int x, int y, int backgroundColor) { 
  tft.fillCircle(x+5, y+5, 6, backgroundColor);
  tft.fillCircle(x-5, y+5, 6, backgroundColor);
  tft.fillTriangle(x, y+18, x-11, y+5, x+11, y+5, backgroundColor);
}

//fonctions physiques

void buttons_update() {
  //test down pressed
  if (digitalRead(28) || digitalRead(24)) {
    downPressed = true;
  } 
  else {
    downPressed = false;
  }  
  //test up pressed
  if (digitalRead(29) || digitalRead(23)) {
    upPressed = true;
  }
  else {
    upPressed = false;
  }
  //test right pressed
  if (digitalRead(30) || digitalRead(22)) {
    rightPressed = true;
  }
  else {
    rightPressed = false;
  }
  //test left pressed
  if (digitalRead(31) || digitalRead(25)) {
    leftPressed = true;
  }
  else {
    leftPressed = false;
  }
  //test red button pressed
  if(digitalRead(27)) {
    redPressed = true;
  }
  else {
    redPressed = false;
  }
  //test green button pressed
  if(digitalRead(26)) {
    greenPressed = true;
  }
  else {
    greenPressed = false;
  }
}
void setup_player(Entity &player) {
  player.x = PSTARTX;
  player.y = PSTARTY;
  player.prevx = PSTARTX;
  player.prevy = PSTARTY;
  player.speedx = 0;
  player.speedy = 0;
  player.ax = 0;
  player.ay = 0;
  player.type = 'p';
  player.direction = true;
  slimeCollide = false;
}
void setup_slime(Entity &slime,int originx,int originy,int leftLimit,int rightLimit) {
  slime.x = originx*BOX_SIZE;
  slime.y = originy*BOX_SIZE;
  slime.ulimit = originy;
  slime.dlimit = originy;
  slime.llimit = leftLimit*BOX_SIZE;
  slime.rlimit = rightLimit*BOX_SIZE;
  slime.type = 's';
  slime.state = false;
  slime.ax = 0;
  slime.ay = 0;
  slime.speedy = 0;
  slime.speedx = 0;
}
void update_physics(Entity &player) {
  player.prevx = player.x;
  player.prevy = player.y;
  player.prevDirection = player.direction;
  slime.prevDirection = slime.direction;

  if(airborn) player.ay = 2;

	if(downPressed) player.ay = 3;
  else player.ay = 2;

  player.ax = 0;

  if(rightPressed) player.ax = 2;
  if(rightPressed && player.speedx < 0) player.speedx = player.speedx + 2;
  
  if(leftPressed) player.ax = -2;
  if(leftPressed && player.speedx > 0) player.speedx = player.speedx - 2;

  if(!(leftPressed || rightPressed)) player.speedx = player.speedx/2;

  next_walls(player);
  update_xposition(player);
  update_yposition(player);
}
void update_slime(Entity &slime) {
  slime.prevx = slime.x;
  slime.prevy = slime.y;

  if(player.y > slime.y - BOX_SIZE*4 + 10 && player.y < slime.y + 40) {
    if(player.x > slime.x - BOX_SIZE*3 && player.x <= slime.x) {
      slime.state = true;
      slime.ax = -1;
    }
    else if(player.x < slime.x + BOX_SIZE*3 && player.x > slime.x) {
      slime.state = true;
      slime.ax = 1;
    }
    else {
    slime.state = false;
    slime.ax = 0;
    slime.speedx = slime.speedx/2;
    }
  }
  else {
    slime.state = false;
    slime.ax = 0;
    slime.speedx = slime.speedx/2;
  }
  if (slime.x + slime.speedx > slime.rlimit-BOX_SIZE) {
    slime.ax = 0;
    slime.speedx = -slime.speedx/2;
    slime.x = slime.rlimit-BOX_SIZE;
  }
  else if (slime.x + slime.speedx < slime.llimit+1) {
    slime.ax = 0;
    slime.speedx = -slime.speedx/2;
    slime.x = slime.llimit+1;
  }
  else {
    slime.speedx = slime.speedx + slime.ax;
    slime.x = slime.x + slime.speedx;
  }
}
void update_xposition(Entity &player) {

  if (player.speedx + player.ax > 10) {
    player.ax = 0;
    player.speedx = 10;
  }
  else if(player.speedx + player.ax < -10) {
    player.ax = 0;
    player.speedx = -10;
  }
  else player.speedx = player.speedx + player.ax;
  
  if(player.x + player.speedx > nextRightWall - BOX_SIZE || player.x + player.speedx > SCREENWIDTH - BOX_SIZE) {
    Serial.println(nextRightWall);
    player.speedx = -player.speedx;
    player.ax = 0;
    player.x = player.x + player.speedx;
  }
  else if (player.x + player.speedx < nextLeftWall || player.x + player.speedx < 0) {
    player.speedx = -player.speedx;
    player.ax = 0;
    player.x = player.x + player.speedx;
  }
  else if(nextRightWall - nextLeftWall == BOX_SIZE) {
    player.speedx = 0;
    player.ax = 0;
    player.x = nextRightWall;
  }
  else player.x = player.x + player.speedx;
}
void update_yposition(Entity &player) {
  player.speedy = player.speedy + player.ay;
  if(player.y + player.speedy > nextFloor - PLAYER_HEIGHT && !downPressed) {
    player.speedy = -player.speedy/2;
    player.y = nextFloor - PLAYER_HEIGHT;
    if(upPressed) {
      airborn = false;
    }
    if(player.speedy < 5 && player.speedy > -5) {
      airborn = false;
    }
  }
  else if(player.y + player.speedy > nextFloor - PLAYER_HEIGHT && downPressed) {
    player.speedy = 0;
    player.y = nextFloor - PLAYER_HEIGHT;
    airborn = false;
  }
  else if(player.y + player.speedy < nextCeilling) {
    player.speedy = -player.speedy/2;
    player.y = nextCeilling;
  }

  if(nextFloor - player.y > PLAYER_HEIGHT + 5) {
    airborn = true;
  }

  if(!airborn) {
    player.ay = 0;
    player.speedy = 0;
  }
  
  if(!airborn && upPressed) {
    airborn = true;
    player.speedy = -13;
	}

  if(player.speedy > 18) {
    player.speedy = 18;
  }

  if(nextFloor - player.y > PLAYER_HEIGHT + 5) {
    airborn = true;
  }
  
  player.y = player.y + player.speedy;
}
void next_walls(Entity &player) {
  tabx = player.prevx/BOX_SIZE;
  tabx2 = (player.prevx+BOX_SIZE-1) /BOX_SIZE;
  taby = player.prevy/BOX_SIZE;
  taby2 = (player.prevy+BOX_SIZE-1)/BOX_SIZE;
  taby3 = (player.prevy + PLAYER_HEIGHT-1)/BOX_SIZE;
  nextRightWall = SCREENWIDTH;
  nextLeftWall = 0;
  nextCeilling = BOX_SIZE; //décalage depuis le haut de l'écran pour considérer la barre de menu
  nextFloor = SCREENHEIGHT - BOX_SIZE;

  // boucle mur droit
  for(int i = tabx + 1;i < MAP_COLS;i++) {
    if(ground[taby][i] || ground[taby2][i] || ground[taby3][i]) {
      nextRightWall = i * BOX_SIZE;
      break;
    }
  }
  // boucle sol
  for(int i = taby + 1;i < MAP_ROWS;i++) {
    if(ground[i][tabx] == true || ground[i][tabx2] == true) {
      nextFloor = i * BOX_SIZE;
      break;
    }
  }
  // boucle mur gauche
  for(int j = 0;j < tabx;j++) {
    if(ground[taby][j] || ground[taby2][j] || ground[taby3][j]) {
      nextLeftWall = (j+1) * BOX_SIZE;
    }
  }
  // plafond
  for(int j = 0;j < taby;j++) {
    if(ground[j][tabx] == true || ground[j][tabx2] == true) {
      nextCeilling = (j+1) * BOX_SIZE;
    }
  }

  //tft.fillRect(0,nextFloor,SCREENWIDTH,1,BLUE);           // affichage test collisions
  //tft.fillRect(0,nextCeilling,SCREENWIDTH,-1,WHITE);
}

// fonctions logiques

void timeCalculation(Party &party) {

  int mil = party.partyTimeMil;
  int sec = party.partyTimeSec;
  int min = party.partyTimeMin;

  party.partyTimeMil++;
  if(gameSetup) {
    party.partyTimeMil--;
    tft.setCursor(10, (SCREENHEIGHT*0.2)/8);
    tft.print("00");
    tft.setTextColor(WHITE);
    tft.setCursor(31,(SCREENHEIGHT*0.2)/8);
    tft.print(":");
    tft.setCursor(40, (SCREENHEIGHT*0.2)/8);
    tft.print("00");
    tft.setCursor(61,(SCREENHEIGHT*0.2)/8);
    tft.print(":");
    tft.setCursor(70, (SCREENHEIGHT*0.2)/8);
    tft.print("00");
  }

  if (party.partyTimeMil > 9) {

    party.partyTimeSec++;
    party.partyTimeMil = 0;

    if (party.partyTimeSec > 59) {
      party.partyTimeMin++;
      party.partyTimeSec = 0;
      tft.setTextColor(BLACK);
      tft.setCursor(10, (SCREENHEIGHT*0.2)/8);
      tft.print("0");
      tft.print(min);
      tft.setTextColor(WHITE);
      tft.setCursor(10, (SCREENHEIGHT*0.2)/8);
      tft.print("0");
      tft.print(party.partyTimeMin);
    }

    tft.setTextColor(BLACK);
    tft.setCursor(40, (SCREENHEIGHT*0.2)/8);
    if(sec < 10) {
      tft.print("0");
      tft.print(sec);
    }
    else tft.print(sec);
    tft.setTextColor(WHITE);
    tft.setCursor(40, (SCREENHEIGHT*0.2)/8);
    if(party.partyTimeSec < 10) {
      tft.print("0");
      tft.print(party.partyTimeSec);
    }
    else tft.print(party.partyTimeSec);
  }

  tft.setTextColor(BLACK);
  tft.setCursor(70, (SCREENHEIGHT*0.2)/8);
  tft.print("0");
  tft.print(mil);
  tft.setTextColor(WHITE);
  tft.setCursor(70, (SCREENHEIGHT*0.2)/8);
  tft.print("0");
  tft.print(party.partyTimeMil);
}
void coll_slime(Entity &player,Entity &slime) {

  int diffx = player.x - slime.x;
  int diffy = player.y - slime.y;
  if((diffx < BOX_SIZE && diffx > -BOX_SIZE) && (diffy < BOX_SIZE && diffy > -2*BOX_SIZE)) {
    slimeCollide = true;
  }
  else {
    slimeCollide = false;
  }
}
void life_sub() {
  lifeCount--;
  if(lifeCount == 2) {
    art_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, BLACK);
  }
  if(lifeCount == 1) {
    art_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, BLACK);
  }
  if(lifeCount == 0) {
    finished = true;
    gOverScreen = true;
    winScreen = false;
  }
}

//=======================================================================================

void setup() {

  //variable pour demmarage
  homeMenu = true;

	//setup du compteur de temps
	timebuffer = millis();

	//setup console et ecran
	Serial.begin(9600);
	tft.begin();
	tft.setRotation(LANDSCAPE);
  randomSeed(analogRead(0)); // Initialisation de la séquence aléatoire
  
	//setup controles
	pinMode(28, INPUT); pinMode(24, INPUT); // Bas
	pinMode(29, INPUT); pinMode(23, INPUT); // Haut
	pinMode(30, INPUT); pinMode(22, INPUT); // Droite
	pinMode(31, INPUT); pinMode(25, INPUT); // Gauche
	pinMode(27, INPUT); // Bouton START (vert)
	pinMode(26, INPUT); // Bouton SELECT (rouge)
	pinMode(32, OUTPUT); // Haut-parleur
}

//===========================================================================================s

void loop() {

  done = false;
  while(homeMenu) {
    if(!done) {
      done = true;
      display_homeMenu();
    }
    if(digitalRead(27)) {
      homeMenu = false;
      multiMenu = true;
    }
  }

  done = false;
  while(multiMenu) {
    if(!done) {
      done = true;
      display_multiMenu();
    }
    if(digitalRead(27)) {
      multiMenu = false;
      gameSetup = true;
    }
  }
  
	//setup jeu
	if(gameSetup == true) {

    //setup graphique
    tft.fillScreen(BLACK);
    display_map();

    //setup logique
    display_menu();
    lifeCount = 3;
    party.partyTimeMil = 0;
    party.partyTimeMin = 0;
    party.partyTimeSec = 0;
    timeCalculation(party);

    //setup entités
    setup_player(player);
    display_player();
    setup_slime(slime,3,10,3,10);     // setup slime n : arg1 , origine x : arg2 , origine y : arg3 , limites murs : arg3 et arg4
    display_slime();

    //setup booleens
    finished = false;
    gameSetup = false;
    airborn = true;
  }

  //deroulement jeu
  while (!finished) {
		  
		timePassed = millis() - timebuffer;
    //boucle pour répeter une action tout les x millisecondes
    if(timePassed > refresh) {  
      buttons_update();
      next_walls(player);
      update_physics(player);
      update_slime(slime);
      coll_slime(player,slime);
      last_square();
      display_player();
      display_slime();
      timeCalculation(party);
      if(slimeCollide) {
        if(player.direction) {
          art_persod_shadow(player.x,player.y);
        }
        else {
          art_persog_shadow(player.x,player.y);
        }                                                 // recouvrement du perso lorsqu'il respawn
        life_sub();
        setup_player(player);
      }
      if(digitalRead(27)) {
        finished = true;
        winScreen = true;
        gOverScreen = false;
      }
		  timebuffer = millis();
    } 
  }

  done = false;
  while(winScreen) {
    if(!done) {
      done = true;
      display_winMenu();
    }
    if(digitalRead(27)) {
      homeMenu = true;
      winScreen = false;
    }
  }

  done = false;
  while(gOverScreen) {
    if(!done) {
      done = true;
      display_loseMenu();
    }
    if(digitalRead(27)) {
      homeMenu = true;
      gOverScreen = false;
    }
  }
}