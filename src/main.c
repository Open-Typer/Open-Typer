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
#include <errno.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#include <locale.h>
#endif
#include "projectconfig.h"
#include "utils.h"
#include "configfile.h"
#include "ui.h"
#include "updater.h"

// Debug build warning
#ifdef DEBUG
#pragma message "This is a debug build."
#endif

int main()
{
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
	SetWindow(140,30);
	// Remove old version after update
	remove("open-typer.exe.bak");
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
					// Update subscribed config file
					char *sub_url = _get_sub_url(confr);
					fclose(confr);
					_download_file(sub_url,_DEFAULT_CONFIG);
					MessageBox(NULL,"Successfully updated.","Update completed",MB_OK);
					WinExec("open-typer.exe",0);
					return 0;
				}
			}
		}
	}
	#endif
	// Main loop
	while(true)
		_main_menu(confr);
	fclose(confr);
	return 0;
}
