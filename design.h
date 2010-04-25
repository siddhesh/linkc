#ifndef __DESIGN_H
#define __DESIGN_H

#include<ncurses.h>

#define BORDER 1
#define NO_BORDER 0

#define SIZE(a)(sizeof(a)/sizeof(a[0]))

enum colors
{
blue_white=1,
white_blue,
black_white,
red_white,
white_black,
cyan_black,
black_cyan,
green_red,
red_black
};

//file menu items
static char *fmenu[]=
{
"New",
"Open       (F3)",
"Save       (F2)",
"Save As",
"Return To Shell",
"Exit"
 };
 
//compile menu items
static char *cmenu[]=
{
"Compile  F(9) ",
"Run      F(10)"
};

static char *hmenu[]=
{
"Help Index",
"Search Topic"
};


//fuction declarations
void initCurses(void);
WINDOW *createWin1(char *title,int h,int w,int sy,int sx,char *msg);
void Screen();
void horizMenu();
void statusLine();
void writeString(WINDOW *win,int sy,int sx,char *title,int color);
int getResponse();
WINDOW **draw_menu(char **menu,int count,int sr,int sc);
void new_file();
char *dialog(char title[10],char button1[10],char button2[10]);
extern char message(char msg[100],char msg1[100],char title[10],char button1[10],char button2[10]);
extern void warning(char title[0],char msg[100],char msg1[100]);
char *dialog1(char title[10],char button1[10],char button2[10]);
char *dialog2(char title[10],char button1[10],char button2[10]);

#endif
