#include <stdint.h>
#include <stdbool.h>

void switchAcc(int acc_num, bool set_state);
void pingResponse();
void configDataFrame(uint8_t config_data[8], int framecount);
void configTerminator(int channel, int framecount);
void statusResponse(int chanel);
