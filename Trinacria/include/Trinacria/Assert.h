#pragma once

#include <iostream>

#define TRCN_ASSERT(b, m) if(!b) {std::cout << m << "\n"; return;}
