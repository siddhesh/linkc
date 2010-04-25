#include<stdlib.h>
#include<string.h>
#include"design.h"
#include"file.h"

int main(void)
{
//extern variables
char *help_topic=NULL,*fname=NULL,buff[5000]={'\0'},file[50]={};
int logc,logr,ch,i,j,k,flag;
FILE *fp;

// initialise curses mode and colors
initCurses();
noecho();
cbreak();
keypad(stdscr,TRUE);
mousemask(ALL_MOUSE_EVENTS,NULL);
curs_set(0);

//displays starting blue screen
Screen();
	while(1)
	{
		int ch;
		ch=getResponse();
		switch(ch)
		{
			//file menu
			case 1:
					//pop up menu file
					(WINDOW**)draw_menu(fmenu,SIZE(fmenu),1,0);
			break;
			
			//new item
			case 2:
				refresh();
				//draws black screen
				new_file();
				noecho();
				//displays filename and line number on black screen
				New();						
			break;
			
			//Return To Shell
			case 5:
				clear();
	       		refresh();
	       		endwin();
	       		system("sh");
	       		clear();
				refresh();
				Screen();
			break;
			
			//exit menu
			case 6:
				clear();
	    		refresh();
				endwin();
				exit(EXIT_SUCCESS);
			break;
			
			/*compile menu
			case 7:
				//pop up menu compile
					(WINDOW**)draw_menu(cmenu,SIZE(cmenu),1,12);
			break;
			
			*/
			/*compile item
			case 8:
				compile();
			break;*/
			
			//help menu
			case 11:
				//pop up  help menu
 					(WINDOW**)draw_menu(hmenu,SIZE(hmenu),1,27);
			break;
			
			//help index
			case KEY_F(1):
			case 12:
				Help();
			break;
			
			//help topic
			case 13:
				help_topic=(char*)calloc(30,sizeof(char));
				help_topic=dialog("| Help |"," Find "," Cancel ");
			break;
			
			
			
			//open file
			case KEY_F(3):
			case 3:
						k=0;
					for(i=2;i<22;++i)
					{
						for(j=0;j<78;++j)
							buff[k++]=(mvinch(i,j)&A_CHARTEXT);
						buff[k++]='\n';
					}
				new_file();
					//pops up open dialogbox				
				fname=(char*)calloc(30,sizeof(char));
				fname=dialog("| Open |"," Open "," Cancel ");
				
				//if user press cancel display screen contents
				if((strlen(fname)==0))
				{
					logc=1;logr=2;
					for(k=0;buff[k]!='\0';++k)
					{
						refresh();
						if(buff[k]=='\n')
						{
					        logr+=1;
					        logc=1;
					       	move(logr+1,logc);
						}
						else
						{
							mvaddch(logr,logc,buff[k]);
							logc+=1;
						}
					}
				}
				
				//othewise open a file and display it's contents
				else
				{
					//redraws new screen
					new_file();
				
				
					if(!(fp=fopen(fname,"r")))
					{
						warning("| Warning |","	No such file ","Press a key to return !!!");
						getch();
						//new_file();
							logc=1;logr=2;
							for(k=0;buff[k]!='\0';++k)
							{
								refresh();
								if(buff[k]=='\n')
								{
					        		logr+=1;
					        		logc=1;
					       			 move(logr+1,logc);
								}
								else
								{
									mvaddch(logr,logc,buff[k]);
									logc+=1;
								}
							}	
					
					}
					//if file is exist display contents 
					else
					{	
						new_file();
						logc=1;logr=2;
						while((ch=fgetc(fp))!=EOF)
						{
							if(ch=='\n')
							{
						        logr+=1;
						        logc=1;
						        move(logr+1,logc);
							}
							else
							{
								mvaddch(logr,logc,ch);
								logc+=1;
							}
						}
						fclose(fp);
						move(2,2);
						refresh();
						
						//displays file name
							move(1,2);
						hline(ACS_HLINE,75);
						strcat(file,"| ");
						strcat(file,fname);
						strcat(file," |");
						//move(1,2);
						//hline(ACS_HLINE,60);
						writeString(stdscr,1,35,file,white_black);
					}
				}
			break;
			
			
		}
	}
}
