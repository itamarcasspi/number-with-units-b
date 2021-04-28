#pragma once
#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
using namespace std;
namespace ariel
{

    class NumberWithUnits
    {

    private:
        double value;
        std::string unit;

    public:
        //constructor
        NumberWithUnits(double val,const string & uni);

        static void read_units(std::ifstream &file_name);

        

       
            //6 arithmetic operators
        friend NumberWithUnits operator+(const NumberWithUnits &a, const NumberWithUnits &b);

        NumberWithUnits& operator+=(const NumberWithUnits &b);

        friend NumberWithUnits operator+(const NumberWithUnits &a);

        friend NumberWithUnits operator-(const NumberWithUnits &a, const NumberWithUnits &b);

        NumberWithUnits& operator-=(const NumberWithUnits &b);

        friend NumberWithUnits operator-(const NumberWithUnits &a);

        //6 comparisment operators

        friend bool operator>(const NumberWithUnits &a, const NumberWithUnits &b);

        friend bool operator>=(const NumberWithUnits &a, const NumberWithUnits &b);

        friend bool operator<(const NumberWithUnits &a, const NumberWithUnits &b);

        friend bool operator<=(const NumberWithUnits &a, const NumberWithUnits &b);

        friend bool operator==(const NumberWithUnits &a, const NumberWithUnits &b);

        friend bool operator!=(const NumberWithUnits &a, const NumberWithUnits &b);

        //increment decrement operators

        friend NumberWithUnits &operator++(NumberWithUnits &a); //prefix

        friend NumberWithUnits operator++(NumberWithUnits &a, int); //postfix

        friend NumberWithUnits &operator--(NumberWithUnits &a); //prefix

        friend NumberWithUnits operator--(NumberWithUnits &a, int); //postfix

        //multiplication operator

        friend NumberWithUnits operator*(const NumberWithUnits &a,const double b);
        friend NumberWithUnits operator*(const double b,const NumberWithUnits &a);

        friend NumberWithUnits operator*(const NumberWithUnits &a,const NumberWithUnits &b);

        //Input output operators

        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &a);

        friend std::istream &operator>>(std::istream &is, NumberWithUnits &a);
    };

}