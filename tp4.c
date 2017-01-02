#include <stdio.h>
#include <stdlib.h>
#include "tp4.h"
#include<time.h>
#include <string.h>



Benevole *nouveauBen(char *nom, char *prenom, int CIN, char sexe, int annee)  //OK
{
    Benevole *ben=malloc(sizeof(Benevole));
    strcpy(ben->nom, nom);
    strcpy(ben->prenom, prenom);
    ben->CID=CIN;
    ben->sexe=sexe;
    ben->annee=annee;
    ben->suivant=NULL;
    return ben;
}


Tranche *nouvelleTranche(int borneSup)   //OK
{
    Tranche *tr=malloc(sizeof(Tranche));
    tr->BorneSup=borneSup;
    tr->filsD=NULL;
    tr->filsG=NULL;
    tr->pere=NULL;
    tr->liste=nouvelleListe();
    return tr;
}


ListBenevoles *nouvelleListe()   //OK
{
    ListBenevoles *list=malloc(sizeof(ListBenevoles));
    list->nb=0;
    list->premier=NULL;
    return list;
}



Tranche *ajoutTranche(Tranche *racine, int borneSup)  //OK, c'est plus propre !
{
    if (racine==NULL)
    {
        racine=nouvelleTranche(borneSup);
        return racine;
    }
    else
    {
        if (borneSup < racine->BorneSup)
        {
            racine->filsG = ajoutTranche(racine->filsG, borneSup);
        }
        else if (borneSup > racine->BorneSup)
        {
            racine->filsD = ajoutTranche(racine->filsD, borneSup);
        }
        return racine;
    }

}


int anneeActuelle()
{
    time_t secondes;
    struct tm instant;
    time(&secondes);
    instant = *localtime(&secondes);
    return instant.tm_year + 1900;
}


int attribuerBorne(int annee)  //OK
{
    int age = anneeActuelle()-annee;
    int b=0;
    while(b <age)
        b=b+4;
    return b;
}


ListBenevoles *insererlist(Tranche *tr, Benevole *bene)  //OK
{
    if (tr->liste->nb==0)
    {
        tr->liste->premier=bene;
        tr->liste->nb=1;
        printf("nb=0\n");
        return bene;                            //OK jusque ici
    }
    else if (tr->liste->premier->annee <= bene->annee)  //ajout en tete
    {
        bene->suivant=tr->liste->premier;
        tr->liste->nb++;
        tr->liste->premier=bene;
        printf("entete\n");
        return bene;
    }
    else
    {
        Benevole *tmp=tr->liste->premier;
        Benevole *tmp1=tr->liste->premier;
        while((tmp->annee < bene->annee)  && tmp!=NULL)   //On se deplace
        {
            tmp1=tmp;
            tmp=tmp->suivant;
        }
        if (tmp1->suivant==NULL) //ajout en queue
        {
            tmp1->suivant=bene;
            tr->liste->nb++;
            printf("queue\n");
            return tr->liste->premier;
        }
        else if(bene->CID!=tmp->CID || (strcmp(bene->nom, tmp->nom)!=0) || (strcmp(bene->prenom, tmp->prenom)!=0) || bene->sexe!=tmp->sexe || bene->annee!=tmp->annee)
        {
            bene->suivant=tmp;
            tmp1->suivant=bene;
            tr->liste->nb++;
            printf("autre\n");
            return tr->liste->premier;
        }
        else //on a pas ajouté mais on renvoie la liste telle quelle
            return tr->liste->premier;
    }
}


Benevole *insererBen(Tranche *racine, Benevole *benevole)  //ok
{
    int bs;
    Tranche *tr; //=malloc(sizeof(Tranche));
    //tr=NULL;
    bs=attribuerBorne(benevole->annee); //borne sup
    racine = ajoutTranche(racine, bs); //tranche ajouté correctement si racine a déjà une tranche.
    tr = chercherTranche(racine, bs);  // on est sur le bon noeud
    insererlist(tr, benevole);
}




