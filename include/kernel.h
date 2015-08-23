#pragma once

#include "syscall.h"

#include "memory.h"
#include "file.h"
#include "module.h"
#include "eventflag.h"
#include "semaphore.h"
#include "barrier.h"
#include "mutex.h"
#include "unknown.h"
#include "registry.h"

#define TEXT (void *)TEXT_ADDRESS
#define DATA (void *)DATA_ADDRESS
