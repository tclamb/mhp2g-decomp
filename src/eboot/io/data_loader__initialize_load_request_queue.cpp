#include "io.hpp"

void data_loader::initialize_load_request_queue() {
    load_request *req = load_request_ringbuf;
    do {
        req++->state = 0;
    } while (req < load_request_ringbuf + sizeof(load_request_ringbuf)/sizeof(load_request));
    load_request_load_head = 0;
    load_request_write_head = 0;
    load_thread_status = 0;
}