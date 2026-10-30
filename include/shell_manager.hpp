#pragma once

#include "singleton.hpp"

struct ShellManager : Singleton<ShellManager> {
    void shell00_set();
};
