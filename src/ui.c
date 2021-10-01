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
void _menuentry(int id, char *name)
{
	printf("%d) %s\n",id,name);
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
				out[0]='1';
				break;
			case 2:
				out[0]='2';
				sprintf(tmp,"%d",_level_selection(cr,lesson_id));
				#ifdef DEBUG
				printf("D: Selected level: %s\n",tmp);
				#endif
				strcat(out,tmp);
				break;
			case 3:
				out[0]='3';
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
