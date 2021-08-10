/****************************************************************
       COSMOS HAL头文件halheads.h
*****************************************************************
                彭东
****************************************************************/
#ifndef _HALHEADS_H
#define _HALHEADS_H
#include "link.h"
#include "halplatform.h"
#include "cpu.h"
#include "halcpuctrl.h"
#include "halmmu.h"
#include "halintupt.h"
#include "halmm.h"
#include "halmach.h"
#ifdef CFG_X86_PLATFORM
#include "io.h"
#include "halgdtidt.h"
#include "i8254.h"
#include "i8259.h"
#include "bdvideo.h"
#include "memmgrmctrl.h"
#endif
#include "halglobal.h"
#include "halinit.h"
#include "halprint.h"
#endif // HALHEADS_H
