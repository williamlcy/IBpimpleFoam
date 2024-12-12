    // points coordination list
    std::vector<vector> Lmarks;
    // points normal vector
    std::vector<vector> Lmarks_normal;
    //points tangent vector
    std::vector<vector> Lmarks_tangential;
    
    //IO open file
    std::ifstream file("Data.txt");

    scalar X, Y, Z;
	
	// read data
    while(file>>X>>Y>>Z)
    {
        vector DataTransfer(X, Y, Z);       
        Lmarks.push_back(DataTransfer);
    }

    label npt = Lmarks.size();
    Lmarks_normal.resize(npt);
    Lmarks_tangential.resize(npt);
    // min and max x-coordinate
    scalar minx=1E300;
    scalar maxx=-1E300;
    
    // cal the normal and tangent vector
    for(label I=0; I<npt; I++)
    {
        //printf("%10.5lf %10.5lf %10.5lf\n", Lmarks[I].x(), Lmarks[I].y(), Lmarks[I].z());
        //magnitude of vector
        scalar r=mag(Lmarks[I]);

        //cal the normal and tangent vector
        Lmarks_normal[I].x()=Lmarks[I].x()/r;
        Lmarks_normal[I].y()=Lmarks[I].y()/r;
        Lmarks_normal[I].z()=0.0;
        Lmarks_tangential[I].x()=Lmarks[I].y()/r;
        Lmarks_tangential[I].y()=-Lmarks[I].x()/r;
        Lmarks_tangential[I].z()=0.0;
        
        minx=std::min(minx,Lmarks[I].x());
        maxx=std::max(maxx,Lmarks[I].x());
    }
    
    file.close();
    const scalar Lmarks_Ds=mag(Lmarks[1]-Lmarks[0]);
    //const scalar Lmarks_Lref=maxx-minx;
