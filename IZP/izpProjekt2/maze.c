#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {

    int rows;
    int cols;
    unsigned char *cells;

} Map;




int mapinit(Map *map, int *rw, int *cl, unsigned char c[], FILE *f){
      
    map->cells = c;
    map->rows = *rw;
    map->cols = *cl;

    int i = 0;
    int y = 0;
    while((y=fgetc(f))!=EOF){

        if(y != '\n' && y != ' '){
            map->cells[i] = y;
            i++;   

        }                
    }

    if(i / map->rows != map->cols){
        printf("Invalid\n");
        return 1;
    }
    if(i / map->cols != map->rows){
        printf("Invalid\n");
        return 1;
    }
    if(map->cols < 1 || map->rows < 1){
        printf("Invalid\n");
        return 1;
    }
    
    
    return 0;

}

int valid_borders_test(Map *ptr){

    char vals[8] = {'0','1','2','3','4','5','6','7'};

    for(int i = 0; i < ptr->cols*ptr->rows; i++){

            for(int j = 0; j < 8; j++){

                if(ptr->cells[i] == vals[j]){
                    break;
                } else if(j == 7){
                    return 1;
                }
            }
        }

    int a = 1;
    int b = 0;
    for(int i = 0; i < ptr->cols*ptr->rows; i++){
            
        b++;
        if(i > 0 && i % ptr->cols ==  0){
            a++;
            b = 1;
        }

        
        if((ptr->cells[i] == '1' || ptr->cells[i] == '3' || ptr->cells[i] == '5' || ptr->cells[i] == '7') && (i % ptr->cols != 0)){

            
            if(i > 0 && ptr->cells[i-1] != '2' && ptr->cells[i-1] != '3' && ptr->cells[i-1] != '6' && ptr->cells[i-1] != '7' && ptr->cells[i-1] != 0){  //nalavo
                
                return 1;
            }         
        }  
        if((ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '4' || ptr->cells[i] == '5') && (i % (ptr->cols+1) != 0) && ((i+1) % ptr->cols != 0)){
            
            
            if(ptr->cells[i+1] != '0' && ptr->cells[i+1] != '2' && ptr->cells[i+1] != '6' && ptr->cells[i+1] != '4' && ptr->cells[i+1] != 0){  //napravo
                
                
                return 1;
            }
        }
        if((ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3') && ((a+b) % 2 == 1)){
            
            if(ptr->cells[i+ptr->cols] != '0' && ptr->cells[i+ptr->cols] != '1' && ptr->cells[i+ptr->cols] != '2' && ptr->cells[i+ptr->cols] != '3' && ptr->cells[i+ptr->cols] >= '0' && ptr->cells[i+ptr->cols] < '8'){  //dole
                
                return 1;
            }
        }
        if((ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3') && ((a+b) % 2 == 0)){

            if(i > ptr->cols-1 && ptr->cells[i-ptr->cols] != '0' && ptr->cells[i-ptr->cols] != '1' && ptr->cells[i-ptr->cols] != '2' && ptr->cells[i-ptr->cols] != '3' && ptr->cells[i-ptr->cols] != 0){  //hore

                return 1;
            }
        }
        if(ptr->cells[i] == '0' || ptr->cells[i] == '2' || ptr->cells[i] == '4' || ptr->cells[i] == '6'){

            
            if((i > 0 && ptr->cells[i-1] != '0' && ptr->cells[i-1] != '1' && ptr->cells[i-1] != '4' && ptr->cells[i-1] != '5' && ptr->cells[i-1] != 0) && (i % ptr->cols != 0)){  //nalavo
                
		    
                return 1;
            }         
        }

        if(ptr->cells[i] == '3' || ptr->cells[i] == '7' || ptr->cells[i] == '2' || ptr->cells[i] == '6'){

            
            if((ptr->cells[i+1] != '1' && ptr->cells[i+1] != '3' && ptr->cells[i+1] != '5' && ptr->cells[i+1] != '7' && ptr->cells[i+1] != 0) && ((i+1) % ptr->cols != 0)){  //napravo
                
                return 1;
            }         
        }
        if((ptr->cells[i] == '4' || ptr->cells[i] == '5' || ptr->cells[i] == '6' || ptr->cells[i] == '7') && ((a+b) % 2 == 1)){

            
            if(i < ((ptr->cols*ptr->rows) - (ptr->cols)) && ptr->cells[i+ptr->cols] != '4' && ptr->cells[i+ptr->cols] != '5' && ptr->cells[i+ptr->cols] != '6' && ptr->cells[i+ptr->cols] != '7' && ptr->cells[i+ptr->cols] != 0){  //dole
                
                return 1;
            }
        }
        if((ptr->cells[i] == '4' || ptr->cells[i] == '5' || ptr->cells[i] == '6' || ptr->cells[i] == '7') && ((a+b) % 2 == 0)){
            
            if(i > ptr->cols-1 && ptr->cells[i-ptr->cols] != '4' && ptr->cells[i-ptr->cols] != '5' && ptr->cells[i-ptr->cols] != '6' && ptr->cells[i-ptr->cols] != '7' && ptr->cells[i-ptr->cols] != 0){  //hore
                
                return 1;
            }          
        }
            
    }
    return 0;     

}

void freemap(Map *map){

    free(map->cells);
    return;
}


int start_border(Map *map, int r, int c, int leftright){
    if(leftright == 2){  //prava ruka
        if(r % 2 == 1 && c == 1){      //pravá při vstupu zleva do bludiště na lichém řádku,
            return 2;
        }
        if(r % 2 == 0 && c == 1){     //dolní při vstupu zleva do bludiště na sudém řádku,
            return 0;
        }
        if(r == 1){                 //levá při vstupu shora do bludiště,
            return 1;
        }
        if(r == map->rows && c != map->cols){         //pravá při vstupu zespodu do bludiště,
            return 2;
        }
        if((r % 2 == (r+c) % 2) && c == map->cols){   //horní při vstupu zprava do bludiště, pokud má políčko horní hranici a
            return 0;
        }
        if((r % 2 != (r+c) % 2) && c == map->cols){      //levá při vstupu zprava do bludiště, pokud má políčko dolní hranici.
            return 1;
        }       
    }

    if(leftright == 1){  //lava ruka
        if(r % 2 == 1 && c == 1){      //horna při vstupu zleva do bludiště na lichém řádku,
            return 0;
        }
        if(r % 2 == 0 && c == 1){     //prava při vstupu zleva do bludiště na sudém řádku,
            return 2;
        }
        if(r == 1 && c != map->cols){                 //prava při vstupu shora do bludiště,
            return 2;
        }
        if(r == map->rows && c != map->cols){         //lava při vstupu zespodu do bludiště,
            return 1;
        }
        if((r+c) % 2 == 0 && c == map->cols){   //lava při vstupu zprava do bludiště, pokud má políčko horní hranici a
            return 1;
        }
        if((r+c) % 2 == 1 && c == map->cols){      //dolna při vstupu zprava do bludiště, pokud má políčko dolní hranici.
            return 0;
        }       
    }
    return 42;
}


bool isborder(Map *map, int r, int c, int border){

    int a;
    int x = 0;
    int y = 1;
    
    int pos[map->cols*map->rows][2];

    for(int j = 0; j < map->rows*map->cols; j++){  //suradnicovy system  
        if(j % map->cols == 0){
            x += 1;
            y = 1;
        } else {
            y += 1;
        }
        pos[j][0] = x;
        pos[j][1] = y;
    }

    for(int i = 0; i < map->cols*map->rows; i++){

        if(pos[i][0] == r && pos[i][1] == c){
            a = i;
            break;
        }
    }

    if(border == 0 && (map->cells[a] == '1' || map->cells[a] == '2' || map->cells[a] == '3' || map->cells[a] == '0')){
        return true;
    } else if(border == 0){
        return false;
    }
    if(border == 1 && (map->cells[a] == '0' || map->cells[a] == '2' || map->cells[a] == '4' || map->cells[a] == '6')){
        return true;
    } else if(border == 1){
        return false;
    }
    if(border == 2 && (map->cells[a] == '1' || map->cells[a] == '4' || map->cells[a] == '5' || map->cells[a] == '0')){
        return true;
    } else if(border == 2){
        return false;
    }

    return 1;
}


int main(int argc, char *argv[]){    


    if(argc < 2 && strcmp(argv[1], "--help") != 0){

        fprintf(stderr, "ERROR: not enough arguments\n");
        return 1;

    }

    if(argc < 2 || strcmp(argv[1], "--help") == 0){      //prikaz --help

        printf("Run with:  ./maze [command] [int of starting row] [int of starting column] [.txt file name]\n");
        printf("Commands are: '--lpath', '--rpath'\nOther commands: '--test', '--help'\n");
        
        return 0;
    }

    int an;
    if(strcmp(argv[1], "--test") == 0){    //an urcuje ktory z argumentov je .txt subor
        an = 2;
    } else {
        an = 4;
    }
    

    Map *ptr = malloc(sizeof(Map));

    
    FILE *file = fopen(argv[an], "r");
    if (file == NULL) {
        perror("Error opening file\n");

        free(ptr);
        fclose(file);

        return 1;
    }

    int ch = 0;
    char a[3];
    char b[3];

    int i = 0;
    while((ch=fgetc(file)) != ' '){
        a[i] = ch;
        i++;
    }
    a[i] = '\0';
    
    i = 0;
    while((ch=fgetc(file)) != '\n'){
        b[i] = ch;
        i++;
    }
    b[i] = '\0';
    

    ptr->rows = atoi(a);
    ptr->cols = atoi(b);
    

    int *prows = &ptr->rows;
    int *pcols = &ptr->cols;

    
    ptr->cells = malloc((ptr->rows*ptr->cols)*sizeof(char));
    int pos[ptr->rows*ptr->cols][2];


    
    if(mapinit(ptr, prows, pcols, ptr->cells, file) == 1){
        
        fclose(file);
        freemap(ptr);
        free(ptr);
        
        return 1;
    }
    
    int r = 0;
    int c = 1;
    
    for(int j = 0; j < ptr->rows*ptr->cols; j++){  //suradnicovy system
        if(j%ptr->cols == 0){
            r += 1;
            c = 1;
        } else {
            c += 1;
        }
        pos[j][0] = r;
        pos[j][1] = c;
    }


    int has_start = 0;
    int possible_starts[(ptr->rows*2)+2*(ptr->cols-2)][2];

    for(int i = 0; i < ptr->rows*ptr->cols; i++){


        if(pos[i][0] == 1){ //ZACIATOK NA PRVOM RIADKU

            if(pos[i][1] == 1){      //roh - lavy horny 
                if(ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3' || ptr->cells[i] == '4' || ptr->cells[i] == '6' || ptr->cells[i] == '0'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] == ptr->cols && ptr->cols % 2 == 0){    //roh - pravy horny, parne stlpce  
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '4' || ptr->cells[i] == '5' ){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] == ptr->cols && ptr->cols % 2 == 1){    //roh - pravy horny, neparne stlpce  
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3' || ptr->cells[i] == '4' || ptr->cells[i] == '5' ){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][0] == 1 && pos[i][1] != ptr->cols && pos[i][1] != 1 && pos[i][1] % 2 == 1){     //1. riadok okrem rohov 
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
                
            }
        }


        if(pos[i][0] == ptr->rows){ //ZACIATOK NA POSLEDNOM RIADKU

            if(pos[i][1] == 1 && ptr->rows % 2 == 1){    //roh - lavy dolny - neparne riadky 
            
                if(ptr->cells[i] == '0' || ptr->cells[i] == '2' || ptr->cells[i] == '4' || ptr->cells[i] == '6'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] == 1 && ptr->rows % 2 == 0){ // roh - lavy dolny - parne riadky 
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3' || ptr->cells[i] == '4' || ptr->cells[i] == '6'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] == ptr->cols && ptr->rows % 2 == ptr->cols % 2){ // roh - pravy dolny - parne, parne / neparne, neparne 
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '4' || ptr->cells[i] == '5'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] == ptr->cols && ptr->rows % 2 != ptr->cols % 2){ // roh - pravy dolny - parne, neparne / neparne, parne 
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3' || ptr->cells[i] == '4' || ptr->cells[i] == '5'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] != 1 && pos[i][1] != ptr->cols && ptr->rows % 2 == 0 && pos[i][1] % 2 == 1){ //parne riadky - mimo rohov 
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }
            if(pos[i][1] != 1 && pos[i][1] != ptr->cols && ptr->rows % 2 == 1 && pos[i][1] % 2 == 0){ //neparne riadky - mimo rohov 
                if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '2' || ptr->cells[i] == '3'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
            }   
        }

        if(pos[i][1] == 1 && pos[i][0] != 1 && pos[i][0] != ptr->rows){ //zaciatok na lavej strane  (rohy neratam)
            if(ptr->cells[i] == '0' || ptr->cells[i] == '2' || ptr->cells[i] == '4' || ptr->cells[i] == '6'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                }
        }    

        if(pos[i][1] == ptr->cols && pos[i][0] != 1 && pos[i][0] != ptr->rows){   //zaciatok na pravej strane (rohy neratam)
            if(ptr->cells[i] == '0' || ptr->cells[i] == '1' || ptr->cells[i] == '4' || ptr->cells[i] == '5'){
                    possible_starts[has_start][0] = pos[i][0];
                    possible_starts[has_start][1] = pos[i][1];
                    has_start++;
                
            }
        }
    }
    
    if(strcmp(argv[1], "--test") == 0){

        if(has_start < 1){
            
            fclose(file);
            freemap(ptr);
            free(ptr);

            printf("Invalid\n");
            return 1;
        }
        if(valid_borders_test(ptr) == 1){
            printf("Invalid\n");
            goto the_end;
        }
        printf("Valid\n");

        goto the_end;  

    
    }

//******************************************************************************************************************


    int start[2];

    start[0] = atoi(argv[2]);  //zaciatocny riadok
    start[1] = atoi(argv[3]);  //zaciatocny stlpec 

    int rw = start[0];
    int cl = start[1];

    if(rw > ptr->rows || cl > ptr->cols){
        fprintf(stderr,"ERROR: invalid start values\n");
        goto the_end;
    }
        
    int ib;
    int previous;

    if(strcmp(argv[1], "--rpath") == 0){

        if(valid_borders_test(ptr) == 1){   //ci je maze vobec valid
            printf("Maze is invalid..\n");
            goto the_end;
        }

        for(int i = 0; i < has_start; i++){

            if(rw == possible_starts[i][0] && cl == possible_starts[i][1]) // ci sa da zacat na zadanych suradniciach
                break;

            if(i == has_start-1){
                fprintf(stderr, "ERROR: Start not possible on given coords\n");
                goto the_end;
            }
        }
    
        if(has_start < 1){
            
            fclose(file);
            freemap(ptr);
            free(ptr);

            printf("Maze is invalid..\n");
            return 1;
        }

        if(start_border(ptr, start[0], start[1], 2) == 0 && cl == 1 && rw % 2 == 0){   //zaciatok z lava, parny riadok

            if(rw != ptr->rows){
                ib = 0;
                previous = 1;
            }
            if(rw == ptr->rows){

                ib = 2;
                previous = 1;

            }
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 2) == 2 && cl == 1 && rw % 2 == 1){   //zaciatok z lava, neparny riadok

            if(rw == 1 && (ptr->cells[0] == '1' || ptr->cells[0] == '2' || ptr->cells[0] == '3')){
                ib = 1;
                previous = 0;
            }
            if(rw == 1 && (ptr->cells[0] == '6' || ptr->cells[0] == '4' || ptr->cells[0] == '0')){
                ib = 2;
                previous = 1;
            }
            if(rw != 1){
                previous = 1;
                ib = 2;
            }
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 2) == 1 && rw == 1 && cl != ptr->cols){   //zaciatok zhora
            ib = 1;
            previous = 0;
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 2) == 2 && rw == ptr->rows && cl != ptr->cols){   //zaciatok zdola
            
            ib = 2;
            previous = 0;
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 2) == 0 && cl == ptr->cols && ptr->cols % 2 == 0){   //zaciatok sprava

            if(rw % 2 == 1){    //ma hornu hranicu
                previous = 2;
                ib = 1;                
            }
            if(rw % 2 == 0){  // ma spodnu hranicu
                previous = 2;
                ib = 0;
            }
            printf("%d,%d\n", rw, cl);
        }
        
        if(start_border(ptr, start[0], start[1], 2) == 1 && cl == ptr->cols && ptr->cols % 2 == 1){  //zaciatok sprava

            if(rw == 1 && (ptr->cells[ptr->cols-1] == '0' || ptr->cells[ptr->cols-1] == '2' || ptr->cells[ptr->cols-1] == '3')){
                previous = 0;
                ib = 1;        
            }
            if(rw == 1 && (ptr->cells[ptr->cols-1] == '1' || ptr->cells[ptr->cols-1] == '4' || ptr->cells[ptr->cols-1] == '6')){
                previous = 2;
                ib = 1;
            }
            if(rw != 1 && rw % 2 == 1){    //ma hornu hranicu
                previous = 2;
                ib = 0;                
            }
            
            if(rw != 1 && rw % 2 == 0){  // ma spodnu hranicu
                previous = 2;
                ib = 1;
            }
            printf("%d,%d\n", rw, cl);
        }
        
        while(true){

            if(isborder(ptr, rw, cl, ib) == true && ib == 2 && (rw+cl) % 2 == 0){
                cl++;
                if(cl > ptr->cols){
                    break;
                } else{
                    printf("%d,%d\n", rw, cl);
                    ib = 0;
                    previous = 1;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 2 && (rw+cl) % 2 == 0){
                if(previous == 0 || previous == 1){
                    ib = 0;
                }

                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 2 && (rw+cl) % 2 == 1){
                cl++;
                if(cl > ptr->cols){
                    break;
                } else{
                    printf("%d,%d\n", rw, cl);
                    ib = 2;
                    previous = 1;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 2 && (rw+cl) % 2 == 1){
                if(previous == 1 || previous == 0){
                    ib = 1;
                }
                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 1 && (rw+cl) % 2 == 1){
                cl--;
                if(cl < 1){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 0;
                    previous = 2;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 1 && (rw+cl) % 2 == 1){
                if(previous == 2 || previous == 0){
                    ib = 0;
                }
                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 1 && (rw+cl) % 2 == 0){
                cl--;
                if(cl < 1){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 1;
                    previous = 2;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 1 && (rw+cl) % 2 == 0){
                if(previous == 2 || previous == 0){
                    ib = 2;
                }
                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 0 && (rw+cl) % 2 == 0){
                rw--;
                if(rw < 1){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 2;
                    previous = 0;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 0 && (rw+cl) % 2 == 0){
                if(previous == 2 || previous == 1){
                    ib = 1;
                }
                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 0 && (rw+cl) % 2 == 1){
                rw++;
                if(rw > ptr->rows){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 1;
                    previous = 0;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 0 && (rw+cl) % 2 == 1){
                if(previous == 2 || previous == 1){
                    ib = 2;
                }
                continue;
                
            }
        }

        
        goto the_end;
        
    }

    // ***********************************************************************************************************************************************

    if(strcmp(argv[1], "--lpath") == 0){

        if(valid_borders_test(ptr) == 1){   //ci je maze vobec valid
            printf("Maze is invalid..\n");
            goto the_end;
        }

        for(int i = 0; i < has_start; i++){

            if(rw == possible_starts[i][0] && cl == possible_starts[i][1]) // ci sa da zacat na zadanych suradniciach
                break;

            if(i == has_start-1){
                fprintf(stderr, "ERROR: Start not possible on given coords\n");
                goto the_end;
            }
        }
   
        if(has_start < 1){
            
            fclose(file);
            freemap(ptr);
            free(ptr);

            printf("Maze is invalid..\n\n");
            return 1;
        }


        if(start_border(ptr, start[0], start[1], 1) == 2 && cl == 1 && rw % 2 == 0){   //zaciatok z lava, parny riadok, zaujima ma prava najprv
            ib = 2;
            previous = 1;
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 1) == 0 && cl == 1 && rw % 2 == 1){   //zaciatok z lava, neparny riadok, zaujma ma najprv horna
        
            if(rw == 1 && (ptr->cells[0] == '1' || ptr->cells[0] == '2' || ptr->cells[0] == '3' || ptr->cells[0] == '0')){
                ib = 2;
                previous = 0;
            }
            if(rw == 1 && (ptr->cells[0] == '6' || ptr->cells[0] == '4')){
                ib = 0;
                previous = 1;
            }
            if(rw != 1){
                previous = 1;
                ib = 0;
            }
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 1) == 2 && rw == 1 && cl != ptr->cols){   //zaciatok zhora, najprv ma zaujma prava
            ib = 2;
            previous = 0;
            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 1) == 1 && rw == ptr->rows && cl != ptr->cols){   //zaciatok zdola, najprv ma zaujima lava
            
            ib = 1;
            previous = 0;

            printf("%d,%d\n", rw, cl);
        }
        if(start_border(ptr, start[0], start[1], 1) != 2 && cl == ptr->cols && ptr->cols % 2 == 0){   //zaciatok sprava

            
            if(rw != ptr->rows && rw % 2 == 1){    //ma spodnu hranicu
                previous = 2;
                ib = 0;                
            }
            if((rw != ptr->rows && rw % 2 == 0) || rw == ptr->rows){  // ma hornu hranicu
                previous = 2;
                ib = 1;
            }
            
            printf("%d,%d\n", rw, cl);
        }
        
        if(start_border(ptr, start[0], start[1], 1) != 2 && cl == ptr->cols && ptr->cols % 2 == 1){  //zaciatok sprava

            
            if(rw == 1 && (ptr->cells[ptr->cols-1] == '2' || ptr->cells[ptr->cols-1] == '3')){
                previous = 0;
                ib = 2;        
            }
            if(rw == 1 && (ptr->cells[ptr->cols-1] == '1' || ptr->cells[ptr->cols-1] == '4' || ptr->cells[ptr->cols-1] == '6' || ptr->cells[ptr->cols-1] == '0' )){
                previous = 2;
                ib = 1;
            }

            if((rw != 1 && rw % 2 == 1) || rw == ptr->rows){    //ma hornu hranicu
                previous = 2;
                ib = 1;                
            }
            
            if(rw != 1 && rw % 2 == 0){  // ma spodnu hranicu
                
                previous = 2;
                ib = 0;
            }

            

            printf("%d,%d\n", rw, cl);
        }
        
        
        while(true){
            
            if(isborder(ptr, rw, cl, ib) == true && ib == 2 && (rw+cl) % 2 == 0){
                cl++;
                if(cl > ptr->cols){
                    break;
                } else{
                    printf("%d,%d\n", rw, cl);
                    ib = 2;
                    previous = 1;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 2 && (rw+cl) % 2 == 0){
                if(previous == 1 || previous == 0){
                    ib = 1;
                }

                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 2 && (rw+cl) % 2 == 1){
                cl++;
                if(cl > ptr->cols){
                    break;
                } else{
                    printf("%d,%d\n", rw, cl);
                    ib = 0;
                    previous = 1;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 2 && (rw+cl) % 2 == 1){
                if(previous == 1 || previous == 0){
                    ib = 0;
                }
                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 1 && (rw+cl) % 2 == 1){
                cl--;
                if(cl < 1){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 1;
                    previous = 2;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 1 && (rw+cl) % 2 == 1){
                
                if(previous == 0 || previous == 2){
                    ib = 2;
                }

                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 1 && (rw+cl) % 2 == 0){
                cl--;
                if(cl < 1){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 0;
                    previous = 2;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 1 && (rw+cl) % 2 == 0){
                if(previous == 2 || previous == 0){
                    ib = 0;
                }
    
                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 0 && (rw+cl) % 2 == 0){
                rw--;
                if(rw < 1){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 1;
                    previous = 0;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 0 && (rw+cl) % 2 == 0){
                if(previous == 1 || previous == 2){
                    ib = 2;
                }

                continue;
            }
            if(isborder(ptr, rw, cl, ib) == true && ib == 0 && (rw+cl) % 2 == 1){
                rw++;
                if(rw > ptr->rows){
                    break;
                } else {
                    printf("%d,%d\n", rw, cl);
                    ib = 2;
                    previous = 0;
                    continue;
                }
            } else if(isborder(ptr, rw, cl, ib) == false && ib == 0 && (rw+cl) % 2 == 1){
                if(previous == 1 || previous == 2){
                    ib = 1;
                }

                continue;
                
            }
        }
        goto the_end;
        
    }
    
    the_end:
        fclose(file);
        freemap(ptr);
        free(ptr);

        return 0;

}
