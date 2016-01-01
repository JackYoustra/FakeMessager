#include "networking.h"
#include "stdint.h"
#include <pebble.h>

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  vibes_double_pulse();
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
  vibes_short_pulse();
}

void networkInit(){
  // Open AppMessage with sensible buffer sizes
  app_message_open(INBOX_MAX_SIZE, OUTBOX_MAX_SIZE);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
}

void sendPacket(NetworkPacket_t* packet){
  APP_LOG(APP_LOG_LEVEL_INFO, "Sending packet");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  
  // Write elements
  dict_write_uint8(iter, IS_CALL_KEY, packet->isCall); // boolean interpreted as unsigned integer
  dict_write_uint8(iter, MESSAGE_INDEX_KEY, packet->messageIndex);
  dict_write_uint8(iter, DURATION_INDEX_KEY, packet->durationIndex);
    
  app_message_outbox_send();
  APP_LOG(APP_LOG_LEVEL_INFO, "Started packet sending");
}