/*
 * Swarthmore College, CS 31
 * Copyright (c) 2019 Swarthmore College Computer Science Department,
 * Swarthmore PA, Professors Tia Newhall and Kevin Webb and Vasanta Chaganti
 */

#include <pthread.h>
#include <pthreadGridVisi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include "colors.h"

#define OUTPUT_NONE (0)
#define OUTPUT_TEXT (1)
#define OUTPUT_VISI (2)

/* For counting the number of live cells in each round. */
static int live = 0;

/* These values need to be shared by all the threads for visualization.
 * You do NOT need to worry about synchronized access to these, the graphics
 * library will take care of that for you. */
static visi_handle handle;
static color3 *image_buf;
static pthread_mutex_t lock;
static pthread_barrier_t barrier;

/* This struct represents all the data you need to keep track of your GOL
 * simulation.  Rather than passing individual arguments into each function,
 * we'll pass in everything in just one of these structs.
 *
 * NOTE: You need to use the provided fields here, but you'll also need to
 * add some of your own. */
struct gol_data {
    /* The number of rows on your GOL game board. */
    int rows;

    /* The number of columns on your GOL game board. */
    int cols;
    /* Start and end row for each thread; start and end column for each thread*/
    int start_row;
    int end_row;
    int start_col;
    int end_col;
    /* The number of threads requested by the user */
    int num_threads;
    /* The number of iterations to run your GOL simulation. */
    int iters;
    /* Type of partition requested by user */
    int partition;
    /* Counting the number of alive cells per thread per round */
    int alive_counted;

    /* Which form of output we're generating:
     * 0: only print the first and last board (good for timing).
     * 1: print the board to the terminal at each round.
     * 2: draw the board using the visualization library. */
    int output_mode;
    int id;
    int* board;
    int* nextStepBoard;
    int numAlive;
    int* indicesAlive;

    /* Type of print partition requested by user */
    int print_partition;

    int start_index;
    int end_index;


};

