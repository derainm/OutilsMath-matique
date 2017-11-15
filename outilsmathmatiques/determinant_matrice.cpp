//
// Created by mderain on 15/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int rang;
int i=0,j=0,k=0;
int n,m,rang;
float value;
float** mat;
float** cpt;
float det;
/*-- calcul de déterminant 2x2 --*/
int det_dim_2(float** mat1)
{
    int det=0;
    det = mat1[0][0]*mat1[1][1]-mat1[0][1]*mat1[1][0];
    return det;
}
/*--allocation tableau déterminant--*/
void alloc_cpt(float*** cpt,int rang)
{
    int j = 0;
    (*cpt) = (float**)malloc(sizeof(float*)*(n+1));
    for ( j = 0; j <= rang; j++)
    {
        (*cpt)[j] = (float*)malloc(sizeof(float)*(n+1));
        (*cpt)[j][n+1]='\0';
    }
        (*cpt)[m+1]=NULL;
}
/*--allocation tableau de tableaux déterminants--*/
void alloc_tableau_de_determinant(float**** tab,int n)
{
    (*tab) = (float***)malloc(sizeof(float**)*(n+1));
    for (j = 0; j <= n; j++)
    {
        (*tab)[j] = (float**)malloc(sizeof(float*)*(n+1));
        (*tab)[j][n+1]=NULL;
    }
}


/*--Allocation de tableau de tableau de déterminants --*/
void alloc_de_tableau_de_tableau_de_determinant(float***** rec,int n)
{
    int j;
    (*rec) = (float****)malloc(sizeof(float***)*(n+1));
    for (j = 0; j <= n; j++)
    {
        (*rec)[j] = (float***)malloc(sizeof(float**)*(n+1));
        (*rec)[j][n+1]=NULL;
    }
        (*rec)[n+1]=NULL;
}
 /*--deallocation tableau --*/
void dealloc_cpt(float*** cpt,int n)
{
    int j = 0;
    for ( j = 0; j <= n; j++)
    {
          free((*cpt)[j]);
    }
        cpt=NULL;
}
/*--deallocation tableau déterminant--*/
void dealloc_tab(float**** tab,int n,int rang)
{
    int j = 0;
    for ( j = 0; j <= rang; j++)
    {
        dealloc_cpt(tab[j],n);
    }
        tab=NULL;
}
/*--deallocation tableau de tableau déterminant--*/
void dealloc_rec(float***** rec,int n,int rang,int taille)
{
    int j = 0;
    for ( j = 0; j <= taille; j++)
    {
        dealloc_tab(rec[j],n,rang);
    }
        rec=NULL;
}
/*-- transposé une matrise --*/
float** transpose_mat(float** cpt,int n)
{
int i=0, j = 0;
float ** tmp;
alloc_cpt(&tmp,n);
    for (j=0; j<= n; j++)
    {
        for(i=0;i<= n;i++)
        {
            tmp[i][j]=cpt[i][j];
        }
    }
    for (j=0; j<= n; j++)
    {
        for(i=0;i<= n;i++)
        {
            cpt[j][i]=tmp[i][j];
        }
    }
    return cpt;
}

