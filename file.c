#include<stdlib.h>
#include<string.h>
#include"file.h"
#include"design.h"

static int in;
static char files_history[5][30];
char *fname=NULL;

void New()
{
int logc,logr,len,ch,i,j,k;
char file[50]={'\0'};
char buff[5000]={'\0'},htopic[50]={'\0'};
char *help_topic=NULL;
FILE *fp;

logc=1,logr=2,len=0;
	while(1)
	{
		scrollok(stdscr,TRUE);
		wrefresh(stdscr);
		move(23,70);
		printw("|%-2d:%-2d|",logr-1,logc);
		move(logr,logc);
		attron(COLOR_PAIR(white_black));
		curs_set(1);
		noecho();
	 	ch=getResponse();
		refresh();
			
		switch(ch)
		{
			//to print characters on screen
			default:
				wrefresh(stdscr);
				attron(COLOR_PAIR(white_black));
				
				if(isascii(ch))
				{
					if(logc>78)
					{
						logr+=1;
						logc=1;
						move(logr,logc);
					}
					attron(COLOR_PAIR(white_black));
					insch(ch);
					mvaddch(logr,79,ACS_VLINE);	
					wrefresh(stdscr);
					move(logr,++logc);
					wrefresh(stdscr);
				}
			break;
		
			//backspace key
			case KEY_BACKSPACE:
			    if(logc>1)
				{
					move(logr, --logc);
					refresh();
					delch();
						for(j=logc;j<COLS-1;++j)
					 		mvaddch(logr,j,' ');
					mvaddch(logr,79,ACS_VLINE);
					move(logr,logc);
					refresh();
					mvprintw(23,70,"|%-2d:%-2d|",logr-1,logc);
					move(logr,logc);
				}
			break;
			
			
			//enter key
			case 10:
				if(logr<22)
				{
					wrefresh(stdscr);
					logc=1;
					mvaddch(logr,79,ACS_VLINE);
					move(++logr,logc);
					wrefresh(stdscr);
				}
			break;
			
			//tab key
			case 9:
				if(logc<78)
				{
					wrefresh(stdscr);
					logc+=4;
					move(logr,logc);
				}
				else
				{
					logr+=1;
					logc=1;
					move(logr,logc);
				}
			break;
			
			//left arrow key
			case KEY_LEFT:
				wrefresh(stdscr);
				if(logc>1)
					move(logr,--logc);
			break;
				
			//right arrow key
			case KEY_RIGHT:
				wrefresh(stdscr);
				if(logc<78)
					move(logr,++logc);
			break;
				
			//up arrow key
			case KEY_UP:	
				wrefresh(stdscr);
				if(logr>2)
					move(--logr,logc);
			break;
		
			//down arrow key
			case KEY_DOWN:
				wrefresh(stdscr);
				if(logr<22)
					move(++logr,logc);
			break;
			
			
			//-------| Activates Mouse Menu |------
			//file menu
			case 1:
				(WINDOW**)draw_menu(fmenu,SIZE(fmenu),1,0);
			break;
		
			//new item
			case 2:
				clear();
				refresh();
				new_file();
				noecho();
				New();
				strcpy(fname,"UNSAVED");						
			break;
			
			//save menu
			case KEY_F(2):
			case 4:
				if (fname==NULL)
				{
					
					//saves contents to the buffer
				k=0;
				for(i=2;i<=logr/*22*/;++i)
				{
					for(j=0;j<78;++j)
						buff[k++]=(mvinch(i,j)&A_CHARTEXT);
					buff[k++]='\n';
				}
				
				//display the screen contents
				new_file();
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
			
					//pops up save dialogbox				
				fname=(char*)calloc(30,sizeof(char));
				//if(strcmp(fname,"UNSAVED")==0)
				fname=dialog("| Save |"," Save "," Cancel ");
				/*else
				{
					printw("Hello \n");
					getch();
					endwin();
					exit(0);
				}*/
					//redraws new screen
				new_file();
					
					//if user press esc(i.e cancel button)
					//erase dialog box and display file contents
				if((strlen(fname)==0))
				{
					new_file();
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
					goto end;
				}
				
				//if file is not exist
				
				
				//if(!(fp=fopen(fname,"r")))
				{
					
					if(!(fp=fopen(fname,"w")))
					{
						clear();
						printw("File Creation Error !!!");
						getch();
						endwin();
						exit(EXIT_FAILURE);
					}
				}
				
				//file:
					//write the screen contents to the file
				i=0;
					while(i<(strlen(buff)-1))
					{
						if(buff[i]=='\n'||logc==78)
						{
							logr+=1;
							logc=1;
						}
						putc(buff[i],fp);
						i++;
						logc++;		
					}
					fclose(fp);
					// write the filename instade of unsaved
				strcat(file,"| ");
				strcat(file,fname);
				strcat(file," |");
				move(1,35);
				hline(ACS_HLINE,12);
				writeString(stdscr,1,35,file,white_black);
				
						//display the contents of the file on screen
					fp=fopen(fname,"r");
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
					
					if(in>5)
						in=0;
					else
					{
						strcpy(files_history[in],fname);
						in++;
					}
					// ---| END LABEL |---
					//end:
					//getch();
				}
				else
				{
					k=0;
					for(i=2;i<=logr/*22*/;++i)
					{
					for(j=0;j<78;++j)
						buff[k++]=(mvinch(i,j)&A_CHARTEXT);
					buff[k++]='\n';
					}
					
					//display the screen contents
					new_file();
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
					{
					
					if(!(fp=fopen(fname,"w")))
					{
						clear();
						printw("File Creation Error !!!");
						getch();
						endwin();
						exit(EXIT_FAILURE);
					}
				}
				
				//file:
					//write the screen contents to the file
				i=0;
					while(i<(strlen(buff)-1))
					{
						if(buff[i]=='\n'||logc==78)
						{
							logr+=1;
							logc=1;
						}
						putc(buff[i],fp);
						i++;
						logc++;		
					}
					fclose(fp);
					// write the filename instade of unsaved
				strcat(file,"| ");
				strcat(file,fname);
				strcat(file," |");
				move(1,35);
				hline(ACS_HLINE,12);
				writeString(stdscr,1,35,file,white_black);
				
						//display the contents of the file on screen
					fp=fopen(fname,"r");
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
					
					if(in>5)
						in=0;
					else
					{
						strcpy(files_history[in],fname);
						in++;
					}
				
				}
				//fclose(fp);
				end:
			break;
			
			//open file
			case KEY_F(3):
			case 3:
						k=0;
					for(i=2;i<=logr/*22*/;++i)
					{
						for(j=0;j<78;++j)
							buff[k++]=(mvinch(i,j)&A_CHARTEXT);
						buff[k++]='\n';
					}
				new_file();
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
			
	
					//pops up open dialogbox				
				fname=(char*)calloc(30,sizeof(char));
				fname=dialog("| Open |"," Open "," Cancel ");
				
				//if user press cancel display screen contents
				if((strlen(fname)==0))
				{
					new_file();
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
						new_file();
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
						move(1,2);
						hline(ACS_HLINE,75);
						refresh();
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
						
						move(1,2);
						hline(ACS_HLINE,75);
						//displays file name
							//move(1,2);
						//hline(ACS_HLINE,75);
						strcat(file,"| ");
						strcat(file,fname);
						strcat(file," |");
						//move(1,2);
						//hline(ACS_HLINE,60);
						writeString(stdscr,1,35,file,white_black);
					}
					
				}
				if(in>5)
						in=0;
					else
					{
						strcpy(files_history[in],fname);
						in++;
					}
			break;
			
			//return to shell
			case 5:
				clear();
	       		refresh();
	       		endwin();
	       		system("sh");
				refresh();
				Screen();
				refresh();
			break;
			
			//exit item
			case 6:
				clear();
	    		refresh();
	    		//getch();
	    		/*for(i=0;i<5;++i)
	    		printw("%s\n",files_history[i]);
	    		printw("Press a key ...\n");
	    		getch();*/
				endwin();
				exit(EXIT_SUCCESS);
			break;
			
			//compile menu
			case 7:
				//pop up menu compile
					(WINDOW**)draw_menu(cmenu,SIZE(cmenu),1,12);
			break;
			
			
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
			
			case KEY_F(7):
				Search();
			break;	
			
			//help topic
			case 13:
					k=0;
					for(i=2;i<22;++i)
					{
						for(j=0;j<78;++j)
							buff[k++]=(mvinch(i,j)&A_CHARTEXT);
						buff[k++]='\n';
					}
					new_file();
					help_topic=(char*)calloc(30,sizeof(char));
					help_topic=dialog("| Help |"," Find "," Cancel ");
					
					
					//if user press cancel display screen contents
					if((strlen(help_topic)==0))
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
						clear();
						strcpy(htopic,"Help/");
						/* strcat(htopic,"  ");*/
						strcat(htopic,help_topic);
						strcat(htopic,".hlp");
						//redraws new screen
						new_file();
				
						if(!(fp=fopen(htopic,"r")))
						{
							warning("| Warning |","	No such help topic ","Press a key to return !!!");
							getch();
							new_file();
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
						else
						{	
							
							Help_topic(htopic);
						
						}
						
					}
			break;
			
			case KEY_F(9):
					Compile(fname);
			break;
			
			case KEY_F(10):
					Run(fname);
			break;		
		}
	}
}

