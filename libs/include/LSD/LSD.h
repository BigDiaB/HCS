
#ifndef LSD_H
#define LSD_H
/*
    LSD: Logging System, Duh!
    Simples Logging-System mit Wichtigkeits-Stufen
    Und simplem File-Handling
    Und simplem Multi-Threading
*/

#define LSD_THREADS_MAX 100
#define LSD_DELTAS_MAX 100

typedef char bool;
const char true = 1;
const char false = 0;

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define LSD_Delta_none          0.0f
#define LSD_Thread_function(X)  void* X(void* i)
#define LSD_Thread_init()       unsigned char LSD_Thread_index = *((int*) i)
#define LSD_Thread_finish()     LSD_Threads[LSD_Thread_index].done = true; return NULL

typedef enum {
    LSD_ltMESSAGE,LSD_ltWARNING,LSD_ltERROR
} LSD_Log_type;

typedef enum {
    LSD_llALL, LSD_llBAD, LSD_llERROR
} LSD_Log_level;

typedef enum {
    LSD_tt_cycle, LSD_tt_single
} LSD_Thread_type;

typedef struct
{
    char* filename;
    char* data;
    int lines;
} LSD_File;

typedef struct {
    char* name;
    void* (*function)(void*);
    bool done;
    pthread_t tid;
    unsigned char lid;
} LSD_Thread;

typedef struct {
    struct timeval begin, end;
    char* name;
    double delta;
    double last_delta;
} LSD_Delta;

LSD_Thread LSD_Threads[LSD_THREADS_MAX];
int LSD_Thread_list[LSD_THREADS_MAX];
int LSD_Thread_used;
LSD_Delta LSD_Deltas[LSD_DELTAS_MAX];
int LSD_Delta_list[LSD_DELTAS_MAX];
int LSD_Delta_used;
LSD_Log_level LSD_Log_level_now;

/* Hiermit kann das Level gesetzt werden, bei dem Logs tatsächlich in stdout angezeigt werden */
void LSD_Log_level_set(LSD_Log_level level);

/* Logging Funktion mit Vargargs, wie bei printf() */
void LSD_Log(LSD_Log_type type, char* format, ...);

/* Öffne eine Datei. Danach kann das Feld "data" für den Inhalt und "lines" für die Anzahl der Zeilen ausgelesen werden */
LSD_File* LSD_File_open(char* filename);

/* Freit den Speicher der für den Inhalt einer Datei benötigt wurde */
void LSD_File_close(LSD_File* f);

/* Schreibt einen char-pointer in eine Datei */
LSD_File* LSD_File_write(LSD_File* file, char* data);

/* Fügt einen Thread mit einem Namen und einer Funktion hinzu */
void LSD_Thread_add(char* name, void*(*func)(void*));

/* Check, ob ein Thread fertiwg ist und entfernt ihn gegebenenfalls aus der Liste */
void LSD_Thread_system();

/* Callback um zu sehen, ob ein Thread fertig ist. Wird ohne LSD_Thread_system() nicht benötigt */
bool LSD_Thread_done(char* name);

/* Füge einen neuen Timer gelinkt mit einem Namen hinzu */
void LSD_Delta_add(char* name);

/* Das LSD-Delta-Struct nach Namen finden */
LSD_Delta* LSD_Delta_get(char* name);

/* LSD-Delta-Struct updaten (Sollte am anfang von einem Loop gecallt werden!) */
void LSD_Delta_tick(char* name);

/* Entfernt ein LSD_Delta */
void LSD_Delta_remove(char* name);

#endif
