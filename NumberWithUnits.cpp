#include "NumberWithUnits.hpp"
#include <map>
#include <cstdlib>
using namespace std;

namespace ariel
{
    const double EPSILON = 0.00001; 
    //static map so it is initialized globally and accesible from all units
    static map<string, map<string, double>> CONVERSION_MAP;

    NumberWithUnits::NumberWithUnits(double val,const string & uni)
    {
        CONVERSION_MAP.at(uni); //if not registered then throw exception


        this->value = val;
        this->unit = uni;
    }

    void NumberWithUnits::read_units(ifstream &file_name) //txt pattern is:  first_val first_unit equal second_val second_unit
    {
        double first_val = 0;
        double second_val = 0;

        string first_unit, second_unit, equal;

        while (file_name >> first_val >> first_unit >> equal >> second_val >> second_unit)
        {

            CONVERSION_MAP[first_unit][second_unit] = second_val;

            CONVERSION_MAP[second_unit][first_unit] = first_val / second_val;

            //now we need to make sure that the values are connected, eg: 1 km = 1000m, m = 100 cm, -> 1 km = 1000*100 cm
            //current_unit is the current map inside the first_unit conversion map.
            //connect the first unit:

            for (auto &current_unit : CONVERSION_MAP[first_unit])
            {
                double current_val = CONVERSION_MAP[second_unit][first_unit] * current_unit.second;

                CONVERSION_MAP[second_unit][current_unit.first] = current_val;
                CONVERSION_MAP[current_unit.first][second_unit] = 1 / current_val;
            }
            //connect the second unit:
            for (auto &current_unit : CONVERSION_MAP[second_unit])
            {
                double current_val = CONVERSION_MAP[first_unit][second_unit] * current_unit.second;

                CONVERSION_MAP[first_unit][current_unit.first] = current_val;
                CONVERSION_MAP[current_unit.first][first_unit] = 1 / current_val;
            }
        }
    }
    //convert from first unit to second unit.
    double convert(double val, const string &first_unit, const string &second_unit)
    {
        if (CONVERSION_MAP[first_unit].count(second_unit) == 0)
        {
            throw "Error. Conversion uknown";
        } //if == 0 means it is not initialized in the conversion map (conversion not in the txt file)
        if (first_unit == second_unit)
        {
            return val;
        }
        return val * CONVERSION_MAP[first_unit][second_unit];
    }
    //6 arithmetic operators
    NumberWithUnits operator+(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        double b_to_a = convert(b.value, b.unit, a.unit);
        NumberWithUnits temp(b_to_a + a.value, a.unit);
        return temp;
    }

    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits &b)
    {
        this->value+= convert(b.value,b.unit,this->unit);
        return *this;
    }

    NumberWithUnits operator+(const NumberWithUnits &a)
    {
        NumberWithUnits temp(+a.value,a.unit);
        return temp;
    }

    NumberWithUnits operator-(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        double b_to_a = convert(b.value, b.unit, a.unit);
        NumberWithUnits temp(a.value-b_to_a, a.unit);
        return temp;
    }

    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits &b)
    {
        this->value -= convert(b.value, b.unit, this->unit);
        return *this;
    }

    NumberWithUnits operator-(const NumberWithUnits &a)
    {
        NumberWithUnits temp(-a.value,a.unit);
        return temp;
    }

    // //6 comparisment operators

    bool operator>(const NumberWithUnits &a, const NumberWithUnits &b)
    {
       return a.value>convert(b.value,b.unit,a.unit);
    }

    bool operator>=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        double b_to_a = convert(b.value,b.unit,a.unit);
        return a.value>b_to_a || abs(a.value-b_to_a)<EPSILON;
    }

    bool operator<(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return a.value<convert(b.value,b.unit,a.unit);
    }

    bool operator<=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        double b_to_a = convert(b.value,b.unit,a.unit);
        return a.value<b_to_a || abs(a.value-b_to_a)<EPSILON;
    }

    bool operator==(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return abs(a.value-convert(b.value,b.unit,a.unit))<EPSILON;
    }

    bool operator!=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return a.value!=convert(b.value,b.unit,a.unit);
    }

    // //increment decrement operators

    NumberWithUnits &operator++(NumberWithUnits &a) //prefix
    {   
        a.value++;
        return a;;
    }
    NumberWithUnits operator++(NumberWithUnits &a, int)//postfix
    {
        NumberWithUnits pre(a.value++,a.unit);
        return pre;
    }

    NumberWithUnits &operator--(NumberWithUnits &a)//prefix
    {
        a.value--;
        return a;;
    }

    NumberWithUnits operator--(NumberWithUnits &a, int)//postfix
    {
        NumberWithUnits pre(a.value--,a.unit);
        return pre;
    }

    // //multiplication operator

    NumberWithUnits operator*(const NumberWithUnits &a, double b)
    {
        return NumberWithUnits(a.value*b, a.unit);
    }

    NumberWithUnits operator*(double b,const NumberWithUnits &a)
    { 
        return NumberWithUnits(a.value*b, a.unit);
    }

    NumberWithUnits operator*(const NumberWithUnits &a,const NumberWithUnits &b)
    {
        double b_to_a = convert(b.value,a.unit,b.unit);
        NumberWithUnits temp(a.value*b_to_a, a.unit);
        return temp;
    }

    // //Input output operators

    ostream &operator<<(ostream &os, const NumberWithUnits &a)
    {
        return os<<a.value<<"["<<a.unit<<"]";
    }

    istream &operator>>(istream &is, NumberWithUnits &a)
    {
        double value = 0;
        string unit;
        is>>value;
        char current = ' ';
        bool flag = false;
        while(current!=']')
        {
            if(current=='[')
            {
                flag = true;
            }
            is>>current;
            if(flag)
            {
                if(current!=' '&&current!=']') 
                {
                    unit+=current;
                }
            }
        }
        if(CONVERSION_MAP.count(unit)==0)
        {
            throw "Unit uknown";
        }
        a.unit = unit;
        a.value = value;
        return is;
    }
}
