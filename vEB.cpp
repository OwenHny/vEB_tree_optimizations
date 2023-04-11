// g++ vEB.cpp mt19937-64.c -o vEB
#include<bits/stdc++.h>
#include <chrono>
#include "./mt64.h"

using namespace std;

#define BASECASE 16
//#define BASECASE 16
//#define BASECASE 256
//#define BASECASE 65536
//#define BASECASE 4294967296 

bool basecase = false;

int iteration_reduction;
int density_modifier;

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
    virtual uint64_t predecessor(uint64_t key){return 0;}
    virtual bool search(uint64_t key){return false;}
};

class basecase_tree : public vEB_tree{

    uint32_t bit_array[BASECASE];

public:
    basecase_tree()
    {
        for(int i = 0; i < BASECASE; i++)
        {
            bit_array[i] = 0;
        }
        min = BASECASE + 1;
        max = 0;
    }

    void insert(uint64_t key) 
    {
        bit_array[key] = 1;
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
        while(!bit_array[key])
        {
            key--;
        }
        return key;
    }

    bool search(uint64_t key)
    {
        return bit_array[key];
    }
};

class vector_basecase_tree : public vEB_tree{

    uint16_t bit_array;

public:
    vector_basecase_tree()
    {
        bit_array = 0x0;
        min = BASECASE + 1;
        max = 0;
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

class vEB_tree_normal: public vEB_tree
{
     
public: 
    uint64_t u;
    uint64_t child_u;
    vEB_tree** cluster;
    vEB_tree* summary;

    vEB_tree_normal(uint64_t universe)
    {
        u = universe;
        child_u = ceil(sqrt(u));

        min = u + 1;
        max = u + 1;
        
        
        cluster = (vEB_tree**)malloc(child_u * sizeof(vEB_tree*));
        for(uint32_t i = 0; i < child_u; i++)
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
            //summary = new vector_basecase_tree();
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

    uint64_t predecessor(uint64_t key) 
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
        else if(cluster[high(key)] != NULL && low(key) >= cluster[high(key)]->getMin()) 
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
            else 
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
                    //cluster[high(key)] = new vector_basecase_tree();
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
    uint32_t insertions = universe >> iteration_reduction;

    auto start = chrono::high_resolution_clock::now();    

    ios_base::sync_with_stdio(false);

        init_genrand64(time(NULL));
    for(uint32_t i = 0; i < insertions; i++)
    {
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;

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

    init_genrand64(time(NULL));
    for(uint32_t i = 0; i < insertions; i++)
    {
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;

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

    init_genrand64(time(NULL));
    for(uint32_t i = 0; i < insertions; i++)
    {
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;

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

       init_genrand64(time(NULL) );
    for(uint32_t i = 0; i < insertions; i++)
    {
        uint32_t key = genrand64_int63();
        key = key >> density_modifier;

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
    
    vEB_tree* head = new vEB_tree_normal(universe);

    test_spin(universe);

    test_insert(head, universe);

    test_predecessor(head, universe);

    test_search(head, universe);

}
