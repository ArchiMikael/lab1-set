// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

const TELEM TELEM_SIZE = sizeof(TELEM) * 8;
const TELEM TELEM_SHIFT = 5;

TBitField::TBitField(int len)
{
  if (len <= 0) { throw "TBitField init Error"; }
  MemLen = len;
  BitLen = len * TELEM_SIZE;
  pMem = new TELEM[BitLen];
  for (int i = 0; i < BitLen; i++) { pMem[i] = 0; }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  if (&bf == this) { throw "TBitField init Error"; }
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[BitLen];
  for (int i = 0; i < BitLen; i++) { pMem[i] = bf.pMem[i]; }
}

TBitField::~TBitField()
{
  if (pMem != nullptr) { delete[] pMem; }
  BitLen = 0;
  MemLen = 0;
  pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if ((n < 0) || (n > MemLen)) { throw "Wrong TBitField function input"; }
  return n >> TELEM_SHIFT;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n < 0 || n > MemLen) { throw "Wrong TBitField function input"; }
  return 1 << (n & (TELEM_SIZE - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return MemLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if ((n < 0) || (n > MemLen)) { throw "Wrong TBitField function input"; }
  int ind = GetMemIndex(n);
  pMem[ind] = pMem[ind] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if ((n < 0) || (n > MemLen)) { throw "Wrong TBitField function input"; }
  int ind = GetMemIndex(n);
  pMem[ind] = pMem[ind] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n < 0) || (n > MemLen)) { throw "Wrong TBitField function input"; }
  return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this != &bf)
  {
    delete[] pMem;
    BitLen = bf.BitLen;
    pMem = new TELEM[BitLen];
    MemLen = bf.MemLen;
    for (int i = 0; i < BitLen; i++) { pMem[i] = bf.pMem[i]; }
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen) { return 0; }
  for (int i = 0; i < BitLen; i++) { if (pMem[i] != bf.pMem[i]) { return 0; } }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return (~(*this == bf)) & 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField Result(bf.MemLen > MemLen ? bf.MemLen : MemLen);
  for (int i = 0; i < BitLen; i++) { Result.pMem[i] = pMem[i]; }
  for (int i = 0; i < bf.BitLen; i++) { Result.pMem[i] = Result.pMem[i] | bf.pMem[i]; }
  return Result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField Result(bf.MemLen > MemLen ? bf.MemLen : MemLen);
  for (int i = 0; i < BitLen; i++) { Result.pMem[i] = pMem[i]; }
  for (int i = 0; i < bf.BitLen; i++) { Result.pMem[i] &= bf.pMem[i]; }
  return Result;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField Result(*this);
  for (int i = 0; i < Result.MemLen; i++)
  {
    if (Result.GetBit(i)) { Result.ClrBit(i); }
    else { Result.SetBit(i); }
  }
  return Result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  int temp;
  for (int i = 0; i < bf.GetLength(); i++)
  {
    cin >> temp;
    if (temp == 1) { bf.SetBit(i); }
    else { if (temp == 0) { bf.ClrBit(i); } }
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.GetLength(); i++) { ostr << bf.GetBit(i); }
  return ostr;
}
