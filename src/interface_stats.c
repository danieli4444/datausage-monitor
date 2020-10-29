#ifndef INTERFACE_STATS
#define INTERFACE_STATS
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <linux/if_link.h>
#include "interface_stats.h"
#include <unistd.h>

char interface_sys_path[] = "/sys/class/net/";

/*
* check that a given interface string is valid
*/
int check_if_by_name(char *wanted_if_name, struct ifaddrs **selected_if){
    struct ifaddrs *if_addr, *ifa;
    int if_count = 0;
    int family;
#ifdef DEBUG
    printf("getting all interfaces:\n");
#endif /* DEBUG */
    if (-1==getifaddrs(&if_addr)){
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = if_addr;ifa !=NULL; ifa = ifa->ifa_next){
        family = ifa->ifa_addr->sa_family;
#ifdef DEBUG
        printf("interface %d: %s\n",if_count, ifa->ifa_name);
        printf("%-8s %s (%d)\n",
                    ifa->ifa_name,
                    (family == AF_PACKET) ? "AF_PACKET" :
                    (family == AF_INET) ? "AF_INET" :
                    (family == AF_INET6) ? "AF_INET6" : "???",
                    family);

        if (family == AF_PACKET && ifa->ifa_data != NULL) {
                   struct rtnl_link_stats *stats = ifa->ifa_data;

                   printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                          "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
               }
#endif /* DEBUG */
        if (!strcmp(wanted_if_name,ifa->ifa_name) && (family == AF_PACKET) ){
#ifdef DEBUG
            printf("interface exists!\n");
#endif /* DEBUG */
            *selected_if = ifa;
            return 0;
        }
        if_count+=1;
    }
#ifdef DEBUG
    printf("interface doesnt exist\n");
#endif /* DEBUG */
    return ERROR_NO_INTERFACE;
}

/*
 * function print current tx stats status of a given interface
 * params:
 * wanted_if_name - wanted interface name as a string
 * returns NULL for fail
 */
struct rtnl_link_stats *get_interface_stats(char *wanted_if_name){
    struct ifaddrs *selected_if = {0};

    if (ERROR_NO_INTERFACE == check_if_by_name(wanted_if_name, &selected_if)){
        printf(" entered wrong interface name \n");
        return NULL;
    }
    else {
        struct rtnl_link_stats *stats = selected_if->ifa_data;
        return stats;
    }
}

/*
 * returns data_usage(bytes) per $period time 
 */
float calc_data_usage(char * wanted_if_name,int period){
    struct rtnl_link_stats *stats_before;
    struct rtnl_link_stats *stats_after;
    float data_usage = 0;
    stats_before = get_interface_stats(wanted_if_name);
    sleep(period);
    stats_after = get_interface_stats(wanted_if_name);
    
    if (NULL == stats_before || NULL == stats_after){
        printf("calc_data_usage failed\n");
        return -1;
    }
    else{
        data_usage = (float)((stats_after->tx_bytes - stats_before->tx_bytes)/period);
#ifdef DEBUG 
        printf("data_usage = %f\n",data_usage);
        printf("before:\n");
        printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                    "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                    stats_before->tx_packets, stats_before->rx_packets,
                    stats_before->tx_bytes, stats_before->rx_bytes);
        printf("after:\n");
        printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                    "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                    stats_after->tx_packets, stats_after->rx_packets,
                    stats_after->tx_bytes, stats_after->rx_bytes);
#endif /* DEBUG */
        return data_usage;
    }
    
}

/*
 * run data usage calculation repeatedly.  
 */

int run_calc_forever(char * wanted_if_name){
    printf("starting ...\n");
    while (1){
        
        int data_usage = calc_data_usage(wanted_if_name,DEFAULT_PERIOD);
        printf(" -----  data_usage of %s interface txbytes per sec is %d Bytes ------ \n\n",wanted_if_name,data_usage);
    }
    return 0;
}
#endif /* INTERFACE_STATS */
