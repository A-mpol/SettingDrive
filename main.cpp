#include "drive-setup.hpp"
#include <iostream>

int main() {
    Drive dorna("/dev/tty.usbserial-AH011Z8K");
    dorna.TurnOn();
    dorna.MoveToPosition(0);
    while (!dorna.InPosition()) {
        int pos = dorna.GetDrivePosition();
        std::cout << pos << '\n';
    }
    dorna.SwitchOff();
    return 0;
}
