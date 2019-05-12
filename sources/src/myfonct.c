#include <stdio.h>
#include <assert.h>
#include <map.h>
#include <player.h>
#include <constant.h>
#include <game.h>
#include <sprite.h>
#include <assert.h>
#include <time.h>
#include <save_load.h>

#include <misc.h>
#include <window.h>
void resize_windows(struct map* map) {
  window_create(SIZE_BLOC*map_get_width(map)
              ,SIZE_BLOC*map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);

}
int boxe_move(enum direction direction,struct map* map,int x,int y){
  int x1=x, y1=y;
  enum bonus_type bonus_type = map_get_bonus_type(map,x,y);

  switch (direction) {
    case SOUTH:
      y1=y+1;
    break;

    case NORTH:
      y1=y-1;
    break;

    case WEST:
      x1=x-1;
    break;

    case EAST:
      x1=x+1;
    break;
  }
  if (!map_is_inside(map, x1, y1))
		return 0;

  if(  map_get_cell_type(map,x1,y1)==CELL_EMPTY ){
    map_set_cell_type(map, x1, y1, map_get_cell_type(map,x,y));
    map_set_bonus_type(map,x1,y1,bonus_type);
    map_set_cell_type(map, x, y, CELL_EMPTY);
    return 1;
  }
  return 0;
}

void cell_bonus_move(struct map* map,struct player *player,int x,int y){
  enum bonus_type bonus_type= map_get_bonus_type(map,x,y);
  switch (bonus_type) {
    case BONUS_BOMB_RANGE_INC:
    if (player_get_range(player)<9)
      player_inc_range(player);
    break;

    case BONUS_BOMB_RANGE_DEC:
    if(player_get_range(player)>1)
      player_dec_range(player);
    break;

    case BONUS_BOMB_NB_INC:
    if (player_get_max_bomb(player)<9)
      player_inc_max_bomb(player);
    if(player_get_nb_bomb(player)<9)
      player_inc_nb_bomb(player);
    break;

    case BONUS_BOMB_NB_DEC:
      if(player_get_max_bomb(player)>1){
        player_dec_max_bomb(player);
        if(player_get_nb_bomb(player)>player_get_max_bomb(player)){
          // si au moins une bomb a ete posé
          if (player_get_nb_bomb(player)>0)
            player_dec_nb_bomb(player);

          }
      }
    break;

    case BONUS_LIFE:
      if (player_get_life(player)<9) {
        player_inc_life(player);
      }
    break;

    case BONUS_MONSTER:
    break;
  }
}

void door_move(struct game* game){
  struct map* map = game_get_current_map(game);
  struct player* player = game_get_player(game);

  int x = player_get_x(player);
  int y = player_get_y(player);

  if (map_get_cell_type(map,x,y) == CELL_DOOR) {
    enum door_type door_type = map_get_door_type(map,x,y);
    if( (door_type%2) == 1 ){ //si the door is open
      game_set_current_lvl(game,(door_type>>1));
      //    player_set_position(player,0,0);
    }
  }
}

void open_the_door(struct game* game) {
  struct map* map = game_get_current_map(game);
  struct player* player = game_get_player(game);

  int x = player_get_x(player);
  int y = player_get_y(player);
  enum door_type door_type = map_get_door_type(map,x,y);
  if(player_get_key(player)>0){
      map_open_the_door(map,x,y);
      player_dec_key(player);
      game_set_current_lvl(game,(door_type>>1));
  }
}

void pause_display() {
  window_create(GAME_PAUSE_WIDTH,GAME_PAUSE_HEIGHT);
  window_display_image(sprite_get_pause_menu(),
  0* SIZE_BLOC,0 * SIZE_BLOC);
}

int pause() {
  SDL_Event event;
  //window_clear();
  pause_display();
  window_refresh();
  while(SDL_WaitEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        return 1;
      case SDL_KEYDOWN:
      //end of pause
        switch (event.key.keysym.sym) {
          case  SDLK_p:
          return 0;
          case  SDLK_s:

          return 2;

          case SDLK_ESCAPE:
    			return 1;

          default:
          break;
        }
      break;
      default:
      break;
    }
  }
return 0;
}

void game_over_display() {
  window_create(GAME_OVER_WIDTH,GAME_OVER_HEIGHT);
  window_display_image(sprite_get_game_over(),
  0* SIZE_BLOC,0 * SIZE_BLOC);
}

int gover(struct game** game) {
  //return 0 = new game
  //return 1 = EXIT
  //return 2 = load
  SDL_Event event;
  //window_clear();
  game_over_display();
  window_refresh();
  game_free(*game);
  while(SDL_WaitEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        return 1;

      case SDL_KEYDOWN:
      //end of pause
        switch (event.key.keysym.sym) {
          case  SDLK_n:
          (*game)=game_new();
          resize_windows(game_get_current_map(*game));
          return 0;
          break;

          case  SDLK_l:
          (*game)=load_save("./save/saved.txt");
          resize_windows(game_get_current_map(*game));
          return 0;
          break;

          case SDLK_ESCAPE:
    			return 1;

          default:
          break;
        }
      break;
      default:
      break;
    }
  }

  return 0;
}

void start_menu_display() {
  window_create(GAME_START_WIDTH,GAME_START_HEIGHT);
  window_display_image(sprite_get_start_menu(),
  0* SIZE_BLOC,0 * SIZE_BLOC);
}

int start_game(struct game** game) {
  SDL_Event event;
  //window_clear();
  start_menu_display();
  window_refresh();
  while(SDL_WaitEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        return 1;
      case SDL_KEYDOWN:
      //end of pause
        switch (event.key.keysym.sym) {
          case  SDLK_n:
          window_create(SIZE_BLOC*map_get_width(game_get_current_map(*game))
  										 ,SIZE_BLOC*map_get_height(game_get_current_map(*game)) + BANNER_HEIGHT + LINE_HEIGHT);
          return 0;
          break;

          case  SDLK_l:
          game_free(*game);
          *game =  load_save("./save/saved.txt");
          window_create(SIZE_BLOC*map_get_width(game_get_current_map(*game))
  										 ,SIZE_BLOC*map_get_height(game_get_current_map(*game)) + BANNER_HEIGHT + LINE_HEIGHT);

          return 0;
          break;

          case SDLK_ESCAPE:
    			return 1;

          default:
          break;
        }
      break;
      default:
      break;
    }
  }
return 0;
}

void you_win_display() {
  window_create(GAME_OVER_WIDTH,GAME_OVER_HEIGHT);
  window_display_image(sprite_get_win_menu(),
  0* SIZE_BLOC,0 * SIZE_BLOC);
}

int you_win(struct game** game) {
  //return 0 = new game
  //return 1 = EXIT
  //return 2 = load
  SDL_Event event;
  //window_clear();
  you_win_display();
  window_refresh();
  game_free(*game);
  while(SDL_WaitEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        return 1;

      case SDL_KEYDOWN:
      //end of pause
        switch (event.key.keysym.sym) {
          case  SDLK_n:
          (*game)=game_new();
          resize_windows(game_get_current_map(*game));
          return 0;
          break;

          case  SDLK_l:
          (*game)=load_save("./save/saved.txt");
          resize_windows(game_get_current_map(*game));
          return 0;
          break;

          case SDLK_ESCAPE:
    			return 1;

          default:
          break;
        }
      break;
      default:
      break;
    }
  }

  return 0;
}
