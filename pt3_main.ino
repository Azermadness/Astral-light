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

#define PSTARTX 50 // position de départ du joueur
#define PSTARTY 50

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
int nextRightWall;
int nextLeftWall;
int nextCeilling;
int nextFloor;
bool ground[MAP_ROWS][MAP_COLS] = {
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {true,true,false,false,false,false,false,false,true,false,false,false,false,false,false,false},
  {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true}
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
bool endScreen;
bool finished;
bool physicsUpdated;
bool airborn;
bool done;

//==========================================================================================

//setup structures

struct Entity {
  	double speedx;      // vitesse horizontale
    double speedy;      // vitesse verticale
    double x;           // Coordonnée X actuelles
  	double y;           // Coordonnée Y actuelles
    double prevx;       // previous X
    double prevy;       // previous Y
    int ax;             // accel horizontale
    int ay;             // accel verticale
    char type;          // definit le type d'entité pour des tests
    bool state;         // etat joueur (vie/mort)
    int ulimit;         //
    int dlimit;         //
    int llimit;         // limite utilisées pour les collisions
    int rlimit;         //
};

//init structures

struct Entity player;
struct Entity slime;

//==========================================================================================

// fonctions graphiques

void art_heart(int x, int y, int color) {
  tft.fillCircle(x+25, y-25, 30, color);
  tft.fillCircle(x-25, y-25, 30, color);
  tft.fillTriangle(x, y+50, x-51, y-10, x+51, y-10, color);
  tft.fillCircle(x+14, y-9, 8, BLACK);
  tft.fillCircle(x-14, y-9, 8, BLACK);
}
void art_star(int color) {
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
void art_blockterre(int x, int y) {
    tft.fillRect(x, y, BOX_SIZE, BOX_SIZE, RED);
}
void art_blocksurface(int x, int y) {
    tft.fillRect(x, y, BOX_SIZE, 5, ORANGE);
    tft.fillRect(x, y+5, BOX_SIZE, 15, RED);
    tft.fillRect(x, y+5, 2, 1, ORANGE);
    tft.fillRect(x, y+6, 1, 1, ORANGE);
    tft.fillRect(x+4, y+4, 2, 1, RED);
    tft.fillRect(x+7, y+5, 1, 1, ORANGE);
    tft.fillRect(x+9, y+4, 1, 1, RED);
    tft.fillRect(x+12, y+4, 1, 1, RED);
    tft.fillRect(x+14, y+5, 1, 1, ORANGE);
    tft.fillRect(x+17, y+4, 2, 1, RED);
    tft.fillRect(x+17, y+3, 1, 1, RED);
}
void art_fire(int x, int y) {
    y=y+10;
    tft.fillTriangle(x-8, y-5, x-8, y-14, x-2, y-9, RED); // point blanc
    tft.fillTriangle(x-7, y-11, x+2, y-8, x-1, y-18, RED); // point jaune
    tft.fillTriangle(x, y-8, x+4, y-9, x+4, y-22, RED); // point bleu f
    tft.fillTriangle(x+4, y-8, x+8, y-13, x+8, y-6, RED); // point bleu c
    tft.fillCircle(x, y, 9, RED);
    tft.fillCircle(x, y+3, 8, ORANGE);
    tft.fillCircle(x, y+4, 5, YELLOW); 
}
void art_Map() {
    for(int i = 0; i<SCREENWIDTH; i=i+BOX_SIZE){
        art_blocksurface(i, 11*BOX_SIZE);              // Sol
    }
    art_blocksurface(0, 8*BOX_SIZE);                  // escalier
    art_blockterre(0, 10*BOX_SIZE);
    art_blockterre(0, 9*BOX_SIZE);
    art_blockterre(0, 11*BOX_SIZE);
    art_blocksurface(BOX_SIZE, 10*BOX_SIZE);                 
    art_blockterre(BOX_SIZE, 11*BOX_SIZE);

    art_blocksurface(8*BOX_SIZE, 10*BOX_SIZE);                 // petit Mur
    art_blockterre(8*BOX_SIZE, 11*BOX_SIZE);
}
void art_portail(int x, int y) {
    tft.fillCircle(x, y, 10, BLUE);

    tft.fillRect(x-1, y-1, 2, 2, CYAN);
    tft.fillRect(x, y-2, 1, 1, CYAN);
    tft.fillRect(x-1, y-3, 1, 1, CYAN);
    tft.fillRect(x-2, y-4, 1, 1, CYAN);
    tft.fillRect(x-5, y-5, 3, 1, CYAN);
    tft.fillRect(x-6, y-4, 1, 1, CYAN);
    tft.fillRect(x-8, y-3, 2, 1, CYAN);
    tft.fillRect(x-9, y-2, 1, 3, CYAN);
    tft.fillRect(x-10, y+1, 1, 3, CYAN);

    tft.fillRect(x-2, y-1, 1, 1, CYAN);
    tft.fillRect(x-3, y, 1, 1, CYAN);
    tft.fillRect(x-4, y+1, 1, 1, CYAN);
    tft.fillRect(x-5, y+2, 1, 3, CYAN);
    tft.fillRect(x-4, y+5, 1, 1, CYAN);
    tft.fillRect(x-3, y+6, 1, 2, CYAN);
    tft.fillRect(x-2, y+8, 3, 1, CYAN);
    tft.fillRect(x+1, y+9, 3, 1, CYAN);

    tft.fillRect(x-1, y+1, 1, 1, CYAN);
    tft.fillRect(x, y+2, 1, 1, CYAN);
    tft.fillRect(x+1, y+3, 1, 1, CYAN);
    tft.fillRect(x+2, y+4, 3, 1, CYAN);
    tft.fillRect(x+5, y+3, 1, 1, CYAN);
    tft.fillRect(x+6, y+2, 2, 1, CYAN);
    tft.fillRect(x+8, y-1, 1, 3, CYAN);
    tft.fillRect(x+9, y-4, 1, 3, CYAN);

    tft.fillRect(x+1, y, 1, 1, CYAN);
    tft.fillRect(x+2, y-1, 1, 1, CYAN);
    tft.fillRect(x+3, y-2, 1, 1, CYAN);
    tft.fillRect(x+4, y-5, 1, 3, CYAN);
    tft.fillRect(x+3, y-6, 1, 1, CYAN);
    tft.fillRect(x+2, y-8, 1, 2, CYAN);
    tft.fillRect(x-1, y-9, 3, 1, CYAN);
    tft.fillRect(x-4, y-10, 3, 1, CYAN);
}
void art_player(int x,int y) {
  tft.fillRect(x+1,y+10,BOX_SIZE-1,BOX_SIZE-1,MAGENTA);
  tft.fillCircle(x+10,y+30,9,MAGENTA);
  tft.fillCircle(x+10,y+10,9,MAGENTA);
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
void art_slime(double x,double y) {
  if(slime.state == true) {
    tft.fillCircle(x+10,y+10,9,CYAN);
  }
  else tft.fillCircle(x+10,y+10,9,GREEN);
}

//fonctions affichage joueur

void last_square() {
  tft.fillRect(slime.prevx,slime.prevy,BOX_SIZE,BOX_SIZE,BLACK);
  tft.fillRect(player.prevx,player.prevy,BOX_SIZE,PLAYER_HEIGHT,BLACK);
}
void display_player() {
  art_player(player.x,player.y);
}
void display_slime() {
  art_slime(slime.x,slime.y);
}

// fonction affichage overlay

void display_winMenu() {
  tft.fillScreen(BLACK);
  art_star(GRAY);
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
  art_star(GRAY);
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
  tft.fillScreen(LIGHTGRAY);
  display_life();
  display_time();
  display_score();
}
void display_multiMenu() {
  tft.fillScreen(BLACK);
  art_star(GRAY);
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
  art_star(GRAY);
  tft.setCursor((SCREENWIDTH/9)-15, SCREENHEIGHT/8);//Peut-être à modifier
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.print("ASTRAL LIGHT");
  
  art_fire(SCREENWIDTH/2, SCREENHEIGHT/2);

  tft.setTextSize(2);
  tft.setCursor((SCREENWIDTH*0.4)/7, (SCREENHEIGHT*3.4)/4);//A centrer
  tft.print("PRESS THE");

  tft.setCursor((SCREENWIDTH*4.3)/7, (SCREENHEIGHT*3.4)/4);//A centrer
  tft.print("FOR START");

  art_button(SCREENWIDTH/2, (SCREENHEIGHT*4.4)/5, GREEN, LIME);//A centrer
}
void display_topBar() {
  tft.fillRect(0, 0, SCREENWIDTH, 30, BLACK);
}
void display_score() {
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  
  tft.setCursor((SCREENHEIGHT*7.2)/8, (SCREENHEIGHT*0.2)/8);
  tft.print("SCORE: ");
  
  tft.setCursor((SCREENHEIGHT*8)/8+50, (SCREENHEIGHT*0.2)/8);
  tft.print("10");
}
void display_life() {
  art_hp((SCREENWIDTH*3.3)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, RED);
}
void display_time() {  
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  
  //tft.setCursor((SCREENHEIGHT*0.1)/8, (SCREENHEIGHT*0.2)/8);
  //tft.print("TIME: ");
  
  tft.setCursor((SCREENHEIGHT*0.1)/8, (SCREENHEIGHT*0.2)/8);
  tft.print("00:01:01");
}
void display_menu() {
  display_topBar();
  display_life();
  display_time();
  display_score();
}
void art_hp(int x, int y, int backgroundColor) { 
  tft.fillCircle(x+5, y+5, 6, backgroundColor);
  tft.fillCircle(x-5, y+5, 6, backgroundColor);
  tft.fillTriangle(x, y+18, x-11, y+5, x+11, y+5, backgroundColor);
}

//==========================================================================================

//setup fonctions

/*void save_screen() {
  incr = 0;
  for(int i = 15;i < (SCREENHEIGHT-20)/2;i++) {
    for(int j = 0;j < SCREENWIDTH/2;j++) {
      if(tft2.readPixel(i*2,j*2) == WHITE) {
        screenSave[incr] = i;
        screenSave[incr+1] = j;
        incr = incr+2;
        Serial.println(screenSave[incr]);
        Serial.println(screenSave[incr+1]);
      }
    }
  }
}*/

/*void print_save(int x,int y,int x1,int y1) {
  for(int i = 0;i < 100;i++) {
    if(screenSave[i]<x1 && screenSave[i]>x && screenSave[i+1]>y && screenSave[i+1]<y1) {
      tft.drawPixel(screenSave[i],screenSave[i+1],WHITE);
    }
  }
}*/

/*void display_buttons() {
  if(upPressed) {
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("UP");
  }
  else {
    tft.setCursor(0, 0);
    tft.setTextColor(LIGHTGRAY);
    tft.setTextSize(2);
    tft.print("UP");
  }
  if(downPressed) {
    tft.setCursor(20, 20);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("DOWN");
  }
  else {
    tft.setCursor(20, 20);
    tft.setTextColor(LIGHTGRAY);
    tft.setTextSize(2);
    tft.print("DOWN");
  }
  if(leftPressed) {
    tft.setCursor(40, 40);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("LEFT");
  }
  else {
    tft.setCursor(40, 40);
    tft.setTextColor(LIGHTGRAY);
    tft.setTextSize(2);
    tft.print("LEFT");
  }
  if(rightPressed) {
    tft.setCursor(60, 60);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("RIGHT");
  }
  else {
    tft.setCursor(60, 60);
    tft.setTextColor(LIGHTGRAY);
    tft.setTextSize(2);
    tft.print("RIGHT");
  }
}*/

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
  if(player.x > slime.x - BOX_SIZE*3 && player.x < slime.x) { // todo player near left
    slime.state = true;
    slime.ax = -1;
  }
  else if(player.x < slime.x + BOX_SIZE*3 && player.x > slime.x) { // todo player near right
    slime.state = true;
    slime.ax = 1;
  }
  else {
    slime.state = false;
    slime.ax = 0;
    slime.speedx = slime.speedx/2;
  }
  if (slime.x + slime.speedx > slime.rlimit-BOX_SIZE) {
    slime.ax = 0;
    slime.speedx = -slime.speedx*0.5;
    slime.x = slime.rlimit-BOX_SIZE;
  }
  else if (slime.x + slime.speedx < slime.llimit+1) {
    slime.ax = 0;
    slime.speedx = -slime.speedx*0.5;
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
    player.speedy = -player.speedy*0.5;
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
  else if(player.y + player.speedy < 0) {
    player.speedy = -player.speedy*0.5;
    player.y = 0;
  }

  if(nextFloor - player.y > PLAYER_HEIGHT + 5) {
    airborn = true;
  }

  if(!airborn) {
    player.ax = 0;
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
  taby2 = (player.prevy+PLAYER_HEIGHT-1)/BOX_SIZE;
  nextRightWall = SCREENWIDTH;
  nextLeftWall = 0;
  nextCeilling = 0;
  nextFloor = SCREENHEIGHT - BOX_SIZE;

  // boucle mur droit
  for(int i = tabx + 1;i < MAP_COLS;i++) {
    if(ground[taby][i] == true || ground[taby2][i] == true) {
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
    if(ground[taby][j] == true || ground[taby2][j] == true) {
      nextLeftWall = (j+1) * BOX_SIZE;
    }
  }
  // plafond
  for(int j = 0;j < tabx;j++) {
    if(ground[j][tabx] == true || ground[j][tabx2] == true) {
      nextCeilling = (j+1) * BOX_SIZE;
    }
  }

  //tft.fillRect(0,nextFloor,SCREENWIDTH,1,BLUE);
  //tft.fillRect(0,nextCeilling,SCREENHEIGHT,-1,WHITE);

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

//===========================================================================================


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
    art_star(WHITE);
    art_Map();
    display_menu();

    //setup entités
    setup_player(player);
    display_player();
    setup_slime(slime,12,10,9,16);
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
      last_square();
      display_player();
      display_slime();
      if(digitalRead(27)) {
        finished = true;
        endScreen = true;
      }
		  timebuffer = millis();
    } 
  }

  done = false;
  while(endScreen) {
    if(!done) {
      done = true;
      display_winMenu();
    }
    if(digitalRead(27)) {
      homeMenu = true;
      endScreen = false;
    }
  }
}