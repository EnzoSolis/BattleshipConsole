#ifndef __BS_GAME_H__
#define __BS_GAME_H__

#include <types.h>

#ifdef _WIN32

#include <windows.h>

BS_INLINE int bs_enable_utf8(void) {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode;
  
  if (hout == INVALID_HANDLE_VALUE) return BS_FAILED;
  if (!GetConsoleMode(hout, &mode)) return BS_FAILED;

  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  return !SetConsoleMode(hout, mode);
}

BS_INLINE void bs_clear_console(void) {
  system("cls");
}

BS_INLINE void bs_sleep(INT milliseconds) {
  Sleep(milliseconds);
}

#elif defined(__linux__)

#include <unistd.h>

BS_INLINE void bs_sleep(U32 milliseconds) {
  usleep(milliseconds * 1000);
}

BS_INLINE int bs_enable_utf8(void ) {
  return isatty(STDOUT_FILENO);
}

BS_INLINE void bs_clear_console() {
  system("clear");
}

#endif

#define BS_TEXT_INTRO ""\
"   ___  ___ ______________   __________ _________ \n"\
"  / _ )/ _ /_  __/_  __/ /  / __/ __/ // /  _/ _ \\\n"\
" / _  / __ |/ /   / / / /__/ _/_\\ \\/ _  // // ___/\n"\
"/____/_/ |_/_/   /_/ /____/___/___/_//_/___/_/    \n"\
"                                                  \n"\

enum BS_CELL_STATE {
  BS_CELL_IS_WATER = 0x0,
  BS_CELL_IS_SHIP = 0x1,
  BS_CELL_IS_SHIP_HIT = 0x02,
  BS_CELL_IS_WATER_HIT = 0x04
};

enum BS_SHIP_TYPE {
  BS_NO_SHIP,
  BS_SHIP_DESTROYER,
  BS_SHIP_SUBMARINE,
  BS_SHIP_CRUISER,
  BS_SHIP_BATTLESHIP,
  BS_SHIP_CARRIER
};

enum BS_SHIP_ORIENTATION {
  BS_SHIP_VERTICAL,
  BS_SHIP_REVERSE_VERTICAL,
  BS_SHIP_HORIZONTAL,
  BS_SHIP_REVERSE_HORIZONTAL
};

typedef struct bs_cell_t {
  U32 ship_id;
  U32 state;
} bs_cell_t, *BSCell;

typedef bs_cell_t BSGrid[10][10];

typedef struct bs_ship_t {
  INT type;
  INT length;
  const char* name;
} bs_ship_t, *BSShip;

#define MAX_LEN_PLAYERNAME 24

typedef struct bs_player_data_t {
  char name[MAX_LEN_PLAYERNAME];
  BSGrid grid;
  int health_ships[5];
  int ship_alive;
} bs_player_data_t, *BSPlayerData;

typedef struct bs_gamecontext_t {
  bs_player_data_t player1;
  bs_player_data_t player2;
  BSPlayerData first, second;
  bs_ship_t ships[5];
  const char* winner;
  int replay;
  int is_end;
} bs_gamecontext_t, *BSGameContex;

#endif//__BS_GAME_H__