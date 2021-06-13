#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#define NODEBUG 0
#define TMPINFO 1
#define INFO 2
#define INFOENC 2
#define INFOREADWRITE 3
#define VERBOSE 4
#define VVERBOSE 5
#define MISS 6
#define ALL 7


#define ISDEBUG NODEBUG

#define AES_BLOCK_SIZE 16
#define HASH_SIZE 32
#define AES_KEY_SIZE 32

#define CHUNK_DISK_SIZE 4096
#define CHUNK_DATA_SIZE_BYTES (CHUNK_DISK_SIZE-AES_BLOCK_SIZE-HASH_SIZE)  // 4KB 4080 - 32 = 4048 // 4096-32 = 4064

// /* A 256 bit key */
// char *key = (char *)"01234567890123456789012345678901";

/* A 128 bit IV */
// char *iv = (char *)"0123456789012345";
uint8_t constiv[] = {0x01, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t const_key[] = {
0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                       0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};


uint8_t *hash_bytes;
char *enc_filename="datax_enc.csv";
char *dec_filename="datax_dec.csv";


void handleEncDecErrors(char *msg) {
	printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
	printf("errors OPENSSL Model.c %s\n", msg);
	printf("Very much possible (almost certain) that you passed an INVALID Key for decryption.\n");
    abort();
}


int simpleSHA256(uint8_t *input, uint32_t len, uint8_t *output) {
	EVP_MD_CTX *ctx;


	/* Create and initialise the context */
	if (!(ctx = EVP_MD_CTX_new())) {
		handleEncDecErrors("Encrypt ctx");
		return -1;
	}

	if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
		handleEncDecErrors("encrypt_securefs init");
		return -1;
	}

	if (1 != EVP_DigestUpdate(ctx, input, len)) {
		handleEncDecErrors("encrypt_securefs update");
		return -1;
	}
	unsigned int lengthOfHash = 0;
	if (1 != EVP_DigestFinal_ex(ctx, output, &lengthOfHash)) {
		handleEncDecErrors("enc final");
		return -1;
	}

	/* Clean up */
	EVP_MD_CTX_free(ctx);
	return 0;
}


int decrypt(uint8_t *ciphertext, int ciphertext_len, uint8_t *key,
					 uint8_t *iv, uint8_t *plaintext) {

	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		handleEncDecErrors("dec ctx");
		return -1;
	}
	EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);


	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv)) {
		handleEncDecErrors("dec init");
		return -1;
	}

	if (ISDEBUG >= ALL)
		printf("key len %d, iv len %d\n", EVP_CIPHER_key_length(EVP_aes_256_cbc()),
			   EVP_CIPHER_iv_length(EVP_aes_256_cbc()));


	if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
		handleEncDecErrors("dec update");
		return -1;
	}
	plaintext_len = len;

	if (ISDEBUG >= ALL) {
		printf("DEC: plaintext len before final %d\n", plaintext_len);
//        printf_ssl_dec_info(ciphertext, ciphertext_len, key, iv, plaintext);
	}
	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if (!EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
		printf("\n--------- ERROR ---------------\n");
		printf("Key used:");
		handleEncDecErrors("dec final");
		return -1;
	}
	plaintext_len += len;

	if (ISDEBUG >= ALL) {
		printf("DEC: plaintext len AFTER final %d\n", plaintext_len);
		printf_ssl_dec_info(ciphertext, ciphertext_len, key, iv, plaintext);
	}

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);


	return plaintext_len;
}

int encrypt(uint8_t *plaintext, int plaintext_len, uint8_t *key,
					 uint8_t *iv, uint8_t *ciphertext) {

	EVP_CIPHER_CTX *ctx;

	int len, ciphertext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		handleEncDecErrors("Encrypt ctx");
		return -1;
	}
	EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv)) {
		handleEncDecErrors("encrypt_securefs init");
		return -1;
	}

	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
		handleEncDecErrors("encrypt_securefs update");
		return -1;
	}
	ciphertext_len = len;

	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
		handleEncDecErrors("enc final");
		return -1;
	}
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

void print_data(char *data, uint64_t len){
    if (ISDEBUG==INFO){
        for(int i=0;i<len;i++){
            printf("%x",data[i]);
        }
        printf("\n");
    }
}

void print_hash(void){
    for(int i=0;i<32;i++){
        printf("%x",hash_bytes[i]);
    }
    printf("\n");
}

