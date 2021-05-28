#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
// #include <iostream>
// #include <fstream>
#include <stdlib.h>
// #include <cmath>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

    char *enc_filename="/tmp/datax_enc.csv";


int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";


void call_decrypt(){

    // char *filename="/tmp/datax_enc.csv";
    char *dec_filename="/tmp/datax_dec.csv";
 int fd;
    struct stat s;
    char *new_bytes, *dec_bytes;


    fd=open(enc_filename,O_RDONLY);
    printf("call_decrypt %s FD of opened files is %d\n",enc_filename,fd);
    if(fd > 0){
      s.st_size=0;
      stat(enc_filename, &s);
      printf("SecureFS: Size of the file is %ld \n",s.st_size);
      new_bytes =(char *)malloc(s.st_size);
      dec_bytes =(char *)malloc(s.st_size+20);
      read(fd,new_bytes, s.st_size);
    }  else{
        perror("open");
        exit (1);
      
    }
  close(fd);


      /* Encrypt the plaintext */
    if(new_bytes){
      int new_len = decrypt (new_bytes, s.st_size, key, iv,
                              dec_bytes);
      int efd=open(dec_filename,O_CREAT|O_WRONLY,777);
      printf("call_decrypt efd of opened files is %d\n",efd);
      write(efd,dec_bytes,new_len);
      close(efd);
    }
}

int ecall_real_main (void)
{

    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */


    /* Message to be encrypted */
    unsigned char *plaintext =
        (unsigned char *)"The quick brown fox jumps over the lazy dog";

   
    int fd;
    struct stat s;

    char *filename="/tmp/datax.csv";

    fd=open(filename,O_RDONLY);
    char *new_bytes, *enc_bytes;
    printf("main FD of opened files is %d\n",fd);
    if(fd > 0){
      s.st_size=0;
      stat(filename, &s);
      printf("SecureFS: Size of the file is %ld \n",s.st_size);
      new_bytes =(char *)malloc(s.st_size);
      enc_bytes =(char *)malloc(s.st_size+20);
      read(fd,new_bytes, s.st_size);
    }  
  close(fd);
   
      /* Encrypt the plaintext */
    if(new_bytes){
      int new_len = encrypt (new_bytes, s.st_size, key, iv,
                              enc_bytes);
      int efd=open(enc_filename,O_CREAT|O_WRONLY,777);
      
      printf("main %s efd of opened files is %d\n",enc_filename, efd);
      if(efd==-1){
          perror("open");
        exit (1);
      }
      write(efd,enc_bytes,new_len);
      close(efd);
    }
    free(enc_bytes);
    free(new_bytes);
    call_decrypt();
    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char ciphertext[128];

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                              ciphertext);


    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                decryptedtext);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    /* Show the decrypted text */
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);

    printf("SECUREFS_TIME %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    return 0;
}