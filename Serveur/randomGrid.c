#include "includes.h"

/********************************************************/
/**
 * @brief  Fonction permettant d'initialiser la grille
 * @param  play : La grille de jeu
 * @return void : Ne retourne rien
 */
void grid_init(Play* play)
{
    for(int y = 0 ; y < NBELEM(play->grid) ; y++)
    {
        for(int x = 0 ; x < NBELEM(*play->grid) ; x++)
        {
            play->grid[y][x].status = WATER;
            play->grid[y][x].id = '~';
        }
    }
}
/********************************************************/
/**
 * @brief  Fonction permettant d'afficher la grille de jeu
 * @param  play : La grille de jeu
 * @return void : Ne retourne rien
 */
void print_play(Play* play)
{
    printf("Affichage de la grille...\n");
    for(int y = 0 ; y < NBELEM(play->grid) ; y++)
    {
        for(int x = 0 ; x < NBELEM(*play->grid) ; x++)
        {
            printf("%c", play->grid[y][x].id);
        }
        puts("");
    }
    printf("\n\n");
}
/********************************************************/
/**
 * @brief  Fonction permettant de copier la grille de jeu
 * @param  play : La grille de jeu
 * @param  play : La nouvelle grille de jeu
 * @return void : Ne retourne rien
 */
void copy_play_grid(Play* play, char* grid)
{
    int i = 0;

    for(int y = 0 ; y < NBELEM (play->grid) ; y++)
    {
        for(int x = 0 ; x < NBELEM (*play->grid) ; x++)
        {
            *(grid + i) = play->grid[y][x].id;
            i++;
        }

    }
}
/********************************************************/
/**
 * @brief  Fonction permettant de vérifier que le bateau est bien positionné entièrement dans la grille
 * @param  play : La grille de jeu
 * @param  boat : Le pointeur sur le bateau dont on veut vérifier le placement
 * @param  pos  : Le même bateau dont on veut vérifier le placement
 * @return ok   : Retourne si oui ou non le bateau est correctement positionné
 */
int check_grid(Play* play, Boat* boat, Boat pos)
{
    int ok = 1;
    int x  = 0;
    int y  = 0;

    for(int i = 0 ; ok && i < boat->length ; i++)
    {
        x = 0;
        y = 0;
        switch(pos.direction)
        {
            case NORTH:
                x = pos.x;
                y = pos.y - boat->length + 1 + i;
                ok = (y >= 0 && play->grid[y][x].status == WATER);
                break;
            case SOUTH:
                x = pos.x;
                y = pos.y + i;
                ok = (y < NBELEM(play->grid) && play->grid[y][x].status == WATER);
                break;
            case EAST:
                x = pos.x + i;
                y = pos.y;
                ok = (x < NBELEM(*play->grid) && play->grid[y][x].status == WATER);
                break;
            case WEST:
                x = pos.x - i;
                y = pos.y;
                ok = (x >= 0 && play->grid[y][x].status == WATER);
                break;
            default:
                return -1;
        }
    }
    return ok;
}
/********************************************************/
/**
 * @brief  Fonction permettant de placer un bateau sur la grille
 * @param  play : La grille de jeu
 * @param  boat : Le pointeur sur le bateau qu'on veut placer
 * @return void : Ne retourne rien
 */
 void put_boat(Play* play, Boat* boat)
{
    Element* current = NULL;

    for(int i = 0 ; i < boat->length ; i++)
    {
        switch(boat->direction)
        {
            case NORTH:
                current = &play->grid[boat->y - boat->length + 1 + i][boat->x];
                current->id = boat->id;
                current->status = BOAT;
                break;
            case SOUTH:
                current = &play->grid[boat->y + i][boat->x];
                current->id = boat->id;
                current->status = BOAT;
                break;
            case EAST:
                current = &play->grid[boat->y][boat->x + i];
                current->id = boat->id;
                current->status = BOAT;
                break;
            case WEST:
                current = &play->grid[boat->y][boat->x - boat->length + 1 + i];
                current->id = boat->id;
                current->status = BOAT;
                break;
            default:
                printf("Erreur dans la generation de la grille\n");
                break;
        }
    }
}
/********************************************************/
/**
 * @brief  Fonction permettant de marquer dans la grille un bateau dans la direction Nord/Sud (North/South)
 * @param  play      : La grille de jeu
 * @param  direction : Direction dans laquelle le bateau pointe
 * @param  x0        : Position x de départ du bateau
 * @param  xlen      : Longueur x du bateau
 * @param  y0        : Position y dans laquelle le bateau pointe
 * @param  ylen      : Longueur y du bateau
 * @return void      : Ne retourne rien
 */
