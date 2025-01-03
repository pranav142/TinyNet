#include <csignal>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <cstring>
#include <cstdarg>
int fd;

#define CMDBUFLEN 128

int run_cmd(char *cmd, ...) {
    va_list ap;
    char buf[CMDBUFLEN];

    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);
    va_end(ap);
    printf("%s\n", buf);
    return system(buf);
}

int set_route(char *dev, const char *cidr) {
    return run_cmd("ip route add %s dev %s", cidr, dev);
}

int set_if_up(char *dev) {
    return run_cmd("ip link set dev %s up", dev);
}

int set_if_address(char *dev, const char *cidr) {
    return run_cmd("ip addr add %s dev %s", cidr, dev);
}

int tun_alloc(char *dev) {
    ifreq ifr{};
    int fd;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0) {
        std::cout << "Cannot open TUN/TAP dev" << std::endl;
        return -1;
    }

    memset(&ifr, 0, sizeof(ifreq));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    if (int err; (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        std::cout << "ERR: Could not ioctl tun: \n" << std::endl;
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

void stop(int) {
    std::cout << "stopping" << std::endl;
    close(fd);
    exit(0);
}

int main() {
    char dev[IFNAMSIZ] = "tap0";

    fd = tun_alloc(dev);
    if (fd < 0) {
        std::cout << "Cannot open TUN/TAP dev" << std::endl;
        exit(1);
    }

    if (set_if_up(dev) != 0) {
        printf("ERROR when setting up if\n");
    }

    if (set_if_address(dev, "10.0.0.1/24") != 0) {
        printf("ERROR when setting address for if\n");
    };

    //   const char *if_name = "10.0.0.0/24";
    //   if (set_route(dev, if_name) != 0) {
    //       printf("ERROR when setting route for if\n");
    //   }

    signal(SIGTERM, stop);

    char buffer[1500];
    while (true) {
        int len = read(fd, buffer, sizeof(buffer));
        if (len > 0) {
            std::cout << len << std::endl;
            // std::cout << buffer << std::endl;
        }
    };


    close(fd);
    return 0;
}
