#include "game.h"
#include <stdio.h>
#include <time.h>
#include <game.h>


BS_INLINE INT bs_get_ship_length(INT type) {
  switch (type) {
    case BS_SHIP_DESTROYER:   return 2;
    case BS_SHIP_CRUISER:     return 3;
    case BS_SHIP_SUBMARINE:   return 3;
    case BS_SHIP_BATTLESHIP:  return 4;
    case BS_SHIP_CARRIER:     return 5;
  }

  return 0;
}

INT bs_validate_position(BSShip ship, BSGrid grid, int orientation, int* x, int* y) {
  int _x = *x, _y = *y;
  switch (orientation) {
    case BS_SHIP_VERTICAL:
      _y+=ship->length-1;
      break;
    case BS_SHIP_HORIZONTAL:
      _x+=ship->length-1;
      break;
    case BS_SHIP_REVERSE_VERTICAL:
      _y-=ship->length-1;
      break;
    case BS_SHIP_REVERSE_HORIZONTAL:
      _x-=ship->length-1;
      break;
  }

  if ((_x < 0) || (_y < 0)) {
    return BS_FAILED;
  }

  // Posicionar el barco en el tablero
  INT max_x = max(_x, *x), min_x = min(_x, *x);
  INT max_y = max(_y, *y), min_y = min(_y, *y);
  INT collitions = 0;

  // detect collitions
  for (int i = min_x; i <= max_x; ++i) {
    for(int j = min_y; j <= max_y; ++j) {
      if (grid[j][i].state == BS_CELL_IS_SHIP) {
        ++collitions;
      }
    }
  }

  if (collitions) return BS_FAILED;

  for (int i = min_x; i <= max_x; ++i) {
    for(int j = min_y; j <= max_y; ++j) {
      grid[j][i].ship_id = ship->type;
      grid[j][i].state = BS_CELL_IS_SHIP;
    }
  }

  return BS_OK;
}

INT bs_game_init() {
  if (bs_enable_utf8()) {
    printf("ERROR: la terminal no soporta escapes ANSI ni utf8...\n");
    return BS_FAILED;
  }

  return BS_OK;
}

void bs_draw_grid(BSGrid grid, U32 match_state) {

  printf("   X 0   1   2   3   4   5   6   7   8   9\n Y ╔═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╗\n");

  for (int i = 0; i < 10; ++i) {
    printf(" %i ║", i);

    for (int j = 0; j < 10; ++j) {
      char c = ' ';

      if ((match_state & BS_CELL_IS_SHIP_HIT) && (grid[i][j].state == BS_CELL_IS_SHIP_HIT)) {
        c = 'X';
      }
      
      if ((match_state & BS_CELL_IS_WATER_HIT) && (grid[i][j].state == BS_CELL_IS_WATER_HIT)) {
        c = 'O';
      }
      
      if ((match_state & BS_CELL_IS_SHIP) && (grid[i][j].state == BS_CELL_IS_SHIP)) {
        c = '*';
      }

      printf(" %c ║", c);
    }

    if (i < 9)
      printf("\n   ╠═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╣\n");
  }

  printf( "\n   ╚═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╝\n");
}

void bs_get_point(int* x, int* y) {
  printf("Posición: \n");
  printf("  X : ");
  scanf("%i", x);
  while (getchar() != '\n');
  printf("  Y : ");
  scanf("%i", y);
  while (getchar() != '\n');
}

INT bs_select_orientation(void) {
  const char* option_ships = 
    "  1. Vertical\n"\
    "  2. Vertical Reversa\n"\
    "  3. Horizontal\n"\
    "  4. Horizontal Reversa\n";
  INT index = -1;

  do {
    printf("Orientación: \n%s  : ", option_ships);
    scanf("%i", &index);
    while (getchar() != '\n');
  } while(index < 1 || index > 4);

  return index-1;
}

void bs_position_ships(BSGameContex gamectx, BSPlayerData player) {
  INT x,y;
  INT orientation;

  for (INT i = 0; i < 5; ++i) {
    do {
      bs_clear_console();
      printf("Posiciona los barcos el jugador \"%s\".\n", player->name);
      printf("Ubicar el %s en la tabla.\n\n", gamectx->ships[i].name);
      bs_draw_grid(player->grid, BS_CELL_IS_SHIP);
      bs_get_point(&x, &y);
      orientation = bs_select_orientation();
    } while(bs_validate_position(&gamectx->ships[i], player->grid, orientation, &x, &y));

    player->health_ships[i] = gamectx->ships[i].length;
  }

  printf("Enter para continuar...\n");
  getchar();
}

