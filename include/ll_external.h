#ifndef LL_EXTERNAL_H_
#define LL_EXTERNAL_H_

/** defines the debounce time in ms */
#define LL_EXT_DEBOUNCE_TIME 10


void ll_ext_init(void);
void ll_ext_run(void);

void ll_ext_enable_intr(void);
void ll_ext_disable_intr(void);

uint64_t ll_ext_intr_occured(void);
uint64_t ll_ext_get_last_intr_time(void);

#endif
