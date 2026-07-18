#include "SerialPort.h"

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open(
    const std::string& portName,
    DWORD baudRate)
{
    Close();

    std::string fullPortName = "\\\\.\\" + portName;

    handle_ = CreateFileA(
        fullPortName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (handle_ == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DCB dcb = {};
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(handle_, &dcb))
    {
        Close();
        return false;
    }

    dcb.BaudRate = baudRate;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(handle_, &dcb))
    {
        Close();
        return false;
    }

    // Update()を止めないため、読み取りを待たない設定
    COMMTIMEOUTS timeouts = {};

    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;

    SetCommTimeouts(handle_, &timeouts);

    PurgeComm(
        handle_,
        PURGE_RXCLEAR | PURGE_TXCLEAR
    );

    return true;
}

void SerialPort::Close()
{
    if (handle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
    }

    receiveBuffer_.clear();
}

bool SerialPort::IsOpen() const
{
    return handle_ != INVALID_HANDLE_VALUE;
}

bool SerialPort::ReadLine(std::string& line)
{
    if (!IsOpen())
    {
        return false;
    }

    char buffer[256];

    DWORD bytesRead = 0;

    const BOOL result = ReadFile(
        handle_,
        buffer,
        sizeof(buffer),
        &bytesRead,
        nullptr
    );

    if (!result || bytesRead == 0)
    {
        return false;
    }

    receiveBuffer_.append(buffer, bytesRead);

    const size_t lineEnd =
        receiveBuffer_.find('\n');

    if (lineEnd == std::string::npos)
    {
        return false;
    }

    line = receiveBuffer_.substr(0, lineEnd);

    receiveBuffer_.erase(
        0,
        lineEnd + 1
    );

    // ESP32の改行が \r\n の場合
    if (!line.empty() && line.back() == '\r')
    {
        line.pop_back();
    }

    return true;
}