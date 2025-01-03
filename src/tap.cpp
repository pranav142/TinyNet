//
// Created by pknadimp on 1/2/25.
//

#include "tap.h"
#include "utils.h"
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>

int set_route(const char *dev, const char *cidr) {
    return run_cmd("ip route add %s dev %s", cidr, dev);
}

int set_if_up(const char *dev) {
    return run_cmd("ip link set dev %s up", dev);
}

int set_if_address(const char *dev, const char *cidr) {
    return run_cmd("ip addr add %s dev %s", cidr, dev);
}

int tap_alloc(const char *dev) {
    ifreq ifr{};
    int fd;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        std::cout << "Cannot open TUN/TAP dev" << std::endl;
        return -1;
    }

    memset(&ifr, 0, sizeof(ifreq));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    if (int err; (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        std::cout << "ERR: Could not ioctl tun: \n" << std::endl;
        close(fd);
        return err;
    }

    // strcpy(dev, ifr.ifr_name);
    return fd;
}

int tap_init(const char *dev, const char* addr) {
    int fd = tap_alloc(dev);
    if (fd < 0) {
        std::cout << "Cannot open TAP dev" << std::endl;
        return TAP_INIT_FAIL;
    }

    if (set_if_up(dev) != 0) {
        std::cout << "ERROR when setting up if" << std::endl;
        return TAP_INIT_FAIL;
    }

    if (set_if_address(dev, addr) != 0) {
        std::cout << "ERROR when setting up address for if" << std::endl;
        return TAP_INIT_FAIL;
    };

    return fd;
}

