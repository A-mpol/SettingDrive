#ifndef drive_setup_hpp
#define drive_setup_hpp

#include "modbus.h"
class Drive {
private:
    modbus_t * drive;
    uint16_t ChangeValueRegister(const uint16_t value_register, const int bit_number, const int bit_value);
    void SetPosition(int position);
    void GoToPosition();
public:
    Drive(char * name="/dev/tty.usbserial-AH011Z8K", int baud=57600,
          char parity='O', int data_bit=8, int stop_bit=1);
    ~Drive() {modbus_close(drive); modbus_free(drive);};
    int TurnOn();
    int SwitchOff();
    void MoveToPosition(const int position);
    int GetDrivePosition();
    bool InPosition();
};

#endif /* drive_setup_hpp */
