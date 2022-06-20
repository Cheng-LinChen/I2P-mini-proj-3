#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <queue>
#include <limits>

#define pos_inf 9223372036000000000.00000000
#define neg_inf -9223372036000000000.00000000

using namespace std;

long long int node_num = 0;
int ones = 0;
int twos = 0;
int first_hand;
char my_symbol;
char enemy_symbol;

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
    char symbol;
    double val;
    vector<node *> child;
} Node;

queue<point> pri;

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
std::array<std::array<char, SIZE>, SIZE> oxboard;

double max_beta = neg_inf;
int dep;

node *root;

int expon_2[3][3][3][3];
int facto_2[3][3][3][3];

int expon[5][4][4][3][3];
int facto[5][4][4][3][3];

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
    if (p.x >= 0 && p.y >= 0 && p.x <= 14 && p.y <= 14)
    {
        return 1;
    }
    return 0;
}

int dist(point p1, point p2)
{
    if (p1.x == p2.x)
    {
        return p2.y - p1.y;
    }
    else
    {
        return p2.x - p1.x;
    }
}

double count_struc(int x, int y, char str_sym)
{
    double ret = 0.0000000;
    point inp;
    inp.x = x;
    inp.y = y;

    int dir_op[4][2] = {{1, 0}, {0, 1}, {1, -1}, {1, 1}};

    for (int i = 0; i < 4; i++)
    {
        point e1 = inp;
        point e2 = inp;
        e2.x -= dir_op[i][0];
        e2.y -= dir_op[i][1];
        if (avail(e2))
        {
            if (oxboard[e2.x][e2.y] == str_sym)
            {
                continue;
            }
        }
        e2 = e1;
        int consec_num = 0;
        while (avail(e2))
        {
            if (oxboard[e2.x][e2.y] == str_sym)
            {
                consec_num++;
                e2.x += dir_op[i][0];
                e2.y += dir_op[i][1];
            }
            else
            {
                break;
            }
        }
        e2.x -= dir_op[i][0];
        e2.y -= dir_op[i][1];
        if (consec_num == 1)
        {
            continue;
        }

        double sch_num = 5 - consec_num;

        double cou = sch_num;
        point b1 = e1;
        point b2 = e2;
        double path_fill_1 = 0.0000000;
        double path_fill_2 = 0.0000000;

        b1.x -= dir_op[i][0];
        b1.y -= dir_op[i][1];
        while (avail(b1) && cou)
        {
            if (oxboard[b1.x][b1.y] == '-')
            {
                b1.x -= dir_op[i][0];
                b1.y -= dir_op[i][1];
            }
            else if (oxboard[b1.x][b1.y] == str_sym)
            {
                b1.x -= dir_op[i][0];
                b1.y -= dir_op[i][1];
                path_fill_1++;
            }
            else
            {
                break;
            }
            cou--;
        }
        b1.x += dir_op[i][0];
        b1.y += dir_op[i][1];

        b2.x += dir_op[i][0];
        b2.y += dir_op[i][1];
        cou = sch_num;
        while (avail(b2) && cou)
        {
            if (oxboard[b2.x][b2.y] == '-')
            {
                b2.x += dir_op[i][0];
                b2.y += dir_op[i][1];
            }
            else if (oxboard[b2.x][b2.y] == str_sym)
            {
                b2.x += dir_op[i][0];
                b2.y += dir_op[i][1];
                path_fill_2++;
            }
            else
            {
                break;
            }
            cou--;
        }
        b2.x -= dir_op[i][0];
        b2.y -= dir_op[i][1];

        if (dist(b1, b2) + 1 < 5)
        {
            continue;
        }

        int dist1 = dist(b1, e1);
        int dist2 = dist(e2, b2);
        int ex_num, fac_num;
        if (consec_num >= 5)
        {
            ex_num = 4;
            fac_num = 30;
        }
        else if (consec_num == 2)
        {
            int a, b, c, d;
            if (dist1 == 0 || dist1 == 1)
            {
                a = 2;
                b = 2;
            }
            else if (dist1 == 2)
            {
                if (oxboard[b1.x][b1.y] == str_sym)
                {
                    a = 2;
                    b = 1;
                }
                else
                {
                    a = 2;
                    b = 0;
                }
            }
            else if (dist1 == 3)
            {
                b1.x += dir_op[i][0];
                b1.y += dir_op[i][1];
                if (oxboard[b1.x][b1.y] == str_sym)
                {
                    a = 1;
                }
                else
                {
                    a = 0;
                }
                b1.x += dir_op[i][0];
                b1.y += dir_op[i][1];
                if (oxboard[b1.x][b1.y] == str_sym)
                {
                    b = 1;
                }
                else
                {
                    b = 0;
                }
                b1.x -= dir_op[i][0];
                b1.y -= dir_op[i][1];
                b1.x -= dir_op[i][0];
                b1.y -= dir_op[i][1];
            }

            if (dist2 == 0 || dist2 == 1)
            {
                c = 2;
                d = 2;
            }
            else if (dist2 == 2)
            {
                d = 0;
                if (oxboard[b2.x][b2.y] == str_sym)
                {
                    c = 2;
                    d = 1;
                }
                else
                {
                    c = 2;
                    d = 0;
                }
            }
            else if (dist2 == 3)
            {
                b2.x -= dir_op[i][0];
                b2.y -= dir_op[i][1];
                if (oxboard[b2.x][b2.y] == str_sym)
                {
                    c = 1;
                }
                else
                {
                    c = 0;
                }
                b2.x -= dir_op[i][0];
                b2.y -= dir_op[i][1];
                if (oxboard[b2.x][b2.y] == str_sym)
                {
                    d = 1;
                }
                else
                {
                    d = 0;
                }
                b2.x += dir_op[i][0];
                b2.y += dir_op[i][1];
                b2.x += dir_op[i][0];
                b2.y += dir_op[i][1];
            }
            if (dist1 == 0)
            {
                if (c == 0 && d == 0)
                {
                    ex_num = 1;
                    fac_num = 3;
                }
                else if (c == 0 && d == 1)
                {
                    ex_num = 2;
                    fac_num = 4;
                }
                else if (c == 1 && d == 0)
                {
                    ex_num = 2;
                    fac_num = 4;
                }
                else if (c == 1 && d == 1)
                {
                    ex_num = 3;
                    fac_num = 5;
                }
            }
            else if (dist2 == 0)
            {
                if (a == 0 && b == 0)
                {
                    ex_num = 1;
                    fac_num = 3;
                }
                else if (a == 0 && b == 1)
                {
                    ex_num = 2;
                    fac_num = 4;
                }
                else if (a == 1 && b == 0)
                {
                    ex_num = 2;
                    fac_num = 4;
                }
                else if (a == 1 && b == 1)
                {
                    ex_num = 3;
                    fac_num = 5;
                }
            }
            else
            {
                ex_num = expon_2[a][b][c][d];
                fac_num = facto_2[a][b][c][d];
                fac_num *= 1.5000000;
            }
        }
        else
        {
            ex_num = expon[consec_num][dist1][dist2][(int)path_fill_1][(int)path_fill_2];
            fac_num = facto[consec_num][dist1][dist2][(int)path_fill_1][(int)path_fill_2];
        }

        double multiplier = 1.0000000 + ((double)fac_num) / 100.000;
        ret += (double)pow(ex_num, 1000.00000000) * multiplier;
    }
    return ret;
}

