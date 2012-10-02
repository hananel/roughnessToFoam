//************************** readMapFile.H **********************
// was "Readmapfile.H"

fileName tmp;
tmp = args.path();
Info<< "Type the name of the WAsP file..." <<endl;
char file[100];
tmp = tmp + "/";

cin.getline(file, 100);
tmp = tmp + file;

cout << "using filename " << tmp << endl;

char nam[300];

int p = 0;
for (int i = 0; i < 100; i++){
    if (file[i] == '.'){
        p = i;
        i = 100;
    }
}

int ii = tmp.size();
for (int i = 0; i < ii; i++){
    nam[i] = tmp[i];
    if (tmp[i] == file[p] && tmp[i+1] == file[p+1] && tmp[i+2] == file[p+2] && tmp[i+3] == file[p+3]){
        nam[i+1] = tmp[i+1];
        nam[i+2] = tmp[i+2];
        nam[i+3] = tmp[i+3];
        nam[i+4] = '\0';
        i = ii+4;
    }
}

ii = 0;
ifstream inFile;
inFile.open(nam);

if (!inFile){
    Info << "Unable to open file " <<file<<endl;
    return (0);
}

char c;
int a = 0;
char d[512];
fileName cadena;

while (!inFile.eof()) {
    printf("****  inside inFile, a = %d, %d\n",a, inFile.tellg());
    if (a < 4) { //We skip the first 4 lines of the map file
        inFile.getline(d, sizeof(d) - 1, '\n');
        printf("****  d = %s\n",d);
    } else {
        inFile.get(c);
        cadena += c;
        cout << "cadena: " << cadena << endl;
    }
    a++;
}
Info<< "DDDDDD File(" << inFile << ") '" << nam << "' has been read" <<endl;
Info<<endl;

Info<< "Now we need to relate the WAsP point files coordinate to our mesh coordinate system. For that purpose we will introduce some data" <<endl;

scalar offset_x = 0;
scalar offset_y = 0;
scalar a1 = 1;
scalar b1 = 0;
scalar a2 = 0;
scalar b2 = 1;

// TODO change so that an input here is if i even want to a a relation between the map file and the wasp file coordinate system - normally there won't be a difference

Info<<endl<< "* Introduce the offset between both coordinate system origins (x0, y0) mesh = offset + x0, y0)wasp : (the offset must be in mesh coordinates) " <<endl;
Info<< "x_value: ";
cin>>offset_x;
Info<< "y_value: ";
cin>>offset_y;

Info<<endl<< "Now introduce the values (x, y) for the point (1, 0) wasp in mesh coordinates" <<endl;
Info<< "x_value: ";
cin>>a1;
Info<< "y_value: ";
cin>>b1;
Info<<endl<< "Finally introduce the values (x, y) for the point (0,1) wasp in mesh coordinates" <<endl;
Info<< "x_value: ";
cin>>a2;
Info<< "y_value: ";
cin>>b2;
Info<<endl;
cout << "using x0 = " << offset_x << ", y0 = " << offset_y << endl;
cout << "using a1 = " << a1 << ", a2 = " << a2 << endl;
cout << "using b1 = " << b1 << ", b2 = " << b2 << endl;

int cad_size = cadena.size();
int n_polygons = 0;
long i = 0;
Vector <scalar> z0InOut; // was "CsCk"

// my addition - Hanan Einav Levy 27/09/12
bool firstPolygon = true;
// end - my addition - Hanan Einav Levy 27/09/12

printf("\ncad_size = %d\n", cad_size);
while (i < cad_size - 3) {
    printf("In while loop - i = %ld\n", i);
    // getting z0In, z0Out and number of vertex of each polygon
    // was "getting Cs,Ks ..."
    int ni = 0;
    char z0In[15]; //was "cs[15]"
    while (cadena[i] != ' '){ //getting z0In - was "getting Cs"
        z0In[ni] = cadena[i]; //was cs[ni] = ..."
        ni++;
        i++;
    }
    scalar z0In_s = atof(z0In); //was "cs_s ..."
    print_string(z0In, sizeof(z0In));
    printf("z0In = %s\n",z0In);
    printf("z0In_s = %f\n",z0In_s);
    i++;

    ni = 0;
    char z0Out[15]; //was "ks[15]"
    while (cadena[i] != ' ') { //getting z0Out - was "getting Ks"
        z0Out[ni] = cadena[i]; //was "Ks[ni] = ..."
        ni++;
        i++;
    }
    scalar z0Out_s = atof(z0Out); // was "ks_s ..."
    printf("z0Out = %s\n",z0Out);
    printf("z0Out_s = %f\n",z0Out_s);
    i++;

    ni = 0;
    char nvertex[15];
    while (cadena[i] != '\n'){ //getting number of vertex
        nvertex[ni] = cadena[i];
        ni++;
        i++;
    }
    scalar nvertex_s = atoi(nvertex);
    List <vector> p_polygon;
    printf("nvertex = %s\n",nvertex);
    printf("nvertex_s = %f\n",nvertex_s);

    p_polygon.setSize(nvertex_s);
    z0InOut[0] = z0In_s;
    z0InOut[1] = z0Out_s;
    z0InOut[2] = nvertex_s;
    printf("polygon %ld has z0in = %f z0Out %f number of points %f\n",i,z0InOut[0],z0InOut[1],z0InOut[2]);

    i++; //We are in the position of the first
         //char of the first vertex of the polygon and we are getting
         // the polygon point data

    if (cadena[i] == '/') i = cad_size; //TODO what's the point??
    for (int j = 0; j < nvertex_s; j++){
        ni = 0;
        char x1[15];
        while (cadena[i] != ' '){
            x1[ni] = cadena[i];
            ni++;
            i++;
        }
        i++;
        x1[ni] = '\0' ;
        
        ni = 0;
        char y1[15];
        while (cadena[i] != '\n'){
            y1[ni] = cadena[i];
            ni++;
            i++;
        }
        y1[ni] = '\0';

        scalar x1_s = offset_x + (a1 - offset_x) * atof(x1) + (a2 - offset_x) * atof(y1);
        scalar y1_s = offset_y + (b1 - offset_y) * atof(x1) + (b2 - offset_y) * atof(y1);

        p_polygon[j][0] = x1_s;
        p_polygon[j][1] = y1_s;
        p_polygon[j][2] = 0; // We don't use the z coordinate.
        i++;

        if (cadena[i] == '/') i = cad_size;
        printf("polygon point %d is (%f,%f,%f)\n",j,p_polygon[j][0],p_polygon[j][1],p_polygon[j][2]);

    }

    #include "checkInside.H"

    // my addition - Hanan Einav Levy 27/09/12
    firstPolygon = false;
    // end - my adition - Hanan Einav Levy 27/09/12
    
    n_polygons++;
    printf("\nn_polygons = %d\n",n_polygons);
}

inFile.close(); //We close the file
