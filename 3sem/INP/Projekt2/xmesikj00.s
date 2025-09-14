; Autor reseni: Juraj Mesík xmesikj00

; Projekt 2 - INP 2024
; Vigenerova sifra na architekture MIPS64

; DATA SEGMENT
                .data
msg:            .asciiz "jurajmesik" ; zasifrovane "wpknotrnbx" 
cipher:         .space  31 ; misto pro zapis zasifrovaneho textu

value_m_shift:  .word 13 ; hodnota posunu 'm'
value_e_shift:  .word 5  ; hodnota posunu 'e' 
value_s_shift:  .word 19 ; hodnota posunu 's'

name_lenght:    .byte 10    ; dlzka mena
value_z:        .byte 122   ; ascii 'z'
value_a:        .byte 97    ; ascii 'a'

params_sys5:    .space  8 ; misto pro ulozeni adresy pocatku
                          ; retezce pro vypis pomoci syscall 5
                          ; (viz nize "funkce" print_string)

; CODE SEGMENT
                .text

main:           ; ZDE NAHRADTE KOD VASIM RESENIM

    lb $t4, value_z($zero)                      ; do $t4 ulozim ascii hodnotu 'z'
    lb $t5, value_a($zero)                      ; do $t5 ulozim ascii hodnotu 'a'
    addi $t6, $t6, 1       
                     
    letter_count:
                lb $t0, msg($t3)                ; v registri $t3 budem mat ulozenu dlzku mena
                beq $t0, $zero, loop
                addi $t3, $t3, 1
                b letter_count


    loop:                                       ; cyklus sa opakuje po 6tich pismenach - 6 posunov tvori pattern
                lb $t0, msg($t2)                ; do t0 nacitam pismeno z 'msg' na pozicii s indexom $t2
                lb $t1, value_m_shift($zero)    ; do $t1 nacitam hodnotu, ktorou sifrujem (prve je 'm')
                add $t0, $t0, $t1               ; t0 = t0 + t1 = ascii hodnota zasifrovaneho pismena
                sub $t4, $t4, $t0               ; t4 = t4 - t0
                bgez $t4, ok1                   ; ak t4 >= 0 preskocim modulo1
                b modulo1                       ; ak je ascii hodnota sifr. pismena vacsia ako 'z' musim vypocitat kruhovy posun
            ok1:
                sb $t0, cipher($t2)             ; na index t2 retazca 'cipher' ulozim zasifrovane pismeno
                beq $t2, $t3, end               ; pokial som spracoval posledne pismeno, prejdem na 'end', kde sa skonci program
                addi $t2, $t2, 1                ; inak sa inkrementuje index $t2
                movz $t0, $zero, $zero          ; reset t0 na 0
                lb $t4, value_z($zero)          ; reset t4 na 'z'
                lb $t5, value_a($zero)          ; reset t5 na 'a'




                lb $t0, msg($t2)
                lb $t1, value_e_shift($zero)
                sub $t0, $t0, $t1               ; kazdy druhy posun je zaporny, preto t0 = t0 - 'e'
                sub $t5, $t0, $t5               ; t5 =  t0 - 'a'
                bgez $t5, ok2                   ; ak t5 >= 0, nepocitam kruhovy posun
                b modulo2               
            ok2:
                sb $t0, cipher($t2)
                beq $t2, $t3, end
                addi $t2, $t2, 1                
                movz $t0, $zero, $zero          
                lb $t4, value_z($zero)          
                lb $t5, value_a($zero)          
            



                lb $t0, msg($t2)
                lb $t1, value_s_shift($zero)   
                add $t0, $t0, $t1              ; t0 = t0 + 's'
                sub $t4, $t4, $t0              ; t4 = 'z' - t0 
                bgez $t4, ok3
                b modulo3
            ok3:
                sb $t0, cipher($t2)             
                beq $t2, $t3, end               
                addi $t2, $t2, 1
                movz $t0, $zero, $zero
                lb $t4, value_z($zero)          
                lb $t5, value_a($zero)           



                lb $t0, msg($t2)                
                lb $t1, value_m_shift($zero)      
                sub $t0, $t0, $t1               ; t0 = t0 - 'm'
                sub $t5, $t0, $t5               ; t5 = t0 - 'a'
                bgez $t5, ok4                   
                b modulo4                       
            ok4:
                sb $t0, cipher($t2)             
                beq $t2, $t3, end               
                addi $t2, $t2, 1                
                movz $t0, $zero, $zero          
                lb $t4, value_z($zero)          
                lb $t5, value_a($zero)          




                lb $t0, msg($t2)
                lb $t1, value_e_shift($zero)
                add $t0, $t0, $t1               ; t0 = t0 + 'e'
                sub $t4, $t4, $t0               ; t4 = 'z' - t0
                bgez $t4, ok5                                 
                b modulo5
            ok5:
                sb $t0, cipher($t2)
                beq $t2, $t3, end
                addi $t2, $t2, 1
                movz $t0, $zero, $zero
                lb $t4, value_z($zero)          
                lb $t5, value_a($zero)          
            



                lb $t0, msg($t2)
                lb $t1, value_s_shift($zero)   
                sub $t0, $t0, $t1               ; t0 = t0 - 's'
                sub $t5, $t0, $t5               ; t5 = t0 - 'a'
                bgez $t5, ok6    
                b modulo6
            ok6:
                sb $t0, cipher($t2)             
                beq $t2, $t3, end               
                addi $t2, $t2, 1
                movz $t0, $zero, $zero
                lb $t4, value_z($zero)          
                lb $t5, value_a($zero)         


                b loop                          ; ak doteraz nebola hodnota $t2 rovna dlzke mena, prejdem na dalsiu iteraciu

        modulo1:
                    sub $t4, $zero, $t4             ; negujem -- -t4 -> t4
                    add $t5, $t5, $t4               ; ku 'a' pricitam zvysok po presiahnuti 'z' -- t5 = t5 + t4
                    sub $t5, $t5, $t6
                    movz $t0, $t5, $zero            ; t0 = nova hodnota po posune
                    b ok1                           ; vratim sa spat do cyklu
        
        modulo2:
                    sub $t5, $zero, $t5             ; negujem -- -t5 -> t5
                    sub $t4 , $t4, $t5              ; od 'z' odcitam zvysok po presiahnuti 'a' -- t4 = t4 - t5
                    addi $t4, $t4, 1                
                    movz $t0, $t4, $zero            ; t0 = nova hodnota po posune
                    b ok2

        modulo3:
                    sub $t4, $zero, $t4             ; negujem -- -t4 -> t4
                    add $t5, $t5, $t4               ; k 'a' pricitam zvysok po presiahnuti 'z' -- t5 = t5 + t4
                    sub $t5, $t5, $t6
                    movz $t0, $t5, $zero            ; t0 = nova hodnota po posune
                    b ok3

        modulo4:
                    sub $t5, $zero, $t5             ; negujem -- -t5 -> t5
                    sub $t4 , $t4, $t5              ; od 'z' odcitam zvysok po presiahnuti 'a' -- t4 = t4 - t5
                    addi $t4, $t4, 1
                    movz $t0, $t4, $zero            ; t0 = nova hodnota po posune
                    b ok4          
        modulo5:
                    sub $t4, $zero, $t4             ; negujem -- -t4 -> t4
                    add $t5, $t5, $t4               ; k 'a' pricitam zvysok po presiahnuti 'z' -- t5 = t5 + t4
                    sub $t5, $t5, $t6
                    movz $t0, $t5, $zero            ; t0 = nova hodnota po posune
                    b ok5                           

        modulo6:
                    sub $t5, $zero, $t5             ; negujem -- -t5 -> t5
                    sub $t4 , $t4, $t5              ; od 'z' odcitam zvysok po presiahnuti 'a' -- t4 = t4 - t5
                    addi $t4, $t4, 1
                    movz $t0, $t4, $zero            ; t0 = nova hodnota po posune
                    b ok6

        end:    

                sb $zero, cipher($t2)               ; na koniec sifry vlozim 0

                daddi $a0, $zero, cipher            ; predam 'cipher' ako argument pre syscall
                jal     print_string ; vypis pomoci print_string - viz nize


; NASLEDUJICI KOD NEMODIFIKUJTE!

                syscall 0   ; halt

print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
