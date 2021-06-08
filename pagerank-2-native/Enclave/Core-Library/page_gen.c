#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "common_rand.h"
#include "config.h"


void GeneratePages(int, int, char *);

struct page_link {
	uint64_t src;
	uint64_t dest;
};

void GeneratePages(int n, int divisor, char *page_file_path)
{
    int i, j, k, noutlinks;
    int *pages = malloc(sizeof(*pages)*n*n); // matrix 1 means link from j->i

    if (divisor <= 0) {
        fprintf(stderr, "ERROR: Invalid divisor '%d' for random initialization, divisor should be greater or equal to 1\n", divisor);
        exit(1);
    }

    uint64_t totalLinks = 0;

    for(i=0; i<n; ++i)
    {
    	noutlinks = 0;
        for(j=0; j<n; ++j)
        {
            if(i!=j && (abs(common_rand())%divisor == 0)){
                pages[i*n+j] = 1;
                noutlinks++;
            } else {
                pages[i*n+j] = 0;
            }
        }

        // the case with no outlinks is avoided
        if(noutlinks == 0)
        {
            do { k = abs(common_rand()) % n; } while ( k == i);
            pages[i*n + k] = 1;
            noutlinks++;
        }

        totalLinks += noutlinks;

    }

    struct page_link* page_data = (struct page_link*) malloc(totalLinks * sizeof(struct page_link));

    uint64_t page_relation_size = 0;
    for(int i=0; i < n; i++)
    {
        for(int j=0; j < n; j++)
        {
        	if(pages[i*n+j] == 1)
        	{
        		page_data[page_relation_size].src = i;
        		page_data[page_relation_size].dest = j;
        		page_relation_size++;
        	}
        }
    }

    FILE *fp = fopen(page_file_path, "w");
	
	for (int i=0; i<page_relation_size; ++i) {
        fwrite (&page_data[i], sizeof(struct page_link), 1, fp);
	}

	fclose(fp);

    printf("Number of Page Relations: %ld\n", page_relation_size);
    printf("Number of Pages: %d\n", n);
    printf("The file is generated at %s \n", page_file_path);

}

int main(int argc, char** argv) {

    GeneratePages(
        NUM_PAGES,
        DIVISOR,
        PAGE_FILE_PATH);

    return 0;
}