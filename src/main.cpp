#include <csignal>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/if.h>
#include <cstring>
#include "tap.h"

int fd;

void stop(int) {
    std::cout << "stopping" << std::endl;
    close(fd);
    exit(0);
}

int main() {
    char dev[] = "tap0";
    fd = tap_init(dev,"10.0.0.1/24");

   // char dev2[] = "tap1";
   // fd = tap_init(dev2,"10.0.0.2/24");

    signal(SIGTERM, stop);
    signal(SIGINT, stop);

    char buffer[1500];
    while (true) {
        ssize_t len = read(fd, buffer, sizeof(buffer));
        if (len > 0) {
            std::cout << len << std::endl;
            // std::cout << buffer << std::endl;
        }
    };

    close(fd);
    return 0;
}
