#pragma once
#include <stdbool.h>
#include <stdint.h>

#define OUTBOX_MAX_SIZE 128
#define INBOX_MAX_SIZE 128

// include in .h b/c privacy unnecessary
// only 3 bytes of size
 struct NetworkPacket{
  bool isCall;
  uint8_t messageIndex; // index of message to send;
  uint8_t durationIndex; // index of the duration to send
};

typedef struct NetworkPacket NetworkPacket_t;

void networkInit();

void sendPacket(NetworkPacket_t* packet);

// constants
#define IS_CALL_KEY 1
#define MESSAGE_INDEX_KEY 2
#define DURATION_INDEX_KEY 3