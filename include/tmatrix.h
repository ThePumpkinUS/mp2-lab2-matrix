// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0)
      throw out_of_range("Vector size should be greater than zero");
    if (sz > static_cast<size_t>(MAX_VECTOR_SIZE))
      throw out_of_range("Vector size is too large");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    if (sz == 0)
      throw out_of_range("Vector size should be greater than zero");
    if (sz > static_cast<size_t>(MAX_VECTOR_SIZE))
      throw out_of_range("Vector size is too large");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v) : sz(v.sz) //Копирования
  {
    pMem = new T[sz];
    std::copy(v.pMem, v.pMem + sz, pMem); //первый последний приёмник
  }
  TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem) //Перемещения
  {
    v.sz = 0;
    v.pMem = nullptr;
  }
  ~TDynamicVector()
  {
    delete[] pMem;
    pMem = nullptr;
    sz = 0;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
    if (this != &v)
    {
      TDynamicVector tmp(v);
      swap(*this, tmp);
    }
    return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept //Перемещающее
  {
    if (this != &v)
    {
      swap(*this, v);
      v.sz = 0;
      delete[] v.pMem;
      v.pMem = nullptr;
    }
    return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
  T& operator[](size_t ind)
  {
    return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
    return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
    if (ind >= sz)
      throw out_of_range("Vector index is out of range");
    return pMem[ind];
  }
  const T& at(size_t ind) const
  {
    if (ind >= sz)
      throw out_of_range("Vector index is out of range");
    return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
    if (sz != v.sz)
      return false;
    for (size_t i = 0; i < sz; i++)
      if (!(pMem[i] == v.pMem[i]))
        return false;
    return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
    return !(*this == v);
  }

///////////////////////////////

  // скалярные операции
  TDynamicVector operator+(T val)
  {
    TDynamicVector res(*this);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = res.pMem[i] + val;
    return res;
  }
  TDynamicVector operator-(T val)
  {
    TDynamicVector res(*this);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = res.pMem[i] - val;
    return res;
  }
  TDynamicVector operator*(T val)
  {
    TDynamicVector res(*this);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = res.pMem[i] * val;
    return res;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw out_of_range("Vectors have different sizes");
    TDynamicVector res(sz);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = pMem[i] + v.pMem[i];
    return res;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw out_of_range("Vectors have different sizes");
    TDynamicVector res(sz);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = pMem[i] - v.pMem[i];
    return res;
  }
  T operator*(const TDynamicVector& v) noexcept(noexcept(T()))
  {
    if (sz != v.sz)
      throw out_of_range("Vectors have different sizes");
    T res = T();
    for (size_t i = 0; i < sz; i++)
      res = res + pMem[i] * v.pMem[i];
    return res;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(checkSize(s))
  {
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
    if (sz != m.sz)
      return false;
    for (size_t i = 0; i < sz; i++)
      if (!(pMem[i] == m.pMem[i]))
        return false;
    return true;
  }

  bool operator!=(const TDynamicMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val)
  {
    TDynamicMatrix res(sz);
    for (size_t i = 0; i < sz; i++)
      for (size_t j = 0; j < sz; j++)
        res.pMem[i][j] = pMem[i][j] * val;
    return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
    if (v.size() != sz)
      throw out_of_range("Matrix and vector have different sizes");
    TDynamicVector<T> res(sz);
    for (size_t i = 0; i < sz; i++)
    {
      T s = T();
      for (size_t j = 0; j < sz; j++)
        s = s + pMem[i][j] * v[j];
      res[i] = s;
    }
    return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
    if (sz != m.sz)
      throw out_of_range("Matrices have different sizes");
    TDynamicMatrix res(sz);
    for (size_t i = 0; i < sz; i++)
      for (size_t j = 0; j < sz; j++)
        res.pMem[i][j] = pMem[i][j] + m.pMem[i][j];
    return res;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
    if (sz != m.sz)
      throw out_of_range("Matrices have different sizes");
    TDynamicMatrix res(sz);
    for (size_t i = 0; i < sz; i++)
      for (size_t j = 0; j < sz; j++)
        res.pMem[i][j] = pMem[i][j] - m.pMem[i][j];
    return res;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
    if (sz != m.sz)
      throw out_of_range("Matrices have different sizes");
    TDynamicMatrix res(sz);
    for (size_t i = 0; i < sz; i++)
      for (size_t j = 0; j < sz; j++)
      {
        T s = T();
        for (size_t k = 0; k < sz; k++)
          s = s + pMem[i][k] * m.pMem[k][j];
        res.pMem[i][j] = s;
      }
    return res;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      for (size_t j = 0; j < v.sz; j++)
        istr >> v.pMem[i][j];
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
    for (size_t i = 0; i < v.sz; i++)
    {
      for (size_t j = 0; j < v.sz; j++)
        ostr << v.pMem[i][j] << ' ';
      ostr << endl;
    }
    return ostr;
  }
};

#endif
