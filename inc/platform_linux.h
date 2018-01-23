#include <uavcan/uavcan.hpp>

/*
 *  Concrete implementation for returning system clock reference
 *  for Linux platform.
 */
uavcan::ISystemClock& getSystemClock();

/*
 *  Concrete implementation for returning CAN driver refernce
 *  for Linux platrofm.
 */
uavcan::ICanDriver& getCanDriver(std::string iface_name);
