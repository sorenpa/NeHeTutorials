#include <stdio.h>
#include "Object3D.h"







void readstr(FILE *f,char *string)                          // Reads A String From File (f)
{
    do                                      // Do This
    {
        fgets(string, 255, f);                          // Gets A String Of 255 Chars Max From f (File)
    } while ((string[0] == '/') || (string[0] == '\n'));                // Read Again If Line Has Comment Or Is Blank
    return;                                     // Return
}

int objload(char *name,Object3D *k)                          // Loads Object From File (name)
{
    int ver;                                    // Will Hold Vertice Count
    float   rx,ry,rz;                               // Hold Vertex X, Y & Z Position
    FILE    *filein;                                // Filename To Open
    char    oneline[255];                               // Holds One Line Of Text (255 Chars Max)
 
    
    if(fopen_s(&filein,name, "rt")==0)                         // Opens The File For Reading Text In Translated Mode
    {
                                            // CTRL Z Symbolizes End Of File In Translated Mode
        readstr(filein,oneline);                            // Jumps To Code That Reads One Line Of Text From The File
        sscanf_s(oneline, "Vertices: %d\n", &ver);                    // Scans Text For "Vertices: ".  Number After Is Stored In ver

        k->InitObject(ver);                                    
    
        for (int i=0;i<ver;i++)                              // Loops Through The Vertices
        {
            readstr(filein,oneline);                        // Reads In The Next Line Of Text
            sscanf_s(oneline, "%f %f %f", &rx, &ry, &rz);             // Searches For 3 Floating Point Numbers, Store In rx,ry & rz
        
            k->GetVertices()[i].m_position[0] = rx;                         // Sets Objects (k) points.x Value To rx
            k->GetVertices()[i].m_position[1] = ry;                         // Sets Objects (k) points.y Value To ry
            k->GetVertices()[i].m_position[2] = rz;                  // Sets Objects (k) points.z Value To rz
        }
        fclose(filein);                                 // Close The File
 
        return ver;
    }

    return 0;
}  