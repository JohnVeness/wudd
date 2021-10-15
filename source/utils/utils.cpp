#include <cstring>
#include <cstddef>
#include <whb/log.h>
#include "logger.h"
#include "utils.h"
#include "TinySHA1.hpp"
#include "../fs/FSUtils.h"

// https://gist.github.com/ccbrown/9722406
void Utils::dumpHex(const void *data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    DEBUG_FUNCTION_LINE_WRITE("0x%08X (0x0000): ", data);
    for (i = 0; i < size; ++i) {
        WHBLogWritef("%02X ", ((unsigned char *) data)[i]);
        if (((unsigned char *) data)[i] >= ' ' && ((unsigned char *) data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char *) data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i + 1) % 8 == 0 || i + 1 == size) {
            WHBLogWritef(" ");
            if ((i + 1) % 16 == 0) {
                WHBLogPrintf("|  %s ", ascii);
                if (i + 1 < size) {
                    DEBUG_FUNCTION_LINE_WRITE("0x%08X (0x%04X); ", ((uint32_t) data + i + 1), i + 1);
                }
            } else if (i + 1 == size) {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8) {
                    WHBLogWritef(" ");
                }
                for (j = (i + 1) % 16; j < 16; ++j) {
                    WHBLogWritef("   ");
                }
                WHBLogPrintf("|  %s ", ascii);
            }
        }
    }
}


std::string Utils::calculateSHA1(const char *buffer, size_t size) {
    sha1::SHA1 s;
    s.processBytes(buffer, size);
    uint32_t digest[5];
    s.getDigest(digest);
    char tmp[48];
    snprintf(tmp, 45, "%08X%08X%08X%08X%08X", digest[0], digest[1], digest[2], digest[3], digest[4]);
    return tmp;
}

std::string Utils::hashFile(const std::string &path) {
    uint8_t *data = NULL;
    uint32_t size = 0;
    FSUtils::LoadFileToMem(path.c_str(), &data, &size);
    if (data == NULL) {
        return calculateSHA1(NULL, 0);
    }
    std::string result = calculateSHA1(reinterpret_cast<const char *>(data), size);
    free(data);
    return result;
}

unsigned int swap_uint32(unsigned int val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

unsigned long long swap_uint64(unsigned long long val) {
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | (val >> 32);
}

/*
 * Hash function used to create a hash of each sector
 * The hashes are then compared to find duplicate sectors
 */
void calculateHash256(unsigned char *data, unsigned int length, unsigned char *hashOut) {
    // cheap and simple hash implementation
    // you can replace this part with your favorite hash method
    memset(hashOut, 0x00, 32);
    for (unsigned int i = 0; i < length; i++) {
        hashOut[i % 32] ^= data[i];
        hashOut[(i + 7) % 32] += data[i];
    }
}