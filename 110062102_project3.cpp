#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <limits>

#define pos_inf 9223372036000000000.00000000;
#define neg_inf -9223372036000000000.00000000;
#define dep 5;
using namespace std;

enum SPOT_STATE
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
typedef struct point
{
    int x;
    int y;
} point;

typedef struct node
{
public:
    point putting_loca;
    double alpha;
    double beta;
    int is_player;
    double val;
    vector<node *> child;
} Node;

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

node *root;

double pow(int x, double y)
{
    double ret = 1.0000000;
    for (int i = 0; i < x; i++)
    {
        ret *= y;
    }
    return ret;
}

bool avail(point p)
{
    if (p.x > 0 && p.y > 0 && p.x < 14 && p.y < 14)
    {
        return 1;
    }
    return 0;
}

double count_struc(int x, int y, int is_player)
{
    double ret = 0.0000000;
    int enemy = 3 - is_player;
    point inp;
    inp.x = x;
    inp.y = y;
    point e1;
    point e2;
    int consec_num;
    double path_fill_1;
    double path_fill_2;
    double multiplier;
    point b1;
    point b2;

    // dir 1
    e1 = inp;
    e2 = inp;
    consec_num = 0;
    while (avail(e2))
    {
        if (board[e2.x][e2.y] == is_player)
        {
            consec_num++;
            e2.x++;
        }
        else
        {
            break;
        }
    }
    b1 = e1;
    b2 = e2;
    path_fill_1 = 0;
    path_fill_2 = 0;
    b1.x--;
    while (avail(b1))
    {
        if (board[b1.x][b1.y] == 0)
        {
            b1.x--;
        }
        else if (board[b1.x][b1.y] == is_player)
        {
            b1.x--;
            path_fill_1++;
        }
    }
    b2.x++;
    while (avail(b2))
    {
        if (board[b2.x][b2.y] == 0)
        {
            b2.x++;
        }
        else if (board[b2.x][b2.y] == is_player)
        {
            b2.x++;
            path_fill_2++;
        }
    }
    if (b2.x - b1.x + 1 < 5)
    {
        return 0.0000000;
    }

    multiplier = 1 + (((e1.x - b1.x) - (path_fill_1)) / consec_num) * (((b2.x - e2.x) - (path_fill_2)) / consec_num);
    ret += pow(consec_num, 1000) * multiplier;
    // dir 1

    // dir 2
    e1 = inp;
    e2 = inp;
    consec_num = 0;
    while (avail(e2))
    {
        if (board[e2.x][e2.y] == is_player)
        {
            consec_num++;
            e2.y++;
        }
        else
        {
            break;
        }
    }
    b1 = e1;
    b2 = e2;
    path_fill_1 = 0;
    path_fill_2 = 0;
    b1.y--;
    while (avail(b1))
    {
        if (board[b1.x][b1.y] == 0)
        {
            b1.y--;
        }
        else if (board[b1.x][b1.y] == is_player)
        {
            b1.y--;
            path_fill_1++;
        }
    }
    b2.y++;
    while (avail(b2))
    {
        if (board[b2.x][b2.y] == 0)
        {
            b2.y++;
        }
        else if (board[b2.x][b2.y] == is_player)
        {
            b2.y++;
            path_fill_2++;
        }
    }
    if (b2.y - b1.y + 1 < 5)
    {
        return 0.0000000;
    }

    multiplier = 1 + (((e1.y - b1.y) - (path_fill_1)) / consec_num) * (((b2.y - e2.y) - (path_fill_2)) / consec_num);
    ret += pow(consec_num, 1000) * multiplier;
    // dir 2

    // dir 3
    e1 = inp;
    e2 = inp;
    consec_num = 0;
    while (avail(e2))
    {
        if (board[e2.x][e2.y] == is_player)
        {
            consec_num++;
            e2.x++;
            e2.y--;
        }
        else
        {
            break;
        }
    }
    b1 = e1;
    b2 = e2;
    path_fill_1 = 0;
    path_fill_2 = 0; 
    b1.x--;
    b1.y++;
    while(avail(b1)){
        if(board[b1.x][b1.y] == 0){
            b1.x--;
            b1.y++;
        }
        else if(board[b1.x][b1.y] == is_player){
            b1.x--;
            b1.y++;
            path_fill_1++;
        }
    }
    b2.x++;
    b2.y--;
    while(avail(b2)){
        if(board[b2.x][b2.y] == 0){
            b2.x++;
            b2.y--;
        }
        else if(board[b2.x][b2.y] == is_player){
            b2.x++;
            b2.y--;
            path_fill_2++;
        }
    }
    if(b2.x-b1.x+1 < 5){
        return 0.0000000;
    }  

    multiplier = 1+(((e1.x - b1.x) - (path_fill_1))/consec_num) * (((b2.x - e2.x) - (path_fill_2))/consec_num);
    ret += pow(consec_num,1000) * multiplier;
    //dir 3

    // dir 4
    e1 = inp;
    e2 = inp;
    consec_num = 0;
    while (avail(e2))
    {
        if (board[e2.x][e2.y] == is_player)
        {
            consec_num++;
            e2.x++;
            e2.y++;
        }
        else
        {
            break;
        }
    }
    b1 = e1;
    b2 = e2;
    path_fill_1 = 0;
    path_fill_2 = 0; 
    b1.x--;
    b1.y--;
    while(avail(b1)){
        if(board[b1.x][b1.y] == 0){
            b1.x--;
            b1.y--;
        }
        else if(board[b1.x][b1.y] == is_player){
            b1.x--;
            b1.y--;
            path_fill_1++;
        }
    }
    b2.x++;
    b2.y++;
    while(avail(b2)){
        if(board[b2.x][b2.y] == 0){
            b2.x++;
            b2.y++;
        }
        else if(board[b2.x][b2.y] == is_player){
            b2.x++;
            b2.y++;
            path_fill_2++;
        }
    }
    if(b2.x-b1.x+1 < 5){
        return 0.0000000;
    }  

    multiplier = 1+(((e1.x - b1.x) - (path_fill_1))/consec_num) * (((b2.x - e2.x) - (path_fill_2))/consec_num);
    ret += pow(consec_num,1000) * multiplier;
    //dir 4

    return ret;
}

