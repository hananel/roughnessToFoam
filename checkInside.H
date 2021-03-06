//************************* checkInside.H **********************
// was "Check_inside.H"

scalar sface = startFace;

forAll(patches, patchi)
{
    const fvPatch& curPatch = patches[patchi];
    if (isType<wallFvPatch>(curPatch))
    {
        forAll(curPatch, facei)
        {
            Vector <scalar> centre_pointi = mesh.faces()[sface+facei].centre(mesh.points());
            // the z0 data is provided in 2D so we don't consider the third dimension - assuming the mesh is oriented in x-y as the .map file
            centre_pointi[2] = 0;
            // debug
            //printf("\n")
            //for (int q = 0; q < centre_pointi.size(); ++q) {
            //    cout << centre_pointi[q] << ", ";
            //}
            //cout << "\n";
            bool inside = checkInside(centre_pointi, p_polygon); //was "... Check_inside ..."
            label inletPatchId1 = mesh.boundaryMesh().findPatchID(curPatch.name());
            if (inside)
            {
                //label inletPatchId1 = mesh.boundaryMesh().findPatchID(curPatch.name()); // Hanan Einav Levy 27/09/12 : I remarked this out - and moved before the if statment
                // debug
                //printf("inside z0.boundaryField()[inletPatchId1 %d][facei %d] = z0InOut[0] %f;\n", inletPatchId1, facei, z0InOut[0]);
                z0.boundaryField()[inletPatchId1][facei] = z0InOut[0]; // inserting the value inside the polygon. -- was "... = CsKs[0]" and then the same for Ks.boundaryField .. with out the else statement I added below
            }
            else // outside - should only happen in the first polygon, otherwise after that all the other polygons wiil be erased
            if (firstPolygon)
            {
                // debug
                //printf("outside z0.boundaryField()[inletPatchId1 %d][facei %d] = z0InOut[1] %f;\n", inletPatchId1, facei, z0InOut[1]);
                z0.boundaryField()[inletPatchId1][facei] = z0InOut[1]; // inserting the value outside the polygon
            }
        }
    }
    sface = sface + patches[patchi].size();
    // debug
    // printf("sface = %d",sface);
}
