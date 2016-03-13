// The purpose of this program is to make a file distribute program
// Also, I want use this to test how much can different line influnt
// the compression algorithm.

#include <stdio.h>
#include <string.h>
#include <offload.h>
#define BUFFERSIZE (1<<23)
char* buffer;

FILE *fastqfile;
FILE *splitfiles[4];
int splitsNum = 0;

size_t readFile(FILE *file,char* buffer, size_t filelens)
{
}

size_t writeFile(FILE *file,char* data, size_t filelens)
{
    if(data==NULL)
        return -1;
    if(filelens==0)
        return -1;
    return fwrite(data, 1, filelens, file);
}

long scanfile()
{
    long lines=0;
    printf("lines=%ld\n",lines);
    return lines; 
}

void genFiles(long filelines)
{

}

int main(int argc, char*argv[])
{
    char fastq[40]="i";
    char genfile[40]="o";
    char mode=0;
    long filelines=0;
    if(argc==4)
    {
        mode = atoi(argv[1]);
        strncpy(fastq, argv[2],40);
        strncpy(genfile, argv[3],40);
    }
    else if(argc==3)
    {
        strncpy(fastq, argv[2],40);
        strncpy(genfile, argv[3],40);
    }
    else if(argc==2)
    {
        printf("[mode=0,1,2] infile outfile\n");
        exit(1);
    }
    fastqfile=fopen(fastq, "r");
    switch(mode)
    {
case 0:
        splitsNum = 2;
        int strl = strlen(genfile);
        for(int i=0;i<splitsNum;i++)
        {
            genfile[strl] = i+'0';
            genfile[strl+1] = '\0';
            splitfiles[i] = fopen(genfile, "a+");
        }
        buffer = malloc(sizeof(char)*BUFFERSIZE);
        filelines = scanfile();
        genFiles(filelines);
        break;
    }

    fclose(fastqfile);
    for (int i=0;i<splitsNum;i++)
        fclose(splitfiles[i]);
    free(buffer);
    return 0;
}
