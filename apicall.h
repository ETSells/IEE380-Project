#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <Windows.h>

#include <curl/curl.h>

#define CURLOPT_ACCEPT_ENCODING 1 // Significantly increases the speed of LibCurl

#define CURLOPT_VERBOSE NULL

struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
int main();