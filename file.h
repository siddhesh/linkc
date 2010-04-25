#ifndef __FILE_H
#define __FILE_H

//function declaration
void New();
void Compile(char file_name[30]);
void Run(char file_name[30]);

//extern variables declaraion
extern int logc,logr,len;
extern char *help_topic,*fname;
extern char htopic[50],buff[5000],file[50];

#endif
