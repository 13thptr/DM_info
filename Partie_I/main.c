#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
void affiche_tous_facteurs(char* mot, int longueur){
    //Entrée: mot de longueur réelle "longueur" (i.e. avec le caractère final.)
    printf("Les facteurs du mot \"%.*s\" sont (avec la convention du sujet):\n",longueur,mot);
    for(int i=0;i<longueur;i++){
        for(int j=i+1;j<=longueur;j++){
            printf("\t u[%d,%d]: \t",i,j);
            //On affiche le facteur mot[i,j] avec mot[j] exclus (n'existe pas forcément en plus).
            for(int k=i;k<j;k++){
                printf("%c",mot[k]);
            }
            (j%3==0)?printf("\n"):1;
        }
    }
    printf("\n");
}
bool est_facteur(char* fac, int len_fac, char*mot, int len_mot){
    //NE PAS OUBLIER DE PRENDRE EN COMPTE '\0', '\n' et autres caractères invisibles pouvant apparaître en entrée!!
    //Version naïve, il y a bien sûr des algorithmes plus efficaces (KMP, Boyer-Moore...)
    if(len_fac>len_mot){
        return false;
    }
    for(int i=0;i<len_mot;i++){
        bool res_i=true;//Le facteur commence à l'indice i
        //Tant que l'on coïncide, aucun problème.
        for(int j=0;j<len_fac;j++){
            if(i+j>=len_mot||mot[i+j]!=fac[j]){
                    res_i=false;
                    break;
            }   
        }
        if(res_i)return res_i;
    }
    return false;
}

bool est_palindrome(char* mot, int len){
    for(int i=0;i<len;i++){
        if(mot[i]!=mot[len-i-1]){
            return false;
        }
    }
    return true;
}

bool facteur_palindrome(char* mot, int indice_debut, int indice_fin){
    assert(indice_debut<indice_fin);//convention du sujet, l'indice de fin est exclus
    for(int k=indice_debut;k<indice_fin;k++){
        if(mot[k]!=mot[indice_fin-k+indice_debut-1]){
            return false;
        }
    }
    return true;
}
int decompte_naif(char mot[], int longueur ){
    //longueur désigne le nombre de caractères "réel" du mot (sans '\0' ni '\n' par exemple)
    int nb=0;
    for(int i=0;i<longueur;i++){
        for(int j=i+1;j<=longueur;j++){    
            if(facteur_palindrome(mot,i,j)){
                nb++;
            }
        }
    }
    return nb;
}
int decompte_naif_inline(char mot[], int longueur){
    int nb=0;
    for(int i=0;i<longueur;i++){
        for(int j=i;j<longueur;j++){
            bool estPalindrome=true;
            for(int k=i;k<j;k++){
                if(mot[k]!=mot[j-k+i]){
                    estPalindrome= false;
                    break;
                }
            }
            if(estPalindrome)nb++;
        }
    }
    return nb;
}
int faux_decompte_naif_sujet(char mot[], int longueur){
    int nb=0;
    for(int i=0;i<longueur;i++){
        for(int j=i;j<longueur;j++){
            bool estPalindrome=true;
            for(int k=i;k<j;k++){
                if(mot[i]!=mot[j-k]){
                    estPalindrome= false;
                    break;
                }
            }
            if(estPalindrome)nb++;
        }
    }
    return nb;
}
int sol_progdyn(char mot[], int longueur){
    int nb=0;
    //initialisation
    bool** P= malloc((longueur+1)*sizeof(bool*));
    for(int i=0;i<longueur+1;i++){
        P[i]=malloc(sizeof(bool)*(longueur+1));
        for(int j=0;j<=longueur;j++){
            P[i][j]=true;//Neutre pour &&.
        }
    }
    for(int i=longueur-1;i>=0;i--){
        for(int j=1;j<longueur+1;j++){
            if(i<j){
                P[i][j]=P[i+1][j-1]&&(mot[i]==mot[j-1]);
                if(P[i][j])nb++;
            }
        }
    }
    //libération
    for(int i=0;i<longueur+1;i++){
        free(P[i]);
    }
    free(P);
    return nb;
}
int sol_Q10(char mot[], int longueur){
    /*
        Attention ! Le code tel quel ne compte que les palindromes impairs !! il faut insérer
        un caractère spécial, par exemple # pour dénombrer puis diviser le résultat par 2.
        On le fait dans une fonction séparée. -> en fait non...on doit gérer le caractère spécial de l'intérieur.
    */
    int nb=0;
    bool** P= malloc((longueur+1)*sizeof(bool*));
    for(int i=0;i<longueur+1;i++){
        P[i]=malloc(sizeof(bool)*(longueur+1));
        for(int j=0;j<longueur+1;j++){
            P[i][j]=true;//Neutre pour &&.
        }
    }
    for(int centre=0;centre<longueur;centre++){
        for(int rayon=1;rayon<=centre;rayon++){
             if(centre+rayon<=longueur){
                P[centre-rayon][centre+rayon]=P[centre-(rayon-1)][centre+rayon-1]&&(mot[centre-rayon]==mot[centre+rayon]);
                if(P[centre-rayon][centre+rayon])nb++;
            }
            
        }
    }
    for(int i=0;i<longueur+1;i++){
        free(P[i]);
    }
    free(P);
    return (nb+(longueur-1)/2)/2;//Cf. Q7, Q8 pour l'explication sur les doublons. 
}
char* insere_special(char* chaine, int longueur){
    char* nouvelle_chaine = malloc(sizeof(char)*(2*longueur+2));
    for(int i=0;i<longueur;i++){
        nouvelle_chaine[2*i]='#';
        nouvelle_chaine[2*i+1]=chaine[i];
    }
    nouvelle_chaine[2*longueur]='#';
    nouvelle_chaine[2*longueur+1]='\0';
    printf("%s\n",nouvelle_chaine);
    return nouvelle_chaine;
}

