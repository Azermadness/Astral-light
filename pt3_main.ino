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
#define BOX_SIZE 20      // Taille d'une casse
#define PLAYER_HEIGHT 40
#define MAP_COLS 16
#define MAP_ROWS 12

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
ILI9341_due tft2 = ILI9341_due(TFT_CS, TFT_DC);

//=========================================================================================

//setup variables

int mapNumber;
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
int pstartx[3] = {30,30,270};
int pstarty[3] = {150,50,160};
bool ground[3][MAP_ROWS][MAP_COLS] = {
  {
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
    {true , true , true , false, false, false, false, false, false, false, true , true , true , false, false, true },
    {true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true },
  },{
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false},
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
    {true , true , false, false, false, false, false, false, false, false, false, false, false, true , true , true },
    {true , false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
    {true , false, false, false, true , false, false, false, false, false, false, false, false, false, false, true },
    {true , false, false, false, true , true , true , true , true , true , true , true , true , true , true , true }
  },{
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true },
    {false, false, false, true , true , true , true , true , true , true , true , true , false, false, false, true },
    {false, false, false, false, false, false, false, false, false, false, true , false, false, false, false, true },
    {false, false, false, false, false, false, false, false, false, false, true , false, false, false, true , true },
    {true , true , true , true , false, false, false, false, false, false, true , false, false, false, false, true },
    {true , false, false, false, false, false, false, false, false, false, true , true , false, false, false, true },
    {true , false, false, false, false, false, false, false, false, false, true , false, false, false, false, true },
    {true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true }
  }
};
int ground_Salle[3][MAP_ROWS][MAP_COLS] = {
  {
    {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {15, 13, 14, 12, 10, 15, 10, 13, 10, 14, 10, 13, 11, 15, 10, 13},
    {10, 12, 11, 10, 14, 10, 12, 15, 11, 13, 12, 15, 14, 10, 12, 14},
    {5 , 10, 10, 13, 11, 14, 10, 10, 10, 12, 10, 14, 12, 10, 11, 13},
    {2 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 14, 12, 15, 11},
    {1 , 13, 14, 11, 12, 10, 15, 10, 11, 10, 13, 12, 11, 10, 14, 10},
    {1 , 10, 15, 10, 13, 10, 11, 10, 13, 11, 14, 15, 10, 12, 11, 2 },
    {1 , 14, 12, 11, 10, 13, 15, 14, 12, 10, 13, 10, 14, 15, 13, 1 },
    {1 , 11, 13, 10, 15, 12, 10, 12, 10, 15, 11, 10, 2 , 10, 11, 1 },
    {1 , 12, 10, 11, 14, 10, 15, 13, 11, 12, 6 , 2 , 1 , 12, 11, 1 },
    {1 , 2 , 2 , 15, 13, 11, 10, 15, 10, 10, 2 , 1 , 1 , 4 , 4 , 1 },
    {1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 1 , 1 , 2 , 2 , 1 }
  },{
    {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {1 , 14, 12, 10, 13, 11, 12, 10, 10, 12, 10, 11, 10, 12, 13, 10},
    {1 , 11, 14, 11, 15, 13, 10, 12, 11, 15, 14, 13, 11, 15, 12, 13},
    {1 , 12, 11, 10, 12, 10, 11, 12, 10, 11, 10, 14, 10, 13, 15, 10},
    {1 , 10, 11, 12, 15, 11, 4 , 14, 10, 15, 12, 10, 4 , 10, 14, 15},
    {1 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 10, 11, 10},
    {1 , 5 , 12, 11, 15, 13, 11, 14, 10, 11, 15, 10, 14, 10, 13, 14},
    {1 , 12, 14, 10, 11, 12, 15, 10, 14, 12, 11, 12, 11, 14, 12, 10},
    {1 , 3 , 12, 10, 13, 10, 11, 13, 12, 10, 14, 10, 15, 3 , 3 , 2 },
    {1 , 12, 10, 13, 10, 11, 12, 15, 14, 11, 10, 13, 10, 12, 15, 1 },
    {1 , 10, 14, 10, 2 , 13, 12, 10, 11, 10, 15, 12, 10, 15, 11, 1 },
    {1 , 15, 11, 12, 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 }
  },{
    {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {13, 10, 11, 14, 12, 15, 10, 11, 15, 14, 10, 12, 11, 10, 14, 1 },
    {12, 15, 13, 10, 11, 10, 13, 14, 10, 12, 14, 10, 13, 15, 11, 1 },
    {10, 13, 10, 15, 12, 11, 13, 15, 11, 10, 11, 14, 11, 14, 10, 1 },
    {11, 14, 12, 4 , 10, 13, 10, 14, 12, 13, 4 , 15, 10, 11, 13, 1 },
    {10, 11, 13, 3 , 3 , 3 , 3 , 3 , 3 , 3 , 2 , 3 , 14, 13, 11, 1 },
    {14, 12, 11, 14, 13, 10, 12, 11, 14, 10, 1 , 15, 12, 10, 11, 1 },
    {10, 13, 10, 15, 11, 10, 12, 10, 15, 11, 1 , 12, 13, 14, 3 , 1 },
    {2 , 3 , 3 , 3 , 10, 14, 15, 10, 14, 11, 1 , 15, 11, 10, 12, 1 },
    {1 , 11, 13, 14, 12, 10, 13, 11, 15, 10, 1 , 3 , 12, 14, 13, 1 },
    {1 , 5 , 12, 15, 13, 11, 10, 13, 12, 14, 1 , 10, 15, 12, 11, 1 },
    {1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 2 , 2 , 2 , 1 }
  }
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
unsigned long timePassedFPS;
unsigned long timeBufferFPS;
bool gameSetup;
bool L1Done;
bool L2Done;
bool homeMenu;
bool multiMenu;
bool winScreen;
bool gOverScreen;
bool finished;
bool physicsUpdated;
bool airborn;
bool done;
bool slimeCollide;
bool shieldCollide;
bool portalCollide;
bool fireCollide;
bool fire2Collide;
bool fellDown;
bool enablePlante;
bool dirPlante;
bool planteCollide;
int portalx;
int portaly;
int plantex;
int plantey;
int flammex[2] = {0,0};
int flammey[2] = {0,0};


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
    bool enabled;       // définit si une entité sera présente ou pas sur la carte
};
struct Party {
  int partyTimeMil;
  int partyTimeSec;
  int partyTimeMin;
};

//init structures

struct Entity player;
struct Entity slime;
struct Entity bouclier;
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
void art_star(int x,int y,int mapNumber) {
  if (ground_Salle[mapNumber][y][x] == 10) {
    tft.fillRect(x*BOX_SIZE+12, y*BOX_SIZE+8, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+4, y*BOX_SIZE+15, 6, 2, VERYLIGHTGRAY); 
    tft.fillRect(x*BOX_SIZE+6, y*BOX_SIZE+12, 2, 8, VERYLIGHTGRAY);
  }
  else if (ground_Salle[mapNumber][y][x] == 11) {
    tft.fillRect(x*BOX_SIZE+15, y*BOX_SIZE, 2, 2, VERYLIGHTGRAY);
  }
  else if (ground_Salle[mapNumber][y][x] == 12) {
    tft.fillRect(x*BOX_SIZE+13, y*BOX_SIZE+1, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE, y*BOX_SIZE+4, 3, 3, VERYLIGHTGRAY);
  }
  else if (ground_Salle[mapNumber][y][x] == 13) {
    tft.fillRect(x*BOX_SIZE+5, y*BOX_SIZE+3, 2, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+4, y*BOX_SIZE+14, 2, 2, VERYLIGHTGRAY);
  }
  else if (ground_Salle[mapNumber][y][x] == 14) {
    tft.fillRect(x*BOX_SIZE+12, y*BOX_SIZE+5, 6, 2, VERYLIGHTGRAY);
    tft.fillRect(x*BOX_SIZE+14, y*BOX_SIZE+2, 2, 8, VERYLIGHTGRAY);
  }
  else if (ground_Salle[mapNumber][y][x] == 15) {
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
void art_map(int x,int y,int mapNumber) {
  if (ground_Salle[mapNumber][y][x] == 1) art_blockterre(x*BOX_SIZE, y*BOX_SIZE); 
  else if (ground_Salle[mapNumber][y][x] == 2) art_blocksurface(x*BOX_SIZE, y*BOX_SIZE);
  else if (ground_Salle[mapNumber][y][x] == 3) art_blockplateforme(x*BOX_SIZE, y*BOX_SIZE); 
  else if (ground_Salle[mapNumber][y][x] == 4) art_fire(x*BOX_SIZE, y*BOX_SIZE);
  else if (ground_Salle[mapNumber][y][x] == 5) {
    art_portal(x*BOX_SIZE,y*BOX_SIZE);
    portalx = x;
    portaly = y;
  }
  else if (ground_Salle[mapNumber][y][x] == 6) art_plante_off(x*BOX_SIZE, y*BOX_SIZE);
  else art_star(x, y, mapNumber);
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
void art_plante_off(int x,int y) {
  tft.fillTriangle(x+6, y+10, x+4, y+12, x+10, y+20, GREEN);
  tft.fillCircle(x+5, y+11, 2, LIME);
  tft.fillTriangle(x+14, y+10, x+16, y+12, x+10, y+20, GREEN);
  tft.fillCircle(x+15, y+11, 2, LIME);
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

  tft.fillCircle(x+5, y+6, 4, VERYLIGHTGRAY);
  tft.drawCircle(x+5, y+6, 4, GRAY);
  tft.fillRoundRect(x+1, y+3, 4, 6, 2, BLACK);

  tft.fillTriangle(x+3, y+8, x+1, y+8, x+2, y+9, BLUE);
  tft.drawLine(x+1, y+4, x+3, y+5, BLUE);
  tft.fillCircle(x+6, y+16, 5, VERYLIGHTGRAY);
  tft.fillTriangle(x+10, y+16, x+2, y+20, x+13, y+27, VERYLIGHTGRAY);
  tft.fillTriangle(x+8, y+29, x+2, y+20, x+13, y+27, VERYLIGHTGRAY);
  tft.drawLine(x+10, y+16, x+14, y+29, GRAY);

  tft.fillCircle(x+5, y+14, 2, WHITE);
  tft.drawCircle(x+5, y+14, 2, GRAY);
  tft.fillCircle(x+16, y+19, 1, WHITE);
  tft.drawCircle(x+16, y+19, 1, GRAY);
  tft.fillTriangle(x+6, y+12, x+5, y+16, x+16, y+18, WHITE);
  tft.fillTriangle(x+15, y+19, x+5, y+16, x+16, y+18, WHITE);

  tft.drawCircle(x+12, y+31, 3, CYAN);
  tft.drawCircle(x+15, y+36, 2, CYAN);
  tft.drawCircle(x+18, y+38, 1, CYAN);

  tft.fillRect(x+2, y+14, 1, 3, RED);
  tft.fillRect(x+1, y+15, 3, 1, RED);

  tft.drawLine(x+5, y+16, x+15, y+19, GRAY);
}
/*void art_persod(int x, int y) {

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
  
}*/
void art_persod(int x, int y) {
  tft.fillCircle(x+15, y+6, 4, VERYLIGHTGRAY);
  tft.drawCircle(x+15, y+6, 4, GRAY);
  tft.fillRoundRect(x+16, y+3, 4, 6, 2, BLACK);

  tft.fillTriangle(x+17, y+8, x+19, y+8, x+18, y+9, BLUE);
  tft.drawLine(x+19, y+4, x+17, y+5, BLUE);
  tft.fillCircle(x+14, y+16, 5, VERYLIGHTGRAY);
  tft.fillTriangle(x+10, y+16, x+18, y+20, x+7, y+27, VERYLIGHTGRAY);
  tft.fillTriangle(x+12, y+29, x+18, y+20, x+7, y+27, VERYLIGHTGRAY);
  tft.drawLine(x+10, y+16, x+6, y+29, GRAY);

  tft.fillCircle(x+15, y+14, 2, WHITE);
  tft.drawCircle(x+15, y+14, 2, GRAY);
  tft.fillCircle(x+4, y+19, 1, WHITE);
  tft.drawCircle(x+4, y+19, 1, GRAY);
  tft.fillTriangle(x+14, y+12, x+15, y+16, x+4, y+18, WHITE);
  tft.fillTriangle(x+5, y+19, x+15, y+16, x+4, y+18, WHITE);

  tft.drawCircle(x+8, y+31, 3, CYAN);
  tft.drawCircle(x+5, y+36, 2, CYAN);
  tft.drawCircle(x+1, y+38, 1, CYAN);

  tft.fillRect(x+18, y+14, 1, 3, RED);
  tft.fillRect(x+17, y+15, 3, 1, RED);

  tft.drawLine(x+15, y+16, x+5, y+19, GRAY);
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

  tft.fillRect(x,y,BOX_SIZE,PLAYER_HEIGHT,BLACK);
}
void art_persod_shadow(int x, int y) {

  tft.fillRect(x,y,BOX_SIZE,PLAYER_HEIGHT,BLACK);
  
}
void art_slimeg_shadow(int x, int y) {

  tft.fillRect(x, y-5, 20, 25, BLACK);
  
}
void art_slimed_shadow(int x, int y) {

  tft.fillRect(x, y-5, 20, 25, BLACK);

}

//fonctions affichage entités

void last_square() {

  // recouvrement perso
  if(player.prevDirection) {
    art_persod_shadow(player.prevx,player.prevy);
  }
  else {
    art_persog_shadow(player.prevx,player.prevy);
  }

  // recouvrement slime
  if (slime.prevDirection) {
    art_slimed_shadow(slime.prevx,slime.prevy);
  }
  else {
    art_slimeg_shadow(slime.prevx,slime.prevy);
  }

  // recouvrement bouclier
  tft.fillRect(bouclier.prevx,bouclier.prevy,BOX_SIZE,PLAYER_HEIGHT,BLACK);

  if(slimeCollide || fireCollide || fellDown || shieldCollide || planteCollide) {
    tft.fillRect(player.x,player.y,BOX_SIZE,PLAYER_HEIGHT,BLACK);
    art_map(player.x/BOX_SIZE,player.y/BOX_SIZE,mapNumber);
    art_map((player.x + (BOX_SIZE - 1)) / BOX_SIZE,player.y/BOX_SIZE,mapNumber);
    art_map(player.x/BOX_SIZE,player.y/BOX_SIZE + 1,mapNumber);
    art_map((player.x + (BOX_SIZE - 1)) / BOX_SIZE,player.y/BOX_SIZE + 1,mapNumber);
    art_map(player.x/BOX_SIZE,(player.y + 2*BOX_SIZE - 1)/BOX_SIZE,mapNumber);
    art_map((player.x + (BOX_SIZE - 1)) / BOX_SIZE,(player.y + 2*BOX_SIZE - 1)/BOX_SIZE,mapNumber);
  }
  
  // refresh graphique perso
  art_map(player.prevx/BOX_SIZE,player.prevy/BOX_SIZE,mapNumber);
  art_map((player.prevx + (BOX_SIZE - 1)) / BOX_SIZE,player.prevy/BOX_SIZE,mapNumber);
  art_map(player.prevx/BOX_SIZE,player.prevy/BOX_SIZE + 1,mapNumber);
  art_map((player.prevx + (BOX_SIZE - 1)) / BOX_SIZE,player.prevy/BOX_SIZE + 1,mapNumber);
  art_map(player.prevx/BOX_SIZE,(player.prevy + 2*BOX_SIZE - 1)/BOX_SIZE,mapNumber);
  art_map((player.prevx + (BOX_SIZE - 1)) / BOX_SIZE,(player.prevy + 2*BOX_SIZE - 1)/BOX_SIZE,mapNumber);

  // refresh graphique slime
  art_map(slime.prevx/BOX_SIZE,slime.prevy/BOX_SIZE,mapNumber);
  art_map((slime.prevx + BOX_SIZE-1)/BOX_SIZE,slime.prevy/BOX_SIZE,mapNumber);
  art_map(slime.prevx/BOX_SIZE,(slime.prevy - BOX_SIZE+10)/BOX_SIZE,mapNumber);
  art_map((slime.prevx + BOX_SIZE-1)/BOX_SIZE,(slime.prevy-BOX_SIZE +10)/BOX_SIZE,mapNumber);

  // refresh graphique bouclier
  art_map(bouclier.prevx/BOX_SIZE,bouclier.prevy/BOX_SIZE,mapNumber);
  art_map((bouclier.prevx + (BOX_SIZE - 1)) / BOX_SIZE,bouclier.prevy/BOX_SIZE,mapNumber);
  art_map(bouclier.prevx/BOX_SIZE,bouclier.prevy/BOX_SIZE + 1,mapNumber);
  art_map((bouclier.prevx + (BOX_SIZE - 1)) / BOX_SIZE,bouclier.prevy/BOX_SIZE + 1,mapNumber);
  art_map(bouclier.prevx/BOX_SIZE,(bouclier.prevy + 2*BOX_SIZE - 1)/BOX_SIZE,mapNumber);
  art_map((bouclier.prevx + (BOX_SIZE - 1)) / BOX_SIZE,(bouclier.prevy + 2*BOX_SIZE - 1)/BOX_SIZE,mapNumber);
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
void display_bouclier() {

  if(bouclier.speedx > 0) {
    bouclier.direction = true;
  }
  if(bouclier.speedx < 0) {
    bouclier.direction = false;
  }
  if(bouclier.direction == true) {
    art_shieldd(bouclier.x,bouclier.y);
  }
  else {
    art_shieldg(bouclier.x,bouclier.y);
  }
}

// fonction affichage overlay

void display_L1Done() {
  tft.fillScreen(BLACK);
}
void display_L2Done() {
  tft.fillScreen(BLACK);
}
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
  display_fps();
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
void display_fps() {
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor((SCREENHEIGHT+BOX_SIZE), (SCREENHEIGHT*0.2)/8);
  tft.print("FPS:");
  tft.print(incr);
}
void display_life() {
  art_hp((SCREENWIDTH*3.3)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, RED);
  if(lifeCount <= 2) {
    art_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, BLACK);
  }
  if(lifeCount == 1) {
    art_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, BLACK);
  }
}
void display_menu() {
  display_topBar();
  display_life();
}
void display_map(int mapNumber) {
  for(int i = 0;i < MAP_COLS;i++) {
    for(int j = 0;j < MAP_ROWS;j++) {
      art_map(i,j,mapNumber);
    }
  }
}
void art_hp(int x, int y, int backgroundColor) { 
  tft.fillCircle(x+5, y+5, 6, backgroundColor);
  tft.fillCircle(x-5, y+5, 6, backgroundColor);
  tft.fillTriangle(x, y+18, x-11, y+5, x+11, y+5, backgroundColor);
}

//fonctions physiques

void buttons_update() {                           // update des boutons
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
void setup_player(Entity &player) {                                                             // initialise les données du joueur
  player.x = pstartx[mapNumber];
  player.y = pstarty[mapNumber];
  player.speedx = 0;
  player.speedy = 0;
  player.ax = 0;
  player.ay = 0;
  player.type = 'p';
  player.direction = true;
  slimeCollide = false;
}
void setup_slime(Entity &slime,int originx,int originy,int leftLimit,int rightLimit) {          // initialise les données du slime
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
  slime.speedx = 8;
  if(originx == -1 && originy == -1) {
    slime.enabled = false;
    slime.x = -100;
    slime.y = -100;
  }
  else {
    slime.enabled = true;
  }
  
  
}
void setup_bouclier(Entity &bouclier,int originx,int originy,int leftLimit,int rightLimit) {    // initialise les données du bouclier
  bouclier.x = originx*BOX_SIZE;
  bouclier.y = originy*BOX_SIZE;
  bouclier.ulimit = originy;
  bouclier.dlimit = originy;
  bouclier.llimit = leftLimit*BOX_SIZE;
  bouclier.rlimit = rightLimit*BOX_SIZE;
  bouclier.type = 'b';
  bouclier.state = false;
  bouclier.ax = 0;
  bouclier.ay = 0;
  bouclier.speedy = 0;
  bouclier.speedx = 0;
  if(originx == -1 && originy == -1) {
    bouclier.enabled = false;
    bouclier.x = -100;
    bouclier.y = -100;
  }
  else {
    bouclier.enabled = true;
  }
}
void setup_plante(int x, int y,bool dir) {                                                      // initialise les données de la plante
  plantex = BOX_SIZE*x;
  plantey = y*BOX_SIZE;
  if(x == -1 && y == -1) return;
  art_plante_off(plantex,plantey);
  dirPlante = dir;
}
void update_physics(Entity &player) {         // actualise la position du joueur
  player.prevx = player.x;    // sauvegarde la pos du joueur
  player.prevy = player.y;
  player.prevDirection = player.direction;
  slime.prevDirection = slime.direction;
  bouclier.prevDirection = bouclier.direction;

  if(airborn) player.ay = 2;            //tests pour determiner la position suivante du joueur

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
void update_slime(Entity &slime) {            // actualise la position du slime

  slime.prevx = slime.x;
  slime.prevy = slime.y;

  if(!slime.enabled) return;

  if (slime.x + slime.speedx > slime.rlimit-BOX_SIZE) {
    slime.speedx = -slime.speedx;
    slime.x = slime.rlimit-BOX_SIZE;
  }
  else if (slime.x + slime.speedx < slime.llimit+1) {
    slime.speedx = -slime.speedx;
    slime.x = slime.llimit+1;
  }
  else {
    slime.x = slime.x + slime.speedx;
  }  
}
void update_bouclier(Entity &bouclier) {      // actualise la position du bouclier

  bouclier.prevx = bouclier.x;
  bouclier.prevy = bouclier.y;
  if(!bouclier.enabled) {
    return;
  }

  if(player.y > bouclier.y - BOX_SIZE*4 + 10 && player.y < bouclier.y + 40) {
    if(player.x > bouclier.x - BOX_SIZE*3 && player.x <= bouclier.x) {
      bouclier.state = true;
      bouclier.ax = -1;
    }
    else if(player.x < bouclier.x + BOX_SIZE*3 && player.x > bouclier.x) {
      bouclier.state = true;
      bouclier.ax = 1;
    }
    else {
      bouclier.state = false;
      bouclier.ax = 0;
      bouclier.speedx = bouclier.speedx/2;
    }
  }
  else {
    bouclier.state = false;
    bouclier.ax = 0;
    bouclier.speedx = bouclier.speedx/2;
  }
  if (bouclier.x + bouclier.speedx > bouclier.rlimit-BOX_SIZE) {
    bouclier.ax = 0;
    bouclier.speedx = -bouclier.speedx/2;
    bouclier.x = bouclier.rlimit-BOX_SIZE;
  }
  else if (bouclier.x + bouclier.speedx < bouclier.llimit+1) {
    bouclier.ax = 0;
    bouclier.speedx = -bouclier.speedx/2;
    bouclier.x = bouclier.llimit+1;
  }
  else {
    bouclier.speedx = bouclier.speedx + bouclier.ax;
    bouclier.x = bouclier.x + bouclier.speedx;
  }
}
void update_plante() {                        // actuallise l'etat de la plante (caché ou pas)
  if(planteCollide) {
    if(dirPlante) art_planted(plantex,plantey-BOX_SIZE);
    else art_planteg(plantex,plantey-BOX_SIZE);
  }
  if(enablePlante) return;
  if(player.y > plantey - BOX_SIZE*4 + 10 && player.y < plantey + 40) {
    if(player.x > plantex - BOX_SIZE*3 && player.x <= plantex) {
      enablePlante = true;
    }
    else if(player.x < plantex + BOX_SIZE*3 && player.x > plantex) {
      enablePlante = true;
    }
    else {
      enablePlante = false;
    }
  }
  if(enablePlante) {
    tft.fillRect(plantex,plantey,BOX_SIZE,BOX_SIZE,BLACK);
    if(dirPlante) art_planted(plantex,plantey-BOX_SIZE);
    else art_planteg(plantex,plantey-BOX_SIZE);
  }
}
void update_xposition(Entity &player) {       // gere les physiques horizontales

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
void update_yposition(Entity &player) {       // gere les physiques verticales
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
    if(nextFloor - nextCeilling > 40) {
      airborn = true;
      player.speedy = -13;
    }
	}

  if(player.speedy > 18) {
    player.speedy = 18;
  }

  if(nextFloor - player.y > PLAYER_HEIGHT + 5) {
    airborn = true;
  }
  
  player.y = player.y + player.speedy;
}
void next_walls(Entity &player) {             // obtiens la position des prochains murs par rapport au joueur
  tabx = player.prevx/BOX_SIZE;
  tabx2 = (player.prevx+BOX_SIZE-1) /BOX_SIZE;
  taby = player.prevy/BOX_SIZE;
  taby2 = (player.prevy+BOX_SIZE-1)/BOX_SIZE;
  taby3 = (player.prevy + PLAYER_HEIGHT-1)/BOX_SIZE;
  nextRightWall = SCREENWIDTH;
  nextLeftWall = 0;
  nextCeilling = BOX_SIZE; //décalage depuis le haut de l'écran pour considérer la barre de menu
  nextFloor = SCREENHEIGHT;

  // boucle mur droit
  for(int i = tabx + 1;i < MAP_COLS;i++) {
    if(ground[mapNumber][taby][i] || ground[mapNumber][taby2][i] || ground[mapNumber][taby3][i]) {
      nextRightWall = i * BOX_SIZE;
      break;
    }
  }
  // boucle sol
  for(int i = taby + 1;i < MAP_ROWS;i++) {
    if(ground[mapNumber][i][tabx] == true || ground[mapNumber][i][tabx2] == true) {
      nextFloor = i * BOX_SIZE;
      break;
    }
  }
  // boucle mur gauche
  for(int j = 0;j < tabx;j++) {
    if(ground[mapNumber][taby][j] || ground[mapNumber][taby2][j] || ground[mapNumber][taby3][j]) {
      nextLeftWall = (j+1) * BOX_SIZE;
    }
  }
  // plafond
  for(int j = 0;j < taby;j++) {
    if(ground[mapNumber][j][tabx] == true || ground[mapNumber][j][tabx2] == true) {
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
    tft.setTextColor(WHITE);
    tft.setCursor(10, (SCREENHEIGHT*0.2)/8);
    if(min < 10) {
      tft.print("0");
      tft.print(min);
    }
    else tft.print(min);
    tft.setCursor(31,(SCREENHEIGHT*0.2)/8);
    tft.print(":");
    tft.setCursor(40, (SCREENHEIGHT*0.2)/8);
    if(sec < 10) {
      tft.print("0");
      tft.print(sec);
    }
    else tft.print(sec);
    tft.setCursor(61,(SCREENHEIGHT*0.2)/8);
    tft.print(":");
    tft.setCursor(70, (SCREENHEIGHT*0.2)/8);
    tft.print("0");
    tft.print(mil);
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
void coll_shield(Entity &player,Entity &bouclier) {

  int diffx = player.x - bouclier.x;
  int diffy = player.y - bouclier.y;
  if((diffx < BOX_SIZE && diffx > -BOX_SIZE) && (diffy < 2*BOX_SIZE && diffy > -2*BOX_SIZE)) {
    shieldCollide = true;
  }
  else {
    shieldCollide = false;
  }
}
void coll_plante() {
  int diffx = player.x - plantex;
  int diffy = player.y - plantey+BOX_SIZE;
  if((diffx < BOX_SIZE && diffx > -BOX_SIZE) && (diffy < 2*BOX_SIZE && diffy > -2*BOX_SIZE)) {
    planteCollide = true;
  }
  else {
    planteCollide = false;
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
void portal_collision(Entity &player) {
  int diffx = player.x - portalx*BOX_SIZE;
  int diffy = player.y - portaly*BOX_SIZE;
  if((diffx < BOX_SIZE && diffx > -BOX_SIZE) && (diffy < BOX_SIZE && diffy > -2*BOX_SIZE)) {
    portalCollide = true;
  }
  else {
    portalCollide = false;
  }
}
void fire_collision(Entity &player) {
  int diffx = player.x - flammex[0]*BOX_SIZE;
  int diffy = player.y - flammey[0]*BOX_SIZE;
  if((diffx < BOX_SIZE && diffx > -BOX_SIZE) && (diffy < BOX_SIZE && diffy > -2*BOX_SIZE)) {
    fireCollide = true;
  }
  else {
    fireCollide = false;
  }
  diffx = player.x - flammex[1]*BOX_SIZE;
  diffy = player.y - flammey[1]*BOX_SIZE;
  if((diffx < BOX_SIZE && diffx > -BOX_SIZE) && (diffy < BOX_SIZE && diffy > -2*BOX_SIZE)) {
    fire2Collide = true;
  }
  else {
    fire2Collide = false;
  }
  fireCollide = fireCollide || fire2Collide;
}
void fell_down() {
  if(player.y > SCREENHEIGHT - PLAYER_HEIGHT-18) fellDown = true;
  else fellDown = false;
}
void collide_test() {
  portal_collision(player);
  fire_collision(player);
  coll_slime(player,slime);
  coll_shield(player,bouclier);
  coll_plante();
  fell_down();
}
void init_gameValues() {
  lifeCount = 3;
  mapNumber = 0;
  party.partyTimeMil = 0;
  party.partyTimeMin = 0;
  party.partyTimeSec = 0;
}

//=======================================================================================

void setup() {

  //variable pour demmarage
  homeMenu = true;

	//setup du compteur de temps
	timebuffer = millis();
  timeBufferFPS = millis(); 

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
      init_gameValues();
    }
  }
  
	//setup jeu
	if(gameSetup == true) {

    //setup graphique
    tft.fillScreen(BLACK);
    display_map(mapNumber);
    display_menu();
    timeCalculation(party);

    //setup entités
    setup_player(player);
    display_player();
    if(mapNumber == 0) {
      enablePlante = false;
      setup_plante(-1,-1,false);
      setup_bouclier(bouclier,-1,-1,0,0);
      setup_slime(slime,3,10,3,10);
      flammex[0] = 13;
      flammey[0] = 10;
      flammex[1] = 14;
      flammey[1] = 10;
    }  
    else if(mapNumber == 1) {
      enablePlante = false;
      setup_slime(slime,-1,-1,0,0);
      setup_bouclier(bouclier,10,9,5,15);
      setup_plante(15,7,false);
      flammex[0] = 6;
      flammey[0] = 4;
      flammex[1] = 12;
      flammey[1] = 4;
    }
    else {
      enablePlante = false;
      setup_bouclier(bouclier,5,9,2,9);
      setup_plante(0,7,true);
      flammex[0] = 3;
      flammey[0] = 4;
      flammex[1] = 10;
      flammey[1] = 4;
    }

    //setup booleens
    finished = false;
    gameSetup = false;
    airborn = true;
  }

  //deroulement jeu
  while (!finished) {
		   
		timePassed = millis() - timebuffer;
    timePassedFPS = millis() - timeBufferFPS;

    //boucle fps
    if(timePassedFPS > 1000) {

      tft.fillRect(SCREENWIDTH-17, (SCREENHEIGHT*0.2)/8,20,18,BLACK);

      display_fps();

      timeBufferFPS = millis();
      incr = 0;
    }

    //boucle pour répeter une action tout les x millisecondes
    if(timePassed > refresh) {  
      buttons_update();
      next_walls(player);
      update_physics(player);
      update_slime(slime);
      update_bouclier(bouclier);
      update_plante();
      collide_test();
      timeCalculation(party);
      last_square();
      display_player();
      if(mapNumber == 0) display_slime(); 
      else if(mapNumber == 1) {
        display_bouclier();
      }
      else {
        display_bouclier();
      }
      if(slimeCollide || fireCollide || fellDown || shieldCollide || planteCollide) {
        life_sub();
        last_square();
        setup_player(player);
        update_plante();
      }
      if(portalCollide) {
        finished = true;
        gOverScreen = false;
        if(mapNumber == 0) {
          mapNumber = 1;
          L1Done = true;
        }
        else if(mapNumber == 1) {
          mapNumber = 2;
          L2Done = true;
        }
        else {
          mapNumber = 0;
          winScreen = true;
        }
      }
      if(digitalRead(27)) {
        finished = true;
        gOverScreen = false;
        if(mapNumber == 0) {
          mapNumber = 1;
          L1Done = true;
        }
        else if(mapNumber == 1) {
          mapNumber = 2;
          L2Done = true;
        }
        else {
          mapNumber = 0;
          winScreen = true;
        }
      }
      timebuffer = millis();
      incr++;
    } 
  }

  done = false;
  while(L1Done){
    if(!done) {
      done = true;
      display_L1Done();
      mapNumber = 1;
    }
    if(digitalRead(27)) {
      gameSetup = true;
      L1Done = false;
    }
  }
  
  done = false;
  while(L2Done){
    if(!done) {
      done = true;
      display_L2Done();
      mapNumber = 2;
    }
    if(digitalRead(27)) {
      gameSetup = true;
      L2Done = false;
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