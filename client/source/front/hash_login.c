#include "../../header/hash_login.h"
#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
// fontion inverser le hashage

#include <stdlib.h>

#include <stdint.h>
#include <ctype.h>
#include "../../header/hash_login.h"

void hash_password_sha256(const char *password, char *output_buffer)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256((const unsigned char *)password, strlen(password), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(output_buffer + (i * 2), "%02x", hash[i]);
    }

    output_buffer[LARGE_STRING -1] = '\0'; // Null-terminate the string once, after the loop
}