void initializer(char* file_name, int* mode, struct gol_data* data);
void nextstepcalc(struct gol_data* data, int i, int j, int index);
int getNeighbors(int x, int y, struct gol_data* data);
int checkNeighbor(int x, int y, struct gol_data* data);
void print_board(struct gol_data *data, int round);
void initializer_fnot(int* mode, struct gol_data* data,int rows,int cols,int iters, int osc);

 void print_board(struct gol_data *data, int round){
     int i= 0; int j= 0;
     int live = 0;
     int index=0;
     /* Print the round number. */
     fprintf(stderr, "Round: %d\n", round);

     for (i = 0; i < data->rows; ++i) {
         for (j = 0; j < data->cols; ++j) {
             index = data->rows *(i) + j;
             //if cell is alive
             if (data->board[index]==1){
               live += 1;
               fprintf(stderr, " @");
             }

           //otherwise
             else{
               fprintf(stderr, " _");
               }
         }
         fprintf(stderr, "\n");
     }
     /* Add some blank space between rounds. */
     fprintf(stderr, "\n\n");
 }

 /* Performs one round of the GOL simulation.
Inputs: Our game of life struct for the thread, color buff
Outputs: None
 */

 void gol_step(color3 *buff, struct gol_data* data) {
     int i , j;
     int index = 0;
     data->alive_counted=0;
     i = data->start_index;

     if (data->partition==0){ //when the user partitions by row
       i = data->start_row;
       j = 0;
       index = (data->cols) *(i) + j;
       while (index != data->end_index) { //as long as we haven't reached the end index
         j=0;
          while(j!=(data->cols) && index != data->end_index){ //while we haven't
          //reached the end of the row and the end index
            index = (data->cols) *(i) + j;
            nextstepcalc(data, i, j, index);
            if (buff != NULL) {
              int buff_index = (data->rows - (i + 1)) * data->cols + j;
              if (data->nextStepBoard[i * data->cols + j]) {
                /* Live cells get the color using this thread's ID as the index */
                buff[buff_index] = colors[data->id];
              }
              else {
                /* Dead cells are blank. */
                buff[buff_index] = c3_black;
              }
            }
            j++;
          }
        i++;
               /* When using visualization, also update the graphical board. */

       }
     }
     else{//when the user partitions by columns
         i = 0;
         index=0;

         while (index != data->end_index) {//while we dont reach the end index
           j=data->start_col;
           while(j!=(data->end_col+1) && index != data->end_index){//while we
             //dont reach tne end index and do not reach the end column of the parition
              index = (data->cols) *(i) + j;
              nextstepcalc(data, i, j, index);
              if (buff != NULL) {
                int buff_index = (data->rows - (i + 1)) * data->cols + j;
                if (data->nextStepBoard[i * data->cols + j]) {
                  /* Live cells get the color using this thread's ID as the index */
                  buff[buff_index] = colors[data->id];
                }
                else {
                  /* Dead cells are blank. */
                  buff[buff_index] = c3_black;
                }
              }
              j++;
            }
          i++;
                 /* When using visualization, also update the graphical board. */

         }


     }
     /* When using text/graphical output, add a delay and clear the terminal.
      * You can adjust/disable these however you like while debugging. */

 }

 /*
 Inputs: The pointer to our gol_data data struct, our x and y coordinates, and
 the corresponding index for our dynamically allocated array
 Purpose: The purpose of this function is to find the state of the board given
 the state of its neighbors
 Returns: none
 */
 void nextstepcalc(struct gol_data* data, int i, int j,int index){
   int  n_aliveNeighbors; //number of neighbors that are currently alive
   n_aliveNeighbors = getNeighbors(i, j, data);

   if (data->board[index] == 1){
     if (n_aliveNeighbors <= 1){
       data->nextStepBoard[index] = 0;
     }
     else if (n_aliveNeighbors >= 4){
       data->nextStepBoard[index] = 0;
     }
     else{
       data->nextStepBoard[index] = 1;
       data->alive_counted++;
     }
   }
   else {
     if (n_aliveNeighbors == 3){
       data->nextStepBoard[index] = 1;
       data->alive_counted++;

     }
     else {
       data->nextStepBoard[index] = 0;
     }

   }

 }

 /*
 Inputs: The pointer to our gol_data data struct, our x and y coordinates
 Purpose: The purpose of this function is to find the number of alive neighbors
 given a certain x and y coordinate
 Returns: the number of alive neighbors
 */
 int getNeighbors(int x, int y, struct gol_data* data){

     int alive_neighbors = 0;
     int n1 = 0;
     int n2 = 0;
     int n3=0;
     int n4=0;
     int n5=0;
     int n6=0;
     int n7=0;
     int n8 = 0;
     if (x > 0) {
       n1 =checkNeighbor(((x-1)+data->cols)%data->cols,y%data->rows,data);
     }
     if (y > 0 && x > 0){

       n2 =checkNeighbor(((x-1)+data->cols)%data->cols,((y-1)+data->rows)%data->rows,data);
     }
     if (y > 0){

       n3 =checkNeighbor(x%data->cols,((y-1)+data->rows)%data->rows,data);

     }
     if (x<data->cols-1 && y>0){

       n4 =checkNeighbor(((x+1)+data->cols)%data->cols,((y-1)+data->rows)%data->rows,data);
     }
     if (x<data->cols-1){

       n5 =checkNeighbor(((x+1)+data->cols)%data->cols,y%data->rows,data);
     }
     if (x < data -> cols - 1 && y < data->rows-1){

       n6 =checkNeighbor(((x+1)+data->cols)%data->cols,((y+1)+data->rows)%data->rows,data);
     }
     if (y < data->rows -1){

       n7 =checkNeighbor(x%data->cols,((y+1)+data->rows)%data->rows,data);
     }

     if (x > 0 && y < data->rows-1){

       n8 =checkNeighbor(((x-1)+data->cols)%data->cols,((y+1)+data->rows)%data->rows,data);
     }

     alive_neighbors = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8;

     return alive_neighbors;
     }

 /*
 Inputs: The pointer to our gol_data data struct, our x and y coordinates
 Purpose: The purpose of this function is to see if the cell is alive or dead
 Returns: 1 if alive, 0 if deadhttps://www.cs.swarthmore.edu/~chaganti/cs31/s19/labs/lab09.html
 */
 int checkNeighbor(int x, int y, struct gol_data* data){
     int index = 0;

     index = data->cols *x + y;
     if (data->board[index] == 1){
       return 1;
   }


     return 0;
 }


 /*
 Inputs: The pointer to our gol_data data struct for a thread
 Purpose: The purpose of this function is to call gol step while mainting synchronization.
 waits for all threads to go through the golstep before printing the new board.
 it also updates the global variable live to keep record of alive cells processed.
 Returns: nothing
 */
