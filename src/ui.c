/*
 * ui.c
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui.h"

void _init_screen(void)
{
	_clear_screen();
	printf("Welcome to %s v%s!\n",_PROJECT_NAME,_PROJECT_VERSION);
}
int _lesson_selection(FILE *cr, bool clear)
{
	start:
		if(clear)
			_clear_screen();
		printf("Choose a lesson:\n");
		int i, lesson_id, lessons = _lesson_count(cr);
		for(i=1; i<=lessons; i++)
			printf("> Lesson %d\n",i);
		printf("\nNumber: ");
		scanf("%d",&lesson_id);
		if((lesson_id < 1) || (lesson_id > lessons))
			goto start;
		return lesson_id;
}
int _level_selection(FILE *cr, int lesson_id)
{
	start:
		_clear_screen();
		printf("Choose a level:\n");
		int i, level_id, levels = _lesson_level_count(cr,lesson_id);
		for(i=1; i<=levels; i++)
			printf("> Level %d\n",i);
		printf("\nNumber: ");
		scanf("%d",&level_id);
		if((level_id < 1) || (level_id > levels))
			goto start;
		return level_id;
}
char *_play_level(FILE *cr, int lesson_id, int level_id)
{
	#ifdef _WIN32
	char *winver = _get_win_release();
	bool legacy=false;
	if(strcmp(winver,"10") != 0)
		legacy=true;
	if(legacy)
	{
		// For Windows 8.1 and below
		fflush(stdout);
		_setmode(_fileno(stdout),_O_U8TEXT);
	}
	#endif
	char *level = _lesson_level_text(cr,lesson_id,level_id);
	#ifdef _WIN32
	wchar_t *wlevel;
	if(legacy)
	{
		wlevel = str_to_wcs(level);
	}
	#endif
	int pos=0, errors=0, hits=0, i;
	float final_time;
	char c;
	#ifdef _WIN32
	wchar_t wc;
	#endif
	char *menu_ret = (char*) malloc(18);
	char *tmp = (char*) malloc(17);
	struct timeval stop, start;
	#ifdef _WIN32
	if(legacy)
		wprintf(L"%ls\n",wlevel);
	else
		printf("%s\n",level);
	#else
	printf("%s\n",level);
	#endif
	bool tmpbool;
	long len;
	#ifdef _WIN32
	if(legacy)
		len=wcslen(wlevel);
	else
		len=strlen(level);
	#else
	len=strlen(level);
	#endif
	while(pos < len)
	{
		#ifdef _WIN32
		if(legacy)
		{
			wc=_getwch();
		}
		else
			c=getch();
		#else
		c=getch();
		#endif
		if(pos == 0)
			gettimeofday(&start, NULL);
		#ifdef _WIN32
		if(legacy)
		{
			tmpbool=(wc == '*');
		}
		else
		{
			tmpbool=(c == '*');
		}
		#else
		tmpbool=(c == '*');
		#endif
		if(tmpbool)
			goto escape;
		#ifdef _WIN32
		else if((!legacy && c == 224) || (legacy && (wc == 224)))
		{
			// Arrow key (used to escape from the level)
			if(legacy)
			{
				wc=_getwch();
				if(wc == 'H')
					goto escape;
				wc='\n';
			}
			else
			{
				c=getch();
				if(c == 'H')
					goto escape;
				c='\n';
			}
		}
		#else
		else if(c == '\033')
		{
			// Arrow key (used to escape from the level)
			getch();
			if(getch() == 'A') // up arrow
				goto escape;
		}
		#endif
		else
		{
			#ifdef _WIN32
			if(legacy)
			{
				tmpbool=(wc == wlevel[pos]);
			}
			else
			{
				tmpbool=(c == level[pos]);
			}
			#else
			tmpbool=(c == level[pos]);
			#endif
			if(tmpbool)
			{
				#ifdef _WIN32
				if(legacy)
					wprintf(L"%lc",wc);
				else
					putchar(c);
				#else
				putchar(c);
				#endif
			}
			else
			{
				#ifdef _WIN32
				if(legacy)
					tmpbool=(wc != '\n');
				else
					tmpbool=(c != '\n');
				#else
				tmpbool=(c != '\n');
				#endif
				if(tmpbool)
					errors++;
				#ifdef _WIN32
				while((!legacy && c != level[pos]) || (legacy && (wc != wlevel[pos])))
				#else
				while(c != level[pos])
				#endif
				{
					#ifdef _WIN32
					if(legacy)
					{
						wc=_getwch();
						tmpbool=(wc == '*');
					}
					else
					{
						c=getch();
						tmpbool=(c == '*');
					}
					#else
					c=getch();
					tmpbool=(c == '*');
					#endif
					if(tmpbool)
						goto escape;
					#ifdef _WIN32
					else if((!legacy && c == 224) || (legacy && (wc == 224)))
					{
						// Arrow key (used to escape from the level)
						if(legacy)
						{
							wc=_getwch();
							if(wc == 'H')
								goto escape;
							wc='\n';
						}
						else
						{
							c=getch();
							if(c == 'H')
								goto escape;
							c='\n';
						}
					}
					#else
					else if(c == '\033')
					{
						// Arrow key (used to escape from the level)
						getch();
						if(getch() == 'A') // up arrow
							goto escape;
						c='\0';
					}
					#endif
					else
					{
						#ifdef _WIN32
						if(legacy)
							tmpbool=(wc != wlevel[pos]);
						else
							tmpbool=(c != level[pos]);
						#else
						tmpbool=(c != level[pos]);
						#endif
						if(tmpbool)
							errors++;
					}
				}
				#ifdef _WIN32
				if(legacy)
					wprintf(L"%lc",wc);
				else
					putchar(c);
				#else
				putchar(c);
				#endif
			}
			hits++;
			pos++;
		}
	}
	gettimeofday(&stop, NULL);
	#ifdef _WIN32
	if(legacy)
	{
		fflush(stdout);
		_setmode(_fileno(stdout),_O_TEXT);
	}
	#endif
	putchar('\n');
	printf("\nMistakes: %d\n",errors);
	final_time = stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec)/1000000.0;
	printf("Hits per minute: %.0f\n",(hits * (60/(final_time))));
	if(errors == 0)
		printf("Perfect!\n");
	return "";
	
	escape:
		#ifdef _WIN32
		if(legacy)
		{
			fflush(stdout);
			_setmode(_fileno(stdout),_O_TEXT);
		}
		#endif
		strcpy(menu_ret,_menu(cr,lesson_id));
		switch(menu_ret[0]) {
			case '1':
				return "";
				break;
			case '2':
				strcpy(tmp,"");
				for(i=1; i < strlen(menu_ret); i++)
					strncat(tmp,&menu_ret[i],1);
				sprintf(menu_ret,"%d.%s",lesson_id,tmp);
				return menu_ret;
				break;
			case '3':
				strcpy(tmp,"");
				for(i=1; i < strlen(menu_ret); i++)
					strncat(tmp,&menu_ret[i],1);
				sprintf(menu_ret,"%s.1",tmp);
				return menu_ret;
				break;
			default:
				return "";
				break;
		}
}
int _play_lesson(FILE *cr, int lesson_id)
{
	int i, i2, levels = _lesson_level_count(cr,lesson_id);
	char *level_ret = (char*) malloc(36);
	char *tmp = (char*) malloc(36);
	for(i=1; i<=levels; i++)
	{
		_clear_screen();
		printf("Press the up arrow or '*' to open menu.\n");
		printf("Lesson %d, level %d:\n\n",lesson_id,i);
		strcpy(level_ret,_play_level(cr,lesson_id,i));
		if(strcmp(level_ret,"") == 0)
		{
			printf("Press ENTER to continue...\n");
			#ifdef _WIN32
			while(getch() != '\r');
			#else
			while(getch() != '\n');
			#endif
		}
		else
		{
			strcpy(tmp,"");
			for(i2=0; i2 < strlen(level_ret); i2++)
			{
				if(level_ret[i2] == '.')
				{
					if(strtol(tmp,NULL,10) != lesson_id)
					{
						printf("exit\n");
						return strtol(tmp,NULL,10);
					}
					strcpy(tmp,"");
				}
				else
					strncat(tmp,&level_ret[i2],1);
			}
			i = strtol(tmp,NULL,10) - 1; // i increments after each period, so decrease it by 1 now
			levels = _lesson_level_count(cr,lesson_id);
		}
	}
	return 0;
}
void _menuentry(int id, char *name)
{
	printf("%d) %s\n",id,name);
}
void _main_menu(FILE *cr)
{
	int sel, lesson_id, new_lesson_id, lessons = _lesson_count(cr);
	bool end=false;
	while(!end)
	{
		_clear_screen();
		// Show welcome message (init screen)
		_init_screen();
		printf("Menu:\n");
		// Start of menu
		_menuentry(1,"Start with lesson 1");
		_menuentry(2,"Choose a lesson");
		_menuentry(3,"Quit");
		// End of menu
		printf("\nNumber: ");
		scanf("%d",&sel);
		end=true;
		switch(sel) {
			case 1:
				lesson_id=1;
				goto gameplay;
				break;
			case 2:
				lesson_id = _lesson_selection(cr,true);
				goto gameplay;
				break;
			case 3:
				exit(0);
				break;
			default:
				end=false;
				break;
		}
	}
	
	gameplay:
		while(true)
		{
			new_lesson_id = _play_lesson(cr,lesson_id);
			if(new_lesson_id == 0)
			{
				if(lesson_id < lessons)
				{
					lesson_id++;
				}
				else
					break;
			}
			else
				lesson_id = new_lesson_id;
		}
}
char *_menu(FILE *cr, int lesson_id)
{
	int sel;
	char *out = (char*) malloc(18);
	char tmp[17];
	strcpy(out,"");
	bool end=false;
	while(!end)
	{
		_clear_screen();
		printf("Menu:\n");
		// Start of menu
		_menuentry(1,"Skip level");
		_menuentry(2,"Choose another level");
		_menuentry(3,"Choose another lesson");
		_menuentry(4,"Quit");
		// End of menu
		printf("\nNumber: ");
		scanf("%d",&sel);
		end=true;
		switch(sel) {
			case 1:
				strcpy(out,"1");
				break;
			case 2:
				strcpy(out,"2");
				sprintf(tmp,"%d",_level_selection(cr,lesson_id));
				#ifdef DEBUG
				printf("D: Selected level: %s\n",tmp);
				#endif
				strcat(out,tmp);
				break;
			case 3:
				strcpy(out,"3");
				sprintf(tmp,"%d",_lesson_selection(cr,true));
				#ifdef DEBUG
				printf("D: Selected lesson: %s\n",tmp);
				#endif
				strcat(out,tmp);
				break;
			case 4:
				exit(0);
				break;
			default:
				end=false;
				break;
		}
	}
	return out;
}

#ifdef _WIN32
void SetWindow(int Width, int Height)
{ 
	COORD coord;
	coord.X = Width;
	coord.Y = Height;
	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}
#endif
