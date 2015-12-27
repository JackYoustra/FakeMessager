#include <pebble.h>
#include "networking.h"
#include "messenger.h"

static Window *mainWindow;
static SimpleMenuLayer *notificationTypeSelectMenuLayer;

static void onMenuButtonPressed(int index, void* context){
  APP_LOG(APP_LOG_LEVEL_INFO, "Select button pressed");
  switch(index){
    case 0:{
      // call button pressed
      
    }
    case 1:{
      // message button pressed
      APP_LOG(APP_LOG_LEVEL_INFO, "Opening messanger window");
      openMessengerWindow();
    }
    default:{
      // should never happen
    }
  }
}

static void menuInit(Window *window){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Initializing window");
  // create items
  SimpleMenuItem menuItems[2];
  menuItems[0] = (SimpleMenuItem){
    .title = "Call",
    .callback = onMenuButtonPressed
  };
  
  menuItems[1] = (SimpleMenuItem){
    .title = "Message",
    .callback = onMenuButtonPressed
  };
  
  SimpleMenuSection menuSections[1];
  menuSections[0] = (SimpleMenuSection){
    .title = NULL,
    .items = menuItems,
    .num_items = 2
  };
  APP_LOG(APP_LOG_LEVEL_DEBUG, "creating full menu");
  notificationTypeSelectMenuLayer = simple_menu_layer_create(layer_get_bounds(window_get_root_layer(window)), // dimensions
                                                             window, // window
                                                             menuSections, // pointer to sections
                                                             1, // number of sections
                                                             NULL); // pointer to app-specific data passed into callback
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(notificationTypeSelectMenuLayer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu created");
}

static void window_load(Window *window){
  //We will add the creation of the Window's elements here soon!
  // setup the menu
  menuInit(window);
}

// use this function to free memory
static void window_unload(Window *window){
  //We will safely destroy the Window's elements here!
  simple_menu_layer_destroy(notificationTypeSelectMenuLayer);
}

static void ringPhone(){
  // Prepare dictionary
  DictionaryIterator *iterator;
  app_message_outbox_begin(&iterator);
  // Write data
  int key = 0;
  unsigned int value = 650;
  dict_write_int(iterator, key, &value, sizeof(unsigned int), false); // last parameter is for signed
  app_message_outbox_send();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {  
  ringPhone();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void init(){
  //Create app elements here
  // make main window
  mainWindow = window_create();
  window_set_window_handlers(mainWindow, (WindowHandlers) {
    // delegate window functions / hooks
    .load = window_load,
    .unload = window_unload,
  });

  // setup network stuff
  networkInit();
  
  // make app move to foreground
  //window_set_click_config_provider(mainWindow, click_config_provider);

  window_stack_push(mainWindow, true);
}

void deinit(){
  //We will safely destroy the Window's elements here!
  window_destroy(mainWindow);
}


int main(){
  init();
  app_event_loop();
  deinit();
}