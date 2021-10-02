/*
 * utils.c
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

#include "utils.h"

bool int_array_contains(int arr[], int target, int count)
{
	int i;
	for(i=0; i < count; i++)
	{
		if(arr[i] == target)
			return true;
	}
	return false;
}
void _clear_screen(void)
{
	#ifdef DEBUG
	printf("D: Skipping screen clearing\n");
	#else
	#ifdef _WIN32
	system("cls");
	#else
	#ifdef __unix__
	printf("\033[H\033[J");
	#endif
	#endif
	#endif
}
int _download_file(const char *url, const char *dest)
{
	#ifdef _WIN32
	return URLDownloadToFile(NULL,url,dest,0,NULL);
	#endif
}

#ifdef _WIN32
char *_get_win_release(void)
{
	char *out = (char*) malloc(16);
	strcpy(out,"");
	if (IsWindowsXPOrGreater())
	{
		strcpy(out,"XP");
	}
	if (IsWindowsVistaOrGreater())
	{
		strcpy(out,"Vista");
	}
	if (IsWindows7OrGreater())
	{
		strcpy(out,"7");
	}
	if (IsWindows8OrGreater())
	{
		strcpy(out,"8");
	}
	if (IsWindows8Point1OrGreater())
	{
		strcpy(out,"8.1");
	}
	if (IsWindows10OrGreater())
	{
		strcpy(out,"10");
	}
	if(strcmp(out,"") == 0)
		return NULL;
	else
		return out;
}
WCHAR* str_to_wcs(const char* str)
{
    size_t size = MultiByteToWideChar(CP_UTF8,0,str,-1,NULL,0)*sizeof(WCHAR)+2;
    WCHAR* out = (WCHAR*) malloc(size);
    wcscpy(out,L"");
    MultiByteToWideChar(CP_UTF8,0,&str[0],-1,out,size);
    return out;
}
bool _win_internet_available(void)
{
	DWORD result;
	InternetGetConnectedState(&result,0);
	return (result == 18);
}
#endif
