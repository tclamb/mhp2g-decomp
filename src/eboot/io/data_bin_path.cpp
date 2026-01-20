#include "io.hpp"

extern "C" char data_bin_path_s[];

extern "C" char* data_bin_path() {
    return data_bin_path_s;
}