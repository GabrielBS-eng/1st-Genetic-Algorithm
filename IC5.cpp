  /*
  FEDERAL UNIVERSITY OF UBERLANDIA
  Faculty of Computer Science
  Computational Inteligence
  Author: Gabriel de Brito Silva
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POP 100
#define IND 11
#define GEN 200
#define TOUR 3
#define PROBCROSS 0.8
#define PROBMUT 0.1
#define EXEC 1000

//VECTOR: S E N D M O R Y

int recursiv1(int individ[][IND], int i, int j, int k);
int recursiv2(int individ[][IND], int i, int k);
void tour(int individ[][IND], int _chose[], int *index = NULL);
void mutationInd(int _mutant[]);
void mutationPop(int _individ[][IND], int totPop);
void general_avaliac(int somebody[]);
void printInd(int somebody[]);
void printPop(int individ[][IND], int totPop, int *good, int *best, int *worst);
void crossover(int individ[][IND], int sons[][IND]);
void roulette(int individ[][IND], int winner[], int *index = NULL);
void sort(int individ[][IND], int totPop);
void elitism(int individ[][IND]);
void bestBetweenFathersAndSons(int individ[][IND], int sons[][IND]);

//TODO: english comment
int recursiv1(int individ[][IND], int i,int j,int k)
{
  if(k==0)  return 1;
  if(individ[i][j] == individ[i][k-1])
  {
    return 0;
  }
  else
  {
    recursiv1(individ, i,j,k-1);
  }
}

//TODO: english comment
int recursiv2(int individ[][IND], int i, int k)
{
  if(k==-1) return 1;
  if(i==0) return 1;
  if(individ[i][k] == individ[i-1][k])
  {
    return 0;
  }
  else
  {
    recursiv2(individ, i, k-1);
  }
}

//TODO: english comment
void general_avaliac(int somebody[])
{
  int send = somebody[0]*1000 + somebody[1]*100 + somebody[2]*10 + somebody[3];
  int more = somebody[4]*1000 + somebody[5]*100 + somebody[6]*10 + somebody[1];
  int money = somebody[4]*10000 + somebody[5]*1000 + somebody[2]*100 + somebody[1]*10 + somebody[7];
  somebody[10] = abs(send + more - money);
}

//TODO: english comment
void printInd(int somebody[])
{
  printf("Print de um indivíduo: ");
  for (int i=0; i<IND; i++) printf("%d ", somebody[i]);
  printf("\n");
}

//TODO: english comment
void printPop(int individ[][IND], int totPop, int *good, int *best, int *worst)
{
  // printf("\nResult:\n");
  for(int i=0;i<totPop;i++)
  {
    for(int j=0;j<IND-3;j++)
    {
      // printf("%d ",individ[i][j]);
    }
    general_avaliac(individ[i]);
    if(individ[i][10]<= 1000) *good = *good + 1;
    if(individ[i][10]<= *best) *best = individ[i][10];
    if(individ[i][10]>= *worst) *worst = individ[i][10];
    // printf("Aptd: %d\n",individ[i][10]);
  }
}

//TODO: english comment
void tour(int individ[][IND], int _chose[], int *index)
{
  int round[TOUR][IND], best, x;

  for(int i=0; i<TOUR; i++)
  {
    x = rand()%POP;
    for(int j=0; j<IND; j++)  round[i][j] = individ[x][j];
    if(i==0) best = i;
    if(round[i][10] < round[best][10]) best = i;
  }
  for(int k=0; k<IND; k++) _chose[k] = round[best][k];
}

void crossover(int individ[][IND], int sons_generation[][IND])
{
  int k, aux, confirm;
  int size_sons = POP*PROBCROSS;
  int father1[IND];
  int father2[IND];
  for (int i=0; i<size_sons; i+=2)
  {
    roulette(individ, father1);
    roulette(individ, father2);
    // tour(individ, father1);
    // tour(individ, father2);
    // printInd(father1);
    // printInd(father2);

    k = rand()%(IND-1);
    do
    {
      confirm = 1;
      aux = father1[k];
      father1[k] = father2[k];
      father2[k] = aux;
      for(int j=0; j<IND-1; j++)
      {
        if(k!=j)
        {
          if(father1[k] == father1[j])
          {
            k = j;
            confirm = 0;
            break;
          }
        }
      }
    }while(!confirm);
    // printInd(father1);
    // printInd(father2);
    general_avaliac(father1);
    general_avaliac(father2);
    // printInd(father1);
    // printInd(father2);
    // getchar(); //             these prints and the getchar are used to check if fathers avaliation has been changed
    for (int p = 0; p<IND; p++)
    {
      sons_generation[i][p] = father1[p];
      sons_generation[i+1][p] = father2[p];
    }
    // printInd(sons_generation[i]);
    // printInd(sons_generation[i+1]);
  }
}

//TODO: english comment
void mutationInd(int _mutant[])
{
  int pos1 = rand()%(IND-3);
  int pos2 = rand()%(IND-3);
  int aux;
  aux = _mutant[pos1];
  _mutant[pos1] = _mutant[pos2];
  _mutant[pos2] = aux;
  general_avaliac(_mutant);
}

//TODO: english comment
void mutationPop(int _individ[][IND], int totPop)
{
  int aux, pos1, pos2, i, j;
  int qnt = POP*PROBMUT;
  for(i=0; i<totPop; i++)
  {
    if(j = rand()%totPop <= qnt)
    {
      int pos1 = rand()%(IND-3);
      int pos2 = rand()%(IND-3);
      int aux;
      aux = _individ[i][pos1];
      _individ[i][pos1] = _individ[i][pos2];
      _individ[i][pos2] = aux;
      general_avaliac(_individ[i]);
    }
  }
}

//TODO: english comment
void roulette(int individ[][IND], int winner[], int *index)
{
  int fit_reverse[POP];
  int acum_sum[POP];
  int sum = 0, r, chose, i;
  for(i=0; i<POP; i++)
  {
    fit_reverse[i] = 100000 - individ[i][10];
    acum_sum[i] = fit_reverse[i] + sum;
    sum = sum + fit_reverse[i];
    // printf("fit_reverse %d", fit_reverse[i]);
    // printf("\t sum %d\n", sum);
  }
  r = rand()%sum;
  for(i=0; i<POP; i++)
  {
    if(r < acum_sum[i])
    {
      if(i==0) chose = i;
      else chose = i-1;
      break;
    }
  }
  for(i=0; i<IND; i++)
  {
    winner[i] = individ[chose][i];
  }
  // printInd(winner);
}

//TODO: english comment
void sort(int individ[][IND], int totPop)
{
  int best;
  int num,aux[IND];
  for(int i = 0; i<totPop; i++)
  {
    best = 100000;
    for(int j = i; j<totPop; j++)
    {
      if(individ[j][10] <= best)
      {
        num = j;
        best = individ[j][10];
      }
    }
    for(int k = 0; k<IND; k++)
    {
      aux[k] = individ[i][k];
      individ[i][k] = individ[num][k];
      individ[num][k] = aux[k];
    }
    // printInd(individ[i]);
  }
}

// TODO: english comment
void elitism(int individ[][IND])
{
  int k, aux, confirm;
  int size_sons = POP*PROBCROSS;
  int father1[IND];
  int father2[IND];
  for (int i=0; i<size_sons; i+=2)
  {
    roulette(individ, father1);
    roulette(individ, father2);
    // tour(individ, father1);
    // tour(individ, father2);
    // printInd(father1);
    // printInd(father2);

    k = rand()%(IND-1);
    do
    {
      confirm = 1;
      aux = father1[k];
      father1[k] = father2[k];
      father2[k] = aux;
      for(int j=0; j<IND-1; j++)
      {
        if(k!=j)
        {
          if(father1[k] == father1[j])
          {
            k = j;
            confirm = 0;
            break;
          }
        }
      }
    }while(!confirm);
    // printInd(father1);
    // printInd(father2);
    general_avaliac(father1);
    general_avaliac(father2);
    // printInd(father1);
    // printInd(father2);
    // getchar(); //             these prints and the getchar are used to check if fathers avaliation has been changed

    // for (int p = 0; p<IND; p++)
    // {
    //   sons_generation[i][p] = father1[p];
    //   sons_generation[i+1][p] = father2[p];
    // }

    // printInd(sons_generation[i]);
    // printInd(sons_generation[i+1]);
  }
}

//TODO: english comment
void bestBetweenFathersAndSons(int individ[][IND], int sons[][IND])
{
  int size_sons = POP*PROBCROSS;
  int totPop = POP + POP*PROBCROSS;
  int fatherAndSons[totPop][IND];
  int best, worst, bestInd, worstInd;
  for (int i=0; i<POP; i++)
  {
    for (int j=0; j<IND; j++)
    {
      fatherAndSons[i][j] = individ[i][j];
    }
  }
  for (int i=0; i<size_sons; i++)
  {
    for (int j=0; j<IND; j++)
    {
      fatherAndSons[i+POP][j] = sons[i][j];
    }
  }
  sort(fatherAndSons,totPop);
  for (int i=0; i<POP; i++)
  {
    for (int j=0; j<IND; j++)
    {
      individ[i][j] = fatherAndSons[i][j];
    }
  }
}

int main()
{
  int individ[POP][IND];
  int pop_sons = POP*PROBCROSS;
  int sons[pop_sons][IND];
  int i,j,e,t=1;
  int v_good, v_best, v_worst;
  int *good = &v_good;
  int *best = &v_best;
  int *worst = &v_worst;
  int bestOfExecution[EXEC];
  int solutions = 0;

  unsigned int semente;
  FILE *fp;
  fp = fopen("/dev/urandom", "r");
  if (fp)
  {
    fread(&semente, sizeof(unsigned int), 1, fp);
    srand(semente);
    fclose(fp);
  }
  else
  {
    srand(t);
    t++;
  }

  for(e=0; e<EXEC; e++)
  {
    srand(e);
    v_good = 0;
    good = &v_good;
    v_best = 100000;
    best = &v_best;
    v_worst = 0;
    worst = &v_worst;
    for(i=0;i<POP;i++)
    {
      do
      {
        for(j=0;j<IND-1;j++)
        {
          do
          {
            individ[i][j] = rand()%10;
          }while(recursiv1(individ, i,j,j) != 1);
        }
      }while(recursiv2(individ, i,j-2) != 1);
    }
    for(int k=0; k<GEN; k++)
    {
      printPop(individ, POP, good, best, worst);
      // printf("\nBons indivíduos: %d\n\n",*good);
      // printf("Melhor: %d\n",*best);
      // printf("Pior: %d\n",*worst);
      // getchar();
      crossover(individ, sons);
      // printPop(sons, pop_sons);
      mutationPop(sons, pop_sons);
      // printPop(sons, pop_sons);
      // getchar();
      bestBetweenFathersAndSons(individ, sons);
      // printInd(individ[3]);
      // printInd(individ[3]);
      // printf("\nGeneration: %d", k+1);
      //getchar();
    }
    if (*best == 0)
    {
     solutions++;
     printf("EXEC %d\t\t", e);
     printInd(individ[0]);
    //  for (i=0; i<POP; i++)
    //  {
    //    if (individ[i][10] == 0)
    //    {
    //      printf("EXEC %d\t\t", e);
    //      printInd(individ[i]);
    //    }
    //  }
    //  printf("\n");
    }
  }
  printf("\nsolutions: %d\n",solutions);
  return 0;
}