/*-affiche tableau 2d --*/
void affiche(float** cpt,int taille)
{
    int i,j;

    for (j = 0; j <= taille; j++)
    {    for (i = 0; i <= taille; i++)
        {
        printf(" %1.0f ",cpt[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


/*-affiche tableau de tableau --*/
void affiche_tab(float*** tab,int rang)
{
int i,j,k;
    printf("\n");
    for(j=0;j<=rang;j++)
    {
        for(k=0;k<rang;k++)
        {
            for(i=0;i<rang;i++)
            {
                printf(" %1.0f ",tab[j][i][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
/*-affiche tableau de tableau de déterminant--*/
void affiche_delta(float**** rec,int rang)
{
int i;
    for(i=0;i<=rang;i++)
    {
       affiche_tab(rec[i],rang);
    }
}


/*--Copy filtré pour le calcul de déterminant--*/
float*** copie_filtre(float*** tab,float** mat,float** cpt,int m,int rang)
{
int j,i,k;
int nb=0;
alloc_cpt(&cpt,rang);
    for (j=0; j <= m; j++)
    {
        for (k = 0; k <= m; k++)
        {
            if(j!=k) //on block une colonne
            {

                for (i = 0; i <= rang; i++)
                {
                    cpt[nb][i] = mat[i][k];
                }
                if(nb>=rang)
                {
                    nb=0;
                }
                else
                {
                    nb=nb+1;
                }
                if(i==k)
                {
                    cpt = transpose_mat(cpt,rang); //danss l'analyse on copie les ligne dans les colonnes il faut remettre la matrice à l'endroit même si det(A)=det(tA)
                    tab[j] =cpt;
                    //affiche(cpt,rang);
                    alloc_cpt(&cpt,rang);  // nouveau tableau
                }
            }
        }
    }
    cpt = transpose_mat(cpt,rang);
    tab[rang+1] = cpt;
    return tab;

}

int main(/*int argc, char const *argv[]*/)
{
	printf("Veuillez entrer les dimensions de la matrice carrée \n");
	scanf("%d",&n);

	m=n; //matrice carré
	n=n-1;//tableaux commence par 0 en c
    m=m-1;
    rang=n-1;

	/*--alocation tableau--*/
	alloc_cpt(&mat,m);
	/*--saisie--*/

	printf("aij i ligne j colonne \n");
	/*for (j = 0; j <= m; j++)
	{
		for (i = 0; i <= n; i++)
		{
		    printf("a%d%d = ",i,j);
			scanf("%f",&value);
			mat[j][i]=value;
		}
	}*/
	for (j = 0; j <= m; j++)
	{
		for (i = 0; i <= n; i++)
		{
			mat[j][i]=(float) rand()/10000; // ne marche pas pour les valeur de dimension pair
		}
	}
/*--affichage de mat --*/
    affiche(mat,n);

    float*** tab;
    float*** t;
    float**** rec;
    float**** tmp;
    int taille;
    int init;
    int taille_alloc;
    int h=m;
    /*-- initialisation de tab--*/
    // printf("dim de tab: %d t:%d \n",rang+1,rang-1+1);
    taille=1; //1
    printf(" rang:%d \n",rang);
    alloc_de_tableau_de_tableau_de_determinant(&rec,taille-1);
    /*--premier copie filtré--*/
    alloc_tableau_de_determinant(&tab,rang+1);
    tab=copie_filtre(tab,mat,cpt,rang+1,rang);
    rec[0]=tab;
    tmp=rec;
    printf("taille:1\n");
    affiche_tab(tmp[0],rang+1);
/*-- on applique la copie filtré jusqu'à ce qu'on obtiens des tableaux de dimensions 2x2 --*/
taille_alloc=h;
do
{
    rang=rang-1;
    printf(" rang:%d \n",rang);
    alloc_tableau_de_determinant(&t,rang);
    init=taille;
    taille=(h+1)*(taille);
    printf("-->taille:%d ",taille);
    for(j=0;j<=taille;j++)
    {
        if(init!=1)
        {
            tab= tmp[j];
        }
        for(i=0;i<rang;i++)
        {
            t=copie_filtre(t,tab[i],cpt,rang+1,rang);
            rec[j]=t;
        }
    }
    //alloc_de_tableau_de_tableau_de_determinant(&tmp,init-1);
    tmp=rec;


 for(j=0;j<taille_alloc;j++)
    {
        affiche_tab(rec[j],rang+1);
    }
        taille_alloc=(h+1)*(taille_alloc);
    printf("taille alloc :%d \n",taille_alloc);
    alloc_de_tableau_de_tableau_de_determinant(&rec,taille_alloc+1);
    //dealloc_tab(&tab,rang+1,rang+2);
    alloc_tableau_de_determinant(&tab,rang+1);
    printf("\n");
    h--;

}while(rang>=2);
    //dealloc_rec(&rec,taille,rang,rang);
getch();
	return 0;
}
