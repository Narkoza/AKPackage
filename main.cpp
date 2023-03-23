#include <iostream>
#include <conio.h>
#include <fstream.h>
#include <string>
#include "package.h"

int getSizeToEnd(ifstream& is);
void pack();
void unpack();
void menu();

std::string packTitle; // nazwa archiwum

int getSizeToEnd(ifstream& is)
{
	auto currentPosition = is.tellg();
	is.seekg(0, is.end);
	auto length = is.tellg() - currentPosition;
	is.seekg(currentPosition, is.beg);
	return length;
}

void pack()
{
	system("CLS");

	header h1;
    file pliki[100];
	int i = 0;

	h1.controlNumber = 2137;
	pliki[100].offset = 0;
	pliki[100].size = 0;

	std::cout<<"\nPodaj nazwe archiwum z rozszerzeniem: ";
	std::cin>>packTitle;

    /* TWORZENIE PLIKOW */

	std::cout << "\nPodaj liczbe plikow: ";
	std::cin >> h1.files;

	for (i = 0; i < h1.files; i++)
	{
		std::cout << "\nPodaj nazwe pliku nr." << i + 1 << ": ";
		std::cin >> pliki[i].title;
	}

	/* TWORZENIE ARCHIWUM */

	ofstream oFile(packTitle.c_str(), ofstream::binary);
	oFile.write((char*)&h1, sizeof(header));

	// startowy offset zaczyna sie po naglowku i danych plikow
	int defaultOffset = sizeof(header) + (sizeof(file) * h1.files);


	/* USTAWIANIE ROZMIARU I OFFSETU DLA PLIKOW */
	for (i = 0; i < h1.files; i++)
	{
		ifstream MyFile(pliki[i].title);

		if (MyFile)
		{
			int rozmiar = getSizeToEnd(MyFile);
			pliki[i].size = rozmiar;
			pliki[i].offset = defaultOffset;
			defaultOffset += rozmiar;
		}
	}

	for (i = 0; i < h1.files; i++)
	{
		// Kopiowanie informacji o danych (ze structa "file") do archiwum
		oFile.write((char*)&pliki[i], sizeof(file));
	}

	/* KOPIOWANIE PLIKOW DO ARCHIWUM */

	for (i = 0; i < h1.files; i++)
	{
		ifstream MyFile(pliki[i].title, ifstream::binary);
		int rozmiar = getSizeToEnd(MyFile);

		if (MyFile)
		{
			std::cout << "\nOtworzono plik " << "\"" << pliki[i].title << "\"\n";
			unsigned char *danePliku = (unsigned char*)malloc(rozmiar);
			MyFile.read(danePliku, rozmiar);
			oFile.write(danePliku, rozmiar);
		}
		else
		{
			std::cout << "\nBlad otwarcia pliku " << pliki[i].title << "\n";
			menu();
		}
			
		std::cout << "Rozmiar pliku: " << rozmiar << " bajtow\n";
	}

	/* WYSWIETLANIE PLIKOW */
	std::cout << "\nLiczba plikow: " << h1.files << ": \n\n";

	for (i = 0; i < h1.files; i++)
	{
		std::cout << "Plik " << i + 1 << ": " << pliki[i].title << "\n";
	}

	std::cout<<"\n\nGotowe!";
	getch();
	menu();
}

void unpack()
{
	system("CLS");

	header h;
	fstream Package;
	file *Files;

	h.controlNumber = 2137;

	std::cout<<"\nPodaj nazwe archiwum z rozszerzeniem: ";
	std::cin>>packTitle;

	Package.open (packTitle.c_str(), ios::in | ios::binary);

		if(Package.is_open())
		{
			Package.read((char*)&h, sizeof(header));

			if(!(h.controlNumber == 2137))
			{
				std::cout<<"Nieprawidlowa suma kontrolna pliku!";
				menu();
			}

			Files = new file[h.files];
			
			/* CZYTAJ DANE O PLIKACH */
			for(int i=0; i<h.files; i++)
			{
				Package.read((char*)&Files[i], sizeof(file));
				std::cout<<h.files<<endl;
			}
			
			/* WYPAKUJ ARCHIWUM */ 
			for(int j=0; j<h.files; j++)
			{
				Package.seekg(Files[j].offset, Package.beg);
				char* data = new char[Files[j].size];
				Package.read(data, Files[j].size);
				std::string out = Files[j].title;
				ofstream ofile;
				ofile.open(out.c_str(), ofstream::binary);
					std::cout<<Files[j].offset<<endl;
					std::cout<<Files[j].size<<endl;
					std::cout<<Files[j].title<<endl;
				ofile.write(data, Files[j].size);
			}
		}
		else
		{
			std::cout<<"Nie mozna otworzyc pliku...\n";
			menu();
		}


	Package.close();

	std::cout<<"\nGotowe!";
	getch();
	menu();
}

void menu()
{
	system("CLS");

	char key;

	std::cout<<"***************************************\n";
	std::cout<<"**                                   **\n";
	std::cout<<"**   AKPackage by Adam K. (C) 2023   **\n";
	std::cout<<"**                                   **\n";
	std::cout<<"***************************************\n\n";

	std::cout<<"WYBIERZ OPCJE: \n";
	std::cout<<"\n[1] Utworz archiwum";
	std::cout<<"\n[2] Wypakuj archiwum";
	std::cout<<"\n[q] Wyjscie";
	std::cout<<"\n\nWybor: ";

	key = getch();

	printf("\n");

	while(key != 'q')
	{
		switch(key)
		{
			case '1':
				pack();
				break;
			case '2':
				unpack();
				break;
			case 'q':
				exit(0);
				break;
			default:
				break;
		}
	}

	exit(0);
}

int main()
{
    menu();
	return 0;
}