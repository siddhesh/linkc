#include"design.h"
#include<stdlib.h>
#include<string.h>

#define CTRL(c) ((c) & 037)

void initCurses(void)
{
initscr();
	if(has_colors()==FALSE)
	{
		printw("Your terminal Doesn't Support Colors !!!\n");
		endwin();
		exit(EXIT_FAILURE);
	}
start_color();

//initialises color pairs 
init_pair(white_blue,COLOR_WHITE,COLOR_BLUE);
init_pair(blue_white,COLOR_BLUE,COLOR_WHITE);
init_pair(black_white,COLOR_BLACK,COLOR_WHITE);
init_pair(red_white,COLOR_RED,COLOR_WHITE);
init_pair(white_black,COLOR_WHITE,COLOR_BLACK);
init_pair(cyan_black,COLOR_BLACK,COLOR_CYAN);
init_pair(black_cyan,COLOR_CYAN,COLOR_BLACK);
init_pair(green_red,COLOR_GREEN,COLOR_RED);
init_pair(red_black, COLOR_RED, COLOR_BLACK);
}

WINDOW *createWin(int h,int w,int sy,int sx,int color,int flag)
{
WINDOW *local;
refresh();
// creates logical window of specified size 
local=newwin(h,w,sy,sx); 
wbkgd(local,COLOR_PAIR(color));

// draws border to window 
	if(flag==BORDER)
		box(local,0,0); 
wrefresh(local);
return local;
}

WINDOW **draw_menu(char **menu,int count,int sr,int sc)
{
int er,i,ec,l,len=0,area;
WINDOW **items;

//calculate ending row for menu
er=sr+count+2;

	//find longest menu name
	for(i=0;i<count;++i)
	{
	 l=strlen(menu[i]);
	 if(l>len)
	 len=l;
	}
//calculate ending column for menu
ec=sc+len+4;

items=(WINDOW**)malloc(count*sizeof(WINDOW*));
items[0]=newwin(er-sr,ec-sc,sr,sc);
wbkgd(items[0],COLOR_PAIR(black_white));
box(items[0],0,0);

	//draws subwindow
	for(i=0;i<count;++i)
	{
		items[i+1]=subwin(items[0],1,ec-sc,sr+=1,sc+=2);
		wprintw(items[i+1],"%s",menu[i]);		
		wrefresh(items[i+1]);
		sc-=2;
	}

wrefresh(items[0]);
return items;
}

//first dotted screen
void Screen()
{
WINDOW *local;
clear();
createWin(1,80,0,0,blue_white,NO_BORDER);
createWin(1,80,24,0,blue_white,NO_BORDER);
local=newwin(23,80,1,0); 
wbkgd(local,COLOR_PAIR(blue_white)|ACS_BOARD);
wrefresh(local);
//draws 2 white bars
horizMenu();
statusLine();
}

void horizMenu()
{
writeString(stdscr,0,2,"^File",black_white);
writeString(stdscr,0,12,"^Compile",black_white);
writeString(stdscr,0,27,"^Help",black_white);
}

//write summary on bottom bar
void statusLine()
{
writeString(stdscr,24,5,"^F^2",black_white);
writeString(stdscr,24,8,"Save",black_white);
writeString(stdscr,24,15,"^F^9",black_white);
writeString(stdscr,24,18,"Compile",black_white);
writeString(stdscr,24,28,"^F^1^0",black_white);
writeString(stdscr,24,32,"Run",black_white);
}

//prints string
void writeString(WINDOW *win,int sy,int sx,char *title,int color)
{
	while(*title!='\0')
	{
		refresh();
		if(*title=='^')
		{
			title++;
			wattron(win,COLOR_PAIR(red_white));
			mvwprintw(win,sy,sx,"%c",*title);
			wattroff(win,COLOR_PAIR(color));
		}
		else
		{
			wattron(win,COLOR_PAIR(color));
			mvwprintw(win,sy,sx,"%c",*title);
			wattroff(win,COLOR_PAIR(color));
		}
		title++;
		sx++;
		wrefresh(win);
	}
}

//receives response from keybord or mouse
int getResponse()
{
int ch;
MEVENT m;
keypad(stdscr,TRUE);
noecho();
mousemask(ALL_MOUSE_EVENTS,NULL);
wrefresh(stdscr);
		ch=getch();
		if(ch==KEY_MOUSE)
		{       
			
			refresh();
			getmouse(&m);
			
			// ---| file menu's co-ordinates |--
					//file menu
			if(m.y==0 && m.x>=2 && m.x<=5)
				return 1;
					
					//new item
			if(m.y==2 && m.x>=2 && m.x<=11)
				return 2;
					
					//open item
			if(m.y==3 && m.x>=2 && m.x<=11)
				return 3;
				
					//save item
			if(m.y==4 && m.x>=2 && m.x<=11)
				return 4;
			
					//return to shell
			if(m.y==6 && m.x>=2 && m.x<=11)
				return 5;
					//exit item
			if(m.y==7 && m.x>=2 && m.x<=11)
				return 6;
		
			
				
				// ---| compile menu's co-ordinates |---
			if(m.y==0 && m.x>=12 && m.x<=21)
				return 7;
				//compile item
			if(m.y==2 && m.x>=12 && m.x<=21)
				return 8;
			
			
			
			// ---| help menu's co-ordinates |---
				//help index
			if(m.y==0 && m.x>=27 && m.y<=31)
				return 11;
				//help index
			if(m.y==2 && m.x>=27 && m.y<=31)
				return 12;
				//help topic
			if(m.y==3 && m.x>=27 && m.y<=31)
				return 13;
			return -1;
		}
	
		else
		{
			//ctrl+f key
			if(ch==CTRL('F'))
				return 1;
			//return -1;
		}		
}

