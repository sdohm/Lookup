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



#include "lookup.h"



const Data*const
learn(ConstCString pathToInputFile)
{


  if(!pathToInputFile)
    return nullptr;

  Data* data = (Data*)calloc(1, sizeof(Data));
  if(!data)
    return nullptr;

  data->maxAngle  = AMAX;
  data->maxDSmall = DMAX;
  data->maxDBig   = DMAX;


  //
  // Populate water table.
  //

  FilePointer fp = nullptr;
  if((fp = fopen(pathToInputFile,
                 READ            )))
  {

    FixedLengthCString line = CSTRINGTERMINATOR;
    while(fgets(line      ,
                LINELENGTH,
                fp         ))
    {

      CString token = strtok(line     ,
                             DELIMITER );
      if(token)
      {
        const Unsigned angleIndex = (((AMAXVAL - atof(token)) * ASTEP) >= AMAX) ?
                                    AMAX                                        :
                                    (Unsigned)((AMAXVAL - atof(token)) * ASTEP)  ;
        if((token = strtok(nullptr  ,
                           DELIMITER )))
        {
          const Unsigned dSmallIndex = ((atof(token) * DSTEP) >= DMAX) ?
                                       DMAX                            :
                                       (Unsigned)(atof(token) * DSTEP)  ;         
          if((token = strtok(nullptr  ,
                             DELIMITER )))
          {
            const Unsigned dBigIndex = ((atof(token) * DSTEP) >= DMAX) ?
                                       DMAX                            :
                                       (Unsigned)(atof(token) * DSTEP)  ;
            if((token = strtok(nullptr  ,
                               DELIMITER )))
            {
              data->waterTable[angleIndex][dSmallIndex][dBigIndex].ePot = atof(token);
              if((token = strtok(nullptr  ,
                                 DELIMITER )))
              {
                data->waterTable[angleIndex][dSmallIndex][dBigIndex].axbNormal.X = atof(token);
                if((token = strtok(nullptr  ,
                                   DELIMITER )))
                {
                  data->waterTable[angleIndex][dSmallIndex][dBigIndex].axbNormal.Y = atof(token);
                  if((token = strtok(nullptr  ,
                                     DELIMITER )))
                  {
                    data->waterTable[angleIndex][dSmallIndex][dBigIndex].axbNormal.Z = atof(token);
                    if((token = strtok(nullptr  ,
                                       DELIMITER )))
                    {
                      data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.O.X = atof(token);
                      if((token = strtok(nullptr  ,
                                         DELIMITER )))
                      {
                        data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.O.Y = atof(token);
                        if((token = strtok(nullptr  ,
                                           DELIMITER )))
                        {
                          data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.O.Z = atof(token);
                          if((token = strtok(nullptr  ,
                                             DELIMITER )))
                          {
                            data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.Ha.X = atof(token);
                            if((token = strtok(nullptr  ,
                                               DELIMITER )))
                            {
                              data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.Ha.Y = atof(token);
                              if((token = strtok(nullptr  ,
                                                 DELIMITER )))
                              {
                                data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.Ha.Z = atof(token);
                                if((token = strtok(nullptr  ,
                                                   DELIMITER )))
                                {
                                  data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.Hb.X = atof(token);
                                  if((token = strtok(nullptr  ,
                                                     DELIMITER )))
                                  {
                                    data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.Hb.Y = atof(token);
                                    if((token = strtok(nullptr  ,
                                                       DELIMITER )))
                                      data->waterTable[angleIndex][dSmallIndex][dBigIndex].grad.Hb.Z = atof(token);
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

    }

    fclose(fp);

  }
  else
    return nullptr;


  //
  // Interpolation.
  //

  bool done = false;
  do
  {
    done = true;
    for(Unsigned i  = 0   ;
                 i != AMAX;
               ++i         )
      for(Unsigned j  = 0   ;
                   j != DMAX;
                 ++j         )
        for(Unsigned k  = 0   ;
                     k != DMAX;
                   ++k         )
          if((data->waterTable[i][j][k].axbNormal.X == 0.0) &&
             (data->waterTable[i][j][k].axbNormal.Y == 0.0) &&
             (data->waterTable[i][j][k].axbNormal.Z == 0.0)   )
          {
            if((k != 0                                                  ) &&
               (!((data->waterTable[i][j][k - 1].axbNormal.X == 0.0) &&
                  (data->waterTable[i][j][k - 1].axbNormal.Y == 0.0) &&
                  (data->waterTable[i][j][k - 1].axbNormal.Z == 0.0)   ))   )// smaller dBig
              data->waterTable[i][j][k] = data->waterTable[i][j][k - 1];
            else if((k != DMAX                                               ) &&
                    (!((data->waterTable[i][j][k + 1].axbNormal.X == 0.0) &&
                       (data->waterTable[i][j][k + 1].axbNormal.Y == 0.0) &&
                       (data->waterTable[i][j][k + 1].axbNormal.Z == 0.0)   ))   )// larger dBig
              data->waterTable[i][j][k] = data->waterTable[i][j][k + 1];
            else if((j != 0                                                  ) &&
                    (!((data->waterTable[i][j - 1][k].axbNormal.X == 0.0) &&
                       (data->waterTable[i][j - 1][k].axbNormal.Y == 0.0) &&
                       (data->waterTable[i][j - 1][k].axbNormal.Z == 0.0)   ))   )// smaller dSmall
              data->waterTable[i][j][k] = data->waterTable[i][j - 1][k];
            else if((j != DMAX                                               ) &&
                    (!((data->waterTable[i][j + 1][k].axbNormal.X == 0.0) &&
                       (data->waterTable[i][j + 1][k].axbNormal.Y == 0.0) &&
                       (data->waterTable[i][j + 1][k].axbNormal.Z == 0.0)   ))   )// larger dSmall
              data->waterTable[i][j][k] = data->waterTable[i][j + 1][k];
            else if((i != 0                                                  ) &&
                    (!((data->waterTable[i - 1][j][k].axbNormal.X == 0.0) &&
                       (data->waterTable[i - 1][j][k].axbNormal.Y == 0.0) &&
                       (data->waterTable[i - 1][j][k].axbNormal.Z == 0.0)   ))   )// smaller angle
              data->waterTable[i][j][k] = data->waterTable[i - 1][j][k];
            else if((i != AMAX                                               ) &&
                    (!((data->waterTable[i + 1][j][k].axbNormal.X == 0.0) &&
                       (data->waterTable[i + 1][j][k].axbNormal.Y == 0.0) &&
                       (data->waterTable[i + 1][j][k].axbNormal.Z == 0.0)   ))   )// larger angle
              data->waterTable[i][j][k] = data->waterTable[i + 1][j][k];
            // No neighbor found; repeat.
            else
              done = false;
          }
  } while(!done);


  return data;


}



Epot
lookup(const Data*const data ,
       Query*const      query )
{


  if((!data ) ||
     (!query)   )
    return 0.0;


  //
  // Convert absolute coordinates to internal coordinates.
  // Also compute orientation information.
  //

  // O-Ha Vector
  const DistanceVector a = {.X = (query->Ha.X) - (query->O.X),
                            .Y = (query->Ha.Y) - (query->O.Y),
                            .Z = (query->Ha.Z) - (query->O.Z) };

  // O-Hb Vector
  const DistanceVector b = {.X = (query->Hb.X) - (query->O.X),
                            .Y = (query->Hb.Y) - (query->O.Y),
                            .Z = (query->Hb.Z) - (query->O.Z) };

  // Distance O-Ha
  const Distance dA = sqrt((a.X * a.X) +
                           (a.Y * a.Y) +
                           (a.Z * a.Z)  );

  // Distance O-Hb
  const Distance dB = sqrt((b.X * b.X) +
                           (b.Y * b.Y) +
                           (b.Z * b.Z)  );

  // Vector Product (O-Ha x O-Hb)
  const VectorProduct axb = {.X = (a.Y * b.Z) - (a.Z * b.Y),
                             .Y = (a.Z * b.X) - (a.X * b.Z),
                             .Z = (a.X * b.Y) - (a.Y * b.X) }; 

  const Scalar axbLength = sqrt((axb.X * axb.X) +
                                (axb.Y * axb.Y) +
                                (axb.Z * axb.Z)  );

  const Vector3 axbNormal = {.X = axb.X / axbLength,
                             .Y = axb.Y / axbLength,
                             .Z = axb.Z / axbLength };

  // Scalar Product (O-Ha . O-Hb)
  const DotProduct aDotB = (a.X * b.X) +
                           (a.Y * b.Y) +
                           (a.Z * b.Z)  ;

  // Angle (O-Ha V O-Hb)
  assert(((dA * dB) != 0.0) && "O-H distances are not zero");
  const Scalar angleTmp = aDotB / (dA * dB);
  const Angle  angle    = ((angleTmp >= -1.0) &&
                           (angleTmp <=  1.0)                 ) ?
                          (acos(angleTmp) * (180.0 / 3.141593)) :
                          104.45                                 ;

  const Unsigned angleIndex = (((AMAXVAL - angle) * ASTEP) >= AMAX) ?
                              AMAX                                  :
                              (Unsigned)((AMAXVAL - angle) * ASTEP)  ;

  assert((angleIndex <= data->maxAngle)  &&
         "using compatible watertable"     );

  // Switch Ha <-> Hb
  const bool switchHaHb = (dA > dB) ?
                          true      :
                          false      ;

  // Smaller Distance (O-Ha <> O-Hb)
  const Distance dSmall = (dA <= dB) ?
                          dA         :
                          dB          ;

  const Unsigned dSmallIndex = ((dSmall * DSTEP) >= DMAX) ?
                               DMAX                       :
                               (Unsigned)(dSmall * DSTEP)  ;

  assert((dSmallIndex <= data->maxDSmall) &&
         "using compatible watertable"      );

  // Bigger Distance (O-Ha <> O-Hb)
  const Distance dBig = (dA >  dB) ?
                        dA         :
                        dB          ;

  const Unsigned dBigIndex = ((dBig * DSTEP) >= DMAX) ?
                             DMAX                     :
                             (Unsigned)(dBig * DSTEP)  ;

  assert((dBigIndex <= data->maxDBig)  &&
         "using compatible watertable"   );


  //
  // Perform database lookup and projection of the gradient
  // on the requested coordinates.
  //

  // Lookup
  if((angleIndex  > data->maxAngle ) ||
     (dSmallIndex > data->maxDSmall) ||
     (dBigIndex   > data->maxDBig  )   )
    return 0.0;
  const Entry match = data->waterTable[angleIndex][dSmallIndex][dBigIndex];

  const Epot ePot = match.ePot;

  const VectorProduct axbNormalMatch = {.X = (switchHaHb) ? (-1) * match.axbNormal.X : match.axbNormal.X,
                                        .Y = (switchHaHb) ? (-1) * match.axbNormal.Y : match.axbNormal.Y,
                                        .Z = (switchHaHb) ? (-1) * match.axbNormal.Z : match.axbNormal.Z };

  const Vector3 gradO = match.grad.O;

  const Vector3 gradHa = (switchHaHb)  ?
                         match.grad.Hb :
                         match.grad.Ha  ;

  const Vector3 gradHb = (switchHaHb)  ?
                         match.grad.Ha :
                         match.grad.Hb  ;

  const Scalar gradOLength = sqrt((gradO.X * gradO.X) +
                                  (gradO.Y * gradO.Y) +
                                  (gradO.Z * gradO.Z)  );
  assert((gradOLength != 0.0) && "O gradient length is non-zero");

  const Vector3 gradONormal = {.X = gradO.X / gradOLength,
                               .Y = gradO.Y / gradOLength,
                               .Z = gradO.Z / gradOLength };

  const Scalar gradHaLength = sqrt((gradHa.X * gradHa.X) +
                                   (gradHa.Y * gradHa.Y) +
                                   (gradHa.Z * gradHa.Z)  );
  assert((gradHaLength != 0.0) && "Ha gradient length is non-zero");

  const Vector3 gradHaNormal = {.X = gradHa.X / gradHaLength,
                                .Y = gradHa.Y / gradHaLength,
                                .Z = gradHa.Z / gradHaLength };

  const Scalar gradHbLength = sqrt((gradHb.X * gradHb.X) +
                                   (gradHb.Y * gradHb.Y) +
                                   (gradHb.Z * gradHb.Z)  );
  assert((gradHbLength != 0.0) && "Hb gradient length is non-zero");

  const Vector3 gradHbNormal = {.X = gradHb.X / gradHbLength,
                                .Y = gradHb.Y / gradHbLength,
                                .Z = gradHb.Z / gradHbLength };

  // Projection
  const Scalar cosTheta = (axbNormal.X * axbNormalMatch.X) +
                          (axbNormal.Y * axbNormalMatch.Y) +
                          (axbNormal.Z * axbNormalMatch.Z)  ;
  assert((cosTheta <= 1.0) && "cos(theta) is valid");
  const Scalar sinTheta = sqrt(1 - cosTheta);

  // Rotation matrix between axbNormal and axbNormalMatch
  const VectorProduct rotationAxis = {.X = (axbNormal.Y * axbNormalMatch.Z) - (axbNormal.Z * axbNormalMatch.Y),
                                      .Y = (axbNormal.Z * axbNormalMatch.X) - (axbNormal.X * axbNormalMatch.Z),
                                      .Z = (axbNormal.X * axbNormalMatch.Y) - (axbNormal.Y * axbNormalMatch.X) };

  const RotationMatrix rotationMatrix = {.AA = cosTheta                                             +
                                               ((rotationAxis.X * rotationAxis.X) * (1 - cosTheta))  ,
                                         .AB = ((rotationAxis.X * rotationAxis.Y) * (1 - cosTheta)) -
                                               (rotationAxis.Z * sinTheta)                           ,
                                         .AC = ((rotationAxis.X * rotationAxis.Z) * (1 - cosTheta)) +
                                               (rotationAxis.Y * sinTheta)                           ,
                                         .BA = ((rotationAxis.Y * rotationAxis.X) * (1 - cosTheta)) +
                                               (rotationAxis.Z * sinTheta)                           ,
                                         .BB = cosTheta                                             +
                                               ((rotationAxis.Y * rotationAxis.Y) * (1 - cosTheta))  ,
                                         .BC = ((rotationAxis.Y * rotationAxis.Z) * (1 - cosTheta)) -
                                               (rotationAxis.X * sinTheta)                           ,
                                         .CA = ((rotationAxis.Z * rotationAxis.X) * (1 - cosTheta)) -
                                               (rotationAxis.Y * sinTheta)                           ,
                                         .CB = ((rotationAxis.Z * rotationAxis.Y) * (1 - cosTheta)) +
                                               (rotationAxis.X * sinTheta)                           ,
                                         .CC = cosTheta                                             +
                                               ((rotationAxis.Z * rotationAxis.Z) * (1 - cosTheta))   };

  // Writeout  
  query->O.X  = ((gradONormal.X  * rotationMatrix.AA) +
                 (gradONormal.X  * rotationMatrix.AB) +
                 (gradONormal.X  * rotationMatrix.AC)  ) * gradOLength ;
  query->O.Y  = ((gradONormal.Y  * rotationMatrix.BA) +
                 (gradONormal.Y  * rotationMatrix.BB) +
                 (gradONormal.Y  * rotationMatrix.BC)  ) * gradOLength ;
  query->O.Z  = ((gradONormal.Z  * rotationMatrix.CA) +
                 (gradONormal.Z  * rotationMatrix.CB) +
                 (gradONormal.Z  * rotationMatrix.CC)  ) * gradOLength ;
  query->Ha.X = ((gradHaNormal.X * rotationMatrix.AA) +
                 (gradHaNormal.X * rotationMatrix.AB) +
                 (gradHaNormal.X * rotationMatrix.AC)  ) * gradHaLength;
  query->Ha.Y = ((gradHaNormal.Y * rotationMatrix.BA) +
                 (gradHaNormal.Y * rotationMatrix.BB) +
                 (gradHaNormal.Y * rotationMatrix.BC)  ) * gradHaLength;
  query->Ha.Z = ((gradHaNormal.Z * rotationMatrix.CA) +
                 (gradHaNormal.Z * rotationMatrix.CB) +
                 (gradHaNormal.Z * rotationMatrix.CC)  ) * gradHaLength;
  query->Hb.X = ((gradHbNormal.X * rotationMatrix.AA) +
                 (gradHbNormal.X * rotationMatrix.AB) +
                 (gradHbNormal.X * rotationMatrix.AC)  ) * gradHbLength;
  query->Hb.Y = ((gradHbNormal.Y * rotationMatrix.BA) +
                 (gradHbNormal.Y * rotationMatrix.BB) +
                 (gradHbNormal.Y * rotationMatrix.BC)  ) * gradHbLength;
  query->Hb.Z = ((gradHbNormal.Z * rotationMatrix.CA) +
                 (gradHbNormal.Z * rotationMatrix.CB) +
                 (gradHbNormal.Z * rotationMatrix.CC)  ) * gradHbLength;


  //
  // Return EPot.
  //

  return ePot;


}

