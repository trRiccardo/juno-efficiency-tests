//constants to read raw file
//trailer words are swapped

#ifndef Constants_h
#define Constants_h

#include <cstdint>
#include <vector>

class Constants {

 public:

 class header {
 public:
   static const uint16_t m1 = 0x805a;
   static const uint16_t m2 = 0x0000;
 };

 class trailer {
 public:
   static constexpr uint16_t start[6] = { 0xaa55, 0x2301, 0x6745,
				      0xab89, 0xefcd, 0x00ff  };
   static const uint16_t end = 0x6908;
 };

};


#endif

