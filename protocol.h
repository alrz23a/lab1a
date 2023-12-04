#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __GCC_IEC_559
#pragma message("GCC ICE 559 defined...")
#else
#error *** do not use this platform
#endif

#include <stdint.h>

struct __attribute__((__packed__)) calcProtocol {
  uint16_t type;
  uint16_t major_version;
  uint16_t minor_version;
  uint32_t id;
  uint32_t arith;
  int32_t inValue1;
  int32_t inValue2;
  int32_t inResult;
  double flValue1;
  double flValue2;
  double flResult;
};

struct __attribute__((__packed__)) calcMessage {
  uint16_t type;
  uint32_t message;
  uint16_t protocol;
  uint16_t major_version;
  uint16_t minor_version;
};

/* arith mapping in calcProtocol
1 - add
2 - sub
3 - mul
4 - div
5 - fadd
6 - fsub
7 - fmul
8 - fdiv
other numbers are reserved
*/

/* calcMessage.type
1 - server-to-client, text protocol
2 - server-to-client, binary protocol
3 - server-to-client, N/A
21 - client-to-server, text protocol
22 - client-to-server, binary protocol
23 - client-to-serve, N/A
calcMessage.message
0 = Not applicable/available (N/A or NA)
1 = OK   // Accept
2 = NOT OK  // Reject
*/
#endif