void mark_ns(Play* play, Direction direction, int x0, int xlen, int y0, int ylen)
{
    for(int x = -1 ; x < xlen + 1 ; x++)
    {
        int xx = x0 + x;
        if(xx >= 0 && xx < (int)NBELEM(*play->grid))
        {
            for(int y = -1 ; y < ylen + 1 ; y++)
            {
                int yy = -1;
                switch(direction)
                {
                    case NORTH:
                        yy = y0 - ylen + 1 + y;
                        break;
                    case SOUTH:
                        yy = y0 + y;
                        break;
                    default:
                        return;
                }
                if(yy != -1)
                {
                    if(yy >= 0 && yy < (int)NBELEM(play->grid))
                    {
                        if(play->grid[yy][xx].status == WATER)
                        {
                            play->grid[yy][xx].status = SEEN;
                            play->grid[yy][xx].id = '~';
                        }
                    }
                }
            }
        }
    }
}
/********************************************************/
/**
 * @brief  Fonction permettant de marquer dans la grille un bateau dans la direction Est/Ouest (East/West)
 * @param  play      : La grille de jeu
 * @param  direction : Direction dans laquelle le bateau pointe
 * @param  x0        : Position x de départ du bateau
 * @param  xlen      : Longueur x du bateau
 * @param  y0        : Position y dans laquelle le bateau pointe
 * @param  ylen      : Longueur y du bateau
 * @return void      : Ne retourne rien
 */
void mark_ew(Play* play, Direction direction, int x0, int xlen, int y0, int ylen)
{
    for(int x = -1 ; x < xlen + 1 ; x++)
    {
        int xx = -1;
        switch(direction)
        {
            case WEST:
                xx = x0 - xlen + 1 + x;
                break;
            case EAST:
                xx = x0 + x;
                break;
            default:
                return;
        }
        if(xx != -1)
        {
            if(xx >= 0 && xx < (int)NBELEM(*play->grid))
            {
                for(int y = -1 ; y < ylen + 1 ; y++)
                {
                    int yy = y0 + y;
                    if(yy >= 0 && yy < (int)NBELEM(play->grid))
                    {
                        if(play->grid[yy][xx].status == WATER)
                        {
                            play->grid[yy][xx].status = SEEN;
                            play->grid[yy][xx].id = '~';
                        }
                    }
                }
            }
        }
    }
}
/********************************************************/
/**
 * @brief  Fonction permettant de marquer dans la grille un bateau
 * @param  play : La grille de jeu
 * @param  boat : Bateau à marquer
 * @return void : Ne retourne rien
 */
void mark_boat(Play* play, Boat* boat)
{
    if(boat->direction == NORTH || boat->direction == SOUTH)
    {
        mark_ns(play, boat->direction, boat->x, 1, boat->y, boat->length);
    }
    else if(boat->direction == EAST || boat->direction == WEST)
    {
        mark_ew(play, boat->direction, boat->x, boat->length, boat->y, 1);
    }
}
/********************************************************/
/**
 * @brief  Fonction permettant de placer aléatoirement un bateau dans la grille
 * @param  play : La grille de jeu
 * @param  boat : Bateau à placer
 * @return void : Ne retourne rien
 */
void placement(Play* play, Boat* boat)
{
    Boat pos = {0};
    int error = 0;

    do
    {
        error = 0;
        pos.y = random(NBELEM(play->grid));
        pos.x = random(NBELEM(*play->grid));

        error = play->grid[pos.y][pos.x].status != WATER;

        if(error == 0)
        {
            pos.direction = random(STATUS_NB);
            error = !check_grid(play, boat, pos);
        }

    } while(error == 1);

    boat->x = pos.x;
    boat->y = pos.y;
    boat->direction = pos.direction;

    put_boat(play, boat);
    mark_boat(play, boat);
}
/********************************************************/
/**
 * @brief  Fonction lançant le placement aléatoire des bateaux en fonction du nombre spécifié par l'admin
 *         Les bateaux sont ajouter à la liste des bateaux de la partie
 * @param  grid     : Pointeur sur la grille de jeu
 * @param  listBoat : Liste des bateaux de la partie
 * @param  listBoat : Nombre de bateaux de la partie que l'admin aura défini
 * @return 0        : Fin normale de la fonction
 */
int random_grid(char* grid, List* listBoat, int nbBoats)
{
    Play play;

    randomize();

    Boat tabBoats[] =
    {
        {0, 0, UNDEFINED, (rand() % 10) + 1, '1'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '2'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '3'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '4'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '5'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '6'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '7'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '8'},
        {0, 0, UNDEFINED, (rand() % 10) + 1, '9'},
    };


    grid_init(&play);

    for(int i = 0 ; i < nbBoats ; i++)
    {
        placement(&play, &tabBoats[i]);
        add_node(listBoat);
        init_data_boat(listBoat->tail, tabBoats[i]);
    }

    print_play(&play);
    copy_play_grid(&play, grid);
    //print_grid(grid[0], NBELEM(grid), NBELEM(grid[0]));

    return 0;
}
/********************************************************/
/**
 * @brief  A SUPPRIMER !!!!!!!!!!!!!!
 */
 /*int main(int argc, char* argv[])
{
    char grid[10][10];
    random_grid(grid[0]);

    printf("Affichage de la copie...\n");
    for(int y = 0 ; y < NBELEM(grid) ; y++)
    {
        for(int x = 0 ; x < NBELEM(*grid) ; x++)
        {
            printf("%c", grid[y][x]);
        }
        puts("");
    }
    printf("\n\n");
}*/
/********************************************************/
