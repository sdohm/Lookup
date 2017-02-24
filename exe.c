/*
 * Copyright 2016-2017 Sebastian Dohm
 * 
 * This file is part of Lookup.
 *
 * Lookup is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Lookup is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Lookup.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "exe.h"

Integer
main(Integer argc  ,
     CString argv[] )
{


  if(argc != 3)
  {
    printf("SYNTAX: lookup.exe <DATAFILE> <OHHFILE>\n");
    printf("OUTPUT: <CPUTIME> <EPOT> <OGRADX> <OGRADY> <OGRADZ> <HAGRADX> <HAGRADY> <HAGRADZ> <HBGRADX> <HBGRADY> <HBGRADZ> \n");
    return 0;
  }


  //
  // Import data from file.
  //

  const Data*const data = learn(argv[1]);
  if(data != nullptr)
  {


    //
    // Read request file.
    //
   
    FilePointer fp     = nullptr;
    Integer     lineNo = 0      ;
    if((fp = fopen(argv[2],
                   "r"     )))
    {
      FixedLengthCString line = "\0";
      Query query = {{0.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0} };
      bool isFirstH = true;
      while(fgets(line      ,
                  LINELENGTH,
                  fp         ))
      {
        ++lineNo;
        if(lineNo == 2)
        {
          if((strcmp(line                              ,
                     "OHH FILE using strict ordering\n" )))
          {
            printf("ERROR: OHHFILE malformated\n");
            return -1;
          }
        }
        if(lineNo >= 3)
        {
          CString token = strtok(line,
                                 " "  );
          if(!(strcmp(token,
                      "O"   )))
          {
            // process O ...
            if((token = strtok(nullptr,
                               " "     )))
            {
              query.O.X = atof(token);
              if((token = strtok(nullptr,
                                 " "     )))
              {
                query.O.Y = atof(token);
                if((token = strtok(nullptr,
                                   " "     )))
                  query.O.Z = atof(token);
                else
                {
                  printf("ERROR: line %d: coordinate malformated\n",
                         lineNo                                    );
                   return -1;
                }
              }
              else
              {
                printf("ERROR: line %d: coordinate malformated\n",
                       lineNo                                    );
                 return -1;
              }  
            }
            else
            {
              printf("ERROR: line %d: coordinate malformated\n",
                      lineNo                                    );
              return -1;
            }
          }
          else if(!(strcmp(token,
                           "H"   )))
          {
            // process H ...
            if(isFirstH)
            {
              isFirstH = false;
              if((token = strtok(nullptr,
                                 " "     )))
              {
                query.Ha.X = atof(token);
                if((token = strtok(nullptr,
                                   " "     )))
                {
                  query.Ha.Y = atof(token);
                  if((token = strtok(nullptr,
                                     " "     )))
                    query.Ha.Z = atof(token);
                  else
                  {
                    printf("ERROR: line %d: coordinate malformated\n",
                           lineNo                                    );
                    return -1;
                  }
                }
                else
                {
                  printf("ERROR: line %d: coordinate malformated\n",
                         lineNo                                    );
                  return -1;
                }
              }
              else
              {
                printf("ERROR: line %d: coordinate malformated\n",
                        lineNo                                    );
                return -1;
              }
            }
            else
            {
              isFirstH = true;
              if((token = strtok(nullptr,
                                 " "     )))
              { 
                query.Hb.X = atof(token);
                if((token = strtok(nullptr,
                                   " "     )))
                { 
                  query.Hb.Y = atof(token);
                  if((token = strtok(nullptr,
                                     " "     )))
                  { 
                    query.Hb.Z = atof(token);
                  }
                  else
                  { 
                    printf("ERROR: line %d: coordinate malformated\n",
                           lineNo                                    );
                    return -1;
                  }
                }
                else
                { 
                  printf("ERROR: line %d: coordinate malformated\n",
                         lineNo                                    );
                  return -1;
                }
              }
              else
              { 
                printf("ERROR: line %d: coordinate malformated\n",
                        lineNo                                    );
                return -1;
              }

            }
          }
          else
          {
            if(!(token = strtok(nullptr,
                                " "     )))
              return 0;
            printf("ERROR: line %d: unknown element\n", 
                   lineNo                              );
            return -1;
          }
          // for each molecule ...
          if(( lineNo          != 3) &&
             ((lineNo + 1) % 3 == 0)   )
          {

            // query and write out ...
            Time before = clock();
            Epot ePot = lookup(data  ,
                               &query );
            Time after = clock();
            
            printf("%.7e %.7e  %.7e  %.7e  %.7e  %.7e  %.7e  %.7e  %.7e  %.7e  %.7e\n",
                   (double)(after - before) / CLOCKS_PER_SEC,
                   ePot      ,
                   query.O.X ,
                   query.O.Y ,
                   query.O.Z ,
                   query.Ha.X,
                   query.Ha.Y,
                   query.Ha.Z,
                   query.Hb.X,
                   query.Hb.Y,
                   query.Hb.Z );

          } 
        }
      }
      fclose(fp);
    }
    else
    {
      printf("ERROR: failed to open <OHHFILE>\n");
      return 0;
    }


    free((Data*)data);


  }
  else
  {
    printf("ERROR: learn function failed\n");
    return -1;
  }


  return 0;


}
