#include <stdio.h>
#include <stdlib.h>

int file_reader(char* filename){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL){
        return 1;
        }
    char delim = '\t';
    while ((read = getline(&line, &len, fp)) != -1) {
        if(line[0]=='u'){
          int m='m';
          int s='s';

          printf("line: %s\n",line);
          char* loc_min= strchar(line, c)
          int min_digits=loc_min-5;
          char* loc_sec=strchar(line,m)

          char* min = strncpy(c)

          //the time starts at 5th index and
          printf("should only be units: %c blah blah blah\n",line[5]);
        }
      }
        //printf("Retrieved line of length %zu:\n", read);
      //  printf("%s", line);


    fclose(fp);
    if (line){
        free(line);
      }
    return 0;
}





int main(void )
{

    char* file1 = "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results4";
    char* file2 = "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results2";
    char* file3 = "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results3";
    char* file4 = "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results4";


    int exper1_times = file_reader(file1);
    if(exper1_times==EXIT_FAILURE){
      printf("Not read in properly\n file 1\n");
    }
    int exper2_times = file_reader(file2);
    if(exper2_times==EXIT_FAILURE){
      printf("Not read in properly\n file 2\n");
    }
    int exper3_times = file_reader(file3);
    if(exper3_times==EXIT_FAILURE){
      printf("Not read in properly\n file 3\n");
    }
    int exper4_times = file_reader(file4);
    if(exper4_times==EXIT_FAILURE){
      printf("Not read in properly\n file 3\n");
    }
    printf("ending correctly");

    return 0;
    //compile code: gcc file_extractor.c -o defiler


}
