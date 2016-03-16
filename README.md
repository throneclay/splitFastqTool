## splitFastqTool

This is a tool to split fastq file.

mode 0 can split fastq file into 2 files. This function is similar to the Linux command "split", It just split 2 part based on the number of lines. You can define the lines in each file.

mode 1 can split fastq file into 4 files. The 4 files is title file, dna file, title2 file and quality file. Based on the structure of the fastq file, It is used to test the performance of different compression algorithm working on different part of a fastq file. I will design a hybrid compression algorithm for bio-data, like fastq and fasta.