void *worker(void *datastruct) {
    // TODO: use type casting to get a struct gol_data * from the void * input.
    int round = 0;
    int ret=0;
    int* temp=NULL;
    struct gol_data* copy_datastruct = (struct gol_data *) datastruct;

    for (int i = 0; i < copy_datastruct->iters; i++) {
      gol_step(image_buf, copy_datastruct);
      int result= pthread_mutex_lock(&lock);
      if(result){
        perror("pthread_mutex_lock\n");
      }
      live=live+copy_datastruct->alive_counted;
      result= pthread_mutex_unlock(&lock);
      if(result){
        perror("pthread_mutex_lock\n");
      }
      temp=copy_datastruct->board;
      copy_datastruct->board=copy_datastruct->nextStepBoard;
      copy_datastruct->nextStepBoard=temp;
      round++;
      ret = pthread_barrier_wait(&barrier);
      if(ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
        perror("pthread_barrier_wait");
        exit(1);
      }
      if (copy_datastruct->id==0){
        if(copy_datastruct->output_mode==1){
          print_board(copy_datastruct,round);
        }
        if (copy_datastruct->output_mode > 0) {
          usleep(100000);
          system("clear");
        }
      }
      if(copy_datastruct->output_mode==2){
        draw_ready(handle);

      }
  }

    return NULL;

}
void usage(void){
  fprintf(stderr,
      " usage:\n"
      " ./gol -t t { -n n -m m -k k [-s] | -f infile} [-x] [-c]"
       "-t  t:      number of threads\n"
       "-n  n:      number of rows in the game board\n"
       "-m  m:      number of columns in the game board\n"
       "-k  k:      number of iterations\n"
       "-s:         initialize to oscillator pattern (default is random):\n"
       "-f infile:  read in board config info from an input file\n"
       "-x:         disable ascii animation (printing out board every iteration)\n"
                   "(default is to run with animation/printing)\n"
       "-c:         do column partitioning (default is row portioning)\n"
       "-h:         print out this help message\n"
       "\n");
  exit(1);

}







void process_args(int ac, char* av[], int* num_threads, struct gol_data* data, int* output_mode){
  srand(1);
  int f_flag=0;
  int osc=0;
  int c = 0;
  int rows = 0;
  int cols = 0;
  int iters = 0;
  char* file_name;
  *output_mode=1; //default is always going to be 1
  while(1) {
    c = getopt(ac, av, "t:n:m:k:f:sxch");

    if(c == -1) {
      break; // no more command line arguments to parse, break out of while loop
         }
       // end while
   switch(c){
        case 'f':{
          if(rows > 0 || cols > 0 ){
            usage();
          }
          f_flag = 1;
          file_name = {optarg};
                }

                break;

        case 't': {
                    *num_threads = atoi(optarg);
                    break;
                   }
        case 'x':{
         *output_mode=0;
        break;
                 }


        case 'n':{
          if(f_flag ==1){
            usage();
          }
        //if f flag is set,
        rows = atoi(optarg);
        break;
                 }
        case 'm':{
          if(f_flag ==1 || atoi(optarg) < 1){
            usage();
          }
        cols = atoi(optarg);
        break;
                 }
        case 'k':{
         if(f_flag ==1 || atoi(optarg) < 0){
             usage();
           }
        iters = atoi(optarg);
        break;
                 }
        case 's':{
              osc = 1;
              break;
                }
        case 'c':{
              data->partition = 1;
              break;
                 }
      }
    }
      if (f_flag == 1){
        initializer(file_name,output_mode,data);
      }
      if(f_flag==0){
        initializer_fnot(output_mode, data, rows,cols,iters,osc);

    }


}

