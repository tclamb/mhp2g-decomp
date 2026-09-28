// Original angle vector callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;

extern "C" void func_em59_09D1A9B8(float *dest, const u32 *angles) {
    dest[0] = 9.58738019107841e-05f * (int)(u16)angles[0];
    dest[1] = 9.58738019107841e-05f * (int)(u16)angles[1];
    dest[2] = 9.58738019107841e-05f * (int)(u16)angles[2];
    dest[3] = 0.0f;
}
