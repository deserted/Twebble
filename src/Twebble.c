#include <pebble.h>
#include "consts.h"
#include "tweet_cell.h"
#include "twitter_keys.h"
//TIMELINE VIEW

MenuLayer *tlMenu;
Tweet tl_tweets[MAX_TWEETS];
int i = 0;
int r = 0;

static uint16_t tl_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t tl_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return (i > 0) ? i : 1;
}

static int16_t tl_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}
static int16_t tl_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

  if (i != 0 && tl_tweets[cell_index->row].text){
      return height_for_tweet_cell(tl_tweets[cell_index->row].text);
  }
  return DEF_TEXT;
  
}
static void tl_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  if (i == 0){

    menu_cell_basic_draw(ctx, cell_layer, "Loading tweets...", NULL, NULL);
  }
  else {

    Tweet tweet = tl_tweets[cell_index->row];
    tweet_cell_for_tweet(tweet.text, tweet.screen_name, cell_layer, ctx);
  }
}

static void tl_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, titles[r]);
}
void tl_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

  
}

void getTL(){

    DictionaryIterator *it;
    app_message_outbox_begin(&it);
    Tuplet tuplet = TupletInteger(KEY_RESOURCE, r);
    dict_write_tuplet(it, &tuplet);
    dict_write_end(it);
    app_message_outbox_send();
}

static void tl_load(Window *tl) {

  Layer *window_layer = window_get_root_layer(tl);
  GRect bounds = layer_get_bounds(window_layer);

  tlMenu = menu_layer_create(bounds);

  menu_layer_set_callbacks(tlMenu, NULL, (MenuLayerCallbacks){
    .get_num_sections = tl_get_num_sections_callback,
    .get_num_rows = tl_get_num_rows_callback,
    .get_header_height = tl_get_header_height_callback,
    .draw_header = tl_draw_header_callback,
    .get_cell_height = tl_get_cell_height_callback,
    .draw_row = tl_draw_row_callback,
    .select_click = tl_select_callback,
  });
  menu_layer_set_click_config_onto_window(tlMenu, tl);
  layer_add_child(window_layer, menu_layer_get_layer(tlMenu));
  getTL();

}

static void tl_unload(Window *tl){

  menu_layer_destroy(tlMenu);
}
void tl_init(){

  Window *tl = window_create();
  window_set_window_handlers(tl, (WindowHandlers){
    .load = tl_load,
    .unload = tl_unload,
  });
  window_stack_push(tl, true);
}

//ROOT VIEW
static Window *window;
static MenuLayer *menu_layer;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return ITEMS;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  menu_cell_basic_draw(ctx, cell_layer, titles[cell_index->row], NULL, NULL);
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Twebble");
}
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

  int row = cell_index->row;
  r = row;
  i = 0;

  switch (row){

    case 0:
      tl_init();
      break;
    case 1:
      tl_init();
      break;
    default:
      break;
  }
}

static void window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  menu_layer_set_click_config_onto_window(menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));

  if (!bluetooth_connection_service_peek()){


  }
}


static void window_unload(Window *window) {
  menu_layer_destroy(menu_layer);
}


//MAIN STUFF

static void msg_received(DictionaryIterator *iter, void *context) {
  
  Tweet t;
  Tuple *resource = dict_find(iter, KEY_RESOURCE);
  if (resource){

    switch (resource->value->int8){

      case RESOURCE_TL:
        if (dict_find(iter, KEY_START)) i = 0;

        APP_LOG(APP_LOG_LEVEL_DEBUG, "TL");
        
        strncpy(t.text, dict_find(iter, KEY_TEXT)->value->cstring, 170);
        strncpy(t.screen_name, dict_find(iter, KEY_SCREEN_NAME)->value->cstring, 20);
        strncpy(t.name, dict_find(iter, KEY_NAME)->value->cstring, 30);
        tl_tweets[i] = t;
        i++;

        if (dict_find(iter, KEY_END)){

          menu_layer_reload_data(tlMenu);
          layer_mark_dirty(menu_layer_get_layer(tlMenu));
        }
        break;
      case RESOURCE_MENTIONS:
        if (dict_find(iter, KEY_START)) i = 0;

        APP_LOG(APP_LOG_LEVEL_DEBUG, "Mn");
        
        strncpy(t.text, dict_find(iter, KEY_TEXT)->value->cstring, 170);
        strncpy(t.screen_name, dict_find(iter, KEY_SCREEN_NAME)->value->cstring, 20);
        strncpy(t.name, dict_find(iter, KEY_NAME)->value->cstring, 30);
        tl_tweets[i] = t;
        i++;

        if (dict_find(iter, KEY_END)){

          menu_layer_reload_data(tlMenu);
        }
        break;
      default:
        break;
    }
  }
}

void sendKeys(){

    char key[] = CONSUMER_KEY;
    char secret[] = CONSUMER_SECRET;
    DictionaryIterator *it;
    app_message_outbox_begin(&it);
    Tuplet tuplet = TupletInteger(KEY_RESOURCE, RESOURCE_KEYS);
    dict_write_tuplet(it, &tuplet);
    
    dict_write_cstring(it, KEY_KEY, key);
    dict_write_cstring(it, KEY_SECRET, secret);

    dict_write_end(it);
    app_message_outbox_send();
}

static void init(void) {
  window = window_create();
  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;

  window_stack_push(window, animated);

  app_message_register_inbox_received(msg_received);
  app_message_open(512, 512);
  sendKeys();
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, max inbox: %i, max outbox: %i", (int)app_message_inbox_size_maximum(), (int)app_message_outbox_size_maximum());

  app_event_loop();
  deinit();
}
