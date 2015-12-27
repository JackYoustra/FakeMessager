#include <pebble.h>
#include "messenger.h"
#include "time_setter.h"
#include "networking.h"

#define NUM_OF_MESSAGES 3

static Window* window;
static SimpleMenuLayer* messageSelectMenuLayer;
static const char* messages[NUM_OF_MESSAGES];

static void onTextButtonPressed(int index, void* context){
  // IT IS time_setter'S RESPONSIBILITY TO FREE THIS WHEN IT EXITS!!!
  NetworkPacket_t *partialPacket = malloc(sizeof(NetworkPacket_t));
  partialPacket->isCall = 0;
  partialPacket->messageIndex = (uint8_t)index;
}

static void menuInit(){
  // create messages
  messages[0] = "I need a quick decision on this...";
  messages[1] = "Hey baby, what's up?";
  messages[2] = "Johnny broke his leg! We need you to come pick him up.";
  
  // create items
  SimpleMenuItem menuItems[2];
  for(int i = 0; i < NUM_OF_MESSAGES; i++){
    menuItems[i].title = messages[i];
    menuItems[i].subtitle = NULL;
    menuItems[i].icon = NULL;
    menuItems[i].callback = onTextButtonPressed;
  }
  
  SimpleMenuSection menuSections[1];
  menuSections[0].title = NULL;
  menuSections[0].items = menuItems;
  menuSections[0].num_items = sizeof(menuItems)/sizeof(SimpleMenuItem);
  
  messageSelectMenuLayer = simple_menu_layer_create(GRect(0, 0, 144, 168), // dimensions
                                                             window, // window
                                                             menuSections, // pointer to sections
                                                             1, // number of sections
                                                             NULL); // pointer to app-specific data passed into callback
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(messageSelectMenuLayer));
}


// window creation code

static void window_load(Window *window){
  //We will add the creation of the Window's elements here soon!
  // setup the menu
  menuInit();
}

// use this function to free memory
static void window_unload(Window *window){
  //We will safely destroy the Window's elements here!
  simple_menu_layer_destroy(messageSelectMenuLayer);

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
 
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void openMessengerWindow(){
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    // delegate window functions / hooks
    .load = window_load,
    .unload = window_unload,
  });
  
  // make app move to foreground
  window_set_click_config_provider(window, click_config_provider);

  window_stack_push(window, true);
}