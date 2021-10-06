#include <stdio.h>
#include <stdlib.h>
#include "4enralla.h"
#include "minimax.h"
int player_computer = 2;
int player_human = 1;
void initializeNode(Node *p)
{ //board
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            p->board[i][j] = 0;
        }
        p->available_cols[i] = 1;
    }
}
void calculateNumChilds(Node *b)
{

    int aux = 0;
    for (int i = 0; i < N; i++)
    {
        if (b->board[0][i] == 0)
        {
            b->available_cols[aux] = i;
            aux++;
        }
    }
    for (int i = aux + 1; i < N; i++) //fill the rest of the vector with (-1)
    {
        b->available_cols[i] = -1;
    }
    b->n_child = aux;
}
void copyBoard(Node **board1, Node *board2)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            (*board1)->board[i][j] = board2->board[i][j];
        }
    }
}
void showNode(Node *p, int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("\t");
    }
    printf("-  %.6g\n", p->value);
}
void showLevel(Node *father, int level)
{
    for (int i = 0; i < father->n_child; i++)
    {
        showNode(father->child[i], level);
    }
}
void walkTreeRec(Node *root, int level)
{
    showNode(root, level);

    for (int i = 0; i < root->n_child; i++)
    {
        walkTreeRec(root->child[i], level + 1);
    }
}
void walkTree(Node *root)
{
    showLevel(root, 1);
    for (int i = 0; i < root->n_child; i++)
    {
        showLevel(root->child[i], 2);
    }
}
void freeLevel(Node *father)
{
    free(father->child);
    free(father);
}
void freeTree(Node *root)
{
    for (int i = 0; i < root->n_child; i++)
    {
        freeTree(root->child[i]);
    }
    freeLevel(root);
}
Node *createNode(Node *father, int n_of_col, int level)
{
    Node *p = (Node *)malloc(sizeof(Node));
    copyBoard(&p, father);
    placeChip(p, n_of_col, player_computer);
    if (level < LEVEL)
    {
        calculateNumChilds(p);
        p->child = malloc(p->n_child * sizeof(Node *));
    }
    else
    {
        p->n_child = 0;
        p->child = NULL;
    }
    return p;
}
void createLevel(Node *father, int level)
{
    for (int i = 0; i < father->n_child; i++)
    {
        father->child[i] = createNode(father, father->available_cols[i], level);
    }
}
void createTree(Node *root, int level)
{

    createLevel(root, level);
    for (int i = 0; i < root->n_child; i++)
    {
        createTree(root->child[i], level + 1);
    }
}
void Score(Node *p)
{
    if (win(p, player_computer))
    {
        p->value = MAX;
    }
    else if (win(p, player_human))
    {
        p->value = MIN;
    }
    else
    {
        p->value = rand() % 100 - 40;
    }
}
void min(Node *p)
{
    double aux = MAX;
    double current_value;
    for (int i = 0; i < p->n_child; i++)
    {
        current_value = p->child[i]->value;
        if (current_value < aux)
        {
            aux = current_value;
        }
    }
    p->value = aux;
}
void max(Node *p)
{
    double aux = MIN;
    double current_value;
    for (int i = 0; i < p->n_child; i++)
    {
        current_value = p->child[i]->value;
        if (current_value > aux)
        {
            aux = current_value;
        }
    }
    p->value = aux;
}
void minMax(Node *p, int level)
{
    if (p->n_child != 0)
    {
        for (int i = 0; i < p->n_child; i++)
        {
            minMax(p->child[i], level + 1);
        }
        if (level % 2)
            max(p);
        else
            min(p);
    }
    else
    {
        Score(p);
    }
}
int chooseColumn(Node *p)
{
    for (int i = 0; i < p->n_child; i++)
    {
        if (p->value == p->child[i]->value)
        {
            return p->available_cols[i];
        }
    }
    return N + 1;
}
int pcMove(Node *old)
{
    Node *root = malloc(sizeof(Node));
    copyBoard(&root, old);
    calculateNumChilds(root);
    //initializeNode(root);
    root->child = malloc(root->n_child * sizeof(Node *));
    createTree(root, 1);
    minMax(root, 0);
    return chooseColumn(root);
    //freeTree(root);
    //walkTreeRec(root, 0);
}
/*int main()
{
    Node *root = malloc(sizeof(Node));
    root->n_child = N;
    initializeNode(root);
    root->value = 100;
    root->child = malloc(root->n_child * sizeof(Node *));
    createTree(root, 1);
    minMax(root, 0);
    int col = finalMove(root);
    //freeTree(root);
    walkTreeRec(root, 0);
    return 0;
}*/
