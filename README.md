# SocketTest
Using socket implement a mail client to sent and recive mail from server

Overview
This is client build base on Mr Nguyen Thanh Quan's server works like a mail app

Features
Work with a configured socket as saved in the config file
Send mail with/without attachment with size <= 3MB
Receive mail and download attachment with your choice
Organize emails into folders based on predefined formats in the config file

Prerequisites
 C++20

Installation
bash
Copy code
git clone https://github.com/ppddddpp/SocketTest.git
cd SocketTest.git

Usage
Debug in your IDE.

Configuration
There is an example configuration you can change for suittable

Server reference:
https://github.com/eugenehr/test-mail-server/releases/download/1.0/test-mail-server-1.0.jar
Source code: https://github.com/eugenehr/test-mail-server/
Language: Java
Version: 8 (not version 11, 17, or 21)
Install Java 8 (JDK 8u361) on your machine: https://gist.github.com/wavezhang/ba8425f24a968ec9b2a8619d7c2d86a6 (Example: jdk-8u361-windows-x64.exe)
Run the program: java -jar test-mail-server-<version>.jar <args>
In case the "java" command is not recognized in the Command Prompt on Windows, refer to this guide: https://www.youtube.com/watch?v=d9ANxJC_P7s