double gen_val()
{
    double ret = 0.00000000;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (oxboard[i][j] == my_symbol)
            {
                ret += count_struc(i, j, my_symbol);
            }
            else if (oxboard[i][j] == enemy_symbol)
            {
                ret -= 5.0000000 * count_struc(i, j, enemy_symbol);
            }
        }
    }
    return ret;
}

node *gen_tree(int depth, int x, int y, double a, double b, char node_sym)
{
    node_num++;
    node _new;
    _new.putting_loca.x = x;
    _new.putting_loca.y = y;
    _new.alpha = a;
    _new.beta = b;
    if (depth == 0)
    {
        if (node_sym == my_symbol)
        {
            _new.alpha = gen_val();
        }
        if (node_sym == enemy_symbol)
        {
            _new.beta = gen_val();
        }
        if (dep == 1)
        {
            if (_new.beta > max_beta)
            {
                pri.push(_new.putting_loca);
                max_beta = _new.beta;
            }
        }
        node *ret = &_new;
        return ret;
    }
    if (node_sym == my_symbol)
    {
        _new.alpha = neg_inf;
        _new.beta = b;
    }
    else if (node_sym == enemy_symbol)
    {
        _new.alpha = a;
        _new.beta = pos_inf;
    }
    else if (node_sym == '+')
    {
        _new.alpha = neg_inf;
        _new.beta = pos_inf;
    }
    vector<point> choi;
    int po[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int px, py;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (oxboard[i][j] != '-')
            {
                continue;
            }
            for (int k = 0; k < 8; k++)
            {
                px = i + po[k][0];
                py = j + po[k][1];
                if (px >= 0 && px <= 14 && py >= 0 && py <= 14)
                {
                    if (oxboard[px][py] != '-')
                    {
                        choi.push_back({i, j});
                        break;
                    }
                }
            }
        }
    }

    if (choi.empty())
    {
        choi.push_back({7, 7});
    }

    for (auto it : choi)
    {
        if (!(node_sym == enemy_symbol))
        {
            oxboard[it.x][it.y] = my_symbol;
            node *child_ptr = gen_tree(depth - 1, it.x, it.y, _new.alpha, _new.beta, enemy_symbol);
            oxboard[it.x][it.y] = '-';
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
            oxboard[it.x][it.y] = enemy_symbol;
            node *child_ptr = gen_tree(depth - 1, it.x, it.y, _new.alpha, _new.beta, my_symbol);
            oxboard[it.x][it.y] = '-';
            if (_new.beta > child_ptr->alpha)
            {
                _new.beta = child_ptr->alpha;
                if (_new.beta <= _new.alpha)
                {
                    break;
                }
            }
            _new.child.push_back(child_ptr);
        }
    }
    if (depth == dep - 1)
    {
        if (_new.beta > max_beta)
        {
            pri.push(_new.putting_loca);
            max_beta = _new.beta;
        }
    }
    node *ret = &_new;
    return ret;
}
void set_expfac()
{
    expon_2[0][0][0][0] = 1;
    expon_2[0][0][0][1] = 2;
    expon_2[0][0][0][2] = 1;
    expon_2[0][0][1][0] = 3;
    expon_2[0][0][1][1] = 3;
    expon_2[0][0][1][2] = 2;
    expon_2[0][0][2][0] = 1;
    expon_2[0][0][2][1] = 1;
    expon_2[0][0][2][2] = 1;
    expon_2[0][1][0][0] = 3;
    expon_2[0][1][0][1] = 3;
    expon_2[0][1][0][2] = 3;
    expon_2[0][1][1][0] = 3;
    expon_2[0][1][1][1] = 3;
    expon_2[0][1][1][2] = 3;
    expon_2[0][1][2][0] = 3;
    expon_2[0][1][2][1] = 3;
    expon_2[0][1][2][2] = 3;
    expon_2[0][2][0][0] = 1;
    expon_2[0][2][0][1] = 1;
    expon_2[0][2][0][2] = 1;
    expon_2[0][2][1][0] = 3;
    expon_2[0][2][1][1] = 3;
    expon_2[0][2][1][2] = 2;
    expon_2[0][2][2][0] = -1;
    expon_2[0][2][2][1] = -1;
    expon_2[0][2][2][2] = -1;
    expon_2[1][0][0][0] = 2;
    expon_2[1][0][0][1] = 2;
    expon_2[1][0][0][2] = 2;
    expon_2[1][0][1][0] = 3;
    expon_2[1][0][1][1] = 3;
    expon_2[1][0][1][2] = 2;
    expon_2[1][0][2][0] = 1;
    expon_2[1][0][2][1] = 1;
    expon_2[1][0][2][2] = 1;
    expon_2[1][1][0][0] = 3;
    expon_2[1][1][0][1] = 3;
    expon_2[1][1][0][2] = 3;
    expon_2[1][1][1][0] = 3;
    expon_2[1][1][1][1] = 3;
    expon_2[1][1][1][2] = 3;
    expon_2[1][1][2][0] = 3;
    expon_2[1][1][2][1] = 3;
    expon_2[1][1][2][2] = 3;
    expon_2[1][2][0][0] = 1;
    expon_2[1][2][0][1] = 1;
    expon_2[1][2][0][2] = 1;
    expon_2[1][2][1][0] = 3;
    expon_2[1][2][1][1] = 3;
    expon_2[1][2][1][2] = 2;
    expon_2[1][2][2][0] = -1;
    expon_2[1][2][2][1] = -1;
    expon_2[1][2][2][2] = -1;
    expon_2[2][0][0][0] = 1;
    expon_2[2][0][0][1] = 1;
    expon_2[2][0][0][2] = 1;
    expon_2[2][0][1][0] = 3;
    expon_2[2][0][1][1] = 3;
    expon_2[2][0][1][2] = 2;
    expon_2[2][0][2][0] = 1;
    expon_2[2][0][2][1] = 1;
    expon_2[2][0][2][2] = 1;
    expon_2[2][1][0][0] = 2;
    expon_2[2][1][0][1] = 2;
    expon_2[2][1][0][2] = 2;
    expon_2[2][1][1][0] = 3;
    expon_2[2][1][1][1] = 3;
    expon_2[2][1][1][2] = 2;
    expon_2[2][1][2][0] = 2;
    expon_2[2][1][2][1] = 2;
    expon_2[2][1][2][2] = 2;
    expon_2[2][2][0][0] = 1;
    expon_2[2][2][0][1] = 2;
    expon_2[2][2][0][2] = 1;
    expon_2[2][2][1][0] = 3;
    expon_2[2][2][1][1] = 3;
    expon_2[2][2][1][2] = 2;
    expon_2[2][2][2][0] = -1;
    expon_2[2][2][2][1] = -1;
    expon_2[2][2][2][2] = -1;

    expon[3][0][2][0][0] = 2;
    expon[3][0][2][0][1] = 3;
    expon[3][2][0][0][0] = 2;
    expon[3][2][0][1][0] = 3;

    expon[3][1][1][0][0] = 2;

    expon[3][1][2][0][0] = 3;
    expon[3][1][2][0][1] = 3;
    expon[3][2][1][0][0] = 3;
    expon[3][2][1][1][0] = 3;

    expon[3][2][2][0][0] = 3;
    expon[3][2][2][0][1] = 3;
    expon[3][2][2][1][0] = 3;
    expon[3][2][2][1][1] = 4;

    expon[4][0][1][0][0] = 3;
    expon[4][1][0][0][0] = 3;
    expon[4][1][1][0][0] = 4;

    ////////////////////////
    facto_2[0][0][0][0] = 5;
    facto_2[0][0][0][1] = 5;
    facto_2[0][0][0][2] = 4;
    facto_2[0][0][1][0] = 5;
    facto_2[0][0][1][1] = 6;
    facto_2[0][0][1][2] = 4;
    facto_2[0][0][2][0] = 3;
    facto_2[0][0][2][1] = 3;
    facto_2[0][0][2][2] = 3;
    facto_2[0][1][0][0] = 5;
    facto_2[0][1][0][1] = 6;
    facto_2[0][1][0][2] = 4;
    facto_2[0][1][1][0] = 7;
    facto_2[0][1][1][1] = 8;
    facto_2[0][1][1][2] = 6;
    facto_2[0][1][2][0] = 3;
    facto_2[0][1][2][1] = 3;
    facto_2[0][1][2][2] = 3;
    facto_2[0][2][0][0] = 3;
    facto_2[0][2][0][1] = 4;
    facto_2[0][2][0][2] = 2;
    facto_2[0][2][1][0] = 3;
    facto_2[0][2][1][1] = 4;
    facto_2[0][2][1][2] = 2;
    facto_2[0][2][2][0] = -1;
    facto_2[0][2][2][1] = -1;
    facto_2[0][2][2][2] = -1;
    facto_2[1][0][0][0] = 5;
    facto_2[1][0][0][1] = 6;
    facto_2[1][0][0][2] = 5;
    facto_2[1][0][1][0] = 6;
    facto_2[1][0][1][1] = 7;
    facto_2[1][0][1][2] = 5;
    facto_2[1][0][2][0] = 4;
    facto_2[1][0][2][1] = 4;
    facto_2[1][0][2][2] = 4;
    facto_2[1][1][0][0] = 6;
    facto_2[1][1][0][1] = 7;
    facto_2[1][1][0][2] = 5;
    facto_2[1][1][1][0] = 8;
    facto_2[1][1][1][1] = 9;
    facto_2[1][1][1][2] = 7;
    facto_2[1][1][2][0] = 4;
    facto_2[1][1][2][1] = 4;
    facto_2[1][1][2][2] = 4;
    facto_2[1][2][0][0] = 3;
    facto_2[1][2][0][1] = 4;
    facto_2[1][2][0][2] = 2;
    facto_2[1][2][1][0] = 4;
    facto_2[1][2][1][1] = 4;
    facto_2[1][2][1][2] = 3;
    facto_2[1][2][2][0] = -1;
    facto_2[1][2][2][1] = -1;
    facto_2[1][2][2][2] = -1;
    facto_2[2][0][0][0] = 4;
    facto_2[2][0][0][1] = 5;
    facto_2[2][0][0][2] = 3;
    facto_2[2][0][1][0] = 4;
    facto_2[2][0][1][1] = 5;
    facto_2[2][0][1][2] = 5;
    facto_2[2][0][2][0] = 2;
    facto_2[2][0][2][1] = 2;
    facto_2[2][0][2][2] = 2;
    facto_2[2][1][0][0] = 5;
    facto_2[2][1][0][1] = 6;
    facto_2[2][1][0][2] = 4;
    facto_2[2][1][1][0] = 6;
    facto_2[2][1][1][1] = 7;
    facto_2[2][1][1][2] = 6;
    facto_2[2][1][2][0] = 4;
    facto_2[2][1][2][1] = 4;
    facto_2[2][1][2][2] = 4;
    facto_2[2][2][0][0] = 3;
    facto_2[2][2][0][1] = 4;
    facto_2[2][2][0][2] = 3;
    facto_2[2][2][1][0] = 5;
    facto_2[2][2][1][1] = 6;
    facto_2[2][2][1][2] = 4;
    facto_2[2][2][2][0] = -1;
    facto_2[2][2][2][1] = -1;
    facto_2[2][2][2][2] = -1;

    facto[3][0][2][0][0] = 5;
    facto[3][0][2][0][1] = 6;
    facto[3][2][0][0][0] = 6;
    facto[3][2][0][1][0] = 6;

    facto[3][1][1][0][0] = 4;

    facto[3][1][2][0][0] = 5;
    facto[3][1][2][0][1] = 6;
    facto[3][2][1][0][0] = 5;
    facto[3][2][1][1][0] = 6;

    facto[3][2][2][0][0] = 7;
    facto[3][2][2][0][1] = 8;
    facto[3][2][2][1][0] = 8;
    facto[3][2][2][1][1] = 9;

    facto[4][0][1][0][0] = 1;
    facto[4][1][0][0][0] = 1;
    facto[4][1][1][0][0] = 5;
}

void read_board(std::ifstream &fin)
{
    fin >> player;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fin >> board[i][j];
            if (board[i][j] == 1)
            {
                ones++;
            }
            else if (board[i][j] == 2)
            {
                twos++;
            }
        }
    }
    if (twos == ones)
    {
        first_hand = 1;
        my_symbol = 'o';
        enemy_symbol = 'x';
    }
    else
    {
        first_hand = 0;
        my_symbol = 'x';
        enemy_symbol = 'o';
    }
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                oxboard[i][j] = '-';
            }
            else if (board[i][j] == 1)
            {
                oxboard[i][j] = 'o';
            }
            else if (board[i][j] == 2)
            {
                oxboard[i][j] = 'x';
            }
        }
    }
}

void write_valid_spot(std::ofstream &fout)
{
    if (ones + twos > 34)
    {
        dep = 3;
    }
    else if (ones + twos > 16)
    {
        dep = 3;
    }
    else if (ones + twos > 4)
    {
        dep = 4;
    }
    else
    {
        dep = 5;
    }
    set_expfac();
    root = gen_tree(dep, 0, 0, neg_inf, pos_inf, '+');
    auto it = pri.back();

    fout << it.x << " " << it.y << std::endl;
    fout.flush();

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
