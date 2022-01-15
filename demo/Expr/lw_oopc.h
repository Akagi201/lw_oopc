// Copyright (C) 2008,2009,2010 by Tom Kao & MISOO Team & Yonghua Jin. All rights reserved.
// Released under the terms of the GNU Library or Lesser General Public License (LGPL).
// Author: Tom Kao(中文名：高焕堂)，MISOO 团队，Yonghua Jin(中文名：金永华)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//    * Redistributions of source code must retain the above copyright
//	notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
//	copyright notice, this list of conditions and the following disclaimer
//	in the documentation and/or other materials provided with the
//	distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef LW_OOPC_H_INCLUDED_
#define LW_OOPC_H_INCLUDED_

#include <malloc.h>

// 配置宏(两种配置选其一):
#define LW_OOPC_USE_STDDEF_OFFSETOF     // 表示使用C标准定义的offsetof
// #define LW_OOPC_USE_USER_DEFINED_OFFSETOF // 表示使用用户自定义的lw_oopc_offsetof宏

// 是否支持内存泄露检测，缺省不支持
// #define LW_OOPC_SUPPORT_MEMORY_LEAK_DETECTOR

// 是否支持调试信息打印(内存分配和释放的详细信息），缺省关闭打印
// #define LW_OOPC_PRINT_DEBUG_INFO

#ifdef LW_OOPC_USE_STDDEF_OFFSETOF
#include <stddef.h>
#define LW_OOPC_OFFSETOF offsetof
#endif

#ifdef LW_OOPC_USE_USER_DEFINED_OFFSETOF
// 有些环境可能不支持，不过，这种情形极少出现
#define LW_OOPC_OFFSETOF(s,m) (size_t)&(((s*)0)->m)
#endif

typedef int lw_oopc_bool;
#define lw_oopc_true	(1)
#define lw_oopc_false	(0)

#ifdef LW_OOPC_SUPPORT_MEMORY_LEAK_DETECTOR

void* lw_oopc_malloc(size_t size, const char* type, const char* file, int line);
void lw_oopc_free(void* memblock);
void lw_oopc_report();

#define lw_oopc_file_line   __FILE__, __LINE__
#define lw_oopc_file_line_params const char* file, int line
#define lw_oopc_delete lw_oopc_free

#else

void lw_oopc_report();

#define lw_oopc_file_line
#define lw_oopc_file_line_params
#define lw_oopc_free free
#define lw_oopc_delete lw_oopc_free

#endif

#define INTERFACE(type)             \
typedef struct type type;           \
void type##_ctor(type* t);          \
int type##_dtor(type* t);           \
struct type

#define ABS_CLASS(type)             \
typedef struct type type;           \
void type##_ctor(type* t);          \
int type##_dtor(type* t);           \
void type##_delete(type* t);        \
struct type

#define CLASS(type)                 \
typedef struct type type;           \
type* type##_new(lw_oopc_file_line_params); \
void type##_ctor(type* t);          \
int type##_dtor(type* t);           \
void type##_delete(type* t);        \
struct type

#ifdef LW_OOPC_SUPPORT_MEMORY_LEAK_DETECTOR
#define CTOR(type)                                      \
    type* type##_new(const char* file, int line) {      \
    struct type *cthis;                                 \
    cthis = (struct type*)lw_oopc_malloc(sizeof(struct type), #type, file, line);   \
    if(!cthis)                                          \
    {                                                   \
        return 0;                                       \
    }                                                   \
    type##_ctor(cthis);                                 \
    return cthis;                                       \
}                                                       \
                                                        \
void type##_ctor(type* cthis) {
#else
#define CTOR(type)                                      \
    type* type##_new() {                                \
    struct type *cthis;                                 \
    cthis = (struct type*)malloc(sizeof(struct type));  \
    if(!cthis)                                          \
    {                                                   \
        return 0;                                       \
    }                                                   \
    type##_ctor(cthis);                                 \
    return cthis;                                       \
}                                                       \
                                                        \
void type##_ctor(type* cthis) {
#endif

#define END_CTOR	}

#define DTOR(type)                  \
void type##_delete(type* cthis)     \
{                                   \
        if(type##_dtor(cthis))      \
        {                           \
                lw_oopc_free(cthis);\
        }                           \
}                                   \
int type##_dtor(type* cthis)        \
{

#define END_DTOR }

#define ABS_CTOR(type)              \
void type##_ctor(type* cthis) {

#define END_ABS_CTOR }

#define FUNCTION_SETTING(f1, f2)	cthis->f1 = f2;

#define IMPLEMENTS(type)	struct type type

#define EXTENDS(type)		struct type type

#define SUPER_PTR(cthis, father) ((father*)(&((cthis)->father)))

#define SUPER_PTR_2(cthis, father, grandfather) \
	SUPER_PTR(SUPER_PTR(cthis, father), grandfather)

#define SUPER_PTR_3(cthis, father, grandfather, greatgrandfather) \
	SUPER_PTR(SUPER_PTR_2(cthis, father, grandfather), greatgrandfather)

#define SUPER_CTOR(father) \
	father##_ctor(SUPER_PTR(cthis, father));

#define SUB_PTR(selfptr, self, child) \
	((child*)((char*)selfptr - LW_OOPC_OFFSETOF(child, self)))

#define SUB_PTR_2(selfptr, self, child, grandchild) \
	SUB_PTR(SUB_PTR(selfptr, self, child), child, grandchild)

#define SUB_PTR_3(selfptr, self, child, grandchild, greatgrandchild) \
	SUB_PTR(SUB_PTR_2(selfptr, self, child, grandchild), grandchild, greatgrandchild)

#define INHERIT_FROM(father, cthis, field)	(cthis)->father.field

#endif
