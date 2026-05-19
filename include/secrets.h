#pragma once

#if __has_include("secrets.local.h")
#include "secrets.local.h"
#else
#include "secrets.example.h"
#warning "In deinem Projekt muss noch das 'secrets.local.h' eingebaut werden!"
#endif
