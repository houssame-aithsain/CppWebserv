CppWebServ

CppWebServ is a custom-built HTTP web server developed using C++. The server is designed to handle multiple HTTP requests concurrently, support CGI for handling file uploads, and integrate WebSocket functionality for real-time communication. This project aims to offer a lightweight, configurable alternative to web servers like Apache and Nginx.

Project Overview

CppWebServ is a C++ web server that efficiently handles HTTP requests, CGI file uploads, and provides real-time updates via WebSockets. It's designed for developers looking for a highly configurable, low-level server option that can be easily customized through a configuration file.
Features

    HTTP Request Handling: Supports GET, POST, and DELETE methods.
    CGI Integration: Handles file uploads via CGI and processes dynamic content.
    WebSockets: Real-time communication for live data updates.
    Multiple Server Support: Hosts multiple servers, each capable of listening on different ports.
    Customizable Config File: Easily configure server settings like IP address, port, hostname, and more.
    URL Redirection: Support for URL redirects via configuration.

Build Instructions

    Clone the repository:

    bash

git clone https://github.com/houssame-aithsain/CppWebserv.git

Navigate to the project directory:

bash

cd CppWebServ

Build the server:

bash

    make

Usage

Once built, you can run the web server using the following command:

bash

./cppwebserv config_file_path

Replace config_file_path with the path to your configuration file.
