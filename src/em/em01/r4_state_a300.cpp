// Original matrix identity callback.
#include "vfpu.h"

extern "C" void func_em01_09D1F400(ScePspFMatrix4 *matrix) {
    vmidt_q(matrix);
}