void Compile(char file_name[30])
{
char cmd[80]={'\0'},ext[30]={'\0'},err[30]={'\0'},file[40]={'\0'};
int i,ch,logr,logc;
FILE *fp;

	//file name without extention
	i=0;
	while(file_name[i]!='.')
	{
	  ext[i]=file_name[i];
	  i++;	
	}
	//if file is alredy exist
	if((fp=fopen(ext,"r")))
	{
		remove(ext);
	}
	//compile command
	strcat(cmd,"gcc ");
	strcat(cmd,file_name);
	strcat(cmd," -o ");
	strcat(cmd,ext);
	strcat(cmd," &> ERR/");
	strcat(cmd,ext);
	strcat(cmd,".err");
	strcat(cmd," -lm");
	system(cmd);	
	
	//file is without error
	if(fp=fopen(ext,"r"))
	{
		warning("| ERROR |","	NO ERROR !!! ","Press a key to return !!!");
		getch();
	}
	
	//if file has errors display it.
	else
	{
		strcpy(err,"ERR/");
		strcat(err,ext);
		strcat(err,".err");
		
		//display errors
		fp=fopen(err,"r");
		
		attron(COLOR_PAIR(white_black));
		createWin(10,80,13,0,black_cyan,BORDER);
		logc=1;
		logr=14;
		move(logr,logc);
		while((ch=fgetc(fp))!=EOF)
		{
			if(ch=='\n'||logc==76)
			{
    			logc=1;
    			move(logr+1,logc);
			}
			else 
			{
				if(isprint(ch))
				{
    			printw("%c",ch);
    			logc+=1;
    			}
			}
		}//while
		fclose(fp);
		getch();
	}//else
	
	
new_file();
//display the file contents 
fp=fopen(file_name,"r");
logc=1;logr=2;
		
move(1,2);
hline(ACS_HLINE,75);
strcat(file,"| ");
strcat(file,file_name);
strcat(file," |");
writeString(stdscr,1,35,file,white_black);

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
}


