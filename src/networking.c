#include "networking.h"
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
  DictionaryIterator *iterator;
  app_message_outbox_begin(&iterator);
  
  // write data
  const uint8_t numOfKeys = 3;
  const uint32_t size = dict_calc_buffer_size(numOfKeys,
                                              sizeof(bool),
                                              sizeof(uint8_t),
                                              sizeof(uint8_t));
  uint8_t buffer[size];
  
  DictionaryIterator iter; // keeps state of serialization process
  // Begin:
  dict_write_begin(&iter, buffer, sizeof(buffer));
  // Write elements
  dict_write_uint8(&iter, IS_CALL_KEY, packet->isCall); // boolean interpreted as unsigned integer
  dict_write_uint8(&iter, MESSAGE_INDEX_KEY, packet->messageIndex);
  dict_write_uint8(&iter, DURATION_INDEX_KEY, packet->durationIndex);
  
  const uint32_t final_size = dict_write_end(&iter);
  
  
  app_message_outbox_send();
}