void bs_new_game(BSGameContex gamectx) {
  BS_ZEROMEMORY(gamectx, sizeof(bs_gamecontext_t));
  const char* names[] = {
    "Destructor",
    "Submarino",
    "Crucero",
    "Acorazado",
    "Portaaviones"
  };

  for (INT i = 0; i < 5; ++i) {
    gamectx->ships[i] = (bs_ship_t){
      .type = i+1,
      .length = bs_get_ship_length(i+1),
      .name = names[i]
    };
  }

  bs_clear_console();
  printf("%s", BS_TEXT_INTRO);

  printf("Player1 name: ");
  fgets(gamectx->player1.name, MAX_LEN_PLAYERNAME, stdin);
  printf("Player2 name: ");
  fgets(gamectx->player2.name, MAX_LEN_PLAYERNAME, stdin);

  gamectx->player1.name[strcspn(gamectx->player1.name, "\n")] = 0;
  gamectx->player2.name[strcspn(gamectx->player2.name, "\n")] = 0;

  gamectx->player1.ship_alive = gamectx->player2.ship_alive = 5;
}

void bs_end_game(BSGameContex gamectx, const char* winner) {
  const char* end_game_msg[] = {
    "   _________  __  ___  ____  __   ___     ___  ___   ___  _____________  ___ \n",
    "  / __/  _/ |/ / / _ \\/ __/ / /  / _ |   / _ \\/ _ | / _ \\/_  __/  _/ _ \\/ _ |\n",
    " / _/_/ //    / / // / _/  / /__/ __ |  / ___/ __ |/ , _/ / / _/ // // / __ |\n",
    "/_/ /___/_/|_/ /____/___/ /____/_/ |_| /_/  /_/ |_/_/|_| /_/ /___/____/_/ |_|\n",
    "                                                                             \n"
  };

  for (int i = 79; i >= 0; --i) {
    bs_clear_console();
    for (int j = 0; j < 5; ++j) {
      printf("%s", &end_game_msg[j][i]);
    }
    bs_sleep(10);
  }

  printf("Ganador: %s\n\n", winner);
  printf("Nueva partida(Y,n): ");
  char buffer = getchar();

  gamectx->replay = (tolower(buffer) == 'n') ? 0 : 1;
}

void bs_verify_hit(BSPlayerData p, int x, int y) {
  BSCell cell  = &p->grid[y][x];

  if (cell->state == BS_CELL_IS_SHIP_HIT)  return;
  if (cell->state == BS_CELL_IS_WATER_HIT) return;

  if (cell->state == BS_CELL_IS_WATER) {
    cell->state = BS_CELL_IS_WATER_HIT;
    return;
  }

  cell->state = BS_CELL_IS_SHIP_HIT;
  INT index = cell->ship_id-1;

  if((--p->health_ships[index]) == 0) {
    --p->ship_alive;
  }
}

int bs_fire(BSPlayerData p1, BSPlayerData p2) {
  printf("Turno de \"%s\"\n", p1->name);
  bs_draw_grid(p2->grid, BS_CELL_IS_SHIP_HIT|BS_CELL_IS_WATER_HIT);

  int x, y;
  printf("Disparar a ");
  bs_get_point(&x, &y);

  bs_clear_console();
  bs_verify_hit(p2, x, y);
  bs_draw_grid(p2->grid, BS_CELL_IS_SHIP_HIT|BS_CELL_IS_WATER_HIT);

  printf("Enter para continuar...\n");
  getchar();

  bs_clear_console();

  return p2->ship_alive == 0;
}

void bs_game_turn(BSGameContex gamectx) {
  bs_clear_console();

  srand(time(NULL));
  int rand_boolean = (rand() % 2) == 1;

  gamectx->first  = (rand_boolean) ? &gamectx->player1 : &gamectx->player2;
  gamectx->second = (rand_boolean) ? &gamectx->player2 : &gamectx->player1;

  do {
    if (bs_fire(gamectx->first, gamectx->second)) {
      gamectx->winner = gamectx->first->name;
      break;
    }
    if (bs_fire(gamectx->second, gamectx->first)) {
      gamectx->winner = gamectx->second->name;
      break;
    }
  } while(1);

  gamectx->is_end = 1;
}

int main(int argc MAYBE_UNUSED, char** argv MAYBE_UNUSED) {
  bs_gamecontext_t gamectx = {0};

  if (bs_game_init()) {
    printf("ERROR: Falló al inicializar\n");
  }

  do {
    bs_new_game(&gamectx);

    bs_position_ships(&gamectx, &gamectx.player1);
    bs_position_ships(&gamectx, &gamectx.player2);

    bs_game_turn(&gamectx);

    bs_end_game(&gamectx, gamectx.winner);

  } while(gamectx.replay);

  return 0;
}
