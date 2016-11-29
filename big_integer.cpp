#include <string>

#include "big_integer.h"


const static long notation = 4294967296;
#define base  32

static inline bool zeroCheck(big_integer &x) {

    size_t tmpForCycle = 0;
    while (x.data_.size() - tmpForCycle != 0 && x.data_[x.data_.size() - tmpForCycle - 1] == 0)
        tmpForCycle++;
    x.data_.deleteLast(tmpForCycle);

    if (x.data_.size() == 0) {
        x.data_.push_back(0);
        x.sign_ = false;
        return true;
    }
    if (x.data_.size() == 1 && x.data_[0] == 0) {
        x.sign_ = false;
        return true;
    }
    return false;
}

static inline big_integer negate(big_integer const x) {
    big_integer save(x);
    save.sign_ = !save.sign_;
    zeroCheck(save);
    return save;
}

big_integer::big_integer() {
    data_.clear();
    data_.push_back(0);
    sign_ = false;
}

big_integer::big_integer(int a) {
    data_.clear();
    int64_t tmp = a;
    sign_ = (a < 0);// false;
    if (tmp < 0) {
        tmp *= (-1);
    }
    data_.push_back((uint32_t) tmp);
    if ((tmp >> base) != 0) {
        data_.push_back((uint32_t) (tmp >> base));
    }
}

big_integer::big_integer(uint64_t a) {
    data_.clear();
    int64_t tmp = a;
    sign_ = false;
    data_.push_back((uint32_t) tmp);
    if ((tmp >> base) != 0) {
        data_.push_back((uint32_t) (tmp >> base));
    }
}

big_integer::big_integer(big_integer const &other) {
    data_ = other.data_;
    sign_ = other.sign_;
}

big_integer::big_integer(std::string const &str) {
    data_.clear();
    data_.push_back(0);
    sign_ = false;
    size_t begin = 0;
    if (str[0] == '-') {
        begin = 1;
    }
    for (size_t i = begin; i < str.size(); i++) {
        *this *= 10;
        *this += (str[i] - '0');
    }
    if (str[0] == '-') {
        sign_ = true;
    }
    zeroCheck(*this);
}

big_integer::~big_integer() {
    data_.clear();
}

big_integer &big_integer::operator=(big_integer const &other) {
    data_=other.data_;
    sign_ = other.sign_;
    return *this;
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
}

big_integer big_integer::operator++(int) {
    return *this += 1;
}

big_integer big_integer::operator--(int) {
    return *this -= 1;
}



big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (sign_ == rhs.sign_) {
        size_t tmpSize = std::max(data_.size(), rhs.data_.size());
        big_integer::dataContainer tmpData(tmpSize,(uint32_t) 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < tmpSize; i++) {
            int64_t tmp1=(i< data_.size() ? data_[i]:0);
            int64_t tmp2=(i < rhs.data_.size() ? rhs.data_[i] : 0);
            carry += tmp1 + tmp2;
            tmpData[i] = (uint32_t) carry;
            carry = (carry >> base);
        }
        if (carry)
            tmpData.push_back((uint32_t) carry);
        swapDC(data_,tmpData);
    } else {
        big_integer save(rhs);
        save.sign_=!save.sign_;
        *this -= save;
    }
    return (*this);
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    if (sign_ == rhs.sign_) {
        size_t tmpSize = std::max(data_.size(), rhs.data_.size());
        big_integer::dataContainer tmpData(tmpSize,(uint32_t) 0);
        bool reserveSub=false;
        if ((*this > rhs) == sign_) {
            reserveSub=true;
            sign_ = !sign_;
        }
        int64_t carry = 0;
        for (size_t i = 0; i < tmpSize; i++) {
            int64_t tmp1=(i< data_.size() ? data_[i]:0);
            int64_t tmp2=(i < rhs.data_.size() ? rhs.data_[i] : 0);
            carry = notation - carry + (reserveSub ? tmp2-tmp1: tmp1 - tmp2);
            tmpData[i] = (uint32_t) carry;
            carry = 1 - (carry >> base);
        }
        swapDC(data_,tmpData);
        zeroCheck(*this);
    } else {
        big_integer save(rhs);
        save.sign_ = !save.sign_;
        *this += save;
    }
    return (*this);
}//

big_integer big_integer::operator+() const {
    return *this;
}
big_integer big_integer::operator-() const {
    return negate(*this);
}
big_integer big_integer::operator~() const {

    return (-*this) - 1;
}//

static big_integer fastMult(big_integer const &x, uint32_t y) {
    uint64_t carry = 0;
    big_integer::dataContainer tmpData(x.data_.size(), (uint32_t) 0);
    for (size_t i = 0; i < x.data_.size(); i++) {
        carry = carry + (uint64_t) x.data_[i] * (uint64_t) y;
        tmpData[i] = (uint32_t) carry;
        carry = (carry >> base);
    }
    if (carry != 0)
        tmpData.push_back((uint32_t) carry);

    big_integer tmp;
    tmp.swapDC(tmp.data_, tmpData);
    zeroCheck(tmp);
    return tmp;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    if (rhs.data_.size() == 1) {
        data_ = fastMult(*this, rhs.data_[0]).data_;
        sign_ = (sign_ != rhs.sign_);
        return *this;
    } else {
        size_t tmpSize = data_.size() + rhs.data_.size();
        dataContainer tmpData(tmpSize + 1, 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < data_.size(); i++) {
            for (size_t j = 0; j < rhs.data_.size(); j++) {
                carry += (uint64_t) tmpData[i + j] + (uint64_t) data_[i] * (uint64_t) rhs.data_[j];
                tmpData[i + j] = (uint32_t) carry;
                carry = (carry >> base);
            }
            tmpData[i + rhs.data_.size()] += carry;
            carry = 0;
        }
        sign_ = (sign_ != rhs.sign_);
        swapDC(data_, tmpData);
        zeroCheck(*this);
        return *this;
    }
}

