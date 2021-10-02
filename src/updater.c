/*
 * updater.c
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

#include "updater.h"

char *_get_latest_version(void)
{
	#ifdef _WIN32
	// This function is enable only for Windows for now
	#ifdef DEBUG
	printf("D: Checking latest version...\n");
	#endif
	char *Makefile_prefix = "https://raw.githubusercontent.com/";
	char *Makefile_suffix = "/master/Makefile";
	char *Makefile_URL = (char*) malloc(strlen(Makefile_prefix)+strlen(_GITHUB_PATH)+strlen(Makefile_suffix)+1);
	sprintf(Makefile_URL,"%s%s%s",Makefile_prefix,_GITHUB_PATH,Makefile_suffix);
	#ifdef _WIN32
	if(!_win_internet_available())
	{
		#ifdef DEBUG
		printf("D: No internet connection\n");
		#endif
		return _PROJECT_VERSION;
	}
	char *Makefile_local_path="$open-typer-latest";
	#else
	char *Makefile_local_path=".open-typer-latest";
	#endif
	// Download Makefile from GitHub (contains latest version code)
	int dwret = _download_file(Makefile_URL,Makefile_local_path);
	if(dwret != 0)
	{
		#ifdef DEBUG
		printf("D: Download failure, error: %d\n",dwret);
		#endif
		return _PROJECT_VERSION;
	}
	#ifdef DEBUG
	printf("D: Downloaded Makefile into %s\n",Makefile_local_path);
	#endif
	// Read first line
	// Should be something like:
	// VERSION = 0.1.3
	FILE *Makefile = fopen(Makefile_local_path,"r");
	char *line = (char*) malloc(64);
	char c;
	strcpy(line,"");
	while((c != '\n') && (c != EOF))
	{
		c=getc(Makefile);
		if((c != '\n') && (c != EOF))
			strncat(line,&c,1);
	}
	fclose(Makefile);
	#ifdef DEBUG
	printf("D: Read first line: %s\n",line);
	#endif
	// Get version code
	char *out = (char*) malloc(strlen(line)+1);
	strcpy(out,"");
	// Version code starts after 10 characters
	for(int i=11; i < strlen(line); i++)
		strncat(out,&line[i],1);
	#ifdef DEBUG
	printf("D: Got version: %s\n",out);
	#endif
	return out;
	#else
	return "";
	#endif
}
int _install_update(char *latest)
{
	#ifdef _WIN32
	// This function is enabled only for Windows for now
	char *URL_prefix = "https://github.com/";
	char *URL_suffix1 = "/releases/download/v";
	#ifdef _WIN32
	char *URL_suffix2 = "open-typer.exe";
	#else
	char *URL_suffix2 = "open-typer-linux-amd64";
	#endif
	char *URL = (char*) malloc(strlen(URL_prefix)+strlen(_GITHUB_PATH)+strlen(URL_suffix1)+strlen(latest)+1+strlen(URL_suffix2)+1);
	sprintf(URL,"%s%s%s%s/%s",URL_prefix,_GITHUB_PATH,URL_suffix1,latest,URL_suffix2);
	#ifdef _WIN32
	if(!_win_internet_available())
	{
		#ifdef DEBUG
		printf("D: No internet connection\n");
		#endif
		return -1;
	}
	#endif
	#ifdef DEBUG
	printf("D: Downloading %s\n",URL);
	#endif
	int dwret = _download_file(URL,URL_suffix2);
	if(dwret != 0)
	{
		#ifdef DEBUG
		printf("D: Download failure, error: %d\n",dwret);
		#endif
		return dwret;
	}
	#ifdef DEBUG
	printf("D: Done\n",URL);
	#endif
	#endif
}
