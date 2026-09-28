// Original matrix identity callback.
#include "vfpu.h"

extern "C" void func_em83_09D23990(ScePspFMatrix4 *matrix) {
    vmidt_q(matrix);
}
