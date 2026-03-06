#pragma once

#include "data_loader_impl.hpp"
#include "singleton.hpp"

struct data_loader : singleton<data_loader>, data_loader_impl  {
    data_loader();
    ~data_loader();
};
