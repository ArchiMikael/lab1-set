// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField((mp >> 5) + 1)
{
  MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField.GetLength() + 1)
{
  MaxPower = s.GetMaxPower();
  BitField = TBitField(s.BitField);
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf.GetLength() >> 5)
{
  MaxPower = bf.GetLength();
  BitField = TBitField(bf);
}

TSet::operator TBitField()
{
  TBitField t(BitField);
  return t;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
  return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
  return (BitField.GetBit(Elem));
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
  BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
  BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
  if (this == &s) { throw "Wrong TSet operator input"; }
  MaxPower = s.GetMaxPower();
  BitField = TBitField(s.BitField);
  return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
  if (MaxPower != s.GetMaxPower()) { return 0; }
  if (BitField == s.BitField) { return 1; }
  else { return 0; }
}

int TSet::operator!=(const TSet &s) const // сравнение
{
  return ((~(*this == s)) & 1);
}

TSet TSet::operator+(const TSet &s) // объединение
{
  TSet Result(*this);
  Result.MaxPower = max(GetMaxPower(), s.GetMaxPower());
  Result.BitField = Result.BitField | s.BitField;
  return Result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
  if (Elem >= MaxPower) { throw "Wrong TSet operator input"; }
  TSet Result(*this);
  Result.BitField.SetBit(Elem);
  return Result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
  if (Elem >= MaxPower) { throw "Wrong TSet operator input"; }
  TSet Result(*this);
  Result.BitField.ClrBit(Elem);
  return Result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
  TSet Result(*this);
  Result.MaxPower = min(GetMaxPower(), s.GetMaxPower());
  Result.BitField = Result.BitField & s.BitField;
  return Result;
}

TSet TSet::operator~(void) // дополнение
{
  TSet Result(*this);
  Result.BitField = ~Result.BitField;
  for (int i = Result.MaxPower; i < Result.BitField.GetLength(); i++) { Result.BitField.ClrBit(i); }
  return Result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
  int i;
  cin >> i;
  if (i >= 0 && i < s.GetMaxPower()) { s.InsElem(i); }
  return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
  cout << '{';
  for (int i = 0; i < s.GetMaxPower(); i++) { if (s.IsMember(i)) { cout << i << ',' << ' '; } }
  cout << '}';
  return ostr;
}
