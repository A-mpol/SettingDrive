#include "drive-setup.hpp"

int main() {
    Drive dorna("/dev/tty.usbserial-AH011Z8K");
    dorna.TurnOn();
    dorna.MoveToPosition(30000);
    return 0;
}
