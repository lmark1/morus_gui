#include <uavcan_linux/uavcan_linux.hpp>


uavcan::ISystemClock& getSystemClock()
{
    static uavcan_linux::SystemClock clock;
    return clock;
}   

uavcan::ICanDriver& getCanDriver(std::string iface_name)
{
    static uavcan_linux::SocketCanDriver driver(dynamic_cast<const uavcan_linux::SystemClock&>(getSystemClock()));
    if (driver.getNumIfaces() == 0)     // Will be executed once
    {
        if (driver.addIface(iface_name) < 0)
        {
            throw std::runtime_error("Unable to find CAN interface. Name: " + iface_name);
        }
    }
    return driver;
}