int main(int argc, char *argv[]) {

    int result=0;
    int output_mode = 0;
    int num_threads = 0;
    struct gol_data data;
    int print_partition=0;
    pthread_t *threads;

    process_args(argc, argv, &num_threads,&data,&output_mode);

    //print_board(&data, 0);
    data.output_mode=output_mode;
    data.partition = atoi(argv[4]);
    data.print_partition=print_partition;



    // start the clock.
    double secs = 0.0;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    pthread_mutex_init(&lock, NULL);
    if (pthread_barrier_init(&barrier, NULL, num_threads)){
      perror("pthread_barrier_init");
      exit(1);
    }

    // set up a gol_data struct for each thread (e.g., with the thread's
    // partition assignment) and create the threads.
    struct gol_data* arguments = (gol_data*)malloc(num_threads * sizeof(struct gol_data));
    if (arguments == NULL) {
      perror("malloc()");
      free(arguments);
      exit(1);
    }
    int partition_size=0;
    int remainder_parition=0;
    int j=0;
    int index_recorder = 0;
    int col_index = 0;
    threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
      perror("malloc()");
      free(threads);
      exit(1);
    }
    j=0;
    if (data.partition ==0){//if partition by row
          if(num_threads>data.rows){
            printf("the number of threads is greater than the number of rows\n Quitting!\n");
            exit(1);
          }
          remainder_parition=data.rows%num_threads;
          partition_size=(data.rows-remainder_parition)/num_threads;
          for (int i =0; i<num_threads; i++){
            arguments[i].iters = data.iters;
            arguments[i].num_threads=num_threads;
            arguments[i].print_partition=data.print_partition;
            arguments[i].id = i;
            arguments[i].partition=data.partition;
            arguments[i].output_mode=data.output_mode;
            arguments[i].cols=data.cols;
            arguments[i].rows=data.rows;
            arguments[i].start_index = index_recorder;
            arguments[i].start_row=i*partition_size+j;
            arguments[i].end_row=(i+1)*partition_size-1+j;
            arguments[i].start_col=0;
            arguments[i].end_col=data.cols-1;
            index_recorder = index_recorder + partition_size*data.cols;
            arguments[i].end_index = index_recorder - 1;
            arguments[i].board=data.board;
            arguments[i].nextStepBoard=data.nextStepBoard;
            if (remainder_parition!=0){//if there is remainder, then we make a constant offsett and update the information
              j++;
              remainder_parition--;
              index_recorder = index_recorder + data.cols;
              arguments[i].end_row++;
              arguments[i].end_index = index_recorder - 1;
            }
            if(arguments[i].print_partition!=0){//prints out the partition information if required
              printf("tid: %d rows: %d : %d (%d) cols: %d : %d (%d) \n",
              arguments[i].id,arguments[i].start_row,arguments[i].end_row,
              arguments[i].rows,arguments[i].start_col,
              arguments[i].end_col,arguments[i].cols);
              fflush(stdout);
            }

          pthread_create(&threads[i], NULL, &worker, &arguments[i]);
        }

    }
    else{//if parition by colunms

        if(num_threads>data.cols){
          printf("the number of threads is greater than the number of cols\n Quitting!\n");
          exit(1);
        }


        remainder_parition=data.cols%num_threads;
        partition_size=(data.cols-remainder_parition)/num_threads;
        for (int i =0; i<num_threads; i++){
          arguments[i].iters = data.iters;
          arguments[i].output_mode=data.output_mode;
          arguments[i].num_threads=num_threads;
          arguments[i].print_partition=data.print_partition;
          arguments[i].id = i;
          arguments[i].partition=data.partition;
          arguments[i].cols=data.cols;
          arguments[i].rows=data.rows;
          col_index = partition_size*i;
          arguments[i].start_index = col_index;
          arguments[i].start_row=0;
          arguments[i].end_row=data.rows-1;
          arguments[i].start_col=i*partition_size+j;
          arguments[i].end_col=arguments[i].start_col+partition_size-1;
          col_index =  data.cols*(data.rows-1)+arguments[i].start_index+partition_size-1 +j;
          arguments[i].end_index = col_index;
          arguments[i].board=data.board;
          arguments[i].nextStepBoard=data.nextStepBoard;
          if (remainder_parition!=0){//if there is remainder, then we make a constant offsett and update the information
            remainder_parition--;
            j++;
            arguments[i].end_col++;
            arguments[i].end_index++;
          }
          if(arguments[i].print_partition!=0){//prints partiton information if required
            printf("tid: %d rows: %d : %d (%d) cols: %d : %d (%d) \n"
            ,arguments[i].id,arguments[i].start_row,arguments[i].end_row,
            arguments[i].rows,arguments[i].start_col,  //output_mode = outmode;

            arguments[i].end_col,arguments[i].cols);
            fflush(stdout);
          }
          pthread_create(&threads[i], NULL, &worker, &arguments[i]);
        }

    }



    /* If we're doing graphics, call run_animation to tell it how many
     * iterations there will be. */
    // if (output_mode == OUTPUT_VISI) {
    //     //pass in the number of iterations as the second parameter
    //     run_animation(handle, data.iters);
    // }  output_mode = atoi(mode);


    //join all the threads (that is, wait in this main thread until all
    // the workers are finished.
    for (int i = 0; i< num_threads; i++){
      pthread_join(threads[i], NULL);
    }

    //  stop the clock, print timing results, and clean up memory.
    gettimeofday(&end_time, NULL);
    secs = end_time.tv_sec*1000000+end_time.tv_usec
     -(start_time.tv_sec*1000000+start_time.tv_usec);
     secs=secs/1000000;

     result=pthread_mutex_destroy(&lock);//destroys the mutex
     if (result){
       perror("Pthread mutex destroyed");
     }
     result = pthread_barrier_destroy(&barrier);//destroys the barrier
     if (result){
       perror("Pthread barrier destroy");
     }

     printf("number of alive cells processed: %d\n",live);
    /* Print the total runtime, in seconds. */
    //prints our time required
    printf("\nTotal time: %0.3f seconds.\n", secs);
    free(arguments);
    free(data.nextStepBoard);
    free(data.board);
    free(threads);

    return 0;
}


