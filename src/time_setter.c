#include <pebble.h>
#include "time_setter.h"
#include "networking.h"

#define NUM_OF_TIMES 5

static Window* window;
static SimpleMenuLayer* timeSelectMenuLayer;
static const char* times[NUM_OF_TIMES];
static NetworkPacket_t* packet;

static void onTimeButtonPressed(int index, void* context){
  // fill the remainder of the network packet
  packet->durationIndex = (uint8_t)index;
  // send packet
  sendPacket(packet);
  free(packet); // not needed anymore so release
  // pop to the top
  window_stack_pop(0);
  window_stack_pop(1); // this one is animated
}

static void menuInit(){
  // create messages, must change above
  times[0] = "One second";
  times[1] = "Fifteen seconds";
  times[2] = "Thirty seconds";
  times[3] = "One minute";
  times[4] = "Five minues";
  
  // create items
  SimpleMenuItem menuItems[NUM_OF_TIMES];
  for(int i = 0; i < NUM_OF_TIMES; i++){
    menuItems[i].title = times[i];
    menuItems[i].subtitle = NULL;
    menuItems[i].icon = NULL;
    menuItems[i].callback = onTimeButtonPressed;
  }
  
  SimpleMenuSection menuSections[1];
  menuSections[0].title = NULL;
  menuSections[0].items = menuItems;
  menuSections[0].num_items = sizeof(menuItems)/sizeof(SimpleMenuItem);
  
  timeSelectMenuLayer = simple_menu_layer_create(GRect(0, 0, 144, 168), // dimensions
                                                             window, // window
                                                             menuSections, // pointer to sections
                                                             sizeof(menuSections)/sizeof(SimpleMenuSection), // number of sections
                                                             NULL); // pointer to app-specific data passed into callback
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(timeSelectMenuLayer));
}


// window creation code

static void window_load(Window *window){
  //We will add the creation of the Window's elements here soon!
  // setup the menu
  menuInit();
}

// use this function to free memory
static void window_unload(Window *window){
  //We will safely destroy the Window's elements here
  simple_menu_layer_destroy(timeSelectMenuLayer);

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
 
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void createTimeSetterWindow(NetworkPacket_t prevMsg){
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
