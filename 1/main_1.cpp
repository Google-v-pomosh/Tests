#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

class CVector
{
public:
    CVector(double x1) { this->x1 = x1; }
    virtual double getLength() const = 0;

protected:
    double x1;
};

class CVector1 : public CVector
{
public:
    CVector1(double x1) : CVector(x1) {}
    double getLength() const override
    {
        return fabs(x1);
    }
};

class CVector2 : public CVector
{
public:
    CVector2(double x1, double x2) : CVector(x1)
    {
        this->x2 = x2;
    }
    double getLength() const override
    {
        return sqrt(x1 * x1 + x2 * x2);
    }

private:
    double x2;
};

class CVectorList
{

public:
    CVectorList()
    {
        size = 0;
        head = nullptr;
    }

private:
    class node
    {

    public:
        node *next;
        CVector *data;

        node(CVector *vec)
        {
            next = nullptr;
            data = vec;
        }
    };
    node *head;
    node *last;
    uint32_t size;

public:
    void Add(CVector *vec)
    {
        if (size == 0)
        {
            this->head = new node(vec);
            last = head;
        }
        else
        {
            last->next = new node(vec);
            last = last->next;
        }
        size++;
    }
    void PrintLength()
    {
        uint32_t tempSize = size;
        node *tempNode = head;
        while(tempSize > 0)
        {
            double len = tempNode->data->getLength();
            std::cout << len << std::endl;
            tempNode = tempNode->next;
            tempSize--;
        }
    }
};

// int main()
// {
//     CVector1 vec1(3.0);
//     CVector2 vec2(3.0, 4.0);
//     CVector2 vec3(6.0, 8.0);
    

//     CVectorList list;
//     list.Add(&vec1);
//     list.Add(&vec2);
//     list.Add(&vec3);
//     list.PrintLength();
// }
