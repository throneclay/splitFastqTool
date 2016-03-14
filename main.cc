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
    // 扫描文件，并检查文件的行数
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
    fclose(fastqfile);
    return lines; 
}
void splitFiles(ulong filelines)
{
    //设置划分比例 按照文件的record进行划分 splitLines = filerecord * splitFactor / splitDivision
    const int splitFactor = 2, splitDivision = 3,recordLines=4;

    ulong readsize=0, sumsize=0,curLines=0;
    int split=0,filechooser=0;

    // 计算要划分的行数
    ulong splitLines = filelines / splitDivision / recordLines;
    splitLines *= splitFactor *recordLines;

    // 进行划分
    while(readsize=readFile(BUFFERSIZE)) 
    {
        sumsize += readsize;
        for(int i=0;i<BUFFERSIZE;i++)
        {
            if(buffer[i]=='\n')
                curLines++;
            // 检查是否到达划分点
            if(buffer[i]=='\n'&&curLines==splitLines)
            {
                split=i;
                filechooser+=1;
                printf("split = %ld\n",split);
            }
        }
        if(split==0)
        {
            // 如果读取的文件还不到划分点，放入filechooser选定的文件
            writeFile(splitfiles[filechooser],buffer,readsize);
        }
        else
        {
            // 如果读取的文件到达划分点，按照split的指示，放入filechooser选定的前后两个文件
            writeFile(splitfiles[filechooser-1],buffer, split); 
            writeFile(splitfiles[filechooser],buffer+split,readsize-split);
            split=0;
        }
    }

}

void splitDiffPart()
{
    char* buff[4];
    for(int i=0;i<4;i++)
        buff[i] = (char*)malloc(sizeof(char)*BUFFERSIZE);

    int index[4] = {0};
    int start=0, end=0, indexChooser=0;

    while(readFile(BUFFERSIZE)) 
    {
        start=0;
        end=0;
        indexChooser = 0;
        index[0]=0; index[1]=0; index[2]=0; index[3]=0;
        for(int i=0;i<BUFFERSIZE;i++)
        {
            if(buffer[i]=='\n')
            {
                end=i;
                memcpy(buff[indexChooser]+index[indexChooser],buffer+start,end-start+1);
                index[indexChooser]+=end-start+1;

                indexChooser=(indexChooser==3?0:indexChooser+1);
                start=i+1;
            }
        }
        for(int i=0;i<4;i++)
        {
            writeFile(splitfiles[i], buff[i],index[i]);
        }
    }
    for(int i=0;i<4;i++)
        free(buff[i]);

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
    buffer = (char*)malloc(sizeof(char)*BUFFERSIZE);
    int strl = 0;
    switch(mode)
    {
case 0:
        splitsNum = 2;
        strl = strlen(genfile);
        for(int i=0;i<splitsNum;i++)
        {
            genfile[strl] = i+'0';
            genfile[strl+1] = '\0';
            splitfiles[i] = fopen(genfile, "a+");
        }
        filelines = scanfile();

        fastqfile=fopen(fastq, "r");
        splitFiles(filelines);
        break;
case 1:
        splitsNum = 4;
        strl = strlen(genfile);
        for(int i=0;i<splitsNum;i++)
        {
            genfile[strl] = i+'0';
            genfile[strl+1] = '\0';
            splitfiles[i] = fopen(genfile, "a+");
        }
        splitDiffPart();
    }

    fclose(fastqfile);
    for (int i=0;i<splitsNum;i++)
        fclose(splitfiles[i]);
    free(buffer);
    return 0;
}
