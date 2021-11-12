
#ifndef LSD_H
#define LSD_H
#define ERRORS_EXIT
/*
    LSD: Logging System, Duh!
    Simples Logging-System mit Wichtigkeits-Stufen
    Und simplem File-Handling
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    LSD_ltMESSAGE,LSD_ltWARNING,LSD_ltERROR
} LSD_Log_type;

typedef enum {
    LSD_llALL, LSD_llBAD, LSD_llERROR
} LSD_Log_level;

void LSD_Log_level_set(LSD_Log_level level);

void LSD_Log(LSD_Log_type type, char* format, ...);

typedef struct
{
    int length;
    char* data;
} LSD_String;

typedef struct
{
    char* filename;
    char* data;
    int lines;
} LSD_File;

LSD_String* LSD_String_new(char* init);
void LSD_String_free(LSD_String* string);
void LSD_String_append(LSD_String* string, char* add);

LSD_File* LSD_File_open(char* filename);
void LSD_File_close(LSD_File* f);
LSD_File* LSD_File_write(LSD_File* file, char* data);

static LSD_Log_level LSD_Log_level_now = LSD_llALL;

#endif
