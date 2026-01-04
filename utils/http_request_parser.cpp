#include <string>
#include <unordered_map>
#include <sstream>
#include <stdexcept>

struct HttpRequest {
    std::string method;
    std::string target;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

HttpRequest parse_http_request(const std::string& raw)
{
    HttpRequest req;

    std::istringstream stream(raw);
    std::string line;

    // 1️⃣ Request line
    if (!std::getline(stream, line))
        throw std::runtime_error("Empty request");

    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    std::istringstream request_line(line);
    request_line >> req.method >> req.target >> req.version;

    if (req.method.empty() || req.target.empty() || req.version.empty())
        throw std::runtime_error("Malformed request line");

    // 2️⃣ Headers
    while (std::getline(stream, line)) {
        if (line == "\r" || line == "")
            break;

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        auto pos = line.find(':');
        if (pos == std::string::npos)
            throw std::runtime_error("Malformed header");

        std::string key   = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Trim leading space
        if (!value.empty() && value.front() == ' ')
            value.erase(0, 1);

        req.headers[key] = value;
    }

    // 3️⃣ Body (optional)
    req.body.assign(std::istreambuf_iterator<char>(stream),
                    std::istreambuf_iterator<char>());

    return req;
}
