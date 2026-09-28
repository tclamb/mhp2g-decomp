// Original matrix identity callback.
#include "vfpu.h"

extern "C" void func_em02_09D1DBC0(ScePspFMatrix4 *matrix) {
    vmidt_q(matrix);
}
