#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../slstatus.h"
#include "../util.h"

const char *
bluetooth_status(const char *unused)
{
    FILE *fp;
    char path[1035];
    char connected[1035] = "";
    const char *fmt;

    // Check if Bluetooth is powered on
    fp = popen("bluetoothctl show | grep 'Powered: yes'", "r");
    if (fp == NULL) {
        return "Error";
    }
    pclose(fp);

    // Check if any devices are connected and get their names
    fp = popen("bluetoothctl devices Connected", "r");
    if (fp == NULL) {
        return "Error";
    }
    if (fgets(path, sizeof(path), fp) != NULL) {
        do {
            // Extract device name from the output
            char *device_name = strstr(path, "Device ");
            if (device_name) {
                device_name += 7; // Skip "Device "
                char *name_start = strchr(device_name, ' ');
                if (name_start) {
                    name_start++;
                    char *name_end = strchr(name_start, '\n');
                    if (name_end) {
                        *name_end = '\0';
                    }
                    strcat(connected, name_start);
                    /*strcat(connected, ", ");*/
                }
            }
        } while (fgets(path, sizeof(path), fp) != NULL);
        // Remove the trailing comma and space
        size_t len = strlen(connected);
        if (len > 2) {
            connected[len - 2] = '\0';
        }
    }
    pclose(fp);
    fmt = bprintf("%s", connected);

    // Return formatted string
    return fmt;
}
