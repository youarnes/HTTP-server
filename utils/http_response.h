#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

std::string http_response(
    int status,
    const std::string& status_text,
    const std::string& content_type,
    const std::string& body
);

#endif
