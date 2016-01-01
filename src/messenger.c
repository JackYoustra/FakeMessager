#include "pebble.h"
#include "messenger.h"
#include "networking.h"
#include "time_setter.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

static Window *window;
static SimpleMenuLayer *messageSelectMenuLayer;
static SimpleMenuSection messageSelectSections[NUM_MENU_SECTIONS];
static SimpleMenuItem messageSelectMenuItems[NUM_FIRST_MENU_ITEMS];

static void messaging_select_callback(int index, void *ctx) {
  NetworkPacket_t *partialPacket = malloc(sizeof(NetworkPacket_t));
  partialPacket->isCall = 0;
  partialPacket->messageIndex = (uint8_t)index;
  // transition to time_setter, call deinit too
  createTimeSetterWindow(partialPacket);
  
  //layer_mark_dirty(simple_menu_layer_get_layer(messageSelectMenuLayer));
}

static void loadMessageWindow(Window *window) {
  messageSelectMenuItems[0] = (SimpleMenuItem) {
    .title = "I need a quick decision on this...",
    .callback = messaging_select_callback,
  };
  messageSelectMenuItems[1] = (SimpleMenuItem) {
    .title = "Hey baby, what's up?",
    .callback = messaging_select_callback,
  };
  
  messageSelectMenuItems[2] = (SimpleMenuItem) {
    .title = "Johnny broke his leg! We need you to come pick him up.",
    .callback = messaging_select_callback,
  };

  messageSelectSections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = messageSelectMenuItems,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  messageSelectMenuLayer = simple_menu_layer_create(bounds, window, messageSelectSections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(messageSelectMenuLayer));
}

void unloadMessageWindow(Window *window) {
  simple_menu_layer_destroy(messageSelectMenuLayer);
}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = loadMessageWindow,
    .unload = unloadMessageWindow,
  });
  window_stack_push(window, true);
}

static void deinit() {
  window_destroy(window);
}

void openMessengerWindow(){
  init();
}