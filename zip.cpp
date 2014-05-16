#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{ 
    int i = 0;

    /* Compressing */
    if(!strcmp(argv[1], "-c"))
    {
        char* data;
        int size = 0;
        ofstream newpack;
        newpack.open("packed.k", ios::binary);

        cout << "Starting to pack " << (argc-2) << " files" << endl;

        /* Go through the command line arguments (as in the files to pack) */
        for(i=2; i < argc; i++)
        {
            /* Read to memory and write to output file */
            ifstream file_to_pack;

            string file = argv[i];
            string fn = file.substr(file.find_last_of("/") + 1);

            if(fn.empty())
            {
                fn = file;
            }

            /* Start reading the data */
            file_to_pack.open(argv[i], ios::binary);

            if(file_to_pack.is_open())
            {
                /* Write the name */
                newpack << "XXX" << endl;
                newpack << fn << endl;

                file_to_pack.seekg(0, ios::end);
                size = file_to_pack.tellg();
                data = new char[size];
                file_to_pack.seekg(0, ios::beg);
                file_to_pack.read(data, size);
                file_to_pack.close();

                cout << "Packing file " << fn << " (" << size << " bytes).." << endl;

                /* Write the data and the end point */
                newpack.write(data, size);
            }
            else
            {
                cout << "Could not open file " << argv[i] << endl;
            }
        }

        /* Close the packed file */
        newpack.close();
        delete[] data;

        cout << "Done." << endl;
    }
    /* Extracting */
    else if(!strcmp(argv[1], "-e"))
    {
        cout << "Extracting packed.k" << endl;
        ifstream packed;

        packed.open("packed.k", ios::binary);

        if(packed.is_open())
        {
            string line;
            bool next_fn = false;
            string filename;
            ofstream extracted;

            /* Read through the packed file */
            while(getline(packed, line))
            {
                /* New file separator */
                if(!strcmp(line.c_str(), "XXX"))
                {
                    if(extracted.is_open())
                    {
                        extracted.close();
                    }

                    next_fn = true;
                    continue;
                }

                /* Get the filename */
                if(next_fn)
                {
                    next_fn = false;
                    filename = line;
                    cout << "Extracting " << filename << ".." << endl;
                    extracted.open(filename.c_str(), ios::binary);
                    continue;
                }

                /* Write to extracted file */
                if(extracted.is_open())
                {
                    extracted << line << endl;
                }
            }

            if(extracted.is_open())
            {
                extracted.close();
            }

        }
        else
        {
             cout << "Could not open packed.k" << endl;
        }
    }
    else
    {
        cout << "Gief me commands!" << endl;
    }
}