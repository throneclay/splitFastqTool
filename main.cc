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
typedef unsigned long ulong;

size_t readFile(size_t filelens)
{
    return fread(buffer, sizeof(char), filelens,fastqfile);
}

size_t writeFile(FILE *file,char* data, size_t filelens)
{
    return fwrite(data, sizeof(char), filelens, file);
}

ulong scanfile()
{
    ulong lines=0;
    ulong readsize=0;
    ulong sumsize=0;
    while(readsize=readFile(BUFFERSIZE)) 
    {
        sumsize+=readsize;
        for(int i=0;i<BUFFERSIZE;i++)
        {
            if(buffer[i]=='\n')
                lines++;
        }
    }
    printf("lines=%ld\n",lines);
    fclose(fastqfile);
    return lines; 
}
void splitFiles(ulong filelines)
{
    const int splitFactor = 2, splitDivision = 3,recordLines=4;
    ulong readsize=0, sumsize=0,curLines=0;
    int split=0,filechooser=0;

    ulong splitLines = filelines / splitDivision / recordLines;
    splitLines *= splitFactor *recordLines;

    printf("splitLines = %ld\n" , splitLines);

    while(readsize=readFile(BUFFERSIZE)) 
    {
        sumsize+=readsize;
        for(int i=0;i<BUFFERSIZE;i++)
        {
            if(buffer[i]=='\n')
                curLines++;
            if(buffer[i]=='\n'&&curLines==splitLines)
            {
                split=i;
                filechooser+=1;
                printf("split = %ld\n",split);
            }
        }
        if(split==0)
        {
            //printf("write file %d size = %d\n",filechooser,readsize);
            writeFile(splitfiles[filechooser],buffer,readsize);
        }
        else
        {
            //printf("start write file2 split = %ld\n",split);
            writeFile(splitfiles[filechooser-1],buffer, split); 
            writeFile(splitfiles[filechooser],buffer+split,readsize-split);
            split=0;
        }
    }

}

int main(int argc, char*argv[])
{
    char fastq[64]="i";
    char genfile[64]="o";
    char mode=0;
    ulong filelines=0;
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
    else
    {
        printf("%s [mode=0,1,2] infile outfile\n",argv[0]);
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
        buffer = (char*)malloc(sizeof(char)*BUFFERSIZE);
        filelines = scanfile();

        fastqfile=fopen(fastq, "r");
        splitFiles(filelines);
        break;
    }

    fclose(fastqfile);
    for (int i=0;i<splitsNum;i++)
        fclose(splitfiles[i]);
    free(buffer);
    return 0;
}
