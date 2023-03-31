// g++ vEB.cpp mt19937-64.c -o vEB
//https://iq.opengenus.org/van-emde-boas-tree/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include<bits/stdc++.h>
#include <chrono>
#include "./mt64.h"

#include <iostream>
#include <bitset>

using namespace std;

#define BASECASE 4
#define BASE_VAR uint16_t
#define VAR_BITS_SHIFT 32

bool basecase = false;

int iteration_reduction = 5;
int density_modifier = 4;

class vEB_tree{
public:
    uint64_t min;
    uint64_t max;

    uint64_t getMin()
    {
        return min;
    }

    uint64_t getMax()
    {
        return max;
    }
 
    virtual void insert(uint64_t key){}
    virtual uint64_t predecessor(uint64_t key){}
    virtual bool search(uint64_t key){}
};

class basecase_tree : public vEB_tree{
   // BASE_VAR min;
   // BASE_VAR max;

    uint8_t bit_array;

public:
    basecase_tree()
    {
        bit_array = 0x0;
        min = BASECASE + 1;
        max = NULL;
    }

    void insert(uint64_t key) 
    {
        bit_array |= 1 << key;
        if(key < min)
        {
            min = key;
        }       
            
        if(key > max)
        {
            max = key;
        }
    }

    uint64_t predecessor(uint64_t key)
    {
        if(min > BASECASE || key < min) 
        {
            return BASECASE + 1;
        }
        while(!(bit_array & (1 <<key)))
        {
            key--;
        }

        return key;
    }

    bool search(uint64_t key)
    {
        return bit_array & 1 << key;
    }
};


class vEB_tree_normal: public vEB_tree{
     
    public: 
    uint64_t u;
    uint64_t child_u;
    //uint64_t min;
    //uint64_t max;
    vEB_tree** cluster;
    vEB_tree* summary;

    vEB_tree_normal(uint64_t universe)
    {
        u = universe;
        child_u = ceil(sqrt(u));

        min = u + 1;
        max = u + 1;
        
        
        cluster = (vEB_tree**)malloc(child_u * sizeof(vEB_tree*));
        for(int i = 0; i < child_u; i++)
        {
            cluster[i] = nullptr;
        }
        if(!basecase || child_u > BASECASE )
        {
            if(u > 2)
            {
                summary = new vEB_tree_normal(child_u);
            }
        }
        else
        {
            summary = new basecase_tree();
        }
    }

    uint64_t high(uint64_t key)
    {
        return key / child_u;
    }

    uint64_t low(uint64_t key)
    {
        return key % child_u;
    }

    uint64_t index(uint64_t high, uint64_t low)
    {
        return (high * child_u) + low;
    }

    uint64_t predecessor(uint64_t key) //broken?
    {
        if(key == min )
        {
            return min;
        }
        else if(key >= max)
        {
            return max;
        }
        else if(u <= 2)
        {
            return u + 1;
        }
        else if(cluster[high(key)] != NULL && low(key) >= cluster[high(key)]->getMin()) //min broken 
        {
            return index(high(key), cluster[high(key)]->predecessor(low(key)));
        }
        else if(high(key) > 0)
        {
            uint64_t high_bits = summary->predecessor(high(key) - 1);
            if(high_bits < child_u)
            {
                uint64_t low_bits =  cluster[high_bits]->max;
                return index(high_bits, low_bits);
            }
        }
        return min;                
    }

    void insert(uint64_t key)
    {
        if(min > u) //min set to u + 1 on allocation therefore will trigger on first insert
        {
            min = key;
            max = key;
        }
        
        else if(key < min)
        {
            uint64_t temp = min;
            min = key;
            key = temp;
        }

        else if(key > min && u > 2)
        {
            if(cluster[high(key)] != NULL)
            {
                cluster[high(key)]->insert(low(key)); //cluster recursve call
            }
            else //if(child_u > 1)
            {
                // create cluster
                summary->insert(high(key)); // summary recursive call
                
                if(!basecase || child_u > BASECASE)
                {
                    cluster[high(key)] = new vEB_tree_normal(child_u);
                }
                else
                {
                    cluster[high(key)] = new basecase_tree();
                }
                cluster[high(key)]->insert(low(key)); //cluster single depth recursion
            }
        }

        if(key > max || max == u + 1)
        {
            max = key;
        }
    }

