#ifndef VECTORITERATOR_H
#define VECTORITERATOR_H

#include <iterator>
#include "gold/debug_assert.h"

namespace nostd
{

struct false_type
{
  static constexpr bool value = false;

  constexpr operator bool () {return value;}
};

struct true_type
{
  static constexpr bool value = true;

  constexpr operator bool () {return value;}
};

template<typename>
struct is_const
    : public false_type { };

template<typename _Tp>
struct is_const<_Tp const>
    : public true_type { };

template<typename T>
struct add_const
{ typedef T const type; };

template<typename T>
using add_const_t = typename add_const<T>::type;

template<typename T, bool is_const>
struct add_const_if {};

template<typename T>
struct add_const_if<T, true>
{
  using type = nostd::add_const_t<T>;
};

template<typename T>
struct add_const_if<T, false>
{
  using type = T;
};

template<typename T, bool is_const>
using add_const_if_t = typename add_const_if<T, is_const>::type;
} //namespace detail

template<typename VectorType>
struct VectorIterator
{
  using self = VectorIterator<VectorType>;
  using value_type = typename VectorType::value_type;
  using const_qual_value_type = nostd::add_const_if_t<value_type, nostd::is_const<VectorType>::value>;
private:
  VectorType *m_data = nullptr;
  int m_pos = -1;
public:
  VectorIterator () {}
  explicit VectorIterator (VectorType &data, int pos = -1) {m_data = &data; m_pos = pos;}

  const_qual_value_type &operator * () const {return (*m_data)[m_pos];}
  const_qual_value_type *operator-> () const {return &(*m_data)[m_pos];}
  const_qual_value_type &operator[] (int n) const {return (*m_data)[m_pos + n];}

  self &operator++ () {m_pos++; return *this;}
  self operator++ (int) {self retval (m_data, m_pos); m_pos++; return retval;}

  self &operator-- () {m_pos--; return *this;}
  self operator-- (int) {self retval (m_data, m_pos); m_pos--; return retval;}

  self &operator += (int n)
  {
    m_pos += n;
    return *this;
  }

  self &operator -= (int n)
  {
    m_pos -= n;
    return *this;
  }

  friend self operator + (const self &it, int n)
  {
    return VectorIterator<VectorType> (*it.m_data, it.m_pos + n);
  }

  friend self operator + (int n, const self &it)
  {
    return VectorIterator<VectorType> (*it.m_data, it.m_pos + n);
  }

  friend self operator - (const self &it, int n)
  {
    return VectorIterator<VectorType> (*it.m_data, it.m_pos - n);
  }

  friend self operator - (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, VectorIterator<VectorType> ());
    return VectorIterator<VectorType> (*lhs.m_data, lhs.m_pos - rhs.m_pos);
  }

  friend bool operator <  (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, false);
    return lhs.m_pos < rhs.m_pos;
  }

  friend bool operator >  (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, false);
    return lhs.m_pos > rhs.m_pos;
  }

  friend bool operator <= (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, false);
    return lhs.m_pos <= rhs.m_pos;
  }

  friend bool operator >= (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, false);
    return lhs.m_pos >= rhs.m_pos;
  }

  friend bool operator == (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, false);
    return lhs.m_pos == rhs.m_pos;
  }

  friend bool operator != (const self &lhs, const self &rhs)
  {
    ASSERT_RETURN (lhs.m_data == rhs.m_data, false);
    return lhs.m_pos != rhs.m_pos;
  }
};



namespace std
{
template<typename T>
struct iterator_traits<VectorIterator<T>>
{
  using difference_type = int;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using iterator_category = random_access_iterator_tag;
};
}

#endif // VECTORITERATOR_H