int main(){
    char *entree = NULL;
    size_t len=0;
    __ssize_t tailleLue;
    tailleLue=getline(&entree,&len,stdin);
    //Note that the newline character, not the null terminator, is counted in the length.
    int n=tailleLue-1;//Longueur sans le caractère nul
    //On doit prendre en compte le caractère nul pour les fonctions avec effets de bord !!!
    printf("Voir la page wikipédia pour le problème de la plus longue sous-chaîne palindromique.\n");
    printf("(Explication claire de l'algorithme de Manacher)\n");

    printf("Nombre de palindromes dans la chaîne (version du sujet, incorrecte):%d\n \n",faux_decompte_naif_sujet(entree,n));  

    printf("Nombre de palindromes dans la chaîne avec decompte_naif:%d\n",decompte_naif(entree,n));  
    printf("Nombre de palindromes dans la chaîne avec decompte_naif_inline:%d\n",decompte_naif_inline(entree,n));   
    
    printf("Nombre de palindromes dans la chaîne en prog dyn:%d\n",sol_progdyn(entree,n));  

    char* chaine_speciale = insere_special(entree,n);
    int nb=sol_Q10(chaine_speciale,2*n+1);
    printf("%s\n",chaine_speciale);
    printf("Nombre de palindromes dans la chaîne (Sol Q10):%d\n",nb);  


    free(chaine_speciale);
    affiche_tous_facteurs(entree,tailleLue-1);//On ne passe pas le caractère \n
    if(est_palindrome(entree,tailleLue-1)){
        printf("Le mot entré est un palindrome\n");
    }else{
        printf("Le mot entré n'est pas un palindrome.\n");
    }
    printf("Test d'appartenance facteur/mot:");
    printf("Entrez un premier mot:\n");
    char *mot = NULL;
    size_t len_mot=0;
    __ssize_t tm;
    tm=getline(&mot,&len_mot,stdin);
    printf("Entrez un second mot pour savoir s'il est facteur du premier:\n");

    char *fac = NULL;
    size_t len_fac=0;
    __ssize_t tf;
    tf=getline(&fac,&len_fac,stdin);

    if(est_facteur(fac,tf-1,mot,tm-1)){
        printf("Le second mot est facteur du premier. \n");
    }
    else{
        printf("Le second mot n'est pas facteur du premier.\n");
    }
    free(entree);
    free(mot);
    free(fac);
    return EXIT_SUCCESS;
}