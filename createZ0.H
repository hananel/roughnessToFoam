//******************* createZ0.H *****************
// was "createCsKs.H"

Info << "Reading z0 field" << endl;

volScalarField z0
    (
    IOobject
        (
         "z0",
         runTime.timeName(),
         mesh,
         IOobject::MUST_READ,
         IOobject::AUTO_WRITE
         ),
         mesh
);
