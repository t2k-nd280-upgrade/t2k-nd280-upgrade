#define THIS_NAME ana_example
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include <TError.h>
#include <time.h> 

void ana_example() {

    cout << endl << "Staring the execution." << endl;
    cout << "The goal of this macro is to visualize selected events." << endl;

    bool jobMode = kFALSE;
    float time = clock();

    /// --------------------------

    ND280SFGDEvent* exampleEvent = new ND280SFGDEvent();

    ND280SFGDVoxel* v1 = new ND280SFGDVoxel(2,4,1);
    ND280SFGDVoxel* v2 = new ND280SFGDVoxel(2,4,2);
    ND280SFGDVoxel* v3 = new ND280SFGDVoxel(2,4,3);
    ND280SFGDVoxel* v4 = new ND280SFGDVoxel(2,4,4);
    ND280SFGDVoxel* v5 = new ND280SFGDVoxel(2,4,5);
    ND280SFGDVoxel* v6 = new ND280SFGDVoxel(2,4,6);
    ND280SFGDVoxel* v7 = new ND280SFGDVoxel(2,4,7);

    vector <ND280SFGDVoxel*> listOfVoxel;

    listOfVoxel.push_back(v1);
    listOfVoxel.push_back(v2);
    listOfVoxel.push_back(v3);
    listOfVoxel.push_back(v4);
    listOfVoxel.push_back(v5);
    listOfVoxel.push_back(v6);
    listOfVoxel.push_back(v7);

    vector <ND280SFGDTrack*> listOfTracks;

    ND280SFGDTrack* exampleTrack = new ND280SFGDTrack(listOfVoxel);

    listOfTracks.push_back(exampleTrack);

    exampleEvent = new ND280SFGDEvent(listOfTracks);

    cout << "# of voxels: " << exampleEvent->GetVoxels().size() << endl;

    cout << "alive" << endl;
    exampleEvent->DrawVoxels();

    /// --------------------------

    time = ( clock() - time ) / CLOCKS_PER_SEC;
    cout << std::setprecision(8) << "Elapsed time: " << time << endl;

    if(!jobMode){
        cout << "This macro remains open until it is manually closed with Ctrl+C\nallowing to watch the output on the output Canvas.\nTo avoid this, use option '-j'." << endl;
        return;
    }
    else{
        cout << "Execution completed successfully." << endl << endl;
        exit(0);
    }
}
