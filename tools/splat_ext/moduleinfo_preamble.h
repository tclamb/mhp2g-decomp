typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int64_t;
typedef unsigned long uint64_t;

#include <psptypes.h>

#ifdef __cplusplus

#define DECLSPEC_EXTERN extern "C"

#else

#define DECLSPEC_EXTERN

#endif

#ifdef __MWERKS__

#pragma define_section _section_sceStubText   ".sceStub.text"         standard RX
#pragma define_section _section_libEntTop     ".lib.ent.top"          standard R
#pragma define_section _section_libEnt        ".lib.ent"              standard R
#pragma define_section _section_libEntBtm     ".lib.ent.btm"          standard R
#pragma define_section _section_libStubTop    ".lib.stub.top"         standard R
#pragma define_section _section_libStub       ".lib.stub"             standard R
#pragma define_section _section_libStubBtm    ".lib.stub.btm"         standard R
#pragma define_section _section_sceModuleInfo ".rodata.sceModuleInfo" standard R
#pragma define_section _section_sceResident   ".rodata.sceResident"   standard R
#pragma define_section _section_sceNid        ".rodata.sceNid"        standard R
#pragma define_section _section_sceVstub      ".rodata.sceVstub"      standard RW // assumed

#define DECLSPEC_SCESTUB_TEXT   DECLSPEC_EXTERN __declspec(_section_sceStubText)
#define DECLSPEC_LIB_ENT_TOP    DECLSPEC_EXTERN __declspec(_section_libEntTop)
#define DECLSPEC_LIB_ENT        DECLSPEC_EXTERN __declspec(_section_libEnt)
#define DECLSPEC_LIB_ENT_BTM    DECLSPEC_EXTERN __declspec(_section_libEntBtm)
#define DECLSPEC_LIB_STUB_TOP   DECLSPEC_EXTERN __declspec(_section_libStubTop)
#define DECLSPEC_LIB_STUB       DECLSPEC_EXTERN __declspec(_section_libStub)
#define DECLSPEC_LIB_STUB_BTM   DECLSPEC_EXTERN __declspec(_section_libStubBtm)
#define DECLSPEC_SCEMODULEINFO  DECLSPEC_EXTERN __declspec(_section_sceModuleInfo)
#define DECLSPEC_SCERESIDENT    DECLSPEC_EXTERN __declspec(_section_sceResident)
#define DECLSPEC_SCENID         DECLSPEC_EXTERN __declspec(_section_sceNid)
#define DECLSPEC_SCEVSTUB       DECLSPEC_EXTERN __declspec(_section_sceVstub)

#else

#define DECLSPEC_SCESTUB_TEXT   DECLSPEC_EXTERN
#define DECLSPEC_LIB_ENT_TOP    DECLSPEC_EXTERN
#define DECLSPEC_LIB_ENT        DECLSPEC_EXTERN
#define DECLSPEC_LIB_ENT_BTM    DECLSPEC_EXTERN
#define DECLSPEC_LIB_STUB_TOP   DECLSPEC_EXTERN
#define DECLSPEC_LIB_STUB       DECLSPEC_EXTERN
#define DECLSPEC_LIB_STUB_BTM   DECLSPEC_EXTERN
#define DECLSPEC_SCEMODULEINFO  DECLSPEC_EXTERN
#define DECLSPEC_SCERESIDENT    DECLSPEC_EXTERN
#define DECLSPEC_SCENID         DECLSPEC_EXTERN
#define DECLSPEC_SCEVSTUB       DECLSPEC_EXTERN

#endif

//#include <pspmoduleinfo.h>
//#include <pspmoduleexport.h>
// there's a few problems to using pspsdk types here:
// - declaring stubs only supported via assembly macros, no struct definitions,
// - sections assigned using __attribute__, which mwccpsp.exe does not support,

typedef u32 psp_import_version;
typedef char psp_import_name[];

typedef u32 psp_export_nids[];
typedef void *psp_export_target;
typedef psp_export_target psp_export_funcs[];
typedef psp_export_target psp_export_vars[];

typedef u32 psp_import_nids[];

typedef const struct
{
    psp_import_name *name;
    u32 flags;
    u8 entry_size;
    u8 var_count;
    u16 func_count;
    const u32 *exports;
} psp_module_export;

typedef const struct
{
    const char *name;
    u32 flags;
    u8 entry_size;
    u8 var_count;   // assigned by postlinker
    u16 func_count; // assigned by postlinker
    void *nids;     // assigned by postlinker
    void *funcs;    // assigned by postlinker
} psp_module_import;

typedef const struct
{
    psp_module_import *header;
    u32 nid;
} psp_stub_text;

typedef const struct
{
    u32 flags;
    char name[0x1c];
    void *gp;
    psp_module_export *exports_begin;
    psp_module_export *exports_end;
    psp_module_import *imports_begin;
    psp_module_import *imports_end;
} psp_module_info;

DECLSPEC_LIB_ENT_TOP const u32 _dummy_lib_ent_top = 0;
DECLSPEC_LIB_ENT_BTM const u32 _dummy_lib_ent_btm = 0;
DECLSPEC_LIB_STUB_TOP const u32 _dummy_lib_stub_top = 0;
DECLSPEC_LIB_STUB_BTM const u32 _dummy_lib_stub_btm = 0;