double count_lengh(int x, int y, int is_player)
{
    double ret = 0.0000000;
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        px--;
        ret += 1;
        if (px < 0)
        {
            break;
        }
    }
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        px++;
        ret += 1;
        if (px > 14)
        {
            break;
        }
    }
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        py--;
        ret += 1;
        if (py < 0)
        {
            break;
        }
    }
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        py++;
        ret += 1;
        if (py > 14)
        {
            break;
        }
    }

    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        px--;
        py--;
        ret += 1;
        if (px < 0 || py < 0)
        {
            break;
        }
    }
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        px--;
        py++;
        ret += 1;
        if (px < 0 || py > 14)
        {
            break;
        }
    }
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        px++;
        py--;
        ret += 1;
        if (px > 14 || py < 0)
        {
            break;
        }
    }
    int px = x;
    int py = y;
    while (board[px][py] != 3 - is_player)
    {
        px++;
        py++;
        ret += 1;
        if (px > 14 || py > 14)
        {
            break;
        }
    }
    return ret;
}

double gen_val(int is_player)
{
    double ret = 0.00000000;
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            if (is_player == 1)
            {
                ret += count_lengh(i, j, is_player);
                ret += count_struc(i, j, is_player);
            }
            else
            {
                ret -= 5 * count_lengh(i, j, 1 + is_player);
                ret -= 5 * count_struc(i, j, 1 + is_player);
            }
        }
    }
    return ret;
}

node *gen_tree(int depth, int x, int y, double a, double b, int is_player)
{
    if (is_player && a >= b)
    {
        return nullptr;
    }
    if (!is_player && a <= b)
    {
        return nullptr;
    }
    node _new;
    _new.putting_loca.x = x;
    _new.putting_loca.y = y;
    _new.alpha = a;
    _new.beta = b;
    if (depth == 0)
    {
        if (is_player)
        {
            //_new.alpha = gen_val(is_player);
        }
        if (!is_player && a <= b)
        {
            //_new.beta = gen_val(is_player)
        }
        node *ret = &_new;
        return ret;
    }

    if (is_player)
    {
        _new.alpha = neg_inf;
        _new.beta = pos_inf;
    }
    else
    {
        _new.alpha = pos_inf;
        _new.beta = neg_inf;
    }

    if (depth == 0)
    {
        //
    }
    else
    {
        vector<point> choi;
        if (x == -1 && y == -1)
        {
            // evap
        }
        int po[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
        int px, py;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                int flag = 0;
                for (int k = 0; k < 8; k++)
                {
                    if (flag)
                    {
                        continue;
                    }
                    px = i + po[k][0];
                    py = j + po[k][1];
                    if (px >= 0 && px <= 14 && py >= 0 && py <= 14)
                    {
                        if (board[px][py] != 0)
                        {
                            choi.push_back({i, j});
                            flag = 1;
                        }
                    }
                }
            }
        }
        for (auto it : choi)
        {
            if (is_player)
            {
                node *child_ptr = gen_tree(depth - 1, it.x, it.y, _new.alpha, _new.beta, 0);
                if (_new.alpha < child_ptr->beta)
                {
                    _new.alpha = child_ptr->beta;
                    if (_new.alpha >= _new.beta)
                    {
                        break;
                    }
                }
                _new.child.push_back(child_ptr);
            }
            else
            {
                node *child_ptr = gen_tree(depth - 1, it.x, it.y, _new.alpha, _new.beta, 1);
                if (_new.beta < child_ptr->alpha)
                {
                    _new.beta = child_ptr->alpha;
                    if (_new.beta >= _new.alpha)
                    {
                        break;
                    }
                }
                _new.child.push_back(child_ptr);
            }
        }
    }
    node *ret = &_new;
    return ret;
}

void read_board(std::ifstream &fin)
{
    fin >> player;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream &fout)
{
    root = gen_tree(5, -1, -1, -9000000000, 900000000, 0);
    /*
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
    */
}

int main(int, char **argv)
{
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
