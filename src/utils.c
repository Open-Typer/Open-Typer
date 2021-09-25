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
	system("clear");
	#endif
	#endif
	#endif
}
