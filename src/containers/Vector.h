#ifndef Vector_H
#define Vector_H

#include <utility>
#include <memory>
#include <new>
#include <cstring>
#include <vector>

#include "impl/VectorIterator.h"



template<typename T>
class Vector
{
public:
  static constexpr int minimum_implicit_positive_capacity = 256;

  using Iterator = VectorIterator<Vector<T>>;
  using ConstIterator = VectorIterator<const Vector<T>>;

  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  using ReverseIterator = std::reverse_iterator<Iterator>;

  using value_type = T;

  Vector () = default;
  ~Vector ()
  {
    deallocate ();
  }

  Vector (const Vector &rhs)
  {
    if (rhs.m_size == 0)
      return;

    m_capacity = rhs.m_size;

    placement_allocate ();
    copy_new_in_place (rhs);
  }

  Vector &operator= (const Vector &rhs)
  {
    if (rhs.m_size == 0)
      {
        clear ();
        return *this;
      }

    if (m_capacity < rhs.m_size)
      clear ();

    if (!m_data)
      {
        m_capacity = rhs.m_size;
        placement_allocate ();
        copy_new_in_place (rhs);
      }
    else
      copy_in_place (rhs);

    return *this;
  }

  Vector (Vector &&rhs)
    : m_capacity (rhs.m_size),
      m_size (rhs.m_size),
      m_data (rhs.m_data)
  {
    rhs.nullify ();
  }

  Vector &operator= (Vector &&rhs)
  {
    clear ();

    m_capacity = rhs.m_size;
    m_size = rhs.m_size;
    m_data = rhs.m_data;

    rhs.nullify ();

    return *this;
  }

  template<typename V>
  Vector (int size, V &&filling_value)
  {
    m_capacity = size;
    placement_allocate ();

    for (int i = 0; i < size; i++)
      create_i (i, std::forward<V> (filling_value));
  }

  Vector (std::initializer_list<T> ilist)
    : Vector (ilist.size ())
  {
    for (auto &&val : ilist)
      emplace_back (std::move (val));
  }

  Vector &operator= (std::initializer_list<T> ilist)
  {
    Vector temp (ilist);

    *this = std::move (temp);

    return *this;
  }

  explicit Vector (int capacity)
  {
    m_capacity = capacity;
    m_size = 0;

    placement_allocate ();
  }

  void reserve (int capacity)
  {
    if (capacity <= m_capacity)
      return;

    reallocate (capacity);
  }

  void resize (int size)
  {
    if (size <= m_size)
      {
        for (int i = size; i < m_size; i++)
          std::destroy_at (reinterpreted ()[i]);

        m_size = size;

        return;
      }

    if (size <= m_capacity)
      {
        for (int i = m_size; i < size; i++)
          create_i (i, T ());

        m_size = size;
        return;
      }

    reallocate (size);

    for (int i = m_size; i < size; i++)
      create_i (i, T ());

    m_size = size;
  }

  void clear ()
  {
    deallocate ();
    nullify ();
  }

  Iterator begin () {return Iterator (*this, 0);}
  ConstIterator begin () const {return ConstIterator (*this, 0);}

  Iterator end () {return Iterator (*this, m_size);}
  ConstIterator end () const {return ConstIterator (*this, m_size);}

  ReverseIterator rbegin () {return Iterator (*this, m_size - 1);}
  ConstIterator rbegin () const {return ConstIterator (*this, m_size - 1);}

  ReverseIterator rend () {return Iterator (*this, -1);}
  ConstReverseIterator rend () const {return ConstIterator (*this, -1);}

  bool empty ()      const {return m_size == 0;}
  int size ()        const {return m_size;}
  int capacity ()    const {return m_capacity;}

  const T &back () const {return get_i (m_size - 1);}
  T &back ()             {return get_i (m_size - 1);}

  const T &operator[] (int pos) const {return reinterpreted ()[pos];}
  T &operator[] (int pos) {return reinterpreted ()[pos];}

  T pop_back ()
  {
    m_size--;
    T retval (std::move (get_i (m_size)));

    return retval;
  }

  template<typename V>
  void push_back (V &&value)
  {
    emplace_back (std::forward<V> (value));
  }

  template<typename... Args>
  void emplace_back (Args &&... args)
  {
    m_size++;
    create_i (m_size - 1, std::forward<Args> (args)...);
  }

  template<typename V>
  void push_back_alloc (V &&value)
  {
    emplace_back_alloc (std::forward<V> (value));
  }

  template<typename... Args>
  void emplace_back_alloc (Args &&... args)
  {
    if (m_size < m_capacity)
      {
        emplace_back (std::forward<Args> (args)...);
        return;
      }

    int new_capacity = (m_capacity == 0) ?
          minimum_implicit_positive_capacity :
          bounded_capacity (m_capacity * 2);

    reallocate (new_capacity);

    emplace_back (std::forward<Args> (args)...);
  }

private:
  int m_capacity = 0;
  int m_size = 0;
  char *m_data = nullptr;

  template<typename... Args>
  T &create_i (int pos, Args &&... args)
  {
    new (m_data + sizeof (T) * pos) T (std::forward<Args> (args)...);
    return get_i (pos);
  }

  T &create_default_i (int pos)
  {
    new (m_data + sizeof (T) * pos) T ();
    return get_i (pos);
  }

  T &get_i (int pos) {return (*this)[pos];}
  const T &get_i (int pos) const {return (*this)[pos];}

  T *reinterpreted () {return reinterpret_cast<T *> (m_data);}
  const T *reinterpreted () const {return reinterpret_cast<const T *> (m_data);}

  void nullify ()
  {
    m_size = 0;
    m_capacity = 0;
    m_data = nullptr;
  }

  static int bounded_capacity (int new_capacity)
  {
    if (new_capacity < minimum_implicit_positive_capacity)
      return minimum_implicit_positive_capacity;

    return new_capacity;
  }

  void deallocate ()
  {
    if (!m_data)
      return;

    for (int i = m_size - 1; i >= 0; i--)
      std::destroy_at (&get_i (i));

    delete[] m_data;
  }

  void reallocate (int new_capacity)
  {
    if (new_capacity == m_capacity)
      return;

    Vector<T> buf (new_capacity);

    for (int i = 0; i < m_size; i++)
      buf.emplace_back (std::move (get_i (i)));

    *this = std::move (buf);
  }

  void placement_allocate ()
  {
    m_data = new char[m_capacity * sizeof (T)];
  }

  void copy_new_in_place (const Vector &rhs)
  {
    m_size = rhs.m_size;

    for (int i = 0; i < m_size; i++)
      new (m_data + i * sizeof (T)) T (rhs[i]);
  }

  void copy_in_place (const Vector &rhs)
  {
    m_size = rhs.m_size;

    for (int i = 0; i < m_size; i++)
      get_i (i) = rhs[i];
  }

};

#endif // Vector_H
