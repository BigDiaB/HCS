
#ifndef LSD_H
#define LSD_H
/*
    LSD: Logging System, Duh!
    Simples Logging-System mit Wichtigkeits-Stufen
    Und simplem File-Handling
    Und simplem Multi-Threading
    Und Vektoren
    Und Array-Funktionen
    Und Path-Funktionen
*/

#define LSD_THREADS_MAX 100
#define LSD_DELTAS_MAX 100
#define LSD_WEBSERVER_MAX_CONNECTIONS 1
#define LSD_WEBSERVER_PORT 1234
#define LSD_WEBSERVER_MAX_READBUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>   

typedef int bool;

#define LSD_Delta_none          0.0f
#define LSD_Thread_function(X)  void* X(void* i)
#define LSD_Thread_init()       unsigned char LSD_Thread_index = *((int*) i)
#define LSD_Thread_finish()     LSD_Threads[LSD_Thread_index].done = true; return NULL

#define LSD_Vec_add(Z,X,Y) Z.x = X.x + Y.x; Z.y = X.y + Y.y;
#define LSD_Vec_sub(Z,X,Y) Z.x = X.x - Y.x; Z.y = X.y - Y.y;
#define LSD_Vec_mul(Z,X,Y) Z.x = X.x * Y.x; Z.y = X.y * Y.y;
#define LSD_Vec_div(Z,X,Y) Z.x = X.x / Y.x; Z.y = X.y / Y.y;

#define true 1
#define false 0

struct LSD_Vec2i
{
    int x,y;
};

struct LSD_Vec2f
{
    float x,y;
};

struct LSD_Vec2d
{
    double x,y;
};

enum LSD_Log_type
{
    LSD_ltMESSAGE,LSD_ltWARNING,LSD_ltERROR, LSD_ltCUSTOM
};

enum LSD_Log_level
{
    LSD_llALL, LSD_llBAD, LSD_llERROR
};

struct LSD_File
{
    char* filename;
    char* data;
    int lines;
};

struct LSD_Thread
{
    char* name;
    void* (*function)(void*);
    bool done;
    pthread_t tid;
    unsigned char lid;
};

struct LSD_Delta
{
    struct timeval begin, end;
    char* name;
    double delta;
    double last_delta;
};

struct LSD_WebServer
{
    int server_fd, client_fd;
    int addrlen;
    struct sockaddr_in address;

    void (*POST_handle)(struct LSD_WebServer* server);
    void (*GET_handle)(struct LSD_WebServer* server);

    char read_buffer[LSD_WEBSERVER_MAX_READBUFFER_SIZE];

    const char* directory_prefix;
    const char* response_template;
    char* response_message;
};





typedef struct LSD_Vec2i LSD_Vec2i;
typedef struct LSD_Vec2f LSD_Vec2f;
typedef struct LSD_Vec2d LSD_Vec2d;
typedef enum LSD_Log_type LSD_Log_type;
typedef enum LSD_Log_level LSD_Log_level;
typedef struct LSD_File LSD_File;
typedef struct LSD_Thread LSD_Thread;
typedef struct LSD_Delta LSD_Delta;
typedef struct LSD_WebServer LSD_WebServer;


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

/* Erstellt einen neuen Vektor mit entsprechenden typen */
LSD_Vec2i LSD_Vec_new_int(int x, int y);
LSD_Vec2f LSD_Vec_new_float(float x, float y);
LSD_Vec2d LSD_Vec_new_double(double x, double y);

/* Entfernt ein Objekt aus einem Array */
void LSD_Math_remove_object_from_array(int arr[], int* max_arr, int* index_to_be_removed);

/* Returnt eine Zahl, die vorher noch nicht verwendet wurde aus einem Array */
int LSD_Math_get_id_from_array(int black_list[], int* black_fill, int max_id);

/* Streckt / Quetscht eine Zahl mit angegebenem Zahlenbereich auf einen neuen Zahlenbereich */
float LSD_Math_map(float num, float min1, float max1, float min2, float max2);

/* Setzt die Working-Directory um setback Chars zurück */
void LSD_File_path_prepare(char* argv[], int setback);

/* Bereitet einen Webserver vor, um später mit LSD_WebServer_serve_while() zu serven */
LSD_WebServer* LSD_WebServer_open(const char* dp,void (*GET)(struct LSD_WebServer* server),void (*POST)(struct LSD_WebServer* server));

/* Beendet jegliche Kommunikation mit Klienten und free'd den Speicherplatz des Servers */
void LSD_WebServer_close(LSD_WebServer* server);

/* Standart POST-, bzw GET-Handler für einen Server der keine speziellen Versionen davon braucht */
void LSD_WebServer_STD_GET(LSD_WebServer* server);
void LSD_WebServer_STD_POST(LSD_WebServer* server);

/* Lässt den Webserver so lange serven, bis der Boolean nicht mehr war ist */
void LSD_WebServer_serve_while(LSD_WebServer* server, bool* running);

/* Lässt den Webserver eine POST-, bzw. GET-Request bearbeiten */
void LSD_WebServer_serve_once(LSD_WebServer* server);

#endif
