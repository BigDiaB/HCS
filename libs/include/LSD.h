
#pragma once
#define ERRORS_EXIT
/*
    LSD: Logging System, Duh!
    Simples Logging-System mit Wichtigkeits-Stufen
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum {
    LSD_ltMESSAGE,LSD_ltWARNING,LSD_ltERROR
} LSD_Log_type;

typedef enum {
    LSD_llALL, LSD_llBAD, LSD_llERROR
} LSD_Log_level;


LSD_Log_level LSD_Log_level_now;

void LSD_Log_level_set(LSD_Log_level level);

void LSD_Log(LSD_Log_type type, char* format, ...);
