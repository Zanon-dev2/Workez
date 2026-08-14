#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

//parte da workez com foco em trabalhar sobre dados, entradas, listas e afins
namespace workez{
    class zlisting{
    public:
    template<typename T>
    zlisting& operator>>(std::vector<T>& lista)
        {
            lista.clear();
            std::string line;
            std::cin>>std::ws;
            getline(std::cin, line);
            std::stringstream ss(line);
            T valor;
            while (ss >> valor)
            {
                lista.push_back(valor);
            }
            return *this;
        }
    };
    inline zlisting zlist;
}