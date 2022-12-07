#pragma once
#include "../commons/data.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define WR_VALUE _IOW('a', 'a', MotorData *)

void sendToDriver(MotorData motorData)
{

    int f = open("/dev/VOLLGAS", O_RDWR);
    if (f < 0)
    {
        printf("Cannot open device file...\n");
        return;
    }

    printf("WR_VALUE %x\n", WR_VALUE);
    ioctl(f, WR_VALUE, (MotorData *)&motorData);

    close(f);
}