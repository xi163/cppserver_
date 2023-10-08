#ifndef INCLUDE_COLOR_H
#define INCLUDE_COLOR_H

#ifdef _windows_

#include "color_win.h"

static int const color[][2] = {
			{FOREGROUND_Red, FOREGROUND_LightRed},      //LVL_FATAL
			{FOREGROUND_Red, FOREGROUND_Purple},        //LVL_ERROR
			{FOREGROUND_Cyan, FOREGROUND_HighCyan},     //LVL_WARN
			{FOREGROUND_White, FOREGROUND_Red},         //LVL_CRITICAL
			{FOREGROUND_Pink, FOREGROUND_White},        //LVL_INFO
			{FOREGROUND_HighGreen, FOREGROUND_Gray},    //LVL_DEBUG
			{FOREGROUND_Yellow, FOREGROUND_LightYellow},//LVL_TRACE
};

#elif defined(_linux_)

#include "color_linux.h"

static int const color[][2] = {
		{FOREGROUND_Red, FOREGROUND_Cyan},     //LVL_FATAL
		{FOREGROUND_Red, FOREGROUND_Cyan},     //LVL_ERROR
		{FOREGROUND_Cyan, FOREGROUND_Purple},  //LVL_WARN
		{FOREGROUND_White, FOREGROUND_Red},    //LVL_CRITICAL
		{FOREGROUND_Purple, FOREGROUND_White}, //LVL_INFO
		{FOREGROUND_Green, FOREGROUND_Yellow}, //LVL_DEBUG
		{FOREGROUND_Yellow, FOREGROUND_Green}, //LVL_TRACE
};

#endif

#endif