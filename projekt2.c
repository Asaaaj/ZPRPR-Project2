/*
	Meno: Adam Jurcisin
	Predmet: Zaklady Proceduralneho Programovania 2
	Skupina: 53
	Tema: Projekt 2
    IDE: VS Code
    Termin: 24.4.2022
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct osoba{
    char meno[200];
    char priezvisko[200];
    struct osoba *dalsi;
}OSOBA;

typedef struct zoznam{
    int id;
    char nazov[151];
    OSOBA *autor;
    char typ[3];
    char cas[5];
    char datum[9];
    struct zoznam *dalsi;
}ZOZNAM;

ZOZNAM *funkcia_n(FILE **subor, ZOZNAM *spajany_zoznam);
void funkcia_v(ZOZNAM *spajany_zoznam);
void funkcia_p(ZOZNAM **spajany_zoznam);
void funkcia_z(ZOZNAM **spajany_zoznam);
void funkcia_h(ZOZNAM *spajany_zoznam);
void funkcia_a(ZOZNAM **spajany_zoznam);
void funkcia_r(ZOZNAM **spajany_zoznam);

int main(){
    FILE *subor = NULL;
    ZOZNAM *spajany_zoznam = NULL;
    char znak;

    do {
        printf("Zadaj znak: ");
        scanf("%c", &znak);
        switch(znak){
            case 'n':
                spajany_zoznam = funkcia_n(&subor, spajany_zoznam);
                break;
            case 'v':;
                funkcia_v(spajany_zoznam);
                break;
            case 'p':
                funkcia_p(&spajany_zoznam);
                break;
            case 'z':
                funkcia_z(&spajany_zoznam);
                break;
            case 'h':
                funkcia_h(spajany_zoznam);
                break;
            case 'a':
                funkcia_a(&spajany_zoznam);
                break;    
            case 'r':   
                funkcia_r(&spajany_zoznam);
                break;
            case 'k':
                if(subor != NULL) fclose(subor);
                while(spajany_zoznam != NULL) {
                    ZOZNAM *temp = spajany_zoznam;
                    spajany_zoznam = spajany_zoznam->dalsi;
                    free(temp);
                }
                break;
        }
    } while(znak != 'k');
    return 0;
}

ZOZNAM *funkcia_n(FILE **subor, ZOZNAM *spajany_zoznam){
    ZOZNAM *hlava = NULL, *temp = NULL;
    OSOBA *osoba = NULL, *pomocna = NULL;
    char nacitany_riadok[202], *tokenOsoby = NULL, **nacitane_meno, *tokenMena = NULL;;
    int pocet_zaznamov = 0, pocet_autorov = 0;
    
    //otvorenie suboru
    if(*subor == NULL){
        *subor = fopen("OrganizacnePodujatia2.txt", "r");
        if(*subor == NULL) printf("Zaznamy neboli nacitane!\n");
        return NULL;
    }
    
    while(spajany_zoznam != NULL){ //vymazanie spajaneho zoznamu
        osoba = spajany_zoznam->autor;
        while(osoba != NULL){
            pomocna = osoba;
            osoba = osoba->dalsi;
            free(pomocna);
        }
        spajany_zoznam->autor = NULL;
        temp = spajany_zoznam;
        spajany_zoznam = spajany_zoznam->dalsi;
        free(temp);
    }
    spajany_zoznam = NULL;

    spajany_zoznam = (ZOZNAM *) malloc (sizeof(ZOZNAM));

    hlava = spajany_zoznam;
    fseek(*subor, 0, SEEK_SET);

    while(fgets(nacitany_riadok, 200, *subor) != NULL){ //nacitanie podla rozdelovacieho riadku
        pocet_autorov = 1;
        fgets(nacitany_riadok, 17, *subor);
        spajany_zoznam->id = atoi(nacitany_riadok);

        fgets(nacitany_riadok, 152, *subor);
        nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
        strcpy(spajany_zoznam->nazov, nacitany_riadok);

        fgets(nacitany_riadok, 202, *subor);
        nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';

        for(int i = 0; i < strlen(nacitany_riadok); i++){ //zistenie poctu autorov jedneho prispevku
            if(nacitany_riadok[i] == '#'){
                pocet_autorov++;
            }
        }

        //pridanie kazdeho autora samostatne do dynamickych poli
        nacitane_meno = (char **) malloc ((pocet_autorov+1 )* sizeof(char*));
        for(int j = 0; j < pocet_autorov; j++){
            nacitane_meno[j] = (char *) malloc (203 * sizeof(char));
        }
        tokenOsoby = strtok(nacitany_riadok, "#");
        strcpy(nacitane_meno[0], tokenOsoby);
        osoba = (OSOBA *) malloc (sizeof(OSOBA));
        spajany_zoznam->autor = osoba;
        for(int k = 1; k < pocet_autorov; k++){
            tokenOsoby = strtok(NULL, "#");
            strcpy(nacitane_meno[k], tokenOsoby);
            osoba->dalsi = (OSOBA *) malloc (sizeof(OSOBA));
            osoba = osoba->dalsi;
        }
        osoba->dalsi = NULL;

        
        osoba = spajany_zoznam->autor;
        for(int m = 0; m < pocet_autorov; m++){ //prejdenie kazdeho pola a ulozenie mena a priezviska do spajaneho zoznamu
            tokenMena = strtok(nacitane_meno[m], " ");
            strcpy(osoba->meno, tokenMena);
            while((tokenMena = strtok(NULL, " ")) != NULL){
                strcat(osoba->priezvisko, tokenMena);
                strcat(osoba->priezvisko, " ");
            }
            osoba->priezvisko[strlen(osoba->priezvisko) - 1] = '\0';
            osoba = osoba->dalsi;
        }

        //uvolnenie pamate dynamickych poli
        for(int n = 0; n <= pocet_autorov; n++){
            free(nacitane_meno[n]);
            nacitane_meno[n] = NULL;
        }
        free(nacitane_meno);
        nacitane_meno = NULL;


        fgets(nacitany_riadok, 4, *subor);
        if((nacitany_riadok[strlen(nacitany_riadok) - 1]) == '\n') nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
        strcpy(spajany_zoznam->typ, nacitany_riadok);

        fgets(nacitany_riadok, 6, *subor);
        nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
        strcpy(spajany_zoznam->cas, nacitany_riadok);

        fgets(nacitany_riadok, 10, *subor);
        nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
        strcpy(spajany_zoznam->datum, nacitany_riadok);
        
        pocet_zaznamov++;

        //vytvorenie dalsieho zaznamu
        spajany_zoznam->dalsi = (ZOZNAM *) malloc(sizeof(ZOZNAM));
        temp = spajany_zoznam; //ulozenie ukazovatela na predposledny zaznam
        spajany_zoznam = spajany_zoznam->dalsi;
        osoba = NULL;
    }

    //v pripade ze bol zaznam posledny uvolenie pamate dalsieho zaznamu a nastavenie ukazovatela na dalsi zaznam na NULL
    temp->dalsi = NULL;
    free(spajany_zoznam);

    printf("Nacitalo sa %d zaznamov\n", pocet_zaznamov);

    return hlava;
}
 
void funkcia_v(ZOZNAM *spajany_zoznam){
    int cislo_zaznamu = 1, cislo_osoby = 1;
    OSOBA *osoba;

    if(spajany_zoznam == NULL){
        printf("Prazdny zoznam zaznamov.\n");
        return;
    }
    printf("\n");
    while(spajany_zoznam != NULL){ //vypisanie spajaneho zoznamu
        osoba = spajany_zoznam->autor;
        printf("%d.\n", cislo_zaznamu++);
        printf("ID cislo: %d\n", spajany_zoznam->id);
        printf("Nazov prispevku: %s\n", spajany_zoznam->nazov);
        printf("Mena autorov:\n");
        while(osoba != NULL){ //vypisanie spajaneho zoznamu osob
            printf("\t%d: %s %s\n", cislo_osoby++, osoba->meno, osoba->priezvisko);
            osoba = osoba->dalsi;
        }
        printf("Typ prezentovania: %s\n", spajany_zoznam->typ);
        printf("Cas prezentovania: %s\n", spajany_zoznam->cas);
        printf("Datum: %s\n", spajany_zoznam->datum);

        cislo_osoby = 1;
        spajany_zoznam = spajany_zoznam->dalsi;
    }
}

void funkcia_p(ZOZNAM **spajany_zoznam){
    ZOZNAM *novy = NULL, *zaciatok = *spajany_zoznam, *temp = NULL;
    OSOBA *osoba = NULL;
    int pocet_zaznamov = 1, pocet_autorov = 1, pozicia;
    char nacitany_riadok[202], **nacitane_meno = NULL, *tokenOsoby, *tokenMena;
    scanf("%d", &pozicia);

    if(pozicia < 0) pozicia = -pozicia;
    
    getc(stdin); //zachytenie nevhodneho znaku z main slucky
    novy = (ZOZNAM *) malloc (sizeof(ZOZNAM));

    //pridanie noveho zaznamu
    fgets(nacitany_riadok, 17, stdin);
    novy->id = atoi(nacitany_riadok);
    fgets(nacitany_riadok, 152, stdin);
    nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
    strcpy(novy->nazov, nacitany_riadok);
    fgets(nacitany_riadok, 202, stdin);
    nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';

    for(int i = 0; i < strlen(nacitany_riadok); i++){ //zistenie poctu autorov jedneho prispevku
        if(nacitany_riadok[i] == '#'){
            pocet_autorov++;
        }
    }

    //pridanie kazdeho autora samostatne do dynamickych poli
    nacitane_meno = (char **) malloc ((pocet_autorov+1 )* sizeof(char*));
    for(int j = 0; j < pocet_autorov; j++){
        nacitane_meno[j] = (char *) malloc (201 * sizeof(char));
    }
    tokenOsoby = strtok(nacitany_riadok, "#");
    strcpy(nacitane_meno[0], tokenOsoby);
    osoba = (OSOBA *) malloc (sizeof(OSOBA));
    novy->autor = osoba;
    for(int k = 1; k < pocet_autorov; k++){
        tokenOsoby = strtok(NULL, "#");
        strcpy(nacitane_meno[k], tokenOsoby);
        osoba->dalsi = (OSOBA *) malloc (sizeof(OSOBA));
        osoba = osoba->dalsi;
    }
    osoba->dalsi = NULL;

        
    osoba = novy->autor;
    for(int m = 0; m < pocet_autorov; m++){ //prejdenie kazdeho pola a ulozenie mena a priezviska do spajaneho zoznamu
        tokenMena = strtok(nacitane_meno[m], " ");
        strcpy(osoba->meno, tokenMena);
        while((tokenMena = strtok(NULL, " ")) != NULL){
            strcat(osoba->priezvisko, tokenMena);
            strcat(osoba->priezvisko, " ");
        }
        osoba->priezvisko[strlen(osoba->priezvisko) - 1] = '\0';
        osoba = osoba->dalsi;
    }

    //uvolnenie pamate dynamickych poli
    for(int n = 0; n < pocet_autorov; n++){
        free(nacitane_meno[n]);
        nacitane_meno[n] = NULL;
    }
    free(nacitane_meno);
    nacitane_meno = NULL;

    fgets(nacitany_riadok, 4, stdin);
    nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
    strcpy(novy->typ, nacitany_riadok);
    fgets(nacitany_riadok, 6, stdin);
    nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
    strcpy(novy->cas, nacitany_riadok);
    fgets(nacitany_riadok, 10, stdin);
    nacitany_riadok[strlen(nacitany_riadok) - 1] = '\0';
    strcpy(novy->datum, nacitany_riadok);
    
    if(pozicia == 1){ //pridanie zaznamu na prvu poziciu
        novy->dalsi = *spajany_zoznam;
        *spajany_zoznam = novy;
        zaciatok = *spajany_zoznam;
    }
    else{ //pridanie zaznamu na inu ako prvu poziciu
        while(*spajany_zoznam != NULL){ //zistenie poctu zaznamov
            pocet_zaznamov++;
            *spajany_zoznam = (*spajany_zoznam)->dalsi;
        }
        *spajany_zoznam = zaciatok;

        for(int i = 1; i <= pocet_zaznamov; i++){ //najdenie pozicie na vlozenie noveho zaznamu
            if((i + 1) == pozicia){
                temp = (*spajany_zoznam)->dalsi;
                (*spajany_zoznam)->dalsi = novy;
                novy->dalsi = temp;
                break;
            }
            if((*spajany_zoznam)->dalsi == NULL){ //pridanie na koniec zoznamu
            novy->dalsi = NULL;
            (*spajany_zoznam)->dalsi = novy;
            }
            else *spajany_zoznam = (*spajany_zoznam)->dalsi;
        }
        *spajany_zoznam = zaciatok;
    }
}

void funkcia_z(ZOZNAM **spajany_zoznam){
    ZOZNAM *hlava = *spajany_zoznam, *prev = *spajany_zoznam, *pomocna = NULL;
    OSOBA *osoba = NULL, *temp;
    char nacitane_meno[202], zoznam_meno[202];
    int vymena = 0;

    fgets(nacitane_meno, 202, stdin);
    fgets(nacitane_meno, 202, stdin);
    nacitane_meno[strlen(nacitane_meno) - 1] = '\0';
    printf("nacitanemeno:%s|\n", nacitane_meno);

    if(*spajany_zoznam != NULL){
        while((*spajany_zoznam) != NULL){
            osoba = (*spajany_zoznam)->autor;
            while(osoba != NULL){
                strcpy(zoznam_meno, osoba->meno);
                strcat(zoznam_meno, " ");
                strcat(zoznam_meno, osoba->priezvisko);

                if(strcasecmp(nacitane_meno, zoznam_meno) == 0){
                    vymena = 1;
                    printf("Prispevok s nazovom %s bol vymazany.\n", (*spajany_zoznam)->nazov);
                    while(osoba != NULL){
                        temp = osoba;
                        osoba = osoba->dalsi;
                        free(temp);
                    }
                    (*spajany_zoznam)->autor = NULL;

                    if(*spajany_zoznam == hlava) hlava = (*spajany_zoznam)->dalsi;
                    if((*spajany_zoznam)->dalsi != NULL){
                        prev->dalsi = (*spajany_zoznam)->dalsi; 
                    }
                    else{
                        prev->dalsi = NULL;
                    }

                    pomocna = *spajany_zoznam;
                    *spajany_zoznam = (*spajany_zoznam)->dalsi;
                    free(pomocna);
                    break;
                }
                osoba = osoba->dalsi; 
            }
            if(vymena == 0){
                prev = *spajany_zoznam;
                *spajany_zoznam = (*spajany_zoznam)->dalsi; 
            }
            vymena = 0;
        }
    }
    *spajany_zoznam = hlava;
}

void funkcia_h(ZOZNAM *spajany_zoznam){
    char typ[4];
    int cislo_prispevku = 1, cislo_autora = 1;
    OSOBA *osoba;
    getc(stdin); //zachytenie nevhodneho znaku z main slucky
    fgets(typ, 4, stdin);
    typ[strlen(typ) - 1] = '\0';
    if(spajany_zoznam != NULL){
        while(spajany_zoznam != NULL){ //prejdenie zoznamu 
            if(strcmp(typ, spajany_zoznam->typ) == 0){ //vypis ak sa zhoduju typy
                osoba = spajany_zoznam->autor;
                printf("%d.\n", cislo_prispevku++);
                printf("ID cislo: %d\n", spajany_zoznam->id);
                printf("Nazov prispevku: %s\n", spajany_zoznam->nazov);
                printf("Mena autorov:\n");
                while(osoba != NULL){ //vypisanie spajaneho zoznamu osob
                    printf("\t%d: %s %s\n", cislo_autora++, osoba->meno, osoba->priezvisko);
                    osoba = osoba->dalsi;
                }
                printf("Typ prezentovania: %s\n", spajany_zoznam->typ);
                printf("Cas prezentovania: %s\n", spajany_zoznam->cas);
                printf("Datum: %s\n", spajany_zoznam->datum);

                cislo_autora = 1;
            }
            spajany_zoznam = spajany_zoznam->dalsi;
        }
    }
    if(cislo_prispevku == 1){
        printf("Pre typ %s nie su ziadne zaznamy.\n", typ);
    }
}

void funkcia_a(ZOZNAM **spajany_zoznam){
    int id; 
    char idchar[150], typ[3], povodny_typ[3], ok = 'f';
    ZOZNAM *zaciatok = *spajany_zoznam;
    
    scanf("%s %s", &idchar, &typ);
    id = atoi(idchar);

    while(ok != 't'){ //kontrola ci zadane id a typ splnaju podmienky  
        if((id%15 == 0) && ((strcmp(typ, "PD") == 0) || (strcmp(typ, "UD") == 0) || (strcmp(typ, "PP") == 0) || (strcmp(typ, "UP") == 0))){
            ok = 't';
        }
        else{ //ak nesplnaju podmienky zadanie udajov znova
            printf("Zadane udaje nie su korekne, zadaj novy retazec:");
            scanf("%d", &id);
            scanf("%s", typ);
        } 
    }

    if(*spajany_zoznam != NULL){
        while(*spajany_zoznam != NULL){
            if(id == (*spajany_zoznam)->id){ //aktualizovanie zaznamu so zhodnym id
                strcpy(povodny_typ, (*spajany_zoznam)->typ);
                strcpy((*spajany_zoznam)->typ, typ);
                printf("Prispevok s nazvom %s sa bude prezentovat %s [%s].\n", (*spajany_zoznam)->nazov, (*spajany_zoznam)->typ, povodny_typ);
            }
            *spajany_zoznam = (*spajany_zoznam)->dalsi;
        }
    }
    *spajany_zoznam = zaciatok;
}

void funkcia_r(ZOZNAM **spajany_zoznam){
    int cislo1, cislo2, temp, i = 2;
    ZOZNAM *prev1 = *spajany_zoznam, *next1 = NULL, *node1 = NULL, *hlava = *spajany_zoznam;
    ZOZNAM *prev2 = *spajany_zoznam, *next2 = NULL, *node2 = NULL;
    scanf("%d %d", &cislo1, &cislo2);

    if(cislo1 > cislo2){ //prehodenie cisel
        temp = cislo1;
        cislo1 = cislo2;
        cislo2 = temp;
    }

    if(*spajany_zoznam != NULL){
        temp = 0;
        while(*spajany_zoznam != NULL){ //zistenie poctu zaznamov
            temp++;
            *spajany_zoznam = (*spajany_zoznam)->dalsi;
        }
        *spajany_zoznam = hlava;
        
        if(cislo2 > temp || cislo1 == cislo2){ //ak je cislo vacsie ako pocet zaznamov alebo sa cisla rovnaju
            return;
        }

        if(cislo1 == 1){ //ak prehadzujeme prvy zaznam
            node1 = *spajany_zoznam;
            next1 = (*spajany_zoznam)->dalsi;
        }

        while((*spajany_zoznam)->dalsi != NULL){ //prechadzanie zoznamom
            if(i == cislo1){ //nastavenie prveho zaznamu na prehodenie
                prev1 = *spajany_zoznam;
                node1 = (*spajany_zoznam)->dalsi;
                next1 = (*spajany_zoznam)->dalsi->dalsi;
            }
            else if(i == cislo2){ //nastavenie druheho zaznamu na prehodenie
                prev2 = *spajany_zoznam;
                node2 = (*spajany_zoznam)->dalsi;
                next2 = (*spajany_zoznam)->dalsi->dalsi;
            }
            i++;
            *spajany_zoznam = (*spajany_zoznam)->dalsi;
        }

        if(cislo1 == 1){ //ak prehadzujeme prvy zaznam
            hlava = node2;
            if(cislo2 - cislo1 > 1){ //ak na seba zaznamy nenadvazuju
                node2->dalsi = next1;
                prev2->dalsi = node1;
                node1->dalsi = next2;
            }
            else{ //ak na seba nadvazuju
                node2->dalsi = node1;
                node1->dalsi = next2;
            }
        }
        else{ //ak prehadzujeme iny ako prvy zaznam
            if(cislo2 - cislo1 > 1){ //ak na seba zaznamy nenadvazuju
                node2->dalsi = next1;
                prev2->dalsi = node1;
                node1->dalsi = next2;
            }
            else{ //ak na seba zaznamy nadvazuju
                node2->dalsi = node1;
                node1->dalsi = next2;
            }
            prev1->dalsi = node2;
            node1->dalsi = next2;
        }

    }
    *spajany_zoznam = hlava;
}
