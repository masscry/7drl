/**
 * @file error.h
 * @author timur
 *
 * Error handling functions.
 *
 */

#pragma once

void errorExpect(bool cond, const char* condt, const char* file, int line);

#define EXPECT(cond) errorExpect(cond, #cond, __FILE__, __LINE__);
