#include "apicall.h"

#define NUM_CALLS 15

CURL* curl_handle;

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main() {
    char* icaoID = "KPHX";

    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    /* Init Curl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();


    char* data = malloc(20*sizeof(char)); // "ids=" + icaoID + "&format=raw";
    strcpy_s(data, 20, "ids=KPHX&format=raw");

    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://aviationweather.gov/api/data/metar");

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

    /* some servers do not like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    //char* retrievedString = malloc(chunk.size);

    /* Before we make the API call, we need to set up a file to write to. */
    FILE* f;
    f = fopen("C:/Users/etsel/Desktop/IEE380Project/C_Output.txt", "w");

    printf("Beginning API Calls...\n");

    for (int i = 0; i < NUM_CALLS; i++) {
        clock_t start, end;
        double time_elapsed_sec;

        start = clock();
        /* get it! */
        res = curl_easy_perform(curl_handle);
        /* check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            printf("Terminal Error w/ Curl request. Exiting.");
            free(chunk.memory);
            exit(2);
        }
        else {
            /*
             * Now, our chunk.memory points to a memory block that is chunk.size
             * bytes big and contains the remote file.
             *
             * Do something nice with it!
             */
            end = clock();
            time_elapsed_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
            fprintf(f, "%f\n",time_elapsed_sec);
        }

        Sleep(2 * 1000);
    }

    printf("Complete!\n");

    fclose(f); // Close the file

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    //free(chunk.memory);

    /* we are done with libcurl, so clean it up */
    curl_global_cleanup();

    return NULL;
}