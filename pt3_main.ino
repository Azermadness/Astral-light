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

#define SCORING1      0xFCC0
#define SCORING2      0xFE60
#define HIGHLIGHT     0xE71C

/* ------------------------------- */
/* Longueur en pixel de l'écran */
#define SCREENWIDTH 320 // Equivalent à tft.width()
/* Largeur en pixel de l'écran */
#define SCREENHEIGHT 240 // Equivalent à tft.height()

#define PSTARTX 50 // position de départ du joueur
#define PSTARTY 50

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
ILI9341_due tft2 = ILI9341_due(TFT_CS, TFT_DC);

//=========================================================================================

//setup variables

int screenSave[200];
int incr;

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
bool finished;
bool physicsUpdated;
bool airborn;

//==========================================================================================

//setup structures

struct Entity {
  	char type; // pacman = P ou ghost = G
  	double speedx; // vitesse horizontale
    double speedy; // vitesse verticale
    double x; // Coordonnée X actuelles
  	double y; // Coordonnée Y actuelles
    double prevx; // previous X
    double prevy; // previous Y
    int ax; // accel horizontale
    int ay; // accel verticale
  	char direc; // GAUCHE = G, Droite = D
  	int invincibility; // Pour le pacman: supérieur à 0 = invinsible. Pour le fantôme: supérieur à 0 = fuite
};

//==========================================================================================

// fonctions graphiques

// ======== fonction coeur ==========

int rEye = 8; // Rayon du du cercle des yeux
int xEye = 8; // Décalage en x du des yeux par rapport au centre
int yEye = 10; // Décalage en y du des yeux par rapport au centre
int yNose = 32; // Décalage en y par rapport au centre
int xNoseCorner = 4; // Décalage en x du coin du nez par rapport au haut du nez

/* Coordonnées pour le bouton */
int rButton = 24; // Rayon du bouton principal
int rButton2 = 18; // Rayon de l'effet sur le bouton

/* Coordonnées pour l'étoile */
// Décalage par rapport au centre du carré
// Décalage des différente coordonnées du triangle autour du carré
int xDecaTriangle4_5 = 9*4;
int yDecaTriangle4_5 = 9*4;

void display_heart(int x, int y) {

  tft.fillCircle(x+8, y+10, 10, BLACK);
  tft.fillCircle(x-8, y+10, 10, BLACK);
  tft.fillCircle(x+xEye, y+yEye, rEye, RED);
  tft.fillCircle(x-xEye, y+yEye, rEye, RED);

  tft.fillTriangle(x, y+36, x-18, y+12, x+18, y+12, BLACK);
  tft.fillTriangle(x, y+yNose, x-16, y+10, x+16, y+10, RED);
}

// fonction etoile
#define REDF     0xF800

