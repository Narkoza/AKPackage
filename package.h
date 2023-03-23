#ifndef _PACKAGE_H_
#define _PACKAGE_H_

struct header
{
    int controlNumber; // liczba kontrolna
    int files; // liczba plikow
};

struct file
{
    char title[64]; // nazwa pliku max. 64 znaki
    int size; // rozmiar pliku
    int offset; // pozycja w pliku
};

#endif