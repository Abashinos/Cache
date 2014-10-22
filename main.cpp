#include <iostream>
#include <iomanip>
#include <list>
#include <chrono>
#include <algorithm>
#include <vector>

#define BUFFER_START_VALUE 1024
#define BUFFER_END_VALUE 10*1024*1024
#define BUFFER_MULTIPLIER 1.1
#define APPROXIMATION_NUM 20
#define CYCLES_NUM 1000
#define COLUMNS_WIDTH 12
#define PRECISION 4
#define GRAPH

using namespace std;
using namespace std::chrono;

struct elem
{
    elem(): next(NULL) {};
    long see[15];

    elem *next;
};

vector<elem> fill_elems (vector<size_t>& indices)
{
    size_t size = indices.size();
    vector<elem> elems(size);
    auto from_index = indices.begin();
    auto to_index = from_index + 1;

    for (;to_index != indices.end(); from_index++, to_index++)
    {
        elems[*from_index].next = &elems[*to_index];
    }
    elems[*from_index].next = &elems[*indices.begin()];
    return elems;
}
void fill_elems (vector<elem> &elems, vector<size_t>& indices)
{
    auto from_index = indices.begin();
    auto to_index = from_index + 1;

    for (;to_index != indices.end(); from_index++, to_index++)
    {
        elems[*from_index].next = &elems[*to_index];
    }
    elems[*from_index].next = &elems[*indices.begin()];
}

double seqAccess (vector<elem> &elems)
{
    elem* el = &elems[0];
    long t_start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    for (int i = 0; i < CYCLES_NUM; ++i)
    {
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;

        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
        el = el->next;
    }
    long t_stop = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    double time = ((double)(t_stop-t_start))/(CYCLES_NUM*32);
    return time;
}

double getAverageAccessTime (vector<elem> &elems) {
    double avTime = 0.0;
    for (int i = 0; i < APPROXIMATION_NUM; ++i) {
        avTime += seqAccess(elems);
    }
    return avTime /= APPROXIMATION_NUM;
}

int main(int argc, char** argv)
{
#ifndef GRAPH
    cout << "Size of element: " << sizeof(elem) << endl;
#endif
    for (unsigned long buf = BUFFER_START_VALUE; buf < BUFFER_END_VALUE; buf*= BUFFER_MULTIPLIER) {
        unsigned long number = buf / sizeof(elem);
#ifndef GRAPH
        cout << buf;
#endif
        vector<elem> elems(buf);
        vector<size_t> indices;

        for (size_t i = 0; i < number; ++i) {
            indices.push_back(i);
        }

        fill_elems(elems, indices);
        double avTimeSeq = getAverageAccessTime(elems);

#ifndef GRAPH
        cout << setprecision(PRECISION) << setw(COLUMNS_WIDTH) << avTimeSeq;
#endif

        srand(unsigned(time(0)));
        random_shuffle(indices.begin(), indices.end());

        fill_elems(elems, indices);
        double avTimeRand = getAverageAccessTime(elems);
#ifndef GRAPH
        cout << setprecision(PRECISION) << setw(COLUMNS_WIDTH) << avTimeRand;
        cout << setprecision(PRECISION) << setw(COLUMNS_WIDTH) << avTimeRand/avTimeSeq << endl;
#endif
#ifdef GRAPH
        cout << setprecision(PRECISION) << buf/1024 << " "
             << (int)avTimeRand << "," << (int)((avTimeRand-(int)avTimeRand)*1000) << endl;
#endif
    }
    return 0;
}