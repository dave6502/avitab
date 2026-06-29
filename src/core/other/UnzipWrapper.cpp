#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <contrib/minizip/unzip.h>
#include "UnzipWrapper.h"

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

// Helper to make directories recursively if they don't exist
static int unzip::ensure_dir(const char *path) {
    char tmp[512];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;
    
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    return mkdir(tmp, 0755);
}

// The clean, 2-argument unzip wrapper function
int unzip::unzip(const char *src_file, const char *dst_dir) {
    unzFile uf = unzOpen64(src_file);
    if (uf == NULL) {
        fprintf(stderr, "Cannot open source zip file: %s\n", src_file);
        return -1;
    }

    unz_global_info64 global_info;
    if (unzGetGlobalInfo64(uf, &global_info) != UNZ_OK) {
        unzClose(uf);
        return -1;
    }

    // Go through every file in the zip archive
    for (uLong i = 0; i < global_info.number_entry; i++) {
        char filename_inzip[256];
        unz_file_info64 file_info;

        if (unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0) != UNZ_OK) {
            break;
        }

        // Construct the full destination path
        char full_dest_path[512];
        snprintf(full_dest_path, sizeof(full_dest_path), "%s/%s", dst_dir, filename_inzip);

        // Check if the entry is a directory (ends with '/')
        size_t name_len = strlen(filename_inzip);
        if (filename_inzip[name_len - 1] == '/') {
            ensure_dir(full_dest_path);
        } else {
            // It's a file. Ensure parent directory paths exist
            char *last_slash = strrchr(full_dest_path, '/');
            if (last_slash) {
                *last_slash = '\0';
                ensure_dir(full_dest_path);
                *last_slash = '/';
            }

            // Extract the file data
            if (unzOpenCurrentFile(uf) == UNZ_OK) {
                FILE *fout = fopen(full_dest_path, "wb");
                if (fout != NULL) {
                    char buffer[4096];
                    int read_bytes;
                    
                    // Stream chunks of decompressed data to the target file
                    do {
                        read_bytes = unzReadCurrentFile(uf, buffer, sizeof(buffer));
                        if (read_bytes > 0) {
                            fwrite(buffer, 1, read_bytes, fout);
                        }
                    } while (read_bytes > 0);
                    
                    fclose(fout);
                }
                unzCloseCurrentFile(uf);
            }
        }

        // Move to the next entry in the ZIP file
        if (i + 1 < global_info.number_entry) {
            if (unzGoToNextFile(uf) != UNZ_OK) break;
        }
    }

    unzClose(uf);
    return 0;
}