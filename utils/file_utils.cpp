#include "file_utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string read_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Cannot open file: " + path);

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