void new_file()
{
createWin(1,80,0,0,blue_white,NO_BORDER);
createWin(1,80,24,0,blue_white,NO_BORDER);
createWin(23,80,1,0,white_black,BORDER);
writeString(stdscr,1,35,"| UNSAVED |",white_black);
horizMenu();
statusLine();
}

//displays dilog box
char *dialog(char title[10],char button1[10],char button2[10])
{
static char str[30];
int i,ch,r,c;
createWin(10,40,7,20,cyan_black,BORDER);
writeString(stdscr,7,35,title,cyan_black);
writeString(stdscr,9,22,"File Name",cyan_black);

r=10,c=22;
createWin(1,35,r,c,white_black,NO_BORDER);

	//save Button	
createWin(3,10,12,26,blue_white,BORDER);
writeString(stdscr,13,28,button1,white_blue);
	//cancel Button	
createWin(3,10,12,43,blue_white,BORDER);
writeString(stdscr,13,44,button2,white_blue);
curs_set(1);
echo();
move(10,23);
refresh();
	for(i=0;i<30;++i)
	{
		ch=getch();
		switch(ch)
		{
			//yes button(Enter)
			case 27:
				createWin(1,35,10,22,white_blue,NO_BORDER);
				move(10,23);
				str[0]='\0';
			return str;
			
			//no button(ESC)
			case 10:
				str[i]='\0';
			return str;

			
			default:
				if(isgraph(ch))
				{
					str[i]=ch;
					r,c+=1;
				}
			break;
			
		}		
	}	
}

//diplays message on screen
char message(char msg[100],char msg1[100],char title[10],char button1[10],char button2[10])
{
int ch;
createWin(10,40,7,20,cyan_black,BORDER);
writeString(stdscr,7,35,title,cyan_black);
writeString(stdscr,9,22,msg,cyan_black);
writeString(stdscr,10,22,msg1,cyan_black);


	//yes Button	
createWin(3,10,12,26,blue_white,BORDER);
writeString(stdscr,13,28,button1,white_blue);
	//cancel Button	
createWin(3,10,12,43,blue_white,BORDER);
writeString(stdscr,13,46,button2,white_blue);
curs_set(0);
noecho();
refresh();
	while(1)
	{
		ch=getch();
		//yes key
		if(ch==10)
			return 'Y';
		//cancel key
		if(ch==27)
			return 'N';
	}
}

//displays warning message
void warning(char title[0],char msg[100],char msg1[100])
{
createWin(7,40,7,20,cyan_black,BORDER);
writeString(stdscr,7,35,title,cyan_black);
writeString(stdscr,9,32,msg,cyan_black);
attron(A_BLINK);
writeString(stdscr,11,27,msg1,cyan_black);
attroff(A_BLINK);
curs_set(0);
noecho();
refresh();	
}


char *dialog1(char title[10],char button1[10],char button2[10])
{
static char str[30];
int i,ch,r,c;
createWin(10,40,7,20,cyan_black,BORDER);
writeString(stdscr,7,35,title,cyan_black);
writeString(stdscr,9,22,"Enter text to search ",cyan_black);

r=10,c=22;
createWin(1,35,r,c,white_black,NO_BORDER);

	//search Button	
createWin(3,10,12,28,blue_white,BORDER);
writeString(stdscr,13,29,button1,white_blue);
	//cancel Button	
createWin(3,10,12,43,blue_white,BORDER);
writeString(stdscr,13,44,button2,white_blue);
curs_set(1);
echo();
move(10,23);
refresh();
	for(i=0;i<30;++i)
	{
		ch=getch();
		switch(ch)
		{
			//yes button(Enter)
			case 27:
				createWin(1,35,10,22,white_blue,NO_BORDER);
				move(10,23);
				str[0]='\0';
			return str;
			
			//no button(ESC)
			case 10:
				str[i]='\0';
			return str;

			
			default:
				if(isgraph(ch))
				{
					str[i]=ch;
					r,c+=1;
				}
			break;
			
		}		
	}	
}

char *dialog2(char title[10],char button1[10],char button2[10])
{
static char str[30];
int i,ch,r,c;
createWin(10,40,7,20,cyan_black,BORDER);
writeString(stdscr,7,35,title,cyan_black);
writeString(stdscr,9,22,"Enter text to Replace ",cyan_black);

r=10,c=22;
createWin(1,35,r,c,white_black,NO_BORDER);

	//replace Button	
createWin(3,11,12,28,blue_white,BORDER);
writeString(stdscr,13,29,button1,white_blue);
	//cancel Button	
createWin(3,10,12,43,blue_white,BORDER);
writeString(stdscr,13,44,button2,white_blue);
curs_set(1);
echo();
move(10,23);
refresh();
	for(i=0;i<30;++i)
	{
		ch=getch();
		switch(ch)
		{
			//yes button(Enter)
			case 27:
				createWin(1,35,10,22,white_blue,NO_BORDER);
				move(10,23);
				str[0]='\0';
			return str;
			
			//no button(ESC)
			case 10:
				str[i]='\0';
			return str;

			
			default:
				if(isgraph(ch))
				{
					str[i]=ch;
					r,c+=1;
				}
			break;
			
		}		
	}	
}

