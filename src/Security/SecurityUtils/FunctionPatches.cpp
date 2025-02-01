#include "../Security.h"

DWORD security::calculate_function_checksum(PVOID func_address) {
    DWORD func_size = security::detect_function_size(func_address);
    crc_t crc;
    crc = crc_init();
    crc = crc_update(crc, (unsigned char*)func_address, func_size);
    return (DWORD)crc;
}

size_t security::detect_function_size(PVOID pFunc) {
    PBYTE pMem = (PBYTE)pFunc;
    size_t nFuncSize = 0;
    do
    {
        ++nFuncSize;
    } while (*(pMem++) != 0xC3);
    return nFuncSize;
}