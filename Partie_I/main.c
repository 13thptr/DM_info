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
char* insere_special(char* chaine, int longueur){
    char* nouvelle_chaine = malloc(sizeof(char)*(2*longueur+2));
    for(int i=0;i<longueur;i++){
        nouvelle_chaine[2*i]='#';
        nouvelle_chaine[2*i+1]=chaine[i];
    }
    nouvelle_chaine[2*longueur]='#';
    nouvelle_chaine[2*longueur+1]='\0';
    return nouvelle_chaine;
}
int sol_Q10(char mot[], int longueur){
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
    return nb+longueur;
}
int base_sur_Q10(char mot[], int longueur){
    char* chaine_speciale = insere_special(mot,longueur);
    int nb = sol_Q10(chaine_speciale,2*longueur+1)-longueur-1;//On compte "longueur" ou "longueur+1" symboles spéciaux dans les palindromes impairs
    free(chaine_speciale);
    return nb/2;//Cf. Q7, Q8 pour l'explication sur les doublons. 
}
/*
Q11. Soient u ∈ Σ∗ , i ∈ [[0, |u| − 1]] et 0 < j ∈ [[1, ρ̂i ]]. En exploitant les différentes symétries, montrer
qu’il existe un palindrome centré en i+ j de rayon min(ρ(i)− j, ρ(i− j)). En déduire ρ̂i+ j ≥ min(ρ̂i − j, ρ̂i− j ).
Préciser à quelle condition il y a égalité.
*/

//La fonction suivante est dans le même esprit que facteur_palindrome.
bool palindrome_centre(char* mot, int centre, int rayon,int longueur){
    assert(0<=centre-rayon&&centre+rayon<=longueur);//convention du sujet, l'indice de fin est exclus
    for(int r=0;r<=rayon;r++){
        if(mot[centre+r]!=mot[centre-r]){
            return false;
        }
    }
    return true;
}
int rayon_max(char *mot, int longueur,int centre){
    //Renvoie le rayon maximal des palindromes centrés en "centre".
    assert(0<=centre&&centre<longueur);
    int r=0;
    while(0<=centre-r&&centre+r<longueur&&mot[centre+r]==mot[centre-r]){
        r++;
    }
    return r-1;
}
int min(int a, int b){
    return a<b?a:b;
}
bool verif_Q11(char* mot, int longueur,int i, int j){
    assert(0<=i&&i<=longueur-1);
    int r_i=rayon_max(mot,longueur,i);
    assert(1<=j&&j<=r_i);
    int r_imj=rayon_max(mot,longueur,i-j);
    return min(r_i-j,r_imj)==rayon_max(mot,longueur,i+j);
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
    printf("Nombre de palindromes IMPAIRS dans la chaîne (Q10):%d\n",sol_Q10(entree,n));  


    //char* chaine_speciale = insere_special(entree,n);
    int nb=base_sur_Q10(entree,n);
    //printf("%s\n",entree);
    printf("Nombre de palindromes (pairs&impairs) dans la chaîne (à l'aide de Q7, Q8, Q10):%d\n",nb);  
    affiche_tous_facteurs(entree,tailleLue-1);//On ne passe pas le caractère \n
    if(est_palindrome(entree,tailleLue-1)){
        printf("Le mot entré est un palindrome.\n");
    }else{
        printf("Le mot entré n'est pas un palindrome.\n");
    }
    printf("Test de la fonction palindrome_centre: \n");
    printf("Entrez une chaîne de caractères contenant un palindrome centré:\n");
    char *test=NULL;
    size_t len_test=0;
    __ssize_t tl=getline(&test,&len_test,stdin);
    printf("Entrez l'indice du centre:\n");
    int centre=0;
    scanf("%d",&centre);
    printf("Entrez le rayon:\n");
    int rayon=0;
    scanf("%d",&rayon);
    printf("Rayon maximal des palindromes centrés en \"%c\" :%d\n",test[centre],rayon_max(test,tl-1,centre));

    if(palindrome_centre(test,centre,rayon,tl-1)){
        printf("\"%.*s\" contient un palindrome centré en %c de rayon %d\n",(int)tl-1,test,test[centre],rayon);
    }
    int j=0;
    scanf("%d",&j);
    if(verif_Q11(test,tl-1,centre,j)){
        printf("Le résultat de Q11 est vérifié.\n");
    }
    else{
        printf("Le résultat de Q11 n'est pas vérifié.\n");
    }
    free(test);
    printf("Test d'appartenance facteur/mot:\n");
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