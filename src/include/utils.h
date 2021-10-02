#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#include <versionhelpers.h>
#endif

bool int_array_contains(int arr[], int target, int count);
void _clear_screen(void);

#ifdef _WIN32
char *_get_win_release(void);
#endif
