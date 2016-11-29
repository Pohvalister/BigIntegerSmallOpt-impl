#include <algorithm>
#include "big_integer.h"

#define dataC(x) big_integer::dataContainer:: x


static inline void memcpy(uint32_t *a, const uint32_t *b, size_t size) {
    for (size_t i = 0; i < size; i++)
        a[i] = b[i];
}

inline void delDataIn(uint32_t *point) {
    if (point[0] == 1) {
        delete[] point;
    } else {
        point[0]--;
    }
}

inline void changeDataIn(uint32_t *&point, size_t size) {
    if (point[0] != 1) {
        point[0]--;
        uint32_t *tmp = new uint32_t[size + 1];
        memcpy((tmp + 1), (point + 1), size);
        point = tmp;
        point[0] = 1;
    }
}

inline void restoreZeros() {
    throw "programm cant get here";
}

dataC(dataContainer()) {
    fZeros = 0;
    count = 0;
    valIn[0] = 0;
}

dataC(dataContainer(size_t
              size, uint32_t
              value)) {
    fZeros = 0;
    count = size;
    if (size <= limit) {
        for (size_t i = 0; i < size; i++)
            valIn[i] = value;

    } else {
        size += 1;
        dataIn = new uint32_t[size];
        dataIn[0] = 1;
        for (size_t i = 1; i < size; i++)
            dataIn[i] = value;
    }
}

dataC(dataContainer(size_t
              size)) {
    fZeros = 0;
    count = size;
    if (size <= limit) {
        for (size_t i = 0; i < size; i++)
            valIn[i] = 0;
    } else {
        size += 1;
        dataIn = new uint32_t[size];
        dataIn[0] = 1;
    }
}

dataC(dataContainer(
              const dataContainer &other)) {
    fZeros = other.fZeros;
    count = other.count;
    if (count < limit) {
        memcpy(valIn, other.valIn, count);
    } else {
        dataIn = other.dataIn;
        dataIn[0]++;
    }
}

dataC(~dataContainer()) {
    if (count > limit)
        delDataIn(dataIn);
}

uint32_t const &dataC(operator[](size_t
                              point) const) {
    static const uint32_t fZero[1]={0};
    if (point < fZeros)
        return fZero[0];
    else if (count <= limit) {
        return valIn[point - fZeros];

    } else
        return dataIn[point - fZeros + 1];
}

uint32_t &dataC(operator[](size_t
                        point)) {
    if (point < fZeros) {
        restoreZeros();
        return valIn[point];
    } else if (count <= limit)
        return valIn[point];
    else {
        changeDataIn(dataIn, count);
        return dataIn[point + 1];
    }
}

big_integer::dataContainer &dataC(operator=(dataContainer const& other)) {
    fZeros = other.fZeros;
    if (count > limit)
        delDataIn(dataIn);
    count = other.count;
    if (count <= limit)
        memcpy(valIn, other.valIn, limit);
    else {
        dataIn = other.dataIn;
        dataIn[0]++;
    }
    return *this;
}

size_t dataC(size() const) {
    return count + fZeros;
}

void dataC(deleteLast(size_t
                   size)) {
    if (size == 0)
        return;
    if (fZeros > 0) restoreZeros();
    if (count - size > limit) {
        uint32_t *tmp = new uint32_t[count - size + 1];
        memcpy(tmp, dataIn, count - size + 1);
        delDataIn(dataIn);
        dataIn = tmp;
    } else if (count > limit) {
        uint32_t tmp[limit];
        memcpy(tmp, (dataIn + 1), count - size);
        delDataIn(dataIn);
        memcpy(valIn, tmp, count - size);
    }
    count -= size;
    if (count == 0) {
        count++;
        valIn[0] = 0;
    }
}

void dataC(push_back(uint32_t
                   value)) {
    if (count < limit)
        valIn[count] = value;
    if (count == limit) {
        uint32_t tmp[limit];
        memcpy(tmp, valIn, limit);
        dataIn = new uint32_t[limit + 2];
        dataIn[0] = 1;
        memcpy((dataIn + 1), tmp, limit);
        dataIn[limit + 1] = value;
    }
    if (count > limit) {
        uint32_t *tmp = new uint32_t[count + 2];
        memcpy(tmp, dataIn, count + 1);
        tmp[count + 1] = value;
        delDataIn(dataIn);
        dataIn = tmp;
    }
    count++;
}

void dataC(clear()) {
    if (count > limit) {
        delDataIn(dataIn);
    }
    count = 0;
    fZeros = 0;
}

void dataC(upgrade(size_t
                   size)) {
    fZeros += size;
}

void big_integer::swapDC(big_integer::dataContainer &a, big_integer::dataContainer &b) {
    if (a.count <= a.limit)
        if (b.count <= b.limit)
            std::swap(a.valIn, b.valIn);
        else {
            uint32_t *tmp = b.dataIn;
            memcpy(b.valIn, a.valIn, a.count);
            a.dataIn = tmp;
        }
    else if (b.count <= b.limit) {
        uint32_t *tmp = a.dataIn;
        memcpy(a.valIn, b.valIn, b.count);
        b.dataIn = tmp;
    } else {
        std::swap(a.dataIn, b.dataIn);
    }
    size_t tmp = a.count;
    a.count = b.count;
    b.count = tmp;
    tmp = a.fZeros;
    a.fZeros = b.fZeros;
    b.fZeros = tmp;
}






