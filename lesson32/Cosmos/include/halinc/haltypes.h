/****************************************************************
        COSMOS HAL类型头文件haltypes.h
*****************************************************************
                彭东
****************************************************************/
#ifndef _HALTYPES_H
#define _HALTYPES_H

#include "halplatform_t.h"
#include "cpu_t.h"
#include "halcpuctrl_t.h"
#include "halmmu_t.h"
#include "halintupt_t.h"
#include "halmm_t.h"
#include "halmach_t.h"
#ifdef CFG_X86_PLATFORM
#include "halgdtidt_t.h"
#include "pages64_t.h"
#include "bdvideo_t.h"
#include "memmgrtypes.h"
#endif

#include "halglobal_t.h"
#include "halinit_t.h"
#include "halprint_t.h"



#endif // HALTYPES_H
