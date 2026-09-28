// Original matrix identity callback.
#include "vfpu.h"

extern "C" void func_em75_09D24120(ScePspFMatrix4 *matrix) {
    vmidt_q(matrix);
}
