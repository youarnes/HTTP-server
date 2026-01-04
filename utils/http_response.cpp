#include "http_response.h"
#include <sstream>

std::string http_response(
    int status,
    const std::string& status_text,
    const std::string& content_type,
    const std::string& body
) {
    std::ostringstream res;

    res << "HTTP/1.1 " << status << " " << status_text << "\r\n";
    res << "Content-Type: " << content_type << "\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << body;

    return res.str();
}
