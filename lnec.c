#include <stdio.h>
#include <cstdint>
#include <string.h>
#include <stdlib.h>


/* Encrypt */
uint32_t pass_v(const char* pass) {
    size_t len = strlen(pass);
    uint32_t pv = 0;
    for (uint32_t i = 0; i < len; i++) {
        switch ((bool)pass[i] % 2) {
        case true: //odd
            pv += pass[i];
            break;
        case false: //even
            pv += (pass[i] / 2);
            break;
        }
    }
    return pv;
}
void encrypt(const char* str, float* output, const char* pass) {
    uint32_t pv = pass_v(pass);
    size_t len = strlen(str);
    for (uint32_t i = 0; i < len; i++) {
        output[i] = (pv * str[i]);
    }
}
bool writetofile(float* output, size_t len, const char* ofile) {
    FILE* f;
    fopen_s(&f, ofile, "w");

    if (!f) { return 0; }

    for (unsigned int x = 0; x < len; x++) {
        fprintf(f, "%1.f.", output[x]);
        if (!(x % 6)) { //newline every 6 lines
            if (x > 0) {
                fprintf(f, "\n");
            }
        }
    }
    fclose(f);
    return 1;
}

size_t readfromfile(float* buf, const char* fname){ //returns strlen
    FILE* f;
    fopen_s(&f, fname, "r");
    if (!f) { printf("rip"); return 0; }

    const int rsize = 1024;
    char fbuf[rsize];
    size_t enc_size = 0;

    while (fgets(fbuf, rsize, f)) {
        char t_buf[rsize / 6];
        int to_write = 0;

        for (int i = 0; fbuf[i] != NULL; i++) {
            switch (fbuf[i] == '.') {
            case true: //reset
                buf[enc_size] = strtof(t_buf, nullptr);
                enc_size++;
                to_write = 0;
                memset(t_buf, '\0', sizeof(t_buf));
                break;
            case false:
                t_buf[to_write] = fbuf[i];
                to_write += 1;
                break;
            }
        }
    }
    return (enc_size);
}
/* Decrypt */
void decrypt(float* enc_data, size_t len, char* output, const char* pass) {
    uint32_t pv = pass_v(pass);
    for (uint32_t i = 0; i < len; i++) {
        output[i] = (enc_data[i] / pv);
    }
    output[len] = '\0'; //add null terminator
}
