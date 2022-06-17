#ifndef __HELPERS_H__
#define __HELPERS_H__

void dump_bytes(unsigned int *ptr, int count, unsigned long addr);
void crcbios(void);
int get_param(char *buf, char **cmd, char **params);
struct command_struct *command_dispatcher(char *command, int nb_params, char **params);
void init_dispatcher(void);
void usleep(int useconds);

//105e6/1000 = 105000
#define CYCLES_PER_MS 105000

#endif
