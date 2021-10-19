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

#define PSTARTX 50 // position de départ du joueur
#define PSTARTY 50

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
ILI9341_due tft2 = ILI9341_due(TFT_CS, TFT_DC);

//=========================================================================================

//setup variables

int screenSave[200];
int incr;
int tabx;
int taby;
int nextRightWall;
int nextLeftWall;
int nextCeilling;
int nextFloor;
bool ground[12][16] = {
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false},
  {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
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
bool testScreen;
bool homeMenu;
bool finished;
bool physicsUpdated;
bool airborn;
bool done;

//==========================================================================================

//setup structures

struct Entity {
  	double speedx; // vitesse horizontale
    double speedy; // vitesse verticale
    double x; // Coordonnée X actuelles
  	double y; // Coordonnée Y actuelles
    double prevx; // previous X
    double prevy; // previous Y
    int ax; // accel horizontale
    int ay; // accel verticale
  	int invincibility; // Pour le pacman: supérieur à 0 = invinsible. Pour le fantôme: supérieur à 0 = fuite
};

//==========================================================================================

// fonctions graphiques

void art_heart(int x, int y, int color) {
  tft.fillCircle(x+25, y-25, 30, color);
  tft.fillCircle(x-25, y-25, 30, color);
  tft.fillTriangle(x, y+50, x-51, y-10, x+51, y-10, color);
  tft.fillCircle(x+14, y-9, 8, BLACK);
  tft.fillCircle(x-14, y-9, 8, BLACK);
}
void art_star() {
    for(int x=0; x < SCREENWIDTH; x=x+100) {
        for(int y=0; y < SCREENHEIGHT; y=y+100) {
            //TODO placer etoiles dans tableau
            tft.fillRect(x, y, 2, 2, WHITE);  //Carré d'un pixel
            tft.fillRect(x+20, y+14, 2, 2, WHITE);
            tft.fillRect(x+44, y+78, 2, 2, WHITE);
            tft.fillRect(x+70, y+100, 2, 2, WHITE);
            tft.fillRect(x+16, y+40, 2, 2, WHITE);
            tft.fillRect(x+88, y+80, 2, 2, WHITE);
            tft.fillRect(x+30, y+30, 2, 2, WHITE);
            tft.fillRect(x+5, y+60, 2, 2, WHITE);
            tft.fillRect(x+12, y+88, 2, 2, WHITE);
            tft.fillRect(x+60, y+2, 2, 2, WHITE);
            tft.fillRect(x+76, y+55, 2, 2, WHITE);
            tft.fillRect(x+95, y+15, 2, 2, WHITE);

            tft.fillRect(x+12, y+8, 3, 3, WHITE);  //Carré de deux pixel
            tft.fillRect(x+20, y+40, 3, 3, WHITE);
            tft.fillRect(x+50, y+10, 3, 3, WHITE);
            tft.fillRect(x+75, y+80, 3, 3, WHITE);
            tft.fillRect(x+15, y+50, 3, 3, WHITE);
            tft.fillRect(x+95, y+75, 3, 3, WHITE);
            tft.fillRect(x+66, y+98, 3, 3, WHITE);
            tft.fillRect(x+33, y+55, 3, 3, WHITE);
            tft.fillRect(x+41, y, 3, 3, WHITE);

            tft.fillRect(x+5, y+21, 6, 2, WHITE);   //Croix 
            tft.fillRect(x+7, y+18, 2, 8, WHITE);
            tft.fillRect(x+45, y+41, 6, 2, WHITE);   //Croix 
            tft.fillRect(x+47, y+38, 2, 8, WHITE);
            tft.fillRect(x+39, y+80, 6, 2, WHITE);   //Croix 
            tft.fillRect(x+41, y+77, 2, 8, WHITE);
            tft.fillRect(x+85, y+90, 6, 2, WHITE);   //Croix 
            tft.fillRect(x+87, y+87, 2, 8, WHITE);
            tft.fillRect(x+98, y+65, 6, 2, WHITE);   //Croix 
            tft.fillRect(x+100, y+62, 2, 8, WHITE);
            tft.fillRect(x+20, y+50, 6, 2, WHITE);   //Croix 
            tft.fillRect(x+22, y+47, 2, 8, WHITE);
        }
    }
}
void art_blockterre(int x, int y) {
    tft.fillRect(x, y, 20, 20, RED);
}
void art_blocksurface(int x, int y) {
    tft.fillRect(x, y, 20, 5, ORANGE);
    tft.fillRect(x, y+5, 20, 15, RED);
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
    for(int i = 0; i<320; i=i+20){
        art_blocksurface(i, 220);              // Sol
        if(i<240){
            art_blocksurface(i, 60);           // Sol suspendu   
        } 
    }

    art_fire(280, 200); 
    art_portail(10, 50); 
    
    art_blockterre(200, 220);                   //Escalier
    art_blockterre(220, 220);
    art_blockterre(240, 220);                 
    art_blockterre(220, 200);
    art_blockterre(240, 180);
    art_blockterre(240, 200);

    art_blocksurface(200, 200);
    art_blocksurface(220, 180);
    art_blocksurface(240, 160);

    art_blockterre(0, 80);                     //Mur gauche
    art_blockterre(0, 100);
    art_blockterre(0, 120);
    art_blockterre(0, 140);
    art_blockterre(0, 160);
    art_blockterre(0, 180);
    art_blockterre(0, 200);
    art_blockterre(0, 220);

    art_blockterre(300, 140);                   //Mur droit
    art_blockterre(300, 160);
    art_blockterre(300, 180);
    art_blockterre(300, 200);
    art_blockterre(300, 220);
    art_blocksurface(300, 120);
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
void display_playtest(int x,int y) {
  tft.fillRect(x,y,20,40,WHITE);
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

//fonctions affichage joueur

void last_square(Entity &player) {
  tft.fillRect(player.prevx,player.prevy,20,40,BLACK);
}
void display_player(Entity &player) {
  last_square(player);
  display_playtest(player.x,player.y);
}

// fonction affichage overlay

void display_winMenu() {
  int backColor = BLACK;
  tft.fillScreen(backColor);
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
void display_topMenu() {
  tft.fillScreen(LIGHTGRAY);
  display_life();
  display_time();
  display_score();
}
void display_multiMenu() {
  int backColor = BLACK;
  tft.fillScreen(backColor);
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
  int backColor = BLACK;
  tft.fillScreen(backColor);
  tft.setCursor(SCREENWIDTH/7, SCREENHEIGHT/8);//Peut-être à modifier
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.print("Nom du jeu");
  
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
  art_hp((SCREENWIDTH*3.3)/8, (SCREENHEIGHT*0.1)/8, GREEN);
  art_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, RED);
  art_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, GREEN);
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
  physicsUpdated = true;
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
  
  if(player.x + player.speedx > SCREENWIDTH-20) {
    player.speedx = -player.speedx;
    player.ax = 0;
    player.x = player.x + player.speedx;
  }
  else if (player.x +player.speedx < 0) {
    player.speedx = -player.speedx;
    player.ax = 0;
    player.x = player.x + player.speedx;
  }
  else player.x = player.x + player.speedx;
}
void update_yposition(Entity &player) {
  player.speedy = player.speedy + player.ay;
  if(player.y + player.speedy > SCREENHEIGHT - 40 && !downPressed) {
    player.speedy = -player.speedy*0.5;
    player.y = SCREENHEIGHT - 40;
    if(upPressed) {
      airborn = false;
    }
    if(player.speedy < 5 && player.speedy > -5) {
      airborn = false;
    }
  }
  else if(player.y + player.speedy > SCREENHEIGHT - 40 && downPressed) {
    player.speedy = 0;
    player.y = SCREENHEIGHT - 40;
    airborn = false;
  }
  else if(player.y + player.speedy < 0) {
    player.speedy = -player.speedy*0.5;
    player.y = 0;
  }

  if(!airborn) {
    player.ax = 0;
    player.speedy = 0;
  }
  
  if(!airborn && upPressed) {
    airborn = true;
    player.speedy = -20;
	}

  player.y = player.y + player.speedy;
}
void next_walls(Entity &player) {
  tabx = player.prevx;
  taby = player.prevy;
  nextRightWall = tabx/20;
  nextLeftWall = tabx/20;
  nextCeilling = taby/20;
  nextFloor = taby/20;
  
  //TODO

  Serial.println(nextRightWall);
  Serial.println(nextLeftWall);
  Serial.println(nextFloor);
  Serial.println(nextCeilling);
  
}

//=======================================================================================

//init structures

struct Entity player;

//=======================================================================================

void setup() {

  //variable pour demmarage
  testScreen = true;
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

	//condition de départ boucles test
	finished = false;

  done = false;
  while(!homeMenu) {
    if(!done) {
      done = true;
      delay(100);
      display_multiMenu();
    }
    if(digitalRead(27)) {
      homeMenu = true;
    }
  }

	//setup differents test
	if(testScreen == true) {
    tft.fillScreen(BLACK);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
		testScreen = false;
    airborn = true;
    art_star();
    //save_screen();
    setup_player(player);
    art_Map();
    display_menu();
    display_player(player);
  }

  // Tant que start n'a pas été appuyé
  while (!finished) {
		  
		timePassed = millis() - timebuffer;
    //boucle pour répeter une action tout les x millisecondes
    if(timePassed > refresh) {  
      buttons_update();
      next_walls(player);
      update_physics(player);
      //print_save(0,0,320,220);
      display_player(player);
      if(digitalRead(27)) {
        finished = true;
      }
		timebuffer = millis();
    } 
  }
  done = false;
  while(!testScreen) {
    if(!done) {
      done = true;
      tft.fillScreen(BLACK);
      tft.setCursor(50, 50);
      tft.setTextSize(8);
      tft.setTextColor(WHITE);
      tft.print("Game Over");
    }
    if(digitalRead(27)) {
      testScreen = true;
    }
  }
}