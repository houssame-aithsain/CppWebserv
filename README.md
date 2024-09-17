```markdown
# CppWebServ

## Introduction
CppWebServ is a lightweight, high-performance web server built using C++. It supports basic HTTP methods such as GET, POST, and DELETE. The server is configurable, and users can define various settings, such as the IP address, port, and limits for request bodies. The project includes CGI support for handling dynamic content.

- **Live Site**: [Link to deployed site]
- **Project Blog**: [Link to final project blog article]
- **Author(s) LinkedIn**:
  - [LinkedIn Profile](https://www.linkedin.com/in/houssame-aithsain)

## Installation

To install and run CppWebServ locally, follow these steps:

### Prerequisites
- **C++ Compiler**: Ensure you have a working C++ compiler (e.g., `g++`).
- **Make**: Ensure `make` is installed.
- **Dependencies**: Make sure to have any additional libraries installed, such as `libcgi`, `sys/socket.h`, `netinet/in.h`.

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/houssame-aithsain/CppWebServ.git
   cd CppWebServ
   ```

2. Compile the project:
   ```bash
   make
   ```

3. Run the server:
   ```bash
   ./cppwebserv [path to configuration file]
   ```

4. Test the server by accessing it via a browser or tools like `curl`:
   ```bash
   curl http://127.0.0.1:1336
   ```

## Usage

Once the server is running, you can make requests using a browser or HTTP tools:

- **GET**: Access static content or dynamically generated content through CGI.
- **POST**: Send data to the server (e.g., form submissions).
- **DELETE**: Remove specific files or data from the server.

The server configuration can be customized through a config file that allows specifying:
- IP Address
- Hostname
- Port
- URL redirection
- Body size limit
- CGI path

### Example Usage:
```bash
curl -X POST http://127.0.0.1:1336/upload -d "file=@test.txt"
```

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes and commit them (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a pull request.

Please ensure all new features and bug fixes come with tests.

## Related Projects

Here are some projects related to CppWebServ:
- [Nginx](https://nginx.org)
- [Apache](https://httpd.apache.org)

## Licensing

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Screenshot

![CppWebServ Screenshot](./src/ScreenShot.png)
```