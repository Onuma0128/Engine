#pragma once

#include <Windows.h>
#include <string>

class SerialPort
{
public:
    SerialPort() = default;
    ~SerialPort();

    bool Open(
        const std::string& portName,
        DWORD baudRate = CBR_115200
    );

    void Close();

    bool IsOpen() const;

    // 受信済みの完全な1行を取得
    bool ReadLine(std::string& line);

private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;

    // 途中まで受信したデータを保持
    std::string receiveBuffer_;
};