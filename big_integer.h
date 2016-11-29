#ifndef TRY_BIG_INTEGER_H
#define TRY_BIG_INTEGER_H

#include <iosfwd>
#include <stdint.h>


struct big_integer;
static big_integer negate (big_integer const x);
static inline bool zeroCheck (big_integer &x);
static big_integer fastMult(big_integer const &x, uint32_t y);
inline void inverse(big_integer &a);
struct big_integer
{
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(uint64_t a);
    explicit big_integer(std::string const& str);
    ~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer const& a);
    ////
    friend big_integer negate(big_integer const x);
    friend bool zeroCheck(big_integer &x);
    friend big_integer fastMult(big_integer const &x, uint32_t y);


private:
    void inverse(big_integer &a);
    struct dataContainer;

    static void swapDC(dataContainer&,dataContainer&);
    struct dataContainer {
        dataContainer();
        dataContainer(size_t, uint32_t);//забить одним символом
        dataContainer(size_t);//
        dataContainer(const dataContainer& other);
        ~dataContainer();
        uint32_t const& operator[](size_t)const;//a[x]=A[x]
        uint32_t & operator[](size_t);//A[x]=a[x]
        dataContainer& operator=(dataContainer const&);
        size_t size()const;
        void deleteLast(size_t);
        void push_back(uint32_t);
        void clear();
        void upgrade (size_t);

        friend struct big_integer;

    private:
        static const size_t limit=sizeof(uint32_t*)/4;
        //const uint32_t fZero[1]={0};
        size_t count;//size of data 1=> valIn
        size_t fZeros;//for upgrade - count of first zeros
        union {
            uint32_t* dataIn;//1st elem - counter; than values
            uint32_t valIn[limit];
        };
    };

    dataContainer data_;
    bool sign_;

};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);



std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);
void operator>>(std::ostream &st, big_integer &a);

#endif // TRY_BIG_INTEGER_H