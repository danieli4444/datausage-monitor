#include <ifaddrs.h>
#define ERROR_NO_INTERFACE -1
#define UKNOWN_ERROR -2
#define DEFAULT_PERIOD 1
//#define DEBUG


int get_if_by_name(char *wanted_if_name, struct ifaddrs **selected_if);
struct rtnl_link_stats *get_interface_stats(char * wanted_if_name);
float calc_data_usage(char *wanted_if_name, int period);
int run_calc_forever(char * wanted_if_name);