Benevole *chercherBen(Tranche *racine,int CIN, int *annee)  //OK
{
    Tranche *tmp=racine;
    int b=attribuerBorne(annee);
    while (tmp->BorneSup != b && tmp!=NULL)
    {
        if (tmp->BorneSup > b)
            tmp=tmp->filsG;
        else tmp=tmp->filsD;
    }
    if (tmp->BorneSup==b) //on a bien trouvé le bon noeud
    {
        Benevole *tmp1=tmp->liste->premier;
        while(tmp1!=NULL && tmp1->annee < annee)
            tmp1=tmp1->suivant;
        if(tmp1->annee == annee && tmp1!=NULL)  //pas en queue de liste, et on est à la bonne année
        {
            while(tmp1->annee== annee && tmp1->CID!=CIN)  //on avance tant qu'on a la bonne année mais pas le bon CIN
            {
                tmp1=tmp1->suivant;
            }
            if (tmp1->CID==CIN && tmp1->annee== annee)  //Si on a trouvé le benevole
                return tmp1;
        }
        return NULL;  //si on a pas trouvé le benevole
    }

}



Tranche *chercherTranche(Tranche *racine, int Bornesup )  //OK
{
    if (racine == NULL)
    {
        return NULL;
    }
    else if (Bornesup == racine->BorneSup)
    {
        return racine;
    }
    else if (Bornesup < racine->BorneSup)
    {
        return chercherTranche(racine->filsG, Bornesup);
    }
    else
    {
        return chercherTranche(racine->filsD, Bornesup);
    }
        return NULL;
}



int supprimerBen(Tranche *racine, int CIN, int annee)  //OK, il manque juste la suppression de la tranche
{
    int b=attribuerBorne(annee);
    Tranche *tr=chercherTranche(racine,b);
    if(tr!=NULL)
    {
        Benevole *courant=tr->liste->premier;
        Benevole *prec = courant;
        if (tr->liste->nb ==0) // liste vide
            return 1;
        else
        {
            while (courant!=NULL && courant ->annee > annee)  // on a les plus jeunes au début de la liste, donc on avance dans la liste pour descendre en année
            {
                prec=courant;
                courant=courant->suivant;
            }
            while (courant != NULL && courant->annee == annee) //on est au premier dont l'annee de naissance est la bonne
            {
                if(courant->CID == CIN)
                {
                        if (prec->CID == courant->CID ) //En tete
                        {
                            if (tr->liste == 1)
                            {
                                tr->liste->premier = NULL;
                                tr->liste->nb--;
                                free(courant);
                                //supprimerTranche(tr, b);
                                return 0;
                            }
                            tr->liste->premier = courant->suivant;
                            tr->liste->nb--;
                            free(courant);
                            return 0;
                        }
                        else if (courant->suivant != NULL)
                        {
                            prec->suivant = courant->suivant;
                            tr->liste->nb--;
                            free(courant);
                            return 0;
                        }
                        else if (courant->suivant == NULL)
                        {
                            prec->suivant = NULL;
                            tr->liste->nb--;
                            free(courant);
                            return 0;
                        }


                }
            }
            if ((courant == NULL) || (courant->annee > annee))
                return 1;

        }
    }
    return 1;
}




int supprimerTranche(Tranche *racine, int borneSup)   // pas ok
{
    int c=1;
    int borne=borneSup;
    Tranche *tranche=racine;
    while(c==1){
    Tranche *tr=chercherTranche(racine, borne);
    if(tr!=NULL)
    {
        while(tr->liste->nb != 0)
        {
            Benevole *tmp=tr->liste->premier;   //on supprime en tete
            free(tmp);
            tr->liste->nb--;
        }
        if (tr->filsG==NULL)
        {
            Tranche *sauv=tr;
            tr->filsG->pere=tr;
            tr=tr->filsG;
            free(tr);
            c=0;   //On supprime sans probleme la feuille, on met c=0 pour sortir de la boucle et quitter le programme
            return 0;
        }
        else if(tr->filsD==NULL)
        {
            Tranche *sauv=tr->filsD;
            tr->filsD->pere=tr;
            tr=tr->filsD;
            free(tr);
            c=0;   //On supprime sans probleme la feuille, on met c=0 pour sortir de la boucle et quitter le programme
            return 0;
        }
        else
        {
            Tranche *courant=tr->filsD;
            while(courant->filsG!=NULL) courant=courant->filsG;
            //on remplace la valeur à supprimer par le plus petit élément du sous arbre droit;
            tr->BorneSup = courant->BorneSup;
            tr->liste= courant->liste;
            borne=tr->BorneSup;
            tranche=tr->filsD;
            //Puis on reste dans la boucle pour supprimer l'élementd ans le sous arbre droit qui est le plus petit successeur
        }

    }
    return 1;
}
}




