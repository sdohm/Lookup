#ifndef LOOKUP_H_
#define LOOKUP_H_



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



#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>



#define nullptr NULL


#define LINELENGTH ((255))
#define DELIMITER " "
#define READ "r"
#define CSTRINGTERMINATOR "\0"


#define AMAXINDEX  ((32))
#define DMAXINDEX  ((16))
#define AMAXVAL   ((180.0))
#define ABASESTEP   ((4.0))
#define DBASESTEP   ((0.4))

#define ASTEP ((SCALE / ABASESTEP))
#define DSTEP ((SCALE / DBASESTEP))

#define AMAX (((AMAXINDEX * SCALE) - 1))
#define DMAX (((DMAXINDEX * SCALE) - 1))



typedef int Integer;

typedef unsigned int Unsigned;

typedef FILE* FilePointer;

typedef char* CString;

typedef const char*const ConstCString;

typedef char FixedLengthCString[255];

typedef double Scalar;

typedef Scalar Epot;

typedef Scalar Distance;

typedef Scalar Angle;

typedef Scalar DotProduct;

typedef Scalar Gradient;

typedef time_t Time;

typedef struct {Scalar X;
                Scalar Y;
                Scalar Z;} Vector3;

typedef Vector3 VectorProduct;

typedef struct {Distance X;
                Distance Y;
                Distance Z;} Coordinate;

typedef struct {Distance X;
                Distance Y;
                Distance Z;} DistanceVector;

typedef struct {Gradient X;
                Gradient Y;
                Gradient Z;} GradientVector;

typedef struct {Vector3 O ;
                Vector3 Ha;
                Vector3 Hb;} Query; 

typedef struct {Epot          ePot     ;
                VectorProduct axbNormal;
                Query         grad     ;} Entry;

typedef Entry WaterTable[AMAXINDEX * SCALE][DMAXINDEX * SCALE][DMAXINDEX * SCALE];

typedef struct {Unsigned   maxAngle  ;
                Unsigned   maxDSmall ;
                Unsigned   maxDBig   ;
                WaterTable waterTable;} Data;

typedef struct {Scalar AA;
                Scalar AB;
                Scalar AC;
                Scalar BA;
                Scalar BB;
                Scalar BC;
                Scalar CA;
                Scalar CB;
                Scalar CC;} Matrix3x3;

typedef Matrix3x3 RotationMatrix;


const Data*const
learn(ConstCString pathToInputFile);


Epot
lookup(const Data*const data ,
       Query*const      query );



#endif // LOOKUP_H_
