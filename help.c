#include"help.h"
#include"design.h"
#include <menu.h>
#include <stdlib.h>
#include <string.h>


//prints a help index
void Help()
{
FILE *fp;
char ch[25]={'\0'},path[50]={'\0'},p;
ITEM **my_items;
int c,n_choices, i;
MENU *my_menu;
WINDOW *my_menu_win,*help;
      
      
        up:
        new_file();
        //create items
        n_choices = SIZE(help_index);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                 my_items[i] = new_item(help_index[i],"");
                 //my_items[i] = new_item(choices[i],"");
        
        // Crate menu 
        my_menu = new_menu((ITEM **)my_items);
        
        //Create the window to be associated with the menu 
        my_menu_win = newwin(15, 25,4, 25);
      
        keypad(my_menu_win, TRUE);
        
        // Set main window and sub window 
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win,10, 20, 3, 1));
        set_menu_format(my_menu,10, 1);
        
        // Set menu mark to the string " -> " 
        set_menu_mark(my_menu, " -> ");
        
        // Print a border around the main window and print a title 
        box(my_menu_win, 0, 0);
        print_in_middle(my_menu_win, 1, 0, 25, "Link C Help", COLOR_PAIR(red_black));
        mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
        mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
        mvwaddch(my_menu_win, 2, 24, ACS_RTEE);
		
		//colors for menu's
		set_menu_fore(my_menu, COLOR_PAIR(green_red) | A_REVERSE);
		set_menu_back(my_menu, COLOR_PAIR(black_cyan));
		set_menu_grey(my_menu, COLOR_PAIR(green_red));

        
        // Post the menu 
        post_menu(my_menu);
        wrefresh(my_menu_win);
        attron(COLOR_PAIR(black_cyan));
       
       
        attroff(COLOR_PAIR(black_cyan));
        refresh();
        while((c = wgetch(my_menu_win)) !=ESC)
        {       switch(c)
                {       case KEY_DOWN:
                                 menu_driver(my_menu, REQ_DOWN_ITEM);
                                 break;
                        case KEY_UP:
                                 menu_driver(my_menu, REQ_UP_ITEM);
                                 break;
                        case KEY_NPAGE:
                                 menu_driver(my_menu, REQ_SCR_DPAGE);
                                 break;
                        case KEY_PPAGE:
                                 menu_driver(my_menu, REQ_SCR_UPAGE);
                                 break;
						//Enter key
                        case 10: 
        						new_file();
        						move(20, 0);
        						clrtoeol();
        						clear();
        						
        						//concatinates file ptah(Help/file.hlp)
        						strcpy(ch,item_name(current_item(my_menu)));
        						strcat(path,"Help/");
        						strcat(path,ch);
        						strcat(path,".hlp");
        						fp=fopen(path,"r");
        						if(fp==NULL)
        						{
        							printw("Help Doesn't Exist !!!");
        							getch();
        							endwin();
        							exit(EXIT_FAILURE);
        						}
        						clear();
        						
        						help=newwin(22,70,2,5);
        						wbkgd(help,COLOR_PAIR(cyan_black));
        						//prints a file
        						while((p=fgetc(fp))!=EOF)
        						{
        							waddch(help,p);
        							refresh();
        						}
        						box(help,0,0);
        						wrefresh(help);
        						getch();
        						ch[0]='\0';
        						path[0]='\0';			    
        						goto up;
        				break;

                }
                wrefresh(my_menu_win);
        }
        // Unpost and free all the memory taken up
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
        new_file();
}

//prints menus in middle of the screen
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{       int length, x, y;
        float temp;
        if(win == NULL)
                win = stdscr;
        getyx(win, y, x);
        if(startx != 0)
                x = startx;
        if(starty != 0)
                y = starty;
        if(width == 0)
                width = 80;
        length = strlen(string);
        temp = (width-length)/ 2;
        x = startx + (int)temp;
        wattron(win, color);
        mvwprintw(win, y, x, "%s", string);
        wattroff(win, color);
        refresh();
}


//disp;ays a specific file
void Help_topic(char *name)
{
FILE *fp;
char p;
WINDOW *help;
      
new_file();
move(20, 0);
clrtoeol();
clear();
       
 if(!(fp=fopen(name,"r")))
  {
   	printw("Help Doesn't Exist !!!");
   	getch();
   	endwin();
   	exit(EXIT_FAILURE);
  }
  clear();
      						
   help=newwin(22,70,2,5);
   wbkgd(help,COLOR_PAIR(cyan_black));
   //prints a file
  
     while((p=fgetc(fp))!=EOF)
     {
     	waddch(help,p);
      	refresh();
     }
  box(help,0,0);
  wrefresh(help);
  getch();
  new_file();        				
}     						
        						


