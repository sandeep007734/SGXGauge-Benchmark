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


/* A 256 bit key */
unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
/* A 128 bit IV */
unsigned char *iv = (unsigned char *)"0123456789012345";
char filename[100],enc_filename[100],dec_filename[100];
    uint8_t *hash_bytes;

void find_prime(){
    int i, num = 1, primes = 0;

    while (num <= 500000) { 
        i = 2; 
        while (i <= num) { 
            if(num % i == 0)
                break;
            i++; 
        }
        if (i == num)
            primes++;

        // system("clear");
        // printf("%d prime numbers calculated\n",primes);
        num++;
    }
    printf("%d prime numbers calculated\n",primes);

}

void print_hash(void){
    for(int i=0;i<32;i++){
        printf("%x",hash_bytes[i]);
    }
    printf("\n");
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}



 void block_write(int fd, char *bytes, size_t len){
    // size_t block_size=4096; // 4KB

    // int total_blocks = len/block_size;
    // int remain = len % block_size;

    // for(int idx=0;idx<total_blocks;idx++){
    //     write(fd,bytes+idx*block_size,block_size);
    // }
    // write(fd,bytes+total_blocks*block_size,remain);
    write(fd,bytes,len);
}

void block_read(int fd, char *bytes, size_t len){
    // size_t block_size=4096; // 4KB

    // int total_blocks = len/block_size;
    // int remain = len % block_size;

    // for(int idx=0;idx<total_blocks;idx++){
    //     read(fd,bytes+idx*block_size,block_size);
    // }
    // read(fd,bytes+total_blocks*block_size,remain);
    read(fd,bytes,len);
}


int simpleSHA256(uint8_t *input, uint32_t len, uint8_t *output) {
    // memset(output,0,32);
    // printf("Input to hash is: \n");
    // for(int i=0;i<20;i++){
    //     printf("%c",input[i]);
    // }
    // printf("\n");

	EVP_MD_CTX *ctx;    

	/* Create and initialise the context */
	if (!(ctx = EVP_MD_CTX_new())) {
		handleErrors();
		return -1;
	}

	if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
		handleErrors();
		return -1;
	}

	if (1 != EVP_DigestUpdate(ctx, input, len)) {
		handleErrors();
		return -1;
	}
	unsigned int lengthOfHash = 0;
	if (1 != EVP_DigestFinal_ex(ctx, output, &lengthOfHash)) {
		handleErrors();
		return -1;
	}

	/* Clean up */
	EVP_MD_CTX_free(ctx);
	return 0;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();


    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    printf("The plaintext is at: %u \n", plaintext);
    printf("The len is at: %d \n", len);

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

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

void call_decrypt()
{

    int fd;
    struct stat s;
    char *new_bytes, *dec_bytes;

    fd = open(enc_filename, O_RDONLY);
    printf("call_decrypt %s FD of opened files is %d\n", enc_filename, fd);
    if (fd > 0)
    {
        s.st_size = 0;
        stat(enc_filename, &s);

        printf("SecureFS: Size of the file is %ld \n", s.st_size);
        new_bytes = (char *)malloc(s.st_size);
        dec_bytes = (char *)malloc(s.st_size - 20);
        block_read(fd, new_bytes, s.st_size);
    }
    else
    {
        perror("open");
        exit(1);
    }
    close(fd);

    simpleSHA256(new_bytes, s.st_size, hash_bytes);
    print_hash();

    /* Encrypt the plaintext */
    int new_len = decrypt(new_bytes, s.st_size, key, iv,
                          dec_bytes);
    simpleSHA256(dec_bytes, new_len, hash_bytes);
    print_hash();


    int dfd = open(dec_filename, O_CREAT | O_WRONLY, 0644);
    if (dfd == -1)
    {
        fprintf(stderr, "Error while opening %s\n", dec_filename);
        perror("open");
        exit(1);
    }
    // printf("\n--\ncall_decrypt efd of opened files is %d\n", dfd);
    block_write(dfd, dec_bytes, new_len);
    close(dfd);

    free(new_bytes);
    free(dec_bytes);
}

int main(int argc, char *argv[])
{
     struct timeval stop, start;
     char cmd[200];

    #ifdef NONPF
        printf("In NON PF Mode\n");
        strcpy(filename,"/tmp/datax.csv");
    #else
        printf("In PF Mode\n");
        strcpy(filename,"/tmp/datax_pf.csv");
    #endif

    

    strcpy(enc_filename,"/tmp/datax_enc_1.csv");
    strcpy(dec_filename,"/tmp/datax_dec_1.csv");

    // sprintf(cmd,"rm -f %s", enc_filename);
    // system(cmd);
    // sprintf(cmd,"rm -f %s", dec_filename);
    // system(cmd);

    // if(argc > 3){
    //     printf("\n--\nCopying the arguments %s %s %s\n---\n",argv[1],argv[2],argv[3]);
    //     strcpy(filename, argv[1]);
    //     strcpy(enc_filename, argv[2]);
    //     strcpy(dec_filename, argv[3]);
    // }
    

    fprintf(stderr, "argc is %d Files are %s and %s\n",argc,filename,enc_filename);

      gettimeofday(&start, NULL);

    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */


    /* Message to be encrypted */
    unsigned char *plaintext =
        (unsigned char *)"The quick brown fox jumps over the lazy dog";

   
    int fd;
    struct stat s;
    hash_bytes = (uint8_t *)malloc(32);

    // -----------------------------------
    fd=open(filename,O_RDONLY);
    char *new_bytes, *enc_bytes;
    printf("main FD of opened files is %d\n",fd);
    if(fd > 0){
      s.st_size=0;
      stat(filename, &s);
      printf("SecureFS: Size of the file is %ld \n",s.st_size);
      new_bytes =(char *)malloc(s.st_size);
      enc_bytes =(char *)malloc(s.st_size+20);
      block_read(fd,new_bytes, s.st_size);
    }else{
        perror("open");
        exit(1);
    }
    simpleSHA256(new_bytes, s.st_size, hash_bytes);
    print_hash();

    close(fd); // reading the plain data done.

    // -----------------------------------
      /* Encrypt the plaintext */
    if (new_bytes)
    {
        int new_len = encrypt(new_bytes, s.st_size, key, iv,
                              enc_bytes);

        simpleSHA256(enc_bytes, new_len, hash_bytes);
        print_hash();

        int efd = open(enc_filename, O_CREAT|O_WRONLY,0644);

        printf("\n---\nmain %s efd of opened files is %d\n", enc_filename, efd);
        if (efd == -1)
        {
            perror("open");
            exit(1);
        }
        block_write(efd, enc_bytes, new_len);
        close(efd);
    }
    free(enc_bytes);
    free(new_bytes);

    // -----------------------------------
    call_decrypt();
    // -----------------------------------
    //find_prime();

    // -----------------------------------
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    int decryptedtext_len, ciphertext_len;
    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,         ciphertext);

    printf("\n---\nCiphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                decryptedtext);
    decryptedtext[decryptedtext_len] = '\0';

    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);
    // -----------------------------------

    free(hash_bytes);
    gettimeofday(&stop, NULL);
    printf("SECUREFS_TIME %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 

    return 0;
}

