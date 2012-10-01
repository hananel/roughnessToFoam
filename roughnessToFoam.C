/*-------------------------------------------------------------------------*\
=========                  |
\\      /   F ield         |
 \\    /    O peration     |
  \\  /     A nd           |
   \\/      M anipulation  |
-------------------------------------------------------------------------

Application:
Roughness writer

Description:
Roughness writer from WAsP file (*.map) to z0 (original was to Cs and Ks) boundary conditions

Author:
copied from Xabier Pedruelo Tapia 2009 MsC thesis
\*----------------------------------------------------------------------*/

# include "wallFvPatch.H"
# include "fvCFD.H"
# include <fstream>
# include <stdlib.h>
using namespace std;

// ************************************************************** //

bool checkInside (vector, List<vector>);

int main( int argc, char *argv[])
{
    printf("%d\n", __LINE__);
        
# include "setRootCase.H"
# include "createTime.H"
# include "createMesh.H"
# include "createZ0.H" // was "createCsKs.H"

    printf("%d\n", __LINE__);

    const fvPatchList& patches = mesh.boundary();
    scalar startFace = mesh.neighbour().size();

    printf("%d\n", __LINE__);
# include "readMapFile.H" //was Readmapfile.H

    printf("%d\n", __LINE__);
    Info<< " Writing z0 " <<endl; //writing Z0 boundary file - was "writing Cs and Ks boundary file" - TODO - is it endl or end1? went with the latter
    z0.write(); //was two lines - "Ks.write(); / Cs.write();
    printf("%d\n", __LINE__);

    return (0);
}

bool checkInside(vector point, List<vector> polygon){
int i,j = polygon.size() - 2;
bool oddNodes = false;
    for (i = 0; i < polygon.size() - 1; i++){
        if ((polygon[i][1] < point[1] && polygon[j][1] >= point[1]) ||
            (polygon[j][1] < point[1] && polygon[i][1] >= point[1]) ){
            if ( polygon[i][0] + (point[1] - polygon[i][1]) / ( polygon[j][1] - polygon[i][1] ) * (polygon[j][0] - polygon[i][0]) < point[0] ){
                oddNodes =! oddNodes;
            }
        }
        j = i;
    }
    return oddNodes; // if the point is inside the polygon returns true
}
