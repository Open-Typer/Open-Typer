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
	while(c != EOF)
	{
		// Lesson ID
		lIDc=0;
		strcpy(part,"");
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != ' ') && (c != EOF))
			c=getc(cr);
		if(c == EOF)
		{
			printf("Invalid config on line %d. (sublesson ID, level ID or config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		c='\0';
		// Level text
		while((c != '\n') && (c != EOF))
			c=getc(cr);
		if(c == EOF)
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
	while(c != EOF)
	{
		// Lesson ID
		lIDc=0;
		strcpy(part,"");
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != ' ') && (c != EOF))
			c=getc(cr);
		if(c == EOF)
		{
			printf("Invalid config on line %d. (level ID or config)\n",line+1);
			#ifdef DEBUG
			printf("D: Function: _lesson_sublesson_count()\nD: Reached EOF\n");
			#endif
			exit(2);
		}
		c='\0';
		// Level text
		while((c != '\n') && (c != EOF))
			c=getc(cr);
		if(c == EOF)
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
	while(c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != ' ') && (c != EOF))
			c=getc(cr);
		if(c == EOF)
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
		while((c != '\n') && (c != EOF))
			c=getc(cr);
		if(c == EOF)
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
char *_lesson_sublesson_level_text(FILE *cr, int tlesson, int tsublesson, int tlevel, bool random_order)
{
	char c='\0';
	int line=0, lesson, sublesson, level, lIDc, text_repeat_type=0, i, wordID, limit_extension=1;
	unsigned int part_alloc=16;
	char *part = (char*) malloc(part_alloc);
	char *part2;
	char *out;
	char repeat_type_str[64];
	strcpy(repeat_type_str,"");
	bool text_repeat=false, end, has_limit_ext;
	// NOTE: realloc is used only in the text part
	// Who would use a number longer than 16 bytes?
	rewind(cr);
	while(c != EOF)
	{
		// Lesson ID
		strcpy(part,"");
		lIDc=0;
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != '.') && (c != EOF))
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
		if(c == EOF)
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
		while((c != ':') && (c != EOF))
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
		if(c == EOF)
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
		while((c != ' ') && (c != EOF))
		{
			c=getc(cr);
			if((c == ' ') || (c == ';'))
			{
				if(c == ' ')
				{
					if(has_limit_ext)
						limit_extension = strtol(part,NULL,10);
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
				if(c == ',')
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
					strcpy(part,"");
				}
				else
					strncat(part,&c,1);
			}
		}
		if(c == EOF)
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
		out = (char*) malloc(_REPEAT_LIMIT*limit_extension+1);
		strcpy(out,"");
		while((c != '\n') && (c != EOF))
		{
			strcpy(part,"");
			while((c != '\n') && (c != EOF))
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
			if(c == EOF)
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
							if(strlen(out)+1+strlen(part2) > (unsigned int) _REPEAT_LIMIT*limit_extension)
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
									if(strlen(_get_word(part,i)) > (unsigned int) _REPEAT_LIMIT*limit_extension)
									{
										end=true;
										break;
									}
									strcpy(out,_get_word(part,i));
								}
								else
								{
									if(strlen(out)+1+strlen(_get_word(part,i)) > (unsigned int) _REPEAT_LIMIT*limit_extension) // 1 is for a space between out and part
									{
										end=true;
										break;
									}
									strcat(out," ");
									strcat(out,_get_word(part,i));
								}
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