    bool search(uint64_t key)
    {
        if(key == min || key == max)
        {
            return true;
        }
        else if(u <= 2)
        {
            return false;
        }
        else if(cluster[high(key)] != NULL)
        {
            return cluster[high(key)]->search(low(key));
        }

        return false; 
    }   
};



void test_insert(vEB_tree *head, uint64_t universe)
{
    uint32_t insertions = universe >> iteration_reduction + 1;

    auto start = chrono::high_resolution_clock::now();    

    ios_base::sync_with_stdio(false);

    for(int i = 0; i < insertions; i++)
    {
        init_genrand64(time(NULL) + i);
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;

    //printf("%" PRIx64 "\n", key);

        head->insert(key);
    }

    auto end = chrono::high_resolution_clock::now();


    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    running_time *= 1e-9;
    cout << " " << fixed
        << running_time << setprecision(9);
    cout << endl;
}

void test_search(vEB_tree *head, uint64_t universe)
{
    uint32_t insertions = universe >> iteration_reduction;

    auto start = chrono::high_resolution_clock::now();    

    ios_base::sync_with_stdio(false);

    for(int i = 0; i < insertions; i++)
    {
        init_genrand64(time(NULL) + i);
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;
 //      uint32_t key = rand()* rand(); //not going to full u

        head->search(key);
    }

    auto end = chrono::high_resolution_clock::now();


    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    running_time *= 1e-9;
    cout << " " << fixed
        << running_time << setprecision(9);
    cout << endl;
}

void test_predecessor(vEB_tree *head, uint64_t universe)
{
    uint32_t insertions = universe >> iteration_reduction;

    auto start = chrono::high_resolution_clock::now();    

    ios_base::sync_with_stdio(false);

    for(int i = 0; i < insertions; i++)
    {
       init_genrand64(time(NULL) + i);
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;
//       uint32_t key = rand()*rand() % universe; //not going to full u

        head->predecessor(key);
    }

    auto end = chrono::high_resolution_clock::now();


    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    running_time *= 1e-9;
    cout << " " << fixed
        << running_time << setprecision(9);
    cout << endl;
}

void test_spin(uint64_t universe)
{
    uint32_t insertions = universe >> iteration_reduction;

    auto start = chrono::high_resolution_clock::now();    

    ios_base::sync_with_stdio(false);

    for(int i = 0; i < insertions; i++)
    {
       init_genrand64(time(NULL) + i);
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;
//       uint32_t key = rand()*rand() % universe; //not going to full u
        key = 0;
    }

    auto end = chrono::high_resolution_clock::now();


    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    running_time *= 1e-9;
    cout << " " << fixed
        << running_time << setprecision(9);
    cout << endl;
}

int main(int argc, char *argv[])
{
//take input
    if(argc < 4){
        printf("please give inputs\n");
        return 1;
    }
    iteration_reduction = atoi(argv[1]);
    density_modifier = atoi(argv[2]);
    basecase = atoi(argv[3]);
    uint32_t universe = 0 - 1;

//    printf("%" PRIx64 "\n", universe);

    vEB_tree* head = new vEB_tree_normal(universe);

    //write test of loop time
    test_spin(universe);

    test_insert(head, universe);

    test_predecessor(head, universe);

    test_search(head, universe);

    //printf("%" PRIx64 "  ?\n", (uint64_t)(1 << 64) - 1 );

//    printf("%" PRIx64 "\n", head->high(test));
//    printf("%" PRIx64 "\n", head->low(test));
//    printf("%" PRIx64 "\n", head->index(head->high(test), head->low(test)));
    

}