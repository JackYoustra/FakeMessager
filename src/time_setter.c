#include "pebble.h"
#include "networking.h"
#include "time_setter.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 5

static Window *window;
static SimpleMenuLayer *timeSelectMenuLayer;
static SimpleMenuSection timeSelectSections[NUM_MENU_SECTIONS];
static SimpleMenuItem timeSelectMenuItems[NUM_FIRST_MENU_ITEMS];
static NetworkPacket_t* packet;

static void time_select_callback(int index, void *ctx) {
  // fill the remainder of the network packet
  packet->durationIndex = (uint8_t)index;
  // send packet
  sendPacket(packet);
  free(packet); // not needed anymore so release
  // pop to the top
  window_stack_pop(0);
  window_stack_pop(1); // this one is animated
  //layer_mark_dirty(simple_menu_layer_get_layer(timeSelectMenuLayer));
}

static void loadTimeWindow(Window *window) {
  timeSelectMenuItems[0] = (SimpleMenuItem) {
    .title = "One second",
    .callback = time_select_callback,
  };
  
  timeSelectMenuItems[1] = (SimpleMenuItem) {
    .title = "Fifteen seconds",
    .callback = time_select_callback,
  };
  
  timeSelectMenuItems[2] = (SimpleMenuItem) {
    .title = "Thirty seconds",
    .callback = time_select_callback,
  };
  
  timeSelectMenuItems[3] = (SimpleMenuItem) {
    .title = "One minute",
    .callback = time_select_callback,
  };
  
  timeSelectMenuItems[4] = (SimpleMenuItem) {
    .title = "Five minues",
    .callback = time_select_callback,
  };

  timeSelectSections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = timeSelectMenuItems,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  timeSelectMenuLayer = simple_menu_layer_create(bounds, window, timeSelectSections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(timeSelectMenuLayer));
}

void unloadTimeWindow(Window *window) {
  simple_menu_layer_destroy(timeSelectMenuLayer);
}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = loadTimeWindow,
    .unload = unloadTimeWindow,
  });
  window_stack_push(window, true);
}

static void deinit() {
  window_destroy(window);
}

void createTimeSetterWindow(NetworkPacket_t* prevMsg){
  packet = prevMsg;
  init();
}