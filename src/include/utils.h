#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#include <versionhelpers.h>
#endif

bool int_array_contains(int arr[], int target, int count);
void _clear_screen(void);
void _download_file(const char *url, const char *dest);

#ifdef _WIN32
char *_get_win_release(void);
WCHAR* str_to_wcs(const char* str);
#endif
