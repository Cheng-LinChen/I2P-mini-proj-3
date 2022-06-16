#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <limits>

#define pos_inf 9223372036000000000;
#define neg_inf -9223372036000000000;
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
    int alpha;
    int beta;
    int is_player;
    int val;
    vector<node *> child;
} Node;

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

node *root;


int gen_val()
{
    //
}

node *gen_tree(int depth, int x, int y, int a, int b, int is_player)
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
        node* ret = &_new;
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
        board[x][y] = is_player;
        vector<point> choi;
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


long long int alpah_beta(int depth, node n, long long int a, long long int b, int is_player)
{
    board[n.x][n.y] = is_player;
    long long ret_val;
    if (depth == 0)
    {
        return gen_val();
    }
    long long int c_val;
    if (is_player == 1)
    {
        ret_val = neg_inf;
        for (auto it : n.child)
        {
            c_val = alpah_beta(depth - 1, *it, ) if (ret_val < alpah_beta())
        }
    }
    else
    {
        ret_val = pos_inf;
    }

    if (1)
    { // keep going
    }

    board[n.x][n.y] = 0;
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
    vector<point> possible_point = gen_possi();
    root = gen_tree();
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