void initializer_fnot(int* mode, struct gol_data* data,int rows,int cols,int iters,int osc){


  //fscanf reads each successive line and stores the integer value in the given
  //address
  data->rows=rows;
  data->cols=cols;
  data->iters=iters;
  data->numAlive=0;

  //dynamically allocate our next step board
  data->nextStepBoard = (int*) malloc(sizeof(int)*(data->rows)*(data->cols));
  if (data->nextStepBoard == NULL){
    printf("ERROR: Out of memory\n");
    exit(1);
  }

  printf("rows: %d, cols : %d\n", rows, cols);
  int n_indices= rows *cols;
  //dynamically allocate an array that stores the state of our board
  data->board = (int*)malloc(sizeof(int)*(rows)*(cols));

  //quits if there is no space available
  if (data->board == NULL){
    printf("ERROR: Out of memory\n");
    exit(1);
  }
for (int i=0; i<rows; i++){
  for(int j=0;j<cols;j++){
       int index = data->cols *(i) + j;
         data->board[index]=0;


    }
  }
if(osc==0){
  for (int i=0; i<rows; i++){
    for(int j=0;j<cols;j++){
       int  alive = rand() % (10 + 1 - 1) + 1;
       int index = data->cols *(i) + j;
       data->board[index]=0;
        if(alive > 5){
          data->board[index]=1;
          data->numAlive++;
        }
      }
    }
  }
else if (osc==1){
  data->numAlive = 3;
  int x_val = cols/2;
  int y_val = rows/2;
  int index = data->cols *(x_val) + y_val;
  for(int i = -1; i<2;i++){

    int index = data->cols *(x_val+i) + y_val;

    data->board[index] = 1;

  }


}
  //dynamically allocate an array that stores the indices of our initial
        //alive coordinates
  // data->indicesAlive = (int*)malloc(sizeof(int)*(data->numAlive+1));
  //
  // if (data->indicesAlive == NULL){
  //       printf("ERROR: Out of memory\n");
  //       exit(1);
  //     }


  // int k = 0;
  // for (int i=0; i<rows; i++){
  //   for(int j=0;j<cols;j++){
  //     int index = data->cols * (i) + j;
  //     if(data->board[index]==1){
  //       printf("n_ alive :%d, K: %d\n", data->numAlive,k);
  //       data->indicesAlive[k] = index;
  //       k++;
  //     }
  //   }
  // }

  // free(data->indicesAlive);


  data->output_mode = *mode;

}



