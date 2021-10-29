/*
 * configfile.cpp
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

#include "configfile.h"

int _lesson_count(FILE *cr)
{
	char c='\0';
	int line=0, out=0, lIDc, lesson;
	// Max possible number of lessons: 1000
	int lessons[1000];
	char *part = (char*) malloc(16);
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		lIDc=0;
		strcpy(part,"");
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config one line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_count()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		if(!int_array_contains(lessons,lesson,out))
		{
			lessons[out]=lesson;
			out++;
		}
		c='\0';
		// Sublesson ID, level ID and config
		while((c != ' ') && ((signed char) c != EOF))
			c=getc(cr);
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (sublesson ID, level ID or config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
			c=getc(cr);
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level text)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		line++;
	}
	return out;
}
int _lesson_sublesson_count(FILE *cr, int tlesson)
{
	char c='\0';
	int line=0, out=0, lIDc, lesson, sublesson;
	// Max possible number of sublessons: 1000
	int sublessons[1000];
	char *part = (char*) malloc(16);
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		lIDc=0;
		strcpy(part,"");
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config one line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_count()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		lIDc=0;
		strcpy(part,"");
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config one line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_count()\nD: Can't store more than 14 bytes in sublesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		if(lesson == tlesson)
		{
			sublesson=strtol(part,NULL,10);
			if(!int_array_contains(sublessons,sublesson,out))
			{
				sublessons[out]=sublesson;
				out++;
			}
		}
		c='\0';
		// Level ID and config
		while((c != ' ') && ((signed char) c != EOF))
			c=getc(cr);
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID or config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
			c=getc(cr);
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level text)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		line++;
	}
	return out;
}
int _lesson_sublesson_level_count(FILE *cr, int tlesson, int tsublesson)
{
	char c='\0';
	int line=0, out=0, lIDc, lesson, sublesson;
	char *part = (char*) malloc(16);
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_count()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_count()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		sublesson=strtol(part,NULL,10);
		c='\0';
		// Level ID and level config
		while((c != ' ') && ((signed char) c != EOF))
			c=getc(cr);
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID or config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		if((lesson == tlesson) && (sublesson == tsublesson))
			out++;
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
			c=getc(cr);
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level text)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		line++;
	}
	return out;
}
int _lesson_sublesson_level_line(FILE *cr, int tlesson, int tsublesson, int tlevel)
{
	char c='\0';
	int line=0, lesson, sublesson, level, lIDc;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		sublesson=strtol(part,NULL,10);
		c='\0';
		// Level ID
		strcpy(part,"");
		while((c != ':') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != ':')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in level ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		level=strtol(part,NULL,10);
		if( ((lesson == tlesson) && (sublesson == tsublesson) && (level == tlevel)) )
		{
			return line+1;
		}
		c='\0';
		// Level repeat config, length config and text
		while((c != '\n') && ((signed char) c != EOF))
		{
			c=getc(cr);
		}
		line++;
	}
	return 0;
}
char* _lesson_sublesson_level_repeat_type(FILE *cr, int tlesson, int tsublesson, int tlevel)
{
	char c='\0';
	int line=0, lesson, sublesson, level, lIDc;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	char repeat_type_str[64];
	strcpy(repeat_type_str,"");
	bool escape, has_limit_ext;
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		sublesson=strtol(part,NULL,10);
		c='\0';
		// Level ID
		strcpy(part,"");
		while((c != ':') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != ':')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in level ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		level=strtol(part,NULL,10);
		// Level config
		strcpy(part,"");
		has_limit_ext=false;
		while((c != ' ') && ((signed char) c != EOF))
		{
			c=getc(cr);
			escape=false;
			if(c == '\\')
			{
				c=getc(cr);
				escape=true;
			}
			if(((c == ' ') || (c == ';')) && !escape)
			{
				if(c == ' ')
				{
					if(!has_limit_ext)
						strcpy(repeat_type_str,part);
				}
				else
				{
					strcpy(repeat_type_str,part);
					has_limit_ext=true;
				}
				strcpy(part,"");
			}
			else
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level config: repeat text bool or limit extension length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in repeat text bool or limit extension\n");
					#endif
					exit(2);
				}
				if((c == ',') && !escape)
				{
					if(!has_limit_ext)
					{
						#ifdef DEBUG
						printf("repeat bool: %s\n",part);
						#endif
					}
					strcpy(part,"");
				}
				else
					strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		#ifdef DEBUG
		printf("D: repeat type: %s\n",repeat_type_str);
		#endif
		if( ((lesson == tlesson) && (sublesson == tsublesson) && (level == tlevel)) )
		{
			char *out = (char*) malloc(strlen(repeat_type_str)+1);
			strcpy(out,repeat_type_str);
			return out;
		}
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
		{
			c=getc(cr);
		}
		line++;
	}
	return (char*) "0";
}
int _lesson_sublesson_level_limit_extension(FILE *cr, int tlesson, int tsublesson, int tlevel)
{
	char c='\0';
	int line=0, lesson, sublesson, level, lIDc;
	int limit_extension = _REPEAT_LIMIT;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	char repeat_type_str[64];
	strcpy(repeat_type_str,"");
	bool escape, has_limit_ext, has_length_ext, has_desc;
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		sublesson=strtol(part,NULL,10);
		c='\0';
		// Level ID
		strcpy(part,"");
		while((c != ':') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != ':')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in level ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		level=strtol(part,NULL,10);
		// Level config
		strcpy(part,"");
		has_limit_ext=false;
		has_length_ext=false;
		has_desc=false;
		while((c != ' ') && ((signed char) c != EOF))
		{
			c=getc(cr);
			escape=false;
			if(c == '\\')
			{
				c=getc(cr);
				escape=true;
			}
			if(((c == ' ') || (c == ';')) && !escape)
			{
				if(c == ' ')
				{
					if(has_limit_ext)
					{
						if(!(has_length_ext) && (!has_desc))
						{
							limit_extension = strtol(part,NULL,10);
						}
					}
					else
						strcpy(repeat_type_str,part);
				}
				else
				{
					strcpy(repeat_type_str,part);
					has_limit_ext=true;
				}
				strcpy(part,"");
			}
			else
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level config: repeat text bool or limit extension length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in repeat text bool or limit extension\n");
					#endif
					exit(2);
				}
				if((c == ',') && !escape)
				{
					if(has_limit_ext)
					{
						if(has_length_ext)
						{
							has_desc=true;
						}
						else
							limit_extension = strtol(part,NULL,10);
						has_length_ext=true;
					}
					else
					{
						#ifdef DEBUG
						printf("repeat bool: %s\n",part);
						#endif
					}
					strcpy(part,"");
				}
				else
					strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		#ifdef DEBUG
		printf("D: repeat type: %s\n",repeat_type_str);
		printf("D: limit extension: %d\n",limit_extension);
		#endif
		if( ((lesson == tlesson) && (sublesson == tsublesson) && (level == tlevel)) )
		{
			return limit_extension;
		}
		else
			limit_extension = _REPEAT_LIMIT;
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
		{
			c=getc(cr);
		}
		line++;
	}
	return _REPEAT_LIMIT;
}
int _lesson_sublesson_level_length_extension(FILE *cr, int tlesson, int tsublesson, int tlevel)
{
	char c='\0';
	int line=0, lesson, sublesson, level, lIDc;
	int length_extension = _REPEAT_LIMIT;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	char repeat_type_str[64];
	strcpy(repeat_type_str,"");
	bool escape, has_limit_ext, has_length_ext, has_desc;
	// Set locale
	setlocale(LC_ALL,"C");
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		sublesson=strtol(part,NULL,10);
		c='\0';
		// Level ID
		strcpy(part,"");
		while((c != ':') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != ':')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in level ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		level=strtol(part,NULL,10);
		// Level config
		strcpy(part,"");
		has_limit_ext=false;
		has_length_ext=false;
		has_desc=false;
		while((c != ' ') && ((signed char) c != EOF))
		{
			c=getc(cr);
			escape=false;
			if(c == '\\')
			{
				c=getc(cr);
				escape=true;
			}
			if(((c == ' ') || (c == ';')) && !escape)
			{
				if(c == ' ')
				{
					if(has_limit_ext)
					{
						if((has_length_ext) && (!has_desc))
						{
							length_extension = strtol(part,NULL,10);
						}
					}
					else
						strcpy(repeat_type_str,part);
				}
				else
				{
					strcpy(repeat_type_str,part);
					has_limit_ext=true;
				}
				strcpy(part,"");
			}
			else
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level config: repeat text bool or limit extension length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in repeat text bool or limit extension\n");
					#endif
					exit(2);
				}
				if((c == ',') && !escape)
				{
					if(has_limit_ext)
					{
						if(has_length_ext)
						{
							length_extension = strtol(part,NULL,10);
							has_desc=true;
						}
						has_length_ext=true;
					}
					else
					{
						#ifdef DEBUG
						printf("repeat bool: %s\n",part);
						#endif
					}
					strcpy(part,"");
				}
				else
					strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		#ifdef DEBUG
		printf("D: repeat type: %s\n",repeat_type_str);
		printf("D: length extension: %d\n",length_extension);
		#endif
		if( ((lesson == tlesson) && (sublesson == tsublesson) && (level == tlevel)) )
		{
			return length_extension;
		}
		else
			length_extension = _REPEAT_LIMIT;
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
		{
			c=getc(cr);
		}
		line++;
	}
	return _REPEAT_LIMIT;
}
char *_lesson_desc(FILE *cr, int tlesson)
{
	char c='\0';
	int line=0, lesson, lIDc;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	char repeat_type_str[64];
	strcpy(repeat_type_str,"");
	bool escape, has_limit_ext, has_length_ext, has_desc;
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		c='\0';
		// Level ID
		strcpy(part,"");
		while((c != ':') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != ':')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in level ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		// Level config
		strcpy(part,"");
		has_limit_ext=false;
		has_length_ext=false;
		has_desc=false;
		while((c != ' ') && ((signed char) c != EOF))
		{
			c=getc(cr);
			escape=false;
			if(c == '\\')
			{
				c=getc(cr);
				escape=true;
			}
			if(((c == ' ') || (c == ';')) && !escape)
			{
				if(c == ' ')
				{
					if(has_limit_ext)
					{
						if(has_length_ext)
						{
							if(has_desc)
							{
								if(lesson == tlesson)
									return part;
							}
						}
					}
					else
						strcpy(repeat_type_str,part);
				}
				else
				{
					strcpy(repeat_type_str,part);
					has_limit_ext=true;
				}
				strcpy(part,"");
			}
			else
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level config: repeat text bool or limit extension length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in repeat text bool or limit extension\n");
					#endif
					exit(2);
				}
				if((c == ',') && !escape)
				{
					if(has_limit_ext)
					{
						if(has_length_ext)
						{
							if(has_desc)
							{
								if(lesson == tlesson)
									return part;
							}
							has_desc=true;
						}
						has_length_ext=true;
					}
					else
					{
						#ifdef DEBUG
						printf("repeat bool: %s\n",part);
						#endif
					}
					strcpy(part,"");
				}
				else
					strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		c='\0';
		// Level text
		while((c != '\n') && ((signed char) c != EOF))
		{
			c=getc(cr);
		}
		line++;
	}
	return (char*) "";
}
char *_lesson_sublesson_level_text(FILE *cr, int tlesson, int tsublesson, int tlevel, bool random_order)
{
	char c='\0';
	int line=0, lesson, sublesson, level, lIDc, text_repeat_type=0, i, limit_extension = _REPEAT_LIMIT, wordID, outLength=0;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	char *part2;
	char *out;
	char repeat_type_str[64];
	strcpy(repeat_type_str,"");
	bool escape, text_repeat=false, end, has_limit_ext, has_length_ext, has_desc;
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while((signed char) c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (lesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (lesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		lesson=strtol(part,NULL,10);
		c='\0';
		// Sublesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != '.')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (sublesson ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in lesson ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
			lIDc++;
		}
		if((signed char) c == EOF)
		{
			if(lIDc == 1)
				break;
			printf("Invalid config on line %d. (sublesson ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		sublesson=strtol(part,NULL,10);
		c='\0';
		// Level ID
		strcpy(part,"");
		while((c != ':') && ((signed char) c != EOF))
		{
			c=getc(cr);
			if(c != ':')
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level ID length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in level ID\n");
					#endif
					exit(2);
				}
				strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level ID)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		level=strtol(part,NULL,10);
		// Level config
		strcpy(part,"");
		has_limit_ext=false;
		has_length_ext=false;
		has_desc=false;
		while((c != ' ') && ((signed char) c != EOF))
		{
			c=getc(cr);
			escape=false;
			if(c == '\\')
			{
				c=getc(cr);
				escape=true;
			}
			if(((c == ' ') || (c == ';')) && !escape)
			{
				if(c == ' ')
				{
					if(has_limit_ext)
					{
						if((!has_length_ext) && (!has_desc))
							limit_extension = strtol(part,NULL,10);
					}
					else
						strcpy(repeat_type_str,part);
				}
				else
				{
					strcpy(repeat_type_str,part);
					has_limit_ext=true;
				}
				strcpy(part,"");
			}
			else
			{
				if(strlen(part) >= 15)
				{
					printf("Invalid config on line %d. (level config: repeat text bool or limit extension length)\n",line+1);
					#ifdef DEBUG
					printf("D: Function: _lesson_sublesson_level_text()\nD: Can't store more than 14 bytes in repeat text bool or limit extension\n");
					#endif
					exit(2);
				}
				if((c == ',') && !escape)
				{
					if(has_limit_ext)
					{
						if(has_length_ext)
							has_desc=true;
						else
						{
							limit_extension = strtol(part,NULL,10);
							has_length_ext=true;
						}
					}
					else
					{
						#ifdef DEBUG
						printf("repeat bool: %s\n",part);
						#endif
						if(strcmp(part,"1") == 0)
							text_repeat=true;
						else if(strcmp(part,"0") == 0)
							text_repeat=false;
						else
						{
							printf("Invalid config on line %d. (level config: repeat text bool)\n",line+1);
							exit(2);
						}
					}
					strcpy(part,"");
				}
				else
					strncat(part,&c,1);
			}
		}
		if((signed char) c == EOF)
		{
			printf("Invalid config on line %d. (level config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		#ifdef DEBUG
		printf("D: repeat type: %s\n",repeat_type_str);
		#endif
		if(strcmp(repeat_type_str,"w") == 0)
			text_repeat_type=1; // repeating word
		else if(strcmp(repeat_type_str,"s") == 0)
			text_repeat_type=2; // repeating string
		else if(strcmp(repeat_type_str,"rw") == 0)
			text_repeat_type=3; // random words
		else
		{
			if(text_repeat)
			{
				printf("Invalid config on line %d. (level config: repeat method)\n",line+1);
				#ifdef DEBUG
				printf("D: Function: _lesson_sublesson_level_text()\nD: Unknown repeat method '%s'\n",repeat_type_str);
				#endif
				exit(2);
			}
		}
		if( !( ((lesson == tlesson) && (sublesson == tsublesson) && (level == tlevel)) ) )
		{
			limit_extension=1;
			has_limit_ext=false;
		}
		#ifdef DEBUG
		if(has_limit_ext)
			printf("D: Limit extension: %d\n",limit_extension);
		#endif
		c='\0';
		// Level text
		#ifdef __unix__
		fflush(cr);
		#endif
		out = (char*) malloc(limit_extension*2+1);
		strcpy(out,"");
		while((c != '\n') && ((signed char) c != EOF))
		{
			strcpy(part,"");
			while((c != '\n') && ((signed char) c != EOF))
			{
				c=getc(cr);
				if((c != '\n') && (c != 0x0d))
				{
					if((strlen(part)+2) > part_alloc)
					{
						part_alloc=part_alloc+2;
						part = (char*) realloc(part,part_alloc);
					}
					strncat(part,&c,1);
				}
			}
			if((signed char) c == EOF)
			{
				printf("Invalid config on line %d. (level text)\n",line+1);
				#ifdef DEBUG
				printf("D: Function: _lesson_sublesson_level_text()\nD: Reached EOF\n");
				#endif
				exit(2);
			}
			if((lesson == tlesson) && (sublesson == tsublesson) && (level == tlevel))
			{
				if(text_repeat)
				{
					if((text_repeat_type == 3) && random_order)
					{
						part2 = (char*) malloc(strlen(part));
						srand(time(0));
						while(true)
						{
							wordID = rand() % _get_word_count(part) + 1;
							strcpy(part2,_get_word(part,wordID));
							if(strlen(out)+1+strlen(part2) > (unsigned int) limit_extension)
								break;
							else
							{
								if(strcmp(out,"") == 0)
									strcpy(out,part2);
								else
								{
									strcat(out," ");
									strcat(out,part2);
								}
							}
						}
						return out;
					}
					else
					{
						end=false;
						while(!end)
						{
							for(i=1; i <= _get_word_count(part); i++)
							{
								if(strcmp(out,"") == 0)
								{
									if(QStringLen(_get_word(part,i)) > limit_extension)
									{
										end=true;
										break;
									}
									strcpy(out,_get_word(part,i));
								}
								else
								{
									if(outLength+1+QStringLen(_get_word(part,i)) > limit_extension) // 1 is for a space between out and part
									{
										end=true;
										break;
									}
									strcat(out," ");
									strcat(out,_get_word(part,i));
									outLength += QStringLen(" ");
								}
								outLength += QStringLen(_get_word(part,i));
							}
						}
						return out;
					}
				}
				else
					return part;
			}
		}
		line++;
	}
	return (char*) "";
}
// Write functions
void _add_level(FILE *cr, int tlesson, int tsublesson, int tlevel,
	bool repeat, char *repeat_type,
	int limit_extension, int length_extension, char *desc,
	const char *level_text)
{
	// Add level line
	fprintf(cr,"%d.%d.%d:",tlesson,tsublesson,tlevel);
	if(repeat)
		putc('1',cr);
	else
		putc('0',cr);
	fprintf(cr,",%s;%d,%d",repeat_type,limit_extension,length_extension);
	if(strcmp(desc,"") != 0)
		fprintf(cr,",%s",desc);
	fprintf(cr," %s\n",level_text);
}
