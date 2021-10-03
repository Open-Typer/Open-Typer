/*
 * main.c
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
 
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#include <locale.h>
#include <fcntl.h>
#include <wchar.h>
#endif
#include "projectconfig.h"
#include "utils.h"
#include "configfile.h"
#include "keyboard.h"
#include "ui.h"
#include "updater.h"

// Windows support warning
#ifdef _WIN32
#pragma message "Windows support is experimental."
#endif

// Debug build warning
#ifdef DEBUG
#pragma message "This is a debug build."
#endif

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
		#ifdef __unix__
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
					#ifdef __unix__
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
		printf("Press ");
		#ifdef __unix__
		printf("the up arrow or ");
		#endif
		printf("'*' to open menu.\n");
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
int main()
{
	#ifdef _WIN32
	// Check for updates
	char *latest_ver = _get_latest_version();
	bool update_success=false;
	if(strcmp(_PROJECT_VERSION,latest_ver) != 0)
	{
		int dialog = MessageBox(NULL,"There's a new update available.\nWould you like to download and install it?\n\nThis will replace the open-typer.exe file.","Update available",MB_ICONEXCLAMATION | MB_YESNO);
		if(dialog == IDYES)
		{
			int instret;
			while(!update_success)
			{
				instret = _install_update(latest_ver);
				if(instret != 0)
				{
					dialog = MessageBox(NULL,"Failed to download and/or install the update!","Update failure",MB_ICONERROR | MB_ABORTRETRYIGNORE);
					if(dialog == IDABORT)
						return 0;
					else if(dialog == IDIGNORE)
						update_success=true;
				}
				else
				{
					MessageBox(NULL,"Please launch the program again.","Update completed",MB_OK);
					return 0;
				}
			}
		}
	}
	#endif
	// Open config file
	errno=0;
	#ifdef _WIN32
	FILE *confr = fopen(_DEFAULT_CONFIG,"rb+");
	#else
	FILE *confr = fopen(_DEFAULT_CONFIG,"r");
	#endif
	if(errno != 0)
	{
		printf("%s: %s\n",_DEFAULT_CONFIG,strerror(errno));
		if(errno == 2)
		{
			_default_config(_DEFAULT_CONFIG);
			#ifdef _WIN32
			confr = fopen(_DEFAULT_CONFIG,"rb+");
			#else
			confr = fopen(_DEFAULT_CONFIG,"r");
			#endif
		}
		else
			exit(1);
	}
	#ifdef _WIN32
	// Check version support
	char *winver = _get_win_release();
	if((strcmp(winver,"7") != 0) && (strcmp(winver,"8") != 0) && (strcmp(winver,"8.1") != 0) && (strcmp(winver,"10") != 0))
	{
		ShowWindow(GetConsoleWindow(),SW_HIDE);
		MessageBox(NULL, "You need at least Windows 7", "Version Not Supported", MB_ICONERROR | MB_OK);
		exit(10);
	}
	// Fix multi-byte characters I/O on Windows
	// Caution: multi-byte char input is broken on Windows 7.
	setlocale(LC_ALL,"en_US.UTF-8");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	// Set console font
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 15;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	// Resize console window (see ui.c)
	SetWindow(120,25);
	#endif
	// Show welcome message (init screen)
	_init_screen();
	// Ask for lesson
	int i, lesson_id, new_lesson_id, lessons = _lesson_count(confr);
	lesson_id = _lesson_selection(confr,false);
	putchar('\n');
	// Main loop
	bool _exit=false;
	char yn;
	while(!_exit)
	{
		new_lesson_id = _play_lesson(confr,lesson_id);
		if(new_lesson_id == 0)
		{
			if(lesson_id < lessons)
			{
				lesson_id++;
			}
			else
				_exit=true;
		}
		else
			lesson_id = new_lesson_id;
	}
	fclose(confr);
	return 0;
}
