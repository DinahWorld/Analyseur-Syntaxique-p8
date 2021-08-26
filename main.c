#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void listeVerbe(char verbe[],char nmcommun[],char adverbe[]);
void phraseCoupe(char phrase[],char *phrasecoupe[],int *taille);
void chercheVerbe(char *liste[], char *phrase[],int tailleListe, int taillePhrase,char *verbe[]);
void analyse(char *liste[],char *phrase[],int tailleListe, int taillePhrase);
void Sujet(char *phrase[],int taille,int taillePhrase);
void majnombre(char *chaine);
void minuscule(char *chaine);

int main()
{
  
  char phrase[100];             //On va acceuillir notre phrase
  char liste[5000];             //On va acceuillir notre liste de verbe
  char ncommun[5000];           //On va acceuillir notre liste de nom commun
  char adverbe[5000];           //On va acceuillir notre liste d'adverbe'


  char *resphrase[100]; 
  char *resliste[5000];
  char *resncommun[5000];
  char *verbe[100];
  char *resadverbe[5000];

 

  int taillePhrase;
  int tailleVerbe;   
  int tailleNcommun;
  int tailleAdverbe;      

  char *p;
  int i,n;
  
  // On lit le texte saisi au clavier
  printf("\t----Bienvenue dans l'analyseur syntaxique de Dinath et Gokhan----\n");

  printf("Entrez votre phrase : \n");
  fgets(phrase, 100, stdin);

  //On vérifie si il y a bien une phrase (ou un mot)
  while (strcmp(phrase,"\n") == 0){

    printf("Entrez une phrase !\n");
    fgets(phrase, 100, stdin);

  }
  
  
  //On enlève le retour charriot de fgets
  p = strchr(phrase, '\n');
  if(p!= NULL)
  {
    *p = '\0';
  }
  
  //on part chercher notre liste de verbe et notre liste de nom commun mettre chaque verbe dans un tableau
  listeVerbe(liste,ncommun,adverbe);
  phraseCoupe(liste,resliste,&tailleVerbe);
   


  //On attribue chaque mot de la phrase à tableau
  phraseCoupe(phrase,resphrase,&taillePhrase);

  //La majuscule d'une phrase est pour désigner un nom propre
  printf("Analyse en cours...\n");
  for (i = 1; i < taillePhrase ; i++){     //i = 1 car le premier mot d'une phrase est en majuscule !
    
    majnombre(resphrase[i]);
  
  }

  //Etant donné que notre dictionnaire contient les mots en minuscule
  //Pour facilité la recherche de mot,on met tout en minuscule
  for (i = 0 ; i < taillePhrase ; i++){     
    
    minuscule(resphrase[i]);
  
  }
  
  //Chercheverbe va trouver tout les mot qui correspond à un verbe
  chercheVerbe(resliste,resphrase,tailleVerbe,taillePhrase,verbe);
  
  //Meme principe que pour chercher un verbe sauf qu'ici on va chercher à partir de la liste de nom commun
  phraseCoupe(ncommun,resncommun,&tailleNcommun);
  printf("nom commun : ");
  analyse(resncommun,resphrase,tailleNcommun,taillePhrase);

  printf("\nadverbe(s) : ");
  phraseCoupe(adverbe,resadverbe,&tailleAdverbe);
  analyse(resadverbe,resphrase,tailleAdverbe,taillePhrase);
  
  return 0;
}

void listeVerbe(char verbe[],char nmcommun[],char adverbe[])
{
    
    FILE* fichier = NULL;

    //On récupere notre liste de verbe
    fichier = fopen("listeVerbe.txt","r+");
    fgets(verbe,5000,fichier);
    fclose(fichier);

    //On récupere notre liste de nom commun
    fichier = fopen("listeNc.txt","r+");
    fgets(nmcommun,5000,fichier);
    fclose(fichier);

    //On récupere notre liste de nom commun
    fichier = fopen("adverbe.txt","r+");
    fgets(adverbe,5000,fichier);
    fclose(fichier);
        
}

void phraseCoupe(char phrase[],char *phrasecoupe[],int *taille)
{
  
  char d[] = " ";
  int i = 0;
  
  char* p = strtok(phrase, d);
  //strtok va à chaque espace d'une phrase mettre le mot precedant l'espace dans une case du tableau phrasecoupe[]
  while (p != NULL)
  {
      phrasecoupe[i] = p;
      i++;
      p = strtok (NULL, " ");
  }
  
  *taille = i;              //La variable taille a maintenant le nombre de mots contenu dans le tableau

}


