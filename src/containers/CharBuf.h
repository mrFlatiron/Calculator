#ifndef CHARBUF_H
#define CHARBUF_H

template<int Capacity>
class CharBuf
{
  static_assert (Capacity > 0, "Size must be a positive integer");
public:
  void clear () {m_size = 0; m_data[0] = 0;}
  bool empty () const {return m_size == 0;}
  int size () const {return m_size;}
  static constexpr int capacity () {return Capacity;}
  void append (char c) {m_data[m_size] = c; m_size++; m_data[m_size] = 0;}
  const char *c_str () const {return m_data;}

  //dangerous, cuz ruins consistency. Use recalculate_size to restore it.
  char *str () {return m_data;}
  void recalculate_size ()
  {
    for (int i = 0; i < Capacity; i++)
      if (m_data[i] == 0)
        {
          m_size = i;
          return;
        }
  }

  char &operator[] (int pos) {return m_data[pos];}
  const char &operator[] (int pos) const {return m_data[pos];}
  const char &back () const {return m_data[m_size - 1];}
private:
  int m_size = 0;
  char m_data[Capacity];
};

#endif // CHARBUF_H
