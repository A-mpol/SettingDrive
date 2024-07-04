#include "drive-setup.hpp"
#include <iostream>

Drive::Drive(char * name, int baud, char parity, int data_bit, int stop_bit) {
    drive = modbus_new_rtu(name, baud, parity, data_bit, stop_bit);
    modbus_connect(drive);
    modbus_set_slave(drive, 1);
}

uint16_t Drive::ChangeValueRegister(const uint16_t value_register, const int bit_number, const int bit_value) {
    uint16_t tmp = 1 << ((bit_number - 1) * 4);
    uint16_t modified_register_value = value_register;
    if (bit_value == 1)
        modified_register_value = value_register | tmp;
    else if (bit_value == 0) {
        tmp = ~tmp;
        modified_register_value = value_register & tmp;
    }
    return modified_register_value;
}

int Drive::TurnOn() {
    uint16_t data[1];
    int success;
    success = modbus_read_registers(drive, 508, 1, data);
    if (success == -1) {
        std::cerr << "Modbus read register 508 failed: " << modbus_strerror(errno) << std::endl;
        return -1;
    }
    int bit_number = 1;
    int bit_value = 1;
    uint16_t register_value = data[0];
    uint16_t modified_register_value;
    modified_register_value = ChangeValueRegister(register_value, bit_number, bit_value);
    success = modbus_write_register(drive, 508, modified_register_value);
    if (success == -1) {
        std::cerr << "Modbus write register 508 failed: " << modbus_strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

int Drive::SwitchOff() {
    uint16_t data[1];
    int success;
    success = modbus_read_registers(drive, 508, 1, data);
    if (success == -1) {
        std::cerr << "Modbus read register 508 failed: " << modbus_strerror(errno) << std::endl;
        return -1;
    }
    int bit_number = 1;
    int bit_value = 0;
    uint16_t register_value = data[0];
    uint16_t modified_register_value;
    modified_register_value = ChangeValueRegister(register_value, bit_number, bit_value);
    success = modbus_write_register(drive, 508, modified_register_value);
    if (success == -1) {
        std::cerr << "Modbus write register 508 failed: " << modbus_strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

void Drive::MoveToPosition(const int position) {
    SetPosition(position);
    GoToPosition();
}

void Drive::SetPosition(int position) {
    if (position < 0)
        position = 0xFFFFFFFF + position + 1;
    int16_t position701 = position & 0xFFFF;
    int16_t position702 = (position >> 16) & 0xFFFF;
    int success;
    success = modbus_write_register(drive, 701, position701);
    if (success == -1) {
        std::cerr << "Modbus write register 701 failed: " << modbus_strerror(errno) << std::endl;
        return;
    }
    success = modbus_write_register(drive, 702, position702);
    if (success == -1) {
        std::cerr << "Modbus write register 702 failed: " << modbus_strerror(errno) << std::endl;
        return;
    }
}

void Drive::GoToPosition() {
    uint16_t data[1];
    int success;
    success = modbus_read_registers(drive, 508, 1, data);
    if (success == -1) {
        std::cerr << "Modbus read register 508 failed: " << modbus_strerror(errno) << std::endl;
        return;
    }
    int bit_number = 3;
    int bit_value = 0;
    uint16_t register_value = data[0];
    uint16_t modified_register_value;
    modified_register_value = ChangeValueRegister(register_value, bit_number, bit_value);
    success = modbus_write_register(drive, 508, modified_register_value);
    if (success == -1) {
        std::cerr << "Modbus write register 508 failed: " << modbus_strerror(errno) << std::endl;
        return;
    }
    success = modbus_read_registers(drive, 508, 1, data);
    if (success == -1) {
        std::cerr << "Modbus read register 508 failed: " << modbus_strerror(errno) << std::endl;
        return;
    }
    bit_value = 1;
    register_value = data[0];
    modified_register_value = ChangeValueRegister(register_value, bit_number, bit_value);
    success = modbus_write_register(drive, 508, modified_register_value);
    if (success == -1) {
        std::cerr << "Modbus write register 508 failed: " << modbus_strerror(errno) << std::endl;
        return;
    }
}

int Drive::GetDrivePosition() {
    uint16_t data[2];
    int success;
    int drive_position;
    success = modbus_read_registers(drive, 1923, 2, data);
    if (success == -1) {
        std::cerr << "Modbus read register 508 failed: " << modbus_strerror(errno) << std::endl;
        return -1;
    }
    drive_position = (data[1] << 16) + data[0];
    return drive_position;
}

bool Drive::InPosition() {
    uint16_t data[1];
    int success;
    success = modbus_read_registers(drive, 1549, 1, data);
    return (data[0] == 11);
}
