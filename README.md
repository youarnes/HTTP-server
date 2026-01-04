# HTTP Server Implementation

A from-scratch HTTP server built using the **Winsock2 library** for socket programming on Windows. This project demonstrates TCP and HTTP server fundamentals for educational purposes.

> ⚠️ **Educational Use Only**: This server is not production-ready. Use established HTTP servers for production environments due to security considerations.

## Current Status

- ✅ TCP server implementation
- ✅ Minimal GET request handling for HTML pages

## Testing

1. Run the provided shell script to start the TCP server
2. Open your browser and navigate to `http://localhost`
3. View the details of the GET request in the server console, and see the static html on the page, if you try to access other files it will return a 404 not found error, with a side 404 HTML message.

## Future Improvements
1. Improve header parsing to handle various HTTP methods and headers
2. Improve code modularity and organization

## Resources

- [Winsock2 Documentation](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/)