void Run(char file_name[30])
{
int i,ch,logr,logc;
char ext[30]={'\0'},r_name[30]={'\0'},file[40]={'\0'};
FILE *fp;	
	Compile(file_name);
	i=0;
	while(file_name[i]!='.')
	{
	  ext[i]=file_name[i];
	  i++;	
	}
	//if file is alredy exist
	if((fp=fopen(ext,"r")))
	{
		clear();
		refresh();
		endwin();
		strcat(r_name,"./");
		strcat(r_name,ext);
		system(r_name);
	}
	else
	{
		warning("| ERROR |","Compile Before Run ","Press a key to return !!!");
	}
getch();
initCurses();
new_file();
//display the file contents 
fp=fopen(file_name,"r");
logc=1;logr=2;
		
move(1,2);
hline(ACS_HLINE,75);
strcat(file,"| ");
strcat(file,file_name);
strcat(file," |");
writeString(stdscr,1,35,file,white_black);

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
}


Search()
{
char *search=NULL,*search1=NULL,*fname1=NULL;
int logc,logr,len,ch,i,j,k;
char file[50]={'\0'};
FILE *fp;

if(fname==NULL)
{
warning("| ERROR |","OPEN A FILE !!! ","Press a key to return !!!");
getch();
}
//printw("%s\n",fname);
else
{
	fname1=(char*)malloc(strlen(fname)+1);
	strcpy(fname1,fname);
	//printw("File  %s\n",fname1);
	refresh();
	search=dialog1("| Search |"," Search "," Cancel ");
	//printw("%s",search);
	search1=(char*)malloc(strlen(search)+1);
	strcpy(search1,search);
	char *replace=NULL;
	replace=dialog2("| Replace |"," Replace "," Cancel ");
	//printw("%s\n",replace);
	//printw("Search %s",search1);
	//printw("File %s\n",fname1);
	refresh();

	char str[100]={'\0'};
	strcpy(str,"sed -i 's/\\<");
	strcat(str,search1);
	strcat(str,"\\>/");
	strcat(str,replace);
	strcat(str,"/g' ");
	strcat(str,fname1);
	//strcat(str,"/g' hello.c");
	//printw("%s",str);
	system(str);
	
	fp=fopen(fname1,"r");
	fclose(fp);
	refresh();
	
	if(!(fp=fopen(fname1,"r")))
	{
	warning("| ERROR |","FILE OPENING ERROR !!! ","Press a key to return !!!");
	getch();

	}
	else
	{	
						move(1,2);
						hline(ACS_HLINE,75);
						refresh();
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
						
						move(1,2);
						hline(ACS_HLINE,75);
						//displays file name
							//move(1,2);
						//hline(ACS_HLINE,75);
						strcat(file,"| ");
						strcat(file,fname1);
						strcat(file," |");
						//move(1,2);
						//hline(ACS_HLINE,60);
						writeString(stdscr,1,35,file,white_black);
						move(2,2);
						refresh();
					}


}	
getch();
//endwin();
//exit(0);
}