void display_star() {
    for(int x=0; x < SCREENWIDTH; x=x+100) {
        for(int y=0; y < SCREENHEIGHT; y=y+100) {
            
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

// fonction bloc de terre
void display_blockterre(int x, int y, int l, int h){
  tft.fillRect(x, y, l, h, RED);
}

// fonction bloc de terre de surface
void display_blocksurface(int x, int y, int l){
  tft.fillRect(x, y, l, 5, ORANGE);
  tft.fillRect(x, y+5, l, 15, RED);
}

// fonction boule de feu
void display_fire(int x, int y){
  y=y+8;
  tft.fillTriangle(x-8, y-5, x-8, y-14, x-2, y-9, RED); // point blanc
  tft.fillTriangle(x-7, y-11, x+2, y-8, x-1, y-18, RED); // point jaune
  tft.fillTriangle(x, y-8, x+4, y-9, x+4, y-22, RED); // point bleu f
  tft.fillTriangle(x+4, y-8, x+8, y-13, x+8, y-6, RED); // point bleu c
  tft.fillCircle(x, y, 9, RED);
  tft.fillCircle(x, y+3, 8, ORANGE);
  tft.fillCircle(x, y+4, 5, YELLOW); 
}

// fonction test boule de feu
void test(){
    display_blocksurface(0, 220, 320);
    display_blockterre(180, 220, 20, 20);
    display_blocksurface(180, 200, 20);

    display_fire(100, 180);
}

// fonction slime

void display_slime(int x,int y) {
    tft.fillRect(x+1, y+ 1, 2, 2, BLUE) ;
    tft.fillRect(x+2, y+ 2, 24, 2, BLUE) ;
    tft.fillRect(x, y, 2, 6, BLUE) ;
    tft.fillRect(x+1, y- 3, 2, 4, BLUE) ;
    tft.fillRect(x+2, y- 5, 2, 2, BLUE) ;
    tft.fillRect(x+3, y- 6, 2, 2, BLUE) ;
    tft.fillRect(x+4, y- 7, 4, 2, BLUE) ;
    tft.fillRect(x+6, y-8, 8, 2, BLUE) ;
    tft.fillRect(x+10, y- 7, 4, 2, BLUE) ;
    tft.fillRect(x+12, y- 6, 2, 2, BLUE) ;
    tft.fillRect(x+13, y- 5, 2, 2, BLUE) ;
    tft.fillRect(x+14, y- 4, 2, 4, BLUE) ;
    tft.fillRect(x+15, y-2, 2, 6, BLUE) ;
    tft.fillRect(x+14, y-3, 2, 2, BLUE) ;
}

// fonction carré refresh
void last_square(Entity &player) {
  tft.fillRect(player.prevx-20,player.prevy,38,38,BLACK);
}

// focntion affichage joueur
void display_player(Entity &player) {
  if(physicsUpdated) {
    last_square(player);
    display_heart(player.x,player.y);
  }
}

// fonction affichage overlay
void display_topMenu() {
  tft.fillRect(0, 0, SCREENWIDTH, 30, BLACK);
}

void display_hp(int x, int y, int backgroundColor) {
  
  tft.fillCircle(x+5, y+5, 6, backgroundColor);
  tft.fillCircle(x-5, y+5, 6, backgroundColor);
  tft.fillTriangle(x, y+18, x-11, y+5, x+11, y+5, backgroundColor);
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
  display_hp((SCREENWIDTH*3.3)/8, (SCREENHEIGHT*0.1)/8, GREEN);
  display_hp((SCREENWIDTH*4)/8, (SCREENHEIGHT*0.1)/8, RED);
  display_hp((SCREENWIDTH*4.7)/8, (SCREENHEIGHT*0.1)/8, GREEN);
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
  display_topMenu();
  display_life();
  display_time();
  display_score();
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

/*bool x_lim(Entity e) {
	if(e.direc == 'D' && e.x >= SCREENWIDTH -5) {
		return true;
	}
	if(e.direc == 'G' && e.y <= 5) {
		return true;
	}
	return false;
}*/

void update_physics(Entity &player) {
  Serial.println("physics updated");
  player.prevx = player.x;
  player.prevy = player.y;

  if(airborn) player.ay = -10;
  
  if(airborn && upPressed) player.ay = -8;
  else player.ay = -10;

  if(!airborn && upPressed) {
    airborn = true;
    player.speedy = 30;
	}

	if(downPressed) player.ay = -20;
  else player.ay = -10;

	if(rightPressed) player.ax = 10;
  else player.ax = 0;

	if(leftPressed) player.ax = 10;
  else player.ax = 0;

  player.speedx = player.speedx + player.ax;
  player.speedy = player.speedy + player.ay;
  player.x = player.x + player.speedx;
  player.y = player.y + player.speedy;

  if(upPressed || downPressed || leftPressed || rightPressed) {
    physicsUpdated = true;
  }
  else physicsUpdated = false;
}

//=======================================================================================

//init structures

struct Entity player;

//=======================================================================================

void setup() {

  //variable pour demmarage testscreen
  testScreen = true;

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

	//setup differents test
	if(testScreen == true) {
    tft.fillScreen(BLACK);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
		testScreen = false;
    airborn = true;
    display_star();
    //save_screen();
    setup_player(player);
    test();
    display_menu();
    display_player(player);
  }

  // Tant que start n'a pas été appuyé
  while (!finished) {
		  
		timePassed = millis() - timebuffer;
    //boucle pour répeter une action tout les x millisecondes
    if(timePassed > refresh) {  
      buttons_update();
      update_physics(player);
      //print_save(0,0,320,220);
      display_player(player);
      if(digitalRead(27)) {
        finished = true;
      }
		timebuffer = millis();
    } 
  }
  bool done = false;
  while(!testScreen) {
    if(!done) {
      done = true;
      tft.fillScreen(BLACK);
      tft.setCursor(50, 50);
      tft.setTextSize(8);
      tft.setTextColor(WHITE);
      tft.print("Game Over");
    }
    tft.print("Game Over");
    if(digitalRead(27)) {
      testScreen = true;
    }
  }
}