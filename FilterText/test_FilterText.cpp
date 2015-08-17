#include "stdafx.h"

#include "FilterText.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void PrintElement( string & text )
{
    cout << text << endl;
}

int main(int argc, char * argv[])
{
    CFilterText filter_text;

    filter_text.RegisterMatchAndEmit( "sky", "emit_1" );
    filter_text.RegisterMatchAndEmit( "your car", "emit_2" );
    filter_text.RegisterMatchAndEmit( "hat", "emit_3" );
    filter_text.RegisterMatchAndEmit( "my car", "emit_4" );

    vector< string > emitted;

    cout << "Process: \"The sky is blue, the ocean is blue, and so is your car.\"" << endl;
    filter_text.ProcessInputText( string( "The sky is blue, the ocean is blue, and so is your car." ), emitted );
    for_each( emitted.begin(), emitted.end(), &PrintElement );

    cout << "--------------------" << endl;
    cout << "Process: \"Who stole my hat\"" << endl;
    filter_text.ProcessInputText( string( "Who stole my hat?" ), emitted );
    for_each( emitted.begin(), emitted.end(), &PrintElement );

    cout << "--------------------" << endl;
    cout << "All Emitted: " << endl;
    filter_text.GetAllEmitted( emitted );
    for_each( emitted.begin(), emitted.end(), &PrintElement );

    cout << "--------------------" << endl;
    cout << "Clear All Emitted. " << endl;
    filter_text.ClearAllEmitted();
    filter_text.GetAllEmitted( emitted );
    for_each( emitted.begin(), emitted.end(), &PrintElement );

    return 0;
}

