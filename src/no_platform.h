
#define INPUT_UP                                k.W
#define INPUT_DOWN                              k.S
#define INPUT_LEFT                              k.A
#define INPUT_RIGHT                             k.D
#define INPUT_JUMP                              k.SPACE
#define INPUT_ACTION_1                          k.O
#define INPUT_ACTION_2                          k.P

#define LIB_PLATFORM_TEXT_TO_SURFACE(X)         TTF_RenderText_Solid(font,X,color)
#define LIB_PLATFORM_TEXT_TO_TEXTURE(X)         SDL_CreateTextureFromSurface(renderer,TTF_RenderText_Solid(font,X,color))

#define LIB_PLATFORM_INIT()                     prepare_path(argv); gettimeofday(&begin, 0); init_SDL(); init_keys(&k)
#define LIB_PLATFORM_DEINIT()                   quit_SDL()
#define LIB_PLATFORM_UPDATE()                   update_keys(); draw_black_bars(); SDL_RenderPresent(renderer); SDL_RenderClear(renderer)

#define LIB_PLATFORM_COLOR_MOD(X,R,G,B)         SDL_SetTextureColorMod(X,R,G,B)
#define LIB_PLATFORM_ALPHA_MOD(X,Y)             SDL_SetTextureAlphaMod(X,Y)
#define LIB_PLATFORM_DRAW_TEXTURE(X,Y,Z)        SDL_RenderCopy(renderer,X,Y,Z)
#define LIB_PLATFORM_FILL_RECT(X)               SDL_RenderFillRect(renderer,&X)
#define LIB_PLATFORM_DRAW_RECT(X)               SDL_RenderDrawRect(renderer,&X)
#define LIB_PLATFORM_SET_DRAW_COLOR(R,G,B,A)    SDL_SetRenderDrawColor(renderer,R,G,B,A)

#define LIB_PLATFORM_LOAD_IMG(X)                IMG_Load(X)
#define LIB_PLATFORM_SURFACE_TO_TEXTURE(X)      SDL_CreateTextureFromSurface(renderer,X)
#define LIB_PLATFORM_TEXTURE_DESTROY(X)         SDL_DestroyTexture(X)


#define LIB_PLATFORM_SFX                        Mix_Chunk*
#define LIB_PLATFROM_MUSIC                      Mix_Music*
#define LIB_PLATFORM_COLOR                      SDL_Color
#define LIB_PLATFORM_RECTANGLE                  SDL_Rect
#define LIB_PLATFORM_SURFACE                    SDL_Surface*
#define LIB_PLATFORM_TEXTURE                    SDL_Texture*

//Variablen, die auf jeder Platform benötigt werden
double WORLD_TO_SCREEN_X = 1000;
double WORLD_TO_SCREEN_Y = 1000;
double DRAW_OFFSET = 0;
double STRETCH_WIDTH = 1;
double STRETCH_HEIGHT = 1;

double delta = 0.0f;
bool frozen = false;

struct timeval begin, end;

bool keys[322] = {0};
bool old_keys[322] = {0};
vec2i mouse_pos;
bool mouse_left;
bool mouse_right;
bool last_mouse_left;
bool last_mouse_right;
bool show_sys_cursor = true;
vec2f camera = {0,0};
bool fullscreen = false;
bool running;


//Funktionen, die auf jeder Platform benötigt werden

void prepare_path(char* argv[])
{
    #define CHARS_TIL_ROOT_OF_PROJ 6
    char path_save[1024];
    char cwd[1024];
    char* p;
    if(!(p = strrchr(argv[0], '/')))
        getcwd(cwd, sizeof(cwd));
    else
    {
        *p = '\0';
        getcwd(path_save, sizeof(path_save));
        chdir(argv[0]);
        getcwd(cwd, sizeof(cwd));
        chdir(path_save);
    }
    cwd[strlen(cwd)- CHARS_TIL_ROOT_OF_PROJ] = '\0';
    chdir(cwd);
}

void draw_black_bars()
{
#ifdef BLACK_BARS
    if (!fullscreen)
        return;
    LIB_PLATFORM_RECTANGLE r;
    LIB_PLATFORM_RECTANGLE r2;
    
    r.x = 0;
    r.y = 0;
    r.w = DRAW_OFFSET;
    r.h = WIN_SIZE.h;
    
    r2.x = WIN_SIZE.w - DRAW_OFFSET;
    r2.y = 0;
    r2.w = DRAW_OFFSET;
    r2.h = WIN_SIZE.h;
    
    LIB_PLATFORM_SET_DRAW_COLOR(0,0,0,255);
    
    LIB_PLATFORM_FILL_RECT(&r);
    LIB_PLATFORM_FILL_RECT(&r2);
    
    LIB_PLATFORM_DRAW_RECT(&r);
    LIB_PLATFORM_DRAW_RECT(&r2);
    
    LIB_PLATFORM_SET_DRAW_COLOR(std.r,std.g,std.b,std.a);
#endif
}

/* !!!NUR FÜR UI-ELEMENTE!!! */
vec2i get_screen_size()
{
    vec2i size = {WORLD_TO_SCREEN_X,WORLD_TO_SCREEN_Y};
    return size;
}

void tick()
{
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    delta = seconds + microseconds*1e-6;
    gettimeofday(&begin, 0);
    if (frozen)
    {
        frozen = false;
        delta = 0.0f;
    }
}

// Diese Funktionen müssen von den jeweiligen Wrappern vervollständigt werden!
void update_keys();
void init_keys();
void change_res(int* vr, int* hr);
void toggle_fullscreen();
void draw_black_bars(); //Das hier ist je nach Platform möglicherweise unnötig, wenn sie nur ein Bildschirm-Format unterstützen sollte

bool isDown(int key);
bool isPressed(int key);
bool isReleased(int key);