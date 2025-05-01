#ifndef HASH_LOGIN_H
#define HASH_LOGIN_H

void hash_password_sha256(const char *password, char *output_buffer);

int unhex_sha256_hash(const char *hex_string, unsigned char *output_hash);

#endif
