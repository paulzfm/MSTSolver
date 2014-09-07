#ifndef MST_UTIL_H_
#define MST_UTIL_H_

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

namespace mst
{


#define DISABLE_COPYING(TypeName)    \
  TypeName(const TypeName&);         \
  void operator = (const TypeName&);

#define ABS(x) ((x) > 0 ? (x) : (-(x)))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define FILE_NOT_FOUND(file) \
  {                          \
    std::cerr << "File [" << file << "] failed to load.\n"; \
    exit(1);                 \
  }

template <class T>
class Point
{
public:
    T x;		// x-coordinate
    T y;		// y-coordinate

    Point() : x(0), y(0) {}
    Point(T& x, T& y) : x(x), y(y) {}
    virtual ~Point() {}

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")";
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ")";
        return ss.str();
    }

    bool near(const Point<T>& p, double scale)
    {
        float eps = 3.0 / scale;
        return (ABS(p.x - this->x) < eps && ABS(p.y - this->y) < eps);
    }

    bool friend operator == (const Point<T>& a, const Point<T>& b)
    {
        return (ABS(a.x - b.x) < 1.0 && ABS(a.y - b.y) < 1.0);
    }

    friend std::ostream& operator << (std::ostream& out, const Point<T>& p)
    {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }

    friend std::ofstream& operator << (std::ofstream& out, const Point<T>& p)
    {
        out << p.x << " " << p.y << "\n";
    }
};

template <class T>
class Edge
{
public:
    Point<T> s;     // starting point of the egde
    Point<T> e;     // ending point of the edge

    Edge() : s(Point<T>()), e(Point<T>()) {}
    Edge(Point<T>& p1, Point<T>& p2) : s(p1), e(p2) {}
    virtual ~Edge() {}

    float distance() const  // euclid distance
    {
        return sqrt((s.x - e.x) * (s.x - e.x) + (s.y - e.y) * (s.y - e.y));
    }

    void print() const
    {
        std::cout << s << " -> " << e;
    }

    bool friend operator == (const Edge<T>& a, const Edge<T>& b)
    {
        return ((a.s == b.s && a.e == b.e) || 
                (a.s == b.e && a.e == b.s));
    }

    friend std::ostream& operator << (std::ostream& out, const Edge<T>& b)
    {
        out << b.s << " -> " << b.e;
        return out;
    }
};

// Discard... Use Graph instead for drawing
// template <class T>
// class Tree
// {
// public:
//     Tree() {}
//     virtual ~Tree() {}

//     void add(Edge<T>& e)
//     {
//         edge_list_.push_back(e);
//     }

//     int get_count() const 
//     {
//         return edge_list_.size();
//     }

//     void print() const
//     {
//         for (int i = 0; i < edge_list_.size(); ++i)
//             std::cout << edge_list_[i] << "\n";
//     }

// private:
//     std::vector< Edge<T> > edge_list_;
// };

// Discard... Because we use a new Clarkson-Delaunay!
//class Vec3f
//{
//public:
//    Vec3f() {}
//    Vec3f(int x, int y, int z) : x_(x), y_(y), z_(z) {}
//    int x() {return x_;}
//    int y() {return y_;}
//    int z() {return z_;}

//    void print()
//    {
//        std::cout << x_ << "," << y_ << "," << z_ << "\n";
//    }

//private:
//    int x_;
//    int y_;
//    int z_;
//};

template <class T>
class Set   // Do not need any compare, compared with the std::set
            // I use this to store unrepeated elements
{
public:
    Set() {}
    virtual ~Set() {}

    void push_back(const T& ele)
    {
        if (find(ele) == -1)
            v.push_back(ele);
    }

    int find(const T& ele) const       // return -1 if not found
    {
        for (int i = 0; i < v.size(); ++i)
            if (v[i] == ele)
                return i;
        return -1;
    }

    int size() const
    {
        return v.size();
    }

    void clear()
    {
        v.clear();
    }

    T operator [](int index)
    {
        return v[index];
    }

private:
    std::vector<T> v;
};

template <class T>
class Graph // this Graph is only for drawing use
{
public:
    Graph() {}

    // for graph
    void set(std::vector< Point<T> >& points, unsigned int* triangles, int numTriangles)
    {
        edge_list.clear();
        for (int i = 0; i < numTriangles; ++i)
        {
            int a = triangles[i * 3];
            int b = triangles[i * 3 + 1];
            int c = triangles[i * 3 + 2];
            edge_list.push_back(Edge<T>(points[a], points[b]));
            edge_list.push_back(Edge<T>(points[b], points[c]));
            edge_list.push_back(Edge<T>(points[a], points[c]));
        }
    }

    // for MST
    void set(std::vector< Point<T> >& points, std::vector<int>& links)
    {
        edge_list.clear();
        int i = 0;
        while (i < links.size())
            edge_list.push_back(Edge<T>(points[links[i++]], points[links[i++]]));
    }

    void print()
    {
        for (int i = 0; i < edge_list.size(); ++i)
            std::cout << edge_list[i] << "\n";
    }

    std::vector< Edge<T> > edge_list;
};

template <typename T>
float euclid_distance(const Point<T>& p1, const Point<T>& p2)
{
    // return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
    // The follwing one is much quicker...
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}


} // namespace mst

#endif // MST_UTIL_H_
