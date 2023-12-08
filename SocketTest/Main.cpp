#include "Header.h"

int main() {
	Mail hello;
	MailWorking test;
	User usertest("config.txt");
	test.sendMail("127.0.0.1", 2225, usertest, hello);
}