void call_decrypt(){

    uint64_t file_size;
    char *new_bytes;
    char *dec_bytes;

    ocall_file_stat(enc_filename, &file_size);
    new_bytes =(char *)malloc(file_size);

    uint64_t pos;

    for(uint64_t i = 0; i< file_size; i+=CHUNK_DISK_SIZE)
    {
        pos = ((i+CHUNK_DISK_SIZE) < file_size) ? CHUNK_DISK_SIZE : (file_size-i);
        ocall_read_file(enc_filename, (new_bytes + i), pos, i);
    }

    printf("\nCall Decrypt: Stat of the file: %ld \n", file_size);
    print_data(new_bytes, file_size);

    printf("Its Hash:");
    simpleSHA256(new_bytes, file_size, hash_bytes);
    print_hash();

    uint64_t new_len = file_size-AES_BLOCK_SIZE;
    printf("new len after encryption : %lu \n", new_len);
    dec_bytes =(char *)malloc(new_len+AES_BLOCK_SIZE);
   
    /* Decrypt the ciphertext */
    if(new_bytes)
    {   
        printf("\nDec: Decrypting...:\n");
        decrypt (new_bytes, file_size, const_key, constiv, dec_bytes);
        printf("Its hash:");
        simpleSHA256(dec_bytes, new_len, hash_bytes);
        print_hash();
    }

    printf("new len after encryption : %lu \n", new_len);

    for(uint64_t i = 0; i< new_len; i+=CHUNK_DISK_SIZE)
    {
        // printf("DEC %s %d \n",dec_filename,i);
        pos = ((i+CHUNK_DISK_SIZE) < new_len) ? CHUNK_DISK_SIZE : (new_len - i);
        ocall_write_file(dec_filename, (dec_bytes + i), pos,i);
    }


    free(new_bytes);
    free(dec_bytes);
}

int ecall_real_main (void)
{

    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */


    #ifdef HIGH_
    char *filename="datax_high.csv";
    #elif LOW_
    char *filename="datax_low.csv";
    #else
    char *filename="datax_medium.csv";
    #endif

    hash_bytes = (uint8_t *)malloc(HASH_SIZE);

    uint64_t file_size;
    char *new_bytes, *enc_bytes;

    ocall_file_stat(filename, &file_size);
    ocall_file_load(filename, file_size);

    new_bytes =(char *)malloc(file_size);
    enc_bytes =(char *)malloc(file_size + AES_BLOCK_SIZE);

    uint64_t pos;

    printf("Reading the file.\n");
    for(uint64_t i = 0; i< file_size; i+=CHUNK_DISK_SIZE)
    {
        pos = ((i+CHUNK_DISK_SIZE) < file_size) ? CHUNK_DISK_SIZE : (file_size-i);
        ocall_read_file(filename, new_bytes + i, pos, i);
    }
    printf("Reading the file... Done\n");
    if (ISDEBUG==INFO){
        printf("The plain file size is: %ld \n", file_size);
        printf("Plain Text: %s \n", new_bytes);
    }
    printf("Its Hash:");
    simpleSHA256(new_bytes, file_size, hash_bytes);
    print_hash();

    uint64_t new_len = file_size+AES_BLOCK_SIZE;

    printf("\nEncrypting..:\n");
    if(new_bytes)
    {
        encrypt (new_bytes, file_size, const_key, constiv, enc_bytes);

        printf("Encrypting.. Done\n");
        printf("The size encrypted file is: %ld \n", new_len);
        print_data(enc_bytes, new_len);
        printf("Its Hash: ");
        simpleSHA256(enc_bytes, new_len, hash_bytes);
        print_hash();
    }

   


    for(uint64_t i = 0; i< new_len; i+=CHUNK_DISK_SIZE)
    {
        pos = ((i+CHUNK_DISK_SIZE) < new_len) ? CHUNK_DISK_SIZE : (new_len-i);
        // printf("enc call ocall_write: Writing %0X at %d of size %d\n",(enc_bytes + i), i, pos);
        ocall_write_file(enc_filename, (enc_bytes + i), pos, i);// filname, content, size
    }

    free(enc_bytes);
    free(new_bytes);
    

    // **********************************************
    // ********************ENCRPT DONE**************
    // **********************************************
    // **********************************************


    call_decrypt();


    /* Message to be encrypted */
    char *plaintext = (char *)"The quick brown fox jumps over the lazy dog";

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    char ciphertext[128];

    /* Buffer for the decrypted text */
    char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), const_key, constiv,
                              ciphertext);


    /* Do something useful with the ciphertext here */
    // printf("Ciphertext is:%s\n", ciphertext);
    //BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, const_key, constiv,
                                decryptedtext);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    /* Show the decrypted text */
    //printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);

    return 0;
}