/*
Inputs: The name of the file that is inputted into the command line, the
mode that we want to run, and the pointer to our gol_data data struct
Purpose: The purpose of this function is to initialize our game of life
data struct.
Returns: none
*/
void initializer(char* file_name, int* mode, struct gol_data* data){
  FILE* infile;
  int output_mode,ret;
  int x_value, y_value, index;
  int trackerIndex=0;

  infile=fopen(file_name,"r"); //opens the file in read-only mode
  if (infile == NULL) { //tells us if the file did not open
    perror("fopen");
    exit(1);
  }
  //fscanf reads each successive line and stores the integer value in the given
  //address
  fscanf(infile, "%d", &data->rows);
  fscanf(infile, "%d", &data->cols);
  fscanf(infile, "%d", &data->iters);
  fscanf(infile, "%d", &data->numAlive);
  //dynamically allocate our next step board
  data->nextStepBoard = (int*) malloc(sizeof(int)*(data->rows)*(data->cols));
  if (data->nextStepBoard == NULL){
    printf("ERROR: Out of memory\n");
    exit(1);
  }
  //dynamically allocate an array that stores the indices of our initial
  //alive coordinates
  data->indicesAlive = (int*)malloc(sizeof(int)*(data->numAlive));

  //quits if there is no space available
  if (data->indicesAlive == NULL){
    printf("ERROR: Out of memory\n");
		exit(1);
  }
  //dynamically allocate an array that stores the state of our board
  data->board = (int*)malloc(sizeof(int)*(data->rows)*(data->cols));
  //quits if there is no space available
  if (data->board == NULL){
    printf("ERROR: Out of memory\n");
		exit(1);
  }
  //reads and stores the coordinates of our initially alive

  for (int i=0; i<data->numAlive; i++){
    ret = fscanf(infile, "%d %d", &x_value, &y_value);
    if (ret != 2){
      printf("invalid data in file\n");
      exit(1);
    }
    index = data->cols *(x_value) + y_value;
    data->indicesAlive[i]= index;

}

  //initializes our board with 0's in all coordinates
  for (int i=0; i<(data->rows)*(data->cols); i++){
      data->board[i] = 0;
  }

  //inserts 1 in the initially alive coordinates
  for (int i=0; i<data->numAlive; i++){
    trackerIndex=data->indicesAlive[i];
    data->board[trackerIndex]=1;
  }

  data->output_mode = *mode;

  free(data->indicesAlive);
  //close file
  fclose(infile);

}
