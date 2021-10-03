#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <wchar.h>
#endif
#include "utils.h"
#include "projectconfig.h"
#include "configfile.h"
#include "keyboard.h"

void _init_screen(void);
int _lesson_selection(FILE *cr, bool clear);
int _level_selection(FILE *cr, int lesson_id);
char *_play_level(FILE *cr, int lesson_id, int level_id);
int _play_lesson(FILE *cr, int lesson_id);
void _menuentry(int id, char *name);
void _main_menu(FILE *cr);
char *_menu(FILE *cr, int lesson_id);

#ifdef _WIN32
void SetWindow(int Width, int Height);
#endif
