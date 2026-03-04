#pragma once

#define GE_CMD_ATEST 0xDB
#define GE_CMD_ZTEST 0xDE
#define GE_CMD_ZWRITEDISABLE 0xE7

#define GE_OP_ALWAYS 1
#define GE_OP_AT_MOST 5
#define GE_OP_AT_LEAST 7

extern u32 *D_eboot_089C70D0;

namespace immediate_ge {
    namespace ge {
        namespace impl {

            inline void emit(u32 cmd) {
                *D_eboot_089C70D0++ = cmd;
            }

        }

        inline void atest(u8 mask, u8 threshold, u8 op) {
            impl::emit(((GE_CMD_ATEST << 24) | (mask << 16) | op) | (threshold << 8));
        }

        inline void ztest(u8 op) {
            impl::emit((GE_CMD_ZTEST << 24) |  op);
        }

        inline void zwritedisable(bool value) {
            impl::emit((GE_CMD_ZWRITEDISABLE << 24) |  value);
        }

    }
}
