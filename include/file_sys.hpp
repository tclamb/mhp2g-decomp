#pragma once

#include "file_sys_impl.hpp"
#include "singleton.hpp"

struct FileSys : Singleton<FileSys>, FileSysImpl  {
    FileSys();
    ~FileSys();
};
