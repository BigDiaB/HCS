
#pragma once
#define ERRORS_EXIT
/*
    LSD: Logging System, Duh!
    Simples Logging-System mit Wichtigkeits-Stufen
*/

typedef enum {
    LSD_ltMESSAGE,LSD_ltWARNING,LSD_ltERROR
} LSD_Log_type;

typedef enum {
    LSD_llALL, LSD_llBAD, LSD_llERROR
} LSD_Log_level;

LSD_Log_level LSD_Log_level_now;

void LSD_Log_level_set(LSD_Log_level level)
{
    LSD_Log_level_now = level;
}

void LSD_Log(LSD_Log_type type, char* format, ...)
{
    char* string;
    va_list args;
    va_start(args, format);
    if(0 > vasprintf(&string, format, args)) string = NULL;
    va_end(args);
    switch (type) {
        case LSD_ltMESSAGE:
            if (LSD_Log_level_now == LSD_llALL)
            printf("Nachricht: %s\n",string);
            break;
        case LSD_ltWARNING:
            if (LSD_Log_level_now != LSD_llERROR)
            printf("WARNUNG!: %s\n",string);
            break;
        case LSD_ltERROR:
            printf("\n####################\nERROR!:\n%s\n\n####################\n",string);
#ifdef ERRORS_EXIT
            exit(1);
#endif
            break;
            
        default:
            break;
    }
    free(string);
}