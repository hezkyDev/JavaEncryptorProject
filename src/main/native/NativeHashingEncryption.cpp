#include <jni.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <string>
#include <vector>

// Function to Base64 encode the encrypted data using OpenSSL
std::string base64Encode(const unsigned char* buffer, size_t length) {
    BIO* bio = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    bio = BIO_push(bio, mem);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    BIO_write(bio, buffer, length);
    BIO_flush(bio);

    BUF_MEM* memPtr;
    BIO_get_mem_ptr(bio, &memPtr);

    std::string base64Encoded(memPtr->data, memPtr->length);
    BIO_free_all(bio);

    return base64Encoded;
}

// JNI method that Java will call
extern "C" JNIEXPORT jstring JNICALL
Java_StringEncryptor_encryptString(JNIEnv* env, jobject obj, jstring input) {
    const char* inputStr = env->GetStringUTFChars(input, NULL);

    // Perform SHA-256 hashing on the input string
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)inputStr, strlen(inputStr), hash);

    // Load the public key for encryption
    FILE* pubKeyFile = fopen("src/main/resources/pi-xcels_public_key.pem", "r");
    if (!pubKeyFile) {
        env->ReleaseStringUTFChars(input, inputStr);
        return env->NewStringUTF("Error: Unable to open public key file");
    }

    EVP_PKEY* pubKey = PEM_read_PUBKEY(pubKeyFile, NULL, NULL, NULL);
    fclose(pubKeyFile);
    if (!pubKey) {
        env->ReleaseStringUTFChars(input, inputStr);
        return env->NewStringUTF("Error: Unable to load public key");
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pubKey, NULL);
    if (!ctx) {
        EVP_PKEY_free(pubKey);
        env->ReleaseStringUTFChars(input, inputStr);
        return env->NewStringUTF("Error: Failed to create context");
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_free(pubKey);
        EVP_PKEY_CTX_free(ctx);
        env->ReleaseStringUTFChars(input, inputStr);
        return env->NewStringUTF("Error: Failed to initialize encryption");
    }

    unsigned char encrypted[256];
    size_t encryptedLen = sizeof(encrypted);
    if (EVP_PKEY_encrypt(ctx, encrypted, &encryptedLen, hash, SHA256_DIGEST_LENGTH) <= 0) {
        EVP_PKEY_free(pubKey);
        EVP_PKEY_CTX_free(ctx);
        env->ReleaseStringUTFChars(input, inputStr);
        return env->NewStringUTF("Error: Encryption failed");
    }

    EVP_PKEY_free(pubKey);
    EVP_PKEY_CTX_free(ctx);

    // Base64 encode the encrypted result
    std::string base64Encrypted = base64Encode(encrypted, encryptedLen);

    env->ReleaseStringUTFChars(input, inputStr);

    // Return the Base64 encoded encrypted string to Java
    return env->NewStringUTF(base64Encrypted.c_str());
}