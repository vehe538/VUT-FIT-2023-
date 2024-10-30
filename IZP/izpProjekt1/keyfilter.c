#include <stdio.h>
#include <string.h>



int main(int argc, char *argv[]){


	char c;
	int num_of_cities = 0;
	int p = 0;


	char cities[42][20];

	while((c = getchar())!= EOF){

		cities[num_of_cities][p] = c;
		p++;
		if(c == '\n'){
			num_of_cities++;
			p = 0;
		}
	

	}


    if(num_of_cities == 0) {

        printf("ERROR: input file is empty or new line wasn't set after the city name\n");
        return 1;
    }

    for(int i = 0; i < num_of_cities; i++){

        if(cities[i][0] > 96){
            printf("ERROR: all city names must start with uppercase letter\n");
            return 1;
        }
    }

    for(int i = 0; i < num_of_cities; i++){
        int j = 1;

        while(cities[i][j] != '\n'){
            if(cities[i][j] < 97 || cities[i][j] > 122){
                printf("ERROR: all city names should contain only letters\n");
                return 1;
            }
            j++;
        }
    }


    //AK NENI INPUT PRAZDNY:
    if(argc > 1){
        int len_of_input = strlen(argv[1]);
        char output[num_of_cities];
        int len_of_output = 0;


        //zistenie ktore mesta sa zhoduju:
        for(int i = 0; i < num_of_cities; i++){
            int match = 0;
            for(int j = 0; j < len_of_input; j++){
                
                if(cities[i][j] == argv[1][j] || cities[i][j] == argv[1][j]-('a'-'A')){
                    match++;
                }

            }
            if(match == len_of_input){
                output[len_of_output] = cities[i][len_of_input]-('a'-'A');
                len_of_output++;
            }


        }
        output[len_of_output] = '\0';
        len_of_output = strlen(output);

        //vymazanie duplikatov

        char new_output[len_of_output];
        new_output[0] = output[0];
        int l = strlen(new_output);

        int is = 0;

        for(int i = 1; i < len_of_output; i++){
            is = 0;
            
            for(int j = 0; j < len_of_output;j++){
                if(output[i] == new_output[j]){
                    is = 1;
                    
                }
                
            }
            
            if(is == 0){
                new_output[l] = output[i];
                l++;
            }
            
        }
        len_of_output = strlen(new_output);
        new_output[l] = '\0';


        //zoradenie do abc poradia:
        int is_min = 0;
        int is_max;
        int x = 0;
        char output_abc[len_of_output];

        if(len_of_output == 2){
            if(new_output[0] < new_output[1]){
                output_abc[0] = new_output[0];
                output_abc[1] = new_output[1];
            } else {
                output_abc[0] = new_output[1];
                output_abc[1] = new_output[0];
            }
            output_abc[len_of_output] = '\0';
        }

        if(len_of_output > 2){

            for(int i = 0; i < len_of_output; i++){
                is_min = 0;
                is_max = 0;
                for(int j = 0; j < len_of_output; j++){

                    if(new_output[i] < new_output[j] && i != j){
                        is_min++;
                    } else if(new_output[i] > new_output[j] && i != j){
                        is_max++;
                    }

                }
                if(is_min == len_of_output-1){
                    output_abc[x] = new_output[i];
                    x++;
                }
                if(i == len_of_output-1){
                    output_abc[x] = new_output[i-(len_of_output-1)];
                } else if(is_max == len_of_output-1){

                    output_abc[len_of_output-1] = new_output[i];
                }
            }
            output_abc[len_of_output] = '\0';
        }

        //AK JE V 'ENABLE' POSLEDNE PISMENO NEJAKEHO MESTA (Brn -O)
        

        if(len_of_output == 1){

            int try_len = 0;
            int issame = 0;
            int city_len = 0;

            char try[len_of_input];

            

            for(int i = 0; i < num_of_cities; i++){

                try[0] = argv[1][0] - ('a'-'A');

                try_len = 0;
                for(int j = 1; j < len_of_input+1; j++){
                    try[j] = argv[1][j];
                    try_len++;
                }

                
                try[try_len] = output[0] + ('a'-'A');
                issame = 0;
                city_len = 0;
                try[try_len+1] = '\0';


                int c = 0;
                while(cities[i][c] != '\n'){
                    city_len++;
                    c++;
                } 


                for(int k = 0; k < try_len+1; k++){
                   

                    if(cities[i][k] != try[k]){
                        issame = 1;
                        break;
                    }
                }

                if(issame == 0 && try_len+1 == city_len){
                    for(int p = 0; p<try_len+1;p++){
                        if(try[p] > 'Z'){
                            try[p] -= ('a'-'A');
                        }
        
                    }
                    printf("Found %s\n", try);
                    return 0;
                }
            }
        }


        if(output[0] < 0){  //ak zadam cele mesto

            char output[len_of_input];
            for(int i = 0; i < len_of_input; i++){
                output[i] = argv[1][i]-('a'-'A');
            }
            printf("Found %s", output);
            return 0;
        } else if(output[0] > 0){
            printf("Enable: %s\n", output_abc);
            return 0;
        }
    }
    //AK JE INPUT PRAZDNY:
    if(argc == 1){

        char output[num_of_cities];

        //zadanie pismen do outputu
        for(int i = 0; i < num_of_cities; i++){
            output[i] = cities[i][0];
        }

        output[num_of_cities] = '\0';        
        int len_of_output = strlen(output);
        
        //vymazanie duplikatov
        char new_output[num_of_cities];
        new_output[0] = output[0];
        len_of_output = 1;

        int is = 0;
        for(int i = 0; i < num_of_cities; i++){

            is = 0;
            
            for(int j = 0; j<len_of_output;j++){
                if(output[i] == output[j-1]){
                    is = 1;
                }
                
            }
            if(is == 0){
                new_output[len_of_output-1] = output[i];
                len_of_output++;
            }
            
        }

        len_of_output = strlen(new_output);
        new_output[len_of_output] = '\0';

        //zoradenie podla abc
        int is_min = 0;
        int x = 0;
        char output_abc[len_of_output];

        for(int i = 0; i < len_of_output; i++){
            is_min = 0;
            for(int j = 0; j < len_of_output; j++){

                if(new_output[i] < new_output[j] && i != j){
                    is_min++;
                }

            }
            if(is_min == len_of_output-1){
                output_abc[x] = new_output[i];
                x++;
            }
            if(i == len_of_output-1){
                output_abc[x] = new_output[i-(len_of_output-1)];
            }
        }
        output_abc[len_of_output] = '\0';
        printf("Enable: %s\n", output_abc);
        return 0;
    }
    printf("Not found...\n");
    return 0;

}