ListBenevoles *BenDhonneur(Tranche *racine) //On part du principe qu'il n'y a pas de tranche d'age vide. OK
{
    if (racine==NULL) return NULL;
    ListBenevoles *honneur;
    ListBenevoles *honneur2 = (ListBenevoles*)malloc(sizeof(ListBenevoles));
    Tranche *tmp=racine;
    Tranche *tmp1= tmp;
    while(tmp->filsD !=NULL)
    {
        tmp1 = tmp;
        tmp = tmp->filsD;
    }  //A la fin de la boucle, tmp pointe sur la tranche d'age la plus vieille
    honneur = tmp->liste;
    Benevole *ben = honneur->premier;
    while (ben->suivant != NULL)
    {
        ben = ben->suivant;
    }
    printf("Le benevole le plus age a %d ans \n", (anneeActuelle() - ben->annee));
   // honneur2->premier = ben;
    //honneur2->nb=1;
    Benevole *ben1 = ben;
    ben = honneur->premier;
    honneur2 = honneur;

    while(ben->suivant !=NULL)
    {
        if (ben->annee != ben1->annee)
        {
            honneur2->premier = ben->suivant;
        }
        ben=ben->suivant;
    }

    return honneur2;
}


int actualiser(Tranche *racine)
{
    int nb=0;
    int b=21;
    int max=bornemax(racine);
    Tranche *tr;
    Benevole *tmp, *tmp1;
    while(b<=max )
    {
        if(tr=chercherTranche(racine, b)!=NULL)
        {
            tmp=tr->liste->premier;
            while(tmp!=NULL && tmp->annee <=tr->BorneSup)
            {
                tmp=tmp->suivant;
            }
            while (tmp->annee > tr->BorneSup)
            {
                tmp1=tmp;
                tmp=tmp->suivant;
                insererBen(racine, tmp1);   //On insere a la bonne borne sup
                free(tmp1); //on supprime de la liste actuelle
                nb++;
            }
        }
        b+=4;
    }
    return nb;
}





int totalBenTranche(Tranche *racine, int borneSup)  //OK
{
    int nb=0;
    Tranche *tr=chercherTranche(racine, borneSup);
    if (tr == NULL)
    {
        return 0;
    }
    Benevole *tmp=tr->liste->premier;
    while(tmp!=NULL)
    {
        nb++;
        tmp=tmp->suivant;
    }
    return nb;
}



int bornemax(Tranche *racine) //OK
{
    Tranche *tmp=racine;
    while(tmp->filsD != NULL)
            tmp=tmp->filsD;
    return tmp->BorneSup;
}



int totalBen(Tranche *racine)  //OK !
{
    int borneSup=20;
    int tot = 0;
    int bmax = bornemax(racine);
    do
    {
        tot += totalBenTranche(racine, borneSup);
        borneSup = borneSup + 4;
    } while (borneSup <= bmax);
    return tot;
}



float pourcentageTranche(Tranche *racine, int borneSup) //ok
{
    float res=0;
    int t;
    if(t=totalBen(racine)!=0)
        res=(((float)totalBenTranche(racine, borneSup)) / t);
    return res*100;
}

void afficherBen(Benevole *ben)  //OK
{
    while(ben!=NULL)
    {
        printf("%20s | %20s | %6c | %20d | %6d \n", ben->nom, ben->prenom, ben->sexe, ben->CID, ben->annee);
        ben=ben->suivant;
    }
}

void afficherTranche(Tranche *racine, int borneSup)  //OK
{
    Tranche *tr=chercherTranche(racine, borneSup);
    Benevole *tmp=tr->liste->premier;
    if(tmp==NULL) printf("liste vide \n");
    else
    {
        printf("       nom         |        prenom       | sexe |        CID          | annee \n");
        afficherBen(tmp);
    }
}



void afficherArbre(Tranche *racine) //OK mais plutôt faire affichahe en ordre croissant
{
    if (racine != NULL)
    {
        afficherArbre(racine->filsD);
        if (racine->filsD != NULL)
        {
            printf(",");
        }
        printf("%d ", racine->BorneSup);
        if (racine->filsG !=NULL)
        {
            printf(",");
        }
        afficherArbre(racine->filsG);
    }
}

void detruire_Arbre(Tranche* racine) //Il faut faire appel à supprimerTranche non ?
{
    if (racine != NULL)
    {
    detruire_Arbre(racine->filsG);
    detruire_Arbre(racine->filsD);
    free(racine);  // les éléments de l'arbre sont mis à des valeurs aléatoires
    }
}