big_integer &big_integer::operator/=(big_integer const &rhs) {

    size_t tmpSize = data_.size() - rhs.data_.size() + 1;
    dataContainer tmpData(tmpSize, 0);
    big_integer divider(rhs);
    big_integer divisible;
    bool tmpSign = (sign_ != rhs.sign_);
    sign_ = false;
    divider.sign_ = false;
    for (size_t i = 0; i < tmpSize; i++) {
        uint64_t left = 0;
        uint64_t right = notation + 1;
        uint64_t mid;
        while (left + 1 < right) {
            mid = (left + right) / 2;
            divisible = fastMult(divider, (uint32_t) mid);
            divisible.data_.upgrade(tmpSize - i - 1);
            if (divisible <= *this)
                left = mid;
            else
                right = mid;
        }
        divisible = (divider * left);
        divisible.data_.upgrade(tmpSize - i - 1);
        (*this) -= divisible;
        tmpData[tmpSize - 1 - i] = (uint32_t) left;
    }
    swapDC(data_, tmpData);
    sign_ = tmpSign;
    zeroCheck(*this);
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    bool sign = this->sign_;
    *this -= (*this / rhs) * rhs;
    this->sign_ = sign;
    return *this;
}


bool operator==(big_integer const &a, big_integer const &b) {
    if (a.sign_ != b.sign_) {
        if ((a.data_.size() == 1 && a.data_[0] == 0) && (b.data_.size() == 1 && b.data_[0] == 0))
            return true;
        return false;
    }
    if (a.data_.size() != b.data_.size())
        return false;
    for (size_t i = 0; i != a.data_.size(); i++) {
        if (a.data_[i] != b.data_[i])
            return false;
    }
    return true;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign_ != b.sign_)
        return a.sign_ > b.sign_;
    if (a.data_.size() != b.data_.size())
        return (a.data_.size() < b.data_.size()) != a.sign_;
    for (size_t i = a.data_.size(); i != 0; i--)
        if (a.data_[i - 1] != b.data_[i - 1])
            return (a.data_[i - 1] < b.data_[i - 1]) != a.sign_;
    return false;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

bool operator>(big_integer const &a, big_integer const &b) {
    return (a >= b) && !(a == b);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

inline void big_integer::inverse(big_integer &a) {
    a += 1;
    for (size_t i = 0; i < a.data_.size(); i++) {
        a.data_[i] = (~a.data_[i]);
    }
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    big_integer save(rhs);
    if (this->sign_)
        inverse(*this);
    if (save.sign_)
        inverse(save);
    size_t tmpSize = std::max(this->data_.size(), save.data_.size());
    dataContainer tmpData(tmpSize);
    for (size_t i = 0; i < tmpSize; i++)
        tmpData[i] = (i < this->data_.size() ? this->data_[i] : 0) & (i < save.data_.size() ? save.data_[i] : 0);

    size_t tmpForWhile = 0;
    while (tmpData.size() - tmpForWhile != 0 && tmpData[tmpData.size() - tmpForWhile - 1] == 0)
        tmpForWhile++;
    tmpData.deleteLast(tmpForWhile);

    swapDC(this->data_, tmpData);
    if (this->sign_ && rhs.sign_) {
        inverse(*this);
    }
    sign_ = sign_ && rhs.sign_;
    zeroCheck(*this);
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    (*this) = (~(*this)) & (~rhs);
    (*this) = (~(*this));
    return (*this);
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    big_integer tmp = (*this) & (~rhs);
    big_integer tmp2 = (~(*this)) & (rhs);
    (*this) = tmp | tmp2;
    return (*this);
}


big_integer &big_integer::operator<<=(int rhs) {
    big_integer tmp(1);
    for (int i = 0; i < rhs; i++)
        tmp *= 2;
    (*this) *= tmp;
    return (*this);
}

big_integer &big_integer::operator>>=(int rhs) {
    big_integer tmp(1);
    for (int i = 0; i < rhs; i++)
        tmp *= 2;
    if (this->sign_) {
        big_integer tmpTmp = (*this) / tmp;
        if (tmp * tmpTmp != (*this)) {
            tmpTmp--;
            std::swap((*this), tmpTmp);
        }
    } else
        (*this) /= tmp;
    return (*this);
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}
big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}
big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}
big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}
big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}
big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}
big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}
big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}
big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}
big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}


std::string to_string(big_integer const &a) {
    std::string str;
    big_integer safe(a), tmp;
    while (safe != 0) {
        tmp = safe % 10;
        safe /= 10;
        str += (tmp.data_[0] + '0');
    }
    if (a.sign_) {
        str += '-';
    }
    std::string answer;

    for (size_t j = str.size(); j > 0; j--) {
        answer = answer + str[j - 1];
    }
    if (answer == "")
        return "0";
    return answer;
}


std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}
void operator>>(std::istream &s, big_integer &a) {
    std::string str;
    s >> str;
    a = big_integer(str);
}
