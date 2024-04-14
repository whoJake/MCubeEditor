#pragma once

#include <fstream>
#include <vector>

class fiDevice
{
public:
    fiDevice();
    ~fiDevice();

    bool open(const char* uri);
    void close();

    std::vector<uint8_t> read(size_t size);

    std::vector<uint8_t> read_at(size_t size, size_t offset);

    size_t get_size() const;
private:
    bool open_drive_file(const char* filepath);

    void try_close();
private:
    std::ifstream m_handle{ };
    size_t m_size{ 0 };
};