void chercheVerbe(char *liste[], char *phrase[],int tailleListe, int taillePhrase,char *verbe[]){

  int i,n;
  int nombre[5000];
  int o = 0;
  char *infinitif[5000];


  for(n = 0; n < taillePhrase; n++){

      infinitif[0] = liste[0];              //Notre variable infinitif contient le premier mot de la liste de verbe à savoir l'infinitif du premier groupe de verbe

    for(i = 0; i < tailleListe; i++){

      if (strcmp(liste[i],"next") == 0){     //Notre liste de verbe contient le mot next qui signifie qu'on quitte un groupe de verbe
      
        infinitif[0] = liste[i + 1];          //le mot apres next est contenu dans la variable infinitif
      
      }
    
      if (strcmp(liste[i],phrase[n]) == 0){   
      //compare un verbe à la position i de notre liste à un mot de notre phrase à la position n
          
          verbe[o] = phrase[n];         
          nombre[o] = n;
          
          printf("verbe : %s = %s(infintif)\n",verbe[o],infinitif[0]);
          o++;
          
          if (o == 2){                        //Si il existe un verbe qui est juste apres le premier verbe alors on est face à un temps compsé
          
            if (nombre[0] == nombre[0 + 1] - 1){
          
              printf("\tTemps compose\n");
          
            }
          
            o = 0;                      //On reintilise o = 0;
          
          }
          

      }
    }
  }

  int nbre = nombre[0];                       //On envoie le nombre de mot qu'il y a avant le verbe (On va dire que le sujet se trouve avant le sujet)
  Sujet(phrase,nbre,taillePhrase);            //On envoie toutes les variables nécessaire afin de trouver le sujet
}

void analyse(char *liste[],char *phrase[],int tailleListe, int taillePhrase){
  //Meme principe que la fonction chercheVerbe
  int i,n, o = 0;
  int nombre[100];
  char *type[100];


  for(n = 0; n < taillePhrase; n++){

      type[0] = liste[0];

    for(i = 0; i < tailleListe; i++){

      if (strcmp(liste[i],"next") == 0){     
        type[0] = liste[i + 1];
      }
    
      if (strcmp(liste[i],phrase[n]) == 0){
        printf("%s(%s), ",phrase[n],type[0]);
      }
    }
  }    
}


void Sujet(char *phrase[],int taille,int taillePhrase){

  int i;
  int n = 0;
  int sujet = 0;
  char* pronom[100] = {"je","j'","tu","il","elle","on","nous","vous","ils","elles"};

  for(n = 0; n < 10; n++){
    for(i = 0;i < taille;i++){      
      if(strcmp(phrase[i],pronom[n]) == 0){                               //On va comparer les mots avec les pronoms 
          printf("sujet : %s (pronom personnel)\n",phrase[i]);
          i = taille;                                         //On arrete le programme un fois que le sujet à été trouvé
          n = 10;                                             //On arrete le programme un fois que le sujet à été trouvé
          sujet = 1;                                          //La variable sujet à 1 donc les fonctions qui suivent ne s'executeront pas
      }
    }
  }


//s'execute seulement si il ne trouve pas le pronom
  if (sujet == 0){
    for (i = 0; i < taille; i++){           
      if (strcmp(phrase[i],",") == 0){                //On dit que le suje ne peut etre dans entre un virgule donc le programme va chercher un virgule               
        n = i + 1;                                    //la variable n va contenir la position de la phrase à partir de la virgule
        i = taille;
      }
      else{
        n = 0;                    
      }
    }

    printf("Sujet : ");
    for (i = n; i < taille; i++){
      printf("%s ",phrase[i]);                      //Il va simplement afficher le groupe de mot avant le verbe
    }
    printf("\n");
  }
  

  printf("Complement du verbe : ");
    for (i = taille + 1;i < taillePhrase; i++){       //Il va simplement afficher le groupe de mot apres  le verbe
      printf("%s ",phrase[i]);
    }
    printf("\n");

}


void majnombre(char *chaine)
{
  int i = 0;
  if (chaine[0]  >= 65 &&  chaine[0] <= 90){
    printf("Nom propre : %s\n",chaine);
  }
  if (chaine[0]  >= 48 &&  chaine[0] <= 57){
      printf("Nombre : %s\n",chaine);
  }

}
void minuscule(char *chaine)
{
    int i = 0;
    for (i = 0; chaine[i] != '\0'; i ++)
    {
        if (chaine[i]  >= 65 &&  chaine[i] <= 90)
         chaine[i] = chaine[i] + 32;
    }

}

