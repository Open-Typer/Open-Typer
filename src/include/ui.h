#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "projectconfig.h"
#include "configfile.h"

void _init_screen(void);
int _lesson_selection(FILE *cr, bool clear);
int _level_selection(FILE *cr, int lesson_id);
void _menuentry(int id, char *name);
char *_menu(FILE *cr, int lesson_id);
