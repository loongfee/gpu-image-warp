#ifndef FILTERWARP_H
#define FILTERWARP_H


/* Warps an image with given rotation */
void Filter_Warp_GPU(int *argc, char** argv,char *fname1, char *outFile, float xRot, float yRot, float zRot);

#endif