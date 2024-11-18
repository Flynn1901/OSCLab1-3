#include "sensor_db.h"

FILE* open_db(char *file, bool append){
    char* mode = 0;
    if(append){ mode = "a";}
    else{ mode = "w";}

    FILE *db = fopen(file, mode);
    if(db == NULL){
        printf("Error opening file %s\n", file); exit(-1);
    }
    return db;
}

int insert_sensor(FILE *f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if(f == NULL){
        printf("Error opening file \n");
        return 0;
    }
    fprintf(f,"%u %f %ld",id, value, ts);
    return 1;
}

int close_db(FILE *f){
    if (fclose(f) == 0) {
        printf("File closed successfully.\n");
        return 1;
    }
    perror("Error closing file");
    return 0;
}
