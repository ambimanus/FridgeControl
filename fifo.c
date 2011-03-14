#include "fifo.h"

void fifo_init (fifo_t *f, uint8_t *buffer, const uint8_t size)
{
    f->count = 0;
    f->pread = f->pwrite = buffer;
    f->read2end = f->write2end = f->size = size;
}

uint8_t fifo_put (fifo_t *f, const uint8_t data)
{
    return _inline_fifo_put (f, data);
}

uint8_t fifo_get_wait (fifo_t *f, int16_t timeout)
{
    uint32_t time = rtc_getTime();
    while (!f->count) {
        if (rtc_getTime() != time) {
            time = rtc_getTime();
            timeout--;
            if (timeout <= 0) {
                return 0xFF;
            }
        }
    }

    return _inline_fifo_get (f);
}

int fifo_get_nowait (fifo_t *f)
{
    if (!f->count)      return -1;

    return (int) _inline_fifo_get (f);
}
